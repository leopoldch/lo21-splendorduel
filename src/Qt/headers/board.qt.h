#ifndef QT_PLATEAU_H
#define QT_PLATEAU_H

#include "token_view.qt.h"
#include <QGridLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QWidget>

#define NJETONS 25

class Qt_Plateau : public QWidget {
	Q_OBJECT;

  public:
	const std::vector<Qt_jeton *> &getJetons() const { return tokens; }
	void connectJetons();

	static Qt_Plateau &getPlateau() {
		if (handler.instance == nullptr)
			handler.instance = new Qt_Plateau();
		return *handler.instance;
	}

	void freePlateau() {
		delete handler.instance;
		handler.instance = nullptr;
	}

	Qt_jeton *getPrivilege1() { return privilege1; }
	Qt_jeton *getPrivilege2() { return privilege2; }
	Qt_jeton *getPrivilege3() { return privilege3; }

  private:
	struct Handler {
		Qt_Plateau *instance = nullptr;
		~Handler() {
			delete instance;
			instance = nullptr;
		}
	};

	static Handler handler;
	Qt_Plateau(QWidget *parent = nullptr);
	~Qt_Plateau();

	Qt_Plateau(const Qt_Plateau &) = delete;
	Qt_Plateau &operator=(const Qt_Plateau &) = delete;

	QGridLayout *layout;
	QGridLayout *privilegesLayout;
	std::vector<Qt_jeton *> tokens;
	Qt_jeton *privilege1;
	Qt_jeton *privilege2;
	Qt_jeton *privilege3;

  signals:
	void clicked();

  protected:
	void mousePressEvent(QMouseEvent *event) override { emit clicked(); }
	void paintEvent(QPaintEvent *event) override;
};

#endif // QT_PLATEAU_H
