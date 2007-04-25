/* $Id: DLFLConnect.cc,v 4.0 2003/12/26 01:58:52 vinod Exp $ */

// Non-inline function definitions for class DLFLObject
// Subroutines that are related to connections between edges/faces

#include "DLFLAux.hh"
#include "DLFLObject.hh"

void DLFLObject :: connectEdges(DLFLEdgePtr eptr1, DLFLFacePtr fptr1,
                                DLFLEdgePtr eptr2, DLFLFacePtr fptr2, bool check)
{
     // Connect 2 half-edges

     // If given edge is not adjacent to corresponding face, returns without doing anything
  DLFLFaceVertexPtr e1fvp1, e1fvp2, e2fvp1, e2fvp2;

  e1fvp1 = eptr1->getFaceVertexPtr(fptr1);
  e2fvp1 = eptr2->getFaceVertexPtr(fptr2);
  if ( e1fvp1 == NULL || e2fvp1 == NULL ) return;

  e1fvp2 = e1fvp1->next(); e2fvp2 = e2fvp1->next();

     // Connect e1fvp1 and e2fvp2, e1fvp2 and e2fvp1
  if ( check )
     {
          // First check if there is already an edge between the 2 corners
          // This can be done by checking if the 2 corners are adjacent
       if ( (e1fvp1->next() == e2fvp2) || (e1fvp1->prev() == e2fvp2) )
          {
               // Edge already exists. Don't insert a new edge
          }
       else
          insertEdge(e1fvp1,e2fvp2);

       if ( (e1fvp2->next() == e2fvp1) || (e1fvp2->prev() == e2fvp1) )
          {
               // Edge already exists. Don't insert a new edge
          }
       else
          insertEdge(e1fvp2,e2fvp1);
     }
  else
     {
       insertEdge(e1fvp1,e2fvp2);
       insertEdge(e1fvp2,e2fvp1);
     }
}

void DLFLObject :: connectEdgesWithoutLoopCheck(DLFLEdgePtr eptr1, DLFLFacePtr fptr1,
                                                DLFLEdgePtr eptr2, DLFLFacePtr fptr2, bool check)
{
     // Connect 2 half-edges

     // If given edge is not adjacent to corresponding face, returns without doing anything
  DLFLFaceVertexPtr e1fvp1, e1fvp2, e2fvp1, e2fvp2;

  e1fvp1 = eptr1->getFaceVertexPtr(fptr1);
  e2fvp1 = eptr2->getFaceVertexPtr(fptr2);
  if ( e1fvp1 == NULL || e2fvp1 == NULL ) return;

  e1fvp2 = e1fvp1->next(); e2fvp2 = e2fvp1->next();

     // Connect e1fvp1 and e2fvp2, e1fvp2 and e2fvp1
  if ( check )
     {
          // First check if there is already an edge between the 2 corners
          // This can be done by checking if the 2 corners are adjacent
       if ( (e1fvp1->next() == e2fvp2) || (e1fvp1->prev() == e2fvp2) )
          {
               // Edge already exists. Don't insert a new edge
          }
       else
          insertEdgeWithoutCheck(e1fvp1,e2fvp2); // Use version which doesn't check for self loops

       if ( (e1fvp2->next() == e2fvp1) || (e1fvp2->prev() == e2fvp1) )
          {
               // Edge already exists. Don't insert a new edge
          }
       else
          insertEdgeWithoutCheck(e1fvp2,e2fvp1); // Use version which doesn't check for self loops
     }
  else
     {
          // Use versions which doesn't check for self loops
       insertEdgeWithoutCheck(e1fvp1,e2fvp2);
       insertEdgeWithoutCheck(e1fvp2,e2fvp1);
     }
}

