## #PROJET LO21 ANALYSE DES CONCEPTS

    ##But du jeu##

    Splendor Duel est un jeu pour 2 joueurs basé sur Splendor,
    qui a été fait pour plusieurs personnes.Il
        a été créé par Marc André et Bruno Cathala,
    et édité par SPACE Cowboys.En tant que maître de bijoux destinés aux
        monarques et autres dirigeants puissants de la Renaissance,
    le joueur affronte son rival en collectionnant des cartes.

    ##Conditions pour la victoire##

    Afin de gagner le jeu,
    un joueur doit remplir l’une des conditions suivantes :

    -Posséder * *
    20 points de Prestige ou plus **.-
            Posséder * * 10 couronnes ou plus **.-
            Posséder * *
                10 points
                    de Prestige ou plus sur des cartes de la même couleur **.

                ##Différents objets du jeu##

#Jetons #

                Il y a * *
                25 jetons **.Il y en a -
            4 de chacune des cinq couleurs des **gemmes **(blue, white, green,
                                                           black et red)
                .-
            2 * *perles * * -3 * *or
        **.

          A la fin de chaque tour,
    le joueur peut posséder **au plus 10 jetons **.S’il dépasse cette limite,
    il doit remettre dans le sac les jetons de son choix afin d’en avoir
    10. Au début du jeu,
    les 25 jetons nécessaires pour remplir le plateau sont placés
        aléatoirement en suivant la spirale dessinée
            .

                **Les jetons Or sont des jokers **et peuvent être utilisés à la
        place de n'importe quel jeton Gemme ou Perle. Ils sont pris au moment de la réservation d’une carte. Une carte ne peut pas être réservée si le joueur en a déjà réservé 3 ou s’il n’y a plus de jetons Or sur le plateau.

            **Propriétés ** :

    -**Color **(les jetons or sont des jokers qui permettent de remplacer
                                  n’importe quelle couleur)
            .

                **Remarques ** :

    -Si le joueur prend 3 jetons de la même couleur ou 2 jetons perles lors d'un tour, son adversaire gagne 1 privilège.

#Cartes #
    ##Cartes joaillerie##

    Il y a * *
        3 types de cartes **séparées par niveau,
    qui devront être mélangées à part,
    avant chaque jeu.Au total il y a 67 cartes
            .

                **Propriétés ** :

    -**Niveau **.-
        **Coût **.- **Point(s)
de prestige **.-
    **Capacité **(il y a 5 types de capacité, la carte peut ne pas en posséder)
          .-
    **Bonus **qui diminue le coût des futurs achats de cartes.-
    **Réservation **(est réservée ou pas).-
    **Couronnes **(possible qu’il n’y en ait pas)
          .

              **Remarques ** :

    -Il est possible d’avoir
    suffisamment de bonus pour réduire le coût d’une carte à
    0 jeton.Si le joueur possède un bonus supérieur au coût de la carte,
    il ne gagne pas de jetons.

            ##Cartes Royales##

            Il y a * *
            4 cartes royales *
                *.Elles peuvent être prises à l’obtention de la
            3e couronne ou de la
            6e. Prendre une carte Royale n’est pas une action
                .Les joueurs placent leurs cartes Royales à côté de leurs cartes
            Joaillerie.

                **Propriétés * *

            -Point(s) de prestige.-
        Capacité(la carte peut ne pas en posséder)
                .

#Privilèges #

            Il y a * *
            3 Privilèges **au total dans le jeu
                .Les Privilèges permettent l’action Privilège.Celle
        -
        ci permet au joueur de prendre sur le plateau
        1 jeton Gemme ou Perle de son choix par
        Privilège dépensé.Il est interdit de prendre un jeton Or dans ce cas
            .

        Ils peuvent être obtenus dans les cas suivant
    : -Si l’adversaire du joueur remplit le plateau.-
        Si 3 jetons de la même couleur ou
        2 jetons Perle sont pris par son adversaire.-
        Capacité privilège qui permet au joueur d’en prendre 1.

            * *Important ** : pendant la partie,
    lorsqu’un joueur est censé prendre un Privilège et que plus aucun n’est
        disponible au
        - dessus du plateau,
    il le prend à son adversaire.S’il possède déjà les trois,
    rien ne se passe.

