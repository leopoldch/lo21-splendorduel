#include "main.h"
#include "Qt_cmake/mainwindow.h"
#include "Qt_cmake/qt_popup_info.h"
#include "Qt_cmake/qt_popup_yesno.h"
#include "classes/jeu.h"
#include "classes/joueur.h"
#include <QApplication>
#include <QMainWindow>
#include <QString>
#include <QVBoxLayout>

int main(int argc, char *argv[]) {
  QApplication app(argc, argv);

  InfoDialog info_dialog("Bienvenue dans le jeu Splendor Duel !");
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

  Jeu::libereJeu();

  char tmp2 = 'Y';
  while (tmp2 == 'Y') {

    popupYesNo relaunch(nullptr, "Voulez vous rejouer (Oui) ou fermer (non) ?");
    relaunch.exec();
    try {
      tmp2 = relaunch.getUserChoice();
      if (tmp2 == 'Y') {
        gameFromScratch(argc, argv);
      }
    } catch (SplendorException &e) {
      cout << e.getInfos() << endl;
      MainWindow::getMainWindow().triggerInfo(e.getInfos());
    }
  }

  History::freeHistory();
  return app.exec();
}
