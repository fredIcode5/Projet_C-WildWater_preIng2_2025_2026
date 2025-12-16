#!/bin/bash

# Vérifie que gnuplot est installé
if ! command -v gnuplot &> /dev/null; then
    echo "Gnuplot n'est pas installé."
    exit
fi


#10 plus petits/10 plus grands 

DATA_FILE="$1"
DATA_FILE2="$2"
option="$3"

cat "$DATA_FILE"
cat "$DATA_FILE2"

OUTPUT1="histo50.png"
OUTPUT2="histo10.png"


#arguments option 

if [[ "$option" == "max" ]]; then
    title="par capacité max."
elif [[ "$option" == "src" ]]; then
    title=" par volume capté."
elif [[ "$option" == "real" ]]; then
    title="par perte réelle."
fi


# Génération de l'histogramme avec Gnuplot ( les 50 plus petites)
gnuplot <<- EOF
    set terminal pngcairo size 1200,700 enhanced font 'Verdana,12'
    set output "$OUTPUT1"

    set datafile separator ";"         # Indique que le séparateur est le point-virgule

    set title "10 plus grandes usines - ${title}"
    set xlabel "Identifiants"
    set ylabel "Volume (millions de m³)"

    set style data histograms
    set style fill solid 1.0 border -1
    set boxwidth 0.8
    set grid y
    set xtics rotate by -90
    # Tracer l'histogramme et convertir les volumes en millions
    plot "$DATA_FILE" using 2:xtic(1) linecolor rgb "skyblue" title "Volume"

    
EOF

# Génération de l'histogramme avec Gnuplot ( les 10 plus grands)
gnuplot <<- EOF
    set terminal pngcairo size 1200,700 enhanced font 'Verdana,12'
    set output "$OUTPUT2"

    set datafile separator ";"         # Indique que le séparateur est le point-virgule

    set title "50 plus petits usines - ${title}"
    set xlabel "Identifiants"
    set ylabel "Volume (millions de m³)"

    set style data histograms
    set style fill solid 1.0 border -1
    set boxwidth 0.8
    set grid y
    set xtics rotate by -90
    # Tracer l'histogramme et convertir les volumes en millions
    plot "$DATA_FILE2" using 2:xtic(1) linecolor rgb "skyblue" title "Volume"

    
EOF



echo "Histogramme généré : $OUTPUT1"
echo "Histogramme généré : $OUTPUT2"

