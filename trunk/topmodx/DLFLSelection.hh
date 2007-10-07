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

#ifndef _DLFLSELECTION_H_
#define _DLFLSELECTION_H_

#include <DLFLObject.hh>

using namespace DLFL;

/*!
	\file DLFLSelection.hh
	\brief Code for selection of vertices, edges and faces
	
	\see MainWindow
*/

void renderVerticesForSelect( DLFLObjectPtr obj ); 		//!< Render the vertices for selection
void renderEdgesForSelect( DLFLObjectPtr obj );				//!< Render the edges for selection
void renderFacesForSelect( DLFLObjectPtr obj );				//!< Render the faces for selection
void renderFaceVerticesForSelect( DLFLFacePtr fp );		//!< Render the face vertices of a face for selection

#endif // _DLFLSELECTION_H_
