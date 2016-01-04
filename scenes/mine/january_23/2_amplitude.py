#
# Simple example scene for a 2D simulation
# Simulation of a buoyant smoke density plume
#
from manta import *
#import manta

from time import sleep

import numpy
import scipy
import scipy.fftpack
from numpy import mean, sqrt, square, arange

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

#This is the noise template that is being
pressure_template_perlin = s.create(RealGrid)
boundary_zero_smoother = s.create(RealGrid)

#This the boundary smoother
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

fo = open("/home/tushar/manta_mikey_1/manta/build/pictures/january_17/data_files/foo.txt", "wb")
f = open("/home/tushar/manta_mikey_1/manta/scenes/mine/january_22/audio.dat")
d = f.readlines()

#main loop
for t in range(400):

#	time range of application of pressure modification
	t1 = 5
	t2 = 305
	if (t==(t1)):
		gui.pause()
	if (t==(t2)):
		gui.pause()
	if (t>=t1)and(t<t2):

		chunk = d[1666*(t-5)+2:1666*(t-5+1)+2]
		chunk_left = []
		chunk_right = []

		for i in chunk:
			chunk_left.append(float(i.split()[0]))
			chunk_right.append(float(i.split()[1]))

		amplitude = sqrt(mean(square(chunk_right))) * 15.0

#	This is for perlin noise template
#	It has a maximum value of multiplier and minimum value of 0.0
#	Note that this function starting at t=5 is important
		create_perlin_template_14(pressure_template_perlin,multiplier=amplitude,res_x=gs_x,res_y=gs_y,time=t)
		pressure_template_perlin.mult(boundary_zero_smoother)

#		This is velocity correction
		solvePressure3_part2(flags=flags, vel=vel, pressure=pressure_template_perlin)
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
#	density.save('/home/tushar/manta_mikey_1/manta/build/pictures/january_11_12/vol_files/2d/den%04d.vol' % t)
	gui.screenshot( '/home/tushar/manta_mikey_1/manta/build/pictures/january_22/images/3/%03d.png' % t )
