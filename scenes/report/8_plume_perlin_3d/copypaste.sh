#!/bin/bash


cd /media/tushar/TOSHIBA\ EXT/5_pressure_positive_negative_5/screenshot

temp0=$(pwd)
temp1=$(ls)
temp2=($temp1)

for var1 in {40..240..40}
do
	temp6=${temp2[$var1]}
	temp7="$temp0/$temp6"

	cp "$temp7" "$temp7"
done


cd /media/tushar/TOSHIBA\ EXT/3_pressure_positive_5/screenshot

temp0=$(pwd)
temp1=$(ls)
temp2=($temp1)

for var1 in {0..249}
do
	temp6=${temp2[$var1]}
	temp7="$temp0/$temp6"

	convert -crop +373+98 "$temp7" "$temp7"
	convert -crop -373-98 "$temp7" "$temp7"
done
