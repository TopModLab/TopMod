/* $Id: ViewWindow.hh,v 1.1 2006/09/04 19:22:53 stuart Exp $ */

#ifndef _VIEW_WINDOW_HH_

#define _VIEW_WINDOW_HH_

// Class for a FLTK Window, which handles rotations using the Arcball controller,
// panning, zooming and dollying.
// Renamed from ArcballWindow since it does more than just rotations now
// This is intended to be a replacement and is a separate class, although the code
// is the same.
// Does not provide a handle() method, so cannot be instantiated
// - this is an Abstract Base Class (ABC)

// Define macros for FLTK mouse buttons.
#define FL_LEFT_MOUSE 1
#define FL_MIDDLE_MOUSE 2
#define FL_RIGHT_MOUSE 3

#include <FL/Fl.H>
#include <FL/Fl_Gl_Window.H>
#include <GL/gl.h>
#include <GL/glu.h>
#include <Arcball.hh>
#include <TransControl.hh>
#include <ZoomControl.hh>
#include <DollyControl.hh>
#include <stdio.h>
#include <Transform.hh>
#include <Camera.hh>

// Enumerations for the different views. The names imply looking from the corresponding
// side. ie Front -> looking *from* front, Right -> looking *from* right
enum View { Persp=0, Front=1, Right=2, Top=3, Back=4, Left=5, Bottom=6 };

enum TransformType { None=0, Pan=1, Rotate=2, Zoom=3, Dolly=4 };

class ViewWindow : public Fl_Gl_Window
{
  protected :

     Arcball arcball;                                  // The arcball controller
     TransControl trcontrol;                           // Translation controller
     ZoomControl zoomcontrol;                          // Zoom controller
     DollyControl dollycontrol;                        // Dolly controller (10.0 scale)
     
     Transformation transform;                         // Combined transformation
     TransformType currenttr;                          // Current transformation

  public :

        // Made public to allow changing camera settings easily (view-volume, near/far)
     Camera camera;                                    // Camera attached to this window

  protected :
     
     View   view;                                      // Which view?

  public :

        // Set eyepoint, center, etc. for perspective views
        // Setting these requires modifying the transformation matrix
        // to get the proper rotation using arcball
     void setPerspView(const Vector3d& eye, const Vector3d& center, const Vector3d& up)
       {
         if ( view == Persp )
            {
              Vector3d neweye = eye - center;
              double eyedist = norm(neweye);
              Matrix4x4 lmat = Transformation::lookat(neweye,Vector3d(0,0,0),up);
              transform = lmat;
              transform.translate(-center);
              camera.setEye(Vector3d(0,0,eyedist));
            }
       }

     void setPerspView(double eyex, double eyey, double eyez,
                       double cenx, double ceny, double cenz,
                       double upx, double upy, double upz)
       {
         setPerspView(Vector3d(eyex, eyey, eyez),
                      Vector3d(cenx, ceny, cenz),
                      Vector3d(upx, upy, upz));
       }

        // Change camera settings to get specified view
     void switchTo(View v)
       {
         view = v;
         camera.setCenter(0,0,0);
         camera.setNearFar(1,1000);
         if ( view == Persp )
            {
              camera.makePerspective();
              camera.setPerspectiveViewVolume(60.0,1.0);
            }
         else
            {
              camera.makeOrthographic();
              camera.setOrthographicViewVolume(2,2,true);
              transform.reset();
            }

         switch ( view )
            {
              case Persp :
                           setPerspView(50,50,50,      // eye point
                                        0,0,0,         // center
                                        0,1,0);        // up vector
                           break;
              case Front :
                           camera.setEye(0,0,100);
                           camera.setUpVector(0,1,0);
                           break;
              case Right :
                           camera.setEye(100,0,0);
                           camera.setUpVector(0,1,0);
                           break;
              case Top :
                           camera.setEye(0,100,0);
                           camera.setUpVector(0,0,-1);
                           break;
              case Back :
                           camera.setEye(0,0,-100);
                           camera.setUpVector(0,1,0);
                           break;
              case Left :
                           camera.setEye(-100,0,0);
                           camera.setUpVector(0,1,0);
                           break;
              case Bottom :
                           camera.setEye(0,-100,0);
                           camera.setUpVector(0,0,1);
                           break;
            }
       }

