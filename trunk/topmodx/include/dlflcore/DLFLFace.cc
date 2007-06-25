/**
 * \file DLFLFace.cc
 */

// Non-inline source code for DLFLFace class

#include "DLFLFace.hh"
#include "DLFLEdge.hh"
#include "DLFLFaceVertex.hh"
#include "DLFLVertex.hh"
//#include "TMPatchFace.hh"

namespace DLFL {

  // Texture coordinates for the 4 corners of a unit square
  static Vector2d unittexcoords[] = { Vector2d(0,0), Vector2d(1,0), Vector2d(1,1), Vector2d(0,1) };

  // Define the static variable. Initialized to 0
  uint DLFLFace::suLastID = 0;

  /*
    Traversing the Face using the DLFLFaceVertexPtr

    The list of DLFLFaceVertexPtr s is circular. So we have to ensure that every node get's traversed
    exactly once.
    head -> points to beginning of Face. 

    if ( head )
    {
    DLFLFaceVertexPtr current = head;
    (do operation on current)
    current = current->next();
    while ( current != head )
    {
    (do operation on current)
    current = current->next();
    }
    }
  */


  // Dump contents of object
  void DLFLFace::dump(ostream& o) const {
    o << "DLFLFace" << endl
      //    << "  ID : " << uID << endl
      //    << "  MatlPtr : " << matl_ptr << endl
      //    << "  Type : " << ftType << endl
      << "  DLFLFaceVertexPtr list" << endl;

    if ( head )
      {
				DLFLFaceVertexPtr current = head; 
				int i=0;

				o << "    " << i << " : " << current << endl; current->dump(o);
				++i; current = current->next();
				while ( current != head )
          {
            o << "    " << i << " : " << current << endl; current->dump(o);
            ++i; current = current->next();
          }
      }
    o << endl;
  }

  // Constructor
  DLFLFace::DLFLFace( DLFLMaterialPtr mp )
    : head(NULL), matl_ptr(mp), ftType(FTNormal), auxcoords(), auxnormal(), centroid(), normal(), flags(0)
  {
    assignID();
    // Add this face to the face-list of any associated material
    if ( matl_ptr ) matl_ptr->addFace(this);
  }

  // Copy constructor
  DLFLFace::DLFLFace(const DLFLFace& face)
    : uID(face.uID), head(NULL), matl_ptr(face.matl_ptr), ftType(face.ftType),
      auxcoords(face.auxcoords), auxnormal(face.auxnormal), centroid(face.centroid), normal(face.normal), flags(face.flags)
  {
    copy(face.head);
    // Add this face to the face-list of any associated material
    if ( matl_ptr ) matl_ptr->addFace(this);
  }

  // Destructor
  DLFLFace::~DLFLFace() {
    // Remove this face from the face-list of the material if there is an associated material
    if ( matl_ptr ) matl_ptr->deleteFace(this);
    destroy();
  }

  // Copy face from another face. Doesn't destroy existing face. So it's more of an insert
  void DLFLFace::copy(DLFLFaceVertexPtr ptr) {
    if ( ptr ) {
      DLFLFaceVertexPtr current = ptr;
      addVertexPtr(current->copy());
      current = current->next();
      while ( current != ptr ) {
				addVertexPtr(current->copy());
				current = current->next();
      }
    }
  }

  // Destroy this face
  void DLFLFace::destroy(void)
  {
    if ( head ) {
      DLFLFaceVertexPtr current = head;
      DLFLFaceVertexPtr temp;
      temp = current;
      current = current->next();
      delete temp;
      while ( current != head ) {
				temp = current;
				current = current->next();
				delete temp;
      }
    }
    head = NULL;
  }

  // Assignment operator
  DLFLFace& DLFLFace::operator = (const DLFLFace& face) {
    destroy();
    copy(face.head);
    uID = face.uID;
    ftType = face.ftType;
    auxcoords = face.auxcoords;
    auxnormal = face.auxnormal;
    centroid = face.centroid;
    normal = face.normal;
    flags = face.flags;
    if ( matl_ptr != face.matl_ptr ) {
      // Remove this face from the face-list of the material
      // if there is an associated material
      if ( matl_ptr ) matl_ptr->deleteFace(this);

      matl_ptr = face.matl_ptr;

      // Add this face to the face-list of the material
      if ( matl_ptr ) matl_ptr->addFace(this);
    }
    return (*this);
  }

