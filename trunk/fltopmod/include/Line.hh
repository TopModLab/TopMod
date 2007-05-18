/* File Name : Line.hh */
/* class declaration for 2D/3D lines */
/* Uses class Point (Point.hh,Point.cc) */
/* Definitions in file Line.cc */

#ifndef _LINE_HH_

#define _LINE_HH_

#include "OpenGL.hh"
#include "Point.hh"

#ifdef __cplusplus

class Line3d;                                          // Forward declaration

class Line2d
{
  public :

     Point2d spt;                                      // Starting point
     Point2d ept;                                      // Ending point

  public :

        // Constructors
     Line2d()
       : spt(0.0,0.0), ept(0.0,0.0)
       {}

     Line2d(const Point2d& sp, const Point2d& ep)
       : spt(sp), ept(ep)
       {}

     Line2d(double sx, double sy,
            double ex, double ey)
       : spt(sx,sy), ept(ex,ey)
       {}

        // Copy constructors
     Line2d(const Line2d& l)
       : spt(l.spt), ept(l.ept)
       {}

        // Destructor
     ~Line2d()
       {}

        // Assignment operator
     Line2d& operator = (const Line2d& l)
       {
         spt = l.spt; ept = l.ept;
         return *this;
       }

        // Member functions
     Point2d& start(void)
       {
         return spt;
       }

     Point2d& end(void)
       {
         return ept;
       }
     
     void display(void);
};

class Line3d
{
  public :

     Point3d spt;                                      // Starting point
     Point3d ept;                                      // Ending point

  public :

        // Constructors
     Line3d()
       : spt(0.0,0.0,0.0), ept(0.0,0.0,0.0)
       {}

     Line3d(const Point3d& sp, const Point3d& ep)
       : spt(sp), ept(ep)
       {}

     Line3d(const Point2d& sp, const Point2d& ep)
       : spt(sp), ept(ep)
       {}
     
     Line3d(const Point2d& sp, const Point3d& ep)
       : spt(sp), ept(ep)
       {}

     Line3d(const Point3d& sp, const Point2d& ep)
       : spt(sp), ept(ep)
       {}
     
     Line3d(double sx, double sy, double sz,
            double ex, double ey, double ez)
       : spt(sx,sy,sz), ept(ex,ey,ez)
       {}

     Line3d(double sx, double sy, double ex, double ey)
       : spt(sx,sy), ept(ex,ey)
       {}

        // Copy constructors
     Line3d(const Line3d& l)
       : spt(l.spt), ept(l.ept)
       {}

     Line3d(const Line2d& l)
       : spt(l.spt), ept(l.ept)
       {}

        // Destructor
     ~Line3d()
       {}

        // Assignment operator
     Line3d& operator = (const Line3d& l)
       {
         spt = l.spt; ept = l.ept;
         return *this;
       }

        // Member functions
     Point3d& start(void)
       {
         return spt;
       }

     Point3d& end(void)
       {
         return ept;
       }
     
     void display(void);
};

void displayLine(const Point2d& spt, const Point2d& ept);
void displayLine(const Point3d& spt, const Point3d& ept);

#endif // #ifdef __cplusplus

#endif // #ifndef _LINE_HH_
