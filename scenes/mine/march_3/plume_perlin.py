from manta import *

# solver params
res = 64
gs_x = res
gs_y = res*1.5
gs_z = res
gs = vec3(gs_x,gs_y,gs_z)
s = FluidSolver(name='main', gridSize = gs)
s.timestep = 1.0
timings = Timings()

# prepare grids
flags = s.create(FlagGrid)
vel = s.create(MACGrid)
density = s.create(RealGrid)
pressure = s.create(RealGrid)

pressure_template_perlin = s.create(RealGrid)

flags.initDomain()
flags.fillGrid()

if (GUI):
	gui = Gui()
	gui.show()

source = s.create(Cylinder, center=gs*vec3(0.5,0.1,0.5), radius=res*0.14, z=gs*vec3(0, 0.02, 0))

#main loop
for t in range(250):
	if t<100:
		densityInflow(flags=flags, density=density, noise=noise, shape=source, scale=1, sigma=0.5)
		
	advectSemiLagrange_time_fraction(flags=flags, vel=vel, grid=density, order=2, time_fraction = 1.0)
	advectSemiLagrange_time_fraction(flags=flags, vel=vel, grid=vel, order=2, time_fraction = 0.5)
	setWallBcs(flags=flags, vel=vel)

	if t<time_max:
		create_perlin_template_3_3D_kernel(pressure_template_perlin,multiplier=6.0,res_x=gs_x,res_y=gs_y,res_z=gs_z,levels=6,time=t,frequency_x=3,frequency_y=9,frequency_z=3)
		solvePressure3_part2(flags=flags, vel=vel, pressure=pressure_template_perlin)
		setWallBcs(flags=flags, vel=vel)

	advectSemiLagrange_time_fraction(flags=flags, vel=vel, grid=vel, order=2, time_fraction = 0.5)
	setWallBcs(flags=flags, vel=vel)

	addBuoyancy(density=density, vel=vel, gravity=vec3(0,-6e-4,0), flags=flags)

	solvePressure3_part1(flags=flags, vel=vel, pressure=pressure, openBound='Y')
	solvePressure3_part2(flags=flags, vel=vel, pressure=pressure)
	setWallBcs(flags=flags, vel=vel)

	timings.display()
	s.step()
