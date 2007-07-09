#include "TMPatchObject.hh"

void TMPatchObject::updateForPatches( DLFLObjectPtr obj ) {
  DLFLFacePtrList face_list = obj->getFaceList( );
  DLFLEdgePtrList edge_list = obj->getEdgeList( );
  DLFLVertexPtrList vertex_list = obj->getVertexList( );

  // Update information stored at each face, vertex, edge and corner for patch rendering

  // Compute doo-sabin coordinates for each face and store them in the auxcoord field of the corner
  // Update the auxcoord field of the face
  Vector3dArray coords;
  DLFLFacePtrList::iterator ffirst=face_list.begin(), flast=face_list.end();
  DLFLFacePtr fp;
  DLFLFaceVertexPtrArray corners;
  int valence;
       
  while ( ffirst != flast ) {
    fp = *ffirst; ++ffirst;
    fp->getCornersAndCoords(corners,coords);
    valence = coords.size();

    if ( valence > 0 ) {
      // Compute Doo-Sabin coordinates - Level 1
      DLFL::computeDooSabinCoords(coords);
      for (int i=0; i < valence; ++i) 
				corners[i]->setAuxCoords(coords[i]);

      // Compute Doo-Sabin coordinates - Level 2
      DLFL::computeDooSabinCoords(coords);
      for (int i=0; i < valence; ++i) 
				corners[i]->setDS2Coord2(coords[i]);

      // Compute the patch point and patch normal
      Vector3d pp, pn;
      DLFL::computeCentroidAndNormal(coords,pp,pn);
      fp->setAuxCoords(pp); fp->setAuxNormal(pn);
    }
  }

  // Compute patch point and normal for all edges
  DLFLEdgePtrList::iterator efirst=edge_list.begin(), elast=edge_list.end();
  DLFLEdgePtr ep;
  while ( efirst != elast ) {
    ep = *efirst; ++efirst;

    Vector3dArray p;
    ep->getEFCornersAuxCoords(p);
            
    // Compute Doo-Sabin coordinates - Level 2
    DLFL::computeDooSabinCoords(p);

    Vector3d pp,pn;
    computeCentroidAndNormal(p,pp,pn);
    ep->setAuxCoords(pp); ep->setAuxNormal(pn);

    DLFLFaceVertexPtrArray fvp;
    ep->getEFCorners(fvp);
    fvp[0]->setDS2Coord3(p[0]); fvp[1]->setDS2Coord1(p[1]);
    fvp[2]->setDS2Coord3(p[2]); fvp[3]->setDS2Coord1(p[3]);
  }

  // Compute patch point and normal for all vertices
  DLFLVertexPtrList::iterator vfirst=vertex_list.begin(), vlast=vertex_list.end();
  DLFLVertexPtr vp;
  while ( vfirst != vlast ) {
    vp = *vfirst; ++vfirst;
    Vector3dArray p;
    vp->getOrderedCornerAuxCoords(p);

    // Compute Doo-Sabin coordinates - Level 2
    DLFL::computeDooSabinCoords(p);

    Vector3d pp,pn;
    DLFL::computeCentroidAndNormal(p,pp,pn);
    vp->setAuxCoords(pp); vp->setAuxNormal(-pn); // Reverse the normal since the rotation order around the vertex is clockwise
            
    DLFLFaceVertexPtrArray fvp;
    vp->getOrderedCorners(fvp);
    for (int i=0; i < fvp.size(); ++i) 
      fvp[i]->setDS2Coord0(p[i]);
  }
}

void TMPatchObject::for_each(void (TMPatchFace::*func)(void)) {
  for( it = patch_list.begin(); it != patch_list.end(); it++ ) {
    TMPatchFacePtr pfp = *it;
    (pfp->*func)();
  }
}

// Free the memory allocated for the patches
void TMPatchObject::destroyPatches() {
	TMPatchFacePtrList::iterator first = patch_list.begin(), last = patch_list.end();
	TMPatchFacePtr pfp = NULL;
	while ( first != last ) {
		pfp = (*first); ++first;
		delete pfp;
	}
	patch_list.clear();
}