  DLFLFaceVertexPtr DLFLFace::addVertex(const DLFLFaceVertex& dfv) {
    // Insert a copy.
    DLFLFaceVertexPtr newfvp = dfv.copy();
    addVertexPtr(newfvp);
    return newfvp;
  }

  DLFLFaceVertexPtr DLFLFace::addVertex(DLFLFaceVertexPtr dfvp) {
    // Insert a copy.
    DLFLFaceVertexPtr newfvp = dfvp->copy();
    addVertexPtr(newfvp);
    return newfvp;
  }

  void DLFLFace::addVertexPtr(DLFLFaceVertexPtr dfvp) {
    // Just insert the pointer.
    // **** WARNING!!! **** The pointer will be freed when the list is deleted!
    // Set face pointer for the face-vertex
    dfvp->setFacePtr(this);
    // Adjust the previous and next fields for this and adjacent face-vertices
    if ( head ) {
      DLFLFaceVertexPtr last = head->prev();
      dfvp->next() = head; head->prev() = dfvp;
      dfvp->prev() = last; last->next() = dfvp;
    } else {
      // face is empty
      // Make the new face-vertex the head
      head = dfvp;
    }
  }

  void DLFLFace::deleteVertexPtr(DLFLFaceVertexPtr dfvp) {
    if ( dfvp->getFacePtr() != this ) return;

    // Adjust pointers of adjacent face-vertices
    DLFLFaceVertexPtr n = dfvp->next();
    DLFLFaceVertexPtr p = dfvp->prev();

    if ( n == p && n == dfvp ) {  // Lone vertex in this face
      // NOTE: If n == p, it does not mean that this is the only vertex
      // n == p will be true even when there are 2 vertices only in the face
      head = NULL;
      return;
    }
  
    n->prev() = p; p->next() = n;

    if ( head == dfvp ) head = n;

    // Reset dfvp's pointers to point to itself
    dfvp->next() = dfvp; dfvp->prev() = dfvp; dfvp->setFacePtr(NULL);
  }

       
  void DLFLFace::getVertexCoords(Vector3dArray& verts) {
    // Get the coordinates of the vertices of this face in a Vector3dArray
    verts.clear(); verts.reserve(size());
    if ( head ) {
      DLFLFaceVertexPtr current = head; 
      verts.push_back(current->getVertexCoords());
      current = current->next();
      while ( current != head ) {
				verts.push_back(current->getVertexCoords());
				current = current->next();
      }
    }
  }

  // Set the material for this face. Update face-list of material also
  void DLFLFace::setMaterial(DLFLMaterialPtr mptr) {
    if ( mptr == matl_ptr ) return;

    // Remove face from old material
    if ( matl_ptr ) matl_ptr->deleteFace(this);

    // Change material
    matl_ptr = mptr;

    // Add face to new material
    if ( matl_ptr ) matl_ptr->addFace(this);
  }

  DLFLFaceVertexPtr DLFLFace::advance(DLFLFaceVertexPtr ptr, uint num) const {
    DLFLFaceVertexPtr newptr = ptr;
    for (uint i=0; i < num; ++i)
      newptr = newptr->next();
    return newptr;
  }

  // Access the face-vertex specified by index. No range checks done
  DLFLFaceVertexPtr DLFLFace::facevertexptr(uint index) {
    return advance(head,index);
  }

  DLFLFaceVertex& DLFLFace::facevertex(uint index) {
    return *advance(head,index);
  }
     
  DLFLFaceVertex DLFLFace::facevertex(uint index) const {
    return *advance(head,index);
  }

  // Access the vertex specified by index. No range checks done
  DLFLVertexPtr DLFLFace::vertexptr(uint index) {
    return facevertexptr(index)->vertex;
  }
     
  DLFLVertex& DLFLFace::vertex(uint index) {
    return *(facevertexptr(index)->vertex);
  }

  DLFLVertex DLFLFace::vertex(uint index) const {
    return *(facevertex(index).vertex);
  }

  // Access the coordinates of the vertex specified by index. No range checks done
  Vector3d& DLFLFace::vertex_coords(uint index) {
    return vertexptr(index)->coords;
  }

  Vector3d DLFLFace::vertex_coords(uint index) const {
    return vertex(index).coords;
  }

  // Access color and normal of face vertex specified by index
  RGBColor& DLFLFace::fv_color(uint index) {
    return facevertexptr(index)->color;
  }

  RGBColor DLFLFace::fv_color(uint index) const {
    return facevertex(index).color;
  }
     
