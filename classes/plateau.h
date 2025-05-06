#ifndef LO21_SPLENDOR_DUEL_PLATEAU_H
#define LO21_SPLENDOR_DUEL_PLATEAU_H
#include "Exception.h"
#include "jetons.h"
#include "sac.h"
#include <iostream>
#include <vector>

const int ordre[25] = {12, 7,  8,  13, 18, 17, 16, 11, 6,  1,  2, 3, 4,
                       9,  14, 19, 24, 23, 22, 21, 20, 15, 10, 5, 0};
enum class Position {
  droite,
  diag_haut_droite,
  dessus,
  diag_haut_gauche,
  gauche,
  diag_bas_gauche,
  dessous,
  diag_bas_droite
};
extern std::initializer_list<Position> Positionsss;

optional<Position> inverse_position(const optional<Position> &pos);
std::string toString(optional<Position> &pos);

using namespace std;

class Plateau {
  /*
   * On prend la disposition de plateau suivante, selon l'ordre des jetons dans
   * le tableau: 0   |  1   |   2  |   3  |  4
   *   --------------------------------
   *     5   |  6   |  7   |   8  |  9
   *   --------------------------------
   *     10  |  11  |  12  |  13  |  14
   *   --------------------------------
   *     15  |  16  |  17  |  18  |  19
   *   --------------------------------
   *     20  |  21  |  22  |  23  |  24
   */

  int nb = 0;
  int current_nb = 0;

  vector<const Jeton *> jetons;
  struct Handler_Plateau {
    Plateau *instance = nullptr;
    ~Handler_Plateau() {
      delete instance;
      instance = nullptr;
    }
  };
  static Handler_Plateau handler_plateau;
  Plateau() : nb(Jeton::getNbMaxJetons()), jetons() {
    for (size_t i = 0; i < Jeton::getNbMaxJetons(); i++) {
      jetons.push_back(nullptr);
    }
  };
  ~Plateau() = default; // car agrégation !
  Plateau &operator=(const Plateau &p) = delete;
  Plateau(const Plateau &p) = delete;

public:
  json toJson() const {
    json j;
    j["jetons"] = {};
    j["nb"] = nb;
    j["current_nb"] = current_nb;
    for (int i = 0; i < jetons.size(); ++i) {
      if (jetons[i] != nullptr) {
        j["jetons"].push_back(jetons[i]->toJson(i));
      } else {
        j["jetons"].push_back(nullptr);
      }
    }
    return j;
  }

  const int getCurrentNb() const { return current_nb; }
  const int getNb() const { return nb; }

  void setCurrentNb(const int value) { current_nb = value; }

  static Plateau &get_plateau();
  static void libere_plateau();

  const bool isInside(const unsigned int uid) {
    for (int i = 0; i < jetons.size(); ++i) {
      if (jetons[i]->get_id() == uid) {
        return true;
      }
    }
    return false;
  }

  unsigned int getIndice(const unsigned int uid) {
    for (int i = 0; i < jetons.size(); ++i) {
      if (jetons[i] != nullptr and jetons[i]->get_id() == uid) {
        return i;
      }
    }
    throw SplendorException("Jeton non présent sur le plateau");
  }

  void setJetons(vector<const Jeton *> j) {
    if (jetons.size() > nb) {
      throw SplendorException("Trop de jetons dans le tableau");
    }
    jetons = j;
  }

  void setNb(unsigned int n) { nb = n; }

  const Jeton *get_plateau_i(int i) const { return jetons[i]; }

  void set_plateau_i(int i, const Jeton *jet) { jetons[i] = jet; }

  const Jeton *get_droite_i(int i) const {
    if ((i + 1) % 5 != 0) {
      return jetons[++i];
    } else {
      return nullptr;
    }
  }
  const Jeton *get_gauche_i(int i) const {
    if (i % 5 != 0) {
      return jetons[--i];
    } else {
      return nullptr;
    }
  }
  const Jeton *get_dessus_i(int i) const {
    if (i > 4) {
      return jetons[i - 5];
    } else {
      return nullptr;
    }
  }
  const Jeton *get_dessous_i(int i) const {
    if (i < 20) {
      return jetons[i + 5];
    } else {
      return nullptr;
    }
  }
  const Jeton *get_droite_dessus_i(int i) const {
    if ((i > 4) && ((i + 1) % 5 != 0)) { // si le jeton n'est pas au niveau
                                         // supérieur et sur un bord à droite
      return jetons[i - 4];
    } else {
      return nullptr;
    }
  }
  const Jeton *get_gauche_dessus_i(int i) const {
    if ((i > 4) && (i % 5 != 0)) { // si le jeton n'est pas au niveau supérieur
                                   // et sur un bord à gauche
      return jetons[i - 6];
    } else {
      return nullptr;
    }
  }

  const Jeton *get_droite_dessous_i(int i) const {
    if ((i < 20) && ((i + 1) % 5 != 0)) { // si le jeton n'est pas au niveau
                                          // inférieur et sur un bord à droite
      return jetons[i + 6];
    } else {
      return nullptr;
    }
  }

  const Jeton *get_gauche_dessous_i(int i) const {
    if ((i < 20) && (i % 5 != 0)) { // si le jeton n'est pas au niveau inférieur
                                    // et sur un bord à droite
      return jetons[i + 4];
    } else {
      return nullptr;
    }
  }

  optional<const Position> jeton_i_est_a_cote(int i, const Jeton *jet);

  void remplir_plateau(Sac &sac);
  void printTab() const;
  bool onlyGold();
  bool colorInPlateau(const optional<enum colorBonus> &couleur);

  vector<int> getIndicesJetonsCouleur(const optional<enum colorBonus> &couleur);
};

#endif // LO21_SPLENDOR_DUEL_PLATEAU_H
