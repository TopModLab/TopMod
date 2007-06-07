/* $Id: Texture.hh,v 2.3 2000/07/20 23:18:05 vinod Exp $ */

#ifndef _TEXTURE_HH_

#define _TEXTURE_HH_

// Class for using any image supported by SGIs Image Format Library
// as a texture in OpenGL.
// The image is stored in interleaved, unsigned char format,
// with the lower-left corner as the origin

#include <ifl/iflFile.h>

#ifdef __APPLE__
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#else
#include <GL/gl.h>
#include <GL/glu.h>
#endif

class Texture;
typedef Texture * TexturePtr;

class Texture
{
  protected :

     unsigned char * pixels;                           // Pixels in image
     iflSize         dims;                             // Image dimensions

        // Read the image from the given file
     void readTexture(const char * imagefile)
       {
         if ( imagefile )
            {
              iflStatus status;
              iflFile * image = iflFile::open(imagefile,O_RDONLY, &status);
              if ( status == iflOKAY )
                 {
                   image->getDimensions(dims);
                   pixels = new unsigned char [dims.y*dims.x*dims.c];

                   iflConfig config(iflUChar,iflInterleaved,0,NULL,0,iflLowerLeftOrigin);
                   status = image->getTile(0,0,0,dims.x,dims.y,1,pixels,&config);
                   if ( status != iflOKAY )
                      {
                           // reset to initial state
                        delete [] pixels; pixels = NULL;
                        dims = iflSize();
                      }
                   image->close();
                 }
            }
       }

  public :

     Texture()
       : pixels(NULL), dims()
       {}

     Texture(const char * imagefile)
       : pixels(NULL), dims()
       {
         readTexture(imagefile);
       }

     Texture(const Texture& tex)
       : pixels(NULL), dims(tex.dims)
       {
         if (tex.pixels)
            {
              pixels = new unsigned char [dims.y*dims.x*dims.c];
              unsigned char * source, * dest;
              source = tex.pixels; dest = pixels;
              for (int i=0; i < dims.y*dims.x*dims.c; ++i)
                 {
                   *dest = *source;
                   dest++; source++;
                 }
            }
       }

     ~Texture()
       {
         delete [] pixels;
       }

     void operator = (const Texture& tex)
       {
         reset(); dims = tex.dims;
         if (tex.pixels)
            {
              pixels = new unsigned char [dims.y*dims.x*dims.c];
              unsigned char * source, * dest;
              source = tex.pixels; dest = pixels;
              for (int i=0; i < dims.y*dims.x*dims.c; ++i)
                 {
                   *dest = *source;
                   dest++; source++;
                 }
            }
       }

        // Delete existing texture stuff
     void reset(void)
       {
         delete [] pixels; pixels = NULL;
         dims = iflSize();
       }

     void readFrom(const char * imagefile)             // Read texture from a given file
       {
         reset();
         readTexture(imagefile);
       }

     bool isValid(void) const                          // Does this object have image data?
       {
         return ( pixels != NULL );
       }

     friend void glTexture(const Texture& tex)
       {
            // Do a glTexImage2D using the given texture
         if ( tex.pixels )
            {
              if ( tex.dims.c == 1 )
                 glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE, tex.dims.x, tex.dims.y, 0,
                              GL_LUMINANCE, GL_UNSIGNED_BYTE, tex.pixels);
              else if ( tex.dims.c == 2 )
                 glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE_ALPHA, tex.dims.x, tex.dims.y, 0,
                              GL_LUMINANCE_ALPHA, GL_UNSIGNED_BYTE, tex.pixels);
              else if ( tex.dims.c == 3 )
                 glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, tex.dims.x, tex.dims.y, 0,
                              GL_RGB, GL_UNSIGNED_BYTE, tex.pixels);
              else if ( tex.dims.c == 4 )
                 glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tex.dims.x, tex.dims.y, 0,
                              GL_RGBA, GL_UNSIGNED_BYTE, tex.pixels);
            }
       }

     friend void glTexImage2D(const Texture& tex)       // For consistency in notation
       {
         glTexture(tex);
       }
};

#endif /* #ifndef _TEXTURE_HH_ */


/*
  $Log: Texture.hh,v $
  Revision 2.3  2000/07/20 23:18:05  vinod
  Made reset() public

  Revision 2.2  2000/05/16 20:36:05  vinod
  Added typedef

  Revision 2.1  2000/04/30 01:19:57  vinod
  Texture class using the IFL for reading images

*/
