#
#	Simple example scene for a 2D simulation
#	Simulation of a buoyant smoke density plume
#
from manta import *
#import manta
import sys
from time import sleep
from subprocess import call

#	start
folder_simulation_number=(sys.argv[1])
t1=int((sys.argv[2]))
t2=int((sys.argv[3]))
#	end

#	solver params
res = int((sys.argv[4]))

gs_x = int(res)
scale_y_resolution = float((sys.argv[5]))
gs_y = int(res * scale_y_resolution)
gs_z = int((res))

#	This decides after every how many time-steps you have to do 
#every_time_step=int((sys.argv[11]))

gs = vec3(gs_x,gs_y,gs_z)
s = Solver(name='main', gridSize = gs)
s.timestep = 1.0
timings = Timings()

#	prepare grids
flags = s.create(FlagGrid)
vel = s.create(MACGrid)
density = s.create(RealGrid)
pressure = s.create(RealGrid)

weight = s.create(RealGrid)
vel_template = s.create(MACGrid)

#	noise field
noise = s.create(NoiseField, loadFromFile=True)
noise.posScale = vec3(45)
noise.clamp = True
noise.clampNeg = 0
noise.clampPos = 1
noise.valScale = 1
noise.valOffset = 0.75
noise.timeAnim = 0.2


# wavelet turbulence octaves
wltnoise = s.create(NoiseField, loadFromFile=True)
# scale according to lowres sim , smaller numbers mean larger vortices
posScale_factor = float((sys.argv[6]))
wltnoise.posScale = vec3( int(0.5*gs.x) ) * posScale_factor
wltnoise.timeAnim = 0.1

strength_wavelet = 0.4
wltStrength = strength_wavelet

flags.initDomain()
flags.fillGrid()

if (GUI):
	gui = Gui()
	gui.show()
#	gui.pause()

#	This is the source shape
source = s.create(Cylinder, center=gs*vec3(0.5,0.1,0.5), radius=int(res*0.14), z=gs*vec3(0, 0.02, 0))
#source = s.create(Cylinder, center=gs*vec3(0.5,0.2,0.5), radius=res*0.2, z=gs*vec3(0, 0.025, 0))

#simulation_directory_name = "/media/tushar/B19D-ABF3/latex1/6_3D_april_11/9_plume_perlin/2_velocity_template/1_screenshot/" + folder_simulation_number
simulation_directory_name = "/media/tushar/B19D-ABF3/latex1/6_3D_april_11/13_final/screenshot/2_velocity_templates_include_source_1.00"

#vol_directory_name = "/media/tushar/B19D-ABF3/latex1/6_3D_april_11/9_plume_perlin/2_velocity_template/2_vol/" + folder_simulation_number
vol_directory_name = "/media/tushar/B19D-ABF3/latex1/6_3D_april_11/13_final/vol/2_velocity_templates_include_source_1.00"

#call(["mkdir", simulation_directory_name])
#call(["mkdir", vol_directory_name])

#time_steps = 4

#file_pointer = open("foo.txt", "wb")

#main loop
for t in range(250):

	if t<150:
		source.applyToGrid(grid=density, value=1)
#	if t<100:
#		densityInflow(flags=flags, density=density, noise=noise, shape=source, scale=1, sigma=0.5)


#	vel.multConst((0,0,0))
#	if ( (t>=t1) and (t<t2) and ((t % every_time_step == 0)) ):

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

	addBuoyancy(density=density, vel=vel, gravity=vec3(0,-6e-4,0), flags=flags)

	solvePressure3_part1(flags=flags, vel=vel, pressure=pressure, openBound='Y')
	solvePressure3_part2(flags=flags, vel=vel, pressure=pressure)
	setWallBcs(flags=flags, vel=vel)

	vel_template.multConst((0,0,0))

#	weight.copyFrom(density)
	if ( (t>=t1) and (t<=t2) ):
		approximate_weight_from_density ( target=weight, source=density )
#	Use this if no restriction with shape, else use applyNoiseVec3_custom_velocity_template
		applyNoiseVec3 ( flags=flags, target=vel_template, noise=wltnoise, scale=wltStrength * 1.0, weight=weight )
#		applyNoiseVec3_custom_velocity_template ( flags=flags, target=vel_template, noise=wltnoise, scale=wltStrength * 1.0, weight=weight, shaper = source )
#		advectSemiLagrange(flags=flags, vel=vel, grid=density, order=2)

	vel_template.add(vel)

	advectSemiLagrange_time_fraction(flags=flags, vel=vel_template, grid=density, order=2, time_fraction = 1.0)
	advectSemiLagrange_time_fraction(flags=flags, vel=vel, grid=vel, order=2, time_fraction = 1.0)
	setWallBcs(flags=flags, vel=vel)

	vol_file_name = vol_directory_name + '/%04d.vol'
	density.save( vol_file_name % t )

#	vel.copyFrom(vel_template)

	timings.display()
	s.step()

	simulation_file_name = simulation_directory_name + '/%03d.png'

	gui.screenshot( simulation_file_name % t )
