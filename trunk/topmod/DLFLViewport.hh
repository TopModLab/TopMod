/* $Id: DLFLViewport.hh,v 4.1 2004/04/18 18:19:27 vinod Exp $ */

#ifndef _DLFL_VIEWPORT_HH_

#define _DLFL_VIEWPORT_HH_

/*
  Viewport class to display a DLFLObject. Provides subroutines for selection also.
*/

#include <Viewport.hh>
//#include <Grid.hh>
#include <FL/Fl.H>
#include <FL/Fl_Gl_Window.H>
#include <FL/fl_draw.H>
#ifdef __APPLE__
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#else
#include <GL/gl.h>
#include <GL/glu.h>
#endif

#include "DLFLObject.hh"
#include "DLFLRenderer.hh"
// 
// // Define macros for FLTK mouse buttons.
// #if FL_MINOR_VERSION < 1
// #define FL_LEFT_MOUSE 1
// #define FL_MIDDLE_MOUSE 2
// #define FL_RIGHT_MOUSE 3
// #endif

class DLFLViewport;
typedef DLFLViewport * DLFLViewportPtr;

class DLFLViewport : public Fl_Gl_Window
{
  protected :

        // Selection lists - these are shared by all viewports
     static DLFLVertexPtrArray sel_vptr_array; // List of selected DLFLVertex pointers
     static DLFLEdgePtrArray sel_eptr_array; // List of selected DLFLEdge pointers
     static DLFLFacePtrArray sel_fptr_array; // List of selected DLFLFace pointers
     static DLFLFaceVertexPtrArray sel_fvptr_array; // List of selected DLFLFaceVertex pointers

     Viewport viewport;                     // Viewport for the window

        // Each viewport will have its own object to display. But since only
        // the pointer is stored, different viewports can share the same object.
     DLFLObjectPtr object;                    // Pointer to DLFLObject


        // Each viewport will have its own renderer. But since only the pointer
        // is stored different viewports can share the same renderer object.
     DLFLRendererPtr renderer;

        // Each viewport will have its own grid
     //Grid grid;                                        // Display grid

        // Boolean flags - these are viewport specific
     bool showgrid;                           // Should grid be shown?
     bool showaxes;                           // Should axes be shown?


  public :

        //--- Initialize the selection lists ---//
     static void initializeSelectionLists(int num)
       {
            // Reserves memory for the arrays to avoid reallocation
            // It is not mandatory to call this function, but recommended
         sel_vptr_array.reserve(num);
         sel_eptr_array.reserve(num);
         sel_fptr_array.reserve(num);
         sel_fvptr_array.reserve(num);
       }

        //--- Add items to the selection lists - check for NULL pointers ---//
     static void addToSelection(DLFLVertexPtr vp)
       {
         if ( vp ) sel_vptr_array.push_back(vp);
       }
     
     static void addToSelection(DLFLEdgePtr ep)
       {
         if ( ep ) sel_eptr_array.push_back(ep);
       }
     
     static void addToSelection(DLFLFacePtr fp)
       {
         if ( fp ) sel_fptr_array.push_back(fp);
       }
     
     static void addToSelection(DLFLFaceVertexPtr fvp)
       {
         if ( fvp ) sel_fvptr_array.push_back(fvp);
       }

        //--- Check if given item is there in the selection list ---//
     static bool isSelected(DLFLVertexPtr vp)
       {
         bool found = false;
         if ( vp )
            {
              for (int i=0; i < sel_vptr_array.size(); ++i)
                 if ( sel_vptr_array[i] == vp )
                    {
                      found = true; break;
                    }
            }
         return found;
       }

     static bool isSelected(DLFLEdgePtr ep)
       {
         bool found = false;
         if ( ep )
            {
              for (int i=0; i < sel_eptr_array.size(); ++i)
                 if ( sel_eptr_array[i] == ep )
                    {
                      found = true; break;
                    }
            }
         return found;
       }

     static bool isSelected(DLFLFacePtr fp)
       {
         bool found = false;
         if ( fp )
            {
              for (int i=0; i < sel_fptr_array.size(); ++i)
                 if ( sel_fptr_array[i] == fp )
                    {
                      found = true; break;
                    }
            }
         return found;
       }

