#include "joker_popup.qt.h"
#include "game.h"
#include "main_window.qt.h"
#include <QLabel>
#include <string>

popupJoker::popupJoker(QWidget *parent) : QDialog(parent) {

	QLabel *typeLabel = new QLabel("Couleur :", this);
	comboBox = new QComboBox(this);

	int bonus_blanc =
	    Game::getGame().getCurrentPlayer().calculateBonus(colorBonus::white);
	int bonus_bleu =
	    Game::getGame().getCurrentPlayer().calculateBonus(colorBonus::blue);
	int bonus_rouge =
	    Game::getGame().getCurrentPlayer().calculateBonus(colorBonus::red);
	int bonus_vert =
	    Game::getGame().getCurrentPlayer().calculateBonus(colorBonus::green);
	int bonus_noir =
	    Game::getGame().getCurrentPlayer().calculateBonus(colorBonus::black);

	if (bonus_blanc > 0) {
		comboBox->addItem("White");
	}
	if (bonus_bleu > 0) {
		comboBox->addItem("Blue");
	}
	if (bonus_rouge > 0) {
		comboBox->addItem("Red");
	}
	if (bonus_vert > 0) {
		comboBox->addItem("Green");
	}
	if (bonus_noir > 0) {
		comboBox->addItem("Black");
	}

	submitButton = new QPushButton("Submit", this);

	QVBoxLayout *layout = new QVBoxLayout(this);
	layout->addWidget(typeLabel);
	layout->addWidget(comboBox);

	setWindowTitle("Choix couleur");

	connect(submitButton, &QPushButton::clicked, this,
	        &popupJoker::onSubmitClicked);
}

void popupJoker::onSubmitClicked() {
	std::string coul = comboBox->currentText().toStdString();
	setColor(stringToBonus(coul));

	accept();
}
