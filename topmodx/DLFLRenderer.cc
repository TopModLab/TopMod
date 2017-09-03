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
*
* Short description of this file
*
* name of .hh file containing function prototypes
*
*/

/* $Id: DLFLRenderer.cc,v 4.1 2004/04/18 18:09:06 vinod Exp $ */

// Non-inline function and static variable definitions for DLFLRenderer class

#include "DLFLRenderer.h"

// int DLFLRenderer::render_flags = 0;
bool DLFLRenderer::sReverseObject = false;
bool DLFLRenderer::sAntialiasing = false;
#ifdef GPU_OK
bool DLFLRenderer::sUseGPU = false;
#endif

QColor DLFLRenderer::sWireframeColor = QColor(0, 0, 0);			//!< wireframe RGB color
QColor DLFLRenderer::sSilhouetteColor = QColor(0, 0, 0);		//!< silhouette RGB color
QColor DLFLRenderer::sVertexColor = QColor(0, 0, 0);			//!< vertex RGB color
QColor DLFLRenderer::sFaceCentroidColor = QColor(0, 0, 0);		//!< face centroid RGB color
QColor DLFLRenderer::sNormalColor = QColor(0, 0, 0);			//!< normals RGB color
double DLFLRenderer::sFaceCentroidThickness = 5.0;				//!< face centroid point size
double DLFLRenderer::sNormalThickness = 5.0;					//!< normal line thickness
double DLFLRenderer::sNormalLength = 1.0;						//!< normal length
double DLFLRenderer::sWireframeThickness = 1.0;					//!< wireframe line thickness
double DLFLRenderer::sSilhouetteThickness = 8.0;				//!< silhouette line thickness
double DLFLRenderer::sVertexThickness = 1.5;					//!< vertex point size
