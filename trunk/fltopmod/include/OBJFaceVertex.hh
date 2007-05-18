/* $Id: OBJFaceVertex.hh,v 1.1 2006/09/04 19:22:53 stuart Exp $ */

#ifndef _OBJ_FACE_VERTEX_HH_

#define _OBJ_FACE_VERTEX_HH_

// Class for a face vertex for the OBJObject class

#include <Vector2d.hh>
#include <Vector3d.hh>
#include "OBJCommon.hh"
#include "OBJVertex.hh"

class OBJFaceVertex
{
  public :

     OBJVertexPtr       vertex;                        // Associated vertex pointer
     Vector3d           normal;                        // Normal
     RGBColor           color;                         // Color 
     Vector2d           texcoord;                      // Texture coordinate

        // Default constructor
     OBJFaceVertex()
       : vertex(NULL), normal(), color(1), texcoord()
       {}

        // Copy constructor
     OBJFaceVertex(const OBJFaceVertex& ofv)
       : vertex(ofv.vertex), normal(ofv.normal), color(ofv.color), texcoord(ofv.texcoord)
       {}

        // Destructor
     ~OBJFaceVertex()
       {}

        // Assignment operator
     OBJFaceVertex& operator = (const OBJFaceVertex& ofv)
       {
         vertex = ofv.vertex; normal = ofv.normal; color = ofv.color; texcoord = ofv.texcoord;
         return (*this);
       }

     void reset(void)
       {
         vertex = NULL; normal.reset(); color = 1.0; texcoord.reset();
       }

     void update(void)
       {
         vertex->addFaceVertex(this);
       }
     
        // Do a glVertex on this vertex, using the normal and with color
     friend void glVertexCN(const OBJFaceVertex& ofv)
       {
         if ( ofv.vertex )
            {
              double x,y,z; ofv.normal.get(x,y,z);
              glNormal3f(x,y,z);
              glColor(ofv.color);
              glVertex(*(ofv.vertex));
            }
       }
     
        // Do a glVertex on this vertex, using the normal and without color
     friend void glVertexN(const OBJFaceVertex& ofv)
       {
         if ( ofv.vertex )
            {
              double x,y,z; ofv.normal.get(x,y,z);
              glNormal3f(x,y,z);
              glVertex(*(ofv.vertex));
            }
       }
     
        // Do a glVertex on this vertex, without using the normal and with color
     friend void glVertexC(const OBJFaceVertex& ofv)
       {
         if ( ofv.vertex )
            {
              glColor(ofv.color);
              glVertex(*(ofv.vertex));
            }
       }
     
        // Do a glVertex on this vertex, without using the normal and without color
        // This function is not named glVertex to allow using it in the
        // STL for_each algorithm, since glVertex is overloaded for OBJVertex also
     friend void glVertexOFV(const OBJFaceVertex& ofv)
       {
         if ( ofv.vertex ) glVertex(*(ofv.vertex));
       }


        //-- Same as above functions but with texture coordinates also --//

     friend void glVertexCNT(const OBJFaceVertex& ofv)
       {
         if ( ofv.vertex )
            {
              double x,y,z; ofv.normal.get(x,y,z);
              glNormal3f(x,y,z);
              glColor(ofv.color);
              glTexCoord2d(ofv.texcoord[0],ofv.texcoord[1]);
              glVertex(*(ofv.vertex));
            }
       }
     
     friend void glVertexNT(const OBJFaceVertex& ofv)
       {
         if ( ofv.vertex )
            {
              double x,y,z; ofv.normal.get(x,y,z);
              glNormal3f(x,y,z);
              glTexCoord2d(ofv.texcoord[0],ofv.texcoord[1]);
              glVertex(*(ofv.vertex));
            }
       }
     
     friend void glVertexCT(const OBJFaceVertex& ofv)
       {
         if ( ofv.vertex )
            {
              glColor(ofv.color);
              glTexCoord2d(ofv.texcoord[0],ofv.texcoord[1]);
              glVertex(*(ofv.vertex));
            }
       }

     friend void glVertexOFVT(const OBJFaceVertex& ofv)
       {
         if ( ofv.vertex )
            {
              glTexCoord2d(ofv.texcoord[0],ofv.texcoord[1]);
              glVertex(*(ofv.vertex));
            }
       }
};

#endif /* #ifndef _OBJ_FACE_VERTEX_HH_ */

/*
  $Log: OBJFaceVertex.hh,v $
  Revision 1.1  2006/09/04 19:22:53  stuart
  Added includes and libs.

  Revision 1.4  2000/09/22 18:16:12  vinod
  Modified to use OBJCommon.hh

  Revision 1.3  2000/04/25 04:01:24  vinod
  Added functions for using texturing

  Revision 1.2  2000/04/20 04:39:19  vinod
  Added texture coordinate member

  Revision 1.1  2000/04/19 21:52:23  vinod
  OBJObject class. Initial version. Modified from SimpleOBJ

*/
