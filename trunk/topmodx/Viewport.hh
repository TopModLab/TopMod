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

/* $Id: Viewport.hh,v 1.5 2002/10/28 07:00:16 vinod Exp $ */

#ifndef _VIEWPORT_HH_

#define _VIEWPORT_HH_

// Class for a generic Viewport. Supports rotation, panning, zooming and dollying
// using a transformation matrix and subroutines to handle corresponding events
// This is a modified version of the ViewWindow class, with all connection to FLTK
// or any other user interface/windowing system removed.


#ifdef __APPLE__
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#else
#include <GL/gl.h>
#include <GL/glu.h>
#endif
#include <Arcball.hh>
#include <TransControl.hh>
#include <ZoomControl.hh>
#include <DollyControl.hh>
#include <Transform.hh>
#include <Camera.hh>

// #include <QtGui>

// Enumerations for the different views. The names imply looking from the corresponding
// side. ie Front -> looking *from* front, Right -> looking *from* right
enum VPView { VPPersp=0, VPFront=1, VPRight=2, VPTop=3, VPBack=4, VPLeft=5, VPBottom=6 };

// Enumerations for current transformation mode
enum VPTransformType { VPNone=0, VPPan=1, VPRotate=2, VPZoom=3, VPDolly=4 };

// Enumerations for mouse events
enum VPMouseEvent { VPUnknown=0, VPPush=1, VPRelease=2, VPDrag=3 };

class Viewport {

	protected :

	int width, height;                                // Width and height of viewport
	Arcball arcball;                                  // The arcball controller
	TransControl trcontrol;                           // Translation controller
	ZoomControl zoomcontrol;                          // Zoom controller
	DollyControl dollycontrol;                        // Dolly controller (10.0 scale)

	Transformation transform;                         // Combined transformation

	VPTransformType currenttr;                        // Current transformation
	VPView   view;                                    // Which view?

	public :
				// Made public to allow changing camera settings easily (view-volume, near/far)
	Camera camera;                                    // Camera attached to this window

				// Set eyepoint, center, etc. for perspective views
				// Setting these requires modifying the transformation matrix
				// to get the proper rotation using arcball
	void setPerspView(const Vector3d& eye, const Vector3d& center, const Vector3d& up) {
		if ( view == VPPersp ) {
			Vector3d neweye = eye - center;
			double eyedist = norm(neweye);
			Matrix4x4 lmat = Transformation::lookat(neweye,Vector3d(0,0,0),up);
			transform = lmat;
			transform.translate(-center);
			camera.setEye(Vector3d(0,0,eyedist));
		}
	}

	void setPerspView(double eyex, double eyey, double eyez, double cenx, double ceny, double cenz, double upx, double upy, double upz) {
		setPerspView(Vector3d(eyex, eyey, eyez), Vector3d(cenx, ceny, cenz), Vector3d(upx, upy, upz));
	}

				// Change camera settings to get specified view
	void switchTo(VPView v)	{
		view = v;
		camera.setCenter(0,0,0);
		camera.setNearFar(1,1000);
		if ( view == VPPersp ) {
			camera.makePerspective();
			camera.setPerspectiveViewVolume(60.0,double(width)/double(height));
		}
		else {
			camera.makeOrthographic();
			camera.setOrthographicViewVolume(2,2,true);
			transform.reset();
		}
		switch ( view )	{
			case VPPersp : setPerspView(50,50,50, 0,0,0, 0,1,0);
				break;
			case VPFront : camera.setEye(0,0,100); camera.setUpVector(0,1,0);
				break;
			case VPRight : camera.setEye(100,0,0); camera.setUpVector(0,1,0);
				break;
			case VPTop : camera.setEye(0,100,0); camera.setUpVector(0,0,-1);
				break;
			case VPBack : camera.setEye(0,0,-100); camera.setUpVector(0,1,0);
				break;
			case VPLeft : camera.setEye(-100,0,0); camera.setUpVector(0,1,0);
				break;
			case VPBottom : camera.setEye(0,-100,0); camera.setUpVector(0,0,1);
				break;
		}
	}

	Viewport(int w, int h)
		: width(w), height(h),
		arcball(), trcontrol(), zoomcontrol(), dollycontrol(10.0),
		transform(), currenttr(VPNone), view(VPPersp), camera() {
		switchTo(view);
	}

	Viewport(int w, int h, VPView v)
		: width(w), height(h), arcball(), trcontrol(), zoomcontrol(), dollycontrol(10.0),
		transform(), currenttr(VPNone), view(v), camera() {
		switchTo(view);
	}

	virtual ~Viewport()
		{}

	VPTransformType current(void) const
	{
		return currenttr;
	}

				// Apply the GL transformation matrices
				// This function should be called when the view needs to be setup
	virtual void reshape(void)
	{
		glViewport(0,0,width,height);
		camera.applyTransform();
	}

				// Convert mouse coordinates to real-world coordinates
				// Give mouse x and y in x and y respectively
	void mouseToViewport(double& x, double& y, double& z)
	{
		x = (2.0*x/width) - 1.0; y = (-2.0*y/height) + 1.0;
		camera.adjust(x,y,z);
		Vector4d p(x,y,z,1), p1;
		Matrix4x4 tmat = inverse(transform.matrix());
		double w;
		p1 = tmat * p;
		p1.get(x,y,z,w); x /= w; y /= w; z /= w;
	}

				// Resize the viewport. Automatically calls reshape
				// This function should be called when the window using this viewport is resized
	virtual void resize(int w, int h)
	{
		width = w; height = h;
		camera.setAspect(double(width)/double(height));
		reshape();
	}

