/* $Id: DLFLExtrude.cc,v 4.2 2004/01/13 02:46:13 vinod Exp $ */

// Non-inline functions definitions for class DLFLObject
// Subroutines that are related to extrusions

#include "DLFLAux.hh"
#include "DLFLObject.hh"

DLFLFacePtr DLFLObject::duplicateFace(DLFLFacePtr fptr, double offset, double rot, double sf)
{
     // Duplicate the given face, use face normal for direction of offset
  Vector3d dir = fptr->computeNormal();
  return duplicateFace(fptr,dir,offset,rot,sf);
}

DLFLFacePtr DLFLObject::duplicateFace(DLFLFacePtr fptr, const Vector3d& dir,
                                        double offset, double rot, double sf)
{
     // Duplicate given face, offsetting, scaling and rotating if necessary
  DLFLFacePtr endface = NULL;

     // First compute the coordinates of the vertices of the new points and store
     // them in an array
  DLFLFaceVertexPtr head;
  head = fptr->front();
  if ( head )
     {
       Vector3d ndir = normalized(dir);
       Vector3dArray newverts;

       fptr->getVertexCoords(newverts);

          // Scale the new vertices about their centroid if scale factor is not 1.0 or 0.0
       sf = Abs(sf);
       if ( isNonZero(sf) && ( Abs(sf-1.0) > ZERO ) ) scale(newverts,sf);
       
          // Rotate the new vertices if rotation is not 0.0
       if ( isNonZero(rot) ) rotate(newverts,ndir,rot*M_PI/180.0);
       
          // Translate the new vertices by given amount along given direction
       if ( Abs(offset) > ZERO )
          {
            translate(newverts,ndir,offset);
          }

       createFace(newverts,fptr->material());

          // Get pointer to the first newly created face (second from last)
       DLFLFacePtrList::reverse_iterator rfirst = face_list.rbegin();
       ++rfirst; endface = (*rfirst);
     }
  return endface;
}

DLFLFacePtr DLFLObject::duplicateFacePlanarOffset(DLFLFacePtr fptr,
                                                    double offset, double rot, double thickness,
                                                    bool fractionalthickness)
{
     // Duplicate the given face, use face normal for direction of offset
     // Offset the vertices in the plane of the face along
     // the angular bisectors by specified thickness.
  Vector3d dir = fptr->computeNormal();
  return duplicateFacePlanarOffset(fptr,dir,offset,rot,thickness,fractionalthickness);
}

