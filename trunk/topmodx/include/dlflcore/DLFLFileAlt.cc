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

#include "DLFLObject.hh"
#include <cstdio>

namespace DLFL {

  struct Corner {
    // Structure that defines a corner in terms of the vertex indices
  public :
    int index;                                        // Index of vertex
    int prev, next;                                   // Indices of previous and next vertices

    Corner(int i, int p, int n)
      : index(i), prev(p), next(n) {}

    Corner(int i=-1)
      : index(i), prev(-1), next(-1) {}

    Corner(const Corner& c)
      : index(c.index), prev(c.prev), next(c.next) {}

    Corner& operator = (const Corner& c) {
      index = c.index; prev = c.prev; next = c.next;
      return (*this);
    }
  };

  struct Edge {
    // Structure which defines an edge in terms of corners
    Corner start, end;                                // Start and end corners of the edge

    Edge()
      : start(), end() {}

    Edge(Corner s, Corner e)
      : start(s), end(e) {}

    Edge(const Edge& e)
      : start(e.start), end(e.end) {}

    Edge& operator = (const Edge& e) {
      start = e.start; end = e.end;
      return (*this);
    }
  };

  typedef vector<Corner> CornerArray;
  typedef list<Corner> CornerList;
  typedef vector<Edge> EdgeArray;
  typedef list<Edge> EdgeList;

  struct Face {
    // Structure to hold Corners in a face. Contains an array of Corners
  public :
    CornerArray corners;

    Face()
      : corners() {}

    ~Face() {
      corners.clear();
    }
  };

  typedef vector<Face> FaceArray;

  static FaceArray faces;                                // Array of faces as read from the OBJ file
  // When faces are read these arrays will be created
  // And then used to create the object using repeated
  // edge insertions

  static DLFLVertexPtrArray vertex_array;                // Array containing vertices from each point-sphere
  // created when vertices are read

  static EdgeList open_edges;                            // Lists containing the edges not yet inserted
  // because of ambiguities in finding correct corner
  // These edges will be inserted at the end of the first loop

  static void updateCornersInFace(Face& face) {
    // Update the prev and next fields for all Corners in the Face
    // Assumes that the index fields have been correctly set for all Corners
    int size, pindex, nindex;
    size = face.corners.size();
    for (int i=0; i < size; ++i) {
      pindex = i-1; if ( pindex == -1 ) pindex = size-1;
      nindex = i+1; if ( nindex == size ) nindex = 0;
      face.corners[i].prev = face.corners[pindex].index;
      face.corners[i].next = face.corners[nindex].index;
    }
  }

  static void printFace(const Face& face) {
    cout << "Face : ";
    for (int i=0; i < face.corners.size(); ++i)
      cout << "<"
	   << face.corners[i].prev << ","
	   << face.corners[i].index << ","
	   << face.corners[i].next << ">"
	   << "  ";
    cout << endl;
  }

  // Alternate versions of insertEdge subroutines which try to make sure original corners are always
  // together and are in the "back" face
  DLFLEdgePtr insertEdgeAltCoFacial( DLFLObjectPtr obj, 
				     DLFLFaceVertexPtr fvptr1, DLFLFaceVertexPtr fvptr2, 
				     bool set_type) {
    // Insert an edge between 2 corners in the same face. Doesn't check if both
    // corners are in the same face or not
    // Doesn't check to see if there is already an edge between the two corners
    // Insertion of the Edge will split the Face into 2 faces
    DLFLFacePtr fp = fvptr1->getFacePtr();
    DLFLMaterialPtr matl = fp->material();

    DLFLFaceVertexPtr fvp, temp;
    DLFLEdgePtr ep;
  
    // Only 1 new face is created.
    DLFLFacePtr newfaceptr = new DLFLFace(matl);

    // Use same type as fp for the new face. Useful for doo-Sabin subdivision
    newfaceptr->setType(fp->getType());
  
    // Set type if caller requested it
    if ( set_type ) {
      // Reset types of the 2 FaceVertexes
      fvptr1->resetType(); fvptr2->resetType();
      fp->setType(FTNew); newfaceptr->setType(FTNew);
    }

    // Create the new face.

    // Add a copy of fvptr2 to the new face
    // Update edge which originally pointed to fvptr2 to point to new face vertex
    temp = newfaceptr->addVertex(fvptr2); temp->addSelfToVertex();
    ep = fvptr2->getEdgePtr(); ep->resetFaceVertexPtr(fvptr2); ep->setNullFaceVertexPtr(temp);

    // Remove face-vertices from fvptr2 to fvptr1
    // from old face and add it to the new face
    fvp = fvptr2->next();
    while ( fvp != fvptr1 ) {
      // Remove fvp from old face and add it to the new face
      temp = fvp->next();
      fp->deleteVertexPtr(fvp); newfaceptr->addVertexPtr(fvp);
      fvp = temp;
    }

    // Add copy of fvptr1 to new face
    // This copy will also be one of the corners of the new Edge
    DLFLFaceVertexPtr fvptr1_copy;
    temp = newfaceptr->addVertex(fvptr1); temp->addSelfToVertex(); fvptr1_copy = temp;

    // Add new face to face list
    obj->addFacePtr(newfaceptr);

    // Create the new Edge between fvptr1_copy and fvptr2 and do necessary updates
    DLFLEdgePtr newedgeptr = new DLFLEdge;
    newedgeptr->setFaceVertexPointers(fvptr1_copy,fvptr2); newedgeptr->updateFaceVertices();
    obj->addEdgePtr(newedgeptr);
    if ( set_type ) newedgeptr->setType(ETNew);

    return newedgeptr;
  }

