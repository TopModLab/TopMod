from dlfl import *
from math import *

def scalarAdd(a,tuple):
    if( len(tuple) < 3 ):
        return
    return (tuple[0]+a,tuple[1]+a,tuple[2]+a)

def scalarMult(s,tuple):
	if( len(tuple) < 3 ):
		return
	return (s*tuple[0],s*tuple[1],s*tuple[2])

def vectorAdd(x,y):
    if( len(x) < 3 or len(y) < 3 ):
        return
    return (x[0]+y[0], x[1]+y[1], x[2]+y[2])

def vectorMinus(x,y):
    if( len(x) < 3 or len(y) < 3 ):
        return
    return (x[0]-y[0], x[1]-y[1], x[2]-y[2])

#def rotate(tuple,theta):
#    if( len(tuple) != 3 ):
#        return

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

def myextrude( faceid, dist, segs ):
    for i in range(segs):
        # Grab face normal
        norm = faceInfo(faceid)['normal']
        norm = scalarMult(dist,norm)
    # Grab the vertices of the face
        vv,ee = walk(faceid)
        newvertexcoords = []
        for v in vv:
            vcoord = vertexInfo(v)['coords']
            vcoord = vectorAdd(vcoord,norm)
            newvertexcoords += [vcoord]
        f1,f2 = createFace(newvertexcoords)
        vsoldface,esoldface = walk(faceid)
        vsnewface,esnewface = walk(f2)
        while len(esnewface) > 0 and len(esoldface) > 0:
            enew = esnewface.pop(0)
            eold = esoldface.pop()
            einfo1 = edgeInfo(enew)
            fnew = einfo1['cornerB'][0]
            if( faceInfo(einfo1['cornerA'][0]) == "new"):
                fnew = einfo1['cornerA'][0]
            einfo2 = edgeInfo(eold)
            fold = einfo2['cornerA'][0]
            if( faceInfo(einfo2['cornerB'][0]) == "new"):
                fnew = einfo2['cornerB'][0]
            print enew, fnew, eold, fold
            connEdges(enew,fnew,eold,fold)
        faceid = f1
    return faceid

def mywaveextrude( faceid, dist, segs ):
    currentface = faceid
    faceinfo = faceInfo(faceid)
    norm = faceinfo['normal']
    centroid = faceinfo['centroid']
    for i in range(segs):
        # Grab face normal
        cdist = dist*(i+1)
        norm = scalarMult(cdist,norm)
        vv,ee = walk(faceid)
        newvertexcoords = []
        for v in vv:
            vcoord = vertexInfo(v)['coords']
            vcoord = vectorMinus(vcoord,centroid)
            vcoord = scalarMult(0.5*sin(2.1*cdist)+1.1*cos(0.6*cdist),vcoord)
            vcoord = vectorAdd(vcoord,centroid)            
            vcoord = vectorAdd(vcoord,norm)
            newvertexcoords += [vcoord]
        f1,f2 = createFace(newvertexcoords)
        vsoldface,esoldface = walk(currentface)
        vsnewface,esnewface = walk(f2)
        while len(esnewface) > 0 and len(esoldface) > 0:
            enew = esnewface.pop(0)
            eold = esoldface.pop()
            einfo1 = edgeInfo(enew)
            fnew = einfo1['cornerB'][0]
            einfo2 = edgeInfo(eold)
            fold = einfo2['cornerA'][0]
            print enew, fnew, eold, fold
            connEdges(enew,fnew,eold,fold)
        return
        currentFace = f1
        norm = faceinfo['normal']
    return faceid
