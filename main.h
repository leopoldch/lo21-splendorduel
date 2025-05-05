#ifndef MAIN_H
#define MAIN_H

#ifndef LO21_SPLENDOR_DUEL_MAIN_H
#define LO21_SPLENDOR_DUEL_MAIN_H

#include "Qt_cmake/mainwindow.h"
#include "classes/history.h"
#include <QApplication>
#include <QThread>
#include <iostream>

void toJson() {
  json j = Jeu::getJeu().toJson();
  std::string s = j.dump(2);
  std::ofstream file("../src/backup.json");
  file << s;
}

void gameFromScratch(int argc, char *argv[]) {

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

  // Init le jeu
  Jeu::getJeu();

  MainWindow::getMainWindow().show();
  Qt_Plateau::getPlateau().connectJetons();
  MainWindow::getMainWindow().getTirages()->connectCartes();

  // Setup des noms

  bool check_names = true;
  while (check_names) {
    try {
      MainWindow::getMainWindow().demanderNoms();
      check_names = false;
    } catch (SplendorException &e) {
      MainWindow::getMainWindow().triggerInfo(e.getInfos());
    }
  }

  MainWindow::getMainWindow().setTopPlayerName(
      QString::fromStdString(Jeu::getJeu().getCurrentPlayer().getName()));
  MainWindow::getMainWindow().setBottomPlayerName(
      QString::fromStdString(Jeu::getJeu().getOpponent().getName()));

  // Setup du plateau
  Jeu::getJeu().get_tour();
  MainWindow::getMainWindow().updatePlateau();
  MainWindow::getMainWindow().updateTirages();
  MainWindow::getMainWindow().updatePrivileges();

  unsigned int from_error = 0;

  while (!Jeu::getJeu().isFinished()) {

    if (from_error == 0) {
      Jeu::getJeu().get_tour();
      MainWindow::getMainWindow().setTopPlayerName(
          QString::fromStdString(Jeu::getJeu().getOpponent().getName()));
      MainWindow::getMainWindow().setBottomPlayerName(
          QString::fromStdString(Jeu::getJeu().getCurrentPlayer().getName()));
      // qDebug() << Jeu::getJeu().getCurrentPlayer().getName();
      MainWindow::getMainWindow().updateQuiJoue();

      try {
        toJson();
      } catch (SplendorException &e) {
        cout << e.getInfos() << endl;
        MainWindow::getMainWindow().triggerInfo(e.getInfos());
      }
    }
    try {
      // qDebug() << "DEBUT TRY";
      Jeu::getJeu().getCurrentPlayer().choice_qt();
      // update affichage

      Jeu::getJeu().getCurrentPlayer().verifJetons_qt();

      if (Jeu::getJeu().getCurrentPlayer().royaleCardEligibility() == 1) {
        Jeu::getJeu().getCurrentPlayer().selectionRoyalCard_qt();
      }

      Jeu::getJeu().tour_suivant();

      MainWindow::getMainWindow().updateScores();
      MainWindow::getMainWindow().updatePlateau();
      MainWindow::getMainWindow().updateTirages();
      MainWindow::getMainWindow().updatePrivileges();
      MainWindow::getMainWindow().update();

      QCoreApplication::processEvents();

      from_error = 0;

    } catch (SplendorException &e) {
      from_error = 1;
      cout << "============= ACTION NON AUTORISÉE =================" << endl;
      cout << e.getInfos() << endl;
    }
  }

  MainWindow::getMainWindow().updateScores();
  MainWindow::getMainWindow().updatePlateau();
  MainWindow::getMainWindow().updateTirages();
  MainWindow::getMainWindow().updatePrivileges();
  MainWindow::getMainWindow().update();

  cout << "=================== Partie terminée ===================" << endl;
  cout << "Nombre de manches : " << Jeu::getJeu().getManche() << endl;
  cout << "Stats du gagnant:" << endl;
  Jeu::getJeu().getCurrentPlayer().print_player();
  Jeu::getJeu().getCurrentPlayer().game_ended(1);
  Jeu::getJeu().getOpponent().game_ended(0);

  cout << "Stats du perdant : " << endl;
  Jeu::getJeu().getOpponent().print_player();

  try {
    Hist();
    toJson();
  } catch (SplendorException &e) {
    cout << e.getInfos() << endl;
  }

  MainWindow::getMainWindow().triggerInfo(
      "Bravo, " + Jeu::getJeu().getCurrentPlayer().getName() + " a gagné!");
}

