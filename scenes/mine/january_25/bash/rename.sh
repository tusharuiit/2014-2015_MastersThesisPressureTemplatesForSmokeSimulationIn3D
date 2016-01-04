#!/bin/bash

cd /home/tushar/manta_mikey_1/manta/build/pictures/january_17/templates/1_temp

temp0=$(pwd)
temp1=$(ls)
temp2=($temp1)

#cd ../montage_3_full_gradient_elliptical

#temp3=${temp2[2]}

foo1='perlin'
foo3='.png'

#echo $foo4

for var1 in {5..99}
do
# temp6=${temp2[$var1]}
# temp7="$temp0/$temp6"
  foo2=$var1
  foo4=$foo1$foo2$foo3

# temp8=${temp5[$var1]}
# temp9="$temp3/$temp8"
# echo "Hi this has to be $temp7"
# echo "Hi this has to be $temp9"

# echo $temp7

if [ $var1 -lt 10 ]
then
#   montage -label %f -frame 5 -background '#336699' -geometry +4+4 "$temp7" "$temp9" "frame_00$var1.png"
  temp='00'
  foo5=$temp$var1
  echo $foo5
  mv $foo4 $foo1$foo5$foo3
elif [ $var1 -lt 100 ]
then
  temp='0'
  foo6=$temp$var1
  mv $foo4 $foo1$foo6$foo3
#   montage -label %f -frame 5 -background '#336699' -geometry +4+4 "$temp7" "$temp9" "frame_0$var1.png"
# else
#  foo6=$var1
#  mv $foo4 $foo1$foo3
#   montage -label %f -frame 5 -background '#336699' -geometry +4+4 "$temp7" "$temp9" "frame_$var1.png"
fi
done

#echo ${temp1[2]}
