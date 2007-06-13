#ifndef _TM_PATCH_OBJECT_HH_
#define _TM_PATCH_OBJECT_HH_

#include "TMPatchFace.hh"

class TMPatchObject;
typedef TMPatchObject* TMPatchObjectPtr;

class TMPatchObject {
protected :

  uint uid;
  TMPatchFacePtrList patch_list;		// List of patch faces
  TMPatchFacePtrList::iterator it;
  int patchsize;				 // Size of each patch

public :

  // Default constructor
  TMPatchObject( uint id ) : uid(id), patch_list(), patchsize(0) { }

  uint id( ) { return uid; };
  int size( ) { return patchsize; };
  const TMPatchFacePtrList& list( ) { return patch_list; };
  void for_each( void (TMPatchFace::*func)(void));

private :
     
  // Copy constructor - private to prevent access
  TMPatchObject(const TMPatchObject& tmpo)
    : patch_list(), patchsize(tmpo.patchsize) { }

  // Assignment operator - private to prevent access
  void operator=(const TMPatchObject& tmpo) { }
       
public :     
  // Destructor
  ~TMPatchObject() { destroyPatches(); }

protected :
  DLFLObjectPtr mObj; // the last obj created from

  // Free the memory allocated for the patches
  void destroyPatches();
  // Build the list of patch faces
  void createPatches( DLFLObjectPtr obj );

public :

  // Set the patch size
  void setPatchSize(int size, DLFLObjectPtr obj = NULL );

  void updateForPatches( DLFLObjectPtr obj );

  // Update the patches
  void updatePatches( DLFLObjectPtr obj = NULL ) {
    if( !obj ) { obj = mObj; }
    if( !obj ) { return; } // never set an obj to update
    updateForPatches(obj);
    createPatches(obj);
  }

  // Render the patches
  void renderPatches(void) {
    glPushMatrix();
    ///transform();
    for_each(&TMPatchFace::render);
    glPopMatrix();
  }

  // Render the object using wireframe patches
  void renderWireframePatches(void) {
    glPushMatrix();
    ///transform();
    //for_each(face_list.begin(), face_list.end(), outline);
    for_each(&TMPatchFace::outline);
    glPopMatrix();
  }

  // Render the object using point patches
  void renderPointPatches(void) {
    glPushMatrix();
    ///transform();
    //for_each(face_list.begin(), face_list.end(), controlpoints);
    for_each(&TMPatchFace::controlpoints);
    glPopMatrix();
  }

  void renderPatchBoundaries(void) {
    glPushMatrix();
    ///transform();
    //for_each(patch_list.begin(), patch_list.end(), tmpf_patch_boundary);
    for_each(&TMPatchFace::patch_boundary);
    glPopMatrix();
  }

  void renderPatchFaceBoundaries(void) {
    glPushMatrix();
    ///transform();
    //for_each(patch_list.begin(), patch_list.end(), tmpf_face_boundary);
    for_each(&TMPatchFace::face_boundary);
    glPopMatrix();
  }

  void renderPatchNormals(void) {
    glPushMatrix();
    ///transform();
    //for_each(patch_list.begin(), patch_list.end(), tmpf_render_normals);
    for_each(&TMPatchFace::renderNormals);
    glPopMatrix();
  }

  // Compute lighting for patches and the base object
  void computeLighting(DLFLObjectPtr obj, LightPtr lightptr) {
    cout << "TMPatchObject::computeLighting" << endl;
    computeLighting(obj,lightptr);
    TMPatchFacePtrList::iterator first = patch_list.begin(), last = patch_list.end();
    TMPatchFacePtr pfp = NULL;
    while ( first != last ) {
      pfp = (*first); ++first;
      pfp->computeLighting(lightptr);
    }
  }

  /* stuart - bezier export */
  void printPatchCVList( ) const {
    cout << "Patch CVs" << endl;
    TMPatchFacePtrList::const_iterator first = patch_list.begin(), last = patch_list.end();
    while( first != last ) {
      (*first)->print(cout);
      ++first;
    }
  }

};


#endif /* #ifndef _TM_PATCH_OBJECT_HH_ */

