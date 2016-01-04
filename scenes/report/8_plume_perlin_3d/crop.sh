#!/bin/bash


cd /home/tushar/Downloads/thesis_latex_templates/cse_texmaker/figures/21_simple_plume_2d

temp0=$(pwd)
temp1=$(ls)
temp2=($temp1)

for var1 in {0..6}
do
	temp6=${temp2[$var1]}
	temp7="$temp0/$temp6"

	convert -crop +190+0 "$temp7" "$temp7"
	convert -crop -190-0 "$temp7" "$temp7"
done

#cd /home/tushar/Downloads/thesis_latex_templates/cse_texmaker/figures/26_gaussian_translation/horizontal/velocity

#temp0=$(pwd)
#temp1=$(ls)
#temp2=($temp1)

#for var1 in {0..5}
#do
#	temp6=${temp2[$var1]}
#	temp7="$temp0/$temp6"

#	convert -crop +190+0 "$temp7" "$temp7"
#	convert -crop -190-0 "$temp7" "$temp7"
#done

#cd /home/tushar/Downloads/thesis_latex_templates/cse_texmaker/figures/26_gaussian_translation/vertical/density

#temp0=$(pwd)
#temp1=$(ls)
#temp2=($temp1)

#for var1 in {0..5}
#do
#	temp6=${temp2[$var1]}
#	temp7="$temp0/$temp6"

#	convert -crop +190+0 "$temp7" "$temp7"
#	convert -crop -190-0 "$temp7" "$temp7"
#done

#cd /home/tushar/Downloads/thesis_latex_templates/cse_texmaker/figures/26_gaussian_translation/vertical/velocity

#temp0=$(pwd)
#temp1=$(ls)
#temp2=($temp1)

#for var1 in {0..5}
#do
#	temp6=${temp2[$var1]}
#	temp7="$temp0/$temp6"

#	convert -crop +190+0 "$temp7" "$temp7"
#	convert -crop -190-0 "$temp7" "$temp7"
#done


#cd /media/tushar/TOSHIBA\ EXT/5_pressure_positive_negative_5/screenshot

#temp0=$(pwd)
#temp1=$(ls)
#temp2=($temp1)

#for var1 in {0..249}
#do
#	temp6=${temp2[$var1]}
#	temp7="$temp0/$temp6"

#	convert -crop +373+98 "$temp7" "$temp7"
#	convert -crop -373-98 "$temp7" "$temp7"
#done


#cd /media/tushar/TOSHIBA\ EXT/3_pressure_positive_5/screenshot

#temp0=$(pwd)
#temp1=$(ls)
#temp2=($temp1)

#for var1 in {0..249}
#do
#	temp6=${temp2[$var1]}
#	temp7="$temp0/$temp6"

#	convert -crop +373+98 "$temp7" "$temp7"
#	convert -crop -373-98 "$temp7" "$temp7"
#done


#cd /media/tushar/B19D-ABF3/latex1/6_3D_april_11/13_final/screenshot/2_velocity_templates_exclude_source_1.25

#temp0=$(pwd)
#temp1=$(ls)
#temp2=($temp1)

#for var1 in {230..249}
#do
#	temp6=${temp2[$var1]}
#	temp7="$temp0/$temp6"

#	convert -crop +373+98 "$temp7" "$temp7"
#	convert -crop -373-98 "$temp7" "$temp7"
#done

#cd /media/tushar/B19D-ABF3/latex1/6_3D_april_11/13_final/screenshot/2_velocity_templates_exclude_source_1.50

#temp0=$(pwd)
#temp1=$(ls)
#temp2=($temp1)

#for var1 in {91..249}
#do
#	temp6=${temp2[$var1]}
#	temp7="$temp0/$temp6"

#	convert -crop +373+98 "$temp7" "$temp7"
#	convert -crop -373-98 "$temp7" "$temp7"
#done

#cd /media/tushar/B19D-ABF3/latex1/6_3D_april_11/13_final/screenshot/2_velocity_templates_include_source_1.00

