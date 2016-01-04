#!/bin/bash

#cd /media/tushar/B19D-ABF3/latex1/6_3D_april_11/13_final/screenshot/1_simple_plume

#temp0=$(pwd)
#temp1=$(ls)
#temp2=($temp1)

#cd /media/tushar/B19D-ABF3/latex1/6_3D_april_11/13_final/screenshot/2_velocity_templates_exclude_source_1.25

#temp3=$(pwd)
#temp4=$(ls)
#temp5=($temp4)

##cd /media/tushar/B19D-ABF3/latex1/6_3D_april_11/13_final/screenshot/2_velocity_templates_exclude_source_1.25

##temp00=$(pwd)
##temp11=$(ls)
##temp22=($temp11)

##cd /media/tushar/B19D-ABF3/latex1/6_3D_april_11/11_pressure_template_rotation/2

##temp33=$(pwd)
##temp44=$(ls)
##temp55=($temp44)

#cd /media/tushar/TOSHIBA\ EXT/final_manta_render/2_velocity_templates_exclude_source_1.25


#for var1 in {0..249}
#do
# temp6=${temp2[$var1]}
# temp7="$temp0/$temp6"

# temp8=${temp5[$var1]}
# temp9="$temp3/$temp8"

## temp66=${temp22[$var1]}
## temp77="$temp00/$temp66"

## temp88=${temp55[$var1]}
## temp99="$temp33/$temp88"

# if [ $var1 -lt 10 ]
# then
#	montage "$temp7" "$temp9" -mode Concatenate  -tile 2x1  "00$var1.png"
##	montage "$temp7" "$temp9" "$temp77" "$temp99" -mode Concatenate  -tile 2x2  "frame_00$var1.png"
##	montage -label %f -frame 5 -background '#336699' -geometry +4+4 "$temp7" "$temp9" "frame_00$var1.jpg"
##	montage -label %f -frame 5 -background '#336699' -geometry +4+4 "$temp7" "$temp9" "$temp77" "00$var1.jpg"
##	montage -label %f -background '#336699' -geometry +4+4 "$temp7" "$temp9" "$temp77" "00$var1.jpg"
##	montage "$temp7" "$temp9" "$temp77" -mode Concatenate  -tile 3x1 "00$var1.png"
##	montage "$temp7" "$temp9" -mode Concatenate  -tile 2x1 "00$var1.png" 

# elif [ $var1 -lt 100 ]
# then
##	montage -label %f -frame 5 -background '#336699' -geometry +4+4 "$temp7" "$temp9" "frame_0$var1.jpg"
#	montage "$temp7" "$temp9" -mode Concatenate  -tile 2x1  "0$var1.png"
##	montage "$temp7" "$temp9" "$temp77" "$temp99" -mode Concatenate  -tile 2x2  "frame_0$var1.png"
##	montage -label %f -frame 5 -background '#336699' -geometry +4+4 "$temp7" "$temp9" "$temp77" "0$var1.jpg"
##	montage -label %f -background '#336699' -geometry +4+4 "$temp7" "$temp9" "$temp77" "0$var1.jpg"
##	montage "$temp7" "$temp9" "$temp77" -mode Concatenate  -tile 3x1 "0$var1.png"
##	montage "$temp7" "$temp9" -mode Concatenate  -tile 2x1 "0$var1.png" 

# else
## 	montage -label %f -frame 5 -background '#336699' -geometry +4+4 "$temp7" "$temp9" "frame_$var1.jpg"
#	montage "$temp7" "$temp9" -mode Concatenate  -tile 2x1  "$var1.png"
##	montage "$temp7" "$temp9" "$temp77" "$temp99" -mode Concatenate  -tile 2x2  "frame_$var1.png"
## 	montage -label %f -frame 5 -background '#336699' -geometry +4+4 "$temp7" "$temp9" "$temp77" "$var1.jpg"
## 	montage -label %f -background '#336699' -geometry +4+4 "$temp7" "$temp9" "$temp77" "$var1.jpg"
## 	montage "$temp7" "$temp9" "$temp77" -mode Concatenate  -tile 3x1 "$var1.png"
##	montage "$temp7" "$temp9" -mode Concatenate  -tile 2x1 "$var1.png" 
#  fi
#done

