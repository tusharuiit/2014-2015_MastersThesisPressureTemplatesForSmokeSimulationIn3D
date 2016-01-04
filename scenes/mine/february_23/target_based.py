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
source_density = s.create(RealGrid)
target_density = s.create(RealGrid)

source_density_convolve = s.create(RealGrid)
target_density_convolve = s.create(RealGrid)

density_minus_density_star = s.create(RealGrid)
Grad_x = s.create()
Grad_y = s.create()

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
#create_source_density_from_image(source_density,multiplier=1.0,res_x=gs_x,res_y=gs_y)
create_source_density_from_image(density,multiplier=1.0,res_x=gs_x,res_y=gs_y)
create_source_density_from_image(density_minus_density_star,multiplier=1.0,res_x=gs_x,res_y=gs_y)
create_target_density_from_image(target_density,multiplier=1.0,res_x=gs_x,res_y=gs_y)

create_source_density_convolve_from_image(source_density_convolve,multiplier=1.0,res_x=gs_x,res_y=gs_y)
#convolve(target_density_convolved , pressure_template_image)

create_target_density_convolve_from_image(target_density_convolve,multiplier=1.0,res_x=gs_x,res_y=gs_y)
#convolve_duplicate(pressure_template_convolved , pressure_template_convolved_python)

#write_to_file_c(target_density_convolve)

create_image_template(pressure_template_image,multiplier=4.0,res_x=gs_x,res_y=gs_y)

#	This is the noise with central mean value as 1
#source.applyRNG_binary_NoiseToGrid_seeded(grid=density, value=1, fraction_white=0.20, res_x = gs_x, res_y = gs_y)

#main loop
last_time_step = 401
for t in range(last_time_step):

#0	For fun, you may create a density shape with the help of an image

#1	Apply driving forces/template: ut = vf * F(smoke_concentration,smoke_concentration*)
#	Apply template : vel = vel - Grad(template)
#	time range of application of pressure modification
#	A big difference between my idea of pressure_templates and fattal_lischinski is that they have a differential
#equation in time. Is my formula vel=vel-Grad(Template) also a DE with time step 1 ?

	t1 = 1
	t2 = 400
	if (t==(t1)):
		gui.pause()
	if (t==(t2)):
		gui.pause()
	if (t>=t1)and(t<=t2):
#		This is velocity correction
#		solvePressure3_part2(flags=flags, vel=vel, pressure=pressure_template_image)
		solvePressure3_part2_target(flags=flags, vel=vel, pressure=target_density_convolve, source_density_convolve=density)
		setWallBcs(flags=flags, vel=vel)

#2.	Attenuate momentum:
		
#		attenuate(flags=flags, vel=vel, attenuate_coefficient=0.0)


#3.	Advect momentum:
	advectSemiLagrange_time_fraction(flags=flags, vel=vel, grid=vel, order=2, time_fraction = 1.0)
	setWallBcs(flags=flags, vel=vel)

#4.	Project: solve for the pressure field given by the Poisson
#	equation and subtract
	if (t>t2):
		solvePressure3_part1(flags=flags, vel=vel, pressure=pressure, openBound='Y')
		solvePressure3_part2(flags=flags, vel=vel, pressure=pressure)
		setWallBcs(flags=flags, vel=vel)

#5.	Advect smoke:
#	advectSemiLagrange_time_fraction(flags=flags, vel=vel, grid=vel, order=2, time_fraction = 0.5)
	advectSemiLagrange_time_fraction(flags=flags, vel=vel, grid=density, order=2, time_fraction = 1.0)

#6.	Gather smoke:
#	Define G
#	We will solve this in an EXPLICIT manner
	
#	smoke_concentration = present grid of the density field

#	Convert the low pressure regions into scalar quantities and gets ready
#	The question is how ? Create a grid of target density
#	Then you have the target_density and pressure_template
#	Assume that the values of the pressure_template vary from 0 to 1
#	Now, just do :
	density_minus_density_star.copyFrom(density)
	density_minus_density_star.sub(target_density)
	calculate_grad_x(Grad_x,density_minus_density_star)
	calculate_grad_y(Grad_y,density_minus_density_star)

	Grad_x.mult(target_density_convolve)
	Grad_x.mult(density)

	Grad_y.mult(target_density_convolve)
	Grad_y.mult(density)

	calculate_divergence(divergence_grid, Grad_x, Grad_y)

	gather(density, divergence_grid)

	timings.display()
	s.step()

#	gui.screenshot( '/home/tushar/manta_mikey_1/manta/build/pictures/february_9/images/1/%03d.png' % t )
