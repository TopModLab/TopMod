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
  NormalRenderer() : DLFLRenderer() {}

  /* Copy constructor */
  NormalRenderer(const NormalRenderer& nr) : DLFLRenderer(nr) {}

  /* Destructor */
  virtual ~NormalRenderer() {}

  /* Assignment operator */
  NormalRenderer& operator = (const NormalRenderer& nr) {
    DLFLRenderer::operator = (nr);
    return (*this);
  }

  /* Implement render function */
  virtual int render(DLFLObjectPtr object) const {
    glEnable(GL_CULL_FACE);
    setCulling();
    glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
    glColor3f(0.25,0.25,0.55);
    object->plainRender();
    drawOverlays(object);
    glDisable(GL_CULL_FACE);
    return 0;
  }
};

#endif // _NORMAL_RENDERER_H_
