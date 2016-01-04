#
# Simple example scene for a 2D simulation
# Simulation of a buoyant smoke density plume
#
from manta import *
#import manta

from time import sleep
import math

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

#	This is the noise template that is being
pressure_template_perlin_x = s.create(RealGrid)
pressure_template_perlin_y = s.create(RealGrid)
pressure_template_perlin_x_y = s.create(RealGrid)

#This is the noise template that is being
boundary_zero_smoother = s.create(RealGrid)

#	This is the boundary smoother being created
file_name_template='/home/tushar/manta_mikey_1/manta/build/pictures/january_13/templates/4/'+str(gs_x)+'x'+str(gs_y)+'.png'
read_image_template(boundary_zero_smoother,multiplier=1.0,res_x=gs_x,res_y=gs_y,smoother_name=file_name_template)

flags.initDomain()
flags.fillGrid()

if (GUI):
	gui = Gui()
	gui.show()
	gui.pause()

#This is the source shape
source = s.create(Cylinder, center=gs*vec3(0.5,0.5,0.5), radius=res*0.48, z=gs*vec3(0, 0.48, 0))

#	This is the noise with central mean value as 1
source.applyRNG_binary_NoiseToGrid_seeded(grid=density, value=1, fraction_white=0.20, res_x = gs_x, res_y = gs_y)

#	Some more important parameters
multiplier = 10
frequency_x_bias_x = 8
frequency_x_bias_y = 32
frequency_y_bias_x = 32
frequency_y_bias_y = 8
folder_templates_number = str(1)

#	This is the noise template
pressure_template_perlin_x = s.create(RealGrid)
pressure_template_perlin_y = s.create(RealGrid)
pressure_template_perlin_x_y = s.create(RealGrid)

start_x = 0.0
start_y = 1.0
time_steps = 30

file_pointer = open("foo.txt", "wb")

#main loop
for t in range(400):

#	time range of application of pressure modification
	t1 = 1
	t2 = 301
	if (t>=t1)and(t<=t2):

#	This is for perlin noise template
		create_perlin_template_sinusoidal_interpolation ( pressure_template_perlin_x, multiplier=multiplier, res_x=gs_x, res_y=gs_y, frequency_x = frequency_x_bias_x, frequency_y = frequency_x_bias_y)
		create_perlin_template_sinusoidal_interpolation ( pressure_template_perlin_y, multiplier=multiplier, res_x=gs_x, res_y=gs_y, frequency_x = frequency_y_bias_x, frequency_y = frequency_y_bias_y)
#		print_perlin_template_sinusoidal_warping ( res_x=gs_x, res_y=gs_y, frequency_x_1 = frequency_y_bias_x, frequency_y_1 = frequency_y_bias_y, frequency_x_2 = frequency_x_bias_x, frequency_y_2 = frequency_x_bias_y, folder_templates_number=folder_templates_number, time =t, time_steps = time_steps )

#	Interpolate
#    We move from x:y = start_x:start_y to x:y start_y:start_y

		time_depender = ((2 * math.pi)/(30.0)) * ( t - 1 )
		linear_interpolate = ( math.sin(time_depender) + 1 )/2.0

		pressure_template_perlin_x.multConst(linear_interpolate)
		pressure_template_perlin_y.multConst(1.0 - linear_interpolate)

		pressure_template_perlin_x_y.multConst(0)
		pressure_template_perlin_x_y.add(pressure_template_perlin_x)
		pressure_template_perlin_x_y.add(pressure_template_perlin_y)
		pressure_template_perlin_x_y.mult(boundary_zero_smoother)

#		This is velocity correction
		solvePressure3_part2 ( flags = flags, vel = vel, pressure = pressure_template_perlin_x_y )
		setWallBcs( flags = flags, vel = vel )

	advectSemiLagrange_time_fraction(flags=flags, vel=vel, grid=vel, order=2, time_fraction = 0.5)
#	advectSemiLagrange_time_fraction(flags=flags, vel=vel, grid=density, order=2, time_fraction = 0.5)
	setWallBcs(flags=flags, vel=vel)

	solvePressure3_part1(flags=flags, vel=vel, pressure=pressure, openBound='Y')
	solvePressure3_part2(flags=flags, vel=vel, pressure=pressure)
	setWallBcs(flags=flags, vel=vel)

	advectSemiLagrange_time_fraction(flags=flags, vel=vel, grid=density, order=2, time_fraction = 1.0)
	advectSemiLagrange_time_fraction(flags=flags, vel=vel, grid=vel, order=2, time_fraction = 0.5)
	setWallBcs(flags=flags, vel=vel)

	timings.display()
	s.step()
#	gui.screenshot( '/media/tushar/TOSHIBA EXT/latex/4_Perlin/8_sinusoidal_warping/density/%03d.png' % t )
