#ifndef _DLFLSELECTION_H_
#define _DLFLSELECTION_H_

#include <DLFLObject.hh>

using namespace DLFL;

// Code for selection of vertices, edges and faces

// Render the vertices for selection
void renderVerticesForSelect( DLFLObjectPtr obj );
// Render the edges for selection
void renderEdgesForSelect( DLFLObjectPtr obj );
// Render the faces for selection
void renderFacesForSelect( DLFLObjectPtr obj );
// Render the face vertices of a face for selection
void renderFaceVerticesForSelect( DLFLFacePtr fp );

#endif // _DLFLSELECTION_H_
