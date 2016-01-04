#
# Simple example scene for a 2D simulation
# Simulation of a buoyant smoke density plume
#
from manta import *
#import manta

# solver params
res = 64

gs = vec3(res,res,1)
s = Solver(name='main', gridSize = gs, dim=2)
s.timestep = 1.0
timings = Timings()

# prepare grids
flags = s.create(FlagGrid)
vel = s.create(MACGrid)
density = s.create(RealGrid)
pressure = s.create(RealGrid)

flags.initDomain()
flags.fillGrid()

if (GUI):
	gui = Gui()
	gui.show()
	gui.pause()

source = s.create(Cylinder, center=gs*vec3(0.5,0.1,0.5), radius=res*0.14, z=gs*vec3(0, 0.02, 0))

#main loop
for t in range(400):
	if t<300:
		source.applyToGrid(grid=density, value=1)

#	advectSemiLagrange(flags=flags, vel=vel, grid=vel,     order=2)

	advectiontimes = 50
	for times in range(advectiontimes):
		advectSemiLagrange_time_fraction(flags=flags, vel=vel, grid=vel, order=2,time_fraction = (1.0/advectiontimes))
		setWallBcs(flags=flags, vel=vel)

	addBuoyancy(density=density, vel=vel, gravity=vec3(0,-4e-3,0), flags=flags)

	solvePressure(flags=flags, vel=vel, pressure=pressure, openBound='Y')
	setWallBcs(flags=flags, vel=vel)
	advectSemiLagrange(flags=flags, vel=vel, grid=density, order=2)    

	timings.display()    
	s.step()
	gui.screenshot( '/home/tushar/manta_mikey_1/manta/build/pictures/december_6_7_8/multi_step_advection/plume_2d/grid_res_64x64/50/%03d_dens.png' % t )
