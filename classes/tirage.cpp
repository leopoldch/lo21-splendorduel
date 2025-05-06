#include "tirage.h"

int Tirage::nb_tirages = 0; // initialisation du compteur à 0

void Tirage::remplirTirage() {

  vector<const JewelryCard *> p = this->pioche.getPioche();

  while (this->nb_cartes < this->max_cartes && !this->pioche.est_vide()) {

    const JewelryCard &c = this->pioche.getCarte();
    this->cartes.push_back(&c);
    this->nb_cartes++;
  }
}

void testTirage() {

  Pioche p1 = Pioche(1);
  Pioche p2 = Pioche(2);
  Pioche p3 = Pioche(3);

  vector<const JewelryCard *> liste_cartes = initCartesJoaillerie();

  p1.InitPioches(&p1, &p2, &p3, liste_cartes);

  std::cout << p1.getPioche().size() << endl;

  Tirage t1 = Tirage(1, 5, p1);
  t1.remplirTirage();

  std::cout << p1.getPioche().size() << endl;

  Tirage t2 = Tirage(2, 4, p2);
  t2.remplirTirage();

  Tirage t3 = Tirage(3, 3, p3);
  t3.remplirTirage();

  Tirage t4 = Tirage(3, 3, p3);
  t4.remplirTirage();

  t1.remplirTirage();
}
