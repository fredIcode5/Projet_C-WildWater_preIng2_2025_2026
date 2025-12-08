#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* ============================================================
   STRUCTURE AVL
   ============================================================ */

typedef struct avl {
    char source[64];
    char identifiant[32];
    long volume;
    struct avl *fg;
    struct avl *fd;
    int eq;
} avl;

/* ============================================================
   FONCTIONS UTILES
   ============================================================ */

int max2(int a, int b) { return a > b ? a : b; }
int min2(int a, int b) { return a < b ? a : b; }

int max3(int a, int b, int c) { return max2(a, max2(b, c)); }
int min3(int a, int b, int c) { return min2(a, min2(b, c)); }

/* ============================================================
   CREATION D'UN NOEUD
   ============================================================ */

avl* createAVL(const char* source, const char* id, long vol) {
    avl* p = malloc(sizeof(avl));
    if (!p) exit(1);

    strcpy(p->source, source);
    strcpy(p->identifiant, id);
    p->volume = vol;
    p->fg = NULL;
    p->fd = NULL;
    p->eq = 0;

    return p;
}

/* ============================================================
   ROTATIONS
   ============================================================ */

avl* rotateLeft(avl* pRoot) {
    avl* pPivot = pRoot->fd;
    pRoot->fd = pPivot->fg;
    pPivot->fg = pRoot;

    int eqa = pRoot->eq;
    int eqp = pPivot->eq;

    pRoot->eq = eqa - max2(eqp, 0) - 1;
    pPivot->eq = min3(eqa - 2, eqa + eqp - 2, eqp - 1);

    return pPivot;
}

avl* rotateRight(avl* pRoot) {
    avl* pPivot = pRoot->fg;
    pRoot->fg = pPivot->fd;
    pPivot->fd = pRoot;

    int eqa = pRoot->eq;
    int eqp = pPivot->eq;

    pRoot->eq = eqa - min2(eqp, 0) + 1;
    pPivot->eq = max3(eqa + 2, eqa + eqp + 2, eqp + 1);

    return pPivot;
}

avl* doubleRotateLeft(avl* pRoot) {
    pRoot->fd = rotateRight(pRoot->fd);
    return rotateLeft(pRoot);
}

avl* doubleRotateRight(avl* pRoot) {
    pRoot->fg = rotateLeft(pRoot->fg);
    return rotateRight(pRoot);
}

/* ============================================================
   EQUILIBRAGE
   ============================================================ */

avl* balanceAVL(avl* pRoot) {
    if (pRoot->eq >= 2) {
        if (pRoot->fd->eq >= 0)
            pRoot = rotateLeft(pRoot);
        else
            pRoot = doubleRotateLeft(pRoot);
    }
    else if (pRoot->eq <= -2) {
        if (pRoot->fg->eq <= 0)
            pRoot = rotateRight(pRoot);
        else
            pRoot = doubleRotateRight(pRoot);
    }
    return pRoot;
}

/* ============================================================
   INSERTION (CUMULE LES VOLUMES SI IDENTIFIANT EXISTANT)
   ============================================================ */

avl* insertAVL(avl* pRoot, const char* source, const char* id, long vol, int* h) {
    if (pRoot == NULL) {
        *h = 1;
        return createAVL(source, id, vol);
    }

    int cmp = strcmp(id, pRoot->identifiant);

    if (cmp < 0) {
        pRoot->fg = insertAVL(pRoot->fg, source, id, vol, h);
        *h = -*h;
    }
    else if (cmp > 0) {
        pRoot->fd = insertAVL(pRoot->fd, source, id, vol, h);
    }
    else {
        pRoot->volume += vol;
        *h = 0;
        return pRoot;
    }

    if (*h != 0) {
        pRoot->eq += *h;
        pRoot = balanceAVL(pRoot);
        if (pRoot->eq == 0) *h = 0;
        else *h = 1;
    }

    return pRoot;
}

/* ============================================================
   PARCOURS INFIXE (pour debug)
   ============================================================ */

void infix(avl* p) {
    if (p != NULL) {
        infix(p->fg);
        printf("%s;%s;%ld\n", p->source, p->identifiant, p->volume);
        infix(p->fd);
    }
}

/* ============================================================
   LIBERATION MEMOIRE
   ============================================================ */

void libererEspace(avl* p) {
    if (p != NULL) {
        libererEspace(p->fg);
        libererEspace(p->fd);
        free(p);
    }
}

/* ============================================================
   TRANSFORMATION AVL → TABLEAU POUR TRI
   ============================================================ */

typedef struct {
    char source[64];
    char identifiant[32];
    long volume;
} Node;

void avlToArray(avl* p, Node* arr, int* index) {
    if (p != NULL) {
        avlToArray(p->fg, arr, index);
        strcpy(arr[*index].source, p->source);
        strcpy(arr[*index].identifiant, p->identifiant);
        arr[*index].volume = p->volume;
        (*index)++;
        avlToArray(p->fd, arr, index);
    }
}

int countIdentifiers(avl* p) {
    if (p == NULL) return 0;
    return 1 + countIdentifiers(p->fg) + countIdentifiers(p->fd);
}

int compareVolume(const void* a, const void* b) {
    const Node* x = (const Node*)a;
    const Node* y = (const Node*)b;
    if (x->volume < y->volume) return -1;
    if (x->volume > y->volume) return 1;
    return 0;
}

/* ============================================================
   MAIN — TRI PAR VOLUME + FICHIER trié
   ============================================================ */

int main() {
    avl* racine = NULL;
    int h;
    char source[64];
    char id[32];
    long vol;
    char line[256];

    while (fgets(line, sizeof(line), stdin)) {
        if (sscanf(line, "%63[^#]#%31[^;];%ld", source, id, &vol) == 3) {
            size_t len = strlen(source);
            if (len > 0 && source[len - 1] == ' ') source[len - 1] = '\0';
            racine = insertAVL(racine, source, id, vol, &h);
        }
    }

    int n = countIdentifiers(racine);
    Node* arr = malloc(n * sizeof(Node));
    int index = 0;

    avlToArray(racine, arr, &index);

    qsort(arr, n, sizeof(Node), compareVolume);

    printf("\n===== CONTENU TRIÉ PAR VOLUME =====\n");
    for (int i = 0; i < n; i++) {
        printf("%s;%s;%ld\n", arr[i].source, arr[i].identifiant, arr[i].volume);
    }

    printf("\nNombre d'identifiants différents : %d\n", n);

    FILE* fichier = fopen("lst_usine.txt", "w");
    if (!fichier) {
        perror("Erreur ouverture lst_usine.txt");
        return 1;
    }

    for (int i = 0; i < n; i++) {
        fprintf(fichier, "%s;%s;%ld\n",
                arr[i].source, arr[i].identifiant, arr[i].volume);
    }

    fclose(fichier);
    printf("\nFichier lst_usine.txt généré avec succès (trié par volume) !\n");

    free(arr);
    libererEspace(racine);
    return 0;
}
