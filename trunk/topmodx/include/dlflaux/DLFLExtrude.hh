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

#ifndef _DLFLEXTRUDE_H_
#define _DLFLEXTRUDE_H_

#include <DLFLCoreExt.hh>
#include <DLFLObject.hh>

namespace DLFL {

  DLFLFacePtr duplicateFace(DLFLObjectPtr obj, DLFLFacePtr fptr, double offset, double rot, double sf);
  DLFLFacePtr duplicateFace(DLFLObjectPtr obj, DLFLFacePtr fptr, const Vector3d& dir, double offset, double rot, double sf);

  DLFLFacePtr duplicateFacePlanarOffset(DLFLObjectPtr obj, DLFLFacePtr fptr, double offset, double rot, double thickness, bool fractionalthickness);
  DLFLFacePtr duplicateFacePlanarOffset(DLFLObjectPtr obj, DLFLFacePtr fptr, const Vector3d& dir, double offset, double rot, double thickness, bool fractionalthickness);

  /* API
  uint extrudeFace(DLFLObjectPtr obj, uint faceID, double d, int num, double rot, double sf = 1.0);
  uint extrudeFaceDS(DLFLObjectPtr obj, uint faceID, double d, int num, double twist = 0.0, double sf = 1.0);
  uint extrudeDualFace(DLFLObjectPtr obj, uint faceID, double d, int num, double rot=0.0, double sf=1.0, bool mesh=false);
  uint extrudeFaceDodeca(DLFLObjectPtr obj, uint faceID, double d, int num, double rot=0.0, double sf = 1.0, bool hexagonalize=false);
  uint extrudeFaceIcosa(DLFLObjectPtr obj, uint faceID, double d, int num, double rot=0.0,  double sf = 1.0);
  void stellateFace(DLFLObjectPtr obj, uint faceID, double d);
  void doubleStellateFace(DLFLObjectPtr obj, uint faceID, double d);
  end API */

  DLFLFacePtr extrudeFace(DLFLObjectPtr obj, DLFLFacePtr fptr, double d);
  DLFLFacePtr extrudeFace(DLFLObjectPtr obj, DLFLFacePtr fptr, double d, int num);
  DLFLFacePtr extrudeFace(DLFLObjectPtr obj, DLFLFacePtr fptr, double d, double rot, double sf = 1.0);
  DLFLFacePtr extrudeFace(DLFLObjectPtr obj, DLFLFacePtr fptr, double d, int num, double rot, double sf = 1.0);
  DLFLFacePtr extrudeFace(DLFLObjectPtr obj, DLFLFacePtr fptr, double d, const Vector3d& dir);
  DLFLFacePtr extrudeFace(DLFLObjectPtr obj, DLFLFacePtr fptr, double d, const Vector3d& dir, int num);
  DLFLFacePtr extrudeFace(DLFLObjectPtr obj, DLFLFacePtr fptr, double d, const Vector3d& dir, double rot, double sf = 1.0);
  DLFLFacePtr extrudeFace(DLFLObjectPtr obj, DLFLFacePtr fptr, double d, const Vector3d& dir, int num, double rot, double sf = 1.0);

  DLFLFacePtr extrudeFacePlanarOffset(DLFLObjectPtr obj, DLFLFacePtr fptr, double d, double rot, double thickness, bool fractionalthickness);
  DLFLFacePtr extrudeFacePlanarOffset(DLFLObjectPtr obj, DLFLFacePtr fptr, double d, const Vector3d& dir, double rot, double thickness, bool fractionalthickness);

  DLFLFacePtr extrudeFaceDS(DLFLObjectPtr obj, DLFLFacePtr fptr, double d, double twist = 0.0, double sf = 1.0);
  DLFLFacePtr extrudeFaceDS(DLFLObjectPtr obj, DLFLFacePtr fptr, double d, int num, double twist = 0.0, double sf = 1.0);
  DLFLFacePtr extrudeFaceDS(DLFLObjectPtr obj, DLFLFacePtr fptr, double d, const Vector3d& dir, double twist = 0.0, double sf = 1.0);
  DLFLFacePtr extrudeFaceDS(DLFLObjectPtr obj, DLFLFacePtr fptr, double d, const Vector3d& dir, int num, double twist = 0.0, double sf = 1.0);

