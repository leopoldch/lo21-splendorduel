#ifndef LO21_SPLENDOR_DUEL_JEU_H
#define LO21_SPLENDOR_DUEL_JEU_H
#include "json.h"
#include <iostream>
class Strategy_player;

class Jeu {
private:
  bool est_termine = false;
  Strategy_player *qui_joue;
  unsigned int manche = 0;
  Strategy_player *adversaire;
  vector<const JewelryCard *>
      cartes_joiallerie;        // Contient toutes les cartes du jeu
  vector<const Jeton *> jetons; // Contient tous les jetons
  vector<const RoyalCard *> cartes_royales;
  vector<const Privilege *> privileges;
  Pioche *p1;
  Pioche *p2;
  Pioche *p3;

  Tirage *tirage_1;
  Tirage *tirage_2;
  Tirage *tirage_3;

  struct Handler {
    Jeu *instance = nullptr;
    ~Handler() {
      delete instance;
      instance = nullptr;
    }
  };

  static Handler handler;
  Jeu();
  Jeu(json data);
  ~Jeu();

  Jeu(const Jeu &) = delete;
  Jeu &operator=(const Jeu &) = delete;

public:
  json toJson() const;

  // Initialiser les noms des joueurs
  void setPlayers(string &name1, string &name2, string &choix1,
                  string &choix2); // version Qt
  void setPlayers();

  void remplirPlateau() {
    Plateau::get_plateau().remplir_plateau(Sac::get_sac());
  }

  Strategy_player &getCurrentPlayer() const { return *qui_joue; }
  Strategy_player &getOpponent() const { return *adversaire; }

  vector<const RoyalCard *> getCartesRoyales() const { return cartes_royales; }

  vector<const JewelryCard *> getCartesJoaillerie() const {
    return cartes_joiallerie;
  }

  const RoyalCard &pullCarteRoyale(unsigned int i) {
    if (i > cartes_royales.size() || i < 0)
      throw SplendorException("Indice non valide ! ");
    const RoyalCard *tmp = cartes_royales[i];
    cartes_royales.erase(cartes_royales.begin() + i);
    return *tmp;
  }

  const unsigned int getNbPrivilege() const { return privileges.size(); }

  const Privilege *getPrivilege() {
    if (privileges.size() <= 0) {
      throw SplendorException("Plus de privilège disponible");
    }
    const Privilege *tmp = privileges[0];
    privileges.erase(privileges.begin());
    return tmp;
  }

  Pioche *getPioche(int num) const {
    if (num == 1)
      return p1;
    if (num == 2)
      return p2;
    if (num == 3)
      return p3;

    throw SplendorException("Cette pioche n'existe pas!");
  }

  void setPrivilege(const Privilege &p) {
    if (privileges.size() >= 3) {
      throw SplendorException(
          "Nombre de privilège max dans le jeu déjà atteint");
    }
    privileges.push_back(&p);
  }

  void printCarteRoyale();

  const bool isFinished();

  Strategy_player &get_tour();
  Tirage *get_tirage_1() { return tirage_1; }
  Tirage *get_tirage_2() { return tirage_2; }
  Tirage *get_tirage_3() { return tirage_3; }
  Tirage *get_tirage(int i) {
    switch (i) {
    case (1):
      return tirage_1;
      break;
    case (2):
      return tirage_2;
      break;
    case (3):
      return tirage_3;
      break;
    default:
      throw SplendorException("Le tirage demandé n'existe pas!");
    }
  }
  void tour_suivant(bool replay = 0);
  static Jeu &getJeu();
  static Jeu &getJeu(json data);
  static void libereJeu();
  static void test();

  const Jeton *getJeton(int i) { return jetons[i]; }
  void setJoueurNames(const string &s1, const string &s2);

  vector<const Privilege *> &get_privileges() { return privileges; }
  unsigned int getManche() { return manche; }
};

#endif // LO21_SPLENDOR_DUEL_JEU_H
