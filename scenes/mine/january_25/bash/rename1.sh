#!/bin/bash

cd /home/tushar/manta_mikey_1/manta/build/pictures/january_25/images/5

temp0=$(pwd)
temp1=$(ls)
temp2=($temp1)

foo1='_wot'
foo3='.png'

for var1 in {000..399}
do
	foo2=$var1
	foo4=$foo2$foo3
	mv $foo4 $foo2$foo1$foo3
done
