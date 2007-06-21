#ifndef _WIREFRAME_RENDERER_H_
#define _WIREFRAME_RENDERER_H_

/**
 * WireframeRenderer
 * A renderer for DLFL objects, derived from DLFLRenderer
 * Renders with face-vertex wireframes only
*/

#include "DLFLRenderer.hh"

class WireframeRenderer;
typedef WireframeRenderer *WireframeRendererPtr;

class WireframeRenderer : public DLFLRenderer {
public :

  /* Default constructor */
  WireframeRenderer( ) : DLFLRenderer( ) { };

	WireframeRenderer(QColor wc, double wt, QColor sc, double st, QColor vc, double vt, QColor nc)
    : DLFLRenderer(wc, wt, sc, st, vc, vt) { }

  /* Copy constructor */
  WireframeRenderer( const WireframeRenderer& nr ) : DLFLRenderer( nr ) { };

  /* Destructor */
  virtual ~WireframeRenderer( ) { };

  /* Assignment operator */
  WireframeRenderer& operator=( const WireframeRenderer& nr ) {
    DLFLRenderer::operator = ( nr );
    return ( *this );
  };

  /* Implement render function */
  virtual int render( DLFLObjectPtr object ) {
    glEnable( GL_CULL_FACE );
		glEnable(GL_BLEND);																			// Enable Blending
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);			// Type Of Blending To Use
		if (DLFLRenderer::antialiasing){
	    glEnable( GL_LINE_SMOOTH );
			glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);									// Set Line Antialiasing
		}
		else {
			glDisable( GL_LINE_SMOOTH );
		}
    //drawWireframe( object );
    drawOverlays( object );
    glDisable( GL_CULL_FACE );
    return 0;
  };

  virtual void setState( ) {
    gr->useColor = false;
    gr->useMaterial = false;
    gr->useTexture = false;
    gr->useOutline = false;
  }

};

#endif // _WIREFRAME_RENDERER_H_
