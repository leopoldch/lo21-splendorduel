#Rapport final

## LO21 : Projet Splendor Duel

Petri Gaspard, Chappuis Léopold, Fouinat--Beal Quentin, Ghitu Alexandru, Green Aubin

## Introduction

Dans le cadre de notre cours de LO21, nous avons été missionnés pour créer une version numérique du jeu Splendor Duel, créé par Marc André et Bruno Cathala, et édité par Space Cowboy. Pour cela, nous devions utiliser le langage C++ et employer une approche orientée objet.
Pour mener à bien ce projet, nous avons dû mobiliser les différentes connaissances acquises en cours ainsi qu'en TD. De plus, un tel projet nous a permis d’apprendre à gérer un projet du début jusqu’à la fin et ce en travaillant en équipe. Ainsi, au cours de cette expérience, nous devions faire preuve d’esprit d’équipe, de rigueur, d’anticipation, d’adaptabilité et de polyvalence face aux différentes tâches que nous devrons traiter.
Dans ce dernier rapport, vous trouverez : 
- un résumé des possibilités de notre application,
- une description détaillée et justifiée de notre architecture,
- un argumentaire détaillé de l'adaptabilité de notre application,
- le planning effectif de notre projet,
- la participation effective des membres du groupe.

## Les possibilités de l'application

- Notre application possède plusieurs modes de jeu, grâce à deux types de joueurs : IA ou humain. L'application permet des parties aussi bien en humain contre humain, IA contre humain ou IA contre IA. Au début d'une partie, les noms des joueurs (humain/IA) sont paramétrables. Cependant faute de temps, nous avons simplement implémenté une IA aléatoire, il n'y a donc pas de paramétrage possible pour le niveau d'adversité de l'IA. A noter qu'il est tout à fait possible et simple d'implémenter une nouvelle IA plus développée grâce au Design Pattern Strategy_Player (détaillé plus loin dans ce rapport) et donc de paramétrer un niveau d'adversité de l'IA au début du jeu.

- Il est possible de procéder à une reprise ou à l'abandon d'une éventuelle partie en cours. La reprise d'une partie est proposée au début du jeu et permet de reprendre la dernière partie jouée si elle n'a pas été finie. Pour abandonner une partie en cours, il suffit de stopper le programme.

- Durant la partie, il est demandé à l'utilisateur (si c'est un humain) de valider ou non son choix d'action et ce à chaque étape du tour. Si le choix n'est pas validé, l'action s'annule.

- Un historique des scores est disponible et permet de sauvegarder pour chaque partie les caractéristiques suivantes : le score des deux joueurs, le joueur perdant et le joueur gagnant avec pour chacun le type de joueur, son nom, son nombre de parties jouées et son nombre de parties gagnées. Le nombre de matchs disputés est également sauvegardé.


## Architecture

L’architecture a été pensée, dans un premier temps, après une analyse générale du jeu Splendor Duel. Nous nous sommes mis d’accord sur les classes, puis nous avons attaqué le développement de ces dernères après une répartition des tâches. Cependant, nous nous sommes vite rendu compte de certains problèmes. Par exemple, au début, nous avons pensé de faire une classe mère *Jeton* puis de faire hériter chaque type de jeton de cette classe, avec sa couleur. Après le début du développement nous avons décidé de créer juste une seule classe qui aura comme attribut le type de jeton (une énumération), cela a rendu le développement beaucoup plus simple. De plus, après avoir été familiarisé sur la partie du cours sur le transtypage, nous avons compris que rester avec la première idée aurait imposé à un moment donné l’utilisation des dynamic cast afin de faire la différence entre les jetons. Ce problème a été évité avec la nouvelle solution. Un autre aspect est la maintenabilité du code: nous avons essayé de faire le maximum afin de le rendre le plus maintenable possible. Notamment, aux endroits où cela a été possible, nous avons ajouté des attributs de type ‘max’, comme par exemple le nombre maximal de jetons ou de cartes afin de rendre possible des éventuelles mises à jour dans le futur.

