#include "player.h"
#include "../Qt_cmake/mainwindow.h"
#include "card.h"
#include "game.h"

#include <QInputDialog>
#include <QMessageBox>

using namespace std;
/******************** Fonctions utilitaires ********************/
int positiveOrNull(int x) {
	if (x < 0)
		return 0;
	return x;
}
/******************** Fonctions utilitaires ********************/

/******************** StrategyPlayer ********************/

// constructeur destructeur
StrategyPlayer::StrategyPlayer(const string &name)
    : points_number(0), jewelry_cards_number(0), royal_cards_number(0),
      crown_number(0), privilege_number(0), name(name), token_number(0),
      reserved_jewelry_cards_number(0), games(0), games_won(0) {}

StrategyPlayer::~StrategyPlayer() {
	// Déstruction cards royales
	for (auto royal_cards : royal_cards) {
		delete royal_cards;
	}
	royal_cards.clear();

	// Déstruction privilèges
	for (auto privilege : privileges) {
		delete privilege;
	}
	privileges.clear();
}

int StrategyPlayer::calculateBonus(enum colorBonus bonus) {
	int result = 0;
	for (auto c : bought_jewelry_cards) {
		if ((*c).getBonus() == bonus) {
			result += (*c).getNbBonus();
		}
	}
	return result;
}

int StrategyPlayer::TokenAmount(const Color &color) const {
	int result = 0;
	for (auto j = tokens.begin(); j != tokens.end(); ++j) {
		if ((*j)->getColor() == color)
			result++;
	}
	return result;
}

void StrategyPlayer::withdrawTokens(const Color &c, int val) {
	int tmp = val;
	std::vector<int> tmp_tab(0);
	for (int k = 0; k < tokens.size(); k++) {
		if ((tokens[k]->getColor() == c) && (tmp != 0)) {
			tmp_tab.push_back(k);
			tmp--;
		}
	}

	if (tmp > 0) {
		throw SplendorException("Pas assez de tokens " + toString(c) +
		                        " pour en supprimer plus ! ");
	}
	for (int j = val - 1; j >= 0; j--) { // erreur!
		Bag::get().insertToken(tokens[tmp_tab[j]]);
		tokens.erase(tokens.begin() + tmp_tab[j]);
		token_number--;
	}
	for (auto j : tokens) {
		cout << *j << endl;
	}
}

// TODO: rename
void StrategyPlayer::reserveCard(Draw *t, const int index) {
	// reservation d'une card d'un draw
	unsigned int count = 0;
	for (int i = 0; i < tokens.size(); ++i) {
		if (tokens[i]->getColor() == Color::gold)
			count++;
	}
	if (count == 0) {
		throw SplendorException(
		    "Le joueur n'a pas de jeton or en sa possession!");
	}
	const JewelryCard &tmp = t->getCard(index);
	reserved_jewelry_cards.push_back(&tmp);
	reserved_jewelry_cards_number++;
	t->fill();
}

// TODO: rename
void StrategyPlayer::reserveCard(Deck *p) {
	// reservation de la card au above de la deck
	unsigned int count = 0;
	for (int i = 0; i < tokens.size(); ++i) {
		if (tokens[i]->getColor() == Color::gold)
			count++;
	}
	if (count == 0) {
		throw SplendorException(
		    "Le player n'a pas de jeton or en sa possession!");
	}
	const JewelryCard &tmp = p->getCard();
	reserved_jewelry_cards.push_back(&tmp);
	reserved_jewelry_cards_number++;
}

void StrategyPlayer::drawToken(int i) {

	if (i > 24 || i < 0) {
		throw SplendorException("Indice du board non valide ! ");
	}

	const Token *tmp = Board::getBoard().getBoardCaseByIndex(i);
	if (tmp == nullptr) {
		throw SplendorException("Token déjà pris !");
	}
	tokens.push_back(tmp);
	Board::getBoard().setTokenOnBoardByIndex(i, nullptr);
	Board::getBoard().setCurrentNb(Board::getBoard().getCurrentNb() - 1);
	std::cout << "Token acquis; nombre de tokens restants sur le board : "
	          << Board::getBoard().getCurrentNb() << std::endl;
	token_number++;
}

void StrategyPlayer::obtainRoyalCard(unsigned int i) {
	// on prend une card dans le game
	if (i > Game::getGame().getRoyalCards().size()) {
		throw SplendorException("Carte non disponible");
	}

	const RoyalCard &tmp = Game::getGame().drawRoyalCard(i);
	royal_cards.push_back(&tmp);
	royal_cards_number++;
	points_number += tmp.getPrestige();

	Game::getGame().getCurrentPlayer().applyRoyalCapacity(
	    tmp, Game::getGame().getOpponent());
}

// Surcharge Qt
void StrategyPlayer::obtainRoyalCard_qt(unsigned int i) {
	// on prend une card dans le game
	if (i > Game::getGame().getRoyalCards().size()) {
		throw SplendorException("Carte non disponible");
	}
	// if (royalCardEligibility() == false) throw SplendorException("Pas
	// eligible.");
	const RoyalCard &tmp = Game::getGame().drawRoyalCard(i);
	royal_cards.push_back(&tmp);
	// ENLEVER DU game
	royal_cards_number++;
	points_number += tmp.getPrestige();

	Game::getGame().getCurrentPlayer().applyRoyalCapacity_Qt(
	    tmp, Game::getGame().getOpponent());
}

bool StrategyPlayer::royalCardEligibility() {
	if (crown_number >= 3 and crown_number < 6 and royal_cards_number == 0) {
		return true;
	}

	else if (crown_number >= 6 and royal_cards_number == 1) {
		return true;
	}

	else {
		return false;
	}
}

bool StrategyPlayer::jokerCardEligibility() {
	unsigned int test = 0;
	for (auto card : bought_jewelry_cards) {
		// on test sur toutes les cards, à partir du moment où le player
		// possède une card d'une color (bonus != nullopt) il peut acheter
		// une card avec un bonus color
		if ((*card).getBonus() != nullopt) {
			return true;
		}
	}
	return false;
}

void StrategyPlayer::obtainPrivilege() {
	// on va chercher dans le draw des privilèges un privilège. (du board ou
	// alors de ton opponent ? ) d'abord je regarde s'il y a des privilèges
	// dans le game :

	if (privilege_number == Privilege::getMaxInstance()) {
		throw SplendorException(
		    "Vous avez déjà le nombre maximum autorisé de privilège!");
	}
	if (Game::getGame().getPrivilegeNumber() == 0) {
		// si l'opponent a tous les privilèges
		if (this == &Game::getGame().getCurrentPlayer()) {
			// si le player qui obtient le privilège est celui dont c'est le
			// tour, on prend à l'opponent
			Game::getGame().getOpponent().withdrawPrivilege();
		} else { // si c'est l'opponent de clui dont c'est le tour, on retire
			     // le privilège à celui qui est en train de jouer
			Game::getGame().getCurrentPlayer().withdrawPrivilege();
		}
	}
	if (Game::getGame().getPrivilegeNumber() == 0) {
		throw SplendorException("Plus de privilège!");
	}
	privileges.push_back(Game::getGame().getPrivilege());
	privilege_number++;
}

void StrategyPlayer::withdrawPrivilege() {
	if (privilege_number == 0) {
		throw SplendorException(
		    "Vous ne pouvez pas retirer de privilège au player");
	}
	Game::getGame().setPrivilege(*privileges[0]);
	privileges.erase(privileges.begin());
	privilege_number--;
}

void StrategyPlayer::fillingBoard() {
	Game::getGame().fillBoard();
	Game::getGame().getOpponent().obtainPrivilege();
	Board::getBoard().printArray();
}

// méthode utilitaire pour le main
bool StrategyPlayer::victoryConditions() {
	if (crown_number >= 10)
		return true;
	if (points_number >= 20)
		return true;

	// test sur les couleurs, si le player possède 10 points pour des cards
	// dont le bonus est de même couleurs, alors le player gagne
	int white_points = 0;
	int blue_points = 0;
	int red_points = 0;
	int green_points = 0;
	int black_points = 0;
	for (auto card : bought_jewelry_cards) {
		optional<colorBonus> bonus = (*card).getBonus();
		if (bonus != nullopt) {
			if (bonus == colorBonus::white)
				white_points += card->getPrestige();
			if (bonus == colorBonus::blue)
				blue_points += card->getPrestige();
			if (bonus == colorBonus::red)
				red_points += card->getPrestige();
			if (bonus == colorBonus::green)
				green_points += card->getPrestige();
			if (bonus == colorBonus::black)
				black_points += card->getPrestige();
		}
	}
	if (white_points >= 10 || blue_points >= 10 || black_points >= 10 ||
	    red_points >= 10 || green_points >= 10) {
		return true;
	}

	// on renvoie false si rien n'est bon!

	return false;
}

void StrategyPlayer::printPlayer() {
	cout << "Player : " << name << endl;
	cout << "Nombre de privilèges : " << privilege_number << endl;
	cout << "Nombre de points : " << points_number << endl;
	cout << "Nombre de couronnes : " << crown_number << endl;
	cout << "Cartes joailleries possédées : " << endl;
	for (auto card : bought_jewelry_cards) {
		cout << *card << endl;
	}
	cout << "---------------------------------------" << endl;
	cout << "Cartes joailleries reservées : " << endl;
	for (auto card : reserved_jewelry_cards) {
		cout << *card << endl;
	}
	cout << "---------------------------------------" << endl;
	cout << "Cartes royales :" << endl;
	for (auto card : royal_cards) {
		cout << *card << endl;
	}
	cout << "---------------------------------------" << endl;
	cout << "Jetons possédés : " << endl;
	for (auto jet : tokens) {
		cout << *jet << endl;
	}
}

int StrategyPlayer::getOptionalChoices() {
	/***Convention***
	Cette méthode renvoie:
	 - 0 si le player ou l'ia ne peut rien faire
	 - 1 si le player ou l'ia ne peut qu'utiliser un privilège,
	 - 2 si le player ou l'ia ne peut que remplir le board,
	 - 3 si le player ou l'ia peut faire les deux.
	****************/
	int nb_choices = 0;
	if (Game::getGame().getCurrentPlayer().getPrivilegeNumber() > 0) {
		nb_choices += 1;
	}
	if (Board::getBoard().getCurrentNb() < Token::getMaxTokenNumber() and
	    Bag::get().getTokenNumber() > 0) {
		nb_choices += 2;
	}
	return nb_choices;
}

void StrategyPlayer::withdrawWhiteToken() {
	int compteur = 0;
	for (size_t i = 0; i < tokens.size(); ++i) {
		if (tokens[i]->getColor() == Color::white) {
			tokens.erase(tokens.begin() + compteur);
			break;
		}
		compteur++;
	}
	token_number--;
}

void StrategyPlayer::withdrawPerlToken() {
	int compteur = 0;
	for (size_t i = 0; i < tokens.size(); ++i) {
		if (tokens[i]->getColor() == Color::perl) {
			tokens.erase(tokens.begin() + compteur);
			break;
		}
		compteur++;
	}
	token_number--;
}

/******************** Player ********************/

// constructeur et destructeur
Player::Player(const string &name) : StrategyPlayer(name) {}
Player::Player(const json data) : StrategyPlayer(data) {}

Player::~Player() {
	// Déstruction cards royales
	for (auto royal_cards : royal_cards) {
		delete royal_cards;
	}
	royal_cards.clear();

	// Déstruction privilèges
	for (auto privilege : privileges) {
		delete privilege;
	}
	privileges.clear();
}

// Méthodes polymorphiques
void Player::choice() {
	int tmp = 0;
	bool end_choice = 0;
	int nb_choice = 0;
	while (!end_choice) {
		try {
			nb_choice =
			    getOptionalChoices(); // bien vérifier la convention sur le
			                          // retour dans la définition de la méthode
			int i = 1;
			cout << "Actions optionnelles disponibles:" << endl;
			if (nb_choice % 2 == 1) {
				cout << "Utiliser un privilège-> " << i++ << endl;
			}
			if (nb_choice >= 2) {
				cout << "Remplir le board -> " << i++ << endl;
			}
			cout << "Actions obligatoire:" << endl;
			cout << "Pour prendre des tokens appuyez sur -> " << i++ << endl;
			cout << "Pour acheter une card appuyez sur -> " << i++ << endl;
			cout << "choices :";
			cin >> tmp;

			if (tmp < 1 or tmp > i + 1) {
				throw SplendorException(
				    "Il n'y a que" + to_string(i) +
				    " choices! Vous ne pouvez pas choisir autre chose!\n");
			}
			string info;
			cout << "Validez-vous votre choices? [Y/N]" << endl;
			cin >> info;
			if (info == "N") {
				cout << "Vous n'avez pas validé , vous devez recommencer voter "
				        "choices!";
				throw SplendorException("");
			}

			switch (nb_choice) { // l'affichage et donc le choices dépend de la
				                 // valeur de retour des choices optionnels
			case (0): {          // aucun choices optionnel possible
				switch (tmp) {
				case 1: {
					tokenSelection();
					end_choice = 1;
					break;
				}
				case 2: {
					cardPurchase();
					end_choice = 1;

					break;
				}
				default: // on continue jusqu'à ce que l'utilisateur choisisse
				         // une entrée valide!
					break;
				}
				break;
			}
			case (1): { // seulement possible d'utiliser un privilège
				switch (tmp) {
				case 1: {
					usePrivilege();
					break;
				}
				case 2: {
					tokenSelection();
					end_choice = 1;
					break;
				}
				case 3: {
					cardPurchase();
					end_choice = 1;
					break;
				}
					//                        case 4: {
					//                            // affichage des tokens du
					//                            jouer !
					//                            //cout<<"Inventaire du player
					//                            :
					//                            "<<Game::getGame().getCurrentPlayer().getName()<<endl;
					//                            // afficher pour chaque type
					//                            break;
					//                        }
				default:
					break;
				}
				break;
			}
			case (2): { // seulement possible de remplir le board
				switch (tmp) {
				case 1: { // remplissage board
					fillingBoard();
					break;
				}
				case 2: {
					tokenSelection();
					end_choice = 1;
					break;
				}
				case 3: {
					cardPurchase();
					end_choice = 1;
					break;
				}
					//                        case 4: {
					//                            // affichage des tokens du
					//                            jouer !
					//                            //cout<<"Inventaire du player
					//                            :
					//                            "<<Game::getGame().getCurrentPlayer().getName()<<endl;
					//                            // afficher pour chaque type
					//                            break;
					//                        }
				default:
					break;
				}
				break;
			}
			case (3): { // deux choices optionnels possibles
				switch (tmp) {
				case 1: {
					usePrivilege();
					break;
				}
				case 2: { // remplissage board
					fillingBoard();
					break;
				}
				case 3: {
					tokenSelection();
					end_choice = 1;
					break;
				}
				case 4: {
					cardPurchase();
					end_choice = 1;
					break;
				}
					//                        case 5: {
					//                            // affichage des tokens du
					//                            jouer !
					//                            //cout<<"Inventaire du player
					//                            :
					//                            "<<Game::getGame().getCurrentPlayer().getName()<<endl;
					//                            // afficher pour chaque type
					//                            break;
					//                        }
				default:
					break;
				}
				break;
			}
			default:
				break;
			}

		} catch (SplendorException &e) {
			cout << e.getInfo() << "\n";
		}
	}
}

