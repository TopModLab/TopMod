/* $Id$ */

#ifndef _TM_VERTEX_CONTROL_POINT_HH_

#define _TM_VERTEX_CONTROL_POINT_HH_

#include "TMControlPoint.hh"
#include "DLFLVertex.hh"

class TMVertexControlPoint;
typedef vector<TMVertexControlPoint> TMVertexControlPointArray;

class TMVertexControlPoint : public TMControlPoint
{
  protected :

     DLFLVertexPtr vertex;

  public :

        // Default constructor
     TMVertexControlPoint()
       : TMControlPoint(), vertex(NULL)
       {}

        // 1-argument constructor
     TMVertexControlPoint(DLFLVertexPtr v)
       : TMControlPoint(), vertex(v)
       {
         if ( vertex ) position = vertex->coords;
       }

        // Copy constructor
     TMVertexControlPoint(const TMVertexControlPoint& tmvcp)
       : TMControlPoint(tmvcp), vertex(tmvcp.vertex)
       {}

        // Destructor
     ~TMVertexControlPoint()
       {}
     
        // Assignment operator
     TMVertexControlPoint& operator = (const TMVertexControlPoint& tmvcp)
       {
         TMControlPoint::operator = (tmvcp);
         vertex = tmvcp.vertex;
         return (*this);
       }

     void setVertex(DLFLVertexPtr v)
       {
         vertex = v;
         if ( vertex ) position = vertex->coords;
       }

     DLFLVertexPtr getVertex(void) const
       {
         return vertex;
       }
};

#endif /* #ifndef _TM_VERTEX_CONTROL_POINT_HH_ */


/*
  $Log$
*/