DLFLFacePtr DLFLObject::duplicateFacePlanarOffset(DLFLFacePtr fptr, const Vector3d& dir,
                                                    double offset, double rot, double thickness,
                                                    bool fractionalthickness)
{
     // Duplicate given face, offsetting (along normal), and rotating if necessary
     // Offset the vertices in the plane of the face along
     // the angular bisectors by specified thickness.
     // Boolean flag indicates if thickness is absolute or fraction of edge length.
  DLFLFacePtr endface = NULL;

     // First compute the coordinates of the vertices of the new points and store
     // them in an array
  DLFLFaceVertexPtr head,fvp;
  head = fptr->front();
  if ( head )
     {
       Vector3d ndir = normalized(dir);
       Vector3dArray newverts;

       fptr->getVertexCoords(newverts);

          // Offset the corners by thickness using the edge vectors to determine direction
          // If boolean flag is set thickness is assumed to be a fraction of edge length.
          // Traverse the face and the coordinate array simultaneously and adjust coordinates
       int i=0, numverts=newverts.size();
       Vector3d pvec, nvec, ovec;
       fvp = head;

          // Put if statement outside of loop for speed
       if ( fractionalthickness )
          {
               // Thickness is relative to edge lengths, use fractions of edge vectors
            do
               {
                 fvp->getEdgeVectors(pvec,nvec); // Edge vectors originating at fvp

                    // If fvp is a winged corner we need to find the new coordinates differently
                 if ( fvp->isWingedCorner() )
                    {
                         // Find the next non-winged corner. It can be concave or convex
                      DLFLFaceVertexPtr nwfvp = fvp->nextNonWingedCorner();
                      if ( nwfvp == NULL )
                         {
                              // This situation should not occur
                           cout << "Something went wrong somewhere..." << endl;
                           return NULL;
                         }

                         // Find the normal at this non-winged corner.
                         // Normal will be adjusted for concave corners
                      Vector3d nwfvpn = nwfvp->computeNormal();

                         // Use this normal to find a vector starting at fvp
                         // and pointing into the face and perpendicular to
                         // the two edges coincident at fvp
                         // This will be the offset vector direction
                      ovec = nwfvpn % nvec; normalize(ovec);
                      ovec *= thickness;
                    }
                 else
                    {
                         // Compute the offset vector using the edge vectors
                      ovec = thickness*(pvec + nvec);

                         // If this corner is a concave corner, flip the offset vector
                      if ( fvp->isConcaveCorner() ) ovec = -ovec;
                    }

                    // Adjust the coordinates of the new vertex using the offset vector
                 newverts[i] += ovec;
                 fvp = fvp->next(); i++;
               }
            while ( fvp != head );
          }
       else
          {
               // Thichness is absolute, use the normalized edge vectors
            do
               {
                 fvp->getEdgeVectors(pvec,nvec); // Edge vectors originating at fvp

                 normalize(pvec); normalize(nvec);

                    // If fvp is a winged corner we need to find the new coordinates differently
                 if ( fvp->isWingedCorner() )
                    {
                         // Find the next non-winged corner. It can be concave or convex
                      DLFLFaceVertexPtr nwfvp = fvp->nextNonWingedCorner();
                      if ( nwfvp == NULL )
                         {
                              // This situation should not occur
                           cout << "Something went wrong somewhere..." << endl;
                           return NULL;
                         }

                         // Find the normal at this non-winged corner.
                         // Normal will be adjusted for concave corners
                      Vector3d nwfvpn = nwfvp->computeNormal();

                         // Use this normal to find a vector starting at fvp
                         // and pointing into the face and perpendicular to
                         // the two edges coincident at fvp
                         // This will be the offset vector direction
                      ovec = nwfvpn % nvec;
                      ovec *= thickness;
                    }
                 else
                    {
                         // Compute the offset vector using the edge vectors
                      ovec = thickness*(pvec + nvec);

                         // If this corner is a concave corner, flip the offset vector
                      if ( fvp->isConcaveCorner() ) ovec = -ovec;
                    }

                    // Adjust the coordinates of the new vertex using the offset vector
                 newverts[i] += ovec;
                 fvp = fvp->next(); i++;
               }
            while ( fvp != head );
          }

          // Rotate the new vertices if rotation is not 0.0
       if ( isNonZero(rot) ) rotate(newverts,ndir,rot*M_PI/180.0);
       
          // Translate the new vertices by given amount along given direction
       if ( Abs(offset) > ZERO )
          {
            translate(newverts,ndir,offset);
          }

       createFace(newverts,fptr->material());

          // Get pointer to the first newly created face (second from last)
       DLFLFacePtrList::reverse_iterator rfirst = face_list.rbegin();
       ++rfirst; endface = (*rfirst);
     }
  return endface;
}

DLFLFacePtr DLFLObject::extrudeFace(DLFLFacePtr fptr, double d)
{
     // Extrude the given face along its normal for a given distance
  Vector3d dir = fptr->computeNormal();
  normalize(dir);
  return extrudeFace(fptr,d,dir,0.0,1.0);
}

DLFLFacePtr DLFLObject::extrudeFace(DLFLFacePtr fptr, double d, int num)
{
  DLFLFacePtr exface = fptr;
  for (int i=0; i < num; ++i)
     {
       exface = extrudeFace(exface,d);
     }
  return exface;
}

DLFLFacePtr DLFLObject::extrudeFace(DLFLFacePtr fptr, double d, double rot, double sf)
{
     // Extrude the given face along its normal for a given distance
     // Rotate and scale the new face w.r.t. old face by given parameters
  Vector3d dir = fptr->computeNormal();
  normalize(dir);
  return extrudeFace(fptr,d,dir,rot,sf);
}

