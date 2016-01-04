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
flag_test = s.create(FlagGrid)
vel = s.create(MACGrid)
vel_test = s.create(MACGrid)
density = s.create(RealGrid)
pressure = s.create(RealGrid)
pressure_modification = s.create(RealGrid)

flags.initDomain()
flags.fillGrid()

if (GUI):
	gui = Gui()
	gui.show()
	#gui.pause()

#source = s.create(Cylinder, center=gs*vec3(0.5,0.1,0.5), radius=res*0.14, z=gs*vec3(0, 0.02, 0))
source = s.create(Cylinder, center=gs*vec3(0.5,0.1,0.5), radius=res*0.14, z=gs*vec3(0, 0.02, 0))

# position/center , radius and sigma for the centre of pressure modifier (Gaussian splat/kernel)
splat_center = vec3(32,24,0.5)
splat_radius = 6
#splat_center.x = 64*0.7
#splat_center.y = 64*0.5
#splat_center.z = 64*0.5
splat_sigma = 1.0
	
#main loop
for t in range(400):
	if t<300:
		source.applyToGrid(grid=density, value=1)
		
#	advectSemiLagrange(flags=flags, vel=vel, grid=density, order=2)    
#	advectSemiLagrange(flags=flags, vel=vel, grid=vel,     order=2 )
	
#	setWallBcs(flags=flags, vel=vel)    
#	addBuoyancy(density=density, vel=vel, gravity=vec3(0,-4e-3,0), flags=flags)
	
#	solvePressure(flags=flags, vel=vel, pressure=pressure, openBound='Y')
#        solvePressure3_part1(flags=flags, vel=vel, pressure=pressure, openBound='Y')
        create_pressure_modifier(pressure_modification , splat_radius , splat_center , splat_sigma)
        modify_pressure(pressure,pressure_modification)
#        solvePressure3_part2(flags=flags, vel=vel, pressure=pressure)
 
#	solvePressure1(flags=flags, vel=vel, pressure=pressure, openBound='Y')
#	solvePressure2(flags=flags, vel=vel, pressure=pressure, openBound='Y')


#	setWallBcs(flags=flags, vel=vel)
	
	timings.display()    
	s.step()

