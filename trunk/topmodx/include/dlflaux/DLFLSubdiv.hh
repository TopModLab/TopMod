#ifndef _DLFLSUBDIV_H_
#define _DLFLSUBDIV_H_

#include <DLFLObject.hh>

namespace DLFL {

  void loopSubdivide( DLFLObjectPtr obj );
  void checkerBoardRemeshing(DLFLObjectPtr obj, double thickness=0.33);
  void simplestSubdivide( DLFLObjectPtr obj );
  void vertexCuttingSubdivide(DLFLObjectPtr obj, double offset=0.25);
  void pentagonalSubdivide2(DLFLObjectPtr obj, double scale_factor=0.75);
  void pentagonalSubdivide(DLFLObjectPtr obj, double offset=0);
  void honeycombSubdivide(DLFLObjectPtr obj);
  void dooSabinSubdivide(DLFLObjectPtr obj, bool check=true);
  void cornerCuttingSubdivide(DLFLObjectPtr obj);
  void modifiedCornerCuttingSubdivide(DLFLObjectPtr obj, double thickness);
  void root4Subdivide(DLFLObjectPtr obj, double a=0.0, double twist=0.0);  
  void catmullClarkSubdivide( DLFLObjectPtr obj );
  void starSubdivide(DLFLObjectPtr obj, double offset = 0.0);
  void sqrt3Subdivide( DLFLObjectPtr obj );
  void fractalSubdivide(DLFLObjectPtr obj, double offset = 1.0);
  void stellateSubdivide( DLFLObjectPtr obj );
  void twostellateSubdivide(DLFLObjectPtr obj, double offset, double curve);
  void setOldVertexType(DLFLObjectPtr obj);
  void setNewSubVertexType(DLFLObjectPtr obj);
  void setNormalVertexType(DLFLObjectPtr obj);
  void domeSubdivide(DLFLObjectPtr obj, double length, double sf);
  void dual1264Subdivide(DLFLObjectPtr obj, double sf);
  void loopStyleSubdivide(DLFLObjectPtr obj,double length);
  void dooSabinSubdivideBCNew(DLFLObjectPtr obj, double sf, double length);
  void dooSabinSubdivideBC(DLFLObjectPtr obj, bool check=true);

  // Face Subdivision
  void subdivideFace(DLFLObjectPtr obj, DLFLFacePtr faceptr, bool usequads=true);
  void subdivideFace(DLFLObjectPtr obj, uint face_index, bool usequads=true);
  void subdivideFaces(DLFLObjectPtr obj, DLFLFacePtrArray fparray, bool usequads=true);
  void subdivideFaces(DLFLObjectPtr obj, DLFLFacePtrList fplist, bool usequads=true);
  void subdivideAllFaces(DLFLObjectPtr obj, bool usequads=true);
} // end namespace

#endif // _DLFLSUBDIV_H_
