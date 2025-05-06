#include "mainwindow.h"
#include "../classes/history.h"
#include "qt_popup_couleur.h"
#include "qt_popup_joker.h"
#include <QDialog>
#include <QScrollArea>
#include <math.h>
#include <sstream>

MainWindow::Handler MainWindow::handler;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), buyingCard(false), current_dialog(nullptr) {
  QLabel *label = new QLabel(this);
  QPixmap pixmap("/path/to/your/image.png");
  label->setPixmap(pixmap);
  label->show();

  topRoyal1 = new QLabel(this);
  topRoyal1->setStyleSheet("background: transparent;");
  topRoyal1->setFixedSize(50, 80);
  topRoyal1->setPixmap(QPixmap());
  topRoyal2 = new QLabel(this);
  topRoyal2->setStyleSheet("background: transparent;");
  topRoyal2->setFixedSize(50, 80);
  topRoyal2->setPixmap(QPixmap());

  bottomRoyal1 = new QLabel(this);
  bottomRoyal1->setStyleSheet("background: transparent;");
  bottomRoyal1->setFixedSize(50, 80);
  bottomRoyal1->setPixmap(QPixmap());
  bottomRoyal2 = new QLabel(this);
  bottomRoyal2->setStyleSheet("background: transparent;");
  bottomRoyal2->setFixedSize(50, 80);
  bottomRoyal2->setPixmap(QPixmap());

  topPrivileges = new QLabel(this);
  topPrivileges->setStyleSheet("QLabel { color: white;}");
  bottomPrivileges = new QLabel(this);
  bottomPrivileges->setStyleSheet("QLabel { color: white;}");

  jeu = &Jeu::getJeu();

  // Initialize player name labels
  topPlayerNameLabel = new QLabel("Player 1", this);
  bottomPlayerNameLabel = new QLabel("Player 2", this);
  topPlayerNameLabel->setStyleSheet("QLabel { color: white; background-color: "
                                    "white; border: 1px solid white; }");
  bottomPlayerNameLabel->setStyleSheet(
      "QLabel { color: white; background-color: white; border: 1px solid "
      "white; }");

  QWidget *centralWidget = new QWidget(this);

  // Background pour toute la fenêtre
  centralWidget->setStyleSheet(
      "background-image: url('../src/background.jpg'); background-position: "
      "center;");

  QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);
  mainLayout->setSpacing(16);

  quijoue = new QLabel("Qui joue", this);
  quijoue->setStyleSheet("QLabel { color: white; background-color: white; "
                         "border: 1px solid white; }");
  mainLayout->addWidget(quijoue, 0, Qt::AlignLeft);

  // ######## SCORE EN HAUT ET BOUTONS ########

  // Scores sur QLCD Display
  topScoreDisplay = new QLCDNumber(this);    // Display avec 2 digits
  bottomScoreDisplay = new QLCDNumber(this); // Display avec 2 digits

  // Afficher 0 pour l'instant
  topScoreDisplay->display(0);
  bottomScoreDisplay->display(0);

  // Les rajouter sur le layout principal
  QGridLayout *topPlayer = new QGridLayout;
  topPlayer->addWidget(topPlayerNameLabel, 0, 1, Qt::AlignCenter);
  topPlayer->addWidget(topScoreDisplay, 1, 1, Qt::AlignCenter);

  QHBoxLayout *topRoyalLayout = new QHBoxLayout();
  topRoyalLayout->addWidget(topRoyal1);
  topRoyalLayout->addWidget(topRoyal2);
  topRoyalLayout->addWidget(topPrivileges);

  QWidget *royalWidget = new QWidget();
  royalWidget->setLayout(topRoyalLayout);

  topPlayer->addWidget(royalWidget, 0, 2, 2, 1, Qt::AlignRight);

  topPlayer->setColumnStretch(0, 1);
  topPlayer->setColumnStretch(1, 1);
  topPlayer->setColumnStretch(2, 1);

  mainLayout->addLayout(topPlayer);

  // Boutons du haut (cartes et jetons)
  viewCardsButtonTop = new QPushButton("Voir cartes achetées", this);
  viewJetonsButtonTop = new QPushButton("Voir jetons", this);
  viewReservedCardsButtonTop = new QPushButton("Voir cartes réservées", this);

  // Faire le texte blanc
  viewCardsButtonTop->setStyleSheet("color: rgba(255, 255, 255, 255);");
  viewJetonsButtonTop->setStyleSheet("color: rgba(255, 255, 255, 255);");
  viewReservedCardsButtonTop->setStyleSheet("color: rgba(255, 255, 255, 255);");

  // Connecter aux fonctionnalités
  connect(viewCardsButtonTop, &QPushButton::clicked, this,
          &MainWindow::showBoughtCardsTop);
  connect(viewJetonsButtonTop, &QPushButton::clicked, this,
          &MainWindow::showJetonsTop);
  connect(viewReservedCardsButtonTop, &QPushButton::clicked, this,
          &MainWindow::showReservedCardsTop);

  // Ajouter un layout et les mettre dedans
  QHBoxLayout *topButtonLayout = new QHBoxLayout(this);
  topButtonLayout->addWidget(viewCardsButtonTop);
  topButtonLayout->addWidget(viewJetonsButtonTop);
  topButtonLayout->addWidget(viewReservedCardsButtonTop);
  mainLayout->addLayout(topButtonLayout);

  // ######## PLATEAU ET TIRAGES ########

  // Plateau et tirages
  QWidget *middleContainer = new QWidget(this);
  QHBoxLayout *middleLayout = new QHBoxLayout(middleContainer);

  // Créer le plateau et les tirages
  plateau = &Qt_Plateau::getPlateau();
  tirages = new Qt_Tirages(this);

  QVBoxLayout *plateauLayout = new QVBoxLayout(this);
  plateauLayout->addWidget(plateau);

  middleLayout->addLayout(
      plateauLayout); // Rajouter le plateau et le bouton au middle layout
  middleLayout->addStretch(); // Stretch pour mettre les tirages à droite
  tirages->setStyleSheet("background-image: url('../src/background.jpg'); "
                         "background-position: center;");
  middleLayout->addWidget(tirages);
  mainLayout->addWidget(middleContainer);

  // ######## CONDITION DE VICTOIRE ET REGLES ########

  QVBoxLayout *regles = new QVBoxLayout;

  // Ajouter le bouton de règles
  QPushButton *viewRegles = new QPushButton("Voir règles", this);
  QPushButton *viewStats = new QPushButton("Historique", this);
  QPushButton *viewStatsPlayers = new QPushButton("Statistiques", this);
  viewRegles->setStyleSheet("color: rgba(255, 255, 255, 255);");
  viewRegles->setFixedWidth(397 / 3);
  viewStats->setStyleSheet("color: rgba(255, 255, 255, 255);");
  viewStats->setFixedWidth(397 / 3);
  viewStatsPlayers->setStyleSheet("color: rgba(255, 255, 255, 255);");
  viewStatsPlayers->setFixedWidth(397 / 3);
  connect(viewRegles, &QPushButton::clicked, this, &MainWindow::openWebLink);
  connect(viewStats, &QPushButton::clicked, this, &MainWindow::showStats);
  connect(viewStatsPlayers, &QPushButton::clicked, this,
          &MainWindow::showStatsPlayers);

  // Conditions de victoire et son image
  QLabel *conditionsVictoire = new QLabel(this);
  QPixmap originalPixmap("../src/Reste_detoure/Conditions_victoire.png");
  QPixmap scaledPixmap = originalPixmap.scaled(
      397 / 3, 330 / 3, Qt::KeepAspectRatio, Qt::SmoothTransformation);
  conditionsVictoire->setPixmap(scaledPixmap);
  conditionsVictoire->setFixedSize(scaledPixmap.size());

  // Les rajouter au layout
  regles->addWidget(conditionsVictoire, Qt::AlignCenter);
  regles->addWidget(viewRegles, Qt::AlignCenter);
  regles->addWidget(viewStats, Qt::AlignCenter);
  regles->addWidget(viewStatsPlayers, Qt::AlignCenter);
  regles->addStretch(1);

  middleLayout->addLayout(regles, Qt::AlignCenter);

  // ######## SCORE EN BAS ET BOUTONS ########

  QGridLayout *bottomPlayer = new QGridLayout;
  bottomPlayer->addWidget(bottomPlayerNameLabel, 0, 1, Qt::AlignCenter);
  bottomPlayer->addWidget(bottomScoreDisplay, 1, 1, Qt::AlignCenter);

  QHBoxLayout *bottomRoyalLayout = new QHBoxLayout();
  bottomRoyalLayout->addWidget(bottomRoyal1);
  bottomRoyalLayout->addWidget(bottomRoyal2);
  bottomRoyalLayout->addWidget(bottomPrivileges);

  QWidget *royalWidgetBottom = new QWidget();
  royalWidgetBottom->setLayout(bottomRoyalLayout);

  bottomPlayer->addWidget(royalWidgetBottom, 0, 2, 2, 1, Qt::AlignRight);

  bottomPlayer->setColumnStretch(0, 1);
  bottomPlayer->setColumnStretch(1, 1);
  bottomPlayer->setColumnStretch(2, 1);

  mainLayout->addLayout(bottomPlayer);

  // Boutons du bas
  viewCardsButtonBottom = new QPushButton("Voir cartes achetées", this);
  viewJetonsButtonBottom = new QPushButton("Voir jetons", this);
  viewReservedCardsButtonBottom =
      new QPushButton("Voir cartes réservées", this);

  viewCardsButtonBottom->setStyleSheet("color: rgba(255, 255, 255, 255);");
  viewJetonsButtonBottom->setStyleSheet("color: rgba(255, 255, 255, 255);");
  viewReservedCardsButtonBottom->setStyleSheet(
      "color: rgba(255, 255, 255, 255);");
  connect(viewCardsButtonBottom, &QPushButton::clicked, this,
          &MainWindow::showBoughtCardsBottom);
  connect(viewJetonsButtonBottom, &QPushButton::clicked, this,
          &MainWindow::showJetonsBottom);
  connect(viewReservedCardsButtonBottom, &QPushButton::clicked, this,
          &MainWindow::showReservedCardsBottom);

  QHBoxLayout *bottomButtonLayout = new QHBoxLayout;
  bottomButtonLayout->addWidget(viewCardsButtonBottom);
  bottomButtonLayout->addWidget(viewJetonsButtonBottom);
  bottomButtonLayout->addWidget(viewReservedCardsButtonBottom);
  mainLayout->addLayout(bottomButtonLayout);

  // Set le widget central
  setCentralWidget(centralWidget);

  // Connexions
  connect(this, &MainWindow::triggerNextAction, this, &MainWindow::nextAction);
  connect(this, &MainWindow::triggerYesNo, this, &MainWindow::YesNo);
  connect(this, &MainWindow::triggerInfo, this, &MainWindow::showInfo);
  connect(this, &MainWindow::triggercolorChoice, this,
          &MainWindow::colorChoice);
  connect(this, &MainWindow::triggercolorJoker, this, &MainWindow::colorJoker);
  connect(this, &MainWindow::jetonActionDone, &wait_for_action_jeton,
          &QEventLoop::quit);
  connect(this, &MainWindow::carteActionDone, &wait_for_action_carte,
          &QEventLoop::quit);
}

