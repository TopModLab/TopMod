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

#ifndef _DLFLCONNECT_H_
#define _DLFLCONNECT_H_

// Non-inline function definitions for class DLFLObject
// Subroutines that are related to connections between edges/faces

#include <DLFLObject.hh>

namespace DLFL {

  void connectEdges( DLFLObjectPtr obj, DLFLEdgePtr eptr1, DLFLFacePtr fptr1,
		     DLFLEdgePtr eptr2, DLFLFacePtr fptr2, bool check=true);
  void connectEdgesWithoutLoopCheck( DLFLObjectPtr obj, DLFLEdgePtr eptr1, DLFLFacePtr fptr1,
				     DLFLEdgePtr eptr2, DLFLFacePtr fptr2, bool check=true);
  void connectFaces( DLFLObjectPtr obj, DLFLFaceVertexPtr fvptr1, DLFLFaceVertexPtr fvptr2);
  void dualConnectFaces( DLFLObjectPtr obj, DLFLFaceVertexPtr fvptr1, DLFLFaceVertexPtr fvptr2);
  void connectFaces( DLFLObjectPtr obj, DLFLFaceVertexPtr fvptr1, DLFLFaceVertexPtr fvptr2, 
		     int numsegs, int maxconn = -1 );
  void connectFaces( DLFLObjectPtr obj, DLFLFacePtr fp1, DLFLFacePtr fp2, int numsegs = 1);
  void hermiteConnectFaces( DLFLObjectPtr obj, DLFLFaceVertexPtr fvptr1, DLFLFaceVertexPtr fvptr2,
			    int numsegs, double wt1, double wt2, int maxconn=-1, int numtwists=0);
  void bezierConnectFaces( DLFLObjectPtr obj, DLFLFaceVertexPtr fvptr1, DLFLFaceVertexPtr fvptr2,
			   int numsegs, double wt1, double wt2);
} // end namespace

#endif // _DLFLCONNECT_H_
