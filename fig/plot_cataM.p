# Gnuplot script file for plotting data 
# This file is called   plot.p
set terminal postscript eps "enhanced"
set output "dtree_cataMargin.eps"
set key Left reverse
set style line 1 lc rgb '#0000ff' pt 6 ps 1.5 lt 1 lw 5
set xrange [2:6]
set yrange [0.19:0.21]
set xlabel "Margin color index"
set ylabel "misclassification rate"
plot '../data/dt_cm' u 8:2 w lp ls 1 t 'misclassification rate'
set output


