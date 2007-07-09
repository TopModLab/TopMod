#include "TMPatchFace.hh"

// Resize the patch array depending on number of corners in the DLFLFace
void TMPatchFace::resizePatchArray(void) {
  if ( dlflface != NULL ) {
    patcharray.resize(dlflface->size());
    for (uint i=0; i < patcharray.size(); ++i) {
			//if( patcharray[i] == NULL )
			//patcharray[i] = new TMPatch;
      patcharray[i].resizePatch(patchsize);
		}
  }
}

// Resize all patches
void TMPatchFace::resizePatches(int psize) {
  if ( psize != patchsize ) {
    patchsize = psize;
    for (uint i=0; i < patcharray.size(); ++i) {
      patcharray[i].resizePatch(patchsize);
		}
  }
}
     
// Create the patches using face information
void TMPatchFace::createPatches(TMPatchMap &patchMap) {
  if ( dlflface == NULL ) return;

  // patcharray will be resized here
  // This is to take care of situations where the no. of corners in the face has
  // changed after it was associated with a TMPatchFace
  resizePatchArray();
	 
  // Get the centroid and normal for the face - these will be used in all patches
  Vector3d vface = dlflface->getAuxCoords();
  Vector3d nface = dlflface->getAuxNormal();
	 
  // Get the corners of the face as an array
  // A patch will be created for each corner
  DLFLFaceVertexPtrArray corners;
  int size;
  dlflface->getCorners(corners);
  size = corners.size();

  Vector3d cp[4][4]; // Grid of control points
  Vector3d cn[4][4]; // Grid of control normals used to modify the control points

  for (int i=0; i < size; ++i) {
    cp[0][0] = corners[i]->vertex->getAuxCoords();
    cn[0][0] = corners[i]->vertex->getAuxNormal();

    cp[1][0] = corners[i]->vnext()->getDS2Coord(0);
    cn[1][0] = cn[0][0];

    cp[1][1] = corners[i]->getDS2Coord(0);
    cn[1][1] = cn[0][0];

    cp[0][1] = corners[i]->vprev()->getDS2Coord(0);
    cn[0][1] = cn[0][0];


    cp[3][0] = corners[i]->getEdgePtr()->getAuxCoords();
    cn[3][0] = corners[i]->getEdgePtr()->getAuxNormal();

    cp[3][1] = corners[i]->next()->getDS2Coord(3);
    cn[3][1] = cn[3][0];

    cp[2][1] = corners[i]->getDS2Coord(1);
    cn[2][1] = cn[3][0];

    cp[2][0] = corners[i]->vnext()->getDS2Coord(3);
    cn[2][0] = cn[3][0];
              

    cp[3][3] = vface;
    cn[3][3] = nface;

    cp[2][3] = corners[i]->prev()->getDS2Coord(2);
    cn[2][3] = cn[3][3];

    cp[2][2] = corners[i]->getDS2Coord(2);
    cn[2][2] = cn[3][3];

    cp[3][2] = corners[i]->next()->getDS2Coord(2);
    cn[3][2] = cn[3][3];


    cp[0][3] = corners[i]->prev()->getEdgePtr()->getAuxCoords();
    cn[0][3] = corners[i]->prev()->getEdgePtr()->getAuxNormal();

    cp[0][2] = corners[i]->vprev()->getDS2Coord(1);
    cn[0][2] = cn[0][3];

    cp[1][2] = corners[i]->getDS2Coord(3);
    cn[1][2] = cn[0][3];

    cp[1][3] = corners[i]->prev()->getDS2Coord(1);
    cn[1][3] = cn[0][3];


    // Adjust the points along the edges of the patch
    cp[1][0] = planarProjectAndIntersect(cp[0][0],cp[3][0],cp[1][1],cp[1][0],cp[0][0],cn[0][0]);
    cp[2][0] = planarProjectAndIntersect(cp[0][0],cp[3][0],cp[2][1],cp[2][0],cp[3][0],cn[3][0]);

    cp[3][1] = planarProjectAndIntersect(cp[3][0],cp[3][3],cp[2][1],cp[3][1],cp[3][0],cn[3][0]);
    cp[3][2] = planarProjectAndIntersect(cp[3][0],cp[3][3],cp[2][2],cp[3][2],cp[3][3],cn[3][3]);

    cp[2][3] = planarProjectAndIntersect(cp[3][3],cp[0][3],cp[2][2],cp[2][3],cp[3][3],cn[3][3]);
    cp[1][3] = planarProjectAndIntersect(cp[3][3],cp[0][3],cp[1][2],cp[1][3],cp[0][3],cn[0][3]);

    cp[0][2] = planarProjectAndIntersect(cp[0][3],cp[0][0],cp[1][2],cp[0][2],cp[0][3],cn[0][3]);
    cp[0][1] = planarProjectAndIntersect(cp[0][3],cp[0][0],cp[1][1],cp[0][1],cp[0][0],cn[0][0]);

    // Adjust the central points
    planarProject(cp[1][1],cp[0][0],cn[0][0]);
    planarProject(cp[2][1],cp[3][0],cn[3][0]);
    planarProject(cp[2][2],cp[3][3],cn[3][3]);
    planarProject(cp[1][2],cp[0][3],cn[0][3]);

    // Adjust the control points
    cp[1][0] = cp[0][0] + (cp[1][0]-cp[0][0])*4.0/3.0;
    cp[1][1] = cp[0][0] + (cp[1][1]-cp[0][0])*4.0/3.0;
    cp[0][1] = cp[0][0] + (cp[0][1]-cp[0][0])*4.0/3.0;

    cp[2][0] = cp[3][0] + (cp[2][0]-cp[3][0])*4.0/3.0;
    cp[2][1] = cp[3][0] + (cp[2][1]-cp[3][0])*4.0/3.0;
    cp[3][1] = cp[3][0] + (cp[3][1]-cp[3][0])*4.0/3.0;

    cp[3][2] = cp[3][3] + (cp[3][2]-cp[3][3])*4.0/3.0;
    cp[2][2] = cp[3][3] + (cp[2][2]-cp[3][3])*4.0/3.0;
    cp[2][3] = cp[3][3] + (cp[2][3]-cp[3][3])*4.0/3.0;

    cp[0][2] = cp[0][3] + (cp[0][2]-cp[0][3])*4.0/3.0;
    cp[1][2] = cp[0][3] + (cp[1][2]-cp[0][3])*4.0/3.0;
    cp[1][3] = cp[0][3] + (cp[1][3]-cp[0][3])*4.0/3.0;

    // Adjust the normals for interior and edge middle points
    cn[1][0] = normalized(3*cn[0][0]+cn[3][0]);
    cn[0][1] = normalized(3*cn[0][0]+cn[0][3]);
    cn[1][1] = normalized(6*cn[0][0]+cn[0][3]+cn[3][0]);

    cn[2][0] = normalized(3*cn[3][0]+cn[0][0]);
    cn[3][1] = normalized(3*cn[3][0]+cn[3][3]);
    cn[2][1] = normalized(6*cn[3][0]+cn[0][0]+cn[3][3]);

    cn[0][2] = normalized(3*cn[0][3]+cn[0][0]);
    cn[1][3] = normalized(3*cn[0][3]+cn[3][3]);
    cn[1][2] = normalized(6*cn[0][3]+cn[0][0]+cn[3][3]);

    cn[2][3] = normalized(3*cn[3][3]+cn[0][3]);
    cn[3][2] = normalized(3*cn[3][3]+cn[3][0]);
    cn[2][2] = normalized(6*cn[3][3]+cn[0][3]+cn[3][0]);
              
    patcharray[i].calculatePatchPoints(cp,cn);
		setPatchPtr(patchMap, &(patcharray[i]), corners[i] );
  }

  // Make adjustment to face point for quadrilaterals
  if ( size == 4 ) {
    TMPatchPtr pptr1, pptr2;
    pptr1 = getPatchPtr(patchMap,corners[0]);
    pptr2 = getPatchPtr(patchMap,corners[2]);

		if( pptr1 == NULL || pptr2 == NULL )
			return;

    Vector3d p00,p01,p10,p11,ip;
    p00 = pptr1->getControlPoint(3,2); p01 = pptr2->getControlPoint(3,2);
    p10 = pptr1->getControlPoint(2,3); p11 = pptr2->getControlPoint(2,3);
    ip = intersectCoplanarLines(p00,p01,p10,p11);

    for (int i=0; i < size; ++i) {
      pptr1 = getPatchPtr(patchMap,corners[i]);
      pptr1->setControlPoint(3,3,ip);
      pptr1->updateGLPointArray();
    }
  }
}

