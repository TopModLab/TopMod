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
*
* Short description of this file
*
* name of .hh file containing function prototypes
*
*/

#include "Camera2.hh"

/*!
* 
* \class Camera2
*	\brief Camera class from Chris Root used as an alternative to the original TopMod camera class
* 
* has an error when the aim and position vectors are not at the same y-value. \todo  fix the camera2 class up vector problem
* 
*/

float DeltaAzim;
float DeltaElev;
float LocalDeltaAzim;
float LocalDeltaElev;

int MouseStartX;
int MouseStartY;
int MousePrevX;
int MousePrevY;

float epsilon = 0.0001;

GLdouble MvMatrix[16];
GLdouble ProjMatrix[16];
GLint ViewPort[4];

int CameraMode = INACTIVE;

Vector3d PrevMousePos;

/*!
* 
* \brief rotate a vector around the X-axis
* 
* @param v the vector to rotate
* @param degree amount of degrees to rotate about the Y-axis
* 
*/
void RotateX(Vector3d *v, float degree){
	float c=cos(3.1415926*degree/180);
	float s=sin(3.1415926*degree/180);
	float v1=(*v)[1]*c-(*v)[2]*s;
	float v2=(*v)[1]*s+(*v)[2]*c;
	(*v)[1]=v1; (*v)[2]=v2;
}

/*!
* 
* \brief rotate a vector around the Y-axis
* 
* @param v the vector to rotate
* @param degree amount of degrees to rotate about the Y-axis
* 
*/
void RotateY(Vector3d *v, float degree){
	float c=cos(3.1415926*degree/180);
	float s=sin(3.1415926*degree/180);
	float v0=(*v)[0]*c+(*v)[2]*s;
	float v2=-(*v)[0]*s+(*v)[2]*c;
	(*v)[0]=v0; (*v)[2]=v2;
}

/*!
* 
* ArbitraryRotate() - rotate around an arbitrary coordinate system specified by
* 
* @param U the up vector
* @param V not sure
* @param W not sure
* @param degreeX amount of degrees to rotate about the Y-axis
* @param degreeY amount of degrees to rotate about the Y-axis
* @param point the new position of the camera passed by reference
* @param aim the camera aim vector
*/
void ArbitraryRotate(Vector3d U, Vector3d V, Vector3d W, float degreeX, float degreeY, Vector3d& point, Vector3d aim) {
	float cx=cos(M_PI*degreeX/180);
	float sx=sin(M_PI*degreeX/180);
	float cy=cos(M_PI*degreeY/180);
	float sy=sin(M_PI*degreeY/180); 

	Matrix4x4 trans;
	// Vector4d(double val1, double val2, double val3, double val4)

	trans.set( 	Vector4d(1.f, 0.f, 0.f, -aim[0]),
		Vector4d(0.f, 1.f, 0.f, -aim[1]),
		Vector4d(0.f, 0.f, 1.f, -aim[2]),
		Vector4d(0.f, 0.f, 0.f, 1.f));

	Matrix4x4 mat;
	mat.set( 	Vector4d(U[0], U[1], U[2], 0.f),
		Vector4d(V[0], V[1], V[2], 0.f),
		Vector4d(W[0], W[1], W[2], 0.f),
		Vector4d(0.f, 0.f, 0.f, 1.f) );

	Matrix4x4 rot;
	Vector4d pos(point[0], point[1], point[2], 1);

	pos = trans*pos;

	pos = mat*pos;

	rot.set( 	Vector4d(1,   0,  0, 0),
		Vector4d(0,  cx, sx, 0),
		Vector4d(0, -sx, cx, 0),
		Vector4d(0,   0,  0, 1) );

	pos = rot*pos;

	rot.set( 	Vector4d(cy, 0, sy,0),
		Vector4d(0, 1,  0,0),
		Vector4d(-sy, 0, cy,0),
		Vector4d(0, 0,  0, 1) );

	pos = rot*pos;

	pos = inverse( mat ) * pos;

	pos = inverse( trans) * pos;

	point.set( pos[0], pos[1], pos[2] );

}