// Surcharge Qt
void Player::choice_Qt() {

	int tmp = 0;
	bool end_choice = 0;
	int nb_choice = 0;
	char info;
	while (!end_choice) {
		try {
			MainWindow::getMainWindow().deactivateButtons();
			nb_choice =
			    getOptionalChoices(); // bien vérifier la convention sur le
			                          // retour dans la définition de la méthode
			MainWindow::getMainWindow().triggerNextAction(&tmp, this);

			if (info == 'N') {
				cout << "Vous n'avez pas validé , vous devez recommencer voter "
				        "choices!";
				throw SplendorException("");
			}

			switch (nb_choice) { // l'affichage et donc le choices dépend de la
				                 // valeur de retour des choices optionnels
			case (0): {          // aucun choices optionnel possible
				switch (tmp) {
				case 1: {
					tokenSelection_Qt();

					end_choice = 1;
					break;
				}
				case 2: {
					cardPurchase_Qt();

					end_choice = 1;

					break;
				}
				default: { // on continue jusqu'à ce que l'utilisateur choisisse
					       // une entrée valide!
					break;
				} break;
				}
				break;
			}
			case (1): { // seulement possible d'utiliser un privilège
				switch (tmp) {
				case 1: {
					usePrivilege_Qt();

					break;
				}
				case 2: {
					tokenSelection_Qt();

					end_choice = 1;
					break;
				}
				case 3: {
					cardPurchase_Qt();

					end_choice = 1;
					break;
				}
				default: {
					break;
				}
				}
				break;
			}
			case (2): { // seulement possible de remplir le board
				switch (tmp) {
				case 1: { // remplissage board
					fillingBoard();

					break;
				}
				case 2: {
					tokenSelection_Qt();

					end_choice = 1;
					break;
				}
				case 3: {
					cardPurchase_Qt();

					end_choice = 1;
					break;
				}
				default: {
					break;
				}
				}
				break;
			}
			case (3): { // deux choices optionnels possibles
				switch (tmp) {
				case 1: {
					usePrivilege_Qt();

					break;
				}
				case 2: { // remplissage board
					fillingBoard();

					break;
				}
				case 3: {
					tokenSelection_Qt();

					end_choice = 1;
					break;
				}
				case 4: {
					cardPurchase_Qt();

					end_choice = 1;
					break;
				}

				default: {
					break;
				}
				}
				break;
			}
			default: {
				break;
			}
			}
			MainWindow::getMainWindow().updateBoard();
			MainWindow::getMainWindow().updateDraws();
			MainWindow::getMainWindow().updateScores();
			MainWindow::getMainWindow().updatePrivileges();
			MainWindow::getMainWindow().update();
		} catch (SplendorException &e) {
			MainWindow::getMainWindow().triggerInfo(e.getInfo());
			cout << e.getInfo() << "\n";
		}
	}
}

void Player::usePrivilege() {
	if (privilege_number <= 0)
		throw SplendorException("Vous n'avez pas de privilège!");
	unsigned int index;
	cout << "Quel jeton voulez-vous piocher ? " << endl;
	cout << "index : ";
	cin >> index;
	Game::getGame().getPlayerRound().drawToken(index);
	withdrawPrivilege();
}

void Player::usePrivilege_Qt() {
	if (privilege_number <= 0)
		throw SplendorException("Vous n'avez pas de privilège!");
	int index = -1;

	MainWindow::getMainWindow().triggerInfo("Veuillez piocher un jeton");
	// Activer les tokens
	MainWindow::getMainWindow().activateTokens();

	while (index == -1 or
	       (index != -1 and
	        Board::getBoard().getBoardCaseByIndex(index)->getColor() ==
	            Color::gold)) {
		MainWindow::getMainWindow().getJetonWaitLoop()->exec();
		index = MainWindow::getMainWindow().getTokenIndexOnClick();
	}

	Game::getGame().getPlayerRound().drawToken(index);
	withdrawPrivilege();
}

void Player::tokenSelection() {
	bool nb_ok = 0;
	bool choix_ok = 0;
	if (Board::getBoard().onlyGold() and
	    Game::getGame().getPlayerRound().getReservedCardNumber() >= 3)
		throw SplendorException(
		    "Pas possible de piocher: il ne reste que des tokens or et vous ne "
		    "pouvez plus réserver de card!");
	while (!choix_ok) {
		try {
			std::vector<int> tmp_tab(0);
			optional<Position> pos = nullopt;
			int gold_number = 0;
			int perl_number = 0;
			string validation;
			while (validation != "Y") {
				while (tmp_tab.size() < 3) {
					unsigned int index = 0;
					cout << "Veuillez renseigner l'index du jeton "
					     << tmp_tab.size() << " que vous voulez prendre ";
					if (tmp_tab.size() >
					    0) { // ajout de la possibilité de s'arrêter
						cout << "-1 pour arrêter la sélection de tokens";
					}
					cout << " :" << endl;
					cout << "choices :";
					cin >> index;
					if (index == -1) {
						break;
					}
					if (Board::getBoard().getBoardCaseByIndex(index) ==
					    nullptr) { // le nombre de cases sur le board
						           // correspond au nombre de tokens dans le
						           // game
						gold_number = 0;
						perl_number = 0;
						tmp_tab.clear();
						throw SplendorException(
						    "Il n'y a pas de jeton à cet index!\n");
					}
					if (index > Token::getMaxTokenNumber()) { // le nombre de
						                                      // cases sur le
						// board correspond au
						// nombre de tokens dans le
						// game
						gold_number = 0;
						perl_number = 0;
						tmp_tab.clear();
						throw SplendorException(
						    "Il n'y a que " +
						    std::to_string(Token::getMaxTokenNumber()) +
						    " places sur le board\n");
					}
					if (Board::getBoard()
					        .getBoardCaseByIndex(index)
					        ->getColor() == Color::gold) {
						gold_number++;
					}
					if (Board::getBoard()
					        .getBoardCaseByIndex(index)
					        ->getColor() == Color::perl) {
						perl_number++;
					}
					if ((gold_number == 1) &&
					    (Game::getGame()
					         .getPlayerRound()
					         .getReservedCardNumber() >= 3)) {
						gold_number = 0;
						perl_number = 0;
						tmp_tab.clear();
						throw SplendorException(
						    "Vous n'avez pas le droit de réserver une "
						    "card supplémentaire!");
					}
					if (gold_number == 1 and tmp_tab.size() > 0) {
						gold_number = 0;
						perl_number = 0;
						tmp_tab.clear();
						throw SplendorException(
						    "Attention, on ne peut prendre un jeton or "
						    "seulement tout seul!");
					}
					tmp_tab.push_back(index);
				}
				cout << "Validez-vous votre sélection? [Y/N] ";
				cin >> validation;
				if (validation != "Y") { // on recommence le choices des tokens
					tmp_tab.clear();
					gold_number = 0;
					perl_number = 0;
					cout << "\n Vous allez recommencer le choices des tokens: "
					     << endl;
					cout << "Tableau" << endl;
					//                    for(int test = 0;
					//                    test<tmp_tab.size();test++){
					//                        cout<<tmp_tab[test]<<" - ";
					//                    }
				}
			}

			// tri du vecteur par selection
			int min = 0;
			for (int j = 0; j < tmp_tab.size() - 1; j++) {
				min = j;
				for (int k = j + 1; k < tmp_tab.size(); k++) {
					if (tmp_tab[k] < tmp_tab[min]) {
						min = k;
					}
				}
				if (min != j) {
					int tmp2 = tmp_tab[min];
					tmp_tab[min] = tmp_tab[j];
					tmp_tab[j] = tmp2;
				}
			}
			// vecteur trié
			// cout<<"vecteur trié\n";

			if (tmp_tab.size() ==
			    2) { // vérification de l'alignement pour 2 tokens
				// cout<<"vérification pour 2 tokens\n";
				const Token *jet1 =
				    Board::getBoard().getBoardCaseByIndex(tmp_tab[0]);
				optional<Position> pos1 =
				    Board::getBoard().tokensAreASide(tmp_tab[1], jet1);
				if (pos1 == nullopt) {
					throw SplendorException("Jetons non-alignés\n");
				}
			}
			if (tmp_tab.size() ==
			    3) { // vérification de l'alignement pour 3 tokens
				// cout<<"vérification pour 3 tokens\n";
				const Token *jet1 =
				    Board::getBoard().getBoardCaseByIndex(tmp_tab[1]);
				optional<Position> pos1 =
				    Board::getBoard().tokensAreASide(tmp_tab[0], jet1);

				const Token *jet2 =
				    Board::getBoard().getBoardCaseByIndex(tmp_tab[2]);
				optional<Position> pos2 =
				    Board::getBoard().tokensAreASide(tmp_tab[1], jet2);

				if ((pos1 != pos2) || (pos1 == nullopt) || (pos2 == nullopt)) {
					throw SplendorException("Jetons non-alignés\n");
				}
			}

			// on a vérifié l'alignement des tokens
			if (perl_number ==
			    2) { // obtention d'un privilège par l'opponent si
				     // on deck les 2 tokens perles en une fois
				Game::getGame().getOpponent().obtainPrivilege();
			}

			if (tmp_tab.size() ==
			    3) { // obtention d'un privilège par l'opponent si
				     // les 3 tokens sont de la même color
				if ((Board::getBoard()
				         .getBoardCaseByIndex(tmp_tab[0])
				         ->getColor() == Board::getBoard()
				                             .getBoardCaseByIndex(tmp_tab[1])
				                             ->getColor()) &&
				    (Board::getBoard()
				         .getBoardCaseByIndex(tmp_tab[1])
				         ->getColor() == Board::getBoard()
				                             .getBoardCaseByIndex(tmp_tab[2])
				                             ->getColor())) {
					Game::getGame().getOpponent().obtainPrivilege();
				}
			}
			for (int i = 0; i < tmp_tab.size();
			     i++) { // acquisition des tokens par le player
				Game::getGame().getCurrentPlayer().drawToken(tmp_tab[i]);
			}
			if (gold_number == 1) {
				cardReservation();
			}
			choix_ok = 1;
		} catch (SplendorException &e) {
			cout << e.getInfo() << "\n";
		}
	}
}

