#include "board.h"
#include <iostream>
#include <optional>

using namespace std;

optional<Position> invertPositions(const optional<Position> &pos) {
	if (pos == Position::right) {
		return Position::left;
	}
	if (pos == Position::diag_upper_right) {
		return Position::diag_lower_left;
	}
	if (pos == Position::above) {
		return Position::below;
	}
	if (pos == Position::diag_upper_left) {
		return Position::diag_lower_right;
	}
	if (pos == Position::left) {
		return Position::right;
	}
	if (pos == Position::diag_lower_left) {
		return Position::diag_upper_right;
	}
	if (pos == Position::below) {
		return Position::above;
	}
	if (pos == Position::diag_lower_right) {
		return Position::diag_upper_left;
	} else
		return nullopt;
}

std::string toString(optional<Position> &pos) {
	if (pos == Position::right) {
		return "right";
	}
	if (pos == Position::diag_upper_right) {
		return "diag_upper_right";
	}
	if (pos == Position::above) {
		return "above";
	}
	if (pos == Position::diag_upper_left) {
		return "diag_upper_left";
	}
	if (pos == Position::left) {
		return "left";
	}
	if (pos == Position::diag_lower_left) {
		return "diag_lower_left";
	}
	if (pos == Position::below) {
		return "below";
	}
	if (pos == Position::diag_lower_right) {
		return "diag_lower_right";
	}
	if (pos == nullopt) {
		return "not a side!";
	}
}

Board::BoardHandler Board::board_handler;
Board &Board::getBoard() {
	if (board_handler.instance == nullptr) {
		board_handler.instance = new Board();
	}
	return *board_handler.instance;
}

void Board::free() {
	delete board_handler.instance;
	board_handler.instance = nullptr;
}

void Board::printArray() const {
	for (size_t i = 0; i < Token::getMaxTokenNumber(); i++) {
		if (tokens[i] != nullptr) {
			if ((i + 1) % 5 == 0) {
				cout << "index : " << i << " " << *tokens[i] << "\n";
			} else {
				cout << "index : " << i << " " << *tokens[i] << " | ";
			}
		} else {
			if ((i + 1) % 5 == 0) {
				cout << "index : " << i << " id: null; couleur: null\n";
			} else {
				cout << "index : " << i << " id: null; couleur: null |";
			}
		}
	}
}

void Board::fill(Bag &bag) {
	srand(static_cast<unsigned>(std::time(nullptr)));
	int i = 0;
	while (Bag::get().getTokenNumber() > 0) {
		if (tokens[order[i]] == nullptr) {
			int bag_number = bag.getTokenNumber();
			int rdm = rand() % bag_number;
			setTokenOnBoardByIndex(order[i], bag.getTokenByIndex(rdm));
			bag.takeTokenByIndex(rdm);
			current_nb++;
		}
		i++;
	}
}

extern std::initializer_list<Position> Positions = {
    Position::right,  Position::diag_upper_right,
    Position::above,  Position::diag_upper_left,
    Position::left,  Position::diag_lower_left,
    Position::below, Position::diag_lower_right};

std::map<std::string, enum Position> positionMap = {
    {"right", Position::right},
    {"diag_upper_right", Position::diag_upper_right},
    {"above", Position::above},
    {"diag_upper_left", Position::diag_upper_left},
    {"left", Position::left},
    {"diag_lower_left", Position::diag_lower_left},
    {"below", Position::below},
    {"diag_lower_right", Position::diag_lower_right}};

optional<const Position> Board::tokensAreASide(int i, const Token *jet) {
	if (jet == getElementToTheRight(i))
		return Position::right;
	if (jet == getElementToTheUpperRight(i))
		return Position::diag_upper_right;
	if (jet == getElementAbove(i))
		return Position::above;
	if (jet == getElementToTheUpperLeft(i))
		return Position::diag_upper_left;
	if (jet == getElementToTheLeft(i))
		return Position::left;
	if (jet == getElementToTheBelowLeft(i))
		return Position::diag_lower_left;
	if (jet == getElementBelow(i))
		return Position::below;
	if (jet == getElementToTheBelowRight(i))
		return Position::diag_lower_right;

	return nullopt;
}

bool Board::onlyGold() {
	if (getCurrentNb() == 0)
		throw SplendorException("Board vide!");
	for (auto jet : tokens) {
		if (jet != nullptr and jet->getColor() != Color::gold)
			return false;
	}
	return true;
}

bool Board::colorsOnBoard(const optional<enum colorBonus> &couleur) {
	if (getCurrentNb() == 0)
		throw SplendorException("Board vide!");
	string s = "Bonus ";
	for (auto jet : tokens) {
		if (jet != nullptr and
		    (s + toString(jet->getColor()) == toString(couleur))) {
			return true;
		}
	}
	return false;
}

vector<int>
Board::getTokenIndexesByColor(const optional<enum colorBonus> &couleur) {
	if (getCurrentNb() == 0)
		throw SplendorException("Board vide!");
	vector<int> result;
	string s = "Bonus ";
	for (auto jet : tokens) {
		if (jet != nullptr and
		    (s + toString(jet->getColor()) != toString(couleur))) {
			result.push_back(Board::getBoard().getIndex(jet->getId()));
		}
	}
	return result;
}