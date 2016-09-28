# Gnuplot script file for plotting data 
# This file is called   plot.p
set terminal postscript eps "enhanced"
set output "dtree_cata.eps"
set key bottom Left reverse
set style line 1 lc rgb '#0000ff' pt 6 ps 1.5 lt 1 lw 5
set style line 2 lc rgb '#8b1a0e' pt 1 ps 1.5 lt 1 lw 5
set xrange [0:25]
set yrange [0:0.55]
set y2range [0:60000]
set ytics nomirror
set y2tics
set xlabel "catalog number"
set ylabel "misclassification rate"
set y2label "tree nodes"
plot '../data/dt_cata' u 8:2 w lp ls 1 t 'misclassification rate' axes x1y1, \
'../data/dt_cata' u 8:6 w lp ls 2 t 'tree nodes' axes x1y2
set output


