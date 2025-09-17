#ifndef LO21_SPLENDOR_DUEL_JOUEUR_H
#define LO21_SPLENDOR_DUEL_JOUEUR_H

#include "Exception.h"
#include "bag.h"
#include "board.h"
#include "draw.h"
#include "privilege.h"
#include <QThread>
#include <iostream>
#include <string>
#include <vector>

// tâches à réaliser : constructeur destructeur Player (game?)
// tester toutes les méthodes ?
// vérifier UML si toutes les méthodes sont implémentées.

using namespace std;

class StrategyPlayer { // Utilisation Design Pattern Strategy
  protected:
	std::string name = "John Doe";
	int is_ia = 0;
	int points_number;
	int crown_number;
	int jewelry_cards_number;
	int reserved_jewelry_cards_number;
	int royal_cards_number;
	int privilege_number;
	int token_number;

	unsigned int games_won;
	unsigned int games;

	static int const max_nb_jetons = 10;
	static int const max_nb_cartes_r = 2;
	static int const max_nb_privileges = 3;
	static int const max_nb_cartes_reservees = 3;

	vector<const JewelryCard *> bought_jewelry_cards;
	vector<const JewelryCard *> reserved_jewelry_cards; // 3 au max
	vector<const RoyalCard *> royal_cards;              // ok pour agrégation?

	vector<const Token *> tokens; // tableau de tokens ? or ?

	vector<const Privilege *> privileges;

	StrategyPlayer &operator=(const StrategyPlayer &) = delete;
	StrategyPlayer(const StrategyPlayer &) = delete;

  public:
	const vector<const RoyalCard *> &getRoyalCards() const {
		return royal_cards;
	}

	void gameEnded(const unsigned int won) {
		if (won != 0) {
			games_won++;
		}
		games++;
	}

	json toJson() const {
		json j;
		j["name"] = name;
		j["is_ia"] = is_ia;
		j["points_number"] = points_number;
		j["crown_number"] = crown_number;
		j["jewelry_cards_number"] = jewelry_cards_number;
		j["reserved_jewelry_cards_number"] = reserved_jewelry_cards_number;
		j["royal_cards_number"] = royal_cards_number;
		j["privilege_number"] = privilege_number;
		j["token_number"] = token_number;
		j["games"] = games;
		j["games_won"] = games_won;
		j["bought_jewelry_cards"] = {};
		j["reserved_jewelry_cards"] = {};
		j["royal_cards"] = {};
		j["tokens"] = {};
		j["privileges"] = {};

		for (int i = 0; i < bought_jewelry_cards.size(); ++i) {
			j["bought_jewelry_cards"].push_back(
			    bought_jewelry_cards[i]->toJson());
		}
		for (int i = 0; i < reserved_jewelry_cards.size(); ++i) {
			j["reserved_jewelry_cards"].push_back(
			    reserved_jewelry_cards[i]->toJson());
		}
		for (int i = 0; i < royal_cards.size(); ++i) {
			j["royal_cards"].push_back(royal_cards[i]->toJson());
		}
		for (int i = 0; i < tokens.size(); ++i) {
			j["tokens"].push_back(tokens[i]->toJson());
		}
		for (int i = 0; i < privileges.size(); ++i) {
			j["privileges"].push_back(privileges[i]->toJson());
		}

		return j;
	}

	json toHistory() const {
		json j;
		j["name"] = name;
		j["is_ia"] = is_ia;
		j["games"] = games;
		j["games_won"] = games_won;
		return j;
	}

	// constructeur destructeur
	StrategyPlayer(const string &name);
	StrategyPlayer(const json data)
	    : points_number(0), jewelry_cards_number(0), royal_cards_number(0),
	      crown_number(0), privilege_number(0), name(data["name"]),
	      token_number(0), reserved_jewelry_cards_number(0),
	      games_won(data["games_won"]), games(data["games"]) {}

	virtual ~StrategyPlayer();

