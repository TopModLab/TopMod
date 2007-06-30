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
  Vector3d DefaultPos;															//!< used if the camera is reset to its initial Position
  Vector3d DefaultAim;															//!< used if the camera is reset to its initial Aim
  Vector3d DefaultUp;																//!< used if the camera is reset to its initial Up vector

  float DefaultAzim;																//!< used if the camera is reset to its initial azimuth
  float DefaultElev;																//!< used if the camera is reset to its initial elevation

  float CurrentAzim;																//!< the current Azimuth
  float CurrentElev;																//!< current elevation

  void Initialize();

		// For GL selection mode
	double     mousex, mousey, pickw, pickh;          //!< Selection region for gluPickMatrix
	GLint *    viewport;                              //!< The viewport
	bool       pickmode;                              //!< Are we in selection mode?

  char* m_name;																			//!< does the camera have a name?
public:
  Vector3d Pos;																			//!< the camera's position in world space
  Vector3d Aim;																			//!< the camera's aim vector, almost always 0,0,0
  Vector3d Up;																			//!< up vector must be perpendicular to position-aim

  float NearPlane;																	//!< does the camera have a name?
  float FarPlane;																		//!< does the camera have a name?
  float Fov;																				//!< does the camera have a name?

  Camera2();																				//!< default constructor

  Camera2(Vector3d P, Vector3d A, Vector3d U); 			//!< constructor setting up camera orientation

  Camera2(Vector3d P, Vector3d A, Vector3d U, float Near, float Far, float ViewAngle); //!< Constructor setting up camera orientation and view volume

  void SetClippingPlanes(float Near, float Far);	//!< sets the clipping planes of the view volume

  void SetFOV(float ViewAngle);										//!< sets the FOV, ViewAngle should be in degrees

  void SetPos(Vector3d P);											  //!< set routine for Position Vector
  void SetAim(Vector3d A);											  //!< set routine for the Aim Vector								
  void SetUp(Vector3d U);											  	//!< set routine for the Up Vector
	inline Vector3d getPos() { return Pos; };				//!< get routine for Position Vector
	inline Vector3d getAim(){ return Aim; };				//!< get routine for the Aim Vector		
	inline Vector3d getUp(){ return Up; };          //!< get routine for the Up Vector

  void Reset();																		//!< reset the camera to its initial position

  void SetCenterOfFocus(Vector3d NewAim);				  //!< focus camera to some input aim position

  void PerspectiveDisplay(int W, int H);

  void HandleMouseEvent(Qt::MouseButton button, QEvent::Type state, int x, int y, int delta = 0);
	void HandleMouseWheel(int delta);
  void HandleMouseMotion(int x, int y );
	
  const Camera2& operator=(const Camera2& cam);

  inline void setName( char* name ) { m_name = name; };
  inline char* getName( ) { return m_name; };

	void enterSelectionMode(double x, double y, double w, double h, GLint * vp); //!< \brief for OpenGL selection
	void leaveSelectionMode(void);																								//!< \brief for OpenGL selection

};

#endif //!< _CAMERA_H