MainWindow::~MainWindow() {
  // Rien
}

void MainWindow::showJetonsBottom() {
  QDialog *jetonsDialog = new QDialog(this);
  setCurrentDialog(jetonsDialog);
  jetonsDialog->setStyleSheet("background-image: url('../src/background.jpg'); "
                              "background-position: center;");

  QVBoxLayout *verticallayout = new QVBoxLayout(jetonsDialog);

  QLabel *name = new QLabel(jetonsDialog);
  name->setText("Jetons de " + QString::fromStdString(
                                   Jeu::getJeu().getCurrentPlayer().getName()));
  name->setStyleSheet("QLabel { color: white; }");

  verticallayout->addWidget(name, Qt::AlignCenter);

  QGridLayout *layout = new QGridLayout(jetonsDialog);
  QWidget *gridWidget = new QWidget(jetonsDialog);

  gridWidget->setLayout(layout);

  int nb = Jeu::getJeu().getCurrentPlayer().getNbJetons();

  for (int i = 0; i < 16; i++) {
    Qt_jeton *j = new Qt_jeton(jetonsDialog);
    j->setIndice(i);
    if (i < nb)
      j->setJeton(Jeu::getJeu().getCurrentPlayer().getJeton()[i]);
    j->setFixedSize(
        60, 60); // Width: 100px, Height: 140px based on 1:1.4 aspect ratio
    j->setStyleSheet("border: 1px solid black;");
    layout->addWidget(j, i / 4, i % 4);
    if (getDiscarding() == true)
      connect(j, &Qt_jeton::jetonClicked, &MainWindow::getMainWindow(),
              &MainWindow::jetonClicked);
    j->updateAppearance();
  }

  // Disable stretching and set fixed size for the layout
  for (int i = 0; i < 4; ++i) {
    layout->setRowStretch(i, 0);
    layout->setColumnStretch(i, 0);
  }

  verticallayout->addWidget(gridWidget);
  jetonsDialog->setLayout(verticallayout);

  jetonsDialog->show();
}