DLFLFacePtr DLFLObject::extrudeFace(DLFLFacePtr fptr, double d, int num, double rot, double sf)
{
  DLFLFacePtr exface = fptr;
  for (int i=0; i < num; ++i)
     {
       exface = extrudeFace(exface,d,rot,sf);
     }
  return exface;
}

DLFLFacePtr DLFLObject::extrudeFace(DLFLFacePtr fptr, double d, const Vector3d& dir)
{
  return extrudeFace(fptr,d,dir,0.0,1.0);
}

DLFLFacePtr DLFLObject::extrudeFace(DLFLFacePtr fptr, double d, const Vector3d& dir,
                                      int num)
{
  DLFLFacePtr exface = fptr;
  for (int i=0; i < num; ++i)
     {
       exface = extrudeFace(exface,d,dir);
     }
  return exface;
}
  
DLFLFacePtr DLFLObject::extrudeFace(DLFLFacePtr fptr, double d, const Vector3d& dir,
                                      double rot, double sf)
{
  DLFLFacePtr endface = duplicateFace(fptr,dir,d,rot,sf);
  if ( endface != NULL )
     {
          // Get the pointer to the second newly inserted face
       DLFLFacePtr nfp = face_list.back();
       
          // The last face will be the one facing the old face
          // Find the first face-vertices in the 2 faces to be connected
       DLFLFaceVertexPtr fvp1, fvp2;
       fvp1 = fptr->firstVertex(); fvp2 = nfp->firstVertex();
       connectFaces(fvp1,fvp2);
     }
  return endface;
}

DLFLFacePtr DLFLObject::extrudeFace(DLFLFacePtr fptr, double d, const Vector3d& dir,
                                      int num, double rot, double sf)
{
  DLFLFacePtr exface = fptr;
  for (int i=0; i < num; ++i)
     {
       exface = extrudeFace(exface,d,dir,rot,sf);
     }
  return exface;
}

DLFLFacePtr DLFLObject::extrudeFacePlanarOffset(DLFLFacePtr fptr,
                                                  double d, double rot, double thickness,
                                                  bool fractionalthickness)
{
  Vector3d dir;
  if ( isNonZero(d) || isNonZero(rot) )
     {
       dir = fptr->computeNormal();
       normalize(dir);
     }
  return extrudeFacePlanarOffset(fptr,d,dir,rot,thickness,fractionalthickness);
}

DLFLFacePtr DLFLObject::extrudeFacePlanarOffset(DLFLFacePtr fptr, double d, const Vector3d& dir,
                                                  double rot, double thickness,
                                                  bool fractionalthickness)
{
  DLFLFacePtr endface = duplicateFacePlanarOffset(fptr,dir,d,rot,thickness,fractionalthickness);
  if ( endface != NULL )
     {
          // Get the pointer to the second newly inserted face
       DLFLFacePtr nfp = face_list.back();
       
          // The last face will be the one facing the old face
          // Find the first face-vertices in the 2 faces to be connected
       DLFLFaceVertexPtr fvp1, fvp2;
       fvp1 = fptr->firstVertex(); fvp2 = nfp->firstVertex();
       connectFaces(fvp1,fvp2);
     }
  return endface;
}

DLFLFacePtr DLFLObject::extrudeFaceDS(DLFLFacePtr fptr, double d,
                                        double twist, double sf)
{
     // Extrude the given face along its normal for a given distance
  Vector3d dir = fptr->computeNormal();
  normalize(dir);
  return extrudeFaceDS(fptr,d,dir,twist,sf);
}

DLFLFacePtr DLFLObject::extrudeFaceDS(DLFLFacePtr fptr, double d,
                                        int num, double twist, double sf)
{
  DLFLFacePtr exface = fptr;
  for (int i=0; i < num; ++i)
     {
       exface = extrudeFaceDS(exface,d,twist,sf);
     }
  return exface;
}

