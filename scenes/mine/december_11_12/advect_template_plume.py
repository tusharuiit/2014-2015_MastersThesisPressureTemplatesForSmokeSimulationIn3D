#
# Simple example scene for a 2D simulation
# Simulation of a noisy smoke density frame
#
from manta import *
#import manta

# solver params
res_x = 128
res_y = 128

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

#This is the source for the advection of the pressure modification
source = s.create(Cylinder, center=gs*vec3(0.5,0.1,0.5), radius=res*0.14, z=gs*vec3(0, 0.02, 0))

# This is for elliptical gaussian
splat_center_ellipse = gs*vec3(0.5,0.5,0.5)
splat_sigma_x = 4.0
splat_sigma_y = 4.0
splat_radius_x = 4
splat_radius_y = 4

# This is for advection splat
advection_splat_center = gs*vec3(0.5,0.5,0.5)
advection_splat_radius = 2
advection_splat_sigma = 1.0

create_pressure_modifier_elliptical_gaussian(pressure_modification_elliptical_gaussian , splat_radius_x , splat_radius_y , splat_center_ellipse , splat_sigma_x , splat_sigma_y)

#main loop
for t in range(400):
	if t<300:
		source.applyToGrid(grid=density, value=1)

	advectSemiLagrange(flags=flags, vel=vel, grid=density, order=2)    
	advectSemiLagrange(flags=flags, vel=vel, grid=vel,     order=2)
	setWallBcs(flags=flags, vel=vel)    

	addBuoyancy(density=density, vel=vel, gravity=vec3(0,-4e-3,0), flags=flags)
	solvePressure3_part1(flags=flags, vel=vel, pressure=pressure, openBound='Y')
#	time range of application of pressure modification
	t1 = 5
	t2 = 95
	if (t==(t1 - 2)):
		gui.pause()
	if (t==(t2 - 2)):
		gui.pause()
	pressure_before.copyFrom(pressure)

	if (t>t1)and(t<t2):
		advectSemiLagrange(flags=flags, vel=vel, grid=pressure_modification_elliptical_gaussian, order=2)

	solvePressure3_part2(flags=flags, vel=vel, pressure=pressure)
	setWallBcs(flags=flags, vel=vel)

	timings.display()
	s.step()
#	gui.screenshot( './pictures/point_1_time_step/grid_res_128/magnify_4/scale_1.6/velocity_only/%03d_velocity_only.png' % t )