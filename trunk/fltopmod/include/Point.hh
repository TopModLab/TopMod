/* File Name : Point.hh */
/* class declaration for a 2D/3D point */
/* Display routines use OpenGL */
/* Definitions in Point.cc */

#ifndef _POINT_HH_

#define _POINT_HH_

#include "OpenGL.hh"

#ifdef __cplusplus

class Point3d;                                         // Forward declaration

class Point2d
{
  public :

     double x,y;
     float size;
     
  public :

     Point2d() 
       : x(0.0), y(0.0), size(1.0)
       {}

     Point2d(double x, double y)
       : x(x), y(y), size(1.0)
       {}

     Point2d(const Point2d& p)
       : x(p.x), y(p.y), size(p.size)
       {}

     ~Point2d()
       {}

     Point2d& operator = (const Point2d& p)
       {
         x = p.x; y = p.y; size = p.size;
         return *this;
       }

     void display(void);

     friend void Vertex(const Point2d& p);
     friend void resetOrigin(const Point2d& p);
};

class Point3d
{
  public :

     double x,y,z;
     float size;
     
  public :

     Point3d() 
       : x(0.0), y(0.0), z(0.0), size(1.0)
       {}

     Point3d(double x, double y, double z=0.0)
       : x(x), y(y), z(z), size(1.0)
       {}

     Point3d(const Point3d& p)
       : x(p.x), y(p.y), z(p.z), size(p.size)
       {}

     Point3d(const Point2d& p)
       : x(p.x), y(p.y), z(0.0), size(p.size)
       {}
     
     ~Point3d()
       {}

     Point3d& operator = (const Point3d& p)
       {
         x = p.x; y = p.y; z = p.z; size = p.size;
         return *this;
       }

     void display(void);

     friend void Vertex(const Point3d& p);
     friend void resetOrigin(const Point3d& p);
};

inline void Vertex(double x, double y, double z)
{
  glVertex3d(x,y,z);
}

inline void Vertex(double x, double y)
{
  glVertex2d(x,y);
}

#endif // #ifdef __cplusplus

#endif // #ifndef _POINT_HH_
