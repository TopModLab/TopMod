#include "TMPatchObject.hh"

void TMPatchObject::updateForPatches( DLFLObjectPtr obj ) {
  DLFLFacePtrList face_list = obj->getFaceList( );
  DLFLEdgePtrList edge_list = obj->getEdgeList( );
  DLFLVertexPtrList vertex_list = obj->getVertexList( );

  // Update information stored at each face, vertex, edge and corner for patch rendering

  // Compute doo-sabin coordinates for each face and store them in the auxcoord field of the corner
  // Update the auxcoord field of the face
  Vector3dArray coords;
  DLFLFacePtrList :: iterator ffirst=face_list.begin(), flast=face_list.end();
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
  DLFLEdgePtrList :: iterator efirst=edge_list.begin(), elast=edge_list.end();
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
  DLFLVertexPtrList :: iterator vfirst=vertex_list.begin(), vlast=vertex_list.end();
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
