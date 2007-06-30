#include "Camera3.hh"


/* ***************** CAMERA *********************/
Camera::Camera()
 : mousex(0), mousey(0), pickw(0), pickh(0), viewport(NULL), pickmode(false) {
  EYE.set(0, 0, 10);
  CENTER.set(0, 0, 0);
  UP.set(0, 1, 0);
  HOMEEYE=EYE;
  HOMECENTER=CENTER;
  HOMEUP=UP;
  MOUSE.set(0,0,0);
  ALT_DOWN=SHIFT_DOWN=0;
	CONSTRAINED_AXIS.set(0,0,0);
}


Camera::Camera(Vector3d eye, Vector3d center, Vector3d up) 
 : mousex(0), mousey(0), pickw(0), pickh(0), viewport(NULL), pickmode(false) {
  Vector3d looking = normalized(center-eye);
  UP=normalized(up);
  HOMEUP=UP;
  UP=normalized(up-(up*looking)*looking);
  EYE=eye;
  HOMEEYE=EYE;
  CENTER=center;
  HOMECENTER=CENTER;
  MOUSE.set(0,0,0);
  ALT_DOWN=SHIFT_DOWN=0;
	CONSTRAINED_AXIS.set(0,0,0);
}


void Camera::Reset() {
  EYE = HOMEEYE;
  CENTER = HOMECENTER;
  UP = HOMEUP;
}

void Camera::HandleMouseEvent(Qt::MouseButton button, QEvent::Type state, int x, int y) {

  MOUSE.set(x,y,0);
  MOUSEBUTTON=button;
	CONSTRAINED_AXIS.set(0,0,0);
	// int mod = glutGetModifiers();
  if( QApplication::keyboardModifiers() == Qt::AltModifier){
    ALT_DOWN=1;
    SHIFT_DOWN=0;
  }
  else if( QApplication::keyboardModifiers() == (Qt::AltModifier | Qt::ShiftModifier) ){
	  ALT_DOWN=1;
    SHIFT_DOWN=1;
  }
	else{
    ALT_DOWN=0;
    SHIFT_DOWN=0;
  }
	if (state == QEvent::MouseButtonRelease){
		ALT_DOWN=0;
		SHIFT_DOWN=0;
	}
}


void Camera::SetCamera(){
	gluLookAt(EYE[0],EYE[1],EYE[2],CENTER[0],CENTER[1],CENTER[2],UP[0],UP[1],UP[2]);
}



/* ***************** ORTHO CAM ************************/

void OrthoCamera::DrawGrid(int size, int spacing){
	
  Vector3d looking = EYE-CENTER;
  Vector3d RIGHT = normalized(UP%looking);
  glDisable(GL_LIGHTING);
  glBegin(GL_LINES);
	  glColor3f(.7,.7,.7);
		for(int i=-size;i<=size;i+=spacing){
			Vector3d p1 = i*RIGHT+size*UP;
			Vector3d p2 = i*RIGHT-size*UP;
			Vector3d p3 = i*UP+size*RIGHT;
			Vector3d p4 = i*UP-size*RIGHT;
		  glVertex3f(p1[0],p1[1],p1[2]);
		  glVertex3f(p2[0],p2[1],p2[2]);
		  glVertex3f(p3[0],p3[1],p3[2]);
		  glVertex3f(p4[0],p4[1],p4[2]);
    }
  glEnd();
  glEnable(GL_LIGHTING);
	
}


OrthoCamera::OrthoCamera(unsigned char axis) {
	
	switch(axis)
	{
	  case 'x':
			EYE.set(1,0,0);
			CENTER.set(0,0,0);
			UP.set(0,1,0);
			break;
		case 'y':
			EYE.set(0,10,0);
			CENTER.set(0,0,0);
			UP.set(0,0,1);
			break;
		case 'z':
			EYE.set(0,0,1);
			CENTER.set(0,0,0);
			UP.set(0,1,0);
			break;
		default:
		  break;
	}
  HOMEEYE=EYE;
  HOMECENTER=CENTER;
  HOMEUP=UP;
	zoom=5;
}

void OrthoCamera::SetProjection(int WinX, int WinY) {
	glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
	float aspect = WinX/(float)WinY;
	if ( pickmode ){
		gluPickMatrix(mousex, mousey, pickw, pickh, viewport);
	}
  glOrtho(-zoom*aspect,zoom*aspect,-zoom,zoom,-10,10);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity( );
	SetCamera();
}



void OrthoCamera::HandleMouseMotion(int x, int y, int WinX, int WinY) {
  if(ALT_DOWN==1)	{
    Vector3d looking = EYE-CENTER;
    Vector3d RIGHT = normalized(UP%looking);
    float dist = norm(looking);
    float viewWidth = dist;
    float pixelToWorld = 2*zoom/WinY;
  
    if(MOUSEBUTTON==Qt::LeftButton) {
      // rotation not allowed in 2D
    }
	
    else if(MOUSEBUTTON==Qt::MidButton) {
      CENTER = CENTER - RIGHT*(x-MOUSE[0])*pixelToWorld + UP*(y-MOUSE[1])*pixelToWorld;
      EYE = EYE - RIGHT*(x-MOUSE[0])*pixelToWorld + UP*(y-MOUSE[1])*pixelToWorld;
    }
  
    else if(MOUSEBUTTON==Qt::RightButton) {
      zoom-=(x-MOUSE[0]+y-MOUSE[1])*pixelToWorld;
    }
  }
  
  MOUSE[0]=x;
  MOUSE[1]=y;
}

