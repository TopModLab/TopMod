/* $Id: PerspectiveWindow.hh,v 1.2 2000/04/28 22:53:26 vinod Exp $ */

#ifndef _PERSPECTIVE_WINDOW_HH_

#define _PERSPECTIVE_WINDOW_HH_

// Class for a perspective view window. Derived from ViewWindow
// Sets the camera for a perspective view (which is the default)
// Has access functions to change camera parameters.
// Allows setting the initial view position (the arcball will work properly
// only if the view stars off on the z axis. So to start from an arbitrary
// view, the initial transformation must be put into the transformation matrix )

#include <ViewWindow.hh>

class PerspectiveWindow : public ViewWindow
{
  public :

     PerspectiveWindow(int x, int y, int w, int h, const char * title = NULL)
       : ViewWindow(x,y,w,h,title)
       {
         camera.makePerspective();
       }

        // Access functions to change camera settings
     void setFieldOfViewY(double angle)
       {
         camera.setFieldOfViewY(angle);
       }

     void setAspectRatio(double aspect)
       {
         camera.setAspect(aspect);
       }

     void setPerspectiveViewVolume(double angle, double aspect)
       {
         camera.setPerspectiveViewVolume(angle,aspect);
       }

     void setNearFar(double near, double far)
       {
         camera.setNearFar(near, far);
       }

        // Set the view (eye, center, upvector)
     void setView(const Vector3d& eye, const Vector3d& center, const Vector3d& up)
       {
         Vector3d neweye = eye - center;
         double eyedist = norm(neweye);
         Matrix4x4 lmat = Transformation::lookat(neweye,Vector3d(0,0,0),up);
         transform = lmat;
         transform.translate(-center);
         camera.setEye(Vector3d(0,0,eyedist));
       }

     void setView(double eyex, double eyey, double eyez,
                  double cenx, double ceny, double cenz,
                  double upx, double upy, double upz)
       {
         setView(Vector3d(eyex, eyey, eyez),
                 Vector3d(cenx, ceny, cenz),
                 Vector3d(upx, upy, upz));
       }
};
         
#endif /* #ifndef _PERSPECTIVE_WINDOW_HH_ */


/*
  $Log: PerspectiveWindow.hh,v $
  Revision 1.2  2000/04/28 22:53:26  vinod
  Implemented setView subroutine

  Revision 1.1  2000/04/26 22:53:58  vinod
  Moved location

  Revision 2.1  2000/04/23 22:16:47  vinod
  Class for perspective views

*/
