#
# Simple example scene for a 2D simulation
# Simulation of a buoyant smoke density plume
#
from manta import *
#import manta

# solver params
res = 64

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

#pressure_modification_1 = s.create(RealGrid)
#pressure_modification_2 = s.create(RealGrid)
#pressure_modification_3 = s.create(RealGrid)
#pressure_modification_4 = s.create(RealGrid)
#pressure_modification_5 = s.create(RealGrid)
#pressure_modification_6 = s.create(RealGrid)
#pressure_modification_7 = s.create(RealGrid)
#pressure_modification_8 = s.create(RealGrid)
#pressure_modification_9 = s.create(RealGrid)

#pressure_modification_10 = s.create(RealGrid)
#pressure_modification_11 = s.create(RealGrid)
#pressure_modification_12 = s.create(RealGrid)
#pressure_modification_13 = s.create(RealGrid)

#pressure_modification_elliptical_gaussian = s.create(RealGrid)

#pressure_modification_inverted_pascal = s.create(RealGrid)

#pressure_modification_inverted_stairs = s.create(RealGrid)

#pressure_modification_smiling_face = s.create(RealGrid)

#pressure_modification_spiral = s.create(RealGrid)

#pressure_modification_advection = s.create(RealGrid)

#pressure_modification_still_frame = s.create(RealGrid)

#pressure_modification_inflow = s.create(RealGrid)

pressure_modification_mexican_hat = s.create(RealGrid)

flags.initDomain()
flags.fillGrid()

if (GUI):
	gui = Gui()
	gui.show()
	gui.pause()

#This is the original source
#source = s.create(Cylinder, center=gs*vec3(0.5,0.1,0.5), radius=res*0.14, z=gs*vec3(0, 0.02, 0))
#This is the source for the still density frame
#source = s.create(Cylinder, center=gs*vec3(0.5,0.5,0.5), radius=res*0.14, z=gs*vec3(0, 0.1, 0))
#This is the source for the advection of the pressure modification
#source = s.create(Cylinder, center=gs*vec3(0.5,0.1,0.5), radius=res*0.14, z=gs*vec3(0, 0.02, 0))
#This is the source for the inflow
#source = s.create(Cylinder, center=gs*vec3(0.5,0.1,0.5), radius=res*0.14, z=gs*vec3(0, 0.04, 0))
#This is the source for noisy density
#source = s.create(Cylinder, center=gs*vec3(0.5,0.5,0.5), radius=res*0.45, z=gs*vec3(0, 0.4, 0))
#This is the source for point density in all time steps
source = s.create(Cylinder, center=gs*vec3(0.5,0.5,0.5), radius=res*0.45, z=gs*vec3(0, 0.4, 0))

# position/center , radius and sigma for the centre of circular pressure modifier (Gaussian splat/kernel)
splat_center_5 = vec3(31,31,0.5)
splat_radius = 2
splat_sigma = 1.0
#splat_center.x = 64*0.7
#splat_center.y = 64*0.5
#splat_center.z = 64*0.5
splat_center_1 = vec3(15,15,0.5)
splat_center_2 = vec3(15,31,0.5)
splat_center_3 = vec3(15,47,0.5)
splat_center_4 = vec3(31,15,0.5)
splat_center_6 = vec3(31,47,0.5)
splat_center_7 = vec3(47,15,0.5)
splat_center_8 = vec3(47,31,0.5)
splat_center_9 = vec3(47,47,0.5)

# This is for elliptical Gaussian
splat_center_ellipse = gs*vec3(0.5,0.5,0.5)
splat_sigma_x = 4.0
splat_sigma_y = 1.0
splat_radius_x = 4
splat_radius_y = 4

# This is for inverted Pascal
pascal_origin = gs*vec3(0.5,0.5,0.5)
pascal_depth = 5

# This is for inverted stairs
stairs_origin = gs*vec3(0.5,0.5,0.5)
stairs_depth = 6

# This is for smiling face
face_origin_nose = gs*vec3(0.5,0.5,0.5)

# This is for spiral
spiral_origin = vec3(32,35,0.5)

# This is for advection splat
advection_splat_center = gs*vec3(0.5,0.5,0.5)
advection_splat_radius = 2
advection_splat_sigma = 1.0

# This is for still frame
#still_frame_splat_center = vec3(31,31,0.5)
#still_frame_splat_center = gs*vec3(0.5,0.5,0.5)
still_frame_splat_center = gs*vec3(0.5,0.5,0.5)
still_frame_splat_radius = 2
still_frame_splat_sigma = 1.0

#This is for inflow velocity
velInflow = vec3(0.5,1.6,0.0)
inflow_splat_center = gs*vec3(0.5,0.5,0.5)
inflow_splat_radius = 2
inflow_splat_sigma = 1.0

# This is for circular mexican hat
mexican_hat_splat_center = gs*vec3(0.5,0.5,0.5)
mexican_hat_splat_sigma = 2.0
mexican_hat_splat_radius = 16

##	This is the noise with central mean value as 1
source.applyRNG_NoiseToGrid(grid=density, value=1, offset_value=0.2)
#	This is the density of value 1 applied at a single point
#source.applydensity_at_pointToGrid(grid=density, position = (gs*vec3(0.5,0.5,0.5)), value=1)

#main loop
for t in range(400):
#	if t<300:
##		source.applyToGrid(grid=density, value=1)
##	This is the noise with central mean value as 1
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

#	This is normal Gaussian
#	create_pressure_modifier_5(pressure_modification_5 , splat_radius , splat_center_5 , splat_sigma)

#	create_pressure_modifier_elliptical_gaussian(pressure_modification_elliptical_gaussian , splat_radius_x , splat_radius_y , splat_center_ellipse , splat_sigma_x , splat_sigma_y)