void MainWindow::showReservedCardsBottom() {
  QDialog *cardsDialog = new QDialog(this);
  cardsDialog->setStyleSheet("background-image: url('../src/background.jpg'); "
                             "background-position: center;");
  QGridLayout *layout = new QGridLayout(cardsDialog);

  int nb = Jeu::getJeu().getCurrentPlayer().getNbCartesReservees();
  for (int i = 0; i < nb; i++) {
    qDebug() << "CARTES RESERVEES";
    Qt_carte *c = new Qt_carte(cardsDialog);
    c->setCard(Jeu::getJeu().getCurrentPlayer().getCartesReserved()[i]);
    c->setFixedSize(75, 105);
    if (getBuyingCard() == true)
      c->setDisabled(false);
    else
      c->setDisabled(true);
    c->setIndice(i);
    c->setReservee(true);
    connect(c, &Qt_carte::carteClicked, &MainWindow::getMainWindow(),
            &MainWindow::carteClicked);
    connect(c, &Qt_carte::carteClicked, cardsDialog, &QDialog::accept);
    layout->addWidget(c, i / 4, i % 4);
    c->updateAppearance();
    qDebug() << c->getCard()->getVisuel();
  }

  cardsDialog->setLayout(layout);
  cardsDialog->exec(); // L'afficher
}

void MainWindow::showBoughtCardsBottom() {
  QDialog *cardsDialog = new QDialog(this);
  cardsDialog->setStyleSheet("background-image: url('../src/background.jpg'); "
                             "background-position: center;");
  QGridLayout *layout = new QGridLayout(cardsDialog);

  int nb = Jeu::getJeu().getCurrentPlayer().getNbCartesJoaillerie();
  for (int i = 0; i < nb; i++) {
    qDebug() << "CARTES ACHETEES";
    Qt_carte *c = new Qt_carte(cardsDialog);
    c->setCard(Jeu::getJeu().getCurrentPlayer().getCartesBought()[i]);
    c->setFixedSize(75, 105);
    c->setDisabled(true);
    layout->addWidget(c, i / 4, i % 4);
    c->updateAppearance();
    qDebug() << c->getCard()->getVisuel();
  }

  cardsDialog->setLayout(layout);
  cardsDialog->exec(); // L'afficher
}

