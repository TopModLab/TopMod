/* $Id$ */

#ifndef _TM_FACE_CONTROL_POINT_HH_

#define _TM_FACE_CONTROL_POINT_HH_

#include "TMControlPoint.hh"
#include "DLFLFace.hh"

class TMFaceControlPoint;
typedef vector<TMFaceControlPoint> TMFaceControlPointArray;

class TMFaceControlPoint : public TMControlPoint
{
  protected :

     DLFLFacePtr face;

  public :

        // Default constructor
     TMFaceControlPoint()
       : TMControlPoint(), face(NULL)
       {}

        // 1-argument constructor
     TMFaceControlPoint(DLFLFacePtr f)
       : TMControlPoint(), face(f)
       {
         if ( face ) position = face->geomCentroid();
       }

        // Copy constructor
     TMFaceControlPoint(const TMFaceControlPoint& tmfcp)
       : TMControlPoint(tmfcp), face(tmfcp.face)
       {}

        // Destructor
     ~TMFaceControlPoint()
       {}
     
        // Assignment operator
     TMFaceControlPoint& operator = (const TMFaceControlPoint& tmfcp)
       {
         TMControlPoint::operator = (tmfcp);
         face = tmfcp.face;
         return (*this);
       }

     void setFace(DLFLFacePtr f)
       {
         face = f;
         if ( face ) position = face->geomCentroid();
       }

     DLFLFacePtr getFace(void) const
       {
         return face;
       }
};

#endif /* #ifndef _TM_FACE_CONTROL_POINT_HH_ */


/*
  $Log$
*/