  DLFLEdgePtr insertEdgeAltNonCoFacial(DLFLObjectPtr obj, 
				       DLFLFaceVertexPtr fvptr1, DLFLFaceVertexPtr fvptr2, 
				       DLFLMaterialPtr matl = NULL) {
    // Insert an edge between 2 corners belonging to different faces
    // Doesn't check if the corners belong to different faces
    DLFLFacePtr fp1 = fvptr1->getFacePtr();
    DLFLFacePtr fp2 = fvptr2->getFacePtr();

    DLFLFacePtr newfaceptr;                              // The new Face
    DLFLFaceVertexPtr fvp, temp, fvptr1_copy;
    int numverts1=0, numverts2=0;                        // No. of verts in each face

    numverts1 = fp1->size(); numverts2 = fp2->size();

    // Reorder face 2 so that it ends at fvptr2
    fp2->reorder(fvptr2->next());

    // Reorder face 1 so that it starts at fvptr1
    fp1->reorder(fvptr1);

    // fp2 will become the new face
    newfaceptr = fp2;

    // If a material is specified use it, otherwise stick with material of the newface (fp2)
    if ( matl ) newfaceptr->setMaterial(matl);

    // Go through all vertices in fp1 and add them to the new face and delete them from the old face
    fvp = fp1->front();
    do {
      temp = fvp->next();
      fp1->deleteVertexPtr(fvp); newfaceptr->addVertexPtr(fvp);
      fvp = temp;
    } while ( fp1->front() );

    // Check if fp1 is a point-sphere. If not add a copy of fvptr1
    fvptr1_copy = fvptr1;
    if ( numverts1 > 1 ) {
      // Add copy of fvptr1. This copy will be a starting corner of the new edge
      temp = newfaceptr->addVertex(fvptr1); temp->addSelfToVertex();
      fvptr1_copy = temp;
    }

    // Check if fp2 is a point-sphere. If not add a copy of fvptr2
    if ( numverts2 > 1 ) {
      // Add copy of fvptr2. The edge which fvptr2 originally pointed to
      // must be updated to point to the new face vertex
      temp = newfaceptr->addVertex(fvptr2); temp->addSelfToVertex();
      DLFLEdgePtr ep = fvptr2->getEdgePtr();
      ep->resetFaceVertexPtr(fvptr2); ep->setNullFaceVertexPtr(temp);
    }

    // Delete face 1 from the face list and free the pointer
    DLFLFacePtrList face_list = obj->getFaceList();
    face_list.remove(fp1); 
    delete fp1;

    // Create the new Edge and do necessary updates
    DLFLEdgePtr newedgeptr = new DLFLEdge;
    newedgeptr->setFaceVertexPointers(fvptr1_copy,fvptr2); newedgeptr->updateFaceVertices();
    obj->addEdgePtr(newedgeptr);
  
    return newedgeptr;
  }

