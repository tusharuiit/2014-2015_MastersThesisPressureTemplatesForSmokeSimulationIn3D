#
#	Simple example scene for a 2D simulation
#	Simulation of a buoyant smoke density plume
#
from manta import *
#import manta
import sys
from time import sleep
from subprocess import call

#Extract the perimeters
multiplier=float(sys.argv[1])
x_frequency_x=int((sys.argv[2]))
x_frequency_y=int((sys.argv[3]))
y_frequency_x=int((sys.argv[3]))
y_frequency_y=int((sys.argv[2]))
frequency_z=int((sys.argv[4]))

# start
folder_simulation_number=(sys.argv[5])
folder_templates_number =(sys.argv[6])
t1=int((sys.argv[7]))
t2=int((sys.argv[8]))
# end

# solver params
res = int((sys.argv[9]))

gs_x = int(res)
gs_y = int(res)
gs_z = int((sys.argv[10]))

#This decides after every how many time-steps you have to do 
every_time_step=int((sys.argv[11]))
print " every_time_step = ", every_time_step

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
pressure_template_perlin_x = s.create(RealGrid)
#pressure_template_perlin_y = s.create(RealGrid)
pressure_template_perlin_x_y = s.create(RealGrid)

#	This is to smooth the boundaries of the pressure template
boundary_zero_smoother = s.create(RealGrid)

flags.initDomain()
flags.fillGrid()

#	This is for smoothing of perlin noise template at the boundaries
#	It has a maximum value of multiplier and minimum value of 0.0
file_name_template='/home/tushar/manta_mikey_1/manta/build/pictures/january_13/templates/4/'+str(gs_x)+'x'+str(gs_y)+'.png'
read_image_template(boundary_zero_smoother,multiplier=1.0,res_x=gs_x,res_y=gs_y,smoother_name=file_name_template)

if (GUI):
	gui = Gui()
	gui.show()
#	gui.pause()

#	This is the source shape
source = s.create(Cylinder, center=gs*vec3(0.5,0.2,0.5), radius=int(res*0.1), z=gs*vec3(0, 0.02, 0))
#source = s.create(Cylinder, center=gs*vec3(0.5,0.2,0.5), radius=res*0.2, z=gs*vec3(0, 0.025, 0))

simulation_directory_name = "/home/tushar/manta_mikey_1/manta/build/pictures/march_11/images/"+folder_simulation_number
templates_directory_name = "/home/tushar/manta_mikey_1/manta/build/pictures/march_11/templates/"+folder_templates_number

call(["mkdir", simulation_directory_name])
call(["mkdir", templates_directory_name])

start_x = 0.0
start_y = 1.0
time_steps = 4

file_pointer = open("foo.txt", "wb")

#main loop
for t in range(gs_z):

	if t<300:
		source.applyToGrid(grid=density, value=1)

	if ( (t>=t1) and (t<t2) and ((t % every_time_step == 0)) ):

		create_perlin_template_26(pressure_template_perlin_x,multiplier=multiplier,res_x=gs_x,res_y=gs_y,res_z=gs_z,frequency_x=x_frequency_x,frequency_y=x_frequency_y,frequency_z=frequency_z,z=t,folder_templates_number=folder_templates_number)
#		create_perlin_template_26(pressure_template_perlin_y,multiplier=multiplier,res_x=gs_x,res_y=gs_y,res_z=gs_z,frequency_x=x_frequency_x,frequency_y=x_frequency_y,frequency_z=frequency_z,z=t,folder_templates_number=folder_templates_number)

#	Interpolate
#    We move from x:y = start_x:start_y to x:y start_y:start_y

#		time_depender = ((t - 1) % time_steps)
#		if(time_depender < (time_steps/2)):
#			linear_interpolate = start_x + ((start_y - start_x)/(time_steps/2.0)) * time_depender
#			value_to_write = str(t) + " linear_interpolate = " + str(linear_interpolate) + "\n"
#			file_pointer.write(value_to_write)
#		else:
#			time_depender = time_depender - time_steps/2
#			linear_interpolate = start_y + ((start_x - start_y)/(time_steps/2.0)) * time_depender
#			value_to_write = str(t) + " linear_interpolate = " + str(linear_interpolate) + "\n"
#			file_pointer.write(value_to_write)

#		pressure_template_perlin_x.multConst(linear_interpolate)
#		pressure_template_perlin_y.multConst(1.0 - linear_interpolate)

		pressure_template_perlin_x_y.multConst(0)
		pressure_template_perlin_x_y.add(pressure_template_perlin_x)
#		pressure_template_perlin_x_y.add(pressure_template_perlin_y)
		pressure_template_perlin_x_y.mult(boundary_zero_smoother)

#		This is velocity correction
#		solvePressure3_part2(flags=flags, vel=vel, pressure=pressure_template_perlin_x_y)
		apply_template_density_localized(flags=flags, vel=vel, pressure_template=pressure_template_perlin_x_y, density=density, density_threshold=-0.00001, density_based_gradient_scaling=1)
#		apply_template_vorticity_density_localized(flags=flags, vel=vel, pressure_template=pressure_template_perlin_x_y, density=density, density_threshold=0.01, vorticity_density_based_gradient_scaling=1,curl_strength_modifier=curl_strength_modifier)
#		apply_template_vorticity_localized(flags=flags, vel=vel, pressure_template=pressure_template_perlin_x_y, density=density, density_threshold=0.01, vorticity_based_gradient_scaling=1,curl_strength_modifier=curl_strength_modifier)
		setWallBcs(flags=flags, vel=vel)

	advectSemiLagrange_time_fraction(flags=flags, vel=vel, grid=vel, order=2, time_fraction = 1.0)
##	advectSemiLagrange_time_fraction(flags=flags, vel=vel, grid=density, order=2, time_fraction = 0.5)
	setWallBcs(flags=flags, vel=vel)

	addBuoyancy(density=density, vel=vel, gravity=vec3(0,-6e-4,0), flags=flags)

	solvePressure3_part1(flags=flags, vel=vel, pressure=pressure, openBound='Y')
	solvePressure3_part2(flags=flags, vel=vel, pressure=pressure)
	setWallBcs(flags=flags, vel=vel)

	advectSemiLagrange_time_fraction(flags=flags, vel=vel, grid=density, order=2, time_fraction = 1.0)
#	advectSemiLagrange_time_fraction(flags=flags, vel=vel, grid=vel, order=2, time_fraction = 1.0)
#	setWallBcs(flags=flags, vel=vel)

	timings.display()
	s.step()

	simulation_file_name=simulation_directory_name+'/%03d.png'

	gui.screenshot( simulation_file_name % t )

#End
