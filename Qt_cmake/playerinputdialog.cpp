#include "playerinputdialog.h"

PlayerInputDialog::PlayerInputDialog(QWidget *parent) : QDialog(parent) {}

void PlayerInputDialog::onSubmitClicked() {
	emit inputValuesSubmitted(name1Edit->text(), name2Edit->text(),
	                          choix1Edit->text(), choix2Edit->text());
	close();
}
