#include "DLFLLighting.hh"

void computeLighting( DLFLFacePtr fp, LightPtr lightptr ) {
  if ( fp->front() ) {
      double Ka = fp->material()->Ka;
      double Kd = fp->material()->Kd;
      double Ks = fp->material()->Ks;
      RGBColor basecolor = fp->material()->color;
      RGBColor fvcolor;
      Vector3d normal, pos;

      DLFLFaceVertexPtr current = fp->front();
      normal = current->getNormal();
      pos = current->getVertexCoords();

      fvcolor = lightptr->illuminate(pos,normal)*Kd;
      // fvcolor *= Kd; 
      fvcolor += (1.0-Kd)*basecolor;

      current->color = fvcolor;

      current = current->next();
      while ( current != fp->front() ) {
	  normal = current->getNormal();
	  pos = current->getVertexCoords();

	  fvcolor = lightptr->illuminate(pos,normal);
	  fvcolor *= Kd;
	  fvcolor += (1.0-Kd)*basecolor;

	  current->color = fvcolor;

	  current = current->next();
	}
    }
}

void computeLighting(DLFLObjectPtr obj, TMPatchObjectPtr po, LightPtr lightptr) {
  DLFLFacePtrList::iterator first, last;
  DLFLFacePtr faceptr;
  first = obj->beginFace(); last = obj->endFace();
  while ( first != last ) {
    faceptr = (*first);
    computeLighting(faceptr,lightptr);
    ++first;
  }

  if( po ) {
    TMPatchFacePtrList patch_list = po->list( );
    TMPatchFacePtrList::iterator pfirst = patch_list.begin(), plast = patch_list.end();
    TMPatchFacePtr pfp = NULL;
    while ( pfirst != plast ) {
      pfp = (*pfirst); ++pfirst;
      pfp->computeLighting(lightptr);
    }
  }
}


