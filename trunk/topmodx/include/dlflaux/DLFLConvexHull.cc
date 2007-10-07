/*
*
* ***** BEGIN GPL LICENSE BLOCK *****
*
* This program is free software; you can redistribute it and/or
* modify it under the terms of the GNU General Public License
* as published by the Free Software Foundation; either version 2
* of the License, or (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program; if not, write to the Free Software  Foundation,
* Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
*
* The Original Code is Copyright (C) 2005 by xxxxxxxxxxxxxx
* All rights reserved.
*
* The Original Code is: all of this file.
*
* Contributor(s): none yet.
*
* ***** END GPL LICENSE BLOCK *****
*
* Short description of this file
*
* name of .hh file containing function prototypes
*
*/

// Source code for DLFLConvexHull class

#include "DLFLConvexHull.hh"
#include "DLFLSubdiv.hh"
#include <DLFLCore.hh>

namespace DLFL {

  long DLFLConvexHull::remove = 1;               // Edge needs to be removed
  long DLFLConvexHull::border = 2;                  // Edge is a border edge
  long DLFLConvexHull::visible = 1;        // Face is visible from new point
  long DLFLConvexHull::invisible = 2;    // Face is invisible from new point

  // Are the 3 given points colinear?
  bool DLFLConvexHull::colinear(const Vector3d& p1, const Vector3d& p2, const Vector3d& p3) {
    // 3 points are colinear if the area of the triangle formed by them is zero
    // The cross product of any two vectors formed by the 3 points determines
    // the area of the triangle formed by them
    Vector3d v1, v2, area;
    v1 = p2-p1; v2 = p3-p1;
    area = v1 % v2;
    if ( !isNonZero(area[0]) && !isNonZero(area[1]) && !isNonZero(area[2]) ) return true;
    return false;
  }

  // Find sign of volume of tetrahedron formed by given face and given point
  // If given face is not a triangle returns 0.
  int DLFLConvexHull::volumeSign(DLFLFacePtr face, const Vector3d& p) {
    if ( face ) {
      if ( face->size() != 3 ) return 0;
      Vector3dArray points;
      face->getVertexCoords(points);

      // Create Vector4ds from the points of the face and the new point
      Vector4d r1(points[0]),r2(points[1]),r3(points[2]),r4(p);

      // Set last component of all Vector4ds to be 1
      r1[3] = r2[3] = r3[3] = r4[3] = 1.0;

      // Create a 4x4 matrix with the 4 Vector4ds
      Matrix4x4 matrix(r1,r2,r3,r4);

      // Find determinant of above matrix. 1/6th of this the volume of the tetrahedron
      double volume = determinant(matrix)/6.0;
      int volsign = 0;
      if ( volume > ZERO ) volsign = 1;
      else if ( volume < -ZERO ) volsign = -1;
      return volsign;
    }
    return 0;
  }

  bool DLFLConvexHull::doubleTriangle( ) {
    InputVertex iv1,iv2,iv3;
    DLFLFaceVertexPtrArray fvptrArray; // Esan
    int numverts = vertices.size();
    int i;
  
    i=0; iv1 = vertices[i]; iv2 = vertices[i+1]; iv3 = vertices[i+2];
    while ( DLFLConvexHull::colinear(iv1.point,iv2.point,iv3.point) ) {
      i++;
      if ( i+2 >= numverts ) return false; // All points are colinear
    }

    // Set the processed flag for the 3 input vertices chosen for initial triangle
    vertices[0].processed = vertices[1].processed = vertices[2].processed = true;

    Vector3dArray triangle;
    triangle.resize(3);
    triangle[0] = iv1.point; triangle[1] = iv2.point; triangle[2] = iv3.point;

    // Create two "twin" faces
    createFace(triangle);

    DLFLFacePtr tfp = firstFace(); // Face which is in the given order (iv1,iv2,iv3)

    // Added by Esan
    tfp->getCorners(fvptrArray);
    (fvptrArray[0]->getVertexPtr())->CHullIndex = vertices[0].index;
    (fvptrArray[1]->getVertexPtr())->CHullIndex = vertices[1].index;
    (fvptrArray[2]->getVertexPtr())->CHullIndex = vertices[2].index;
    // End additions by Esan

    // Find a fourth noncoplanar vertex to form the tetrahedron
    for (i=0; i < numverts; ++i) {
      int volsign = DLFLConvexHull::volumeSign(tfp,vertices[i].point);
      if ( vertices[i].processed == false && volsign != 0) {
	// Make sure this vertex is the first one added
	// Swap this vertex with the vertex at the beginning of the array
	// Even if the first vertex in the array is part of the initial triangle, it doesn't
	// affect anything, since it's already been processed and will not be considered again
	InputVertex temp = vertices[0];
	temp.index = vertices[0].index; // Esan
	vertices[0] = vertices[i]; vertices[i] = temp;
	vertices[0].index = vertices[i].index; vertices[i].index = temp.index; // Esan
	break;
      }
    }
    if ( i == numverts ) return false; // All points are coplanar

    return true;
  }

