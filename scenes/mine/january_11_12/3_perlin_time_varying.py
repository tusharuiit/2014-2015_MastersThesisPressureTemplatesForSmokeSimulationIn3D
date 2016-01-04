#
# Simple example scene for a 2D simulation
# Simulation of a buoyant smoke density plume
#
from manta import *
#import manta

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
density1 = s.create(RealGrid)
pressure = s.create(RealGrid)

#This is the noise template that is being
pressure_template_perlin = s.create(RealGrid)
boundary_zero_smoother = s.create(RealGrid)

create_image_template(boundary_zero_smoother,multiplier=1.0,res_x=gs_x,res_y=gs_y)
	# This is for perlin noise template
	# It has a maximum value of multiplier and minimum value of 0.0
#create_perlin_template(pressure_template_perlin,multiplier=4.0,res_x=gs_x,res_y=gs_y,frequency_changer=8)
#pressure_template_perlin.mult(boundary_zero_smoother)

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
for t in range(600):

	advectSemiLagrange_time_fraction(flags=flags, vel=vel, grid=density, order=2, time_fraction = 0.5)
	advectSemiLagrange_time_fraction(flags=flags, vel=vel, grid=vel, order=2, time_fraction = 0.5)
	setWallBcs(flags=flags, vel=vel)

#	time range of application of pressure modification
	t1 = 5
	t2 = 305
	if (t==(t1)):
		gui.pause()
	if (t==(t2)):
		gui.pause()
	if (t>=t1)and(t<t2):
#	# This is for perlin noise template
#	# It has a maximum value of multiplier and minimum value of 0.0
		create_perlin_template_varying_scaled(pressure_template_perlin,multiplier=4.0,res_x=gs_x,res_y=gs_y,frequency_changer=t)
		pressure_template_perlin.mult(boundary_zero_smoother)

#		This is velocity correction
		solvePressure3_part2(flags=flags, vel=vel, pressure=pressure_template_perlin)
		setWallBcs(flags=flags, vel=vel)

	advectSemiLagrange_time_fraction(flags=flags, vel=vel, grid=vel, order=2, time_fraction = 0.5)
	advectSemiLagrange_time_fraction(flags=flags, vel=vel, grid=density, order=2, time_fraction = 0.5)
	setWallBcs(flags=flags, vel=vel)    

	solvePressure3_part1(flags=flags, vel=vel, pressure=pressure, openBound='Y')
	solvePressure3_part2(flags=flags, vel=vel, pressure=pressure)
	setWallBcs(flags=flags, vel=vel)

	timings.display()
	s.step()

	density1.copyFrom(density)
#	density1.multConst(100.0)
	density1.save('/home/tushar/manta_mikey_1/manta/build/pictures/january_11_12/vol_files/1/2d/den%04d.vol' % t)
#	gui.screenshot( '/home/tushar/manta_mikey_1/manta/build/pictures/january_9/7/images/2/%03d.png' % t )
