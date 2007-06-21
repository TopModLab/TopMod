/* $Id: DLFLRenderer.cc,v 4.1 2004/04/18 18:09:06 vinod Exp $ */

// Non-inline function and static variable definitions for DLFLRenderer class

#include "DLFLRenderer.hh"

// int DLFLRenderer::render_flags = 0;
bool DLFLRenderer::reverse_object = false;
bool DLFLRenderer::antialiasing = false;
#ifdef GPU_OK
bool DLFLRenderer::useGPU = false;
#endif
