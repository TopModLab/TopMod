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

#include <Python.h>

#include <DLFLCore.hh>
#include <DLFLExtrude.hh>
#include <DLFLSubdiv.hh>
#include <DLFLDual.hh>
#include <DLFLConnect.hh>
#include <DLFLCrust.hh>

typedef DLFL::DLFLFaceVertex Corner;
typedef DLFL::DLFLFaceVertexPtr CornerPtr;
typedef DLFL::DLFLFaceVertexPtrArray CornerPtrArray;

/// Prototypes

/* Object Management */
static PyObject *dlfl_load_obj(PyObject *self, PyObject *args);
static PyObject *dlfl_kill_obj(PyObject *self, PyObject *args);
static PyObject *dlfl_switch_obj(PyObject *self, PyObject *args);
static PyObject *dlfl_save_obj(PyObject *self, PyObject *args);
// static PyObject *dlfl_save_lg3d(PyObject *self, PyObject *args);
// static PyObject *dlfl_save_stl(PyObject *self, PyObject *args);
//static PyObject *dlfl_query_obj(PyObject *self, PyObject *args);

/* Core */
static PyObject *dlfl_insert_edge(PyObject *self, PyObject *args);
static PyObject *dlfl_delete_edge(PyObject *self, PyObject *args);
static PyObject *dlfl_collapse_edge(PyObject *self, PyObject *args);
static PyObject *dlfl_subdivide_edge(PyObject *self, PyObject *args);

static PyObject *dlfl_create_vertex(PyObject *self, PyObject *args);
static PyObject *dlfl_remove_vertex(PyObject *self, PyObject *args);

static PyObject *dlfl_create_face(PyObject *self, PyObject *args);

/* Remove from then object's lists, then delete from memory */
/* DANGEROUS!! - don't work in GUI mode, better to just use deleteEdge */
//static PyObject *dlfl_destroy_vertex(PyObject *self, PyObject *args);
//static PyObject *dlfl_destroy_edge(PyObject *self, PyObject *args);
//static PyObject *dlfl_destroy_face(PyObject *self, PyObject *args);

/* IDs */
static PyObject *dlfl_faces(PyObject *self, PyObject *args);
static PyObject *dlfl_edges(PyObject *self, PyObject *args);
static PyObject *dlfl_verts(PyObject *self, PyObject *args);
static PyObject *dlfl_corners(PyObject *self, PyObject *args);

//static PyObject *dlfl_boundary_walk(PyObject *self, PyObject *args);
static PyObject *dlfl_walk(PyObject *self, PyObject *args);
static PyObject *dlfl_corner_walk(PyObject *self, PyObject *args);
static PyObject *dlfl_next(PyObject *self, PyObject *args);
static PyObject *dlfl_prev(PyObject *self, PyObject *args);
static PyObject *dlfl_cornerFromEdgeFace(PyObject *self, PyObject *args);
static PyObject *dlfl_saveCorner(PyObject *self, PyObject *args);
static PyObject *dlfl_restoreCorner(PyObject *self, PyObject *args);
//static PyObject *dlfl_walk_vertices(PyObject *self, PyObject *args);
//static PyObject *dlfl_walk_edges(PyObject *self, PyObject *args);

/* Info */
static PyObject *dlfl_print_object(PyObject *self, PyObject *args);
static PyObject *dlfl_vertexInfo(PyObject *self, PyObject *args);
static PyObject *dlfl_edgeInfo(PyObject *self, PyObject *args);
static PyObject *dlfl_faceInfo(PyObject *self, PyObject *args);
static PyObject *dlfl_cornerInfo(PyObject *self, PyObject *args);
static PyObject *dlfl_centroid(PyObject *self, PyObject *args);

/* Auxiliary */
static PyObject *dlfl_extrude(PyObject *self, PyObject *args);
static PyObject *dlfl_subdivide(PyObject *self, PyObject *args);
static PyObject *dlfl_subdivide_face(PyObject *self, PyObject *args);
static PyObject *dlfl_subdivide_faces(PyObject *self, PyObject *args);
static PyObject *dlfl_dual(PyObject *self, PyObject *args);
static PyObject *dlfl_connectEdges(PyObject *self, PyObject *args);
static PyObject *dlfl_connectCorners(PyObject *self, PyObject *args);
static PyObject *dlfl_connectFaces(PyObject *self, PyObject *args);
static PyObject *dlfl_addHandle(PyObject *self, PyObject *args);
static PyObject *dlfl_rindmodel(PyObject *self, PyObject *args);
static PyObject *dlfl_wireframe(PyObject *self, PyObject *args);
static PyObject *dlfl_column(PyObject *self, PyObject *args);
//static PyObject *dlfl_sierpinsky(PyObject *self, PyObject *args);
//static PyObject *dlfl_multiface(PyObject *self, PyObject *args);
//static PyObject *dlfl_menger(PyObject *self, PyObject *args);

/* Transform */
static PyObject *dlfl_translate(PyObject *self, PyObject *args);
static PyObject *dlfl_scale(PyObject *self, PyObject *args);
static PyObject *dlfl_freezeTrans(PyObject *self, PyObject *args);
static PyObject *dlfl_move(PyObject *self, PyObject *args);

PyMODINIT_FUNC initdlfl(void);

