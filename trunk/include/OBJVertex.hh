/* $Id: OBJVertex.hh,v 1.1 2006/09/04 19:22:53 stuart Exp $ */

#ifndef _OBJ_VERTEX_HH_

#define _OBJ_VERTEX_HH_

// Class to store a vertex for the OBJObject class

#include <GL/gl.h>
#include <Vector3d.hh>
#include "OBJCommon.hh"

class OBJVertex
{
  public :

     Vector3d              coords;                     // Coordinates of vertex
     OBJFaceVertexPtrList  fvptr_list;                 // List of face vertices sharing this vertex
     
        // Default constructor
     OBJVertex()
       : coords(), fvptr_list()
       {}

        // 1 argument constructor
     OBJVertex(const Vector3d& vec)
       : coords(vec), fvptr_list()
       {}

        // 3 argument constructor
     OBJVertex(double x, double y, double z)
       : coords(x,y,z), fvptr_list()
       {}

        // Copy constructor
     OBJVertex(const OBJVertex& ov)
       : coords(ov.coords), fvptr_list(ov.fvptr_list)
       {}

        // Destructor
     ~OBJVertex()
       {}

        // Assignment operator
     OBJVertex& operator = (const OBJVertex& ov)
       {
         coords = ov.coords; fvptr_list = ov.fvptr_list;
         return (*this);
       }

     void reset(void)
       {
         coords.reset(); fvptr_list.clear();
       }

     void addFaceVertex(OBJFaceVertexPtr fvptr)
       {
         fvptr_list.push_back(fvptr);
       }

        // Read a vertex from an input stream.
        // The 3 coordinates should be specified separated by spaces (as in OBJ format)
     friend istream& operator >> (istream& i, OBJVertex& ov)
       {
            // Read x,y,z coordinates.
         double x,y,z;
         i >> x >> y >> z;
         ov.coords.set(x,y,z);
         return i;
       }

        // Write a vertex to an output stream. Writes in OBJ format
     friend ostream& operator << (ostream& o, const OBJVertex& ov)
       {
            // Only coordinates are written.
         double x,y,z;
         ov.coords.get(x,y,z);
         o << "v " << x << ' ' << y << ' ' << z << endl;
         return o;
       }

        // Do a glVertex on this point
     friend void glVertex(const OBJVertex& ov)
       {
         glVertex3dv(ov.coords.getCArray());
       }
};

#endif /* #ifndef _OBJ_VERTEX_HH_ */

/*
  $Log: OBJVertex.hh,v $
  Revision 1.1  2006/09/04 19:22:53  stuart
  Added includes and libs.

  Revision 1.5  2000/09/25 17:59:27  vinod
  Changed face vertex list to linked list from array

  Revision 1.4  2000/09/22 18:18:43  vinod
  Modified to use OBJCommon.hh

  Revision 1.3  2000/05/05 20:53:07  vinod
  Now uses vector form of glVertex

  Revision 1.2  2000/04/20 04:39:03  vinod
  Added 1-arg constructor

  Revision 1.1  2000/04/19 21:52:24  vinod
  OBJObject class. Initial version. Modified from SimpleOBJ

*/