     ViewWindow(int x, int y, int w, int h, const char * l = NULL)
       : Fl_Gl_Window(x,y,w,h,l),
         arcball(Vector3d(0,0,0),1.0), trcontrol(), zoomcontrol(), dollycontrol(10.0),
         transform(), currenttr(None), camera(), view(Persp)
       {
         switchTo(view);
       }

        // Derived classes can override this
     virtual void reshape(void)
       {
            // This function should be called by the draw() function when the view
            // needs to be setup

            // Now uses the Camera class
         glViewport(0,0,w(),h());
         camera.applyTransform();
       }

        // Handle rotation by mouse. Will do the rotation irrespective of which mouse
        // button was used and irrespective of other modifiers. Does not call
        // redraw or return any value. Will always update the controller. So this
        // function should be called only when rotation is required
     virtual void handle_rotate(int event)
       {
            // Don't do anything if we aren't in a neutral state or aren't rotating
         if ( currenttr != None && currenttr != Rotate ) return;

         currenttr = Rotate;
         if ( view == Persp )                          // No rotations for ortho views
            {
              double x,y;
                 // Transform coords to lie between -1 to 1
              x = ( double(Fl::event_x() << 1) / w() ) - 1.0;
              y = ( -double(Fl::event_y() << 1) / h() ) + 1.0;
              
              arcball.mouse(x,y); arcball.update();

              if ( event == FL_PUSH ) arcball.beginDrag();
              else if ( event == FL_RELEASE )
                 {
                   arcball.endDrag();
                   transform.rotate(arcball.quat_value());  // Update the combined transformation
                   arcball.reset();
                 }
            }
         if ( event == FL_RELEASE ) currenttr = None;
       }

        // Handle panning by mouse. Will do the panning irrespective of which mouse
        // button was used and irrespective of other modifiers. Does not call
        // redraw or return any value. Will always update the controller. So this
        // function should be called only when panning is required
     virtual void handle_pan(int event)
       {
            // Don't do anything if we aren't in a neutral state or aren't panning
         if ( currenttr != None && currenttr != Pan ) return;

         currenttr = Pan;

         double x,y,z;

            // Transform coords to lie between -1 to 1
         x = ( double(Fl::event_x() << 1) / w() ) - 1.0;
         y = ( -double(Fl::event_y() << 1) / h() ) + 1.0;

            // Adjust the x and y values so that moving mouse by 1 pixel
            // on screen moves point under mouse by 1 pixel
         camera.adjust(x,y,z);
         trcontrol.mouse(x,y,z); trcontrol.update();

         if ( event == FL_PUSH ) trcontrol.beginDrag();
         else if ( event == FL_RELEASE )
            {
              trcontrol.endDrag();
              transform.translate(trcontrol.trans_value());// Update the combined transformation
              trcontrol.reset(); currenttr = None;
            }
       }

        // Handle zooming by mouse. Will do the zooming irrespective of which mouse
        // button was used and irrespective of other modifiers. Does not call
        // redraw or return any value. Will always update the controller. So this
        // function should be called only when zooming is required

        // Only the x movement is used for zooming. Zooming is done using the Zoom
        // controller which computes the apprpriate transformation matrix
     virtual void handle_zoom(int event)
       {
            // Don't do anything if we aren't in a neutral state or aren't zooming
         if ( currenttr != None && currenttr != Zoom ) return;

         currenttr = Zoom;

         double z;

            // Transform coords to lie between -1 to 1
         z = ( double(Fl::event_x() << 1) / w() ) - 1.0;

            // Currently both orthographic and perspective zoom is handled by same
            // controller which simply does a scaling
         zoomcontrol.mouse(z); zoomcontrol.update();

         if ( event == FL_PUSH ) zoomcontrol.beginDrag();
         else if ( event == FL_RELEASE )
            {
              zoomcontrol.endDrag();
              transform.scale(zoomcontrol.zoom_value());// Update the combined transformation
              zoomcontrol.reset(); currenttr = None;
            }
       }

