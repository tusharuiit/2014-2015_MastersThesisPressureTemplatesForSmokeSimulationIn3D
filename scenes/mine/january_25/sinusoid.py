#
# Simple example scene for a 2D simulation
# Simulation of a buoyant smoke density plume
#
from manta import *
#import manta

from time import sleep
import sys

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

#main loop
for t in range(400):

#	time range of application of pressure modification
	t1 = 1
	t2 = 301
	if (t==(t1)):
		gui.pause()
	if (t==(t2)):
		gui.pause()
	if ((t>=t1)and(t<t2)):
		
		create_perlin_template_16(pressure_template_perlin,multiplier=12.0,res_x=gs_x,res_y=gs_y,time=t, x_factor=7.0, y_factor=7.0)
		pressure_template_perlin.mult(boundary_zero_smoother)
#		This is velocity correction
		solvePressure3_part2(flags=flags, vel=vel, pressure=pressure_template_perlin)
		setWallBcs(flags=flags, vel=vel)

	advectSemiLagrange_time_fraction(flags=flags, vel=vel, grid=vel, order=2, time_fraction = 0.5)
	setWallBcs(flags=flags, vel=vel)    

	solvePressure3_part1(flags=flags, vel=vel, pressure=pressure, openBound='Y')
	solvePressure3_part2(flags=flags, vel=vel, pressure=pressure)
	setWallBcs(flags=flags, vel=vel)

	advectSemiLagrange_time_fraction(flags=flags, vel=vel, grid=density, order=2, time_fraction = 1.0)
	advectSemiLagrange_time_fraction(flags=flags, vel=vel, grid=vel, order=2, time_fraction = 0.5)
	setWallBcs(flags=flags, vel=vel)

	timings.display()
	s.step()

#	gui.screenshot( '/home/tushar/manta_mikey_1/manta/build/pictures/january_25/images/15/%03d_dens.png' % t )
