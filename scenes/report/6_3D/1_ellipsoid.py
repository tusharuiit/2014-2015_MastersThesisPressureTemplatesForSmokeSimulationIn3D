#
# Simple example scene (hello world)
# Simulation of a buoyant smoke density plume

#import pdb; pdb.set_trace()

from manta import *

# solver params
res = 64

gs_x = int(1.5*res)
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

pressure_modification = s.create(RealGrid)

flags.initDomain()
flags.fillGrid()

if (GUI):
	gui = Gui()
	gui.show()
	gui.pause()

source = s.create(Box, p0=gs*vec3(0.0,0.0,0.0), p1=gs*vec3(1.0,1.0,1.0))
#source = s.create(Cylinder, center=gs*vec3(0.5,0.5,0.5), radius=res*0.48, z=gs*vec3(0, 0.48, 0))

# This is for elliptical gaussian
# a m n
# m b p
# n p c
center_ellipsoid = gs*vec3(0.5,0.5,0.5)
m = 0.0
n = 0.0
p = 0.0
a = 1.0/(2.0 * pow(12.0,2.0))
b = 1.0/(2.0 * pow(2.0,2.0))
c = 1.0/(2.0 * pow(2.0,2.0))

radius_x = int((gs_x*0.5) - 4.0)
radius_y = int((gs_y*0.5) - 4.0)
radius_z = int((gs_z*0.5) - 4.0)

#	This is the noise with central mean value as 1
source.applyRNG_binary_NoiseToGrid_seeded_3D(grid=density, value=1, fraction_white=0.40, res_x = gs_x, res_y = gs_y, res_z = gs_z)

create_ellipsoidal_gaussian_3D(pressure_modification, radius_x, radius_y, radius_z, center_ellipsoid, a, b, c, m, n, p , multiplier = 3.5)

#	main loop
for t in range(400):

	advectSemiLagrange(flags=flags, vel=vel, grid=vel, order=2, strength=1.0)

	setWallBcs(flags=flags, vel=vel)    

	solvePressure3_part1(flags=flags, vel=vel, pressure=pressure, openBound='Y')
	solvePressure3_part2(flags=flags, vel=vel, pressure=pressure)

	setWallBcs(flags=flags, vel=vel)

	advectSemiLagrange(flags=flags, vel=vel, grid=density, order=2)    

#	density.save('/media/tushar/B19D-ABF3/latex1/6_3D_april_11/7_vol/2/%04d.vol' % t)

	t1 = 1
	t2 = 91

	if ((t>=t1)and(t<=t2)):
#	This is velocity correction
		solvePressure3_part2(flags=flags, vel=vel, pressure=pressure_modification)

		setWallBcs(flags=flags, vel=vel)

	timings.display()
	s.step()
#	gui.screenshot( '/media/tushar/B19D-ABF3/latex1/6_3D_april_11/7_vol/1/%03d.png' % t )	
