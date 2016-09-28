# Gnuplot script file for plotting data 
# This file is called   plot.p
set terminal postscript eps "enhanced"
set output "comp.eps"
set key off
set style line 1 lc rgb "red"
set style line 2 lc rgb "blue"
set style line 3 lc rgb "green"
set boxwidth 0.5
set style fill solid
set ylabel "misclassification rate"
plot '../data/comp' every ::0::0 u 1:3:xtic(2) w boxes ls 1,\
'../data/comp' every ::1::1 u 1:3:xtic(2) w boxes ls 2,\
'../data/comp' every ::2::2 u 1:3:xtic(2) w boxes ls 3
set output