// Adjust the edge points for each patch in the face
void TMPatchFace::adjustEdgePoints(TMPatchMap &patchMap) {
  if ( dlflface == NULL ) return;
  DLFLFaceVertexPtrArray corners; dlflface->getCorners(corners);
  DLFLFaceVertexPtr fvp, nfvp, pfvp;
  TMPatchPtr pptr, npptr, ppptr;
  Vector3d p00,p01,p10,p11,ip;
  for (uint i=0; i < corners.size(); ++i) {
    fvp = corners[i]; pptr = getPatchPtr(patchMap,fvp);
    nfvp = corners[i]->getOppositeCorner(); npptr = getPatchPtr(patchMap,nfvp);
    pfvp = corners[i]->prev()->vnext(); ppptr = getPatchPtr(patchMap,pfvp);

    p00 = pptr->getControlPoint(2,0); p01 = npptr->getControlPoint(2,0);
    p10 = pptr->getControlPoint(3,1); p11 = npptr->getControlPoint(3,1);
    ip = intersectCoplanarLines(p00,p01,p10,p11);
    pptr->setControlPoint(3,0,ip);
    npptr->setControlPoint(3,0,ip);

    p00 = pptr->getControlPoint(0,2); p01 = ppptr->getControlPoint(0,2);
    p10 = pptr->getControlPoint(1,3); p11 = ppptr->getControlPoint(1,3);
    ip = intersectCoplanarLines(p00,p01,p10,p11);
    pptr->setControlPoint(0,3,ip);
    ppptr->setControlPoint(0,3,ip);
              
    pptr->updateGLPointArray();
    npptr->updateGLPointArray();
    ppptr->updateGLPointArray();
  }
}
