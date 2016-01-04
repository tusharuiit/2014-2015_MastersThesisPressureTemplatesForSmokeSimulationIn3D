#!/bin/bash

cd /home/tushar/manta_mikey_1/manta/build
#make -j4
#./manta ../scenes/mine/march_11/velocity_templates.py 8 1 300 128 400
#./manta ../scenes/mine/march_11/velocity_templates.py 9 1 300 128 400
#./manta ../scenes/mine/march_11/velocity_templates.py 10 1 300 128 400

#./manta ../scenes/report/8_plume_perlin_3d/3_velocity_templates_3d.py 1 15 300 64 1.5 1.25
#2_velocity_templates_exclude_source_1.25
#./manta ../scenes/report/8_plume_perlin_3d/3_velocity_templates_3d.py 2 1 200 64 1.5 1.25
#2_velocity_templates_exclude_source_1.50
#./manta ../scenes/report/8_plume_perlin_3d/3_velocity_templates_3d.py 2 1 200 64 1.5 1.50
#./manta ../scenes/report/8_plume_perlin_3d/3_velocity_templates_3d.py 3 15 300 64 1.5 1.50

#2_velocity_templates_include_source
#./manta ../scenes/report/8_plume_perlin_3d/3_velocity_templates_3d.py 2 1 200 64 1.5 1.25
#Code 1 , this one is with source included, 2_velocity_templates_include_source_1.25
#./manta ../scenes/report/8_plume_perlin_3d/3_velocity_templates_3d.py 2 15 200 64 1.5 1.25
#Code 1 , this one is with source included, 2_velocity_templates_include_source_1.50
#./manta ../scenes/report/8_plume_perlin_3d/3_velocity_templates_3d.py 2 15 200 64 1.5 1.50
#Code 1 , this one is with source included, 2_velocity_templates_include_source_1.00
./manta ../scenes/report/8_plume_perlin_3d/3_velocity_templates_3d.py 2 15 200 64 1.5 1.00


#./combine2.sh

#Play with Velocity Templates

#Play with upward velocity and high density regions
#with velocity.y and high density value

#Play with frequencies

#Play with unequal frequencies

#Play with outside the source

#Play only with horizontal velocities

#Play only with horizontal velocities in certain time steps or gradually increasing or decreasing in magnitude in the X direction
