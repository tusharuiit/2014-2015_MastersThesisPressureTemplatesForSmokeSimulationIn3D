cd /home/tushar/manta_mikey_1/manta/build/pictures/march_11/images/1

temp0=$(pwd)
temp1=$(ls)
temp2=($temp1)

cd /home/tushar/manta_mikey_1/manta/build/pictures/march_11/images/8

temp3=$(pwd)
temp4=$(ls)
temp5=($temp4)

cd /home/tushar/manta_mikey_1/manta/build/pictures/march_11/images/9

temp00=$(pwd)
temp11=$(ls)
temp22=($temp11)

cd /home/tushar/manta_mikey_1/manta/build/pictures/march_11/images/10

temp33=$(pwd)
temp44=$(ls)
temp55=($temp44)

cd /home/tushar/manta_mikey_1/manta/build/pictures/march_11/montages/2


for var1 in {0..399}
do
 temp6=${temp2[$var1]}
 temp7="$temp0/$temp6"
 temp8=${temp5[$var1]}
 temp9="$temp3/$temp8"
 temp66=${temp22[$var1]}
 temp77="$temp00/$temp66"
 temp88=${temp55[$var1]}
 temp99="$temp33/$temp88"

 if [ $var1 -lt 10 ]
 then
	montage "$temp7" "$temp9" "$temp77" "$temp99" -mode Concatenate  -tile 2x2  "00$var1.png"
#	montage "$temp7" "$temp9" -mode Concatenate  -tile 2x2  "frame_00$var1.png"
#	montage "$temp7" "$temp9" "$temp77" "$temp99" -mode Concatenate  -tile 2x2  "frame_00$var1.png"
#	montage -label %f -frame 5 -background '#336699' -geometry +4+4 "$temp7" "$temp9" "00$var1.png"
#	montage "$temp7" "$temp9" "$temp77" -mode Concatenate  -tile 2x2  "00$var1.png"
 elif [ $var1 -lt 100 ]
 then
#	montage -label %f -frame 5 -background '#336699' -geometry +4+4 "$temp7" "$temp9" "0$var1.png"
	montage "$temp7" "$temp9" "$temp77" "$temp99" -mode Concatenate  -tile 2x2  "0$var1.png"
#	montage "$temp7" "$temp9" "$temp77" "$temp99" -mode Concatenate  -tile 2x2  "frame_0$var1.png"
#	montage "$temp7" "$temp9" -mode Concatenate  -tile 2x2  "frame_0$var1.png"
#	montage "$temp7" "$temp9" "$temp77" -mode Concatenate  -tile 2x2  "0$var1.png"
 else
# 	montage -label %f -frame 5 -background '#336699' -geometry +4+4 "$temp7" "$temp9" "$var1.png"
	montage "$temp7" "$temp9" "$temp77" "$temp99" -mode Concatenate  -tile 2x2  "$var1.png"
#	montage "$temp7" "$temp9" "$temp77" "$temp99" -mode Concatenate  -tile 2x2  "frame_$var1.png"
#	montage "$temp7" "$temp9" -mode Concatenate  -tile 2x2  "frame_$var1.png"
#	montage "$temp7" "$temp9" "$temp77" -mode Concatenate  -tile 2x2  "$var1.png"
 fi
done

#echo ${temp1[2]}
