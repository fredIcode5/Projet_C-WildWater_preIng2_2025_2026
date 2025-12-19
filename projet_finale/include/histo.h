#ifndef HISTO_H
#define HISTO_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* --- STRUCTURES --- */

typedef struct avl {
    char identifiant[128];
    long vol_max;   // Utilisé pour le volume brut (max ou dernier selon l'option)
    long vol_capte; // Utilisé pour le volume net cumulé
    float perte;    // Taux de perte
    struct avl *fg;
    struct avl *fd;
    int eq;
} avl;

typedef struct {
    char identifiant[128];
    long vol_max;
    long vol_capte;
    float perte;
} Node;

/* --- PROTOTYPES --- */

// Gestion de l'arbre
avl* createAVL(const char* id, long vol1, float perte, const char* option);
avl* insertAVL(avl* pRoot, const char* id, long vol1, float perte, int* h, const char* option);
void freeAVL(avl* p);

// Équilibrage
avl* rotateLeft(avl* pRoot);
avl* rotateRight(avl* pRoot);
avl* doubleRotateLeft(avl* pRoot);
avl* doubleRotateRight(avl* pRoot);
avl* balanceAVL(avl* pRoot);

// Utilitaires
int max2(int a, int b);
int min2(int a, int b);
int countIdentifiers(avl* p);
void avlToArray(avl* p, Node* arr, int* index);
int compareVolume(const void* a, const void* b);

#endif
