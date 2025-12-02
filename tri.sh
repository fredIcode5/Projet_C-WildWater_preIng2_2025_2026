#!/bin/bash

# Vérifie qu'on a bien 2 arguments
if [ $# -lt 2 ]; then
    echo "Usage: $0 Fichier Identifiant_Plant"
    echo "Exemple : $0 fichier.txt JA200000I"
    exit 1
fi

FICHIER=$1          # Premier argument : le fichier source
IDENTIFIANT=$2      # Deuxième argument : l'identifiant du plant


# Vérifier que le fichier existe
if [ ! -f "$FICHIER" ]; then
    echo "Erreur : le fichier '$FICHIER' n'existe pas."
    exit 1
fi

grep "Plant #$IDENTIFIANT" "$FICHIER" > resultat.txt

echo "Les lignes après Plant #$IDENTIFIANT ont été enregistrées dans resultat.txt"