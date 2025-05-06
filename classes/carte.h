#ifndef LO21_SPLENDOR_DUEL_CARTE_H
#define LO21_SPLENDOR_DUEL_CARTE_H
#include "Exception.h"
#include <array>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <initializer_list>
#include <iostream>
#include <nlohmann/json.hpp>
#include <optional>
#include <string>
#include <vector>

#include <nlohmann/json.hpp>

using json = nlohmann::json;

using namespace std;

enum class Capacity {
  rejouer,
  voler_pion_adverse,
  prendre_privilege,
  prendre_sur_plateau,
  joker
};
enum class Color { bleu, rouge, vert, blanc, noir, perle, gold };
enum class colorBonus { bleu, red, vert, blanc, noir, joker };

optional<Capacity> getCapaciteFromString(const std::string &str);
optional<colorBonus> getBonusFromString(const std::string &str);

std::string toString(Color c);
std::string toString(Capacity c);
std ::string toString(optional<colorBonus> bonus);

std::string toStringJson(Color c);
std::string toStringJson(Capacity c);
std ::string toStringJson(optional<colorBonus> bonus);

std::ostream &operator<<(std::ostream &f, Color c);
std::ostream &operator<<(std::ostream &f, optional<Capacity> c);
std::ostream &operator<<(ostream &f, optional<colorBonus> b);

extern std::initializer_list<Color> Colors;
extern std::initializer_list<Capacity> Capacities;
extern std::initializer_list<colorBonus> ColorBonuses;

class Card {

protected:
  static const int MAX = 71;
  static int nb_cartes;

private:
  Card &operator=(Card &c) = delete;

  const optional<Capacity> capacite;
  const int points_prestige;

  const std::string visuel;

public:
  static int getNbCartes() { return nb_cartes; }

  virtual ~Card() { nb_cartes--; }

  explicit Card(int points_prestige = 0,
                std::optional<Capacity> capacite = std::nullopt,
                std::string visuel = "")
      : capacite(capacite), points_prestige(points_prestige), visuel(visuel) {
    if (nb_cartes > MAX) {
      throw SplendorException("Maximum de cartes atteint");
    }

    if (points_prestige < 0 || points_prestige > 10) {
      throw SplendorException("Valeur non autorisée");
    }
    ++nb_cartes;
  }

  const optional<Capacity> &getCapacite() const { return capacite; }

  const std::string getVisuel() const { return visuel; }

  const int getPrestige() const { return points_prestige; }
};

class RoyalCard : public Card {

  static const int max_royal_card = 4;

public:
  json toJson() const;

  static const int getMaxCarteR() { return max_royal_card; }

  RoyalCard(int points_prestige = 0, std::optional<Capacity> capacite = nullopt,
            std::string visuel = "")
      : Card(points_prestige, capacite, visuel) {
    if (points_prestige < 0 || points_prestige > 10) {
      throw SplendorException("Valeur non autorisée");
    }
    if (nb_cartes > MAX) {
      throw SplendorException("Maximum de cartes atteint");
    }
  }
};

class JewelryCard : public Card {
  const int cout_blanc;
  const int cout_bleu;
  const int cout_rouge;
  const int cout_vert;
  const int cout_noir;
  const int cout_perle;
  const int niveau;
  const int nb_couronnes;
  mutable optional<enum colorBonus> bonus;
  const int bonus_nombre;

  static const int max_carte_j = 67;

public:
  json toJson() const;

  const int getCostWhite() const { return cout_blanc; }
  const int getCostBlue() const { return cout_bleu; }
  const int getCostRed() const { return cout_rouge; }
  const int getCostGreen() const { return cout_vert; }
  const int getCostBlack() const { return cout_noir; }
  const int getCostPerl() const { return cout_perle; }
  static const int getMaxCarteJ() { return max_carte_j; }