/*!
* \brief default constructor... sets position to 0, 0, 5, aimed at the origin with the up vector set to the y axis
*/
Camera2::Camera2() 
: mousex(0), mousey(0), pickw(0), pickh(0), viewport(NULL), pickmode(false) {

	Pos.set(0, 5, 15);
	Aim.set(0, 5, 0);
	Up.set(0, 1, 0);

	// set default view volume
	NearPlane = 1.0;
	FarPlane = 1000.0;
	Fov = 60.0;

	Initialize();
}

/*!
* \brief constructor to set a camera to a desired orientation
* 
* @param P position in 3D
* @param A the aim coordinate
* @param U the up vector 
* 
*/
Camera2::Camera2(Vector3d P, Vector3d A, Vector3d U)
: mousex(0), mousey(0), pickw(0), pickh(0), viewport(NULL), pickmode(false) {

	Vector3d dir = A-P;
	normalize( dir );
	Vector3d up = U;
	normalize( up );
	float dot = dir*up;

	// up vector and aim vector aren't perpendicular
	if (fabs(dot) > epsilon) {
		fprintf (stderr, "Improper camera orientation. Can't create camera!\n");
		this->~Camera2();
		return;
	} else if (dir[2] != 0.0) {
		up[2] = -(up[0]*dir[0] + up[1]*dir[1])/dir[2];
	}

	Pos = P;
	Aim = A;
	Up = up;

	// set default view volume
	NearPlane = 1.0;
	FarPlane = 1000.0;
	Fov = 60.0;

	Initialize();
}

/*!
* \brief Constructor setting up camera orientation and view volume
* 
* @param P is position in 3D
* @param A is aim coordinate in 3D 
* @param U is up vector
* @param Near is near clipping plane
* @param Far is far clipping plane
* @param ViewAngle is field of view angle in degrees
* 
*/
Camera2::Camera2( Vector3d P, Vector3d A, Vector3d U, float Near, float Far, float ViewAngle) 
	: mousex(0), mousey(0), pickw(0), pickh(0), viewport(NULL), pickmode(false) {

	Vector3d aMinusP = A-P;
	normalize( aMinusP );
	Vector3d Up = U;

	// orientation of camera isn't perpendicular
	if( aMinusP*Up > epsilon) {
		fprintf (stderr, "Improper camera orientation. Can't create camera!\n");
		this->~Camera2();
		return;
	}

	Pos = P;
	Aim = A;

	NearPlane = Near;
	FarPlane = Far;
	Fov = ViewAngle;

	Initialize();
}


/*!
* \brief Initialize routine setting up defaults
*/
void Camera2::Initialize() {
	Vector3d tmp, tmp1, tmp2;
	Vector3d axisOrigin, updatePos;
	float dist;

	DefaultPos = Pos;
	DefaultAim = Aim;
	DefaultUp = Up;

	// find the angle around the x axis 
	updatePos = Pos - Aim;
	axisOrigin.set(updatePos[0], 0, 0);
	Vector3d t = axisOrigin - updatePos;
	dist =  sqrt(t.lengthsqr());
	tmp1.set(updatePos[0], 0, dist);

	tmp = updatePos;
	normalize(tmp);
	normalize(tmp1);

	CurrentElev = rad2deg( acos(tmp*tmp1) );

	// find the angle around the y axis
	axisOrigin.set(0, updatePos[1], 0);
	t = axisOrigin - updatePos;
	dist =  sqrt(t.lengthsqr());
	// dist = sqrt(lengthsqr(axisOrigin-updatePos));

	tmp2.set(0, updatePos[1], dist);
	normalize(tmp2);

	CurrentAzim = 360.0 - rad2deg(acos( tmp2*tmp) );

	DefaultElev = CurrentElev;
	DefaultAzim = CurrentAzim;

}

