#include "qt_popup_couleur.h"
#include "../classes/game.h"
#include "mainwindow.h"
#include <QLabel>
#include <string>

popupCouleur::popupCouleur(QWidget *parent) : QDialog(parent) {

	QLabel *typeLabel = new QLabel("Couleur :", this);
	comboBox = new QComboBox(this);

	comboBox->addItem("White");
	comboBox->addItem("Blue");
	comboBox->addItem("Black");
	comboBox->addItem("Perle");
	comboBox->addItem("Red");
	comboBox->addItem("Green");

	QLabel *nombreLabel = new QLabel("Nombre :", this);
	comboBoxNb = new QComboBox(this);
	for (int i = 1; i <= Game::getGame().getCurrentPlayer().TokenAmount(Color::gold);
	     i++) {
		comboBoxNb->addItem(QString::fromStdString(std::to_string(i)));
	}

	submitButton = new QPushButton("Submit", this);

	QVBoxLayout *layout = new QVBoxLayout(this);
	layout->addWidget(typeLabel);
	layout->addWidget(comboBoxNb);
	layout->addWidget(comboBox);

	setWindowTitle("Choix couleur");

	connect(submitButton, &QPushButton::clicked, this,
	        &popupCouleur::onSubmitClicked);
}

void popupCouleur::onSubmitClicked() {
	std::string nb_string = comboBoxNb->currentText().toStdString();
	std::string coul = comboBox->currentText().toStdString();

	setColor(stringToColor(coul));
	setNb(std::stoi(nb_string));

	accept();
}
