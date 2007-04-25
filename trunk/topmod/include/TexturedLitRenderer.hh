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

class TexturedLitRenderer : public TexturedRenderer
{
  public :

        /* Default constructor */
     TexturedLitRenderer()
       : TexturedRenderer()
       {}

        /* Copy constructor */
     TexturedLitRenderer(const TexturedLitRenderer& tlr)
       : TexturedRenderer(tlr)
       {}

        /* Construct using given image file as texture */
     TexturedLitRenderer(const char * filename)
       : TexturedRenderer(filename)
       {}

        /* Assignment operator */
     TexturedLitRenderer& operator = (const TexturedLitRenderer& tlr)
       {
         TexturedRenderer::operator = (tlr);
         return (*this);
       }

        /* Destructor */
     virtual ~TexturedLitRenderer()
       {}

        /* Implement render function */
     virtual int render(DLFLObjectPtr object) const
       {
         if ( isValid() == false ) return -1;
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

/*
  $Log: TexturedLitRenderer.hh,v $
  Revision 4.2  2004/04/18 18:20:35  vinod
  Minor change. Culling mode is now disabled *after* overlays are drawn

  Revision 4.1  2004/04/18 18:09:06  vinod
  Enabled culling when rendering. Added option to reverse object when rendering.

  Revision 4.0  2003/12/26 01:58:53  vinod
  Major version sync.

  Revision 3.0  2003/09/19 16:06:47  vinod
  Major version update

  Revision 2.4  2002/10/30 04:45:29  vinod
  Added support for loading JPG and PNG texture files using Fl_Image classes

  Revision 2.3  2002/10/22 21:26:56  vinod
  Added typedefs

  Revision 2.2  2002/10/06 06:10:15  vinod
  Added check for negative texture_id in render

  Revision 2.1  2002/09/23 07:15:28  vinod
  DLFL Rendering classes

*/

