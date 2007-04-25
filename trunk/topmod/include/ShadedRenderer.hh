/* $Id: ShadedRenderer.hh,v 4.2 2004/04/18 18:20:35 vinod Exp $ */

#ifndef _SHADED_RENDERER_HH_

#define _SHADED_RENDERER_HH_

/*
  ShadedRenderer
  A renderer for DLFL objects, derived from DLFLRenderer
  Renders with face-vertex normals, with material colors
*/

#include "DLFLRenderer.hh"

class ShadedRenderer;
typedef ShadedRenderer * ShadedRendererPtr;

class ShadedRenderer : public DLFLRenderer
{
  public :

        /* Default constructor */
     ShadedRenderer()
       : DLFLRenderer()
       {}

        /* Copy constructor */
     ShadedRenderer(const ShadedRenderer& nr)
       : DLFLRenderer(nr)
       {}

        /* Assignment operator */
     ShadedRenderer& operator = (const ShadedRenderer& nr)
       {
         DLFLRenderer::operator = (nr);
         return (*this);
       }

        /* Destructor */
     virtual ~ShadedRenderer()
       {}

        /* Implement render function */
     virtual int render(DLFLObjectPtr object) const
       {
         glEnable(GL_CULL_FACE);
         setCulling();
         glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
         object->render();
         drawOverlays(object);
         glDisable(GL_CULL_FACE);
         return 0;
       }
};

#endif /* #ifndef _SHADED_RENDERER_HH_ */

/*
  $Log: ShadedRenderer.hh,v $
  Revision 4.2  2004/04/18 18:20:35  vinod
  Minor change. Culling mode is now disabled *after* overlays are drawn

  Revision 4.1  2004/04/18 18:09:06  vinod
  Enabled culling when rendering. Added option to reverse object when rendering.

  Revision 4.0  2003/12/26 01:58:53  vinod
  Major version sync.

  Revision 3.0  2003/09/19 16:06:47  vinod
  Major version update

  Revision 2.2  2002/10/22 21:26:56  vinod
  Added typedefs

  Revision 2.1  2002/10/08 05:20:16  vinod
  DLFLRendering classes

*/
