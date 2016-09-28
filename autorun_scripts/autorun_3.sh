#!/bin/bash
# author: Tianyi Gu

k="5"
i="2"
while (($i <= 10))
do
	((k=i*100))	
	echo "$k"
	if [ "$k" -ne "500" ]; then
	./digitRecRF "$k" 3 4000 27| grep "finalmr" >> ./data/rf_treeNum
	fi
	((i++))
done

i="1"
while (($i <= 10))
do
	((k=i*10))	
	echo "$k"
	./digitRecRF 100 3 4000 "$k"| grep "finalmr" >> ./data/rf_feature
	((i++))
done

i="1"
while (($i <= 6))
do
	((k=i*3))	
	echo "$k"
	./digitRecRF 100 "$k" 4000 27 | grep "finalmr" >> ./data/rf_cata
	((i++))
done

i="1"
while (($i <= 10))
do
	((k=i*1000))	
	echo "$k"
	./digitRecRF 100 3 "$k" 27 | grep "finalmr" >> ./data/rf_samples
	((i++))
done
