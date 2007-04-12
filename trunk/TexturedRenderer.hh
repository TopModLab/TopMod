/* $Id: TexturedRenderer.hh,v 4.2 2004/04/18 18:20:35 vinod Exp $ */

#ifndef _TEXTURED_RENDERER_HH_

#define _TEXTURED_RENDERER_HH_

/*
  TexturedRenderer
  A renderer for DLFL objects, derived from DLFLRenderer
  Renders with face-vertex normals, without material colors or fave-vertex colors.
  Renders with texture, specified through an OpenGL texture ID
*/

#include "DLFLRenderer.hh"
#include <FL/Fl_Image.H>
#include <FL/Fl_JPEG_Image.H>
#include <FL/Fl_PNG_Image.H>
#include <FL/filename.H>

class TexturedRenderer;
typedef TexturedRenderer * TexturedRendererPtr;

class TexturedRenderer : public DLFLRenderer
{
  protected :

     Fl_Image * image;       // Fl_Image pointer containing image data
     int width, height, depth;        // Width, height and color depth
     GLuint texture_id;

        // Read the image from the given file
     void readTexture(const char * imagefile)
       {
         if ( fl_filename_match(imagefile,"*.jpg") ||
              fl_filename_match(imagefile,"*.jpeg") ||
              fl_filename_match(imagefile,"*.JPG") ||
              fl_filename_match(imagefile,"*.JPEG") )
            image = new Fl_JPEG_Image(imagefile);
         else if ( fl_filename_match(imagefile,"*.png") ||
                   fl_filename_match(imagefile,"*.PNG") )
            image = new Fl_PNG_Image(imagefile);
         if ( image )
            {
              width = image->w(); height = image->h(); depth = image->d();
            }
       }

     void glTexture(void)
       {
            // Do a glTexImage2D using the texture image data
         if ( image && image->data() )
            {
              if ( depth == 1 )
                 glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE, width, height, 0,
                              GL_LUMINANCE, GL_UNSIGNED_BYTE, image->data()[0]);
              else if ( depth == 2 )
                 glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE_ALPHA, width, height, 0,
                              GL_LUMINANCE_ALPHA, GL_UNSIGNED_BYTE, image->data()[0]);
              else if ( depth== 3 )
                 glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0,
                              GL_RGB, GL_UNSIGNED_BYTE, image->data()[0]);
              else if ( depth == 4 )
                 glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0,
                              GL_RGBA, GL_UNSIGNED_BYTE, image->data()[0]);
            }
       }

  public :

        /* Default constructor */
     TexturedRenderer()
       : DLFLRenderer(), image(NULL), width(0), height(0), depth(0), texture_id(0)
       {}

        /* Copy constructor */
     TexturedRenderer(const TexturedRenderer& tr)
       : DLFLRenderer(tr), image(tr.image->copy()), width(tr.width), height(tr.height), depth(tr.depth),
         texture_id(tr.texture_id)
       {}

        /* Construct using given image file as texture */
     TexturedRenderer(const char * filename)
       : DLFLRenderer(), image(NULL), width(0), height(0), depth(0), texture_id(0)
       {
         readTexture(filename);
       }

        /* Assignment operator */
     TexturedRenderer& operator = (const TexturedRenderer& tr)
       {
         DLFLRenderer::operator = (tr);
         delete image;
         image = tr.image->copy();
         width = tr.width; height = tr.height; depth = tr.depth;
         texture_id = tr.texture_id;
         return (*this);
       }

        /* Destructor */
     virtual ~TexturedRenderer()
       {
         delete image;
       }

        /* Associate a texture with this renderer */
     void setTexture(const char * filename)
       {
         delete image; image = NULL;
         readTexture(filename);
       }

     bool isValid(void) const                          // Does this object have image data?
       {
         return ( (image != NULL) && (image->data() != NULL) && (depth > 0) );
       }

        /*
          Override base class method
          Initialize the texture stuff. This should be called after viewport
          has been setup in the draw() method
        */
     virtual void initialize(void)
       {
         if ( isValid() )
            {
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
     virtual int render(DLFLObjectPtr object) const
       {
         if ( isValid() == false ) return -1;
         glEnable(GL_CULL_FACE);
         setCulling();
         glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
         glColor3f(1.0,1.0,1.0);
         glEnable(GL_TEXTURE_2D);
         glBindTexture(GL_TEXTURE_2D, texture_id);
         object->plainRenderT();
         glDisable(GL_TEXTURE_2D);
         drawOverlays(object);
         glDisable(GL_CULL_FACE);
         return 0;
       }
};

#endif /* #ifndef _TEXTURED_RENDERER_HH_ */

/*
  $Log: TexturedRenderer.hh,v $
  Revision 4.2  2004/04/18 18:20:35  vinod
  Minor change. Culling mode is now disabled *after* overlays are drawn

  Revision 4.1  2004/04/18 18:09:06  vinod
  Enabled culling when rendering. Added option to reverse object when rendering.

  Revision 4.0  2003/12/26 01:58:53  vinod
  Major version sync.

  Revision 3.0  2003/09/19 16:06:47  vinod
  Major version update

  Revision 2.5  2002/10/30 04:45:29  vinod
  Added support for loading JPG and PNG texture files using Fl_Image classes

  Revision 2.4  2002/10/22 21:26:56  vinod
  Added typedefs

  Revision 2.3  2002/10/08 05:20:47  vinod
  Made texture id int to allow negative numbers indicating invalid texture

  Revision 2.2  2002/10/06 06:09:20  vinod
  Added check for negative texture_id in render

  Revision 2.1  2002/09/23 07:15:28  vinod
  DLFL Rendering classes

*/
