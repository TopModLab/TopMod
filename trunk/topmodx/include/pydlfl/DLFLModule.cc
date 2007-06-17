#include <Python.h>

#include <DLFLCore.hh>
#include <DLFLExtrude.hh>
#include <DLFLSubdiv.hh>
#include <DLFLDual.hh>

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

static PyObject *dlfl_faces(PyObject *self, PyObject *args);
static PyObject *dlfl_edges(PyObject *self, PyObject *args);
static PyObject *dlfl_verts(PyObject *self, PyObject *args);
static PyObject *dlfl_faceverts(PyObject *self, PyObject *args);

static PyObject *dlfl_boundary_walk(PyObject *self, PyObject *args);
static PyObject *dlfl_walk_vertices(PyObject *self, PyObject *args);
static PyObject *dlfl_walk_edges(PyObject *self, PyObject *args);

/* Auxiliary */
static PyObject *dlfl_extrude(PyObject *self, PyObject *args);
static PyObject *dlfl_subdivide(PyObject *self, PyObject *args);
static PyObject *dlfl_subdiv_face(PyObject *self, PyObject *args);
static PyObject *dlfl_dual(PyObject *self, PyObject *args);

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
  {"faces",          dlfl_faces,          METH_VARARGS, "Grab all/selected the faces of the object"},
  {"edges",          dlfl_edges,          METH_VARARGS, "Grab all/selected the edges of the object"},
  {"verts",          dlfl_verts,          METH_VARARGS, "Grab all/selected the vertices of the object"},
  {"faceverts"       dlfl_faceverts,      METH_VARARGS, "Grab all/selected the face-vertices of the object"},
  {"walk",           dlfl_boundary_walk,  METH_VARARGS, "walk(int)"},
  {"walk_vertices",  dlfl_walk_vertices,  METH_VARARGS, "walk_vertices(int)"},
  {"walk_edges",     dlfl_walk_edges,     METH_VARARGS, "walk_edges(int)"},
  {"print_object",   dlfl_print_object,   METH_VARARGS, "Print object information"},
  /* Auxiliary Below */
  {"extrude",        dlfl_extrude,        METH_VARARGS, "Extrude a face"},
  {"subdivide",      dlfl_subdivide,      METH_VARARGS, "Subdivide a mesh"},
  {"subdiv_face",    dlfl_subdiv_face,    METH_VARARGS, "Subdivide a Face"},
  {"dual",           dlfl_dual,           METH_VARARGS, "Dual of mesh"},
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

/* Mesh Identification Helpers (Verts,Edges,Faces, FaceVertices ) */
static PyObject *
dlfl_faces(PyObject *self, PyObject *args)
{
  if( !currObj ) {
    Py_INCREF(Py_None);
    return Py_None;
  }

  // Storage/Output
  vector<int> faces;
  PyObject *flist;
  PyObject *face; // tmp for each loop

  verts = currObj->get

  list = PyList_New(verts.size());
  for( int i = 0; i < verts.size(); i++) {
    vert = Py_BuildValue("i", verts[i]);
    PyList_SetItem(list, i, vert);
  }
  Py_INCREF(list);
  return list;
}

static PyObject *
dlfl_edges(PyObject *self, PyObject *args)
{
}

static PyObject *
dlfl_verts(PyObject *self, PyObject *args)
{ 
}

static PyObject *
dlfl_faceverts(PyObject *self, PyObject *args)
{
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

 /* Auxiliary Below */
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
  DLFL::DLFLFacePtr ofp = 0;
  int outFaceID = -1;

  if( choice != -1 ) {
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

  int choiceSize = 22;
  const char* choices[] = { "loop",
			    "checker",
			    "simplest",
			    "vertex-cut",
			    "pentagon",
			    "dual-pentagon",
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
			    "linear-vertex"};

  char* subdivType;
  int size;
  double attrb1 = 0, attrb2 = 0; 
  bool battrb1 = false, battrb2 = false; 
  int argc = PyArg_ParseTuple(args, "s#|dd", &subdivType, &size, &attrb1, &attrb2 );
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
      DLFL::cornerCuttingSubdivide( currObj );
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
    case 21 : // linear-vertex
    default : // linear-vertex
      if( battrb1 ) DLFL::subdivideAllFaces( currObj, (bool)attrb1 );
      else DLFL::subdivideAllFaces( currObj );
    }
  }
  Py_INCREF(Py_None);
  return Py_None;
}


static PyObject *
dlfl_subdiv_face(PyObject *self, PyObject *args) {
  int fid;
  bool usequads = true;

  if( !PyArg_ParseTuple(args, "i|b", &fid, &usequads) )
    return NULL;

  if( currObj )
    DLFL::subdivideFace( currObj, currObj->findFace(fid), usequads );

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