DLFLFacePtr DLFLObject::extrudeFaceDS(DLFLFacePtr fptr, double d, const Vector3d& dir,
                                        double twist, double sf)
{
     // Extrude the given face along the given direction for a given distance
  DLFLFacePtr endface = NULL;

     // First compute the coordinates of the vertices of the new points and store
     // them in an array
  DLFLFaceVertexPtr head;
  head = fptr->front();
  if ( head )
     {
       Vector3d ndir = normalized(dir);
       Vector3dArray oldverts,newverts;

       fptr->getVertexCoords(oldverts);
       uint numverts = oldverts.size();           // No. of vertices in original face

          // New vertices will be computed using the twist factor
       newverts.resize(numverts,d*ndir);
       for (int i=0; i < numverts-1; ++i)
          {
            newverts[i] += (1.0-twist)*oldverts[i] + twist*oldverts[i+1];
          }
       newverts[numverts-1] += (1.0-twist)*oldverts[numverts-1] + twist*oldverts[0];

       double coef;
       Vector3d p;
       for (int i=0; i < numverts; ++i)
          {
            p.reset();
            for (int j=0; j < numverts; ++j)
               {
                 if ( i == j ) coef = 0.25 + 5.0/(4.0*numverts);
                 else coef = ( 3.0 + 2.0*cos(2.0*(i-j)*M_PI/numverts) ) / (4.0*numverts);
                 p += coef*newverts[j];
               }
            oldverts[i] = p;
          }

          // Scale the new vertices about their centroid if scale factor is not 1.0 or 0.0
       sf = Abs(sf);
       if ( isNonZero(sf) && ( Abs(sf-1.0) > ZERO ) ) scale(oldverts,sf);
       
       createFace(oldverts,fptr->material());

          // Get pointer to the first newly created face (second from last)
       DLFLFacePtrList::reverse_iterator rfirst = face_list.rbegin();
       ++rfirst; endface = (*rfirst);

          // Get the pointer to the second newly inserted face
       DLFLFacePtr nfp = face_list.back();
       
          // The last face will be the one facing the old face
          // Find the first face-vertices in the 2 faces to be connected
       DLFLFaceVertexPtr fvp1, fvp2;
       fvp1 = fptr->firstVertex(); fvp2 = nfp->firstVertex();
       connectFaces(fvp1,fvp2);
     }
  return endface;
}

DLFLFacePtr DLFLObject::extrudeFaceDS(DLFLFacePtr fptr, double d, const Vector3d& dir, int num, double twist, double sf)
{
  DLFLFacePtr exface = fptr;
  for (int i=0; i < num; ++i)
     {
       exface = extrudeFaceDS(exface,d,dir,twist,sf);
     }
  return exface;
}

DLFLFacePtr DLFLObject::extrudeDualFace(DLFLFacePtr fptr, double d, double rot, double sf, bool mesh)
{
     // Extrude the given face along its normal for a given distance
     // Rotate and scale the new face w.r.t. old face by given parameters
  Vector3d dir = fptr->computeNormal();
  normalize(dir);
  return extrudeDualFace(fptr,d,dir,rot,sf,mesh);
}

DLFLFacePtr DLFLObject::extrudeDualFace(DLFLFacePtr fptr, double d, int num, double rot, double sf, bool mesh)
{
  DLFLFacePtr exface = fptr;
  for (int i=0; i < num; ++i)
     {
       exface = extrudeDualFace(exface,d,rot,sf,mesh);
     }
  return exface;
}

