//
// Created by Léopold Chappuis on 14/12/2023.
//

#include "json.h"

vector<const JewelryCard *> jewelryCardFromJson(json data) {

	std::vector<const JewelryCard *> jewelry_cards_instances;

	for (const auto &jewelry_card_data : data) {

		if ((jewelry_card_data["capacity"] == 0) &&
		    (jewelry_card_data["color_bonus"] == 0)) {
			JewelryCard *instance = new JewelryCard(
			    jewelry_card_data["prestige_points"],
			    jewelry_card_data["white_cost"], jewelry_card_data["blue_cost"],
			    jewelry_card_data["red_cost"], jewelry_card_data["green_cost"],
			    jewelry_card_data["black_cost"], jewelry_card_data["perl_cost"],
			    jewelry_card_data["level"], jewelry_card_data["crown_number"],
			    jewelry_card_data["bonus_number"], nullopt, nullopt,
			    jewelry_card_data["visual"]);
			jewelry_cards_instances.push_back(instance);
		} else if ((jewelry_card_data["capacity"] != 0) &&
		           (jewelry_card_data["color_bonus"] == 0)) {

			JewelryCard *instance = new JewelryCard(
			    jewelry_card_data["prestige_points"],
			    jewelry_card_data["white_cost"], jewelry_card_data["blue_cost"],
			    jewelry_card_data["red_cost"], jewelry_card_data["green_cost"],
			    jewelry_card_data["black_cost"], jewelry_card_data["perl_cost"],
			    jewelry_card_data["level"], jewelry_card_data["crown_number"],
			    jewelry_card_data["bonus_number"], nullopt,
			    getCapacityFromString(jewelry_card_data["capacity"]),
			    jewelry_card_data["visual"]);
			jewelry_cards_instances.push_back(instance);

		} else if ((jewelry_card_data["capacity"] == 0) &&
		           (jewelry_card_data["color_bonus"] != 0)) {

			JewelryCard *instance = new JewelryCard(
			    jewelry_card_data["prestige_points"],
			    jewelry_card_data["white_cost"], jewelry_card_data["blue_cost"],
			    jewelry_card_data["red_cost"], jewelry_card_data["green_cost"],
			    jewelry_card_data["black_cost"], jewelry_card_data["perl_cost"],
			    jewelry_card_data["level"], jewelry_card_data["crown_number"],
			    jewelry_card_data["bonus_number"],
			    getBonusFromString(jewelry_card_data["color_bonus"]), nullopt,
			    jewelry_card_data["visual"]);
			jewelry_cards_instances.push_back(instance);
		} else {

			JewelryCard *instance = new JewelryCard(
			    jewelry_card_data["prestige_points"],
			    jewelry_card_data["white_cost"], jewelry_card_data["blue_cost"],
			    jewelry_card_data["red_cost"], jewelry_card_data["green_cost"],
			    jewelry_card_data["black_cost"], jewelry_card_data["perl_cost"],
			    jewelry_card_data["level"], jewelry_card_data["crown_number"],
			    jewelry_card_data["bonus_number"],
			    getBonusFromString(jewelry_card_data["color_bonus"]),
			    getCapacityFromString(jewelry_card_data["capacity"]),
			    jewelry_card_data["visual"]);
			jewelry_cards_instances.push_back(instance);
		}
	}
	return jewelry_cards_instances;
}

vector<const RoyalCard *> royalCardsFromJson(json data) {

	std::vector<const RoyalCard *> royal_cards_instances;

	for (const auto &royal_card_data : data) {
		if (royal_card_data["capacity"] != NULL) {
			RoyalCard *instance = new RoyalCard(
			    royal_card_data["prestige_points"],
			    getCapacityFromString(royal_card_data["capacity"]),
			    royal_card_data["visual"]);
			royal_cards_instances.push_back(instance);
		} else {
			RoyalCard *instance =
			    new RoyalCard(royal_card_data["prestige_points"], nullopt,
			                  royal_card_data["visual"]);
			royal_cards_instances.push_back(instance);
		}
	}
	return royal_cards_instances;
}

vector<const Token *> tokensFromJson(json data) {
	std::vector<const Token *> token_array;
	for (const auto &jeton : data) {
		if (jeton != nullptr) {
			Token *tmp =
			    new Token(jeton["id"], jeton["color"], jeton["visual"]);
			token_array.push_back(tmp);
		} else {
			token_array.push_back(nullptr);
		}
	}
	return token_array;
}

vector<const Privilege *> privilegesFromJson(json data, unsigned int nb) {
	if (nb > 3) {
		throw SplendorException(
		    "On ne peut pas avoir plus de 3 privilèges. Le json est corrompu.");
	}
	std::vector<const Privilege *> privilege_array;
	for (int i = 0; i < nb; ++i) {
		privilege_array.push_back(Privilege::getForJson(data[i]["id"]));
	}
	return privilege_array;
}

Deck &deckFromJson(json data) {
	Deck *deck = new Deck(data["level"]);
	deck->setDeck(jewelryCardFromJson(data["jewelry_cards"]));
	return *deck;
}

Draw &drawFromJson(json data, Deck &deck) {
	Draw *draw = new Draw(data["level"], data["max_cards"], deck);
	draw->setNbCartes(data["cards_number"]);
	draw->setTirage(jewelryCardFromJson(data["jewelry_cards"]));
	return *draw;
}

void boardFromJson(json data) {
	Board &deck = Board::getBoard();
	deck.setNb(data["nb"]);
	vector<const Token *> tokens;
	tokens = tokensFromJson(data["tokens"]);

	if (tokens.size() > data["nb"]) {
		throw SplendorException("erreur dans le nombre de tokens sur le board. "
		                        "La partie ne peut pas être chargée.");
	}
	int number_jetons_saved = tokens.size();
	for (int i = number_jetons_saved; i < data["nb"]; ++i) {
		tokens.push_back(nullptr);
	}
	deck.setJetons(tokens);
	deck.setCurrentNb(data["current_nb"]);
}

void bagFromJson(json data) {
	if (data["tokens_in_bag_number"] != 0 && !data["tokens"].is_null()) {
		Bag &bag = Bag::get();
		vector<const Token *> tokens;

		tokens = tokensFromJson(data["tokens"]);

		for (int i = 0; i < tokens.size(); ++i) {
			bag.insertToken(tokens[i]);
		}

		bag.setAmountofToken(data["nb"]); // normalement pas besoin de reset nb
		                                // mais on sait jamais
	}
}
