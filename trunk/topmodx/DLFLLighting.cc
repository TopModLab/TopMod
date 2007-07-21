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
		
		#ifdef GPU_OK
		if (usegpu){
			fvcolor = RGBColor(0,0,0);
		}
		else {
    	fvcolor = lightptr->illuminate(pos,normal)*Kd;
	    fvcolor += (1.0-Kd)*basecolor;
		}
		#else
  	fvcolor = lightptr->illuminate(pos,normal)*Kd;
    fvcolor += (1.0-Kd)*basecolor;		
		#endif
    current->color = fvcolor;

    current = current->next();
    while ( current != fp->front() ) {
      normal = current->getNormal();
      pos = current->getVertexCoords();

      #ifdef GPU_OK
			if (usegpu){
				// cgSetParameter3f(CgData::instance()->Kd, basecolor.r, basecolor.g, basecolor.b);
				fvcolor = RGBColor(0,0,0);
			}
			else {
	    	fvcolor = lightptr->illuminate(pos,normal)*Kd;
		    fvcolor += (1.0-Kd)*basecolor;
			}
			#else
    	fvcolor = lightptr->illuminate(pos,normal)*Kd;
	    fvcolor += (1.0-Kd)*basecolor;
			#endif
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
	// int patchsize = (po)?po->list().size():0;
	// 
	// QProgressDialog *progress = new QProgressDialog("Computing Lighting...", "Cancel", 0, obj->num_faces() + patchsize);
	// progress->setMinimumDuration(4000);
	// progress->setWindowModality(Qt::WindowModal);
	// int progressvalue = 0;
	
  DLFLFacePtrList::iterator first, last;
  DLFLFacePtr faceptr;
  first = obj->beginFace(); last = obj->endFace();
  while ( first != last ) {
		// progress->setValue(progressvalue++);
		// QApplication::processEvents();
		
    faceptr = (*first);
    computeLighting(faceptr,lightptr, usegpu);
    ++first;
  }
  if( po ) {
    TMPatchFacePtrList patch_list = po->list( );
    TMPatchFacePtrList::iterator pfirst = patch_list.begin(), plast = patch_list.end();
    TMPatchFacePtr pfp = NULL;
    while ( pfirst != plast ) {
			// progress->setValue(progressvalue++);
			// QApplication::processEvents();
	
      pfp = (*pfirst); ++pfirst;
      pfp->computeLighting(lightptr);
    }
  }
	// progress->setValue(obj->num_faces() + patchsize);
}


