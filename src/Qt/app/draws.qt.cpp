#include "draws.qt.h"
#include "card_view.qt.h"
#include "main_window.qt.h"
#include <QPixmap>
#include <QVBoxLayout>

Qt_Tirages::Qt_Tirages(QWidget *parent) : QWidget(parent) {
	QVBoxLayout *mainLayout = new QVBoxLayout(this);
	QVBoxLayout *verticalLayout = new QVBoxLayout(this);

	deck1 = new Qt_carte(this);
	deck2 = new Qt_carte(this);
	deck3 = new Qt_carte(this);

	//    deck1->setIndice(-1);
	//    deck2->setIndice(-2);
	//    deck3->setIndice(-3);

	royalCardsImage = nullptr;

	// Setup each tier
	setupTierLayout(tier1Layout, tier1Cards, 5, deck1);
	setupTierLayout(tier2Layout, tier2Cards, 4, deck2);
	setupTierLayout(tier3Layout, tier3Cards, 3, deck3);
	setupTierLayout(royalLayout, royalCards, 4, royalCardsImage);

	// Add the tier layouts to the left side of the horizontal layout
	verticalLayout->addLayout(royalLayout);
	verticalLayout->addLayout(tier1Layout);
	verticalLayout->addLayout(tier2Layout);
	verticalLayout->addLayout(tier3Layout);

	// Justify the cards to the right
	verticalLayout->addSpacerItem(
	    new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum));

	mainLayout->addLayout(verticalLayout);
}

void Qt_Tirages::setupTierLayout(QHBoxLayout *&layout,
                                 std::vector<Qt_carte *> &cards, int cardCount,
                                 Qt_carte *deck) {
	layout = new QHBoxLayout(this);

	layout->setSpacing(0);
	layout->setContentsMargins(0, 0, 0, 0);

	for (int i = 0; i < cardCount; ++i) {
		Qt_carte *card = new Qt_carte(this);
		card->setReservee(false);
		card->setIndice(i);
		card->setStyleSheet("background: transparent;");
		card->setFixedSize(75, 105);
		cards.push_back(card);
		layout->addWidget(card);
	}

	if (deck != nullptr) {
		switch (cardCount) {
		case 5: {
			deck->setIndice(-1);
			break;
		}
		case 4: {
			deck->setIndice(-2);
			break;
		}
		case 3: {
			deck->setIndice(-3);
			break;
		}
		}

		deck->setFixedSize(75, 105);
		deck->setStyleSheet("background: transparent;");
		layout->addWidget(deck);
	}
}

void Qt_Tirages::connectCartes() {
	qDebug() << "ACtiate for buy";
	// Draw 1
	for (int i = 0; i < 5; i++) {
		connect(this->getTier1()[i], &Qt_carte::carteClicked,
		        &MainWindow::getMainWindow(), &MainWindow::carteClicked);
	}

	// Draw 2
	for (int i = 0; i < 4; i++) {
		connect(this->getTier2()[i], &Qt_carte::carteClicked,
		        &MainWindow::getMainWindow(), &MainWindow::carteClicked);
	}

	// Draw 3
	for (int i = 0; i < 3; i++) {
		connect(this->getTier3()[i], &Qt_carte::carteClicked,
		        &MainWindow::getMainWindow(), &MainWindow::carteClicked);
	}

	// Draw cards royales
	for (int i = 0; i < 4; i++) {
		connect(this->getRoyalCards()[i], &Qt_carte::carteClicked,
		        &MainWindow::getMainWindow(), &MainWindow::carteClicked);
	}
	// Cartes reservées

	// Decks
	connect(this->getDeck1(), &Qt_carte::carteClicked,
	        &MainWindow::getMainWindow(), &MainWindow::carteClicked);
	connect(this->getDeck2(), &Qt_carte::carteClicked,
	        &MainWindow::getMainWindow(), &MainWindow::carteClicked);
	connect(this->getDeck3(), &Qt_carte::carteClicked,
	        &MainWindow::getMainWindow(), &MainWindow::carteClicked);
}