#Plateau #

    Le plateau permettra de connaître son état,
    c’est - à - dire les jetons et leur emplacement - **Matrice_jetons **.-
        **Nb_jetons **.-
        **Nb_privilèges **(qui n’ont pas été pris par quelqu’un)
              .

#Sac de jetons #

          Ce sac contient les jetons utilisés par laes joueurs lors de
          l'achat de cartes joaillerie  Cette classe nous permet de
          connaître les jetons qui y
          sont(bien qu 'ils sont privé pour l' utilisateur, cf : encapsulation)
              .-
        **Nb_jetons_bleus **.- **Nb_jetons_rouges **.- **Nb_jetons_blancs **.-
        **Nb_jetons_verts **.- **Nb_jetons_noirs **.- **Nb_or **.-
        **Nb_perles **.-
        **Jetons **(tableau)
                .

#Tour

            L’objet Tour servira à définir les propriétés et possibilités de
            chaque tour.Sa durée de vie sera égale à celle d’un tour dans le jeu
                .(Actions null signifie qu’aucune n’a été faite pour l’instant)
                .Il sera un attribut pour d'un objet de la classe jeu qui définie une partie.

                    **Propriétés ** :

    -**Joueur actif * *
            -**Joueur passif **(son adversaire) -
        **Actions facultatives ** {
  null, privilege, reconstituer_tableau
}
- **Actions obligatoires** {null, prendre_jetons, reserver, acheter_joaillerie}

**Actions** :

- Donner naissance au tour suivant 
- Marquer la victoire

#Jeu #

**Propriétés** :

- Joueur 1
- Joueur 2
- Quel tour est en train d’être joué
- Plateau
- Sac jetons
- Cartes_niv1
- Cartes_niv2
- Cartes_niv3
- royal_cards
- Privilèges
- Terminé ou pas

#Joueur

- Nom
- Nb_points
- Nb_couronnes
- Cartes

#Actions #

## Facultatives ##

Au début de son tour, le joueur effectue **0, 1 ou 2 actions optionnelles dans l’ordre indiqué** puis 1 et 1 seule action obligatoire.
1.	Utiliser un **privilège**
2.	**Reconstituer le plateau**

## Obligatoires ##

**Cas particulier** : si vous ne pouvez pas effectuer d’action obligatoire, vous devez effectuer l’action optionnelle « Remplir le plateau de jeu » avant de choisir votre action obligatoire.

Le joueur doit effectuer **une et une seule** des trois actions suivantes :
- **Prendre jusqu’à 3 jetons**
Le joueur prend jusqu’à 3 jetons Gemme et/ou Perle sur le plateau. Ceux-ci doivent être adjacents les uns aux autres en ligne, en colonne ou en diagonale. De plus, il ne doit y avoir ni case vide ni jeton Or entre les jetons pris. Il est permis de ne prendre que 2 jetons adjacents, ou même 1 seul. Il est interdit de prendre un jeton Or avec cette action.
- Prendre **1 jeton Or pour réserver 1 carte Joaillerie**.
- Acheter **1 carte Joaillerie**.


### Concepts de cours
- Notion de classe (Jetons, Tour, ...)
- Encapsulation (ex :
on ne veut pas
        qu'un joueur puisse modifier son nombre de jetons ou de privilèges) -
    Instanciation et mort des objets(optimisation mémoire)
        -> surchage constructeur et destructeur
    -
    Surcharge
    d'opérateur (ex : pour afficher correctement un jeton dans la console) -
    Liens entre objets,
    associations entre classes(voir UML)
