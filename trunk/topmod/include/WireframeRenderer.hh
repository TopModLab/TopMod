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
  virtual int render( DLFLObjectPtr object ) const {
    glEnable( GL_CULL_FACE );

    glEnable( GL_LINE_SMOOTH );
    glEnable( GL_BLEND );
    glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
    glHint( GL_LINE_SMOOTH_HINT, GL_NICEST );

    drawWireframe( object );
    drawOverlays( object );
    glDisable( GL_CULL_FACE );
    return 0;
  };
};

#endif // _WIREFRAME_RENDERER_H_
