#
# Simple example scene for a 2D simulation
# Simulation of a buoyant smoke density plume
#
from manta import *
#import manta

from time import sleep

# solver params
res = 64

gs_x = res
gs_y = res
gs_z = res
gs = vec3(gs_x,gs_y,gs_z)
#s = Solver(name='main', gridSize = gs, dim=2)
s = Solver(name='main', gridSize = gs, dim=3)

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
pressure_template_perlin = s.create(RealGrid)
#boundary_zero_smoother = s.create(RealGrid)

#	This is for perlin noise template
#	It has a maximum value of multiplier and minimum value of 0.0
#create_boundary_smoother_3D_kernel(boundary_zero_smoother,multiplier=1.0,res_x=gs_x,res_y=gs_y,res_z=gs_z)

flags.initDomain()
flags.fillGrid()

if (GUI):
	gui = Gui()
	gui.show()
#	gui.pause()

#This is the source shape
#source = s.create(Cylinder, center=gs*vec3(0.5,0.5,0.5), radius=res*0.48, z=gs*vec3(0, 0.48, 0))

#	This is the noise with central mean value as 1
#source.applyRNG_binary_NoiseToGrid_seeded_3D_kernel_trial(grid=density, value=1, fraction_white=0.80, res_x = gs_x, res_y = gs_y, res_z = gs_z)
applyRNG_binary_NoiseToGrid_direct(grid=density, Real value=1.0, Real fraction_white=0.5, int res_x = 64, int res_y = 64, int res_z = 64)

gui.pause()

#main loop
for t in range(1):

#	print "HELLO WORLD"
#	time range of application of pressure modification
	t1 = 1
	t2 = 301
#	if (t==(t1)):
#		gui.pause()
#	if (t==(t2)):
#		gui.pause()
	if (t>=t1)and(t<t2):
		print "HELLO WORLD 3"
		create_perlin_template_3_3D_kernel(pressure_template_perlin,multiplier=12.0,res_x=gs_x,res_y=gs_y,levels=6,time=t)
#		pressure_template_perlin.mult(boundary_zero_smoother)
#		This is velocity correction
		solvePressure3_part2(flags=flags, vel=vel, pressure=pressure_template_perlin)
		setWallBcs(flags=flags, vel=vel)

	advectSemiLagrange_time_fraction(flags=flags, vel=vel, grid=vel, order=2, time_fraction = 0.5)
	setWallBcs(flags=flags, vel=vel)    

	solvePressure3_part1(flags=flags, vel=vel, pressure=pressure, openBound='Y')
	solvePressure3_part2(flags=flags, vel=vel, pressure=pressure)
	setWallBcs(flags=flags, vel=vel)

	advectSemiLagrange_time_fraction(flags=flags, vel=vel, grid=density, order=2, time_fraction = 1.0)
	advectSemiLagrange_time_fraction(flags=flags, vel=vel, grid=vel, order=2, time_fraction = 0.5)
	setWallBcs(flags=flags, vel=vel)

	timings.display()
	print "HELLO WORLD 1"
	s.step()
	print "HELLO WORLD 2"
#	gui.screenshot( '/home/tushar/manta_mikey_1/manta/build/pictures/january_25/images/9/%03d.png' % t )
