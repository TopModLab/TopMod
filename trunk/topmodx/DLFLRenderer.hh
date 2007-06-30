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

  static QColor mWireframeColor;									//!< wireframe RGB color
  static QColor mSilhouetteColor;								//!< silhouette RGB color
  static QColor mVertexColor;										//!< vertex RGB color
  static QColor mFaceCentroidColor;							//!< face centroid RGB color
	static QColor mNormalColor;										//!< normals RGB color
	static double mFaceCentroidThickness;					//!< face centroid point size
	static double mNormalThickness;								//!< normal line thickness
	static double mNormalLength;										//!< normal length 
	static double mWireframeThickness;							//!< wireframe line thickness
	static double mSilhouetteThickness;						//!< silhouette line thickness
  static double mVertexThickness;								//!< vertex point size

protected :

  GeometryRenderer *gr;										//!< pointer to the singleton class GeometryRenderer

  /* Integer which can be used to represent a combination of render flags */
  // static int render_flags;

  /* Flag indicating if we should front or back side of object */
  int render_flags;
  static bool reverse_object;
	static bool useGPU;
	static bool antialiasing;

public :

  /* Enumeration for render flags */
  // enum RenderFlags { ShowWireframe=1, ShowSilhouette=2, ShowVertices=4, ShowFaceNormals=8, ShowFaceCentroids=16 };

