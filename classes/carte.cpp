#include "carte.h"
#include <fstream>
int Card::nb_cartes = 0;

std::string toString(Color c) {
  switch (c) {
  case Color::rouge:
    return "Rouge";
  case Color::bleu:
    return "Bleu";
  case Color::vert:
    return "Vert";
  case Color::blanc:
    return "Blanc";
  case Color::noir:
    return "Noir";
  case Color::perle:
    return "Perle";
  case Color::gold:
    return "Gold";
  default:
    throw SplendorException("Color inconnue");
  }
}
std::string toString(optional<Capacity> c) {
  if (c != nullopt) {
    if (c == Capacity::rejouer) {
      return "Rejouer";
    } else if (c == Capacity::voler_pion_adverse) {
      return "Voler un pion adverse";
    } else if (c == Capacity::prendre_privilege) {
      return "Prendre un privilege";
    } else if (c == Capacity::prendre_sur_plateau) {
      return "Prendre un pion de la même couleur que le bonus de la carte sur "
             "le plateau";
    } else if (c == Capacity::joker) {
      return "Joker";
    } else {
      throw SplendorException("Capacity inconnue");
    }
  } else {
    return "Null";
  }
}
std ::string toString(optional<enum colorBonus> bonus) {
  if (bonus != nullopt) {
    if (bonus == colorBonus::bleu) {
      return "Bonus Bleu";
    } else if (bonus == colorBonus::red) {
      return "Bonus Rouge";
    } else if (bonus == colorBonus::vert) {
      return "Bonus Vert";
    } else if (bonus == colorBonus::blanc) {
      return "Bonus Blanc";
    } else if (bonus == colorBonus::noir) {
      return "Bonus Noir";
    } else if (bonus == colorBonus::joker) {
      return "Bonus Joker";
    }
  } else {
    return "Null";
  }
}

std::string toStringJson(Color c) {
  switch (c) {
  case Color::rouge:
    return "rouge";
  case Color::bleu:
    return "rouge";
  case Color::vert:
    return "vert";
  case Color::blanc:
    return "blanc";
  case Color::noir:
    return "noir";
  case Color::perle:
    return "perle";
  case Color::gold:
    return "gold";
  default:
    throw SplendorException("Color inconnue");
  }
}
std::string toStringJson(optional<Capacity> c) {
  if (c != nullopt) {
    if (c == Capacity::rejouer) {
      return "rejouer";
    } else if (c == Capacity::voler_pion_adverse) {
      return "voler_pion_adverse";
    } else if (c == Capacity::prendre_privilege) {
      return "prendre_privilege";
    } else if (c == Capacity::prendre_sur_plateau) {
      return "prendre_sur_plateau";
    } else if (c == Capacity::joker) {
      return "joker";
    }
  } else {
    return "null";
  }
}
std ::string toStringJson(optional<enum colorBonus> bonus) {
  if (bonus != nullopt) {
    if (bonus == colorBonus::bleu) {
      return "bleu";
    } else if (bonus == colorBonus::red) {
      return "rouge";
    } else if (bonus == colorBonus::vert) {
      return "vert";
    } else if (bonus == colorBonus::blanc) {
      return "blanc";
    } else if (bonus == colorBonus::noir) {
      return "noir";
    } else if (bonus == colorBonus::joker) {
      return "joker";
    }
  } else {
    return "null";
  }
}

std::ostream &operator<<(std::ostream &f, optional<Capacity> c) {
  if (c == nullopt)
    return f << " Null ";
  return f << toString(c);
}

std::ostream &operator<<(std::ostream &f, Color c) {
  f << toString(c);
  return f;
}

std::ostream &operator<<(ostream &f, optional<enum colorBonus> b) {
  f << toString(b);
  return f;
}

extern std::initializer_list<Color> Colors = {
    Color::rouge, Color::bleu, Color::vert, Color::blanc, Color::noir};
extern std::initializer_list<Capacity> Capacities = {
    Capacity::rejouer, Capacity::voler_pion_adverse,
    Capacity::prendre_privilege, Capacity::prendre_sur_plateau,
    Capacity::joker};
extern std::initializer_list<enum colorBonus> Bonus_couleurs = {
    colorBonus::bleu,  colorBonus::red,  colorBonus::vert,
    colorBonus::blanc, colorBonus::noir, colorBonus::joker};

