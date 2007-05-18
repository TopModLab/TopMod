/* $Id: GlutMaster.hh,v 1.1 2006/09/04 19:22:52 stuart Exp $ */

#ifndef _GLUT_MASTER_HH_

#define _GLUT_MASTER_HH_

// Master class for GLUT C++ encapsulation
// Stores all init values and provides a function to actually create a GLUT window
// Assumes existence of a class GlutWindow
// Has static functions which are registered as callbacks for all windows created

#include <glut.h>
#include <stdlib.h>
#include "GlutWindow.hh"

#define MAX_WINDOWS 10

class GlutMaster;
typedef GlutMaster * GlutMasterPtr;

class GlutMaster
{
  private :

        // Static functions which are registered as callbacks
     static void _displayFunction(void)
       {
         windows[glutGetWindow()]->displayFunction();
       }
     
     static void _entryFunction(int state)
       {
         windows[glutGetWindow()]->entryFunction(state);
       }
     
     static void _idleFunction(void)
       {
         windows[glutGetWindow()]->idleFunction();
       }
     
     static void _keyboardFunction(unsigned char c, int x, int y)
       {
         windows[glutGetWindow()]->keyboardFunction(c,x,y);
       }
     
     static void _keyboardUpFunction(unsigned char c, int x, int y)
       {
         windows[glutGetWindow()]->keyboardUpFunction(c,x,y);
       }
     
     static void _motionFunction(int x, int y)
       {
         windows[glutGetWindow()]->motionFunction(x,y);
       }
     
     static void _mouseFunction(int button, int state, int x, int y)
       {
         windows[glutGetWindow()]->mouseFunction(button,state,x,y);
       }
     
     static void _passiveMotionFunction(int x, int y)
       {
         windows[glutGetWindow()]->passiveMotionFunction(x,y);
       }
     
     static void _reshapeFunction(int width, int height)
       {
         windows[glutGetWindow()]->reshapeFunction(width,height);
       }
     
     static void _specialFunction(int key, int x, int y)
       {
         windows[glutGetWindow()]->specialFunction(key,x,y);
       }
     
     static void _specialUpFunction(int key, int x, int y)
       {
         windows[glutGetWindow()]->specialUpFunction(key,x,y);
       }
     
     static void _visibilityFunction(int state)
       {
         windows[glutGetWindow()]->visibilityFunction(state);
       }

     static void registerCallbacks(void)
       {
         glutDisplayFunc(_displayFunction);
         glutEntryFunc(_entryFunction);
         glutIdleFunc(_idleFunction);
         glutKeyboardFunc(_keyboardFunction);
         glutKeyboardUpFunc(_keyboardUpFunction);
         glutMotionFunc(_motionFunction);
         glutMouseFunc(_mouseFunction);
         glutPassiveMotionFunc(_passiveMotionFunction);
         glutReshapeFunc(_reshapeFunction);
         glutSpecialFunc(_specialFunction);
         glutSpecialUpFunc(_specialUpFunction);
         glutVisibilityFunc(_visibilityFunction);
       }

  protected :

     static GlutWindowPtr * windows;                   // Array of window pointers
     static bool initialized;                          // Flag to indicate initialization status
     static uint max_windows;                          // Max. no. of windows that can be created

  public :
     
        // Cleanup. Free memory, etc. This is registered as an exit callback with atexit
     static void cleanup(void)
       {
         delete [] windows; windows = NULL;
       }
     
        // Initialize the array containing window pointers and call glutInit
     static void initialize(int * argcp, char ** argv, uint max = MAX_WINDOWS)
       {
         if ( initialized ) return;
         initialized = true;
         max_windows = max;
         windows = new GlutWindowPtr[max_windows];
         for (int i=0; i < max_windows; ++i)
            windows[i] = NULL;

         glutInit(argcp,argv);
         atexit(cleanup);
       }

        // Initialize with dummy arguments to glutInit
     static void initialize(uint max = MAX_WINDOWS)
       {
         char * dummy_argv[1];
         int dummy_argc = 1;
         dummy_argv[0] = "run";
         GlutMaster :: initialize(&dummy_argc,dummy_argv,max);
       }

  private :
     
        // Default constructor
     GlutMaster()
       {}

        // Copy constructor
     GlutMaster(const GlutMaster&)
       {}

  public :
     
        // Destructor
     ~GlutMaster()
       {}

  private :
     
        // Assignment operator
     GlutMaster& operator = (const GlutMaster&)
       {
         return (*this);
       }

  private :
     
        // Create a new GLUT window and associate the given GlutWindow ptr with
        // that window. This function is not supposed to be called by the user directly
        // This function will be called by the constructor of GlutWindow, which is declared
        // to be a friend

     friend class GlutWindow;

        // Add the given window to the list of windows
     static bool addWindow(GlutWindowPtr gwinptr)
       {
         if ( !initialized ) return false;
         int id = gwinptr->iGlutId;
         if ( id >= max_windows ) return false;
         windows[id] = gwinptr;
         return true;
       }
     
     static bool createWindow(char * title, GlutWindowPtr gwinptr)
       {
         int winid = glutCreateWindow(title);
         gwinptr->iGlutId = winid;

         bool success = GlutMaster :: addWindow(gwinptr);

            // Try to add the window to the array. If not possible destroy it
         if ( success ) GlutMaster :: registerCallbacks();
         else glutDestroyWindow(winid);

         return success;
       }
};

#endif /* #ifndef _GLUT_MASTER_HH_ */


/*
  $Log: GlutMaster.hh,v $
  Revision 1.1  2006/09/04 19:22:52  stuart
  Added includes and libs.

  Revision 1.4  2000/02/08 07:45:08  vinod
  Changed createWindow to be static

  Revision 1.3  2000/02/08 07:36:57  vinod
  Removed references to BaseObject

  Revision 1.2  2000/02/08 07:33:57  vinod
  Made constructors and assignment operator private

  Revision 1.1.1.1  2000/02/03 03:59:11  vinod
  GlutMaster library

*/