void DLFLObject :: connectFaces(DLFLFaceVertexPtr fvptr1, DLFLFaceVertexPtr fvptr2)
{
     // Connect two faces using repeated insertEdge operations
     // The 2 corners have to belong to different faces
  if ( coFacial(fvptr1,fvptr2) ) return;

  DLFLEdgePtr lastedge;
  bool done = false;
  DLFLFacePtr fptr1 = fvptr1->getFacePtr(), fptr2;
  DLFLMaterialPtr matl = fptr1->material();
  DLFLFaceVertexPtr fvp1, fvp2;
  int numsides1, numsides2;

     // Insert an edge between the given corners
  lastedge = insertEdge2(fvptr1,fvptr2,matl);

  while ( !done )
     {
       lastedge->getFaceVertexPointers(fvp1,fvp2);
       fptr1 = fvp1->getFacePtr(); fptr2 = fvp2->getFacePtr();
       numsides1 = fptr1->size(); numsides2 = fptr2->size();

          // Find the face which has more than 4 sides
       if ( numsides1 > 4 )
          {
               // fptr1 has more than 4 sides. Next edge has to be inserted in this face
            fvp2 = fvp1->next();
            fvp2 = fvp2->next();
            fvp1 = fvp1->prev();
            lastedge = insertEdge1(fvp1,fvp2,false);
          }
       else if ( numsides2 > 4 )
          {
               // fptr2 has more than 4 sides. Next edge has to be inserted in this face
            fvp1 = fvp2->next();
            fvp1 = fvp1->next();
            fvp2 = fvp2->prev();
            lastedge = insertEdge1(fvp2,fvp1,false);
          }
       else
          {
               // Both faces have <= 4 sides. So we are done
            done = true;
          }
     }
}

void DLFLObject :: dualConnectFaces(DLFLFaceVertexPtr fvptr1, DLFLFaceVertexPtr fvptr2)
{
     // Connect two faces using repeated insertEdge operations
     // All newly created faces will be triangles.
     // Every vertex in the new face 1 will be connected to 2 adjacent vertices in face 2
     // and every vertex in face 2 will be connected to 2 adjacent vertices in face 1.
     // The 2 corners have to belong to different faces
  if ( coFacial(fvptr1,fvptr2) ) return;

  DLFLEdgePtr lastedge;
  bool done = false, parity = true;
  DLFLFacePtr fptr1 = fvptr1->getFacePtr(), fptr2;
  DLFLMaterialPtr matl = fptr1->material();
  DLFLFaceVertexPtr fvp1, fvp2;
  int numsides1, numsides2;

     // Insert an edge between the given corners
  lastedge = insertEdge2(fvptr1,fvptr2,matl);

  while ( !done )
     {
       lastedge->getFaceVertexPointers(fvp1,fvp2);
       fptr1 = fvp1->getFacePtr(); fptr2 = fvp2->getFacePtr();
       numsides1 = fptr1->size(); numsides2 = fptr2->size();

          // Find the face which has more than 3 sides
          // and insert the next edge in that face. Corners are chosen
          // depending on the parity flag
       if ( numsides1 > 3 )
          {
               // fptr1 has more than 3 sides. Next edge has to be inserted in this face
            if ( parity )
               {
                 fvp2 = fvp1->next(); fvp2 = fvp2->next();
               }
            else
               {
                 fvp2 = fvp1->next(); fvp1 = fvp1->prev();
               }
            parity = !parity;
			
            lastedge = insertEdge1(fvp1,fvp2,false);
          }
       else if ( numsides2 > 3 )
          {
               // fptr2 has more than 3 sides. Next edge has to be inserted in this face
            if ( parity )
               {
                 fvp1 = fvp2->next(); fvp1 = fvp1->next();
               }
            else
               {
                 fvp2 = fvp2->prev(); fvp1 = fvp2->next();
               }
            parity = !parity;

            lastedge = insertEdge1(fvp2,fvp1,false);
          }
       else
          {
               // Both faces have <= 3 sides. So we are done
            done = true;
          }
     }
}

