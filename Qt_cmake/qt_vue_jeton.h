#ifndef QT_VUE_JETON_H
#define QT_VUE_JETON_H

#include "../classes/tokens.h"
#include <QBrush>
#include <QPainter>
#include <QPen>
#include <QPushButton>
#include <QResizeEvent>
#include <QStaticText>
#include <QString>
#include <QWidget>

class Qt_jeton : public QPushButton {
	Q_OBJECT

  private:
	bool isClicked; // Track si le jeton a été click
	const Token *jeton;
	int index;

  public:
	explicit Qt_jeton(QWidget *parent = nullptr);
	void toggleClicked();
	void updateAppearance(); // Mise à jour image

	const Token *getToken() const { return jeton; }
	void setJeton(const Token *j) { jeton = j; }

	const int getIndex() const { return index; }
	void setIndice(int x) { index = x; }

  protected:
	void paintEvent(QPaintEvent *event) override;

  signals:
	void jetonClicked(Qt_jeton *j); // Signal à émettre si le jeton a été click

  private slots:
	void clickedEvent() { emit jetonClicked(this); }
};

#endif // QT_VUE_JETON_H
