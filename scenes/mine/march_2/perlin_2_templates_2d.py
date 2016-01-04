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

vel_extra_1 = s.create(MACGrid)

weight = s.create(RealGrid)
weight_value = 1
assign_value_to_weight(weight,weight_value)

#This is the noise template that is being
pressure_template_perlin_x = s.create(RealGrid)
boundary_zero_smoother = s.create(RealGrid)

#	This is for perlin noise template
create_image_template(boundary_zero_smoother,multiplier=1.0,res_x=gs_x,res_y=gs_y)

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

# wavelet turbulence octaves
wltnoise = s.create(NoiseField, fixedSeed = 333, loadFromFile=True)
# scale according to lowres sim , smaller numbers mean larger vortices
#Original
wltnoise.posScale = vec3( int(0.5*gs.x) ) * 0.5
wltnoise.timeAnim = 0.1
wltStrength = 0.4

#create_perlin_template_25(pressure_template_perlin_x,multiplier=12.0,res_x=gs_x,res_y=gs_y,time=t,frequency_x=7.0,frequency_y=21.0,sign_translate_x = 0.0, sign_translate_y = 0.0)

#main loop
for t in range(250):

#	time range of application of pressure modification
	create_perlin_template_25(pressure_template_perlin_x,multiplier=12.0,res_x=gs_x,res_y=gs_y,time=t,frequency_x=7.0,frequency_y=21.0,sign_translate_x = 0.0, sign_translate_y = 0.0)
	pressure_template_perlin_x.mult(boundary_zero_smoother)

##	This is velocity correction
#	solvePressure3_part2(flags=flags, vel=vel, pressure=pressure_template_perlin_x)
#	setWallBcs(flags=flags, vel=vel)

#	Modify the pressure template
	applyNoiseVec3( flags=flags, target=vel_extra_1, noise=wltnoise, scale=wltStrength*1.0 , weight=weight )
	advectSemiLagrange_time_fraction (flags=flags, vel = vel_extra_1, grid = pressure_template_perlin_x, order = 2, time_fraction = 0.5)
#	vel_extra_1.multConst(0)

#	advectSemiLagrange_time_fraction(flags=flags, vel=vel, grid=vel, order=2, time_fraction = 0.5)
#	setWallBcs(flags=flags, vel=vel)    

##	PPE
#	solvePressure3_part1(flags=flags, vel=vel, pressure=pressure, openBound='Y')
#	solvePressure3_part2(flags=flags, vel=vel, pressure=pressure)
#	setWallBcs(flags=flags, vel=vel)

#	advectSemiLagrange_time_fraction(flags=flags, vel=vel, grid=density, order=2, time_fraction = 1.0)
#	advectSemiLagrange_time_fraction(flags=flags, vel=vel, grid=vel, order=2, time_fraction = 0.5)
#	setWallBcs(flags=flags, vel=vel)

	timings.display()
	s.step()
#	gui.screenshot( '/home/tushar/manta_mikey_1/manta/build/pictures/february_27/images/4/%03d.png' % t )