  DLFLEdgePtr insertEdgeAlt(DLFLObjectPtr obj, 
			    DLFLFaceVertexPtr fvptr1, DLFLFaceVertexPtr fvptr2, bool set_type = false,
			    DLFLMaterialPtr matl = NULL) {
    if ( coFacial(fvptr1,fvptr2) ) {
      if ( fvptr1 != fvptr2 ) 
	return insertEdgeAltCoFacial(obj,fvptr1,fvptr2,set_type);
      return NULL;
    }
    return insertEdgeAltNonCoFacial(obj,fvptr1,fvptr2,matl);
  }

  void DLFLObject::readObjectAlt(istream& i) {
    if ( !i ) {
      cerr  << "Incomplete OBJ file." << endl;
      return;
    }

    // Clear the object first
    reset();

    // Clear the static arrays
    faces.clear(); vertex_array.clear();

    DLFLMaterialPtr cur_mtl = matl_list.front();
    RGBColor color;
    bool matl_added = false;
    char matl_name[10];
    Vector3d xyz;
    Vector2d uv;
    char c,c2;
    Corner corner;  // Represents a corner of a face
    Face face;      // Represents a face in the object

    // Read each line and set the Vertex, Normal, Face, Color or Texture
    while ( i ) {
      removeWhiteSpace(i); i.get(c); i.get(c2);
      if ( c == 'c' && c2 == ' ' ) {
	// Read a color specification
	i >> color; cur_mtl = findMaterial(color);
	if ( cur_mtl == NULL ) {                    // No matching material found
	  if (matl_added == false) {
	    // No new materials have been added.
	    // Set default material to be this color
	    setColor(color); matl_added = true;
	    cur_mtl = matl_list.front();
	  } else {
	    // Atleast 1 new material was added, but none of the
	    // existing materials match this color. So create a new
	    // material with this color and add it to the list
	    sprintf(matl_name,"material%d",matl_list.size());
	    matl_list.push_back(new DLFLMaterial(matl_name,color));
	    cur_mtl = matl_list.back();
	  }
	}
      } else if ( c == 'v' ) {
	if ( c2 == ' ' ) {
	  // Read a vertex specification
	  i >> xyz;

	  // Create a point sphere using this vertex and add the vertex to the vertex array.
	  // NOTE: Vertex is automatically created and added to vertex list
	  // A new face will also be created and added to face list
	  DLFLFaceVertexPtr bfvptr = createPointSphere(xyz,cur_mtl);
	  bfvptr->backface = true; // This is not used in the current method
	  vertex_array.push_back(bfvptr->vertex);
	} else if ( c2 == 'n' ) {
	  // Read a normal specification
	  // NORMALS ARE NOT USED IN THIS METHOD, BUT WE MUST STILL READ THE LINE
	  i >> xyz; 
	} else if ( c2 == 't' ) {
	  // Read a texture coordinate
	  // TEXTURE COORDS ARE NOT USED IN THIS METHOD, BUT WE MUST STILL READ THE LINE
	  i >> uv;
	}
      } else if ( c == 'f' && c2 == ' ' ) {
	// Read a face specification
	c = i.peek();
	if ( c != '\n' ) {
	  int v,vt,vn;

	  // Read vertices in order and create a Corner for each vertex
	  // Add it to the Face. prev and next fields for each Corner will
	  // be updated after all corners in a face have been read
	  do {
	    i >> v; vt = -1; vn = -1; c = i.peek();
	    if ( c == '/' ) {
	      i.get(c); c = i.peek();
	      if ( c != '/' ) {
		i >> vt; c = i.peek();
	      }
	      if ( c == '/' ) {
		i.get(c); i >> vn; c = i.peek();
	      }
	    }

	    // We have v,vt and vn now
	    // Set the index for the vertex for current Corner
	    // prev and next fields will be updated later
	    corner.index = v-1;
	    face.corners.push_back(corner);
	  } while ( c != '\n' );

	  // We now have a face with indices for the corners
	  // We have to update the prev and next fields for all corners in the face
	  updateCornersInFace(face);

	  // Add the face to the array of faces
	  faces.push_back(face);

	  // Clear the corners from the face
	  face.corners.clear();
	}
      }
      if ( c2 != '\n' ) readTillEOL(i);
    }

    // Go through the array of Faces and create the object using insertEdge operations
    // to create each edge in the Face
    // If an edge already exists between 2 vertices it wont be created
    DLFLFaceVertexPtr sfvp, efvp;                        // Edge start and end corners
    DLFLVertexPtr svp, evp;                              // Edge start and end vertices
    DLFLVertexPtr svp_prev, svp_next, evp_prev, evp_next; // previous and next vertices for each corner
    Corner scorner, ecorner;                             // Starting and ending corners
    for (int j=0; j < faces.size(); ++j) {
      face = faces[j];
      int size = face.corners.size();
      for (int k=0; k < size; ++k) {
	scorner = face.corners[k]; ecorner = face.corners[(k+1)%size];
	svp = vertex_array[scorner.index]; evp = vertex_array[ecorner.index];
	if ( edgeExists(svp,evp) == false ) {
	  // Find the corners referring to svp and evp which have to be connected
	  // The previous and next vertex information for the current corner will
	  // be used to find the correct corners
	  svp_prev = vertex_array[scorner.prev]; svp_next = vertex_array[scorner.next];
	  evp_prev = vertex_array[ecorner.prev]; evp_next = vertex_array[ecorner.next];

	  sfvp = svp->getFaceVertexWithPrev(svp_prev);
	  if ( sfvp == NULL ) sfvp = svp->getFaceVertexWithNext(svp_next);
                 
	  efvp = evp->getFaceVertexWithPrev(evp_prev);
	  if ( efvp == NULL ) efvp = evp->getFaceVertexWithNext(evp_next);

	  // Check to make sure we have non NULL FaceVertex pointers
	  if ( sfvp != NULL && efvp != NULL ) {
	    // Insert an edge between sfvp and efvp
	    insertEdgeAlt(this,sfvp,efvp);
	  } else {
	    // cout << "Could not find correct corner for vertex. Postponing edge insertion (1)" << endl;
	    Edge edge(scorner,ecorner);
	    open_edges.push_back(edge);
	  }
	}
      }
    }

    // Insert edges which were not inserted earlier

    EdgeList::iterator first, last;
    Edge edge;
    first = open_edges.begin(); last = open_edges.end();
    int depth = 2, num=0;
    int size = open_edges.size();
    int psize = size;
    bool looping = false;
    // Check for an infinite loop has to be included here since we add elements to the list
    // within this loop
    while ( !open_edges.empty() ) {
      edge = open_edges.front(); open_edges.pop_front();
      scorner = edge.start; ecorner = edge.end;
      svp = vertex_array[scorner.index]; evp = vertex_array[ecorner.index];
      if ( edgeExists(svp,evp) == false ) {
	// Find the corners referring to svp and evp which have to be connected
	// The previous and next vertex information for the current corner will
	// be used to find the correct corners
	svp_prev = vertex_array[scorner.prev]; svp_next = vertex_array[scorner.next];
	evp_prev = vertex_array[ecorner.prev]; evp_next = vertex_array[ecorner.next];

	sfvp = svp->getFaceVertexWithPrev(svp_prev);
	if ( sfvp == NULL ) sfvp = svp->getFaceVertexWithNext(svp_next);
	// If we are getting into an infinite loop try backfacing corner
	if ( sfvp == NULL && looping ) {
	  //                 cout << "Couldn't find correct corner. Trying backfacing corner (" << depth << ")" << endl;
	  sfvp = svp->getBackFaceVertex();
	}

	efvp = evp->getFaceVertexWithPrev(evp_prev);
	if ( efvp == NULL ) efvp = evp->getFaceVertexWithNext(evp_next);
	// If we are getting into an infinite loop try backfacing corner
	if ( efvp == NULL && looping ) {
	  //                 cout << "Couldn't find correct corner. Trying backfacing corner (" << depth << ")" << endl;
	  efvp = evp->getBackFaceVertex();
	}

	// Check to make sure we have non NULL FaceVertex pointers
	if ( sfvp != NULL && efvp != NULL ) {
	  // Insert an edge between sfvp and efvp
	  insertEdgeAlt(this,sfvp,efvp);
	} else {
	  //cout << "Could not find correct corner for vertex. Postponing edge insertion ("
	  //     << depth << ")" << endl;
	  open_edges.push_back(edge);
	}
      }
      ++num;
      if ( num >= size ) {
	//cout << psize << " " << size << endl;
	depth++; size = open_edges.size();
	num = 0;
	// If we are getting into an infinte loop, set looping flag
	// so backfacing corners will be used next time
	// NOTE: That will always break the deadlocks and thus prevent infinite loops
	// We want to do that only as a last resort
	if ( size == psize ) looping = true;
	psize = size;
      }
    }

    // Make all Vertexes, Edges and Faces unique (IDs become sequential)
    makeUnique();
  }

} // end namespace