void gameFromJson(int argc, char *argv[]) {

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

    if (data["est_termine"])
      throw SplendorException("Ce jeu est terminé!");

    Jeu::getJeu(data);

    cout << "cartes des joueurs : " << endl;
    cout << Jeu::getJeu().getCurrentPlayer().getNbCartesAchetees() << endl;
    cout << Jeu::getJeu().getOpponent().getNbCartesAchetees() << endl;

    cout << "cartes des tirages : " << endl;

    cout << Jeu::getJeu().get_tirage_1()->getNbCartes() << endl << endl;
    cout << Jeu::getJeu().get_tirage_2()->getNbCartes() << endl << endl;
    cout << Jeu::getJeu().get_tirage_3()->getNbCartes() << endl << endl;

    cout << "cartes royales : " << endl;
    cout << Jeu::getJeu().getCartesRoyales().size() << endl;

    cout << "Type des joueurs : " << endl;
    cout << Jeu::getJeu().getOpponent().getIa() << endl;
    cout << Jeu::getJeu().getCurrentPlayer().getIa() << endl;

  } catch (SplendorException &e) {
    cout << e.getInfos() << endl;
    MainWindow::getMainWindow().triggerInfo(
        e.getInfos() + "\nVous allez recommencer une partie de 0:");
    gameFromScratch(argc, argv);
  }

  srand(static_cast<unsigned>(std::time(nullptr)));

  MainWindow::getMainWindow().show();
  Qt_Plateau::getPlateau().connectJetons();
  MainWindow::getMainWindow().getTirages()->connectCartes();

  // Setup des noms
  MainWindow::getMainWindow().setTopPlayerName(
      QString::fromStdString(Jeu::getJeu().getCurrentPlayer().getName()));
  MainWindow::getMainWindow().setBottomPlayerName(
      QString::fromStdString(Jeu::getJeu().getOpponent().getName()));

  // Setup du plateau
  Jeu::getJeu().get_tour();
  MainWindow::getMainWindow().updatePlateau();
  MainWindow::getMainWindow().updateTirages();
  MainWindow::getMainWindow().updatePrivileges();

  cout << "cartes des joueurs& : " << endl;
  cout << Jeu::getJeu().getCurrentPlayer().getNbCartesAchetees() << endl;
  cout << Jeu::getJeu().getOpponent().getNbCartesAchetees() << endl;

  cout << "cartes des tirages : " << endl;

  cout << Jeu::getJeu().get_tirage_1()->getNbCartes() << endl << endl;
  cout << Jeu::getJeu().get_tirage_2()->getNbCartes() << endl << endl;
  cout << Jeu::getJeu().get_tirage_3()->getNbCartes() << endl << endl;

  unsigned int from_error = 0;

  while (!Jeu::getJeu().isFinished()) {

    if (from_error == 0) {
      Jeu::getJeu().get_tour();
      MainWindow::getMainWindow().setTopPlayerName(
          QString::fromStdString(Jeu::getJeu().getOpponent().getName()));
      MainWindow::getMainWindow().setBottomPlayerName(
          QString::fromStdString(Jeu::getJeu().getCurrentPlayer().getName()));
      // qDebug() << Jeu::getJeu().getCurrentPlayer().getName();
      MainWindow::getMainWindow().updateQuiJoue();

      /*
      cout<<"Etat des joueurs : "<<endl;
      Jeu::getJeu().getCurrentPlayer().print_player();
      cout<<"Bonus
      blanc"<<Jeu::getJeu().getCurrentPlayer().calculateBonus(colorBonus::blanc)<<endl;
      cout<<"Bonus
      vert"<<Jeu::getJeu().getCurrentPlayer().calculateBonus(colorBonus::vert)<<endl;
      cout<<"Bonus
      bleu"<<Jeu::getJeu().getCurrentPlayer().calculateBonus(colorBonus::bleu)<<endl;
      cout<<"Bonus
      rouge"<<Jeu::getJeu().getCurrentPlayer().calculateBonus(colorBonus::red)<<endl;
      cout<<"Bonus
      noir"<<Jeu::getJeu().getCurrentPlayer().calculateBonus(colorBonus::noir)<<endl;
      cout<<"Bonus
      joker"<<Jeu::getJeu().getCurrentPlayer().calculateBonus(colorBonus::joker)<<endl;
      cout<<endl;
      */

      // cout<<"Il y a "<<Sac::get_sac().get_nb_sac()<<" jetons dans le
      // sac."<<endl;

      /*
      cout<<"\n\nPlateau :"<<endl;
      Plateau::get_plateau().printTab();

      cout<<"\n\nTirage1 :"<<endl;
      cout<<*Jeu::getJeu().get_tirage_1()<<endl;
      cout<<"\nTirage2 :"<<endl;
      cout<<*Jeu::getJeu().get_tirage_2()<<endl;
      cout<<"\nTirage3 :"<<endl;
      cout<<*Jeu::getJeu().get_tirage_3()<<endl;
      */

      // cout<<"c'est à "<<Jeu::getJeu().get_tour().getName()<<" de jouer !
      // "<<endl; Faire un popup avec c'est à de jouer

      try {
        toJson();
      } catch (SplendorException &e) {
        cout << e.getInfos() << endl;
        MainWindow::getMainWindow().triggerInfo(e.getInfos());
      }
    }
    try {
      // qDebug() << "DEBUT TRY";
      Jeu::getJeu().getCurrentPlayer().choice_qt();
      // update affichage

      Jeu::getJeu().getCurrentPlayer().verifJetons_qt();

      if (Jeu::getJeu().getCurrentPlayer().royaleCardEligibility() == 1) {
        Jeu::getJeu().getCurrentPlayer().selectionRoyalCard_qt();
      }

      Jeu::getJeu().tour_suivant();

      MainWindow::getMainWindow().updateScores();
      MainWindow::getMainWindow().updatePlateau();
      MainWindow::getMainWindow().updateTirages();
      MainWindow::getMainWindow().updatePrivileges();
      MainWindow::getMainWindow().update();

      QCoreApplication::processEvents();

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
  MainWindow::getMainWindow().triggerInfo(
      "Bravo, " + Jeu::getJeu().getCurrentPlayer().getName() + " a gagné!");
}

#endif // LO21_SPLENDOR_DUEL_MAIN_H

#endif // MAIN_H
