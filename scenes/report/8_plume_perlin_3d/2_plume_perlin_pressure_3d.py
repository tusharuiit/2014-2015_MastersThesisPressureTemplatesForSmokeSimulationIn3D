#
#	Simple example scene (hello world)
#	Simulation of a buoyant smoke density plume

#	import pdb; pdb.set_trace()

from manta import *

import sys
from time import sleep
from subprocess import call

#	Extract the perimeters
multiplier=float(sys.argv[1]) #15
x_frequency_x=int((sys.argv[2])) #9
x_frequency_y=int((sys.argv[3])) #27
#	y_frequency_x=int((sys.argv[3])) #27
#	y_frequency_y=int((sys.argv[2])) #9
#	frequency_z=int((sys.argv[4]))

t1=int((sys.argv[4]))
t2=int((sys.argv[5]))
#	end

#	solver parameters
res = int((sys.argv[6]))

res_x = res
size_scale_y = float((sys.argv[7]))
res_y = int(res * size_scale_y)

gs_x = int(res_x)
gs_y = int(res_y)
gs_z = res

#	This decides after every how many time-steps you have to do 
every_time_step=int((sys.argv[8]))

folder_simulation_number=(sys.argv[9])

gs = vec3(gs_x,gs_y,gs_z)
s = FluidSolver(name='main', gridSize = gs)
s.timestep = 1.0
timings = Timings()

#	prepare grids
flags = s.create(FlagGrid)
vel = s.create(MACGrid)
density = s.create(RealGrid)
pressure = s.create(RealGrid)

#	This is the noise template
pressure_template_perlin_x = s.create(RealGrid)
#	pressure_template_perlin_y = s.create(RealGrid)
#	pressure_template_perlin_x_y = s.create(RealGrid)

#	noise field
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
	gui.pause()

#	source = s.create(Cylinder, center=gs*vec3(0.5,0.1,0.5), radius=res*0.14, z=gs*vec3(0, 0.02, 0))
#source = s.create(Cylinder, center=gs*vec3(0.5,0.2,0.5), radius=res*0.25, z=gs*vec3(0, 0.05, 0))
source = s.create(Cylinder, center=gs*vec3(0.5,0.2,0.5), radius=res*0.25, z=gs*vec3(0, 0.05, 0))

simulation_directory_name = "/media/tushar/B19D-ABF3/latex1/6_3D_april_11/10_pressure_template_parametrization" + folder_simulation_number

call(["mkdir", simulation_directory_name])

#	main loop
for t in range(160):
	if t<100:
#		densityInflow(flags=flags, density=density, noise=noise, shape=source, scale=1, sigma=0.5)
		source.applyToGrid(grid=density, value=1)

	if ( (t>=t1) and (t<t2) and ((t % every_time_step == 0)) ):

		create_perlin_template_3D_kernel ( pressure_template_perlin_x, multiplier=multiplier, res_x=gs_x, res_y=gs_y, res_z=gs_z, levels=6, time=t, frequency_x=x_frequency_x, frequency_y=x_frequency_y, frequency_z=10, translation_x=-0.05, translation_y=-0.05, translation_z=-0.05 )
##		create_perlin_template_3D_kernel(pressure_template_perlin_y,multiplier=9.0,res_x=gs_x,res_y=gs_y,res_z=gs_z,levels=6,time=t,frequency_x=7,frequency_y=21,frequency_z=10,translation_x=-0.05, translation_y=-0.05, translation_z=-0.05)

##		pressure_template_perlin_x_y.multConst(0)
##		pressure_template_perlin_x_y.add(pressure_template_perlin_x)
##		pressure_template_perlin_x_y.add(pressure_template_perlin_y)
##		pressure_template_perlin_x_y.mult(boundary_zero_smoother)

##		Apply the template
		apply_template_density_localized ( flags=flags, vel=vel, pressure_template=pressure_template_perlin_x, density=density, density_threshold=-0.00001, density_based_gradient_scaling=1 )
		setWallBcs( flags=flags, vel=vel )

	advectSemiLagrange_time_fraction(flags=flags, vel=vel, grid=vel, order=2, time_fraction = 1.0)
#	advectSemiLagrange_time_fraction(flags=flags, vel=vel, grid=density, order=2, time_fraction = 0.5)
	setWallBcs(flags=flags, vel=vel)

	addBuoyancy(density=density, vel=vel, gravity=vec3(0,-6e-4,0), flags=flags)

	solvePressure3_part1(flags=flags, vel=vel, pressure=pressure, openBound='Y')
	solvePressure3_part2(flags=flags, vel=vel, pressure=pressure)
	setWallBcs(flags=flags, vel=vel)

	advectSemiLagrange_time_fraction(flags=flags, vel=vel, grid=density, order=2, time_fraction = 1.0)
#	advectSemiLagrange_time_fraction(flags=flags, vel=vel, grid=vel, order=2, time_fraction = 1.0)
#	setWallBcs(flags=flags, vel=vel)

#	density.save('/media/tushar/B19D-ABF3/latex1/6_3D_april_11/9_plume_perlin/1_perlin_template/2_vol/%04d.vol' % t)

	timings.display()
	s.step()

	simulation_file_name = simulation_directory_name + '/%03d.png'

	gui.screenshot( simulation_file_name % t )

#	gui.screenshot( '/home/tushar/manta_mikey_1/manta/build/pictures/february_27/images/5/%03d.png' % t )
