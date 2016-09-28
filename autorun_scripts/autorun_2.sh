#!/bin/bash
# author: Tianyi Gu

k="5"
i="1"
while (($i <= 10))
do
	((k=i*5))	
	echo "$k"
	./digitRecRT 8 "$k"| grep "finalmr" >> ./data/dt_cata
	((i++))
done

