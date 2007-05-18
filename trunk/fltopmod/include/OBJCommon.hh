/* $Id: OBJCommon.hh,v 1.1 2006/09/04 19:22:53 stuart Exp $ */

#ifndef _OBJ_COMMON_HH_

#define _OBJ_COMMON_HH_

// Common definitions and declarations for the OBJ classes

#include <Inlines.hh>
#include <StreamIO.hh>
#include <Color.hh>
#include <string.h>
#include <stdlib.h>
#include <bool.h>
#include <vector>
#include <list>
#include <algorithm>
#include <iterator>

// This is required if the standard versions of the STL header files are included
using namespace std;

// Forward declare various classes and define typedefs for simplicity

class OBJVertex;
class OBJFaceVertex;
class OBJFace;
class OBJObject;
class OBJMaterial;

typedef OBJVertex * OBJVertexPtr;
typedef OBJFaceVertex * OBJFaceVertexPtr;
typedef OBJFace * OBJFacePtr;
typedef OBJObject * OBJObjectPtr;
typedef OBJMaterial * OBJMaterialPtr;

typedef vector<OBJVertex> OBJVertexArray;
typedef vector<OBJVertexPtr> OBJVertexPtrArray;
typedef list<OBJVertex> OBJVertexList;
typedef list<OBJVertexPtr> OBJVertexPtrList;

typedef vector<OBJFaceVertex> OBJFaceVertexArray;
typedef vector<OBJFaceVertexPtr> OBJFaceVertexPtrArray;
typedef list<OBJFaceVertex> OBJFaceVertexList;
typedef list<OBJFaceVertexPtr> OBJFaceVertexPtrList;

typedef vector<OBJFace> OBJFaceArray;
typedef vector<OBJFacePtr> OBJFacePtrArray;
typedef list<OBJFace> OBJFaceList;
typedef list<OBJFacePtr> OBJFacePtrList;

typedef vector<OBJObject> OBJObjectArray;
typedef vector<OBJObjectPtr> OBJObjectPtrArray;
typedef list<OBJObject> OBJObjectList;
typedef list<OBJObjectPtr> OBJObjectPtrList;

typedef vector<OBJMaterial> OBJMaterialArray;
typedef vector<OBJMaterialPtr> OBJMaterialPtrArray;
typedef list<OBJMaterial> OBJMaterialList;
typedef list<OBJMaterialPtr> OBJMaterialPtrList;

#endif /* #ifndef _OBJ_COMMON_HH_ */


/*
  $Log: OBJCommon.hh,v $
  Revision 1.1  2006/09/04 19:22:53  stuart
  Added includes and libs.

  Revision 1.2  2000/09/25 17:56:49  vinod
  Added iterator include file

  Revision 1.1  2000/09/22 18:19:46  vinod
  Common include,definitions,declarations for OBJ classes

*/
