/*
*
* ***** BEGIN GPL LICENSE BLOCK *****
*
* This program is free software; you can redistribute it and/or
* modify it under the terms of the GNU General Public License
* as published by the Free Software Foundation; either version 2
* of the License, or (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program; if not, write to the Free Software  Foundation,
* Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
*
* The Original Code is Copyright (C) 2005 by xxxxxxxxxxxxxx
* All rights reserved.
*
* The Original Code is: all of this file.
*
* Contributor(s): none yet.
*
* ***** END GPL LICENSE BLOCK *****
*
* Short description of this file
*
* name of .hh file containing function prototypes
*
*/

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