static PyMethodDef DLFLMethods[] = {
	/* Object Management */
  {"load",           dlfl_load_obj,       METH_VARARGS, "load(string)"},
  {"save",           dlfl_save_obj,       METH_VARARGS, "save(string)"},  
  // {"saveLG3d",       dlfl_save_lg3d,      METH_VARARGS, "saveLG3d(string)"},  
  // {"saveSTL",        dlfl_save_stl,       METH_VARARGS, "saveSTL(string)"},  
  {"kill",           dlfl_kill_obj,       METH_VARARGS, "kill(object id)"},
  {"switch",         dlfl_switch_obj,     METH_VARARGS, "switch(object id)"},
  //{"query",          dlfl_query_obj,      METH_VARARGS, "query()"},
	/* Core */
  {"insertEdge",     dlfl_insert_edge,    METH_VARARGS, "Inserts an Edge"},
  {"deleteEdge",     dlfl_delete_edge,    METH_VARARGS, "Delete an Edge."},
  {"collapseEdge",   dlfl_collapse_edge,  METH_VARARGS, "Collapse an Edge."},
  {"subdivideEdge",  dlfl_subdivide_edge, METH_VARARGS, "Subdivide an Edge."},
	{"createVertex",   dlfl_create_vertex,  METH_VARARGS, "Create a vertex at (x,y,z)"},
	{"removeVertex",   dlfl_remove_vertex,  METH_VARARGS, "Remove a vertex by ID"},
	/*{"destroyVertex",  dlfl_destroy_vertex, METH_VARARGS, "Remove a vertex from the object"},
	{"destroyEdge",    dlfl_destroy_edge,   METH_VARARGS, "Remove a edge from the object"},
	{"destroyFace",    dlfl_destroy_face,   METH_VARARGS, "Remove a face from the object"},*/
	{"createFace",     dlfl_create_face,    METH_VARARGS, "Create a face from a list of coordinates (specified as 3-tuples)"},
	/* IDs */
  {"faces",          dlfl_faces,          METH_VARARGS, "Grab all/selected the faces of the object"},
  {"edges",          dlfl_edges,          METH_VARARGS, "Grab all/selected the edges of the object"},
  {"verts",          dlfl_verts,          METH_VARARGS, "Grab all/selected the vertices of the object"},
  {"corners",        dlfl_corners,        METH_VARARGS, "Grab all/selected the face-vertices of the object"},
  {"walk",           dlfl_walk,           METH_VARARGS, "Walk around a face and get all of the vertices/edges in lists"},
  {"cornerWalk",     dlfl_corner_walk,    METH_VARARGS, "Walk around a face and get all of the corners in a list"},
  {"next",           dlfl_next,           METH_VARARGS, "Walk to next corner in Linked List"},
  {"prev",           dlfl_prev,           METH_VARARGS, "Walk to previous corner in Linked List"},
  {"getCorner",  dlfl_cornerFromEdgeFace, METH_VARARGS, "Get a corner given an edge and a face pointer"},
  {"saveCorner",     dlfl_saveCorner,     METH_VARARGS, "Saves a corner by grabbing a corner ID. This can then be restored later. This is useful when performing operations like insertEdge that change face ids"},
  {"restoreCorner",  dlfl_restoreCorner,  METH_VARARGS, "Restores a corner from a corner ID. Returns (faceid,vertexid). This is useful when performing operations like insertEdge that change face ids"},
  /*{"walkVertices",   dlfl_walk_vertices,  METH_VARARGS, "walk_vertices(int)"},
		{"walkEdges",      dlfl_walk_edges,     METH_VARARGS, "walk_edges(int)"},*/
	/* Info */
  {"printObject",   dlfl_print_object,   METH_VARARGS, "Print object information"},
  {"vertexInfo",    dlfl_vertexInfo,     METH_VARARGS, "Position,Valence,etc into dictionary"},
  {"edgeInfo",      dlfl_edgeInfo,       METH_VARARGS, "Midpoint,Verts,etc into dictionary"},
  {"faceInfo",      dlfl_faceInfo,       METH_VARARGS, "Centroid,Verts,etc into dictionary"},
	{"cornerInfo",    dlfl_cornerInfo,     METH_VARARGS, "Face,Vertex,Edge,etc into dictionary"},
  {"centroid",      dlfl_centroid,       METH_VARARGS, "Get centroid of vertices"},
  /* Auxiliary Below */
  {"extrude",        dlfl_extrude,        METH_VARARGS, "Extrude a face"},
  {"subdivide",      dlfl_subdivide,      METH_VARARGS, "Subdivide a mesh"},
  {"subdivideFace",  dlfl_subdivide_face, METH_VARARGS, "Subdivide a Face"},
  {"subdivideFaces",  dlfl_subdivide_faces, METH_VARARGS, "Subdivide a list of Faces"},
  {"dual",           dlfl_dual,           METH_VARARGS, "Dual of mesh"},
  {"connectEdges",   dlfl_connectEdges,   METH_VARARGS, "Connect an edge on one face with another"},
  {"connectCorners", dlfl_connectCorners, METH_VARARGS, "Connect two corners (i.e. Add Hole/Handle)"},
  {"connectFaces",   dlfl_connectFaces,   METH_VARARGS, "Connect two faces (i.e. Add Hole/Handle Closest Vertex)"},
  {"addHandle",      dlfl_addHandle,      METH_VARARGS, "Connect two faces (i.e. Add Hole/Handle Shape Interpolation). Choose either \"hermite\" or \"bezier\""},
	{"rind",           dlfl_rindmodel,      METH_VARARGS, "Rind modeling. Given a set of faces, create a crust"},
	{"wireframe",      dlfl_wireframe,      METH_VARARGS, "Creates a wireframe of the mesh"},
	{"column",         dlfl_column,         METH_VARARGS, "Creates a wireframe of the mesh via column modeling"},
	/*	{"sierpinsky",     dlfl_sierpinsky,     METH_VARARGS, ""},
	{"multiface",      dlfl_multiface,      METH_VARARGS, ""},
	{"menger",         dlfl_menger,         METH_VARARGS, ""},*/
	/* Transform */
  {"translate",      dlfl_translate,      METH_VARARGS, "Translate Object"},
  {"scale",          dlfl_scale,          METH_VARARGS, "Scale Object"},
	{"freezeTrans",    dlfl_freezeTrans,    METH_VARARGS, "Freeze Transformations"},
  {"move",      dlfl_move,      METH_VARARGS, "Move Vertices"},
  {NULL, NULL, 0, NULL}        /* Sentinel */
};

static PyObject *DLFLError;
static bool usingGUI = false;

// Pointer to the current object that we are editing
DLFL::DLFLObject *currObj = 0;
static DLFL::DLFLObjectPtrArray objArray;

/**
 *  Load/Save Files
 */

static PyObject *
dlfl_load_obj(PyObject *self, PyObject *args)
{
  char *filename;
  int size;
  int objId = -1;

  if( !PyArg_ParseTuple(args, "s#", &filename, &size) )
    return NULL;

  if( !usingGUI ) {
    currObj = DLFL::readObjectFile( filename );
    objArray.push_back( currObj );

    if( currObj )
      objId = objArray.size()-1; //currObj->getID();
  }
  return Py_BuildValue("i", objId );
}
/*
static PyObject *
dlfl_query_obj(PyObject *self, PyObject *args)
{
  return Py_BuildValue("i", usingGUI );
}
*/
static PyObject *
dlfl_kill_obj(PyObject *self, PyObject *args)
{
  int idx;
  if( !PyArg_ParseTuple(args, "i", &idx) )
    return NULL;

  DLFL::DLFLObjectPtrArray::iterator it = objArray.begin();
  for( int i = 0; i < idx; i++) { it++; }
  DLFL::DLFLObjectPtr obj = objArray[idx];

  /*if( currObj == obj ) {
    if( objArray.size() > 1 )
    currObj = objArray[objArray.size()-1];
    else
    }*/

  objArray.erase( it );
  currObj = NULL;

  Py_INCREF(Py_None);
  return Py_None;
}

static PyObject *
dlfl_switch_obj(PyObject *self, PyObject *args)
{
  int idx;
  if( !PyArg_ParseTuple(args, "i", &idx) )
    return NULL;
  
 int objId = idx;
  if( idx < 0 || idx >= objArray.size() ) {
    objId = -1;
  } else {
    currObj = objArray[idx];
  }
  return Py_BuildValue("i", objId );
}

static PyObject *
dlfl_save_obj(PyObject *self, PyObject *args)
{
  char *filename; int size;
  if( !PyArg_ParseTuple(args, "s#", &filename, &size) )
    filename = NULL;

  bool wrote;
  if( currObj )
    wrote = DLFL::writeObjectFile( currObj, filename );
  else
    wrote = false;

  return Py_BuildValue( "b", wrote );
}

/**
 * Insert/Delete/Collapse/Subdivide Edge
 */