void OrthoCamera::HandleMouseWheel(int delta, int WinX, int WinY){
  
  Vector3d looking = EYE-CENTER;
  float dist = norm(looking);
  float viewWidth = dist;
  float pixelToWorld = 2*zoom/WinY;
  
  zoom-=(delta)*pixelToWorld;
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
void OrthoCamera::enterSelectionMode(double x, double y, double w, double h, GLint * vp) {
	mousex = x; mousey = y; pickw = w; pickh = h; viewport = vp;
	pickmode = true;
}

/*!
* \brief no longer in selection (or picking) mode
*/
void OrthoCamera::leaveSelectionMode(void) {
	pickmode = false; viewport = NULL;
}

/********************* PERSP CAM **********************/

void PerspCamera::DrawGrid(int size, int spacing) {
	glDisable(GL_LIGHTING);
  glBegin(GL_LINES);
  for(int i=-size;i<=size;i+=spacing){
	  glColor3f(.7,.7,.7);
		glVertex3f(i,0,size);
		glVertex3f(i,0,-size);
		glVertex3f(-size,0,i);
		glVertex3f(size,0,i);
  }
	glEnd();
  glEnable(GL_LIGHTING);  
}

void PerspCamera::SetProjection(int WinX, int WinY) {
	glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  if ( pickmode ){
		gluPickMatrix(mousex, mousey, pickw, pickh, viewport);
	}
	gluPerspective(FOV, WinX/(float)WinY, near, far);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity( );
	SetCamera();
}


void PerspCamera::HandleMouseMotion(int x, int y,int WinX, int WinY) {
  if(ALT_DOWN==1){
    Vector3d looking = EYE-CENTER;
    Vector3d RIGHT = normalized(UP%looking);
    float dist = norm(looking);
    float viewWidth = dist;
    float pixelToWorld = viewWidth/WinY;
    Vector3d AXIS;
    if(MOUSEBUTTON==Qt::LeftButton) {
			// 		  if(SHIFT_DOWN==1) {
			//         if(norm(CONSTRAINED_AXIS)==0){
			//           if(abs(x-MOUSE[0])>abs(y-MOUSE[1]))
			// 		  CONSTRAINED_AXIS=HOMEUP;
			// 		else
			// 		  CONSTRAINED_AXIS=RIGHT;
			// 	}
			// 	AXIS=((x-MOUSE[0])*UP*CONSTRAINED_AXIS+(y-MOUSE[1])*RIGHT*CONSTRAINED_AXIS)*normalized(CONSTRAINED_AXIS);
			// }
			// else
			//if shift is pressed then do a pan
			if (SHIFT_DOWN==1) {
				CENTER = CENTER - RIGHT*(x-MOUSE[0])*pixelToWorld + UP*(y-MOUSE[1])*pixelToWorld;
	      EYE = EYE - RIGHT*(x-MOUSE[0])*pixelToWorld + UP*(y-MOUSE[1])*pixelToWorld;
      }
			else {
	      AXIS = normalized((x-MOUSE[0])*UP + (y-MOUSE[1])*RIGHT);
				
	      Vector3d RtRot = normalized(AXIS%looking);
	      float omega = -.005;
	      float mouseDist = sqrt((x-MOUSE[0])*(x-MOUSE[0])+(y-MOUSE[1])*(y-MOUSE[1]));
	      EYE = looking*cos(omega*mouseDist)+norm(looking)*RtRot*sin(omega*mouseDist);
	      EYE=EYE+CENTER;
	      RIGHT = normalized(RIGHT - (RIGHT*HOMEUP)*HOMEUP);
	      UP = -normalized(RIGHT % looking);
			}
    }
	
    else if(MOUSEBUTTON==Qt::MidButton) {
      CENTER = CENTER - RIGHT*(x-MOUSE[0])*pixelToWorld + UP*(y-MOUSE[1])*pixelToWorld;
      EYE = EYE - RIGHT*(x-MOUSE[0])*pixelToWorld + UP*(y-MOUSE[1])*pixelToWorld;
    }
  
    else if(MOUSEBUTTON==Qt::RightButton)
      EYE=EYE-(normalized(looking))*(x-MOUSE[0]+y-MOUSE[1])*pixelToWorld;
  }
  MOUSE[0]=x;
  MOUSE[1]=y;

}

void PerspCamera::HandleMouseWheel(int delta, int WinX, int WinY) {
	Vector3d looking = EYE-CENTER;
	float dist = norm(looking);
  float viewWidth = dist;
  float pixelToWorld = viewWidth/WinY;
  
  EYE=EYE-(normalized(looking))*(delta)*pixelToWorld;
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
void PerspCamera::enterSelectionMode(double x, double y, double w, double h, GLint * vp) {
	mousex = x; mousey = y; pickw = w; pickh = h; viewport = vp;
	pickmode = true;
}

/*!
* \brief no longer in selection (or picking) mode
*/
void PerspCamera::leaveSelectionMode(void) {
	pickmode = false; viewport = NULL;
}
