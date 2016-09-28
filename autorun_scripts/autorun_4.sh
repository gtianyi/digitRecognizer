#!/bin/bash
# author: Tianyi Gu

k="5"
i="1"
while (($i <= 10))
do
	((k=i*1000))	
	echo "$k"
	./digitRecRF 500 3 "$k" 80 | grep "finalmr" >> ./data/rf_samples
	((i++))
done
