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
*/

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
#ifdef _WIN32 || _WIN64
#include <windows.h>
#endif
#include <GL/gl.h>
#include <GL/glu.h>
#endif

#include <DLFLObject.h>
#include "GeometryRenderer.h"
#include "TMPatchObject.h"

#include "CgData.h"

#ifdef GPU_OK
using namespace Cg;
#endif // GPU_OK


using namespace DLFL;

class DLFLRenderer;
typedef DLFLRenderer * DLFLRendererPtr;

class DLFLRenderer
{
protected:
	/* Default constructor */
	DLFLRenderer()
	{
		// mWireframeThickness(0.1);mSilhouetteThickness(8.0);mVertexThickness(5.0);mFaceCentroidThickness(5.0);mNormalThickness(1.0);mNormalLength(0.5)
#ifdef GPU_OK
		DLFLRenderer::sUseGPU = false;
#endif
		DLFLRenderer::sAntialiasing = false;
		DLFLRenderer::sWireframeColor.setRgbF(0.0, 0.0, 0.0, 0.9);
		DLFLRenderer::sSilhouetteColor.setRgbF(0.0, 0.0, 0.0, 0.8);
		DLFLRenderer::sVertexColor.setRgbF(0.5, 1.0, 0.0, 0.9);
		DLFLRenderer::sFaceCentroidColor.setRgbF(0.5, 0.0, 0.0, 0.9);
		DLFLRenderer::sNormalColor.setRgbF(0.5, 0.0, 0.0, 0.9);
		render_flags = 0;
		// reverse_object = 0;
		gr = GeometryRenderer::instance();
		gr->drawWireframe = true;
	};

	DLFLRenderer(QColor wc, double wt, QColor sc, double st, QColor vc, double vt, QColor fc, double ft, QColor nc, double nt, bool gpu = false, bool aa = false)
	{
		// mWireframeThickness(wt), mSilhouetteThickness(st), mVertexThickness(vt), mWireframeColor(wc),
		// mSilhouetteColor(sc), mVertexColor(vc),mFaceCentroidColor(fc), mFaceCentroidThickness(ft),
		// mNormalColor(nc), mNormalThickness(nt),
		// mNormalLength(0.5);
#ifdef GPU_OK
		DLFLRenderer::sUseGPU = gpu;
#endif
		DLFLRenderer::sAntialiasing = aa;
		render_flags = 0;
		gr = GeometryRenderer::instance();
		gr->drawWireframe = true;
	};

	/* Copy constructor */
	DLFLRenderer(const DLFLRenderer&) {};

public:

	/* Assignment operator */
	DLFLRenderer& operator = (const DLFLRenderer&)
	{
		return (*this);
	};

	/* Destructor */
	virtual ~DLFLRenderer() {}

	/* Set the render flags */
	void setRenderFlags(int rflags)
	{
		// render_flags = rflags;
	};

	void toggleWireframe(void)
	{
		// render_flags ^= ShowWireframe;
		gr->drawWireframe = !(gr->drawWireframe);
	};

	void toggleSilhouette(void)
	{
		// render_flags ^= ShowSilhouette;
		gr->drawSilhouette = !(gr->drawSilhouette);
	};

	void toggleVertices(void)
	{
		// render_flags ^= ShowVertices;
		gr->drawVertices = !(gr->drawVertices);
	};

	void toggleFaceNormals(void)
	{
		// render_flags ^= ShowFaceNormals;
		gr->drawFaceNormals = !(gr->drawFaceNormals);
	};

	void toggleFaceCentroids(void)
	{
		// render_flags ^= ShowFaceCentroids;
		gr->drawFaceCentroids = !(gr->drawFaceCentroids);
	};

#ifdef GPU_OK
	void toggleGPU()
	{
		DLFLRenderer::sUseGPU = !DLFLRenderer::sUseGPU;
		gr->useGPU = !gr->useGPU;
	};
#endif

	void toggleAntialiasing()
	{
		DLFLRenderer::sAntialiasing = !DLFLRenderer::sAntialiasing;
		gr->antialiasing = !(gr->antialiasing);
	};

