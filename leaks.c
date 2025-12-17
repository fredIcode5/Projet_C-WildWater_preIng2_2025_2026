#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*----------STRUCTURE----------*/

typedef struct Noeud {
    char* id;                // Identifiant unique de l'acteur (ex: "Storage #13178")
    double volume_entree;       // Volume d'eau arrivant dans ce nœud
    struct Enfant* liste_fils;  // Liste chaînée des nœuds en aval (enfants)
    int nb_fils;                // Nombre d'enfants pour diviser le volume équitablement 
} Noeud;

typedef struct Enfant {
    struct Noeud* ptr_noeud;    // Pointeur vers le nœud enfant
    double ratio_fuite;         // % de fuite sur ce tronçon spécifique [cite: 64, 75]
    struct Enfant* suivant;     // Prochain enfant dans la liste
} Enfant;

typedef struct AVL {
    char* id;               // Clé de recherche (l'identifiant)
    Noeud* adresse_noeud;       // Pointeur vers le nœud correspondant dans l'arbre principal
    struct AVL *gauche;    // Fils gauche
    struct AVL *droit;    // Fils droit
    int eq;                // Pour l'équilibrage de l'AVL [cite: 204, 209]
} AVL;

/*----------GESTION----------*/

int min(int a, int b) { return (a < b) ? a : b; }
int max(int a, int b) { return (a > b) ? a : b; }

int min3(int a, int b, int c) { return min(a, min(b, c)); }
int max3(int a, int b, int c) { return max(a, max(b, c)); }

AVL* creerAVL(char* id, struct Noeud* n) {
    AVL* nouveau = (AVL*)malloc(sizeof(AVL));
    if (nouveau == NULL){
        exit(EXIT_FAILURE);
    }
    nouveau->id = strdup(id);
    nouveau->adresse_noeud = n;
    nouveau->gauche = NULL;
    nouveau->droit = NULL;
    nouveau->eq = 0;
    return nouveau;
}

AVL* rotationGauche(AVL* a) {
    AVL* pivot = a->droit;
    int eq_a = a->eq;
    int eq_p = pivot->eq;

    a->droit = pivot->gauche;
    pivot->gauche = a;

    a->eq = eq_a - max(eq_p, 0) - 1;
    pivot->eq = min3(eq_a - 2, eq_a + eq_p - 2, eq_p - 1);

    return pivot;
}

AVL* rotationDroite(AVL* a) {
    AVL* pivot = a->gauche;
    int eq_a = a->eq;
    int eq_p = pivot->eq;

    a->gauche = pivot->droit;
    pivot->droit = a;

    a->eq = eq_a - min(eq_p, 0) + 1;
    pivot->eq = max3(eq_a + 2, eq_a + eq_p + 2, eq_p + 1);

    return pivot;
}

AVL* doubleRotationGauche(AVL* a) {
    a->droit = rotationDroite(a->droit);
    return rotationGauche(a);
}

AVL* doubleRotationDroite(AVL* a) {
    a->gauche = rotationGauche(a->gauche);
    return rotationDroite(a);
}

AVL* equilibrerAVL(AVL* a) {
    if (a->eq >= 2) {
        if (a->droit->eq >= 0) return rotationGauche(a);
        return doubleRotationGauche(a);
    } else if (a->eq <= -2) {
        if (a->gauche->eq <= 0) return rotationDroite(a);
        return doubleRotationDroite(a);
    }
    return a;
}

AVL* insertionAVL(AVL* a, char* id, Noeud* n, int *h) {
    if (a == NULL) {
        *h = 1;
        return creerAVL(id, n);
    }
    int comp = strcmp(id, a->id);
    if (comp < 0) {
        a->gauche = insertionAVL(a->gauche, id, n, h);
        *h = -(*h);
    } else if (comp > 0) {
        a->droit = insertionAVL(a->droit, id, n, h);
    } else {
        *h = 0;
        return a;
    }
    if (*h != 0) {
        a->eq += *h;
        a = equilibrerAVL(a);
        *h = (a->eq == 0) ? 0 : 1;
    }
    return a;
}

Noeud* rechercherAVL(AVL* a, char* id) {
    if (a == NULL) return NULL;
    int comp = strcmp(id, a->id);
    if (comp == 0) return a->adresse_noeud;
    if (comp < 0) return rechercherAVL(a->gauche, id);
    return rechercherAVL(a->droit, id);
}

Noeud* creerNoeud(char* id) {
    Noeud* n = (Noeud*)malloc(sizeof(Noeud));
    if (n == NULL) exit(EXIT_FAILURE);
    n->id = strdup(id);
    n->volume_entree = 0;
    n->liste_fils = NULL;
    n->nb_fils = 0;
    return n;
}

void ajouterLienEnfant(Noeud* parent, Noeud* enfant, double fuite) {
    Enfant* nouveauRelais = (Enfant*)malloc(sizeof(Enfant));
    if (nouveauRelais == NULL) exit(EXIT_FAILURE);
    
    nouveauRelais->ptr_noeud = enfant;
    nouveauRelais->ratio_fuite = fuite;
    // Insertion en tête de la liste chaînée des fils
    nouveauRelais->suivant = parent->liste_fils;
    parent->liste_fils = nouveauRelais;
    parent->nb_fils++;
}

