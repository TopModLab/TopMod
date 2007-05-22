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

#ifdef __APPLE__
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#else
#include <GL/gl.h>
#include <GL/glu.h>
#endif
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
     enum RenderFlags { ShowWireframe=1, ShowSilhouette=2, ShowVertices=4, ShowNormals=8 };

  protected :
     
        /* Default constructor */
     DLFLRenderer()
       {}

        /* Copy constructor */
     DLFLRenderer(const DLFLRenderer&)
       {}

  public :

        /* Assignment operator */
     DLFLRenderer& operator = (const DLFLRenderer&)
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

     void toggleNormals(void)
       {
         DLFLRenderer::render_flags ^= ShowNormals;
       }

     void toggleObjectOrientation(void)
       {
         if ( DLFLRenderer::reverse_object ) DLFLRenderer::reverse_object = false;
         else DLFLRenderer::reverse_object = true;
       }

     static bool isReversed(void)
       {
         return DLFLRenderer::reverse_object;
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
         glColor4f(0.1,0.1,0.2,0.8);
         glDepthRange(0.0,1.0-0.0005);
         object->renderEdges(1.5);//32.03232
	 glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
       }
     
     static void drawSilhouette(DLFLObjectPtr object)
       {
         glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
         glColor4f(0.0,0.0,0.0,0.8);
         glDepthRange(0.1,1.0);
         if ( reverse_object ) object->renderEdges(2.0);
         else object->renderEdges(6.0);
	 glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
       }
     
     static void drawVertices(DLFLObjectPtr object)
       {
         glColor4f(0.5,1.0,0.0,0.9);
         glDepthRange(0.0,1.0-0.00075);
         object->renderVertices(5.0);
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
       }
};

#endif 
