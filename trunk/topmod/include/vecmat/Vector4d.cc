/* $Id: Vector4d.cc,v 1.2 2000/02/09 22:15:08 vinod Exp $ */

// Definitions for non-inline functions in Vector4d.hh

#include "Vector4d.hh"
#include "Vector2d.hh"
#include "Vector3d.hh"

void Vector4d :: copyFrom(const Vector2d& vec)
{
  elem[0] = vec[0]; elem[1] = vec[1]; elem[2] = elem[3] = 0.0;
}

void Vector4d :: copyFrom(const Vector3d& vec)
{
  elem[0] = vec[0]; elem[1] = vec[1]; elem[2] = vec[2]; elem[3] = 0.0;
}

/*
  $Log: Vector4d.cc,v $
  Revision 1.2  2000/02/09 22:15:08  vinod
  Code modifications to take care of file/class renames

  Revision 1.1  2000/02/09 22:01:25  vinod
  Renamed

  Revision 1.1  2000/02/09 21:21:44  vinod
  Moved into VecMat directory

  Revision 1.1  1999/10/18 05:31:35  vinod
  Vectors using statically allocated arrays

*/