/*!
* \brief set functions for Pos vector
* 
* \note be careful with these because if you set either of them which causes the 
* orientation of the camera to be un-orthogonal, then you'll see problems.just 
* remember that (Aim - Pos).normalize() % Up == 0, or you'll see problems
* 
*/
void Camera2::SetPos(Vector3d P) {
	Pos = P;
}

/*!
* \brief set functions for Aim vector
* 
* \note be careful with these because if you set either of them which causes the 
* orientation of the camera to be un-orthogonal, then you'll see problems.just 
* remember that (Aim - Pos).normalize() % Up == 0, or you'll see problems
* 
*/
void Camera2::SetAim(Vector3d A) {
	Aim = A;
}

/*!
* \brief set functions for Up vector
* 
* \note be careful with these because if you set either of them which causes the 
* orientation of the camera to be un-orthogonal, then you'll see problems.just 
* remember that (Aim - Pos).normalize() % Up == 0, or you'll see problems
* 
*/
void Camera2::SetUp(Vector3d U) {
	Up = U;
}

/*!
* \brief sets the near and far clipping planes for the camera view
*/
void Camera2::SetClippingPlanes(float Near, float Far) {
	NearPlane = Near;
	FarPlane = Far;
}

/*!
* \brief sets the field of view of the camera, 
* 
* @param ViewAngle is in degrees
* 
*/
void Camera2::SetFOV(float ViewAngle) {
	Fov = ViewAngle;
}

/*!
* \brief resets the camera to its original orientation
*/
void Camera2::Reset() {
	Pos = DefaultPos;
	Aim = DefaultAim;
	Up = DefaultUp;

	CurrentElev = DefaultElev;
	CurrentAzim = DefaultAzim;
}

/*!
* \brief sets the camera's aim to be the given vector NewAim
*/
void Camera2::SetCenterOfFocus(Vector3d NewAim) {
	Vector3d dif = NewAim - Aim;

	Aim = NewAim;
	Pos = Pos + dif;
}

/*!
* \brief Enter selection mode and set the pick region for doing selection
* 
* @param x x-coordinate of mouse
* @param y y-coordinate of mouse
* @param w width of the selection region
* @param h height of the selection region
* @param vp the viewport to select from
* 
*/
void Camera2::enterSelectionMode(double x, double y, double w, double h, GLint * vp) {
	mousex = x; mousey = y; pickw = w; pickh = h; viewport = vp;
	pickmode = true;
}

/*!
* \brief no longer in selection (or picking) mode
*/
void Camera2::leaveSelectionMode(void) {
	pickmode = false; viewport = NULL;
}

/*!
* \brief function to use the camera as the opengl camera
* 
* @param W width of the viewport window
* @param H height of the viewport window
* 
*/
void Camera2::PerspectiveDisplay(int W, int H) {
	// set up the projection matrix
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	if ( pickmode ){
		gluPickMatrix(mousex, mousey, pickw, pickh, viewport);
	}
	gluPerspective(Fov, (float) W/(float) H, NearPlane, FarPlane);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity( );
	gluLookAt(Pos[0], Pos[1], Pos[2], Aim[0], Aim[1], Aim[2], Up[0], Up[1], Up[2]);
}

