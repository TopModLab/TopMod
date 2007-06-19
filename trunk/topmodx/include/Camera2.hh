#ifndef _CAMERA_HH
#define _CAMERA_HH

#include <QApplication>
#include <QEvent>

#ifdef __APPLE__
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#else
#include <GL/gl.h>
#include <GL/glu.h>
#endif
// #include <gmtl/gmtl.h>
// #include <gmtl/VecOps.h>
#include <Vector3d.hh>
#include <Matrix4x4.hh>
#include <math.h>
#include "Base/Inlines.hh"

#define INACTIVE 0
#define TRANSLATE 1
#define ROTATE 2
#define ZOOM 3

void ArbitraryRotate(Vector3d U, Vector3d V, Vector3d W, float degreeX, float degreeY, Vector3d& point, Vector3d aim);

class Camera2 {
private:
  // all variables starting with 'Default' hold the initial camera values
  // these values are used if the camera is reset to its initial state
  Vector3d DefaultPos;
  Vector3d DefaultAim;
  Vector3d DefaultUp;

  float DefaultAzim;
  float DefaultElev;

  float CurrentAzim;
  float CurrentElev;

  void Initialize();

		// For GL selection mode
	double     mousex, mousey, pickw, pickh;          // Selection region for gluPickMatrix
	GLint *    viewport;                              // The viewport
	bool       pickmode;                              // Are we in selection mode?

  char* m_name;
public:
  Vector3d Pos;
  Vector3d Aim; 
  Vector3d Up;

  float NearPlane;
  float FarPlane;
  float Fov;

  // constructors

  // default constructor
  Camera2();

  // constructor setting up camera orientation
  // P is position in 3D, A is the aim coordinate in 3D, U is the up vector
  Camera2(Vector3d P, Vector3d A, Vector3d U);

  // constructor setting up camera orientation and view volume
  // P is position in 3D, A is aim coordinate in 3D, U is up vector
  // Near is near clipping plane, Far is far clipping plane, 
  // ViewAngle is field of view angle in degrees
  Camera2(Vector3d P, Vector3d A, Vector3d U, float Near, float Far, float ViewAngle);

  // sets the clipping planes of the view volume
  void SetClippingPlanes(float Near, float Far);

  // sets the FOV, ViewAngle should be in degrees
  void SetFOV(float ViewAngle);	

  // set routines for Pos, Aim, and Up vector
  void SetPos(Vector3d P);
  void SetAim(Vector3d A);
  void SetUp(Vector3d U);
	inline Vector3d getPos() { return Pos; };
	inline Vector3d getAim(){ return Aim; };
	inline Vector3d getUp(){ return Up; };

  // reset the camera to its initial position
  void Reset();

  // focus camera to some input aim position
  void SetCenterOfFocus(Vector3d NewAim);

  // function to use the camera as the opengl camera
  // W and H are the width and height of the window
  void PerspectiveDisplay(int W, int H);

  // function that handles mouse events
  void HandleMouseEvent(Qt::MouseButton button, QEvent::Type state, int x, int y, int delta = 0);

	// function that handles mouse scroll wheel events
	void HandleMouseWheel(int delta);

  // function that handles mouse movements
  void HandleMouseMotion(int x, int y );
	
  const Camera2& operator=(const Camera2& cam);

  inline void setName( char* name ) { m_name = name; };
  inline char* getName( ) { return m_name; };

	//for OpenGL selection
	void enterSelectionMode(double x, double y, double w, double h, GLint * vp);
	void leaveSelectionMode(void);


};

#endif // _CAMERA_H