void MainWindow::showBoughtCardsTop() {
  QDialog *cardsDialog = new QDialog(this);
  cardsDialog->setStyleSheet("background-image: url('../src/background.jpg'); "
                             "background-position: center;");
  QGridLayout *layout = new QGridLayout(cardsDialog);

  int nb = Jeu::getJeu().getOpponent().getNbCartesJoaillerie();
  for (int i = 0; i < nb; i++) {
    qDebug() << "CARTES ACHETEES";
    Qt_carte *c = new Qt_carte(cardsDialog);
    c->setCard(Jeu::getJeu().getOpponent().getCartesBought()[i]);
    c->setFixedSize(75, 105);
    c->setDisabled(true);
    layout->addWidget(c, i / 4, i % 4);
    c->updateAppearance();
    qDebug() << c->getCard()->getVisuel();
  }

  cardsDialog->setLayout(layout);
  cardsDialog->exec(); // L'afficher
}

void MainWindow::showJetonsTop() {
  QDialog *jetonsDialog = new QDialog(this);
  jetonsDialog->setStyleSheet("background-image: url('../src/background.jpg'); "
                              "background-position: center;");
  setCurrentDialog(jetonsDialog);

  QVBoxLayout *verticallayout = new QVBoxLayout(jetonsDialog);

  QLabel *name = new QLabel(jetonsDialog);
  name->setText("Jetons de " +
                QString::fromStdString(Jeu::getJeu().getOpponent().getName()));
  name->setStyleSheet("QLabel { color: white; }");

  verticallayout->addWidget(name, Qt::AlignCenter);

  QGridLayout *layout = new QGridLayout(jetonsDialog);
  QWidget *gridWidget = new QWidget(jetonsDialog);

  gridWidget->setLayout(layout);

  int nb = Jeu::getJeu().getOpponent().getNbJetons();

  for (int i = 0; i < 16; i++) {
    Qt_jeton *j = new Qt_jeton(jetonsDialog);
    j->setIndice(i);
    if (i < nb)
      j->setJeton(Jeu::getJeu().getOpponent().getJeton()[i]);
    j->setFixedSize(60, 60);
    j->setStyleSheet("border: 1px solid black;");
    layout->addWidget(j, i / 4, i % 4);
    j->setDisabled(true);
    if (i < nb && MainWindow::getMainWindow().getStealingJeton() == true &&
        Jeu::getJeu().getOpponent().getJeton()[i]->getColor() != Color::gold) {
      j->setDisabled(false);
      connect(j, &Qt_jeton::jetonClicked, &MainWindow::getMainWindow(),
              &MainWindow::jetonClicked);
      connect(j, &Qt_jeton::jetonClicked, jetonsDialog, &QDialog::accept);
    }
    j->updateAppearance();
  }

  // Disable stretching and set fixed size for the layout
  for (int i = 0; i < 4; ++i) {
    layout->setRowStretch(i, 0);
    layout->setColumnStretch(i, 0);
  }

  verticallayout->addWidget(gridWidget);
  jetonsDialog->setLayout(verticallayout);

  jetonsDialog->exec();
}

void MainWindow::showReservedCardsTop() {
  QDialog *cardsDialog = new QDialog(this);
  cardsDialog->setStyleSheet("background-image: url('../src/background.jpg'); "
                             "background-position: center;");
  QGridLayout *layout = new QGridLayout(cardsDialog);

  int nb = Jeu::getJeu().getOpponent().getNbCartesReservees();
  for (int i = 0; i < nb; i++) {
    qDebug() << "CARTES RESERVEES";
    Qt_carte *c = new Qt_carte(cardsDialog);
    c->setCard(Jeu::getJeu().getOpponent().getCartesReserved()[i]);
    c->setFixedSize(75, 105);
    c->setDisabled(true);
    c->setIndice(i);
    layout->addWidget(c, i / 4, i % 4);
    c->updateAppearance();
    qDebug() << c->getCard()->getVisuel();
  }

  cardsDialog->setLayout(layout);
  cardsDialog->exec(); // L'afficher
}

void MainWindow::updateTopScore(int score) { topScoreDisplay->display(score); }

void MainWindow::updateBottomScore(int score) {
  bottomScoreDisplay->display(score);
}

void MainWindow::remplirPlateau() {
  // Mettre l'action ici
}

void MainWindow::openWebLink() {
  QDesktopServices::openUrl(
      QUrl("https://cdn.1j1ju.com/medias/da/39/6a-splendor-duel-regle.pdf"));
}

void MainWindow::setTopPlayerName(const QString &name) {
  topPlayerNameLabel->setText(name);
  topPlayerNameLabel->update();
}

void MainWindow::setBottomPlayerName(const QString &name) {
  bottomPlayerNameLabel->setText(name);
  bottomPlayerNameLabel->update();
}

