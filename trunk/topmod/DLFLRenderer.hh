/* $Id: DLFLRenderer.hh,v 4.2 2004/04/23 10:42:06 vinod Exp $ */

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

#ifdef __APPLE__
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#else
#include <GL/gl.h>
#include <GL/glu.h>
#endif
#include "DLFLObject.hh"

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

		
protected :

	  /* Integer which can be used to represent a combination of render flags */
	// static int render_flags;

	  /* Flag indicating if we should front or back side of object */
	// static bool reverse_object;

	int render_flags;
	static bool reverse_object;

	public :

	  /* Enumeration for render flags */
	enum RenderFlags { ShowWireframe=1, ShowSilhouette=2, ShowVertices=4, ShowNormals=8 };

protected :

	  /* Default constructor */
	DLFLRenderer() 
	: mWireframeThickness(1.5), mSilhouetteThickness(6.0), mVertexThickness(4.0) {
		mWireframeColor.setRgbF(0.0,0.0,0.0,0.9);
		mSilhouetteColor.setRgbF(0.0,0.0,0.0,0.8);
		mVertexColor.setRgbF(0.5,1.0,0.0,0.9);
		render_flags = 0;
		// reverse_object = 0;
	};
	
	DLFLRenderer(QColor wc, double wt, QColor sc, double st, QColor vc, double vt)
	: mWireframeThickness(wt), mSilhouetteThickness(st), mVertexThickness(vt) {
		mWireframeColor = wc;
		mSilhouetteColor = sc;
		mVertexColor = vc;
		render_flags = 0;
		// reverse_object = 0;
	};

	  /* Copy constructor */
	DLFLRenderer(const DLFLRenderer&) {};

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
	};

	void toggleObjectOrientation(void) {
	   if ( DLFLRenderer::reverse_object ) DLFLRenderer::reverse_object = false;
	   else DLFLRenderer::reverse_object = true;
	};

	static bool isReversed(void) {
	   return DLFLRenderer::reverse_object;
	};

	  /* Get the render flags */
	int getRenderFlags(void) const {
	   return render_flags;
	};

	  /*
	    Render the DLFLObject specified by the given pointer.
	    This is a virtual function which has to be implemented by derived classes.
	    Return value can be used for error reporting..
	  */
	virtual int render(DLFLObjectPtr object) = 0;

	  /*
	    Perform any initializations required
	  */
	virtual void initialize(void) {
	      // Derived classes should override this if necessary 
	      // to do any initializations (like texture setup)
	};

public :

	  /*
	    Set the culling based on which side of object we want to display.
	    Has effect only if culling is enabled
	  */
	void setCulling(void) {
	   if ( DLFLRenderer::reverse_object ) glCullFace(GL_FRONT);
	   else glCullFace(GL_BACK);
	};

	  /* Methods for various types of rendering */

	void drawWireframe(DLFLObjectPtr object) {
		glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
		glColor4f(mWireframeColor.redF(), mWireframeColor.greenF(), mWireframeColor.blueF(), mWireframeColor.alphaF());
		glDepthRange(0.0,1.0-0.0005);
		object->renderEdges(mWireframeThickness);
		glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
	};

	void drawSilhouette(DLFLObjectPtr object) {
		glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
		glColor4f(mSilhouetteColor.redF(),mSilhouetteColor.greenF(),mSilhouetteColor.blueF(),mSilhouetteColor.alphaF());
		glDepthRange(0.1,1.0);
		if ( DLFLRenderer::reverse_object ) object->renderEdges(2.0);
		else object->renderEdges(mSilhouetteThickness);
		glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
	};

	void drawVertices(DLFLObjectPtr object) {
	   glColor4f(mVertexColor.redF(),mVertexColor.greenF(),mVertexColor.blueF(),mVertexColor.alphaF());
	   glDepthRange(0.0,1.0-0.00075);
	   object->renderVertices(mVertexThickness);
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