static PyObject *
dlfl_insert_edge(PyObject *self, PyObject *args)
{
  uint faceId1, faceId1b;
  uint vertId1, vertId1b;
  uint faceId2, faceId2b;
  uint vertId2, vertId2b;
  int edgeId = -1;
	bool check = true;

  if( !PyArg_ParseTuple(args, "(ii)(ii)|b", &faceId1, &vertId1, &faceId2, &vertId2, &check) )
    return NULL;

  if( currObj ) {
    edgeId = DLFL::insertEdge( currObj, 
															 faceId1, vertId1, faceId2, vertId2, 
															 faceId1b, vertId1b, faceId2b, vertId2b, 
															 check, false );
    currObj->clearSelected( );
  }

  return Py_BuildValue("i,(ii)(ii)(ii)(ii)", edgeId, 
											 faceId1, vertId1, faceId2, vertId2,
											 faceId1b, vertId1b, faceId2b, vertId2b );
}

static PyObject *
dlfl_delete_edge(PyObject *self, PyObject *args)
{
	int edgeId;
	std::vector<int> faceIds;

  if( !PyArg_ParseTuple(args, "i", &edgeId) )
    return NULL;
  if( !currObj )
    return NULL;

  if( currObj ) {
    faceIds = DLFL::deleteEdgeID( currObj, edgeId );
    currObj->clearSelected( );
  }

	PyObject *face;
	PyObject *list = PyList_New(faceIds.size());
	for( int i = 0; i < (int)faceIds.size(); i++ ) {
		face = Py_BuildValue("i", faceIds[i] );
		PyList_SetItem(list, i, face);
	}

	Py_INCREF(list);
	return list;
}

static PyObject *
dlfl_collapse_edge(PyObject *self, PyObject *args)
{
  int edgeId;

  if( !PyArg_ParseTuple(args, "i", &edgeId) )
    return NULL;
  if( !currObj )
    return NULL;

  int vertId = DLFL::collapseEdgeID( currObj, edgeId );
  currObj->clearSelected( );

  return Py_BuildValue("i", vertId );
}

static PyObject *
dlfl_subdivide_edge(PyObject *self, PyObject *args)
{
  int edgeId, numDivs;

  if( !currObj )
    return NULL;

  if( PyArg_ParseTuple(args, "i", &edgeId) ) {
    int vertId = DLFL::subdivideEdgeID( currObj, edgeId );
    currObj->clearSelected( );
    return Py_BuildValue("[i]", vertId );
  } else if( PyArg_ParseTuple(args, "ii", &numDivs, &edgeId ) ) {
    vector<int> verts = DLFL::subdivideEdgeID( currObj, numDivs, edgeId );
    PyObject *vert;
    PyObject *list = PyList_New(verts.size());
    for( int i = 0; i < verts.size(); i++) {
      vert = Py_BuildValue("i", verts[i]);
      PyList_SetItem(list, i, vert);
    }
    currObj->clearSelected( );
    Py_INCREF(list);
    return list;
  }

  Py_INCREF(Py_None);
  return Py_None;
}


static PyObject *dlfl_create_vertex(PyObject *self, PyObject *args) {
	double x, y, z;

	if( !PyArg_ParseTuple(args, "(ddd)", &x, &y, &z) )
			return NULL;

	if( !usingGUI ) {
		if( currObj == NULL )
			currObj = new DLFL::DLFLObject;
	}
	//uint *fvid = DLFL::createVertex( x, y, z, currObj );
	DLFL::DLFLFaceVertexPtr fvp = currObj->createPointSphere(Vector3d(x,y,z));
	uint fid = fvp->getFaceID();//fvid[0];
	uint vid = fvp->getVertexID();//fvid[1];
	//delete fvid; fvid = 0;
	//uint oid = currObj->getID();
  return Py_BuildValue("(ii)", fid, vid );
}

static PyObject *dlfl_remove_vertex(PyObject *self, PyObject *args) {
	uint vid, fid;
	if( !PyArg_ParseTuple(args, "(ii)", &vid, &fid) )
		return NULL;
	DLFL::removeVertex( currObj, vid, fid );

  Py_INCREF(Py_None);
  return Py_None;
}

static PyObject *dlfl_create_face(PyObject *self, PyObject *args) {
	PyObject *list, *tuple, *coord;
	bool bothNew = false;
	if( !PyArg_ParseTuple(args, "O!|b", &PyList_Type, &list, &bothNew) )
		return NULL;
	int size = PyList_Size(list);
	if( size < 0 )
		return NULL;

	Vector3d vec;
	std::vector<Vector3d> varray;
	for( int i = 0; i < size; i++ ) {
		tuple = PyList_GetItem(list, i);
		if(!PyTuple_Check(tuple)) continue;
		if(PyTuple_Size(tuple) != 3) continue;
		for( int j = 0; j < 3; j++ ) {
			coord = PyTuple_GetItem(tuple, j);
			if( !PyFloat_Check(coord) ) break;
			vec[j] = PyFloat_AsDouble( coord );
		}
		varray.push_back(vec);
	}

	DLFL::DLFLFacePtrArray fpa;
	fpa = currObj->createFace( varray, NULL, bothNew );
	
	if( !bothNew )
		currObj->lastFace()->setType(DLFL::FTNew);

	if( fpa.size() == 2 ) {
		return Py_BuildValue( "ii", fpa[0]->getID(), fpa[1]->getID() );
	}

	Py_INCREF(Py_None);
	return Py_None;
}

/* Mesh Identification Helpers (Verts,Edges,Faces, FaceVertices ) */

static PyObject *
dlfl_faces(PyObject *self, PyObject *args)
{
  if( !currObj ) {
    Py_INCREF(Py_None);
    return Py_None;
  }

  bool selected = false;
  if( usingGUI ) {
    selected = true;
    if( !PyArg_ParseTuple(args, "|b", &selected ) )
      return NULL;
  }

  PyObject *flist=0, *face=0;
  if(selected) {
    DLFL::DLFLFacePtrArray fpa = currObj->sel_fptr_array;
    flist = PyList_New(fpa.size());
    for( int i = 0; i < (int)fpa.size(); i++ ) {
      face = Py_BuildValue("i", fpa[i]->getID());
      PyList_SetItem(flist, i, face);
    }
  } else {
    DLFL::DLFLFacePtrList fpl = currObj->getFaceList();
    DLFL::DLFLFacePtrList::iterator it; int i;
    flist = PyList_New(fpl.size());
    for( i = 0, it = fpl.begin(); it != fpl.end(); i++, it++ ) {
      face = Py_BuildValue("i", (*it)->getID());
      PyList_SetItem(flist, i, face);
    }
  }
  Py_INCREF(flist);
  return flist;
}

static PyObject *
dlfl_edges(PyObject *self, PyObject *args)
{
  if( !currObj ) {
    Py_INCREF(Py_None);
    return Py_None;
  }

  bool selected = false;
  if( usingGUI ) {
    selected = true;
    if( !PyArg_ParseTuple(args, "|b", &selected ) )
      return NULL;
  }

  PyObject *elist=0, *edge=0;
  if(selected) {
    DLFL::DLFLEdgePtrArray epa = currObj->sel_eptr_array;
    elist = PyList_New(epa.size());
    for( int i = 0; i < (int)epa.size(); i++ ) {
      edge = Py_BuildValue("i", epa[i]->getID());
      PyList_SetItem(elist, i, edge);
    }
  } else {
    DLFL::DLFLEdgePtrList epl = currObj->getEdgeList();
    DLFL::DLFLEdgePtrList::iterator it; int i;
    elist = PyList_New(epl.size());
    for( i = 0, it = epl.begin(); it != epl.end(); i++, it++ ) {
      edge = Py_BuildValue("i", (*it)->getID());
      PyList_SetItem(elist, i, edge);
    }
  }
  Py_INCREF(elist);
  return elist;
}