void MainWindow::updateTirages() {

  // Update des pointeurs carte

  // Tirage 1
  qDebug() << "Tirage1";
  for (int i = 0; i < 5; i++) {
    tirages->getTier1()[i]->setCard(
        Jeu::getJeu().get_tirage_1()->getTirage()[i]);

    // Update des visuels
    if (i < Jeu::getJeu().get_tirage_1()->getNbCartes())
      tirages->getTier1()[i]->updateAppearance();
    else {
      tirages->getTier1()[i]->setIcon(QIcon());
      tirages->getTier1()[i]->setIconSize(this->size());
    }
  }

  // Tirage 2
  qDebug() << "Tirage2";
  for (int i = 0; i < 4; i++) {
    tirages->getTier2()[i]->setCard(
        Jeu::getJeu().get_tirage_2()->getTirage()[i]);

    // Update des visuels
    if (i < Jeu::getJeu().get_tirage_2()->getNbCartes())
      tirages->getTier2()[i]->updateAppearance();
    else {
      tirages->getTier2()[i]->setIcon(QIcon());
      tirages->getTier2()[i]->setIconSize(this->size());
    }
  }

  // Tirage 3
  qDebug() << "Tirage3";
  for (int i = 0; i < 3; i++) {
    tirages->getTier3()[i]->setCard(
        Jeu::getJeu().get_tirage_3()->getTirage()[i]);

    // Update des visuels
    if (i < Jeu::getJeu().get_tirage_3()->getNbCartes())
      tirages->getTier3()[i]->updateAppearance();
    else {
      tirages->getTier3()[i]->setIcon(QIcon());
      tirages->getTier3()[i]->setIconSize(this->size());
    }
  }

  // Cartes royales
  qDebug() << "CartesRoyales";
  for (int i = 0; i < 4; i++) {
    tirages->getRoyalCards()[i]->setCard(Jeu::getJeu().getCartesRoyales()[i]);
    // Update des visuels
    if (i < Jeu::getJeu().getCartesRoyales().size())
      tirages->getRoyalCards()[i]->updateAppearance();
    else {
      tirages->getRoyalCards()[i]->setIcon(QIcon());
      tirages->getRoyalCards()[i]->setIconSize(this->size());
    }
  }

  if (tirages->getDeck1() != nullptr) {
    if (!Jeu::getJeu().getPioche(1)->est_vide())
      tirages->getDeck1()->updateAppearance(
          "../src/Reste_detoure/Pioche_niveau_1.png");
    else {
      tirages->getDeck1()->setIcon(QIcon());
      tirages->getDeck1()->setIconSize(this->size());
      tirages->getDeck1()->setDisabled(true);
    }
  }
  if (tirages->getDeck2() != nullptr) {
    if (!Jeu::getJeu().getPioche(2)->est_vide())
      tirages->getDeck2()->updateAppearance(
          "../src/Reste_detoure/Pioche_niveau_2.png");
    else {
      tirages->getDeck2()->setIcon(QIcon());
      tirages->getDeck2()->setIconSize(this->size());
      tirages->getDeck2()->setDisabled(true);
    }
  }
  if (tirages->getDeck3() != nullptr) {
    if (!Jeu::getJeu().getPioche(3)->est_vide())
      tirages->getDeck3()->updateAppearance(
          "../src/Reste_detoure/Pioche_niveau_3.png");
    else {
      tirages->getDeck3()->setIcon(QIcon());
      tirages->getDeck3()->setIconSize(this->size());
      tirages->getDeck3()->setDisabled(true);
    }
  }

  // Cartes royales et privileges des joueurs

  // Top
  int top_privileges = Jeu::getJeu().getOpponent().getNbPrivileges();
  topPrivileges->setText(
      "Nombre privilèges: " +
      QString::fromStdString(std::to_string(top_privileges)));
  switch (Jeu::getJeu().getOpponent().getNbCartesRoyales()) {
  case 0: {
    topRoyal1->setPixmap(QPixmap());
    topRoyal2->setPixmap(QPixmap());
    break;
  }
  case 1: {
    topRoyal1->setPixmap(
        QPixmap(
            QString::fromStdString(
                Jeu::getJeu().getOpponent().getRoyalCards()[0]->getVisuel()))
            .scaled(topRoyal1->size(), Qt::KeepAspectRatio,
                    Qt::SmoothTransformation));
    topRoyal2->setPixmap(QPixmap());
    break;
  }
  case 2: {
    topRoyal1->setPixmap(
        QPixmap(
            QString::fromStdString(
                Jeu::getJeu().getOpponent().getRoyalCards()[0]->getVisuel()))
            .scaled(topRoyal1->size(), Qt::KeepAspectRatio,
                    Qt::SmoothTransformation));
    topRoyal2->setPixmap(
        QPixmap(
            QString::fromStdString(
                Jeu::getJeu().getOpponent().getRoyalCards()[1]->getVisuel()))
            .scaled(topRoyal2->size(), Qt::KeepAspectRatio,
                    Qt::SmoothTransformation));
    break;
  }
  }

  // Bottom
  int bottom_privileges = Jeu::getJeu().getCurrentPlayer().getNbPrivileges();
  bottomPrivileges->setText(
      "Nombre privilèges: " +
      QString::fromStdString(std::to_string(bottom_privileges)));
  switch (Jeu::getJeu().getCurrentPlayer().getNbCartesRoyales()) {
  case 0: {
    bottomRoyal1->setPixmap(QPixmap());
    bottomRoyal2->setPixmap(QPixmap());
    break;
  }
  case 1: {
    bottomRoyal1->setPixmap(
        QPixmap(QString::fromStdString(Jeu::getJeu()
                                           .getCurrentPlayer()
                                           .getRoyalCards()[0]
                                           ->getVisuel()))
            .scaled(bottomRoyal1->size(), Qt::KeepAspectRatio,
                    Qt::SmoothTransformation));
    bottomRoyal2->setPixmap(QPixmap());

    break;
  }
  case 2: {
    bottomRoyal1->setPixmap(
        QPixmap(QString::fromStdString(Jeu::getJeu()
                                           .getCurrentPlayer()
                                           .getRoyalCards()[0]
                                           ->getVisuel()))
            .scaled(bottomRoyal1->size(), Qt::KeepAspectRatio,
                    Qt::SmoothTransformation));
    bottomRoyal2->setPixmap(
        QPixmap(QString::fromStdString(Jeu::getJeu()
                                           .getCurrentPlayer()
                                           .getRoyalCards()[1]
                                           ->getVisuel()))
            .scaled(bottomRoyal2->size(), Qt::KeepAspectRatio,
                    Qt::SmoothTransformation));
    break;
  }
  }
}