  bool DLFLConvexHull::addOne(InputVertex& iv) {
    // Add given vertex to hull
    // Assumes that iv has not been processed yet

    Vector3d p = iv.point;
    bool foundvisible = false;
  
    // Mark the faces that are visible from p (iv)
    DLFLFacePtrList::iterator ffirst, flast;
    DLFLFacePtr fp;
  
    ffirst = face_list.begin(); flast = face_list.end();
    while ( ffirst != flast ) {
      fp = (*ffirst); ++ffirst;
      if ( DLFLConvexHull::volumeSign(fp,p) < 0 ) {
	fp->flags = DLFLConvexHull::visible;
	foundvisible = true;
      } else
	fp->flags = DLFLConvexHull::invisible;
    }

    // If no faces are visible from p then p is inside the hull
    if ( foundvisible == false ) {
      iv.onhull = false;
      return false;
    }

    // Mark edges in the interior of visible region for deletion
    // Create a new face based on each border edge
    // A border edge is one where only one of the adjacent faces is visible
    DLFLEdgePtrList::iterator efirst, elast;
    DLFLEdgePtr ep;
    DLFLFacePtr fp1,fp2;
    DLFLEdgePtrList remove_list; // List of edges to be removed
    DLFLEdgePtrList border_list; // List of border edges

    efirst = edge_list.begin(); elast = edge_list.end();
    while ( efirst != elast ) {
      ep = (*efirst); ++efirst;
      ep->getFacePointers(fp1,fp2);
      if ( fp1->flags == DLFLConvexHull::visible && fp2->flags == DLFLConvexHull::visible ) {
	// Interior edge : add edge to remove_list
	remove_list.push_back(ep);
      } else if ( fp1->flags == DLFLConvexHull::visible || fp2->flags == DLFLConvexHull::visible ) {
	// Border edge : add edge to border_list
	border_list.push_back(ep);
      }
    }

    // Go through remove_list and remove all edges
    while ( !remove_list.empty() ) {
      ep = remove_list.front(); remove_list.pop_front();
      deleteEdge(this,ep,true);
    }

    // Find the face bounded by the border edges - call it the conebase
    // This can be done by taking any border edge and finding the face
    // adjacent to it which is not invisible.
    DLFLFacePtr conebase = NULL;

    ep = border_list.front();
    ep->getFacePointers(fp1,fp2);

    if ( fp1->flags == DLFLConvexHull::invisible ) conebase = fp2;
    else if ( fp2->flags == DLFLConvexHull::invisible ) conebase = fp1;
    else {
      // This should not happen. Problem somewhere
      cout << "Oops! Something went wrong somewhere!" << endl;
      return false;
    }

    // Subdivide the conebase face using triangles. This will create a new point
    // at the centroid, which can then simply be moved to the apex of the cone
    subdivideFace(this,conebase,false);

    // The centroid will be the last DLFLVertex in the vertex_list
    DLFLVertexPtr apex = vertex_list.back();

    // Adjust coordinates of the apex to be the new point we are adding to the hull
    apex->coords = p;
    apex->CHullIndex = iv.index;

    return true;
  }
  

  bool DLFLConvexHull::constructHull( ) {
    // Create Initial double-triangle
    if ( doubleTriangle() == false ) {
      cout << "Could not form initial polytope" << endl;
      return false;
    }

    // Go through list of InputVertices and add ones which have not been processed yet
    InputVertex iv;
    bool retval = true; // Tells us if any vertex is inside convex hull
    for (int i=0; i < vertices.size(); ++i) {
      iv = vertices[i];
      if ( !iv.processed ) {
	iv.index = vertices[i].index; // Esan
	iv.processed = true;
	retval = addOne(iv);
      }
    }
    return retval;
  }

  // Create a convex hull from given list of vertices
  // Old object is destroyed
  bool DLFLConvexHull::createHull(const Vector3dArray& p) {
    reset(); // Inherited from DLFLObject class
    vertices.clear(); vertices.resize(p.size());
    for (int i=0; i < p.size(); ++i) {
      vertices[i].point = p[i];
      vertices[i].index = i;
      vertices[i].processed = false;
    }

    bool retval = constructHull();

    return retval;
  }

} // end namespace