#ffmpeg -r 15 -qscale 2  -i ./%03d.png ./output.mp4

#cd /media/tushar/B19D-ABF3/latex1/6_3D_april_11/13_final/screenshot/1_simple_plume

#temp0=$(pwd)
#temp1=$(ls)
#temp2=($temp1)

#cd /media/tushar/B19D-ABF3/latex1/6_3D_april_11/13_final/screenshot/2_velocity_templates_exclude_source_1.50

#temp3=$(pwd)
#temp4=$(ls)
#temp5=($temp4)

#cd /media/tushar/TOSHIBA\ EXT/final_manta_render/2_velocity_templates_exclude_source_1.50


#for var1 in {0..249}
#do
# temp6=${temp2[$var1]}
# temp7="$temp0/$temp6"

# temp8=${temp5[$var1]}
# temp9="$temp3/$temp8"

# if [ $var1 -lt 10 ]
# then
#	montage "$temp7" "$temp9" -mode Concatenate  -tile 2x1  "00$var1.png"
# elif [ $var1 -lt 100 ]
# then
#	montage "$temp7" "$temp9" -mode Concatenate  -tile 2x1  "0$var1.png"
# else
#	montage "$temp7" "$temp9" -mode Concatenate  -tile 2x1  "$var1.png"
#  fi
#done

#ffmpeg -r 15 -qscale 2  -i ./%03d.png ./output.mp4


#cd /media/tushar/B19D-ABF3/latex1/6_3D_april_11/13_final/screenshot/1_simple_plume

#temp0=$(pwd)
#temp1=$(ls)
#temp2=($temp1)

#cd /media/tushar/B19D-ABF3/latex1/6_3D_april_11/13_final/screenshot/2_velocity_templates_include_source_1.00

#temp3=$(pwd)
#temp4=$(ls)
#temp5=($temp4)

#cd /media/tushar/TOSHIBA\ EXT/final_manta_render/2_velocity_templates_include_source_1.00

#for var1 in {0..249}
#do
# temp6=${temp2[$var1]}
# temp7="$temp0/$temp6"

# temp8=${temp5[$var1]}
# temp9="$temp3/$temp8"

# if [ $var1 -lt 10 ]
# then
#	montage "$temp7" "$temp9" -mode Concatenate  -tile 2x1  "00$var1.png"
# elif [ $var1 -lt 100 ]
# then
#	montage "$temp7" "$temp9" -mode Concatenate  -tile 2x1  "0$var1.png"
# else
#	montage "$temp7" "$temp9" -mode Concatenate  -tile 2x1  "$var1.png"
#  fi
#done

#ffmpeg -r 15 -qscale 2  -i ./%03d.png ./output.mp4

#cd /media/tushar/B19D-ABF3/latex1/6_3D_april_11/13_final/screenshot/1_simple_plume

#temp0=$(pwd)
#temp1=$(ls)
#temp2=($temp1)

#cd /media/tushar/B19D-ABF3/latex1/6_3D_april_11/13_final/screenshot/2_velocity_templates_include_source_1.25

#temp3=$(pwd)
#temp4=$(ls)
#temp5=($temp4)

#cd /media/tushar/TOSHIBA\ EXT/final_manta_render/2_velocity_templates_include_source_1.25

#for var1 in {0..249}
#do
# temp6=${temp2[$var1]}
# temp7="$temp0/$temp6"

# temp8=${temp5[$var1]}
# temp9="$temp3/$temp8"

# if [ $var1 -lt 10 ]
# then
#	montage "$temp7" "$temp9" -mode Concatenate  -tile 2x1  "00$var1.png"
# elif [ $var1 -lt 100 ]
# then
#	montage "$temp7" "$temp9" -mode Concatenate  -tile 2x1  "0$var1.png"
# else
#	montage "$temp7" "$temp9" -mode Concatenate  -tile 2x1  "$var1.png"
#  fi
#done

