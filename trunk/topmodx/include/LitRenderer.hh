#ifndef _LIT_RENDERER_HH_
#define _LIT_RENDERER_HH_

/*
  LitRenderer
  A renderer for DLFL objects, derived from DLFLRenderer
  Renders with face-vertex colors and normals.
  Renders using light information.
*/

#include "DLFLRenderer.hh"

class LitRenderer;
typedef LitRenderer * LitRendererPtr;

class LitRenderer : public DLFLRenderer {
public :

  /* Default constructor */
  LitRenderer() : DLFLRenderer() { }
	
  LitRenderer(QColor wc, double wt, QColor sc, double st, QColor vc, double vt)
    : DLFLRenderer(wc, wt, sc, st, vc, vt) { }

  /* Copy constructor */
  LitRenderer(const LitRenderer& lr) : DLFLRenderer(lr) {}

  /* Assignment operator */
  LitRenderer& operator=(const LitRenderer& lr) {
    DLFLRenderer::operator=(lr);
    return (*this);
  }

  /* Destructor */
  virtual ~LitRenderer() { }

  /* Implement render function */
  virtual int render(DLFLObjectPtr object) {  
		glEnable(GL_BLEND);																			// Enable Blending
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);			// Type Of Blending To Use

		if (DLFLRenderer::antialiasing){
	    glEnable( GL_LINE_SMOOTH );
			glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);									// Set Line Antialiasing
		}
		else {
	    glDisable( GL_LINE_SMOOTH );
		}
    glEnable(GL_CULL_FACE);
    setCulling();
    glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
    //object->renderFaces();
    gr->render( object );
    drawOverlays( object );
    glDisable(GL_CULL_FACE);
    return 0;
  }

  virtual void setState( ) {
    gr->useColor = true;
    gr->useMaterial = true;
    gr->useTexture = false;
    gr->useOutline = false;
  }

};

#endif /* #ifndef _LIT_RENDERER_HH_ */
