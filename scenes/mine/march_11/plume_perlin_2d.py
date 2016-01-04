#
#	Simple example scene for a 2D simulation
#	Simulation of a buoyant smoke density plume
#
from manta import *
#import manta
import sys
from time import sleep
from subprocess import call

# solver params
res = 64

gs_x = 160
gs_y = 160
gs = vec3(gs_x,gs_y,1)

s = Solver(name='main', gridSize = gs, dim=2)
s.timestep = 1.0
timings = Timings()

#	prepare grids
flags = s.create(FlagGrid)
vel = s.create(MACGrid)
density = s.create(RealGrid)
pressure = s.create(RealGrid)

#	This is the noise template
pressure_template_perlin = s.create(RealGrid)

pressure_template_perlin_x = s.create(RealGrid)
pressure_template_perlin_y = s.create(RealGrid)
pressure_template_perlin_x_y = s.create(RealGrid)

#	This is to smooth the boundaries of the pressure template
boundary_zero_smoother = s.create(RealGrid)

#	This is for smoothing of perlin noise template at the boundaries
#	It has a maximum value of multiplier and minimum value of 0.0
file_name_template='/home/tushar/manta_mikey_1/manta/build/pictures/january_13/templates/4/'+str(gs_x)+'x'+str(gs_y)+'.png'
create_image_template(boundary_zero_smoother,multiplier=1.0,res_x=gs_x,res_y=gs_y,smoother_name=file_name_template)

flags.initDomain()
flags.fillGrid()

if (GUI):
	gui = Gui()
	gui.show()
#	gui.pause()

#	This is the source shape
source = s.create(Cylinder, center=gs*vec3(0.5,0.1,0.5), radius=res*0.225, z=gs*vec3(0, 0.02, 0))
#source = s.create(Cylinder, center=gs*vec3(0.5,0.2,0.5), radius=res*0.2, z=gs*vec3(0, 0.025, 0))

#	This is the source shape
#source = s.create(Cylinder, center=gs*vec3(0.5,0.5,0.5), radius=res*0.48, z=gs*vec3(0, 0.48, 0))
##	This is the noise with central mean value as 1
#source.applyRNG_binary_NoiseToGrid_seeded(grid=density, value=1, fraction_white=0.20, res_x = gs.x, res_y = gs.y)

#Extract the perimeters
multiplier_x=float(sys.argv[1])
multiplier_y=float(sys.argv[2])
frequency_x_bias_x=float(sys.argv[3])
frequency_x_bias_y=float(sys.argv[4])
frequency_y_bias_x=float(sys.argv[5])
frequency_y_bias_y=float(sys.argv[6])
density_threshold=float(sys.argv[7])
vorticity_density_based_gradient_scaling=int(sys.argv[8])
run_count=(sys.argv[9])
#	Time range of application of the template
t1=int(sys.argv[10])
t2=int(sys.argv[11])

directory_name='/home/tushar/manta_mikey_1/manta/build/pictures/march_6/images/'+run_count
call(["mkdir", directory_name])

#main loop
for t in range(400):

	if t<300:
		source.applyToGrid(grid=density, value=1)

#	time range of application of pressure modification
#	if (t==(t1)):
#		gui.pause()
#	if (t==(t2)):
#		gui.pause()
	if ((t>=t1)and(t<t2)):

		create_perlin_template_25(pressure_template_perlin_x,multiplier=multiplier_x,res_x=gs_x,res_y=gs_y,time=t,frequency_x=frequency_x_bias_x,frequency_y=frequency_x_bias_y,sign_translate_x = -0.0, sign_translate_y = -0.0)
		create_perlin_template_25(pressure_template_perlin_y,multiplier=multiplier_y,res_x=gs_x,res_y=gs_y,time=t,frequency_x=frequency_y_bias_x,frequency_y=frequency_y_bias_y,sign_translate_x = -0.0, sign_translate_y = -0.0)

		pressure_template_perlin_x.mult(boundary_zero_smoother)
		pressure_template_perlin_y.mult(boundary_zero_smoother)

		pressure_template_perlin_x_y.multConst(0)
		pressure_template_perlin_x_y.add(pressure_template_perlin_x)
		pressure_template_perlin_x_y.add(pressure_template_perlin_y)

		pressure_template_perlin_x_y.mult(boundary_zero_smoother)

#	This is velocity correction
#		apply_template_localized(flags=flags, vel=vel, pressure_template=pressure_template_perlin_x_y, density=density, density_threshold=density_threshold, density_based_gradient_scaling=density_based_gradient_scaling)
		apply_template_vorticity_density_localized(flags=flags, vel=vel, pressure_template=pressure_template_perlin_x_y, density=density, density_threshold=density_threshold, vorticity_density_based_gradient_scaling=vorticity_density_based_gradient_scaling)
		setWallBcs(flags=flags, vel=vel)

	advectSemiLagrange_time_fraction(flags=flags, vel=vel, grid=vel, order=2, time_fraction = 0.5)
##	advectSemiLagrange_time_fraction(flags=flags, vel=vel, grid=density, order=2, time_fraction = 0.5)
	setWallBcs(flags=flags, vel=vel)

	addBuoyancy(density=density, vel=vel, gravity=vec3(0,-6e-4,0), flags=flags)

	solvePressure3_part1(flags=flags, vel=vel, pressure=pressure, openBound='Y')
	solvePressure3_part2(flags=flags, vel=vel, pressure=pressure)
	setWallBcs(flags=flags, vel=vel)

	advectSemiLagrange_time_fraction(flags=flags, vel=vel, grid=density, order=2, time_fraction = 1.0)
	advectSemiLagrange_time_fraction(flags=flags, vel=vel, grid=vel, order=2, time_fraction = 0.5)
	setWallBcs(flags=flags, vel=vel)

	timings.display()
	s.step()

	if(t==399):
		call(["killall", "-9", "manta"])
	
	target_directory_name=directory_name+'/%03d.png'

#	gui.screenshot( '/home/tushar/manta_mikey_1/manta/build/pictures/march_4/images/4/velocity/%03d.png' % t )
#	gui.screenshot( target_directory_name % t )