#ffmpeg -r 15 -qscale 2  -i ./%03d.png ./output.mp4

#cd /media/tushar/B19D-ABF3/latex1/6_3D_april_11/13_final/screenshot/1_simple_plume

#temp0=$(pwd)
#temp1=$(ls)
#temp2=($temp1)

#cd /media/tushar/B19D-ABF3/latex1/6_3D_april_11/13_final/screenshot/2_velocity_templates_include_source_1.50

#temp3=$(pwd)
#temp4=$(ls)
#temp5=($temp4)

#cd /media/tushar/TOSHIBA\ EXT/final_manta_render/2_velocity_templates_include_source_1.50

#for var1 in {0..249}
#do
# temp6=${temp2[$var1]}
# temp7="$temp0/$temp6"

# temp8=${temp5[$var1]}
# temp9="$temp3/$temp8"

# if [ $var1 -lt 10 ]
# then
#	montage "$temp7" "$temp9" -mode Concatenate  -tile 2x1  "00$var1.png"
# elif [ $var1 -lt 100 ]
# then
#	montage "$temp7" "$temp9" -mode Concatenate  -tile 2x1  "0$var1.png"
# else
#	montage "$temp7" "$temp9" -mode Concatenate  -tile 2x1  "$var1.png"
#  fi
#done

#ffmpeg -r 15 -qscale 2  -i ./%03d.png ./output.mp4

#cd /media/tushar/B19D-ABF3/latex1/6_3D_april_11/13_final/screenshot/1_simple_plume

#temp0=$(pwd)
#temp1=$(ls)
#temp2=($temp1)

#cd /media/tushar/B19D-ABF3/latex1/6_3D_april_11/13_final/screenshot/3_pressure_positive_1

#temp3=$(pwd)
#temp4=$(ls)
#temp5=($temp4)

#cd /media/tushar/TOSHIBA\ EXT/final_manta_render/3_pressure_positive_1

#for var1 in {0..249}
#do
# temp6=${temp2[$var1]}
# temp7="$temp0/$temp6"

# temp8=${temp5[$var1]}
# temp9="$temp3/$temp8"

# if [ $var1 -lt 10 ]
# then
#	montage "$temp7" "$temp9" -mode Concatenate  -tile 2x1  "00$var1.png"
# elif [ $var1 -lt 100 ]
# then
#	montage "$temp7" "$temp9" -mode Concatenate  -tile 2x1  "0$var1.png"
# else
#	montage "$temp7" "$temp9" -mode Concatenate  -tile 2x1  "$var1.png"
#  fi
#done

#ffmpeg -r 15 -qscale 2  -i ./%03d.png ./output.mp4

#cd /media/tushar/B19D-ABF3/latex1/6_3D_april_11/13_final/screenshot/1_simple_plume

#temp0=$(pwd)
#temp1=$(ls)
#temp2=($temp1)

#cd /media/tushar/B19D-ABF3/latex1/6_3D_april_11/13_final/screenshot/3_pressure_positive_2

#temp3=$(pwd)
#temp4=$(ls)
#temp5=($temp4)

#cd /media/tushar/TOSHIBA\ EXT/final_manta_render/3_pressure_positive_2

#for var1 in {0..249}
#do
# temp6=${temp2[$var1]}
# temp7="$temp0/$temp6"

# temp8=${temp5[$var1]}
# temp9="$temp3/$temp8"

# if [ $var1 -lt 10 ]
# then
#	montage "$temp7" "$temp9" -mode Concatenate  -tile 2x1  "00$var1.png"
# elif [ $var1 -lt 100 ]
# then
#	montage "$temp7" "$temp9" -mode Concatenate  -tile 2x1  "0$var1.png"
# else
#	montage "$temp7" "$temp9" -mode Concatenate  -tile 2x1  "$var1.png"
#  fi
#done

#ffmpeg -r 15 -qscale 2  -i ./%03d.png ./output.mp4

#cd /media/tushar/B19D-ABF3/latex1/6_3D_april_11/13_final/screenshot/1_simple_plume

