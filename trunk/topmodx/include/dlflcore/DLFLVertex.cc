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

/**
 * \file DLFLVertex.cc
 */

// Non-inline function definitions for DLFLVertex class

#include "DLFLVertex.hh"
#include "DLFLFace.hh"
#include "DLFLEdge.hh"

namespace DLFL {
  uint DLFLVertex::suLastID = 0;

  // Dump contents of this object to an output stream
  void DLFLVertex::dump(ostream& o) const {
    o << "DLFLVertex" << endl
      //    << "  ID : " << uID << endl
      //    << "  Coords : " << coords << endl
      //    << "  Type : " << vtType << endl
      << "  fvpList" << endl;

    DLFLFaceVertexPtrList::const_iterator first, last;
    int i=0;
  
    first = fvpList.begin(); last = fvpList.end();
    while ( first != last ) {
      o << "    " << i++ << " : " << (*first) << endl;
      ++first;
    }
    o << endl;
  }

  // Reset type of vertex, face-vertices and edges associated with this vertex
  void DLFLVertex::resetTypeDeep(void) {
    // Reset vertex type
    resetType();

    // Go through face-vertex list and reset type of each face-vertex
    // and the edge starting at that face-vertex
    DLFLFaceVertexPtrList::iterator first, last;
    DLFLFaceVertexPtr fvp;
    first = fvpList.begin(); last = fvpList.end();
    while ( first != last ) {
      fvp = (*first); ++first;
      fvp->resetType();
      fvp->getEdgePtr()->resetType();
    }
  }

  // Set the texture coordinates for all FaceVertexes referring to this vertex
  void DLFLVertex::setTexCoords(const Vector2d& texcoord) {
    DLFLFaceVertexPtrList::iterator first, last;

    first = fvpList.begin(); last = fvpList.end();
    while ( first != last ) {
      (*first)->texcoord = texcoord;
      ++first;
    }
  }

  // Set the color values for all FaceVertexes referring to this vertex
  void DLFLVertex::setColor(const RGBColor& color) {
    DLFLFaceVertexPtrList::iterator first, last;

    first = fvpList.begin(); last = fvpList.end();
    while ( first != last ) {
      (*first)->color = color;
      ++first;
    }
  }

  // Set the normals for all FaceVertexes referring to this vertex
  Vector3d DLFLVertex::computeNormal( bool set ) {
    DLFLFaceVertexPtrList::iterator first, last;
    Vector3d normal;
    int i=0;

    first = fvpList.begin(); last = fvpList.end();
    while ( first != last ) {
      normal += (*first)->computeNormal();
      ++first; ++i;
    }
    normal /= i;
    if ( set ) {
      first = fvpList.begin();
      while ( first != last ) {
	(*first)->normal = normal;
	++first;
      }
    }
    return normal;
  }

  // Compute the normals for all FaceVertexes referring to this vertex
  // Update the vertex normal and return it
  Vector3d DLFLVertex::updateNormal(bool recompute) {
    DLFLFaceVertexPtrList::iterator first, last;

    first = fvpList.begin(); last = fvpList.end();
    normal.reset();
    if ( recompute )
      while ( first != last ) {
	normal += (*first)->computeNormal();
	++first;
      }
    else
      while ( first != last ) {
	normal += (*first)->normal;
	++first;
      }
     
    normalize(normal);
    return normal;
  }

  Vector3d DLFLVertex::getNormals(Vector3dArray& normals) {
    // Return normals at all corners in an Vector3dArray
    Vector3d avenormal;
    int numnormals = fvpList.size();
    if ( numnormals > 0 ) {
      normals.clear(); normals.reserve(numnormals);

      DLFLFaceVertexPtrList::const_iterator first, last;
      DLFLFaceVertexPtr fvp = NULL;
      first = fvpList.begin(); last = fvpList.end();
      while ( first != last ) {
	fvp = (*first); ++first;
	normals.push_back(fvp->normal); avenormal += fvp->normal;
      }
      avenormal /= numnormals;
    }
    return avenormal;
  }

  // Set tex coordinates, color and normal info for all FaceVertexes referring to this vertex
  void DLFLVertex::setFaceVertexProps(const Vector2d& texcoord, const RGBColor& color, const Vector3d& n) {
    DLFLFaceVertexPtrList::iterator first, last;

    first = fvpList.begin(); last = fvpList.end();
    while ( first != last ) {
      (*first)->texcoord = texcoord;
      (*first)->color = color;
      (*first)->normal = n;
      ++first;
    }
  }