	void toggleObjectOrientation(void)
	{
		DLFLRenderer::sReverseObject != DLFLRenderer::sReverseObject;
		if (gr) gr->isReversed = DLFLRenderer::sReverseObject;
		if (DLFLRenderer::sReverseObject)
		{
			DLFLRenderer::sReverseObject = false;
		}
		else
		{
			DLFLRenderer::sReverseObject = true;
		}
	};

	static bool isReversed(void)
	{
		return DLFLRenderer::sReverseObject;
	};

	/* Get the render flags */
	int getRenderFlags(void) const
	{
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
	virtual int render(TMPatchObjectPtr patchObject) { return 0; };
	/*
	Perform any initializations required
	*/
	virtual void initialize(void)
	{
		// Derived classes should override this if necessary
		// to do any initializations (like texture setup)
	};

	virtual void setState() {};

public:

	/*
	Set the culling based on which side of object we want to display.
	Has effect only if culling is enabled
	*/
	void setCulling(void)
	{
		if (DLFLRenderer::sReverseObject)
			glCullFace(GL_FRONT);
		else
			glCullFace(GL_BACK);
	};

	/* Methods for various types of rendering */
	void drawWireframe(DLFLObjectPtr object)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glColor4f(
			DLFLRenderer::sWireframeColor.redF(),
			DLFLRenderer::sWireframeColor.greenF(),
			DLFLRenderer::sWireframeColor.blueF(),
			DLFLRenderer::sWireframeColor.alphaF());
		glDepthRange(0.0, 1.0 - 0.0005);
		//object->renderEdges(mWireframeThickness);
		//test material
		gr->renderEdges(object, DLFLRenderer::sWireframeThickness);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	};

