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

/**
 * \file DLFLCore.hh
 */

#ifndef _DLFLCORE_H_
#define _DLFLCORE_H_

#include "DLFLObject.hh"

namespace DLFL {

  /***************
   * Insert Edge *
   ***************/

  int insertEdge( DLFLObjectPtr obj, 
									uint &faceId1, uint &vertId1,
									uint &faceId2, uint &vertId2,
									uint &faceId1b, uint &vertId1b,
									uint &faceId2b, uint &vertId2b,
									bool check,
									bool set_type = false );

  /**
   * The general case insertEdge subroutine. Calls one of the insertEdge implementations, 
   * depending on whether the corners are cofacial or not. If the 2 corners are cofacial
   * checks to see if the 2 pointers refer to the same corner, if so doesn't do insert.
   */
  DLFLEdgePtr insertEdge( DLFLObjectPtr obj, DLFLFaceVertexPtr fvptr1, DLFLFaceVertexPtr fvptr2, bool set_type = false, DLFLMaterialPtr matl = NULL  );
  DLFLEdgePtr insertEdgeCoFacial( DLFLObjectPtr obj, DLFLFaceVertexPtr fvptr1, DLFLFaceVertexPtr fvptr2, bool set_type = false );
  DLFLEdgePtr insertEdgeNonCoFacial( DLFLObjectPtr obj, DLFLFaceVertexPtr fvptr1, DLFLFaceVertexPtr fvptr2, DLFLMaterialPtr matl = NULL );
  DLFLEdgePtr insertEdgeWithoutCheck( DLFLObjectPtr obj, DLFLFaceVertexPtr fvptr1, DLFLFaceVertexPtr fvptr2, bool set_type = false, DLFLMaterialPtr matl = NULL );
  
  /***************
   * Delete Edge *
   ***************/

  // If the cleanup flag is true, any point-spheres created
  // because of the edge deletion will be removed from the object
	std::vector<int>  deleteEdgeID( DLFLObjectPtr obj, uint edgeId, bool cleanup = true );
  void deleteEdge( DLFLObjectPtr obj, uint edge_index, bool cleanup = true );
  DLFLFacePtrArray deleteEdge( DLFLObjectPtr obj, DLFLEdgePtr edgeptr, bool cleanup = true );
  
  /*****************
   * Collapse Edge *
   *****************/

  int  collapseEdgeID( DLFLObjectPtr obj, const uint edgeId, bool cleanup = true );
  void collapseEdge( DLFLObjectPtr obj, uint edge_index, bool cleanup = true );
  DLFLVertexPtr collapseEdge( DLFLObjectPtr obj, DLFLEdgePtr edgeptr, bool cleanup = true );

  /******************
   * Subdivide Edge *
   ******************/

  // Return pointer to the newly added vertex 
  int subdivideEdgeID( DLFLObjectPtr obj, uint edgeId, bool set_type = false );
  DLFLVertexPtr subdivideEdge( DLFLObjectPtr obj, DLFLEdgePtr edgeptr, bool set_type = false );
  DLFLVertexPtr subdivideEdge( DLFLObjectPtr obj, uint edge_index );

  // Subdivide into multiple edges
  vector<int> subdivideEdgeID( DLFLObjectPtr obj, int num_divs, uint edgeId, bool set_type = false );
  DLFLVertexPtrArray subdivideEdge( DLFLObjectPtr obj, int num_divs, DLFLEdgePtr edgeptr, bool set_type = false );
  void subdivideEdge( DLFLObjectPtr obj, int num_divs, uint edge_index );

  // Trisect an edge - new points will be calculated based on scale factor
  // with respect to mid point of edge OR offset from end points
  // Boolean flag indicates if specified number is a scale factor or offset distance
  // Offset distance is clamped to lie between 0.01 and edgelength/2 - 0.01
  // Scale factor is clamped to lie between 0.01 and 0.99 (inclusive)
  void trisectEdge( DLFLObjectPtr obj, DLFLEdgePtr edgeptr, double num, bool scale, bool set_type );
  
  // Subdivide all edges in the object
  void subdivideAllEdges( DLFLObjectPtr obj, int num_divs = 2, bool set_type = false );

  // Subdivide all edges in the object which are not self loops
  void subdivideNonLoopingEdges( DLFLObjectPtr obj, int num_divs = 2, bool set_type = false );

  // Split valence 2 vertices into 2 vertices
  void splitValence2Vertices( DLFLObjectPtr obj, double offset = -0.1 );

  // Splice two corners
  // Insert edge and then collapse edge
  void spliceCorners( DLFLObjectPtr obj, DLFLFaceVertexPtr fvptr1, DLFLFaceVertexPtr fvptr2 );

  /** 
   * If both sides of an edge are co-planar, the edge will be removed.
   * First version looks at all edges in the object.
   * Second and third version looks at the specified list/array of edges 
   */
  void edgeCleanup( DLFLObjectPtr obj );
  void edgeCleanup( DLFLObjectPtr obj, const DLFLEdgePtrList& edges );
  void edgeCleanup( DLFLObjectPtr obj, const DLFLEdgePtrArray& edges );

  void cleanup2gons( DLFLObjectPtr obj );
  /**
   * Cleanup Valence 2 vertices (winged vertices)
   * Removes all winged vertices
   */
  void cleanupWingedVertices( DLFLObjectPtr obj );

   /***********************
    * Read In Object Data *
    ***********************/
  
  /* Create a DLFL object from an input stream which should contain
   * an object in OBJ format */
  DLFLObject* readObjectFile( char* filename, char *mtlfilename =NULL);
  bool writeObjectFile( DLFLObject *obj, char* filename = NULL, char *mtlfilename=NULL );

	/************************
	 * Create/Remove Vertex *
	 ************************/

	uint* createVertex( double x, double y, double z, DLFLObjectPtr &obj, bool set_type = false );
	void removeVertex( const DLFLObjectPtr &obj, uint vertId, uint faceId );

} // end namespace DLFL

#endif // _DLFLCORE_H_
