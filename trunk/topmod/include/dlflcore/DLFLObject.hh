/**
 * \file DLFLObject.hh
 */

#ifndef _DLFL_OBJECT_HH_
#define _DLFL_OBJECT_HH_

// The Doubly Linked Face List object classs
// Contains a vertex list, edge list and a face list
// The lists actually store pointerss

#include "DLFLCommon.hh"
#include "DLFLVertex.hh"
#include "DLFLFaceVertex.hh"
#include "DLFLEdge.hh"
#include "DLFLFace.hh"
#include "DLFLMaterial.hh"
#include <Graphics/Transform.hh>

namespace DLFL {

/**
 * DLFLObject class
 */
class DLFLObject {
public :

  /// Constructor
  DLFLObject()
    : position(), scale_factor(1), rotation(),
      vertex_list(), edge_list(), face_list()/*, patch_list(), patchsize(4)*/ {
    assignID();
    // Add a default material
    matl_list.push_back(new DLFLMaterial("default",0.5,0.5,0.5));
    mFilename = NULL;
  };

  /// Destructor
  ~DLFLObject( ) {
    clearLists();
    if( mFilename ) { delete [] mFilename; mFilename = NULL; }
  };

protected :

  // Distinct ID for each instance
  static uint suLastID;

  // Generate a new unique ID
  static uint newID( ) {
    uint temp = suLastID;
    suLastID++;
    return temp;
  };

  static Transformation tr;                         // For doing GL transformations

public :

  static DLFLVertexPtrArray vparray;                // For selection
  static DLFLEdgePtrArray   eparray;                // For selection
  static DLFLFacePtrArray   fparray;                // For selection
  static DLFLFaceVertexPtrArray fvparray;           // For selection

  Vector3d           position;                      // Position of object
  Vector3d           scale_factor;                  // Scale of object
  Quaternion         rotation;                      // Rotation of object

  inline void removeVertex( DLFLVertexPtr vp ) { vertex_list.remove(vp); };
  inline void removeEdge( DLFLEdgePtr ep ) { edge_list.remove(ep); };
  inline void removeFace( DLFLFacePtr fp ) { face_list.remove(fp); };

  void computeNormals( );

protected :

  DLFLVertexPtrList          vertex_list;           // The vertex list
  DLFLEdgePtrList            edge_list;             // The edge list
  DLFLFacePtrList            face_list;             // The face list
  DLFLMaterialPtrList        matl_list;             // Material list (for rendering)

  //TMPatchFacePtrList patch_list;		 // List of patch faces
  //int patchsize;				 // Size of each patch
     
  uint uID;                                      // ID for this object
  char *mFilename;
  // Assign a unique ID for this instance
  void assignID( ) { uID = DLFLObject::newID(); };

  // Free the memory allocated for the patches
  /*void destroyPatches( ) {
    TMPatchFacePtrList::iterator first = patch_list.begin(), last = patch_list.end();
    TMPatchFacePtr pfp = NULL;
    while ( first != last ) {
      pfp = (*first); ++first;
      delete pfp;
    }
    patch_list.clear();
    };*/

  // Free all the pointers in the lists and clear the lists
  void clearLists( ) {
    clear(vertex_list);
    clear(edge_list);
    clear(face_list);
    clear(matl_list);
    //destroyPatches();
  };

private :

  /// Copy Constructor - make proper copy, don't just copy pointers
  DLFLObject( const DLFLObject& dlfl )
    : position(dlfl.position), scale_factor(dlfl.scale_factor), rotation(dlfl.rotation),
      vertex_list(dlfl.vertex_list), edge_list(dlfl.edge_list), face_list(dlfl.face_list), matl_list(dlfl.matl_list),
      //patch_list(dlfl.patch_list), patchsize(dlfl.patchsize),
      uID(dlfl.uID) { };

