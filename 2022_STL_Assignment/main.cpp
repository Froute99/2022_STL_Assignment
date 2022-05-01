/*
 * 2022 04 07
 * STL Assignment
 *
 * File name : main.cpp
 * Author    : JH Kim
 */

#include <iostream>
#include <fstream>
#include <vector>
#include <array>
#include <algorithm>
#include <numeric>
#include <format>

class Player {
public:
	Player() = default;
	Player(std::string name, int score, int id, size_t num);

	void Read(std::istream& in);
	void Write(std::ostream& out) const;
	void Show();

	std::string GetName() const { return name; }
	int GetScore() const { return score; }
	int GetId() const { return id; }
	size_t GetNum() const { return num; }

	bool operator==(const int id) const { return this->id == id; }
private:
	std::string name;
	int score;
	int id;
	size_t num;
	char* dataPtr;
};

void ShowWhatYouFound(const std::vector<Player>& v, const std::vector<Player>::iterator& found);

namespace {
	const int objectNumber = 2000000;
}

int main() {

	std::ifstream in{ "2022 STL 과제 파일", std::ios::binary };

	if (!in.is_open()) {
		std::cout << "File Doesn't Exist" << std::endl;
		return -1;
	}

	std::array<Player, objectNumber>* a = new std::array<Player, objectNumber>();

	std::cout << "Player 정보 읽는중\n";

	Player player;
	for (int i = 0; i < objectNumber; ++i) {
		player.Read(in);
		a->at(i) = player;
	}

	//while (in.read((char*)&player, sizeof(Player))) {
	//	char* dataPtr = new char[player.GetNum()];
	//	in.read(dataPtr, player.GetNum());
	//	v.emplace_back(player);

	//	total += player.GetScore();		// 알고리즘 accumulate
	//	
	//	if (player.GetNum() == 999) {
	//		out.write((char*)&player, sizeof(Player));
	//		out.write((char*)dataPtr, player.GetNum());

	//		++count;
	//	}

	//	delete[] dataPtr;		// 메모리 누수 해결을 위해서
	//}
	//system("cls");

	std::ofstream out{ "바이트 수 999", std::ios::binary };
	long long int count = std::count_if(a->begin(), a->end(),
		[&out](const Player& p) {
			p.Write(out);
			return p.GetNum() == 999;
		});

	std::cout << "자원수 999인 플레이어 개수 : " << count << std::endl;
	

	std::cout << "마지막 원소 - ";
	a->back().Show();
	
	//long long inet total = std::accumulate(a->begin(), a->end(), 0,
	//	[](long long int sum, const Player& p) {
	//		return sum + p.GetScore();
	//	});

	long long int total = 0;
	for (const auto& x : *a) {
		total += x.GetScore();
	}

	std::cout << "평균점수: " << total / objectNumber << std::endl;

	//// get an id from input
	//int idToFind;
	//bool isValid;
	//std::cout << "찾고 싶은 Id를 입력하세요: ";
	//do {
	//	std::cin >> idToFind;

	//	if (isValid = std::cin.fail()) {
	//		std::cout << "입력이 잘못 되었습니다!" << std::endl;
	//		std::cin.clear();
	//		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	//		continue;
	//	}

	//	std::vector<Player>::iterator found =
	//		std::find(v.begin(), v.end(), idToFind);
	//	std::cout << std::endl;

	//	if (found == v.end()) {
	//		std::cout <<
	//			"아이디가 " << idToFind
	//			<< " 인 플레이어는 존재하지 않습니다"
	//			<< std::endl;
	//		continue;
	//	}

	//	// sort by id
	//	std::cout << "아이디 오름차순" << std::endl << std::endl;
	//	std::sort(v.begin(), v.end(),
	//		[](const Player& p1, const Player& p2) {
	//			return p1.GetId() < p2.GetId();
	//		});
	//	found = std::find(v.begin(), v.end(), idToFind);
	//	std::vector<Player>::iterator it = found;

	//	// 중복 id 출력
	//	if (v.begin() <= it && it < v.end()) {
	//		while (it->GetId() == idToFind) {
	//			ShowWhatYouFound(v, it++);
	//		}
	//	}

	//	// sort by name
	//	std::cout << "이름 오름차순" << std::endl << std::endl;
	//	std::sort(v.begin(), v.end(),
	//		[](const Player& p1, const Player& p2) {
	//			return p1.GetName() < p2.GetName();
	//		});
	//	found = std::find(v.begin(), v.end(), idToFind);
	//	ShowWhatYouFound(v, found);


	//	// sort by score
	//	std::cout << "점수 오름차순" << std::endl << std::endl;
	//	std::sort(v.begin(), v.end(),
	//		[](const Player& p1, const Player& p2) {
	//			return p1.GetScore() < p2.GetScore();
	//		});
	//	found = std::find(v.begin(), v.end(), idToFind);
	//	ShowWhatYouFound(v, found);
	//} while (true);
}



Player::Player(std::string name, int score, int id, size_t num)
	: name(name), score(score), id(id), num(num), dataPtr(nullptr) {}

void Player::Read(std::istream& in) {
	in.read((char*)this, sizeof(Player));
	dataPtr = new char[num];
	in.read(dataPtr, num);
}

void Player::Write(std::ostream& out) const {
	out.write((char*)this, sizeof(Player));
	out.write((char*)dataPtr, num);
}

void Player::Show() {
	std::cout << std::format("이름: {:<17} ", name)
		<< std::format("아이디: {:<7} ", id)
		<< std::format("점수: {:<10} ", score)
		<< std::format("자원수: {:<4}", num)
		<< std::endl;
}

void ShowWhatYouFound(const std::vector<Player>& v, const std::vector<Player>::iterator& found) {
	if (found > v.begin())
		(found - 1)->Show();
	else
		std::cout << "아이디가 "
		<< found->GetId() << "인 객체는 첫번째 객체입니다"
		<< std::endl;

	found->Show();

	if (found < v.end())
		(found + 1)->Show();
	else
		std::cout << "아이디가 "
		<< found->GetId() << "인 객체는 마지막 객체입니다"
		<< std::endl;

	std::cout << std::endl << std::endl;
}
