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

#ifndef _LIT_RENDERER_HH_
#define _LIT_RENDERER_HH_

/*
  LitRenderer
  A renderer for DLFL objects, derived from DLFLRenderer
  Renders with face-vertex colors and normals.
  Renders using light information.
*/

#include "../DLFLRenderer.hh"

class LitRenderer;
typedef LitRenderer * LitRendererPtr;

class LitRenderer : public DLFLRenderer {
public :

  /* Default constructor */
  LitRenderer() : DLFLRenderer() { }
	
  LitRenderer(QColor wc, double wt, QColor sc, double st, QColor vc, double vt, QColor fc, double ft, QColor nc, double nt)
    : DLFLRenderer(wc, wt, sc, st, vc, vt, fc, ft, nc, nt) { }

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
    glEnable(GL_CULL_FACE);
    setCulling();
    glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);

		glEnable(GL_BLEND);																			// Enable Blending
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);			// Type Of Blending To Use

		if (DLFLRenderer::antialiasing){
	    glEnable( GL_LINE_SMOOTH );
			glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);									// Set Line Antialiasing
		}
		else {
	    glDisable( GL_LINE_SMOOTH );
		}
    //object->renderFaces();
    gr->render( object );
    drawOverlays( object );
    glDisable(GL_CULL_FACE);
    return 0;
  }

  virtual void setState( ) {
    gr->useLighting = true;
		gr->useColorable = false; 
    gr->useMaterial = true;
    gr->useTexture = false;
    gr->useOutline = false;
  }

};

#endif /* #ifndef _LIT_RENDERER_HH_ */