#temp0=$(pwd)
#temp1=$(ls)
#temp2=($temp1)

#for var1 in {0..249}
#do
#	temp6=${temp2[$var1]}
#	temp7="$temp0/$temp6"

#	convert -crop +373+98 "$temp7" "$temp7"
#	convert -crop -373-98 "$temp7" "$temp7"
#done

#cd /media/tushar/B19D-ABF3/latex1/6_3D_april_11/13_final/screenshot/2_velocity_templates_include_source_1.25

#temp0=$(pwd)
#temp1=$(ls)
#temp2=($temp1)

#for var1 in {0..249}
#do
#	temp6=${temp2[$var1]}
#	temp7="$temp0/$temp6"

#	convert -crop +373+98 "$temp7" "$temp7"
#	convert -crop -373-98 "$temp7" "$temp7"
#done

#cd /media/tushar/B19D-ABF3/latex1/6_3D_april_11/13_final/screenshot/2_velocity_templates_include_source_1.50

#temp0=$(pwd)
#temp1=$(ls)
#temp2=($temp1)

#for var1 in {0..249}
#do
#	temp6=${temp2[$var1]}
#	temp7="$temp0/$temp6"

#	convert -crop +373+98 "$temp7" "$temp7"
#	convert -crop -373-98 "$temp7" "$temp7"
#done

#cd /media/tushar/B19D-ABF3/latex1/6_3D_april_11/13_final/screenshot/3_pressure_positive_1

#temp0=$(pwd)
#temp1=$(ls)
#temp2=($temp1)

#for var1 in {91..249}
#do
#	temp6=${temp2[$var1]}
#	temp7="$temp0/$temp6"

#	convert -crop +373+98 "$temp7" "$temp7"
#	convert -crop -373-98 "$temp7" "$temp7"
#done

#cd /media/tushar/B19D-ABF3/latex1/6_3D_april_11/13_final/screenshot/3_pressure_positive_2

#temp0=$(pwd)
#temp1=$(ls)
#temp2=($temp1)

#for var1 in {91..249}
#do
#	temp6=${temp2[$var1]}
#	temp7="$temp0/$temp6"

#	convert -crop +373+98 "$temp7" "$temp7"
#	convert -crop -373-98 "$temp7" "$temp7"
#done

#cd /media/tushar/B19D-ABF3/latex1/6_3D_april_11/13_final/screenshot/3_pressure_positive_3

#temp0=$(pwd)
#temp1=$(ls)
#temp2=($temp1)

#for var1 in {91..249}
#do
#	temp6=${temp2[$var1]}
#	temp7="$temp0/$temp6"

#	convert -crop +373+98 "$temp7" "$temp7"
#	convert -crop -373-98 "$temp7" "$temp7"
#done

#cd /media/tushar/B19D-ABF3/latex1/6_3D_april_11/13_final/screenshot/3_pressure_positive_4

#temp0=$(pwd)
#temp1=$(ls)
#temp2=($temp1)

#for var1 in {0..249}
#do
#	temp6=${temp2[$var1]}
#	temp7="$temp0/$temp6"

#	convert -crop +373+98 "$temp7" "$temp7"
#	convert -crop -373-98 "$temp7" "$temp7"
#done

#cd /media/tushar/B19D-ABF3/latex1/6_3D_april_11/13_final/screenshot/4_pressure_negative_1

#temp0=$(pwd)
#temp1=$(ls)
#temp2=($temp1)

#for var1 in {0..249}
#do
#	temp6=${temp2[$var1]}
#	temp7="$temp0/$temp6"

#	convert -crop +373+98 "$temp7" "$temp7"
#	convert -crop -373-98 "$temp7" "$temp7"
#done

#cd /media/tushar/B19D-ABF3/latex1/6_3D_april_11/13_final/screenshot/4_pressure_negative_2

#temp0=$(pwd)
#temp1=$(ls)
#temp2=($temp1)

#for var1 in {0..249}
#do
#	temp6=${temp2[$var1]}
#	temp7="$temp0/$temp6"

