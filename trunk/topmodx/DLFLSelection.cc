#include "DLFLSelection.hh"
#include "GeometryRenderer.hh"

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
