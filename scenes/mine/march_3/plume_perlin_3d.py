#
# Simple example scene (hello world)
# Simulation of a buoyant smoke density plume

#import pdb; pdb.set_trace()

from manta import *

# solver params
res = 64
gs_x = res
gs_y = res*1.5
gs_z = res
gs = vec3(gs_x,gs_y,gs_z)
#gs = vec3(res,1.5*res,res)
s = FluidSolver(name='main', gridSize = gs)
s.timestep = 1.0
timings = Timings()

# prepare grids
flags = s.create(FlagGrid)
vel = s.create(MACGrid)
density = s.create(RealGrid)
pressure = s.create(RealGrid)

pressure_template_perlin_x = s.create(RealGrid)
pressure_template_perlin_y = s.create(RealGrid)
#pressure_template_perlin = s.create(RealGrid)

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

#source = s.create(Cylinder, center=gs*vec3(0.5,0.1,0.5), radius=res*0.14, z=gs*vec3(0, 0.02, 0))
source = s.create(Cylinder, center=gs*vec3(0.5,0.2,0.5), radius=res*0.25, z=gs*vec3(0, 0.05, 0))
	
#main loop
for t in range(250):
	if t<100:
		densityInflow(flags=flags, density=density, noise=noise, shape=source, scale=1, sigma=0.5)
		
	# optionally, enforce inflow velocity
#	source.applyToGrid(grid=vel, value=velInflow)

	advectSemiLagrange_time_fraction(flags=flags, vel=vel, grid=vel, order=2, time_fraction = 0.5)
	setWallBcs(flags=flags, vel=vel)

	addBuoyancy(density=density, vel=vel, gravity=vec3(0,-6e-4,0), flags=flags)
	
#	solvePressure(flags=flags, vel=vel, pressure=pressure, useResNorm=True)
#	setWallBcs(flags=flags, vel=vel)
	solvePressure3_part1(flags=flags, vel=vel, pressure=pressure, openBound='Y')
	solvePressure3_part2(flags=flags, vel=vel, pressure=pressure)
	setWallBcs(flags=flags, vel=vel)

	advectSemiLagrange(flags=flags, vel=vel, grid=density, order=2)    

	create_perlin_template_3_3D_kernel(pressure_template_perlin_x,multiplier=9.0,res_x=gs_x,res_y=gs_y,res_z=gs_z,levels=6,time=t,frequency_x=21,frequency_y=7,frequency_z=10,translation_x=-0.05, translation_y=-0.05, translation_z=-0.05)
	create_perlin_template_3_3D_kernel(pressure_template_perlin_y,multiplier=9.0,res_x=gs_x,res_y=gs_y,res_z=gs_z,levels=6,time=t,frequency_x=7,frequency_y=21,frequency_z=10,translation_x=-0.05, translation_y=-0.05, translation_z=-0.05)

	if(t%2==0):
		solvePressure3_part2(flags=flags, vel=vel, pressure=pressure_template_perlin_x)
		solvePressure3_part2(flags=flags, vel=vel, pressure=pressure_template_perlin_y)
		setWallBcs(flags=flags, vel=vel)
	else:
		solvePressure3_part2(flags=flags, vel=vel, pressure=pressure_template_perlin_y)
		solvePressure3_part2(flags=flags, vel=vel, pressure=pressure_template_perlin_x)
		setWallBcs(flags=flags, vel=vel)

	advectSemiLagrange_time_fraction(flags=flags, vel=vel, grid=vel, order=2, time_fraction = 0.5)
	setWallBcs(flags=flags, vel=vel)

#	density.save('/media/tushar/TOSHIBA EXT/feb_26/3/den%04d.vol' % t)
	
	timings.display()
	s.step()

	gui.screenshot( '/home/tushar/manta_mikey_1/manta/build/pictures/february_27/images/5/%03d.png' % t )
