#
# Simple example scene for a 2D simulation
# Simulation of a buoyant smoke density plume
#
from manta import *
#import manta

from time import sleep

# solver params
res = 128

res_x = 1.0*res
res_y = 1.0*res
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

pressure_modification_elliptical_gaussian = s.create(RealGrid)

flags.initDomain()
flags.fillGrid()

if (GUI):
	gui = Gui()
	gui.show()
	gui.pause()

#This is the source shape
source = s.create(Cylinder, center=gs*vec3(0.5,0.5,0.5), radius=res_x*0.48, z=gs*vec3(0, 0.48, 0))

# This is for elliptical gaussian
splat_center_ellipse = gs*vec3(0.5,0.5,0.5)
splat_sigma_x = 16.0
splat_sigma_y = 2.0
splat_radius_x = (res_x*0.5) - 4.0
splat_radius_y = (res_y*0.25) - 2.0

#	This is the noise with central mean value as 1
source.applyRNG_binary_NoiseToGrid_seeded(grid=density, value=1, fraction_white=0.20, res_x = res_x, res_y = res_y)

#This is elliptical Gaussian
create_pressure_modifier_elliptical_gaussian_with_multiplier(pressure_modification_elliptical_gaussian , splat_radius_x , splat_radius_y , splat_center_ellipse , splat_sigma_x , splat_sigma_y , multiplier = 4.0, lower_factor = 1.0, higher_factor = 1.0)

#	main loop
for t in range(400):

#	time range of application of pressure modification
	t1 = 5
	t2 = 95

	setInflowBcs(vel, dire='y', value=vec3(0.0,1.0,0.0))

	if (t>t1)and(t<t2):
		advectSemiLagrange(flags=flags, vel=vel, grid=pressure_modification_elliptical_gaussian, order=2)

	advectSemiLagrange_time_fraction(flags=flags, vel=vel, grid=density, order=2, time_fraction = 1.0)
	advectSemiLagrange_time_fraction(flags=flags, vel=vel, grid=vel, order=2, time_fraction = 0.5)
	setWallBcs(flags=flags, vel=vel)    
	setInflowBcs(vel, dire='y', value=vec3(0.0,1.0,0.0))


	if (t==(t1-2)):
		gui.pause()
	if (t==(t2-2)):
		gui.pause()

	if (t>t1)and(t<t2):
#		This is velocity correction with template
		solvePressure3_part2(flags=flags, vel=vel, pressure=pressure_modification_elliptical_gaussian)
		setWallBcs(flags=flags, vel=vel)
		setInflowBcs(vel, dire='y', value=vec3(0.0,1.0,0.0))

	advectSemiLagrange_time_fraction(flags=flags, vel=vel, grid=vel, order=2, time_fraction = 0.5)
	setWallBcs(flags=flags, vel=vel)    
	setInflowBcs(vel, dire='y', value=vec3(0.0,1.0,0.0))

	solvePressure3_part1(flags=flags, vel=vel, pressure=pressure, openBound='Y')
	solvePressure3_part2(flags=flags, vel=vel, pressure=pressure)
	setWallBcs(flags=flags, vel=vel)
	setInflowBcs(vel, dire='y', value=vec3(0.0,1.0,0.0))

	timings.display()
	s.step()
#	gui.screenshot('/home/tushar/manta_mikey_1/manta/build/pictures/december_2/advect_template/inflow_velocity/multiplier/2.0/0.5/template/%03d_den.png' % t )
