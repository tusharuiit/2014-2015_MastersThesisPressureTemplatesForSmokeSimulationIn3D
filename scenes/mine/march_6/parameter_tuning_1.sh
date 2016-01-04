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
#vorticity_based_gradient_scaling=1
#Run count for directory
#time_1=50
#time_2=225


#./manta ../scenes/mine/march_6/plume_perlin_templates_2d.py 6.5 6.5 10.0 30.0 30.0 10.0 0.01 1 1 6 50 225
#./manta ../scenes/mine/march_6/plume_perlin_2d.py 20.5 20.5 10.0 30.0 30.0 10.0 0.01 1 2 50 225
./manta ../scenes/mine/march_6/plume_perlin_2d.py 9.5 9.5 10.0 30.0 30.0 10.0 0.01 1 4 50 225