  Vector3d& DLFLFace::fv_normal(uint index) {
    return facevertexptr(index)->normal;
  }

  Vector3d DLFLFace::fv_normal(uint index) const {
    return facevertex(index).normal;
  }

  // Set the color of all face vertices of this face
  void DLFLFace::setColor(const RGBColor& col) {
    if ( head ) {
      DLFLFaceVertexPtr current = head; 
      current->color = col;
      current = current->next();
      while ( current != head ) {
				current->color = col;
				current = current->next();
      }
    }
  }

  void DLFLFace::setColor(double r, double g, double b) {
    if ( head ) {
      DLFLFaceVertexPtr current = head; 
      current->color.color.set(r,g,b);
      current = current->next();
      while ( current != head ) {
				current->color.color.set(r,g,b);
				current = current->next();
      }
    }
  }

  void DLFLFace::randomAssignTexCoords(void) {
    // Assumes that the face is a quad
    //int start = lrand48() % 4; // Random no 0,1,2 or 3
    int start = rand() % 4; // Random no 0,1,2 or 3

    if ( head ) {
      DLFLFaceVertexPtr current = head; 
      current->texcoord = unittexcoords[start];
      ++start; start %= 4;
      current = current->next();
      while ( current != head ) {
				current->texcoord = unittexcoords[start];
				++start; start %= 4;
				current = current->next();
      }
    }
  }
  
  void DLFLFace::updateMaterial(void) {
    if ( matl_ptr ) matl_ptr->addFace(this);
  }

  Vector3d DLFLFace::geomSum(void) const {
    Vector3d sumg;
    if ( head ) {
      DLFLFaceVertexPtr current = head; 
      sumg = current->vertex->coords;
      current = current->next();
      while ( current != head ) {
				sumg += current->vertex->coords;
				current = current->next();
      }
    }
    return sumg;
  }

  Vector2d DLFLFace::textureSum(void) const {
    Vector2d sumtc;
    if ( head ) {
      DLFLFaceVertexPtr current = head; 
      sumtc = current->texcoord;
      current = current->next();
      while ( current != head ) {
				sumtc += current->texcoord;
				current = current->next();
      }
    }
    return sumtc;
  }

  RGBColor DLFLFace::colorSum(void) const {
    RGBColor sumc;
    if ( head ) {
      DLFLFaceVertexPtr current = head; 
      sumc = current->color;
      current = current->next();
      while ( current != head ) {
				sumc += current->color;
				current = current->next();
      }
    }
    return sumc;
  }

  Vector3d DLFLFace::normalSum(void) const {
    Vector3d sumn;
    if ( head ) {
      DLFLFaceVertexPtr current = head; 
      sumn = current->normal;
      current = current->next();
      while ( current != head ) {
				sumn += current->normal;
				current = current->next();
      }
    }
    return sumn;
  }

  void DLFLFace::getSums(Vector3d& sumg, Vector2d& sumtc, RGBColor& sumc, Vector3d& sumn) const
  {
    if ( head ) {
      DLFLFaceVertexPtr current = head; 
      sumg = current->vertex->coords;
      sumtc = current->texcoord;
      sumc = current->color;
      sumn = current->normal;
      current = current->next();
      while ( current != head ) {
				sumg += current->vertex->coords;
				sumtc += current->texcoord;
				sumc += current->color;
				sumn += current->normal;
				current = current->next();
      }
    }
  }

  void DLFLFace::updateCentroid(void) {
    centroid.reset();
    if ( head ) {
      int num = 0;
      DLFLFaceVertexPtr current = head; 
      centroid = current->vertex->coords; ++num;
      current = current->next();
      while ( current != head ) {
				centroid += current->vertex->coords; ++num;
				current = current->next();
      }
      centroid /= num;
    }
  }

  Vector2d DLFLFace::textureCentroid(void) const {
    Vector2d centc;
    if ( head ) {
      int num = 0;
      DLFLFaceVertexPtr current = head; 
      centc = current->texcoord; ++num;
      current = current->next();
      while ( current != head ) {
				centc += current->texcoord; ++num;
				current = current->next();
      }
      centc /= num;
    }
    return centc;
  }

  RGBColor DLFLFace::colorCentroid(void) const
  {
    RGBColor cenc;
    if ( head ) {
      int num = 0;
      DLFLFaceVertexPtr current = head; 
      cenc = current->color; ++num;
      current = current->next();
      while ( current != head ) {
				cenc += current->color; ++num;
				current = current->next();
      }
      cenc /= num;
    }
    return cenc;
  }