void Player::tokenSelection_Qt() {
	MainWindow::getMainWindow().activateTokens();

	// qDebug() << "SELECTION JETONS QT";
	bool nb_ok = 0;
	bool choix_ok = 0;
	if (Board::getBoard().onlyGold() and
	    Game::getGame().getPlayerRound().getReservedCardNumber() >= 3) {
		throw SplendorException(
		    "Pas possible de piocher: il ne reste que des tokens or et vous ne "
		    "pouvez plus réserver de card!");
	}
	while (!choix_ok) {
		try {
			qDebug() << "Try selection";
			std::vector<int> tmp_tab(0);
			optional<Position> pos = nullopt;
			int gold_number = 0;
			int perl_number = 0;
			string validation;
			char choice_valid = 'a';
			while (choice_valid != 'Y') {
				MainWindow::getMainWindow().activateTokens();
				qDebug() << "While choice_valid";
				while (tmp_tab.size() < 3) {
					qDebug() << "While size()";
					int index = 0;

					MainWindow::getMainWindow().getJetonWaitLoop()->exec();

					index = MainWindow::getMainWindow().getTokenIndexOnClick();
					qDebug() << index;
					if (index == -1)
						continue;

					if (Board::getBoard().getBoardCaseByIndex(index) ==
					    nullptr) { // le nombre de cases sur le board
						           // correspond au nombre de tokens dans le
						           // game
						gold_number = 0;
						perl_number = 0;
						tmp_tab.clear();
						throw SplendorException(
						    "Il n'y a pas de jeton à cet index!\n");
					}
					if (index > Token::getMaxTokenNumber()) { // le nombre de
						                                      // cases sur le
						// board correspond au
						// nombre de tokens dans le
						// game
						gold_number = 0;
						perl_number = 0;
						tmp_tab.clear();
						throw SplendorException(
						    "Il n'y a que " +
						    std::to_string(Token::getMaxTokenNumber()) +
						    " places sur le board\n");
					}
					if (Board::getBoard()
					        .getBoardCaseByIndex(index)
					        ->getColor() == Color::gold) {
						gold_number++;
					}
					if (Board::getBoard()
					        .getBoardCaseByIndex(index)
					        ->getColor() == Color::perl) {
						perl_number++;
					}
					if ((gold_number == 1) &&
					    (Game::getGame()
					         .getPlayerRound()
					         .getReservedCardNumber() >= 3)) {
						gold_number = 0;
						perl_number = 0;
						tmp_tab.clear();
						throw SplendorException(
						    "Vous n'avez pas le droit de réserver une "
						    "card supplémentaire!");
					}
					if (gold_number == 1 and tmp_tab.size() > 0) {
						gold_number = 0;
						perl_number = 0;
						tmp_tab.clear();
						throw SplendorException(
						    "Attention, on ne peut prendre un jeton or "
						    "seulement tout seul!");
					}
					tmp_tab.push_back(index);

					if (tmp_tab.size() < 3) {
						MainWindow::getMainWindow().triggerYesNo(
						    &choice_valid, "Sélectionner encore un jeton ");

						if (choice_valid == 'Y')
							qDebug() << "Yes choice";
						else
							qDebug() << "No choice";

						if (choice_valid == 'N') {
							break;
						}
					}
				}

				MainWindow::getMainWindow().triggerYesNo(
				    &choice_valid, "Validez-vous votre sélection ?");
				/*
				cout<<"Validez-vous votre sélection? [Y/N] ";
				cin>>validation;
				*/
				if (choice_valid !=
				    'Y') { // on recommence le choices des tokens
					tmp_tab.clear();
					gold_number = 0;
					perl_number = 0;
					MainWindow::getMainWindow().triggerInfo(
					    "Vous allez recommencer le choices des tokens.");

					// cout<<"\n Vous allez recommencer le choices des tokens:
					// "<<endl; cout<<"Tableau"<<endl;
					//                    for(int test = 0;
					//                    test<tmp_tab.size();test++){
					//                        cout<<tmp_tab[test]<<" - ";
					//                    }
				}
			}

			// tri du vecteur par selection
			int min = 0;
			for (int j = 0; j < tmp_tab.size() - 1; j++) {
				min = j;
				for (int k = j + 1; k < tmp_tab.size(); k++) {
					if (tmp_tab[k] < tmp_tab[min]) {
						min = k;
					}
				}
				if (min != j) {
					int tmp2 = tmp_tab[min];
					tmp_tab[min] = tmp_tab[j];
					tmp_tab[j] = tmp2;
				}
			}

			// ### GESTION NON ALLIGNEMENT
			if (tmp_tab.size() ==
			    2) { // vérification de l'alignement pour 2 tokens
				// cout<<"vérification pour 2 tokens\n";
				const Token *jet1 =
				    Board::getBoard().getBoardCaseByIndex(tmp_tab[0]);
				optional<Position> pos1 =
				    Board::getBoard().tokensAreASide(tmp_tab[1], jet1);
				if (pos1 == nullopt) {
					throw SplendorException("Jetons non-alignés\n");
				}
			}
			if (tmp_tab.size() ==
			    3) { // vérification de l'alignement pour 3 tokens
				// cout<<"vérification pour 3 tokens\n";
				const Token *jet1 =
				    Board::getBoard().getBoardCaseByIndex(tmp_tab[1]);
				optional<Position> pos1 =
				    Board::getBoard().tokensAreASide(tmp_tab[0], jet1);

				const Token *jet2 =
				    Board::getBoard().getBoardCaseByIndex(tmp_tab[2]);
				optional<Position> pos2 =
				    Board::getBoard().tokensAreASide(tmp_tab[1], jet2);

				if ((pos1 != pos2) || (pos1 == nullopt) || (pos2 == nullopt)) {
					tmp_tab.clear();
					throw SplendorException("Jetons non-alignés\n");
				}
			}

			// on a vérifié l'alignement des tokens
			if (perl_number ==
			    2) { // obtention d'un privilège par l'opponent si
				     // on deck les 2 tokens perles en une fois
				Game::getGame().getOpponent().obtainPrivilege();
			}

			if (tmp_tab.size() ==
			    3) { // obtention d'un privilège par l'opponent si
				     // les 3 tokens sont de la même color
				if ((Board::getBoard()
				         .getBoardCaseByIndex(tmp_tab[0])
				         ->getColor() == Board::getBoard()
				                             .getBoardCaseByIndex(tmp_tab[1])
				                             ->getColor()) &&
				    (Board::getBoard()
				         .getBoardCaseByIndex(tmp_tab[1])
				         ->getColor() == Board::getBoard()
				                             .getBoardCaseByIndex(tmp_tab[2])
				                             ->getColor())) {
					Game::getGame().getOpponent().obtainPrivilege();
				}
			}
			for (int i = 0; i < tmp_tab.size();
			     i++) { // acquisition des tokens par le player
				Game::getGame().getCurrentPlayer().drawToken(tmp_tab[i]);
			}

			if (gold_number == 1) {
				MainWindow::getMainWindow().deactivateButtons();
				cardReservation_Qt();
			}
			choix_ok = 1;
		} catch (SplendorException &e) {
			MainWindow::getMainWindow().triggerInfo(e.getInfo() +
			                                        "Veuillez recommencer: ");
			cout << e.getInfo() << "\n";
		}
	}
}

void Player::cardReservation() {
	cout << "\n\nTirage1 :" << endl;
	cout << *Game::getGame().getFirstDraw() << endl;
	cout << "\nTirage2 :" << endl;
	cout << *Game::getGame().getSecondDraw() << endl;
	cout << "\nTirage3 :" << endl;
	cout << *Game::getGame().getThirdDraw() << endl;
	string tirageOuPioche;
	cout << "\nSouhaitez-vous réserver une card dans un draw ou la première "
	        "card d'une deck? [T/P]"
	     << endl;
	cin >> tirageOuPioche;
	if (tirageOuPioche != "P") {
		// Le player choisit de réserver la card sur un draw
		int choices = 0;
		while (choices != 1 && choices != 2 && choices != 3) {
			cout << "Dans quel draw vous voulez réserver une card ?" << endl;
			cout << "choices";
			cin >> choices;
		}
		switch (choices) {
		case 1: {
			unsigned int index = 0;
			cout << "Veuillez renseigner l'index de la card que vous voulez "
			        "retirer ! "
			     << endl;
			cout << "choices : ";
			cin >> index;
			reserveCard(Game::getGame().getFirstDraw(), index);
			break;
		}
		case 2: {
			unsigned int index = 0;
			cout << "Veuillez renseigner l'index de la card que vous voulez "
			        "retirer ! "
			     << endl;
			cout << "choices : ";
			cin >> index;
			reserveCard(Game::getGame().getSecondDraw(), index);
			break;
		}
		case 3: {
			unsigned int index = 0;
			cout << "Veuillez renseigner l'index de la card que vous voulez "
			        "retirer ! "
			     << endl;
			cout << "choices : ";
			cin >> index;
			reserveCard(Game::getGame().getThirdDraw(), index);
			break;
		}
		}
	} else {
		int choices = 0;
		cout << "Sur quelle deck souhaitez-vous réserver la card? " << endl;
		cout << "choices: ";
		cin >> choices;
		while (choices != 1 && choices != 2 && choices != 3) {
			cout << "Sur quelle deck voulez-vous réserver une card ?" << endl;
			cout << "choices";
			cin >> choices;
		}
		reserveCard(Game::getGame().getDeck(choices));
	}
}

void Player::cardReservation_Qt() {
	MainWindow::getMainWindow().activateForReserve();
	string tirageOuPioche;

	MainWindow::getMainWindow().activateForBuy();
	MainWindow::getMainWindow().getCarteWaitLoop()->exec();

	// Click card et récup la ref de la card et index dans draw ou dans les
	// cards reservées
	Qt_carte *last_card_clicked =
	    MainWindow::getMainWindow().getLastCardClicked();

	// Carte dans la deck
	if (last_card_clicked->getIndex() < 0) {
		Game::getGame().getCurrentPlayer().reserveCard(
		    Game::getGame().getDeck(-1 * last_card_clicked->getIndex()));
	}

	// Carte dans draw
	else {
		const JewelryCard *c =
		    dynamic_cast<const JewelryCard *>(last_card_clicked->getCard());
		int niveau_tirage = c->getLevel();
		Draw *draw = Game::getGame().getDrawById(niveau_tirage);
		Game::getGame().getCurrentPlayer().reserveCard(
		    draw, last_card_clicked->getIndex());
	}
}

void Player::applyCapacity(const JewelryCard &card, StrategyPlayer &opponent) {
	if (card.getCapacity().has_value()) {
		std::optional<Capacity> capa = card.getCapacity();
		if (capa == Capacity::steal_opponent_pawn) {
			cout << "Utilisation de capacité : vous pouvez prendre un jeton "
			        "gemme ou "
			        "perl à votre opponent.\n";

			if ((opponent.getToken().empty()) &&
			    (!opponent.onlyGoldInJetons())) {
				cout << "Dommage votre opponent ne possède pas de jeton "
				        "gemme ou "
				        "perl!"
				     << endl;
			} else {
				vector<const Token *> jetons_adversaire = opponent.getToken();
				cout << "Voici les tokens de votre opponent: " << endl;
				int i = 0;
				for (auto jet : jetons_adversaire) {
					cout << "Indice : " << i++ << ", " << *jet << endl;
				}
				int choice = -1;
				do {
					if (choice != -1) {
						cout << "Vous ne pouvez pas prendre un jeton or!"
						     << endl;
					}
					cout << "Quel jeton souhaitez vous lui voler?" << endl;
					cout << "Choix : ";
					cin >> choice;
				} while (jetons_adversaire[choice]->getColor() == Color::gold);
				tokens.push_back(jetons_adversaire[choice]);
				token_number++;
				jetons_adversaire.erase(jetons_adversaire.begin() + choice);
				opponent.setNbJetons(opponent.getTokenNumber() - 1);
			}
		} else if (capa == Capacity::take_priviledge) {
			Game::getGame().getCurrentPlayer().obtainPrivilege();
		} else if (capa == Capacity::take_on_board) {
			cout << "Utilisation de capacité : vous pouvez prendre un jeton de "
			        "la "
			        "color bonus de la card\n";
			const optional<enum colorBonus> &color = card.getBonus();
			if (Board::getBoard().colorsOnBoard(color)) {
				bool choix_ok = 0;
				unsigned int index = 0;
				while (!choix_ok) {
					try {
						cout << "Veuillez renseigner l'index du jeton que "
						        "vous voulez "
						        "prendre\n ";
						cout << "choices :" << endl;
						cin >> index;
						if (Board::getBoard().getBoardCaseByIndex(index) ==
						    nullptr) {
							index = 0;
							throw SplendorException(
							    "Il n'y a pas de jeton à cet index!\n");
						}
						if (index < 0 or
						    index >= Token::getMaxTokenNumber()) { // le nombre
							                                       // de cases
							// sur le board
							// correspond au
							// nombre de tokens
							// dans le game
							index = 0;
							throw SplendorException(
							    "Il n'y a que " +
							    std::to_string(Token::getMaxTokenNumber()) +
							    " places sur le board\n");
						}
						string s = "Bonus ";
						if (s + toString(Board::getBoard()
						                     .getBoardCaseByIndex(index)
						                     ->getColor()) !=
						    toString(color)) {
							index = 0;
							throw SplendorException(
							    "il faut choisir un jeton de la color du "
							    "bonus!\n");
						} else {
							Game::getGame().getCurrentPlayer().drawToken(index);
							choix_ok = 1;
						}
					} catch (SplendorException &e) {
						cout << e.getInfo() << "\n";
					}
				}
			}
		} else { // si la capacité est de replay
			Game::getGame().nextRound(1);
		}
	}
	if (card.getBonus() == colorBonus::joker) {
		cout << "Utilisation de capacité : vous pouvez transformer le joker en "
		        "un "
		        "bonus de color en l'associant à"
		        "une de vos card dotée d'au moins un bonus.\n";
		int bonus_blanc = calculateBonus(colorBonus::white);
		int bonus_bleu = calculateBonus(colorBonus::blue);
		int bonus_rouge = calculateBonus(colorBonus::red);
		int bonus_vert = calculateBonus(colorBonus::green);
		int bonus_noir = calculateBonus(colorBonus::black);

		bool verif_choix = false;
		try {
			while (!verif_choix) {
				cout << "Faites votre choices :" << endl;
				int option = 0;
				if (bonus_blanc > 0) {
					cout << "Bonus white [1]" << endl;
					option++;
				}
				if (bonus_bleu > 0) {
					cout << "Bonus blue [2]" << endl;
					option++;
				}
				if (bonus_rouge > 0) {
					cout << "Bonus red [3]" << endl;
					option++;
				}
				if (bonus_vert > 0) {
					cout << "Bonus green [4]" << endl;
					option++;
				}
				if (bonus_noir > 0) {
					cout << "Bonus black [5]" << endl;
					option++;
				}
				// Vérifiez si aucune option n'est disponible
				if (option == 0) {
					verif_choix = true;
					throw SplendorException("vous ne possédez aucune card "
					                        "dotée de bonus.. Capacité"
					                        " sans effet\n");
				} else {
					int choices;
					cin >> choices;
					colorBonus b;
					switch (choices) {
					case 1:
						b = colorBonus::white;
						card.changeBonusColor(b);
						verif_choix = true;
						break;
					case 2:
						b = colorBonus::blue;
						card.changeBonusColor(b);
						verif_choix = true;
						break;
					case 3:
						b = colorBonus::red;
						card.changeBonusColor(b);
						verif_choix = true;
						break;
					case 4:
						b = colorBonus::green;
						card.changeBonusColor(b);
						verif_choix = true;
						break;
					case 5:
						b = colorBonus::black;
						card.changeBonusColor(b);
						;
						verif_choix = true;
						break;
					default:
						cout << "Choix invalide, veuillez recommencer.\n";
						break;
					}
				}
			}
		} catch (SplendorException &e) {
			cout << e.getInfo() << "\n";
		}
	}
}

// Surcharge Qt
void Player::applyCapacity_Qt(const JewelryCard &card,
                              StrategyPlayer &opponent) {
	MainWindow::getMainWindow().deactivateButtons();
	if (card.getCapacity().has_value()) {
		std::optional<Capacity> capa = card.getCapacity();
		if (capa == Capacity::steal_opponent_pawn) {
			if (Game::getGame().getOpponent().getTokenNumber() != 0) {
				MainWindow::getMainWindow().deactivateButtons();
				MainWindow::getMainWindow().setStealingJeton(true);

				MainWindow::getMainWindow().triggerInfo(
				    "Utilisation de capacité : vous devez prendre un jeton "
				    "gemme ou "
				    "perl à votre opponent");
				MainWindow::getMainWindow().getJetonWaitLoop()->exec();

				int index = MainWindow::getMainWindow()
				                .getTokenIndexOnClick(); // index à récupérer
				                                         // grace au qt

				qDebug() << index;

				// Ajout chez le player et suppression chez l'opponent
				vector<const Token *> &jetons_adversaire = opponent.getToken();
				tokens.push_back(jetons_adversaire[index]);
				token_number++;
				jetons_adversaire.erase(jetons_adversaire.begin() + index);
				opponent.setNbJetons(opponent.getTokenNumber() - 1);
				MainWindow::getMainWindow().setStealingJeton(false);
			}

		} else if (capa == Capacity::take_priviledge) {
			// ok pas besoin de changer pour le Qt
			MainWindow::getMainWindow().triggerInfo(
			    "Vous allez obtenir un privilège");
			Game::getGame().getCurrentPlayer().obtainPrivilege();
		} else if (capa == Capacity::take_on_board) {
			const optional<colorBonus> &color = card.getBonus();
			if (Board::getBoard().colorsOnBoard(color)) {
				MainWindow::getMainWindow().triggerInfo(
				    "Utilisation de capacité : vous pouvez prendre un jeton " +
				    toString(card.getBonus()) + " sur le board");

				MainWindow::getMainWindow().activateJetonColor(
				    colorBonusToColor(color));

				MainWindow::getMainWindow().getJetonWaitLoop()->exec();
				int index = MainWindow::getMainWindow()
				                .getTokenIndexOnClick(); // index à récupérer
				                                         // grace au qt

				Game::getGame().getCurrentPlayer().drawToken(index);
			}

		} else { // La seule capacité possible est de replay
			// Rien à changer pour le Qt
			MainWindow::getMainWindow().triggerInfo("Vous allez replay");
			Game::getGame().nextRound(1);
		}
	}
	if (card.getBonus() == colorBonus::joker) {
		colorBonus b;
		MainWindow::getMainWindow().triggercolorJoker(&b);
		card.changeBonusColor(b);
	}
}

