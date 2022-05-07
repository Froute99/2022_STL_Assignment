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

	std::ifstream in{ "2022 STL ���� ����", std::ios::binary };

	if (!in.is_open()) {
		std::cout << "File Doesn't Exist" << std::endl;
		return -1;
	}


	std::array<Player, objectNumber>* players = new std::array<Player, objectNumber>();

	std::cout << "Player ���� �д���\n";

	// ���� �б�
	Player player;
	int i = 0;
	while (player.Read(in)) {
		players->at(i++) = player;
	}
	system("cls");		// player ���� �д��� �޽��� ����� ����


	// 1. ������ ��ü ���
	std::cout << "������ ���� ���� -";
	players->back().Show();


	// 2. ���� ��հ�
	long long int total = 0;
	total = std::accumulate(players->begin(), players->end(), total,
		[](long long int sum, const Player& p) {
			return sum + p.GetScore();
		});
	std::cout << "������� : " << std::fixed << double(total) / objectNumber << std::endl;


	// 3. ����Ʈ �� 999
	std::ofstream out{ "����Ʈ �� 999", std::ios::binary };
	long long int count = std::count_if(players->begin(), players->end(),
		[&out](const Player& p) {
			if (p.GetNum() == 999) {
				p.Write(out);
				return true;
			}
			return false;
		});
	std::cout << "�ڿ��� 999�� �÷��̾� ���� : " << count << std::endl;
	out.close();		// ���α׷� �������� ���� �ۼ��� ������ ����!!!

	
	bool sortedById = false;
	// 4. id �Է�
	int idToFind;
	do {
		std::cout << "\nã�� ���� Id�� �Է��ϼ���: ";
		std::cin >> idToFind;

		// �߸��� �Է��϶� ex) �ִ밪�� �Ѵ� ����, ���� ��
		if (std::cin.fail()) {
			std::cout << "\t�Է��� �߸� �Ǿ����ϴ�!" << std::endl;
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
		
		// �������� �ʴ� id
		if (!std::binary_search(players->begin(), players->end(), idToFind, Comp())) {
			std::cout <<
				"\t���̵� " << idToFind << " �� �÷��̾�� �������� �ʽ��ϴ�\n";
			continue;
		}

		auto pair = std::equal_range(players->begin(), players->end(), idToFind, Comp());

		// �յڷ� ��� + �׵θ� üũ
		std::cout << "\n���̵� ��������\n\n";
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

		std::cout << "\n�̸� ��������\n\n";
		std::sort(players->begin(), players->end(),
			[](const Player& p1, const Player& p2) {
				return p1.GetName() < p2.GetName();
			});
		auto found = std::find(players->begin(), players->end(), idToFind);
		FoundPlayerPrintHelper(players->begin(), players->end(), found);


		std::cout << "\n���� ��������\n\n";
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
	std::cout << std::format("  �̸�: {:<17} ", name)
		<< std::format("���̵�: {:<7} ", id)
		<< std::format("����: {:<10} ", score)
		<< std::format("�ڿ���: {:<4}", num)
		<< std::endl;
}

void FoundPlayerPrintHelper(const std::array<Player, objectNumber>::const_iterator b, const std::array<Player, objectNumber>::const_iterator e, const std::array<Player, objectNumber>::iterator& found) {
	// ����
	if (found > b)
		(found - 1)->Show();
	else
		//std::cout << "  ���̵� "
		//<< found->GetId() << "�� ��ü�� ù��° ��ü�Դϴ�\n";
		std::cout << "===============================================================\n";

	// ����
	found->Show();

	// ����
	if (found < e)
		(found + 1)->Show();
	else
		//std::cout << "  ���̵� "
		//<< found->GetId() << "�� ��ü�� ������ ��ü�Դϴ�\n";
		std::cout << "===============================================================\n";

	std::cout << std::endl;
}
