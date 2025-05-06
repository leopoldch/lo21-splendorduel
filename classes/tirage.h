#ifndef LO21_SPLENDOR_DUEL_TIRAGE_H
#define LO21_SPLENDOR_DUEL_TIRAGE_H
#include "Exception.h"
#include "carte.h"
#include "pioche.h"
#include <cstring>
#include <iostream>
#include <vector>
using namespace std;

class Tirage {

  static const int nb_max_tirages = 3; // limite de tirage
  static int nb_tirages;               // compteur
  Pioche &pioche;
  const int niveau;
  int nb_cartes;
  const int max_cartes;
  vector<const JewelryCard *> cartes;

public:
  const json toJson() {
    json j;
    j["niveau"] = getNiveau();
    j["nb_cartes"] = nb_cartes;
    j["max_cartes"] = max_cartes;
    j["cartes_joailleries"] = {};

    for (int i = 0; i < cartes.size(); ++i) {
      j["cartes_joailleries"].push_back(cartes[i]->toJson());
    }

    return j;
  }

  // constructeur du tirage
  Tirage(int niv, int max, Pioche &p)
      : niveau(niv), max_cartes(max), pioche(p) {
    if (p.getNiveau() != niveau) {
      throw SplendorException(
          "La pioche n'est pas du même niveau que le tirage !");
    } else if (nb_tirages == nb_max_tirages) {
      throw SplendorException("Nombre maximum de tirages dépassé !");
    }

    nb_cartes = 0; // on initialise le nombre de cartes à 0
    nb_tirages++;
  }

  ~Tirage() { nb_tirages--; }

  // définition des getters
  const int getNiveau() const { return niveau; }
  const int getNbCartes() const { return nb_cartes; }
  void setNbCartes(int nb) { nb_cartes = nb; }

  Pioche &getPioche() const { return pioche; }
  vector<const JewelryCard *> &getTirage() { return cartes; }

  void setTirage(vector<const JewelryCard *> t) { cartes = t; }

  // déclaration de la méthode qui permet de remplir le Tirage avec les cartes
  // (voir tirage.cpp)
  void remplirTirage();

  const JewelryCard &getCarte(unsigned int indice) {

    if (nb_cartes == 0) {
      throw SplendorException("Attention pas de cartes dans le tirage !");
    }

    const JewelryCard *carte_retour = cartes[indice];
    cartes.erase(cartes.begin() + indice);
    nb_cartes--;

    return *carte_retour;
  }

  const JewelryCard &getCarteSansSupr(unsigned int indice) {
    if (nb_cartes == 0) {
      throw SplendorException("Attention pas de cartes dans le tirage !");
    }
    return *cartes[indice];
  }

private:
  Tirage &operator=(const Tirage &t) = delete;
  Tirage(const Tirage &t) = delete;
};

// surchage de l'opérateur pour print sur la sortie standard le tirage
inline std::ostream &operator<<(std::ostream &f, Tirage &t) {
  int i = 0;
  for (const JewelryCard *c : t.getTirage()) {
    f << "indice : " << i << " carte : " << *c << endl;
    i++;
  }
  return f;
}

void testTirage();

#endif // LO21_SPLENDOR_DUEL_TIRAGE_H