	void drawSilhouette(DLFLObjectPtr object)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glColor4f(
			DLFLRenderer::sSilhouetteColor.redF(),
			DLFLRenderer::sSilhouetteColor.greenF(),
			DLFLRenderer::sSilhouetteColor.blueF(),
			DLFLRenderer::sSilhouetteColor.alphaF());
		glDepthRange(0.1, 1.0);
		if (DLFLRenderer::sReverseObject) //object->renderEdges(mWireframeThickness);
			gr->renderEdges(object, DLFLRenderer::sWireframeThickness);
		else //object->renderEdges(mSilhouetteThickness);
			gr->renderEdges(object, DLFLRenderer::sSilhouetteThickness);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	};

	void drawVertices(DLFLObjectPtr object)
	{
		glColor4f(
			DLFLRenderer::sVertexColor.redF(),
			DLFLRenderer::sVertexColor.greenF(),
			DLFLRenderer::sVertexColor.blueF(),
			DLFLRenderer::sVertexColor.alphaF());
		glDepthRange(0.0, 1.0 - 0.00075);
		//object->renderVertices(mVertexThickness);
		gr->renderVertices(object, sVertexThickness);
	};

	void drawFaceCentroids(DLFLObjectPtr object)
	{
#ifdef GPU_OK
		if (DLFLRenderer::sUseGPU)
		{
			cgSetParameter3f(CgData::instance()->basecolor, 0.0, 0.0, 0.0);
			// cgSetParameter3f(CgData::instance()->basecolor, mVertexColor.redF(), mVertexColor.greenF(), mVertexColor.blueF());
			cgSetParameter3f(CgData::instance()->Ka, 0.0, 0.0, 0.0);
			cgSetParameter3f(CgData::instance()->Kd, 0.0, 0.0, 0.0);
			cgSetParameter3f(CgData::instance()->Ks, 0.0, 0.0, 0.0);
			cgSetParameter1f(CgData::instance()->shininess, 0.0);
		}
#endif
		glColor4f(
			DLFLRenderer::sFaceCentroidColor.redF(),
			DLFLRenderer::sFaceCentroidColor.greenF(),
			DLFLRenderer::sFaceCentroidColor.blueF(),
			DLFLRenderer::sFaceCentroidColor.alphaF());
		glDepthRange(0.0, 1.0 - 0.00075);
		//object->renderVertices(mVertexThickness);
		gr->renderFaceCentroids(object, DLFLRenderer::sFaceCentroidThickness);
	};

	void drawFaceNormals(DLFLObjectPtr object)
	{
		glColor4f(
			DLFLRenderer::sNormalColor.redF(),
			DLFLRenderer::sNormalColor.greenF(),
			DLFLRenderer::sNormalColor.blueF(),
			DLFLRenderer::sNormalColor.alphaF());
		glDepthRange(0.0, 1.0 - 0.00075);
		//object->renderVertices(mVertexThickness);
		gr->renderFaceNormals(object, DLFLRenderer::sNormalThickness, DLFLRenderer::sNormalLength);
	};

	/*
	This checks the render flags and calls the above functions.
	This is not defined as static since it needs access to the render flags
	*/
	void drawOverlays(DLFLObjectPtr object)
	{
		// if ( render_flags & ShowWireframe ) drawWireframe(object);
		// if ( render_flags & ShowSilhouette ) drawSilhouette(object);
		// if ( render_flags & ShowVertices ) drawVertices(object);
		// if ( render_flags & ShowFaceNormals ) drawFaceNormals(object);
		// if ( render_flags & ShowFaceCentroids ) drawFaceCentroids(object);

		if (gr->drawWireframe) drawWireframe(object);
		if (gr->drawSilhouette) drawSilhouette(object);
		if (gr->drawVertices) drawVertices(object);
		if (gr->drawFaceNormals) drawFaceNormals(object);
		if (gr->drawFaceCentroids) drawFaceCentroids(object);
	};

	//getters and setters for color and thickness
	void setSilhouetteColor(QColor c) { DLFLRenderer::sSilhouetteColor = c; };
	void setSilhouetteThickness(double t) { DLFLRenderer::sSilhouetteThickness = t; };
	void setWireframeColor(QColor c) { DLFLRenderer::sWireframeColor = c; };
	void setWireframeThickness(double t) { DLFLRenderer::sWireframeThickness = t; };
	void setVertexColor(QColor c) { DLFLRenderer::sVertexColor = c; };
	void setVertexThickness(double t) { DLFLRenderer::sVertexThickness = t; };
	void setFaceCentroidColor(QColor c) { DLFLRenderer::sFaceCentroidColor = c; };
	void setFaceCentroidThickness(double t) { DLFLRenderer::sFaceCentroidThickness = t; };
	void setNormalColor(QColor c) { DLFLRenderer::sNormalColor = c; };
	void setNormalThickness(double t) { DLFLRenderer::sNormalThickness = t; };
	void setNormalLength(double l) { DLFLRenderer::sNormalLength = l; };

public:

	/* Enumeration for render flags */
	// enum RenderFlags { ShowWireframe=1, ShowSilhouette=2, ShowVertices=4, ShowFaceNormals=8, ShowFaceCentroids=16 };

protected:
	//!< pointer to the singleton class GeometryRenderer
	GeometryRenderer *gr;

	/* Integer which can be used to represent a combination of render flags */
	// static int render_flags;

	/* Flag indicating if we should front or back side of object */
	int render_flags;
	static bool sReverseObject;
#ifdef GPU_OK
	static bool sUseGPU;
#endif
	static bool sAntialiasing;

private:
	static QColor sWireframeColor;				//!< wireframe RGB color
	static QColor sSilhouetteColor;				//!< silhouette RGB color
	static QColor sVertexColor;					//!< vertex RGB color
	static QColor sFaceCentroidColor;			//!< face centroid RGB color
	static QColor sNormalColor;					//!< normals RGB color
	static double sFaceCentroidThickness;		//!< face centroid point size
	static double sNormalThickness;				//!< normal line thickness
	static double sNormalLength;				//!< normal length
	static double sWireframeThickness;			//!< wireframe line thickness
	static double sSilhouetteThickness;			//!< silhouette line thickness
	static double sVertexThickness;				//!< vertex point size

};

#endif