  // Assignment operator
  DLFLObject& operator=( const DLFLObject& dlfl ) {
    position = dlfl.position;
    scale_factor = dlfl.scale_factor;
    rotation = dlfl.rotation;

    // Destroy the existing object
    clearLists();

    // Copy the lists from the new object
    vertex_list = dlfl.vertex_list;
    edge_list = dlfl.edge_list;
    face_list = dlfl.face_list;
    matl_list = dlfl.matl_list;
    //patch_list = dlfl.patch_list;
    //patchsize = dlfl.patchsize;
	 
    uID = dlfl.uID;
    return (*this);
  };

public :

  // Dump contents of this object
  void dump(ostream& o) const;

  //--- Query Functions ---//

  // Compute the genus of the mesh using Euler formula
  int genus( ) const {
    int v = num_vertices();
    int e = num_edges();
    int f = num_faces();
    int g = 1 - ( v - e +  f )/2;
    return g;
  };

  inline uint getID( ) const { return uID; };
  inline size_t num_vertices( ) const { return vertex_list.size(); };
  inline size_t num_edges( ) const { return edge_list.size(); };
  inline size_t num_faces( ) const { return face_list.size(); };
  inline size_t num_materials( ) const { return matl_list.size(); }

  // Probably should switch to just using these in the future
  // then you could get rid of all these accessors below.
  const DLFLVertexPtrList& getVertexList( ) const { return vertex_list; };
  const DLFLEdgePtrList& getEdgeList( ) const { return edge_list; };
  //const DLFLFacePtrList& getFaceList( ) const { return face_list; };
  DLFLFacePtrList& getFaceList( ) { return face_list; }; // needed not const for subdivideAllFaces

  //-- List based access to the 3 lists --//
  DLFLVertexPtr firstVertex( ) { return vertex_list.front(); }
  DLFLEdgePtr firstEdge( ) { return edge_list.front(); }
  DLFLFacePtr firstFace( ) { return face_list.front(); }
  DLFLMaterialPtr firstMaterial( ) { return matl_list.front(); }
  DLFLVertexPtr lastVertex( ) { return vertex_list.back(); };
  DLFLEdgePtr lastEdge( ) { return edge_list.back(); };
  DLFLFacePtr lastFace( ) { return face_list.back(); }
  DLFLMaterialPtr lastMaterial( ) { return matl_list.back(); }
  DLFLVertexPtr findVertex(const uint vid);
  DLFLEdgePtr findEdge(const uint eid);
  DLFLFacePtr findFace(const uint fid);
  DLFLVertexPtrList::iterator beginVertex( ) { return vertex_list.begin(); }
  DLFLVertexPtrList::iterator endVertex( ) { return vertex_list.end(); }
  DLFLEdgePtrList::iterator beginEdge( ) { return edge_list.begin(); }
  DLFLEdgePtrList::iterator endEdge( ) { return edge_list.end(); }
  DLFLFacePtrList::iterator beginFace( ) { return face_list.begin(); }
  DLFLFacePtrList::iterator endFace( ) { return face_list.end(); }
  DLFLFacePtrList::reverse_iterator rbeginFace( ) { return face_list.rbegin(); }
  DLFLFacePtrList::reverse_iterator rendFace( ) { return face_list.rend(); }

  //--- Access the lists through arrays ---//
  void getVertices(DLFLVertexPtrArray& vparray) {
    vparray.clear(); vparray.reserve(vertex_list.size());
    DLFLVertexPtrList::const_iterator first = vertex_list.begin(), last = vertex_list.end();
    while ( first != last ) {
      vparray.push_back(*first);
      ++first;
    }
  };

  void getEdges(DLFLEdgePtrArray& eparray) {
    eparray.clear(); eparray.reserve(edge_list.size());
    DLFLEdgePtrList::const_iterator first = edge_list.begin(), last = edge_list.end();
    while ( first != last ) {
      eparray.push_back(*first);
      ++first;
    }
  };

  void getFaces(DLFLFacePtrArray& fparray) {
    fparray.clear(); fparray.reserve(face_list.size());
    DLFLFacePtrList::const_iterator first = face_list.begin(), last = face_list.end();
    while ( first != last ) {
      fparray.push_back(*first);
      ++first;
    }
  };