     static bool isSelected(DLFLFaceVertexPtr fvp)
       {
         bool found = false;
         if ( fvp )
            {
              for (int i=0; i < sel_fvptr_array.size(); ++i)
                 if ( sel_fvptr_array[i] == fvp )
                    {
                      found = true; break;
                    }
            }
         return found;
       }

        //--- Set the selected item at given index ---//
        //--- If size of array is smaller than index item will be added to end of array ---//
     static void setSelectedVertex(int index, DLFLVertexPtr vp)
       {
         if ( vp && index >= 0 )
            {
              if ( index < sel_vptr_array.size() ) sel_vptr_array[index] = vp;
              else sel_vptr_array.push_back(vp);
            }
       }

     static void setSelectedEdge(int index, DLFLEdgePtr ep)
       {
         if ( ep && index >= 0 )
            {
              if ( index < sel_eptr_array.size() ) sel_eptr_array[index] = ep;
              else sel_eptr_array.push_back(ep);
            }
       }

     static void setSelectedFace(int index, DLFLFacePtr fp)
       {
         if ( fp && index >= 0 )
            {
              if ( index < sel_fptr_array.size() ) sel_fptr_array[index] = fp;
              else sel_fptr_array.push_back(fp);
            }
       }

     static void setSelectedFaceVertex(int index, DLFLFaceVertexPtr fvp)
       {
         if ( fvp && index >= 0 )
            {
              if ( index < sel_fvptr_array.size() ) sel_fvptr_array[index] = fvp;
              else sel_fvptr_array.push_back(fvp);
            }
       }

        //--- Return the selected items at given index ---//
     static DLFLVertexPtr getSelectedVertex(int index)
       {
         if ( index < sel_vptr_array.size() ) return sel_vptr_array[index];
         return NULL;
       }

     static DLFLEdgePtr getSelectedEdge(int index)
       {
         if ( index < sel_eptr_array.size() ) return sel_eptr_array[index];
         return NULL;
       }

     static DLFLFacePtr getSelectedFace(int index)
       {
         if ( index < sel_fptr_array.size() ) return sel_fptr_array[index];
         return NULL;
       }

     static DLFLFaceVertexPtr getSelectedFaceVertex(int index)
       {
         if ( index < sel_fvptr_array.size() ) return sel_fvptr_array[index];
         return NULL;
       }

        //--- Find the number of items in the selection lists ---//
     static int numSelectedVertices(void)
       {
         return sel_vptr_array.size();
       }
     
     static int numSelectedEdges(void)
       {
         return sel_eptr_array.size();
       }
     
     static int numSelectedFaces(void)
       {
         return sel_fptr_array.size();
       }
     
     static int numSelectedFaceVertices(void)
       {
         return sel_fvptr_array.size();
       }

        //--- Clear individual selection lists ---//
     static void clearSelectedVertices(void)
       {
         sel_vptr_array.clear();
       }

     static void clearSelectedEdges(void)
       {
         sel_eptr_array.clear();
       }

     static void clearSelectedFaces(void)
       {
         sel_fptr_array.clear();
       }

     static void clearSelectedFaceVertices(void)
       {
         sel_fvptr_array.clear();
       }

        // Reset all selection lists
     static void clearSelected(void)
       {
         sel_vptr_array.clear();
         sel_eptr_array.clear();
         sel_fptr_array.clear();
         sel_fvptr_array.clear();
       }

        // Draw the selected items
     static void drawSelected(void);

        // Subroutine to translate FLTK events to Viewport events
     static VPMouseEvent translateFLTKEvent(int flevent)
       {
         return ( (flevent == FL_PUSH) ? VPPush :
                  ( (flevent == FL_RELEASE) ? VPRelease :
                    ( (flevent == FL_DRAG) ? VPDrag : VPUnknown ) ) );
       }

        // Constructor
     DLFLViewport(int x, int y, int w, int h)
       : Fl_Gl_Window(x,y,w,h,NULL),
         viewport(w,h), object(NULL), renderer(NULL), /*grid(ZX,20.0,10),*/
         showgrid(false), showaxes(false)
       {
         mode(FL_RGB|FL_DOUBLE|FL_DEPTH);
       }

        // Constructor
     DLFLViewport(int x, int y, int w, int h, DLFLRendererPtr rp, DLFLObjectPtr op=NULL)
       : Fl_Gl_Window(x,y,w,h,NULL),
         viewport(w,h), object(op), renderer(rp),  /*grid(ZX,20.0,10),*/
         showgrid(false), showaxes(false)
       {
         mode(FL_RGB|FL_DOUBLE|FL_DEPTH);
       }