std::map<std::string, enum colorBonus> bonusMap = {
    {"bleu", colorBonus::bleu}, {"red", colorBonus::red},
    {"vert", colorBonus::vert}, {"blanc", colorBonus::blanc},
    {"noir", colorBonus::noir}, {"joker", colorBonus::joker}};

optional<enum colorBonus> getBonusFromString(const std::string &str) {
  auto it = bonusMap.find(str);
  if (it != bonusMap.end()) {
    return it->second;
  } else {
    return nullopt;
  }
}

std::map<std::string, Capacity> capaciteMap = {
    {"rejouer", Capacity::rejouer},
    {"voler_pion_adverse", Capacity::voler_pion_adverse},
    {"prendre_privilege", Capacity::prendre_privilege},
    {"prendre_sur_plateau", Capacity::prendre_sur_plateau},
    {"joker", Capacity::joker}};

optional<Capacity> getCapaciteFromString(const std::string &str) {
  auto it = capaciteMap.find(str);
  if (it != capaciteMap.end()) {
    return it->second;
  } else {
    return nullopt;
  }
}

vector<const RoyalCard *> initCartesRoyales() {
  std::ifstream file("../src/cartes.json");

  if (!file.is_open()) {
    std::cerr << "Failed to open the JSON file." << std::endl;
    throw SplendorException("Fichier non ouvert");
  }

  json data;
  file >> data;
  file.close();

  std::vector<const RoyalCard *> cartes_royales_instances;

  for (const auto &carte_royale_data : data["cartes_royales"]) {

    std::string image = carte_royale_data["lien"];
    std::string visuel = "../src/Cartes_detourees/" + image;

    if (!carte_royale_data["capacite"].is_null()) {
      RoyalCard *instance = new RoyalCard(
          carte_royale_data["points_prestige"],
          getCapaciteFromString(carte_royale_data["capacite"]), visuel);
      cartes_royales_instances.push_back(instance);
    } else {
      RoyalCard *instance =
          new RoyalCard(carte_royale_data["points_prestige"], nullopt, visuel);
      cartes_royales_instances.push_back(instance);
    }
  }
  return cartes_royales_instances;
}

vector<const JewelryCard *> initCartesJoaillerie() {
  std::ifstream file("../src/cartes.json");

  if (!file.is_open()) {
    std::cerr << "Failed to open the JSON file." << std::endl;
    throw SplendorException("Fichier non ouvert");
  }

  json data;
  file >> data;
  file.close();
  std::vector<const JewelryCard *> cartes_joailleries_instances;
  for (const auto &carte_joaillerie_data : data["cartes_joailleries"]) {

    std::string image = carte_joaillerie_data["lien"];
    std::string visuel = "../src/Cartes_detourees/" + image;

    if (carte_joaillerie_data["capacite"].is_null() &&
        carte_joaillerie_data["bonus_couleur"].is_null()) {

      JewelryCard *instance = new JewelryCard(
          carte_joaillerie_data["points_prestige"],
          carte_joaillerie_data["cout_blanc"],
          carte_joaillerie_data["cout_bleu"],
          carte_joaillerie_data["cout_rouge"],
          carte_joaillerie_data["cout_vert"],
          carte_joaillerie_data["cout_noir"],
          carte_joaillerie_data["cout_perle"], carte_joaillerie_data["niveau"],
          carte_joaillerie_data["nb_couronne"],
          carte_joaillerie_data["bonus_nombre"], nullopt, nullopt, visuel);
      cartes_joailleries_instances.push_back(instance);
    } else if (!carte_joaillerie_data["capacite"].is_null() &&
               carte_joaillerie_data["bonus_couleur"].is_null()) {

      JewelryCard *instance = new JewelryCard(
          carte_joaillerie_data["points_prestige"],
          carte_joaillerie_data["cout_blanc"],
          carte_joaillerie_data["cout_bleu"],
          carte_joaillerie_data["cout_rouge"],
          carte_joaillerie_data["cout_vert"],
          carte_joaillerie_data["cout_noir"],
          carte_joaillerie_data["cout_perle"], carte_joaillerie_data["niveau"],
          carte_joaillerie_data["nb_couronne"],
          carte_joaillerie_data["bonus_nombre"], nullopt,
          getCapaciteFromString(carte_joaillerie_data["capacite"]), visuel);
      cartes_joailleries_instances.push_back(instance);

    } else if (carte_joaillerie_data["capacite"].is_null() &&
               !carte_joaillerie_data["bonus_couleur"].is_null()) {

      JewelryCard *instance = new JewelryCard(
          carte_joaillerie_data["points_prestige"],
          carte_joaillerie_data["cout_blanc"],
          carte_joaillerie_data["cout_bleu"],
          carte_joaillerie_data["cout_rouge"],
          carte_joaillerie_data["cout_vert"],
          carte_joaillerie_data["cout_noir"],
          carte_joaillerie_data["cout_perle"], carte_joaillerie_data["niveau"],
          carte_joaillerie_data["nb_couronne"],
          carte_joaillerie_data["bonus_nombre"],
          getBonusFromString(carte_joaillerie_data["bonus_couleur"]), nullopt,
          visuel);
      cartes_joailleries_instances.push_back(instance);
    } else {

      JewelryCard *instance = new JewelryCard(
          carte_joaillerie_data["points_prestige"],
          carte_joaillerie_data["cout_blanc"],
          carte_joaillerie_data["cout_bleu"],
          carte_joaillerie_data["cout_rouge"],
          carte_joaillerie_data["cout_vert"],
          carte_joaillerie_data["cout_noir"],
          carte_joaillerie_data["cout_perle"], carte_joaillerie_data["niveau"],
          carte_joaillerie_data["nb_couronne"],
          carte_joaillerie_data["bonus_nombre"],
          getBonusFromString(carte_joaillerie_data["bonus_couleur"]),
          getCapaciteFromString(carte_joaillerie_data["capacite"]), visuel);
      cartes_joailleries_instances.push_back(instance);
    }
  }
  return cartes_joailleries_instances;
}