  void DLFLFace::updateNormal(void) {
    normal.reset();
    if ( head ) {
      int num = 0;
      DLFLFaceVertexPtr current = head;
      do {
				normal += current->computeNormal(); ++num;
				current = current->next();
      } while ( current != head );
      normalize(normal);
    }
  }

  Vector3d DLFLFace::computeNormal(void) {
    normal.reset();
    if ( head ) {
      int num = 0;
      DLFLFaceVertexPtr current = head;
      do {
				normal += current->computeNormal(); ++num;
				current = current->next();
      }
      while ( current != head );
      normal /= num; normalize(normal);

      current = head;
      do {
				current->setNormal(normal);
				current = current->next();
      } while ( current != head );
    }
    return normal;
  }

  void DLFLFace::storeNormals(void) {
    if ( head ) {
      DLFLFaceVertexPtr current = head;
      current->computeNormal();
      current = current->next();
      while ( current != head ) {
				current->computeNormal();
				current = current->next();
      }
    }
  }

  void DLFLFace::getCentroids(Vector3d& ceng, Vector2d& centc, RGBColor& cenc, Vector3d& cenn) const {
    if ( head ) {
      int num = 0;
      DLFLFaceVertexPtr current = head; 
      ceng = current->vertex->coords;
      centc = current->texcoord;
      cenc = current->color;
      cenn = current->normal;
      ++num;
      current = current->next();
      while ( current != head ) {
				ceng += current->vertex->coords;
				centc += current->texcoord;
				cenc += current->color;
				cenn += current->normal;
				++num;
				current = current->next();
      }
      ceng /= num;
      centc /= num;
      cenc /= num;
      cenn /= num;
    }
  }

  uint DLFLFace::size(void) const {
    int num=0;
    if ( head ) {
      DLFLFaceVertexPtr current = head; 
      ++num; current = current->next();
      while ( current != head ) {
				++num; current = current->next();
      }
    }
    return num;
  }

  void DLFLFace::resetTypeDeep(void) {
    resetType();
    if ( head ) {
      DLFLFaceVertexPtr current = head; 
      current->resetType();
      current = current->next();
      while ( current != head ) {
				current->resetType();
				current = current->next();
      }
    }
  }

  void DLFLFace::updateFacePointers(void) {
    if ( head ) {
      DLFLFaceVertexPtr current = head; 
      current->setFacePtr(this);
      current = current->next();
      while ( current != head ) {
				current->setFacePtr(this);
				current = current->next();
      }
    }
  }

  void DLFLFace::resetFacePointers(void) {
    if ( head ) {
      DLFLFaceVertexPtr current = head; 
      current->setFacePtr(NULL);
      current = current->next();
      while ( current != head ) {
				current->setFacePtr(NULL);
				current = current->next();
      }
    }
  }

  void DLFLFace::addFaceVerticesToVertices(void) {
    if ( head ) {
      DLFLFaceVertexPtr current = head; 
      current->addSelfToVertex();
      current = current->next();
      while ( current != head ) {
				current->addSelfToVertex();
				current = current->next();
      }
    }
  }

  void DLFLFace::deleteFaceVerticesFromVertices(void)
  {
    if ( head ) {
      DLFLFaceVertexPtr current = head; 
      current->deleteSelfFromVertex();
      current = current->next();
      while ( current != head ) {
				current->deleteSelfFromVertex();
				current = current->next();
      }
    }
  }

  void DLFLFace::addFaceVerticesToEdges(void) {
    if ( head ) {
      DLFLFaceVertexPtr current = head; 
      current->addSelfToEdge();
      current = current->next();
      while ( current != head ) {
				current->addSelfToEdge();
				current = current->next();
      }
    }
  }

  void DLFLFace::deleteFaceVerticesFromEdges(void) {
    if ( head ) {
      DLFLFaceVertexPtr current = head; 
      current->deleteSelfFromEdge();
      current = current->next();
      while ( current != head ) {
				current->deleteSelfFromEdge();
				current = current->next();
      }
    }
  }

  void DLFLFace::addFaceVerticesToVerticesAndEdges(void) {
    if ( head ) {
      DLFLFaceVertexPtr current = head; 
      current->addSelfToVertex();
      current->addSelfToEdge();
      current = current->next();
      while ( current != head ) {
				current->addSelfToVertex();
				current->addSelfToEdge();
				current = current->next();
      }
    }
  }

