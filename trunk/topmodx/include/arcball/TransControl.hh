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

/* $Id: TransControl.hh,v 2.3 2000/05/02 19:08:58 vinod Exp $ */

#ifndef _TRANS_CONTROL_HH_

#define _TRANS_CONTROL_HH_

// Class for a translation controller

#include "../vecmat/Vector3d.hh"
#include "../vecmat/Matrix4x4.hh"

class TransControl
{
  protected :

     Matrix4x4 mNow;                                   // Current transformation matrix
     bool dragging;
     Vector3d vNow, vDown;                             // Mouse translations/points
     Vector3d tNow, tDown;                             // Current translation

  public :

     TransControl()                           // Default constructor
       : mNow(), dragging(false), vNow(), vDown(), tNow(), tDown()
       {}

     TransControl(const TransControl& tc) // Copy constructor
       : mNow(tc.mNow), dragging(tc.dragging), vNow(tc.vNow), vDown(tc.vDown),
         tNow(tc.tNow), tDown(tc.tDown)
       {}

     ~TransControl()                          // Destructor
       {}

     TransControl& operator = (const TransControl& tc)
       {
         mNow = tc.mNow; dragging = tc.dragging; vNow = tc.vNow; vDown = tc.vDown;
         tNow = tc.tNow; tDown = tc.tDown;
         return (*this);
       }

     void reset(void)
       {
         tNow.reset(); tDown.reset(); mNow.reset();
       }
     
     void mouse(const Vector3d& pos)                   // Specify mouse position
       {
         vNow = pos;
       }

     void mouse(double x, double y, double z=0.0)      // Specify mouse position
       {
         vNow.set(x,y,z);
       }
     
     Matrix4_4 value(void) const                       // Get the translation matrix
       {
         return mNow;
       }

     Vector3d trans_value(void) const                  // Get the translation vector
       {
         return tNow;
       }
     
     void beginDrag(void)                              // Begin a drag
       {
         dragging = true; vDown = vNow;
       }
     
     void endDrag(void)                                // End a drag
       {
         dragging = false; tDown = tNow;
       }

     bool isDragging(void) const                       // Check dragging status
       {
         return dragging;
       }

     void update(void)                                 // Update the vectors and matrices
       {
         if ( dragging )
            {
              tNow = tDown; tNow += vNow; tNow -= vDown;

                 /*
                   NOTE : Matrix should be transposed before using in OpenGL
                 */
              mNow[0][3] = tNow[0];
              mNow[1][3] = tNow[1];
              mNow[2][3] = tNow[2];
            }
       }
};

#endif /* #ifndef _TRANS_CONTROL_HH_ */


/*
  $Log: TransControl.hh,v $
  Revision 2.3  2000/05/02 19:08:58  vinod
  Rotation matrix is now mathematically correct (transpose of GL)

  Revision 2.2  2000/03/07 22:41:02  vinod
  Added z coord with default 0 to mouse() function

  Revision 2.1  2000/03/03 19:37:45  vinod
  Moved from Graphics directory

  Revision 1.1  2000/03/03 00:50:54  vinod
  Translation(panning) controller

*/
