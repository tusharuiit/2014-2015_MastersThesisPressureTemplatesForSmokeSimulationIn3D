#
# Simple example scene for a 2D simulation
# Simulation of a buoyant smoke density plume
#
from manta import *
#import manta

from time import sleep

# solver params
res = 128

res_x = res
res_y = res
gs = vec3(res_x,res_y,1)
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

#This is the pressure just after CG solve
pressure_before = s.create(RealGrid)

pressure_template_perlin = s.create(RealGrid)

flags.initDomain()
flags.fillGrid()

if (GUI):
	gui = Gui()
	gui.show()
	gui.pause()

#This is the source shape
source = s.create(Cylinder, center=gs*vec3(0.5,0.5,0.5), radius=res*0.48, z=gs*vec3(0, 0.48, 0))

# This is for perlin noise template
# It has a maximum value of multiplier and minimum value of 0.0
#create_perlin_template(pressure_template_perlin,multiplier=4.0,res_x=res_x,res_y=res_y)
#create_perlin_template_1(pressure_template_perlin,multiplier=4.0,res_x=res_x,res_y=res_y)

#	This is the noise with central mean value as 1
source.applyRNG_binary_NoiseToGrid_seeded(grid=density, value=1, fraction_white=0.20, res_x = res_x, res_y = res_y)

#create_perlin_template_varying(pressure_template_perlin,time=8,multiplier=4.0,res_x=res_x,res_y=res_y)

#main loop
for t in range(400):

#	setInflowBcs(vel, dire='y', value=vec3(0.0,0.3,0.0))

	advectSemiLagrange_time_fraction(flags=flags, vel=vel, grid=density, order=2, time_fraction = 1.0)
#	advectSemiLagrange_time_fraction(flags=flags, vel=vel, grid=vel, order=2, time_fraction = 1.0)
#	setWallBcs(flags=flags, vel=vel)    
#	setInflowBcs(vel, dire='y', value=vec3(0.0,0.3,0.0))

#	time range of application of pressure modification
	t1 = 5
	t2 = 185
	if (t==(t1-2)):
		gui.pause()
	if (t==(t2-2)):
		gui.pause()
	if (t>t1)and(t<t2):
	
		
		create_perlin_template_varying(pressure_template_perlin,time=t,multiplier=4.0,res_x=res_x,res_y=res_y)
#		This is velocity correction
		solvePressure3_part2(flags=flags, vel=vel, pressure=pressure_template_perlin)
		setWallBcs(flags=flags, vel=vel)
#		setInflowBcs(vel, dire='y', value=vec3(0.0,0.3,0.0))

	advectSemiLagrange_time_fraction(flags=flags, vel=vel, grid=vel, order=2, time_fraction = 1.0)
	setWallBcs(flags=flags, vel=vel)    
#	setInflowBcs(vel, dire='y', value=vec3(0.0,0.3,0.0))

	solvePressure3_part1(flags=flags, vel=vel, pressure=pressure, openBound='Y')
	solvePressure3_part2(flags=flags, vel=vel, pressure=pressure)
	print " POINT 4 "
	setWallBcs(flags=flags, vel=vel)
#	setInflowBcs(vel, dire='y', value=vec3(0.0,0.3,0.0))
	print " POINT 7 "
	timings.display()
	s.step()
#	gui.screenshot( '/home/tushar/manta_mikey_1/manta/build/pictures/december_15/grid_size_2/6/%03d_dens.png' % t )