  void DLFLFace::deleteFaceVerticesFromVerticesAndEdges(void) {
    if ( head ) {
      DLFLFaceVertexPtr current = head; 
      current->deleteSelfFromVertex();
      current->deleteSelfFromEdge();
      current = current->next();
      while ( current != head ) {
				current->deleteSelfFromVertex();
				current->deleteSelfFromEdge();
				current = current->next();
      }
    }
  }

  int DLFLFace::getEdges(DLFLEdge ** edges) {
    int length = size();
    if ( length < 2 ) return 0;

    // Atleast 2 vertices are there in this DLFLFace (atleast 3 should be there, but
    // that doesn't affect this function)
    *edges = new DLFLEdge[length];
  
    if ( head ) {
      int index = 0;
      DLFLFaceVertexPtr current = head; 

      edges[0][length-1].setFaceVertexPtr2(current);  // Last Edge
      edges[0][index].setFaceVertexPtr1(current);     // First Edge
      current = current->next();
      while ( current != head ) {
				edges[0][index].setFaceVertexPtr2(current);
				index++;
				edges[0][index].setFaceVertexPtr1(current);
				current = current->next();
      }
    }
    return length;
  }

  // This function is different from the previous one in that it doesn't create any new edges
  // It simply gets the existing edges. Puts the edge pointers into an array
  void DLFLFace::getEdges(DLFLEdgePtrArray& edges) {
    edges.clear(); edges.reserve(size());
    if ( head ) {
      DLFLFaceVertexPtr current = head;

      edges.push_back(current->getEdgePtr());
      current = current->next();
      while ( current != head ) {
				edges.push_back(current->getEdgePtr());
				current = current->next();
      }
    }
  }

  // Same as above but puts edge pointers into a linked list
  void DLFLFace::getEdges(DLFLEdgePtrList& edges) {
    edges.clear();
    if ( head ) {
      DLFLFaceVertexPtr current = head;

      edges.push_back(current->getEdgePtr());
      current = current->next();
      while ( current != head ) {
				edges.push_back(current->getEdgePtr());
				current = current->next();
      }
    }
  }

  // Get the face vertex pointers in the face as an array
  void DLFLFace::getCorners(DLFLFaceVertexPtrArray& corners) {
    corners.clear(); corners.reserve(size());

    if ( head )
      {
				DLFLFaceVertexPtr current = head;

				corners.push_back(current);
				current = current->next();
				while ( current != head )
          {
            corners.push_back(current);
            current = current->next();
          }
      }
  }

	DLFLFaceVertexPtr DLFLFace::findFaceVertex( uint vertexId ) const {
		if(head) {
			DLFLFaceVertexPtr current = head;
			do {
				uint id = current->getVertexID();
				if( vertexId == id )
					return current;
				current = current->next();
			} while( current != head );
		}
	}

  // Get the corners and the coords
  void DLFLFace::getCornersAndCoords(DLFLFaceVertexPtrArray& corners, Vector3dArray& coords) {
    int n = size();
    corners.clear(); corners.reserve(n);
    coords.clear(); coords.reserve(n);

    if ( head ) {
      DLFLFaceVertexPtr current = head;

      corners.push_back(current);
      coords.push_back(current->getVertexCoords());
      current = current->next();
      while ( current != head ) {
				corners.push_back(current);
				coords.push_back(current->getVertexCoords());
				current = current->next();
      }
    }
  }

  // Does this face contain the given face-vertex?
  bool DLFLFace::contains(DLFLFaceVertexPtr fvptr) {
    if ( head ) {
      DLFLFaceVertexPtr current = head; 
      if ( current == fvptr ) return true;
      current = current->next();
      while ( current != head ) {
				if ( current == fvptr ) return true;
				current = current->next();
      }
    }
    return false;
  }

  bool DLFLFace::sharesOneVertex( DLFLFacePtr dfp ){
    if (head) {
      int numSharedVerts = 0;
      DLFLFaceVertexPtr current = head;
      DLFLFaceVertexPtrArray dfvparray;
      dfp->getCorners(dfvparray);
      vector<DLFLFaceVertexPtr>::iterator it;
      for ( it = dfvparray.begin(); it != dfvparray.end(); it++){
				current = head;
				if (current->getVertexPtr() == (*it)->getVertexPtr())
					numSharedVerts++;
				current = current->next();
				while (current != head){
					if (current->getVertexPtr() == (*it)->getVertexPtr())
						numSharedVerts++;
					current = current->next();
				}
				// cout << numSharedVerts << std::endl;
      }//end for it loop
      if (numSharedVerts == 1) return true;
      else return false;
    }
    return false;	
  }