DLFLFacePtr DLFLObject::extrudeDualFace(DLFLFacePtr fptr, double d, const Vector3d& dir, double rot, double sf, bool mesh)
{
     // Extrude the given face along the given direction for a given distance
  DLFLFacePtr endface = NULL;

     // First compute the coordinates of the vertices of the new points and store
     // them in an array
  DLFLFaceVertexPtr head;
  head = fptr->front();
  if ( head )
     {
       Vector3d ndir = normalized(dir);
       Vector3dArray newverts;

	  // If mesh flag is true, edges in the original face will be deleted.
          // Store those edges in a temporary array
       DLFLEdgePtrArray ep_arr;
       if ( mesh ) fptr->getEdges(ep_arr);

          // New face will contain the midpoints of edges of old face, appropriately transformed
       DLFLFaceVertexPtr current = head;
       newverts.push_back(current->getEdgePtr()->getMidPoint() + d*ndir);
       current = current->next();
       while ( current != head )
          {
            newverts.push_back(current->getEdgePtr()->getMidPoint() + d*ndir);
            current = current->next();
          }

          // Scale the new vertices about their centroid if scale factor is not 1.0 or 0.0
       sf = Abs(sf);
       if ( isNonZero(sf) && ( Abs(sf-1.0) > ZERO ) ) scale(newverts,sf);
       
          // Rotate the new vertices if rotation is not 0.0
       if ( isNonZero(rot) ) rotate(newverts,ndir,rot*M_PI/180.0);

          // Create the new face(s)
       createFace(newverts,fptr->material());

          // Get pointers to the newly created faces
       DLFLFacePtrList::reverse_iterator rfirst = face_list.rbegin();
       DLFLFacePtr nfp = (*rfirst);
       ++rfirst;
       endface = (*rfirst);

          // The last face (nfp) will be the one facing the old face
          // Find the first face-vertices in the 2 faces to be connected
       DLFLFaceVertexPtr fvp1, fvp2;
       fvp1 = fptr->firstVertex(); fvp2 = nfp->firstVertex();
       dualConnectFaces(fvp1,fvp2);

          // If the mesh flag is true delete the edges in the original face
       if ( mesh )
          {
            DLFLEdgePtrArray::iterator el_first, el_last;
            DLFLEdgePtr ep;

            el_first = ep_arr.begin(); el_last = ep_arr.end();
            while ( el_first != el_last )
               {
                 ep = (*el_first); ++el_first;
                 deleteEdge(ep,true); 
               }
          }
     }
  return endface;
}

DLFLFacePtr DLFLObject::extrudeDualFace(DLFLFacePtr fptr, double d, const Vector3d& dir, int num, double rot, double sf, bool mesh)
{
  DLFLFacePtr exface = fptr;
  for (int i=0; i < num; ++i)
     {
       exface = extrudeDualFace(exface,d,dir,rot,sf,mesh);
     }
  return exface;
}


void DLFLObject::stellateFace(DLFLFacePtr fptr, double d)
{
     // Stellate the given face along its normal for a given distance
  Vector3d dir = fptr->computeNormal();
  normalize(dir);
  stellateFace(fptr,d,dir);
}

void DLFLObject::stellateFace(DLFLFacePtr fptr, double d, const Vector3d& dir)
{
     // Stellation is like extrusion but creates a cone instead of a cylinder
  DLFLMaterialPtr matl = fptr->material();
  DLFLEdgePtr lastedge;
  DLFLFacePtr fptr1;
  DLFLFaceVertexPtr fvp1, fvp2;
  int numsides1;
  bool done;

     // Create the point sphere which will be the tip of the cone
  fvp1 = createPointSphere(fptr->geomCentroid()+d*dir,matl);
	   
  fvp2 = fptr->firstVertex();

     // Insert an edge between the given corners
     // NOTE: Make sure point-sphere corner is specified first
  lastedge = insertEdge2(fvp1,fvp2,matl);

  done = false;
  while ( !done )
     {
       fvp1 = lastedge->getFaceVertexPtr1();

          // Get face in which fvp1 is there. This will always be the face
          // in which the next edge has to be inserted because the point-sphere corner
          // is always specified first in the insertEdge call
       fptr1 = fvp1->getFacePtr(); numsides1 = fptr1->size();

          // If the face fptr1 has more than 3 sides insert another edge
       if ( numsides1 > 3 )
          {
               // Skip the next corner to get the corner for next edge insertion
            fvp2 = fvp1->next(); fvp2 = fvp2->next();

               // Make sure point-sphere corner is specified first
            lastedge = insertEdge1(fvp1,fvp2,false);
          }
       else
          {
               // All edges have been inserted.
            done = true;
          }
     }
}

void DLFLObject::doubleStellateFace(DLFLFacePtr fptr, double d)
{
     // Original version by Eric. Modified by Vinod.
     // Double stellation
     // Zero-length stellatation followed by another zero-length stellation
     // of the new faces. Midpoint is then moved by specified distance along normal.
  Vector3d dir = fptr->computeNormal();

     // Do zero-length stellation
  stellateFace(fptr,0.0);

     // Get newly added vertex
  DLFLVertexPtr vp = lastVertex();

     // Find faces adjacent to new vertex
  DLFLFacePtrArray newfaces;
  vp->getFaces(newfaces);

     // Do zero-length stellation on new faces
  for (int i=0; i < newfaces.size(); ++i) stellateFace(newfaces[i],0.0);

     // Adjust position of centroid of original face - created by first stellation
  vp->coords += dir * d;
}

