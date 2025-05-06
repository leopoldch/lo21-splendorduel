//
// Created by leo on 18/12/2023.
//

#include "history.h"
History::Handler History::handler;

Strategy_player &fromJsonStrategyPLayer(json data) {
  if (data["is_ia"] == 1) {
    string tmp = data["nom"];
    if (History::getHistory().inHistory(tmp, 1)) {
      Strategy_player *player = History::getHistory().pullPlayer(tmp, 1);
      player->setNbCouronnes(data["nb_courones"]);
      player->setPoints(data["nb_points"]);
      player->setNbPrivileges(data["nb_privileges"]);
      player->setIa(data["is_ia"]);
      player->setNbJetons(data["nb_jetons"]);
      player->setNbRCards(data["nb_cartes_r"]);
      player->setNbJCards(data["nb_cartes_j"]);
      player->setNbJCardsReserved(data["nb_cartes_j_reservees"]);

      player->setJetons(fromJsonJetons(data["jetons"]));
      player->setJewellryCard(
          fromJsonJewelryCard(data["cartes_joaillerie_achetees"]));
      player->setJewellryCardReserved(
          fromJsonJewelryCard(data["cartes_joaiellerie_reservees"]));
      player->setRoyalCard(fromJsonRoyalCard(data["cartes_royale"]));
      player->setPrivileges(
          fromJsonPrivileges(data["privileges"], data["nb_privileges"]));
      return *player;
    } else {
      IA *player = new IA(tmp);
      player->setNbCouronnes(data["nb_courones"]);
      player->setPoints(data["nb_points"]);
      player->setNbPrivileges(data["nb_privileges"]);
      player->setIa(data["is_ia"]);
      player->setNbJetons(data["nb_jetons"]);
      player->setNbRCards(data["nb_cartes_r"]);
      player->setNbJCards(data["nb_cartes_j"]);
      player->setNbJCardsReserved(data["nb_cartes_j_reservees"]);

      player->setJetons(fromJsonJetons(data["jetons"]));
      player->setJewellryCard(
          fromJsonJewelryCard(data["cartes_joaillerie_achetees"]));
      player->setJewellryCardReserved(
          fromJsonJewelryCard(data["cartes_joaiellerie_reservees"]));
      player->setRoyalCard(fromJsonRoyalCard(data["cartes_royale"]));
      player->setPrivileges(
          fromJsonPrivileges(data["privileges"], data["nb_privileges"]));
      return *player;
    }

  } else {
    string tmp = data["nom"];
    if (History::getHistory().inHistory(tmp, 0)) {
      Strategy_player *player = History::getHistory().pullPlayer(tmp, 0);
      player->setNbCouronnes(data["nb_courones"]);
      player->setPoints(data["nb_points"]);
      player->setNbPrivileges(data["nb_privileges"]);
      player->setIa(data["is_ia"]);
      player->setNbJetons(data["nb_jetons"]);
      player->setNbRCards(data["nb_cartes_r"]);
      player->setNbJCards(data["nb_cartes_j"]);
      player->setNbJCardsReserved(data["nb_cartes_j_reservees"]);

      player->setJetons(fromJsonJetons(data["jetons"]));
      player->setJewellryCard(
          fromJsonJewelryCard(data["cartes_joaillerie_achetees"]));
      player->setJewellryCardReserved(
          fromJsonJewelryCard(data["cartes_joaiellerie_reservees"]));
      player->setRoyalCard(fromJsonRoyalCard(data["cartes_royale"]));
      player->setPrivileges(
          fromJsonPrivileges(data["privileges"], data["nb_privileges"]));
      return *player;
    } else {
      Joueur *player = new Joueur(tmp);
      player->setNbCouronnes(data["nb_courones"]);
      player->setPoints(data["nb_points"]);
      player->setNbPrivileges(data["nb_privileges"]);
      player->setIa(data["is_ia"]);
      player->setNbJetons(data["nb_jetons"]);
      player->setNbRCards(data["nb_cartes_r"]);
      player->setNbJCards(data["nb_cartes_j"]);
      player->setNbJCardsReserved(data["nb_cartes_j_reservees"]);

      player->setJetons(fromJsonJetons(data["jetons"]));
      player->setJewellryCard(
          fromJsonJewelryCard(data["cartes_joaillerie_achetees"]));
      player->setJewellryCardReserved(
          fromJsonJewelryCard(data["cartes_joaiellerie_reservees"]));
      player->setRoyalCard(fromJsonRoyalCard(data["cartes_royale"]));
      player->setPrivileges(
          fromJsonPrivileges(data["privileges"], data["nb_privileges"]));
      return *player;
    }
  }
}

