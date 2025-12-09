#!/bin/bash

if [ $# -lt 1 ]; then
    echo "Usage: $0 Fichier"
    echo "Exemple : $0 fichier.txt"
    exit 1
fi

Fichier=$1

if [ ! -f "$Fichier" ]; then
    echo "Erreur : le fichier '$Fichier' n'existe pas."
    exit 1
fi

# Compile le code C
gcc creeravlfinale.c -o creeravlfinale

# Envoie les informations au programme C
grep -E '^-\;[^-;]+;[^-;]+;[^-;]+;[^-;]+$' "$Fichier" \
    | cut -d';' -f2,4 \
    | ./creeravlfinale

echo "Les usines ont été enregistrées dans lst_usine.txt"
head -n 5 lst_usine.txt > extrait.txt
tail -n 5 lst_usine.txt >> extrait.txt