void Player::applyRoyalCapacity(const RoyalCard &card,
                                StrategyPlayer &opponent) {
	if (card.getCapacity().has_value()) {
		std::optional<Capacity> capa = card.getCapacity();
		if (capa == Capacity::steal_opponent_pawn) {
			cout << "Utilisation de capacité : vous pouvez prendre un jeton "
			        "gemme ou "
			        "perl à votre opponent.\n";

			if (opponent.getToken().empty()) {
				cout << "Dommage votre opponent ne possède pas de jeton "
				        "gemme ou "
				        "perl!"
				     << endl;
			} else {
				vector<const Token *> jetons_adversaire = opponent.getToken();
				cout << "Voici les tokens de votre opponent: " << endl;
				int i = 0;
				for (auto jet : jetons_adversaire) {
					cout << "Indice : " << i++ << ", " << *jet << endl;
				}
				int choice = -1;
				do {
					if (choice != -1) {
						cout << "Vous ne pouvez pas prendre un jeton or!"
						     << endl;
					}
					cout << "Quel jeton souhaitez vous lui voler?" << endl;
					cout << "Choix : ";
					cin >> choice;
				} while (jetons_adversaire[choice]->getColor() == Color::gold);
				tokens.push_back(jetons_adversaire[choice]);
				token_number++;
				jetons_adversaire.erase(jetons_adversaire.begin() + choice);
				opponent.setNbJetons(opponent.getTokenNumber() - 1);
			}
		} else if (capa == Capacity::take_priviledge) {
			Game::getGame().getCurrentPlayer().obtainPrivilege();
			cout << "Capacité de la card: Vous avez obtenu un privilège!"
			     << endl;
		} else {
			Game::getGame().nextRound(1);
			cout << "Capacité de la card: Rejouer! Vous allez recommencer"
			     << endl;
		}
	}
}

// Surcharge Qt
void Player::applyRoyalCapacity_Qt(const RoyalCard &card,
                                   StrategyPlayer &opponent) {
	if (card.getCapacity().has_value()) {
		std::optional<Capacity> capa = card.getCapacity();
		if (capa == Capacity::steal_opponent_pawn) {
			if (Game::getGame().getOpponent().getTokenNumber() != 0) {
				MainWindow::getMainWindow().deactivateButtons();
				MainWindow::getMainWindow().setStealingJeton(true);

				MainWindow::getMainWindow().triggerInfo(
				    "Utilisation de capacité : vous devez prendre un jeton "
				    "gemme ou "
				    "perl à votre opponent");
				MainWindow::getMainWindow().getJetonWaitLoop()->exec();

				int index = MainWindow::getMainWindow()
				                .getTokenIndexOnClick(); // index à récupérer
				                                         // grace au qt

				qDebug() << index;

				// Ajout chez le player et suppression chez l'opponent
				vector<const Token *> &jetons_adversaire = opponent.getToken();
				tokens.push_back(jetons_adversaire[index]);
				token_number++;
				jetons_adversaire.erase(jetons_adversaire.begin() + index);
				opponent.setNbJetons(opponent.getTokenNumber() - 1);
				MainWindow::getMainWindow().setStealingJeton(false);
			}
		} else if (capa == Capacity::take_priviledge) {
			// Rien à changer dans la Qt
			MainWindow::getMainWindow().triggerInfo(
			    "Vous obtenez un privilège");
			Game::getGame().getCurrentPlayer().obtainPrivilege();
			cout << "Capacité de la card: Vous avez obtenu un privilège!"
			     << endl;
		} else {
			MainWindow::getMainWindow().triggerInfo("Vous allez replay");
			Game::getGame().nextRound(1);
			cout << "Capacité de la card: Rejouer! Vous allez recommencer"
			     << endl;
		}
	}
}

void Player::cardPurchase() {
	unsigned int choice = -1;
	if (Game::getGame().getCurrentPlayer().getReservedCardNumber() != 0) {

		while (choice != 1 && choice != 0) {
			cout << "Voulez vous acheter une card que vous avez réserver "
			        "auparavant "
			        "? 1 pour oui /0 pour non"
			     << endl;
			cout << "choices";
			cin >> choice;
		}
		if (choice == 1) {
			// alors on doit lui print les cards qu'il peut acheter (celles
			// qu'il a déjà reservé)
			vector<const JewelryCard *> &reserved =
			    Game::getGame().getCurrentPlayer().getReservedCards();
			for (int i = 0; i < reserved.size(); ++i) {
				cout << "index : " << i << " " << *reserved[i] << endl;
			}
			unsigned int index;
			cout << "veuillez renseigner l'index de la card choisie : ";
			cin >> index;
			Game::getGame().getCurrentPlayer().buyReservedCard(index);
		} else {
			cout << "\n\nTirage1 :" << endl;
			cout << *Game::getGame().getFirstDraw() << endl;
			cout << "\nTirage2 :" << endl;
			cout << *Game::getGame().getSecondDraw() << endl;
			cout << "\nTirage3 :" << endl;
			cout << *Game::getGame().getThirdDraw() << endl;
			int choices = 0;
			while (choices != 1 && choices != 2 && choices != 3) {
				cout << "Dans quel draw vous voulez acheter une card ?" << endl;
				cout << "choices";
				cin >> choices;
			}
			switch (choices) {
			case 1: {
				unsigned int index = 0;
				cout << "Veuillez renseigner l'index de la card que vous "
				        "voulez "
				        "acheter ! "
				     << endl;
				cout << "choices : ";
				cin >> index;
				Game::getGame().getCurrentPlayer().buyCard(
				    Game::getGame().getFirstDraw(), index);
				break;
			}
			case 2: {
				unsigned int index = 0;
				cout << "Veuillez renseigner l'index de la card que vous "
				        "voulez "
				        "acheter ! "
				     << endl;
				cout << "choices : ";
				cin >> index;
				Game::getGame().getCurrentPlayer().buyCard(
				    Game::getGame().getSecondDraw(), index);
				break;
			}
			case 3: {
				unsigned int index = 0;
				cout << "Veuillez renseigner l'index de la card que vous "
				        "voulez "
				        "acheter ! "
				     << endl;
				cout << "choices : ";
				cin >> index;
				Game::getGame().getCurrentPlayer().buyCard(
				    Game::getGame().getThirdDraw(), index);
				break;
			}
			}
		}
	} else {
		cout << "\n\nTirage1 :" << endl;
		cout << *Game::getGame().getFirstDraw() << endl;
		cout << "\nTirage2 :" << endl;
		cout << *Game::getGame().getSecondDraw() << endl;
		cout << "\nTirage3 :" << endl;
		cout << *Game::getGame().getThirdDraw() << endl;
		int choices = 0;
		while (choices != 1 && choices != 2 && choices != 3) {
			cout << "Dans quel draw vous voulez acheter une card ?" << endl;
			cout << "choices";
			cin >> choices;
		}
		switch (choices) {
		case 1: {
			unsigned int index = 0;
			cout << "Veuillez renseigner l'index de la card que vous voulez "
			        "acheter ! "
			     << endl;
			cout << "choices : ";
			cin >> index;
			Game::getGame().getCurrentPlayer().buyCard(
			    Game::getGame().getFirstDraw(), index);
			break;
		}
		case 2: {
			unsigned int index = 0;
			cout << "Veuillez renseigner l'index de la card que vous voulez "
			        "acheter ! "
			     << endl;
			cout << "choices : ";
			cin >> index;
			Game::getGame().getCurrentPlayer().buyCard(
			    Game::getGame().getSecondDraw(), index);
			break;
		}
		case 3: {
			unsigned int index = 0;
			cout << "Veuillez renseigner l'index de la card que vous voulez "
			        "acheter ! "
			     << endl;
			cout << "choices : ";
			cin >> index;
			Game::getGame().getCurrentPlayer().buyCard(
			    Game::getGame().getFirstDraw(), index);
			break;
		}
		}
	}
}

// Surcharge Qt
void Player::cardPurchase_Qt() {
	MainWindow::getMainWindow().setBuyingCard(true);
	// qDebug() << "Carte qt";
	MainWindow::getMainWindow().activateForBuy();
	MainWindow::getMainWindow().getCarteWaitLoop()->exec();

	Qt_carte *last_card_clicked =
	    MainWindow::getMainWindow().getLastCardClicked();
	qDebug() << last_card_clicked->getIndex()
	         << last_card_clicked->getReserved()
	         << last_card_clicked->getCard()->getVisual();

	// Carte reservées
	if (last_card_clicked->getReserved() == true) {
		qDebug() << "CARTE EFFECTIVEMENT R";
		Game::getGame().getCurrentPlayer().buyReservedCard_Qt(
		    last_card_clicked->getIndex());
	}

	// Carte dans draw
	else {
		qDebug() << "CARTE PAS EFFECTIVEMENT R";
		const JewelryCard *c =
		    dynamic_cast<const JewelryCard *>(last_card_clicked->getCard());
		int niveau_tirage = c->getLevel();
		Draw *draw = Game::getGame().getDrawById(niveau_tirage);
		Game::getGame().getCurrentPlayer().buyCard_Qt(
		    draw, last_card_clicked->getIndex());
	}
	MainWindow::getMainWindow().setBuyingCard(false);
	last_card_clicked->setReservee(false);
}

void Player::buyCard(Draw *t, const int index) {

	// la card qu'il veut supp c'est la ième du draw t

	const JewelryCard &card = t->getCardWithoutDeletion(index);
	if (card.getBonus() == colorBonus::joker and
	    jokerCardEligibility() == false) {
		throw SplendorException("Vous n'avez pas le droit d'acheter une card "
		                        "avec un bonus joker car "
		                        "vous ne pouvez pas assigner le bonus!");
	}

	// ici calculer bonus permet de retirer du cout total des cards le bonus
	// des cards déjà possédées.
	int white_cost =
	    positiveOrNull(card.getCostWhite() - calculateBonus(colorBonus::white));
	int blue_cost =
	    positiveOrNull(card.getCostBlue() - calculateBonus(colorBonus::blue));
	int red_cost =
	    positiveOrNull(card.getCostRed() - calculateBonus(colorBonus::red));
	int green_cost =
	    positiveOrNull(card.getCostGreen() - calculateBonus(colorBonus::green));
	int black_cost =
	    positiveOrNull(card.getCostBlack() - calculateBonus(colorBonus::black));
	int perl_cost = card.getCostPerl();

	// Vérifier si le player veut utiliser des tokens en or (s'il en possède)
	// Et diminuer le coût respectivement
	int nb_gold = 0;
	if (TokenAmount(Color::gold) > 0) {
		std::cout << "Voulez-vous utiliser un(des) jeton(s) or? [Y/N]"
		          << std::endl;
		std::string choices;
		std::cin >> choices;
		int nb;
		if (choices == "Y") {
			while (TokenAmount(Color::gold) > nb_gold) {
				// choices de la color
				std::cout << "Couleur remplacée? Stop pour arrêter d'utiliser "
				             "des tokens or:"
				          << std::endl;
				std::cin >> choices;
				if (choices == "stop" || "Stop") {
					break;
				}
				do {
					// choices du nombre de tokens or utilisé pour la color en
					// question
					std::cout << "Nombre?" << std::endl;
					std::cin >> nb;
				} while (nb > TokenAmount(Color::gold));
				try {
					if (nb > TokenAmount(Color::gold) - nb_gold)
						throw SplendorException(
						    "Vous n'avez pas asseez de tokens or pour en "
						    "dépenser autant!");
					if ((choices == "white" || "White") && (nb > white_cost))
						throw SplendorException(
						    "Le cout white est inférieur au nombre de tokens "
						    "or que vous "
						    "souhaitez utiliser en tant que joker!");
					if ((choices == "blue" || "Blue") && (nb > blue_cost))
						throw SplendorException(
						    "Le cout blue est inférieur au nombre de tokens or "
						    "que vous "
						    "souhaitez utiliser en tant que joker!");
					if ((choices == "red" || "Red") && (nb > red_cost))
						throw SplendorException(
						    "Le cout red est inférieur au nombre de tokens "
						    "or que vous "
						    "souhaitez utiliser en tant que joker!");
					if ((choices == "green" || "Green") && (nb > green_cost))
						throw SplendorException(
						    "Le cout green est inférieur au nombre de tokens "
						    "or "
						    "que vous "
						    "souhaitez utiliser en tant que joker!");
					if ((choices == "black" || "Black") && (nb > black_cost))
						throw SplendorException(
						    "Le cout black est inférieur au nombre de tokens "
						    "or "
						    "que vous "
						    "souhaitez utiliser en tant que joker!");
					if ((choices == "perl" || "Perle") && (nb > white_cost))
						throw SplendorException(
						    "Le cout perl est inférieur au nombre de tokens "
						    "or que vous "
						    "souhaitez utiliser en tant que joker!");
					if (choices != "white" || "White" || "blue" || "Blue" ||
					    "red" || "Red" || "green" || "Green" || "black" ||
					    "Black" || "perl" || "Perle")
						throw SplendorException("Couleur de jeton inconnue!");

					// Si on a passé tous ces tests on peut utiliser les tokens
					// or
					nb_gold += nb;
					if (choices == "white" || "White")
						white_cost = positiveOrNull(white_cost - nb);
					if (choices == "blue" || "Blue")
						blue_cost = positiveOrNull(blue_cost - nb);
					if (choices == "red" || "Red")
						red_cost = positiveOrNull(red_cost - nb);
					if (choices == "green" || "Green")
						green_cost = positiveOrNull(green_cost - nb);
					if (choices == "black" || "Black")
						black_cost = positiveOrNull(black_cost - nb);
					if (choices == "perl" || "Perle")
						perl_cost = positiveOrNull(perl_cost - nb);
				} catch (SplendorException e) {
					cout << e.getInfo() << endl;
				}
			}
		}
	}
	// Vérifier si assez de tokens
	int eligible_purchase = 0;

	// vérifier si on a le nombre de tokens pour acheter
	if (TokenAmount(Color::white) >= white_cost &&
	    TokenAmount(Color::blue) >= blue_cost &&
	    TokenAmount(Color::red) >= red_cost &&
	    TokenAmount(Color::green) >= green_cost &&
	    TokenAmount(Color::black) >= black_cost &&
	    TokenAmount(Color::perl) >= perl_cost) {
		eligible_purchase = 1;
	}

	if (eligible_purchase == 0)
		throw SplendorException("Pas assez de tokens pour acheter la card !");

	// Retirer les tokens utilisés et les mettre dans le bag
	withdrawTokens(Color::white, white_cost);
	withdrawTokens(Color::blue, blue_cost);
	withdrawTokens(Color::red, red_cost);
	withdrawTokens(Color::green, green_cost);
	withdrawTokens(Color::black, black_cost);
	withdrawTokens(Color::perl, perl_cost);
	withdrawTokens(Color::gold, nb_gold);

	// Mettre la card dans la main du player et la supprimer du draw
	bought_jewelry_cards.push_back(&(t->getCard(index)));
	try {
		t->fill();
	} catch (SplendorException &e) {
	}

	// Rajouter le nb de couronnes
	crown_number += card.getNbCrown();
	points_number += card.getPrestige();
	jewelry_cards_number++;
	// Dans le main tester si eligible pour card royale et appeler get card
	// royale

	Game::getGame().getCurrentPlayer().applyCapacity(
	    card, Game::getGame().getOpponent());
}

