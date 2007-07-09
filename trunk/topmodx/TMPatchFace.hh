#ifndef _TM_PATCH_FACE_HH_
#define _TM_PATCH_FACE_HH_

#include "TMPatch.hh"

/* Class for a TopMod Bezier Face containing several Patches */

class TMPatchFace;
typedef TMPatchFace *TMPatchFacePtr;

typedef list<TMPatchFacePtr> TMPatchFacePtrList;
typedef vector<TMPatchFacePtr> TMPatchFacePtrArray;

typedef list<TMPatchFace> TMPatchFaceList;
typedef vector<TMPatchFace> TMPatchFaceArray;

class TMPatchFace {
protected :

  int patchsize;		// Grid size of each patch in the face
  TMPatchArray patcharray;	// Array of Bezier patches for this face
  DLFLFacePtr dlflface;	// DLFLFace associated with this Bezier face

  // Resize the patch array depending on number of corners in the DLFLFace
  void resizePatchArray(void);

public :

  // Default & 1-arg constructor
  TMPatchFace(int psize=4)
    : patchsize(psize), patcharray(), dlflface(NULL) {}

  // Destructor
  ~TMPatchFace() {}

  // Copy constructor
  TMPatchFace(const TMPatchFace& tmpf)
    : patchsize(tmpf.patchsize), patcharray(tmpf.patcharray), dlflface(tmpf.dlflface) {}

  // Assignment operator
  TMPatchFace&  operator = (const TMPatchFace& tmpf) {
    patchsize = tmpf.patchsize; patcharray = tmpf.patcharray; dlflface = tmpf.dlflface;
    return (*this);
  }

  // Resize all patches
  void resizePatches(int psize);

  // Set the DLFLFace pointer
  void setDLFLFace(DLFLFacePtr fp) {
    dlflface = fp;
    resizePatchArray();
  }
     
  // Create the patches using face information
  void createPatches(TMPatchMap &patchMap);

  // Adjust the edge points for each patch in the face
  void adjustEdgePoints(TMPatchMap &patchMap);
              
  // Compute lighting for the patches in this face
  void computeLighting(LightPtr lightptr) {
    DLFLMaterialPtr matl;
    for (uint i=0; i < patcharray.size(); ++i) {
      matl = dlflface->material();
      patcharray[i].computeLighting(matl->color,matl->Ka,matl->Kd,matl->Ks,lightptr);
    }
  }
     
  // Render the patches using filled polygons
  void render(void) {for (uint i=0; i < patcharray.size(); ++i) {
      patcharray[i].render();
    }
  }

  // Render the patches using outlined polygons
  void outline(void) {
    for (uint i=0; i < patcharray.size(); ++i) patcharray[i].outline();
  }

  // Render the boundaries of the patches
  void patch_boundary(void) {
    for (uint i=0; i < patcharray.size(); ++i) patcharray[i].patch_boundary();
  }

  // Render the boundaries of the patches
  void face_boundary(void) {
    for (uint i=0; i < patcharray.size(); ++i) patcharray[i].face_boundary();
  }

  // Render the control grids of the patches
  void controlgrid(void) {
    for (uint i=0; i < patcharray.size(); ++i) patcharray[i].controlgrid();
  }

  // Show the conrtol points
  void controlpoints(void) {
    for (uint i=0; i < patcharray.size(); ++i) patcharray[i].controlpoints();
  }

  // Show the normals
  void renderNormals(void) {
    for (uint i=0; i < patcharray.size(); ++i) patcharray[i].renderNormals();
  }
     
  /* stuart - bezier export */ 
  int print( ostream &stream ) {
    for( int i = 0; i < patcharray.size(); i++ ) {
      patcharray[i].printControlPoints( stream );
      stream << std::endl;
    }
    return patcharray.size( );
  }
  /*
    friend void tmpf_render(TMPatchFacePtr pfp) {
    pfp->render();
    }

    friend void tmpf_outline(TMPatchFacePtr pfp) {
    pfp->outline();
    }

    friend void tmpf_patch_boundary(TMPatchFacePtr pfp) {
    pfp->patch_boundary();
    }

    friend void tmpf_face_boundary(TMPatchFacePtr pfp) {
    pfp->face_boundary();
    }

    friend void tmpf_controlgrid(TMPatchFacePtr pfp) {
    pfp->controlgrid();
    }
     
    friend void tmpf_controlpoints(TMPatchFacePtr pfp) {
    pfp->controlpoints();
    }

    friend void tmpf_render_normals(TMPatchFacePtr pfp) {
    pfp->renderNormals();
    }*/
};

#endif /* #ifndef _TM_PATCH_FACE_HH_ */

