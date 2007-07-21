#ifndef _DLFL_LIGHTING_HH_
#define _DLFL_LIGHTING_HH_

// Subroutines for lighting computations in DLFL

#include "TMPatchObject.hh"
#include <DLFLObject.hh>

#include <QApplication>
#include <QProgressDialog>

#include "CgData.hh"

#ifdef GPU_OK
using namespace Cg;
#endif // GPU_OK

void computeLighting( DLFLFacePtr fp, LightPtr lightptr, bool usegpu = false);
void computeLighting( DLFLObjectPtr obj, TMPatchObjectPtr po, LightPtr lightptr, bool usegpu = false );

#endif /* #ifndef _DLFL_LIGHTING_HH_ */

