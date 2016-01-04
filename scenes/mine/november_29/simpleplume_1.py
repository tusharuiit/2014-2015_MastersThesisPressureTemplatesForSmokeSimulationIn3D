#
# Simple example scene (hello world)
# Simulation of a buoyant smoke density plume

#import pdb; pdb.set_trace()

from manta import *

# solver params
res = 64
gs = vec3(res,1.5*res,res)
s = FluidSolver(name='main', gridSize = gs)
s.timestep = 1.0
timings = Timings()

# prepare grids
flags = s.create(FlagGrid)
vel = s.create(MACGrid)
density = s.create(RealGrid)
pressure = s.create(RealGrid)

pressure_modification = s.create(RealGrid)

# noise field
noise = s.create(NoiseField, loadFromFile=True)
noise.posScale = vec3(45)
noise.clamp = True
noise.clampNeg = 0
noise.clampPos = 1
noise.valScale = 1
noise.valOffset = 0.75
noise.timeAnim = 0.2

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
a = 1.0/(2.0 * 16.0 * 16.0)
b = 1.0/(2.0 * 2.0 * 2.0)
c = 1.0/(2.0 * 2.0 * 2.0)

radius_x = int((res*0.5) - 4.0)
radius_y = int((res*0.25) - 1.0)
radius_z = int((res*0.25) - 1.0)

#	This is the noise with central mean value as 1
source.applyRNG_binary_NoiseToGrid_seeded_3D(grid=density, value=1, fraction_white=0.20 ,res=res)

create_pressure_modifier_elliptical_gaussian_with_multiplier_3D(pressure_modification, radius_x, radius_y, radius_z, center_ellipsoid, a, b, c, m, n, p , multiplier = 4.0, lower_factor = 1.0, higher_factor = 1.0)
	
#main loop
for t in range(250):
#	if t<100:
#		densityInflow(flags=flags, density=density, noise=noise, shape=source, scale=1, sigma=0.5)
		
	# optionally, enforce inflow velocity
#	source.applyToGrid(grid=vel, value=velInflow)

	advectSemiLagrange(flags=flags, vel=vel, grid=density, order=2)    
	advectSemiLagrange(flags=flags, vel=vel, grid=vel    , order=2, strength=1.0)
	
	setWallBcs(flags=flags, vel=vel)    
#	addBuoyancy(density=density, vel=vel, gravity=vec3(0,-6e-4,0), flags=flags)
	
#	solvePressure(flags=flags, vel=vel, pressure=pressure, useResNorm=True)
	solvePressure3_part1(flags=flags, vel=vel, pressure=pressure, openBound='Y')
	solvePressure3_part2(flags=flags, vel=vel, pressure=pressure)

	setWallBcs(flags=flags, vel=vel)
	#density.save('den%04d.uni' % t)
	
	t1 = 5
	t2 = 95
	if (t==(t1-2)):
		gui.pause()
	if (t==(t2-2)):
		gui.pause()

	if (t>t1)and(t<t2):
#		This is velocity correction
		solvePressure3_part2(flags=flags, vel=vel, pressure=pressure_modification)

		setWallBcs(flags=flags, vel=vel)

	timings.display()
	s.step()