La création d’une classe dédiée à chaque objet dans notre projet, accompagnée de l’utilisation intensive de vecteurs pour stocker les données, a été une décision stratégique visant à simplifier le développement et à rendre le code plus robuste. En exploitant les fonctionnalités intégrées des vecteurs, telles que les itérateurs, l’ajout et la suppression d’éléments, ainsi que la recherche, nous avons pu bénéficier d’une flexibilité accrue dans la manipulation des données au sein de chaque classe.

Cependant, la création de classes individuelles ne représentait qu’une partie du défi. La véritable difficulté résidait dans l’établissement de liens cohérents entre ces classes pour assurer un fonctionnement harmonieux du jeu. Cette interconnexion a exigé une réflexion approfondie sur les dépendances entre les objets du jeu, garantissant que chaque action effectuée dans une classe impacte de manière appropriée les autres composants du système. La coordination de ces interactions a demandé une attention particulière pour éviter les erreurs potentielles et garantir la cohérence globale du jeu.

Nous allons à présent détailler, pour chaque classe, certaines décisions d'architecture.

### Les modules (classes)

#### Jeu

La classe Jeu joue un rôle central dans notre développement, agissant en tant que contrôleur principal du jeu. Elle assume la responsabilité de l’initialisation et de la destruction de tous les éléments du jeu, tels que les joueurs, les cartes, les jetons, et autres. En conséquence, nous avons pris la décision d’appliquer le Design Pattern Singleton à cette classe, garantissant ainsi qu’il n’existe qu’une seule instance de cet objet dans le jeu.

Cette classe englobe également des méthodes essentielles pour exécuter les “grandes actions” du jeu, telles que le remplissage du plateau. De plus, elle est chargée de contrôler l’état global du jeu, coordonnant les tours des joueurs et mettant fin à la partie si l’un d’entre eux remporte la victoire.

**Remarque**:

La responsabilité du jeu s’étend à la construction et à la destruction de la plupart des objets, mais une particularité intervient lors de la destruction des instances de joueur. Dans ce contexte, seuls les privilèges et les cartes royales sont également détruits, simultanément avec la libération de la main du joueur. Cette décision découle du fait qu’une fois qu’un joueur a acquis une carte ou un privilège, celui-ci est retiré du jeu pour éviter toute duplication et préserver l’intégrité de la composition globale.

#### Strategy_player (Joueur) : l’humain et l’IA

Le module Strategy_player représente les participants au jeu. Chaque instance de cette classe encapsule les informations spécifiques à un participant, telles que son nom, le nombre de points accumulés, le nombre de couronnes, le nombre de cartes de joaillerie et royales acquises, le nombre de jetons, et le nombre de privilèges détenus. Certains de ces attributs sont soumis à des limites maximales définies par des constantes statiques, reflétant les contraintes du jeu.

Les collections de cartes de joaillerie achetées, réservées et royales sont gérées à l’aide de std::vector. De même, les jetons, privilèges et autres agrégats de données sont également stockés et gérés de la même manière. L’utilisation des vecteurs nous a fait gagner beaucoup de temps et nous a permis de garantir une certaine robustesse de notre code.

La classe Strategy_player expose des méthodes communes aux joueurs et IA: accès et modification d’attributs, remplissage du plateau, etc.

Les classes filles donnent les méthodes nécessitant une adaptation en fonction du cas d’utilisation (joueur ou IA). En effet, la plupart de ces méthodes sont sensiblement équivalentes. Cependant, les entrées au clavier d’un humain sont remplacées par des choix aléatoires pour l’IA.

L’utilisation du Design Pattern strategy permet de garantir un ajout ultérieur d’autres IA de façon simple et en ne remettant pas en cause le fonctionnement des types joueur et IA. L’ajout d’un nouveau type d’IA ou de joueur ne nécessiterait qu’un développement d’une classe fille de Strategy_player qui serait adapté à l’utilisation voulue pour ce nouveau type de participant.

**Remarque**:

