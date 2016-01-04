#
# Simple example scene for a 2D simulation
# Simulation of a buoyant smoke density plume
#
from manta import *
#import manta

#Image for the template
#from PIL import Image
import Image
import os

im = Image.open("/home/tushar/manta_mikey_1/manta/scenes/mine/november_24/templates_krueger/november_25/rotated_ellipse_2.png")
#Can be many different formats.
extreme_values = im.getextrema()
#max_pixel_value = extreme_values[0][1]
max_pixel_value = extreme_values[1]
#print "max_pixel_value = ", max_pixel_value
#wait = raw_input("PRESS ENTER TO CONTINUE.")

pix = im.load()
x,y=im.size


# solver params
res = 128

gs = vec3(res,res,1)
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

#This is the pressure template
pressure_template = s.create(RealGrid)

flags.initDomain()
flags.fillGrid()

if (GUI):
	gui = Gui()
	gui.show()
	gui.pause()

#This is the original source
#source = s.create(Cylinder, center=gs*vec3(0.5,0.1,0.5), radius=res*0.14, z=gs*vec3(0, 0.02, 0))
#This is the source shape for noise in this case
source = s.create(Cylinder, center=gs*vec3(0.5,0.5,0.5), radius=res*0.48, z=gs*vec3(0, 0.48, 0))

# This is for the centre of the template in the grid
template_center_x = int(res*0.5)
template_center_y = int(res*0.5)
template_center_z = int(res*0.5)
template_center = vec3(template_center_x,template_center_y,template_center_z)

#	Create the template
#	2. Assign the values of the image to the template grid
m = template_center_x - int(x/2)
n = template_center_y - int(y/2)
for i in range(x):
	for j in range(y):
		multiplier = 4.0
#		value_pixel = (multiplier)*(pix[i,j][0]/(max_pixel_value * 1.0))
		value_pixel = (multiplier)*(pix[i,j]/(max_pixel_value * 1.0))
#		print " value_pixel = ",value_pixel
		zval = 0
		pressure_template.assign_value(m+i,n+j,zval,value_pixel)

#pressure_template.printGrid(0,False)

#	This is the noise with central mean value as 1
source.applyRNG_binary_NoiseToGrid_seeded(grid=density, value=1, fraction_white=0.20 ,res=res)
#wait = raw_input("PRESS ENTER TO CONTINUE.")

print "ENTERING LOOP"

#main loop
for t in range(399):
#	if t<300:
#		source.applyToGrid(grid=density, value=1)

	advectSemiLagrange(flags=flags, vel=vel, grid=density, order=2)    
	advectSemiLagrange(flags=flags, vel=vel, grid=vel,     order=2)

	setWallBcs(flags=flags, vel=vel)    

#	timings.display()
#	gui.pause()
#	gui.screenshot( './pictures/november_3/3/density_3steps/%03d_after_advection_density.png' % t )

#	if t<200:
#		addBuoyancy(density=density, vel=vel, gravity=vec3(0,-4e-3,0), flags=flags)

	solvePressure3_part1(flags=flags, vel=vel, pressure=pressure, openBound='Y')

#	gui.screenshot( './pictures/november_3/3/pressure_after_solve/%03d_pressure_after_solve.png' % t )

	solvePressure3_part2(flags=flags, vel=vel, pressure=pressure)

	setWallBcs(flags=flags, vel=vel)

#	timings.display()
#	gui.pause()
#	gui.screenshot( './pictures/november_3/3/density_3steps/%03d_after_pressure_solve_density.png' % t )

#	gui.screenshot( './pictures/november_3/3/pressure_modification/%03d.png' % t )

#	time range of application of pressure modification
	t1 = 5
	t2 = 185
	if (t==(t1-2)):
		gui.pause()
	if (t==(t2-2)):
		gui.pause()

	if (t>t1)and(t<t2):
#		This is velocity correction
		solvePressure3_part2(flags=flags, vel=vel, pressure=pressure_template)

		setWallBcs(flags=flags, vel=vel)

#		timings.display()
#		gui.pause()
#		gui.screenshot( './pictures/november_3/3/density_3steps/%03d_after_pressure_modify_density.png' % t )

	s.step()
	gui.screenshot( '/home/tushar/manta_mikey_1/manta/build/pictures/november_24/rotated_ellipse_2_1/%03d_end_density.png' % t )
#	gui.screenshot( './pictures/november_3/star12/%03d_density.png' % t )