void DLFLObject :: connectFaces(DLFLFaceVertexPtr fvptr1, DLFLFaceVertexPtr fvptr2,
                                int numsegs, int maxconn)
{
     // Connect 2 faces with multiple segments. Intermediate points are calculated
     // by linear interpolation between end points using number of segments

  if ( numsegs == 1 )
     {
       connectFaces(fvptr1,fvptr2);
       return;
     }

     // Reorder both faces so that they start at the selected corners
     // Reverse the second face before reordering for proper connection
  DLFLFacePtr f1, f2;
  f1 = fvptr1->getFacePtr();  f2 = fvptr2->getFacePtr();

  if ( f1->size() != f2->size() ) return;

  f1->reorder(fvptr1); f2->reverse(); f2->reorder(fvptr2);
  
     // Get the coordinates of the vertices of the 2 faces
  Vector3dArray verts1, verts2;
  f1->getVertexCoords(verts1); f2->getVertexCoords(verts2);

     // Reverse second face to take it back to it's original order
  f2->reverse(); f2->reorder(fvptr2);

  double t, dt = 1.0/double(numsegs);
  Vector3dArray verts;
  DLFLFacePtr nf2,nf3;
  DLFLFaceVertexPtr nfvp1,nfvp2,nfvp3;
  DLFLFacePtrList :: reverse_iterator rfirst;
  DLFLMaterialPtr matl = f1->material();
  int numverts = verts1.size();
  int numconnected = 0;

  if ( maxconn == -1 ) maxconn = numsegs;

  nfvp1 = fvptr1; verts.resize(numverts);

  for (int i=1; i < numsegs; ++i)
     {
       t = double(i)*dt;

          // Find linearly interpolated resolution parameters
       linearInterpolate(verts1,verts2,verts,t);

          // Create a new face using computed coordinates
       createFace(verts,matl);

          // Get the pointers to the newly inserted faces
       rfirst = face_list.rbegin();
       nf2 = (*rfirst); ++rfirst; nf3 = (*rfirst);

          // Get pointers to the first FaceVertex in each of these faces
       nfvp2 = nf2->firstVertex(); nfvp3 = nf3->firstVertex();

          // Connect nf1 and nf2
       if ( numconnected < maxconn )
          {
            ++numconnected;
            connectFaces(nfvp1,nfvp2);
          }
       
          // Make nf3 be the new nf1
       nfvp1 = nfvp3;
     }
     // Make the last connection
  nf2 = f2; nfvp2 = fvptr2;
  if ( numconnected < maxconn ) connectFaces(nfvp1,nfvp2);
}

void DLFLObject :: connectFaces(DLFLFacePtr fp1, DLFLFacePtr fp2, int numsegs)
{
     // Find 2 closest vertices between the 2 faces
  DLFLFaceVertexPtr fvp1, fvp2;

  DLFLFace::findClosestCorners(fp1,fp2,fvp1,fvp2);

  if ( fvp1 && fvp2 ) connectFaces(fvp1,fvp2,numsegs);
}