  //--- Terminal printing functions : useful for debugging ---//
  void print( ) const {
    // Print a summary of the DLFLObject
    cout << "Number of vertices : " << vertex_list.size() << endl;
    cout << "Number of faces : " << face_list.size() << endl;
    cout << "Number of edges : " << edge_list.size() << endl;
    cout << "Number of materials : " << matl_list.size() << endl;
    cout << "Genus : " << genus() << endl;
  };

  void printVertexList( ) const {
    cout << "Vertex List" << endl;
    DLFLVertexPtrList::const_iterator first = vertex_list.begin(), last = vertex_list.end();
    while ( first != last ) {
      (*first)->print();
      ++first;
    }
  };

  void printEdgeList( ) const {
    cout << "Edge List" << endl;
    DLFLEdgePtrList::const_iterator first = edge_list.begin(), last = edge_list.end();
    while ( first != last ) {
      (*first)->print();
      ++first;
    }
  };

  void printFaceList( ) const {
    cout << "Face List" << endl;
    /*
    DLFLFacePtrList::const_iterator first = face_list.begin(), last = face_list.end();
    while ( first != last ) {
      cout << *(*first) << endl;
      ++first;
      }*/
  };
     
  void printFaces( ) const {
    cout << "Faces" << endl;

    DLFLFacePtrList::const_iterator first = face_list.begin(), last = face_list.end();
    while ( first != last ) {
      (*first)->print();
      ++first;
    }
  };
     
  //--- Mutative Functions ---//
  // Reset the whole object
  void reset( ) {
    position.reset(); scale_factor.set(1,1,1); rotation.reset();
    clearLists();
    // Add a default material
    matl_list.push_back(new DLFLMaterial("default",0.5,0.5,0.5));
  };

  void makeVerticesUnique( ) {
    // Make vertices unique
    DLFLVertexPtrList::iterator vfirst=vertex_list.begin(), vlast=vertex_list.end();
    while ( vfirst != vlast ) {
      (*vfirst)->makeUnique();
      ++vfirst;
    }
  };

  void makeEdgesUnique( ) {
    // Make edges unique
    DLFLEdgePtrList::iterator efirst=edge_list.begin(), elast=edge_list.end();
    while ( efirst != elast ) {
      (*efirst)->makeUnique();
      ++efirst;
    }
  }

  void makeFacesUnique( ) {
    // Make faces unique
    DLFLFacePtrList::iterator ffirst=face_list.begin(), flast=face_list.end();
    while ( ffirst != flast ) {
      (*ffirst)->makeUnique();
      ++ffirst;
    }
  };

  void makeUnique( ) {
    assignID();
    makeVerticesUnique();
    makeEdgesUnique();
    makeFacesUnique();
  };

  void destroy( ) {
    // Clear the DLFL structures
    clearLists();
  };

  // Combine two DLFLObject instances into 1 object
  // Lists of second object are cleared. Otherwise when that object is destroyed,
  // pointers in this object will become invalid.
  void splice(DLFLObject& object);

  // Reverse the orientation of all faces in the object
  // This also requires reversing all edges in the object
  void reverse( );

  void addVertex(const DLFLVertex& vertex);         // Insert a copy
  void addVertex(DLFLVertexPtr vertexptr);          // Insert a copy
  void addVertexPtr(DLFLVertexPtr vertexptr) {
    // Insert the pointer.
    // **** WARNING!!! **** Pointer will be freed when list is deleted
    vertex_list.push_back(vertexptr);
  };

  void addEdge(const DLFLEdge& edge);               // Insert a copy
  void addEdge(DLFLEdgePtr edgeptr);                // Insert a copy
  void addEdgePtr(DLFLEdgePtr edgeptr) {
    // Insert the pointer.
    // **** WARNING!!! **** Pointer will be freed when list is deleted
    edge_list.push_back(edgeptr);
  };

