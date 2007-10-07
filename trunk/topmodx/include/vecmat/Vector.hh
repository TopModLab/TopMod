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

/* $Id: Vector.hh,v 1.2 2000/02/09 22:15:07 vinod Exp $ */

#ifndef _VECTOR_HH_

#define _VECTOR_HH_

// Common stuff for all the static Vector classes
// Forward declarations for all the classes is included here, along with
// common include files and prototypes for functions that use different kinds
// of Vectors

// Common include files
#include "../Base/BaseObject.hh"
#include "../Base/StreamIO.hh"
#include "../Base/Constants.hh"
#include "../Base/Inlines.hh"

#include <cstdlib>
#include <cmath>

// Forward declarations
class Vector2d;
class Vector3d;
class Vector4d;

// Function prototypes

   // Cross products for 2d and 3d Vectors
Vector3d operator % (const Vector2d& vec1, const Vector2d& vec2);
Vector3d operator % (const Vector3d& vec1, const Vector2d& vec2);
Vector3d operator % (const Vector2d& vec1, const Vector3d& vec2);

#endif /* #ifndef _VECTOR_HH_ */


/*
  $Log: Vector.hh,v $
  Revision 1.2  2000/02/09 22:15:07  vinod
  Code modifications to take care of file/class renames

  Revision 1.1  2000/02/09 22:01:23  vinod
  Renamed

  Revision 1.1  2000/02/09 21:21:45  vinod
  Moved into VecMat directory

  Revision 1.1  1999/10/18 05:31:35  vinod
  Vectors using statically allocated arrays

*/
