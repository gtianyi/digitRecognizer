# Gnuplot script file for plotting data 
# This file is called   plot.p
set terminal postscript eps "enhanced"
set output "dtreertsamples.eps"
set key Left reverse
set style line 1 lc rgb '#0000ff' pt 6 ps 1.5 lt 1 lw 5
set xrange [1000:30000]
set yrange [0:0.12]
set xlabel "samples number"
set ylabel "misclassification rate"
plot '../data/rf_samples' u 6:2 w lp ls 1 t 'misclassification rate'
set output