void Player::buyCard_Qt(Draw *t, const int index) {

	// la card qu'il veut supp c'est la ième du draw t

	const JewelryCard &card = t->getCardWithoutDeletion(index);
	if (card.getBonus() == colorBonus::joker &&
	    jokerCardEligibility() == false) {
		throw SplendorException("Vous n'avez pas le droit d'acheter une card "
		                        "avec un bonus joker car "
		                        "vous ne pouvez pas assigner le bonus!");
	}

	// ici calculer bonus permet de retirer du cout total des cards le bonus
	// des cards déjà possédées.
	int white_cost =
	    positiveOrNull(card.getCostWhite() - calculateBonus(colorBonus::white));
	int blue_cost =
	    positiveOrNull(card.getCostBlue() - calculateBonus(colorBonus::blue));
	int red_cost =
	    positiveOrNull(card.getCostRed() - calculateBonus(colorBonus::red));
	int green_cost =
	    positiveOrNull(card.getCostGreen() - calculateBonus(colorBonus::green));
	int black_cost =
	    positiveOrNull(card.getCostBlack() - calculateBonus(colorBonus::black));
	int perl_cost = card.getCostPerl();

	// Vérifier si le player veut utiliser des tokens en or (s'il en possède)
	// Et diminuer le coût respectivement
	int nb_gold = 0;
	char choices;
	if (TokenAmount(Color::gold) > 0) {

		MainWindow::getMainWindow().triggerYesNo(
		    &choices, "Voulez-vous utiliser un(des) jeton(s) or?");

		int nb;
		while ((TokenAmount(Color::gold) > nb_gold) && (choices == 'Y')) {
			// choices de la color
			//  recup les 2 val
			Color color;

			MainWindow::getMainWindow().triggercolorChoice(&color, &nb);

			try {
				if (nb > TokenAmount(Color::gold) - nb_gold)
					throw SplendorException("Vous n'avez pas asseez de tokens "
					                        "or pour en dépenser autant!");
				if ((color == Color::white) && (nb > white_cost))
					throw SplendorException(
					    "Le cout white est inférieur au nombre de tokens or "
					    "que vous "
					    "souhaitez utiliser en tant que joker!");
				if ((color == Color::blue) && (nb > blue_cost))
					throw SplendorException(
					    "Le cout blue est inférieur au nombre de tokens or que "
					    "vous "
					    "souhaitez utiliser en tant que joker!");
				if ((color == Color::red) && (nb > red_cost))
					throw SplendorException(
					    "Le cout red est inférieur au nombre de tokens or "
					    "que vous "
					    "souhaitez utiliser en tant que joker!");
				if ((color == Color::green) && (nb > green_cost))
					throw SplendorException(
					    "Le cout green est inférieur au nombre de tokens or "
					    "que "
					    "vous "
					    "souhaitez utiliser en tant que joker!");
				if ((color == Color::black) && (nb > black_cost))
					throw SplendorException(
					    "Le cout black est inférieur au nombre de tokens or "
					    "que "
					    "vous "
					    "souhaitez utiliser en tant que joker!");
				if ((color == Color::perl) && (nb > white_cost))
					throw SplendorException(
					    "Le cout perl est inférieur au nombre de tokens or "
					    "que vous "
					    "souhaitez utiliser en tant que joker!");
				if ((color != Color::white) && (color != Color::blue) &&
				    (color != Color::red) && (color != Color::green) &&
				    (color != Color::black) && (color != Color::perl))
					throw SplendorException("Couleur de jeton!");

				// Si on a passé tous ces tests, on peut utiliser les tokens or
				nb_gold += nb;
				if (color == Color::white)
					white_cost = positiveOrNull(white_cost - nb);
				if (color == Color::blue)
					blue_cost = positiveOrNull(blue_cost - nb);
				if (color == Color::red)
					red_cost = positiveOrNull(red_cost - nb);
				if (color == Color::green)
					green_cost = positiveOrNull(green_cost - nb);
				if (color == Color::black)
					black_cost = positiveOrNull(black_cost - nb);
				if (color == Color::perl)
					perl_cost = positiveOrNull(perl_cost - nb);
			} catch (SplendorException e) {
				MainWindow::getMainWindow().triggerInfo(e.getInfo());
				cout << e.getInfo() << endl;
			}
			if (TokenAmount(Color::gold) > nb_gold)
				MainWindow::getMainWindow().triggerYesNo(
				    &choices, "Utiliser encore un jeton or ?");
		}
	}
	// Vérifier si assez de tokens
	int eligible_purchase = 0;

	// vérifier si on a le nombre de tokens pour acheter
	if (TokenAmount(Color::white) >= white_cost &&
	    TokenAmount(Color::blue) >= blue_cost &&
	    TokenAmount(Color::red) >= red_cost &&
	    TokenAmount(Color::green) >= green_cost &&
	    TokenAmount(Color::black) >= black_cost &&
	    TokenAmount(Color::perl) >= perl_cost) {
		eligible_purchase = 1;
	}

	if (eligible_purchase == 0)
		throw SplendorException("Pas assez de tokens pour acheter la card !");

	// Retirer les tokens utilisés et les mettre dans le bag
	withdrawTokens(Color::white, white_cost);
	withdrawTokens(Color::blue, blue_cost);
	withdrawTokens(Color::red, red_cost);
	withdrawTokens(Color::green, green_cost);
	withdrawTokens(Color::black, black_cost);
	withdrawTokens(Color::perl, perl_cost);
	withdrawTokens(Color::gold, nb_gold);

	// Mettre la card dans la main du player et la supprimer du draw
	bought_jewelry_cards.push_back(&(t->getCard(index)));
	try {
		t->fill();
	} catch (SplendorException &e) {
	}

	// Rajouter le nb de couronnes
	crown_number += card.getNbCrown();
	points_number += card.getPrestige();
	jewelry_cards_number++;
	// Dans le main tester si eligible pour card royale et appeler get card
	// royale

	Game::getGame().getCurrentPlayer().applyCapacity_Qt(
	    card, Game::getGame().getOpponent());
	MainWindow::getMainWindow().updateDraws();
}

void Player::buyReservedCard(const int index) {
	if (reserved_jewelry_cards.size() == 0 || index > 3) {
		throw SplendorException("Pas de cards réservées");
	}

	// on doit vérifier que l'achat peut se faire

	const JewelryCard *card = reserved_jewelry_cards[index];

	if (card->getBonus() == colorBonus::joker and
	    jokerCardEligibility() == false) {
		throw SplendorException("Vous n'avez pas le droit d'acheter une card "
		                        "avec un bonus joker car "
		                        "vous ne pouvez pas assigner le bonus!");
	}

	int white_cost = positiveOrNull(card->getCostWhite() -
	                                calculateBonus(colorBonus::white));
	int blue_cost =
	    positiveOrNull(card->getCostBlue() - calculateBonus(colorBonus::blue));
	int red_cost =
	    positiveOrNull(card->getCostRed() - calculateBonus(colorBonus::red));
	int green_cost = positiveOrNull(card->getCostGreen() -
	                                calculateBonus(colorBonus::green));
	int black_cost = positiveOrNull(card->getCostBlack() -
	                                calculateBonus(colorBonus::black));
	int perl_cost = card->getCostPerl();

	int nb_gold = 0;
	if (TokenAmount(Color::gold) > 0) {
		std::cout << "Voulez-vous utiliser un(des) jeton(s) or? [Y/N]"
		          << std::endl;
		std::string choices;
		std::cin >> choices;
		int nb;
		if (choices == "Y") {
			while (TokenAmount(Color::gold) > nb_gold) {
				// choices de la color
				std::cout << "Couleur remplacée? Stop pour arrêter d'utiliser "
				             "des tokens or:"
				          << std::endl;
				std::cin >> choices;
				if (choices == "stop" || "Stop") {
					break;
				}
				do {
					// choices du nombre de tokens or utilisé pour la color en
					// question
					std::cout << "Nombre?" << std::endl;
					std::cin >> nb;
				} while (nb > TokenAmount(Color::gold));
				try {
					if (nb > TokenAmount(Color::gold) - nb_gold)
						throw SplendorException(
						    "Vous n'avez pas asseez de tokens or pour en "
						    "dépenser autant!");
					if ((choices == "white" || "White") && (nb > white_cost))
						throw SplendorException(
						    "Le cout white est inférieur au nombre de tokens "
						    "or que vous "
						    "souhaitez utiliser en tant que joker!");
					if ((choices == "blue" || "Blue") && (nb > blue_cost))
						throw SplendorException(
						    "Le cout blue est inférieur au nombre de tokens or "
						    "que vous "
						    "souhaitez utiliser en tant que joker!");
					if ((choices == "red" || "Red") && (nb > red_cost))
						throw SplendorException(
						    "Le cout red est inférieur au nombre de tokens "
						    "or que vous "
						    "souhaitez utiliser en tant que joker!");
					if ((choices == "green" || "Green") && (nb > green_cost))
						throw SplendorException(
						    "Le cout green est inférieur au nombre de tokens "
						    "or "
						    "que vous "
						    "souhaitez utiliser en tant que joker!");
					if ((choices == "black" || "Black") && (nb > black_cost))
						throw SplendorException(
						    "Le cout black est inférieur au nombre de tokens "
						    "or "
						    "que vous "
						    "souhaitez utiliser en tant que joker!");
					if ((choices == "perl" || "Perle") && (nb > white_cost))
						throw SplendorException(
						    "Le cout perl est inférieur au nombre de tokens "
						    "or que vous "
						    "souhaitez utiliser en tant que joker!");
					if (choices != "white" || "White" || "blue" || "Blue" ||
					    "red" || "Red" || "green" || "Green" || "black" ||
					    "Black" || "perl" || "Perle")
						throw SplendorException("Couleur de jeton inconnue!");

					// Si on a passé tous ces tests, on peut utiliser les tokens
					// or
					nb_gold += nb;
					if (choices == "white" || "White")
						white_cost = positiveOrNull(white_cost - nb);
					if (choices == "blue" || "Blue")
						blue_cost = positiveOrNull(blue_cost - nb);
					if (choices == "red" || "Red")
						red_cost = positiveOrNull(red_cost - nb);
					if (choices == "green" || "Green")
						green_cost = positiveOrNull(green_cost - nb);
					if (choices == "black" || "Black")
						black_cost = positiveOrNull(black_cost - nb);
					if (choices == "perl" || "Perle")
						perl_cost = positiveOrNull(perl_cost - nb);
				} catch (SplendorException e) {
					cout << e.getInfo() << endl;
				}
			}
		}
	}

	// Vérifier si assez de tokens
	int eligible_purchase = 0;

	// vérifier si on a le nombre de tokens pour acheter
	if (TokenAmount(Color::white) >= white_cost &&
	    TokenAmount(Color::blue) >= blue_cost &&
	    TokenAmount(Color::red) >= red_cost &&
	    TokenAmount(Color::green) >= green_cost &&
	    TokenAmount(Color::black) >= black_cost &&
	    TokenAmount(Color::perl) >= perl_cost) {
		eligible_purchase = 1;
	}

	if (eligible_purchase == 0)
		throw SplendorException("Pas assez de tokens pour acheter la card !");

	// Retirer les tokens utilisés et les mettre dans le bag
	withdrawTokens(Color::white, white_cost);
	withdrawTokens(Color::blue, blue_cost);
	withdrawTokens(Color::red, red_cost);
	withdrawTokens(Color::green, green_cost);
	withdrawTokens(Color::black, black_cost);
	withdrawTokens(Color::perl, perl_cost);
	withdrawTokens(Color::gold, nb_gold);

	reserved_jewelry_cards.erase(reserved_jewelry_cards.begin() + index);
	reserved_jewelry_cards_number--;
	bought_jewelry_cards.push_back(card);
	jewelry_cards_number++;
	crown_number += card->getNbCrown();
	points_number += card->getPrestige();

	Game::getGame().getCurrentPlayer().applyCapacity(
	    *card, Game::getGame().getOpponent());
}

