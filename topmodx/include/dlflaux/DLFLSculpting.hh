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

#include <DLFLObject.hh>
#include <DLFLCore.hh>
#include <DLFLCoreExt.hh>
#include "DLFLConvexHull.hh"

namespace DLFL {

  static int s = 0;
  static DLFLConvexHull * convexhull = NULL;

  //new changes; these needs to be declared outside the function for local-cut
  static DLFLVertexPtr * sverts;
  static DLFLEdgePtr * sedges;
  static int cutcount;
	static DLFLFaceVertexPtr * newcorners;
	static DLFLEdgePtr * edges2del;
	// DLFLVertexPtr * sverts;
	// DLFLEdgePtr * sedges;
	// int cutcount;
	static int e2dsize, ncsize;
	
  void createConvexHull( DLFLObjectPtr obj );

  void createDualConvexHull( DLFLObjectPtr obj );
  DLFLObjectPtr createDualConvexHull( DLFLObjectPtr obj, const Vector3dArray &ovarray);

  void peelByPlane( DLFLObjectPtr obj, Vector3d normal,Vector3d P0);
	void localCut(DLFLObjectPtr obj, DLFLVertexPtr vp,Vector3d normal,Vector3d P0);
		
  void performCutting( DLFLObjectPtr obj, int type,float offsetE,float offsetV,bool global,bool selected) ;
	void cutSelectedFaces( DLFLObjectPtr obj, float offsetE,float offsetV,bool global=false,bool selected=false);
	void cutSelectedEdges( DLFLObjectPtr obj, float offsetE,float offsetV,bool global=false,bool selected=false);
	void cutSelectedVertices( DLFLObjectPtr obj, float offsetE,float offsetV,bool global=false,bool selected=false);
	
	int isMarked(DLFLVertexPtr vp);
	void autoMarkEdges(DLFLObjectPtr obj);
	int getCutIndex(DLFLVertexPtr vp);
	int getCutIndex(DLFLEdgePtr ep);
	
} // end namespace
