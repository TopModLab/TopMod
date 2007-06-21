#ifndef _DLFL_RENDERER_HH_
#define _DLFL_RENDERER_HH_

// Abstrtact base class for various renderers for DLFL
// Defines the minimum interface that renderer classes must implement
// Also contains an integer which can be used to store render flags
// The way the integer is used to specify the render flags is left
// to the derived class. An enumeration for render flags is also defined
// with the various values defined as powers of 2 to allow ORing together
// The class also provides static functions for rendering the object as a wireframe,
// silhouette and vertices, which can be used by derived classes

#include <QColor>
#include <QMessageBox>

#ifdef __APPLE__
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#else
#include <GL/gl.h>
#include <GL/glu.h>
#endif

#include <DLFLObject.hh>
#include "GeometryRenderer.hh"
#include "TMPatchObject.hh"

#include "CgData.hh"

#ifdef GPU_OK
using namespace Cg;
#endif // GPU_OK


using namespace DLFL;

class DLFLRenderer;
typedef DLFLRenderer * DLFLRendererPtr;

class DLFLRenderer {
	
private :

  QColor mWireframeColor;
  QColor mSilhouetteColor;
  QColor mVertexColor;
  double mWireframeThickness;
  double mSilhouetteThickness;
  double mVertexThickness;

	#ifdef GPU_OK
	CgData mCg;
	#endif

protected :

  GeometryRenderer *gr;

  /* Integer which can be used to represent a combination of render flags */
  // static int render_flags;

  /* Flag indicating if we should front or back side of object */
  int render_flags;
  static bool reverse_object;
	static bool useGPU;
	static bool antialiasing;

public :

  /* Enumeration for render flags */
  enum RenderFlags { ShowWireframe=1, ShowSilhouette=2, ShowVertices=4, ShowNormals=8 };

protected :

  /* Default constructor */
  DLFLRenderer() : mWireframeThickness(1.5), mSilhouetteThickness(6.0), mVertexThickness(4.0){
		DLFLRenderer::useGPU = true; 
		DLFLRenderer::antialiasing = false; 
    mWireframeColor.setRgbF(0.0,0.0,0.0,0.9);
    mSilhouetteColor.setRgbF(0.0,0.0,0.0,0.8);
    mVertexColor.setRgbF(0.5,1.0,0.0,0.9);
    render_flags = 0;
    // reverse_object = 0;
    gr = GeometryRenderer::instance( );
  };
	
  DLFLRenderer(QColor wc, double wt, QColor sc, double st, QColor vc, double vt, bool gpu = true, bool aa = false)
    : mWireframeThickness(wt), mSilhouetteThickness(st), mVertexThickness(vt) {
		DLFLRenderer::useGPU = gpu; 
		DLFLRenderer::antialiasing = aa; 
    mWireframeColor = wc;
    mSilhouetteColor = sc;
    mVertexColor = vc;
    render_flags = 0;
    // reverse_object = 0;
    gr = GeometryRenderer::instance( );
  };

  /* Copy constructor */
  DLFLRenderer(const DLFLRenderer&) { };

public :

  /* Assignment operator */
  DLFLRenderer& operator = (const DLFLRenderer&) {
    return (*this);
  };

  /* Destructor */
  virtual ~DLFLRenderer() {}
  
  /* Set the render flags */
  void setRenderFlags(int rflags) {
    render_flags = rflags;
  };

  void toggleWireframe(void) {
    render_flags ^= ShowWireframe;
  };

  void toggleSilhouette(void) {
    render_flags ^= ShowSilhouette;
  };

  void toggleVertices(void) {
    render_flags ^= ShowVertices;
  };

  void toggleNormals(void) {
    render_flags ^= ShowNormals;
    gr->drawFaceNormals = !(gr->drawFaceNormals);
  };

	void toggleGPU() {
		DLFLRenderer::useGPU = !DLFLRenderer::useGPU; 
	};

	void toggleAntialiasing(){ 
		DLFLRenderer::antialiasing = !DLFLRenderer::antialiasing; 
	};

  void toggleObjectOrientation(void) {
    DLFLRenderer::reverse_object != DLFLRenderer::reverse_object;
    if(gr) gr->isReversed = DLFLRenderer::reverse_object;
    if ( DLFLRenderer::reverse_object ) {
      DLFLRenderer::reverse_object = false;
    } else {
      DLFLRenderer::reverse_object = true;
    }
  };

  static bool isReversed(void) {
    return DLFLRenderer::reverse_object;
  };

  /* Get the render flags */
  int getRenderFlags(void) const {
    return render_flags;
  };

