/* $Id: LitRenderer.hh,v 4.2 2004/04/18 18:20:35 vinod Exp $ */

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
	LitRenderer()
	: DLFLRenderer()	{
		
	}
	
	LitRenderer(QColor wc, double wt, QColor sc, double st, QColor vc, double vt)
	: DLFLRenderer(wc, wt, sc, st, vc, vt) {

	}

	/* Copy constructor */
	LitRenderer(const LitRenderer& lr)
	: DLFLRenderer(lr)
	{}

	/* Assignment operator */
	LitRenderer& operator = (const LitRenderer& lr) {
		DLFLRenderer::operator = (lr);
		return (*this);
	}

	/* Destructor */
	virtual ~LitRenderer() {}

	/* Implement render function */
	virtual int render(DLFLObjectPtr object) {
		glEnable (GL_BLEND); 
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		
		glEnable(GL_CULL_FACE);
		setCulling();
		glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
		object->renderFaces();
		drawOverlays(object);
		glDisable(GL_CULL_FACE);
		return 0;
	}

};

#endif /* #ifndef _LIT_RENDERER_HH_ */

/*
  $Log: LitRenderer.hh,v $
  Revision 4.2  2004/04/18 18:20:35  vinod
  Minor change. Culling mode is now disabled *after* overlays are drawn

  Revision 4.1  2004/04/18 18:09:06  vinod
  Enabled culling when rendering. Added option to reverse object when rendering.

  Revision 4.0  2003/12/26 01:58:53  vinod
  Major version sync.

  Revision 3.0  2003/09/19 16:06:47  vinod
  Major version update

  Revision 2.2  2002/10/22 21:26:56  vinod
  Added typedefs

  Revision 2.1  2002/09/23 07:15:29  vinod
  DLFL Rendering classes

*/