static PyObject *
dlfl_verts(PyObject *self, PyObject *args)
{
  if( !currObj ) {
    Py_INCREF(Py_None);
    return Py_None;
  }

  bool selected = false;
  if( usingGUI ) {
    selected = true;
    if( !PyArg_ParseTuple(args, "|b", &selected ) )
      return NULL;
  }

  PyObject *vlist=0, *vert=0;
  if(selected) {
    DLFL::DLFLVertexPtrArray vpa = currObj->sel_vptr_array;
    vlist = PyList_New(vpa.size());
    for( int i = 0; i < (int)vpa.size(); i++ ) {
      vert = Py_BuildValue("i", vpa[i]->getID());
      PyList_SetItem(vlist, i, vert);
    }
  } else {
    DLFL::DLFLVertexPtrList vpl = currObj->getVertexList();
    DLFL::DLFLVertexPtrList::iterator it; int i;
    vlist = PyList_New(vpl.size());
    for( i = 0, it = vpl.begin(); it != vpl.end(); i++, it++ ) {
      vert = Py_BuildValue("i", (*it)->getID());
      PyList_SetItem(vlist, i, vert);
    }
  }
  Py_INCREF(vlist);
  return vlist;  
}

static PyObject *
dlfl_corners(PyObject *self, PyObject *args)
{
  if( !currObj ) {
    Py_INCREF(Py_None);
    return Py_None;
  }

  bool selected = false;
  if( usingGUI ) {
    selected = true;
    if( !PyArg_ParseTuple(args, "|b", &selected ) )
      return NULL;
  }

  PyObject *fvlist=0, *fvert=0;
  if(selected) {
    CornerPtrArray fvpa = currObj->sel_fvptr_array;
    fvlist = PyList_New(fvpa.size());
    for( int i = 0; i < (int)fvpa.size(); i++ ) {
      fvert = Py_BuildValue("(ii)", fvpa[i]->getFaceID(), fvpa[i]->getVertexID() );
      PyList_SetItem(fvlist, i, fvert);
    }
  } else {
    CornerPtrArray fvpa;
    DLFL::DLFLFacePtrList fpl = currObj->getFaceList();
    DLFL::DLFLFacePtrList::iterator it; int i;
    for( i = 0, it = fpl.begin(); it != fpl.end(); i++, it++ ) {
      CornerPtr head = (*it)->front();
      CornerPtr curr = head;
      do {
	fvpa.push_back(curr);
	DLFL::advance(curr);
      } while(curr != head);
    }
    fvlist = PyList_New(fvpa.size());
    for( int i = 0; i < (int)fvpa.size(); i++ ) {
      fvert = Py_BuildValue("(ii)", fvpa[i]->getFaceID(), fvpa[i]->getVertexID() );
      PyList_SetItem(fvlist, i, fvert);
    }
  }
  Py_INCREF(fvlist);
  return fvlist;
}
/*
static PyObject *
dlfl_boundary_walk(PyObject *self, PyObject *args) {
  int faceId;

  if( !currObj )
    return NULL;
  if( !PyArg_ParseTuple(args, "i", &faceId) )
    return NULL;
  currObj->boundaryWalkID( faceId );

  Py_INCREF(Py_None);
  return Py_None;
}
*/
static PyObject *dlfl_corner_walk(PyObject *self, PyObject *args) {
  int faceId;

  vector<int> verts; // output: verts walked
  vector<int> edges; // not using here, just to pass
  PyObject *list; // py output: corners walked
  PyObject *corner; // for looping

  if( !currObj )
    return NULL;

  if( !PyArg_ParseTuple(args, "i", &faceId) )
    return NULL;
	currObj->walk( faceId, verts, edges );

  list = PyList_New(verts.size());
  for( int i = 0; i < verts.size(); i++) {
    corner = Py_BuildValue("(ii)", faceId, verts[i]);
    PyList_SetItem(list, i, corner);
  }
  Py_INCREF(list);
  return list;
}

static PyObject *dlfl_walk(PyObject *self, PyObject *args) {
  int faceId; // input: face to walk

	vector<int> verts; // output: verts walked
  vector<int> edges; // output: edges walked
  PyObject *vlist; // py output: verts walked
  PyObject *elist; // py output: edges walked
	PyObject *vert; // for looping
  PyObject *edge; // for looping
  
  if( !currObj )
    return NULL;

  if( !PyArg_ParseTuple(args, "i", &faceId) )
    return NULL;
  currObj->walk( faceId, verts, edges );

	vlist = PyList_New(verts.size());
	for( int i = 0; i < verts.size(); i++ ) {
		vert = Py_BuildValue("i", verts[i]);
		PyList_SetItem(vlist, i, vert);
	}
  elist = PyList_New(edges.size());
  for( int i = 0; i < edges.size(); i++) {
    edge = Py_BuildValue("i", edges[i]);
    PyList_SetItem(elist, i, edge);
  }
  //Py_INCREF(vlist);
  //Py_INCREF(elist);
  return Py_BuildValue("OO", vlist, elist);
}

static PyObject *dlfl_next(PyObject *self, PyObject *args) {
	int faceId, vertId;
	if( !PyArg_ParseTuple(args, "(ii)", &faceId, &vertId) )
    return NULL;
	if( !currObj )
		return NULL;
	DLFL::DLFLFacePtr fp1 = currObj->findFace(faceId);
  if( !fp1 ) return Py_BuildValue("(ii)", -1, -1);
	DLFL::DLFLFaceVertexPtr fvp1 = fp1->findFaceVertex(vertId);
  if( !fvp1 ) return Py_BuildValue("(ii)", -1, -1);
	DLFL::DLFLFaceVertexPtr fvp2 = fvp1->next();
  if( !fvp2 ) return Py_BuildValue("(ii)", -1, -1);
	faceId = fvp2->getFaceID();
	vertId = fvp2->getVertexID();
  return Py_BuildValue("(ii)", faceId, vertId );
}

static PyObject *dlfl_prev(PyObject *self, PyObject *args) {
	int faceId, vertId;
	if( !PyArg_ParseTuple(args, "(ii)", &faceId, &vertId) )
    return NULL;
	if( !currObj )
		return NULL;
	DLFL::DLFLFacePtr fp1 = currObj->findFace(faceId);
  if( !fp1 ) return Py_BuildValue("(ii)", -1, -1);
	DLFL::DLFLFaceVertexPtr fvp1 = fp1->findFaceVertex(vertId);
  if( !fvp1 ) return Py_BuildValue("(ii)", -1, -1);
	DLFL::DLFLFaceVertexPtr fvp2 = fvp1->prev();
  if( !fvp2 ) return Py_BuildValue("(ii)", -1, -1);
	faceId = fvp2->getFaceID();
	vertId = fvp2->getVertexID();
  return Py_BuildValue("(ii)", faceId, vertId );
}

