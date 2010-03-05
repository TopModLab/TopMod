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

/* $Id: Vector2d.cc,v 1.2 2000/02/09 22:15:07 vinod Exp $ */

// Definitions for non-inline functions in Vector2d.hh

#include "Vector2d.hh"
#include "Vector3d.hh"
#include "Vector4d.hh"

void Vector2d :: copyFrom(const Vector3d& vec)
{
  elem[0] = vec[0]; elem[1] = vec[1];
}

void Vector2d :: copyFrom(const Vector4d& vec)
{
  elem[0] = vec[0]; elem[1] = vec[1];
}

/*
  $Log: Vector2d.cc,v $
  Revision 1.2  2000/02/09 22:15:07  vinod
  Code modifications to take care of file/class renames

  Revision 1.1  2000/02/09 22:01:23  vinod
  Renamed

  Revision 1.1  2000/02/09 21:21:43  vinod
  Moved into VecMat directory

  Revision 1.1  1999/10/18 05:31:34  vinod
  Vectors using statically allocated arrays

*/
