/* $Id: OrthographicWindow.hh,v 1.1 2000/04/26 22:53:58 vinod Exp $ */

#ifndef _ORTHOGRAPHIC_WINDOW_HH_

#define _ORTHOGRAPHIC_WINDOW_HH_

// Class for an orthographic view window. Derived from ViewWindow
// Overrides the handle_rotate() method to not do anything
// Overrides the handle_dolly() method to simple call handle_zoom(), since dollying
// in an orthographic projection does not achieve the desired effect
// Sets the camera for an orthographic-view
// 6 different orthographic views are possible 1 looking at each axis and 1 looking
// down each axis. Default is front view
// The switchTo() method allows switching between the 6 views

#include <ViewWindow.hh>

// Enumerations for the 6 different views. The names imply looking from the corresponding
// side. ie Front -> looking *from* front, Right -> looking *from* right

enum OrthoView { Front=0, Right=1, Top=2, Back=3, Left=4, Bottom=5 };

class OrthographicWindow : public ViewWindow
{
  protected :

        // This is really not needed, but is there for possible future use
     OrthoView view;                                   // Which orthographic view?

  public :

        // Change the eye position and up-vector according to desired view
     void switchTo(OrthoView v)
       {
         view = v; camera.setCenter(0,0,0);
         switch ( view )
            {
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

     OrthographicWindow(int x, int y, int w, int h, const char * title = NULL)
       : ViewWindow(x,y,w,h,title), view(Front)
       {
         camera.makeOrthographic();
         camera.setOrthographicViewVolume(2,2,true);
         camera.setNearFar(1,1000);
         switchTo(view);
       }

     virtual void handle_rotate(int event)
       {
            // Set current transformation to rotate for proper
            // handling of multiple mouse buttons
         if ( currenttr == None || currenttr == Rotate )
            {
              currenttr = Rotate;
              if ( event == FL_RELEASE ) currenttr = None;
            }

            // Do nothing - rotation disabled for orthographic projections
       }

     virtual void handle_dolly(int event)
       {
            // Simple call handle_zoom, since dollying has not effect for orthographic projections
         handle_zoom(event);
       }

        // Access functions to change camera setings
     void setViewVolume(double uwidth, double vheight)
       {
         camera.setOrthographicViewVolume(uwidth, vheight, true);
       }

     void setNearFar(double near, double far)
       {
         camera.setNearFar(near,far);
       }
};

#endif /* #ifndef _ORTHOGRAPHIC_WINDOW_HH_ */


/*
  $Log: OrthographicWindow.hh,v $
  Revision 1.1  2000/04/26 22:53:58  vinod
  Moved location

  Revision 2.2  2000/04/24 18:48:20  vinod
  Modified handle_rotate() so that multiple button events can be handled
  correctly. Bug fix in upvector for Top view and Bottom view

  Revision 2.1  2000/04/23 22:16:38  vinod
  Class for orthographic views

*/
