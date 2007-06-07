#ifndef _NORMAL_RENDERER_H_
#define _NORMAL_RENDERER_H_

/**
 * NormalRenderer
 * A renderer for DLFL objects, derived from DLFLRenderer
 * Renders with face-vertex wireframes with solid colors, no shading/materials/lighting
*/

#include "DLFLRenderer.hh"

class NormalRenderer;
typedef NormalRenderer *NormalRendererPtr;

class NormalRenderer : public DLFLRenderer {	
public :
  /* Default constructor */
  NormalRenderer() : DLFLRenderer() {
  }
	
  NormalRenderer(QColor wc, double wt, QColor sc, double st, QColor vc, double vt, QColor nc)
    : DLFLRenderer(wc, wt, sc, st, vc, vt) {
  }

  /* Copy constructor */
  NormalRenderer(const NormalRenderer& nr) : DLFLRenderer(nr) { }

  /* Assignment operator */
  NormalRenderer& operator = (const NormalRenderer& nr) {
    DLFLRenderer::operator = (nr);
    return (*this);
  }

  /* Destructor */
  virtual ~NormalRenderer() {}

  /* Implement render function */
  virtual int render(DLFLObjectPtr object)  {		
    glEnable(GL_CULL_FACE);
    setCulling();
    glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
    //glColor4f(mNormalColor.redF(),mNormalColor.greenF(),mNormalColor.blueF(),mNormalColor.alphaF());
    //object->plainRender();
    gr->render( object );
    drawOverlays(object);
    glDisable(GL_CULL_FACE);
    return 0;
  }
	
  virtual void setState( ) {
    gr->useColor = false;
    gr->useMaterial = true;
    gr->useTexture = false;
    gr->useOutline = false;
  }
};

#endif /* #ifndef _NORMAL_RENDERER_HH_ */

