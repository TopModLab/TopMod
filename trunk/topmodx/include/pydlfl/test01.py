#! /usr/bin/env python

from dlfl import *
from random import *
from math import *

# Load in an object
load("cube.obj")

face = 6

# 360 degree Loop to use sine for distance/scaling with random twisting
for i in range(0,370,10):
    face = extrude( "cubical", face, abs(sin(radians(i)))+0.1,1, random()*20-10, abs(sin(radians(i)))+0.5 )

# Take the dual twice
dual()
dual()

# Subdivide the mesh
subdivide("doo-sabin")

# Insert an Edge
#insert_edge((7,8),(9,11))

# Save out as a new file
save("cuberandom.obj")

#####

import time

load("cube.obj")
face = faces(0)[0]

for i in range(14):
    t = abs(sin(time.time()))
    s = abs(sin(time.time()))
    face = extrude("cubical", face, t, 1, 0, s)
    time.sleep(1)

dual()
subdivide("checker",0.33)
dual()

oldfaces = faces(0)
punchfaces = []
for face in oldfaces:
	if( faceInfo(face)['size'] == 4 ):
		vv,ee = walk(face)
		punch = False
		for v in vv:
			if( vertexInfo(v)['valence'] == 3 ):
				punch = True
		if( punch ):
			punchfaces += [face]

rind(punchfaces,True,0.8)
