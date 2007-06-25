from dlfl import *
import math

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

		
def doosabin():
	"""Doo-Sabin Remeshing Algorithm"""
	# Store old info to delete later
	oldfaces = faces()
	oldedges = edges()
	oldverts = verts()
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
		cc = cornerWalk(f)
		cc2 = cornerWalk(nf2)
		while len(cc) > 0 and len(cc2) > 0:
			e1 = cornerInfo(cc.pop(0))['edge'] # pop first item
			e2 = cornerInfo(cc2.pop())['edge'] # pop last item
			eindex = e1 - edgestart
			# print e1, edgestart, eindex
			if elist1[eindex] == -1:
				elist1[eindex] = e2
			else:
				elist2[eindex] = e2
	# Delete old stuff
	deleteData(oldfaces,oldedges,oldverts)
	# Go through and connect half-edges
	i = 0
	while i < len(elist1):
		if elist1[i] != -1 and elist2[i] != -1 :
			# Check Edge List 1
			einfo1 = edgeInfo(elist1[i])
			ca1 = einfo1['cornerA']
			cb1 = einfo1['cornerB']
			# print ca1,faceInfo(ca1[0])['type'],";",cb1,faceInfo(cb1[0])['type']
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
			# print 'connect:', elist1[i], f1, elist2[i]
			connectEdges((elist1[i], f1), (elist2[i], f2), False)
		else :
			return "Null pointers found"
		i = i + 1
	# Done with Doo-Sabin remeshing algorithm


# Perform...
load("cube.obj")
doosabin()
doosabin()
save("dscube.obj")