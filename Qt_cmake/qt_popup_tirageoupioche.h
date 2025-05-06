#ifndef QT_POPUP_TIRAGEOUPIOCHE_H
#define QT_POPUP_TIRAGEOUPIOCHE_H

#include <QDialog>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>

class popupTiragePioche : public QDialog {
  Q_OBJECT

private:
  char userChoice;

public:
  popupTiragePioche(QWidget *parent = nullptr) : QDialog(parent) {
    QVBoxLayout *layout = new QVBoxLayout(this);

    QPushButton *tirage = new QPushButton("Tirage", this);
    connect(tirage, &QPushButton::clicked, this,
            [this]() { onChoiceMade('T'); });
    layout->addWidget(tirage);

    QPushButton *pioche = new QPushButton("Pioche", this);
    connect(pioche, &QPushButton::clicked, this,
            [this]() { onChoiceMade('P'); });
    layout->addWidget(pioche);
  }

  int getUserChoice() const { return userChoice; }

private slots:
  void onChoiceMade(char choice) {
    userChoice = choice;
    accept();
  }
};

#endif // QT_POPUP_TIRAGEOUPIOCHE_H
