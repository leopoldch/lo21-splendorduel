#include "plateau.h"
#include <iostream>
#include <optional>

using namespace std;

optional<Position> inverse_position(const optional<Position> &pos) {
  if (pos == Position::droite) {
    return Position::gauche;
  }
  if (pos == Position::diag_haut_droite) {
    return Position::diag_bas_gauche;
  }
  if (pos == Position::dessus) {
    return Position::dessous;
  }
  if (pos == Position::diag_haut_gauche) {
    return Position::diag_bas_droite;
  }
  if (pos == Position::gauche) {
    return Position::droite;
  }
  if (pos == Position::diag_bas_gauche) {
    return Position::diag_haut_droite;
  }
  if (pos == Position::dessous) {
    return Position::dessus;
  }
  if (pos == Position::diag_bas_droite) {
    return Position::diag_haut_gauche;
  } else
    return nullopt;
}

std::string toString(optional<Position> &pos) {
  if (pos == Position::droite) {
    return "droite";
  }
  if (pos == Position::diag_haut_droite) {
    return "diag_haut_droite";
  }
  if (pos == Position::dessus) {
    return "dessus";
  }
  if (pos == Position::diag_haut_gauche) {
    return "diag_haut_gauche";
  }
  if (pos == Position::gauche) {
    return "gauche";
  }
  if (pos == Position::diag_bas_gauche) {
    return "diag_bas_gauche";
  }
  if (pos == Position::dessous) {
    return "dessous";
  }
  if (pos == Position::diag_bas_droite) {
    return "diag_bas_droite";
  }
  if (pos == nullopt) {
    return "pas à coté!";
  }
}

Plateau::Handler_Plateau Plateau::handler_plateau;
Plateau &Plateau::get_plateau() {
  if (handler_plateau.instance == nullptr) {
    handler_plateau.instance = new Plateau();
    // au départ le plateau est rempli
    // handler_plateau.instance->remplir_plateau(Sac::get_sac());
  }
  return *handler_plateau.instance;
}

void Plateau::libere_plateau() {
  delete handler_plateau.instance;
  handler_plateau.instance = nullptr;
}

void Plateau::printTab() const {
  for (size_t i = 0; i < Jeton::getNbMaxJetons(); i++) {
    if (jetons[i] != nullptr) {
      if ((i + 1) % 5 == 0) {
        cout << "indice : " << i << " " << *jetons[i] << "\n";
      } else {
        cout << "indice : " << i << " " << *jetons[i] << " | ";
      }
    } else {
      if ((i + 1) % 5 == 0) {
        cout << "indice : " << i << " id: null; couleur: null\n";
      } else {
        cout << "indice : " << i << " id: null; couleur: null |";
      }
    }
  }
}

void Plateau::remplir_plateau(Sac &sac) {
  srand(static_cast<unsigned>(std::time(nullptr)));
  int i = 0;
  while (Sac::get_sac().get_nb_sac() > 0) {
    if (jetons[ordre[i]] == nullptr) {
      int nb_sac = sac.get_nb_sac();
      int rdm = rand() % nb_sac;
      set_plateau_i(ordre[i], sac.get_jeton_i(rdm));
      sac.retirer_jeton_i(rdm);
      current_nb++;
    }
    i++;
  }
}

extern std::initializer_list<Position> Positionsss = {
    Position::droite,  Position::diag_haut_droite,
    Position::dessus,  Position::diag_haut_gauche,
    Position::gauche,  Position::diag_bas_gauche,
    Position::dessous, Position::diag_bas_droite};

std::map<std::string, enum Position> positionMap = {
    {"droite", Position::droite},
    {"diag_haut_droite", Position::diag_haut_droite},
    {"dessus", Position::dessus},
    {"diag_haut_gauche", Position::diag_haut_gauche},
    {"gauche", Position::gauche},
    {"diag_bas_gauche", Position::diag_bas_gauche},
    {"dessous", Position::dessous},
    {"diag_bas_droite", Position::diag_bas_droite}};

optional<const Position> Plateau::jeton_i_est_a_cote(int i, const Jeton *jet) {
  if (jet == get_droite_i(i))
    return Position::droite;
  if (jet == get_droite_dessus_i(i))
    return Position::diag_haut_droite;
  if (jet == get_dessus_i(i))
    return Position::dessus;
  if (jet == get_gauche_dessus_i(i))
    return Position::diag_haut_gauche;
  if (jet == get_gauche_i(i))
    return Position::gauche;
  if (jet == get_gauche_dessous_i(i))
    return Position::diag_bas_gauche;
  if (jet == get_dessous_i(i))
    return Position::dessous;
  if (jet == get_droite_dessous_i(i))
    return Position::diag_bas_droite;

  return nullopt;
}

bool Plateau::onlyGold() {
  if (getCurrentNb() == 0)
    throw SplendorException("Plateau vide!");
  for (auto jet : jetons) {
    if (jet != nullptr and jet->getColor() != Color::gold)
      return false;
  }
  return true;
}

bool Plateau::colorInPlateau(const optional<enum colorBonus> &couleur) {
  if (getCurrentNb() == 0)
    throw SplendorException("Plateau vide!");
  string s = "Bonus ";
  for (auto jet : jetons) {
    if (jet != nullptr and
        (s + toString(jet->getColor()) == toString(couleur))) {
      return true;
    }
  }
  return false;
}

vector<int>
Plateau::getIndicesJetonsCouleur(const optional<enum colorBonus> &couleur) {
  if (getCurrentNb() == 0)
    throw SplendorException("Plateau vide!");
  vector<int> result;
  string s = "Bonus ";
  for (auto jet : jetons) {
    if (jet != nullptr and
        (s + toString(jet->getColor()) != toString(couleur))) {
      result.push_back(Plateau::get_plateau().getIndice(jet->get_id()));
    }
  }
  return result;
}