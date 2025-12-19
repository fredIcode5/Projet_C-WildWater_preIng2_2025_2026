#!/bin/bash


# Gestion du temps (millisecondes approx.)

start_time=$(date +%s.%N)


# Fonctions utilitaires

error_exit() {
    echo "Erreur : $1" >&2
    end_time=$(date +%s.%N)
    DURATION=$(echo "($end_time - $start_time) * 1000" | bc)
    echo "Durée totale du script : ${DURATION} ms"
    exit 1
}


# Vérification du nombre d’arguments
if [ "$#" -lt 2 ]; then
    error_exit "il manque des arguments."
fi

if [ "$#" -gt 4 ]; then
    error_exit "Trop d’arguments fournis."
fi


# Arguments
DATA_FILE="$1"
COMMAND="$2"
SUBCOMMAND="$3"

if [[ "$COMMAND" != "histo" && "$COMMAND" != "leaks" ]]; then
    error_exit "Commande invalide : '$COMMAND'. Les commandes autorisées sont 'histo' ou 'leaks'."
fi

# Variables
TEMP_FILE="temp.csv"
EXEC1=leaks_program
EXEC2=histo_program

# Vérification fichier de données
if [ ! -f "$DATA_FILE" ]; then
    error_exit "Fichier de données introuvable : $DATA_FILE"
fi


# Compile les .c
if [ ! -f "$EXEC1" ] || [ ! -f "$EXEC2" ]; then
    echo "Compilation des programmes C..."
    make
    if [ $? -ne 0 ]; then
        echo "Erreur lors de la compilation."
        exit 3
    fi
fi




# Traitement des commandes

case "$COMMAND" in

    histo)
        if [ "$#" -ne 3 ]; then
            error_exit "La commande histo peut utiliser max ou src ou real ."
        fi

        if [[ "$SUBCOMMAND" != "max" && "$SUBCOMMAND" != "src" && "$SUBCOMMAND" != "real" ]]; then
            error_exit "Option histo invalide : $SUBCOMMAND"
        fi
        echo "DEBUG : SUBCOMMAND='$SUBCOMMAND'"
        if [[ "$SUBCOMMAND" = "max" ]]; then
            grep -E '^-;[^-;]+;-;[^-;]+;-$' "$DATA_FILE" | cut -d';' -f2,4 | ./histo_program "$SUBCOMMAND"
            echo "Les usines ont été enregistrées dans lst_usine.txt"
            sort -t';' -k2,2n lst_usine.txt
            cat lst_usine.txt
            head -n 50 lst_usine.txt > extrait50.txt
            tail -n 10 lst_usine.txt > extrait10.txt 

            awk -F';' '{ gsub(/[^0-9]/,"",$2); if ($2!="") print $1 ";" $2 }' extrait10.txt > donnee_histo.txt
            awk -F';' '{ gsub(/[^0-9]/,"",$2); if ($2!="") print $1 ";" $2 }' extrait50.txt > donnee_histo2.txt

            cat donnee_histo.txt

            ./creation_histo.sh donnee_histo.txt donnee_histo2.txt "$SUBCOMMAND"

            graph="vol_max10"
            graph2="vol_max50"
            
            explorer.exe "graphe/${graph}.png"
            explorer.exe "graphe/${graph2}.png"
            
        fi

         if [[ "$SUBCOMMAND" = "src" ]]; then
            grep -E '^-;[^-;]+;[^-;]+;[^-;]+;[^-;]+$' "$DATA_FILE" | cut -d';' -f3,4 | ./histo_program "$SUBCOMMAND"
            echo "Les usines ont été enregistrées dans lst_usine.txt"
            sort -t';' -k2,2n lst_usine.txt
            cat lst_usine.txt
            

            head -n 10 lst_usine.txt | sort -t';' -k2,2n > extrait10.txt
            tail -n 50 lst_usine.txt | sort -t';' -k2,2n > extrait50.txt

            awk -F';' '{ gsub(/[^0-9]/,"",$2); if ($2!="") print $1 ";" $2 }' extrait10.txt > donnee_histo.txt
            awk -F';' '{ gsub(/[^0-9]/,"",$2); if ($2!="") print $1 ";" $2 }' extrait50.txt > donnee_histo2.txt


            ./creation_histo.sh donnee_histo.txt donnee_histo2.txt "$SUBCOMMAND"

            graph="vol_capte10"
            graph2="vol_capte50"

            explorer.exe "graphe/${graph}.png"
            explorer.exe "graphe/${graph2}.png"  
        fi
        
        if [[ "$SUBCOMMAND" = "real" ]]; then
            grep -E '^-;[^-;]+;[^-;]+;[^-;]+;[^-;]+$' "$DATA_FILE" | cut -d';' -f3,4,5 | ./histo_program "$SUBCOMMAND" 
            echo "Les usines ont été enregistrées dans lst_usine.txt"
            sort -t';' -k2,2n lst_usine.txt
            
            
            head -n 50 lst_usine.txt | sort -t';' -k2,2n > extrait50.txt
            tail -n 10 lst_usine.txt | sort -t';' -k2,2n > extrait10.txt

            awk -F';' '{ gsub(/[^0-9]/,"",$2); if ($2!="") print $1 ";" $2 }' extrait10.txt > donnee_histo.txt
            awk -F';' '{ gsub(/[^0-9]/,"",$2); if ($2!="") print $1 ";" $2 }' extrait50.txt > donnee_histo2.txt

            

            ./creation_histo.sh donnee_histo.txt donnee_histo2.txt "$SUBCOMMAND"

           graph="vol_traitement10"
           graph2="vol_traitement50"

           explorer.exe "graphe/${graph}.png"
	   explorer.exe "graphe/${graph2}.png" 
        fi

        ;;
   leaks)
    if [ "$#" -ne 3 ]; then
            error_exit "Commande Invalide : la commande n'admet que 2 arguments."
        fi
    grep -E "^[^;]+;[^;]+;${SUBCOMMAND};" "$DATA_FILE" > "$TEMP_FILE"
    grep -E "^[^;]+;${SUBCOMMAND};[^;]+;" "$DATA_FILE" >> "$TEMP_FILE"
    grep "^${SUBCOMMAND};" "$DATA_FILE" >> "$TEMP_FILE"

    cut -d';' -f2,3,4,5 "$TEMP_FILE" > "${TEMP_FILE}.tmp" \
        && mv "${TEMP_FILE}.tmp" "$TEMP_FILE"

    if [ ! -s "$TEMP_FILE" ]; then
        echo "Erreur : Aucune donnée trouvée pour l'usine '$SUBCOMMAND'."
        rm -f "$TEMP_FILE"
        exit 4
    fi

    echo "Lancement du calcul..."
    ./leaks_program "$TEMP_FILE" "$SUBCOMMAND" | tee -a historique_rendements.dat
    ;;
    
*)
esac

# Fin du script : affichage du temps

echo -n > lst_usine.txt
end_time=$(date +%s.%N)
DURATION=$(echo "($end_time - $start_time) * 1000" | bc)
echo "Traitement terminé avec succès."
echo "Durée totale du script : ${DURATION} ms"

# Nettoyage
rm -f "$TEMP_FILE"
rm -f donnee_histo.txt donnee_histo2.txt
make clean

exit 0
