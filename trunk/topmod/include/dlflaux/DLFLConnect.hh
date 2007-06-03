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