//--- Additions by Eric ---//

DLFLFacePtr DLFLObject::extrudeFaceDodeca(DLFLFacePtr fptr, double d, int num, double rot, double sf, bool hexagonalize)
{
     // Dodecahedral extrusion
  DLFLFacePtr exface = fptr;
  DLFLEdgePtrArray ep_arr;

  double sf3 = pow(sf, 1.0/2.0);
  if ( hexagonalize )
     {
       exface = extrudeFaceDodeca(exface, d, rot, sf3);

       for (int i=0; i < num-1; ++i)
          {
            exface->getEdges(ep_arr);
            exface = extrudeHexTileFace(exface, d, rot, sf3);
          }
     }
  else
     {
       for (int i=0; i < num; ++i)
          {
            exface = extrudeFaceDodeca(exface,d, rot, sf3);
          }
     }
  return exface;
}

DLFLFacePtr DLFLObject::extrudeHexTileFace(DLFLFacePtr fptr, double d, double rot, double sf)
{
     // Hexagonal extrusion for multiple dodecahedral extrusion.
  DLFLFacePtr exface = fptr;
  DLFLEdgePtrArray ep_arr;
  double ddiv3 = d / 3.0;
  double sf_2 = sqrt(sf);

     // extrude and delete the unneeded edges
  exface = extrudeDualFace(exface, ddiv3, rot, sf_2, true);
  exface->getEdges(ep_arr);

  exface = extrudeFace(exface, ddiv3, 0.0, sf_2);

     // delete more unneeded edges, the same way dual extrude does
  DLFLEdgePtrArray::iterator el_first, el_last;
  DLFLEdgePtr ep;
  el_first = ep_arr.begin(); el_last = ep_arr.end();
  while ( el_first != el_last )
     {
       ep = (*el_first); ++el_first;
       deleteEdge(ep,true);
     }
  return exface;
}

DLFLFacePtr DLFLObject::extrudeFaceDodeca(DLFLFacePtr fptr, double d, double rot, double sf)
{
     // Dodecahedral extrusion
     // Sequence of extrude, dual extrude and extrude followed by deletion of lateral edges
  DLFLFacePtr exface = fptr;
  DLFLEdgePtrArray ep_arr;

  double ddiv3 = d / 3.0;
  double sf_3 = pow(sf, 1.0/3.0);

  exface = extrudeFace(exface,ddiv3,0.0,sf_3);

     // extrude and delete the unneeded edges
  exface = extrudeDualFace(exface, ddiv3, rot, sf_3, true);
  exface->getEdges( ep_arr);

  exface = extrudeFace(exface, ddiv3, 0.0, sf_3);

     // delete more unneeded edges, the same way dual extrude does
  DLFLEdgePtrArray::iterator el_first, el_last;
  DLFLEdgePtr ep;
  el_first = ep_arr.begin(); el_last = ep_arr.end();
  while ( el_first != el_last )
     {
       ep = (*el_first); ++el_first;
       deleteEdge(ep,true);
     }
  return exface;
}

DLFLFacePtr DLFLObject::extrudeFaceIcosa(DLFLFacePtr fptr, double d, int num, double rot,  double sf)
{
     // Icosahedral extrusion
  DLFLFacePtr exface = fptr;

  double sf3 = pow(sf, 1.0/2.0);
  for (int i=0; i < num; ++i)
     {
       exface = extrudeFaceIcosa(exface,d, rot,  sf3);
     }
  return exface;

}

   // is performed by extruding, dual extruding, then extruding again.
   // then delete the latitude edges
