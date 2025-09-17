#include "board.qt.h"
#include "main_window.qt.h"
#include "token_view.qt.h"

Qt_Plateau::Handler Qt_Plateau::handler;

Qt_Plateau::~Qt_Plateau() {
	// Rien
}

Qt_Plateau::Qt_Plateau(QWidget *parent) : QWidget(parent) {
	layout = new QGridLayout(this);

	layout->setHorizontalSpacing(7);
	layout->setVerticalSpacing(18);

	const int jetonWidth = 64 * 0.75;  // 4 gaps, 5 tokens
	const int jetonHeight = 64 * 0.75; // 4 gaps, 5 tokens

	// Setup card grid
	for (int i = 0; i < NJETONS; ++i) {
		Qt_jeton *jeton = new Qt_jeton(this);
		jeton->setIndice(i);
		jeton->setStyleSheet("background: transparent;");
		jeton->setFixedSize(jetonWidth, jetonHeight);
		layout->addWidget(jeton, i / 5, i % 5);
		tokens.push_back(jeton);
	}

	// Setup privileges
	privilegesLayout = new QGridLayout(this);
	privilegesLayout->setHorizontalSpacing(4);

	privilege1 = new Qt_jeton(this);
	privilege2 = new Qt_jeton(this);
	privilege3 = new Qt_jeton(this);

	const int privilegeSize = 60 * 0.75;
	privilege1->setFixedSize(privilegeSize, privilegeSize);
	privilege1->setStyleSheet("background: transparent;");
	privilege2->setFixedSize(privilegeSize, privilegeSize);
	privilege2->setStyleSheet("background: transparent;");
	privilege3->setFixedSize(privilegeSize, privilegeSize);
	privilege3->setStyleSheet("background: transparent;");

	privilegesLayout->addWidget(privilege1, 0, 0); // Row 0, Column 0
	privilegesLayout->addWidget(privilege2, 0, 1); // Row 0, Column 1
	privilegesLayout->addWidget(privilege3, 0, 2); // Row 0, Column 2

	layout->addLayout(privilegesLayout, 5, 1, 1, 3);

	for (int i = 0; i < 5; ++i) {
		layout->setRowStretch(i, 0);
		layout->setColumnStretch(i, 0);
	}

	setFixedSize(371 * 0.75,
	             446 * 0.75); // Adjust size based on number of rows, columns,
	                          // and jeton size
}

void Qt_Plateau::paintEvent(QPaintEvent *event) {
	QPainter painter(this);

	QPixmap backgroundPixmap("../src/assets/rest_detoured/Board.png");
	QPixmap stretchedPixmap =
	    backgroundPixmap.scaled(371 * 0.75, 446 * 0.75, Qt::IgnoreAspectRatio,
	                            Qt::SmoothTransformation);

	int x = (this->width() - stretchedPixmap.width()) / 2;
	int y = (this->height() - stretchedPixmap.height()) / 2;

	painter.drawPixmap(x, y, stretchedPixmap);

	QWidget::paintEvent(event);
}

void Qt_Plateau::connectJetons() {
	for (int i = 0; i < NJETONS; i++) {
		connect(this->tokens[i], &Qt_jeton::jetonClicked,
		        &MainWindow::getMainWindow(), &MainWindow::jetonClicked);
	}
}
