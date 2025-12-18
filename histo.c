#include "histo.h"

/* --- FONCTIONS UTILITAIRES --- */
int max2(int a, int b) { return a > b ? a : b; }
int min2(int a, int b) { return a < b ? a : b; }

/* --- GESTION AVL --- */

avl* createAVL(const char* id, long vol1, float perte, const char* option) {
    avl* p = malloc(sizeof(avl));
    if (!p) exit(1);

    strcpy(p->identifiant, id);
    p->fg = NULL;
    p->fd = NULL;
    p->eq = 0;

    if (strcmp(option, "real") == 0) {
        double perte_decimale = (double)perte / 100.0;
        p->vol_max = vol1; 
        p->vol_capte = (long)((double)vol1 * (1.0 - perte_decimale));
        p->perte = perte;
    } else if (strcmp(option, "max") == 0) {
        p->vol_max = vol1;
        p->vol_capte = 0;
        p->perte = 0.0;
    } else { // src
        p->vol_max = 0;
        p->vol_capte = vol1;
        p->perte = 0.0;
    }
    return p;
}

avl* rotateLeft(avl* pRoot) {
    avl* pPivot = pRoot->fd;
    pRoot->fd = pPivot->fg;
    pPivot->fg = pRoot;
    int eqa = pRoot->eq;
    int eqp = pPivot->eq;
    pRoot->eq = eqa - max2(eqp, 0) - 1;
    pPivot->eq = min2(min2(eqa - 2, eqa + eqp - 2), eqp - 1); // Remplacement de min3
    return pPivot;
}

avl* rotateRight(avl* pRoot) {
    avl* pPivot = pRoot->fg;
    pRoot->fg = pPivot->fd;
    pPivot->fd = pRoot;
    int eqa = pRoot->eq;
    int eqp = pPivot->eq;
    pRoot->eq = eqa - min2(eqp, 0) + 1;
    pPivot->eq = max2(max2(eqa + 2, eqa + eqp + 2), eqp + 1); // Remplacement de max3
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

avl* balanceAVL(avl* pRoot) {
    if (pRoot->eq >= 2) {
        if (pRoot->fd->eq >= 0) pRoot = rotateLeft(pRoot);
        else pRoot = doubleRotateLeft(pRoot);
    } else if (pRoot->eq <= -2) {
        if (pRoot->fg->eq <= 0) pRoot = rotateRight(pRoot);
        else pRoot = doubleRotateRight(pRoot);
    }
    return pRoot;
}

avl* insertAVL(avl* pRoot, const char* id, long vol1, float perte, int* h, const char* option) {
    if (pRoot == NULL) {
        *h = 1;
        return createAVL(id, vol1, perte, option);
    }

    int cmp = strcmp(id, pRoot->identifiant);
    if (cmp < 0) {
        pRoot->fg = insertAVL(pRoot->fg, id, vol1, perte, h, option);
        *h = -*h;
    } else if (cmp > 0) {
        pRoot->fd = insertAVL(pRoot->fd, id, vol1, perte, h, option);
    } else {
        // Mise à jour si existant
        if (strcmp(option, "max") == 0) {
            if (vol1 > pRoot->vol_max) pRoot->vol_max = vol1;
        } else if (strcmp(option, "src") == 0) {
            pRoot->vol_capte += vol1;
        } else if (strcmp(option, "real") == 0) {
            pRoot->vol_max = vol1;
            pRoot->perte = perte;
            double perte_decimale = (double)perte / 100.0;
            pRoot->vol_capte += (long)((double)vol1 * (1.0 - perte_decimale));
        }
        *h = 0;
        return pRoot;
    }

    if (*h != 0) {
        pRoot->eq += *h;
        pRoot = balanceAVL(pRoot);
        *h = (pRoot->eq == 0) ? 0 : 1;
    }
    return pRoot;
}

void freeAVL(avl* p) {
    if (p) {
        freeAVL(p->fg);
        freeAVL(p->fd);
        free(p);
    }
}

/* --- TRANSFORMATION ET TRI --- */

void avlToArray(avl* p, Node* arr, int* index) {
    if (p) {
        avlToArray(p->fg, arr, index);
        strcpy(arr[*index].identifiant, p->identifiant);
        arr[*index].vol_max = p->vol_max;
        arr[*index].vol_capte = p->vol_capte;
        arr[*index].perte = p->perte;
        (*index)++;
        avlToArray(p->fd, arr, index);
    }
}

int countIdentifiers(avl* p) {
    return p ? 1 + countIdentifiers(p->fg) + countIdentifiers(p->fd) : 0;
}

int compareVolume(const void* a, const void* b) {
    const Node* x = (const Node*)a;
    const Node* y = (const Node*)b;
    // Tri sur vol_max
    return (x->vol_max > y->vol_max) - (x->vol_max < y->vol_max);
}

/* --- MAIN --- */

int main(int argc, char* argv[]) {
    if (argc != 2) return 1;

    avl* racine = NULL;
    int h;
    char id[128], line[512];
    long vol1;
    float perte;
    const char* option = argv[1];

    while (fgets(line, sizeof(line), stdin)) {
        if (strcmp(option, "real") == 0) {
            if (sscanf(line, "%127[^;];%ld;%f", id, &vol1, &perte) == 3)
                racine = insertAVL(racine, id, vol1, perte, &h, option);
        } else {
            if (sscanf(line, "%127[^;];%ld", id, &vol1) == 2)
                racine = insertAVL(racine, id, vol1, 0.0, &h, option);
        }
    }

    int n = countIdentifiers(racine);
    if (n > 0) {
        Node* arr = malloc(n * sizeof(Node));
        int idx = 0;
        avlToArray(racine, arr, &idx);
        qsort(arr, n, sizeof(Node), compareVolume);

        FILE* f = fopen("lst_usine.txt", "w");
        if (f) {
            for (int i = 0; i < n; i++) {
                if (strcmp(option, "real") == 0)
                    fprintf(f, "%s;%ld;%ld\n", arr[i].identifiant, arr[i].vol_capte, arr[i].vol_max);
                else if (strcmp(option, "max") == 0)
                    fprintf(f, "%s;%ld\n", arr[i].identifiant, arr[i].vol_max);
                else
                    fprintf(f, "%s;%ld\n", arr[i].identifiant, arr[i].vol_capte);
            }
            fclose(f);
        }
        free(arr);
    }
    freeAVL(racine);
    return 0;
}
