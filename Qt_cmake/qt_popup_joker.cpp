#include "qt_popup_joker.h".h "
#include "../classes/jeu.h"
#include "mainwindow.h"
#include <QLabel>
#include <string>

popupJoker::popupJoker(QWidget *parent) : QDialog(parent) {

  QLabel *typeLabel = new QLabel("Couleur :", this);
  comboBox = new QComboBox(this);

  int bonus_blanc =
      Jeu::getJeu().getCurrentPlayer().calculateBonus(colorBonus::blanc);
  int bonus_bleu =
      Jeu::getJeu().getCurrentPlayer().calculateBonus(colorBonus::bleu);
  int bonus_rouge =
      Jeu::getJeu().getCurrentPlayer().calculateBonus(colorBonus::red);
  int bonus_vert =
      Jeu::getJeu().getCurrentPlayer().calculateBonus(colorBonus::vert);
  int bonus_noir =
      Jeu::getJeu().getCurrentPlayer().calculateBonus(colorBonus::noir);

  if (bonus_blanc > 0) {
    comboBox->addItem("Blanc");
  }
  if (bonus_bleu > 0) {
    comboBox->addItem("Bleu");
  }
  if (bonus_rouge > 0) {
    comboBox->addItem("Rouge");
  }
  if (bonus_vert > 0) {
    comboBox->addItem("Vert");
  }
  if (bonus_noir > 0) {
    comboBox->addItem("Noir");
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