void Player::buyReservedCard_Qt(const int index) {
	if (reserved_jewelry_cards.size() == 0 || index > 3) {
		throw SplendorException("Pas de cards réservées");
	}

	// on doit vérifier que l'achat peut se faire

	const JewelryCard *card = reserved_jewelry_cards[index];

	if (card->getBonus() == colorBonus::joker and
	    jokerCardEligibility() == false) {
		throw SplendorException("Vous n'avez pas le droit d'acheter une card "
		                        "avec un bonus joker car "
		                        "vous ne pouvez pas assigner le bonus!");
	}

	int white_cost = positiveOrNull(card->getCostWhite() -
	                                calculateBonus(colorBonus::white));
	int blue_cost =
	    positiveOrNull(card->getCostBlue() - calculateBonus(colorBonus::blue));
	int red_cost =
	    positiveOrNull(card->getCostRed() - calculateBonus(colorBonus::red));
	int green_cost = positiveOrNull(card->getCostGreen() -
	                                calculateBonus(colorBonus::green));
	int black_cost = positiveOrNull(card->getCostBlack() -
	                                calculateBonus(colorBonus::black));
	int perl_cost = card->getCostPerl();

	int nb_gold = 0;
	if (TokenAmount(Color::gold) > 0) {

		char choices;
		MainWindow::getMainWindow().triggerYesNo(
		    &choices, "Voulez-vous utiliser un(des) jeton(s) or?");

		int nb;

		while ((TokenAmount(Color::gold) > nb_gold) && (choices == 'Y')) {
			// choices de la color
			//  recup les 2 val
			Color color;

			MainWindow::getMainWindow().triggercolorChoice(&color, &nb);

			try {
				if (nb > TokenAmount(Color::gold) - nb_gold)
					throw SplendorException("Vous n'avez pas asseez de tokens "
					                        "or pour en dépenser autant!");
				if ((color == Color::white) && (nb > white_cost))
					throw SplendorException(
					    "Le cout white est inférieur au nombre de tokens or "
					    "que vous "
					    "souhaitez utiliser en tant que joker!");
				if ((color == Color::blue) && (nb > blue_cost))
					throw SplendorException(
					    "Le cout blue est inférieur au nombre de tokens or que "
					    "vous "
					    "souhaitez utiliser en tant que joker!");
				if ((color == Color::red) && (nb > red_cost))
					throw SplendorException(
					    "Le cout red est inférieur au nombre de tokens or "
					    "que vous "
					    "souhaitez utiliser en tant que joker!");
				if ((color == Color::green) && (nb > green_cost))
					throw SplendorException(
					    "Le cout green est inférieur au nombre de tokens or "
					    "que "
					    "vous "
					    "souhaitez utiliser en tant que joker!");
				if ((color == Color::black) && (nb > black_cost))
					throw SplendorException(
					    "Le cout black est inférieur au nombre de tokens or "
					    "que "
					    "vous "
					    "souhaitez utiliser en tant que joker!");
				if ((color == Color::perl) && (nb > white_cost))
					throw SplendorException(
					    "Le cout perl est inférieur au nombre de tokens or "
					    "que vous "
					    "souhaitez utiliser en tant que joker!");
				if ((color != Color::white) && (color != Color::blue) &&
				    (color != Color::red) && (color != Color::green) &&
				    (color != Color::black) && (color != Color::perl))
					throw SplendorException("Couleur de jeton!");

				// Si on a passé tous ces tests, on peut utiliser les tokens or
				nb_gold += nb;
				if (color == Color::white)
					white_cost = positiveOrNull(white_cost - nb);
				if (color == Color::blue)
					blue_cost = positiveOrNull(blue_cost - nb);
				if (color == Color::red)
					red_cost = positiveOrNull(red_cost - nb);
				if (color == Color::green)
					green_cost = positiveOrNull(green_cost - nb);
				if (color == Color::black)
					black_cost = positiveOrNull(black_cost - nb);
				if (color == Color::perl)
					perl_cost = positiveOrNull(perl_cost - nb);
			} catch (SplendorException e) {
				MainWindow::getMainWindow().triggerInfo(e.getInfo());
				cout << e.getInfo() << endl;
			}
			if (TokenAmount(Color::gold) > nb_gold)
				MainWindow::getMainWindow().triggerYesNo(
				    &choices, "Utiliser encore un jeton or ?");
		}
	}

	// Vérifier si assez de tokens
	int eligible_purchase = 0;

	// vérifier si on a le nombre de tokens pour acheter
	if (TokenAmount(Color::white) >= white_cost &&
	    TokenAmount(Color::blue) >= blue_cost &&
	    TokenAmount(Color::red) >= red_cost &&
	    TokenAmount(Color::green) >= green_cost &&
	    TokenAmount(Color::black) >= black_cost &&
	    TokenAmount(Color::perl) >= perl_cost) {
		eligible_purchase = 1;
	}

	if (eligible_purchase == 0)
		throw SplendorException("Pas assez de tokens pour acheter la card !");

	// Retirer les tokens utilisés et les mettre dans le bag
	withdrawTokens(Color::white, white_cost);
	withdrawTokens(Color::blue, blue_cost);
	withdrawTokens(Color::red, red_cost);
	withdrawTokens(Color::green, green_cost);
	withdrawTokens(Color::black, black_cost);
	withdrawTokens(Color::perl, perl_cost);
	withdrawTokens(Color::gold, nb_gold);

	reserved_jewelry_cards.erase(reserved_jewelry_cards.begin() + index);
	reserved_jewelry_cards_number--;
	bought_jewelry_cards.push_back(card);
	jewelry_cards_number++;
	crown_number += card->getNbCrown();
	points_number += card->getPrestige();

	Game::getGame().getCurrentPlayer().applyCapacity_Qt(
	    *card, Game::getGame().getOpponent());
}

void Player::royalCardSelection() {
	cout << "Votre nombre de couronne vous donne le droit de piocher une card "
	        "royale!"
	     << endl;
	Game::getGame().printRoyalCards();
	int tmp;
	bool choix_ok = 0;
	while (!choix_ok) {
		cout << "Veuillez entrer l'index de la card royale que vous souhaitez "
		        "prendre:"
		     << endl;
		cout << "choices: ";
		cin >> tmp;
		if (tmp >= 0 and tmp < Game::getGame().getRoyalCards().size()) {
			string validate;
			cout << "Valider ? [Y/N] :" << endl;
			cout << "choices : ";
			cin >> validate;
			if (validate == "Y") {
				choix_ok = 1; // on peut sortir de la boucle
			}
		} else {
			cout << "Indice de card invalide!" << endl;
		}
	}
	obtainRoyalCard(tmp);
}

// Surcharge Qt

void Player::royalCardSelection_Qt() {

	MainWindow::getMainWindow().activateForRoyalCard();
	MainWindow::getMainWindow().getCarteWaitLoop()->exec();

	Qt_carte *last_card_clicked =
	    MainWindow::getMainWindow().getLastCardClicked();

	int tmp =
	    last_card_clicked
	        ->getIndex(); // Récupération de l'index à l'aide d'un signal ici

	obtainRoyalCard_qt(tmp);
}

void Player::tokenVerification() {
	if (token_number > 10) {
		int nb = token_number - 10;
		cout << "Vous avez trop de tokens, vous devez en remettre " << nb
		     << " dans le bag." << endl;
		cout << "Voici vos tokens:" << endl;
		for (int i = 0; i < token_number; i++) {
			cout << "Indice " << i << " : " << *tokens[i] << endl;
		}
		vector<int> tab(0);
		int i = 0;
		int curNb = nb;
		while (curNb > 0) {
			int tmp;
			cout << "Indice du jeton " << i++ << " :" << endl;
			cout << "Choix : ";
			cin >> tmp;
			bool inTab = 0;
			for (int j = 0; j < tab.size();
			     j++) { // vérification qu'on a pas déjà essayé de l'enlever
				if (tab[j] == tmp) {
					inTab = 1;
				}
			}
			if (tmp >= 0 and tmp < token_number and inTab == false) {
				tab.push_back(tmp);
				curNb--;
			}
		}
		// tri du tableau
		std::make_heap(tab.begin(), tab.end());
		std::sort_heap(tab.begin(), tab.end());
		for (int k = tab.size() - 1; k >= 0;
		     k--) { // suppression de la fin vers le début pour ne pas décaler
			        // les indices
			Bag::get().insertToken(tokens[tab[k]]);
			tokens.erase(tokens.begin() + tab[k]);
			token_number--;
		}
	}
}

void Player::tokenVerification_Qt() {
	if (token_number > 10) {
		MainWindow::getMainWindow().setDiscarding(true);
		MainWindow::getMainWindow().deactivateButtons();
		int nb = token_number - 10;
		MainWindow::getMainWindow().triggerInfo("Vous devez enlever " +
		                                        std::to_string(nb) + " tokens");

		vector<int> tab(0);
		int i = 0;
		int curNb = nb;
		while (curNb > 0) {

			MainWindow::getMainWindow().getJetonWaitLoop()->exec();
			int tmp = MainWindow::getMainWindow().getTokenIndexOnClick();

			bool inTab = 0;
			for (int j = 0; j < tab.size();
			     j++) { // vérification qu'on a pas déjà essayé de l'enlever
				if (tab[j] == tmp) {
					inTab = 1;
				}
			}
			if (tmp >= 0 and tmp < token_number and inTab == false) {
				tab.push_back(tmp);
				curNb--;
			}
		}
		MainWindow::getMainWindow().acceptCurrentDialog();
		// tri du tableau
		std::make_heap(tab.begin(), tab.end());
		std::sort_heap(tab.begin(), tab.end());
		for (int k = tab.size() - 1; k >= 0;
		     k--) { // suppression de la fin vers le début pour ne pas décaler
			        // les indices
			Bag::get().insertToken(tokens[tab[k]]);
			tokens.erase(tokens.begin() + tab[k]);
			token_number--;
		}
	}

	MainWindow::getMainWindow().setDiscarding(false);
}
/******************** Player ********************/

/******************** RandomPlayer - aka IA ********************/

RandomPlayer::RandomPlayer(const string &name) : StrategyPlayer(name) {}

RandomPlayer::RandomPlayer(const json data) : StrategyPlayer(data) {}

RandomPlayer::~RandomPlayer() {
	// Déstruction cards royales
	for (auto royal_cards : royal_cards) {
		delete royal_cards;
	}
	royal_cards.clear();

	// Déstruction privilèges
	for (auto privilege : privileges) {
		delete privilege;
	}
	privileges.clear();
}

// Méthodes polymorphiques adaptées pour un RandomPlayer
void RandomPlayer::choice() {
	cout << "choices RandomPlayer:" << endl;
	bool end_choice = 0;
	int nb_choice = 0;
	while (!end_choice) {
		try {
			int i = 0;
			nb_choice =
			    getOptionalChoices(); // bien vérifier la convention sur le
			                          // retour dans la définition de la méthode
			i += nb_choice + 2;
			int tmp = rand() % i + 1;

			if (tmp < 1 or tmp > i + 1) {
				throw SplendorException(
				    "Il n'y a que" + to_string(i) +
				    " choices! Vous ne pouvez pas choisir autre chose!\n");
			}
			switch (nb_choice) { // l'affichage et donc le choices dépend de la
				                 // valeur de retour des choices optionnels
			case (0): {          // aucun choices optionnel possible
				switch (tmp) {
				case 1: {
					cout << "Le RandomPlayer sélectionne des tokens!" << endl;
					tokenSelection();
					end_choice = 1;
					break;
				}
				case 2: {
					cout << "Le RandomPlayer achète des cards!" << endl;
					cardPurchase();
					end_choice = 1;

					break;
				}
				default: // on continue jusqu'à ce que l'utilisateur choisisse
				         // une entrée valide!
					break;
				}
				break;
			}
			case (1): { // seulement possible d'utiliser un privilège
				switch (tmp) {
				case 1: {
					cout << "Le RandomPlayer utilise un privilège!" << endl;
					usePrivilege();
					break;
				}
				case 2: {
					cout << "Le RandomPlayer sélectionne des tokens!" << endl;
					tokenSelection();
					end_choice = 1;
					break;
				}
				case 3: {
					cout << "Le RandomPlayer achète des cards!" << endl;
					cardPurchase();
					end_choice = 1;
					break;
				}
				default:
					break;
				}
				break;
			}
			case (2): { // seulement possible de remplir le board
				switch (tmp) {
				case 1: { // remplissage board
					cout << "Le RandomPlayer remplie le board!" << endl;
					fillingBoard();
					break;
				}
				case 2: {
					cout << "Le RandomPlayer sélectionne des tokens!" << endl;
					tokenSelection();
					end_choice = 1;
					break;
				}
				case 3: {
					cout << "Le RandomPlayer achète des cards!" << endl;
					cardPurchase();
					end_choice = 1;
					break;
				}
				default:
					break;
				}
				break;
			}
			case (3): { // deux choices optionnels possibles
				switch (tmp) {
				case 1: {
					cout << "Le RandomPlayer utilise un privilège!" << endl;
					usePrivilege();
					break;
				}
				case 2: { // remplissage board
					cout << "Le RandomPlayer remplie le board!" << endl;
					fillingBoard();
					break;
				}
				case 3: {
					cout << "Le RandomPlayer sélectionne des tokens!" << endl;
					tokenSelection();
					end_choice = 1;
					break;
				}
				case 4: {
					cout << "Le RandomPlayer achète des cards!" << endl;
					cardPurchase();
					end_choice = 1;
					break;
				}
				default:
					break;
				}
				break;
			}
			default:
				break;
			}

		} catch (SplendorException &e) {
			cout << e.getInfo() << "\n";
		}
	}
}

// surcharge Qt

