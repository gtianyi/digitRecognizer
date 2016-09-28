# Gnuplot script file for plotting data 
# This file is called   plot.p
set terminal postscript eps "enhanced"
set output "dtree1.eps"
set key Left reverse
set style line 1 lc rgb '#0000ff' pt 6 ps 1.5 lt 1 lw 5
set style line 2 lc rgb '#8b1a0e' pt 1 ps 1.5 lt 1 lw 5
set xrange [2:10]
set yrange [0.28:0.3]
set y2range [0:20000]
set ytics nomirror
set y2tics
set xlabel "pruning delta"
set ylabel "misclassification rate"
set y2label "tree nodes"
plot '../data/dt_pru' u 4:2 w lp ls 1 t 'misclassification rate' axes x1y1, \
'../data/dt_pru' u 4:6 w lp ls 2 t 'tree nodes' axes x1y2
set output


