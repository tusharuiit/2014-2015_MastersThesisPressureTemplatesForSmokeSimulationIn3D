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

gs_x = int(res*0.75)
gs_y = int(res*1.25)
gs_z = int((sys.argv[5]))

#	This decides after every how many time-steps you have to do 
#every_time_step=int((sys.argv[11]))

gs = vec3(gs_x,gs_y,1)
s = Solver(name='main', gridSize = gs, dim=2)
s.timestep = 1.0
timings = Timings()

#	prepare grids
flags = s.create(FlagGrid)
vel = s.create(MACGrid)
density = s.create(RealGrid)
pressure = s.create(RealGrid)

weight = s.create(RealGrid)
#assign_value_to_weight(weight, 1)
vel_template = s.create(MACGrid)

# wavelet turbulence octaves
wltnoise = s.create(NoiseField, loadFromFile=True)
# scale according to lowres sim , smaller numbers mean larger vortices
wltnoise.posScale = vec3( int(0.5*gs.x) ) * 1.25
wltnoise.timeAnim = 0.1

wltStrength = 0.4

flags.initDomain()
flags.fillGrid()

if (GUI):
	gui = Gui()
	gui.show()
#	gui.pause()

#	This is the source shape
source = s.create(Cylinder, center=gs*vec3(0.5,0.2,0.5), radius=int(res*0.1), z=gs*vec3(0, 0.02, 0))
#source = s.create(Cylinder, center=gs*vec3(0.5,0.2,0.5), radius=res*0.2, z=gs*vec3(0, 0.025, 0))

simulation_directory_name = "/home/tushar/manta_mikey_1/manta/build/pictures/march_11/images/"+folder_simulation_number

call(["mkdir", simulation_directory_name])

time_steps = 4

file_pointer = open("foo.txt", "wb")

#main loop
for t in range(gs_z):

	if t<300:
		source.applyToGrid(grid=density, value=1)

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

#	weight.copyFrom(density)
	approximate_weight_from_density(target=weight,source=density)
	vel_template.multConst((0,0,0))
	applyNoiseVec3( flags=flags, target=vel_template, noise=wltnoise, scale=wltStrength*1.0 , weight=weight)
	vel_template.add(vel)
#	advectSemiLagrange(flags=flags, vel=vel, grid=density, order=2)

	advectSemiLagrange_time_fraction(flags=flags, vel=vel_template, grid=density, order=2, time_fraction = 1.0)
	advectSemiLagrange_time_fraction(flags=flags, vel=vel, grid=vel, order=2, time_fraction = 1.0)
	setWallBcs(flags=flags, vel=vel)

#	vel.copyFrom(vel_template)

	timings.display()
	s.step()

	simulation_file_name=simulation_directory_name+'/%03d.png'

	gui.screenshot( simulation_file_name % t )

#End
