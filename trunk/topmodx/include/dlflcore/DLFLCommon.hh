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

/**
 * \file DLFLCommon.hh
 */

#ifndef _DLFL_COMMON_HH_
#define _DLFL_COMMON_HH_

// Common includes, definitions and declarations for DLFL

#include <vector>
#include <list>
#include <ext/hash_map>
#include <algorithm>
#include <iterator>
#include <Vector2d.hh>
#include <Vector3d.hh>
#include <Matrix4x4.hh>
#include <Base/Inlines.hh>
#include <Base/StreamIO.hh>
#include <Graphics/Color.hh>
#include <cstring>
#include <cstdlib>

// strstream is deprecated in the new standard.
// The GNU compiler complies with this but the SGI compiler doesn't
#ifdef __GNUG__
#include <sstream>
typedef stringstream StringStream;
#else
#include <strstream.h>
typedef strstream StringStream;
#endif

#ifdef __GNUG__
#include <fstream>
#else
#include <fstream.h>
#endif

// This is required if the standard versions of the STL header files are included
using namespace std;

// Forward declare all the classes and define typedefs for simplicity

namespace DLFL {

  class DLFLVertex;
  class DLFLFaceVertex;
  class DLFLEdge;
  class DLFLFace;
  class DLFLObject;
  class DLFLMaterial;
  //class TMPatch;
  //class TMPatchFace;

  typedef DLFLVertex * DLFLVertexPtr;
  typedef DLFLFaceVertex * DLFLFaceVertexPtr;
  typedef DLFLEdge * DLFLEdgePtr;
  typedef DLFLFace * DLFLFacePtr;
  typedef DLFLObject * DLFLObjectPtr;
  typedef DLFLMaterial * DLFLMaterialPtr;
  //typedef TMPatch * TMPatchPtr;
  //typedef TMPatchFace * TMPatchFacePtr;

  typedef vector<DLFLVertex> DLFLVertexArray;
  typedef vector<DLFLVertexPtr> DLFLVertexPtrArray;
  typedef list<DLFLVertex> DLFLVertexList;
  typedef list<DLFLVertexPtr> DLFLVertexPtrList;

  typedef vector<DLFLFaceVertex> DLFLFaceVertexArray;
  typedef vector<DLFLFaceVertexPtr> DLFLFaceVertexPtrArray;
  typedef list<DLFLFaceVertex> DLFLFaceVertexList;
  typedef list<DLFLFaceVertexPtr> DLFLFaceVertexPtrList;

  typedef vector<DLFLEdge> DLFLEdgeArray;
  typedef vector<DLFLEdgePtr> DLFLEdgePtrArray;
  typedef list<DLFLEdge> DLFLEdgeList;
  typedef list<DLFLEdgePtr> DLFLEdgePtrList;

  typedef vector<DLFLFace> DLFLFaceArray;
  typedef vector<DLFLFacePtr> DLFLFacePtrArray;
  typedef list<DLFLFace> DLFLFaceList;
  typedef list<DLFLFacePtr> DLFLFacePtrList;

  typedef vector<DLFLObject> DLFLObjectArray;
  typedef vector<DLFLObjectPtr> DLFLObjectPtrArray;
  typedef list<DLFLObject> DLFLObjectList;
  typedef list<DLFLObjectPtr> DLFLObjectPtrList;

  typedef vector<DLFLMaterial> DLFLMaterialArray;
  typedef vector<DLFLMaterialPtr> DLFLMaterialPtrArray;
  typedef list<DLFLMaterial> DLFLMaterialList;
  typedef list<DLFLMaterialPtr> DLFLMaterialPtrList;

  typedef vector<Vector4d> Vector4dArray;
  typedef vector<Vector4dPtr> Vector4dPtrArray;
  typedef list<Vector4d> Vector4dList;
  typedef list<Vector4dPtr> Vector4dPtrList;

  typedef vector<Vector4dArray> Vector4dGrid;
  typedef vector<Vector4dPtrArray> Vector4dPtrGrid;

