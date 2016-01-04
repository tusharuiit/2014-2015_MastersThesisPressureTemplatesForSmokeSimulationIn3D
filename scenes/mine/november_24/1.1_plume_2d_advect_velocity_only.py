#Schema (with plume flow initialized):
#Advect(Vel with Vel)
#addBuoyancy - may be
#P = CG(Vel)
#Vel' = Vel - Grad(P)
#Advect(density)
#Create P_modified
#Vel'' = Vel' - Grad(P_modified)
#Vel = Vel''

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
vel = s.create(MACGrid)
density = s.create(RealGrid)
pressure = s.create(RealGrid)

#This is a dummy grid created for the velocity-only visualization in the GUI.Velocity is MAC Grid
velocity_only = s.create(RealGrid)

#This is the pressure just after CG solve
pressure_before = s.create(RealGrid)

pressure_modification_elliptical_gaussian = s.create(RealGrid)

flags.initDomain()
flags.fillGrid()

if (GUI):
	gui = Gui()
	gui.show()
	#gui.pause()

source = s.create(Cylinder, center=gs*vec3(0.5,0.1,0.5), radius=res*0.14, z=gs*vec3(0, 0.02, 0))

# This is for elliptical gaussian
splat_center_ellipse = gs*vec3(0.5,0.5,0.5)
splat_sigma_x = 16.0
splat_sigma_y = 2.0
splat_radius_x = (res*0.5) - 4.0
splat_radius_y = (res*0.25) - 1.0

#main loop
for t in range(400):
	if t<300:
		source.applyToGrid(grid=density, value=1)

	advectSemiLagrange(flags=flags, vel=vel, grid=vel,     order=2)

	setWallBcs(flags=flags, vel=vel)    

	addBuoyancy(density=density, vel=vel, gravity=vec3(0,-4e-3,0), flags=flags)

#	solvePressure(flags=flags, vel=vel, pressure=pressure, openBound='Y')

	solvePressure3_part1(flags=flags, vel=vel, pressure=pressure, openBound='Y')

	solvePressure3_part2(flags=flags, vel=vel, pressure=pressure)

	setWallBcs(flags=flags, vel=vel)

	advectSemiLagrange(flags=flags, vel=vel, grid=density, order=2)

#	Creating this at all time steps for visualization of the pressure template
#	This is elliptical Gaussian
	create_pressure_modifier_elliptical_gaussian_with_multiplier(pressure_modification_elliptical_gaussian , splat_radius_x , splat_radius_y , splat_center_ellipse , splat_sigma_x , splat_sigma_y , multiplier = 2.0)

#	time range of application of pressure modification
	t1 = 60
	t2 = 150
	if (t==(t1 - 2)):
		gui.pause()
	if (t==(t2 - 2)):
		gui.pause()

	if (t>t1)and(t<t2):

#		modify_pressure_constant(pressure,pressure_modification_elliptical_gaussian,multiplier=4)

#		This is velocity correction
		solvePressure3_part2(flags=flags, vel=vel, pressure=pressure_modification_elliptical_gaussian)

		setWallBcs(flags=flags, vel=vel)

#	pressure_before.copyFrom(pressure)

	timings.display()    
	s.step()
	gui.screenshot( './pictures/november_3/1.1/%03d_density.png' % t )
