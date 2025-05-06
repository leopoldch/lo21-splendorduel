//
// Created by Léopold Chappuis on 14/12/2023.
//

#include "json.h"

vector<const JewelryCard *> fromJsonJewelryCard(json data) {
  // cout<<data.dump(2)<<endl;

  // fix les noms avec getCapacityFromString & getBonusFromString

  std::vector<const JewelryCard *> cartes_joailleries_instances;

  for (const auto &carte_joaillerie_data : data) {

    if ((carte_joaillerie_data["capacite"] == 0) &&
        (carte_joaillerie_data["bonus_couleur"] == 0)) {
      JewelryCard *instance = new JewelryCard(
          carte_joaillerie_data["points_prestige"],
          carte_joaillerie_data["cout_blanc"],
          carte_joaillerie_data["cout_bleu"],
          carte_joaillerie_data["cout_rouge"],
          carte_joaillerie_data["cout_vert"],
          carte_joaillerie_data["cout_noir"],
          carte_joaillerie_data["cout_perle"], carte_joaillerie_data["niveau"],
          carte_joaillerie_data["nb_couronne"],
          carte_joaillerie_data["bonus_nombre"], nullopt, nullopt,
          carte_joaillerie_data["visuel"]);
      cartes_joailleries_instances.push_back(instance);
    } else if ((carte_joaillerie_data["capacite"] != 0) &&
               (carte_joaillerie_data["bonus_couleur"] == 0)) {

      JewelryCard *instance = new JewelryCard(
          carte_joaillerie_data["points_prestige"],
          carte_joaillerie_data["cout_blanc"],
          carte_joaillerie_data["cout_bleu"],
          carte_joaillerie_data["cout_rouge"],
          carte_joaillerie_data["cout_vert"],
          carte_joaillerie_data["cout_noir"],
          carte_joaillerie_data["cout_perle"], carte_joaillerie_data["niveau"],
          carte_joaillerie_data["nb_couronne"],
          carte_joaillerie_data["bonus_nombre"], nullopt,
          getCapaciteFromString(carte_joaillerie_data["capacite"]),
          carte_joaillerie_data["visuel"]);
      cartes_joailleries_instances.push_back(instance);

    } else if ((carte_joaillerie_data["capacite"] == 0) &&
               (carte_joaillerie_data["bonus_couleur"] != 0)) {

      JewelryCard *instance = new JewelryCard(
          carte_joaillerie_data["points_prestige"],
          carte_joaillerie_data["cout_blanc"],
          carte_joaillerie_data["cout_bleu"],
          carte_joaillerie_data["cout_rouge"],
          carte_joaillerie_data["cout_vert"],
          carte_joaillerie_data["cout_noir"],
          carte_joaillerie_data["cout_perle"], carte_joaillerie_data["niveau"],
          carte_joaillerie_data["nb_couronne"],
          carte_joaillerie_data["bonus_nombre"],
          getBonusFromString(carte_joaillerie_data["bonus_couleur"]), nullopt,
          carte_joaillerie_data["visuel"]);
      cartes_joailleries_instances.push_back(instance);
    } else {

      JewelryCard *instance = new JewelryCard(
          carte_joaillerie_data["points_prestige"],
          carte_joaillerie_data["cout_blanc"],
          carte_joaillerie_data["cout_bleu"],
          carte_joaillerie_data["cout_rouge"],
          carte_joaillerie_data["cout_vert"],
          carte_joaillerie_data["cout_noir"],
          carte_joaillerie_data["cout_perle"], carte_joaillerie_data["niveau"],
          carte_joaillerie_data["nb_couronne"],
          carte_joaillerie_data["bonus_nombre"],
          getBonusFromString(carte_joaillerie_data["bonus_couleur"]),
          getCapaciteFromString(carte_joaillerie_data["capacite"]),
          carte_joaillerie_data["visuel"]);
      cartes_joailleries_instances.push_back(instance);
    }
  }
  return cartes_joailleries_instances;
}

vector<const RoyalCard *> fromJsonRoyalCard(json data) {

  std::vector<const RoyalCard *> cartes_royales_instances;

  for (const auto &carte_royale_data : data) {

    if (carte_royale_data["capacite"] != NULL) {
      RoyalCard *instance =
          new RoyalCard(carte_royale_data["points_prestige"],
                        getCapaciteFromString(carte_royale_data["capacite"]),
                        carte_royale_data["visuel"]);
      cartes_royales_instances.push_back(instance);
    } else {
      RoyalCard *instance = new RoyalCard(carte_royale_data["points_prestige"],
                                          nullopt, carte_royale_data["visuel"]);
      cartes_royales_instances.push_back(instance);
    }
  }
  return cartes_royales_instances;
}

vector<const Jeton *> fromJsonJetons(json data) {
  std::vector<const Jeton *> tab_jetons;
  for (const auto &jeton : data) {
    if (jeton != nullptr) {
      Jeton *tmp = new Jeton(jeton["id"], jeton["color"], jeton["visuel"]);
      tab_jetons.push_back(tmp);
    } else {
      tab_jetons.push_back(nullptr);
    }
  }
  return tab_jetons;
}

vector<const Privilege *> fromJsonPrivileges(json data, unsigned int nb) {
  if (nb > 3) {
    throw SplendorException(
        "On ne peut pas avoir plus de 3 privilèges. Le json est corrompu.");
  }
  std::vector<const Privilege *> tab;
  for (int i = 0; i < nb; ++i) {
    tab.push_back(Privilege::getForJson(data[i]["id"]));
  }
  return tab;
}

Pioche &fromJsonPioche(json data) {
  // ici on doit init les jewellery cards
  Pioche *p = new Pioche(data["niveau"]);
  p->setPioche(fromJsonJewelryCard(data["cartes_joailleries"]));
  return *p;
}

Tirage &fromJsonTirage(json data, Pioche &p) {
  Tirage *t = new Tirage(data["niveau"], data["max_cartes"], p);
  t->setNbCartes(data["nb_cartes"]);
  t->setTirage(fromJsonJewelryCard(data["cartes_joailleries"]));
  return *t;
}

void fromJsonPlateau(json data) {
  Plateau &p = Plateau::get_plateau();
  p.setNb(data["nb"]);
  // on récupère les jetons et on les met tous en sac uns par uns ?
  vector<const Jeton *> jetons;
  jetons = fromJsonJetons(data["jetons"]);

  if (jetons.size() > data["nb"]) {
    throw SplendorException("erreur dans le nombre de jetons sur le plateau. "
                            "La partie ne peut pas être chargée.");
  }
  int number_jetons_saved = jetons.size();
  for (int i = number_jetons_saved; i < data["nb"]; ++i) {
    jetons.push_back(nullptr);
  }
  p.setJetons(jetons);
  // cout<<*jetons[0]<<endl;
  p.setCurrentNb(data["current_nb"]);
}

void fromJsonSac(json data) {
  if (data["nb"] != 0 && !data["jetons"].is_null()) {
    Sac &s = Sac::get_sac();
    // on récupère les jetons et on les met tous en sac uns par uns ?
    vector<const Jeton *> jetons;

    jetons = fromJsonJetons(data["jetons"]);

    for (int i = 0; i < jetons.size(); ++i) {
      s.mettre_jeton_sac(jetons[i]);
    }

    s.set_nb_sac(
        data["nb"]); // normalement pas besoin de reset nb mais on sait jamais
  }
}