void MainWindow::updatePlateau() {

  Plateau::get_plateau().printTab();

  // Update des pointeurs jeton
  for (int i = 0; i < NJETONS; i++) {
    plateau->getJetons()[i]->setJeton(Plateau::get_plateau().get_plateau_i(i));
  }

  // Update des visuels
  for (int i = 0; i < NJETONS; i++) {
    plateau->getJetons()[i]->updateAppearance();
  }
}

void MainWindow::updatePrivileges() {
  int nb_privileges = Jeu::getJeu().getNbPrivilege();
  QIcon icon(
      QPixmap(QString::fromStdString("../src/Reste_detoure/Privilege.png")));
  switch (nb_privileges) {
  case 0:
    plateau->getPrivilege1()->setIcon(QIcon());
    plateau->getPrivilege1()->setIconSize(plateau->getPrivilege1()->size());
    plateau->getPrivilege2()->setIcon(QIcon());
    plateau->getPrivilege2()->setIconSize(plateau->getPrivilege2()->size());
    plateau->getPrivilege3()->setIcon(QIcon());
    plateau->getPrivilege3()->setIconSize(plateau->getPrivilege3()->size());
    break;
  case 1:
    // qDebug() << jeton->getVisuel();
    plateau->getPrivilege1()->setIcon(icon);
    plateau->getPrivilege1()->setIconSize(plateau->getPrivilege1()->size());
    plateau->getPrivilege2()->setIcon(QIcon());
    plateau->getPrivilege2()->setIconSize(plateau->getPrivilege2()->size());
    plateau->getPrivilege3()->setIcon(QIcon());
    plateau->getPrivilege3()->setIconSize(plateau->getPrivilege3()->size());
    break;
  case 2:
    // qDebug() << jeton->getVisuel();
    plateau->getPrivilege1()->setIcon(icon);
    plateau->getPrivilege1()->setIconSize(plateau->getPrivilege1()->size());
    // qDebug() << jeton->getVisuel();
    plateau->getPrivilege2()->setIcon(icon);
    plateau->getPrivilege2()->setIconSize(plateau->getPrivilege2()->size());
    // qDebug() << jeton->getVisuel();
    plateau->getPrivilege3()->setIcon(QIcon());
    plateau->getPrivilege3()->setIconSize(plateau->getPrivilege3()->size());
    break;
  case 3:
    // qDebug() << jeton->getVisuel();
    plateau->getPrivilege1()->setIcon(icon);
    plateau->getPrivilege1()->setIconSize(plateau->getPrivilege1()->size());
    // qDebug() << jeton->getVisuel();
    plateau->getPrivilege2()->setIcon(icon);
    plateau->getPrivilege2()->setIconSize(plateau->getPrivilege2()->size());
    // qDebug() << jeton->getVisuel();
    plateau->getPrivilege3()->setIcon(icon);
    plateau->getPrivilege3()->setIconSize(plateau->getPrivilege3()->size());
    break;
  }
  plateau->getPrivilege1()->update();
  plateau->getPrivilege2()->update();
  plateau->getPrivilege3()->update();
}

void MainWindow::nextAction(int *tmp, Joueur *j) {

  // qDebug() << "IN NEXT ACTION";
  int nb_choice = j->getOptionalChoices();

  ChoiceDialog dialog(nb_choice, this);
  if (dialog.exec() == QDialog::Accepted) {
    *tmp = dialog.getUserChoice();
  }
}

void MainWindow::YesNo(char *choice, const std::string &string) {

  popupYesNo dialog(this, string);
  if (dialog.exec() == QDialog::Accepted) {
    *choice = dialog.getUserChoice();
  }
}

void MainWindow::showInfo(const string &string) {

  InfoDialog dialog(QString::fromStdString(string), this);
  dialog.exec();
}

void MainWindow::jetonClicked(Qt_jeton *j) {

  if (j != nullptr && j->getJeton() != nullptr) {
    qDebug() << "Jeton cliqué : " << j->getJeton()->getVisuel();
    setIndiceJetonClick(j->getIndice());
  } else {
    setIndiceJetonClick(-1);
  }
  j->setDisabled(true);

  MainWindow::getMainWindow().jetonActionDone();
}

