#include "token_view.qt.h"
#include "main_window.qt.h"
#include <QCoreApplication>

Qt_jeton::Qt_jeton(QWidget *parent)
    : QPushButton(parent), jeton(nullptr), isClicked(false) {
	connect(this, SIGNAL(clicked()), this, SLOT(clickedEvent()));
}

void Qt_jeton::toggleClicked() {
	isClicked = !isClicked;
	update();
}

void Qt_jeton::paintEvent(QPaintEvent *event) {
	QPushButton::paintEvent(event);

	QPainter painter(this);
	if (isDown()) {
		painter.fillRect(rect(), QColor(128, 128, 128, 128));
	}
}

void Qt_jeton::updateAppearance() {
	if (jeton) {
		QIcon icon(QPixmap(QString::fromStdString(jeton->getVisual())));
		this->setIcon(icon);
		this->setIconSize(this->size());
	} else {
		this->setIcon(QIcon());
		this->setIconSize(this->size());
	}
	this->update();
}
