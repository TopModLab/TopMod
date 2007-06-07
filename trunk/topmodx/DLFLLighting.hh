#ifndef _DLFL_LIGHTING_HH_
#define _DLFL_LIGHTING_HH_

// Subroutines for lighting computations in DLFL

#include "TMPatchObject.hh"
#include <DLFLObject.hh>

void computeLighting( DLFLFacePtr fp, LightPtr lightptr );
void computeLighting( DLFLObjectPtr obj, TMPatchObjectPtr po, LightPtr lightptr );

#endif /* #ifndef _DLFL_LIGHTING_HH_ */

