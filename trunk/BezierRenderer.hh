#ifndef _BEZIER_RENDERER_HH_

#define _BEZIER_RENDERER_HH_

/*
  BezierRenderer
  A renderer for DLFL objects, derived from DLFLRenderer
  Renders bezier patches corresponding to every face vertex
  Renders using light information.
*/

#include "DLFLRenderer.hh"
#include "TMBezierSurface.hh"

class BezierRenderer;
typedef BezierRenderer * BezierRendererPtr;

class BezierRenderer : public DLFLRenderer
{
  protected :

     TMBezierSurface beziersurface;

  public :

        /* Default constructor */
     BezierRenderer()
       : DLFLRenderer(), beziersurface()
       {}

        /* 1-arg constructor */
     BezierRenderer(DLFLObjectPtr object)
       : DLFLRenderer(), beziersurface(object)
       {}

        /* Copy constructor */
     BezierRenderer(const BezierRenderer& br)
       : DLFLRenderer(br), beziersurface(br.beziersurface)
       {}

        /* Assignment operator */
     BezierRenderer& operator = (const BezierRenderer& br)
       {
         DLFLRenderer::operator = (br);
         beziersurface = br.beziersurface;
         return (*this);
       }

        /* Destructor */
     virtual ~BezierRenderer()
       {}

        /* Attach a new object to the bezier surface */
     void setObject(DLFLObjectPtr object)
       {
         beziersurface.setObject(object);
       }

        /* Change the type of bezier surface */
     void setPatchType(TMPatchType type)
       {
         beziersurface.setPatchType(type);
       }

        /* Update the bezier surface - to be called if object is changed*/
     void updateBezierSurface(void)
       {
         beziersurface.createSurface();
       }

     void drawBezierOverlays() const
       {
         if ( DLFLRenderer::render_flags & ShowPatchWireframe)
            {
              glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
              glColor3f(0.25,0.25,0.5);
              glDepthRange(0.0,1.0-0.0005);
              beziersurface.renderWireframe();
            }
         if ( DLFLRenderer::render_flags & ShowPatchVertices)
            {
              glPolygonMode(GL_FRONT_AND_BACK,GL_POINT); 
              glColor3f(0.2,0.75,0.75);
              glDepthRange(0.0,1.0-0.00075);
              beziersurface.renderPoints();
            }
       }
       
        /* Implement render function */
     virtual int render(DLFLObjectPtr) const
       {
         glEnable(GL_CULL_FACE);
         setCulling();
         glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
//         object->renderPatches();
         glColor3f(0.75,0.75,0.75);
         beziersurface.render();
         drawBezierOverlays();
//         drawOverlays(object);
         glDisable(GL_CULL_FACE);
         return 0;
       }
};

#endif /* #ifndef _BEZIER_RENDERER_HH_ */
