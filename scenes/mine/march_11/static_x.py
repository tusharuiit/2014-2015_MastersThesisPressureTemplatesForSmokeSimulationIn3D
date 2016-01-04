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
frequency_x=int((sys.argv[2]))
frequency_y=int((sys.argv[3]))
frequency_z=int((sys.argv[4]))

#TODO start
folder_simulation_number=(sys.argv[5])
folder_templates_number =(sys.argv[6])
t1=int((sys.argv[7]))
t2=int((sys.argv[8]))
#TODO end

# solver params
res = int((sys.argv[9]))

gs_x = res
gs_y = res
gs_z = int((sys.argv[10]))

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
source = s.create(Cylinder, center=gs*vec3(0.5,0.5,0.5), radius=res*0.48, z=gs*vec3(0, 0.48, 0))
#	This is the noise with central mean value as 1
source.applyRNG_binary_NoiseToGrid_seeded(grid=density, value=1, fraction_white=0.30, res_x = gs.x, res_y = gs.y)

simulation_directory_name = "/home/tushar/manta_mikey_1/manta/build/pictures/march_9/images/"+folder_simulation_number
templates_directory_name = "/home/tushar/manta_mikey_1/manta/build/pictures/march_9/templates/"+folder_templates_number

call(["mkdir", simulation_directory_name])
call(["mkdir", templates_directory_name])

#main loop
for t in range(gs_z):

#	if t<300:
#		source.applyToGrid(grid=density, value=1)

	if ((t>=t1)and(t<t2)):

		create_perlin_template_26(pressure_template_perlin,multiplier=multiplier,res_x=gs_x,res_y=gs_y,res_z=gs_z,frequency_x=frequency_x,frequency_y=frequency_y,frequency_z=frequency_z,z=t,folder_templates_number=folder_templates_number)
		pressure_template_perlin.mult(boundary_zero_smoother)

#	This is velocity correction
#		apply_template_localized(flags=flags, vel=vel, pressure_template=pressure_template_perlin, density=density, density_threshold=0.0001, density_based_gradient_scaling=1)
#		apply_template_vorticity_density_localized(flags=flags, vel=vel, pressure_template=pressure_template_perlin, density=density, density_threshold=0.1, vorticity_density_based_gradient_scaling=1)
		solvePressure3_part2(flags=flags, vel=vel, pressure=pressure_template_perlin)
		setWallBcs(flags=flags, vel=vel)

	advectSemiLagrange_time_fraction(flags=flags, vel=vel, grid=vel, order=2, time_fraction = 0.5)
#	advectSemiLagrange_time_fraction(flags=flags, vel=vel, grid=density, order=2, time_fraction = 0.5)
	setWallBcs(flags=flags, vel=vel)

#	addBuoyancy(density=density, vel=vel, gravity=vec3(0,-6e-4,0), flags=flags)

	solvePressure3_part1(flags=flags, vel=vel, pressure=pressure, openBound='Y')
	solvePressure3_part2(flags=flags, vel=vel, pressure=pressure)
	setWallBcs(flags=flags, vel=vel)

	advectSemiLagrange_time_fraction(flags=flags, vel=vel, grid=density, order=2, time_fraction = 1.0)
	advectSemiLagrange_time_fraction(flags=flags, vel=vel, grid=vel, order=2, time_fraction = 0.5)
	setWallBcs(flags=flags, vel=vel)

	timings.display()
	s.step()

	simulation_file_name=simulation_directory_name+'/%03d.png'

#	gui.screenshot( '/home/tushar/manta_mikey_1/manta/build/pictures/march_4/images/4/velocity/%03d.png' % t )
	gui.screenshot( simulation_file_name % t )

#End