void DLFLObject :: hermiteConnectFaces(DLFLFaceVertexPtr fvptr1, DLFLFaceVertexPtr fvptr2,
                                       int numsegs, double wt1, double wt2,
                                       int maxconn, int numtwists)
{
  if ( numsegs == 1 )
     {
       connectFaces(fvptr1,fvptr2);
       return;
     }

     // Do hermite interpolated handle. Centroid is interpolated using hermite interpolation
     // Vertices are computed in relation to the centroid with linear interpolation from first
     // face to second face

     // Reorder both faces so that they start at the selected corners
     // Reverse the second face before reordering for proper connection
  DLFLFacePtr f1, f2;
  f1 = fvptr1->getFacePtr();  f2 = fvptr2->getFacePtr();

  if ( f1->size() != f2->size() ) return;

  f1->reorder(fvptr1); f2->reverse(); f2->reorder(fvptr2);
  
     // Get the coordinates of the vertices of the 2 faces
  Vector3dArray verts1, verts2;
  f1->getVertexCoords(verts1); f2->getVertexCoords(verts2);

     // Get the normals to the 2 faces. Second face is already reversed
  Vector3d n1, n2;
  n1 = f1->computeNormal(); n2 = f2->computeNormal();

     // Reverse second face to take it back to it's original order
  f2->reverse(); f2->reorder(fvptr2);

  
     //--- Find interpolation parameters ---//

     // Find and store centroid of 2 polygons
  Vector3d cen1 = centroid(verts1), cen2 = centroid(verts2);

     // Translate both polygons to origin
  translate(verts1,Vector3d(0,0,0)); translate(verts2,Vector3d(0,0,0));

     // The plane to rotate both polygons to is found as follows.
     // Find the vector from the centroid of the second polygon to
     // the centroid of the first polygon. This will be the normal
     // to the plane
  Vector3d rotplane(cen2-cen1); normalize(rotplane);
  rotate(verts1,n1,rotplane);
  rotate(verts2,n2,rotplane);
  
     // The reference X and Y axes are chosen as follows
     // x-axis : vector from centroid (origin) to the midpoint of the last edge in the first polygon
     // y-axis : use x-axis and normal to the plane (rotplane) to find y axis
     // Note that both polygons are already centered at the origin and on the rotation plane
  Vector3d xref, yref;
  int numverts = verts1.size();

  xref = (verts1[0]+verts1[numverts-1]); normalize(xref);
  yref = rotplane % xref;

     // If either of the 2 polygons has a normal pointing in exactly the opposite direction
     // as the normal to the rotation plane, it can't be rotated to the rotation plane
     // and hence the handle can't be created without additional information
  if ( Abs(n1*rotplane + 1.0) < ZERO || Abs(n2*rotplane + 1.0) < ZERO )
     {
       cout << "Normals are opposite. Can't create handle!" << endl;
       return;
     }

     // Resolve both polygons using xref and yref
  DoubleArray angle1, angle2, distance1, distance2;

     // resolvePolygon does the resize for the arrays
  resolvePolygon(verts1,xref,yref,angle1,distance1);
  resolvePolygon(verts2,xref,yref,angle2,distance2);

     // If the angle for the first point in the second polygon is greater than
     // the angle for the first point in the first polygon, and the difference
     // is greater than pi, subtract 2pi from all angles in second polygon
     // to avoid more than 1 twist
  if ( (angle2[0] - angle1[0]) > M_PI )
     {
       for (int i=0; i < angle2.size(); ++i)
          angle2[i] -= 2.0*M_PI;
     }

     // If we want any extra twists, add multiples of 2pi to second polygon angles
  if ( numtwists > 0 )
     {
       for (int i=0; i < angle2.size(); ++i)
          angle2[i] += 2.0*M_PI*numtwists;
     }
       
     // Now use the resolution parameters to find intermediate polygons
  double t, dt = 1.0/double(numsegs);
  Vector3dArray verts;
  DoubleArray angle, distance;
  Vector3d p,v,dvdt;
  DLFLFacePtr nf2,nf3;
  DLFLFaceVertexPtr nfvp1,nfvp2,nfvp3;
  DLFLFacePtrList :: reverse_iterator rfirst;
  DLFLMaterialPtr matl = f1->material();
  int numconnected = 0;

  if ( maxconn == -1 ) maxconn = numsegs;

  nfvp1 = fvptr1;
  verts.resize(numverts); angle.resize(numverts); distance.resize(numverts);

     // Multiply the normals by the weights for hermite interpolation
  n1 *= wt1; n2 *= wt2;
  double curve_length = hermiteCurveLength(cen1,n1,cen2,n2,8*numsegs);
  double segment_length = curve_length / double(numsegs);
  t = 0.0; p = cen1; v = n1; dvdt = 6.0*(cen2-cen1) - 4.0*n1 - 2.0*n2;

  for (int i=1; i < numsegs; ++i)
     {
//       dt = segment_length/(norm(v) + norm(dvdt)*t);
       t = double(i)*dt;

          // Find linearly interpolated resolution parameters
       linearInterpolate(angle1,angle2,angle,t);
       linearInterpolate(distance1,distance2,distance,t);

          // Since angles are stored as differences, except for first angle
          // they have to be added to get correct angles
          //for (int j=1; j < numverts; ++j)
          //angle[j] += angle[j-1];

          // Reconstruct interpolated polygon using interpolated resolution parameters
       reconstructPolygon(verts,xref,yref,angle,distance);

          // Do hermite interpolation on centroids and normals to find interpolated point and normal
       hermiteInterpolate(cen1,n1,cen2,n2,p,v,dvdt,t);

          // Rotate reconstructed polygon from rotation plane (XY plane) to plane with v as normal
          // Rotation is done around centroid of polygon
       rotate(verts,rotplane,v);
       
          // Translate rotated polygon from origin to p
       translate(verts,p);

          // Create a new face using computed coordinates
       createFace(verts,matl);

          // Get the pointers to the newly inserted faces
       rfirst = face_list.rbegin();
       nf2 = (*rfirst); ++rfirst; nf3 = (*rfirst);

          // Get pointers to the first FaceVertex in each of these faces
       nfvp2 = nf2->firstVertex(); nfvp3 = nf3->firstVertex();

          // Connect nf1 and nf2
       if ( numconnected < maxconn )
          {
            ++numconnected;
            connectFaces(nfvp1,nfvp2);
          }
       
          // Make nf3 be the new nf1
       nfvp1 = nfvp3;
     }
     // Make the last connection
  nf2 = f2; nfvp2 = fvptr2;
  if ( numconnected < maxconn ) connectFaces(nfvp1,nfvp2);
}


