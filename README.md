Ce projet a été réalisé dans le cadre du cursus pré-Ing2 (2025-2026) à CY Tech. 
Il consiste en un programme d'analyse de données massive concernant le réseau de distribution d'eau en France.
L'application combine un script Shell pour l'orchestration et des programmes en C pour les calculs intensifs, permettant de générer des histogrammes de performance des usines et de détecter les fuites sur le réseau.

Le programme propose deux fonctionnalités principales:
1.Histo (histo): Génération d'histogrammes classant les usines selon différents critères (Capacité, Volume capté, Volume réel traité).
2.Fuites (leaks): Calcul du volume total de perte d'eau pour une usine spécifique en parcourants l'ensemble de son réseau de distribution aval.

I/Usage
Le point d'entrée unique du programme est le script myScript.sh.
Syntaxe générale: ./myScript.sh [FICHIER_CSV] [COMMANDE] [OPTION]

  1. Génération d'Histogrammes (histo)

Cette commande analyse les usines et génère des graphiques PNG dans le dossier graphe/
Syntaxe général: ./myScript.sh data.csv histo [mode]
Modes disponibles :
  -max : Classement par capacité maximale de traitement12.
  -src : Classement par volume total capté aux sources13.
  -real : Classement par volume réellement traité (après pertes captage).

Sortie: -Affiche les graphiques via xdg-open. 
        -Crée deux images : une pour les 10 plus grandes usines et une pour les 50 plus petites.

 2. Détection de Fuites (leaks)
Cette commande calcule le rendement d'une usine spécifique en mesurant les pertes jusqu'aux usagers.
Syntaxe général : ./myScript.sh data.csv leaks "Nom_De_L_Usine"
(Attention : Le nom de l'usine doit être entre guillemets si il contient des espaces).

Sortie: -Affiche le volume traité, le volume perdu et le rendement dans la console.
        -Enregistre le résultat dans data/historique_rendements.dat.
        -Si l'usine est introuvable, enregistre une ligne avec la valeur -117.


3.Exemples d'exécution:

Générer l'histogramme des capacités max
./myScript.sh Data/data.csv histo max

 Calculer les fuites pour l'usine "Facility complex #RH400057F"
./myScript.sh Data/data.csv leaks "Facility complex #RH400057F"

