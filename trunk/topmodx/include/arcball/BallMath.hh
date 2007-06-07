/* $Id: BallMath.hh,v 2.0 2000/02/09 04:36:08 vinod Exp $ */

#ifndef _BALL_MATH_HH_

#define _BALL_MATH_HH_

#include "../vecmat/Vector3d.hh"
#include "../vecmat/Quaternion.hh"
#include "../Base/Inlines.hh"
#include <cmath>

Vector3d mouseOnSphere(const Vector3d& mouse, const Vector3d& ballCenter, double ballRadius);
Vector3d constrainToAxis(const Vector3d& loose, const Vector3d& axis);
int nearestConstraintAxis(const Vector3d& loose, Vector3d * axes, int nAxes);
Quaternion quatFromBallPoints(const Vector3d& from, const Vector3d& to);
void quatToBallPoints(const Quaternion& q, Vector3d& arcFrom, Vector3d& arcTo);
Vector3d bisect(const Vector3d& v1, const Vector3d& v2);

#endif /* #ifndef _BALL_MATH_HH_ */

/*
  $Log: BallMath.hh,v $
  Revision 2.0  2000/02/09 04:36:08  vinod
  Version change

  Revision 1.2  2000/01/21 20:56:37  vinod
  Added const modifiers where necessary, to math cc file

  Revision 1.1.1.1  2000/01/21 18:37:29  vinod
  Arcball Class

*/