static PyObject *dlfl_cornerFromEdgeFace(PyObject *self, PyObject *args) {
	int faceId, edgeId, vertId;
	if( !PyArg_ParseTuple(args, "ii", &edgeId, &faceId) )
    return NULL;
	if( !currObj )
		return NULL;
	DLFL::DLFLEdgePtr ep = currObj->findEdge(edgeId);
	DLFL::DLFLFacePtr fp = currObj->findFace(faceId);
	if( !ep || !fp ) return Py_BuildValue("(ii)", -1, -1 );
	DLFL::DLFLFaceVertexPtr fvp = ep->getFaceVertexPtr(fp);
	if( !fvp ) return Py_BuildValue("(ii)", -1, -1 );
	faceId = fvp->getFaceID();
	vertId = fvp->getVertexID();
  return Py_BuildValue("(ii)", faceId, vertId );
}

static PyObject *dlfl_saveCorner(PyObject *self, PyObject *args) { 
	int faceId, vertId;
	if( !PyArg_ParseTuple(args, "(ii)", &faceId, &vertId) )
    return NULL;
	if( !currObj )
		return NULL;
	DLFL::DLFLFacePtr fp = currObj->findFace(faceId);
	if( !fp ) return Py_BuildValue("i", -1);
	DLFL::DLFLFaceVertexPtr fvp = fp->findFaceVertex(vertId);
	if( !fvp ) return Py_BuildValue("i", -1 );
	int cornerID = fvp->getID( );
	return Py_BuildValue("i", cornerID );
}

static PyObject *dlfl_restoreCorner(PyObject *self, PyObject *args) { 
	int cornerId, faceId, vertId;
	if( !PyArg_ParseTuple(args, "i", &cornerId) )
    return NULL;
	if( !currObj )
		return NULL;
	DLFL::DLFLFaceVertexPtr fvp = currObj->findFaceVertex(cornerId);
	if( !fvp ) return Py_BuildValue("(ii)", -1, -1 );
	faceId = fvp->getFaceID();
	vertId = fvp->getVertexID();
	return Py_BuildValue("(ii)", faceId, vertId );
}


static PyObject *
dlfl_print_object(PyObject *self, PyObject *args) {
  if( !currObj )
    return NULL;

  currObj->print();

  Py_INCREF(Py_None);
  return Py_None;
}

static PyObject *dlfl_vertexInfo(PyObject *self, PyObject *args) {
	if( !currObj ) {
		Py_INCREF(Py_None);
		return Py_None;
	}

	int id;
		if( !PyArg_ParseTuple(args, "i", &id) )
			return NULL;
		DLFL::DLFLVertexPtr vp = currObj->findVertex(id);
		if( !vp ) {
			Py_INCREF(Py_None);
			return Py_None;
		}
		const double *coords = (vp->getCoords()).getCArray();
		int valence = vp->valence();
		DLFL::DLFLVertexType vt = vp->getType();
		char* type = new char[10];
		switch( vt ) {
		case 0:
			strcpy(type, "normal");
			break;
		case 1:
			strcpy(type, "newFace");
			break;
		case 2:
			strcpy(type, "newEdge");
			break;
		case 3:
			strcpy(type, "new");
			break;
		case 4 :
			strcpy(type, "old");
			break;
		case 5:
			strcpy(type, "newSub");
			break;
		case 6 :
			strcpy(type, "wireIn");
			break;
		case 7 :
			strcpy(type, "wire");
			break;
		case 8 :
			strcpy(type, "wireSub");
			break;
		case 9 :
			strcpy(type, "wireSubIn");
			break;
		default :
			strcpy(type, "normal");
		}

		PyObject *info = Py_BuildValue("{s:i,s:s,s:(ddd),s:i}", 
												 "id", id,
												 "type", type,
												 "coords", coords[0], coords[1], coords[2],
												 "valence", valence);

		delete [] type;
		return info;
}

static PyObject *dlfl_edgeInfo(PyObject *self, PyObject *args) { 
	if( !currObj ) {
		Py_INCREF(Py_None);
		return Py_None;
	}

		int id;
		if( !PyArg_ParseTuple(args, "i", &id) )
			return NULL;
		DLFL::DLFLEdgePtr ep = currObj->findEdge(id);
		if( !ep ) {
			Py_INCREF(Py_None);
			return Py_None;
		}
		const double *midpoint = (ep->getMidPoint()).getCArray();
		const double *normal = (ep->getNormal()).getCArray();

		CornerPtr fvpa, fvpb;
		ep->getFaceVertexPointers( fvpa, fvpb );
		int fa = fvpa->getFaceID();
		int va = fvpa->getVertexID();
		int fd = fvpb->getFaceID();
		int vd = fvpa->getVertexID();

		fvpa = fvpa->next();
		fvpb = fvpb->next();

		int fc = fvpa->getFaceID();
		int vc = fvpa->getVertexID();
		int fb = fvpb->getFaceID();
		int vb = fvpa->getVertexID();

		double length = ep->length();

		DLFL::DLFLEdgeType et = ep->getType();
		char* type = new char[10];
		switch( et ) {
		case 0:
			strcpy(type, "normal");
			break;
		case 1:
			strcpy(type, "new");
			break;
		case 2:
			strcpy(type, "collapse");
			break;
		case 3:
			strcpy(type, "collapseAux");
			break;
		case 4 :
			strcpy(type, "original");
			break;
		case 5:
			strcpy(type, "convexHull");
			break;
		case 6 :
			strcpy(type, "noDelete");
			break;
		case 7 :
			strcpy(type, "delete");
			break;
		default :
			strcpy(type, "normal");
		}

		PyObject *info = Py_BuildValue("{s:i,s:s,s:(ddd),s:(ddd),s:(ii),s:(ii),s:(ii),s:(ii),s:d}", 
																	 "id", id,
																	 "type", type,
																	 "midpoint", midpoint[0], midpoint[1], midpoint[2],
																	 "normal", normal[0], normal[1], normal[2],
																	 "cornerA", fa, va,
																	 "cornerB", fb, vb,
																	 "cornerC", fc, vc,
																	 "cornerD", fd, vd,
																	 "length", length );
		delete [] type;
		return info;
}

static PyObject *dlfl_faceInfo(PyObject *self, PyObject *args) { 
	if( !currObj ) {
		Py_INCREF(Py_None);
		return Py_None;
	}

		int id;
		if( !PyArg_ParseTuple(args, "i", &id) )
			return NULL;
		DLFL::DLFLFacePtr fp = currObj->findFace(id);
		if( !fp ) {
			Py_INCREF(Py_None);
			return Py_None;
		}
		DLFL::DLFLFaceType ft = fp->getType();
		const double *centroid = (fp->geomCentroid()).getCArray();
		const double *normal = (fp->getNormal(true)).getCArray();

		char* type = new char[10];
		switch( ft ) {
		case 0:
			strcpy(type, "normal");
			break;
		case 1:
			strcpy(type, "new");
			break;
		case 2:
			strcpy(type, "hole");
			break;
		case 3:
			strcpy(type, "holeIn");
			break;
		case 4 :
			strcpy(type, "wire");
			break;
		default :
			strcpy(type, "normal");
		}

		PyObject *info = Py_BuildValue("{s:i,s:s,s:(ddd),s:(ddd),s:i}", 
																	"id", id,
																	"type", type,
																	"centroid", centroid[0], centroid[1], centroid[2],
																	"normal", normal[0], normal[1], normal[2],
																	"size", fp->size());
		delete [] type;
		return info;
}

