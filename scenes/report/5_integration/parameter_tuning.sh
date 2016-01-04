#!/bin/bash

cd /home/tushar/manta_mikey_1/manta/build
#make -j4
#./manta ../scenes/mine/march_9/equal.py 1.0 4 4 4 1
#./manta ../scenes/mine/march_9/equal.py 1.0 12 4 4 2 64 64 256 32
#./manta ../scenes/mine/march_9/equal.py 1.0 12 4 4 3 64 64 256 32
#./manta ../scenes/mine/march_9/x_and_y.py 1.0 12 4 4 4 5 6 64 64 256
#./manta ../scenes/mine/march_9/x_y_xy.py   1.0 12 4 4 7 8 9 64 64 256
#./manta ../scenes/mine/march_9/static_x.py  1    2  3 4 5 6 7   8   9   10
#./manta ../scenes/mine/march_9/static_x.py   12.0 24 8 8 1 9 1   254 128 256
#./manta ../scenes/mine/march_9/static_x_y.py   11.0 21 7 2 1 10 1   398 128 400
#./manta ../scenes/mine/march_9/static_x_y.py           11.0 21 7 2 2 11 1 398 128 400
#./manta ../scenes/mine/march_9/static_interpolate.py    11.0 21 7 2 2 11 1 398 128 400
#	Global
#./manta ../scenes/mine/march_9/plume_interpolate.py    9.0 21 7 2 4 11 1 398 128 400
#	Density local
#./manta ../scenes/mine/march_9/plume_interpolate.py    1.0 15 5 1 5 11 1 398 128 400
#1	Plume
#./manta ../scenes/mine/march_11/plume_interpolate.py    2.5 21 7 2 1 11 1 398 128 400 2.0
#2	apply_template_density_localized
#./manta ../scenes/mine/march_11/plume_interpolate.py    2.5 21 7 2 2 11 1 398 128 400 2.0
#3	apply_template_density_localized
#./manta ../scenes/mine/march_11/plume_interpolate.py    8.5 21 7 2 3 11 1 398 128 400 2.0
#4	apply_template_density_localized
#./manta ../scenes/mine/march_11/plume_interpolate.py    12.5 21 7 2 4 11 1 398 128 400 2.0
#5	apply_template_density_localized - Very strong for a few time steps
#./manta ../scenes/mine/march_11/plume_interpolate.py    30.5 21 7 1 5 11 85 95 128 400 2.0
#6	apply_template_density_localized - Very strong in X for 1 time step every Y time-steps between
#./manta ../scenes/mine/march_11/6.py    15.0 9 27 1 6 11 55 105 128 400 15
#./manta ../scenes/mine/march_11/6.py     15.0 9 27 1 7 11 50 120 128 400 10
#./manta ../scenes/mine/march_11/plume_all_directions.py 10.0 9 27 1 11 11 50 250 128 400


#--Till now it was the one till March 11. Now on it is for the report.
./manta ../scenes/report/5_integration/plume_2d_x.py 15.0 9 27 1 7 11 50 120 128 400 10




#Play with Velocity Templates

#Play with upward velocity and high density regions
#with velocity.y and high density value

#Play with frequencies

#Play with unequal frequencies

#Play with outside the source

#Play only with horizontal velocities

#Play only with horizontal velocities in certain time steps or gradually increasing or decreasing in magnitude in the X direction
