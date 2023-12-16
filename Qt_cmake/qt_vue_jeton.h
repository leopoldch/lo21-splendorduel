#ifndef QT_VUE_JETON_H
#define QT_VUE_JETON_H

#include <QWidget>
#include <QResizeEvent>
#include <QPainter>
#include <QString>
#include <QStaticText>
#include <QWidget>
#include <QPen>
#include <QBrush>
#include <QPushButton>
#include "../classes/jetons.h"


class Qt_jeton : public QPushButton {
    Q_OBJECT

private:
    bool isClicked; // Track whether the jeton has been clicked

public:
    explicit Qt_jeton(QWidget *parent = nullptr);
    void toggleClicked(); // Method to change the clicked state

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override; // Handle mouse press events

signals:
    void jetonClicked(); // Signal to emit when the jeton is clicked

public slots:
              // void onJetonClicked(); // Slot to handle jeton clicks
};


#endif // QT_VUE_JETON_H
