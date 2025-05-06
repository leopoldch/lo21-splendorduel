#ifndef LO21_SPLENDOR_DUEL_JOUEUR_H
#define LO21_SPLENDOR_DUEL_JOUEUR_H

#include "Exception.h"
#include "plateau.h"
#include "privilege.h"
#include "sac.h"
#include "tirage.h"
#include <QThread>
#include <iostream>
#include <string>
#include <vector>

// tâches à réaliser : constructeur destructeur Joueur (jeu?)
// tester toutes les méthodes ?
// vérifier UML si toutes les méthodes sont implémentées.

using namespace std;

class Strategy_player { // Utilisation Design Pattern Strategy
protected:
  std::string nom = "John Doe";
  int is_ia = 0;
  int nb_points;
  int nb_courones;
  int nb_cartes_j;
  int nb_cartes_j_reservees;
  int nb_cartes_r;
  int nb_privileges;
  int nb_jetons;

  unsigned int games_won;
  unsigned int games;

  static int const max_nb_jetons = 10;
  static int const max_nb_cartes_r = 2;
  static int const max_nb_privileges = 3;
  static int const max_nb_cartes_reservees = 3;

  vector<const JewelryCard *> cartes_joaillerie_achetees;
  vector<const JewelryCard *> cartes_joaiellerie_reservees; // 3 au max
  vector<const RoyalCard *> cartes_royale; // ok pour agrégation?

  vector<const Jeton *> jetons; // tableau de jetons ? or ?

  vector<const Privilege *> privileges;

  Strategy_player &operator=(const Strategy_player &) = delete;
  Strategy_player(const Strategy_player &) = delete;

public:
  const vector<const RoyalCard *> &getRoyalCards() const {
    return cartes_royale;
  }

  void game_ended(const unsigned int won) {
    if (won != 0) {
      games_won++;
    }
    games++;
  }

  json toJson() const {
    json j;
    j["nom"] = nom;
    j["is_ia"] = is_ia;
    j["nb_points"] = nb_points;
    j["nb_courones"] = nb_courones;
    j["nb_cartes_j"] = nb_cartes_j;
    j["nb_cartes_j_reservees"] = nb_cartes_j_reservees;
    j["nb_cartes_r"] = nb_cartes_r;
    j["nb_privileges"] = nb_privileges;
    j["nb_jetons"] = nb_jetons;
    j["games"] = games;
    j["games_won"] = games_won;
    j["cartes_joaillerie_achetees"] = {};
    j["cartes_joaiellerie_reservees"] = {};
    j["cartes_royale"] = {};
    j["jetons"] = {};
    j["privileges"] = {};

    for (int i = 0; i < cartes_joaillerie_achetees.size(); ++i) {
      j["cartes_joaillerie_achetees"].push_back(
          cartes_joaillerie_achetees[i]->toJson());
    }
    for (int i = 0; i < cartes_joaiellerie_reservees.size(); ++i) {
      j["cartes_joaiellerie_reservees"].push_back(
          cartes_joaiellerie_reservees[i]->toJson());
    }
    for (int i = 0; i < cartes_royale.size(); ++i) {
      j["cartes_royale"].push_back(cartes_royale[i]->toJson());
    }
    for (int i = 0; i < jetons.size(); ++i) {
      j["jetons"].push_back(jetons[i]->toJson());
    }
    for (int i = 0; i < privileges.size(); ++i) {
      j["privileges"].push_back(privileges[i]->toJson());
    }

    return j;
  }

  json toHistory() const {
    json j;
    j["nom"] = nom;
    j["is_ia"] = is_ia;
    j["games"] = games;
    j["games_won"] = games_won;
    return j;
  }

  // constructeur destructeur
  Strategy_player(const string &nom);
  Strategy_player(const json data)
      : nb_points(0), nb_cartes_j(0), nb_cartes_r(0), nb_courones(0),
        nb_privileges(0), nom(data["nom"]), nb_jetons(0),
        nb_cartes_j_reservees(0), games_won(data["games_won"]),
        games(data["games"]) {}

  virtual ~Strategy_player();

