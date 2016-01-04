#
# Simple example scene for a 2D simulation
# Simulation of a buoyant smoke density plume
#
from manta import *
#import manta

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

#	template
pressure_template_perlin = s.create(RealGrid)

flags.initDomain()
flags.fillGrid()

if (GUI):
	gui = Gui()
	gui.show()
	#gui.pause()

source = s.create(Cylinder, center=gs*vec3(0.5,0.1,0.5), radius=res*0.14, z=gs*vec3(0, 0.02, 0))

#	This is for perlin noise template
#	It has a maximum value of multiplier and minimum value of 0.0
create_perlin_template(pressure_template_perlin,multiplier=2.0,res_x=gs_x,res_y=gs_y,frequency_changer=16)

#main loop
for t in range(400):
	if t<300:
		source.applyToGrid(grid=density, value=1)

	advectSemiLagrange(flags=flags, vel=vel, grid=density, order=2)    
	advectSemiLagrange(flags=flags, vel=vel, grid=vel,     order=2)
	setWallBcs(flags=flags, vel=vel)    

	addBuoyancy(density=density, vel=vel, gravity=vec3(0,-4e-3,0), flags=flags)

	if((t%5)==0):
		solvePressure3_part1(flags=flags, vel=vel, pressure=pressure, openBound='Y')
		pressure_template_perlin.multConst(1)
		pressure.mult(pressure_template_perlin)
		solvePressure3_part2(flags=flags, vel=vel, pressure=pressure)
		setWallBcs(flags=flags, vel=vel)
	else:
		solvePressure3_part1(flags=flags, vel=vel, pressure=pressure, openBound='Y')
		solvePressure3_part2(flags=flags, vel=vel, pressure=pressure)
		setWallBcs(flags=flags, vel=vel)

	timings.display()
	s.step()
#	gui.screenshot( './pictures/november_3/1.2/%03d_density.png' % t )
