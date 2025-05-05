#ifndef LO21_SPLENDOR_DUEL_MAIN_H
#define LO21_SPLENDOR_DUEL_MAIN_H

#include "classes/history.h"
#include <iostream>

void toJson() {
  json j = Jeu::getJeu().toJson();
  std::string s = j.dump(2);
  std::ofstream file("../src/backup.json");
  file << s;
}

void gameFromScratch() {
  srand(static_cast<unsigned>(std::time(nullptr)));

  try {
    std::ifstream file("../src/history.json");

    if (!file.is_open()) {
      std::cerr << "Failed to open the JSON file." << std::endl;
      throw SplendorException("Fichier non ouvert");
    }
    json hist;
    file >> hist;
    file.close();
    History::getHistory().initHistory(hist);
  } catch (SplendorException &e) {
    cout << " Historique non ouvert " << endl;
  }

  Jeu::getJeu();
  cout << "Privilèges dans le jeu:" << endl;

  Jeu::getJeu().setPlayers();

  cout << "Le jeu est sur le point de commencer !\nC'est au joueur1 d'engager "
          "la partie !"
       << endl;

  unsigned int from_error = 0;

  while (!Jeu::getJeu().isFinished()) {

    if (from_error == 0) {
      Jeu::getJeu().get_tour();

      cout << "Etat des joueurs : " << endl;
      Jeu::getJeu().getCurrentPlayer().print_player();
      cout << "Bonus blanc"
           << Jeu::getJeu().getCurrentPlayer().calculateBonus(colorBonus::blanc)
           << endl;
      cout << "Bonus vert"
           << Jeu::getJeu().getCurrentPlayer().calculateBonus(colorBonus::vert)
           << endl;
      cout << "Bonus bleu"
           << Jeu::getJeu().getCurrentPlayer().calculateBonus(colorBonus::bleu)
           << endl;
      cout << "Bonus rouge"
           << Jeu::getJeu().getCurrentPlayer().calculateBonus(colorBonus::red)
           << endl;
      cout << "Bonus noir"
           << Jeu::getJeu().getCurrentPlayer().calculateBonus(colorBonus::noir)
           << endl;
      cout << "Bonus joker"
           << Jeu::getJeu().getCurrentPlayer().calculateBonus(colorBonus::joker)
           << endl;
      cout << endl;

      cout << "Il y a " << Sac::get_sac().get_nb_sac() << " jetons dans le sac."
           << endl;

      cout << "\n\nPlateau :" << endl;
      Plateau::get_plateau().printTab();

      cout << "\n\nTirage1 :" << endl;
      cout << *Jeu::getJeu().get_tirage_1() << endl;
      cout << "\nTirage2 :" << endl;
      cout << *Jeu::getJeu().get_tirage_2() << endl;
      cout << "\nTirage3 :" << endl;
      cout << *Jeu::getJeu().get_tirage_3() << endl;

      cout << "c'est à " << Jeu::getJeu().get_tour().getName() << " de jouer ! "
           << endl;

      try {
        toJson();
      } catch (SplendorException &e) {
        cout << e.getInfos() << endl;
      }
    }
    try {

      Jeu::getJeu().getCurrentPlayer().choice();
      Jeu::getJeu().getCurrentPlayer().verifJetons();
      if (Jeu::getJeu().getCurrentPlayer().royaleCardEligibility() == 1) {
        Jeu::getJeu().getCurrentPlayer().selectionRoyalCard();
      }
      Jeu::getJeu().tour_suivant();
      from_error = 0;

    } catch (SplendorException &e) {
      from_error = 1;
      cout << "============= ACTION NON AUTORISÉE =================" << endl;
      cout << e.getInfos() << endl;
    }
  }

  cout << "=================== Partie terminée ===================" << endl;
  cout << "Nombre de manches : " << Jeu::getJeu().getManche() << endl;
  cout << "Stats du gagnant:" << endl;
  Jeu::getJeu().getCurrentPlayer().print_player();
  // attention si won != 0 alors il a gagné sinon non
  Jeu::getJeu().getCurrentPlayer().game_ended(1);
  Jeu::getJeu().getOpponent().game_ended(0);

  try {
    Hist();
    toJson();
  } catch (SplendorException &e) {
    cout << e.getInfos() << endl;
  }

  Jeu::libereJeu();
}

