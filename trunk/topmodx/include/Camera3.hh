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

#ifndef Camera3_H
#define Camera3_H

#include <QApplication>
#include <QEvent>

#ifdef __APPLE__
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#else
#include <GL/gl.h>
#include <GL/glu.h>
#endif

#include <Vector3d.hh>
// #include <math.h>
// #include "Base/Inlines.hh"

/* ******************* Camera ************************
	Base class for camera
	
	You should create a camera either of type PerspCamera or OrthoCamera
	depending on the type of projection you want.  In both cases you can
	call an empty default constructor, or a constructor to specify a 
	specific Eye, Center and Up vector of your choice
	
	Perspective cameras also can have their fieldofview (FOV), and near
	and far clipping planes specified in the constructor if you don't like
	the defaults
	
	Orthographic cameras can only move and zoom.  The zoom is controlled 
	by the zoom parameter, not the distance from the EYE to the CENTER.
	This can be set in the constructor as well, or with the SetZoom() function
	
	Also, Orthographic cameras have a constructor that lets you pass a
	character for default 'x', 'y', and 'z' axes.
	
	Note that the constructor does NOT check to see if the up vector
	is perpendicular to the looking vector, but fortunately
	OpenGL is pretty robust about this.
*/

class Camera
{

public:
	Camera();
	Camera(Vector3d eye,Vector3d center,Vector3d up);
	~Camera(){};
	void setEye(Vector3d eye){EYE=eye; HOMEEYE=eye;};
	void setCenter(Vector3d center){CENTER=center; HOMECENTER=center;};
	void setUp(Vector3d up){UP=up; HOMEUP=up;};
	Vector3d getEye(){ return EYE; };
	Vector3d getCenter(){ return CENTER; };
	Vector3d getUp(){ return UP; };
	void Reset();
	void HandleMouseEvent(Qt::MouseButton button, QEvent::Type state, int x, int y);
	void SetCamera();
	
	virtual void DrawGrid(int size, int spacing)=0;
	virtual void SetProjection(int WinX, int WinY)=0;
	virtual void HandleMouseMotion(int x, int y, int WinX, int WinY)=0;
	virtual void HandleMouseWheel(int delta, int WinX, int WinY)=0;
	virtual void enterSelectionMode(double x, double y, double w, double h, GLint * vp)=0;
	virtual void leaveSelectionMode()=0;																								//!< \brief for OpenGL selection
	
	
protected:
  Vector3d EYE;
  Vector3d CENTER;
  Vector3d UP;
  Vector3d HOMEEYE;
  Vector3d HOMECENTER;
  Vector3d HOMEUP;

  Vector3d MOUSE;
  Qt::MouseButton MOUSEBUTTON;
  int ALT_DOWN;
  int SHIFT_DOWN;
	Vector3d CONSTRAINED_AXIS;
	
	// For GL selection mode
	double     mousex, mousey, pickw, pickh;          //!< Selection region for gluPickMatrix
	GLint *    viewport;                              //!< The viewport
	bool       pickmode;                              //!< Are we in selection mode?
	
};


class OrthoCamera : public Camera
{

public:
	
	OrthoCamera():zoom(5), Camera(){};
	OrthoCamera(unsigned char axis);
	OrthoCamera(Vector3d eye, Vector3d center, Vector3d up):zoom(5),Camera(eye,center,up){};
	OrthoCamera(Vector3d eye, Vector3d center, Vector3d up, float z):zoom(z),Camera(eye,center,up){};
	~OrthoCamera(){};

	void SetZoom(float z){zoom=z;};
	float GetZoom(){return zoom;};
	
	virtual void DrawGrid(int size=500, int spacing=1);
	virtual void SetProjection(int WinX, int WinY);
	virtual void HandleMouseMotion(int x, int y, int WinX, int WinY);
	virtual void HandleMouseWheel(int delta, int WinX, int WinY);
	virtual void enterSelectionMode(double x, double y, double w, double h, GLint * vp);
	virtual void leaveSelectionMode();																								//!< \brief for OpenGL selection
	
protected:
  float zoom;
};


class PerspCamera : public Camera
{

public:
	PerspCamera():FOV(60),nearplane(0.1),farplane(500),Camera(){};
	PerspCamera(Vector3d eye, Vector3d center, Vector3d up):FOV(60),nearplane(0.1),farplane(500),Camera(eye,center,up){};
	PerspCamera(Vector3d eye, Vector3d center, Vector3d up, float fov, float n, float f):FOV(fov),nearplane(n),farplane(f),Camera(eye,center,up){};
	~PerspCamera(){};
	
	virtual void DrawGrid(int size=10, int spacing=1);
	virtual void SetProjection(int WinX, int WinY);
	virtual void HandleMouseMotion(int x, int y, int WinX, int WinY);
	virtual void HandleMouseWheel(int delta, int WinX, int WinY);
	virtual void enterSelectionMode(double x, double y, double w, double h, GLint * vp);
	virtual void leaveSelectionMode();																								//!< \brief for OpenGL selection
	void setNearPlane(float n) { nearplane = n; };
	void setFarPlane(float f) { farplane = f; };
	void setFOV(float f){ FOV = f; };
	
protected:
  float FOV;
  float nearplane;
  float farplane;
};

#endif