// Build the list of patch faces
void TMPatchObject::createPatches( DLFLObjectPtr obj ) {
  destroyPatches();
	destroyPatchMap( patchMap );
  DLFLFacePtrList face_list = obj->getFaceList( );
	DLFLFacePtrList::iterator ffirst = face_list.begin(), flast = face_list.end();
	DLFLFacePtr fp = NULL;
	TMPatchFacePtr pfp = NULL;

	while ( ffirst != flast ) {
		fp = (*ffirst); ++ffirst;
		pfp = new TMPatchFace(patchsize);
		pfp->setDLFLFace(fp); 
		pfp->createPatches(patchMap);
		patch_list.push_back(pfp);
	}

	// Adjust the edge points for all patches
	DLFLEdgePtrList edge_list = obj->getEdgeList();
	DLFLEdgePtrList::iterator efirst = edge_list.begin(), elast = edge_list.end();
	DLFLEdgePtr ep = NULL;
	DLFLFaceVertexPtr fvp1,fvp2;
	TMPatchPtr pp1, pp2;
	Vector3d p00,p01,p10,p11,ip;
	while ( efirst != elast ) {
		ep = (*efirst); ++efirst;
		ep->getCorners(fvp1,fvp2);
		pp1 = getPatchPtr(patchMap,fvp1); pp2 = getPatchPtr(patchMap,fvp2);

		if( pp1 == NULL || pp2 == NULL )
			return;

		p00 = pp1->getControlPoint(2,0); 
		p01 = pp2->getControlPoint(2,0);
		p10 = pp1->getControlPoint(3,1); 
		p11 = pp2->getControlPoint(3,1);
		ip = intersectCoplanarLines(p00,p01,p10,p11);

		pp1->setControlPoint(3,0,ip); pp2->setControlPoint(3,0,ip);
		pp1->updateGLPointArray(); pp2->updateGLPointArray();

		pp1 = getPatchPtr(patchMap,fvp1->next()); pp2 = getPatchPtr(patchMap,fvp2->next());
		pp1->setControlPoint(0,3,ip); pp2->setControlPoint(0,3,ip);
		pp1->updateGLPointArray(); pp2->updateGLPointArray();
	}

	// Adjust the vertex points for 4-valence vertices
	DLFLVertexPtrList vertex_list = obj->getVertexList( );
	DLFLVertexPtrList::iterator vfirst = vertex_list.begin(), vlast = vertex_list.end();
	DLFLVertexPtr vp = NULL;
	while ( vfirst != vlast ) {
		vp = (*vfirst); ++vfirst;
		if ( vp->valence() == 4 ) {
			DLFLFaceVertexPtrArray vcorners;
			vp->getOrderedCorners(vcorners);
			pp1 = getPatchPtr(patchMap,vcorners[0]); pp2 = getPatchPtr(patchMap,vcorners[2]);

			p00 = pp1->getControlPoint(1,0); p01 = pp2->getControlPoint(1,0);
			p10 = pp1->getControlPoint(0,1); p11 = pp2->getControlPoint(0,1);
			ip = intersectCoplanarLines(p00,p01,p10,p11);
				
			for( int i = 0; i < 4; ++i ) {
				pp1 = getPatchPtr(patchMap,vcorners[i]);
				pp1->setControlPoint(0,0,ip);
				pp1->updateGLPointArray();
			}
		}
	}
                 
	/*
		TMPatchFacePtrList::iterator pfirst = patch_list.begin(), plast = patch_list.end();
		while ( pfirst != plast ) {
		pfp = (*pfirst); ++pfirst;
		pfp->adjustEdgePoints(patchMap);
		}
	*/
}

void TMPatchObject::setPatchSize( int size, DLFLObjectPtr obj ){
  if ( size != patchsize && size > 0 ) {
    patchsize = size;
    if( !obj ) { obj = mObj; }
    if( !obj ) { return; }
    createPatches( obj );
  }
}

/* stuart - bezier export */
void TMPatchObject::objPatchWrite( ostream& o ) {
  o << "g patches" << std::endl
    << "mg 1 0.5"  << std::endl << std::endl;
  TMPatchFacePtrList::const_iterator first = patch_list.begin(), last = patch_list.end();
  int i = 0;
  int v = 0;
  while( first != last ) {
    o << "# Face " << i+1 << std::endl;
    int npatches = (*first)->print(o);
    int j = 0;
    while( j < npatches ) {
      o << "# Patch" << i+j+1 << std::endl;
      o << "cstype bezier" << std::endl
	<< "deg 3 3" << std::endl
	<< "surf 0.000000 1.000000 0.000000 1.000000 " 
	<< 1+v << " " << 2+v << " " << 3+v << " " << 4+v << " " << 5+v << " " << 6+v << " " << 7+v << " " << 8+v << " " << 9+v << " " << 10+v << " " << 11+v << " " << 12+v << " " << 13+v << " " << 14+v << " " << 15+v << " " << 16+v << std::endl
	<< "parm u 0.000000 1.000000" << std::endl
	<< "parm v 0.000000 1.000000" << std::endl
	<< "end" << std::endl << std::endl;
      j++;
      v += 16;
    }
    ++first;
    i++;
  }
}