static PyObject *dlfl_cornerInfo(PyObject *self, PyObject *args) {
	if( !currObj ) {
		Py_INCREF(Py_None);
		return Py_None;
	} else {
		int fid, vid;
		if( !PyArg_ParseTuple(args, "(ii)", &fid, &vid) )
			return NULL;
		DLFL::DLFLFacePtr fp = currObj->findFace(fid);
		if( !fp ) {
			Py_INCREF(Py_None);
			return Py_None;
		}
		DLFL::DLFLFaceVertexPtr fvp = fp->findFaceVertex(vid);
		if( !fvp ) {
			Py_INCREF(Py_None);
			return Py_None;
		}
		int eid = fvp->getEdgeID();

		DLFL::DLFLFaceVertexType fvt = fvp->getType();
		char* type = new char[10];
		switch( fvt ) {
		case 0:
			strcpy(type, "normal");
			break;
		case 1:
			strcpy(type, "new");
			break;
		case 2:
			strcpy(type, "wire");
			break;
		default :
			strcpy(type, "normal");
		}

		PyObject *info = Py_BuildValue("{s:i,s:i,s:i,s:s}", 
																	 "face", fid,
																	 "vertex", vid,
																	 "edge", eid,
																	 "type", type );
		delete [] type;
		return info;
	}
}

static PyObject *dlfl_centroid(PyObject *self, PyObject *args) { 
	if( !currObj ) {
		Py_INCREF(Py_None);
		return Py_None;
	} else {
		Py_INCREF(Py_None);
		return Py_None;		
	}
}

 /* Auxiliary */

static PyObject *
dlfl_extrude(PyObject *self, PyObject *args) {
  if( !currObj ) {
    Py_INCREF(Py_None);
    return Py_None;
  }

  const char* choices[] = { "doo-sabin",
			    "dodeca",
			    "icosa",
			    "octa",
			    "stellate",
			    "double-stellate",
			    "cubical" };

  char* extrudeType;
  int size, faceid;
  double dist = 2.0; int num = 1; double rotTwist = 0.0; double sf = 1.0; bool boolean = false;
  if( !PyArg_ParseTuple(args, "s#i|diddb", &extrudeType, &size, &faceid, 
			&dist, &num, &rotTwist, &sf, &boolean ) )
    return NULL;

  int choice = -1;
  for(int i = 0; i < 7; i++ ) {
    if( strncmp(extrudeType,choices[i],size) == 0 ) {
      choice = i;
      break;
    }
  }

  DLFL::DLFLFacePtr fp = currObj->findFace(faceid);
	if( !fp ) return Py_BuildValue("i", -1 );
  DLFL::DLFLFacePtr ofp = 0;
  int outFaceID = -1;

  if( choice != -1 && fp != NULL ) {
    switch( choice ) {
    case 0 : // doo-sabin
      ofp = DLFL::extrudeFaceDS( currObj, fp, dist, num, rotTwist, sf );
      break;
    case 1 : // dodeca
      ofp = DLFL::extrudeFaceDodeca( currObj, fp, dist, num, rotTwist, sf, boolean );
      break;
    case 2 : // icosa
      ofp = DLFL::extrudeFaceIcosa( currObj, fp, dist, num, rotTwist, sf );
      break;
    case 3 : // octa a.k.a. extrude dual face
      ofp = DLFL::extrudeDualFace( currObj, fp, dist, num, rotTwist, sf, boolean );
      break;
    case 4 : // stellate
      DLFL::stellateFace( currObj, fp, dist );
      break;
    case 5 : // double-stellate
      DLFL::doubleStellateFace( currObj, fp, dist );
      break;
    case 6 : // cubical
    default : // cubical
      ofp = DLFL::extrudeFace( currObj, fp, dist, num, rotTwist, sf );
    }

    if( ofp )
      outFaceID = ofp->getID();

    currObj->clearSelected( );

    return Py_BuildValue("i", outFaceID );
  }

  Py_INCREF(Py_None);
  return Py_None;
}

static PyObject *
dlfl_subdivide(PyObject *self, PyObject *args) {
  if( !currObj ) {
    Py_INCREF(Py_None);
    return Py_None;
  }

  int choiceSize = 26;
  const char* choices[] = { "loop",
														"checker",
														"simplest",
														"vertex-cut",
														"pentagon",
														"pentagon-preserve",
														"honeycomb",
														"doo-sabin",
														"doo-sabin-bc",
														"doo-sabin-bc-new",
														"corner-cut",
														"modified-corner-cut",
														"root4",
														"catmull-clark",
														"star",
														"sqrt3",
														"fractal",
														"stellate",
														"double-stellate",
														"dome",
														"dual-12.6.4",
														"loop-style",
														"linear-vertex",
														"allfaces",
														"root3"};

  char* subdivType;
  int size;
  double attrb1 = 0, attrb2 = 0; 
  bool battrb1 = false, battrb2 = false; 
  int argc = PyArg_ParseTuple(args, "s#|ddbb", &subdivType, &size, &attrb1, &attrb2 );
  if( !argc )
    return NULL;
  if( argc >= 3 )
    battrb1 = true;
  if( argc >= 4)
    battrb2 = true;

  int choice = -1;
  for(int i = 0; i < choiceSize; i++ ) {
    if( strncmp(subdivType,choices[i],size) == 0 ) {
      choice = i;
      break;
    }
  }

  if( choice != -1 ) {
    switch( choice ) {
    case 0 : // loop
      DLFL::loopSubdivide( currObj );
      break;
    case 1 : // checker
      if( battrb1 ) DLFL::checkerBoardRemeshing( currObj, attrb1 );
      else DLFL::checkerBoardRemeshing( currObj );
      break;
    case 2 : // simplest
      DLFL::simplestSubdivide( currObj );
      break;
    case 3 : // vertex-cut
      if( battrb1 ) DLFL::vertexCuttingSubdivide( currObj, attrb1 );
      else DLFL::vertexCuttingSubdivide( currObj );
      break;
    case 4 : // pentagon
      DLFL::pentagonalSubdivide( currObj, attrb1 ); // since default is 0 don't need if...else
      break;
    case 5 : // dual-pentagon
      if( battrb1 ) DLFL::pentagonalSubdivide2( currObj, attrb1 );
      else DLFL::pentagonalSubdivide2( currObj );
      break;
    case 6 : // honeycomb
      DLFL::honeycombSubdivide( currObj );
      break;
    case 7 : // doo-sabin
      if( battrb1 ) DLFL::dooSabinSubdivide( currObj, (bool)attrb1 );
      else DLFL::dooSabinSubdivide( currObj );
      break;
    case 8 : // doo-sabin-bc
      if( battrb1 ) DLFL::dooSabinSubdivideBC( currObj, (bool)attrb1 );
      else DLFL::dooSabinSubdivideBC( currObj );
      break;
    case 9 : // doo-sabin-bc-new
      DLFL::dooSabinSubdivideBCNew( currObj, attrb1, attrb2 ); // no defaults
      break;
    case 10 : // corner-cut
      DLFL::cornerCuttingSubdivide( currObj, attrb1 );
      break;
    case 11 : // modified-corner-cut
      DLFL::modifiedCornerCuttingSubdivide( currObj, attrb1 );
      break;
    case 12 : // root4
      if( battrb1 && battrb2 ) DLFL::root4Subdivide( currObj, attrb1, attrb2 );
      else if( battrb1 ) DLFL::root4Subdivide( currObj, attrb1 );
      else DLFL::root4Subdivide( currObj );
      break;
    case 13 : // catmull-clark
      DLFL::catmullClarkSubdivide( currObj );
      break;
    case 14 : // star
      if( battrb1 ) DLFL::starSubdivide( currObj, attrb1 );
      else DLFL::starSubdivide( currObj );
      break;
    case 15 : // sqrt3
      DLFL::sqrt3Subdivide( currObj );
      break;
    case 16 : // fractal
      if( battrb1 ) DLFL::fractalSubdivide( currObj, attrb1 );
      else DLFL::fractalSubdivide( currObj );
      break;
    case 17 : // stellate
      DLFL::stellateSubdivide( currObj );
      break;
    case 18 : // double-stellate
      DLFL::twostellateSubdivide( currObj, attrb1, attrb2 ); // no defaults
      break;
    case 19 : // dome
      DLFL::domeSubdivide( currObj, attrb1, attrb2 ); // no defaults
      break;
    case 20 : // dual-12.6.4
      DLFL::dual1264Subdivide( currObj, attrb1 ); // no defaults
      break;
		case 21 : // loop style
			DLFL::loopStyleSubdivide( currObj, attrb1 );
			break;
		case 24 : // root3 - dave
			DLFL::createDual(currObj, true); // Use accurate method
			DLFL::honeycombSubdivide(currObj);
			DLFL::createDual(currObj,true); // Use accurate method
			break;
		case 22 : // linear-vertex
		case 23 : // allfaces
    default : // allfaces
      if( battrb1 ) DLFL::subdivideAllFaces( currObj, (bool)attrb1 );
      else DLFL::subdivideAllFaces( currObj );
    }
  }
  Py_INCREF(Py_None);
  return Py_None;
}