Jeu::Jeu(json data) {

  // on veut init le jeu avec tout ce dont on a besoin

  est_termine = data["est_termine"];
  cout << "init j1\n" << endl;

  qui_joue = &fromJsonStrategyPLayer(data["qui_joue"]);

  manche = data["manche"];
  cout << "init j2\n" << endl;
  adversaire = &fromJsonStrategyPLayer(data["adversaire"]);

  // volontaire de pas init
  // cartes_joiallerie = fromJsonJewelryCard(data["cartes_joiallerie"]);
  // jetons = fromJsonJetons(data["jetons"]);

  cartes_royales = fromJsonRoyalCard(data["cartes_royales"]);
  privileges = fromJsonPrivileges(data["privileges"], data["nb_privileges"]);

  cout << "init pioche1\n" << endl;
  p1 = &fromJsonPioche(data["pioche1"]);
  cout << "init pioche2\n" << endl;
  p2 = &fromJsonPioche(data["pioche2"]);
  cout << "init pioche3\n" << endl;
  p3 = &fromJsonPioche(data["pioche3"]);

  tirage_1 = &fromJsonTirage(data["tirage1"], *p1);

  tirage_2 = &fromJsonTirage(data["tirage2"], *p2);

  tirage_3 = &fromJsonTirage(data["tirage3"], *p3);

  fromJsonSac(data["sac"]);

  fromJsonPlateau(data["plateau"]);
}

Match::Match(json data) {

  if (data["gagnant"]["is_ia"] == 1) {

    string tmp = data["gagnant"]["nom"];
    if (!History::getHistory().inHistory(tmp, 1)) {
      gagnant = new IA(tmp);
      gagnant->setWins(data["gagnant"]["games_won"]);
      gagnant->setPlayed(data["gagnant"]["games"]);
      gagnant->setIa(1);
      History::getHistory().addPlayer();

    } else {
      gagnant = History::getHistory().pullPlayer(tmp, 1);
    }

  } else {
    string tmp = data["gagnant"]["nom"];
    if (!History::getHistory().inHistory(tmp, 0)) {
      gagnant = new Joueur(tmp);
      gagnant->setWins(data["gagnant"]["games_won"]);
      gagnant->setPlayed(data["gagnant"]["games"]);
      gagnant->setIa(0);
      History::getHistory().addPlayer();

    }

    else {
      gagnant = History::getHistory().pullPlayer(tmp, 0);
    }
  }
  if (data["adversaire"]["is_ia"] == 1) {
    string tmp = data["adversaire"]["nom"];
    if (!History::getHistory().inHistory(tmp, 1)) {
      adversaire = new IA(tmp);
      adversaire->setWins(data["adversaire"]["games_won"]);
      adversaire->setPlayed(data["adversaire"]["games"]);
      adversaire->setIa(1);
      History::getHistory().addPlayer();
    } else {
      adversaire = History::getHistory().pullPlayer(tmp, 1);
    }

  } else {
    string tmp = data["adversaire"]["nom"];
    if (!History::getHistory().inHistory(tmp, 0)) {
      adversaire = new Joueur(tmp);
      adversaire->setWins(data["adversaire"]["games_won"]);
      adversaire->setPlayed(data["adversaire"]["games"]);
      adversaire->setIa(0);
      History::getHistory().addPlayer();
    } else {
      adversaire = History::getHistory().pullPlayer(tmp, 0);
    }
  }
  score_gagnant = data["score_gagnant"];
  score_adversaire = data["score_adversaire"];
}

