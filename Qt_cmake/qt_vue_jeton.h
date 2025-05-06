#ifndef QT_VUE_JETON_H
#define QT_VUE_JETON_H

#include "../classes/jetons.h"
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
  const Jeton *jeton;
  int indice;

public:
  explicit Qt_jeton(QWidget *parent = nullptr);
  void toggleClicked();
  void updateAppearance(); // Mise à jour image

  const Jeton *getJeton() const { return jeton; }
  void setJeton(const Jeton *j) { jeton = j; }

  const int getIndice() const { return indice; }
  void setIndice(int x) { indice = x; }

protected:
  void paintEvent(QPaintEvent *event) override;

signals:
  void jetonClicked(Qt_jeton *j); // Signal à émettre si le jeton a été click

private slots:
  void clickedEvent() { emit jetonClicked(this); }
};

#endif // QT_VUE_JETON_H