  //store all neighboring faces in fparray, passed by reference
  void DLFLFace::getNeighboringFaces(DLFLFacePtrArray& fparray) {
    fparray.clear();
    DLFLFaceVertexPtr current = head;
    DLFLFacePtrArray fparr;
    vector<DLFLFacePtr>::iterator it;
    current->getVertexPtr()->getFaces(fparr);
    for ( it = fparr.begin(); it != fparr.end(); it++){
      if ((*it) != this) 
				fparray.push_back((*it));
    }	
    current = current->next();
    while (current != head){
      current->getVertexPtr()->getFaces(fparr);
      for ( it = fparr.begin(); it != fparr.end(); it++){
				if ((*it) != this) 
					fparray.push_back((*it));
      }
      current = current->next();
    }
  }

  // Find next DLFLFaceVertex - just return the next() for given face-vertex
  DLFLFaceVertexPtr DLFLFace::nextFaceVertex(DLFLFaceVertexPtr fvptr) {
    return fvptr->next();
  }

  // Find previous DLFLFaceVertex - just return the prev() for given face-vertex
  DLFLFaceVertexPtr DLFLFace::prevFaceVertex(DLFLFaceVertexPtr fvptr) {
    return fvptr->prev();
  }

  // Find the DLFLFaceVertex closest to the given point
  DLFLFaceVertexPtr DLFLFace::findClosest(const Vector3d& p) {
    DLFLFaceVertexPtr closest = NULL;

    if ( head ) {
      DLFLFaceVertexPtr current = head;
      double dist, mindist;
      mindist = dist = normsqr(p - current->vertex->coords);
      closest = current;
      current = current->next();
      while ( current != head ) {
				dist = normsqr(p - current->vertex->coords);
				if ( dist < mindist ) {
					mindist = dist; closest = current;
				}
				current = current->next();
      }
    }
    return closest;
  }

  // Find the two corners which are closest to each other in the two faces
  // The passed DLFLFaceVertex pointers will be set to the closest corners,
  // the first one from the first face and the second one from the second face
  void DLFLFace::findClosestCorners(DLFLFacePtr fp1, DLFLFacePtr fp2,
                                    DLFLFaceVertexPtr& fvp1, DLFLFaceVertexPtr& fvp2) {
    // Traverse face 1 and find closest point in face 2 for each point in face 1
    // using findClosest member function in DLFLFace
    fvp1 = NULL; fvp2 = NULL;

    if ( fp1->head ) {
      double dist, mindist;
      DLFLFaceVertexPtr cur1, cur2;
      fvp1 = cur1 = fp1->head;
      fvp2 = cur2 = fp2->findClosest(cur1->vertex->coords);
      mindist = dist = normsqr(cur1->vertex->coords - cur2->vertex->coords);
      cur1 = cur1->next();
      while ( cur1 != fp1->head ) {
				cur2 = fp2->findClosest(cur1->vertex->coords);
				dist = normsqr(cur1->vertex->coords - cur2->vertex->coords);
				if ( dist < mindist ) {
					mindist = dist; fvp1 = cur1; fvp2 = cur2; 
				}
				cur1 = cur1->next();
      }
    }
  }

  // Insert a new DLFLFaceVertex after a given DLFLFaceVertex
  DLFLFaceVertexPtr DLFLFace::insertAfter(DLFLFaceVertexPtr fvptr, DLFLFaceVertexPtr new_fvp, bool copy) {
    DLFLFaceVertexPtr fvp;

    if ( copy ) fvp = new_fvp->copy();
    else fvp = new_fvp;

    fvp->setFacePtr(this);
  
    fvp->next() = fvptr->next();
    fvp->next()->prev() = fvp;
    fvp->prev() = fvptr;
    fvptr->next() = fvp;

    return fvp;
  }

  // Insert a new DLFLFaceVertex before a given DLFLFaceVertex
  DLFLFaceVertexPtr DLFLFace::insertBefore(DLFLFaceVertexPtr fvptr, DLFLFaceVertexPtr new_fvp, bool copy) {
    // Find the previous face vertex to the given face vertex
    // and insert the new one after that
    DLFLFaceVertexPtr prev = fvptr->prev();
    return insertAfter(prev,new_fvp,copy);
  }

