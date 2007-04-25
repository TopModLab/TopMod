/* $Id: DLFLConvexHull.hh,v 4.1 2004/01/20 09:26:35 vinod Exp $ */

#ifndef _DLFL_CONVEX_HULL_HH_

#define _DLFL_CONVEX_HULL_HH_

// Classes and subroutines for ConvexHull related operations using DLFL

#include "DLFLObject.hh"
#include "DLFLAux.hh"

class DLFLConvexHull : public DLFLObject
{
  protected :

        // Bit flags for use during hull construction
     static long remove;                   // Edge needs to be removed
     static long border;                      // Edge is a border edge
     static long visible;            // Face is visible from new point
     static long invisible;        // Face is invisible from new point

        // Structure for storing the input vertices
     struct InputVertex
     {
       public :

          Vector3d point;                      // Coordinates of point
          bool processed;            // Has this point been processed?
          bool onhull;                   // Is this point on the hull?
	  int index;                 // Used in column modeling - Esan

          InputVertex()
            : point(), processed(false), onhull(false)
            {}

          InputVertex(const Vector3d& p)
            : point(p), processed(false), onhull(false)
            {}
          
          InputVertex(const InputVertex& iv)
            : point(iv.point), processed(iv.processed), onhull(iv.onhull)
            {}

          ~InputVertex()
            {}

          InputVertex& operator = (const InputVertex& iv)
            {
              point = iv.point; processed = iv.processed; onhull = iv.onhull;
              return (*this);
            }
     };
     

     typedef vector<InputVertex> InputVertexArray;
     typedef list<InputVertex> InputVertexList;
     
     InputVertexArray vertices; // Input vertices for hull construction
     
  public :

        // Default constructor
     DLFLConvexHull()
       : DLFLObject(), vertices()
       {}

        // Construct from given array of Vector3ds
     DLFLConvexHull(const Vector3dArray& p)
       : DLFLObject(), vertices()
       {
         vertices.resize(p.size());
         for (int i=0; i < p.size(); ++i)
            vertices[i].point = p[i];
       }

        // Copy constructor
     DLFLConvexHull(const DLFLConvexHull& dchull)
       : DLFLObject(dchull), vertices(dchull.vertices)
       {}

        // Destructor
     ~DLFLConvexHull()
       {}

        // Assignment operator
     DLFLConvexHull& operator = (const DLFLConvexHull& dchull)
       {
         DLFLObject::operator = (dchull);
         vertices = dchull.vertices;
         return (*this);
       }


        // Create a convex hull from given list of vertices
        // Old object is destroyed
     bool createHull(const Vector3dArray& p);

        //--- Subroutines for creation of Convex Hull ---//

        // Are the 3 given points co-linear?
     static bool colinear(const Vector3d& p1, const Vector3d& p2, const Vector3d& p3);

        // Find sign of volume of tetrahedron formed by given face and given point
        // If given face is not a triangle returns 0.
     static int volumeSign(DLFLFacePtr face, const Vector3d& p);

        // Create the initial polytope
     bool doubleTriangle(void);
     
        // Add a vertex from the list to the convex hull
     bool addOne(InputVertex& iv);

        // Construct the hull from given array of points
        // Returns false is any of the given points is not on the convex hull
     bool constructHull(void);
};


#endif /* #ifndef _DLFL_CONVEX_HULL_HH_ */


/*
  $Log: DLFLConvexHull.hh,v $
  Revision 4.1  2004/01/20 09:26:35  vinod
  Added index member variable for column modeling

  Revision 4.0  2003/12/26 01:58:52  vinod
  Major version sync.

  Revision 3.0  2003/09/19 16:06:47  vinod
  Major version update

  Revision 2.2  2003/04/18 21:30:57  vinod
  Moved createHull code to source file

  Revision 2.1  2003/04/12 01:28:00  vinod
  Convex hull computation methods - initial version

*/
