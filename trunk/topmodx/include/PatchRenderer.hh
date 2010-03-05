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

#ifndef _PATCH_RENDERER_HH_
#define _PATCH_RENDERER_HH_

/*
  PatchRenderer
  A renderer for DLFL objects, derived from DLFLRenderer
  Renders using Bezier patches
*/

#include "../DLFLRenderer.hh"

class PatchRenderer;
typedef PatchRenderer * PatchRendererPtr;

class PatchRenderer : public DLFLRenderer {
	
public :

  /* Default constructor */
  PatchRenderer() : DLFLRenderer() {}
	
  PatchRenderer(QColor wc, double wt, QColor sc, double st, QColor vc, double vt, QColor fc, double ft, QColor nc, double nt)
    : DLFLRenderer(wc, wt, sc, st, vc, vt, fc, ft, nc, nt) { }

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
  virtual void drawPatchOverlays( TMPatchObjectPtr patch_object ) {
    if ( gr->drawWireframe ) {
      glColor3d(0.0,0.0,0.0);
      glLineWidth(.8);
      glDepthRange(0.0,1.0-0.0005);
      patch_object->renderWireframePatches();
    }

    if ( gr->drawSilhouette ) {
      glColor3d(0.4,0.6,0.4);
      glLineWidth(3.0);
      glDepthRange(0.0,1.0-0.00055);
      patch_object->renderPatchBoundaries();

      glColor3d(0.2,0.2,0.3);
      glLineWidth(3.0);
      glDepthRange(0.0,1.0-0.0006);
      patch_object->renderPatchFaceBoundaries();
		}

    if ( gr->drawVertices ){
      glColor3d(0.2,0.2,0.3);
      glLineWidth(3.0);
      glDepthRange(0.0,1.0-0.0006);
      patch_object->renderPatchFaceBoundaries();

      glColor3d(0.5,1.0,0.0);
      glPointSize(5.0);
      glDepthRange(0.0,1.0-0.00075);
      patch_object->renderPointPatches();
    }

    if ( gr->drawFaceNormals )	{
      glColor3d(0.2,0.8,0.2);
      glLineWidth(1.0);
      patch_object->renderPatchNormals();
    }
  }

  /* Implement render function */
  virtual int render( TMPatchObjectPtr patchObject ) { // DLFLObjectPtr object )
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
    glColor3d(0.3,0.5,0.8);
    patchObject->renderPatches();
    drawPatchOverlays(patchObject);
    glColor3d(0.0,0.0,0.0);
    glDisable(GL_CULL_FACE);
    return 0;
  }

  virtual int render( DLFLObjectPtr object ) {
    glEnable( GL_CULL_FACE );
		glEnable(GL_BLEND);																			// Enable Blending
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);			// Type Of Blending To Use
		if (DLFLRenderer::antialiasing){
	    glEnable( GL_LINE_SMOOTH );
			glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);									// Set Line Antialiasing
		}
		else {
			glDisable( GL_LINE_SMOOTH );
		}
    //drawWireframe( object );
    drawOverlays( object );
    glDisable( GL_CULL_FACE );
    return 0;
  }

  virtual void setState( ) {
    gr->useLighting = false;
		gr->useColorable = false; 
    gr->useOutline = true;
		gr->useTexture = false;
		gr->useMaterial = false;
  }

};

#endif /* #ifndef _PATCH_RENDERER_HH_ */
