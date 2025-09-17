#ifndef LO21_SPLENDOR_DUEL_MAIN_H
#define LO21_SPLENDOR_DUEL_MAIN_H

#include "classes/history.h"
#include <iostream>

void toJson() {
	json j = Game::getGame().toJson();
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

	Game::getGame();
	cout << "Privilèges dans le game:" << endl;

	Game::getGame().setPlayers();

	cout << "Le game est sur le point de commencer !\nC'est au joueur1 "
	        "d'engager "
	        "la partie !"
	     << endl;

	unsigned int from_error = 0;

	while (!Game::getGame().isFinished()) {

		if (from_error == 0) {
			Game::getGame().getPlayerRound();

			cout << "Etat des joueurs : " << endl;
			Game::getGame().getCurrentPlayer().printPlayer();
			cout << "Bonus white"
			     << Game::getGame().getCurrentPlayer().calculateBonus(
			            colorBonus::white)
			     << endl;
			cout << "Bonus green"
			     << Game::getGame().getCurrentPlayer().calculateBonus(
			            colorBonus::green)
			     << endl;
			cout << "Bonus blue"
			     << Game::getGame().getCurrentPlayer().calculateBonus(
			            colorBonus::blue)
			     << endl;
			cout << "Bonus red"
			     << Game::getGame().getCurrentPlayer().calculateBonus(
			            colorBonus::red)
			     << endl;
			cout << "Bonus black"
			     << Game::getGame().getCurrentPlayer().calculateBonus(
			            colorBonus::black)
			     << endl;
			cout << "Bonus joker"
			     << Game::getGame().getCurrentPlayer().calculateBonus(
			            colorBonus::joker)
			     << endl;
			cout << endl;

			cout << "Il y a " << Bag::get().getTokenNumber()
			     << " tokens dans le bag." << endl;

			cout << "\n\nPlateau :" << endl;
			Board::getBoard().printArray();

			cout << "\n\nTirage1 :" << endl;
			cout << *Game::getGame().getFirstDraw() << endl;
			cout << "\nTirage2 :" << endl;
			cout << *Game::getGame().getSecondDraw() << endl;
			cout << "\nTirage3 :" << endl;
			cout << *Game::getGame().getThirdDraw() << endl;

			cout << "c'est à " << Game::getGame().getPlayerRound().getName()
			     << " de jouer ! " << endl;

			try {
				toJson();
			} catch (SplendorException &e) {
				cout << e.getInfo() << endl;
			}
		}
		try {

			Game::getGame().getCurrentPlayer().choice();
			Game::getGame().getCurrentPlayer().tokenVerification();
			if (Game::getGame().getCurrentPlayer().royalCardEligibility() ==
			    1) {
				Game::getGame().getCurrentPlayer().royalCardSelection();
			}
			Game::getGame().nextRound();
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
	// attention si won != 0 alors il a gagné sinon non
	Game::getGame().getCurrentPlayer().gameEnded(1);
	Game::getGame().getOpponent().gameEnded(0);

	try {
		Hist();
		toJson();
	} catch (SplendorException &e) {
		cout << e.getInfo() << endl;
	}

	Game::free();
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

		// si maximum de cards atteint alors cards générées en trop.
		// mauvaise gestion des cards !

		if (data["is_finished"]) {
			cout << "vous ne pouvez pas reprendre une partie terminée !"
			     << endl;
			return;
		}

		Game::getGame(data);

	} catch (SplendorException &e) {
		cout << e.getInfo() << endl;
	}

	unsigned int from_error = 0;

	while (!Game::getGame().isFinished()) {

		if (from_error == 0) {
			Game::getGame().getPlayerRound();

			cout << "Etat des joueurs : " << endl;
			Game::getGame().getCurrentPlayer().printPlayer();
			cout << "Bonus white"
			     << Game::getGame().getCurrentPlayer().calculateBonus(
			            colorBonus::white)
			     << endl;
			cout << "Bonus green"
			     << Game::getGame().getCurrentPlayer().calculateBonus(
			            colorBonus::green)
			     << endl;
			cout << "Bonus blue"
			     << Game::getGame().getCurrentPlayer().calculateBonus(
			            colorBonus::blue)
			     << endl;
			cout << "Bonus red"
			     << Game::getGame().getCurrentPlayer().calculateBonus(
			            colorBonus::red)
			     << endl;
			cout << "Bonus black"
			     << Game::getGame().getCurrentPlayer().calculateBonus(
			            colorBonus::black)
			     << endl;
			cout << "Bonus joker"
			     << Game::getGame().getCurrentPlayer().calculateBonus(
			            colorBonus::joker)
			     << endl;
			cout << endl;

			cout << "Il y a " << Bag::get().getTokenNumber()
			     << " tokens dans le bag." << endl;

			cout << "\n\nPlateau :" << endl;
			Board::getBoard().printArray();

			cout << "\n\nTirage1 :" << endl;
			cout << *Game::getGame().getFirstDraw() << endl;
			cout << "\nTirage2 :" << endl;
			cout << *Game::getGame().getSecondDraw() << endl;
			cout << "\nTirage3 :" << endl;
			cout << *Game::getGame().getThirdDraw() << endl;

			cout << "c'est à " << Game::getGame().getPlayerRound().getName()
			     << " de jouer ! " << endl;

			try {
				toJson();
			} catch (SplendorException &e) {
				cout << e.getInfo() << endl;
			}
		}
		try {

			Game::getGame().getCurrentPlayer().choice();
			Game::getGame().getCurrentPlayer().tokenVerification();
			if (Game::getGame().getCurrentPlayer().royalCardEligibility() ==
			    1) {
				Game::getGame().getCurrentPlayer().royalCardSelection();
			}
			Game::getGame().nextRound();
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

	Game::free();
}

#endif // LO21_SPLENDOR_DUEL_MAIN_H