				// Handle rotation by mouse
	virtual void handle_rotate(VPMouseEvent event, int event_x, int event_y)
	{
						// Don't do anything if we aren't in a neutral state or aren't rotating
		if ( currenttr != VPNone && currenttr != VPRotate ) return;

		currenttr = VPRotate;
		if ( view == VPPersp )                        // No rotations for ortho views
		{
			double x,y;
								// Transform coords to lie between -1 to 1
			x = ( double(event_x << 1) / width ) - 1.0;
			y = ( -double(event_y << 1) / height ) + 1.0;

			arcball.mouse(x,y); arcball.update();

			if ( event == VPPush ) arcball.beginDrag();
			// else if (event == VPDrag) transform.rotate(arcball.quat_value());  // Update the combined transformation
			else if ( event == VPRelease )
			{
				arcball.endDrag();
				transform.rotate(arcball.quat_value());  // Update the combined transformation
				arcball.reset();
			}
		}
		if ( event == VPRelease ) currenttr = VPNone;
	}

				// Handle panning by mouse
	virtual void handle_pan(VPMouseEvent event, int event_x, int event_y)
	{
						// Don't do anything if we aren't in a neutral state or aren't panning
		if ( currenttr != VPNone && currenttr != VPPan ) return;

		currenttr = VPPan;

		double x,y,z;

						// Transform coords to lie between -1 to 1
		x = ( double(event_x << 1) / width ) - 1.0;
		y = ( -double(event_y << 1) / height ) + 1.0;

						// Adjust the x and y values so that moving mouse by 1 pixel
						// on screen moves point under mouse by 1 pixel
		camera.adjust(x,y,z);
		trcontrol.mouse(x,y,z); trcontrol.update();

		if ( event == VPPush ) trcontrol.beginDrag();
		// else if (event == VPDrag) transform.translate(trcontrol.trans_value());// Update the combined transformation
		else if ( event == VPRelease )
		{
			trcontrol.endDrag();
			transform.translate(trcontrol.trans_value());// Update the combined transformation
			trcontrol.reset(); currenttr = VPNone;
		}
	}

				// Handle zooming by mouse (only x movement is used)
	virtual void handle_zoom(VPMouseEvent event, int event_x, int event_y=0)
	{
						// Don't do anything if we aren't in a neutral state or aren't zooming
		if ( currenttr != VPNone && currenttr != VPZoom ) return;

		currenttr = VPZoom;
		double z;
						// Transform coords to lie between -1 to 1
		z = ( double(event_x << 1) / width ) - 1.0;
						// Currently both orthographic and perspective zoom is handled by same
						// controller which simply does a scaling
		zoomcontrol.mouse(z); zoomcontrol.update();

		if ( event == VPPush ) zoomcontrol.beginDrag();
		// else if (event == VPDrag) transform.scale(zoomcontrol.zoom_value());// Update the combined transformation
		else if ( event == VPRelease )
		{
			zoomcontrol.endDrag();
			transform.scale(zoomcontrol.zoom_value());// Update the combined transformation
			zoomcontrol.reset(); currenttr = VPNone;
		}
	}

				// Handle dollying by mouse (only x movement is used)
	virtual void handle_dolly(VPMouseEvent event, int event_x, int event_y=0)
	{
						// For orthographic views, call handle_zoom, since handle_dolly has no effect
		if ( view != VPPersp ) handle_zoom(event,event_x,event_y);
		else
		{
								// Don't do anything if we aren't in a neutral state or aren't dollying
			if ( currenttr != VPNone && currenttr != VPDolly ) return;

			currenttr = VPDolly;

			double z;

								// Transform coords to lie between -1 to 1
			z = ( double(event_x << 1) / width ) - 1.0;

			dollycontrol.mouse(z); dollycontrol.update();

			if ( event == VPPush ) dollycontrol.beginDrag();
			// else if (event == VPDrag ) 
			else if ( event == VPRelease )
			{
				dollycontrol.endDrag();
											// Shouldn't reset since the value is directly used for transformations
											// separate from other transformations
				currenttr = VPNone;
			}
		}
	}

	virtual bool send_to_current(VPMouseEvent event, int event_x, int event_y)
	{
						// Send the given event to the subroutine handling the current transformation
		bool handled = true;
		switch ( currenttr )
		{
			case VPPan :
			handle_pan(event,event_x,event_y); break;
			case VPZoom :
			handle_zoom(event,event_x,event_y); break;
			case VPRotate :
			handle_rotate(event,event_x,event_y); break;
			case VPDolly :
			handle_dolly(event,event_x,event_y); break;
			case VPNone :
			default :
			handled = false;
		}
		return handled;
	}

	Transformation apply_transform(void)                   // Apply the transformation
	{
		double mat[16];
			// Do the dollying separately before everything else
		glTranslated(0,0,dollycontrol.dolly_value());
		switch ( currenttr )
		{
			case VPPan :
			trcontrol.value().fillArrayColumnMajor(mat);
			glMultMatrixd(mat);
			break;
			case VPZoom :
			zoomcontrol.value().fillArrayColumnMajor(mat);
			glMultMatrixd(mat);
			break;
			case VPRotate :
			arcball.value().fillArrayColumnMajor(mat);
			glMultMatrixd(mat);
			break;
			case VPDolly :
			case VPNone :
			default : break;
		}
		///	transform.apply();
		transform.fillArrayColumnMajor(mat);
		glMultMatrixd(mat);
		// transform.invert();
		// transform.apply();
		camera.setPos(inverse(transform.matrix())*camera.getEye());
		return transform;
	}
};

#endif /* #ifndef _VIEWPORT_HH_ */
