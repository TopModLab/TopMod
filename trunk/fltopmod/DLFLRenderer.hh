/* $Id: DLFLRenderer.hh,v 4.2 2004/04/23 10:42:06 vinod Exp $ */

#ifndef _DLFL_RENDERER_HH_

#define _DLFL_RENDERER_HH_

// Abstrtact base class for various renderers for DLFL
// Defines the minimum interface that renderer classes must implement
// Also contains an integer which can be used to store render flags
// The way the integer is used to specify the render flags is left
// to the derived class. An enumeration for render flags is also defined
// with the various values defined as powers of 2 to allow ORing together
// The class also provides static functions for rendering the object as a wireframe,
// silhouette and vertices, which can be used by derived classes

#include <GL/gl.h>
#include <GL/glu.h>
#include "DLFLObject.hh"

class DLFLRenderer;
typedef DLFLRenderer * DLFLRendererPtr;

class DLFLRenderer
{
  protected :

        /* Integer which can be used to represent a combination of render flags */
     static int render_flags;

        /* Flag indicating if we should front or back side of object */
     static bool reverse_object;

  public :

        /* Enumeration for render flags */
     enum RenderFlags { ShowWireframe=1, ShowSilhouette=2, ShowVertices=4, 
                        ShowPatchWireframe=8, ShowPatchVertices=16 };

  protected :
     
        /* Default constructor */
     DLFLRenderer()
       {}

        /* Copy constructor */
     DLFLRenderer(const DLFLRenderer& renderer)
       {}

  public :

        /* Assignment operator */
     DLFLRenderer& operator = (const DLFLRenderer& renderer)
       {
         return (*this);
       }

        /* Destructor */
     virtual ~DLFLRenderer()
       {}

        /* Set the render flags */
     void setRenderFlags(int rflags)
       {
         DLFLRenderer::render_flags = rflags;
       }

     void toggleWireframe(void)
       {
         DLFLRenderer::render_flags ^= ShowWireframe;
       }

     void toggleSilhouette(void)
       {
         DLFLRenderer::render_flags ^= ShowSilhouette;
       }

     void toggleVertices(void)
       {
         DLFLRenderer::render_flags ^= ShowVertices;
       }

     void togglePatchWireframe(void)
       {
         DLFLRenderer::render_flags ^= ShowPatchWireframe;
       }

     void togglePatchVertices(void)
       {
         DLFLRenderer::render_flags ^= ShowPatchVertices;
       }

     void toggleObjectOrientation(void)
       {
         if ( DLFLRenderer::reverse_object ) DLFLRenderer::reverse_object = false;
         else DLFLRenderer::reverse_object = true;
       }

        /* Get the render flags */
     int getRenderFlags(void) const
       {
         return DLFLRenderer::render_flags;
       }


        /*
          Render the DLFLObject specified by the given pointer.
          This is a virtual function which has to be implemented by derived classes.
          Return value can be used for error reporting..
        */
     virtual int render(DLFLObjectPtr object) const = 0;


        /*
          Perform any initializations required
        */
     virtual void initialize(void)
       {
            // Derived classes should override this if necessary 
            // to do any initializations (like texture setup)
       }

  public :

        /*
          Set the culling based on which side of object we want to display.
          Has effect only if culling is enabled
        */
     static void setCulling(void)
       {
         if ( reverse_object ) glCullFace(GL_FRONT);
         else glCullFace(GL_BACK);
       }

        /* Methods for various types of rendering */

     static void drawWireframe(DLFLObjectPtr object)
       {
         glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
         glColor3f(0.1,0.1,0.2);
         glDepthRange(0.0,1.0-0.0005);
         object->renderEdges(0.9);
       }
     
     static void drawSilhouette(DLFLObjectPtr object)
       {
         glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
         glColor3f(0.0,0.0,0.0);
         glDepthRange(0.1,1.0);
         if ( reverse_object ) object->renderEdges(2.0);
         else object->renderEdges(6.0);
       }
     
     static void drawVertices(DLFLObjectPtr object)
       {
         glColor3f(0.5,1.0,0.0);
         glDepthRange(0.0,1.0-0.00075);
         object->renderVertices(5.0);
       }

     static void drawPatchWireframe(DLFLObjectPtr object)
       {
         glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
         glColor3f(0.1,0.1,0.2);
         glDepthRange(0.0,1.0-0.0005);
         object->renderPatchWireframe();
       }

     static void drawPatchVertices(DLFLObjectPtr object)
       {
         glPolygonMode(GL_FRONT_AND_BACK,GL_POINT); 
         glColor3f(1.0,1.0,0.0);
         glDepthRange(0.0,1.0-0.00075);
         object->renderPatchVertices(5.0);
       }

        /*
          This checks the render flags and calls the above functions.
          This is not defined as static since it needs access to the render flags
        */
     void drawOverlays(DLFLObjectPtr object) const
       {
         if ( DLFLRenderer::render_flags & ShowWireframe ) DLFLRenderer::drawWireframe(object);
         if ( DLFLRenderer::render_flags & ShowSilhouette ) DLFLRenderer::drawSilhouette(object);
         if ( DLFLRenderer::render_flags & ShowVertices ) DLFLRenderer::drawVertices(object);
//         if ( DLFLRenderer::render_flags & ShowPatchWireframe) DLFLRenderer::drawPatchWireframe(object);
//         if ( DLFLRenderer::render_flags & ShowPatchVertices) DLFLRenderer::drawPatchVertices(object);
       }
};

#endif /* #ifndef _DLFL_RENDERER_HH_ */

/*
  $Log: DLFLRenderer.hh,v $
  Revision 4.2  2004/04/23 10:42:06  vinod
  Changed edge rendering thickness for silhouette when reverse_object is rendered

  Revision 4.1  2004/04/18 18:09:06  vinod
  Enabled culling when rendering. Added option to reverse object when rendering.

  Revision 4.0  2003/12/26 01:58:53  vinod
  Major version sync.

  Revision 3.5  2003/12/08 17:04:32  vinod
  Removed glBegin and glEnd from drawVertices method.
  They are now down in the lower level renderVertices method for correct operation

  Revision 3.4  2003/12/07 01:04:56  vinod
  Modified wireframe and silhouette modes to render only the edges instead of every face

  Revision 3.3  2003/10/26 05:20:12  vinod
  Changed point size and depth range for vertex rendering

  Revision 3.2  2003/10/26 05:11:29  vinod
  Fixed vertex rendering

  Revision 3.1  2003/10/25 22:32:55  vinod
  Made the render_flags variable a class variable

  Revision 3.0  2003/09/19 16:06:47  vinod
  Major version update

  Revision 2.3  2002/10/30 03:41:44  vinod
  Added virtual method to allow initialization

  Revision 2.2  2002/10/16 14:14:47  vinod
  Added toggle methods for overlays

  Revision 2.1  2002/09/23 07:15:27  vinod
  DLFL Rendering classes

*/
