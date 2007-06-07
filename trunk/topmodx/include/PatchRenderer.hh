#ifndef _PATCH_RENDERER_HH_
#define _PATCH_RENDERER_HH_

/*
  PatchRenderer
  A renderer for DLFL objects, derived from DLFLRenderer
  Renders using Bezier patches
*/

#include "DLFLRenderer.hh"

class PatchRenderer;
typedef PatchRenderer * PatchRendererPtr;

class PatchRenderer : public DLFLRenderer {
	
public :

  /* Default constructor */
  PatchRenderer() : DLFLRenderer() {}
	
  PatchRenderer(QColor wc, double wt, QColor sc, double st, QColor vc, double vt)
    : DLFLRenderer(wc, wt, sc, st, vc, vt) { }

  /* Copy constructor */
  PatchRenderer(const PatchRenderer& nr)
    : DLFLRenderer(nr)
  {}

  /* Assignment operator */
  PatchRenderer& operator = (const PatchRenderer& nr) {
    operator = (nr);
    return (*this);
  }

  /* Destructor */
  virtual ~PatchRenderer() {}

  /* Draw the overlays - different from regular renderers */
  virtual void drawOverlays( TMPatchObjectPtr patch_object ) {//DLFLObjectPtr object ) {    
    if ( render_flags & ShowWireframe ) {
      glColor3d(0.0,0.0,0.0);
      glLineWidth(.8);
      glDepthRange(0.0,1.0-0.0005);
      patch_object->renderWireframePatches();
    }

    if ( render_flags & ShowSilhouette ) {
      glColor3d(0.4,0.6,0.4);
      glLineWidth(3.0);
      glDepthRange(0.0,1.0-0.00055);
      patch_object->renderPatchBoundaries();

      glColor3d(0.2,0.2,0.3);
      glLineWidth(3.0);
      glDepthRange(0.0,1.0-0.0006);
      patch_object->renderPatchFaceBoundaries();
      }

    if ( render_flags & ShowVertices ){
      glColor3d(0.2,0.2,0.3);
      glLineWidth(3.0);
      glDepthRange(0.0,1.0-0.0006);
      patch_object->renderPatchFaceBoundaries();

      glColor3d(0.5,1.0,0.0);
      glPointSize(5.0);
      glDepthRange(0.0,1.0-0.00075);
      patch_object->renderPointPatches();
    }

    if ( render_flags & ShowNormals )	{
      glColor3d(0.2,0.8,0.2);
      glLineWidth(1.0);
      patch_object->renderPatchNormals();
    }
  }

  virtual void drawOverlays( DLFLObjectPtr object ) { }

  /* Implement render function */
  virtual int render( TMPatchObjectPtr patchObject ) { // DLFLObjectPtr object )
    glEnable(GL_CULL_FACE);
    setCulling();
    glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
    glColor3d(0.3,0.5,0.8);
    //gr->renderPatches( object );
    patchObject->renderPatches();
    drawOverlays(patchObject);
    glColor3d(0.0,0.0,0.0);
    //gr->renderEdges( object, 1.0 );
    glDisable(GL_CULL_FACE);
    return 0;
  }

  virtual int render( DLFLObjectPtr object ) {
    glEnable(GL_CULL_FACE);					
    setCulling();
    glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
    glColor3d(0.0,0.0,0.0);
    gr->renderEdges( object, 1.0 );
    glDisable(GL_CULL_FACE);
    return 0;
  }

  virtual void setState( ) {
    gr->useColor = false;
    gr->useOutline = true;
  }

};

#endif /* #ifndef _PATCH_RENDERER_HH_ */