  // méthodes virtuelles pures
  virtual void choice() = 0;
  virtual void choice_qt() = 0;
  virtual void utilisationPrivilege() = 0;
  virtual void utilisationPrivilege_qt() = 0;
  virtual void selection_jetons() = 0;
  virtual void achat_carte() = 0;
  virtual void achat_carte_qt() = 0;
  virtual void buyCard(Tirage *t, const int indice) = 0;
  virtual void buyCard_qt(Tirage *t, const int indice) = 0;
  virtual void buyCardFromReserve(const int indice) = 0;
  virtual void buyCardFromReserve_qt(const int indice) = 0;
  virtual void applicationCapacite(const JewelryCard &carte,
                                   Strategy_player &adversaire) = 0;
  virtual void applicationCapacite_qt(const JewelryCard &carte,
                                      Strategy_player &adversaire) = 0;
  virtual void applicationCapaciteRoyale(const RoyalCard &carte,
                                         Strategy_player &adversaire) = 0;
  virtual void applicationCapaciteRoyale_qt(const RoyalCard &carte,
                                            Strategy_player &adversaire) = 0;
  virtual void reservation_carte() = 0;
  virtual void reservation_carte_qt() = 0;
  virtual void selectionRoyalCard() = 0;
  virtual void selectionRoyalCard_qt() = 0;
  virtual void verifJetons() = 0;
  virtual void verifJetons_qt() = 0;

  // getters setters
  const vector<const JewelryCard *> &getCartesBought() const {
    return cartes_joaillerie_achetees;
  }
  const int getNbCartesJoaillerie() const { return nb_cartes_j; }
  const int getNbJetons() const { return nb_jetons; }
  const string getName() const { return nom; }
  void setName(string &s) { nom = s; }
  const int getNbCartesAchetees() const {
    return cartes_joaillerie_achetees.size();
  }
  const int getNbCartesReservees() const {
    return cartes_joaiellerie_reservees.size();
  }
  const int getNbPoints() const { return nb_points; }
  void setPoints(int nb) { nb_points = nb; }
  const int getNbCouronnes() const { return nb_courones; }
  void setNbCouronnes(int nb) { nb_courones = nb; }
  const int getNbPrivileges() const { return nb_privileges; }
  void setNbPrivileges(int nb) {
    if (nb_privileges > max_nb_privileges) {
      throw SplendorException("fichier de chargement corrompu -1");
    }
    nb_privileges = nb;
  }
  const int getNbCartesRoyales() const { return nb_cartes_r; }
  vector<const JewelryCard *> &getCartesReserved() {
    return cartes_joaiellerie_reservees;
  }
  void increment_carte_royale() { nb_cartes_r = nb_cartes_r + 1; }

  void setIa(unsigned int nb) {
    if (nb == 0) {
      is_ia = 0;
    } else {
      is_ia = 1;
    }
  }

  unsigned int getIa() const { return is_ia; }

  void setWins(unsigned int nb) { games_won = nb; }
  void setPlayed(unsigned int nb) { games = nb; }

  const unsigned int getWins() const { return games_won; }
  const unsigned int getPlayed() const { return games; }

  // setters rajoutés pour les besoins du JSON
  void setJewellryCardReserved(vector<const JewelryCard *> j) {
    if (nb_cartes_j_reservees < j.size()) {
      throw SplendorException("Fichier de sauvegarde corrompu -2");
    }
    cartes_joaiellerie_reservees = j;
  }
  void setJewellryCard(vector<const JewelryCard *> j) {
    if (nb_cartes_j < j.size()) {
      throw SplendorException("Fichier de sauvegarde corrompu -3");
    }
    cartes_joaillerie_achetees = j;
  }
  void setJetons(vector<const Jeton *> j) {
    /*if(j.size() > max_nb_jetons){
        throw SplendorException("Fichier de sauvegarde corrompu -4");
    }*/
    jetons = j;
  }
  void setRoyalCard(vector<const RoyalCard *> r) {
    if (nb_cartes_r < r.size()) {
      throw SplendorException("Fichier de sauvegarde corrompu 5");
    }
    cartes_royale = r;
  }
  void setPrivileges(vector<const Privilege *> p) {
    if (nb_privileges < p.size()) {
      throw SplendorException("Fichier de sauvegarde corrompu 6");
    }
    privileges = p;
  }
  void setNbJetons(unsigned int nb) {
    if (nb > max_nb_jetons) {
      throw SplendorException("fichier de chargement corrompu 7");
    }
    nb_jetons = nb;
  }
  void setNbJCards(unsigned int nb) { nb_cartes_j = nb; }

  void setNbJCardsReserved(unsigned int nb) {
    if (nb > max_nb_cartes_reservees) {
      throw SplendorException("Fichier de chargement corrompu 8");
    }
    nb_cartes_j_reservees = nb;
  }

  void setNbRCards(unsigned int nb) {
    if (nb > max_nb_cartes_r) {
      throw SplendorException("Fichier de chargement corrompu 9");
    }
    nb_cartes_r = nb;
  }

  vector<const Jeton *> &getJeton() { return jetons; }

  vector<const Privilege *> &get_privilege() { return privileges; }

  // méthode utilitaires aux classes filles

