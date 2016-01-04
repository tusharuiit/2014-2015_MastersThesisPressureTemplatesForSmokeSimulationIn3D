#!/bin/bash

cd /home/tushar/manta_mikey_1/manta/build/pictures/december_4/1/4.0

temp0=$(pwd)
temp1=$(ls)
temp2=($temp1)

for var1 in {0..399}
do
 temp6=${temp2[$var1]}
 temp7="$temp0/$temp6"
 temp8=${temp5[$var1]}
 temp9="$temp3/$temp8"
# echo "Hi this has to be $temp7"

 if [ $var1 -lt 10 ]
 then
   montage -label %f -frame 5 -background '#336699' -geometry +4+4 "$temp7" "$temp9" "frame_00$var1.jpg"
 fi
done

#echo ${temp1[2]}