  typedef vector<Vector3d> Vector3dArray;
  typedef vector<Vector3dPtr> Vector3dPtrArray;
  typedef list<Vector3d> Vector3dList;
  typedef list<Vector3dPtr> Vector3dPtrList;

  typedef vector<Vector3dArray> Vector3dGrid;
  typedef vector<Vector3dPtrArray> Vector3dPtrGrid;

  typedef vector<Vector2d> Vector2dArray;
  typedef vector<Vector2dPtr> Vector2dPtrArray;
  typedef list<Vector2d> Vector2dList;
  typedef list<Vector2dPtr> Vector2dPtrList;

  typedef vector<Vector2dArray> Vector2dGrid;
  typedef vector<Vector2dPtrArray> Vector2dPtrGrid;

  typedef vector<double> DoubleArray;
  typedef list<double> DoubleList;

  typedef vector<DoubleArray> DoubleGrid;

  typedef vector<int> IntArray;
  typedef list<int> IntList;

  typedef vector<IntArray> IntGrid;

  // Define some types/flags needed for subdivision surfaces and other operations
  enum DLFLVertexType { 
    VTNormal=0, 
    VTNewFacePoint=1, 
    VTNewEdgePoint=2, 
    VTNewPoint=3, 
    VTOld=4, 
    VTNewSub=5,
    VTWireInside=6, 
    VTWire=7, 
    VTWireSubD=8, 
    VTWireSubDInside=9 };

  enum DLFLFaceVertexType { 
    FVTNormal=0, 
    FVTNew=1, 
    FVTWire=2 };

  enum DLFLEdgeType { 
    ETNormal=0, 
    ETNew=1, 
    ETCollapse=2, 
    ETCollapseAux=3,
    ETOriginal=4, 
    ETChull=5, 
    ETdoNotDelete=6, 
    ETdoDelete=7 };

  enum DLFLFaceType { 
    FTNormal=0, 
    FTNew=1, 
    FTHole=2, 
    FTHoleInside=3,
    FTWire=4 };


  //-- Common utility functions --//

  /*
    Remove node specified by index from vector specified by array
    It is assumed that order of elements within the array is not important.
    Copy the last element to the specified index and delete the last element.
    Iterators pointing to the deleted index and to the last element are invalidated.
    Relative ordering of iterators is also changed for index, last element and adjacent nodes.
  */

  template <class T, class Alloc>
  inline void remove(vector<T,Alloc>& array, typename vector<T,Alloc>::size_type index) {
    array[index] = array.back();
    array.pop_back();
  }

  // Functions to clear lists/arrays of pointers, where the lists/arrays own
  // the objects pointed to.
  // These are needed since the STL functions merely erase the node

  void erase_dvp(DLFLVertexPtr vp);
  void erase_dfvp(DLFLFaceVertexPtr fvp);
  void erase_dep(DLFLEdgePtr ep);
  void erase_dfp(DLFLFacePtr fp);
  void erase_dop(DLFLObjectPtr op);
  void erase_dmp(DLFLMaterialPtr mp);

  void clear(DLFLVertexPtrArray& vparray);
  void clear(DLFLFaceVertexPtrArray& fvparray);
  void clear(DLFLEdgePtrArray& eparray);
  void clear(DLFLFacePtrArray& fparray);
  void clear(DLFLObjectPtrArray& oparray);
  void clear(DLFLMaterialPtrArray& mparray);

  void clear(DLFLVertexPtrList& vplist);
  void clear(DLFLFaceVertexPtrList& fvplist);
  void clear(DLFLEdgePtrList& eplist);
  void clear(DLFLFacePtrList& fplist);
  void clear(DLFLObjectPtrList& oplist);
  void clear(DLFLMaterialPtrList& mplist);

  struct eqstr {
    bool operator() ( int a, int b ) const {
      return a == b;
    }
  };

  typedef __gnu_cxx::hash<unsigned int> Hash;
  typedef __gnu_cxx::hash_map<unsigned int, unsigned int, Hash, eqstr> HashMap;

} // end namespace

#endif /* #ifndef _DLFL_COMMON_HH_ */
