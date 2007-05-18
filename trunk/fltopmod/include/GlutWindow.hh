/* $Id: GlutWindow.hh,v 1.1 2006/09/04 19:22:52 stuart Exp $ */

#ifndef _GLUT_WINDOW_HH_

#define _GLUT_WINDOW_HH_

// Class to encapsulate a simple GLUT window

#include <glut.h>
#include <stdlib.h>

typedef unsigned int uint;

class GlutWindow;
typedef GlutWindow * GlutWindowPtr;

// Forward declare the GlutMaster class
class GlutMaster;

class GlutWindow
{
  protected :

     int iWidth,iHeight;                               // Window width and height
     int iGlutId;                                      // Window ID from GLUT

     friend class GlutMaster;
     
  protected :

        // Make these private to prevent access and catch errors at compile time
     GlutWindow(const GlutWindow&)
       {}

     void operator = (const GlutWindow&)
       {}
     
  public :

        // Various constructors for creating windows
     GlutWindow(char * title, uint mode = GLUT_RGBA);
     GlutWindow(char * title, int w, int h, uint mode = GLUT_RGBA);
     GlutWindow(char * title, int w, int h, int x, int y, uint mode = GLUT_RGBA);
     
     virtual ~GlutWindow()                             // Destructor
       {}
     
        //--- Functions to modify/query the size, position, etc. of a window ---//

     void getSize(int& w, int& h)
       {
         w = iWidth; h = iHeight;
       }

     int id(void)
       {
         return iGlutId;
       }

     void resize(int w, int h)                         // Called after creating the window
       {
         glutSetWindow(iGlutId);
         glutReshapeWindow(w,h);
         iWidth = glutGet(GLUT_WINDOW_WIDTH);
         iHeight = glutGet(GLUT_WINDOW_HEIGHT);
       }

     void reposition(int x, int y)                     // Called after creating the window
       {
         glutSetWindow(iGlutId);
         glutPositionWindow(x,y);
       }

        // Virtual functions which are the default callback functions
        // Derived classes should override these to provide meaningful implementations
     virtual void displayFunction(void)
       {}
     
     virtual void entryFunction(int)
       {}
     
     virtual void idleFunction(void)
       {}
     
     virtual void keyboardFunction(unsigned char, int, int)
       {}
     
     virtual void keyboardUpFunction(unsigned char, int, int)
       {}
     
     virtual void motionFunction(int, int)
       {}
     
     virtual void mouseFunction(int, int, int, int)
       {}
     
     virtual void passiveMotionFunction(int, int)
       {}
     
     virtual void reshapeFunction(int w, int h)
       {
         iWidth = w; iHeight = h;
         glViewport(0,0,w,h);
       }
     
     virtual void specialFunction(int, int, int)
       {}
     
     virtual void specialUpFunction(int, int, int)
       {}

     virtual void visibilityFunction(int state)
       {
         if ( state == GLUT_VISIBLE ) glutPostRedisplay();
       }
};

#endif /* #ifndef _GLUT_WINDOW_HH_ */


/*
  $Log: GlutWindow.hh,v $
  Revision 1.1  2006/09/04 19:22:52  stuart
  Added includes and libs.

  Revision 1.5  2000/02/08 10:31:29  vinod
  Removed derivation from BaseObject

  Revision 1.4  2000/02/08 10:29:20  vinod
  Changed private to protected for GlutWindow constructors to allow derivarion

  Revision 1.3  2000/02/08 07:45:33  vinod
  Uses static fns from GlutMaster now

  Revision 1.2  2000/02/08 07:28:41  vinod
  *** empty log message ***

  Revision 1.1.1.1  2000/02/03 03:59:12  vinod
  GlutMaster library

*/