DLFLFacePtr DLFLObject::extrudeFaceIcosa(DLFLFacePtr fptr, double d, double rot, double sf)
{
     // Icosahedral extrusion
  DLFLFacePtr exface = fptr;
  DLFLFacePtr otherface;
  DLFLEdgePtrArray ep_arr;
  DLFLVertexPtr vp;

  double ddiv3 = d / 3.0;
  double stellate_amount;
  double center_to_vert;

  Vector3d center_sphere, center_sphere2;
  Vector3d center_normal;
  Vector3d first_face_centroid;

  center_sphere2 = center_sphere = exface->geomCentroid();
  center_normal = exface->computeNormal();
  center_sphere += center_normal * (d / 3.0);
  center_sphere2 += center_normal * (d / 2.0);

     // dual extrude, then regular extrude
  exface = extrudeDualFace(exface, ddiv3, rot, sf, false);
  exface->getEdges( ep_arr);
  vp = lastVertex();
  center_to_vert = normsqr(vp->coords - center_sphere);

     // now have the radius of the shape sphere

  exface = extrudeFace(exface, ddiv3*2.0);

     // delete top edges from dual extrude
  DLFLEdgePtrArray::iterator el_first, el_last;
  DLFLEdgePtr ep;
  el_first = ep_arr.begin(); el_last = ep_arr.end();
  while ( el_first != el_last )
     {
       ep = (*el_first); ++el_first;
       deleteEdge(ep,true);
     }

  bool started = false;

     // stellate the top faces
  exface->getEdges(ep_arr);
  el_first = ep_arr.begin(); el_last = ep_arr.end();
  while ( el_first != el_last )
  {
    ep = (*el_first); ++el_first;
    otherface = ep->getOtherFacePointer(exface);
    if ( started == false )
       {
         first_face_centroid = otherface->geomCentroid();
         stellate_amount = center_to_vert - normsqr(first_face_centroid - center_sphere2);
         started = true;
       }

       // stellateFace(otherface, 1/pow(sf, 1.0/3.0));
    stellateFace(otherface, stellate_amount/2.0);
  }
  return exface;
}

//--- End additions by Eric ---//

void DLFLObject::extrudeFace(DLFLFacePtr fptr)
{
     // Extrude the given face using previously computed coordinates.
  DLFLFaceVertexPtr head;
  head = fptr->front();
  if ( head )
     {
       Vector3dArray newverts;

       DLFLFaceVertexPtr current = head;
       newverts.push_back(current->getAuxCoords());
       current = current->next();
       while ( current != head )
          {
            newverts.push_back(current->getAuxCoords());
            current = current->next();
          }
       createFace(newverts,fptr->material());

          // Get the pointer to the second newly inserted face
       DLFLFacePtr nfp;
       DLFLFacePtrList::reverse_iterator rfirst = face_list.rbegin();
       nfp = (*rfirst);

          // The last face will be the one facing the old face
          // Find the first face-vertices in the 2 faces to be connected
       DLFLFaceVertexPtr fvp1, fvp2;
       fvp1 = fptr->firstVertex(); fvp2 = nfp->firstVertex();
       connectFaces(fvp1,fvp2);
     }
}

/*
  $Log: DLFLExtrude.cc,v $
  Revision 4.2  2004/01/13 02:46:13  vinod
  Changed names of dodecahedral and icosahedral extrude methods

  Revision 4.1  2004/01/13 01:58:23  vinod
  Added double stellation, dodecahedral and icosahedral extrusions

  Revision 4.0  2003/12/26 01:58:52  vinod
  Major version sync.

  Revision 3.4  2003/11/21 15:26:23  vinod
  Added check and workaround for winged corners in duplicateFace with planar offset

  Revision 3.3  2003/11/16 06:45:06  vinod
  Added duplicateFacePlanarOffset and extrudeFacePlanarOffset methods

  Revision 3.2  2003/11/06 17:21:27  vinod
  Modified duplicateFace to use the new translate method to offset the new face

  Revision 3.1  2003/09/26 15:29:23  vinod
  Added duplicateFace method. Modified basic extrude method to use duplicateFace

  Revision 3.0  2003/09/19 16:06:47  vinod
  Major version update

  Revision 2.1  2003/01/25 19:44:56  vinod
  Sources for extrude subroutines. See DLFLObject.cc for previous logs

*/