// Json

json JewelryCard::toJson() const {
  json j;
  if (getCapacite() != nullopt) {
    j["capacite"] = toStringJson(getCapacite());
  } else {
    j["capacite"] = NULL;
  }
  j["points_prestige"] = getPrestige();
  j["niveau"] = getNiveau();
  if (getBonus() != nullopt) {
    j["bonus_couleur"] = toStringJson(getBonus());
  } else {
    j["bonus_couleur"] = NULL;
  }
  j["bonus_nombre"] = getNbBonus();
  j["nb_couronne"] = getNbCrown();
  j["cout_blanc"] = getCostWhite();
  j["cout_bleu"] = getCostBlue();
  j["cout_vert"] = getCostGreen();
  j["cout_rouge"] = getCostRed();
  j["cout_noir"] = getCostBlack();
  j["cout_perle"] = getCostPerl();
  j["visuel"] = getVisuel();

  return j;
}
json RoyalCard::toJson() const {
  json j;
  if (getCapacite() != nullopt) {
    j["capacite"] = toStringJson(getCapacite());
  } else {
    j["capacite"] = NULL;
  }
  j["points_prestige"] = getPrestige();
  j["visuel"] = getVisuel();

  return j;
}

// Tests unitaires de la classe Card :

void testInitCartes() {

  vector<const RoyalCard *> test = initCartesRoyales();
  vector<const JewelryCard *> test2 = initCartesJoaillerie();

  for (int k = 0; k < test2.size(); k++) {
    cout << *test2[k] << endl;
  }
  for (int k = 0; k < test.size(); k++) {
    cout << *test[k] << endl;
  }

  cout << "\n" << Card::getNbCartes() << endl;
}

const Color colorBonusToColor(const optional<colorBonus> &couleur) {
  if (couleur == colorBonus::red)
    return Color::rouge;
  if (couleur == colorBonus::vert)
    return Color::vert;
  if (couleur == colorBonus::bleu)
    return Color::bleu;
  if (couleur == colorBonus::blanc)
    return Color::blanc;
  if (couleur == colorBonus::noir)
    return Color::noir;
  else
    throw SplendorException("Bonus couleur inconnu!");
}

const colorBonus stringToBonus(const std::string &str) {
  if (str == "Rouge")
    return colorBonus::red;
  if (str == "Vert")
    return colorBonus::vert;
  if (str == "Bleu")
    return colorBonus::bleu;
  if (str == "Blanc")
    return colorBonus::blanc;
  if (str == "Noir")
    return colorBonus::noir;
  else
    throw SplendorException("Str Bonus couleur inconnu!");
}
