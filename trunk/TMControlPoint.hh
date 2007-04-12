/* $Id$ */

#ifndef _TM_CONTROL_POINTT_HH_

#define _TM_CONTROL_POINTT_HH_

#include "DLFLCommon.hh"

class TMControlPoint;
typedef TMControlPoint * TMControlPointPtr;

class TMControlPoint
{
  public :

     Vector3d position;
     Vector3d normal;
     Vector2d texcoord;
     RGBColor color;
     double radius;

  public :

        // Default constructor
     TMControlPoint()
       : position(), normal(), texcoord(), color(), radius(0.0)
       {}

        // Copy constructor
     TMControlPoint(const TMControlPoint& tmcp)
       : position(tmcp.position), normal(tmcp.normal), texcoord(tmcp.texcoord),
         color(tmcp.color), radius(tmcp.radius)
       {}

        // Destructor (Pure virtual)
     virtual ~TMControlPoint()
       {}

        // Assignment operator
     TMControlPoint& operator = (const TMControlPoint& tmcp)
       {
         position = tmcp.position;
         normal = tmcp.normal;
         texcoord = tmcp.texcoord;
         color = tmcp.color;
         radius = tmcp.radius;
         return (*this);
       }
};

#endif /* #ifndef _TM_CONTROL_POINT_HH_ */


/*
  $Log$
*/
