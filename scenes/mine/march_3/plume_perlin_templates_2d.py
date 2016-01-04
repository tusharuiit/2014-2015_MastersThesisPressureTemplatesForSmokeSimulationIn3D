#
# Simple example scene for a 2D simulation
# Simulation of a buoyant smoke density plume
#
from manta import *
#import manta

from time import sleep

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

#This is the noise template that is being
pressure_template_perlin_x = s.create(RealGrid)
pressure_template_perlin_y = s.create(RealGrid)
pressure_template_perlin_x_y = s.create(RealGrid)
pressure_template_perlin_rotate = s.create(RealGrid)
pressure_template_perlin_linear = s.create(RealGrid)
pressure_template_perlin_linear_x = s.create(RealGrid)
pressure_template_perlin_linear_y = s.create(RealGrid)
pressure_template_perlin_linear_x_y = s.create(RealGrid)
boundary_zero_smoother = s.create(RealGrid)

# noise field
noise = s.create(NoiseField, loadFromFile=True)
noise.posScale = vec3(45)
noise.clamp = True
noise.clampNeg = 0
noise.clampPos = 1
noise.valScale = 1
noise.valOffset = 0.75
noise.timeAnim = 0.2

#	This is for perlin noise template
#	It has a maximum value of multiplier and minimum value of 0.0
create_image_template(boundary_zero_smoother,multiplier=1.0,res_x=gs_x,res_y=gs_y)

flags.initDomain()
flags.fillGrid()

if (GUI):
	gui = Gui()
	gui.show()
	gui.pause()

#	This is the source shape
source = s.create(Cylinder, center=gs*vec3(0.5,0.2,0.5), radius=res*0.2, z=gs*vec3(0, 0.025, 0))
#source = s.create(Cylinder, center=gs*vec3(0.5,0.1,0.5), radius=res*0.14, z=gs*vec3(0, 0.02, 0))

##	This is the source shape
#source = s.create(Cylinder, center=gs*vec3(0.5,0.5,0.5), radius=res*0.48, z=gs*vec3(0, 0.48, 0))
##	This is the noise with central mean value as 1
#source.applyRNG_binary_NoiseToGrid_seeded(grid=density, value=1, fraction_white=0.20, res_x = gs.x, res_y = gs.y)

#	Time range of application of the template
t1 = 1
t2 = 225

#main loop
for t in range(250):

	if t<100:
		source.applyToGrid(grid=density, value=1)
#	if t<250:
#		densityInflow(flags=flags, density=density, noise=noise, shape=source, scale=1, sigma=0.5)

#	time range of application of pressure modification
	if (t==(t1)):
		gui.pause()
	if (t==(t2)):
		gui.pause()
	if ((t>=t1)and(t<t2)):

#		create_perlin_template_24(pressure_template_perlin,multiplier=8.0,res_x=gs_x,res_y=gs_y,time=t,start_x=5.0,start_y=15.0,time_steps=60)
		create_perlin_template_25(pressure_template_perlin_x,multiplier=3.0,res_x=gs_x,res_y=gs_y,time=t,frequency_x=3.0,frequency_y=9.0,sign_translate_x = -0.1, sign_translate_y = -0.1)
		create_perlin_template_25(pressure_template_perlin_y,multiplier=5.0,res_x=gs_x,res_y=gs_y,time=t,frequency_x=15.0,frequency_y=5.0,sign_translate_x = -0.1, sign_translate_y = -0.1)
		pressure_template_perlin_x_y.multConst(0)
		pressure_template_perlin_x_y.add(pressure_template_perlin_x)
		pressure_template_perlin_x_y.add(pressure_template_perlin_y)

		create_perlin_template_22(pressure_template_perlin_rotate, multiplier = 5.0, res_x=gs_x, res_y=gs_y, time = t, x_factor=7.0, y_factor=21.0)

		create_perlin_template_17(pressure_modification=pressure_template_perlin_linear, multiplier = 5.0, res_x=gs_x, res_y=gs_y, time=t, start_x=6.0, start_y=18.0, time_steps=30)

		create_perlin_template_17(pressure_modification=pressure_template_perlin_linear_x, multiplier = 3.0, res_x=gs_x, res_y=gs_y, time=t, start_x=4.0, start_y=12.0, time_steps=30)
		create_perlin_template_17(pressure_modification=pressure_template_perlin_linear_y, multiplier = 3.0, res_x=gs_x, res_y=gs_y, time=t, start_x=12.0, start_y=4.0, time_steps=30)
		pressure_template_perlin_linear_x_y.multConst(0)
		pressure_template_perlin_linear_x_y.add(pressure_template_perlin_linear_x)
		pressure_template_perlin_linear_x_y.add(pressure_template_perlin_linear_y)

		pressure_template_perlin_x.mult(boundary_zero_smoother)
		pressure_template_perlin_y.mult(boundary_zero_smoother)
		pressure_template_perlin_rotate.mult(boundary_zero_smoother)
		pressure_template_perlin_linear.mult(boundary_zero_smoother)
		pressure_template_perlin_x_y.mult(boundary_zero_smoother)
		pressure_template_perlin_linear_x_y.mult(boundary_zero_smoother)

#	This is velocity correction
#		apply_template_localized( flags=flags, vel=vel, pressure_template=pressure_template_perlin_x, density=density )
#		apply_template_localized( flags=flags, vel=vel, pressure_template=pressure_template_perlin_y, density=density )
		apply_template_localized(flags=flags, vel=vel, pressure_template=pressure_template_perlin_x_y, density=density)
#		apply_template_localized(flags=flags, vel=vel, pressure_template=pressure_template_perlin_rotate, density=density)
#		apply_template_localized(flags=flags, vel=vel, pressure_template=pressure_template_perlin_linear, density=density)
#		apply_template_localized( flags=flags, vel=vel, pressure_template=pressure_template_perlin_linear_x_y, density=density )
		setWallBcs(flags=flags, vel=vel)
#		solvePressure3_part2(flags=flags, vel=vel, pressure=pressure_template_perlin_linear_x_y)
#		setWallBcs(flags=flags, vel=vel)

#	advectSemiLagrange_time_fraction(flags=flags, vel=vel, grid=vel, order=2, time_fraction = 0.5)
##	advectSemiLagrange_time_fraction(flags=flags, vel=vel, grid=density, order=2, time_fraction = 0.5)
#	setWallBcs(flags=flags, vel=vel)

	addBuoyancy(density=density, vel=vel, gravity=vec3(0,-6e-4,0), flags=flags)

	solvePressure3_part1(flags=flags, vel=vel, pressure=pressure, openBound='Y')
	solvePressure3_part2(flags=flags, vel=vel, pressure=pressure)
	setWallBcs(flags=flags, vel=vel)

	advectSemiLagrange_time_fraction(flags=flags, vel=vel, grid=density, order=2, time_fraction = 1.0)
	advectSemiLagrange_time_fraction(flags=flags, vel=vel, grid=vel, order=2, time_fraction = 1.5)
	setWallBcs(flags=flags, vel=vel)

	timings.display()
	s.step()
#	gui.screenshot( '/home/tushar/manta_mikey_1/manta/build/pictures/march_3/images/5/velocity/%03d.png' % t )