  JewelryCard(int points_prestiges = 0, int cout_blanc = 0, int cout_bleu = 0,
              int cout_rouge = 0, int cout_vert = 0, int cout_noir = 0,
              int cout_perle = 0, int niveau = 1, int nb_couronnes = 0,
              int bonus_nombre = 0, optional<enum colorBonus> bonus = nullopt,
              optional<Capacity> capacite = nullopt, std::string visuel = "")
      : Card(points_prestiges, capacite, visuel), cout_blanc(cout_blanc),
        cout_bleu(cout_bleu), cout_rouge(cout_rouge), cout_vert(cout_vert),
        cout_noir(cout_noir), cout_perle(cout_perle), niveau(niveau),
        nb_couronnes(nb_couronnes), bonus(bonus), bonus_nombre(bonus_nombre) {
    if (nb_cartes > MAX) {

      throw SplendorException("Maximum de cartes atteint");
    }
    if (cout_blanc < 0 || cout_blanc > 10 || cout_bleu < 0 || cout_bleu > 10 ||
        cout_rouge < 0 || cout_rouge > 10 || cout_vert < 0 || cout_vert > 10 ||
        cout_noir < 0 || cout_noir > 10 || cout_perle < 0 || cout_perle > 10 ||
        niveau < 1 || niveau > 3 || nb_couronnes < 0 || nb_couronnes > 5 ||
        bonus_nombre < 0 || bonus_nombre > 5) {
      throw SplendorException("Valeur non autorisée");
    }
  }

  const int getNiveau() const { return niveau; }
  const int getNbCrown() const { return nb_couronnes; }
  const optional<enum colorBonus> &getBonus() const { return bonus; }
  const int getNbBonus() const { return bonus_nombre; }
  void changerCouleurBonus(colorBonus b) const { bonus = b; }
};

inline std::ostream &operator<<(std::ostream &os, const Card &c) {
  os << "Capacity : " << c.getCapacite()
     << " points prestiges : " << c.getPrestige() << endl;
  return os;
}
inline std::ostream &operator<<(std::ostream &os, const RoyalCard &c) {
  os << "Capacity : " << c.getCapacite()
     << ", points prestiges : " << c.getPrestige() << endl;
  return os;
}
inline std::ostream &operator<<(std::ostream &os, const JewelryCard &c) {

  os << "Capacity : " << c.getCapacite();
  if (c.getPrestige() > 0) {
    os << ", points prestiges : " << c.getPrestige();
  }
  if (c.getCostPerl() > 0) {
    os << ", Cout Perle :" << c.getCostPerl();
  }
  if (c.getCostWhite() > 0) {
    os << ", Cout Blanc :" << c.getCostWhite();
  }
  if (c.getCostBlue() > 0) {
    os << ", Cout Bleu :" << c.getCostBlue();
  }
  if (c.getCostRed() > 0) {
    os << ", Cout Rouge :" << c.getCostRed();
  }
  if (c.getCostGreen() > 0) {
    os << ", Cout Vert :" << c.getCostGreen();
  }
  if (c.getCostBlack() > 0) {
    os << ", Cout Noir :" << c.getCostBlack();
  }
  if (c.getNbBonus() > 0) {
    os << ", Bonus : " << c.getBonus()
       << ", nombre de bonus : " << c.getNbBonus();
  }
  if (c.getNbCrown() > 0) {
    os << ", Nb Couronne : " << c.getNbCrown();
  }
  os << ", Niveau : " << c.getNiveau() << endl;

  return os;
}

inline Color stringToColor(std::string &str) {
  if (str == "Bleu")
    return Color::bleu;
  if (str == "Blanc")
    return Color::blanc;
  if (str == "Vert")
    return Color::vert;
  if (str == "Rouge")
    return Color::rouge;
  if (str == "Noir")
    return Color::noir;
  if (str == "Perle")
    return Color::perle;
  if (str == "Gold")
    return Color::gold;
  else
    throw SplendorException("La string passé ne correspond à aucune couleur! "
                            "Attention, fonction sensible à la casse!");
}
const Color colorBonusToColor(const optional<enum colorBonus> &couleur);
const colorBonus stringToBonus(const std::string &str);
vector<const RoyalCard *> initCartesRoyales();
vector<const JewelryCard *> initCartesJoaillerie();

void testInitCartes();
#endif // LO21_SPLENDOR_DUEL_CARTE_H