#	create_pressure_modifier_inverted_pascal(pressure_modification_inverted_pascal , pascal_origin , pascal_depth)

#	create_pressure_modifier_inverted_stairs(pressure_modification_inverted_stairs , stairs_origin , stairs_depth)

#	create_pressure_modifier_smiling_face(pressure_modification_smiling_face , face_origin_nose)

#	create_pressure_modifier_spiral(pressure_modification_spiral , spiral_origin)

#	create_pressure_modifier_advection(pressure_modification_advection , advection_splat_radius , advection_splat_center , advection_splat_sigma)

#	create_pressure_modifier_still_frame(pressure_modification_still_frame , still_frame_splat_radius , still_frame_splat_center , still_frame_splat_sigma)

#	create_pressure_modifier_inflow(pressure_modification_inflow , inflow_splat_radius , inflow_splat_center , inflow_splat_sigma)

#	This is mexican hat
	create_pressure_modifier_mexican_hat(pressure_modification_mexican_hat , mexican_hat_splat_radius , mexican_hat_splat_center , mexican_hat_splat_sigma)

#	This is Gaussian with alternate positive and negative
#	create_pressure_modifier_10(pressure_modification_10 , splat_radius , splat_center_5 , splat_sigma)
#	This is Gaussian with alternate positive and zero
#	create_pressure_modifier_11(pressure_modification_11 , splat_radius , splat_center_5 , splat_sigma)
#	This is a constant function = 1 with radius as Gaussian
#	create_pressure_modifier_12(pressure_modification_12 , splat_radius , splat_center_5 , splat_sigma)
#	This is a function with alternating 1 and -1 , radius as Gaussian
#	create_pressure_modifier_13(pressure_modification_13 , splat_radius , splat_center_5 , splat_sigma)

#	create_pressure_modifier_5(pressure_modification_1 , splat_radius , splat_center_1 , splat_sigma)
#	create_pressure_modifier_5(pressure_modification_2 , splat_radius , splat_center_2 , splat_sigma)
#	create_pressure_modifier_5(pressure_modification_3 , splat_radius , splat_center_3 , splat_sigma)
#	create_pressure_modifier_5(pressure_modification_4 , splat_radius , splat_center_4 , splat_sigma)
#	create_pressure_modifier_5(pressure_modification_6 , splat_radius , splat_center_6 , splat_sigma)
#	create_pressure_modifier_5(pressure_modification_7 , splat_radius , splat_center_7 , splat_sigma)
#	create_pressure_modifier_5(pressure_modification_8 , splat_radius , splat_center_8 , splat_sigma)
#	create_pressure_modifier_5(pressure_modification_9 , splat_radius , splat_center_9 , splat_sigma)

#	time range of application of pressure modification
	t1 = 80
	t2 = 150
	if (t==77):
		gui.pause()
	if (t==148):
		gui.pause()
	pressure_before.copyFrom(pressure)

#	if (t==82):
#		source.applydensity_at_pointToGrid(grid=density, position = (gs*vec3(0.5,0.5,0.5)), value=0)

	if (t>t1)and(t<t2):
#		modify_pressure_constant(pressure,pressure_modification_1,multiplier=-25)
#		modify_pressure_constant(pressure,pressure_modification_2,multiplier=-25)
#		modify_pressure_constant(pressure,pressure_modification_3,multiplier=-25)
#		modify_pressure_constant(pressure,pressure_modification_4,multiplier=-25)
#		modify_pressure_constant(pressure,pressure_modification_5,multiplier=2)
#		modify_pressure_constant(pressure,pressure_modification_6,multiplier=-25)
#		modify_pressure_constant(pressure,pressure_modification_7,multiplier=-25)
#		modify_pressure_constant(pressure,pressure_modification_8,multiplier=-25)
#		modify_pressure_constant(pressure,pressure_modification_9,multiplier=-25)

#		modify_pressure_constant(pressure,pressure_modification_10,multiplier=-25)
#		modify_pressure_constant(pressure,pressure_modification_11,multiplier=-25)
#		modify_pressure_constant(pressure,pressure_modification_12,multiplier=-25)
#		modify_pressure_constant(pressure,pressure_modification_13,multiplier=-25)

#		modify_pressure_variable(pressure,pressure_modification_5,multiplier=5)

#		modify_pressure_constant(pressure,pressure_modification_elliptical_gaussian,multiplier=-25)

#		modify_pressure_constant(pressure,pressure_modification_inverted_pascal,multiplier=25)

#		modify_pressure_constant(pressure,pressure_modification_inverted_stairs,multiplier=1)

#		modify_pressure_constant(pressure,pressure_modification_smiling_face,multiplier=-4)

#		modify_pressure_constant(pressure,pressure_modification_spiral,multiplier=1)

#		modify_pressure_constant(pressure,pressure_modification_advection,multiplier=1)
#		vel.multConst(-1);
#		advectSemiLagrange(flags=flags, vel=vel, grid=pressure_modification_advection, order=2)
#		vel.multConst(1);

#		modify_pressure_constant(pressure,pressure_modification_still_frame,multiplier=-4)

#		modify_pressure_constant(pressure,pressure_modification_inflow,multiplier=1)

		modify_pressure_constant(pressure,pressure_modification_mexican_hat,multiplier=1)

	solvePressure3_part2(flags=flags, vel=vel, pressure=pressure)

	setWallBcs(flags=flags, vel=vel)

	timings.display()

	s.step()
	print "Exited s.step()"
	gui.pause()
	print "Test PRINTING WITH VALUE OF t = "
	print t
#	gui.pause()
#	gui.screenshot( './pictures/point_1_time_step/grid_res_128/magnify_4/scale_1.6/velocity_only/%03d_velocity_only.png' % t )
