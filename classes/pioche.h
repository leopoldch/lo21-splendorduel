#ifndef LO21_SPLENDOR_DUEL_PIOCHE_H
#define LO21_SPLENDOR_DUEL_PIOCHE_H
#include "carte.h"
#include <algorithm>
#include <iostream>
#include <random>

using namespace std;

class Pioche {
protected:
  static const int max_pioches = 3;
  static int nb_pioches;

private:
  const int niveau;

  Pioche &operator=(Pioche &p) = delete;

  vector<const JewelryCard *> pioche;

public:
  ~Pioche() { nb_pioches--; }

  Pioche(int niv) : niveau(niv) {
    if (nb_pioches > max_pioches) {
      throw SplendorException("Maximum de pioches atteint");
    }
    ++nb_pioches;
  }

  int getNiveau() const { return niveau; }

  bool est_vide() { return pioche.empty(); }

  vector<const JewelryCard *> getPioche() const { return pioche; }

  static int getNbPioches() { return nb_pioches; }

  const void setPioche(vector<const JewelryCard *> p) { pioche = p; }

  static void InitPioches(Pioche *p1, Pioche *p2, Pioche *p3,
                          vector<const JewelryCard *> &cartes);

  const JewelryCard &getCarte();

  const json toJson() {
    json j;
    j["niveau"] = getNiveau();
    j["cartes_joailleries"] = {};
    for (int i = 0; i < pioche.size(); ++i) {
      j["cartes_joailleries"].push_back(pioche[i]->toJson());
    }
    return j;
  }
};

#endif // LO21_SPLENDOR_DUEL_MAIN_H