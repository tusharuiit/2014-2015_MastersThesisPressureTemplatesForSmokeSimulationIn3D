#
# Simple example scene for a 2D simulation
# Simulation of a buoyant smoke density plume
#
from manta import *
#import manta

from time import sleep

#	solver params
res = 128

gs_x = res
gs_y = res
gs = vec3(gs_x,gs_y,1)
s = Solver(name='main', gridSize = gs, dim=2)
s.timestep = 1.0
timings = Timings()

#	prepare grids
flags = s.create(FlagGrid)
#	flag_test = s.create(FlagGrid)
vel = s.create(MACGrid)
#	vel_test = s.create(MACGrid)
density = s.create(RealGrid)
pressure = s.create(RealGrid)

#	template
pressure_template_perlin = s.create(RealGrid)

flags.initDomain()
flags.fillGrid()

if (GUI):
	gui = Gui()
	gui.show()
	gui.pause()

#This is the source shape
source = s.create(Cylinder, center=gs*vec3(0.5,0.5,0.5), radius=res*0.48, z=gs*vec3(0, 0.48, 0))

#	This is for perlin noise template
#	It has a maximum value of multiplier and minimum value of 0.0
create_perlin_template(pressure_template_perlin,multiplier=1.5,res_x=gs_x,res_y=gs_y,frequency_changer=16)

#	This is the noise with central mean value as 1
source.applyRNG_binary_NoiseToGrid_seeded(grid=density, value=1, fraction_white=0.20, res_x = gs_x, res_y = gs_y)

#main loop
for t in range(400):

#	ADVECTION
	advectSemiLagrange_time_fraction(flags=flags, vel=vel, grid=density, order=2, time_fraction = 1.0)
	advectSemiLagrange_time_fraction(flags=flags, vel=vel, grid=vel, order=2, time_fraction = 1.0)
	setWallBcs(flags=flags, vel=vel)

#	time range of application of pressure modification
	t1 = 5
	t2 = 35

	if (t>t1)and(t<t2):
	#	PPE
		if(t%2 == 0):
			solvePressure3_part1(flags=flags, vel=vel, pressure=pressure, openBound='Y')
			solvePressure3_part2(flags=flags, vel=vel, pressure=pressure)
			setWallBcs(flags=flags, vel=vel)
	#	PPE
	if(t>=t2):
		solvePressure3_part1(flags=flags, vel=vel, pressure=pressure, openBound='Y')
		solvePressure3_part2(flags=flags, vel=vel, pressure=pressure)
		setWallBcs(flags=flags, vel=vel)

	if (t==(t1-2)):
		gui.pause()
	if (t==(t2-2)):
		gui.pause()
	if (t>t1)and(t<t2):
#		This is velocity correction/TEMPLATE APPLICATION
		solvePressure3_part2(flags=flags, vel=vel, pressure=pressure_template_perlin)
		setWallBcs(flags=flags, vel=vel)

	timings.display()
	s.step()
#	gui.screenshot( '/home/tushar/manta_mikey_1/manta/build/pictures/january_8/PPE_chosen/2/%03d.png' % t )