#	convert -crop +373+98 "$temp7" "$temp7"
#	convert -crop -373-98 "$temp7" "$temp7"
#done

#cd /media/tushar/B19D-ABF3/latex1/6_3D_april_11/13_final/screenshot/4_pressure_negative_3

#temp0=$(pwd)
#temp1=$(ls)
#temp2=($temp1)

#for var1 in {0..249}
#do
#	temp6=${temp2[$var1]}
#	temp7="$temp0/$temp6"

#	convert -crop +373+98 "$temp7" "$temp7"
#	convert -crop -373-98 "$temp7" "$temp7"
#done

#cd /media/tushar/B19D-ABF3/latex1/6_3D_april_11/13_final/screenshot/4_pressure_negative_4

#temp0=$(pwd)
#temp1=$(ls)
#temp2=($temp1)

#for var1 in {0..249}
#do
#	temp6=${temp2[$var1]}
#	temp7="$temp0/$temp6"

#	convert -crop +373+98 "$temp7" "$temp7"
#	convert -crop -373-98 "$temp7" "$temp7"
#done

#cd /media/tushar/B19D-ABF3/latex1/6_3D_april_11/13_final/screenshot/5_pressure_positive_negative_1

#temp0=$(pwd)
#temp1=$(ls)
#temp2=($temp1)

#for var1 in {0..249}
#do
#	temp6=${temp2[$var1]}
#	temp7="$temp0/$temp6"

#	convert -crop +373+98 "$temp7" "$temp7"
#	convert -crop -373-98 "$temp7" "$temp7"
#done

#cd /media/tushar/B19D-ABF3/latex1/6_3D_april_11/13_final/screenshot/5_pressure_positive_negative_2

#temp0=$(pwd)
#temp1=$(ls)
#temp2=($temp1)

#for var1 in {0..249}
#do
#	temp6=${temp2[$var1]}
#	temp7="$temp0/$temp6"

#	convert -crop +373+98 "$temp7" "$temp7"
#	convert -crop -373-98 "$temp7" "$temp7"
#done

#cd /media/tushar/B19D-ABF3/latex1/6_3D_april_11/13_final/screenshot/5_pressure_positive_negative_3

#temp0=$(pwd)
#temp1=$(ls)
#temp2=($temp1)

#for var1 in {0..249}
#do
#	temp6=${temp2[$var1]}
#	temp7="$temp0/$temp6"

#	convert -crop +373+98 "$temp7" "$temp7"
#	convert -crop -373-98 "$temp7" "$temp7"
#done

#cd /media/tushar/B19D-ABF3/latex1/6_3D_april_11/13_final/screenshot/5_pressure_positive_negative_4

#temp0=$(pwd)
#temp1=$(ls)
#temp2=($temp1)

#for var1 in {0..249}
#do
#	temp6=${temp2[$var1]}
#	temp7="$temp0/$temp6"

#	convert -crop +373+98 "$temp7" "$temp7"
#	convert -crop -373-98 "$temp7" "$temp7"
#done


##cd /media/tushar/B19D-ABF3/latex1/6_3D_april_11/9_plume_perlin/2_velocity_template/1_screenshot/2

##temp0=$(pwd)
##temp1=$(ls)
##temp2=($temp1)

##for var1 in {0..159}
##do
##	temp6=${temp2[$var1]}
##	temp7="$temp0/$temp6"

##	convert -crop +373+98 "$temp7" "$temp7"
##	convert -crop -373-98 "$temp7" "$temp7"
##done

##cd /media/tushar/B19D-ABF3/latex1/6_3D_april_11/9_plume_perlin/2_velocity_template/1_screenshot/3

##temp0=$(pwd)
##temp1=$(ls)
##temp2=($temp1)

##for var1 in {0..159}
##do
##	temp6=${temp2[$var1]}
##	temp7="$temp0/$temp6"

##	convert -crop +373+98 "$temp7" "$temp7"
##	convert -crop -373-98 "$temp7" "$temp7"
##done
