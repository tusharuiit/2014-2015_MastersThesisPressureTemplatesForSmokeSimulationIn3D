#
# Simple example scene (hello world)
# Simulation of a buoyant smoke density plume

#import pdb; pdb.set_trace()

from manta import *

#Dimenstion
dim = 2

# solver params
res = 128
gs = vec3(res,res,res)
#gs = vec3(res,1.5*res,res)
if (dim==2):
	gs.z = 1  # 2D
	
s = FluidSolver(name='main', gridSize = gs, dim = dim)
s.timestep = 1.0
timings = Timings()

# prepare grids
flags = s.create(FlagGrid)
vel = s.create(MACGrid)
density = s.create(RealGrid)
pressure = s.create(RealGrid)

weight = s.create(RealGrid)
weight_value = 1
assign_value_to_weight(weight,weight_value)

# noise field
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

##source = s.create(Cylinder, center=gs*vec3(0.5,0.1,0.5), radius=res*0.14, z=gs*vec3(0, 0.02, 0))
#source = s.create(Cylinder, center=gs*vec3(0.5,0.2,0.5), radius=res*0.25, z=gs*vec3(0, 0.05, 0))
#This is the source shape
source = s.create(Cylinder, center=gs*vec3(0.5,0.5,0.5), radius=res*0.48, z=gs*vec3(0, 0.48, 0))

#	This is the noise with central mean value as 1
source.applyRNG_binary_NoiseToGrid_seeded(grid=density, value=1, fraction_white=0.20, res_x = gs.x, res_y = gs.y)

# wavelet turbulence octaves

wltnoise = s.create(NoiseField, loadFromFile=True)
# scale according to lowres sim , smaller numbers mean larger vortices
#Original
#wltnoise.posScale = vec3( int(0.5*gs.x) ) * 0.5
#Fits for 2D
#wltnoise.posScale = vec3( int(0.5*gs.x) ) * 1.0
#Fits for 2D
wltnoise.posScale = vec3( int(0.5*gs.x) ) * 0.5

wltnoise.timeAnim = 0.1

wltStrength = 0.4

	
#main loop
for t in range(250):
#	if t<100:
#		densityInflow(flags=flags, density=density, noise=noise, shape=source, scale=1, sigma=0.5)
	
	# optionally, enforce inflow velocity
#	source.applyToGrid(grid=vel, value=velInflow)

	advectSemiLagrange(flags=flags, vel=vel, grid=density, order=2)    
	advectSemiLagrange(flags=flags, vel=vel, grid=vel    , order=2, strength=1.0)

	setWallBcs(flags=flags, vel=vel)    
#	addBuoyancy(density=density, vel=vel, gravity=vec3(0,-6e-4,0), flags=flags)

	solvePressure(flags=flags, vel=vel, pressure=pressure, useResNorm=True)
	setWallBcs(flags=flags, vel=vel)

#	applyNoiseVec3( flags=flags, target=vel, noise=wltnoise, scale=wltStrength*1.0 , weight=weight)
	applyNoiseVec3( flags=flags, target=vel, noise=wltnoise, scale=wltStrength*0.02 , weight=weight)

#	density.save('/media/tushar/TOSHIBA EXT/feb_26/4/den%04d.vol' % t)

	timings.display()
	s.step()
	gui.screenshot( '/home/tushar/manta_mikey_1/manta/build/pictures/february_27/images/3/%03d.png' % t )