AVL* chargerDonnees(char* nomFichier, AVL* racineAVL, int* h, char* idUsine) {
    FILE* f = fopen(nomFichier, "r");
    if (f == NULL) {
        perror("Erreur lors de l'ouverture du fichier");
        return racineAVL;
    }

    char ligne[512], id_amont[100], id_aval[100], vol_str[64], fuite_str[64];
    double volume, fuite;

    while (fgets(ligne, sizeof(ligne), f)) {
        if (sscanf(ligne, "%99[^;];%99[^;];%63[^;];%63[^;\n]", id_amont, id_aval, vol_str, fuite_str) == 4) {
            volume = (vol_str[0] == '-') ? 0.0 : atof(vol_str);
            fuite = (fuite_str[0] == '-') ? 0.0 : atof(fuite_str);

            // Gestion Amont
            Noeud* n_amont = rechercherAVL(racineAVL, id_amont);
            if (n_amont == NULL) {
                n_amont = creerNoeud(id_amont);
                racineAVL = insertionAVL(racineAVL, id_amont, n_amont, h);
            }

            // Gestion Aval
            Noeud* n_aval = rechercherAVL(racineAVL, id_aval);
            if (n_aval == NULL) {
                n_aval = creerNoeud(id_aval);
                racineAVL = insertionAVL(racineAVL, id_aval, n_aval, h);
            }

            // Si l'aval de la ligne est notre usine, c'est l'entrée d'eau principale
            if (strcmp(id_aval, idUsine) == 0) {
                n_aval->volume_entree += volume;
            }

            ajouterLienEnfant(n_amont, n_aval, fuite);
        }
    }
    fclose(f);
    return racineAVL;
}

/*----------CALCUL----------*/

double calculerTotalPertes(Noeud* actuel, double volume_entrant) {
    if (actuel == NULL || actuel->nb_fils == 0) {
        return 0; // On est arrivé au bout (consommateur final)
    }

    double pertes_totales = 0;
    // Répartition équitable du volume entrant entre chaque enfant
    double volume_par_fils = volume_entrant / actuel->nb_fils;

    Enfant* temp = actuel->liste_fils;
    while (temp != NULL) {
        // Calcul de la perte absolue sur ce tronçon spécifique
        double perte_troncon = volume_par_fils * (temp->ratio_fuite / 100.0); // ratio_fuite est en pourcentage
        
        // Le volume qui arrive réellement au nœud suivant
        double volume_restant = volume_par_fils - perte_troncon;

        // On cumule la perte actuelle + les pertes dans le sous-arbre de l'enfant
        pertes_totales += perte_troncon + calculerTotalPertes(temp->ptr_noeud, volume_restant);
        
        temp = temp->suivant;
    }

    return pertes_totales;
}

/*----------MEMOIRE----------*/

void libererReseau(AVL* racineAVL) {
    if (racineAVL == NULL) return;
    
    // On parcourt l'AVL pour atteindre chaque Noeud unique
    libererReseau(racineAVL->gauche);
    libererReseau(racineAVL->droit);
    
    Noeud* n = racineAVL->adresse_noeud;
    // Libérer la liste des enfants
    Enfant* actuel = n->liste_fils;
    while (actuel != NULL) {
        Enfant* temp = actuel;
        actuel = actuel->suivant;
        free(temp);
    }
    // Libérer l'ID et le Noeud 
    free(n->id);
    free(n);
}

// Libère l'index AVL
void libererAVL(AVL* a) {
    if (a == NULL) return;
    libererAVL(a->gauche);
    libererAVL(a->droit);
    free(a->id); 
    free(a);
}

/*----------MAIN----------*/

int main(int argc, char* argv[]) {
    if (argc < 3) {
        fprintf(stderr, "Usage: %s <fichier_csv> <id_usine>\n", argv[0]);
        return 1;
    }

    char* nomFichier = argv[1];
    char* idUsine = argv[2];
    
    AVL* racine = NULL;
    int h = 0;

    // 1. Chargement
    racine = chargerDonnees(nomFichier, racine, &h, idUsine);

    // 2. Recherche de l'usine de départ
    Noeud* usine = rechercherAVL(racine, idUsine);
    if (usine == NULL) {
        fprintf(stderr, "Erreur : Usine %s introuvable.\n", idUsine);
        libererReseau(racine);
        libererAVL(racine);
        return 2;
    }

    // 3. Calcul
    double totalFuites = calculerTotalPertes(usine, usine->volume_entree);
    double rendement = ((usine->volume_entree - totalFuites) / usine->volume_entree) * 100;

    // 4. Affichage
    printf("Usine: %s | Volume Traité: %.2f m3 | Total Fuites: %.2f m3 (Rendement: %.2f%%)\n", 
            idUsine, usine->volume_entree, totalFuites, rendement);

    // 5. Nettoyage final
    libererReseau(racine);
    libererAVL(racine);

    return 0;
}