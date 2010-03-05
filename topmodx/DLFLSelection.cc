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

#include "DLFLSelection.hh"
#include "GeometryRenderer.hh"

/**!
*
* \brief functions for rendering edges, faces, vertices of DLFL Objects
* 	
*	\note 
* 
* \see DLFLSelection
*/

extern DLFLEdgePtrArray DLFLObject::eparray;
extern DLFLFacePtrArray DLFLObject::fparray;
extern DLFLVertexPtrArray DLFLObject::vparray;
extern DLFLFaceVertexPtrArray DLFLObject::fvparray;

// Code for selection of vertices, edges and faces

// Render the vertices for selection
void renderVerticesForSelect( DLFLObjectPtr obj ) {
  DLFLVertexPtrList vertex_list = obj->getVertexList( );
  // Adjust the size of the array used for selection to be atleast equal to
  // number of vertices
  DLFLObject::vparray.reserve(vertex_list.size()); // STL checks for current capacity

  // Copy the pointers from the vertex_list to the vertex array
  // Render the vertices for selection at the same time (saves one loop)
  DLFLVertexPtrList::const_iterator first, last;
  DLFLVertexPtr vptr = NULL;
  int i=0;
  
  first = vertex_list.begin(); last = vertex_list.end();
  while ( first != last ) {
    glLoadName(i); vptr = (*first);
    //glBegin(GL_POINTS); {
    GeometryRenderer::instance()->renderVertex( vptr );
    ///vptr->render();
    //} glEnd();
    DLFLObject::vparray[i++] = vptr;
    ++first;
  }
}

// Render the edges for selection
void renderEdgesForSelect( DLFLObjectPtr obj ) {
  DLFLEdgePtrList edge_list = obj->getEdgeList( );
  // Adjust the size of the array used for selection to be atleast equal to
  // number of edges
  DLFLObject::eparray.reserve(edge_list.size()); // STL checks for current capacity

  // Copy the pointers from the edge_list to the edge array
  // Render the edges for selection at the same time (saves one loop)
  DLFLEdgePtrList::const_iterator first, last;
  DLFLEdgePtr eptr = NULL;
  int i=0;
  
  first = edge_list.begin(); last = edge_list.end();
  while ( first != last ) {
    glLoadName(i); eptr = (*first);
    ///eptr->render();
    glBegin(GL_LINES); {
      GeometryRenderer::instance()->renderEdge(eptr);
    } glEnd();
    DLFLObject::eparray[i++] = eptr;
    ++first;
  }
}

// Render the faces for selection
void renderFacesForSelect( DLFLObjectPtr obj ) {
  DLFLFacePtrList face_list = obj->getFaceList( );
  // Adjust the size of the array used for selection to be atleast equal to
  // number of faces
  DLFLObject::fparray.reserve(face_list.size()); // STL checks for current capacity

  // Copy the pointers from the face_list to the face array
  // Render the faces for selection at the same time (saves one loop)
  DLFLFacePtrList::const_iterator first, last;
  DLFLFacePtr fptr = NULL;
  int i=0;
  
  first = face_list.begin(); last = face_list.end();
  while ( first != last ) {
    glLoadName(i); fptr = (*first);
    if ( fptr->size() == 1 ) {
      GLdouble depthrange[2];
      GLfloat pointsize;
      glGetDoublev(GL_DEPTH_RANGE,depthrange);
      glGetFloatv(GL_POINT_SIZE,&pointsize);
      glDepthRange(0.0,1.0-0.005);
      glPointSize(3.0);
      GeometryRenderer::instance()->renderFace(fptr);
      ///fptr->render_FV();
      glPointSize(pointsize);
      glDepthRange(depthrange[0],depthrange[1]);
    } else {
      GeometryRenderer::instance()->renderFace(fptr);
      ///fptr->render_FV();
    }
    DLFLObject::fparray[i++] = fptr;
    ++first;
  }
}

// Render the face vertices of a face for selection
void renderFaceVerticesForSelect( DLFLFacePtr fp ) {
  // Copy the pointers from the face_list to the face array
  // Render the faces for selection at the same time (saves one loop)
  DLFLFaceVertexPtr head;
  head = fp->front();
  if ( head ) {
    int i=0;
    // Adjust the size of the array used for selection to be atleast equal to
    // number of faces
    DLFLObject::fvparray.reserve(fp->size()); // STL checks for current capacity

    DLFLFaceVertexPtr current = head;
    glLoadName(i);
    glBegin(GL_POINTS); {
      ///current->render();
      GeometryRenderer::instance()->renderFaceVertex(current);
    } glEnd();
    DLFLObject::fvparray[i++] = current;
    current = current->next();
    while ( current != head ) {
      glLoadName(i);
      glBegin(GL_POINTS); {
	GeometryRenderer::instance()->renderFaceVertex(current);
	///current->render();
      } glEnd();
      DLFLObject::fvparray[i++] = current;
      current = current->next();
    }
  }
}