#temp0=$(pwd)
#temp1=$(ls)
#temp2=($temp1)

#cd /media/tushar/B19D-ABF3/latex1/6_3D_april_11/13_final/screenshot/3_pressure_positive_3

#temp3=$(pwd)
#temp4=$(ls)
#temp5=($temp4)

#cd /media/tushar/TOSHIBA\ EXT/final_manta_render/3_pressure_positive_3

#for var1 in {0..249}
#do
# temp6=${temp2[$var1]}
# temp7="$temp0/$temp6"

# temp8=${temp5[$var1]}
# temp9="$temp3/$temp8"

# if [ $var1 -lt 10 ]
# then
#	montage "$temp7" "$temp9" -mode Concatenate  -tile 2x1  "00$var1.png"
# elif [ $var1 -lt 100 ]
# then
#	montage "$temp7" "$temp9" -mode Concatenate  -tile 2x1  "0$var1.png"
# else
#	montage "$temp7" "$temp9" -mode Concatenate  -tile 2x1  "$var1.png"
#  fi
#done

#ffmpeg -r 15 -qscale 2  -i ./%03d.png ./output.mp4

#cd /media/tushar/B19D-ABF3/latex1/6_3D_april_11/13_final/screenshot/1_simple_plume

#temp0=$(pwd)
#temp1=$(ls)
#temp2=($temp1)

#cd /media/tushar/B19D-ABF3/latex1/6_3D_april_11/13_final/screenshot/3_pressure_positive_4

#temp3=$(pwd)
#temp4=$(ls)
#temp5=($temp4)

#cd /media/tushar/TOSHIBA\ EXT/final_manta_render/3_pressure_positive_4

#for var1 in {0..249}
#do
# temp6=${temp2[$var1]}
# temp7="$temp0/$temp6"

# temp8=${temp5[$var1]}
# temp9="$temp3/$temp8"

# if [ $var1 -lt 10 ]
# then
#	montage "$temp7" "$temp9" -mode Concatenate  -tile 2x1  "00$var1.png"
# elif [ $var1 -lt 100 ]
# then
#	montage "$temp7" "$temp9" -mode Concatenate  -tile 2x1  "0$var1.png"
# else
#	montage "$temp7" "$temp9" -mode Concatenate  -tile 2x1  "$var1.png"
#  fi
#done

#ffmpeg -r 15 -qscale 2  -i ./%03d.png ./output.mp4

#cd /media/tushar/B19D-ABF3/latex1/6_3D_april_11/13_final/screenshot/1_simple_plume

#temp0=$(pwd)
#temp1=$(ls)
#temp2=($temp1)

#cd /media/tushar/B19D-ABF3/latex1/6_3D_april_11/13_final/screenshot/4_pressure_negative_1

#temp3=$(pwd)
#temp4=$(ls)
#temp5=($temp4)

#cd /media/tushar/TOSHIBA\ EXT/final_manta_render/4_pressure_negative_1

#for var1 in {0..249}
#do
# temp6=${temp2[$var1]}
# temp7="$temp0/$temp6"

# temp8=${temp5[$var1]}
# temp9="$temp3/$temp8"

# if [ $var1 -lt 10 ]
# then
#	montage "$temp7" "$temp9" -mode Concatenate  -tile 2x1  "00$var1.png"
# elif [ $var1 -lt 100 ]
# then
#	montage "$temp7" "$temp9" -mode Concatenate  -tile 2x1  "0$var1.png"
# else
#	montage "$temp7" "$temp9" -mode Concatenate  -tile 2x1  "$var1.png"
#  fi
#done

#ffmpeg -r 15 -qscale 2  -i ./%03d.png ./output.mp4

#cd /media/tushar/B19D-ABF3/latex1/6_3D_april_11/13_final/screenshot/1_simple_plume

#temp0=$(pwd)
#temp1=$(ls)
#temp2=($temp1)

#cd /media/tushar/B19D-ABF3/latex1/6_3D_april_11/13_final/screenshot/4_pressure_negative_2

#temp3=$(pwd)
#temp4=$(ls)
#temp5=($temp4)

