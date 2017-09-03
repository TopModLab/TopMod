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

/* $Id: Arcball.hh,v 2.7 2000/03/03 00:48:47 vinod Exp $ */

#ifndef _ARCBALL_HH_

#define _ARCBALL_HH_

#include "../vecmat/Vector3d.hh"
#include "../vecmat/Quaternion.hh"
#include "../Base/Inlines.hh"
#include <cmath>
#include "../vecmat/Matrix4x4.hh"

enum AxisSet { CameraAxes=0, BodyAxes=1, NoAxes=2 };

typedef Vector3d * Axes;

class Arcball
{
  protected :
     
    Vector3d v3Center;                                 // Center of ball
    double dRadius;                                    // Radius of ball
    Quaternion qNow, qDown, qDrag;
    Vector3d vNow, vDown, vFrom, vTo;
    Matrix4_4 mNow;
    bool dragging;
    Axes sets[2];
    AxisSet axisSet;
    int axisIndex;

  public :
     
     Arcball();                                        // Default constructor
     Arcball(const Vector3d& cen, double rad);         // 2 arg constructor
     Arcball(const Arcball& ab);                       // Copy constructor
     ~Arcball();                                       // Destructor

     Arcball& operator = (const Arcball& ab);          // Assignment operator

     void place(const Vector3d& cen, double rad)       // Place the arcball
       {
         v3Center = cen; dRadius = rad;
       }

     void place(double x, double y, double z, double rad) // Place the arcball
       {
         v3Center.set(x,y,z); dRadius = rad;
       }
     
     void center(const Vector3d& cen)                  // Set the center
       {
         v3Center = cen;
       }

     void center(double x, double y, double z)         // Set the center
       {
         v3Center.set(x,y,z);
       }
     
     void radius(double rad)                           // Set the radius
       {
         dRadius = rad;
       }

     void reset(void)
       {
         qDown.reset(); qNow.reset(); mNow.reset();
       }
     
     void mouse(const Vector3d& pos)                   // Specify mouse position
       {
         vNow = pos;
       }

     void mouse(double x, double y)                    // Specify mouse position
       {
         vNow.set(x,y,0);
       }
     
     void useSet(AxisSet set)                          // Use specified axis set
       {
         if ( !dragging ) axisSet = set;
       }
     
     void update(void);                                // Update the quats and matrices

     Vector3d center(void) const                       // Get the center
       {
         return v3Center;
       }

     double radius(void) const                         // Get the radius
       {
         return dRadius;
       }
     
     Matrix4_4 value(void) const                       // Get the rot matrix
       {
         return mNow;
       }

     Quaternion quat_value(void) const                 // Get the rot quaternion
       {
         return qNow;
       }
     
     void beginDrag(void)                              // Begin a drag
       {
         dragging = true; vDown = vNow;
       }
     
     void endDrag(void)                                // End a drag
       {
         dragging = false; qDown = qNow;
         sets[BodyAxes][0] = mNow[0];
         sets[BodyAxes][1] = mNow[1];
         sets[BodyAxes][2] = mNow[2];
       }

     bool isDragging(void) const                       // Check dragging status
       {
         return dragging;
       }
     
        // Draw the arcball
     void draw(void) const;                            // Draw the arcball

  private :
     
     static void drawAnyArc(const Vector3d& from, const Vector3d& to);
     static void drawHalfArc(const Vector3d& n);

     void drawConstraints(void) const;
     void drawDragArc(void) const;

  protected :

        // Math routines. Moved into class from BallMath
     static Vector3d mouseOnSphere(const Vector3d& mouse, const Vector3d& center, double radius);
     static Vector3d constrainToAxis(const Vector3d& loose, const Vector3d& axis);
     static int nearestConstraintAxis(const Vector3d& loose, Vector3d * axes, int nAxes);
     static Quaternion quatFromBallPoints(const Vector3d& from, const Vector3d& to);
     static void quatToBallPoints(const Quaternion& q, Vector3d& arcFrom, Vector3d& arcTo);
     static Vector3d bisect(const Vector3d& v1, const Vector3d& v2);
};

#endif /* #ifndef _ARCBALL_HH_ */


/*
  $Log: Arcball.hh,v $
  Revision 2.7  2000/03/03 00:48:47  vinod
  Fixed reset()

  Revision 2.6  2000/03/03 00:09:22  vinod
  Changed to use new include files
  Added reset function

  Revision 2.5  2000/02/10 08:38:01  vinod
  Added place, center, mouse functions which take double's

  Revision 2.4  2000/02/10 08:18:04  vinod
  Added access functions for center and radius

  Revision 2.3  2000/02/10 07:43:01  vinod
  Added 2 arg constructor.
  Changed draw to take no arguments. Added comments

  Revision 2.2  2000/02/10 05:43:30  vinod
  Added quat_value fn to get value of current Quaternion

  Revision 2.1  2000/02/10 04:47:48  vinod
  Added isDragging function

  Revision 2.0  2000/02/09 04:36:08  vinod
  Version change

  Revision 1.2  2000/02/09 04:32:34  vinod
  Moved math routines into Arcball class

  Revision 1.1.1.1  2000/01/21 18:37:29  vinod
  Arcball Class

*/
