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
		gr->useColorable = false; 
    gr->useMaterial = true;
    gr->useTexture = false;
    gr->useOutline = false;
  }

};

#endif /* #ifndef _SHADED_RENDERER_HH_ */
