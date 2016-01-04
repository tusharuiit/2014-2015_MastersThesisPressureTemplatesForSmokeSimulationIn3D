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
#flag_test = s.create(FlagGrid)
vel = s.create(MACGrid)
#vel_test = s.create(MACGrid)
density = s.create(RealGrid)
pressure = s.create(RealGrid)

#This is the pressure modification being tested on all the points
velocity_modification_plain_horizontal = s.create(RealGrid)

flags.initDomain()
flags.fillGrid()

if (GUI):
	gui = Gui()
	gui.show()
	gui.pause()

#This is the original source
#source = s.create(Cylinder, center=gs*vec3(0.5,0.1,0.5), radius=res*0.14, z=gs*vec3(0, 0.02, 0))
#This is the source for point density in all time steps
source = s.create(Cylinder, center=gs*vec3(0.5,0.5,0.5), radius=res*0.45, z=gs*vec3(0, 0.4, 0))

#	This is the density of value 1 applied at a single point
#source.applydensity_at_pointToGrid(grid=density, position = (gs*vec3(0.5,0.5,0.5)), value=1)
#source.applydensity_at_pointToGrid(grid=density, position = (gs*vec3(0.5,0.5,0.5)), value=1)
disp_y = 0.0
disp_x = 0.0

position_displaced = ((gs*vec3(0.5 , 0.5 , 0.5)) + vec3(disp_x,disp_y,0.0))
source.applydensity_at_pointToGrid(grid=density, position=position_displaced , value=1)

#main loop
for t in range(400):
#	if t<300:
##		source.applyToGrid(grid=density, value=1)

#       Inflow thingy may be put/imposed here , just before advectSemiLagrange
#	if t<200:
#		source.applyToGrid(grid=vel, value=velInflow)

	advectSemiLagrange(flags=flags, vel=vel, grid=density, order=2)    
	advectSemiLagrange(flags=flags, vel=vel, grid=vel,     order=2)

	setWallBcs(flags=flags, vel=vel)    

#	if t<200:
#		addBuoyancy(density=density, vel=vel, gravity=vec3(0,-4e-3,0), flags=flags)

#	time range of application of velocity template
	t1 = 80
	if (t==77):
		gui.pause()
#	if (t==87):
#		gui.pause()

	if (t==t1):
		value = vec3(0.5,0.0,0.0)
#		create_velocity_modifier_plane_horizontal(flags, vel_modifier, value)
		modify_velocity_plain_horizontal(flags, vel, value)

#	if (t>=(t1+5)):
#		solvePressure3_part1(flags=flags, vel=vel, pressure=pressure, openBound='Y')
#		solvePressure3_part2(flags=flags, vel=vel, pressure=pressure)

	setWallBcs(flags=flags, vel=vel)

	timings.display()

	s.step()
#	gui.screenshot( './pictures/point_1_time_step/grid_res_128/magnify_4/scale_1.6/velocity_only/%03d_velocity_only.png' % t )