static PyObject *dlfl_subdivide_face(PyObject *self, PyObject *args) {
  int fid;
  bool usequads = true;

  if( !PyArg_ParseTuple(args, "i|b", &fid, &usequads) )
    return NULL;

  if( currObj ) {
		DLFL::DLFLFacePtr fp = currObj->findFace(fid);
		if(fp) {
			DLFL::subdivideFace( currObj, fp, usequads );
			currObj->clearSelected( );
		}
  }

  Py_INCREF(Py_None);
  return Py_None;
}

static PyObject *dlfl_subdivide_faces(PyObject *self, PyObject *args) { 
	bool usequads = true;

	DLFL::DLFLFacePtrArray faces;
	DLFL::DLFLFacePtrArray::iterator it;

	PyObject *list, *faceid;

	if( !PyArg_ParseTuple( args, "O!|b", &PyList_Type, &list, &usequads ) )
		return NULL;

	int size = PyList_Size(list);

	if( currObj && size > 0 ) {

		for( int i = 0; i < size; i++ ) {
			faceid = PyList_GetItem(list,i);
			if( !PyInt_Check(faceid) ) break;
			uint id = (uint) PyInt_AsLong(faceid);
			DLFL::DLFLFacePtr fp = currObj->findFace( id );
			if(fp) { faces.push_back(fp); }
		}

		DLFL::subdivideFaces( currObj, faces, usequads );
	}

	Py_INCREF(Py_None);
  return Py_None;
}

static 
PyObject *dlfl_dual(PyObject *self, PyObject *args) 
{
  bool accurate = false;
  PyArg_ParseTuple(args, "|b", &accurate);
  if( currObj ) {
    DLFL::createDual( currObj, accurate );
    currObj->clearSelected( );
  }
  Py_INCREF(Py_None);
  return Py_None;
}

static PyObject *dlfl_connectEdges(PyObject *self, PyObject *args) { 
	int edgeId1, edgeId2;
	int faceId1, faceId2;
	bool loopCheck = false;

	if( !PyArg_ParseTuple(args, "(ii)(ii)|b", &edgeId1, &faceId1, &edgeId2, &faceId2, &loopCheck ) )
		return NULL;

	DLFL::DLFLEdgePtr e1 = currObj->findEdge(edgeId1);
	DLFL::DLFLEdgePtr e2 = currObj->findEdge(edgeId2);
	DLFL::DLFLFacePtr f1 = currObj->findFace(faceId1);
	DLFL::DLFLFacePtr f2 = currObj->findFace(faceId2);

	if( e1 && e2 && f1 && f2 ) {

		if( loopCheck )
			DLFL::connectEdges( currObj, e1, f1, e2, f2 );
		else
			DLFL::connectEdgesWithoutLoopCheck( currObj, e1, f1, e2, f2 );
	}

	Py_INCREF(Py_None);
	return Py_None;
}

static PyObject *dlfl_connectCorners(PyObject *self, PyObject *args) {
	int v1, v2;
	int f1, f2;
	int numsegs = 1;
	int maxconn = -1;

	DLFL::DLFLFacePtr fp1, fp2;
	DLFL::DLFLFaceVertexPtr fvp1, fvp2;
	char* dual = 0; int size;

	if( !PyArg_ParseTuple( args, "(ii)(ii)|iis#", &f1, &v1, &f2, &v2, &numsegs, &maxconn, dual, &size) )
		return NULL;

	if( currObj ) {
		fp1 = currObj->findFace(f1);
		fp2 = currObj->findFace(f2);
		if( fp1 && fp2 ) {
			fvp1 = fp1->findFaceVertex(v1);
			fvp2 = fp2->findFaceVertex(v2);
			if( fvp1 && fvp2 ) {
				if( dual && strncmp( dual, "dual", size) == 0 ) {
					DLFL::dualConnectFaces( currObj, fvp1, fvp2 );
				} else {
					DLFL::connectFaces( currObj, fvp1, fvp2, numsegs, maxconn);
				}
			}
		}
	}

	Py_INCREF( Py_None );
	return Py_None;
}

static PyObject *dlfl_connectFaces(PyObject *self, PyObject *args) {
	int f1, f2;
	int numsegs = 1;
	int maxconn = -1;

	DLFL::DLFLFacePtr fp1, fp2;

	if( !PyArg_ParseTuple( args, "ii|ii", &f1, &f2, &numsegs, &maxconn ) )
		return NULL;

	if( currObj ) {
		fp1 = currObj->findFace(f1);
		fp2 = currObj->findFace(f2);
		if( fp1 && fp2 ) {
		DLFL::connectFaces( currObj, fp1, fp2, numsegs );//, maxconn );
		}
	}

	Py_INCREF( Py_None );
	return Py_None;
}

