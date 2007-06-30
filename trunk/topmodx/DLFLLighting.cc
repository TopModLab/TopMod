#include "DLFLLighting.hh"

void computeLighting( DLFLFacePtr fp, LightPtr lightptr, bool usegpu ) {
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

		if (usegpu){
		  // cgSetParameter3f(CgData::instance()->Ka, Ka, Ka, Ka);
		  // cgSetParameter3f(CgData::instance()->Kd, Kd, Kd, Kd);
		  // cgSetParameter3f(CgData::instance()->Ks, Ks, Ks, Ks);
		  // cgSetParameter1f(CgData::instance()->shininess, 50);
			fvcolor = RGBColor(0,0,0);
		}
		else {
			// cgSetParameter3f(CgData::instance()->Ka, 0.0, 0.0, 0.0);
			// 		  cgSetParameter3f(CgData::instance()->Kd, 0.0, 0.0, 0.0);
			// 		  cgSetParameter3f(CgData::instance()->Ks, 0.0, 0.0, 0.0);
			// 		  cgSetParameter1f(CgData::instance()->shininess, 0);
		  
    	fvcolor = lightptr->illuminate(pos,normal)*Kd;
	    fvcolor += (1.0-Kd)*basecolor;
		}
    // fvcolor *= Kd; 
    current->color = fvcolor;

    current = current->next();
    while ( current != fp->front() ) {
      normal = current->getNormal();
      pos = current->getVertexCoords();

      if (usegpu){
				// cgSetParameter3f(CgData::instance()->Kd, basecolor.r, basecolor.g, basecolor.b);
				fvcolor = RGBColor(0,0,0);
				// fvcolor = basecolor;
			}
			else {
	    	fvcolor = lightptr->illuminate(pos,normal)*Kd;
	    	// fvcolor *= Kd;
		    fvcolor += (1.0-Kd)*basecolor;
			}
    	current->color = fvcolor;

			// current->color = RGBColor(((double)rand() / ((double)(RAND_MAX)+(double)(1)) ),
			// 													((double)rand() / ((double)(RAND_MAX)+(double)(1)) ),
			// 													((double)rand() / ((double)(RAND_MAX)+(double)(1)) )
			// 													);
      current = current->next();
    }
  }
}

void computeLighting(DLFLObjectPtr obj, TMPatchObjectPtr po, LightPtr lightptr, bool usegpu) {
		// std::cout<< "usegpu = " << usegpu << "\n";
		
	  DLFLFacePtrList::iterator first, last;
	  DLFLFacePtr faceptr;
	  first = obj->beginFace(); last = obj->endFace();
	  while ( first != last ) {
	    faceptr = (*first);
	    computeLighting(faceptr,lightptr, usegpu);
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


