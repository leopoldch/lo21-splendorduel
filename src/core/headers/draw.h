#ifndef LO21_SPLENDOR_DUEL_TIRAGE_H
#define LO21_SPLENDOR_DUEL_TIRAGE_H
#include "Exception.h"
#include "card.h"
#include "deck.h"
#include <cstring>
#include <iostream>
#include <vector>
using namespace std;

class Draw {

	static const int max_draw_amount = 3; // limite de draw
	static int draw_count;                // compteur
	Deck &deck;
	const int level;
	int cards_number;
	const int max_cards;
	vector<const JewelryCard *> cards;

  public:
	const json toJson() {
		json j;
		j["level"] = getLevel();
		j["cards_number"] = cards_number;
		j["max_cards"] = max_cards;
		j["jewelry_cards"] = {};

		for (int i = 0; i < cards.size(); ++i) {
			j["jewelry_cards"].push_back(cards[i]->toJson());
		}

		return j;
	}

	// constructeur du draw
	Draw(int level, int max_cards, Deck &deck)
	    : level(level), max_cards(max_cards), deck(deck) {
		if (deck.getLevel() != level) {
			throw SplendorException(
			    "La deck n'est pas du même level que le draw !");
		} else if (draw_count == max_draw_amount) {
			throw SplendorException("Nombre maximum de tirages dépassé !");
		}

		cards_number = 0; // on initialise le nombre de cards à 0
		draw_count++;
	}

	~Draw() { draw_count--; }

	// définition des getters
	const int getLevel() const { return level; }
	const int getCardsNumber() const { return cards_number; }
	void setNbCartes(int nb) { cards_number = nb; }

	Deck &getDeck() const { return deck; }
	vector<const JewelryCard *> &getTirage() { return cards; }

	void setTirage(vector<const JewelryCard *> draw) { cards = draw; }

	// déclaration de la méthode qui permet de remplir le Draw avec les cards
	// (voir draw.cpp)
	void fill();

	const JewelryCard &getCard(unsigned int index) {

		if (cards_number == 0) {
			throw SplendorException("Attention pas de cards dans le draw !");
		}

		const JewelryCard *return_card = cards[index];
		cards.erase(cards.begin() + index);
		cards_number--;

		return *return_card;
	}

	const JewelryCard &getCardWithoutDeletion(unsigned int index) {
		if (cards_number == 0) {
			throw SplendorException("Attention pas de cards dans le draw !");
		}
		return *cards[index];
	}

  private:
	Draw &operator=(const Draw &draw) = delete;
	Draw(const Draw &draw) = delete;
};

// surchage de l'opérateur pour print sur la sortie standard le draw
inline std::ostream &operator<<(std::ostream &f, Draw &draw) {
	int i = 0;
	for (const JewelryCard *c : draw.getTirage()) {
		f << "index : " << i << " card : " << *c << endl;
		i++;
	}
	return f;
}

void testTirage();

#endif // LO21_SPLENDOR_DUEL_TIRAGE_H