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

  void createConvexHull( DLFLObjectPtr obj );

  void createDualConvexHull( DLFLObjectPtr obj );
  DLFLObjectPtr createDualConvexHull( DLFLObjectPtr obj, const Vector3dArray &ovarray);

  void peelByPlane( DLFLObjectPtr obj, Vector3d normal,Vector3d P0);

  void performCutting( DLFLObjectPtr obj, int type,float offsetE,float offsetV,bool global,bool selected) ;
} // end namespace
