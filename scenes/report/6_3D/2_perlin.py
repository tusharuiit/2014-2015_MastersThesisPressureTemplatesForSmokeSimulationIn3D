#
# Simple example scene (hello world)
# Simulation of a buoyant smoke density plume

#import pdb; pdb.set_trace()

from manta import *

# solver params
res = 96

gs_x = int(res)
gs_y = int(res)
gs_z = int(res)

gs = vec3(gs_x,gs_y,gs_z)

s = FluidSolver(name='main', gridSize = gs)
s.timestep = 1.0
timings = Timings()

# prepare grids
flags = s.create(FlagGrid)
vel = s.create(MACGrid)
density = s.create(RealGrid)
pressure = s.create(RealGrid)

pressure_template = s.create(RealGrid)

flags.initDomain()
flags.fillGrid()

if (GUI):
	gui = Gui()
	gui.show()
	gui.pause()

source = s.create(Box, p0=gs*vec3(0.0,0.0,0.0), p1=gs*vec3(1.0,1.0,0.5))
#source = s.create(Box, p0=gs*vec3(0.0,0.0,0.0), p1=gs*vec3(1.0,1.0,0.6))
#source = s.create(Box, p0=gs*vec3(0.25,0.25,0.25), p1=gs*vec3(0.75,0.75,0.75))
#source = s.create(Box, p0=gs*vec3(0.0,0.0,0.0), p1=gs*vec3(1.0,1.0,1.0))
#source = s.create(Cylinder, center=gs*vec3(0.5,0.5,0.5), radius=res*0.48, z=gs*vec3(0, 0.48, 0))

#	This is the noise with central mean value as 1
source.applyRNG_binary_NoiseToGrid_seeded_3D(grid=density, value=1, fraction_white=0.05, res_x = gs_x, res_y = gs_y, res_z = gs_z)

#	main loop
for t in range(250):

	advectSemiLagrange(flags=flags, vel=vel, grid=vel, order=2, strength=1.0)

	setWallBcs(flags=flags, vel=vel)    

	solvePressure3_part1(flags=flags, vel=vel, pressure=pressure, openBound='Y')
	solvePressure3_part2(flags=flags, vel=vel, pressure=pressure)

	setWallBcs(flags=flags, vel=vel)

	advectSemiLagrange(flags=flags, vel=vel, grid=density, order=2)    


	density.save('/media/tushar/TOSHIBA EXT/april_19/vol/2_perlin_3d/%04d.vol' % t)

	t1 = 1
	t2 = 101

	if ((t>=t1)and(t<=t2)):
#	This is for perlin noise template
#	It has a maximum value of multiplier and minimum value of 0.0

		create_perlin_template_x_y_abrupt_translation_3D ( pressure_modification = pressure_template, multiplier = 10.0, res_x = gs_x, res_y = gs_y, res_z = gs_z, frequency_changer_1 = 7, frequency_changer_2 = 25, time = t )

#	This is velocity correction
		solvePressure3_part2(flags=flags, vel=vel, pressure=pressure_template)

		setWallBcs(flags=flags, vel=vel)

	timings.display()
	s.step()
	gui.screenshot( '/media/tushar/TOSHIBA EXT/april_19/png/2_perlin_3d/%03d.png' % t )
