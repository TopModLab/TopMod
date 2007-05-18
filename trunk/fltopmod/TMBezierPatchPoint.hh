/* $Id$ */

#ifndef _TM_BEZIER_PATH_POINT_HH_

#define _TM_BEZIER_PATH_POINT_HH_

#include "include/Vector3d.hh"
#include <vector>

using namespace std;

class TMBezierPatchPoint;
typedef vector<TMBezierPatchPoint> TMBezierPatchPointArray;

class TMBezierPatchPoint
{
  public :

     Vector3d position; // Geometric position of this point
     double u,v; // Parametric coordinates of this point

  public :

        // Default constructor
     TMBezierPatchPoint()
       : position(), u(0.0), v(0.0)
       {}

        // 1-argument constructor
     TMBezierPatchPoint(const Vector3d& p)
       : position(p), u(0.0), v(0.0)
       {}

        // 3-argument constructor
     TMBezierPatchPoint(const Vector3d& p, double _u, double _v)
       : position(p), u(_u), v(_v)
       {}
     
        // Copy constructor
     TMBezierPatchPoint(const TMBezierPatchPoint& tmbpp)
       : position(tmbpp.position), u(tmbpp.u), v(tmbpp.v)
       {}

        // Destructor
     ~TMBezierPatchPoint()
       {}

        // Assignment operator
     TMBezierPatchPoint& operator = (const TMBezierPatchPoint& tmbpp)
       {
         position = tmbpp.position;
         u = tmbpp.u;
         v = tmbpp.v;
         return (*this);
       }

        // Assignment from a Vector3d
     TMBezierPatchPoint& operator = (const Vector3d& p)
       {
         position = p;
         return (*this);
       }
};

#endif /* #ifndef _TM_BEZIER_PATH_POINT_HH_ */


/*
  $Log$
*/
