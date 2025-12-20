Ce projet a été réalisé dans le cadre du cursus pré-Ing2 (2025-2026) à CY Tech.

Il consiste en un programme d'analyse de données concernant le réseau de distribution d'eau (couvrant une large partie du territoire, environ "1/3 de la France" ). L'application combine un script Shell pour l'orchestration et des programmes en C pour les calculs, permettant de générer des histogrammes de performance des usines et de détecter les fuites sur le réseau.

Structure du Projet
L'arborescence du dossier projet_finale doit être organisée comme suit:

projet_finale/

├── include/

│   ├── histogram.h

│   └── leaks.h

├── source/

│   ├── histogram.c

│   └── leaks.c

├── Makefile

├── myScript.sh

I. Installation et Préparation
Avant de lancer le programme, veuillez suivre ces étapes scrupuleusement pour garantir la bonne exécution des scripts.



1.Mise en place des données : Téléchargez et placez les fichiers de données suivants directement dans le dossier projet_finale :
  -c-wildwater_v0.dat
  -c-wildwater_v3.dat 

2.Préparation de l'environnement : Ouvrez un terminal et placez-vous dans le dossier du projet :
   cd chemin/vers/projet_finale
3.Lancez les commandes suivantes:
  sed -i 's/\r$//' myScript.sh
  
  chmod +x creation_histo.sh myScript.sh


II. Utilisation
Le point d'entrée unique du programme est le script myScript.sh.

Syntaxe générale :
./myScript.sh [FICHIER_DONNÉES] [COMMANDE] [OPTION]

Le programme propose deux fonctionnalités principales :

1. Génération d'Histogrammes (histo)
Cette commande analyse les stations et génère des graphiques au format PNG.

Syntaxe : ./myScript.sh [FICHIER] histo [mode]

Modes disponibles :

  max : Classement par capacité maximale de traitement.
  src : Classement par volume total capté aux sources.
  real : Classement par volume réellement traité (après pertes captage).

Résultats :
Les graphiques sont générés automatiquement dans le dossier graphe/.
Deux images sont créées par exécution : une pour les 10 plus grandes usines et une pour les 50 plus petites.
Les graphiques s'affichent automatiquement à la fin du traitement.

2. Détection de Fuites (leaks)
Cette commande calcule le rendement d'une usine spécifique en mesurant les pertes jusqu'aux usagers (en parcourant l'ensemble de son réseau de distribution aval).

Syntaxe : ./myScript.sh [FICHIER] leaks "Nom_De_L_Usine" 
(Note : Le nom de l'usine doit être entre guillemets s'il contient des espaces).

Résultats :
Affiche le volume traité, le volume perdu et le rendement directement dans la console.
L'historique des résultats est sauvegardé dans le fichier data/historique_rendements.dat.
Si l'usine est introuvable, le programme enregistre une ligne avec la valeur -1.


III. Exemples d'exécution
Voici des exemples concrets pour tester le programme (assurez-vous d'utiliser les noms de fichiers présents dans le dossier, ex: c-wildwater_v0.dat) :

Générer l'histogramme des capacités max :
./myScript.sh c-wildwater_v0.dat histo max

Calculer les fuites pour l'usine "Facility complex #RH400057F" :
./myScript.sh c-wildwater_v0.dat leaks "Facility complex #RH400057F"