protected :

  /* Default constructor */
  DLFLRenderer() {
		// mWireframeThickness(0.1);mSilhouetteThickness(8.0);mVertexThickness(5.0);mFaceCentroidThickness(5.0);mNormalThickness(1.0);mNormalLength(0.5)
		DLFLRenderer::useGPU = true; 
		DLFLRenderer::antialiasing = false; 
    DLFLRenderer::mWireframeColor.setRgbF(0.0,0.0,0.0,0.9);
    DLFLRenderer::mSilhouetteColor.setRgbF(0.0,0.0,0.0,0.8);
    DLFLRenderer::mVertexColor.setRgbF(0.5,1.0,0.0,0.9);
		DLFLRenderer::mFaceCentroidColor.setRgbF(0.5,0.0,0.0,0.9);
		DLFLRenderer::mNormalColor.setRgbF(0.5,0.0,0.0,0.9);
    render_flags = 0;
    // reverse_object = 0;
    gr = GeometryRenderer::instance( );
		gr->drawWireframe = true;
  };
	
  DLFLRenderer(QColor wc, double wt, QColor sc, double st, QColor vc, double vt, QColor fc, double ft, QColor nc, double nt, bool gpu = true, bool aa = false) {
// mWireframeThickness(wt), mSilhouetteThickness(st), mVertexThickness(vt), mWireframeColor(wc),
			// mSilhouetteColor(sc), mVertexColor(vc),mFaceCentroidColor(fc), mFaceCentroidThickness(ft),
			// mNormalColor(nc), mNormalThickness(nt), 
		// mNormalLength(0.5);
		DLFLRenderer::useGPU = gpu; 
		DLFLRenderer::antialiasing = aa; 
    render_flags = 0;
    gr = GeometryRenderer::instance( );
		gr->drawWireframe = true;
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
    // render_flags = rflags;
  };

  void toggleWireframe(void) {
    // render_flags ^= ShowWireframe;
		gr->drawWireframe = !(gr->drawWireframe);
  };

  void toggleSilhouette(void) {
    // render_flags ^= ShowSilhouette;
		gr->drawSilhouette = !(gr->drawSilhouette);
  };

  void toggleVertices(void) {
    // render_flags ^= ShowVertices;
		gr->drawVertices = !(gr->drawVertices);
  };

  void toggleFaceNormals(void) {
    // render_flags ^= ShowFaceNormals;
    gr->drawFaceNormals = !(gr->drawFaceNormals);
  };

  void toggleFaceCentroids(void) {
    // render_flags ^= ShowFaceCentroids;
    gr->drawFaceCentroids = !(gr->drawFaceCentroids);
  };

	void toggleGPU() {
		DLFLRenderer::useGPU = !DLFLRenderer::useGPU; 
		gr->useGPU = !gr->useGPU;
	};

	void toggleAntialiasing(){ 
		DLFLRenderer::antialiasing = !DLFLRenderer::antialiasing; 
		gr->antialiasing = !(gr->antialiasing);
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

	// #ifdef GPU_OK
	// void setCgData(CgData cg){	mCg = cg; } 
	// #endif
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
      cgSetParameter3f(CgData::instance()->basecolor, 0.0, 0.0, 0.0);
      cgSetParameter3f(CgData::instance()->Ka, 0.0, 0.0, 0.0);
      cgSetParameter3f(CgData::instance()->Kd, 0.0, 0.0, 0.0);
      cgSetParameter3f(CgData::instance()->Ks, 0.0, 0.0, 0.0);
      cgSetParameter1f(CgData::instance()->shininess, 0.0);
    }
		#endif
    glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
    glColor4f( DLFLRenderer::mWireframeColor.redF(), DLFLRenderer::mWireframeColor.greenF(), DLFLRenderer::mWireframeColor.blueF(), DLFLRenderer::mWireframeColor.alphaF());
    glDepthRange(0.0,1.0-0.0005);
    //object->renderEdges(mWireframeThickness);
    //test material
    gr->renderEdges( object, DLFLRenderer::mWireframeThickness );
    glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
  };

  void drawSilhouette(DLFLObjectPtr object) {
		#ifdef GPU_OK
	  if(DLFLRenderer::useGPU) {
      cgSetParameter3f(CgData::instance()->basecolor, 0.0, 0.0, 0.0);
      cgSetParameter3f(CgData::instance()->Ka, 0.0, 0.0, 0.0);
      cgSetParameter3f(CgData::instance()->Kd, 0.0, 0.0, 0.0);
      cgSetParameter3f(CgData::instance()->Ks, 0.0, 0.0, 0.0);
      cgSetParameter1f(CgData::instance()->shininess, 0.0);
    }
		#endif
    glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
    glColor4f(DLFLRenderer::mSilhouetteColor.redF(),DLFLRenderer::mSilhouetteColor.greenF(),DLFLRenderer::mSilhouetteColor.blueF(),DLFLRenderer::mSilhouetteColor.alphaF());
    glDepthRange(0.1,1.0);
    if ( DLFLRenderer::reverse_object ) //object->renderEdges(mWireframeThickness);
      gr->renderEdges( object, DLFLRenderer::mWireframeThickness );
    else //object->renderEdges(mSilhouetteThickness);
      gr->renderEdges( object, DLFLRenderer::mSilhouetteThickness );
    glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
  };

  void drawVertices(DLFLObjectPtr object) {
		#ifdef GPU_OK
	  if(DLFLRenderer::useGPU) {
      cgSetParameter3f(CgData::instance()->basecolor, 0.0, 0.0, 0.0);
      // cgSetParameter3f(CgData::instance()->basecolor, mVertexColor.redF(), mVertexColor.greenF(), mVertexColor.blueF());
      cgSetParameter3f(CgData::instance()->Ka, 0.0, 0.0, 0.0);
      cgSetParameter3f(CgData::instance()->Kd, 0.0, 0.0, 0.0);
      cgSetParameter3f(CgData::instance()->Ks, 0.0, 0.0, 0.0);
      cgSetParameter1f(CgData::instance()->shininess, 0.0);
    }
		#endif 
    glColor4f(DLFLRenderer::mVertexColor.redF(),DLFLRenderer::mVertexColor.greenF(),DLFLRenderer::mVertexColor.blueF(),DLFLRenderer::mVertexColor.alphaF());
    glDepthRange(0.0,1.0-0.00075);
    //object->renderVertices(mVertexThickness);
    gr->renderVertices( object, mVertexThickness );
  };

  void drawFaceCentroids(DLFLObjectPtr object) {
		#ifdef GPU_OK
	  if(DLFLRenderer::useGPU) {
      cgSetParameter3f(CgData::instance()->basecolor, 0.0, 0.0, 0.0);
      // cgSetParameter3f(CgData::instance()->basecolor, mVertexColor.redF(), mVertexColor.greenF(), mVertexColor.blueF());
      cgSetParameter3f(CgData::instance()->Ka, 0.0, 0.0, 0.0);
      cgSetParameter3f(CgData::instance()->Kd, 0.0, 0.0, 0.0);
      cgSetParameter3f(CgData::instance()->Ks, 0.0, 0.0, 0.0);
      cgSetParameter1f(CgData::instance()->shininess, 0.0);
    }
		#endif 
    glColor4f(DLFLRenderer::mFaceCentroidColor.redF(),DLFLRenderer::mFaceCentroidColor.greenF(),DLFLRenderer::mFaceCentroidColor.blueF(),DLFLRenderer::mFaceCentroidColor.alphaF());
    glDepthRange(0.0,1.0-0.00075);
    //object->renderVertices(mVertexThickness);
    gr->renderFaceCentroids( object, DLFLRenderer::mFaceCentroidThickness );
  };

  void drawFaceNormals(DLFLObjectPtr object) {
		#ifdef GPU_OK
	  if(DLFLRenderer::useGPU) {
      cgSetParameter3f(CgData::instance()->basecolor, 0.0, 0.0, 0.0);
      cgSetParameter3f(CgData::instance()->Ka, 0.0, 0.0, 0.0);
      cgSetParameter3f(CgData::instance()->Kd, 0.0, 0.0, 0.0);
      cgSetParameter3f(CgData::instance()->Ks, 0.0, 0.0, 0.0);
      cgSetParameter1f(CgData::instance()->shininess, 0.0);
    }
		#endif 
		// std::cout<<"drawing face normals!!! \n";
    glColor4f(DLFLRenderer::mNormalColor.redF(),DLFLRenderer::mNormalColor.greenF(),DLFLRenderer::mNormalColor.blueF(),DLFLRenderer::mNormalColor.alphaF());
    glDepthRange(0.0,1.0-0.00075);
    //object->renderVertices(mVertexThickness);
    gr->renderFaceNormals( object, DLFLRenderer::mNormalThickness, DLFLRenderer::mNormalLength );
  };

  /*
    This checks the render flags and calls the above functions.
    This is not defined as static since it needs access to the render flags
  */
  void drawOverlays(DLFLObjectPtr object) {
    // if ( render_flags & ShowWireframe ) drawWireframe(object);
    // if ( render_flags & ShowSilhouette ) drawSilhouette(object);
    // if ( render_flags & ShowVertices ) drawVertices(object);
    // if ( render_flags & ShowFaceNormals ) drawFaceNormals(object);
    // if ( render_flags & ShowFaceCentroids ) drawFaceCentroids(object);

    if ( gr->drawWireframe ) drawWireframe(object);
    if ( gr->drawSilhouette ) drawSilhouette(object);
    if ( gr->drawVertices ) drawVertices(object);
    if ( gr->drawFaceNormals ) drawFaceNormals(object);
    if ( gr->drawFaceCentroids ) drawFaceCentroids(object);
  };
	
	//getters and setters for color and thickness
	void setSilhouetteColor(QColor c){ DLFLRenderer::mSilhouetteColor = c; };
	void setSilhouetteThickness(double t){ DLFLRenderer::mSilhouetteThickness = t; };
	void setWireframeColor(QColor c){	DLFLRenderer::mWireframeColor = c; };
	void setWireframeThickness(double t){ DLFLRenderer::mWireframeThickness = t; };
	void setVertexColor(QColor c){ DLFLRenderer::mVertexColor = c; };
	void setVertexThickness(double t){ DLFLRenderer::mVertexThickness = t; };
	void setFaceCentroidColor(QColor c){ DLFLRenderer::mFaceCentroidColor = c; };
	void setFaceCentroidThickness(double t){ DLFLRenderer::mFaceCentroidThickness = t; };
	void setNormalColor(QColor c){ DLFLRenderer::mNormalColor = c; };
	void setNormalThickness(double t){ DLFLRenderer::mNormalThickness = t; };
	void setNormalLength(double l){ DLFLRenderer::mNormalLength = l; };
	
	
};

#endif 
