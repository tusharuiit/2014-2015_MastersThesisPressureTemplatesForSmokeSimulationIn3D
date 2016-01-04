#
# Simple example scene for a 2D simulation
# Simulation of a noisy smoke density frame
#
from manta import *
#import manta

# solver params
#res = 64
res = 128

gs = vec3(res,res,1)
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

pressure_modification_inverted_stairs = s.create(RealGrid)

flags.initDomain()
flags.fillGrid()

if (GUI):
	gui = Gui()
	gui.show()
	gui.pause()

#This is the original source
#source = s.create(Cylinder, center=gs*vec3(0.5,0.1,0.5), radius=res*0.14, z=gs*vec3(0, 0.02, 0))
#This is the source for noisy density with a certain percentage of black points
source = s.create(Cylinder, center=gs*vec3(0.5,0.5,0.5), radius=res*0.45, z=gs*vec3(0, 0.4, 0))

# This is for inverted stairs
stairs_origin = gs*vec3(0.5,0.5,0.5)
stairs_depth = 6

#	This is the noise with central mean value as 1 and random values from (value-offset_value) to (value+offset_value)
source.applyRNG_binary_NoiseToGrid(grid=density, value=1, fraction_white=0.20 )

#	main loop
for t in range(400):
#	if t<300:
#		source.applyToGrid(grid=density, value=1)
#	This is the noise with central mean value as 1
#		source.applyRNG_NoiseToGrid(grid=density, value=1, offset_value=0.2)

#       Inflow thingy may be put/imposed here , just before advectSemiLagrange
#	if t<200:
#		source.applyToGrid(grid=vel, value=velInflow)

	advectSemiLagrange(flags=flags, vel=vel, grid=density, order=2)    
	advectSemiLagrange(flags=flags, vel=vel, grid=vel,     order=2)

	setWallBcs(flags=flags, vel=vel)    

#	if t<200:
#		addBuoyancy(density=density, vel=vel, gravity=vec3(0,-4e-3,0), flags=flags)

	solvePressure3_part1(flags=flags, vel=vel, pressure=pressure, openBound='Y')

	create_pressure_modifier_inverted_stairs(pressure_modification_inverted_stairs , stairs_origin , stairs_depth)

#	time range of application of pressure modification
	t1 = 80
	t2 = 120
	if (t==(t1-2)):
		gui.pause()
	if (t==(t2-2)):
		gui.pause()
	pressure_before.copyFrom(pressure)

	if (t>t1)and(t<t2):
		modify_pressure_constant(pressure,pressure_modification_inverted_stairs,multiplier=1)

	solvePressure3_part2(flags=flags, vel=vel, pressure=pressure)

	setWallBcs(flags=flags, vel=vel)

	timings.display()

	s.step()
#	gui.pause()
#	gui.screenshot( './pictures/point_1_time_step/grid_res_128/magnify_4/scale_1.6/velocity_only/%03d_velocity_only.png' % t )
