#ifndef _SHADED_RENDERER_HH_
#define _SHADED_RENDERER_HH_

/*
  ShadedRenderer
  A renderer for DLFL objects, derived from DLFLRenderer
  Renders with face-vertex normals, with material colors
*/

#include "../DLFLRenderer.hh"

class ShadedRenderer;
typedef ShadedRenderer * ShadedRendererPtr;

class ShadedRenderer : public DLFLRenderer {

public :
  /* Default constructor */
  ShadedRenderer() : DLFLRenderer() { }

  ShadedRenderer(QColor wc, double wt, QColor sc, double st, QColor vc, double vt)
    : DLFLRenderer(wc, wt, sc, st, vc, vt) { }
	
  /* Copy constructor */
  ShadedRenderer(const ShadedRenderer& nr)
    : DLFLRenderer(nr) { }

  /* Assignment operator */
  ShadedRenderer& operator = (const ShadedRenderer& nr)	{
    DLFLRenderer::operator = (nr);
    return (*this);
  }

  /* Destructor */
  virtual ~ShadedRenderer() {}

  /* Implement render function */
  virtual int render(DLFLObjectPtr object) {
    glEnable(GL_CULL_FACE);
    setCulling();
    glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
    gr->render( object );
    //object->render();
    drawOverlays(object);
    glDisable(GL_CULL_FACE);
    return 0;
  }

  virtual void setState( ) { 
    gr->useLighting = false;
    gr->useMaterial = true;
    gr->useTexture = false;
    gr->useOutline = false;
  }

};

#endif /* #ifndef _SHADED_RENDERER_HH_ */
