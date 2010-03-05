#! /usr/bin/env python

from dlfl import *
import math

import sysx
from PyQt4 import QtCore, QtGui

def add2tuples(x,y):
	if( len(x) < 3 or len(y) < 3 ):
		return
	return (x[0]+y[0], x[1]+y[1], x[2]+y[2]) 

def scalarMult(s,t):
	if( len(t) < 3 ):
		return
	return (s*t[0],s*t[1],s*t[2])

def deleteData(faces,edges,verts):
	for f in faces: 
		destroyFace(f)
	for e in edges:
		destroyEdge(e)
	for v in verts:
		destroyVertex(v)

def connEdges(e1,f1,e2,f2):
	c1 = getCorner(e1,f1)
	c2 = getCorner(e2,f2)
	c1p = saveCorner(c1)
	c2p = saveCorner(c2)
	if( c1 < 0 or c2 < 0 ):
		return
	c1n = next(c1)
	c2n = next(c2)
	c1np = saveCorner(c1n)
	c2np = saveCorner(c2n)
	if( next(c1) != c2n and prev(c1) != c2n ):
		e,c1,c2n,c3,c4 = insertEdge(c1,c2n,False)
	c1  = restoreCorner(c1p)
	c2  = restoreCorner(c2p)
	c1n = restoreCorner(c1np)
	c2n = restoreCorner(c2np)
	if( next(c1n) != c2 and prev(c1n) != c2 ):
		e,c1n,c2,c3,c4 = insertEdge(c1n,c2,False)

def doosabin():
	"""Doo-Sabin Remeshing Algorithm"""
	# Store old info to delete later
	oldfaces = faces(0)
	oldedges = edges(0)
	oldverts = verts(0)
	edgestart = oldedges[0] # edge start for offset
	firstface = oldfaces[0]
	# Initialize to all -1's to size of oldedges
	elist1 = [-1]
	elist1 *= len(oldedges)
	elist2 = [-1] 
	elist2 *= len(oldedges)
	# Compute the coordinates for each face
	for f in oldfaces:
		vv,ee = walk(f)
		coeff = 0.0;
		numVerts = len(vv)
		newvertexcoords = []
		# compute the new coordinates
		i=-1
		for v1 in vv:
			i = i+1
			vcoordnew = (0,0,0)
			j=-1
			for v2 in vv:
				j = j+1
				vcoord = vertexInfo(v2)['coords']
				if i == j:
					coeff = 0.25 + 5.0/(4.0*numVerts)
				else:
					coeff = ( 3.0+2.0*math.cos(2.0*(i-j)*math.pi/numVerts) )/( 4.0*numVerts )
				vcoordnew = add2tuples( vcoordnew, scalarMult(coeff,vcoord))
			newvertexcoords += [vcoordnew] # newvertexcoords is a list of tuples so we must use []
		# create a new face with the new coordinates
		nf1,nf2 = createFace(newvertexcoords) # will mark nf1,nf2 as "new"
		# create an array of corners from which connections can be made
		vw,ew = walk(f)
		vw2,ew2 = walk(nf2)
		while len(ew) > 0 and len(ew2) > 0:
			e1 = ew.pop(0)
			e2 = ew2.pop()
			eindex = e1 - edgestart
			if elist1[eindex] == -1:
				elist1[eindex] = e2
			else:
				elist2[eindex] = e2
	# Delete old stuff
	deleteData(oldfaces,oldedges,oldverts)
	# Go through and connect half-edges
	i = 0
	while i < len(oldedges):
		if elist1[i] != -1 and elist2[i] != -1 :
			# Check Edge List 1
			einfo1 = edgeInfo(elist1[i])
			ca1 = einfo1['cornerA']
			cb1 = einfo1['cornerB']
			if faceInfo(ca1[0])['type'] == "new":
				f1 = ca1[0]
			elif faceInfo(cb1[0])['type'] == "new":
				f1 = cb1[0]
			# Check Edge List 2
			einfo2 = edgeInfo(elist2[i])
			ca2 = einfo2['cornerA']
			cb2 = einfo2['cornerB']
			if faceInfo(ca2[0])['type'] == "new":
				f2 = ca2[0]
			elif faceInfo(cb2[0])['type'] == "new":
				f2 = cb2[0]
			#connectEdges((elist1[i], f1), (elist2[i], f2), False)
			connEdges(elist1[i], f1, elist2[i], f2)
			#return
		else :
			return "NULL pointers found"
		i = i + 1
	# Done with Doo-Sabin remeshing algorithm


	class MyWidget(QtGui.QWidget):
	    def __init__(self, parent=None):
	        QtGui.QWidget.__init__(self, parent)

	        self.setFixedSize(200, 120)

	        self.quit = QtGui.QPushButton("Quit", self)
	        self.quit.setGeometry(62, 40, 75, 30)
	        self.quit.setFont(QtGui.QFont("Times", 18, QtGui.QFont.Bold))

	        self.connect(self.quit, QtCore.SIGNAL("clicked()"),
	                     QtGui.qApp, QtCore.SLOT("quit()"))


	app = QtGui.QApplication(sys.argv)
	widget = MyWidget()
	widget.show()
	sys.exit(app.exec_())
	
# Perform...
#load("/Users/stuart/topmod/topmodx/cube.obj")
#test()
#doosabin()
#doosabin()
#save("/Users/stuart/Desktop/test.obj")
