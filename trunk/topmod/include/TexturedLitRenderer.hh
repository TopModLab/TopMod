/* $Id: TexturedLitRenderer.hh,v 4.2 2004/04/18 18:20:35 vinod Exp $ */

#ifndef _TEXTURED_LIT_RENDERER_HH_

#define _TEXTURED_LIT_RENDERER_HH_

/*
  TexturedLitRenderer
  A renderer for DLFL objects, derived from DLFLTexturedRenderer
  Renders with face-vertex normals, without material colors or fave-vertex colors.
  Renders with texture, specified through an OpenGL texture ID
  Renders with lighting information
*/

#include "TexturedRenderer.hh"

class TexturedLitRenderer;
typedef TexturedLitRenderer * TexturedLitRendererPtr;

class TexturedLitRenderer : public TexturedRenderer {

public :

	/* Default constructor */
	TexturedLitRenderer()
	: TexturedRenderer(){
		
	}

	/* Copy constructor */
	TexturedLitRenderer(const TexturedLitRenderer& tlr)
	: TexturedRenderer(tlr) {
		
	}

	/* Construct using given image file as texture */
	TexturedLitRenderer(const char * filename)
	: TexturedRenderer(filename) {
		
	}
	
	TexturedLitRenderer(QColor wc, double wt, QColor sc, double st, QColor vc, double vt)
	: TexturedRenderer(wc, wt, sc, st, vc, vt) {

	}

	/* Assignment operator */
	TexturedLitRenderer& operator = (const TexturedLitRenderer& tlr) {
		TexturedRenderer::operator = (tlr);
		return (*this);
	}

	/* Destructor */
	virtual ~TexturedLitRenderer() { }

	/* Implement render function */
	virtual int render(DLFLObjectPtr object) {
		if ( isValid() == false ) return -1;
		
		glEnable (GL_BLEND); 
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		
		glEnable(GL_CULL_FACE);
		setCulling();
		glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, texture_id);
		object->renderFacesT();
		glDisable(GL_TEXTURE_2D);
		drawOverlays(object);
		glDisable(GL_CULL_FACE);
		return 0;
	}

};

#endif /* #ifndef _TEXTURED_LIT_RENDERER_HH_ */
