#ifndef _DLFLEXTRUDE_H_
#define _DLFLEXTRUDE_H_

#include <DLFLCoreExt.hh>
#include <DLFLObject.hh>

namespace DLFL {

    DLFLFacePtr duplicateFace(DLFLObjectPtr obj, DLFLFacePtr fptr, double offset, double rot, double sf);
    DLFLFacePtr duplicateFace(DLFLObjectPtr obj, DLFLFacePtr fptr, const Vector3d& dir, double offset, double rot, double sf);

    DLFLFacePtr duplicateFacePlanarOffset(DLFLObjectPtr obj, DLFLFacePtr fptr, double offset, double rot, double thickness, bool fractionalthickness);
    DLFLFacePtr duplicateFacePlanarOffset(DLFLObjectPtr obj, DLFLFacePtr fptr, const Vector3d& dir, double offset, double rot, double thickness, bool fractionalthickness);

    DLFLFacePtr extrudeFace(DLFLObjectPtr obj, DLFLFacePtr fptr, double d);
    DLFLFacePtr extrudeFace(DLFLObjectPtr obj, DLFLFacePtr fptr, double d, int num);
    DLFLFacePtr extrudeFace(DLFLObjectPtr obj, DLFLFacePtr fptr, double d, double rot, double sf = 1.0);
    DLFLFacePtr extrudeFace(DLFLObjectPtr obj, DLFLFacePtr fptr, double d, int num, double rot, double sf = 1.0);
    DLFLFacePtr extrudeFace(DLFLObjectPtr obj, DLFLFacePtr fptr, double d, const Vector3d& dir);
    DLFLFacePtr extrudeFace(DLFLObjectPtr obj, DLFLFacePtr fptr, double d, const Vector3d& dir, int num);
    DLFLFacePtr extrudeFace(DLFLObjectPtr obj, DLFLFacePtr fptr, double d, const Vector3d& dir, double rot, double sf = 1.0);
    DLFLFacePtr extrudeFace(DLFLObjectPtr obj, DLFLFacePtr fptr, double d, const Vector3d& dir, int num, double rot, double sf = 1.0);

    DLFLFacePtr extrudeFacePlanarOffset(DLFLObjectPtr obj, DLFLFacePtr fptr, double d, double rot, double thickness, bool fractionalthickness);
    DLFLFacePtr extrudeFacePlanarOffset(DLFLObjectPtr obj, DLFLFacePtr fptr, double d, const Vector3d& dir, double rot, double thickness, bool fractionalthickness);

    DLFLFacePtr extrudeFaceDS(DLFLObjectPtr obj, DLFLFacePtr fptr, double d, double twist = 0.0, double sf = 1.0);
    DLFLFacePtr extrudeFaceDS(DLFLObjectPtr obj, DLFLFacePtr fptr, double d, int num, double twist = 0.0, double sf = 1.0);
    DLFLFacePtr extrudeFaceDS(DLFLObjectPtr obj, DLFLFacePtr fptr, double d, const Vector3d& dir, double twist = 0.0, double sf = 1.0);
    DLFLFacePtr extrudeFaceDS(DLFLObjectPtr obj, DLFLFacePtr fptr, double d, const Vector3d& dir, int num, double twist = 0.0, double sf = 1.0);

    DLFLFacePtr extrudeDualFace(DLFLObjectPtr obj, DLFLFacePtr fptr, double d, double rot=0.0, double sf=1.0, bool mesh=false);
    DLFLFacePtr extrudeDualFace(DLFLObjectPtr obj, DLFLFacePtr fptr, double d, int num, double rot=0.0, double sf=1.0, bool mesh=false);
    DLFLFacePtr extrudeDualFace(DLFLObjectPtr obj, DLFLFacePtr fptr, double d, const Vector3d& dir, double rot=0.0, double sf=1.0, bool mesh=false);
    DLFLFacePtr extrudeDualFace(DLFLObjectPtr obj, DLFLFacePtr fptr, double d, const Vector3d& dir, int num, double rot=0.0, double sf=1.0, bool mesh=false);
    
    void stellateFace(DLFLObjectPtr obj, DLFLFacePtr fptr, double d);
    void stellateFace(DLFLObjectPtr obj, DLFLFacePtr fptr, double d, const Vector3d& dir);

    void doubleStellateFace(DLFLObjectPtr obj, DLFLFacePtr fptr, double d);

    DLFLFacePtr extrudeFaceDodeca(DLFLObjectPtr obj, DLFLFacePtr fptr, double d, int num, double rot=0.0, double sf = 1.0, bool hexagonalize=false);

    DLFLFacePtr extrudeHexTileFace(DLFLObjectPtr obj, DLFLFacePtr fptr, double d, double rot=0.0, double sf = 1.0);

    DLFLFacePtr extrudeFaceDodeca(DLFLObjectPtr obj, DLFLFacePtr fptr, double d, double rot=0.0, double sf = 1.0);

    DLFLFacePtr extrudeFaceIcosa(DLFLObjectPtr obj, DLFLFacePtr fptr, double d, int num, double rot=0.0,  double sf = 1.0);

    DLFLFacePtr extrudeFaceIcosa(DLFLObjectPtr obj, DLFLFacePtr fptr, double d, double rot, double sf = 1.0);
    void extrudeFace(DLFLObjectPtr obj, DLFLFacePtr fptr);


} // end namespace DLFL

#endif // _DLFLEXTRUDE_H_
