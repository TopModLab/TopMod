/* $Id: BallMath.hh,v 1.1 2006/09/04 19:22:52 stuart Exp $ */

#ifndef _BALL_MATH_HH_

#define _BALL_MATH_HH_

#include <SVector3d.hh>
#include <Quaternion.hh>
#include <Inlines.hh>
#include <math.h>

Vector3d mouseOnSphere(const Vector3d& mouse, const Vector3d& ballCenter, double ballRadius);
Vector3d constrainToAxis(const Vector3d& loose, const Vector3d& axis);
int nearestConstraintAxis(const Vector3d& loose, Vector3d * axes, int nAxes);
Quaternion quatFromBallPoints(const Vector3d& from, const Vector3d& to);
void quatToBallPoints(const Quaternion& q, Vector3d& arcFrom, Vector3d& arcTo);
Vector3d bisect(const Vector3d& v1, const Vector3d& v2);

#endif /* #ifndef _BALL_MATH_HH_ */

/*
  $Log: BallMath.hh,v $
  Revision 1.1  2006/09/04 19:22:52  stuart
  Added includes and libs.

  Revision 1.2  2000/01/21 20:56:37  vinod
  Added const modifiers where necessary, to math cc file

  Revision 1.1.1.1  2000/01/21 18:37:29  vinod
  Arcball Class

*/