	// méthodes virtuelles pures
	virtual void choice() = 0;
	virtual void choice_Qt() = 0;
	virtual void usePrivilege() = 0;
	virtual void usePrivilege_Qt() = 0;
	virtual void tokenSelection() = 0;
	virtual void cardPurchase() = 0;
	virtual void cardPurchase_Qt() = 0;
	virtual void buyCard(Draw *t, const int index) = 0;
	virtual void buyCard_Qt(Draw *t, const int index) = 0;
	virtual void buyReservedCard(const int index) = 0;
	virtual void buyReservedCard_Qt(const int index) = 0;
	virtual void applyCapacity(const JewelryCard &card,
	                           StrategyPlayer &opponent) = 0;
	virtual void applyCapacity_Qt(const JewelryCard &card,
	                              StrategyPlayer &opponent) = 0;
	virtual void applyRoyalCapacity(const RoyalCard &card,
	                                StrategyPlayer &opponent) = 0;
	virtual void applyRoyalCapacity_Qt(const RoyalCard &card,
	                                   StrategyPlayer &opponent) = 0;
	virtual void cardReservation() = 0;
	virtual void cardReservation_Qt() = 0;
	virtual void royalCardSelection() = 0;
	virtual void royalCardSelection_Qt() = 0;
	virtual void tokenVerification() = 0;
	virtual void tokenVerification_Qt() = 0;

	// getters setters
	const vector<const JewelryCard *> &getCartesBought() const {
		return bought_jewelry_cards;
	}
	const int getJewelryCardNumber() const { return jewelry_cards_number; }
	const int getTokenNumber() const { return token_number; }
	const string getName() const { return name; }
	void setName(string &s) { name = s; }
	const int getBoughtCardNumber() const {
		return bought_jewelry_cards.size();
	}
	const int getReservedCardNumber() const {
		return reserved_jewelry_cards.size();
	}
	const int getNbPoints() const { return points_number; }
	void setPoints(int nb) { points_number = nb; }
	const int getCrownNumber() const { return crown_number; }
	void setCrownNumber(int nb) { crown_number = nb; }
	const int getPrivilegeNumber() const { return privilege_number; }
	void setPrivilegeNumber(int nb) {
		if (privilege_number > max_nb_privileges) {
			throw SplendorException("fichier de chargement corrompu -1");
		}
		privilege_number = nb;
	}
	const int getRoyalCardNumber() const { return royal_cards_number; }
	vector<const JewelryCard *> &getReservedCards() {
		return reserved_jewelry_cards;
	}
	void incrementRoyalCard() { royal_cards_number = royal_cards_number + 1; }

	void setRandomPlayer(unsigned int nb) {
		if (nb == 0) {
			is_ia = 0;
		} else {
			is_ia = 1;
		}
	}

	unsigned int getRandomPlayer() const { return is_ia; }

	void setWins(unsigned int nb) { games_won = nb; }
	void setPlayed(unsigned int nb) { games = nb; }

	const unsigned int getWins() const { return games_won; }
	const unsigned int getPlayed() const { return games; }

	// setters rajoutés pour les besoins du JSON
	void setJewellryCardReserved(vector<const JewelryCard *> j) {
		if (reserved_jewelry_cards_number < j.size()) {
			throw SplendorException("Fichier de sauvegarde corrompu -2");
		}
		reserved_jewelry_cards = j;
	}
	void setJewellryCard(vector<const JewelryCard *> j) {
		if (jewelry_cards_number < j.size()) {
			throw SplendorException("Fichier de sauvegarde corrompu -3");
		}
		bought_jewelry_cards = j;
	}
	void setJetons(vector<const Token *> j) {
		/*if(j.size() > max_nb_jetons){
		    throw SplendorException("Fichier de sauvegarde corrompu -4");
		}*/
		tokens = j;
	}
	void setRoyalCard(vector<const RoyalCard *> r) {
		if (royal_cards_number < r.size()) {
			throw SplendorException("Fichier de sauvegarde corrompu 5");
		}
		royal_cards = r;
	}
	void setPrivileges(vector<const Privilege *> p) {
		if (privilege_number < p.size()) {
			throw SplendorException("Fichier de sauvegarde corrompu 6");
		}
		privileges = p;
	}
	void setNbJetons(unsigned int nb) {
		if (nb > max_nb_jetons) {
			throw SplendorException("fichier de chargement corrompu 7");
		}
		token_number = nb;
	}
	void setNbJCards(unsigned int nb) { jewelry_cards_number = nb; }

	void setNbJCardsReserved(unsigned int nb) {
		if (nb > max_nb_cartes_reservees) {
			throw SplendorException("Fichier de chargement corrompu 8");
		}
		reserved_jewelry_cards_number = nb;
	}

	void setNbRCards(unsigned int nb) {
		if (nb > max_nb_cartes_r) {
			throw SplendorException("Fichier de chargement corrompu 9");
		}
		royal_cards_number = nb;
	}

	vector<const Token *> &getToken() { return tokens; }

	vector<const Privilege *> &get_privilege() { return privileges; }

	// méthode utilitaires aux classes filles

