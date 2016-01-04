#
# Simple example scene for a 2D simulation
# Simulation of a buoyant smoke density plume
#
from manta import *
#import manta

# solver params
res = 128

gs = vec3(res,res,1)
s = Solver(name='main', gridSize = gs, dim=2)
s.timestep = 1.0
timings = Timings()

# prepare grids
flags = s.create(FlagGrid)
#flag_test = s.create(FlagGrid)
vel = s.create(MACGrid)
density = s.create(RealGrid)
pressure = s.create(RealGrid)

#Create the circular Gaussian template here
pressure_template = s.create(RealGrid)

flags.initDomain()
flags.fillGrid()

if (GUI):
	gui = Gui()
	gui.show()
	gui.pause()

#This is the original source
#source = s.create(Cylinder, center=gs*vec3(0.5,0.1,0.5), radius=res*0.14, z=gs*vec3(0, 0.02, 0))
source = s.create(Cylinder, center=gs*vec3(0.5,0.5,0.5), radius=res*0.48, z=gs*vec3(0, 0.48, 0))

# position/center , radiii and sigmas for the elliptical Gaussian
center = gs*vec3(0.5,0.5,0.5)
sigma_x = 16.0
sigma_y = 2.0
radius_x = (res*0.5) - 4.0
radius_y = (res*0.25) - 1.0

##	This is the random noise
source.applyRNG_binary_NoiseToGrid_seeded(grid=density, value=1, fraction_white=0.30, res_x = gs.x, res_y = gs.y)

t1 = 1
t2 = 91

#main loop
for t in range(400):

	advectSemiLagrange(flags=flags, vel=vel, grid=density, order=2)
	advectSemiLagrange(flags=flags, vel=vel, grid=vel,     order=2)
	setWallBcs(flags=flags, vel=vel)    

	solvePressure3_part1(flags=flags, vel=vel, pressure=pressure, openBound='Y')
	solvePressure3_part2(flags=flags, vel=vel, pressure=pressure)
	setWallBcs(flags=flags, vel=vel)

#	This is elliptical Gaussian
	create_elliptical_gaussian( pressure_modification = pressure_template , radius_x = radius_x, radius_y = radius_y, splat_center = center, sigma_x = sigma_x, sigma_y = sigma_y, multiplier = 4.0)

	if (t>=t1)and(t<=t2):
		solvePressure3_part2(flags=flags, vel=vel, pressure=pressure_template)
		setWallBcs(flags=flags, vel=vel)

	timings.display()
	s.step()

#	gui.screenshot( '/media/tushar/TOSHIBA EXT/latex/3/template_velocity_1/%03d.png' % t )