  void DLFLVertex::vertexTrace(void) const {
    // Output all edges incident on this DLFLVertex in the specific rotation order

    // Pick a DLFLFaceVertex from the list - first one
    DLFLFaceVertexPtr fvptr1 = fvpList.front();

    if ( fvptr1 == NULL ) return;

    DLFLEdgePtr e1,e2;
    DLFLFaceVertexPtr fvptr2;
    DLFLFacePtr f;

    // Find Edge starting at this DLFLFaceVertex
    e1 = fvptr1->getEdgePtr();
    e2 = e1;

    do {
      cout << (*e2) << " ";

      // Find the other DLFLFaceVertex for this DLFLEdge
      fvptr2 = e2->getOtherFaceVertexPtr(fvptr1);
      if ( fvptr2 == NULL ) return;

      // Find the Face to which the second DLFLFaceVertex belongs
      f = fvptr2->getFacePtr();

      // Find the DLFLFaceVertex following the second DLFLFaceVertex in this Face
      fvptr1 = f->nextFaceVertex(fvptr2);

      // Find Edge starting at this DLFLFaceVertex
      e2 = fvptr1->getEdgePtr();
    }
    while ( e2 != e1 );

    cout << endl;
  }

  int DLFLVertex::getEdges(DLFLEdgePtr ** edges) const {
    // Create an array of DLFLEdge's incident on this DLFLVertex. The number of DLFLEdges
    // is returned. Memory will be allocated inside this function, which should
    // be freed by the caller. Pass a pointer to the array (DLFLEdge ** or DLFLEdgePtr *)
    // Equivalent to a vertex trace
    // In case of an error -1 is returned, but memory is not freed

    int num_edges = fvpList.size();
    int i = 0;

    if ( num_edges <= 0 ) return -1;

    // Allocate memory for the DLFLEdgePtr array
    *edges = new DLFLEdgePtr[num_edges];
  
    DLFLFaceVertexPtrList::const_iterator first, last;
    DLFLFaceVertexPtr fvp = NULL;
    first = fvpList.begin(); last = fvpList.end();
    while ( first != last ) {
      fvp = (*first); ++first;
      edges[0][i++] = fvp->getEdgePtr();
    }
    return num_edges;
  }

  void DLFLVertex::getEdges(DLFLEdgePtrArray& edges) const {
    // Return edges in an DLFLEdgePtrArray
		// if (fvpList ){
	    edges.clear(); edges.reserve(fvpList.size());

	    DLFLFaceVertexPtrList::const_iterator first, last;
	    DLFLFaceVertexPtr fvp = NULL;
	    first = fvpList.begin(); last = fvpList.end();
	    while ( first != last ) {
	      fvp = (*first); ++first;
	      edges.push_back(fvp->getEdgePtr());
	    }
		// }
  }

  DLFLEdgePtr DLFLVertex::getEdgeTo(DLFLVertexPtr vp) {
    // Get the Edge incident on this Vertex which connects to given Vertex
    // If no such edge exists, returns NULL
    DLFLEdgePtr ep, retep = NULL;
    DLFLFaceVertexPtrList::iterator first, last;
    DLFLFaceVertexPtr fvp = NULL, ofvp;
    first = fvpList.begin(); last = fvpList.end();
    while ( first != last ) {
      fvp = (*first); ++first;
      ep = fvp->getEdgePtr();
      if ( ep != NULL ) {
	ofvp = ep->getOtherFaceVertexPtr(fvp);
	if ( ofvp && ofvp->vertex == vp ) {
	  retep = ep; break;
	}
      }
    }
    return retep;
  }
  
  void DLFLVertex::getFaceVertices(DLFLFaceVertexPtrArray& fvparray) {
    // Go through the face-vertex-pointer list and add each
    // face vertex pointer to the array
    fvparray.clear(); fvparray.reserve(fvpList.size());
    DLFLFaceVertexPtrList::iterator first, last;
    DLFLFaceVertexPtr fvp = NULL;
    first = fvpList.begin(); last = fvpList.end();
    while ( first != last ) {
      fvp = (*first); ++first;
      fvparray.push_back(fvp);
    }
  }

  void DLFLVertex::getOrderedFaceVertices(DLFLFaceVertexPtrArray& fvparray) {
    // Get the face vertices associated with this vertex in the clockwise rotation order
    fvparray.clear();

    DLFLFaceVertexPtr fvpstart = fvpList.front();
    if ( fvpstart == NULL ) return;

    fvparray.reserve(fvpList.size());
    DLFLFaceVertexPtr fvp = fvpstart;

    do {
      fvparray.push_back(fvp);
      fvp = fvp->vnext();
    } while ( fvp != fvpstart );
  }


