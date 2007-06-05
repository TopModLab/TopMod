/* $Id: DLFLCommon.hh,v 4.4 2004/01/20 22:16:36 vinod Exp $ */

#ifndef _DLFL_COMMON_HH_

#define _DLFL_COMMON_HH_

// Common includes, definitions and declarations for DLFL

#include <vector>
#include <list>
#include <algorithm>
#include <iterator>

#include "include/vecmat/Vector2d.hh"
#include "include/vecmat/Vector3d.hh"
#include "include/vecmat/Matrix4x4.hh" 
#include "include/Base/Inlines.hh"
#include "include/Base/StreamIO.hh"
#include "include/Graphics/Color.hh"
#include <string.h>
#include <stdlib.h>
//lights
#include "include/Light/Light.hh"
#include "include/Light/SpotLight.hh"
#include "include/Light/PointLight.hh"
#include "include/Light/AmbientLight.hh"

#include <sstream>
typedef stringstream StringStream;
#include <fstream>


// This is required if the standard versions of the STL header files are included
using namespace std;


// Forward declare all the classes and define typedefs for simplicity

class DLFLVertex;
class DLFLFaceVertex;
class DLFLEdge;
class DLFLFace;
class DLFLObject;
class DLFLMaterial;
class TMPatch;
class TMPatchFace;
class DLFLLocator;  // brianb

typedef DLFLVertex * DLFLVertexPtr;
typedef DLFLLocator * DLFLLocatorPtr; // brianb
typedef DLFLFaceVertex * DLFLFaceVertexPtr;
typedef DLFLEdge * DLFLEdgePtr;
typedef DLFLFace * DLFLFacePtr;
typedef DLFLObject * DLFLObjectPtr;
typedef DLFLMaterial * DLFLMaterialPtr;
typedef TMPatch * TMPatchPtr;
typedef TMPatchFace * TMPatchFacePtr;

typedef Vector3d DLFLControlPoint;                         // brianb
typedef vector<DLFLControlPoint> DLFLControlPointArray; // brianb
typedef vector<DLFLControlPoint> DLFLControlPoint2dArray;// brianb

typedef vector<DLFLLocator> DLFLLocatorArray;        // brianb
typedef vector<DLFLLocatorPtr> DLFLLocatorPtrArray; // brianb
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

typedef vector<Light> LightArray;
typedef vector<LightPtr> LightPtrArray;
typedef list<Light> LightList;
typedef list<LightPtr> LightPtrList;


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

// VTOld and VTNewSub vertex types added by Bei and Cansin
// VTWireInside, VTWire, VTWireSubD, VTWireSubDInside added by Esan
// FVTWire added by Esan
// ETOriginal, ETChull, ETdoNotDelete, ETdoDelete added by Esan
// FTHole, FTHoleInside, FTWire added by Esan
enum DLFLVertexType { VTNormal=0, VTNewFacePoint=1, VTNewEdgePoint=2, VTNewPoint=3, VTOld=4, VTNewSub=5,
                      VTWireInside=6, VTWire=7, VTWireSubD=8, VTWireSubDInside=9 };
enum DLFLFaceVertexType { FVTNormal=0, FVTNew=1, FVTWire=2  };
enum DLFLEdgeType { ETNormal=0, ETNew=1, ETCollapse=2, ETCollapseAux=3,
                    ETOriginal=4, ETChull=5, ETdoNotDelete=6, ETdoDelete=7 };
enum DLFLFaceType { FTNormal=0, FTNew=1, FTHole=2, FTHoleInside=3, FTWire=4 };


//-- Common utility functions --//

/*
  Remove node specified by index from vector specified by array
  It is assumed that order of elements within the array is not important.
  Copy the last element to the specified index and delete the last element.
  Iterators pointing to the deleted index and to the last element are invalidated.
  Relative ordering of iterators is also changed for index, last element and adjacent nodes.
*/

template <class T, class Alloc>
inline void remove(vector<T,Alloc>& array, typename vector<T,Alloc>::size_type index)
{
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
void erase_dlp(DLFLLocatorPtr lp);  // brianb

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

#endif /* #ifndef _DLFL_COMMON_HH_ */

/*
  $Log: DLFLCommon.hh,v $
  Revision 4.4  2004/01/20 22:16:36  vinod
  Changed check back to HAVE_SSTREAM from GNUG for StringStream

  Revision 4.3  2004/01/20 17:03:51  vinod
  Changed check for HAVE_STREAM to __GNUG__ to make it compatible with Cygwin

  Revision 4.2  2004/01/20 09:25:38  vinod
  Added new types for use in Column Modeling

  Revision 4.1  2004/01/19 20:19:29  vinod
  Added 2 new vertex types VTOld and VTNewSub

  Revision 4.0  2003/12/26 01:58:52  vinod
  Major version sync.

  Revision 3.2  2003/12/01 08:59:58  vinod
  Added include for Matrix4x4 class

  Revision 3.1  2003/11/29 04:22:28  vinod
  Added 2 new edge types used in edge collapse

  Revision 3.0  2003/09/19 16:06:47  vinod
  Major version update

  Revision 2.6  2002/12/30 04:06:59  vinod
  Added FTHole type in enum DLFLFaceType

  Revision 2.5  2002/10/25 15:36:32  vinod
  Moved 'using namespace std' to after include files

  Revision 2.4  2002/10/25 15:34:17  vinod
  Changed strstream to strstream.h for SGI.
  Added include of fstream/fstream.h

  Revision 2.3  2002/10/25 03:06:08  vinod
  Added macros to use sstream/strstream depending on library compatibility

  Revision 2.2  2002/10/22 21:22:05  vinod
  Removed bool.h from list of includes

  Revision 2.1  2002/04/23 21:33:45  vinod
  Added typedefs for arrays and lists of Vector2d and int

  Revision 2.0  2001/07/25 05:12:42  vinod
  Major version sync

  Revision 1.4  2001/05/15 21:09:13  vinod
  Added typedefs for double

  Revision 1.3  2001/05/15 19:37:26  vinod
  Added typedefs for Vector3d

  Revision 1.2  2000/11/28 19:13:23  vinod
  Added lighting stuff

  Revision 1.1.1.1  2000/11/02 19:32:43  vinod
  New DLFL Sources

*/
