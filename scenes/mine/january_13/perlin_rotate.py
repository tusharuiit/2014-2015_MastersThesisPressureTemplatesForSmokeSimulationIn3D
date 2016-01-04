#
# Simple example scene for a 2D simulation
# Simulation of a buoyant smoke density plume
#
from manta import *
#import manta

import os

from time import sleep

# solver params
res = 128

gs_x = res
gs_y = res
gs = vec3(gs_x,gs_y,1)
s = Solver(name='main', gridSize = gs, dim=2)
s.timestep = 1.0
timings = Timings()

# prepare grids
flags = s.create(FlagGrid)
#flag_test = s.create(FlagGrid)
vel = s.create(MACGrid)
#vel_test = s.create(MACGrid)
density = s.create(RealGrid)
pressure = s.create(RealGrid)

#This is a dummy grid created for the velocity-only visualization in the GUI.Velocity is MAC Grid
velocity_only = s.create(RealGrid)

pressure_template_image = s.create(RealGrid)
boundary_zero_smoother = s.create(RealGrid)
#	This is for perlin noise template
#	It has a maximum value of multiplier and minimum value of 0.0
create_image_template(boundary_zero_smoother,multiplier=1.0,res_x=gs_x,res_y=gs_y)

flags.initDomain()
flags.fillGrid()

if (GUI):
	gui = Gui()
	gui.show()
	gui.pause()

#This is the source shape
source = s.create(Cylinder, center=gs*vec3(0.5,0.5,0.5), radius=res*0.48, z=gs*vec3(0, 0.48, 0))

#	This is the noise with central mean value as 1
source.applyRNG_binary_NoiseToGrid_seeded(grid=density, value=1, fraction_white=0.20, res_x = gs_x, res_y = gs_y)

# Open a file
#fo = open("/home/tushar/manta_mikey_1/manta/scenes/mine/january_5/foo.txt", "wb")

#main loop
last_time_step = 400
for t in range(last_time_step):

	# This is for an image template
	# It has a maximum value of multiplier and minimum value of 0.0
	create_image_template_varying(pressure_template_image,t,multiplier=8.0,res_x=gs_x,res_y=gs_y)
	pressure_template_image.mult(boundary_zero_smoother)

#	time range of application of pressure modification
	t1 = 5
	t2 = 305
	if (t==(t1)):
		gui.pause()
	if (t==(t2)):
		gui.pause()
	if (t>=t1)and(t<t2):
#		This is velocity correction
		solvePressure3_part2(flags=flags, vel=vel, pressure=pressure_template_image)
		setWallBcs(flags=flags, vel=vel)

	advectSemiLagrange_time_fraction(flags=flags, vel=vel, grid=vel, order=2, time_fraction = 0.5)
#	advectSemiLagrange_time_fraction(flags=flags, vel=vel, grid=density, order=2, time_fraction = 0.5)
	setWallBcs(flags=flags, vel=vel)    

	solvePressure3_part1(flags=flags, vel=vel, pressure=pressure, openBound='Y')
	solvePressure3_part2(flags=flags, vel=vel, pressure=pressure)
	setWallBcs(flags=flags, vel=vel)

	advectSemiLagrange_time_fraction(flags=flags, vel=vel, grid=density, order=2, time_fraction = 1.0)
	advectSemiLagrange_time_fraction(flags=flags, vel=vel, grid=vel, order=2, time_fraction = 0.5)
	setWallBcs(flags=flags, vel=vel)

	timings.display()
	s.step()

	gui.screenshot( '/home/tushar/manta_mikey_1/manta/build/pictures/january_13/4/%03d.png' % t )