Après avoir développé l’IA, nous avons pu tester le programme principal (main)
en faisant jouer une IA contre une autre
    IA.L’intérêt de cette manipulation a été de vérifier le bon fonctionnement
        d’une partie dans son intégralité et d’assurer que le programme ne tombe
            pas dans des états particuliers que nous n’aurions pas suspecté en
                ayant joué nous mêmes.Par exemple,
    nous nous sommes rendus compte que nous n’avions pas géré le cas dans lequel
            un joueur ou une IA voulait piocher un jeton sur le plateau alors
                qu’il ne restait plus que des jetons or
        et que le joueur ne pouvait plus réserver de
        carte(donc ne peut pas piocher de jetons or),
    ce qui provoquait une boucle infinie.De nombreux cas similaires à celui
        - ci ont pu être identifiés grâce à cette ‘simulation’.Ainsi,
    le développement de l’IA nous a permis d’apporter plus de
    robustesse à notre programme principal
        .

    ####Carte

    Nous avons une classe mère Card et
    2 classes filles Royal_Card et JewelryCard
        .L’idée de l’héritage permet de conserver les propriétés communes aux
    2 classes filles afin de ne pas avoir à dupliquer le code et à respecter le
    principe SOLID
        .

    ####Tirage

    Cette classe constitue une représentation de chacun des trois tirages
    disponibles dans le jeu.Chaque tirage est essentiellement une
    collection de cartes,
    et pour simplifier la gestion de ces cartes,
    nous avons opté pour
    l’utilisation de vecteurs.Cette approche offre une souplesse significative,
    notamment lors du remplissage du tirage,
    où il suffit de piocher une carte depuis la pioche et de l’ajouter au tirage
        en utilisant la fonction “push” associée aux vecteurs.

        L’affichage du contenu complet d’un tirage est
        également rendu aisé grâce à l’opérateur “
        <<”,
    permettant ainsi une visualisation rapide pendant le
    déroulement du jeu.De plus,
    une fonction clé, “getCarte()”,
    a été implémentée pour faciliter le transfert d’une carte du tirage vers la
    main du joueur.

    Étant donné qu’il n’y a que trois tirages dans le jeu,
    nous avons introduit un attribut statique de classe pour limiter le nombre
    maximal de tirages.Cette approche assure la présence constante de seulement
    trois instances de la classe de tirage,
    garantissant ainsi une gestion efficace et cohérente de cette
    composante du jeu.

    En résumé,
    la classe tirage permet non seulement la gestion des cartes,
    mais également l’affichage et le transfert fluide de ces cartes entre le
    tirage et la main du joueur
        .L’utilisation judicieuse des vecteurs et l’utilisation des membres
    statiques contribuent à la robustesse et à l’efficacité de cette composante.

    ####Pioche

    La classe Pioche est conçue pour représenter une collection de cartes de
    joaillerie,
    principalement implémentée à l’aide d’un vecteur.À l’initialisation,
    chaque pioche est mélangée pour introduire une composante
    aléatoire dans le jeu.Comme dans le cas des tirages,
    un attribut statique est utilisé pour
    restreindre le nombre de pioches créées à un maximum de 3.

    ####Sac

    La classe Sac représente une collection de jetons,
    structurée au moyen d’un vecteur.Contrairement à d’autres composants du jeu,
    aucune composante aléatoire n’est intégrée dans cette classe.L’aspect
    aléatoire est introduit lors du remplissage du plateau,
    où les jetons sont attribués de manière aléatoire.La classe Sac adopte
    également le design pattern ‘Singleton’,
    reflétant la singularité de cette entité dans l’ensemble du jeu,
    car il n’y a qu’un seul sac utilisé.Le Sac n’a pas de
    responsabilité pour les jetons,
    qui sont crées et libérés avec le Jeu
        .

    ####Plateau

    La classe Plateau représente une collection de jetons
    organisée à l’aide d’un tableau à une dimension(vecteur) de
    25 pointeurs vers des jetons constants.Pour simuler la structure matricielle
    5x5 d’un véritable plateau,
    des opérations modulo sont
    employées pour naviguer dans le vecteur.Par exemple,
    le jeton d’indice 5 correspond au dessous du jeton d’indice
    0. Le recours au Design Pattern ‘Singleton’ s’impose également,
    étant donné qu’il existe une seule instance de plateau dans le jeu.

    L’aspect aléatoire des jetons prend place au moment du
    remplissage du plateau,
    ajoutant une dimension de variabilité au jeu
        .

    ####Privilège

    La classe Privilege représente l’une des classes les plus simples de notre
    projet.Son design a été pensé de manière à restreindre le nombre
    d’instances de privilèges à trois,
    grâce à l’utilisation d’un membre statique.Chaque privilège est
    identifié par un identificateur distinct,
    simplifiant ainsi leur manipulation au sein du système
        .Cette approche minimaliste contribue à la clarté et à
    l’efficacité de la gestion des privilèges.

    ## #Macth

    ## #History

    La classe History représente l
    'historique des parties jouées sur notre jeu. Un objet de la classe History est un objet pouvant regrouper tous les matchs de tous les joueurs du jeu (on parle ici des instance de Strategy_player, et pas uniquement de la classe fille Joueur). De ce fait, nous avons convenu que la classe serait implémentée à l' aide
    du design pattern Singleton,
    afin d 'en garantir l' unicité.Cette classe assure l
    'enregistrement des matchs au sein d' un fichier json,
    ainsi que la récupération des données de ce fichier pour permettre l
    'affichage de l' historique aux joueurs.De plus,
    nous avons choisi pour cette classe qu
    'un joueur entrant un nom déjà contenu dans l' historique sera alors
    considéré comme le même joueur.Non seulement cette classe permet cela,
    mais elle permettra en plus au jeu de repartir du même pointeur de joueur.

    ## #Développement du Main

    Le programme principal(main) assure le bon déroulement d’une partie. C’est le développement de celui-ci qui nous a causé le plus de problèmes. En effet, le main doit garantir la bonne interaction entre l’ensemble des classes. De plus, après avoir fait le choix d’utiliser le design pattern ‘Strategy’ pour que le main soit adapté autant pour un joueur humain qu’une IA, nous avons du reprendre le main dans son intégralité car celui-ci n’était pas adapté à l’utilisation de ce design pattern.