        // Destructor
     ~DLFLViewport()
       {}

     void switchTo(VPView view)
       {
            // Change grid orientation for front/back and left/right views
         // switch ( view )
         //    {
         //      case VPFront :
         //      case VPBack :
         //                   //grid.setPlane(XY);
         //                   break;
         //      case VPLeft :
         //      case VPRight :
         //                   //grid.setPlane(YZ);
         //                   //break;
         //      default :
         //                   ;//grid.setPlane(ZX);
         //    }
         viewport.switchTo(view);
         if ( view == VPPersp )
               // Change camera settings to zoom in closer in perspective view
            viewport.setPerspView(10,10,10,0,0,0,0,1,0);
       }

        // Toggle grid display on/off
     void toggleGrid(void)
       {
         //if ( showgrid == true ) showgrid = false;
         //else showgrid = true;
       }

        // Toggle axis display on/off
     void toggleAxes(void)
       {
         if ( showaxes == true ) showaxes = false;
         else showaxes = true;
       }

        // Set the object which should be shown in this viewport
     void setObject(DLFLObjectPtr op)
       {
         object = op;
       }

        // Set the renderer for this viewport
     void setRenderer(DLFLRendererPtr rp)
       {
         renderer = rp;
       }

        // Set the render flags
     void setRenderFlags(int rflags)
       {
         if ( renderer ) renderer->setRenderFlags(rflags);
       }

        // Toggle wireframe
     void toggleWireframe(void)
       {
         if ( renderer ) renderer->toggleWireframe();
       }

        // Toggle silhouette
     void toggleSilhouette(void)
       {
         if ( renderer ) renderer->toggleSilhouette();
       }

        // Toggle vertices
     void toggleVertices(void)
       {
         if ( renderer ) renderer->toggleVertices();
       }

        // Toggle object orientation
     void toggleObjectOrientation(void)
       {
         if ( renderer ) renderer->toggleObjectOrientation();
       }
     
        // Get the render flags
     int getRenderFlags(void) const
       {
         if ( renderer )
            return renderer->getRenderFlags();
         return -1;
       }

        // Subroutines for selecting Vertices, Edges, Faces and FaceVertices (Corners)
     DLFLVertexPtr selectVertex(int mx, int my);
     DLFLEdgePtr selectEdge(int mx, int my);
     DLFLFacePtr selectFace(int mx, int my);
     DLFLFaceVertexPtr selectFaceVertex(DLFLFacePtr fp, int mx, int my);

        // Implement the draw method of Fl_Gl_Window base class
     void draw(void);

        // Event handler for the viewport. Handles only focus events and mouse events
        // which relate to navigational controls (ALT + push/drag/release)
     int handle(int event);
};


#endif /* #ifndef _DLFL_VIEWPORT_HH_ */


/*
  $Log: DLFLViewport.hh,v $
  Revision 4.1  2004/04/18 18:19:27  vinod
  Added toggleObjectOrientation method

  Revision 4.0  2003/12/26 01:58:53  vinod
  Major version sync.

  Revision 3.0  2003/09/19 16:06:47  vinod
  Major version update

  Revision 2.9  2003/01/25 08:07:27  vinod
  Added isSelected() static methods

  Revision 2.8  2002/10/22 21:24:57  vinod
  Fixed bugs in grid constructors. Added checks for NULL renderer

  Revision 2.7  2002/10/17 02:21:29  vinod
  Fixed typo in toggleVertices

  Revision 2.6  2002/10/16 14:16:03  vinod
  Added toggle methods for overlays. Removed resize override

  Revision 2.5  2002/10/09 14:56:21  vinod
  Added method to set selected item at index

  Revision 2.4  2002/10/09 07:27:48  vinod
  Added check for NULL pointers when adding to selection lists

  Revision 2.3  2002/10/08 16:17:13  vinod
  Removed active viewport member, removed copy constr and assignment op.

  Revision 2.2  2002/10/02 16:43:06  vinod
  Selection lists are now implemented as arrays. Added access functions for selection lists

  Revision 2.1  2002/09/23 07:17:12  vinod
  Viewport class to handle display of object and mouse events

*/
