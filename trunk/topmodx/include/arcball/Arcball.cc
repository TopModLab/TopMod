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

/* $Id: Arcball.cc,v 2.2 2000/05/02 19:08:30 vinod Exp $ */

#ifdef __APPLE__
#include <OpenGL/gl.h>
#else
#include <GL/gl.h>
#endif

#include "Arcball.hh"

#define LG_NSEGS 4
#define NSEGS (1<<LG_NSEGS)
#define CIRCSEGS 32
#define HALFCIRCSEGS 16
#define RIMCOLOR()    glColor3f(255, 255, 255)
#define FARCOLOR()    glColor3f(195, 127, 31)
#define NEARCOLOR()   glColor3f(255, 255, 63)
#define DRAGCOLOR()   glColor3f(127, 255, 255)
#define RESCOLOR()    glColor3f(195, 31, 31)

   /*
     Establish reasonable initial values for controller.
   */
Arcball :: Arcball()
{
  v3Center.set(0,0,0);
  dRadius = 1.0;
  dragging = false;
  axisSet = NoAxes;

  sets[CameraAxes] = new Vector3d[3];
  sets[BodyAxes] = new Vector3d[3];

     // All matrices are initially identity matrices
     // Camera axes are simply the X,Y and Z axes. They don't change
  sets[CameraAxes][0].set(1.0,0.0,0.0); // X axis for camera
  sets[CameraAxes][1].set(0.0,1.0,0.0); // Y axis for camera
  sets[CameraAxes][2].set(0.0,0.0,1.0); // Z axis for camera

     // Body axes are same as camera axes initially. Body axes are the rows of the
     // Rotation matrix. Uses functions to convert Vector4d to Vector3d
     // Need to use the vector library
  sets[BodyAxes][0] = mNow[0];
  sets[BodyAxes][1] = mNow[1];
  sets[BodyAxes][2] = mNow[2];
}

Arcball :: Arcball(const Vector3d& cen, double rad)
{
  v3Center = cen;
  dRadius = rad;
  dragging = false;
  axisSet = NoAxes;

  sets[CameraAxes] = new Vector3d[3];
  sets[BodyAxes] = new Vector3d[3];

     // All matrices are initially identity matrices
     // Camera axes are simply the X,Y and Z axes. They don't change
  sets[CameraAxes][0].set(1.0,0.0,0.0); // X axis for camera
  sets[CameraAxes][1].set(0.0,1.0,0.0); // Y axis for camera
  sets[CameraAxes][2].set(0.0,0.0,1.0); // Z axis for camera

     // Body axes are same as camera axes initially. Body axes are the rows of the
     // Rotation matrix. Uses functions to convert Vector4d to Vector3d
     // Need to use the vector library
  sets[BodyAxes][0] = mNow[0];
  sets[BodyAxes][1] = mNow[1];
  sets[BodyAxes][2] = mNow[2];
}

Arcball :: Arcball(const Arcball& ab)
{
     // Copy only the center and radius. initialize to same as new arcball
  v3Center = ab.v3Center;
  dRadius = ab.dRadius;

  dragging = false;
  axisSet = NoAxes;

  sets[CameraAxes] = new Vector3d[3];
  sets[BodyAxes] = new Vector3d[3];

     // Camera axes are simply the X,Y and Z axes. They don't change
  sets[CameraAxes][0].set(1.0,0.0,0.0); // X axis for camera
  sets[CameraAxes][1].set(0.0,1.0,0.0); // Y axis for camera
  sets[CameraAxes][2].set(0.0,0.0,1.0); // Z axis for camera

     // Body axes are same as camera axes initially
  sets[BodyAxes][0] = mNow[0];
  sets[BodyAxes][1] = mNow[1];
  sets[BodyAxes][2] = mNow[2];
}

Arcball :: ~Arcball()
{
  delete [] sets[CameraAxes];
  delete [] sets[BodyAxes];
}

Arcball& Arcball :: operator = (const Arcball& ab)
{
     // Copy only the center and radius. Reset rest to same as new arcball
  v3Center = ab.v3Center;
  dRadius = ab.dRadius;

  dragging = false;
  axisSet = NoAxes;

  sets[CameraAxes] = new Vector3d[3];
  sets[BodyAxes] = new Vector3d[3];

     // Camera axes are simply the X,Y and Z axes. They don't change
  sets[CameraAxes][0].set(1.0,0.0,0.0); // X axis for camera
  sets[CameraAxes][1].set(0.0,1.0,0.0); // Y axis for camera
  sets[CameraAxes][2].set(0.0,0.0,1.0); // Z axis for camera

     // Body axes are same as camera axes initially
  sets[BodyAxes][0] = mNow[0];
  sets[BodyAxes][1] = mNow[1];
  sets[BodyAxes][2] = mNow[2];

  return (*this);
}
  
   /*
     Using vDown, vNow, dragging, and axisSet, compute rotation etc.
   */
void Arcball :: update(void)
{
  vFrom = mouseOnSphere(vDown, v3Center, dRadius);
  vTo = mouseOnSphere(vNow, v3Center, dRadius);
  if (dragging)
     {
       if ( axisSet != NoAxes)
          {
	    vFrom = constrainToAxis(vFrom, sets[axisSet][axisIndex]);
	    vTo = constrainToAxis(vTo, sets[axisSet][axisIndex]);
	}
       qDrag = quatFromBallPoints(vFrom, vTo);
       qNow = qDrag * qDown;
     }
  else
     {
       if ( axisSet != NoAxes)
          axisIndex = nearestConstraintAxis(vTo, sets[axisSet], 3);
     }
     /*
       NOTE : Matrix should be transposed before using in OpenGL
     */
  mNow = toMatrix4(qNow);
}

