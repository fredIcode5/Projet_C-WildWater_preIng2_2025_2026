#!/bin/bash

echo "=== Génération de l'histogramme ==="

# Vérifie si gnuplot est installé
if ! command -v gnuplot &> /dev/null
then
    echo "Erreur : gnuplot n'est pas installé."
    echo "Installe-le avec : sudo apt install gnuplot"
    exit 1
fi

# Exécution du script Gnuplot
gnuplot plot_histogramme.gnu

# Vérification
if [ -f "histogramme.png" ]; then
    echo "Histogramme généré avec succès : histogramme.png"
else
    echo "Erreur : le fichier histogramme.png n'a pas été créé."
fi
