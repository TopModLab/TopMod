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
  ~TMPatchObject() {}

protected :
  DLFLObjectPtr mObj; // the last obj created from

  // Free the memory allocated for the patches
  void destroyPatches() {
    TMPatchFacePtrList::iterator first = patch_list.begin(), last = patch_list.end();
    TMPatchFacePtr pfp = NULL;
    while ( first != last ) {
      pfp = (*first); ++first;
      delete pfp;
    }
    patch_list.clear();
  }

  // Build the list of patch faces
  void createPatches( DLFLObjectPtr obj ) {
    destroyPatches();
    DLFLFacePtrList face_list = obj->getFaceList( );
    DLFLFacePtrList::iterator ffirst = face_list.begin(), flast = face_list.end();
    DLFLFacePtr fp = NULL;
    TMPatchFacePtr pfp = NULL;
       
    while ( ffirst != flast ) {
      fp = (*ffirst); ++ffirst;
      pfp = new TMPatchFace(patchsize);
      pfp->setDLFLFace(fp); pfp->createPatches();
      patch_list.push_back(pfp);
    }

    // Adjust the edge points for all patches
    DLFLEdgePtrList edge_list = obj->getEdgeList( );
    DLFLEdgePtrList :: iterator efirst = edge_list.begin(), elast = edge_list.end();
    DLFLEdgePtr ep = NULL;
    DLFLFaceVertexPtr fvp1,fvp2;
    TMPatchPtr pp1, pp2;
    Vector3d p00,p01,p10,p11,ip;
    while ( efirst != elast ) {
      ep = (*efirst); ++efirst;
      ep->getCorners(fvp1,fvp2);
      pp1 = getPatchPtr(fvp1); pp2 = getPatchPtr(fvp2);

      p00 = pp1->getControlPoint(2,0); p01 = pp2->getControlPoint(2,0);
      p10 = pp1->getControlPoint(3,1); p11 = pp2->getControlPoint(3,1);
      ip = intersectCoplanarLines(p00,p01,p10,p11);

      pp1->setControlPoint(3,0,ip); pp2->setControlPoint(3,0,ip);
      pp1->updateGLPointArray(); pp2->updateGLPointArray();

      pp1 = getPatchPtr(fvp1->next()); pp2 = getPatchPtr(fvp2->next());
      pp1->setControlPoint(0,3,ip); pp2->setControlPoint(0,3,ip);
      pp1->updateGLPointArray(); pp2->updateGLPointArray();
    }

    // Adjust the vertex points for 4-valence vertices
    DLFLVertexPtrList vertex_list = obj->getVertexList( );
    DLFLVertexPtrList :: iterator vfirst = vertex_list.begin(), vlast = vertex_list.end();
    DLFLVertexPtr vp = NULL;
    while ( vfirst != vlast ) {
      vp = (*vfirst); ++vfirst;
      if ( vp->valence() == 4 ) {
	DLFLFaceVertexPtrArray vcorners;
	vp->getOrderedCorners(vcorners);
	pp1 = getPatchPtr(vcorners[0]); pp2 = getPatchPtr(vcorners[2]);

	p00 = pp1->getControlPoint(1,0); p01 = pp2->getControlPoint(1,0);
	p10 = pp1->getControlPoint(0,1); p11 = pp2->getControlPoint(0,1);
	ip = intersectCoplanarLines(p00,p01,p10,p11);

	for (int i=0; i < 4; ++i) {
	  pp1 = getPatchPtr(vcorners[i]);
	  pp1->setControlPoint(0,0,ip);
	  pp1->updateGLPointArray();
	}
      }
    }
                 
    /*
      TMPatchFacePtrList :: iterator pfirst = patch_list.begin(), plast = patch_list.end();
      while ( pfirst != plast )
      {
      pfp = (*pfirst); ++pfirst;
      pfp->adjustEdgePoints();
      }
    */
  }

public :

  // Set the patch size
  void setPatchSize(int size, DLFLObjectPtr obj = NULL ) {
    if ( size != patchsize && size > 0 ) {
	patchsize = size;
	if( !obj ) { obj = mObj; }
	if( !obj ) { return; }
	createPatches( obj );
    }
  }

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

