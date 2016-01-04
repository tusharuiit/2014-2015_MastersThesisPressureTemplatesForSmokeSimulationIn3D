#!/bin/bash

cd /home/tushar/manta_mikey_1/manta/build/pictures/february_26/rendered/3
source /home/tushar/mitsuba/mitsuba/setpath.sh
mitsuba den0205_75_512.xml

#for var1 in {0000..0249}
#do
# cp hetvol_1000.xml den$var1.xml
#done

#for var1 in {0000..0249}
#do
# sed -i s/smoke.vol/den$var1.vol/g den$var1.xml
#done

#for var1 in {0150..0154}
#do
# mitsuba den$var1.xml
#done


#for var1 in {0150..0174}
#do
# convert den$var1.exr den$var1.png
#done