	int calculateBonus(enum colorBonus bonus);
	int TokenAmount(const Color &couleur) const;
	void withdrawTokens(const Color &c, int val);
	void reserveCard(Draw *t, const int index);
	void reserveCard(Deck *p);
	void drawToken(int i);
	void obtainRoyalCard(unsigned int i);
	void obtainRoyalCard_qt(unsigned int i);
	bool royalCardEligibility();
	bool jokerCardEligibility();
	void obtainPrivilege();
	void withdrawPrivilege();
	void fillingBoard();
	bool
	onlyGoldInJetons() { // true si le player n'a que des tokens or, false sinon
		for (auto jet : tokens) {
			if (jet != nullptr and jet->getColor() != Color::gold)
				return false;
		}
		return true;
	}

	// méthode utilitaire pour le main
	bool victoryConditions(); // si le player rempli une des trois conditions de
	                          // victoire, renvoie true
	void printPlayer();
	int getOptionalChoices();
	void withdrawWhiteToken();
	void withdrawPerlToken();
};

class Player : public StrategyPlayer {

  public:
	// Constructeur et destructeur
	Player(const string &name);
	Player(const json data);
	~Player();

	// Méthodes polymorphiques adaptées pour un player
	void choice();
	void choice_Qt();
	void usePrivilege();
	void usePrivilege_Qt();
	void tokenSelection();
	void tokenSelection_Qt();
	void applyCapacity(const JewelryCard &card, StrategyPlayer &opponent);
	void applyCapacity_Qt(const JewelryCard &card, StrategyPlayer &opponent);
	void applyRoyalCapacity(const RoyalCard &card, StrategyPlayer &opponent);
	void applyRoyalCapacity_Qt(const RoyalCard &card, StrategyPlayer &opponent);
	void cardPurchase();
	void cardPurchase_Qt();
	void buyCard(Draw *t, const int index);
	void buyCard_Qt(Draw *t, const int index);
	void buyReservedCard(const int index);
	void buyReservedCard_Qt(const int index);
	void cardReservation();
	void cardReservation_Qt();
	void royalCardSelection();
	void royalCardSelection_Qt();
	void tokenVerification();
	void tokenVerification_Qt();
};

class RandomPlayer : public StrategyPlayer {
  public:
	RandomPlayer(const string &name = "RandomPlayer");
	RandomPlayer(const json data);
	~RandomPlayer();

	// Méthodes polymorphiques adaptées pour une RandomPlayer
	void choice();
	void choice_Qt();
	void usePrivilege();
	void usePrivilege_Qt() { usePrivilege(); }
	void tokenSelection();
	void cardPurchase();
	void cardPurchase_Qt() { cardPurchase(); }
	void buyCard(Draw *t, const int index);
	void buyCard_Qt(Draw *t, const int index) { buyCard(t, index); }
	void buyReservedCard(const int index);
	void buyReservedCard_Qt(const int index) { buyReservedCard(index); }
	void cardReservation();
	void cardReservation_Qt() { cardReservation(); }
	void royalCardSelection();
	void royalCardSelection_Qt() { royalCardSelection(); }
	void applyCapacity(const JewelryCard &card, StrategyPlayer &opponent);
	void applyCapacity_Qt(const JewelryCard &card, StrategyPlayer &opponent) {
		applyCapacity(card, opponent);
	}
	void applyRoyalCapacity(const RoyalCard &card, StrategyPlayer &opponent);
	void applyRoyalCapacity_Qt(const RoyalCard &card,
	                           StrategyPlayer &opponent) {
		applyRoyalCapacity(card, opponent);
	}
	void tokenVerification();
	void tokenVerification_Qt() { tokenVerification(); }
};

/**************** Fonctions utilitaires ****************/
inline std::ostream &operator<<(std::ostream &os, const StrategyPlayer &j) {
	os << "Pseudo : " << j.getName();
	if (j.getNbPoints() != 0) {
		os << "nombre de points : " << j.getNbPoints();
	}
	if (j.getCrownNumber() != 0) {
		os << " nombre de couronnes : " << j.getCrownNumber();
	}
	if (j.getReservedCardNumber() != 0) {
		os << " nombre de cards réservées : " << j.getReservedCardNumber();
	}
	if (j.getPrivilegeNumber() != 0) {
		os << " nombre de privilège : " << j.getPrivilegeNumber();
	}
	if (j.getTokenNumber() != 0) {
		os << " nombre de tokens : " << j.getTokenNumber();
	}
	os << endl;
	return os;
}

int positiveOrNull(int x);

void testPlayers();

#endif // LO21_SPLENDOR_DUEL_JOUEUR_H
