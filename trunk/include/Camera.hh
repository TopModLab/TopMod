/* $Id: Camera.hh,v 1.1 2006/09/04 19:22:52 stuart Exp $ */

#ifndef _CAMERA_HH_

#define _CAMERA_HH_

// Class to encapsulate an OpenGL type camera
// Contains the eye position, point of interest (lookat) and up vector
// along with projection parameters (field-of-view, aspect ratio, near/far)
// Supports both perspective and orthographic projections
// Supports only symmetric-views (along each axis)

#include "Vector3d.hh"
#include <GL/gl.h>
#include <math.h>
#include <Inlines.hh>

enum ProjectionType { Orthographic=0, Perspective=1 };

class Camera
{
  protected :

     Vector3d   center;                                // Point of interest
     Vector3d   eye;                                   // Eye position
     Vector3d   up;                                    // Up-vector
     double nearplane;
     double farplane;                             // Near/far clipping planes

        // For orthographic/perspective projection
     double     umin, umax, vmin, vmax;                // Viewing volume
     double     fovy, aspect;                          // Y field-of-view, aspect ratio

     ProjectionType projtype;                          // Type of projection
     double     dist;                                  // Dist from eye to center

        // For GL selection mode
     double     mousex, mousey, pickw, pickh;          // Selection region for gluPickMatrix
     GLint *    viewport;                              // The viewport
     bool       pickmode;                              // Are we in selection mode?

  public :

     Camera(double aspratio=1.0)
       : center(0,0,0), eye(0,0,50), up(0,1,0),
         nearplane(1), farplane(1000),
         umin(-1), umax(1), vmin(-1), vmax(1),
         fovy(45.0), aspect(aspratio),
         projtype(Perspective), dist(50.0),
         mousex(0), mousey(0), pickw(0), pickh(0), viewport(NULL), pickmode(false)
       {}

     Camera(const Camera& cam)
       : center(cam.center), eye(cam.eye), up(cam.up),
         nearplane(cam.nearplane), farplane(cam.farplane),
         umin(cam.umin), umax(cam.umax), vmin(cam.vmin), vmax(cam.vmax),
         fovy(cam.fovy), aspect(cam.aspect),
         projtype(cam.projtype), dist(cam.dist),
         mousex(0), mousey(0), pickw(0), pickh(0), viewport(NULL), pickmode(false)
       {}

     ~Camera()
       {}

     Camera& operator = (const Camera& cam)
       {
         center = cam.center; eye = cam.eye; up = cam.up;
         nearplane = cam.nearplane; farplane = cam.farplane;
         umin = cam.umin; umax = cam.umax; vmin = cam.vmin; vmax = cam.vmax;
         fovy = cam.fovy; aspect = cam.aspect;
         projtype = cam.projtype; dist = cam.dist;
         return (*this);
       }

        //--- Member functions ---//
     
     void toggleProjectionType(void)
       {
            // Toggle projection type between orthographic and perspective
         if ( projtype == Perspective )
            {
              projtype = Orthographic;
              
                 // Use fovy and aspect to compute corresponding umin,vmin,etc.
              vmax = dist * tan(deg2rad(fovy/2.0)); vmin = -vmax;
              umax = aspect * vmax; umin = -umax;
            }
         else
            {
              projtype = Perspective;

                 // Use umin,vmin,etc. to compute corresponding fovy and aspect
              fovy = 2.0*rad2deg(atan2(vmax,dist));
              aspect = umax/vmax;
            }
       }

     void makeOrthographic(void)
       {
         if ( projtype == Perspective ) toggleProjectionType();
       }

     void makePerspective(void)
       {
         if ( projtype == Orthographic ) toggleProjectionType();
       }

     double distance(void) const
       {
         return dist;
       }
     
     void setEye(const Vector3d& v)
       {
         eye = v; dist = norm(eye-center);
       }

     void setEye(double x, double y, double z)
       {
         eye.set(x,y,z); dist = norm(eye-center);
       }
     
     void setCenter(const Vector3d& v)
       {
         center = v; dist = norm(eye-center);
       }

     void setCenter(double x, double y, double z)
       {
         center.set(x,y,z); dist = norm(eye-center);
       }

     void setUpVector(const Vector3d& v)
       {
         up = v;
       }

     void setUpVector(double x, double y, double z)
       {
         up.set(x,y,z);
       }

