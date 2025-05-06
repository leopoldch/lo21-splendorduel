//
// Created by Léopold Chappuis on 14/12/2023.
//

#ifndef LO21_SPLENDOR_DUEL_JSON_H
#define LO21_SPLENDOR_DUEL_JSON_H

#include "joueur.h"
#include <nlohmann/json.hpp>

vector<const JewelryCard *> fromJsonJewelryCard(json data);
vector<const RoyalCard *> fromJsonRoyalCard(json data);
vector<const Jeton *> fromJsonJetons(json data);
std::vector<const Privilege *> fromJsonPrivileges(json data, unsigned int nb);
Pioche &fromJsonPioche(json data);
Tirage &fromJsonTirage(json data, Pioche &p);
void fromJsonPlateau(json data);
void fromJsonSac(json data);

#endif // LO21_SPLENDOR_DUEL_JSON_H
