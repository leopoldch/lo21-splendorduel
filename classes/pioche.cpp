#include "pioche.h"

int Pioche::nb_pioches = 0;

const JewelryCard &Pioche::getCarte() {
  if (this->pioche.size() == 0) {
    throw SplendorException("Attention pas de cartes dans la pioche !");
  }
  const JewelryCard *carte_retour = pioche[0];
  pioche.erase(pioche.begin());
  return *carte_retour;
}

void Pioche::InitPioches(Pioche *p1, Pioche *p2, Pioche *p3,
                         vector<const JewelryCard *> &cartes) {
  if (nb_pioches != max_pioches) {
    throw SplendorException("les pioches ne sont pas toutes créées");
  }

  for (const JewelryCard *carte : cartes) { // pour chaque carte (ptr constant
                                            // vers une carte) du vecteur cartes
    switch (carte->getNiveau()) {
    case 1:
      (p1->pioche).push_back(carte);
      break;
    case 2:
      (p2->pioche).push_back(carte);
      break;
    case 3:
      (p3->pioche).push_back(carte);
      break;
    }
  }
  // Melange les éléments des pioches
  random_device rd;
  mt19937 g(rd());

  shuffle((p1->pioche).begin(), (p1->pioche).end(), g);
  shuffle((p2->pioche).begin(), (p2->pioche).end(), g);
  shuffle((p3->pioche).begin(), (p3->pioche).end(), g);
}