     void setNearFar(double n, double f)
       {
         nearplane = Abs(n); farplane = Abs(f);
       }

        //--- Following are for orthographic projection ---//
     
     void setOrthographicViewVolume(double uwidth, double vheight, bool adjust=true)
       {
         umin = -Abs(uwidth)/2.0; umax = -umin; vmin = -Abs(vheight)/2.0; vmax = -vmin;
         if ( adjust ) aspect = umax/vmax;
       }

     void setUWidth(double uwidth, bool adjust=true)
       {
         umin = -Abs(uwidth)/2.0; umax = -umin;
         if ( adjust )
            {
              vmax = umax/aspect; vmin = -vmax;
            }
       }

     void setVHeight(double vheight, bool adjust=true)
       {
         vmin = -Abs(vheight)/2.0; vmax = -vmin;
         if ( adjust )
            {
              umax = vmax * aspect; umin = -umax;
            }
       }

        //--- Following are for perspective projection ---//
     
     void setPerspectiveViewVolume(double angle, double asp)
       {
         fovy = Abs(angle); aspect = Abs(aspect);
       }

     void setAspect(double asp)
       {
         aspect = Abs(asp);
       }

     void setFieldOfViewY(double angle)
       {
         fovy = Abs(angle);
       }

        // Adjust mouse coordinates for proper pan. Assumes input x,y are between -1,1
        // z is not used for computing anything, but depending on the view-vector
        // and up-vector, z may be set to get proper panning
     void adjust(double& x, double& y, double& z)
       {
         double tx, ty;
         if ( projtype == Orthographic )
            {
                 // Orthographic projection
              tx = umin*(1.0-x)*0.5 + umax*(1.0+x)*0.5;
              ty = vmin*(1.0-y)*0.5 + vmax*(1.0+y)*0.5;
            }
         else if ( projtype == Perspective )
            {
                 // Perspective projection
              tx = x*dist*tan(deg2rad(fovy*aspect*0.5));
              ty = y*dist*tan(deg2rad(fovy*0.5));
            }
            // Depending on the view-vector and up-vector, set appropriate coordinates
            // view is eye-center, z is along view, x is up % view, y is z % x
         Vector3d Z = normalized(eye-center);
         Vector3d X = normalized(up) % Z;
         Vector3d Y = Z % X;
         Vector3d p = tx*X + ty*Y;
         p.get(x,y,z);
       }

        //--- Enter selection mode and set the pick region for doing selection ---//
     void enterSelectionMode(double x, double y, double w, double h, GLint * vp)
       {
         mousex = x; mousey = y; pickw = w; pickh = h; viewport = vp;
         pickmode = true;
       }

     void leaveSelectionMode(void)
       {
         pickmode = false; viewport = NULL;
       }

        //--- Apply the camera transformation ---//

     void applyTransform(void) const
       {
         glMatrixMode(GL_PROJECTION);
         glLoadIdentity();
         if ( pickmode )
            gluPickMatrix(mousex, mousey, pickw, pickh, viewport);
         if ( projtype == Perspective )      gluPerspective(fovy,aspect,nearplane,farplane);
         else if ( projtype == Orthographic) glOrtho(umin,umax,vmin,vmax,nearplane,farplane);
         
         glMatrixMode(GL_MODELVIEW);
         glLoadIdentity();
         gluLookAt(eye[0],eye[1],eye[2],center[0],center[1],center[2],up[0],up[1],up[2]);
       }
};

#endif /* #ifndef _CAMERA_HH_ */


/*
  $Log: Camera.hh,v $
  Revision 1.1  2006/09/04 19:22:52  stuart
  Added includes and libs.

  Revision 2.7  2002/10/25 15:42:05  vinod
  Fixed include location for GL headers

  Revision 2.6  2000/05/16 21:26:10  vinod
  Minor corrections in variable name and function name

  Revision 2.5  2000/05/16 21:22:23  vinod
  Removed incorrect include file glu.h

  Revision 2.4  2000/05/16 21:05:55  vinod
  Change leavePickMode to leaveSelectMode for consistent naming

  Revision 2.3  2000/05/16 20:36:33  vinod
  Added GL selection support

  Revision 2.2  2000/04/24 18:46:14  vinod
  Modified adjust() to handle pans for arbitrary views correctly

  Revision 2.1  2000/04/07 21:51:47  vinod
  Camera class for windows

*/
