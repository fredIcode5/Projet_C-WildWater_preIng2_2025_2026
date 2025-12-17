#!/bin/bash

# 1. Vérification des arguments
if [ $# -ne 2 ]; then
    echo "Usage: $0 <fichier_donnees.csv> <id_usine>"
    exit 1
fi

FILE_DATA=$1
ID_USINE=$2
EXEC="leaks_program"
TEMP_FILE="temp_leaks.csv"

# 2. Vérification de l'existence du fichier de données
if [ ! -f "$FILE_DATA" ]; then
    echo "Erreur : Le fichier '$FILE_DATA' est introuvable."
    exit 2
fi

# 3. Compilation automatique via le Makefile
if [ ! -f "$EXEC" ]; then
    echo "Compilation du programme C..."
    make
    if [ $? -ne 0 ]; then
        echo "Erreur lors de la compilation."
        exit 3
    fi
fi

# 4. Pré-traitement des données
echo "Extraction des données pour l'usine : $ID_USINE..."

#Lancement du chronomètre
start_time=$(date +%s.%N)

# On crée le fichier temporaire
# FILTRE 1 : Lignes où l'ID_USINE reçoit de l'eau (en 3ème position)
grep -E "^[^;]+;[^;]+;${ID_USINE};" "$FILE_DATA" > "$TEMP_FILE"

# FILTRE 2 : Lignes où l'ID_USINE distribue (en 2ème position)
grep -E "^[^;]+;${ID_USINE};[^;]+;" "$FILE_DATA" >> "$TEMP_FILE"

# FILTRE 3 : Réseau aval (si l'ID_USINE est en 1ère position comme usine traitante)
grep "^${ID_USINE};" "$FILE_DATA" >> "$TEMP_FILE"

# On nettoie pour ne garder que : Amont;Aval;Volume;Fuite
# On cible les colonnes 2, 3, 4 et 5
cut -d';' -f2,3,4,5 "$TEMP_FILE" > "${TEMP_FILE}.tmp" && mv "${TEMP_FILE}.tmp" "$TEMP_FILE"

# Vérification si l'usine existe dans le fichier
if [ ! -s "$TEMP_FILE" ]; then
    echo "Erreur : Aucune donnée trouvée pour l'usine '$ID_USINE'."
    rm -f "$TEMP_FILE"
    exit 4
fi

# 5. Lancement du programme C
echo "Lancement du calcul..."

# On lance le calcul, on affiche le résultat à l'écran et on l'ajoute à l'historique
./$EXEC "$TEMP_FILE" "$ID_USINE" | tee -a historique_rendements.dat

end_time=$(date +%s.%N)
# Utilisation de 'bc' pour une soustraction précise des nanosecondes
duration=$(echo "$end_time - $start_time" | bc)

echo "-------------------------------------------"
echo "Traitement TOTAL (Extraction + C) terminé en $duration secondes."
echo "-------------------------------------------"
# 6. Nettoyage du fichier temporaire (TOUJOURS EN DERNIER)
rm -f "$TEMP_FILE"

exit 0