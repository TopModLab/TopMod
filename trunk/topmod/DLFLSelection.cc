/* $Id: DLFLSelection.cc,v 4.0 2003/12/26 01:58:53 vinod Exp $ */

#include "DLFLObject.hh"

// Code for selection of vertices, edges and faces

// The class variables used for selection are defined here

DLFLVertexPtrArray DLFLObject :: vparray;
DLFLEdgePtrArray DLFLObject :: eparray;
DLFLFacePtrArray DLFLObject :: fparray;
DLFLFaceVertexPtrArray DLFLObject :: fvparray;

   // Render the vertices for selection
void DLFLObject :: renderVerticesForSelect(void) const
{
     // Adjust the size of the array used for selection to be atleast equal to
     // number of vertices
  vparray.reserve(vertex_list.size()); // STL checks for current capacity

     // Copy the pointers from the vertex_list to the vertex array
     // Render the vertices for selection at the same time (saves one loop)
  DLFLVertexPtrList :: const_iterator first, last;
  DLFLVertexPtr vptr = NULL;
  int i=0;
  
  first = vertex_list.begin(); last = vertex_list.end();
  while ( first != last )
     {
       glLoadName(i); vptr = (*first);
       glBegin(GL_POINTS);
       vptr->render();
       glEnd();
       vparray[i++] = vptr;
       ++first;
     }
}

   // Render the edges for selection
void DLFLObject :: renderEdgesForSelect(void) const
{
     // Adjust the size of the array used for selection to be atleast equal to
     // number of edges
  eparray.reserve(edge_list.size()); // STL checks for current capacity

     // Copy the pointers from the edge_list to the edge array
     // Render the edges for selection at the same time (saves one loop)
  DLFLEdgePtrList :: const_iterator first, last;
  DLFLEdgePtr eptr = NULL;
  int i=0;
  
  first = edge_list.begin(); last = edge_list.end();
  while ( first != last )
     {
       glLoadName(i); eptr = (*first);
       eptr->render();
       eparray[i++] = eptr;
       ++first;
     }
}

   // Render the faces for selection
void DLFLObject :: renderFacesForSelect(void) const
{
     // Adjust the size of the array used for selection to be atleast equal to
     // number of faces
  fparray.reserve(face_list.size()); // STL checks for current capacity

     // Copy the pointers from the face_list to the face array
     // Render the faces for selection at the same time (saves one loop)
  DLFLFacePtrList :: const_iterator first, last;
  DLFLFacePtr fptr = NULL;
  int i=0;
  
  first = face_list.begin(); last = face_list.end();
  while ( first != last )
     {
       glLoadName(i); fptr = (*first);
       if ( fptr->size() == 1 )
          {
            GLdouble depthrange[2];
            GLfloat pointsize;
            glGetDoublev(GL_DEPTH_RANGE,depthrange);
            glGetFloatv(GL_POINT_SIZE,&pointsize);
            glDepthRange(0.0,1.0-0.005);
            glPointSize(3.0);
            fptr->render_FV();
            glPointSize(pointsize);
            glDepthRange(depthrange[0],depthrange[1]);
          }
       else
          fptr->render_FV();

       fparray[i++] = fptr;
       ++first;
     }
}

   // Render the face vertices of a face for selection
void DLFLObject :: renderFaceVerticesForSelect(DLFLFacePtr fp) const
{
     // Copy the pointers from the face_list to the face array
     // Render the faces for selection at the same time (saves one loop)
  DLFLFaceVertexPtr head;
  head = fp->front();
  if ( head )
     {
       int i=0;
          // Adjust the size of the array used for selection to be atleast equal to
          // number of faces
       fvparray.reserve(fp->size()); // STL checks for current capacity

       DLFLFaceVertexPtr current = head;
       glLoadName(i);
       glBegin(GL_POINTS);
       current->render();
       glEnd();
       fvparray[i++] = current;
       current = current->next();
       while ( current != head )
          {
            glLoadName(i);
            glBegin(GL_POINTS);
            current->render();
            glEnd();
            fvparray[i++] = current;
            current = current->next();
          }
     }
}


/*
  $Log: DLFLSelection.cc,v $
  Revision 4.0  2003/12/26 01:58:53  vinod
  Major version sync.

  Revision 3.0  2003/09/19 16:06:47  vinod
  Major version update

  Revision 2.0  2001/07/25 05:12:45  vinod
  Major version sync

  Revision 1.3  2001/07/25 05:03:50  vinod
  Added modification for point-spheres when selecting faces

  Revision 1.2  2001/07/16 04:33:25  vinod
  Modified to use new list structure

  Revision 1.1.1.1  2000/11/02 19:32:43  vinod
  New DLFL Sources

*/
