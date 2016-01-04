#
#	Simple example scene for a 2D simulation
#	Simulation of a buoyant smoke density plume
#
from manta import *
#import manta
import sys
from time import sleep
from subprocess import call

# solver params
res = int((sys.argv[6]))

gs_x = res
gs_y = res
gs_z = int((sys.argv[7]))

gs = vec3(gs_x,gs_y,1)
s = Solver(name='main', gridSize = gs, dim=2)
s.timestep = 1.0
timings = Timings()

#	prepare grids
flags = s.create(FlagGrid)

#	This is the noise template
pressure_template_perlin = s.create(RealGrid)

flags.initDomain()
flags.fillGrid()

if (GUI):
	gui = Gui()
	gui.show()
#	gui.pause()

#Extract the perimeters
multiplier=float(sys.argv[1])
frequency_x=int((sys.argv[2]))
frequency_y=int((sys.argv[3]))
frequency_z=int((sys.argv[4]))
run_count=((sys.argv[5]))

create_perlin_template_26(pressure_template_perlin,multiplier=multiplier,res_x=gs_x,res_y=gs_y,res_z=int((sys.argv[8])),start_x=frequency_x,start_y=frequency_y,frequency_z=frequency_z,run_count=run_count,time_steps=int((sys.argv[9])))

#End