void MainWindow::carteClicked(Qt_carte *c) {
  if (c != nullptr)
    qDebug() << "Carte cliquée : " << c->getIndice() << c->getReservee();
  else
    qDebug() << "nullptr carte";
  if (c != nullptr and (c->getCard() != nullptr || c->getIndice() < 0))
    setDerniereCarteClick(c);
  else
    qDebug() << "carte click problem";
  MainWindow::getMainWindow().carteActionDone();
}

void MainWindow::deactivateButtons() {
  // Jetons
  int nbmax = Jeton::getNbMaxJetons();
  for (int i = 0; i < nbmax; i++) {
    plateau->getPlateau().getJetons()[i]->setEnabled(false);
  }

  // Tirage 1
  for (int i = 0; i < 5; i++) {
    getTirages()->getTier1()[i]->setEnabled(false);
  }

  // Tirage 2
  for (int i = 0; i < 4; i++) {
    getTirages()->getTier2()[i]->setEnabled(false);
  }

  // Tirage 3
  for (int i = 0; i < 3; i++) {
    getTirages()->getTier3()[i]->setEnabled(false);
  }

  // Tirage cartes royales
  for (int i = 0; i < 4; i++) {
    getTirages()->getRoyalCards()[i]->setEnabled(false);
  }

  // Les 3 pioches

  getTirages()->getDeck1()->setEnabled(false);
  getTirages()->getDeck2()->setEnabled(false);
  getTirages()->getDeck3()->setEnabled(false);

  // Déactiver les cartes reservées des 2 joueurs
}

void MainWindow::activateJetons() {
  // Jetons
  int nbmax = Jeton::getNbMaxJetons();
  for (int i = 0; i < nbmax; i++) {
    plateau->getPlateau().getJetons()[i]->setEnabled(true);
  }
}

void MainWindow::activateForReserve() {
  // Tirage 1
  for (int i = 0; i < Jeu::getJeu().get_tirage_1()->getTirage().size(); i++) {
    getTirages()->getTier1()[i]->setEnabled(true);
  }

  // Tirage 2
  for (int i = 0; i < Jeu::getJeu().get_tirage_2()->getTirage().size(); i++) {
    getTirages()->getTier2()[i]->setEnabled(true);
  }

  for (int i = 0; i < Jeu::getJeu().get_tirage_3()->getTirage().size(); i++) {
    getTirages()->getTier3()[i]->setEnabled(true);
  }

  // Les 3 pioches

  if (!Jeu::getJeu().getPioche(1)->est_vide())
    getTirages()->getDeck1()->setEnabled(true);
  if (!Jeu::getJeu().getPioche(2)->est_vide())
    getTirages()->getDeck2()->setEnabled(true);
  if (!Jeu::getJeu().getPioche(3)->est_vide())
    getTirages()->getDeck3()->setEnabled(true);
}

void MainWindow::activateForBuy() {
  qDebug() << "ACtiate for buy";
  // Tirage 1
  for (int i = 0; i < Jeu::getJeu().get_tirage_1()->getTirage().size(); i++) {
    getTirages()->getTier1()[i]->setEnabled(true);
  }

  // Tirage 2
  for (int i = 0; i < Jeu::getJeu().get_tirage_2()->getTirage().size(); i++) {
    getTirages()->getTier2()[i]->setEnabled(true);
  }

  // Tirage 3
  for (int i = 0; i < Jeu::getJeu().get_tirage_3()->getTirage().size(); i++) {
    getTirages()->getTier3()[i]->setEnabled(true);
  }

  // Cartes reservées par le joueur en question activées dans le constructeur de
  // la popup
}

void MainWindow::activateForRoyalCard() {
  // Seul les boutons des cartes non-nulles sont activés !!! a vérifier !!!

  for (int i = 0; i < Jeu::getJeu().getCartesRoyales().size(); i++) {
    getTirages()->getRoyalCards()[i]->setEnabled(true);
  }
}

void MainWindow::updateQuiJoue() {
  quijoue->setText(
      QString::fromStdString("C'est à " +
                             Jeu::getJeu().getCurrentPlayer().getName()) +
      " de jouer");
}

void MainWindow::colorChoice(Color *c, int *nb) {
  // Créer la fenêtre color choice et recup c et nb
  popupCouleur dialog(this);
  if (dialog.exec() == QDialog::Accepted) {
    *c = dialog.getColor();
    *nb = dialog.getNb();
  }
}

void MainWindow::activateJetonColor(const Color &c) {
  // Jetons
  int nbmax = Jeton::getNbMaxJetons();
  for (int i = 0; i < nbmax; i++) {
    if (plateau->getPlateau().getJetons()[i]->getJeton() != nullptr &&
        plateau->getPlateau().getJetons()[i]->getJeton()->getColor() == c)
      plateau->getPlateau().getJetons()[i]->setEnabled(true);
  }
}

void MainWindow::colorJoker(colorBonus *b) {
  // Créer la fenêtre color choice et recup c et nb
  popupJoker dialog(this);
  if (dialog.exec() == QDialog::Accepted) {
    *b = dialog.getColor();
  }
}