#cd /media/tushar/TOSHIBA\ EXT/final_manta_render/4_pressure_negative_2

#for var1 in {0..249}
#do
# temp6=${temp2[$var1]}
# temp7="$temp0/$temp6"

# temp8=${temp5[$var1]}
# temp9="$temp3/$temp8"

# if [ $var1 -lt 10 ]
# then
#	montage "$temp7" "$temp9" -mode Concatenate  -tile 2x1  "00$var1.png"
# elif [ $var1 -lt 100 ]
# then
#	montage "$temp7" "$temp9" -mode Concatenate  -tile 2x1  "0$var1.png"
# else
#	montage "$temp7" "$temp9" -mode Concatenate  -tile 2x1  "$var1.png"
#  fi
#done

#ffmpeg -r 15 -qscale 2  -i ./%03d.png ./output.mp4

#cd /media/tushar/B19D-ABF3/latex1/6_3D_april_11/13_final/screenshot/1_simple_plume

#temp0=$(pwd)
#temp1=$(ls)
#temp2=($temp1)

#cd /media/tushar/B19D-ABF3/latex1/6_3D_april_11/13_final/screenshot/4_pressure_negative_3

#temp3=$(pwd)
#temp4=$(ls)
#temp5=($temp4)

#cd /media/tushar/TOSHIBA\ EXT/final_manta_render/4_pressure_negative_3

#for var1 in {0..249}
#do
# temp6=${temp2[$var1]}
# temp7="$temp0/$temp6"

# temp8=${temp5[$var1]}
# temp9="$temp3/$temp8"

# if [ $var1 -lt 10 ]
# then
#	montage "$temp7" "$temp9" -mode Concatenate  -tile 2x1  "00$var1.png"
# elif [ $var1 -lt 100 ]
# then
#	montage "$temp7" "$temp9" -mode Concatenate  -tile 2x1  "0$var1.png"
# else
#	montage "$temp7" "$temp9" -mode Concatenate  -tile 2x1  "$var1.png"
#  fi
#done

#ffmpeg -r 15 -qscale 2  -i ./%03d.png ./output.mp4

#cd /media/tushar/B19D-ABF3/latex1/6_3D_april_11/13_final/screenshot/1_simple_plume

#temp0=$(pwd)
#temp1=$(ls)
#temp2=($temp1)

#cd /media/tushar/B19D-ABF3/latex1/6_3D_april_11/13_final/screenshot/4_pressure_negative_4

#temp3=$(pwd)
#temp4=$(ls)
#temp5=($temp4)

#cd /media/tushar/TOSHIBA\ EXT/final_manta_render/4_pressure_negative_4

#for var1 in {0..249}
#do
# temp6=${temp2[$var1]}
# temp7="$temp0/$temp6"

# temp8=${temp5[$var1]}
# temp9="$temp3/$temp8"

# if [ $var1 -lt 10 ]
# then
#	montage "$temp7" "$temp9" -mode Concatenate  -tile 2x1  "00$var1.png"
# elif [ $var1 -lt 100 ]
# then
#	montage "$temp7" "$temp9" -mode Concatenate  -tile 2x1  "0$var1.png"
# else
#	montage "$temp7" "$temp9" -mode Concatenate  -tile 2x1  "$var1.png"
#  fi
#done

#ffmpeg -r 15 -qscale 2  -i ./%03d.png ./output.mp4

#cd /media/tushar/B19D-ABF3/latex1/6_3D_april_11/13_final/screenshot/1_simple_plume

#temp0=$(pwd)
#temp1=$(ls)
#temp2=($temp1)

#cd /media/tushar/B19D-ABF3/latex1/6_3D_april_11/13_final/screenshot/5_pressure_positive_negative_1

#temp3=$(pwd)
#temp4=$(ls)
#temp5=($temp4)

#cd /media/tushar/TOSHIBA\ EXT/final_manta_render/5_pressure_positive_negative_1

#for var1 in {0..249}
#do
# temp6=${temp2[$var1]}
# temp7="$temp0/$temp6"