  DLFLFacePtr extrudeDualFace(DLFLObjectPtr obj, DLFLFacePtr fptr, double d, double rot=0.0, double sf=1.0, bool mesh=false);
  DLFLFacePtr extrudeDualFace(DLFLObjectPtr obj, DLFLFacePtr fptr, double d, int num, double rot=0.0, double sf=1.0, bool mesh=false);
  DLFLFacePtr extrudeDualFace(DLFLObjectPtr obj, DLFLFacePtr fptr, double d, const Vector3d& dir, double rot=0.0, double sf=1.0, bool mesh=false);
  DLFLFacePtr extrudeDualFace(DLFLObjectPtr obj, DLFLFacePtr fptr, double d, const Vector3d& dir, int num, double rot=0.0, double sf=1.0, bool mesh=false);
    
  void stellateFace(DLFLObjectPtr obj, DLFLFacePtr fptr, double d);
  void stellateFace(DLFLObjectPtr obj, DLFLFacePtr fptr, double d, const Vector3d& dir);

  void doubleStellateFace(DLFLObjectPtr obj, DLFLFacePtr fptr, double d);

  DLFLFacePtr extrudeHexTileFace(DLFLObjectPtr obj, DLFLFacePtr fptr, double d, double rot=0.0, double sf = 1.0);

  DLFLFacePtr extrudeFaceDodeca(DLFLObjectPtr obj, DLFLFacePtr fptr, double d, int num, double rot=0.0, double sf = 1.0, bool hexagonalize=false);
  DLFLFacePtr extrudeFaceDodeca(DLFLObjectPtr obj, DLFLFacePtr fptr, double d, double rot=0.0, double sf = 1.0);
	
	DLFLFacePtr extrudeFaceDodeca(DLFLObjectPtr obj, DLFLFacePtr fptr, double angle, int num, double ex_dist1, double ex_dist2, double ex_dist3, bool hexagonalize);
	DLFLFacePtr extrudeFaceDodeca(DLFLObjectPtr obj, DLFLFacePtr fptr, double angle, double ex_dist1, double ex_dist2, double ex_dist3);
	DLFLFacePtr extrudeDodeca_Symmetric(DLFLObjectPtr obj, DLFLFacePtr fptr, double d, double rot, double sf, int modfactor);

	DLFLFacePtr extrudeFaceCubOcta(DLFLObjectPtr obj, DLFLFacePtr fptr, double angle, int num, double ex_dist1, double ex_dist2, double ex_dist3);
	DLFLFacePtr extrudeFaceCubOcta(DLFLObjectPtr obj, DLFLFacePtr fptr, double angle, double ex_dist1, double ex_dist2, double ex_dist3);
	DLFLFacePtr extrudeFaceSmallRhombiCubOcta(DLFLObjectPtr obj, DLFLFacePtr fptr, double angle, int num, double ex_dist1, double ex_dist2, double ex_dist3);
	DLFLFacePtr extrudeFaceSmallRhombiCubOcta(DLFLObjectPtr obj, DLFLFacePtr fptr, double angle, double ex_dist1, double ex_dist2, double ex_dist3);
	
	Vector3d GetRingPosition(DLFLVertexPtrList :: iterator vl_ring, int position, int numfaces);
	DLFLVertexPtr GetRingVertex(DLFLVertexPtrList :: iterator vl_ring, int position, int numfaces);
	
	DLFLFacePtr extrudeFaceIcosa(DLFLObjectPtr obj, DLFLFacePtr fptr, double angle, int num, double ex_dist1, double ex_dist2, double ex_dist3);
	DLFLFacePtr extrudeFaceIcosa(DLFLObjectPtr obj, DLFLFacePtr fptr, double angle, double ex_dist1, double ex_dist2, double ex_dist3);
	
  // DLFLFacePtr extrudeFaceIcosa(DLFLObjectPtr obj, DLFLFacePtr fptr, double d, int num, double rot=0.0,  double sf = 1.0);
  // DLFLFacePtr extrudeFaceIcosa(DLFLObjectPtr obj, DLFLFacePtr fptr, double d, double rot, double sf = 1.0);

  void extrudeFace(DLFLObjectPtr obj, DLFLFacePtr fptr);

	void extrudeFaceDome(DLFLObjectPtr obj, DLFLFacePtr fptr, double length, double rot=0.0, double sf=1.0);

} // end namespace DLFL

#endif // _DLFLEXTRUDE_H_
