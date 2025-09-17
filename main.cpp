#include "main.h"
#include "main_window.qt.h"
#include "info_popup.qt.h"
#include "yesno_popup.qt.h"
#include "game.h"
#include "player.h"
#include <QApplication>
#include <QMainWindow>
#include <QString>
#include <QVBoxLayout>

int main(int argc, char *argv[]) {
	QApplication app(argc, argv);

	InfoDialog info_dialog("Bienvenue dans le game Splendor Duel !");
	info_dialog.exec();

	popupYesNo yesno(nullptr, "Voulez vous commencer une nouvelle partie ou "
	                          "finir la dernière partie en cours (Oui/Non) ?");
	yesno.exec();
	char tmp = yesno.getUserChoice();

	if (tmp == 'Y') {
		gameFromScratch(argc, argv);
	} else {
		gameFromJson(argc, argv);
	}

	Game::free();

	char tmp2 = 'Y';
	while (tmp2 == 'Y') {

		popupYesNo relaunch(nullptr,
		                    "Voulez vous replay (Oui) ou fermer (non) ?");
		relaunch.exec();
		try {
			tmp2 = relaunch.getUserChoice();
			if (tmp2 == 'Y') {
				gameFromScratch(argc, argv);
			}
		} catch (SplendorException &e) {
			MainWindow::getMainWindow().triggerInfo(e.getInfo());
		}
	}

	History::freeHistory();
	return app.exec();
}