  void DLFLFace::reorder(DLFLFaceVertexPtr fvptr) {
    // Reorder the face-vertices so that the given face-vertex is the first one
    // Simply have to change the head pointer to point to the new face vertex
    // Check if fvptr belongs to this face. If yes, simply reset head to be fvptr
    // Otherwise don't change anything
    if ( this->contains(fvptr) ) head = fvptr;
  }

  void DLFLFace::reverse(void) {
    // Reverse the order of face-vertices in the face
    // For every face-vertex, swap the previous and next fields
    if ( head ) {
      DLFLFaceVertexPtr current = head; 
      current->reverse();
      current = current->next();
      while ( current != head ) {
				current->reverse();
				current = current->next();
      }
    }
  }

  vector<int> DLFLFace::vertexWalk( ) const {
    vector<int> verts;
    if ( head ) {
      DLFLFaceVertexPtr current = head;
      if ( current->getFacePtr() != this ) {
				cerr << "Data-structure consistency error!" << endl;
				cerr << "Vertex : " << current->getVertexID() << endl
						 << "Face : " << current->getFacePtr()->getID() << endl
						 << "Edge : " << current->getEdgePtr()->getID() << endl;
				return verts;
      }

      if ( current->getVertexPtr() != NULL )
				verts.push_back( current->getVertexID() );
      current = current->next();
      while ( current != head ) {
				if ( current->getVertexPtr() != NULL )
					verts.push_back( current->getVertexID() );
				current = current->next();
      }
    }
    return verts;
  }

  vector<int> DLFLFace::edgeWalk( ) const {
    vector<int> edges;
    if ( head ) {
      DLFLFaceVertexPtr current = head;
      if ( current->getFacePtr() != this ) {
				cerr << "Data-structure consistency error!" << endl;
				cerr << "Vertex : " << current->getVertexID() << endl
						 << "Face : " << current->getFacePtr()->getID() << endl
						 << "Edge : " << current->getEdgePtr()->getID() << endl;
				return edges;
      }

      if ( current->getEdgePtr() != NULL )
				edges.push_back( current->getEdgeID() );
      current = current->next();
      while ( current != head ) {
				if ( current->getEdgePtr() != NULL )
					edges.push_back( current->getEdgeID() );
				current = current->next();
      }
    }

    return edges;
  }

  // Boundary walk of DLFLFace
  void DLFLFace::boundaryWalk(void) const {
    cout << "DLFLFace " << uID << " (" << size() << ") : ";
    if ( head ) {
      DLFLFaceVertexPtr current = head;
      if ( current->getFacePtr() != this ) {
				cerr << "Data-structure consistency error!" << endl;
				cerr << "Vertex : " << current->getVertexID() << endl
						 << "Face : " << current->getFacePtr()->getID() << endl
						 << "Edge : " << current->getEdgePtr()->getID() << endl;
				return;
      }
      cout << current->getVertexID();
      cout << '[' << current->backface << ']';
      if ( current->getEdgePtr() != NULL )
				cout << "--(" << current->getEdgeID() << ")-->";   
      current = current->next();
      while ( current != head ) {
				cout << current->getVertexID();
				cout << '[' << current->backface << ']';
				if ( current->getEdgePtr() != NULL )
					cout << "--(" << current->getEdgeID() << ")-->";   
				current = current->next();
      }
      cout << current->getVertexID() << '[' << current->backface << ']' << endl;
    }
  }

  void DLFLFace::printPointers(void) const {
    cout << "DLFLFace " << uID << " (" << size() << ") : ";
    if ( head ) {
      DLFLFaceVertexPtr current = head; 
      cout << current;
      cout << "--(" << current->getEdgePtr() << ")-->";
      current = current->next();
      while ( current != head ) {
				cout << current;
				cout << "--(" << current->getEdgePtr() << ")-->";
				current = current->next();
      }
      cout << current << endl;
    }
  }

  // Write out DLFLFace in OBJ format
  void DLFLFace::objWrite(ostream& o, uint min_id) const {
    uint index;
    if ( head ) {
      o << 'f';
      DLFLFaceVertexPtr current = head; 
      index = current->getVertexID() - min_id;
      o << ' ' << index;
      current = current->next();
      while ( current != head ) {
				index = current->getVertexID() - min_id;
				o << ' ' << index;
				current = current->next();
      }
      o << endl;
    }
  }

