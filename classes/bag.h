#ifndef LO21_SPLENDOR_DUEL_SAC_H
#define LO21_SPLENDOR_DUEL_SAC_H
#include "tokens.h"
#include <iostream>
#include <vector>

using namespace std;

class Bag {
	int tokens_in_bag_number;
	vector<const Token *> tokens;
	struct BagHandler {
		Bag *instance = nullptr;
		~BagHandler() {
			delete instance;
			instance = nullptr;
		}
	};
	static BagHandler bag_handler;
	Bag() = default;
	~Bag() = default; // car agrégation !

	Bag &operator=(const Bag &s) = delete;
	Bag(const Bag &s) = delete;

	void initBag();

  public:
	json toJson() const {
		json j;
		// This nb needs to change 
		j["nb"] = getTokenNumber();
		j["tokens"] = {};
		for (int i = 0; i < tokens.size(); ++i) {
			j["tokens"].push_back(tokens[i]->toJson());
		}

		return j;
	}

	void printBag();
	static Bag &get();
	static void free();

	const int getTokenNumber() const { return tokens_in_bag_number; }

	void setAmountofToken(int nbr) {
		if (tokens_in_bag_number < 0) {
			throw SplendorException(
			    "Il n'y a déjà plus de tokens dans le bag!");
		}
		tokens_in_bag_number = nbr;
	}
	const Token *getTokenByIndex(int i) const { return tokens[i]; }
	void placeTokenInBagByIndex(int i, Token *jet) { tokens[i] = jet; }
	void insertToken(const Token *jet);

	void takeTokenByIndex(int i) {
		if ((i < 0) || (i >= Bag::getTokenNumber())) {
			throw SplendorException(
			    "L'index du jeton ne peut pas être négatif, ou "
			    "supérieur au nombre total de tokens autorisés");
		}
		tokens.erase(tokens.begin() + i);
		--tokens_in_bag_number;
	}
};

#endif // LO21_SPLENDOR_DUEL_SAC_H
