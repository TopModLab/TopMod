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
*/


#ifndef _DLFL_CONVEX_HULL_HH_
#define _DLFL_CONVEX_HULL_HH_

// Classes and subroutines for ConvexHull related operations using DLFL

#include <DLFLObject.hh>
#include <DLFLCoreExt.hh>

namespace DLFL {

  class DLFLConvexHull : public DLFLObject {
  protected :

    // Bit flags for use during hull construction
    static long remove;                   // Edge needs to be removed
    static long border;                      // Edge is a border edge
    static long visible;            // Face is visible from new point
    static long invisible;        // Face is invisible from new point

    // Structure for storing the input vertices
    struct InputVertex {
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

      InputVertex& operator = (const InputVertex& iv) {
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
      for (int i=0; i < (int)p.size(); ++i)
	vertices[i].point = p[i];
    }

  private :
    // Copy constructor
    DLFLConvexHull(const DLFLConvexHull& dchull)
      : DLFLObject(), vertices(dchull.vertices)
    {}

  public :
    // Destructor
    ~DLFLConvexHull() {}

  private :
    // Assignment operator
    DLFLConvexHull& operator = (const DLFLConvexHull& dchull) {
      return (*this);
    }

  public :
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
    bool doubleTriangle( );
     
    // Add a vertex from the list to the convex hull
    bool addOne(InputVertex& iv);

    // Construct the hull from given array of points
    // Returns false is any of the given points is not on the convex hull
    bool constructHull( );
  };

} // end namespace

#endif /* _DLFL_CONVEX_HULL_HH_ */