/*!
* \brief function that handles mouse events
* 
* @param button which button was pressed or released
* @param state was the mouse pressed, released or dragged?
* @param x the global x position of the mouse event
* @param y the global y position of the mouse event
* @param delta the amount the scroll wheel was moved if any
* 
*/
void Camera2::HandleMouseEvent(Qt::MouseButton button, QEvent::Type state, int x, int y, int delta) {
	float realy;
	double wx, wy, wz;

	// check to see if the ALT key has been used
	// int modstate = SDL_GetModState( );

	if (state == QEvent::MouseButtonRelease && CameraMode != INACTIVE) {
		// update the elevation and roll of the camera
		CurrentElev += DeltaElev;
		CurrentAzim += DeltaAzim;

		//printf("%f %f\n", CurrentElev, CurrentAzim);

		// reset the change in elevation and roll of the camera
		DeltaElev = DeltaAzim = 0.0;

		CameraMode = INACTIVE;
	} 
	else if (state == QEvent::MouseButtonPress && QApplication::keyboardModifiers() == Qt::AltModifier ) {
		// set the new mouse state
		MouseStartX = MousePrevX = x;
		MouseStartY = MousePrevY = y;

		// alt key and mouse button have been pressed, camera will move

		switch (button) {
			case Qt::LeftButton:
			// rotating camera
			CameraMode = ROTATE;
			break;
			case Qt::MidButton:
			if( QApplication::keyboardModifiers() == Qt::ShiftModifier ) { // Stupid little hack since Mac makes alt+LMB == MMB
				CameraMode = ROTATE;
			break;
		}
			// translating camera:
		CameraMode = TRANSLATE;

			// get the modelview and projection matrices for projection
			// of the mouse's cursor into 3D
		glGetIntegerv(GL_VIEWPORT, ViewPort);
		glGetDoublev(GL_MODELVIEW_MATRIX, MvMatrix);
		glGetDoublev(GL_PROJECTION_MATRIX, ProjMatrix);

			// viewport[3] is height of window in pixels
		realy = ViewPort[3] - y - 1;

			// project the aim of the camera into window coordinates
			// only concerned about getting the depth (wz) from here
		gluProject( Aim[0], Aim[1], Aim[2], MvMatrix, ProjMatrix, ViewPort, &wx, &wy, &wz);

		double mp0, mp1, mp2;
		mp0 = PrevMousePos[0]; mp1 = PrevMousePos[1]; mp2 = PrevMousePos[2];

			// from the depth found from the previous call, project the
			// mouse coordinates into 3D coordinates
		gluUnProject((GLfloat)x,(GLfloat)realy,wz,MvMatrix,ProjMatrix,ViewPort,&mp0, &mp1, &mp2);

		PrevMousePos[0] = (float) mp0;
		PrevMousePos[1] = (float) mp1;
		PrevMousePos[2] = (float) mp2;
		break;

		case Qt::RightButton:
			// zooming camera:
		CameraMode = ZOOM;
		break;
	}// end switch (button)
}
else if (state == QEvent::MouseButtonPress && QApplication::keyboardModifiers() == (Qt::AltModifier | Qt::ShiftModifier) ) {
	switch(button){
		case Qt::LeftButton:
			// translating camera:
		CameraMode = TRANSLATE;

			// get the modelview and projection matrices for projection
			// of the mouse's cursor into 3D
		glGetIntegerv(GL_VIEWPORT, ViewPort);
		glGetDoublev(GL_MODELVIEW_MATRIX, MvMatrix);
		glGetDoublev(GL_PROJECTION_MATRIX, ProjMatrix);

			// viewport[3] is height of window in pixels
		realy = ViewPort[3] - y - 1;

			// project the aim of the camera into window coordinates
			// only concerned about getting the depth (wz) from here
		gluProject( Aim[0], Aim[1], Aim[2], MvMatrix, ProjMatrix, ViewPort, &wx, &wy, &wz);

		double mp0, mp1, mp2;
		mp0 = PrevMousePos[0]; mp1 = PrevMousePos[1]; mp2 = PrevMousePos[2];

			// from the depth found from the previous call, project the
			// mouse coordinates into 3D coordinates
		gluUnProject((GLfloat)x,(GLfloat)realy,wz,MvMatrix,ProjMatrix,ViewPort,&mp0, &mp1, &mp2);

		PrevMousePos[0] = (float) mp0;
		PrevMousePos[1] = (float) mp1;
		PrevMousePos[2] = (float) mp2;
		break;
	}; 
}
}

/*! 
* \brief function that handles mouse scroll wheel events
*	
* @param delta the amount the scroll wheel moved in degrees. i think.
* 
*/
void Camera2::HandleMouseWheel(int delta){
		// mouse wheel scrolled, sent delta in 1/8ths of a degree
			// camera is zooming in
	float z = (float) delta/3000.0;

	Vector3d dir = Aim - Pos;

	if (sqrt(dir.lengthsqr()) < 0.1 && z > 0) {
				// move the aim position too when you get in really close
		z *= 10.0;
		Aim = Aim + z*dir;
	}
			// update the new position
	Pos = Pos + z*dir;	
}

