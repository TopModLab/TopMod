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


/* Rough outline of planarization algorithm */
#include <DLFLCore.hh>
#include <DLFLCoreExt.hh>
#include <cmath>
#include <cassert>

namespace DLFL {

  void meshsmooth( DLFLObjectPtr obj ) {
    DLFLFacePtrList::iterator fl_first, fl_last;
    DLFLFacePtr faceptr;
  
    fl_first = obj->beginFace(); fl_last = obj->endFace();

    /* Loop through all faces */
    while ( fl_first != fl_last ) {
      faceptr = (*fl_first); ++fl_first;

      /*
	Find the average normal for the face and the centroid
	Store these in the member variables of the face for later use
      */
      faceptr->normal = faceptr->computeNormal();
      faceptr->centroid = faceptr->geomCentroid();

      /*
	If the normal and point to define the plane are calculated
	using some other method, calculate them and store them in
	the 'normal' and 'centroid' member variables in the face
      */
    }


    DLFLVertexPtrList::iterator vl_first, vl_last;
    DLFLVertexPtr vertexptr;
    DLFLFaceVertexPtrArray fvparray;
    Vector3dArray p_array, n_array; // Arrays of point and normals for each face adjacent to a vertex
    int num_faces;
    Vector3d average, averagenormal; 
  
    vl_first = obj->beginVertex(); vl_last = obj->endVertex();

    /* Loop through all vertices */
    while ( vl_first != vl_last ) {
      vertexptr = (*vl_first); ++vl_first;

      /*
	Get all the corners for this vertex
      */
      fvparray.clear(); // The local array must be cleared before calling getFaceVertices
      vertexptr->getFaceVertices(fvparray);
      num_faces = fvparray.size();
       
      /*
	Go through the array of face-vertices and find the normal and centroid
	for each face
      */
      p_array.clear(); n_array.clear(); // The local arrays must be cleared for each Vertex

      // If we resize the array before filling it, we can avoid unnecessary memory allocation
      // thus speeding up the process a little bit.
      //p_array.resize(num_faces,Vector3d(0)); n_array.resize(num_faces,Vector3d(0));
      p_array.resize(num_faces,Vector3d()); n_array.resize(num_faces,Vector3d());
      for (int i=0; i <num_faces; ++i) {
	DLFLFacePtr fp = fvparray[i]->getFacePtr();

	// Get the normal and centroid for this face (which we calculated above)
	// and store them in our local arrays
	n_array[i] = fp->normal;
	p_array[i] = fp->centroid;
	average+=p_array[i];
	averagenormal+=n_array[i];
	    
      }
      average/=num_faces; normalize(averagenormal);
      Vector3d new_pos = vertexptr->coords; // This is the old position

      new_pos+=(average-new_pos)-((average-new_pos)*averagenormal)*averagenormal;
               
      vertexptr->coords = new_pos;
      average-=average; 
    }
  }

} // end namespace