void RandomPlayer::choice_Qt() {
	cout << "choices RandomPlayer:" << endl;
	bool end_choice = 0;
	int nb_choice = 0;
	while (!end_choice) {
		QCoreApplication::processEvents();
		try {
			int i = 0;
			nb_choice =
			    getOptionalChoices(); // bien vérifier la convention sur le
			                          // retour dans la définition de la méthode
			i += nb_choice + 2;
			int tmp = rand() % i + 1;

			if (tmp < 1 or tmp > i + 1) {
				throw SplendorException(
				    "Il n'y a que" + to_string(i) +
				    " choices! Vous ne pouvez pas choisir autre chose!\n");
			}
			QCoreApplication::processEvents();
			switch (nb_choice) { // l'affichage et donc le choices dépend de la
				                 // valeur de retour des choices optionnels
			case (0): {          // aucun choices optionnel possible
				switch (tmp) {
				case 1: {
					cout << "Le RandomPlayer sélectionne des tokens!" << endl;
					tokenSelection();
					end_choice = 1;
					break;
				}
				case 2: {
					cout << "Le RandomPlayer achète des cards!" << endl;
					cardPurchase();
					end_choice = 1;
					break;
				}
				default: // on continue jusqu'à ce que l'utilisateur choisisse
				         // une entrée valide!
					break;
				}
				break;
			}
			case (1): { // seulement possible d'utiliser un privilège
				switch (tmp) {
				case 1: {
					cout << "Le RandomPlayer utilise un privilège!" << endl;
					usePrivilege();
					break;
				}
				case 2: {
					cout << "Le RandomPlayer sélectionne des tokens!" << endl;
					tokenSelection();
					end_choice = 1;
					break;
				}
				case 3: {
					cout << "Le RandomPlayer achète des cards!" << endl;
					cardPurchase();
					end_choice = 1;
					break;
				}
				default:
					break;
				}
				break;
			}
			case (2): { // seulement possible de remplir le board
				switch (tmp) {
				case 1: { // remplissage board
					cout << "Le RandomPlayer remplie le board!" << endl;
					fillingBoard();
					break;
				}
				case 2: {
					cout << "Le RandomPlayer sélectionne des tokens!" << endl;
					tokenSelection();
					end_choice = 1;
					break;
				}
				case 3: {
					cout << "Le RandomPlayer achète des cards!" << endl;
					cardPurchase();
					end_choice = 1;
					break;
				}
				default:
					break;
				}
				break;
			}
			case (3): { // deux choices optionnels possibles
				switch (tmp) {
				case 1: {
					cout << "Le RandomPlayer utilise un privilège!" << endl;
					usePrivilege();
					break;
				}
				case 2: { // remplissage board
					cout << "Le RandomPlayer remplie le board!" << endl;
					fillingBoard();
					break;
				}
				case 3: {
					cout << "Le RandomPlayer sélectionne des tokens!" << endl;
					tokenSelection();
					end_choice = 1;
					break;
				}
				case 4: {
					cout << "Le RandomPlayer achète des cards!" << endl;
					cardPurchase();
					end_choice = 1;
					break;
				}
				default:
					break;
				}
				break;
			}
			default: {
				break;
			}
			}
			MainWindow::getMainWindow().updateBoard();
			MainWindow::getMainWindow().updateDraws();
			MainWindow::getMainWindow().updateScores();
			MainWindow::getMainWindow().updatePrivileges();
			MainWindow::getMainWindow().update();

		} catch (SplendorException &e) {
			cout << e.getInfo() << "\n";
		}
	}
}

void RandomPlayer::usePrivilege() {
	if (privilege_number <= 0)
		throw SplendorException("Vous n'avez pas de privilège!");
	unsigned int index = rand() % Token::getMaxTokenNumber();
	Game::getGame().getPlayerRound().drawToken(index);
	withdrawPrivilege();
}

void RandomPlayer::tokenSelection() {
	bool nb_ok = 0;
	bool choix_ok = 0;

	if (Board::getBoard().onlyGold() and
	    Game::getGame().getPlayerRound().getReservedCardNumber() >= 3)
		throw SplendorException(
		    "Pas possible de piocher: il ne reste que des tokens or et vous ne "
		    "pouvez plus réserver de card!");
	if (Board::getBoard().getCurrentNb() == 0)
		throw SplendorException("Board vide!");
	while (!choix_ok) {
		try {
			std::vector<int> tmp_tab(0);
			optional<Position> pos = nullopt;
			int gold_number = 0;
			int perl_number = 0;
			string validation;
			while (tmp_tab.size() < 3) {
				unsigned int index;
				index = rand() % Token::getMaxTokenNumber();

				if (tmp_tab.size() > 0) { // actif que après le 1e choices
					if (rand() % 2 == 0) {
						break;
					}
				}
				if (Board::getBoard().getBoardCaseByIndex(index) ==
				    nullptr) { // le nombre de cases sur le board correspond
					           // au nombre de tokens dans le game
					gold_number = 0;
					perl_number = 0;
					tmp_tab.clear();

					throw SplendorException(
					    "Il n'y a pas de jeton à cet index!\n");
				}
				if (index >
				    Token::getMaxTokenNumber()) { // le nombre de cases sur le
					                              // board correspond au nombre
					                              // de tokens dans le game
					gold_number = 0;
					perl_number = 0;
					tmp_tab.clear();
					throw SplendorException(
					    "Il n'y a que " +
					    std::to_string(Token::getMaxTokenNumber()) +
					    " places sur le board\n");
				}
				if (Board::getBoard().getBoardCaseByIndex(index)->getColor() ==
				    Color::gold) {
					gold_number++;
				}
				if (Board::getBoard().getBoardCaseByIndex(index)->getColor() ==
				    Color::perl) {
					perl_number++;
				}
				if ((gold_number == 1) &&
				    (Game::getGame().getPlayerRound().getReservedCardNumber() >=
				     3)) {
					gold_number = 0;
					perl_number = 0;
					tmp_tab.clear();
					throw SplendorException(
					    "Vous n'avez pas le droit de réserver une card "
					    "supplémentaire!");
				}
				if (gold_number == 1 and tmp_tab.size() > 0) {
					gold_number = 0;
					perl_number = 0;
					tmp_tab.clear();
					throw SplendorException("Attention, on ne peut prendre un "
					                        "jeton or seulement tout seul!");
				}
				tmp_tab.push_back(index);
			}

			// tri du vecteur par selection
			int min = 0;
			for (int j = 0; j < tmp_tab.size() - 1; j++) {
				min = j;
				for (int k = j + 1; k < tmp_tab.size(); k++) {
					if (tmp_tab[k] < tmp_tab[min]) {
						min = k;
					}
				}
				if (min != j) {
					int tmp2 = tmp_tab[min];
					tmp_tab[min] = tmp_tab[j];
					tmp_tab[j] = tmp2;
				}
			}
			// vecteur trié
			// cout<<"vecteur trié\n";

			if (tmp_tab.size() ==
			    2) { // vérification de l'alignement pour 2 tokens
				// cout<<"vérification pour 2 tokens\n";
				const Token *jet1 =
				    Board::getBoard().getBoardCaseByIndex(tmp_tab[0]);
				optional<Position> pos1 =
				    Board::getBoard().tokensAreASide(tmp_tab[1], jet1);
				if (pos1 == nullopt) {
					throw SplendorException("Jetons non-alignés\n");
				}
			}
			if (tmp_tab.size() ==
			    3) { // vérification de l'alignement pour 3 tokens
				// cout<<"vérification pour 3 tokens\n";
				const Token *jet1 =
				    Board::getBoard().getBoardCaseByIndex(tmp_tab[1]);
				optional<Position> pos1 =
				    Board::getBoard().tokensAreASide(tmp_tab[0], jet1);

				const Token *jet2 =
				    Board::getBoard().getBoardCaseByIndex(tmp_tab[2]);
				optional<Position> pos2 =
				    Board::getBoard().tokensAreASide(tmp_tab[1], jet2);

				if ((pos1 != pos2) || (pos1 == nullopt) || (pos2 == nullopt)) {
					throw SplendorException("Jetons non-alignés\n");
				}
			}

			// on a vérifié l'alignement des tokens
			if (perl_number ==
			    2) { // obtention d'un privilège par l'opponent si
				     // on deck les 2 tokens perles en une fois
				Game::getGame().getOpponent().obtainPrivilege();
			}

			if (tmp_tab.size() ==
			    3) { // obtention d'un privilège par l'opponent si
				     // les 3 tokens sont de la même color
				if ((Board::getBoard()
				         .getBoardCaseByIndex(tmp_tab[0])
				         ->getColor() == Board::getBoard()
				                             .getBoardCaseByIndex(tmp_tab[1])
				                             ->getColor()) &&
				    (Board::getBoard()
				         .getBoardCaseByIndex(tmp_tab[1])
				         ->getColor() == Board::getBoard()
				                             .getBoardCaseByIndex(tmp_tab[2])
				                             ->getColor())) {
					Game::getGame().getOpponent().obtainPrivilege();
				}
			}
			for (int i = 0; i < tmp_tab.size();
			     i++) { // acquisition des tokens par le player
				Game::getGame().getCurrentPlayer().drawToken(tmp_tab[i]);
			}
			if (gold_number == 1) {
				cardReservation();
			}
			choix_ok = 1;
		} catch (SplendorException &e) {
			cout << e.getInfo() << "\n";
		}
	}
}

void RandomPlayer::cardReservation() {
	if (rand() % 2 == 0) { // réservation sur un draw
		int draw = rand() % Deck::getDecksNumber() +
		           1; // on suppose que le nombre de deck est égal au nombre
		              // de draw (entre 1 et 3)
		switch (draw) {
		case 1: {
			unsigned int index =
			    rand() % Game::getGame().getFirstDraw()->getCardsNumber();
			reserveCard(Game::getGame().getFirstDraw(), index);
			break;
		}
		case 2: {
			unsigned int index =
			    rand() % Game::getGame().getSecondDraw()->getCardsNumber();
			reserveCard(Game::getGame().getSecondDraw(), index);
			break;
		}
		case 3: {
			unsigned int index =
			    rand() % Game::getGame().getThirdDraw()->getCardsNumber();
			reserveCard(Game::getGame().getThirdDraw(), index);
			break;
		}
		}
	} else { // on reserve sur une deck
		int deck = rand() % Deck::getDecksNumber() + 1; // entre 1 et 3
		reserveCard(Game::getGame().getDeck(deck));
	}
	for (auto c : Game::getGame().getCurrentPlayer().getReservedCards()) {
		cout << *c << endl;
	}
}

void RandomPlayer::cardPurchase() {
	unsigned int choice = -1;
	if (Game::getGame().getCurrentPlayer().getReservedCardNumber() >
	    0) { // si le player a des cards reservées
		choice = rand() % 2;
		if (choice == 1) { // achat d'une card qu'il a reservé
			cout << "\nAchat d'une card reservée!" << endl;
			int nb_carte_reservee =
			    Game::getGame().getCurrentPlayer().getReservedCards().size();
			unsigned int index = rand() % nb_carte_reservee;
			Game::getGame().getCurrentPlayer().buyReservedCard(index);
		} else {
			cout << "\nAchat d'une card sur un draw!" << endl;
			int draw = (rand() % Deck::getDecksNumber()) + 1; // entre 1 et 3
			switch (draw) {
			case 1: {
				cout << "Achat d'une card du draw 1." << endl;
				unsigned int nbCartesT1 =
				    Game::getGame().getFirstDraw()->getCardsNumber();
				if (nbCartesT1 == 0)
					throw SplendorException("Plus de card dans le Draw 1");
				unsigned int index = rand() % nbCartesT1;
				cout << *Game::getGame().getFirstDraw()->getTirage()[index]
				     << endl;
				Game::getGame().getCurrentPlayer().buyCard(
				    Game::getGame().getFirstDraw(), index);
				break;
			}
			case 2: {
				cout << "Achat d'une card du draw 2." << endl;
				unsigned int nbCartesT2 =
				    Game::getGame().getSecondDraw()->getCardsNumber();
				if (nbCartesT2 == 0)
					throw SplendorException("Plus de card dans le Draw 2");
				unsigned int index = rand() % nbCartesT2;
				cout << *Game::getGame().getSecondDraw()->getTirage()[index]
				     << endl;
				Game::getGame().getCurrentPlayer().buyCard(
				    Game::getGame().getSecondDraw(), index);
				break;
			}
			case 3: {
				cout << "Achat d'une card du draw 3." << endl;
				unsigned int nbCartesT3 =
				    Game::getGame().getThirdDraw()->getCardsNumber();
				if (nbCartesT3 == 0)
					throw SplendorException("Plus de card dans le Draw 3");
				unsigned int index = rand() % nbCartesT3;
				cout << *Game::getGame().getThirdDraw()->getTirage()[index]
				     << endl;
				Game::getGame().getCurrentPlayer().buyCard(
				    Game::getGame().getThirdDraw(), index);
				break;
			}
			}
		}
	} else { // si on ne peut que acheter
		cout << "\nAchat d'une card sur un draw!" << endl;
		int draw = (rand() % Deck::getDecksNumber()) + 1; // entre 1 et 3
		switch (draw) {
		case 1: {
			cout << "Achat d'une card du draw 1." << endl;
			unsigned int nbCartesT1 =
			    Game::getGame().getFirstDraw()->getCardsNumber();
			if (nbCartesT1 == 0)
				throw SplendorException("Plus de card dans le Draw 1");
			unsigned int index = rand() % nbCartesT1;
			cout << *Game::getGame().getFirstDraw()->getTirage()[index] << endl;
			Game::getGame().getCurrentPlayer().buyCard(
			    Game::getGame().getFirstDraw(), index);
			break;
		}
		case 2: {
			cout << "Achat d'une card du draw 2." << endl;
			unsigned int nbCartesT2 =
			    Game::getGame().getSecondDraw()->getCardsNumber();
			if (nbCartesT2 == 0)
				throw SplendorException("Plus de card dans le Draw 2");
			unsigned int index = rand() % nbCartesT2;
			cout << *Game::getGame().getSecondDraw()->getTirage()[index]
			     << endl;
			Game::getGame().getCurrentPlayer().buyCard(
			    Game::getGame().getSecondDraw(), index);
			break;
		}
		case 3: {
			cout << "Achat d'une card du draw 3." << endl;
			unsigned int nbCartesT3 =
			    Game::getGame().getThirdDraw()->getCardsNumber();
			if (nbCartesT3 == 0)
				throw SplendorException("Plus de card de le Draw 3");
			unsigned int index = rand() % nbCartesT3;
			cout << *Game::getGame().getThirdDraw()->getTirage()[index] << endl;
			Game::getGame().getCurrentPlayer().buyCard(
			    Game::getGame().getThirdDraw(), index);
			break;
		}
		}
	}
}

