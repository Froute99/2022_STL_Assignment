/*
 * File Made: 2022. 04. 07
 * STL Assignment
 *
 * File name  : main.cpp
 * Author     : JeongHak Kim
 * Student No : 2020182048
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

	bool Read(std::istream& in);
	void Write(std::ostream& out) const;
	void Show() const;

	std::string GetName() const { return name; }
	int GetScore() const { return score; }
	int GetId() const { return id; }
	size_t GetNum() const { return num; }

	bool operator==(const int id) const { return this->id == id; }
	bool operator<(const int id) const { return this->id < id; }
private:
	std::string name;
	int score;
	int id;
	size_t num;
	char* dataPtr;
};

struct Comp {
	bool operator()(const Player& p, const int i) const { return p.GetId() < i; }
	bool operator()(const int i, const Player& p) const { return i < p.GetId(); }
};

namespace {
	constexpr int objectNumber = 2000000;
}

// helper function which print front and back found players' data
void FoundPlayerPrintHelper(const std::array<Player, objectNumber>::const_iterator b, const std::array<Player, objectNumber>::const_iterator e, const std::array<Player, objectNumber>::iterator& found);

int main() {

	std::ifstream in{ "2022 STL 과제 파일", std::ios::binary };

	if (!in.is_open()) {
		std::cout << "File Doesn't Exist" << std::endl;
		return -1;
	}


	std::array<Player, objectNumber>* players = new std::array<Player, objectNumber>();

	std::cout << "Player 정보 읽는중\n";

	// 파일 읽기
	Player player;
	int i = 0;
	while (player.Read(in)) {
		players->at(i++) = player;
	}
	system("cls");		// player 정보 읽는중 메시지 지우기 위함


	// 1. 마지막 객체 출력
	std::cout << "마지막 원소 정보 -";
	players->back().Show();


	// 2. 점수 평균값
	long long int total = 0;
	total = std::accumulate(players->begin(), players->end(), total,
		[](long long int sum, const Player& p) {
			return sum + p.GetScore();
		});
	std::cout << "평균점수 : " << std::fixed << double(total) / objectNumber << std::endl;


	// 3. 바이트 수 999
	std::ofstream out{ "바이트 수 999", std::ios::binary };
	long long int count = std::count_if(players->begin(), players->end(),
		[&out](const Player& p) {
			if (p.GetNum() == 999) {
				p.Write(out);
				return true;
			}
			return false;
		});
	std::cout << "자원수 999인 플레이어 개수 : " << count << std::endl;
	out.close();		// 프로그램 종료전에 파일 작성을 끝내기 위함!!!

	
	bool sortedById = false;
	// 4. id 입력
	int idToFind;
	do {
		std::cout << "\n찾고 싶은 Id를 입력하세요: ";
		std::cin >> idToFind;

		// 잘못된 입력일때 ex) 최대값을 넘는 정수, 문자 등
		if (std::cin.fail()) {
			std::cout << "\t입력이 잘못 되었습니다!" << std::endl;
			std::cin.clear();
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			continue;
		}

		// sort by id
		if (!sortedById) {
			std::sort(players->begin(), players->end(),
				[](const Player& p1, const Player& p2) {
					return p1.GetId() < p2.GetId();
				});
			sortedById = true;
		}
		
		// 존재하지 않는 id
		if (!std::binary_search(players->begin(), players->end(), idToFind, Comp())) {
			std::cout <<
				"\t아이디가 " << idToFind << " 인 플레이어는 존재하지 않습니다\n";
			continue;
		}

		auto pair = std::equal_range(players->begin(), players->end(), idToFind, Comp());

		// 앞뒤로 출력 + 테두리 체크
		std::cout << "\n아이디 오름차순\n\n";
		if (pair.first == players->begin()) {
			std::cout << "=============================================================\n";
		}
		else {
			(pair.first - 1)->Show();
		}
		while (pair.first < pair.second) {
			pair.first->Show();
			++pair.first;
		}
		if (pair.second == players->end()) {
			std::cout << "=============================================================\n";
		}
		else {
			pair.second->Show();
		}

		sortedById = false;

		std::cout << "\n이름 오름차순\n\n";
		std::sort(players->begin(), players->end(),
			[](const Player& p1, const Player& p2) {
				return p1.GetName() < p2.GetName();
			});
		auto found = std::find(players->begin(), players->end(), idToFind);
		FoundPlayerPrintHelper(players->begin(), players->end(), found);


		std::cout << "\n점수 오름차순\n\n";
		std::sort(players->begin(), players->end(),
			[](const Player& p1, const Player& p2) {
				return p1.GetScore() < p2.GetScore();
			});
		found = std::find(players->begin(), players->end(), idToFind);
		FoundPlayerPrintHelper(players->begin(), players->end(), found);
	} while (true);
}


Player::Player(std::string name, int score, int id, size_t num)
	: name(name), score(score), id(id), num(num), dataPtr(nullptr) {}

bool Player::Read(std::istream& in) {
	if (in.read((char*)this, sizeof(Player))) {
		dataPtr = new char[num];
		in.read(dataPtr, num);
		return true;
	}
	return false;
}

void Player::Write(std::ostream& out) const {
	out.write((char*)this, sizeof(Player));
	out.write((char*)dataPtr, num);
}

void Player::Show() const {
	std::cout << std::format("  이름: {:<17} ", name)
		<< std::format("아이디: {:<7} ", id)
		<< std::format("점수: {:<10} ", score)
		<< std::format("자원수: {:<4}", num)
		<< std::endl;
}

void FoundPlayerPrintHelper(const std::array<Player, objectNumber>::const_iterator b, const std::array<Player, objectNumber>::const_iterator e, const std::array<Player, objectNumber>::iterator& found) {
	// 앞쪽
	if (found > b)
		(found - 1)->Show();
	else
		//std::cout << "  아이디가 "
		//<< found->GetId() << "인 객체는 첫번째 객체입니다\n";
		std::cout << "===============================================================\n";

	// 본인
	found->Show();

	// 뒤쪽
	if (found < e)
		(found + 1)->Show();
	else
		//std::cout << "  아이디가 "
		//<< found->GetId() << "인 객체는 마지막 객체입니다\n";
		std::cout << "===============================================================\n";

	std::cout << std::endl;
}
