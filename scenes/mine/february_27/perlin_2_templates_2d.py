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
pressure = s.create(RealGrid)

#This is the noise template that is being
pressure_template_perlin_x = s.create(RealGrid)
pressure_template_perlin_y = s.create(RealGrid)
boundary_zero_smoother = s.create(RealGrid)

#	This is for perlin noise template
#	It has a maximum value of multiplier and minimum value of 0.0
create_image_template(boundary_zero_smoother,multiplier=1.0,res_x=gs_x,res_y=gs_y)

flags.initDomain()
flags.fillGrid()

if (GUI):
	gui = Gui()
	gui.show()
#	gui.pause()

#This is the source shape
source = s.create(Cylinder, center=gs*vec3(0.5,0.5,0.5), radius=res*0.48, z=gs*vec3(0, 0.48, 0))

#	This is the noise with central mean value as 1
source.applyRNG_binary_NoiseToGrid_seeded(grid=density, value=1, fraction_white=0.20, res_x = gs_x, res_y = gs_y)

#main loop
for t in range(250):

#	time range of application of pressure modification
	t1 = 1
	t2 = 301
	if (t==(t1)):
		gui.pause()
	if (t==(t2)):
		gui.pause()
	if ((t>=t1)and(t<t2)):

#		create_perlin_template_24(pressure_template_perlin,multiplier=8.0,res_x=gs_x,res_y=gs_y,time=t,start_x=5.0,start_y=15.0,time_steps=60)
		create_perlin_template_25(pressure_template_perlin_x,multiplier=12.0,res_x=gs_x,res_y=gs_y,time=t,frequency_x=7.0,frequency_y=21.0,sign_translate_x = 0.1, sign_translate_y = 0.1)
		create_perlin_template_25(pressure_template_perlin_y,multiplier=12.0,res_x=gs_x,res_y=gs_y,time=t,frequency_x=21.0,frequency_y=7.0,sign_translate_x = 0.1, sign_translate_y = 0.1)
		pressure_template_perlin_x.mult(boundary_zero_smoother)
		pressure_template_perlin_y.mult(boundary_zero_smoother)
#		This is velocity correction
		if(t%2==0):
			solvePressure3_part2(flags=flags, vel=vel, pressure=pressure_template_perlin_x)
			solvePressure3_part2(flags=flags, vel=vel, pressure=pressure_template_perlin_y)
			setWallBcs(flags=flags, vel=vel)
		else:
			solvePressure3_part2(flags=flags, vel=vel, pressure=pressure_template_perlin_y)
			solvePressure3_part2(flags=flags, vel=vel, pressure=pressure_template_perlin_x)
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
	gui.screenshot( '/home/tushar/manta_mikey_1/manta/build/pictures/february_27/images/4/%03d.png' % t )