  int calculateBonus(enum colorBonus bonus);
  int nbJeton(const Color &couleur) const;
  void withdrawJetons(const Color &c, int val);
  void reserver_carte(Tirage *t, const int indice);
  void reserver_carte(Pioche *p);
  void piocher_jeton(int i);
  void obtainRoyaleCard(unsigned int i);
  void obtainRoyaleCard_qt(unsigned int i);
  bool royaleCardEligibility();
  bool jokerCardEligibility();
  void obtainPrivilege();
  void retirerPrivilege();
  void remplissagePlateau();
  bool
  onlyGoldInJetons() { // true si le joueur n'a que des jetons or, false sinon
    for (auto jet : jetons) {
      if (jet != nullptr and jet->getColor() != Color::gold)
        return false;
    }
    return true;
  }

  // méthode utilitaire pour le main
  bool victoryConditions(); // si le joueur rempli une des trois conditions de
                            // victoire, renvoie true
  void print_player();
  int getOptionalChoices();
  void retirerJetonBlanc();
  void retirerJetonPerle();
};

class Joueur : public Strategy_player {

public:
  // Constructeur et destructeur
  Joueur(const string &nom);
  Joueur(const json data);
  ~Joueur();

  // Méthodes polymorphiques adaptées pour un joueur
  void choice();
  void choice_qt();
  void utilisationPrivilege();
  void utilisationPrivilege_qt();
  void selection_jetons();
  void selection_jetons_qt();
  void applicationCapacite(const JewelryCard &carte,
                           Strategy_player &adversaire);
  void applicationCapacite_qt(const JewelryCard &carte,
                              Strategy_player &adversaire);
  void applicationCapaciteRoyale(const RoyalCard &carte,
                                 Strategy_player &adversaire);
  void applicationCapaciteRoyale_qt(const RoyalCard &carte,
                                    Strategy_player &adversaire);
  void achat_carte();
  void achat_carte_qt();
  void buyCard(Tirage *t, const int indice);
  void buyCard_qt(Tirage *t, const int indice);
  void buyCardFromReserve(const int indice);
  void buyCardFromReserve_qt(const int indice);
  void reservation_carte();
  void reservation_carte_qt();
  void selectionRoyalCard();
  void selectionRoyalCard_qt();
  void verifJetons();
  void verifJetons_qt();
};

class IA : public Strategy_player {
public:
  IA(const string &nom = "IA");
  IA(const json data);
  ~IA();

  // Méthodes polymorphiques adaptées pour une IA
  void choice();
  void choice_qt();
  void utilisationPrivilege();
  void utilisationPrivilege_qt() { utilisationPrivilege(); }
  void selection_jetons();
  void achat_carte();
  void achat_carte_qt() { achat_carte(); }
  void buyCard(Tirage *t, const int indice);
  void buyCard_qt(Tirage *t, const int indice) { buyCard(t, indice); }
  void buyCardFromReserve(const int indice);
  void buyCardFromReserve_qt(const int indice) { buyCardFromReserve(indice); }
  void reservation_carte();
  void reservation_carte_qt() { reservation_carte(); }
  void selectionRoyalCard();
  void selectionRoyalCard_qt() { selectionRoyalCard(); }
  void applicationCapacite(const JewelryCard &carte,
                           Strategy_player &adversaire);
  void applicationCapacite_qt(const JewelryCard &carte,
                              Strategy_player &adversaire) {
    applicationCapacite(carte, adversaire);
  }
  void applicationCapaciteRoyale(const RoyalCard &carte,
                                 Strategy_player &adversaire);
  void applicationCapaciteRoyale_qt(const RoyalCard &carte,
                                    Strategy_player &adversaire) {
    applicationCapaciteRoyale(carte, adversaire);
  }
  void verifJetons();
  void verifJetons_qt() { verifJetons(); }
};

/**************** Fonctions utilitaires ****************/
inline std::ostream &operator<<(std::ostream &os, const Strategy_player &j) {
  os << "Pseudo : " << j.getName();
  if (j.getNbPoints() != 0) {
    os << "nombre de points : " << j.getNbPoints();
  }
  if (j.getNbCouronnes() != 0) {
    os << " nombre de couronnes : " << j.getNbCouronnes();
  }
  if (j.getNbCartesReservees() != 0) {
    os << " nombre de cartes réservées : " << j.getNbCartesReservees();
  }
  if (j.getNbPrivileges() != 0) {
    os << " nombre de privilège : " << j.getNbPrivileges();
  }
  if (j.getNbJetons() != 0) {
    os << " nombre de jetons : " << j.getNbJetons();
  }
  os << endl;
  return os;
}

int positiveOrNull(int x);

void testJoueurs();

#endif // LO21_SPLENDOR_DUEL_JOUEUR_H
