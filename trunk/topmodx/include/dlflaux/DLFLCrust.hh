#ifndef _DLFLCRUST_H_
#define _DLFLCRUST_H_

#include <DLFLObject.hh>

namespace DLFL {

  static DLFLFacePtrArray crustfp1;
  static DLFLFacePtrArray crustfp2;
  static int crust_num_old_faces;
  static int crust_min_face_id;

  static DLFLVertexPtrArray crustvp1;
  static DLFLVertexPtrArray crustvp2;
  static int crust_min_vert_id;
  static int crust_num_old_verts;

  /*
    Create a crust for this object.
    Creates an inner (outer if thickness is negative) surface
    duplicating the existing surface and moving every vertex along
    the average normal at the vertex for the given distance.
    Boolean flag indicates if crust should be of uniform thickness
    which means thickness at vertices will be adjusted to account
    for normal averaging.
  */
  void createCrust(DLFLObjectPtr obj, double thickness, bool uniform = true);

  /*
    Create a crust for this object.
    Creates an inner (outer if scale_factor is negative) surface
    duplicating the existing surface and scaling the inner or outer surface
    w.r.t centroid of object.
  */
  void createCrustWithScaling(DLFLObjectPtr obj, double scale_factor = 0.9 );
  void cmMakeHole(DLFLObjectPtr obj, DLFLFacePtr fp, bool cleanup = true );
  void createCrustForWireframe(DLFLObjectPtr obj, double thickness = 0.1 );
  void createWireframeWithSegments(DLFLObjectPtr obj, double thickness = 0.1, int numSides = 4);
  void tagMatchingFaces(DLFLObjectPtr obj, DLFLFacePtr fptr);
	void selectMatchingFaces(DLFLObjectPtr obj, DLFLFacePtr fptr, DLFLFacePtrArray &fparray);
  void punchHoles(DLFLObjectPtr obj);
  void makeWireframe(DLFLObjectPtr obj, double crust_thickness = 0.1, bool split = true );
  void makeWireframeWithColumns(DLFLObjectPtr obj, double wireframe_thickness, int wireframe_segments);

} // end namespace

#endif // _DLFLCRUST_H_