void gameFromJson() {

  try {
    std::ifstream file("../src/history.json");

    if (!file.is_open()) {
      std::cerr << "Failed to open the JSON file." << std::endl;
      throw SplendorException("Fichier non ouvert");
    }
    json hist;
    file >> hist;
    file.close();
    History::getHistory().initHistory(hist);
  } catch (SplendorException &e) {
    cout << " Historique non ouvert " << endl;
  }

  try {
    std::ifstream file("../src/backup.json");

    if (!file.is_open()) {
      std::cerr << "Failed to open the JSON file." << std::endl;
      throw SplendorException("Fichier non ouvert");
    }

    json data;
    file >> data;
    file.close();

    // si maximum de cartes atteint alors cartes générées en trop.
    // mauvaise gestion des cartes !

    if (data["est_termine"]) {
      cout << "vous ne pouvez pas reprendre une partie terminée !" << endl;
      return;
    }

    Jeu::getJeu(data);

  } catch (SplendorException &e) {
    cout << e.getInfos() << endl;
  }

  unsigned int from_error = 0;

  while (!Jeu::getJeu().isFinished()) {

    if (from_error == 0) {
      Jeu::getJeu().get_tour();

      cout << "Etat des joueurs : " << endl;
      Jeu::getJeu().getCurrentPlayer().print_player();
      cout << "Bonus blanc"
           << Jeu::getJeu().getCurrentPlayer().calculateBonus(colorBonus::blanc)
           << endl;
      cout << "Bonus vert"
           << Jeu::getJeu().getCurrentPlayer().calculateBonus(colorBonus::vert)
           << endl;
      cout << "Bonus bleu"
           << Jeu::getJeu().getCurrentPlayer().calculateBonus(colorBonus::bleu)
           << endl;
      cout << "Bonus rouge"
           << Jeu::getJeu().getCurrentPlayer().calculateBonus(colorBonus::red)
           << endl;
      cout << "Bonus noir"
           << Jeu::getJeu().getCurrentPlayer().calculateBonus(colorBonus::noir)
           << endl;
      cout << "Bonus joker"
           << Jeu::getJeu().getCurrentPlayer().calculateBonus(colorBonus::joker)
           << endl;
      cout << endl;

      cout << "Il y a " << Sac::get_sac().get_nb_sac() << " jetons dans le sac."
           << endl;

      cout << "\n\nPlateau :" << endl;
      Plateau::get_plateau().printTab();

      cout << "\n\nTirage1 :" << endl;
      cout << *Jeu::getJeu().get_tirage_1() << endl;
      cout << "\nTirage2 :" << endl;
      cout << *Jeu::getJeu().get_tirage_2() << endl;
      cout << "\nTirage3 :" << endl;
      cout << *Jeu::getJeu().get_tirage_3() << endl;

      cout << "c'est à " << Jeu::getJeu().get_tour().getName() << " de jouer ! "
           << endl;

      try {
        toJson();
      } catch (SplendorException &e) {
        cout << e.getInfos() << endl;
      }
    }
    try {

      Jeu::getJeu().getCurrentPlayer().choice();
      Jeu::getJeu().getCurrentPlayer().verifJetons();
      if (Jeu::getJeu().getCurrentPlayer().royaleCardEligibility() == 1) {
        Jeu::getJeu().getCurrentPlayer().selectionRoyalCard();
      }
      Jeu::getJeu().tour_suivant();
      from_error = 0;

    } catch (SplendorException &e) {
      from_error = 1;
      cout << "============= ACTION NON AUTORISÉE =================" << endl;
      cout << e.getInfos() << endl;
    }
  }

  cout << "=================== Partie terminée ===================" << endl;
  cout << "Nombre de manches : " << Jeu::getJeu().getManche() << endl;
  cout << "Stats du gagnant:" << endl;
  Jeu::getJeu().getCurrentPlayer().print_player();
  Jeu::getJeu().getCurrentPlayer().game_ended(1);
  Jeu::getJeu().getOpponent().game_ended(0);

  try {
    Hist();
    toJson();
  } catch (SplendorException &e) {
    cout << e.getInfos() << endl;
  }

  Jeu::libereJeu();
}

#endif // LO21_SPLENDOR_DUEL_MAIN_H
