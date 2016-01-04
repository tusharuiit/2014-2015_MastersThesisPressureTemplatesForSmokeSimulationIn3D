#!/bin/bash

cd /home/tushar/manta_mikey_1/manta/build/pictures/january_27/templates/1

temp0=$(pwd)
temp1=$(ls)
temp2=($temp1)

cd /home/tushar/manta_mikey_1/manta/build/pictures/january_27/templates/2

temp3=$(pwd)
temp4=$(ls)
temp5=($temp4)

cd /home/tushar/manta_mikey_1/manta/build/pictures/january_27/templates/3


for var1 in {0..299}
do
 temp6=${temp2[$var1]}
 temp7="$temp0/$temp6"
 temp8=${temp5[$var1]}
 temp9="$temp3/$temp8"

 if [ $var1 -lt 10 ]
 then
   montage -label %f -frame 5 -background '#336699' -geometry +4+4 "$temp7" "$temp9" "frame_00$var1.jpg"
 elif [ $var1 -lt 100 ]
 then
   montage -label %f -frame 5 -background '#336699' -geometry +4+4 "$temp7" "$temp9" "frame_0$var1.jpg"
 else
   montage -label %f -frame 5 -background '#336699' -geometry +4+4 "$temp7" "$temp9" "frame_$var1.jpg"
 fi
done

#echo ${temp1[2]}