static void glVertexV3(const Vector3d& pt)
{
  glVertex3f(pt[0],pt[1],pt[2]);
}

static void unitCircle(void)
{
     // Draw a unit circle in the XY plane, centered at the origin
  float dtheta = 2.0*M_PI/32.0;
  
  glBegin(GL_LINE_LOOP);
  for (int i=0; i < 32; ++i)
     glVertex3f(cosf(i*dtheta),sinf(i*dtheta),0.0);
  glEnd();
}
  
   /*
     Draw the controller with all its arcs
   */
void Arcball :: draw(void) const
{
  glMatrixMode(GL_PROJECTION);
  glPushMatrix();
  glLoadIdentity();
  glOrtho(-1.0,1.0,-1.0,1.0,-1.0,1.0);
  
  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
  glLoadIdentity();
  {
    RIMCOLOR();
    glScaled(dRadius,dRadius,dRadius);
    unitCircle();

    drawConstraints();
    drawDragArc();
  }
  glPopMatrix();
  glMatrixMode(GL_PROJECTION);
  glPopMatrix();
  glMatrixMode(GL_MODELVIEW);
}

   /*
     Draw an arc defined by its ends.
   */
void Arcball :: drawAnyArc(const Vector3d& vFrom, const Vector3d& vTo) // static
{
  int i;
  Vector3d pts[NSEGS+1];
  double dot;

  pts[0] = vFrom;
  pts[1] = pts[NSEGS] = vTo;
  for (i=0; i < LG_NSEGS; ++i) pts[1] = bisect(pts[0], pts[1]);

  dot = 2.0*(pts[0]*pts[1]);
  for (i=2; i < NSEGS; ++i)
     pts[i] = pts[i-1]*dot - pts[i-2];

  glBegin(GL_LINE_STRIP);
  for (i=0; i <= NSEGS; ++i)
     glVertexV3(pts[i]);
  glEnd();
}

   /*
     Draw the arc of a semi-circle defined by its axis.
   */
void Arcball :: drawHalfArc(const Vector3d& n)         // static
{
  Vector3d p, m;

  if ( fabs(n[2]-1.0) > 1.0e-5 )
     {
       p[0] = n[1]; p[1] = -n[0];
       normalize(p);
     }
  else
     {
       p[0] = 0; p[1] = 1;
     }
  m = p % n;
  drawAnyArc(p, m);
  drawAnyArc(m, -p);
}

   /*
     Draw all constraint arcs.
   */
void Arcball :: drawConstraints(void) const
{
  if ( axisSet == NoAxes ) return;

  Vector3d axis;
  int i;

  for (i=0; i < 3; ++i)
     {
       if ( axisIndex != i)
          {
	    if (dragging) continue;
	    FARCOLOR();
          }
       else NEARCOLOR();
       axis = sets[axisSet][i];
       if ( fabs(axis[2]-1.0) < 1.0e-5 )
          unitCircle();
             //drawCircle(0.0, 0.0, 1.0);
       else
          drawHalfArc(axis);
     }
}

   /*
     Draw "rubber band" arc during dragging.
   */
void Arcball :: drawDragArc(void) const
{
  if ( dragging )
     {
       DRAGCOLOR();
       drawAnyArc(vFrom, vTo);
     }
}

//--- Math subroutines moved here from BallMath ---//

   /*
     Convert window coordinates to sphere coordinates.
   */
Vector3d Arcball :: mouseOnSphere(const Vector3d& mouse, const Vector3d& center, double radius)
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
Quaternion Arcball :: quatFromBallPoints(const Vector3d& from, const Vector3d& to)
{
  return Quaternion(from % to,from * to);
}

   /*
     Convert a unit quaternion to two points on unit sphere
     Assumes that the given quaternion is a unit quaternion
   */
void Arcball :: quatToBallPoints(const Quaternion& q, Vector3d& arcFrom, Vector3d& arcTo)
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
Vector3d Arcball :: constrainToAxis(const Vector3d& loose, const Vector3d& axis)
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
int Arcball :: nearestConstraintAxis(const Vector3d& loose, Vector3d * axes, int nAxes)
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
Vector3d Arcball :: bisect(const Vector3d& v1, const Vector3d& v2)
{
  Vector3d v = v1 + v2;
  float Nv = normsqr(v);;

  if (Nv < 1.0e-5) v.set(0.0,0.0,1.0);
  else             v /= sqrt(Nv);
  return v;
}


/*
  $Log: Arcball.cc,v $
  Revision 2.2  2000/05/02 19:08:30  vinod
  Rotation matrix is now mathematically correct (transpose of GL)

  Revision 2.1  2000/02/10 07:43:25  vinod
  Added 2 arg constructor. Changed draw to take no arguments

  Revision 2.0  2000/02/09 04:36:07  vinod
  Version change

  Revision 1.5  2000/02/09 04:32:34  vinod
  Moved math routines into Arcball class

  Revision 1.4  2000/02/03 22:56:18  vinod
  *** empty log message ***

  Revision 1.3  2000/02/01 06:15:40  vinod
  Added unitCircle, changed draw routines

  Revision 1.2  2000/01/21 22:12:38  vinod
  *** empty log message ***

  Revision 1.1.1.1  2000/01/21 18:37:29  vinod
  Arcball Class

*/
