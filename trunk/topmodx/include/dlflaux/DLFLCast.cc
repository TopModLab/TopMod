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

#include "DLFLCast.hh"
#include <DLFLCoreExt.hh>
#include <cmath>
#include <cassert>

namespace DLFL {

  void spheralize( DLFLObjectPtr obj ) {
    DLFLVertexPtrList::iterator vl_first, vl_last;
    int num_vertices=0; 
    Vector3d center;
    DLFLVertexPtr vertexptr;
    Vector3d buffer;

    vl_first = obj->beginVertex();
    vl_last  = obj->endVertex();

    while ( vl_first != vl_last ) {
      vertexptr = (*vl_first); ++vl_first;

      center += vertexptr->coords; 
      num_vertices++; 
    }
    center /= num_vertices;

    vl_first = obj->beginVertex();
    vl_last  = obj->endVertex();

    while ( vl_first != vl_last ) {
      vertexptr = (*vl_first); ++vl_first;

      Vector3d new_pos = vertexptr->coords; // This is the old position

      buffer = new_pos-center;
      normalize(buffer); 
      vertexptr->coords = buffer; /*+center;*/
    }
  }

  void planarize( DLFLObjectPtr obj ) {
    DLFLFacePtrList::iterator fl_first, fl_last;
    DLFLFacePtr faceptr;
  
    fl_first = obj->beginFace();
    fl_last = obj->endFace();

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
  
    vl_first = obj->beginVertex();
    vl_last = obj->endVertex();

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
	//cout << "Before p = " << p_array[i] << " n = " << n_array[i] << endl;
	//cout << "After p = " << p_array[i] << " n = " << n_array[i] << endl << endl; 	    
      }

      Vector3d new_pos = vertexptr->coords; // This is the old position

      Vector3d buffer1,buffer2; 
      float t, deltat; int m; 
      m=100; t=0.3;deltat=t/(1.1*m); 
      for (int j=0; j <m; ++j) {
	t-=deltat; 
	for (int i=0; i <num_faces; ++i) {
	  buffer1=new_pos-p_array[i];
	  new_pos-=t*(n_array[i]*buffer1)*n_array[i];
	}
      }
       
      /*
	buffer1=new_pos-p_array[0];
	buffer2=n_array[0];
	t=0;  
	if(fabs((n_array[0]*buffer2))>0.000001)
	t=(n_array[0]*buffer1)/(n_array[0]*buffer2);
	new_pos-=t*buffer2;
       
	if(num_faces>0)
	{
	buffer1=new_pos-p_array[1];
	buffer2=n_array[1]-(n_array[0]*n_array[1])*n_array[0];
	if(fabs((buffer2*buffer2))<0.000001) 
	buffer2=n_array[2];
	t=0;  
	if(fabs((n_array[1]*buffer2))>0.000001) 
	t=(n_array[1]*buffer1)/(n_array[1]*buffer2);
	new_pos-=t*buffer2;
	}
       
	else if(num_faces>1)
	{
	buffer1=new_pos-p_array[2];
	buffer2=n_array[0]%n_array[1];
	if(fabs((buffer2*buffer2))<0.000001) 
	buffer2=n_array[2]-(n_array[0]*n_array[2])*n_array[0];
	else if(fabs((buffer2*buffer2))<0.000001)
	buffer2=n_array[2]-(n_array[1]*n_array[2])*n_array[1];
	else if(fabs((buffer2*buffer2))<0.000001)
	buffer2=n_array[2]; 
	t=0;  
	if(fabs((n_array[2]*buffer2))>0.000001) 
	t=(n_array[2]*buffer1)/(n_array[2]*buffer2);
	new_pos-=t*buffer2;
	}
      */
           
      /*
	Use the 2 arrays (n_array and p_array) to compute the new position for this vertex
      */
       
      vertexptr->coords = new_pos;
    }
  }

} // end namespace
