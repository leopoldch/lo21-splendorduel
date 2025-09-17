#ifndef QT_TIRAGES_H
#define QT_TIRAGES_H

#include "qt_vue_carte.h"
#include <QHBoxLayout>
#include <QLabel>
#include <QVector>
#include <QWidget>

class Qt_Tirages : public QWidget {
	Q_OBJECT

  public:
	explicit Qt_Tirages(QWidget *parent = nullptr);
	std::vector<Qt_carte *> &getTier1() { return tier1Cards; }
	std::vector<Qt_carte *> &getTier2() { return tier2Cards; }
	std::vector<Qt_carte *> &getTier3() { return tier3Cards; }
	std::vector<Qt_carte *> &getRoyalCards() { return royalCards; }

	Qt_carte *getDeck1() { return deck1; }
	Qt_carte *getDeck2() { return deck2; }
	Qt_carte *getDeck3() { return deck3; }

	void connectCartes();

  private:
	QHBoxLayout *tier1Layout;
	QHBoxLayout *tier2Layout;
	QHBoxLayout *tier3Layout;
	QHBoxLayout *royalLayout;

	std::vector<Qt_carte *> tier1Cards;
	std::vector<Qt_carte *> tier2Cards;
	std::vector<Qt_carte *> tier3Cards;
	std::vector<Qt_carte *> royalCards;

	Qt_carte *deck1;
	Qt_carte *deck2;
	Qt_carte *deck3;

	Qt_carte *royalCardsImage;

	void setupTierLayout(QHBoxLayout *&layout, std::vector<Qt_carte *> &cards,
	                     int cardCount, Qt_carte *deckImage);

  signals:
	void clicked();

  protected:
	void mousePressEvent(QMouseEvent *event) override { emit clicked(); }
};

#endif // QT_TIRAGES_H
