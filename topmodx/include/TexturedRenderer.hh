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

#ifndef _TEXTURED_RENDERER_HH_
#define _TEXTURED_RENDERER_HH_

/*
  TexturedRenderer
  A renderer for DLFL objects, derived from DLFLRenderer
  Renders with face-vertex normals, without material colors or fave-vertex colors.
  Renders with texture, specified through an OpenGL texture ID
*/
#include <QFile>
#include <QString>
#include <QImage>
#include "../DLFLRenderer.hh"

class TexturedRenderer;
typedef TexturedRenderer * TexturedRendererPtr;

class TexturedRenderer : public DLFLRenderer {

protected :

  QImage * image;       // QImage pointer containing image data
  int width, height, depth;        // Width, height and color depth
  GLuint texture_id;

  // Read the image from the given file
  void readTexture(const char *imagefile) {
    QString filename(imagefile);
    if ( 	filename.indexOf(".jpg",filename.length()-5) == filename.length()-4 ||
					filename.indexOf(".jpeg",filename.length()-5) == filename.length()-4 ||
					filename.indexOf(".JPG",filename.length()-5) == filename.length()-4 ||
					filename.indexOf(".JPEG",filename.length()-5) == filename.length()-4 ||
					filename.indexOf(".png",filename.length()-5) == filename.length()-4 ||			  
					filename.indexOf(".PNG",filename.length()-5) == filename.length()-4 )
      image = new QImage(imagefile);
		// std::cout << image->height() << "\n";
    if ( !image->isNull() )	{
      width = image->width(); 
      height = image->height(); 
      depth = image->depth()/8;
			// glTexture();
    }
  }

  void glTexture(void) {
		// std::cout << image->bits() << "\n";
	
    // Do a glTexImage2D using the texture image data
    if ( image && !image->isNull() ) {
			// std::cout<<"depth  = " <<  depth << "\n";
      if ( depth == 1 )
				glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE, width, height, 0, GL_LUMINANCE, GL_UNSIGNED_BYTE, image->bits());
      else if ( depth == 2 )
				glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE_ALPHA, width, height, 0, GL_LUMINANCE_ALPHA, GL_UNSIGNED_BYTE, image->bits());
      else if ( depth== 3 )
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image->bits());
      else if ( depth == 4 )
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0,	GL_RGBA, GL_UNSIGNED_BYTE, image->bits());
    }
  }

public :

  /* Default constructor */
  TexturedRenderer()
    : DLFLRenderer(), image(NULL), width(0), height(0), depth(0), texture_id(0) { 
		
  }
	
  TexturedRenderer(QColor wc, double wt, QColor sc, double st, QColor vc, double vt, QColor fc, double ft, QColor nc, double nt)
    : DLFLRenderer(wc, wt, sc, st, vc, vt, fc, ft, nc, nt) {

  }

  /* Copy constructor */
  TexturedRenderer(const TexturedRenderer& tr)
    : DLFLRenderer(tr), width(tr.width), height(tr.height), depth(tr.depth), texture_id(tr.texture_id) { 
    image = new QImage(tr.image->bits(),tr.width,tr.height,tr.image->format());
    //(*image) = tr.image->copy();
  }

  /* Construct using given image file as texture */
  TexturedRenderer(const char * filename)
    : DLFLRenderer(), image(NULL), width(0), height(0), depth(0), texture_id(0) {
    readTexture(filename);
  }

  /* Assignment operator */
  TexturedRenderer& operator = (const TexturedRenderer& tr) {
    DLFLRenderer::operator = (tr);
    delete image;
    image = new QImage(tr.image->bits(),tr.width,tr.height,tr.image->format());
    width = tr.width; height = tr.height; depth = tr.depth;
    texture_id = tr.texture_id;
    return (*this);
  }

  /* Destructor */
  virtual ~TexturedRenderer() {
    delete image;
  }

  /* Associate a texture with this renderer */
  void setTexture(const char * filename) {
    delete image; image = NULL;
    readTexture(filename);
  }

  bool isValid(void) {
    return ( (image != NULL) && (!image->isNull()) && (depth > 0) );
  }

  /*
    Override base class method
    Initialize the texture stuff. This should be called after viewport
    has been setup in the draw() method
  */
  virtual void initialize(void) {
    if ( isValid() ) {
      glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

      glGenTextures(1,&texture_id);
      glBindTexture(GL_TEXTURE_2D, texture_id);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
      glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
      glTexture();
    }
  }

  /* Implement render function. Doesn't render if texture_id is negative */
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
    glColor3f(1.0,1.0,1.0);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture_id);
    //object->plainRenderT();
    gr->render( object );
    glDisable(GL_TEXTURE_2D);
    drawOverlays(object);
    glDisable(GL_CULL_FACE);
    return 0;
  }

  virtual void setState( ) {
    gr->useLighting = false;
		gr->useColorable = false; 
    gr->useMaterial = false;
    gr->useTexture = true;
    gr->useOutline = false;
  }
};

#endif /* #ifndef _TEXTURED_RENDERER_HH_ */