static PyObject *dlfl_addHandle(PyObject *self, PyObject *args) {
	char *type;
	int size;
	int f1, f2, v1, v2;
	int numsegs, maxconn=-1, numtwists=0;
	double wt1, wt2; // weights

	DLFL::DLFLFacePtr fp1, fp2;
	DLFL::DLFLFaceVertexPtr fvp1, fvp2;

	if( !PyArg_ParseTuple( args, "s#(ii)(ii)i|ddii", type, &size, &f1, &v1, &f2, &v2, &numsegs, &wt1, &wt2, &maxconn, &numtwists ) )
		return NULL;

	fp1 = currObj->findFace(f1);
	fp2 = currObj->findFace(f2);
	if( fp1 && fp2 ) {
		fvp1 = fp1->findFaceVertex(v1);
		fvp2 = fp2->findFaceVertex(v2);
		if( fvp1 && fvp2 ) {
			
			if( strncmp( type, "hermite", size) == 0 ) {
				DLFL::hermiteConnectFaces( currObj, fvp1, fvp2, numsegs, wt1, wt2, maxconn, numtwists );
			} else if( strncmp( type, "bezier", size) == 0 ) {
				DLFL::bezierConnectFaces( currObj, fvp1, fvp2, numsegs, wt1, wt2 );
			}
		}
	}
	Py_INCREF( Py_None );
	return Py_None;
}

static PyObject *dlfl_rindmodel(PyObject *self, PyObject *args) { 
	bool useScaling;
	double thickScale; // thickness if useScaling is false, else scaleFactor
	bool uniform = true;

	DLFL::DLFLFacePtrArray faces;
	DLFL::DLFLFacePtrArray::iterator it;

	PyObject *list, *faceid;

	if( !PyArg_ParseTuple( args, "O!bd|d", &PyList_Type, &list, &useScaling, &thickScale, &uniform ) )
		return NULL;

	int size = PyList_Size(list);

	if( currObj && size > 0 ) {

		for( int i = 0; i < size; i++ ) {
			faceid = PyList_GetItem(list,i);
			if( !PyInt_Check(faceid) ) break;
			uint id = (uint) PyInt_AsLong(faceid);
			DLFL::DLFLFacePtr fp = currObj->findFace( id );
			if(fp) { faces.push_back(fp); }
		}

		if( useScaling ) {
			DLFL::createCrustWithScaling( currObj, thickScale );
		} else {
			DLFL::createCrust( currObj, thickScale, uniform );
		}

		for( it = faces.begin(); it != faces.end(); it++ )
			(*it)->setType(DLFL::FTHole);
		DLFL::punchHoles(currObj);
	}

	Py_INCREF(Py_None);
  return Py_None;
}

static PyObject *dlfl_wireframe(PyObject *self, PyObject *args) {
	double thickness = 0.1;
	bool split = true;

	if( !PyArg_ParseTuple(args, "d|b", &thickness, &split) )
		return NULL;

	if( currObj && thickness != 0 )
		DLFL::makeWireframe( currObj, thickness, split);

	Py_INCREF( Py_None );
	return Py_None;
}

static PyObject *dlfl_column(PyObject *self, PyObject *args) { 
	double thickness;
	int segments;

	if( !PyArg_ParseTuple(args, "di", &thickness, &segments) )
		return NULL;

	if( currObj )
		DLFL::makeWireframeWithColumns( currObj, thickness, segments );

	Py_INCREF( Py_None );
	return Py_None;
}

//static PyObject *dlfl_sierpinsky(PyObject *self, PyObject *args) { return NULL; }

//static PyObject *dlfl_multiface(PyObject *self, PyObject *args) { return NULL; }

//static PyObject *dlfl_menger(PyObject *self, PyObject *args) { return NULL; }

static PyObject *dlfl_translate(PyObject *self, PyObject *args) { 
	double x,y,z;
	bool relative = true;
	if( !PyArg_ParseTuple(args, "(ddd)|b", &x, &y, &z, &relative) )
		return NULL;
	if( currObj ) {
		Vector3d pos(x,y,z);
		if( relative )
			pos = currObj->position + pos;
		currObj->position = pos;
		currObj->transform();
	}

	Py_INCREF(Py_None);
	return Py_None;
}

static PyObject *dlfl_scale(PyObject *self, PyObject *args) {
	double x,y,z;
	if( !PyArg_ParseTuple(args, "(ddd)", &x, &y, &z) )
		return NULL;
	if( currObj ) {
		Vector3d scale(x,y,z);
		currObj->scale_factor = scale;
		currObj->transform();
	}

	Py_INCREF(Py_None);
	return Py_None;
}

static PyObject *dlfl_freezeTrans(PyObject *self, PyObject *args) {
	if( currObj ) {
		currObj->freezeTransformations();
	}
	Py_INCREF(Py_None);
	return Py_None;
}

static PyObject *dlfl_move(PyObject *self, PyObject *args) {
	PyObject *list, *id;
	bool relative = true;
	double x,y,z;
	if( !PyArg_ParseTuple(args, "O!(ddd)|b", &PyList_Type, &list, &x, &y, &z, &relative) )
		return NULL;
	int size = PyList_Size(list);
	if( size < 0 )
		return NULL;

	if( currObj ) {
		Vector3d vec = Vector3d(x,y,z);
		DLFL::DLFLVertexPtrArray vparray;
		for( int i = 0; i < size; i++ ) {
			id = PyList_GetItem(list, i);
			if( !PyInt_Check(id) ) break;
			int vid = PyInt_AsLong( id );
			DLFL::DLFLVertexPtr vp = currObj->findVertex(vid);
			if( vp )
				vparray.push_back(vp);
		}
		
		for( int i = 0; i < (int)vparray.size(); i++ ) {
			if( relative ) {
				(vparray[i])->coords += vec;
			} else {
				(vparray[i])->coords = vec;
			}
		}
	}
	Py_INCREF(Py_None);
	return Py_None;
}

/* C API Stuff */
//static void **PyDLFL_API;

#define NUM_C_API_FUNCS 2

static void
PyDLFL_UsingGUI( bool ugui ) 
{ 
  usingGUI = true;
}

static void
PyDLFL_PassObject( DLFL::DLFLObject* obj )
{
  currObj = obj;
}

/**
 * Initialize
 */

PyMODINIT_FUNC initdlfl(void)
{
	PyObject *dlfl;
	static void *PyDLFL_API[NUM_C_API_FUNCS];

	dlfl = Py_InitModule("dlfl", DLFLMethods);

	DLFLError = PyErr_NewException("dlfl.error", NULL, NULL);
	Py_INCREF(DLFLError);
	PyModule_AddObject(dlfl, "error", DLFLError);

	PyObject *dlflDict = PyModule_GetDict(dlfl); 

	/* Initialize the C API pointer array */ 
	PyDLFL_API[0] = (void *)PyDLFL_UsingGUI; 
	PyDLFL_API[1] = (void *)PyDLFL_PassObject;
	/* Create a CObject containing the API pointer array's address */ 
	PyObject *c_api_object = PyCObject_FromVoidPtr((void *)PyDLFL_API, NULL); 
	if (c_api_object != NULL) { 
		/* Create a name for this object in the module’s namespace */ 
		PyDict_SetItemString(dlflDict, "_C_API", c_api_object); 
		Py_DECREF(c_api_object); 
	}
}

