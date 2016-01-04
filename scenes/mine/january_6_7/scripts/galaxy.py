#
# Simple example scene for a 2D simulation
# Simulation of a buoyant smoke density plume
#
from manta import *
#import manta

import os

from time import sleep

# solver params
res = 256

gs_x = res
gs_y = res
gs = vec3(gs_x,gs_y,1)
s = Solver(name='main', gridSize = gs, dim=2)
s.timestep = 1.0
timings = Timings()

# prepare grids
flags = s.create(FlagGrid)
#flag_test = s.create(FlagGrid)
vel = s.create(MACGrid)
#vel_test = s.create(MACGrid)
density = s.create(RealGrid)
pressure = s.create(RealGrid)

#This is a dummy grid created for the velocity-only visualization in the GUI.Velocity is MAC Grid
velocity_only = s.create(RealGrid)

#This is the pressure just after CG solve
pressure_before = s.create(RealGrid)

pressure_template_image = s.create(RealGrid)

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

# Open a file
fo = open("/home/tushar/manta_mikey_1/manta/scenes/mine/january_5/foo.txt", "wb")

#main loop
last_time_step = 400
for t in range(last_time_step):

#	time range of application of pressure modification
	t1 = 3
	t2 = 97

	if (t>t1)and(t<t2):
	# This is for an image template
	# It has a maximum value of multiplier and minimum value of 0.0
		create_image_template_varying(pressure_template_image,t,multiplier=0.25,res_x=gs_x,res_y=gs_y)


	if (t==(t1-1)):
		gui.pause()
	if (t==(t2-1)):
		gui.pause()
	if (t>t1)and(t<t2):
#		This is velocity correction
		solvePressure3_part2(flags=flags, vel=vel, pressure=pressure_template_image)
		setWallBcs(flags=flags, vel=vel)

	advectSemiLagrange_time_fraction(flags=flags, vel=vel, grid=density, order=2, time_fraction = 1.0)
	print "Maximum absolute value of density = ", density.getMaxAbsValue();
	advectSemiLagrange_time_fraction(flags=flags, vel=vel, grid=vel, order=2, time_fraction = 1.0)
	print "Maximum absolute value of velocity = ", vel.getMaxAbsValue();
	setWallBcs(flags=flags, vel=vel)
	fo.write( "Maximum absolute value of density = "+str(density.getMaxAbsValue())+"\n");
	fo.write( "Maximum absolute value of velocity = "+str(vel.getMaxAbsValue())+"\n");
	fo.write( "Maximum absolute value of pressure = "+str(pressure.getMaxAbsValue())+"\n");
	fo.write( "Maximum absolute value of pressure_template = "+str(pressure_template_image.getMaxAbsValue())+"\n");
	fo.write("\n");

#	if (t>t2):
#		solvePressure3_part1(flags=flags, vel=vel, pressure=pressure, openBound='Y')
#		solvePressure3_part2(flags=flags, vel=vel, pressure=pressure)
#		setWallBcs(flags=flags, vel=vel)

	timings.display()
	s.step()

	file_out = 0
	xlength = gs_x
	ylength = gs_y
	dx = xlength/gs_x
	dy = ylength/gs_y
	print "TESTING AND TASTING"
#	do_vtk(file_out, t, xlength, ylength, gs_x, gs_y, dx, dy, vel, density)
	if(t == (last_time_step - 1)):
##		os.system("cd /home/tushar/manta_mikey_1/manta/build/pictures/january_3")
#		os.system("sed -i -e 's/,/./g' /home/tushar/manta_mikey_1/manta/build/pictures/january_3/1/*.vtk")
		# Close opend file
		fo.close()

	gui.screenshot( '/home/tushar/manta_mikey_1/manta/build/pictures/january_6/1/images/%03d.png' % t )
