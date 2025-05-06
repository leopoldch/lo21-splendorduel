#ifndef LO21_SPLENDOR_DUEL_JETONS_H
#define LO21_SPLENDOR_DUEL_JETONS_H
#include "Exception.h"
#include "carte.h"
#include <iostream>
#include <vector>

using namespace std;

class Jeton {
  // il faut garantir l'unicité de chaque jeton
  const int id;
  const Color couleur;

  static int nb_perle;
  static const int max_perle = 2;
  static int nb_or;
  static const int max_or = 3;
  static int nb_vert;
  static const int max_vert = 4;
  static int nb_rouge;
  static const int max_rouge = 4;
  static int nb_bleu;
  static const int max_bleu = 4;
  static int nb_blanc;
  static const int max_blanc = 4;
  static int nb_noir;
  static const int max_noir = 4;

  const std::string visuel;

public:
  Jeton(int id, const Color &coul, std::string visuel)
      : id(id), couleur(coul), visuel(visuel) {

    if ((id < 1) || (id > Jeton::getNbMaxJetons())) {
      throw SplendorException("Mauvais id de jetons!");
    }

    switch (coul) {
    case Color::rouge: {
      if (nb_rouge < max_rouge) {
        nb_rouge++;
      } else {
        throw SplendorException("Impossible de créer plus de jetons red!");
      }
      break;
    }
    case Color::bleu: {
      if (nb_bleu < max_bleu) {
        nb_bleu++;
      } else {
        throw SplendorException("Impossible de créer plus de jetons bleu!");
      }
      break;
    }
    case Color::vert: {
      if (nb_vert < max_vert) {
        nb_vert++;
      } else {
        throw SplendorException("Impossible de créer plus de jetons vert!");
      }
      break;
    }
    case Color::blanc: {
      if (nb_blanc < max_blanc) {
        nb_blanc++;
      } else {
        throw SplendorException("Impossible de créer plus de jetons blanc!");
      }
      break;
    }
    case Color::noir: {
      if (nb_noir < max_noir) {
        nb_noir++;
      } else {
        throw SplendorException("Impossible de créer plus de jetons noir!");
      }
      break;
    }
    case Color::perle: {
      if (nb_perle < max_perle) {
        nb_perle++;
      } else {
        throw SplendorException("Impossible de créer plus de jetons perle!");
      }
      break;
    }
    case Color::gold: {
      if (nb_or < max_or) {
        nb_or++;
      } else {
        throw SplendorException("Impossible de créer plus de jetons or!");
      }
      break;
    }
    default: {
      throw SplendorException("Color inconnue");
      break;
    }
    }
  }
  ~Jeton() {
    switch (couleur) {
    case Color::rouge: {
      if (nb_rouge > 0) {
        nb_rouge--;
      } else {
        throw SplendorException("Nombre de jetons red <= 0!");
      }
      break;
    }
    case Color::bleu: {
      if (nb_bleu > 0) {
        nb_bleu--;
      } else {
        throw SplendorException("Nombre de jetons bleu <= 0!");
      }
      break;
    }
    case Color::vert: {
      if (nb_vert > 0) {
        nb_vert--;
      } else {
        throw SplendorException("Nombre de jetons vert <= 0!");
      }
      break;
    }
    case Color::blanc: {
      if (nb_blanc > 0) {
        nb_blanc--;
      } else {
        throw SplendorException("Nombre de jetons blanc <= 0!");
      }
      break;
    }
    case Color::noir: {
      if (nb_noir > 0) {
        nb_noir--;
      } else {
        throw SplendorException("Nombre de jetons noir <= 0!");
      }
      break;
    }
    case Color::perle: {
      if (nb_perle > 0) {
        nb_perle--;
      } else {
        throw SplendorException("Nombre de jetons  <= 0!");
      }
      break;
    }
    case Color::gold: {
      if (nb_or > 0) {
        nb_or--;
      } else {
        throw SplendorException("Nombre de jetons vert <= 0!");
      }
      break;
    }
    default: {
      throw SplendorException("Color de jetons inconnue");
      break;
    }
    }
  }
  Jeton &operator=(const Jeton &jet) = delete;
  Jeton(const Jeton &jet) = delete;

  const std::string getVisuel() const { return visuel; }

  const Color &getColor() const { return couleur; }
  const int get_id() const { return id; }
  static int getNbMaxJetons() {
    return max_bleu + max_blanc + max_or + max_noir + max_perle + max_rouge +
           max_vert;
  }

  json toJson(const int place = -1) const {
    json j;
    j["id"] = get_id();
    j["color"] = getColor();
    j["visuel"] = getVisuel();
    return j;
  }
};

inline ostream &operator<<(ostream &f, const Jeton &jet) {
  f << "id: " << jet.get_id() << "; couleur: " << toString(jet.getColor());
  return f;
}

vector<const Jeton *> initJetons();

#endif // LO21_SPLENDOR_DUEL_JETONS_H
