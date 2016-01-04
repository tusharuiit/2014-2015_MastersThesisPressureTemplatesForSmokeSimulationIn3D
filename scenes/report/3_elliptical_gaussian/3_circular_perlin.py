#
# Simple example scene for a 2D simulation
# Simulation of a buoyant smoke density plume
#
from manta import *
#import manta

from time import sleep

# solver params
res = 128

gs_x = res
gs_y = res
gs = vec3(gs_x,gs_y,1)
s = Solver(name='main', gridSize = gs, dim=2)
s.timestep = 1.0
timings = Timings()

# prepare grids
flags = s.create(FlagGrid)
vel = s.create(MACGrid)
density = s.create(RealGrid)
pressure = s.create(RealGrid)

#This is the noise template that is being
pressure_template_perlin = s.create(RealGrid)
boundary_zero_smoother = s.create(RealGrid)

#	This is for perlin noise template
#	It has a maximum value of multiplier and minimum value of 0.0
file_name_template='/home/tushar/manta_mikey_1/manta/build/pictures/january_13/templates/4/'+str(gs_x)+'x'+str(gs_y)+'.png'
read_image_template(boundary_zero_smoother,multiplier=1.0,res_x=gs_x,res_y=gs_y,smoother_name=file_name_template)
create_static_perlin_template_general ( pressure_modification = pressure_template_perlin, multiplier = 9.0, res_x = gs_x, res_y = gs_y, frequency_changer_x = 8, frequency_changer_y = 8, time = 1 )
pressure_template_perlin.mult(boundary_zero_smoother)

flags.initDomain()
flags.fillGrid()

if (GUI):
	gui = Gui()
	gui.show()
	gui.pause()

#This is the source shape
source = s.create(Cylinder, center=gs*vec3(0.5,0.5,0.5), radius=res*0.48, z=gs*vec3(0, 0.48, 0))

#	This is the noise with central mean value as 1
source.applyRNG_binary_NoiseToGrid_seeded(grid=density, value=1, fraction_white=0.30, res_x = gs_x, res_y = gs_y)

#main loop
for t in range(400):

#	time range of application of pressure modification
	t1 = 1
	t2 = 301
	if ((t>=t1)and(t<=t2)):

#		This is velocity correction
		solvePressure3_part2(flags=flags, vel=vel, pressure=pressure_template_perlin)
		setWallBcs(flags=flags, vel=vel)

	advectSemiLagrange_time_fraction(flags=flags, vel=vel, grid=vel, order=2, time_fraction = 0.5)
#	advectSemiLagrange_time_fraction(flags=flags, vel=vel, grid=density, order=2, time_fraction = 0.5)
	setWallBcs(flags=flags, vel=vel)    

	solvePressure3_part1(flags=flags, vel=vel, pressure=pressure, openBound='Y')
	solvePressure3_part2(flags=flags, vel=vel, pressure=pressure)
	setWallBcs(flags=flags, vel=vel)

	advectSemiLagrange_time_fraction(flags=flags, vel=vel, grid=density, order=2, time_fraction = 1.0)
	advectSemiLagrange_time_fraction(flags=flags, vel=vel, grid=vel, order=2, time_fraction = 0.5)
	setWallBcs(flags=flags, vel=vel)

	timings.display()
	s.step()
	gui.screenshot( '/media/tushar/TOSHIBA EXT/latex/4_Perlin/1_circular_perlin/density/%03d.png' % t )
