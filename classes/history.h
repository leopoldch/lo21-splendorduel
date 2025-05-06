//
// Created by leo on 18/12/2023.
//

#ifndef LO21_SPLENDOR_DUEL_HISTORY_H
#define LO21_SPLENDOR_DUEL_HISTORY_H

#include "jeu.h"

class Match {

private:
  Strategy_player *gagnant;
  Strategy_player *adversaire;
  unsigned int score_gagnant;
  unsigned int score_adversaire;

public:
  json toHistory() {
    json j;
    j["gagnant"] = gagnant->toHistory();
    j["adversaire"] = adversaire->toHistory();
    j["score_gagnant"] = score_gagnant;
    j["score_adversaire"] = score_adversaire;
    return j;
  }

  Match(json data);

  ~Match() {
    delete gagnant;
    delete adversaire;
  }

  Strategy_player *getWinner() const { return gagnant; }
  Strategy_player *getOpponent() const { return adversaire; }
  unsigned int getScoreWinner() const { return score_gagnant; }
  unsigned int getScoreOpponent() const { return score_adversaire; }
};

class History {

private:
  // singleton
  struct Handler {
    History *instance = nullptr;

    ~Handler() {
      delete instance;
      instance = nullptr;
    }
  };

  static Handler handler;

  // attributs
  vector<Match *> matches;
  unsigned int nb_match = 0;
  unsigned int nb_joueurs = 0;

  // méthodes

  History() = default;

  ~History() {
    for (Match *match : matches) {
      delete match;
    }
    matches.clear();
    delete &handler;
  }

  History(const History &) = delete;

  History &operator=(const History &) = delete;

public:
  static void freeHistory() { delete &History::getHistory(); }

  void addPlayer() { nb_joueurs++; }

  const unsigned int getNbPlayers() const { return nb_joueurs; }

  vector<Match *> getMatches() const { return matches; }

  const bool inHistory(const string nom, const int is_ia) const {

    for (int i = 0; i < matches.size(); ++i) {
      if (matches[i]->getWinner()->getName() == nom &&
          matches[i]->getWinner()->getIa() == is_ia)
        return true;
      if (matches[i]->getOpponent()->getName() == nom &&
          matches[i]->getOpponent()->getIa() == is_ia)
        return true;
    }
    return false;
  }

  unsigned int getSize() const { return nb_match; }

  void initHistory(json data) {
    for (int i = 0; i < data["nb_matches"]; ++i) {
      Match *tmp = new Match(data["matches"][i]);
      matches.push_back(tmp);
      nb_match++;
    }
  }

  Strategy_player *pullPlayer(const string nom, const unsigned int is_ia) {
    for (int i = 0; i < matches.size(); ++i) {
      if (matches[i]->getWinner()->getName() == nom &&
          matches[i]->getWinner()->getIa() == is_ia) {
        return matches[i]->getWinner();
      }
      if (matches[i]->getOpponent()->getName() == nom &&
          matches[i]->getOpponent()->getIa() == is_ia) {
        return matches[i]->getOpponent();
      }
    }
    throw SplendorException("joueur non trouvé dans l'historique");
  }

  json toHistory() {

    json j;
    j = {};
    for (int i = 0; i < matches.size(); ++i) {
      j.push_back(matches[i]->toHistory());
    }
    return j;
  }

  static History &getHistory() {
    if (handler.instance == nullptr)
      handler.instance = new History;
    return *handler.instance;
  }
};

void Hist();

#endif // LO21_SPLENDOR_DUEL_HISTORY_H