  void addFace(const DLFLFace& face);               // Insert a copy
  void addFace(DLFLFacePtr faceptr);                // Insert a copy
  void addFacePtr(DLFLFacePtr faceptr) {
    // Insert the pointer.
    // **** WARNING!!! **** Pointer will be freed when list is deleted
    //if ( faceptr->material() == NULL )
      // If Face doesn't have a material assigned to it, assign the default material
    faceptr->setMaterial(matl_list.front());
    face_list.push_back(faceptr);
  };

  DLFLVertexPtr getVertexPtr(uint index) const {
    if ( index >= vertex_list.size() ) return NULL;
    DLFLVertexPtrList::const_iterator i=vertex_list.begin();
    advance(i,index);
    return (*i);
  };
     
  DLFLVertexPtr getVertexPtrID(uint id) const {
    DLFLVertexPtrList::const_iterator first=vertex_list.begin(), last=vertex_list.end();
    DLFLVertexPtr vptr = NULL;
    while (first != last) {
      if ( (*first)->getID() == id ) {
	vptr = (*first);
	break;
      }
      ++first;
    }
    return vptr;
  };

  void updateEdgeList( ) {
    DLFLEdgePtrList::iterator first=edge_list.begin(), last=edge_list.end();
    while ( first != last ) {
      (*first)->updateFaceVertices();
      ++first;
    }
  };

  // Check if an edge exists between two given vertices
  bool edgeExists(DLFLVertexPtr vptr1, DLFLVertexPtr vptr2);

  // Check if an edge exists between vertices given by two corners
  // Simply calls above function
  inline bool edgeExists(DLFLFaceVertexPtr fvptr1, DLFLFaceVertexPtr fvptr2) {
    return edgeExists(fvptr1->vertex,fvptr2->vertex);
  };

  // Check if a given edge exists in the edge list. If it does pointer is set to that edge
  bool edgeExists(const DLFLEdge& e, DLFLEdgePtr& eptr) {
    DLFLEdgePtrList::iterator first=edge_list.begin(), last=edge_list.end();
    while ( first != last ) {
      if ( (*(*first)) == e ) {
	eptr = *first;
	return true;
      }
      ++first;
    }
    eptr = NULL;
    return false;
  };

  void addEdges(DLFLEdge * edges, int num_edges) {
    DLFLEdgePtr eptr;
  
    for (int i=0; i < num_edges; ++i) {
      if ( edgeExists(edges[i],eptr) == false ) {
	addEdge(edges[i]);
      } else {
	// If Edge already exists, then the second FaceVertexPtr in the Edge must
	// be changed to that from the new Edge with the same ID as the second one
	// in this Edge
	int id2 = (eptr->getFaceVertexPtr2())->getVertexID();
	int eid1 = (edges[i].getFaceVertexPtr1())->getVertexID();

	if (eid1 == id2)
	  eptr->setFaceVertexPtr2(edges[i].getFaceVertexPtr1());
	else
	  eptr->setFaceVertexPtr2(edges[i].getFaceVertexPtr2());
      }
      eptr = NULL;
    }
  };

  void addEdgesWithoutCheck(DLFLEdge * edges, int num_edges) {
    for (int i=0; i < num_edges; ++i)
      addEdge(edges[i]);
  };

  void updateFaceList( ) {
    DLFLFacePtrList::iterator first=face_list.begin(), last=face_list.end();
    while (first != last) {
      (*first)->updateFacePointers();
      (*first)->addFaceVerticesToVertices();
      ++first;
    }
  };

  void createFace(const Vector3dArray& verts, DLFLMaterialPtr matl = NULL, bool set_type = false);

  DLFLFaceVertexPtr createPointSphere(const Vector3d& v, DLFLMaterialPtr matl = NULL );

  // Compute the bounding box for this object
  void boundingBox(Vector3d& min, Vector3d& max) const;
  bool boundaryWalkID( uint faceId );
  vector<int> vertWalk( uint faceId );
  vector<int> edgeWalk( uint faceId );
  void boundaryWalk(uint face_index);
  void vertexTrace(uint vertex_index);

