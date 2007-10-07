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

/* $Id: Vector.cc,v 1.1 2000/04/30 02:50:51 vinod Exp $ */

#include "Vector.hh"
#include "Vector2d.hh"
#include "Vector3d.hh"
#include "Vector4d.hh"

   // Cross product between 2D vectors
Vector3d operator % (const Vector2d& vec1, const Vector2d& vec2)
{
  Vector3d crossp, v1(vec1), v2(vec2);

  crossp = v1 % v2;
  return crossp;
}

   // Cross product between 3D and 2D vectors
Vector3d operator % (const Vector3d& vec1, const Vector2d& vec2)
{
  Vector3d crossp, v2(vec2);

  crossp = vec1 % v2;
  return crossp;
}

   // Cross product between 2D and 3D vectors
Vector3d operator % (const Vector2d& vec1, const Vector3d& vec2)
{
  Vector3d crossp, v1(vec1);

  crossp = v1 % vec2;
  return crossp;
}

/*
  $Log: Vector.cc,v $
  Revision 1.1  2000/04/30 02:50:51  vinod
  Common subroutines for the Vector classes

*/