void Hist() {

  json j1; // on récup l'ancien historique
  int tmp = 0;

  // on ajoute le nouveau
  json jtmp = History::getHistory().toHistory();
  cout << History::getHistory().getSize() << endl;

  for (int i = 0; i < History::getHistory().getSize(); ++i) {
    j1["matches"].push_back(jtmp[i]);
  }

  json j2 = {};
  j2["gagnant"] = Jeu::getJeu().getCurrentPlayer().toHistory();
  j2["adversaire"] = Jeu::getJeu().getOpponent().toHistory();
  j2["score_gagnant"] = Jeu::getJeu().getCurrentPlayer().getNbPoints();
  j2["score_adversaire"] = Jeu::getJeu().getOpponent().getNbPoints();
  j1["matches"].push_back(j2);

  tmp += History::getHistory().getSize();
  tmp += 1;

  j1["nb_matches"] = tmp;
  std::string history = j1.dump(2);
  cout << history << endl;
  std::ofstream file("../src/history.json");
  file << history;
}

void Jeu::setPlayers(string &name1, string &name2, string &choix1,
                     string &choix2) {

  if (name1 == name2) {
    throw SplendorException("Les joueurs doivent être différents !");
  }
  if (name1 == "" or name2 == "") {
    throw SplendorException("Les deux joueurs doivent avoir un nom!");
  }

  if (choix1 == "IA")
    choix1 = "I";
  else
    choix1 = "J";

  if (choix2 == "IA")
    choix2 = "I";
  else
    choix2 = "J";

  srand(static_cast<unsigned>(std::time(nullptr)));
  if (rand() % 2 == 0) { // joueur qui débute la partie est tiré aléatoirement
    if (choix1 == "J") {
      if (History::getHistory().inHistory(name1, 0)) {
        qui_joue = History::getHistory().pullPlayer(name1, 0);
      } else {
        qui_joue = new Joueur(name1);
        qui_joue->setIa(0);
      }
    } else {

      if (History::getHistory().inHistory(name1, 1)) {
        qui_joue = History::getHistory().pullPlayer(name1, 1);
      } else {
        qui_joue = new IA(name1);
        qui_joue->setIa(1);
      }
    }

    if (choix2 == "J") {

      if (History::getHistory().inHistory(name2, 0)) {
        adversaire = History::getHistory().pullPlayer(name2, 0);
      } else {
        adversaire = new Joueur(name2);
        adversaire->setIa(0);
      }

    } else {
      if (History::getHistory().inHistory(name2, 1)) {
        adversaire = History::getHistory().pullPlayer(name2, 1);
      } else {
        adversaire = new IA(name2);
        adversaire->setIa(1);
      }
    }
  } else {
    if (choix1 == "J") {
      if (History::getHistory().inHistory(name1, 0)) {
        adversaire = History::getHistory().pullPlayer(name1, 0);
      } else {
        adversaire = new Joueur(name1);
        adversaire->setIa(0);
      }

    } else {
      if (History::getHistory().inHistory(name1, 1)) {
        adversaire = History::getHistory().pullPlayer(name1, 1);
      } else {
        adversaire = new IA(name1);
        adversaire->setIa(1);
      }
    }
    if (choix2 == "J") {

      if (History::getHistory().inHistory(name2, 0)) {
        qui_joue = History::getHistory().pullPlayer(name2, 0);
      } else {
        qui_joue = new Joueur(name2);
        qui_joue->setIa(0);
      }
    } else {
      if (History::getHistory().inHistory(name2, 1)) {
        qui_joue = History::getHistory().pullPlayer(name2, 1);
      } else {
        qui_joue = new IA(name2);
        qui_joue->setIa(1);
      }
    }
  }
  adversaire->obtainPrivilege(); // Le joueur qui ne commence pas démarre avec
                                 // un privilège
}
