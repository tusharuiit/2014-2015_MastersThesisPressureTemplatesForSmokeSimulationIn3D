#
# Simple example scene for a 2D simulation
# Simulation of a buoyant smoke density plume
#
from manta import *
#import manta

import os

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

#This is a dummy grid created for the velocity-only visualization in the GUI.Velocity is MAC Grid
velocity_only = s.create(RealGrid)

#This is the pressure just after CG solve
pressure_before = s.create(RealGrid)

pressure_template_image = s.create(RealGrid)

#Some more grids to be created
#......
#.......
target_density = s.create(RealGrid)

flags.initDomain()
flags.fillGrid()

if (GUI):
	gui = Gui()
	gui.show()
	gui.pause()

#This is the source shape
source = s.create(Cylinder, center=gs*vec3(0.5,0.5,0.5), radius=res*0.48, z=gs*vec3(0, 0.48, 0))

# This is for an image template
# It has a maximum value of multiplier and minimum value of 0.0
create_image_template(pressure_template_image,multiplier=4.0,res_x=gs_x,res_y=gs_y)
convolve(pressure_template_convolved_python , pressure_template_image)
convolve_duplicate(pressure_template_convolved , pressure_template_convolved_python)

#	This is the noise with central mean value as 1
source.applyRNG_binary_NoiseToGrid_seeded(grid=density, value=1, fraction_white=0.20, res_x = gs_x, res_y = gs_y)

#main loop
last_time_step = 401
for t in range(last_time_step):

#1	Apply driving forces/template: ut = νf * F(ρ,ρ*)
#	Apply template : vel = vel - Grad(template)
#	time range of application of pressure modification
	t1 = 1
	t2 = 400
	if (t==(t1)):
		gui.pause()
	if (t==(t2)):
		gui.pause()
	if (t>=t1)and(t<=t2):
#		This is velocity correction
		solvePressure3_part2(flags=flags, vel=vel, pressure=pressure_template_image)
		setWallBcs(flags=flags, vel=vel)

#2.	Attenuate momentum: ut = − νd * u


#3.	Advect momentum: ut = −u.∇u
	advectSemiLagrange_time_fraction(flags=flags, vel=vel, grid=vel, order=2, time_fraction = 1.0)
	setWallBcs(flags=flags, vel=vel)

#4.	Project: solve for the pressure field p given by the Poisson
#	equation ∇2p = ∇.u and subtract ∇p from u.
	if (t>t2):
		solvePressure3_part1(flags=flags, vel=vel, pressure=pressure, openBound='Y')
		solvePressure3_part2(flags=flags, vel=vel, pressure=pressure)
		setWallBcs(flags=flags, vel=vel)

#5.	Advect smoke: ρt = −u.∇ρ
#	advectSemiLagrange_time_fraction(flags=flags, vel=vel, grid=vel, order=2, time_fraction = 0.5)
	advectSemiLagrange_time_fraction(flags=flags, vel=vel, grid=density, order=2, time_fraction = 1.0)

#6.	Gather smoke: ρt = νg*G(ρ,ρ*)
#	Define G(ρ,ρ*) such that G(ρ,ρ*) = ∇.[ρρ~ * ∇(ρ−ρ*)]
#	ρt = νg * ∇.[ρρ~ * ∇(ρ−ρ*)]
#	We will solve this in an explicit manner
	
#	ρ = present grid of the density field
#	ρ* = Inverse of the Pressure template field in terms of the scalar
#	Convert the low pressure regions into scalar quantities and ρ* gets ready
#	The question is how ? Create a grid of target density
#	Then you have the target_density and pressure_template
#	Assume that the values of the pressure_template vary from 0 to 1
#	Now, just do :
#	(ρ* = target_density = (1.0 - pressure_template))


	timings.display()
	s.step()

	gui.screenshot( '/home/tushar/manta_mikey_1/manta/build/pictures/february_9/images/1/%03d.png' % t )
