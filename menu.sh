
#!/bin/bash


# Gestion du temps (millisecondes approx.)

START_TIME=$(date +%s)


# Fonctions utilitaires

error_exit() {
    echo "Erreur : $1" >&2
    END_TIME=$(date +%s)
    DURATION=$(( (END_TIME - START_TIME) * 1000 ))
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
OPTIONAL_ARG="$4"


# Vérification fichier de données

if [ ! -f "$DATA_FILE" ]; then
    error_exit "Fichier de données introuvable : $DATA_FILE"
fi


# Compile le code C
gcc creeravlfinale.c -o creeravlfinale
sed -i 's/\r$//' avl.c
gcc avl.c -o avl


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
            grep -E '^-;[^-;]+;-;[^-;]+;-$' "$DATA_FILE" | cut -d';' -f2,4 | ./avl.c "$SUBCOMMAND"
            echo "Les usines ont été enregistrées dans lst_usine.txt"
            cat lst_usine.txt
            head -n 10 lst_usine.txt > extrait10.txt
            tail -n 50 lst_usine.txt > extrait50.txt 

            cut -d';' -f2,3 extrait10.txt > donnee_histo.txt
            cut -d';' -f2,3 extrait50.txt > donnee_histo2.txt 

            cat donnee_histo.txt

            ./creerhisto.sh donnee_histo.txt donnee_histo2.txt "$SUBCOMMAND"

            explorer.exe histo10.png
            explorer.exe histo50.png
            
        fi

         if [[ "$SUBCOMMAND" = "src" ]]; then
            grep -E '^-;[^-;]+;[^-;]+;[^-;]+;[^-;]+$' "$DATA_FILE" | cut -d';' -f3,4 | ./avlbeta.c "$SUBCOMMAND"
            echo "Les usines ont été enregistrées dans lst_usine.txt"
            cat lst_usine.txt
            head -n 10 lst_usine.txt > extrait10.txt
            tail -n 50 lst_usine.txt > extrait50.txt 

            cut -d';' -f1,2 extrait10.txt > donnee_histo.txt
            cut -d';' -f1,2 extrait50.txt > donnee_histo2.txt 

            ./creerhisto.sh donnee_histo.txt donnee_histo2.txt "$SUBCOMMAND"

            explorer.exe histo10.png
            explorer.exe histo50.png
            

            
        fi
esac

# Fin du script : affichage du temps

END_TIME=$(date +%s)
DURATION=$(( (END_TIME - START_TIME) * 1000 ))
echo "Traitement terminé avec succès."
echo "Durée totale du script : ${DURATION} ms"

exit 0
