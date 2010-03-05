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

#ifndef _COLORABLE_RENDERER_HH_
#define _COLORABLE_RENDERER_HH_

/*
  ColorableRenderer
  A renderer for DLFL objects, derived from DLFLRenderer
  Renders with face-vertex normals, with material colors
*/

#include "../DLFLRenderer.hh"

class ColorableRenderer;
typedef ColorableRenderer * ColorableRendererPtr;

class ColorableRenderer : public DLFLRenderer {

public :
  /* Default constructor */
  ColorableRenderer() : DLFLRenderer() { }

  ColorableRenderer(QColor wc, double wt, QColor sc, double st, QColor vc, double vt)
    {}//: DLFLRenderer(wc, wt, sc, st, vc, vt) { }
	  
  /* Copy constructor */
  ColorableRenderer(const ColorableRenderer& nr)
    : DLFLRenderer(nr) { }

  /* Assignment operator */
  ColorableRenderer& operator = (const ColorableRenderer& nr)	{
    DLFLRenderer::operator = (nr);
    return (*this);
  }

  /* Destructor */
  virtual ~ColorableRenderer() {}

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
    gr->useLighting = false;
		gr->useColorable = true;
    gr->useMaterial = false;
    gr->useTexture = false;
    gr->useOutline = false;
  }

};

#endif /* #ifndef _COLORABLE_RENDERER_HH_ */
