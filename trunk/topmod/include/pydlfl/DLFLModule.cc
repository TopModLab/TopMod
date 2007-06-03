
#include <Python.h>
#include "DLFLCore.hh"

/// Prototypes

static PyObject *dlfl_load_obj(PyObject *self, PyObject *args);
static PyObject *dlfl_kill_obj(PyObject *self, PyObject *args);
static PyObject *dlfl_switch_obj(PyObject *self, PyObject *args);
static PyObject *dlfl_save_obj(PyObject *self, PyObject *args);
static PyObject *dlfl_query_obj(PyObject *self, PyObject *args);

static PyObject *dlfl_insert_edge(PyObject *self, PyObject *args);
static PyObject *dlfl_delete_edge(PyObject *self, PyObject *args);
static PyObject *dlfl_collapse_edge(PyObject *self, PyObject *args);
static PyObject *dlfl_subdivide_edge(PyObject *self, PyObject *args);

static PyObject *dlfl_print_object(PyObject *self, PyObject *args);

static PyObject *dlfl_boundary_walk(PyObject *self, PyObject *args);
static PyObject *dlfl_walk_vertices(PyObject *self, PyObject *args);
static PyObject *dlfl_walk_edges(PyObject *self, PyObject *args);

PyMODINIT_FUNC initdlfl(void);

static PyMethodDef DLFLMethods[] = {
  {"load",           dlfl_load_obj,       METH_VARARGS, "load(string)"},
  {"save",           dlfl_save_obj,       METH_VARARGS, "save(string)"},  
  {"kill",           dlfl_kill_obj,       METH_VARARGS, "kill(object id)"},
  {"switch",         dlfl_switch_obj,     METH_VARARGS, "switch(object id)"},
  {"query",          dlfl_query_obj,      METH_VARARGS, "query()"},
  {"insert_edge",    dlfl_insert_edge,    METH_VARARGS, "insert_edge((face,vert),(face,vert))"},
  {"delete_edge",    dlfl_delete_edge,    METH_VARARGS, "Delete an Edge."},
  {"collapse_edge",  dlfl_collapse_edge,  METH_VARARGS, "Collapse an Edge."},
  {"subdivide_edge", dlfl_subdivide_edge, METH_VARARGS, "Subdivide an Edge."},
  {"walk",           dlfl_boundary_walk,  METH_VARARGS, "walk(int)"},
  {"walk_vertices",  dlfl_walk_vertices,  METH_VARARGS, "walk_vertices(int)"},
  {"walk_edges",     dlfl_walk_edges,     METH_VARARGS, "walk_edges(int)"},
  {"print_object",   dlfl_print_object,   METH_VARARGS, "Print object information"},
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
  } else {
    objId = -2;
  }

  return Py_BuildValue("i", objId );
}

static PyObject *
dlfl_query_obj(PyObject *self, PyObject *args)
{
  return Py_BuildValue("i", usingGUI );
}

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
  int faceId1;
  int vertId1;
  int faceId2;
  int vertId2;
  int edgeId = -1;

  if( !PyArg_ParseTuple(args, "(ii)(ii)", &faceId1, &vertId1, &faceId2, &vertId2) )
    return NULL;

  if( currObj ) {
    edgeId = DLFL::insertEdge( currObj, faceId1, vertId1, faceId2, vertId2 );
  }

  return Py_BuildValue("i", edgeId );
}

static PyObject *
dlfl_delete_edge(PyObject *self, PyObject *args)
{
  int edgeId;

  if( !PyArg_ParseTuple(args, "i", &edgeId) )
    return NULL;
  if( !currObj )
    return NULL;

  DLFL::deleteEdgeID( currObj, edgeId );
  return Py_BuildValue("i", edgeId );
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
    return Py_BuildValue("i", vertId );
  } else if( PyArg_ParseTuple(args, "ii", &numDivs, &edgeId ) ) {
    vector<int> verts = DLFL::subdivideEdgeID( currObj, numDivs, edgeId );
    PyObject *vert;
    PyObject *list = PyList_New(verts.size());
    for( int i = 0; i < verts.size(); i++) {
      vert = Py_BuildValue("i", verts[i]);
      PyList_SetItem(list, i, vert);
    }
    Py_INCREF(list);
    return list;
  }

  Py_INCREF(Py_None);
  return Py_None;
}

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

static PyObject *dlfl_walk_vertices(PyObject *self, PyObject *args) {
  int faceId;

  vector<int> verts; // output: edges walked
  PyObject *list; // py output: edges walked
  PyObject *vert; // for looping

  if( !currObj )
    return NULL;

  if( !PyArg_ParseTuple(args, "i", &faceId) )
    return NULL;
  verts = currObj->vertWalk( faceId );

  list = PyList_New(verts.size());
  for( int i = 0; i < verts.size(); i++) {
    vert = Py_BuildValue("i", verts[i]);
    PyList_SetItem(list, i, vert);
  }
  Py_INCREF(list);
  return list;
}

static PyObject *dlfl_walk_edges(PyObject *self, PyObject *args) {
  int faceId; // input: face to walk

  vector<int> edges; // output: edges walked
  PyObject *list; // py output: edges walked
  PyObject *edge; // for looping
  
  if( !currObj )
    return NULL;

  if( !PyArg_ParseTuple(args, "i", &faceId) )
    return NULL;
  edges = currObj->edgeWalk( faceId );

  list = PyList_New(edges.size());
  for( int i = 0; i < edges.size(); i++) {
    edge = Py_BuildValue("i", edges[i]);
    PyList_SetItem(list, i, edge);
  }
  Py_INCREF(list);
  return list;
}

static PyObject *
dlfl_print_object(PyObject *self, PyObject *args) {
  if( !currObj )
    return NULL;

  currObj->print();

  Py_INCREF(Py_None);
  return Py_None;
}

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
    PyObject *m;
    static void *PyDLFL_API[NUM_C_API_FUNCS];

    m = Py_InitModule("dlfl", DLFLMethods);

    DLFLError = PyErr_NewException("dlfl.error", NULL, NULL);
    Py_INCREF(DLFLError);
    PyModule_AddObject(m, "error", DLFLError);

    /* Initialize the C API pointer array */ 
    PyDLFL_API[0] = (void *)PyDLFL_UsingGUI; 
    PyDLFL_API[1] = (void *)PyDLFL_PassObject;
    /* Create a CObject containing the API pointer array's address */ 
    PyObject *c_api_object = PyCObject_FromVoidPtr((void *)PyDLFL_API, NULL); 
    if (c_api_object != NULL) { 
      /* Create a name for this object in the module’s namespace */ 
      PyObject *d = PyModule_GetDict(m); 
      PyDict_SetItemString(d, "_C_API", c_api_object); 
      Py_DECREF(c_api_object); 
    }
}