void DLFLObject :: bezierConnectFaces(DLFLFaceVertexPtr fvptr1, DLFLFaceVertexPtr fvptr2,
                                      int numsegs, double wt1, double wt2)
{
     // Do hermite interpolated handle. All vertices are interpolated using hermite interpolation

     // Reorder both faces so that they start at the selected corners
     // Reverse the second face before reordering for proper connection
  DLFLFacePtr f1, f2;
  f1 = fvptr1->getFacePtr();  f2 = fvptr2->getFacePtr();

  if ( f1->size() != f2->size() ) return;

  f1->reorder(fvptr1); f2->reverse(); f2->reorder(fvptr2);

     // Get the coordinates of the vertices of the 2 faces
  Vector3dArray verts1, verts2;
  f1->getVertexCoords(verts1); f2->getVertexCoords(verts2);

     // Get the normals to the 2 faces. Second face is already reversed so we don't have
     // to fix the normal
  Vector3d n1, n2;
  n1 = wt1*f1->computeNormal(); n2 = wt2*f2->computeNormal();

     // Reverse second face to take it back to it's original order
  f2->reverse(); f2->reorder(fvptr2);

     // Compute 3 intermediate vertices and create new faces
  double t, dt = 1.0/numsegs;
  Vector3dArray verts;
  Vector3d p,v,dvdt;
  int numverts = verts1.size();
  DLFLFacePtr nf2,nf3;
  DLFLFaceVertexPtr nfvp1,nfvp2,nfvp3;
  DLFLFacePtrList :: reverse_iterator rfirst;
  
  nfvp1 = fvptr1;
  verts.resize(numverts);
  for (int i=1; i < numsegs; ++i)
     {
       t = i*dt;
       for (int j=0; j < numverts; ++j)
          hermiteInterpolate(verts1[j],n1,verts2[j],n2,verts[j],v,dvdt,t);

       createFace(verts,f1->material());

          // Get the pointers to the newly inserted faces
       rfirst = face_list.rbegin();
       nf2 = (*rfirst); ++rfirst; nf3 = (*rfirst);

          // Get pointers to the first FaceVertex in each of these faces
       nfvp2 = nf2->firstVertex(); nfvp3 = nf3->firstVertex();

          // Connect nf1 and nf2
       connectFaces(nfvp1,nfvp2);

          // Make nf3 be the new nf1
       nfvp1 = nfvp3;
     }

     // Make the last connection
  nf2 = f2; nfvp2 = fvptr2;
  connectFaces(nfvp1,nfvp2);
}

/*
  $Log: DLFLConnect.cc,v $
  Revision 4.0  2003/12/26 01:58:52  vinod
  Major version sync.

  Revision 3.2  2003/11/29 22:03:42  vinod
  Added max. connections parameter for regular connectFaces

  Revision 3.1  2003/11/29 04:20:11  vinod
  Added check for single segment curved handle, which is same as generic handle

  Revision 3.0  2003/09/19 16:06:47  vinod
  Major version update

  Revision 2.6  2003/04/29 08:59:01  vinod
  Attempted equal-curve-length parametrization using second derivatives.
  Doesn't work yet. Reverted to original method for now

  Revision 2.5  2003/04/14 15:44:30  vinod
  Modified hermiteConnect to create equal-length segments.
  Not working correctly yet

  Revision 2.4  2003/02/27 16:27:33  vinod
  Moved multiConnectFaces to separate file, DLFLMultiConnect.cc

  Revision 2.3  2003/02/08 00:03:19  vinod
  Changed co-facial check method in multi-connect

  Revision 2.2  2003/01/30 07:03:03  vinod
  Added multiConnectFaces

  Revision 2.1  2003/01/25 19:44:35  vinod
  Sources for edge/face connect subroutine. See DLFLObject.cc for previous logs

*/
