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

/* $Id: BallMath.cc,v 2.0 2000/02/09 04:36:08 vinod Exp $ */

#include "BallMath.hh"

   /*
     Convert window coordinates to sphere coordinates.
   */
Vector3d mouseOnSphere(const Vector3d& mouse, const Vector3d& center, double radius)
{
  Vector3d ballMouse;
  register double magsqr;

  ballMouse[0] = (mouse[0] - center[0]) / radius;
  ballMouse[1] = (mouse[1] - center[1]) / radius;
  magsqr = sqr(ballMouse[0]) + sqr(ballMouse[1]);
  if (magsqr > 1.0)
     {
       register double scale = 1.0/sqrt(magsqr);
       ballMouse[0] *= scale; ballMouse[1] *= scale;
       ballMouse[2] = 0.0;
     }
  else
     {
       ballMouse[2] = sqrt(1.0 - magsqr);
     }
  return ballMouse;
}

   /*
     Construct a unit quaternion from two points on unit sphere
   */
Quaternion quatFromBallPoints(const Vector3d& from, const Vector3d& to)
{
  return Quaternion(from % to,from * to);
}

   /*
     Convert a unit quaternion to two points on unit sphere
     Assumes that the given quaternion is a unit quaternion
   */
void quatToBallPoints(const Quaternion& q, Vector3d& arcFrom, Vector3d& arcTo)
{
  double s;
  s = sqrt(sqr(q[0]) + sqr(q[1]));
  if (fabs(s) < 1.0e-5) arcFrom.set(0.0, 1.0, 0.0);
  else                  arcFrom.set(-q[1]/s, q[0]/s, 0.0);

  arcTo[0] = q[3]*arcFrom[0] - q[2]*arcFrom[1];
  arcTo[1] = q[3]*arcFrom[1] + q[2]*arcFrom[0];
  arcTo[2] = q[0]*arcFrom[1] - q[1]*arcFrom[0];
  if (q[3] < 0.0) arcFrom *= -1.0;
}

   /*
     Force sphere point to be perpendicular to axis.
   */
Vector3d constrainToAxis(const Vector3d& loose, const Vector3d& axis)
{
  Vector3d onPlane;
  register float norm;
  onPlane = loose - axis * (axis*loose);
  norm = normsqr(onPlane);
  if (norm > 0.0)
     {
       if (onPlane[2] < 0.0) onPlane *= -1.0;
       onPlane /= sqrt(norm);
     }
  else if ( fabs(axis[2]-1.0) < 1.0e-5 )
     {
       onPlane.set(1.0, 0.0, 0.0);
     }
  else
     {
       onPlane.set(-axis[1], axis[0], 0.0); normalize(onPlane);
     }
  return onPlane;
}

   /*
     Find the index of nearest arc of axis set.
   */
int nearestConstraintAxis(const Vector3d& loose, Vector3d * axes, int nAxes)
{
  Vector3d onPlane;
  register float max, dot;
  register int i, nearest;
  max = -1.0; nearest = 0;
  for (i=0; i < nAxes; i++)
     {
       onPlane = constrainToAxis(loose, axes[i]);
       dot = onPlane * loose;
       if ( dot > max )
          {
            max = dot; nearest = i;
          }
     }
  return nearest;
}

   /*
     Halve arc between unit vectors v1 and v2.
     Assumes that v1 and v3 are unit vectors
   */
Vector3d bisect(const Vector3d& v1, const Vector3d& v2)
{
  Vector3d v = v1 + v2;
  float Nv = normsqr(v);;

  if (Nv < 1.0e-5) v.set(0.0,0.0,1.0);
  else             v /= sqrt(Nv);
  return v;
}

/*
  $Log: BallMath.cc,v $
  Revision 2.0  2000/02/09 04:36:08  vinod
  Version change

  Revision 1.1.1.1  2000/01/21 18:37:29  vinod
  Arcball Class

*/
