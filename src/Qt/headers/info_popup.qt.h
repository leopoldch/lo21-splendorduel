#ifndef QT_POPUP_INFO_H
#define QT_POPUP_INFO_H

#include <QDialog>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>

class InfoDialog : public QDialog {
	Q_OBJECT

  public:
	// Constructor with a message parameter
	explicit InfoDialog(const QString &message, QWidget *parent = nullptr)
	    : QDialog(parent) {
		QVBoxLayout *layout = new QVBoxLayout(this);

		QLabel *label = new QLabel(message, this);
		layout->addWidget(label);

		QPushButton *okButton = new QPushButton("OK", this);
		layout->addWidget(okButton);

		connect(okButton, &QPushButton::clicked, this, &QDialog::accept);

		setLayout(layout);
		connect(okButton, &QPushButton::clicked, this, &QDialog::accept);
	}
};

#endif // QT_POPUP_INFO_H
