#include "debug.h"

int main() {

	char tmp;
	cout << "Bienvenue dans le game splendor duel ! " << endl;
	cout << "voulez vous commencer une nouvelle partie ou finir la dernière "
	        "partie en cours ? [N/R]"
	     << endl;
	cin >> tmp;
	while (tmp != 'R' && tmp != 'N') {
		cout
		    << "voulez vous commencer une nouvelle partie ou finir la dernière "
		       "partie en cours ? [N/R]"
		    << endl;
		cin >> tmp;
	}
	if (tmp == 'N') {
		gameFromScratch();
	} else {
		gameFromJson();
	}

	return 0;
}
