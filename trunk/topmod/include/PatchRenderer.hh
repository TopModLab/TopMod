/* $Id$ */

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
	PatchRenderer()
	: DLFLRenderer()
	{}
	
	PatchRenderer(QColor wc, double wt, QColor sc, double st, QColor vc, double vt)
	: DLFLRenderer(wc, wt, sc, st, vc, vt) {

	}

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
	virtual void drawOverlays(DLFLObjectPtr object) {
		if ( render_flags & ShowWireframe ) {
			glColor3f(0.0,0.0,0.0);
			glLineWidth(4);
			glDepthRange(0.0,1.0-0.0005);
			object->renderWireframePatches();
		}
		if ( render_flags & ShowSilhouette ) {
			glColor3f(0.0,1.0,0.0);
			glLineWidth(6.0);
			glDepthRange(0.0,1.0-0.00055);
			object->renderPatchBoundaries();

			glColor3f(1.0,0.0,0.0);
			glLineWidth(8.0);
			glDepthRange(0.0,1.0-0.0006);
			object->renderPatchFaceBoundaries();
		}
		if ( render_flags & ShowVertices ){
			/*
			glColor3f(0.2,0.2,0.3);
			glLineWidth(3.0);
			glDepthRange(0.0,1.0-0.0006);
			object->renderPatchFaceBoundaries(); */

			glColor3f(0.5,1.0,0.0);
			glPointSize(5.0);
			glDepthRange(0.0,1.0-0.00075);
			object->renderPointPatches();
		}
		if ( render_flags & ShowNormals )	{
			glColor3f(0.2,0.8,0.2);
			glLineWidth(1.0);
			object->renderPatchNormals();
		}
	}


	/* Implement render function */
	virtual int render(DLFLObjectPtr object) {
				//added by dave...
		// glEnable(GL_LINE_SMOOTH);
		glEnable (GL_BLEND);
		glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		// glHint (GL_LINE_SMOOTH_HINT, GL_NICEST);
		// glLineWidth (1.5);

		glEnable(GL_CULL_FACE);					
		setCulling();
		glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
		glColor3f(0.3,0.5,0.8);
		object->renderPatches();
		drawOverlays(object);

		glColor3f(0.0,0.0,0.0);
		// glColor3f(0.5,0.2,0.2);

		object->renderEdges(1.0);
		glDisable(GL_CULL_FACE);
		return 0;
	}

};

#endif /* #ifndef _PATCH_RENDERER_HH_ */

/*
  $Log$
*/
