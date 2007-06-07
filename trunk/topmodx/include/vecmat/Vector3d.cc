/* $Id: Vector3d.cc,v 1.3 2000/04/30 02:50:08 vinod Exp $ */

// Definitions for non-inline functions in Vector3d.hh

#include "Vector3d.hh"
#include "Vector2d.hh"
#include "Vector4d.hh"

void Vector3d :: copyFrom(const Vector2d& vec)
{
  elem[0] = vec[0]; elem[1] = vec[1]; elem[2] = 0.0;
}

void Vector3d :: copyFrom(const Vector4d& vec)
{
  elem[0] = vec[0]; elem[1] = vec[1]; elem[2] = vec[2];
}

/*
  $Log: Vector3d.cc,v $
  Revision 1.3  2000/04/30 02:50:08  vinod
  Moved cross product functions into Vector.cc

  Revision 1.2  2000/02/09 22:15:07  vinod
  Code modifications to take care of file/class renames

  Revision 1.1  2000/02/09 22:01:24  vinod
  Renamed

  Revision 1.1  2000/02/09 21:21:44  vinod
  Moved into VecMat directory

  Revision 1.1  1999/10/18 05:31:34  vinod
  Vectors using statically allocated arrays

*/
