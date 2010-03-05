/*
*
* ***** BEGIN GPL LICENSE BLOCK *****
*
* This program is free software; you can redistribute it and/or
* modify it under the terms of the GNU General Public License
* as published by the Free Software Foundation; either version 2
* of the License, or (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program; if not, write to the Free Software  Foundation,
* Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
*
* The Original Code is Copyright (C) 2005 by xxxxxxxxxxxxxx
* All rights reserved.
*
* The Original Code is: all of this file.
*
* Contributor(s): none yet.
*
* ***** END GPL LICENSE BLOCK *****
*/

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
  TexturedLitRenderer() : TexturedRenderer(){ }

  /* Copy constructor */
  TexturedLitRenderer(const TexturedLitRenderer& tlr)
    : TexturedRenderer(tlr) { }

  /* Construct using given image file as texture */
  TexturedLitRenderer(const char * filename)
    : TexturedRenderer(filename) { }
	
  TexturedLitRenderer(QColor wc, double wt, QColor sc, double st, QColor vc, double vt, QColor fc, double ft, QColor nc, double nt)
    : TexturedRenderer(wc, wt, sc, st, vc, vt, fc, ft, nc, nt) { }

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
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture_id);
    gr->render( object );
    //object->renderFacesT();
    glDisable(GL_TEXTURE_2D);
    drawOverlays(object);
    glDisable(GL_CULL_FACE);
    return 0;
  }

  virtual void setState( ) {
    gr->useLighting = true;
		gr->useColorable = false; 
    gr->useTexture = true;
    gr->useMaterial = false;
    gr->useOutline = false;
  }

};

#endif /* #ifndef _TEXTURED_LIT_RENDERER_HH_ */
