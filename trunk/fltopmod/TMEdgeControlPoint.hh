/* $Id$ */

#ifndef _TM_EDGE_CONTROL_POINT_HH_

#define _TM_EDGE_CONTROL_POINT_HH_

#include "TMControlPoint.hh"
#include "DLFLEdge.hh"

class TMEdgeControlPoint;
typedef vector<TMEdgeControlPoint> TMEdgeControlPointArray;

class TMEdgeControlPoint : public TMControlPoint
{
  protected :

     DLFLEdgePtr edge;

  public :

        // Default constructor
     TMEdgeControlPoint()
       : TMControlPoint(), edge(NULL)
       {}

        // 1-argument constructor
     TMEdgeControlPoint(DLFLEdgePtr e)
       : TMControlPoint(), edge(e)
       {
         if ( edge ) position = edge->getMidPoint();
       }

        // Copy constructor
     TMEdgeControlPoint(const TMEdgeControlPoint& tmecp)
       : TMControlPoint(tmecp), edge(tmecp.edge)
       {}

        // Destructor
     ~TMEdgeControlPoint()
       {}
     
        // Assignment operator
     TMEdgeControlPoint& operator = (const TMEdgeControlPoint& tmecp)
       {
         TMControlPoint::operator = (tmecp);
         edge = tmecp.edge;
         return (*this);
       }

     void setEdge(DLFLEdgePtr e)
       {
         edge = e;
         if ( edge ) position = edge->getMidPoint();
       }

     DLFLEdgePtr getEdge(void) const
       {
         return edge;
       }
};

#endif /* #ifndef _TM_EDGE_CONTROL_POINT_HH_ */


/*
  $Log$
*/
