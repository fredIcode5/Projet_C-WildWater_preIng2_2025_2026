#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define TAILLE_ID 100


// Cette structure représente le lien vers un enfant (nœud aval).
typedef struct Tuyau {
    struct Station* enfant;     // Pointeur vers la station "Fille" (destination)
    float volume;               // Volume d'eau qui passe dans ce tuyau (colonne 4 du CSV)
    float pourcentage_fuite;    // Pourcentage de fuite (colonne 5 du CSV)
    struct Tuyau* suivant;      // Pointeur vers le prochain tuyau (liste chaînée)
} Tuyau;


// Représente une Usine, un Stockage, une Jonction, etc.
typedef struct Station {
    char id[TAILLE_ID];         // L'identifiant unique (ex: "Plant #JA200000I")
    Tuyau* liste_tuyaux;        // Tête de la liste chaînée des connexions sortantes (enfants)
    
} Station;


// --- STRUCTURE 3 : Le Nœud AVL (L'index de recherche) ---
typedef struct AVL {
    char* id;                   // La Clé de tri : Identifiant de la station (copie dynamique)
    Station* station_ref;       // La Donnée : Pointeur vers la structure Station réelle définie plus haut
    int hauteur;                // Hauteur du nœud pour l'équilibrage de l'arbre
    struct AVL* gauche;         // Sous-arbre gauche (identifiants alphabétiquement plus petits)
    struct AVL* droite;         // Sous-arbre droit (identifiants alphabétiquement plus grands)
} AVL;


// Fonction pour créer un nouveau noeud AVL
AVL* creer_avl(char* id, Station* station) {
    AVL* nouveau = malloc(sizeof(AVL));
    if (nouveau == NULL) {
        exit(1); // Erreur d'allocation critique
    }
    
    // On copie l'ID pour que l'AVL soit indépendant
    nouveau->id = strdup(id); 
    
    // On pointe vers la station du réseau
    nouveau->station_ref = station;
    
    nouveau->hauteur = 1;
    nouveau->gauche = NULL;
    nouveau->droite = NULL;
    
    return nouveau;
}