void RandomPlayer::buyCard(Draw *t, const int index) {
	const JewelryCard &card = t->getCardWithoutDeletion(index);

	if (card.getBonus() == colorBonus::joker and
	    jokerCardEligibility() == false) {
		throw SplendorException("Vous n'avez pas le droit d'acheter une card "
		                        "avec un bonus joker car "
		                        "vous ne pouvez pas assigner le bonus!");
	}

	// ici calculer bonus permet de retirer du cout total des cards le bonus
	// des cards déjà possédées.
	int white_cost =
	    positiveOrNull(card.getCostWhite() - calculateBonus(colorBonus::white));
	int blue_cost =
	    positiveOrNull(card.getCostBlue() - calculateBonus(colorBonus::blue));
	int red_cost =
	    positiveOrNull(card.getCostRed() - calculateBonus(colorBonus::red));
	int green_cost =
	    positiveOrNull(card.getCostGreen() - calculateBonus(colorBonus::green));
	int black_cost =
	    positiveOrNull(card.getCostBlack() - calculateBonus(colorBonus::black));
	int perl_cost = card.getCostPerl();

	// Vérifier si le player veut utiliser des tokens en or (s'il en possède)
	// Et diminuer le coût respectivement
	int nb_gold = 0;
	if (TokenAmount(Color::gold) > 0) {
		while (TokenAmount(Color::gold) > nb_gold and
		       rand() % 2 ==
		           1) { // rand pour arret aléatoire entre les choices
			            // (possibilité de ne pas utiliser tous les tokens)
			// choices de la color
			if (rand() % 2 == 1) { // on arrête la
				break;
			}

			// choices du nombre de tokens or utilisé pour la color en question
			int nb = rand() % TokenAmount(Color::gold) - nb_gold;
			int choices = rand() % 6;
			if (choices == 0 and white_cost >= nb)
				white_cost -= nb;
			nb_gold += nb;
			if (choices == 1 and blue_cost >= nb)
				blue_cost -= nb;
			nb_gold += nb;
			if (choices == 2 and red_cost >= nb)
				red_cost -= nb;
			nb_gold += nb;
			if (choices == 3 and green_cost >= nb)
				green_cost -= nb;
			nb_gold += nb;
			if (choices == 4 and black_cost >= nb)
				black_cost -= nb;
			nb_gold += nb;
			if (choices == 5 and perl_cost >= nb)
				perl_cost -= nb;
			nb_gold += nb;
			// si pas de bon choices par rapport au cout, l'ia recommence
		}
	}
	// Vérifier si assez de tokens
	int eligible_purchase = 0;

	// vérifier si on a le nombre de tokens pour acheter
	if (TokenAmount(Color::white) >= white_cost &&
	    TokenAmount(Color::blue) >= blue_cost &&
	    TokenAmount(Color::red) >= red_cost &&
	    TokenAmount(Color::green) >= green_cost &&
	    TokenAmount(Color::black) >= black_cost &&
	    TokenAmount(Color::perl) >= perl_cost) {
		eligible_purchase = 1;
	}

	if (eligible_purchase == 0)
		throw SplendorException("Pas assez de tokens pour acheter la card !");

	// Retirer les tokens utilisés et les mettre dans le bag
	withdrawTokens(Color::white, white_cost);
	withdrawTokens(Color::blue, blue_cost);
	withdrawTokens(Color::red, red_cost);
	withdrawTokens(Color::green, green_cost);
	withdrawTokens(Color::black, black_cost);
	withdrawTokens(Color::perl, perl_cost);
	withdrawTokens(Color::gold, nb_gold);

	// Mettre la card dans la main du player et la supprimer du draw
	bought_jewelry_cards.push_back(&(t->getCard(index)));
	try {
		t->fill();
	} catch (SplendorException &e) {
	}

	// Rajouter le nb de couronnes
	crown_number += card.getNbCrown();
	jewelry_cards_number++;
	points_number += card.getPrestige();
	// Dans le main tester si eligible pour card royale et appeler get card
	// royale

	Game::getGame().getCurrentPlayer().applyCapacity(
	    card, Game::getGame().getOpponent());
}

void RandomPlayer::buyReservedCard(const int index) {
	if (reserved_jewelry_cards.size() == 0 || index > 3) {
		throw SplendorException("Pas de cards réservées");
	}

	// on doit vérifier que l'achat peut se faire

	const JewelryCard *card = reserved_jewelry_cards[index];

	if (card->getBonus() == colorBonus::joker and
	    jokerCardEligibility() == false) {
		throw SplendorException("Vous n'avez pas le droit d'acheter une card "
		                        "avec un bonus joker car "
		                        "vous ne pouvez pas assigner le bonus!");
	}

	int white_cost = positiveOrNull(card->getCostWhite() -
	                                calculateBonus(colorBonus::white));
	int blue_cost =
	    positiveOrNull(card->getCostBlue() - calculateBonus(colorBonus::blue));
	int red_cost =
	    positiveOrNull(card->getCostRed() - calculateBonus(colorBonus::red));
	int green_cost = positiveOrNull(card->getCostGreen() -
	                                calculateBonus(colorBonus::green));
	int black_cost = positiveOrNull(card->getCostBlack() -
	                                calculateBonus(colorBonus::black));
	int perl_cost = card->getCostPerl();

	int nb_gold = 0;
	if (TokenAmount(Color::gold) > 0) {
		while ((TokenAmount(Color::gold) > nb_gold) and
		       rand() % 2 ==
		           1) { // rand pour arret aléatoire entre les choices
			            // (possibilité de ne pas utiliser tous les tokens)
			// choices de la color
			if (rand() % 2 == 1) { // on arrête la
				break;
			}

			// choices du nombre de tokens or utilisé pour la color en question
			int nb = rand() % TokenAmount(Color::gold) - nb_gold;

			int choices = rand() % 6;
			if (choices == 0 and white_cost >= nb)
				white_cost -= nb;
			nb_gold += nb;
			if (choices == 1 and blue_cost >= nb)
				blue_cost -= nb;
			nb_gold += nb;
			if (choices == 2 and red_cost >= nb)
				red_cost -= nb;
			nb_gold += nb;
			if (choices == 3 and green_cost >= nb)
				green_cost -= nb;
			nb_gold += nb;
			if (choices == 4 and black_cost >= nb)
				black_cost -= nb;
			nb_gold += nb;
			if (choices == 5 and perl_cost >= nb)
				perl_cost -= nb;
			nb_gold += nb;
			// si pas de bon choices par rapport au cout, l'ia recommence
		}
	}

	// Vérifier si assez de tokens
	int eligible_purchase = 0;

	// vérifier si on a le nombre de tokens pour acheter
	if (TokenAmount(Color::white) >= white_cost &&
	    TokenAmount(Color::blue) >= blue_cost &&
	    TokenAmount(Color::red) >= red_cost &&
	    TokenAmount(Color::green) >= green_cost &&
	    TokenAmount(Color::black) >= black_cost &&
	    TokenAmount(Color::perl) >= perl_cost) {
		eligible_purchase = 1;
	}

	if (eligible_purchase == 0)
		throw SplendorException("Pas assez de tokens pour acheter la card !");

	// Retirer les tokens utilisés et les mettre dans le bag
	withdrawTokens(Color::white, white_cost);
	withdrawTokens(Color::blue, blue_cost);
	withdrawTokens(Color::red, red_cost);
	withdrawTokens(Color::green, green_cost);
	withdrawTokens(Color::black, black_cost);
	withdrawTokens(Color::perl, perl_cost);
	withdrawTokens(Color::gold, nb_gold);

	reserved_jewelry_cards.erase(reserved_jewelry_cards.begin() + index);
	reserved_jewelry_cards_number--;
	bought_jewelry_cards.push_back(card);
	jewelry_cards_number++;
	points_number += card->getPrestige();
	this->crown_number += card->getNbCrown();

	Game::getGame().getCurrentPlayer().applyCapacity(
	    *card, Game::getGame().getOpponent());
}

void RandomPlayer::royalCardSelection() {
	int nb_carte_r_dans_jeu = Game::getGame().getRoyalCards().size();
	int tmp = rand() % nb_carte_r_dans_jeu;
	obtainRoyalCard(tmp);
}

void RandomPlayer::applyCapacity(const JewelryCard &card,
                                 StrategyPlayer &opponent) {
	if (card.getCapacity().has_value()) {
		std::optional<Capacity> capa = card.getCapacity();
		if (capa == Capacity::steal_opponent_pawn) {
			if (capa == Capacity::steal_opponent_pawn) {
				cout << "Le RandomPlayer peut voler un pion!\n";
				if (opponent.getToken().empty()) {
					cout << "Dommage vous n'avez pas de jeton gemme ou perl!"
					     << endl;
				} else {
					vector<const Token *> jetons_adversaire =
					    opponent.getToken();
					int choice;
					do {
						choice = rand() % jetons_adversaire.size();
					} while (jetons_adversaire[choice]->getColor() ==
					         Color::gold);
					cout << "Pion volé :" << *jetons_adversaire[choice] << endl;
					tokens.push_back(jetons_adversaire[choice]);
					token_number++;
					jetons_adversaire.erase(jetons_adversaire.begin() + choice);
					opponent.setNbJetons(opponent.getTokenNumber() - 1);
				}
			}
		} else if (capa == Capacity::take_priviledge) {
			cout << "Utilisation de capacité : le RandomPlayer gagne un "
			        "privilège\n";
			Game::getGame().getCurrentPlayer().obtainPrivilege();
		} else if (capa == Capacity::take_on_board) {
			cout << "Utilisation de capacité : le RandomPlayer peut piocher un "
			        "jeton de la "
			        "color bonus de la card\n";
			const optional<enum colorBonus> &color = card.getBonus();
			if (Board::getBoard().colorsOnBoard(color)) {
				vector<int> valid_indexes =
				    Board::getBoard().getTokenIndexesByColor(color);

				std::random_device rd;
				std::mt19937 gen(rd());
				std::uniform_int_distribution<> distribution(
				    0, valid_indexes.size() - 1);
				int index = distribution(gen);
				int choice = valid_indexes[index];

				try {
					Game::getGame().getCurrentPlayer().drawToken(choice);
				} catch (SplendorException &e) {
					cout << e.getInfo() << "\n";
				}
			} else {
				cout << "Dommage, il n'y a plus de tokens de cette color sur "
				        "le "
				        "board."
				     << endl;
			}
		} else {
			Game::getGame().nextRound(1);
			cout << "Le RandomPlayer rejoue (capacité de la card qu'elle vient "
			        "d'acheter)!"
			     << endl;
		}
	}
	if (card.getBonus() == colorBonus::joker) {
		cout << "Utilisation de capacité : le RandomPlayer transforme le joker "
		        "en un "
		        "bonus de "
		        "color en l'associant à"
		        " une de vos card dotée d'au moins un bonus.\n";
		int bonus_blanc = calculateBonus(colorBonus::white);
		int bonus_bleu = calculateBonus(colorBonus::blue);
		int bonus_rouge = calculateBonus(colorBonus::red);
		int bonus_vert = calculateBonus(colorBonus::green);
		int bonus_noir = calculateBonus(colorBonus::black);

		try {
			vector<int> possible_choices;
			int option = 0;
			if (bonus_blanc > 0) {
				possible_choices.push_back(1);
				option++;
			}
			if (bonus_bleu > 0) {
				possible_choices.push_back(2);
				option++;
			}
			if (bonus_rouge > 0) {
				possible_choices.push_back(3);
				option++;
			}
			if (bonus_vert > 0) {
				possible_choices.push_back(4);
				option++;
			}
			if (bonus_noir > 0) {
				possible_choices.push_back(5);
				option++;
			}
			// Vérifiez si aucune option n'est disponible
			if (option == 0) {
				throw SplendorException(
				    "vous ne possédez aucune card dotée de bonus.. Capacité"
				    " sans effet\n");
			} else {
				std::random_device rd;
				std::mt19937 gen(rd());
				std::uniform_int_distribution<> distribution(
				    0, possible_choices.size() - 1);
				int index = distribution(gen);
				int choices = possible_choices[index];

				colorBonus b;
				switch (choices) {
				case 1:
					b = colorBonus::white;
					card.changeBonusColor(b);
					break;
				case 2:
					b = colorBonus::blue;
					card.changeBonusColor(b);
					break;
				case 3:
					b = colorBonus::red;
					card.changeBonusColor(b);
					break;
				case 4:
					b = colorBonus::green;
					card.changeBonusColor(b);
					break;
				case 5:
					b = colorBonus::black;
					card.changeBonusColor(b);
					break;
				default:
					cout << "Choix invalide, veuillez recommencer. Ancien "
					        "choices : "
					     << choices << endl;
					break;
				}
				cout << "Couleur du bonus choisi par le RandomPlayer : "
				     << toString(b) << endl;
			}
		} catch (SplendorException &e) {
			cout << e.getInfo() << "\n";
		}
	}
}

void RandomPlayer::applyRoyalCapacity(const RoyalCard &card,
                                      StrategyPlayer &opponent) {
	if (card.getCapacity().has_value()) {
		cout << "Application de la capacité de la card royale!" << endl;
		std::optional<Capacity> capa = card.getCapacity();
		if (capa == Capacity::steal_opponent_pawn) {
			if (capa == Capacity::steal_opponent_pawn) {
				cout << "Le RandomPlayer peut voler un pion!\n";
				if (opponent.getToken().empty()) {
					cout << "Dommage vous n'avez pas de jeton gemme ou perl!"
					     << endl;
				} else {
					vector<const Token *> jetons_adversaire =
					    opponent.getToken();
					int choice;
					do {
						choice = rand() % jetons_adversaire.size();
					} while (jetons_adversaire[choice]->getColor() ==
					         Color::gold);
					tokens.push_back(jetons_adversaire[choice]);
					token_number++;
					jetons_adversaire.erase(jetons_adversaire.begin() + choice);
					opponent.setNbJetons(opponent.getTokenNumber() - 1);
				}
			}
		} else if (capa == Capacity::take_priviledge) {
			cout << "Le RandomPlayer prend un privilège" << endl;
			Game::getGame().getCurrentPlayer().obtainPrivilege();
		} else {
			cout << "Le RandomPlayer peut replay" << endl;
			Game::getGame().nextRound(1);
		}
	}
}

void RandomPlayer::tokenVerification() {
	if (token_number > 10) {
		int nb = token_number - 10;
		cout << "Le RandomPlayer a trop de tokens, elle doit en remettre " << nb
		     << " dans le bag." << endl;
		cout << "Voici ses tokens:" << endl;
		for (int i = 0; i < token_number; i++) {
			cout << "Indice " << i << " : " << *tokens[i] << endl;
		}
		vector<int> tab(0);
		int i = 0;
		int curNb = nb;
		while (curNb > 0) {
			int tmp = rand() % token_number;
			// cout<<tmp<<endl;
			bool inTab = 0;
			for (int j = 0; j < tab.size();
			     j++) { // vérification qu'on n'a pas déjà essayé de l'enlever
				if (tab[j] == tmp) {
					inTab = 1;
					cout << "Déjà dans tab" << endl;
				}
			}
			if (tmp >= 0 and tmp < token_number and inTab == 0) {
				tab.push_back(tmp);
				curNb--;
			}
		}
		// tri du tableau
		std::make_heap(tab.begin(), tab.end());
		std::sort_heap(tab.begin(), tab.end());
		for (int k = tab.size() - 1; k >= 0;
		     k--) { // suppression de la fin vers le début pour ne pas décaler
			        // les indices
			Bag::get().insertToken(tokens[tab[k]]);
			tokens.erase(tokens.begin() + tab[k]);
			token_number--;
		}
	}
}

/******************** RandomPlayer ********************/