# temp8=${temp5[$var1]}
# temp9="$temp3/$temp8"

# if [ $var1 -lt 10 ]
# then
#	montage "$temp7" "$temp9" -mode Concatenate  -tile 2x1  "00$var1.png"
# elif [ $var1 -lt 100 ]
# then
#	montage "$temp7" "$temp9" -mode Concatenate  -tile 2x1  "0$var1.png"
# else
#	montage "$temp7" "$temp9" -mode Concatenate  -tile 2x1  "$var1.png"
#  fi
#done

#ffmpeg -r 15 -qscale 2  -i ./%03d.png ./output.mp4

#cd /media/tushar/B19D-ABF3/latex1/6_3D_april_11/13_final/screenshot/1_simple_plume

#temp0=$(pwd)
#temp1=$(ls)
#temp2=($temp1)

#cd /media/tushar/B19D-ABF3/latex1/6_3D_april_11/13_final/screenshot/5_pressure_positive_negative_2

#temp3=$(pwd)
#temp4=$(ls)
#temp5=($temp4)

#cd /media/tushar/TOSHIBA\ EXT/final_manta_render/5_pressure_positive_negative_2

#for var1 in {0..249}
#do
# temp6=${temp2[$var1]}
# temp7="$temp0/$temp6"

# temp8=${temp5[$var1]}
# temp9="$temp3/$temp8"

# if [ $var1 -lt 10 ]
# then
#	montage "$temp7" "$temp9" -mode Concatenate  -tile 2x1  "00$var1.png"
# elif [ $var1 -lt 100 ]
# then
#	montage "$temp7" "$temp9" -mode Concatenate  -tile 2x1  "0$var1.png"
# else
#	montage "$temp7" "$temp9" -mode Concatenate  -tile 2x1  "$var1.png"
#  fi
#done

#ffmpeg -r 15 -qscale 2  -i ./%03d.png ./output.mp4

#cd /media/tushar/B19D-ABF3/latex1/6_3D_april_11/13_final/screenshot/1_simple_plume

#temp0=$(pwd)
#temp1=$(ls)
#temp2=($temp1)

#cd /media/tushar/B19D-ABF3/latex1/6_3D_april_11/13_final/screenshot/5_pressure_positive_negative_3

#temp3=$(pwd)
#temp4=$(ls)
#temp5=($temp4)

#cd /media/tushar/TOSHIBA\ EXT/final_manta_render/5_pressure_positive_negative_3

#for var1 in {0..249}
#do
# temp6=${temp2[$var1]}
# temp7="$temp0/$temp6"

# temp8=${temp5[$var1]}
# temp9="$temp3/$temp8"

# if [ $var1 -lt 10 ]
# then
#	montage "$temp7" "$temp9" -mode Concatenate  -tile 2x1  "00$var1.png"
# elif [ $var1 -lt 100 ]
# then
#	montage "$temp7" "$temp9" -mode Concatenate  -tile 2x1  "0$var1.png"
# else
#	montage "$temp7" "$temp9" -mode Concatenate  -tile 2x1  "$var1.png"
#  fi
#done

#ffmpeg -r 15 -qscale 2  -i ./%03d.png ./output.mp4

#cd /media/tushar/B19D-ABF3/latex1/6_3D_april_11/13_final/screenshot/1_simple_plume

#temp0=$(pwd)
#temp1=$(ls)
#temp2=($temp1)

#cd /media/tushar/B19D-ABF3/latex1/6_3D_april_11/13_final/screenshot/5_pressure_positive_negative_4

#temp3=$(pwd)
#temp4=$(ls)
#temp5=($temp4)

#cd /media/tushar/TOSHIBA\ EXT/final_manta_render/5_pressure_positive_negative_4

#for var1 in {0..249}
#do
# temp6=${temp2[$var1]}
# temp7="$temp0/$temp6"

# temp8=${temp5[$var1]}
# temp9="$temp3/$temp8"