	#ifdef GPU_OK
	void setCgData(CgData cg){	mCg = cg; } 
	#endif
  /*
    Render the DLFLObject specified by the given pointer.
    This is a virtual function which has to be implemented by derived classes.
    Return value can be used for error reporting..
  */
  virtual int render(DLFLObjectPtr object) = 0;
  virtual int render(TMPatchObjectPtr patchObject) { /* most renderers leave this blank*/ };
  /*
    Perform any initializations required
  */
  virtual void initialize(void) {
    // Derived classes should override this if necessary 
    // to do any initializations (like texture setup)
  };

  virtual void setState( ) { };

public :

  /*
    Set the culling based on which side of object we want to display.
    Has effect only if culling is enabled
  */
  void setCulling(void) {
    if ( DLFLRenderer::reverse_object ) 
      glCullFace(GL_FRONT);
    else 
      glCullFace(GL_BACK);
  };

  /* Methods for various types of rendering */
  void drawWireframe(DLFLObjectPtr object) {
		#ifdef GPU_OK
    if(DLFLRenderer::useGPU) {
      cgSetParameter3f(mCg.Ke, 0.0, 0.0, 0.0);
      cgSetParameter3f(mCg.Ka, 0.0, 0.0, 0.0);
      cgSetParameter3f(mCg.Kd, 0.0, 0.0, 0.0);
      cgSetParameter3f(mCg.Ks, 0.0, 0.0, 0.0);
      cgSetParameter1f(mCg.shininess, 0.0);
    }
		#endif
    glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
    glColor4f( mWireframeColor.redF(), mWireframeColor.greenF(), mWireframeColor.blueF(), mWireframeColor.alphaF());
    glDepthRange(0.0,1.0-0.0005);
    //object->renderEdges(mWireframeThickness);
    //test material
    gr->renderEdges( object, mWireframeThickness );
    glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
  };

  void drawSilhouette(DLFLObjectPtr object) {
		#ifdef GPU_OK
	  if(DLFLRenderer::useGPU) {
      cgSetParameter3f(mCg.Ke, 0.0, 0.0, 0.0);
      cgSetParameter3f(mCg.Ka, 0.0, 0.0, 0.0);
      cgSetParameter3f(mCg.Kd, 0.0, 0.0, 0.0);
      cgSetParameter3f(mCg.Ks, 0.0, 0.0, 0.0);
      cgSetParameter1f(mCg.shininess, 0.0);
    }
		#endif
    glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
    glColor4f(mSilhouetteColor.redF(),mSilhouetteColor.greenF(),mSilhouetteColor.blueF(),mSilhouetteColor.alphaF());
    glDepthRange(0.1,1.0);
    if ( DLFLRenderer::reverse_object ) //object->renderEdges(mWireframeThickness);
      gr->renderEdges( object, mWireframeThickness );
    else //object->renderEdges(mSilhouetteThickness);
      gr->renderEdges( object, mSilhouetteThickness );
    glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
  };

  void drawVertices(DLFLObjectPtr object) {
		#ifdef GPU_OK
	  if(DLFLRenderer::useGPU) {
      cgSetParameter3f(mCg.Ke, 0.0, 0.0, 0.0);
      cgSetParameter3f(mCg.Ka, mVertexColor.redF(),mVertexColor.greenF(),mVertexColor.blueF());
      cgSetParameter3f(mCg.Kd, 0.0, 0.0, 0.0);
      cgSetParameter3f(mCg.Ks, 0.0, 0.0, 0.0);
      cgSetParameter1f(mCg.shininess, 0.0);
    }
		#endif 
    glColor4f(mVertexColor.redF(),mVertexColor.greenF(),mVertexColor.blueF(),mVertexColor.alphaF());
    glDepthRange(0.0,1.0-0.00075);
    //object->renderVertices(mVertexThickness);
    gr->renderVertices( object, mVertexThickness );
  };

  /*
    This checks the render flags and calls the above functions.
    This is not defined as static since it needs access to the render flags
  */
  void drawOverlays(DLFLObjectPtr object) {
    if ( render_flags & ShowWireframe ) drawWireframe(object);
    if ( render_flags & ShowSilhouette ) drawSilhouette(object);
    if ( render_flags & ShowVertices ) drawVertices(object);
  };
	
  //getters and setters for color and thickness
  void setSilhouetteColor(QColor c){ mSilhouetteColor = c; };
  QColor getSilhouetteColor(){ return mSilhouetteColor; };
  void setSilhouetteThickness(double t){ mSilhouetteThickness = t; };
  double getSilhouetteThickness(){ return mSilhouetteThickness; };
	
  void setWireframeColor(QColor c){	mWireframeColor = c; };
  QColor getWireframeColor(){ return mWireframeColor; };
  void setWireframeThickness(double t){ mWireframeThickness = t; };
  double getWireframeThickness(){ return mWireframeThickness; };
	
  void setVertexColor(QColor c){ mVertexColor = c; };
  QColor getVertexColor(){ return mVertexColor; };
  void setVertexThickness(double t){ mVertexThickness = t; };
  double getVertexThickness(){ return mVertexThickness;	};
	
};

#endif 
