#
# Simple example scene for a 2D simulation
# Simulation of a buoyant smoke density plume
#
from manta import *
#import manta

# solver params
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

pressure_modification_elliptical_gaussian_x = s.create(RealGrid)
pressure_modification_elliptical_gaussian_y = s.create(RealGrid)

flags.initDomain()
flags.fillGrid()

if (GUI):
	gui = Gui()
	gui.show()
	gui.pause()

#This is the original source
#source = s.create(Cylinder, center=gs*vec3(0.5,0.1,0.5), radius=res*0.14, z=gs*vec3(0, 0.02, 0))
#This is the source for point density
source = s.create(Cylinder, center=gs*vec3(0.5,0.5,0.5), radius=res*0.48, z=gs*vec3(0, 0.48, 0))

# This is for elliptical gaussians x and y superimposed
# This is for elliptical gaussian x
x_splat_center_ellipse = gs*vec3(0.5,0.5,0.5)
x_splat_sigma_x = 16.0
x_splat_sigma_y = 2.0
x_splat_radius_x = (res*0.5) - 1.0
x_splat_radius_y = (res*0.25) - 1.0
# This is for elliptical gaussian y
y_splat_center_ellipse = gs*vec3(0.5,0.5,0.5)
y_splat_sigma_x = 2.0
y_splat_sigma_y = 16.0
y_splat_radius_x = (res*0.25) - 1.0
y_splat_radius_y = (res*0.5) - 1.0


#	This is the noise with central mean value as 1
source.applyRNG_binary_NoiseToGrid(grid=density, value=1, fraction_white=0.20 )

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

#	This is elliptical Gaussian
	create_pressure_modifier_elliptical_gaussian(pressure_modification_elliptical_gaussian_x , x_splat_radius_x , x_splat_radius_y , x_splat_center_ellipse , x_splat_sigma_x , x_splat_sigma_y)
	create_pressure_modifier_elliptical_gaussian(pressure_modification_elliptical_gaussian_y , y_splat_radius_y , y_splat_radius_y , y_splat_center_ellipse , y_splat_sigma_x , y_splat_sigma_y)

#	time range of application of pressure modification
	t1 = 80
	t2 = 120
	if (t==(t1-2)):
		gui.pause()
	if (t==(t2-2)):
		gui.pause()
	pressure_before.copyFrom(pressure)

#	if (t==82):
#		source.applydensity_at_pointToGrid(grid=density, position = (gs*vec3(0.5,0.5,0.5)), value=0)

	if (t>t1)and(t<t2):
		modify_pressure_constant(pressure,pressure_modification_elliptical_gaussian_x,multiplier=2.0)
		modify_pressure_constant(pressure,pressure_modification_elliptical_gaussian_y,multiplier=2.0)

	solvePressure3_part2(flags=flags, vel=vel, pressure=pressure)

	setWallBcs(flags=flags, vel=vel)

	timings.display()

	s.step()
#	gui.screenshot( './pictures/noise_elliptical_gaussian/grid_res_128/noise_percent_12.5/sigma_16_2/scale_8/%03d_density.png' % t )
