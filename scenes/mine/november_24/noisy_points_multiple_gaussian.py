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

pressure_modification_1 = s.create(RealGrid)
pressure_modification_2 = s.create(RealGrid)
pressure_modification_3 = s.create(RealGrid)
pressure_modification_4 = s.create(RealGrid)
pressure_modification_5 = s.create(RealGrid)
pressure_modification_6 = s.create(RealGrid)
pressure_modification_7 = s.create(RealGrid)
pressure_modification_8 = s.create(RealGrid)
pressure_modification_9 = s.create(RealGrid)

flags.initDomain()
flags.fillGrid()

if (GUI):
	gui = Gui()
	gui.show()
	gui.pause()

#This is the original source
#source = s.create(Cylinder, center=gs*vec3(0.5,0.1,0.5), radius=res*0.14, z=gs*vec3(0, 0.02, 0))
#This is the source for noisy density
source = s.create(Cylinder, center=gs*vec3(0.5,0.5,0.5), radius=res*0.45, z=gs*vec3(0, 0.4, 0))

# position/center , radius and sigma for the centre of circular pressure modifier (Gaussian splat/kernel)
splat_center_5 = gs*vec3(0.5,0.5,0.5)
splat_radius = 2
splat_sigma = 1.0
#splat_center.x = 64*0.7
#splat_center.y = 64*0.5
#splat_center.z = 64*0.5
splat_center_1 = gs*vec3(0.25,0.25,0.5)
splat_center_2 = gs*vec3(0.25,0.5,0.5)
splat_center_3 = gs*vec3(0.25,0.75,0.5)
splat_center_4 = gs*vec3(0.5,0.25,0.5)
splat_center_6 = gs*vec3(0.5,0.75,0.5)
splat_center_7 = gs*vec3(0.75,0.25,0.5)
splat_center_8 = gs*vec3(0.75,0.5,0.5)
splat_center_9 = gs*vec3(0.75,0.75,0.5)

#	This is the noise with central mean value as 1
#	Create a noisy still source/frame/source of black and white points with a chosen percentage of black points
source.applyRNG_NoiseToGrid(grid=density, value=1, offset_value=0.2)

#main loop
for t in range(400):
#	if t<300:
#		source.applyToGrid(grid=density, value=1)

#       Inflow thingy may be put/imposed here , just before advectSemiLagrange
#	if t<200:
#		source.applyToGrid(grid=vel, value=velInflow)

	advectSemiLagrange(flags=flags, vel=vel, grid=density, order=2)    
	advectSemiLagrange(flags=flags, vel=vel, grid=vel,     order=2)

	setWallBcs(flags=flags, vel=vel)    

#	if t<200:
#		addBuoyancy(density=density, vel=vel, gravity=vec3(0,-4e-3,0), flags=flags)

	solvePressure3_part1(flags=flags, vel=vel, pressure=pressure, openBound='Y')

#	This is normal Gaussian placed at the centre
	create_pressure_modifier_5(pressure_modification_5 , splat_radius , splat_center_5 , splat_sigma)

#	These are normal Gaussians placed at different points of the domain
	create_pressure_modifier_5(pressure_modification_1 , splat_radius , splat_center_1 , splat_sigma)
	create_pressure_modifier_5(pressure_modification_2 , splat_radius , splat_center_2 , splat_sigma)
	create_pressure_modifier_5(pressure_modification_3 , splat_radius , splat_center_3 , splat_sigma)
	create_pressure_modifier_5(pressure_modification_4 , splat_radius , splat_center_4 , splat_sigma)
	create_pressure_modifier_5(pressure_modification_6 , splat_radius , splat_center_6 , splat_sigma)
	create_pressure_modifier_5(pressure_modification_7 , splat_radius , splat_center_7 , splat_sigma)
	create_pressure_modifier_5(pressure_modification_8 , splat_radius , splat_center_8 , splat_sigma)
	create_pressure_modifier_5(pressure_modification_9 , splat_radius , splat_center_9 , splat_sigma)

#	time range of application of pressure modification
	t1 = 80
	t2 = 120
	if (t==(t1 - 3)):
		gui.pause()
	if (t==(t2 - 3)):
		gui.pause()
	pressure_before.copyFrom(pressure)

	if (t>t1)and(t<t2):
		modify_pressure_constant(pressure,pressure_modification_1,multiplier=1)
		modify_pressure_constant(pressure,pressure_modification_2,multiplier=1)
		modify_pressure_constant(pressure,pressure_modification_3,multiplier=1)
		modify_pressure_constant(pressure,pressure_modification_4,multiplier=1)
		modify_pressure_constant(pressure,pressure_modification_5,multiplier=1)
		modify_pressure_constant(pressure,pressure_modification_6,multiplier=1)
		modify_pressure_constant(pressure,pressure_modification_7,multiplier=1)
		modify_pressure_constant(pressure,pressure_modification_8,multiplier=1)
		modify_pressure_constant(pressure,pressure_modification_9,multiplier=1)

	solvePressure3_part2(flags=flags, vel=vel, pressure=pressure)

	setWallBcs(flags=flags, vel=vel)

	timings.display()

	s.step()
#	gui.pause()
#	gui.screenshot( './pictures/point_1_time_step/grid_res_128/magnify_4/scale_1.6/velocity_only/%03d_velocity_only.png' % t )
