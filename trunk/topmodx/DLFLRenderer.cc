/* $Id: DLFLRenderer.cc,v 4.1 2004/04/18 18:09:06 vinod Exp $ */

// Non-inline function and static variable definitions for DLFLRenderer class

#include "DLFLRenderer.hh"

// int DLFLRenderer::render_flags = 0;
bool DLFLRenderer::reverse_object = false;
bool DLFLRenderer::antialiasing = false;
#ifdef GPU_OK
bool DLFLRenderer::useGPU = false;
#endif

QColor DLFLRenderer::mWireframeColor = QColor(0,0,0);									//!< wireframe RGB color
QColor DLFLRenderer::mSilhouetteColor = QColor(0,0,0);								//!< silhouette RGB color
QColor DLFLRenderer::mVertexColor = QColor(0,0,0);										//!< vertex RGB color
QColor DLFLRenderer::mFaceCentroidColor = QColor(0,0,0);							//!< face centroid RGB color
QColor DLFLRenderer::mNormalColor = QColor(0,0,0);										//!< normals RGB color
double DLFLRenderer::mFaceCentroidThickness = 5.0;					//!< face centroid point size
double DLFLRenderer::mNormalThickness = 5.0;								//!< normal line thickness
double DLFLRenderer::mNormalLength = 1.0;										//!< normal length 
double DLFLRenderer::mWireframeThickness = 1.0;							//!< wireframe line thickness
double DLFLRenderer::mSilhouetteThickness = 8.0;						//!< silhouette line thickness
double DLFLRenderer::mVertexThickness = 1.5;								//!< vertex point size
