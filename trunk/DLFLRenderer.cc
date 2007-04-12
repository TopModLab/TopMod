/* $Id: DLFLRenderer.cc,v 4.1 2004/04/18 18:09:06 vinod Exp $ */

// Non-inline function and static variable definitions for DLFLRenderer class

#include "DLFLRenderer.hh"

int DLFLRenderer::render_flags = 0;
bool DLFLRenderer::reverse_object = false;

/*
  $Log: DLFLRenderer.cc,v $
  Revision 4.1  2004/04/18 18:09:06  vinod
  Enabled culling when rendering. Added option to reverse object when rendering.

  Revision 4.0  2003/12/26 01:58:53  vinod
  Major version sync.

  Revision 3.1  2003/10/25 22:33:15  vinod
  Static variable definition for DLFLRenderer class

*/
