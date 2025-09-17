#ifndef MAIN_H
#define MAIN_H

#ifndef LO21_SPLENDOR_DUEL_MAIN_H
#define LO21_SPLENDOR_DUEL_MAIN_H

#include "main_window.qt.h"
#include "history.h"
#include <QApplication>
#include <QThread>
#include <iostream>

void toJson() {
	json j = Game::getGame().toJson();
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

	// Init le game
	Game::getGame();

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
			MainWindow::getMainWindow().triggerInfo(e.getInfo());
		}
	}

	MainWindow::getMainWindow().setTopPlayerName(
	    QString::fromStdString(Game::getGame().getCurrentPlayer().getName()));
	MainWindow::getMainWindow().setBottomPlayerName(
	    QString::fromStdString(Game::getGame().getOpponent().getName()));

	// Setup du board
	Game::getGame().getPlayerRound();
	MainWindow::getMainWindow().updateBoard();
	MainWindow::getMainWindow().updateDraws();
	MainWindow::getMainWindow().updatePrivileges();

	unsigned int from_error = 0;

	while (!Game::getGame().isFinished()) {

		if (from_error == 0) {
			Game::getGame().getPlayerRound();
			MainWindow::getMainWindow().setTopPlayerName(QString::fromStdString(
			    Game::getGame().getOpponent().getName()));
			MainWindow::getMainWindow().setBottomPlayerName(
			    QString::fromStdString(
			        Game::getGame().getCurrentPlayer().getName()));
			// qDebug() << Game::getGame().getCurrentPlayer().getName();
			MainWindow::getMainWindow().updateQuiJoue();

			try {
				toJson();
			} catch (SplendorException &e) {
				cout << e.getInfo() << endl;
				MainWindow::getMainWindow().triggerInfo(e.getInfo());
			}
		}
		try {
			// qDebug() << "DEBUT TRY";
			Game::getGame().getCurrentPlayer().choice_Qt();
			// update affichage

			Game::getGame().getCurrentPlayer().tokenVerification_Qt();

			if (Game::getGame().getCurrentPlayer().royalCardEligibility() ==
			    1) {
				Game::getGame().getCurrentPlayer().royalCardSelection_Qt();
			}

			Game::getGame().nextRound();

			MainWindow::getMainWindow().updateScores();
			MainWindow::getMainWindow().updateBoard();
			MainWindow::getMainWindow().updateDraws();
			MainWindow::getMainWindow().updatePrivileges();
			MainWindow::getMainWindow().update();

			QCoreApplication::processEvents();

			from_error = 0;

		} catch (SplendorException &e) {
			from_error = 1;
			cout << "============= ACTION NON AUTORISÉE ================="
			     << endl;
			cout << e.getInfo() << endl;
		}
	}

	MainWindow::getMainWindow().updateScores();
	MainWindow::getMainWindow().updateBoard();
	MainWindow::getMainWindow().updateDraws();
	MainWindow::getMainWindow().updatePrivileges();
	MainWindow::getMainWindow().update();

	cout << "=================== Partie terminée ===================" << endl;
	cout << "Nombre de manches : " << Game::getGame().getRoundCount() << endl;
	cout << "Stats du winner:" << endl;
	Game::getGame().getCurrentPlayer().printPlayer();
	Game::getGame().getCurrentPlayer().gameEnded(1);
	Game::getGame().getOpponent().gameEnded(0);

	cout << "Stats du perdant : " << endl;
	Game::getGame().getOpponent().printPlayer();

	try {
		Hist();
		toJson();
	} catch (SplendorException &e) {
		cout << e.getInfo() << endl;
	}

	MainWindow::getMainWindow().triggerInfo(
	    "Bravo, " + Game::getGame().getCurrentPlayer().getName() + " a gagné!");
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

		// si maximum de cards atteint alors cards générées en trop.
		// mauvaise gestion des cards !

		if (data["is_finished"])
			throw SplendorException("Ce game est terminé!");

		Game::getGame(data);

		cout << "cards des joueurs : " << endl;
		cout << Game::getGame().getCurrentPlayer().getBoughtCardNumber()
		     << endl;
		cout << Game::getGame().getOpponent().getBoughtCardNumber() << endl;

		cout << "cards des tirages : " << endl;

		cout << Game::getGame().getFirstDraw()->getCardsNumber() << endl
		     << endl;
		cout << Game::getGame().getSecondDraw()->getCardsNumber() << endl
		     << endl;
		cout << Game::getGame().getThirdDraw()->getCardsNumber() << endl
		     << endl;

		cout << "cards royales : " << endl;
		cout << Game::getGame().getRoyalCards().size() << endl;

		cout << "Type des joueurs : " << endl;
		cout << Game::getGame().getOpponent().getRandomPlayer() << endl;
		cout << Game::getGame().getCurrentPlayer().getRandomPlayer() << endl;

	} catch (SplendorException &e) {
		cout << e.getInfo() << endl;
		MainWindow::getMainWindow().triggerInfo(
		    e.getInfo() + "\nVous allez recommencer une partie de 0:");
		gameFromScratch(argc, argv);
	}

	srand(static_cast<unsigned>(std::time(nullptr)));

	MainWindow::getMainWindow().show();
	Qt_Plateau::getPlateau().connectJetons();
	MainWindow::getMainWindow().getTirages()->connectCartes();

	// Setup des noms
	MainWindow::getMainWindow().setTopPlayerName(
	    QString::fromStdString(Game::getGame().getCurrentPlayer().getName()));
	MainWindow::getMainWindow().setBottomPlayerName(
	    QString::fromStdString(Game::getGame().getOpponent().getName()));

	// Setup du board
	Game::getGame().getPlayerRound();
	MainWindow::getMainWindow().updateBoard();
	MainWindow::getMainWindow().updateDraws();
	MainWindow::getMainWindow().updatePrivileges();

	cout << "cards des joueurs& : " << endl;
	cout << Game::getGame().getCurrentPlayer().getBoughtCardNumber() << endl;
	cout << Game::getGame().getOpponent().getBoughtCardNumber() << endl;

	cout << "cards des tirages : " << endl;

	cout << Game::getGame().getFirstDraw()->getCardsNumber() << endl << endl;
	cout << Game::getGame().getSecondDraw()->getCardsNumber() << endl << endl;
	cout << Game::getGame().getThirdDraw()->getCardsNumber() << endl << endl;

	unsigned int from_error = 0;

	while (!Game::getGame().isFinished()) {

		if (from_error == 0) {
			Game::getGame().getPlayerRound();
			MainWindow::getMainWindow().setTopPlayerName(QString::fromStdString(
			    Game::getGame().getOpponent().getName()));
			MainWindow::getMainWindow().setBottomPlayerName(
			    QString::fromStdString(
			        Game::getGame().getCurrentPlayer().getName()));
			MainWindow::getMainWindow().updateQuiJoue();

			try {
				toJson();
			} catch (SplendorException &e) {
				cout << e.getInfo() << endl;
				MainWindow::getMainWindow().triggerInfo(e.getInfo());
			}
		}
		try {
			Game::getGame().getCurrentPlayer().choice_Qt();

			Game::getGame().getCurrentPlayer().tokenVerification_Qt();

			if (Game::getGame().getCurrentPlayer().royalCardEligibility() ==
			    1) {
				Game::getGame().getCurrentPlayer().royalCardSelection_Qt();
			}

			Game::getGame().nextRound();

			MainWindow::getMainWindow().updateScores();
			MainWindow::getMainWindow().updateBoard();
			MainWindow::getMainWindow().updateDraws();
			MainWindow::getMainWindow().updatePrivileges();
			MainWindow::getMainWindow().update();

			QCoreApplication::processEvents();

			from_error = 0;

		} catch (SplendorException &e) {
			from_error = 1;
			cout << "============= ACTION NON AUTORISÉE ================="
			     << endl;
			cout << e.getInfo() << endl;
		}
	}

	cout << "=================== Partie terminée ===================" << endl;
	cout << "Nombre de manches : " << Game::getGame().getRoundCount() << endl;
	cout << "Stats du winner:" << endl;
	Game::getGame().getCurrentPlayer().printPlayer();
	Game::getGame().getCurrentPlayer().gameEnded(1);
	Game::getGame().getOpponent().gameEnded(0);

	try {
		Hist();
		toJson();
	} catch (SplendorException &e) {
		cout << e.getInfo() << endl;
	}
	MainWindow::getMainWindow().triggerInfo(
	    "Bravo, " + Game::getGame().getCurrentPlayer().getName() + " a gagné!");
}

#endif // LO21_SPLENDOR_DUEL_MAIN_H

#endif // MAIN_H