## Évoluabilité

Tout au long de la conception du projet, il nous a tenu à coeur de faire attention à ce que notre structure soit facilement modifiable, et ce dès le commencement. 

Par exemple, utilisation du design pattern Strategy pour les classes de joueurs. Il suffit de rajouter une nouvelle classe qui hérite de Strategy_player pour ajouter un nouveau type de joueur ou d'IA, par exemple une IA avec un niveau de difficulté plus élevé. En utilisant le principe de polymorphisme, les classes filles (Joueur et IA) peuvent hériter du comportement de la classe mère, et grâce aux méthodes virtuelles, on peut facilement surcharger des méthodes, ou réutiliser les méthodes de la classe mère sur un objet d'une classe fille.
<img src="UML_Strategy_player.png" alt="drawing" width="200"/>

Nous avons seulement un ou deux attributs à changer pour rajouter des tirages, des pioches, des jetons... Capacités et couleurs gérés dans des enums class -> suffit de rajouter le nom dans l'enum 

## Planning

Deux premières semaines : Première structure du projet avec définition d'un premier UML et réflexions sur l'implémentation des classes.

Deux dernières semaines : Fin du débug du main, ajout de l'historique et de la sauvegarde de la partie en cours ainsi que Qt.

## Contribution

Pourcentage de contribution sur l'ensemble du projet :
(à titre indicatif pour l'instant)
- Alexandru : 25%
- Aubin : 25%
- Gaspard : 15%
- Léopold : 20%
- Quentin : 15%

Temps consacré au projet :
- Alexandru : ~
- Aubin : ~
- Gaspard : ~
- Léopold : ~
- Quentin : ~