  void readObject( istream& i );
  void readDLFL( istream& i, bool clearold = true );
  void writeObject( ostream& o, bool with_normals, bool with_tex_coords );
  void writeDLFL(ostream& o, bool reverse_faces);
  inline void setFilename( char *filename ) { 
    if( mFilename) { delete [] mFilename; mFilename = NULL; }
    mFilename = filename; 
  };
  inline char* getFilename( ) { return mFilename; }
  
  DLFLMaterialPtr findMaterial( const RGBColor& color ) {
    DLFLMaterialPtr matl = NULL;
    DLFLMaterialPtrList::iterator first, last;
    first = matl_list.begin(); last = matl_list.end();
    while ( first != last ) {
      if ( (*first)->equals(color) ) {
	matl = (*first); break;
      }
      ++first;
    }

    return matl;
  };

  void setColor( const RGBColor& col ) {
    // matl_list[0] is always the default material
    matl_list.front()->setColor(col);
  };

};
  // Build the list of patch faces
  /*void createPatches() {
    destroyPatches();
    DLFLFacePtrList::iterator ffirst = face_list.begin(), flast = face_list.end();
    DLFLFacePtr fp = NULL;
    TMPatchFacePtr pfp = NULL;
       
    while ( ffirst != flast ) {
      fp = (*ffirst); ++ffirst;
      pfp = new TMPatchFace(patchsize);
      pfp->setDLFLFace(fp); pfp->createPatches();
      patch_list.push_back(pfp);
      }

    // Adjust the edge points for all patches
    DLFLEdgePtrList::iterator efirst = edge_list.begin(), elast = edge_list.end();
    DLFLEdgePtr ep = NULL;
    DLFLFaceVertexPtr fvp1,fvp2;
    TMPatchPtr pp1, pp2;
    Vector3d p00,p01,p10,p11,ip;
    while ( efirst != elast ) {
      ep = (*efirst); ++efirst;
      ep->getCorners(fvp1,fvp2);
      pp1 = fvp1->getPatchPtr(); pp2 = fvp2->getPatchPtr();
      
      p00 = pp1->getControlPoint(2,0); p01 = pp2->getControlPoint(2,0);
      p10 = pp1->getControlPoint(3,1); p11 = pp2->getControlPoint(3,1);
      ip = intersectCoplanarLines(p00,p01,p10,p11);
      
      pp1->setControlPoint(3,0,ip); pp2->setControlPoint(3,0,ip);
      pp1->updateGLPointArray(); pp2->updateGLPointArray();
      
      pp1 = fvp1->next()->getPatchPtr(); pp2 = fvp2->next()->getPatchPtr();
      pp1->setControlPoint(0,3,ip); pp2->setControlPoint(0,3,ip);
      pp1->updateGLPointArray(); pp2->updateGLPointArray();
    }
    
    // Adjust the vertex points for 4-valence vertices
    DLFLVertexPtrList::iterator vfirst = vertex_list.begin(), vlast = vertex_list.end();
    DLFLVertexPtr vp = NULL;
    while ( vfirst != vlast ) {
      vp = (*vfirst); ++vfirst;
      if ( vp->valence() == 4 ) {
	DLFLFaceVertexPtrArray vcorners;
	vp->getOrderedCorners(vcorners);
	pp1 = vcorners[0]->getPatchPtr(); pp2 = vcorners[2]->getPatchPtr();

	p00 = pp1->getControlPoint(1,0); p01 = pp2->getControlPoint(1,0);
	p10 = pp1->getControlPoint(0,1); p11 = pp2->getControlPoint(0,1);
	ip = intersectCoplanarLines(p00,p01,p10,p11);

	for (int i=0; i < 4; ++i) {
	  pp1 = vcorners[i]->getPatchPtr();
	  pp1->setControlPoint(0,0,ip);
	  pp1->updateGLPointArray();
	}
      }
    }
    }
  };*/
} // end namespace
#endif /* _DLFL_OBJECT_HH_ */
