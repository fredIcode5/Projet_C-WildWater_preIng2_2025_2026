# Terminal et sortie
set terminal pngcairo size 800,600
set output "histogramme.png"

# Style histogramme
set style data histograms
set style fill solid 1.0 border -1
set boxwidth 0.7

# Titres
set title "Exemple d'histogramme"
set xlabel "Catégories"
set ylabel "Valeurs"

# Tracé
plot "donnees.txt" using 2:xtic(1) title "Mesures" lc rgb "blue"
