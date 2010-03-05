#ifndef _GL_++_HH_
#define _GL_++_HH_

// Wrapper functions which provide an easy interface for using gl functions
// with C++ classes (Vector, Matrix, Quaternion)
// These functions are for convenience only and not necessarily the most efficient

#include <Vector2d.hh>
#include <Vector3d.hh>
#include <Vector4d.hh>
#include <Matrix3x3.hh>
#include <Matrix4x4.hh>
#include <Quaternion.hh>

#ifdef __APPLE__
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#else
#include <GL/gl.h>
#include <GL/glu.h>
#endif

//--- glVertex functions ---//

inline void glVertex(const Vector2d& vec) {
  glVertex2dv(vec);
}

inline void glVertex(const Vector3d& vec) {
  glVertex3dv(vec);
}

inline void glVertex(const Vector4d& vec) {
  glVertex4dv(vec);
}


inline void glVertex2(const Vector2d& vec) {
  glVertex2dv(vec);
}

inline void glVertex2(const Vector3d& vec) {
  glVertex2dv(vec);
}

inline void glVertex2(const Vector4d& vec) {
  glVertex2dv(vec);
}


inline void glVertex3(const Vector2d& vec) {
  glVertex2dv(Vector3d(vec));
}

inline void glVertex3(const Vector3d& vec) {
  glVertex3dv(vec);
}

inline void glVertex3(const Vector4d& vec) {
  glVertex3dv(vec);
}


inline void glVertex4(const Vector2d& vec) {
  glVertex4dv(Vector4d(vec));
}

inline void glVertex4(const Vector3d& vec) {
  glVertex4dv(Vector4d(vec));
}

inline void glVertex4(const Vector4d& vec) {
  glVertex4dv(vec);
}


//--- glNormal functions ---//

inline void glNormal(const Vector2d& vec) {
  glNormal3dv(Vector3d(vec));
}

inline void glNormal(const Vector3d& vec) {
  glNormal3dv(vec);
}

inline void glNormal(const Vector4d& vec) {
  glNormal3dv(vec);
}


//--- glColor functions ---//

inline void glColor(const Vector3d& vec) {
  glColor3dv(vec);
}

inline void glColor(const Vector4d& vec) {
  glColor4dv(vec);
}


inline void glColor3(const Vector3d& vec) {
  glColor3dv(vec);
}

inline void glColor3(const Vector4d& vec) {
  glColor3dv(vec);
}


inline void glColor4(const Vector3d& vec) {
  glColor4d(vec[0],vec[1],vec[2],1.0);
}

inline void glColor4(const Vector4d& vec) {
  glColor4dv(vec);
}


//--- glTranslate functions ---//

inline void glTranslate(const Vector2d& vec) {
  glTranslated(vec[0],vec[1],0.0);
}

inline void glTranslate(const Vector3d& vec) {
  glTranslated(vec[0],vec[1],vec[2]);
}

inline void glTranslate(const Vector4d& vec) {
  glTranslated(vec[0],vec[1],vec[2]);
}


//--- glScale functions ---//

inline void glScale(const Vector2d& vec) {
  glScaled(vec[0],vec[1],1.0);
}

inline void glScale(const Vector3d& vec) {
  glScaled(vec[0],vec[1],vec[2]);
}

inline void glScale(const Vector4d& vec) {
  glScaled(vec[0],vec[1],vec[2]);
}


//--- glRotate functions ---//

inline void glRotate(const Vector3d& axis, double angle) {
  glRotated(angle,axis[0],axis[1],axis[2]);
}

inline void glRotate(double angle, const Vector3d& axis) {
  glRotated(angle,axis[0],axis[1],axis[2]);
}

inline void glRotate(const Vector4d& axisangle) {
  glRotated(axisangle[3],axisangle[0],axisangle[1],axisangle[2]);
}

inline void glRotate(const Quaternion& quat) {
  Vector3d axis;
  double angle;
  quat.getAxisAndAngle(axis,angle);
  glRotated(angle,axis[0],axis[1],axis[2]);
}


//--- gl matrix related functions ---//

inline void glLoadMatrix(const Matrix4x4& mat) {
  double m[16];
  mat.fillArrayColumnMajor(mat);
  glLoadMatrixd(m);
}

inline void glLoadMatrix(const Matrix3x3& mat) {
  glLoadMatrix(Matrix4x4(mat));
}


inline void glMultMatrix(const Matrix4x4& mat) {
  double m[16];
  mat.fillArrayColumnMajor(mat);
  glMultMatrix(m);
}

inline void glMultMatrix(const Matrix3x3& mat) {
  glMultMatrix(Matrix4x4(mat));
}

#endif /* #ifndef _GL_++_HH_ */

