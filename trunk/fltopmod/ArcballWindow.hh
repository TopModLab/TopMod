/* $Id: ArcballWindow.hh,v 1.2 2000/05/02 19:15:57 vinod Exp $ */

#ifndef _FL_GL_ARCBALL_WINDOW_HH_

#define _FL_GL_ARCBALL_WINDOW_HH_

// Class for a FLTK GL Arcball window, which handles arcball rotations by default

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
#include "Camera.hh"

enum TransformType { None=0, Pan=1, Rotate=2, Zoom=3, Dolly=4 };

class ArcballWindow : public Fl_Gl_Window
{
  protected :

     Arcball arcball;                                  // The arcball controller
     TransControl trcontrol;                           // Translation controller
     ZoomControl zoomcontrol;                          // Zoom controller
     DollyControl dollycontrol;                        // Dolly controller (10.0 scale)
     
     Transformation transform;                         // Combined transformation
     TransformType currenttr;                          // Current transformation

     Camera camera;                                    // Camera attached to this window
     
  public :

     ArcballWindow(int x, int y, int w, int h, const char * l = NULL)
       : Fl_Gl_Window(x,y,w,h,l),
         arcball(Vector3d(0,0,0),1.0), trcontrol(), zoomcontrol(), dollycontrol(10.0),
         transform(), currenttr(None), camera()
       {}

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

         double x,y;

         currenttr = Rotate;

            // Transform coords to lie between -1 to 1
         x = ( double(Fl::event_x() << 1) / w() ) - 1.0;
         y = ( -double(Fl::event_y() << 1) / h() ) + 1.0;
              
         arcball.mouse(x,y); arcball.update();

         if ( event == FL_PUSH ) arcball.beginDrag();
         else if ( event == FL_RELEASE )
            {
              arcball.endDrag();
              transform.rotate(arcball.quat_value());  // Update the combined transformation
              arcball.reset(); currenttr = None;
            }
       }

        // Handle panning by mouse. Will do the panning irrespective of which mouse
        // button was used and irrespective of other modifiers. Does not call
        // redraw or return any value. Will always update the controller. So this
        // function should be called only when panning is required
     virtual void handle_pan(int event)
       {
            // Don't do anything if we aren't in a neutral state or aren't panning
         if ( currenttr != None && currenttr != Pan ) return;

         double x,y,z;

         currenttr = Pan;

            // Transform coords to lie between -1 to 1
         x = ( double(Fl::event_x() << 1) / w() ) - 1.0;
         y = ( -double(Fl::event_y() << 1) / h() ) + 1.0;
              
            // Adjust the x and y values so that moving mouse by 1 pixel
            // on screen moves point under mouse by 1 pixel
         camera.adjust(x,y,z);
         trcontrol.mouse(x,y); trcontrol.update();

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

         double z;

         currenttr = Zoom;

            // Transform coords to lie between -1 to 1
         z = ( double(Fl::event_x() << 1) / w() ) - 1.0;

            // Currently both orthographic and perspective zoom is handled by same
            // controller which simply does a scaling
            /*
         if ( projtype == Orthographic )
            {
            }
         else if ( projtype == Perspective )
            {
            }
            */
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
            // Don't do anything if we aren't in a neutral state or aren't dollying
         if ( currenttr != None && currenttr != Dolly ) return;

         double z;

         currenttr = Dolly;

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

        // Implement handle() function for backward compatibility
     virtual int handle(int event)
       {
         if ( event == FL_PUSH || event == FL_RELEASE || event == FL_DRAG )
            {
              if ( Fl::event_button() == FL_LEFT_MOUSE ) handle_rotate(event);
              else if ( Fl::event_button() == FL_MIDDLE_MOUSE ) handle_pan(event);

              if ( event != FL_PUSH ) redraw();
              return 1;
            }
         return Fl_Gl_Window::handle(event);
       }
     
     void arcball_draw(void)                           // Draw the arcball
       {
         arcball.draw();
       }

     void arcball_transform(void) const                // For backward compatibility
       {
         do_transform();
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

#endif /* #ifndef _FL_GL_ARCBALL_WINDOW_HH_ */


/*
  $Log: ArcballWindow.hh,v $
  Revision 1.2  2000/05/02 19:15:57  vinod
  Changed matrix filling order, since controllers update() were changed

  Revision 1.1  2000/04/26 22:53:57  vinod
  Moved location

  Revision 2.5  2000/04/24 18:45:30  vinod
  Added z coord for camera adjustment in handle_pan()

  Revision 2.4  2000/04/20 06:54:39  vinod
  Added send_to_current subroutine

  Revision 2.3  2000/04/07 21:52:33  vinod
  Modified to use the new Camera class

  Revision 2.2  2000/03/31 09:15:32  vinod
  Added Dolly controller and handle_dolly subroutine

  Revision 2.1  2000/03/20 19:57:36  vinod
  Modified handle_zoom to use the new Zoom controller

  Revision 2.0  2000/03/12 00:49:30  vinod
  Major revision number change

  Revision 1.7  2000/03/07 23:31:15  vinod
  fixed recursion bug in handle() function

  Revision 1.6  2000/03/07 22:51:14  vinod
  Re-Added handle() function for backward compatibility

  Revision 1.5  2000/03/07 22:47:52  vinod
  Replace single handle() function with handle_pan(), handle_rotate() and
  handle_zoom() (added in this version). Derived classes should call the
  appropriate handle_* function as per their needs.

  Revision 1.4  2000/03/07 20:38:16  vinod
  Added missing include glu.h

  Revision 1.3  2000/03/07 20:34:29  vinod
  Fixed pan control so object moves along with mouse. The control will now
  work for orthographic projections also. Added parameters for
  view/projection in the class (which have to be set by derived classes for
  the pan control to work correctly). Added reshape virtual function

  Revision 1.2  2000/03/03 00:49:55  vinod
  Added Pan control

  Revision 1.1  2000/03/01 01:27:19  vinod
  Moved from FLTK directory

  Revision 1.3  2000/02/11 08:35:27  vinod
  Made handle() a virtual function

  Revision 1.2  2000/02/11 04:02:03  vinod
  Added missing include files

  Revision 1.1  2000/02/11 03:26:28  vinod
  Class for an FLTK window with built-in arcball support

*/
