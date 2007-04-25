/* $Id$ */

#ifndef _TM_PATCH_OBJECT_HH_

#define _TM_PATCH_OBJECT_HH_

/*
  Class for a TopMod object rendered using Bezier patches. Uses TMPatchFace to render faces
  Derived from DLFLObject. Adds patch rendering methods to DLFLObject
*/

#include "DLFLObject.hh"
#include "TMPatchFace.hh"

class TMPatchObject : public DLFLObject
{
  protected :

     TMPatchFacePtrList patch_list;		// List of patch faces
     int patchsize;				 // Size of each patch

  public :

	// Default constructor
     TMPatchObject()
       : DLFLObject(), patch_list(), patchsize(0)
     {}

  private :
     
	// Copy constructor - private to prevent access
     TMPatchObject(const TMPatchObject& tmpo)
       : DLFLObject(tmpo), patch_list(), patchsize(tmpo.patchsize)
     {}

	// Assignment operator - private to prevent access
     void operator = (const TMPatchObject& tmpo)
     {}
       
  public :
     
	// Destructor
     ~TMPatchObject()
     {}

  protected :

	// Free the memory allocated for the patches
     void destroyPatches()
       {
         TMPatchFacePtrList :: iterator first = patch_list.begin(), last = patch_list.end();
         TMPatchFacePtr pfp = NULL;
         while ( first != last )
            {
              pfp = (*first); ++first;
              delete pfp;
            }
       }

	// Build the list of patch faces
     void createPatches()
     {
       destroyPatches();
       DLFLFacePtrList :: iterator ffirst = face_list.begin(), flast = face_list.end();
       DLFLFacePtr fp = NULL;
       TMPatchFacePtr pfp = NULL;
       
       while ( ffirst != flast )
	  {
	    fp = (*ffirst); ++ffirst;
	    pfp = new TMPatchFacePtr(patchsize);
	    pfp->setDLFLFace(fp); pfp->createPatches();
	    patch_list.push_back(pfp);
	  }
     }

  public :

	// Set the patch size
     void setPatchSize(int size)
     {
       if ( size != patchsize && size > 0 )
	  {
	    patchsize = size;
	    createPatches();
	  }
     }

	// Update the patches
     void updatePatches()
     {
       createPatches();
     }

        // Render the patches
     void renderPatches(void) const
       {
         glPushMatrix();
         transform();
         for_each(patch_list.begin(), patch_list.end(), render);
         glPopMatrix();
       }

        // Render the object using wireframe patches
     void renderWireframePatches(void) const
       {
         glPushMatrix();
         transform();
         for_each(face_list.begin(), face_list.end(), outline);
         glPopMatrix();
       }

        // Render the object using point patches
     void renderPointPatches(void) const
       {
         glPushMatrix();
         transform();
         for_each(face_list.begin(), face_list.end(), controlpoints);
         glPopMatrix();
       }

        // Compute lighting for patches and the base object
     void computeLighting(LightPtr lightptr)
       {
         cout << "TMPatchObject::computeLighting" << endl;
         DLFLObject::computeLighting(lightptr);
         TMPatchFacePtrList :: iterator first = patch_list.begin(), last = patch_list.end();
         TMPatchFacePtr pfp = NULL;
         while ( first != last )
            {
              pfp = (*first); ++first;
              pfp->computeLighting(lightptr);
            }
       }
};


#endif /* #ifndef _TM_PATCH_OBJECT_HH_ */


/*
  $Log$
*/
