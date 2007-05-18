/* $Id: NormalRenderer.hh,v 4.2 2004/04/18 18:20:35 vinod Exp $ */

#ifndef _NORMAL_RENDERER_HH_

#define _NORMAL_RENDERER_HH_

/*
  NormalRenderer
  A renderer for DLFL objects, derived from DLFLRenderer
  Renders with face-vertex normals, without material colors or face-vertex colors
*/

#include "DLFLRenderer.hh"

class NormalRenderer;
typedef NormalRenderer * NormalRendererPtr;

class NormalRenderer : public DLFLRenderer
{
  public :

        /* Default constructor */
     NormalRenderer()
       : DLFLRenderer()
       {}

        /* Copy constructor */
     NormalRenderer(const NormalRenderer& nr)
       : DLFLRenderer(nr)
       {}

        /* Assignment operator */
     NormalRenderer& operator = (const NormalRenderer& nr)
       {
         DLFLRenderer::operator = (nr);
         return (*this);
       }

        /* Destructor */
     virtual ~NormalRenderer()
       {}

        /* Implement render function */
     virtual int render(DLFLObjectPtr object) const
       {
         glEnable(GL_CULL_FACE);
         setCulling();
         glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
         glColor3f(0.25,0.25,0.55);
         object->plainRender();
         drawOverlays(object);
         glDisable(GL_CULL_FACE);
         return 0;
       }
};

#endif /* #ifndef _NORMAL_RENDERER_HH_ */

/*
  $Log: NormalRenderer.hh,v $
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

  Revision 2.1  2002/09/23 07:15:28  vinod
  DLFL Rendering classes

*/