void MainWindow::showStats() {
  QDialog *dialog = new QDialog(this); // Create a new QDialog instance

  vector<Match *> matches = History::getHistory().getMatches();
  std::string txt = "";
  if (matches.size() != 0) {
    txt += "Historique des matchs : ";
    txt += "\n\n";

    for (int i = 0; i < matches.size(); i++) {
      txt += "\n";
      txt += "   Match ";
      txt += std::to_string(i + 1);
      txt += "    ||    Score : ";
      txt += std::to_string(matches[i]->getScoreWinner());
      txt += " - ";
      txt += std::to_string(matches[i]->getScoreOpponent());
      txt += "\n";
      txt += "      Joueur gagnant :    ";
      txt += matches[i]->getWinner()->getName();
      txt += "\n";
      txt += "      Joueur perdant :    ";
      txt += matches[i]->getOpponent()->getName();
      txt += "\n";
    }
  } else {
    txt += "Pas de match dans l'historique !";
  }

  QLabel *text = new QLabel(QString::fromStdString(txt));
  text->setWordWrap(true);

  // Create a QScrollArea
  QScrollArea *scrollArea = new QScrollArea(this);
  scrollArea->setWidgetResizable(true);
  scrollArea->setWidget(text);

  QVBoxLayout *layout =
      new QVBoxLayout(); // Use QVBoxLayout or another specific layout class
  layout->addWidget(scrollArea);

  dialog->setLayout(layout); // Set layout to the dialog

  dialog->exec();
}

const bool inside(const std::string s, vector<std::string> vect) {
  for (int i = 0; i < vect.size(); ++i) {
    if (vect[i] == s) {
      return true;
    }
  }
  return false;
}

template <typename T>
std::string to_string_with_precision(const T a_value, const int n = 6) {
  std::ostringstream out;
  out.precision(n);
  out << std::fixed << a_value;
  return out.str();
}

void MainWindow::showStatsPlayers() {

  QDialog *dialog = new QDialog(this); // Create a new QDialog instance

  vector<Match *> matches = History::getHistory().getMatches();
  vector<std::string> names;

  std::string txt = "";
  if (matches.size() != 0) {
    txt += "Nombre de parties jouées :  ";
    txt += to_string(matches.size());
    txt += "\n";

    txt += "Nombre de points pour gagner en moyenne : ";
    float winner_points = 0, loser_points = 0;
    for (int i = 0; i < matches.size(); ++i) {
      winner_points += (float)matches[i]->getScoreWinner();
      loser_points += (float)matches[i]->getScoreOpponent();
    }
    winner_points = winner_points / matches.size();
    loser_points = loser_points / matches.size();
    txt += to_string_with_precision(winner_points, 2);
    txt += "\n";
    txt += "Nombre de points des perdants en moyenne : ";
    txt += to_string_with_precision(loser_points, 2);
    txt += "\n";

    txt += "\n\n";

    txt += "Statistique des joueurs  ";
    txt += "\n\n";
    txt += "Nombre total de joueurs : ";
    txt += std::to_string(History::getHistory().getNbPlayers());
    txt += "\n";
    for (int i = 0; i < matches.size(); i++) {
      // ici pour les statistiques si on a pas déjà ajouté la personne donc si
      // son nom n'est pas encore dans le tableau

      if (!inside(matches[i]->getWinner()->getName(), names)) {
        // alors on affiche les stats du joueur
        txt += "\n";
        txt += "  Joueur : ";
        txt += matches[i]->getWinner()->getName();
        txt += "\n";
        txt += "    Nombre de matchs joués : ";
        txt += std::to_string(matches[i]->getWinner()->getPlayed());
        txt += "\n";
        txt += "    Nombre de matches gagnés : ";
        txt += std::to_string(matches[i]->getWinner()->getWins());
        float moy = (float)(matches[i]->getWinner()->getWins()) /
                    (float)(matches[i]->getWinner()->getPlayed());
        txt += " -    Pourcentage de victoire : ";
        txt += to_string_with_precision(moy * 100, 2);
        txt += "%";
        txt += "\n";
        names.push_back(matches[i]->getWinner()->getName());
      }

      if (!inside(matches[i]->getOpponent()->getName(), names)) {
        // alors on affiche les stats du joueur
        txt += "\n";
        txt += "  Joueur : ";
        txt += matches[i]->getOpponent()->getName();
        txt += "\n";
        txt += "    Nombre de matchs joués : ";
        txt += std::to_string(matches[i]->getOpponent()->getPlayed());
        txt += "\n";
        txt += "    Nombre de matches gagnés : ";
        txt += std::to_string(matches[i]->getOpponent()->getWins());
        float moy = (float)(matches[i]->getOpponent()->getWins()) /
                    (float)(matches[i]->getOpponent()->getPlayed());
        txt += " -    Pourcentage de victoire : ";
        txt += to_string_with_precision(moy * 100, 2);
        txt += "%";
        txt += "\n";
        names.push_back(matches[i]->getOpponent()->getName());
      }
    }
  } else {
    txt += "Pas de statistiques des joueurs";
  }

  QLabel *text = new QLabel(QString::fromStdString(txt));
  text->setWordWrap(true);

  // Create a QScrollArea
  QScrollArea *scrollArea = new QScrollArea(this);
  scrollArea->setWidgetResizable(true);
  scrollArea->setWidget(text);

  QVBoxLayout *layout =
      new QVBoxLayout(); // Use QVBoxLayout or another specific layout class
  layout->addWidget(scrollArea);

  dialog->setLayout(layout); // Set layout to the dialog

  dialog->exec();
}