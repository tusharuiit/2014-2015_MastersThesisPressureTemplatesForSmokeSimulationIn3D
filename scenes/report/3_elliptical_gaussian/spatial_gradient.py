#
# Simple example scene for a 2D simulation
# Simulation of a buoyant smoke density plume
#
from manta import *
#import manta

from time import sleep

# solver params
res = 128

gs = vec3(res,res,1)
s = Solver(name='main', gridSize = gs, dim=2)
s.timestep = 1.0
timings = Timings()

# prepare grids
flags = s.create(FlagGrid)
vel = s.create(MACGrid)
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
splat_sigma_x = 16.0
splat_sigma_y = 2.0
splat_radius_x = (res*0.5) - 4.0
splat_radius_y = (res*0.25) - 1.0

#	This is the noise with central mean value as 1
source.applyRNG_binary_NoiseToGrid_seeded(grid=density, value=1, fraction_white=0.30 ,res_x = gs.x, res_y = gs.y)

#main loop
for t in range(400):
#	if t<300:
#		source.applyToGrid(grid=density, value=1)

	advectSemiLagrange(flags=flags, vel=vel, grid=density, order=2)
	advectSemiLagrange(flags=flags, vel=vel, grid=vel,     order=2)

	setWallBcs(flags=flags, vel=vel)    

	solvePressure3_part1(flags=flags, vel=vel, pressure=pressure, openBound='Y')
	solvePressure3_part2(flags=flags, vel=vel, pressure=pressure)

	setWallBcs(flags=flags, vel=vel)


#	Creating this at all time steps for visualization of the pressure template
#	This is elliptical Gaussian
	create_pressure_modifier_elliptical_gaussian_with_multiplier(pressure_modification_elliptical_gaussian , splat_radius_x , splat_radius_y , splat_center_ellipse , splat_sigma_x , splat_sigma_y , multiplier = 0.5, lower_factor = 0.0, higher_factor = 5.0)

#	time range of application of pressure modification
	t1 = 1
	t2 = 151

	if (t>=t1)and(t<=t2):
#		This is velocity correction
		solvePressure3_part2(flags=flags, vel=vel, pressure=pressure_modification_elliptical_gaussian)

		setWallBcs(flags=flags, vel=vel)

	timings.display()

	s.step()
	gui.screenshot( '/media/tushar/TOSHIBA EXT/latex/3_elliptical_gaussian/7_spatial_gradient/%03d.png' % t )
