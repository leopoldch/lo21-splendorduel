#include "jetons.h"
#include "carte.h"
using namespace std;

int Jeton::nb_blanc = 0;
int Jeton::nb_bleu = 0;
int Jeton::nb_rouge = 0;
int Jeton::nb_vert = 0;
int Jeton::nb_noir = 0;
int Jeton::nb_or = 0;
int Jeton::nb_perle = 0;

vector<const Jeton *> initJetons() {
  vector<const Jeton *> tab;
  int id = 1;

  for (int i = 0; i < 3; ++i) {
    Jeton *tmp =
        new Jeton(id, Color::gold, "../src/Reste_detoure/Jeton_or.png");
    tab.push_back(tmp);
    id++;
  }
  for (int i = 0; i < 2; ++i) {
    Jeton *tmp =
        new Jeton(id, Color::perle, "../src/Reste_detoure/Jeton_perle.png");
    tab.push_back(tmp);
    id++;
  }
  for (int i = 0; i < 4; ++i) {
    Jeton *tmp =
        new Jeton(id, Color::rouge, "../src/Reste_detoure/Jeton_rouge.png");
    tab.push_back(tmp);
    id++;
  }
  for (int i = 0; i < 4; ++i) {
    Jeton *tmp =
        new Jeton(id, Color::bleu, "../src/Reste_detoure/Jeton_bleu.png");
    tab.push_back(tmp);
    id++;
  }
  for (int i = 0; i < 4; ++i) {
    Jeton *tmp =
        new Jeton(id, Color::vert, "../src/Reste_detoure/Jeton_vert.png");
    tab.push_back(tmp);
    id++;
  }
  for (int i = 0; i < 4; ++i) {
    Jeton *tmp =
        new Jeton(id, Color::noir, "../src/Reste_detoure/Jeton_noir.png");
    tab.push_back(tmp);
    id++;
  }
  for (int i = 0; i < 4; ++i) {
    Jeton *tmp =
        new Jeton(id, Color::blanc, "../src/Reste_detoure/Jeton_blanc.png");
    tab.push_back(tmp);
    id++;
  }

  return tab;
}
