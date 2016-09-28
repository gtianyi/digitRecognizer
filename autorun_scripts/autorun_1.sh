#!/bin/bash
# author: Tianyi Gu

./digitRecRT 0 | grep "finalmr" >> ./data/dt_pru
echo "0"
./digitRecRT 2 | grep "finalmr" >> ./data/dt_pru
echo "2"
./digitRecRT 4 | grep "finalmr" >> ./data/dt_pru
echo "4"
./digitRecRT 6 | grep "finalmr" >> ./data/dt_pru
echo "6"
k="8"
while (($k <= 10000))
do
	echo "$k"
	./digitRecRT "$k"| grep "finalmr" >> ./data/dt_pru
	((k=k*10))
done

