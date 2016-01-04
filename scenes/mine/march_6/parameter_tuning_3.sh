#!/bin/bash

cd /home/tushar/manta_mikey_1/manta/build

#What are the paramters to be altered ?
#multiplier_x=1.0
#multiplier_y=1.0
#frequency_x_bias_x=7.0
#frequency_x_bias_y=21.0
#frequency_y_bias_x=21.0
#frequency_y_bias_y=7.0
#density_threshold=0.01
#density_based_gradient_scaling=1
#Run count for directory
#time_1=50
#time_2=225


./manta ../scenes/mine/march_6/plume_perlin_2d_3.py 2.0 2.0 6.0 18.0 0.9 0.9 3.0 3.0 18.0 6.0 0.9 0.9 3.0 3.0 0.01 1 2 50 225
