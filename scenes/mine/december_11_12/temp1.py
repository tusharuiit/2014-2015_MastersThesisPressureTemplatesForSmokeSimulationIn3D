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

pressure_modification_elliptical_gaussian = s.create(RealGrid)

flags.initDomain()
flags.fillGrid()

if (GUI):
	gui = Gui()
	gui.show()
	gui.pause()

#This is the source shape
source = s.create(Cylinder, center=gs*vec3(0.5,0.5,0.5), radius=res*0.48, z=gs*vec3(0, 0.48, 0))

# This is for elliptical gaussian
splat_center_ellipse = gs*vec3(0.5,0.5,0.5)
splat_center_ellipse_rounded = vec3(0,0,0)
splat_sigma_x = 16.0
splat_sigma_y = 2.0
splat_radius_x = (res*0.5) - 4.0
splat_radius_y = (res*0.25) - 1.0
velocity_center = vec3(0.0,0.0,0.0)

#	This is the noise with central mean value as 1
source.applyRNG_binary_NoiseToGrid_seeded(grid=density, value=1, fraction_white=0.20, res_x = res_x, res_y = res_y)

#main loop
for t in range(400):

	setInflowBcs(vel, dire='y', value=vec3(0.0,0.3,0.0))

	advectSemiLagrange_time_fraction(flags=flags, vel=vel, grid=density, order=2, time_fraction = 1.0)
	advectSemiLagrange_time_fraction(flags=flags, vel=vel, grid=vel, order=2, time_fraction = 1.0)
	setWallBcs(flags=flags, vel=vel)    
	setInflowBcs(vel, dire='y', value=vec3(0.0,0.3,0.0))

#	splat_center_ellipse_rounded.x = int(splat_center_ellipse.x+0.5)
#	splat_center_ellipse_rounded.y = int(splat_center_ellipse.y+0.5)
#	splat_center_ellipse_rounded.z = int(splat_center_ellipse.z+0.5)
#	velocity_center = vel.get(splat_center_ellipse_rounded)
#	print "velocity_center.x = ",velocity_center.x
#	print "velocity_center.y = ",velocity_center.y
#	print "velocity_center.z = ",velocity_center.z

#	This is elliptical Gaussian
	splat_center_ellipse = (splat_center_ellipse)+((s.timestep)*(velocity_center))
	print "splat_center_ellipse.x = ",splat_center_ellipse.x
	print "splat_center_ellipse.y = ",splat_center_ellipse.y
	print "splat_center_ellipse.z = ",splat_center_ellipse.z
	splat_center_ellipse_rounded.x = int(splat_center_ellipse.x+0.5)
	splat_center_ellipse_rounded.y = int(splat_center_ellipse.y+0.5)
	splat_center_ellipse_rounded.z = int(splat_center_ellipse.z+0.5)
	create_pressure_modifier_elliptical_gaussian_with_multiplier(pressure_modification_elliptical_gaussian , splat_radius_x , splat_radius_y , splat_center_ellipse_rounded , splat_sigma_x , splat_sigma_y , multiplier = 10.0, lower_factor = 1.0, higher_factor = 1.0)

#	time range of application of pressure modification
	t1 = 5
	t2 = 95
	if (t==(t1-2)):
		gui.pause()
	if (t==(t2-2)):
		gui.pause()
	if (t>t1)and(t<t2):
#		This is velocity correction
		solvePressure3_part2(flags=flags, vel=vel, pressure=pressure_modification_elliptical_gaussian)
		setWallBcs(flags=flags, vel=vel)
		setInflowBcs(vel, dire='y', value=vec3(0.0,0.3,0.0))


	advectSemiLagrange_time_fraction(flags=flags, vel=vel, grid=vel, order=2, time_fraction = 0.5)
	setWallBcs(flags=flags, vel=vel)    
	setInflowBcs(vel, dire='y', value=vec3(0.0,0.3,0.0))

#	splat_center_ellipse_rounded.x = int(splat_center_ellipse.x+0.5)
#	splat_center_ellipse_rounded.y = int(splat_center_ellipse.y+0.5)
#	splat_center_ellipse_rounded.z = int(splat_center_ellipse.z+0.5)
#	velocity_center = vel.get(splat_center_ellipse_rounded)
#	print "velocity_center.x = ",velocity_center.x
#	print "velocity_center.y = ",velocity_center.y
#	print "velocity_center.z = ",velocity_center.z

	solvePressure3_part1(flags=flags, vel=vel, pressure=pressure, openBound='Y')
	solvePressure3_part2(flags=flags, vel=vel, pressure=pressure)
	setWallBcs(flags=flags, vel=vel)
	setInflowBcs(vel, dire='y', value=vec3(0.0,0.3,0.0))

	splat_center_ellipse_rounded.x = int(splat_center_ellipse.x+0.5)
	splat_center_ellipse_rounded.y = int(splat_center_ellipse.y+0.5)
	splat_center_ellipse_rounded.z = int(splat_center_ellipse.z+0.5)
	velocity_center = vel.get(splat_center_ellipse_rounded)
	print "velocity_center.x = ",velocity_center.x
	print "velocity_center.y = ",velocity_center.y
	print "velocity_center.z = ",velocity_center.z

	timings.display()
	s.step()
#	gui.screenshot( '/home/tushar/manta_mikey_1/manta/build/pictures/december_3/6/density/%03d_dens.png' % t )