        // Handle dollying by mouse. Will do the dollying irrespective of which mouse
        // button was used and irrespective of other modifiers. Does not call
        // redraw or return any value. Will always update the controller. So this
        // function should be called only when dollying is required

        // Only the x movement is used for dollying. Dollying is done using the Dolly
        // controller which computes the appropriate transformation matrix
        // The transformation from dollying is not multiplied onto the transformation
        // matrix since this should be done outside of all other transformations
     virtual void handle_dolly(int event)
       {
            // For orthographic views, call handle_zoom, since handle_dolly has no effect
         if ( view != Persp ) handle_zoom(event);
         else
            {
                 // Don't do anything if we aren't in a neutral state or aren't dollying
              if ( currenttr != None && currenttr != Dolly ) return;

              currenttr = Dolly;

              double z;

                 // Transform coords to lie between -1 to 1
              z = ( double(Fl::event_x() << 1) / w() ) - 1.0;

              dollycontrol.mouse(z); dollycontrol.update();

              if ( event == FL_PUSH ) dollycontrol.beginDrag();
              else if ( event == FL_RELEASE )
                 {
                   dollycontrol.endDrag();
                      // Shouldn't reset since the value is directly used for transformations
                      // separate from other transformations
                   currenttr = None;
                 }
            }
       }

     virtual bool send_to_current(int event)
       {
            // Send the given event to the subroutine handling the current transformation
         bool handled = true;
         switch ( currenttr )
            {
              case Pan :
                           handle_pan(event); break;
              case Zoom :
                           handle_zoom(event); break;
              case Rotate :
                           handle_rotate(event); break;
              case Dolly :
                           handle_dolly(event); break;
              default :
                           handled = false;
            }
         return handled;
       }

     void arcball_draw(void)                           // Draw the arcball
       {
         arcball.draw();
       }

     void do_transform(void) const                     // Apply the transformation
       {
         double mat[16];

            // Do the dollying separately before everything else
         glTranslated(0,0,dollycontrol.dolly_value());
         switch ( currenttr )
            {
              case Pan :
                           trcontrol.value().fillArrayColumnMajor(mat);
                           glMultMatrixd(mat);
                           break;
              case Zoom :
                           zoomcontrol.value().fillArrayColumnMajor(mat);
                           glMultMatrixd(mat);
                           break;
              case Rotate :
                           arcball.value().fillArrayColumnMajor(mat);
                           glMultMatrixd(mat);
                           break;
            }
         transform.apply();
       }

        //--- Access arcball parameters ---//
     
     void arcball_center(const Vector3d& cen) 
       {
         arcball.center(cen);
       }

     void arcball_radius(double rad)
       {
         arcball.radius(rad);
       }

     Vector3d arcball_center(void) const
       {
         return arcball.center();
       }

     double arcball_radius(void) const
       {
         return arcball.radius();
       }
};

#endif /* #ifndef _VIEW_WINDOW_HH_ */


/*
  $Log: ViewWindow.hh,v $
  Revision 1.1  2006/09/04 19:22:53  stuart
  Added includes and libs.

  Revision 1.3  2000/05/02 19:15:57  vinod
  Changed matrix filling order, since controllers update() were changed

  Revision 1.2  2000/04/30 21:43:04  vinod
  Integrated orthographic and perspective windows into ViewWindow itself

  Revision 1.1  2000/04/26 22:53:59  vinod
  Moved location

  Revision 2.3  2000/04/24 18:46:59  vinod
  Added z coord for handle_pan() to pan properly for arbitrary views

  Revision 2.2  2000/04/23 21:19:32  vinod
  Camera.hh now included from search directories

  Revision 2.1  2000/04/23 21:17:21  vinod
  A class for a viewing window which supports rotation, panning, zooming and
  dollying. Was originally ArcballWindow (look in ArcballWindow.hh for log
  history). Removed some backward compatibility functions and the handle()
  method to make this an ABC. Intended to be a base class for different kinds
  of viewing windows (eg. TopView, FrontView, PerspectiveView, etc.)

*/