/*! 
* \brief function that handles mouse movements
*	
* @param x the global x position of the mouse event
* @param y the global y position of the mouse event
* 
*/
void Camera2::HandleMouseMotion(int x, int y ) {
	int mouse_dx, mouse_dy, d;
	float z;
	Vector3d MousePos, dir;
	Vector3d WindowX, WindowY, WindowZ;
	float realy;
	double wx, wy, wz;

	if (CameraMode != INACTIVE) {

		// find the greates change in mouse position 
		mouse_dx = x - MousePrevX;
		mouse_dy = y - MousePrevY;

		if (abs(mouse_dx) > abs(mouse_dy)) 
			d = mouse_dx;
		else
			d = mouse_dy;

		switch (CameraMode) {
			case ZOOM:
			// camera is zooming in
			z = (float) d/100.0;

			dir = Aim - Pos;

			if (sqrt(dir.lengthsqr()) < 0.1 && z > 0) {
				// move the aim position too when you get in really close
				z *= 10.0;
				Aim = Aim + z*dir;
			}

			// update the new position
			Pos = Pos + z*dir;
			break;
			case ROTATE:
			// camera is rotating
			// get rate of change in screen coordinates from when the 
			// mouse was first pressed
			DeltaAzim = ((float) (x - MouseStartX))/3.0;
			DeltaElev = ((float) (y - MouseStartY))/3.0;

			// get rate of change in screen coordinate from prev mouse pos
			LocalDeltaAzim = ((float) mouse_dx)/3.0;
			LocalDeltaElev = ((float) mouse_dy)/3.0;

			// rotate the window coordinate system by the rate of change
			// from the onset of the mouse event

			// got this small section of code from Dr. House
			WindowX.set(1, 0, 0);
			WindowY.set(0, 1, 0);

			RotateX(&WindowX, CurrentElev+DeltaElev);
			RotateY(&WindowX, CurrentAzim+DeltaAzim);
			WindowX[2] = -WindowX[2];

			RotateX(&WindowY, CurrentElev+DeltaElev);
			RotateY(&WindowY, CurrentAzim+DeltaAzim);
			WindowY[2] = -WindowY[2];

			WindowZ = WindowX%WindowY;
			normalize(WindowZ);

			ArbitraryRotate(WindowX, WindowY, WindowZ, 
				LocalDeltaElev, 0, Pos, Aim);

			ArbitraryRotate(Vector3d(1, 0, 0), Vector3d(0, 1, 0), 
				Vector3d(0, 0, 1), 0, -LocalDeltaAzim, Pos, Aim);

			Up = WindowY;
			normalize(Up);

			break;
	    case TRANSLATE:
	      // camera is translating
	      realy = ViewPort[3] - y - 1;

	      gluProject( Aim[0], Aim[1], Aim[2],  MvMatrix, ProjMatrix, ViewPort, &wx, &wy, &wz);

	      double mp0, mp1, mp2;
	      mp0 = MousePos[0]; mp1 = MousePos[1]; mp2 = MousePos[2];

	      gluUnProject((GLfloat) x, (GLfloat) realy, wz, MvMatrix, ProjMatrix, ViewPort,&mp0, &mp1, &mp2);

	      MousePos[0] = (float) mp0;
	      MousePos[1] = (float) mp1;
	      MousePos[2] = (float) mp2;

	      // move both the camera position and its aim coordinate
	      dir = MousePos - PrevMousePos;
	      Pos = Pos - dir;
	      Aim = Aim - dir;

	      PrevMousePos = MousePos;
	      break;
	    }

		MousePrevX = x;
		MousePrevY = y;
	}
}

/*!
* \brief equals operator
*/
const Camera2& Camera2::operator=(const Camera2& Cam) {
	Aim = Cam.Aim;
	Pos = Cam.Pos;
	Up = Cam.Up;

	return *this;
}
