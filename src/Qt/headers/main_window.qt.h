#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "tokens.h"
#include "choice_popup.qt.h"
#include "board.qt.h"
#include "draws.qt.h"
#include <QApplication>
#include <QDesktopServices>
#include <QDialog>
#include <QInputDialog>
#include <QLCDNumber>
#include <QLineEdit>
#include <QMainWindow>
#include <QPushButton>
#include <QUrl>
#include <QVBoxLayout>

#include "history.h"
#include "text_popup.qt.h"
#include "info_popup.qt.h"
#include "deck_or_draw_popup.qt.h"
#include "yesno_popup.qt.h"
#include "token_view.qt.h"

#define RIEN "../src/rien.png"

class MainWindow : public QMainWindow {
	Q_OBJECT

  private:
	QLabel *topRoyal1;
	QLabel *topRoyal2;

	QLabel *bottomRoyal1;
	QLabel *bottomRoyal2;

	QLabel *topPrivileges;
	QLabel *bottomPrivileges;

	QLCDNumber *topScoreDisplay;
	QLCDNumber *bottomScoreDisplay;

	QPushButton *viewCardsButtonBottom;
	QPushButton *viewJetonsButtonBottom;
	QPushButton *viewReservedCardsButtonBottom;

	QPushButton *viewCardsButtonTop;
	QPushButton *viewJetonsButtonTop;
	QPushButton *viewReservedCardsButtonTop;

	QLabel *topPlayerNameLabel;
	QLabel *bottomPlayerNameLabel;

	QEventLoop wait_for_action_jeton;
	QEventLoop wait_for_action_carte;

	Qt_Plateau *board;
	Qt_Tirages *tirages;

	QLabel *quijoue;

	Game *game;

	bool buyingCard;
	bool stealingJeton;

	struct Handler {
		MainWindow *instance = nullptr;
		~Handler() {
			delete instance;
			instance = nullptr;
		}
	};

	static Handler handler;
	MainWindow(QWidget *parent = nullptr);
	~MainWindow();

	MainWindow(const MainWindow &) = delete;
	MainWindow &operator=(const MainWindow &) = delete;

	int indice_jeton_click;
	Qt_carte *derniere_carte_click;

	bool discarding;

	QDialog *current_dialog;

	void closeEvent(QCloseEvent *event) override {
		Game::free();
		History::freeHistory();
		exit(0);
	}

  public:
	const bool getDiscarding() const { return discarding; }
	void setDiscarding(bool x) { discarding = x; }

	void setBuyingCard(bool x) { buyingCard = x; }
	const bool getBuyingCard() { return buyingCard; }

	void setStealingJeton(bool x) { stealingJeton = x; }
	const bool getStealingJeton() { return stealingJeton; }

	void updateQuiJoue();

	QEventLoop *getCarteWaitLoop() { return &wait_for_action_carte; }
	QEventLoop *getJetonWaitLoop() { return &wait_for_action_jeton; }

	int getTokenIndexOnClick() const { return indice_jeton_click; }
	void setIndiceJetonClick(int x) { indice_jeton_click = x; }

	Qt_carte *getLastCardClicked() const { return derniere_carte_click; }
	void setDerniereCarteClick(Qt_carte *c) { derniere_carte_click = c; }

	void updateTopScore(int score); // Méthode de mise à jour du score du haut
	void updateBottomScore(int score); //  -- du bas

	void updateScores() {
		int s1 = Game::getGame().getCurrentPlayer().getNbPoints();
		bottomScoreDisplay->display(s1);
		int s2 = Game::getGame().getOpponent().getNbPoints();
		topScoreDisplay->display(s2);
	}

	void demanderNoms() {

		InputPopup *popup = new InputPopup(this);
		popup->setModal(true);
		popup->exec();
	}

	void setTopPlayerName(const QString &name);
	void setBottomPlayerName(const QString &name);

	void updateBoard();
	void updateDraws();
	void updatePrivileges();

	static MainWindow &getMainWindow() {
		if (handler.instance == nullptr)
			handler.instance = new MainWindow();
		return *handler.instance;
	}

	void freeMainWindow() {
		delete handler.instance;
		handler.instance = nullptr;
	}

	void deactivateButtons();
	void activateTokens();
	void activateForReserve();
	void activateForBuy();
	void activateForRoyalCard();

	Qt_Tirages *getTirages() const { return tirages; }

	void activateJetonColor(const Color &c);

	void acceptCurrentDialog() {
		if (current_dialog != nullptr) {
			current_dialog->accept();
			current_dialog = nullptr;
		}
	}

	void setCurrentDialog(QDialog *d) { current_dialog = d; }

  private slots:
	void showBoughtCardsTop();
	void showReservedCardsTop();
	void showJetonsTop();

	void showBoughtCardsBottom();
	void showReservedCardsBottom();
	void showJetonsBottom();

	void YesNo(char *choice, const std::string &string);
	void fillBoard();
	void openWebLink();
	void nextAction(int *tmp, Player *j);
	void showInfo(const string &string);
	void colorChoice(Color *c, int *nb);
	void colorJoker(colorBonus *b);
	void showStats();
	void showStatsPlayers();

  public slots:
	void jetonClicked(Qt_jeton *);
	void carteClicked(Qt_carte *);

  signals:
	void triggerNextAction(int *tmp, Player *j);
	void triggerYesNo(char *choice, const std::string &string = "");
	void triggerInfo(const string &string);
	void triggercolorChoice(Color *c, int *nb);
	void triggercolorJoker(colorBonus *b);
	void jetonActionDone();
	void carteActionDone();
};

#endif // MAINWINDOW_H
