#ifndef LEAKS_H
#define LEAKS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*---------- STRUCTURES ----------*/

typedef struct Noeud {
    char* id;
    double volume_entree;
    struct Enfant* liste_fils;
    int nb_fils;
} Noeud;

typedef struct Enfant {
    struct Noeud* ptr_noeud;
    double ratio_fuite;
    struct Enfant* suivant;
} Enfant;

typedef struct AVL {
    char* id;
    Noeud* adresse_noeud;
    struct AVL *gauche;
    struct AVL *droit;
    int eq;
} AVL;

/*----------FONCTIONS ----------*/

int min(int a, int b);
int max(int a, int b);
int min3(int a, int b, int c);
int max3(int a, int b, int c);

// GESTION
AVL* creerAVL(char* id, Noeud* n);
AVL* rotationGauche(AVL* a);
AVL* rotationDroite(AVL* a);
AVL* doubleRotationGauche(AVL* a);
AVL* doubleRotationDroite(AVL* a);
AVL* equilibrerAVL(AVL* a);
AVL* insertionAVL(AVL* a, char* id, Noeud* n, int* h);
Noeud* rechercherAVL(AVL* a, char* id);
Noeud* creerNoeud(char* id);
void ajouterLienEnfant(Noeud* parent, Noeud* enfant, double fuite);
AVL* chargerDonnees(char* nomFichier, AVL* racineAVL, int* h, char* idUsine);

//CALCUL
double calculerTotalPertes(Noeud* actuel, double volume_entrant);

//MEMOIRE
void libererReseau(AVL* a);
void libererAVL(AVL* a);

#endif