# if [ $var1 -lt 10 ]
# then
#	montage "$temp7" "$temp9" -mode Concatenate  -tile 2x1  "00$var1.png"
# elif [ $var1 -lt 100 ]
# then
#	montage "$temp7" "$temp9" -mode Concatenate  -tile 2x1  "0$var1.png"
# else
#	montage "$temp7" "$temp9" -mode Concatenate  -tile 2x1  "$var1.png"
#  fi
#done

#ffmpeg -r 15 -qscale 2  -i ./%03d.png ./output.mp4

#cd /media/tushar/B19D-ABF3/latex1/6_3D_april_11/13_final/screenshot/1_simple_plume

#temp0=$(pwd)
#temp1=$(ls)
#temp2=($temp1)

#cd /media/tushar/TOSHIBA\ EXT/5_pressure_positive_negative_5/screenshot

#temp3=$(pwd)
#temp4=$(ls)
#temp5=($temp4)

#cd /media/tushar/TOSHIBA\ EXT/final_manta_combine/5_pressure_positive_negative_5

#for var1 in {0..249}
#do
# temp6=${temp2[$var1]}
# temp7="$temp0/$temp6"

# temp8=${temp5[$var1]}
# temp9="$temp3/$temp8"

# if [ $var1 -lt 10 ]
# then
#	montage "$temp7" "$temp9" -mode Concatenate  -tile 2x1  "00$var1.png"
# elif [ $var1 -lt 100 ]
# then
#	montage "$temp7" "$temp9" -mode Concatenate  -tile 2x1  "0$var1.png"
# else
#	montage "$temp7" "$temp9" -mode Concatenate  -tile 2x1  "$var1.png"
#  fi
#done

#ffmpeg -r 15 -qscale 2  -i ./%03d.png ./output.avi

#cd /media/tushar/B19D-ABF3/latex1/6_3D_april_11/13_final/screenshot/1_simple_plume

#temp0=$(pwd)
#temp1=$(ls)
#temp2=($temp1)

#cd /media/tushar/TOSHIBA\ EXT/3_pressure_positive_5/screenshot


#temp3=$(pwd)
#temp4=$(ls)
#temp5=($temp4)

#cd /media/tushar/TOSHIBA\ EXT/final_manta_combine/3_pressure_positive_5

#for var1 in {0..249}
#do
# temp6=${temp2[$var1]}
# temp7="$temp0/$temp6"

# temp8=${temp5[$var1]}
# temp9="$temp3/$temp8"

# if [ $var1 -lt 10 ]
# then
#	montage "$temp7" "$temp9" -mode Concatenate  -tile 2x1  "00$var1.png"
# elif [ $var1 -lt 100 ]
# then
#	montage "$temp7" "$temp9" -mode Concatenate  -tile 2x1  "0$var1.png"
# else
#	montage "$temp7" "$temp9" -mode Concatenate  -tile 2x1  "$var1.png"
#  fi
#done

#ffmpeg -r 15 -qscale 2  -i ./%03d.png ./output.avi

cd /media/tushar/TOSHIBA\ EXT/mitsuba_render_1/png/1_simple_plume

temp0=$(pwd)
temp1=$(ls)
temp2=($temp1)

cd /media/tushar/TOSHIBA\ EXT/mitsuba_render_1/png/2_velocity_templates_exclude_source_1.25


temp3=$(pwd)
temp4=$(ls)
temp5=($temp4)

cd /media/tushar/TOSHIBA\ EXT/mitsuba_render_1/montage

for var1 in {0..249}
do
 temp6=${temp2[$var1]}
 temp7="$temp0/$temp6"

 temp8=${temp5[$var1]}
 temp9="$temp3/$temp8"

 if [ $var1 -lt 10 ]
 then
	montage "$temp7" "$temp9" -mode Concatenate  -tile 2x1  "00$var1.png"
 elif [ $var1 -lt 100 ]
 then
	montage "$temp7" "$temp9" -mode Concatenate  -tile 2x1  "0$var1.png"
 else
	montage "$temp7" "$temp9" -mode Concatenate  -tile 2x1  "$var1.png"
  fi
done

ffmpeg -r 10 -qscale 2  -i ./%03d.png ./output.mp4