  void DLFLVertex::getCornerAuxCoords(Vector3dArray& coords) const {
    coords.clear(); coords.reserve(fvpList.size());
    DLFLFaceVertexPtrList::const_iterator first, last;
    DLFLFaceVertexPtr fvp = NULL;
    first = fvpList.begin(); last = fvpList.end();
    while ( first != last ) {
      fvp = (*first); ++first;
      coords.push_back(fvp->getAuxCoords());
    }
  }

  void DLFLVertex::getOrderedCornerAuxCoords(Vector3dArray& coords) const {
    // Get the aux coords of face vertices associated with this vertex in the clockwise rotation order
    coords.clear();

    DLFLFaceVertexPtr fvpstart = fvpList.front();
    if ( fvpstart == NULL ) return;

    coords.reserve(fvpList.size());
    DLFLFaceVertexPtr fvp = fvpstart;

    do {
      coords.push_back(fvp->getAuxCoords());
      fvp = fvp->vnext();
    } while ( fvp != fvpstart );
  }


  void DLFLVertex::getFaces(DLFLFacePtrArray& fparray) {
    // Go through the face-vertex-pointer list and add
    // face pointer of each face vertex pointer to the array
    fparray.clear(); fparray.reserve(fvpList.size());
    DLFLFaceVertexPtrList::iterator first, last;
    DLFLFaceVertexPtr fvp = NULL;
    first = fvpList.begin(); last = fvpList.end();
    while ( first != last ) {
      fvp = (*first); ++first;
      fparray.push_back(fvp->getFacePtr());
    }
  }

  DLFLFaceVertexPtr DLFLVertex::getFaceVertexInFace(DLFLFacePtr fp) {
    // Get the FaceVertex belonging to the given face. If only 1 face-vertex
    // is there in the list, return that. If more than 1 exist but nothing
    // belongs to given face, return NULL
    DLFLFaceVertexPtrList::iterator first, last;
    DLFLFaceVertexPtr fvp, retfvp = NULL;
    first = fvpList.begin(); last = fvpList.end();
    if ( fvpList.size() == 1 )
      retfvp = (*first);
    else {
      while ( first != last ) {
	fvp = (*first); ++first;
	if ( fvp->getFacePtr() == fp ) {
	  retfvp = fvp;
	  break;
	}
      }
    }
    return retfvp;
  }

  DLFLFaceVertexPtr DLFLVertex::getFaceVertexWithPrev(DLFLVertexPtr vp) {
    // Get the FaceVertex which has the given Vertex before it in it's Face
    // If only 1 FaceVertex refers to this Vertex, will return that
    // If there are more than 1 and none of them satisfies the condition, returns NULL
    DLFLFaceVertexPtrList::iterator first, last;
    DLFLFaceVertexPtr fvp, retfvp = NULL;
    first = fvpList.begin(); last = fvpList.end();
    if ( fvpList.size() == 1 ) 
      retfvp = (*first);
    else {
      while ( first != last ) {
	fvp = (*first); ++first;
	if ( fvp->prev()->vertex == vp ) {
	  retfvp = fvp;
	  break;
	}
      }
    }
    return retfvp;
  }

  DLFLFaceVertexPtr DLFLVertex::getFaceVertexWithNext(DLFLVertexPtr vp) {
    // Get the FaceVertex which has the given Vertex after it in it's Face
    // If only 1 FaceVertex refers to this Vertex, will return that
    // If there are more than 1 and none of them satisfies the condition, returns NULL
    DLFLFaceVertexPtrList::iterator first, last;
    DLFLFaceVertexPtr fvp, retfvp = NULL;
    first = fvpList.begin(); last = fvpList.end();
    if ( fvpList.size() == 1 ) 
      retfvp = (*first);
    else {
      while ( first != last ) {
	fvp = (*first); ++first;
	if ( fvp->next()->vertex == vp ) {
	  retfvp = fvp;
	  break;
	}
      }
    }
    return retfvp;
  }

  DLFLFaceVertexPtr DLFLVertex::getBackFaceVertex(void) {
    // Get the FaceVertex which has the 'backface' flag set
    // If no such FaceVertex is found, returns NULL
    DLFLFaceVertexPtrList::iterator first, last;
    DLFLFaceVertexPtr fvp, retfvp = NULL;
    first = fvpList.begin(); last = fvpList.end();
    while ( first != last ) {
      fvp = (*first); ++first;
      if ( fvp->backface == true ) {
	retfvp = fvp; break;
      }
    }
    return retfvp;
  }

} // end namespace
