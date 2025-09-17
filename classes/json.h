//
// Created by Léopold Chappuis on 14/12/2023.
//

#ifndef LO21_SPLENDOR_DUEL_JSON_H
#define LO21_SPLENDOR_DUEL_JSON_H

#include "player.h"
#include <nlohmann/json.hpp>

vector<const JewelryCard *> jewelryCardFromJson(json data);
vector<const RoyalCard *> royalCardsFromJson(json data);
vector<const Token *> tokensFromJson(json data);
std::vector<const Privilege *> privilegesFromJson(json data, unsigned int nb);
Deck &deckFromJson(json data);
Draw &drawFromJson(json data, Deck &p);
void boardFromJson(json data);
void bagFromJson(json data);

#endif // LO21_SPLENDOR_DUEL_JSON_H
