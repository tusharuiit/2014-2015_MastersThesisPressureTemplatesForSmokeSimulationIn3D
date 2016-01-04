#!/bin/bash

cd /media/tushar/TOSHIBA\ EXT/Nils/today/4_perlin/1_circular_perlin/density

temp0=$(pwd)
temp1=$(ls)
temp2=($temp1)

for var1 in {0..399}
do
	temp6=${temp2[$var1]}
	temp7="$temp0/$temp6"

	convert -crop +190+0 "$temp7" "$temp7"
	convert -crop -190+0 "$temp7" "$temp7"
done