  void DLFLFace::objWriteWithNormals(ostream& o, uint min_id, uint& normal_id_start) const {
    uint index;
    if ( head ) {
      o << 'f';
      DLFLFaceVertexPtr current = head; 
      index = current->getVertexID() - min_id;
      o << ' ' << index << "//" << normal_id_start;
      ++normal_id_start;
      current = current->next();
      while ( current != head ) {
				index = current->getVertexID() - min_id;
				o << ' ' << index << "//" << normal_id_start;
				++normal_id_start;
				current = current->next();
      }
      o << endl;
    }
  }

  void DLFLFace::objWriteWithTexCoords(ostream& o, uint min_id, uint& tex_id_start) const {
    uint index;
    if ( head ) {
      o << 'f';
      DLFLFaceVertexPtr current = head; 
      index = current->getVertexID() - min_id;
      o << ' ' << index << '/' << tex_id_start;
      ++tex_id_start;
      current = current->next();
      while ( current != head ) {
				index = current->getVertexID() - min_id;
				o << ' ' << index << '/' << tex_id_start;
				++tex_id_start;
				current = current->next();
      }
      o << endl;
    }
  }

  void DLFLFace::objWriteWithNormalsAndTexCoords(ostream& o, uint min_id, uint& normal_id_start, uint& tex_id_start) const {
    uint index;
    if ( head ) {
      o << 'f';
      DLFLFaceVertexPtr current = head; 
      index = current->getVertexID() - min_id;
      o << ' ' << index << '/' << tex_id_start << '/' << normal_id_start;
      ++tex_id_start; ++normal_id_start;
      current = current->next();
      while ( current != head ) {
				index = current->getVertexID() - min_id;
				o << ' ' << index << '/' << tex_id_start << '/' << normal_id_start;
				++tex_id_start; ++normal_id_start;
				current = current->next();
      }
      o << endl;
    }
  }

  // Write out normals at each vertex in OBJ format
  void DLFLFace::objWriteNormals(ostream& o) const {
    if ( head ) {
      Vector3d n;
      DLFLFaceVertexPtr current = head; 
      n = current->getNormal();
      o << "vn " << n[0] << ' ' << n[1] << ' ' << n[2] << endl;
      current = current->next();
      while ( current != head ) {
				n = current->getNormal();
				o << "vn " << n[0] << ' ' << n[1] << ' ' << n[2] << endl;
				current = current->next();
      }
    }
  }

  // Write out texture coordinates at each vertex in OBJ format
  void DLFLFace::objWriteTexCoords(ostream& o) const {
    if ( head ) {
      Vector2d t;
      DLFLFaceVertexPtr current = head; 
      t = current->getTexCoords();
      o << "vt " << t[0] << ' ' << t[1] << endl;
      current = current->next();
      while ( current != head ) {
				t = current->getTexCoords();
				o << "vt " << t[0] << ' ' << t[1] << endl;
				current = current->next();
      }
    }
  }

  void DLFLFace::writeDLFL(ostream& o) const {
    if ( head ) {
      DLFLFaceVertexPtr current = head; 
      o << 'f';
      //o << " {" << getID() << "}"; 
      o << ' ' << current->getIndex();
      current = current->next();
      while ( current != head ) {
				o << ' ' << current->getIndex();
				current = current->next();
      }
      o << endl;
    }
  }

  void DLFLFace::writeDLFLReverse(ostream& o) const {
    if ( head ) {
      DLFLFaceVertexPtr current = head; 
      o << 'f';
      //o << " {" << getID() << "}";   
      o << ' ' << current->getIndex();
      current = current->prev();
      while ( current != head ) {
				o << ' ' << current->getIndex();
				current = current->prev();
      }
      o << endl;
    }
  }

  void DLFLFace::for_each(void (*func)(DLFLFaceVertexPtr)) const {
    // Apply given function to every face-vertex in the face starting at head
    if ( head ) {
      DLFLFaceVertexPtr current = head; 
      func(current);
      current = current->next();
      while ( current != head ) {
				func(current);
				current = current->next();
      }
    }
  }

  ostream& operator << (ostream& o, const DLFLFace& face) {
    o << "Face (" << face.size() << ") : ";
    if ( face.head ) {
      DLFLFaceVertexPtr current = face.head; 
      o << current->vertex->coords;
      current = current->next();
      while ( current != face.head ) {
				o << ", " << current->vertex->coords;
				current = current->next();
      }
    }
    return o;
  }

} // end namespace
