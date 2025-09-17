#include "deck.h"

int Deck::decks_number = 0;

const JewelryCard &Deck::getCard() {
	if (this->deck.size() == 0) {
		throw SplendorException("Attention pas de cards dans la deck !");
	}
	const JewelryCard *return_card = deck[0];
	deck.erase(deck.begin());
	return *return_card;
}

void Deck::initDecks(Deck *deck1, Deck *deck2, Deck *deck3,
                         vector<const JewelryCard *> &cards) {
	if (decks_number != max_decks) {
		throw SplendorException("les pioches ne sont pas toutes créées");
	}

	for (const JewelryCard *card :
	     cards) { // pour chaque card (ptr constant
		           // vers une card) du vecteur cards
		switch (card->getLevel()) {
		case 1:
			(deck1->deck).push_back(card);
			break;
		case 2:
			(deck2->deck).push_back(card);
			break;
		case 3:
			(deck3->deck).push_back(card);
			break;
		}
	}
	// Melange les éléments des pioches
	random_device rd;
	mt19937 g(rd());

	shuffle((deck1->deck).begin(), (deck1->deck).end(), g);
	shuffle((deck2->deck).begin(), (deck2->deck).end(), g);
	shuffle((deck3->deck).begin(), (deck3->deck).end(), g);
}
