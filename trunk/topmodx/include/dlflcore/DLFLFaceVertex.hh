/**
 * \file DLFLFaceVertex.hh
 */

#ifndef _DLFL_FACE_VERTEX_HH_
#define _DLFL_FACE_VERTEX_HH_

// Class for a face vertex (corner) for the DLFLObject class
// Based on OBJFaceVertex class

// Contains a pointer to a Vertex and a pointer to the Edge which *starts* at this Face Vertex
// NOTE : The same physical point may be the start of many edges, but for a given face
// only 1 Edge can start at a physical point
// 'start' implies existence of a rotation order for the face
// Each FaceVertex also contains a pointer to the Face to which it belongs

// The FaceVertex class can now also be used directly as a node in a list
// It contains pointers to the next and previous nodes of the list

#include "DLFLCommon.hh"
#include "DLFLCoreExt.hh"
#include "DLFLEdge.hh"
#include "DLFLVertex.hh"

namespace DLFL {

  class DLFLFaceVertex {
  public:
    static void setLastID( uint id ) {
      if( id > suLastID )
	suLastID = id;
    };
  protected:
    static uint suLastID;

    static uint newID( ) {
      uint temp = suLastID;
      suLastID++;
      return temp;
    };

  public :
    DLFLVertexPtr      vertex;                        // Associated vertex pointer
    Vector3d           normal;                        // Normal
    RGBColor           color;                         // Color 
    Vector2d           texcoord;                      // Texture coordinate
    bool               backface;                      // Flag indicating this is part of a "back" face
                                                      // Used in reading OBJ files
                                                      // Default value is 'false'
  protected :
    uint               uID;                            // Id for face vertex
    uint               index;                         // Index for use in file output
    DLFLEdgePtr        epEPtr;                        // Pointer to the Edge
    DLFLFacePtr        fpFPtr;                        // Pointer to the Face
     
    // List node data
    DLFLFaceVertexPtr  fvpNext;                       // Pointer to next FaceVertex
    DLFLFaceVertexPtr  fvpPrev;                       // Pointer to previous FaceVertex

    DLFLFaceVertexType fvtType;                       // For use in subdivision surfaces
    Vector3d           auxcoords;                     // Coords for use during subdivs, extrude, etc.
    Vector3d           auxnormal;                     // Additional storage for normal
    Vector3d           ds2coords[4];                  // Level-2 Doo Sabin coordinates
    //TMPatchPtr         tmpp;                          // Pointer to the TMPatch corresponding to this corner

    void assignID( ) {
      uID = DLFLFaceVertex::newID();
    };

  public :
    // Default constructor
    DLFLFaceVertex(bool bf=false);
    // 2 arg-constructor - copy the pointers
    DLFLFaceVertex(DLFLVertexPtr vptr, DLFLEdgePtr eptr, bool bf=false);
    // Copy constructor
    DLFLFaceVertex(const DLFLFaceVertex& dfv);
    // Destructor
    ~DLFLFaceVertex();

    // Assignment operator
    DLFLFaceVertex& operator=(const DLFLFaceVertex& dfv);

    DLFLFaceVertexPtr copy( ) const;

    // Dump contents of this object
    void dump(ostream& o) const;

    // Average 2 facevertexes
    // Average the coordinates, normals, color and texture coordinates
    // Assumes that all 3 have valid vertex pointers
    friend void average(const DLFLFaceVertex& dfv1, const DLFLFaceVertex& dfv2, DLFLFaceVertex& ave);
    friend void average(DLFLFaceVertexPtr dfvp1, DLFLFaceVertexPtr dfvp2, DLFLFaceVertexPtr avep);

    // Query Functions
    uint getIndex( ) const { return index; };
    uint getID( ) const { return uID; };
    DLFLFaceVertexType getType( ) const { return fvtType; };
    DLFLVertexType getVertexType( ) const { return vertex->getType(); };
    DLFLVertexPtr getVertexPtr( ) const { return vertex; };
    DLFLEdgePtr getEdgePtr( ) const { return epEPtr; };
    DLFLFaceVertexPtr getOppositeCorner( ) { return epEPtr->getOtherFaceVertexPtr(this); };
    DLFLFacePtr getFacePtr( ) const { return fpFPtr; };
    uint getVertexID( ) const { return vertex->getID(); };
    uint getFaceID( ) const;
    uint getEdgeID( ) const { return epEPtr->getID(); };
    Vector3d getNormal( ) const { return normal; };
    Vector3d getVertexCoords( ) const { return vertex->coords; };
    Vector2d getTexCoords( ) const { return texcoord; };
    Vector3d getAuxCoords( ) const { return auxcoords; };
    Vector3d getAuxNormal( ) const { return auxnormal; };
    void getDS2Coords(Vector3d& dsc0, Vector3d& dsc1, Vector3d& dsc2, Vector3d& dsc3) const { 
      dsc0 = ds2coords[0]; dsc1 = ds2coords[1]; dsc2 = ds2coords[2]; dsc3 = ds2coords[3];
    };
    Vector3d getDS2Coord(uint index) const {
      // Assumes index is within range (0 to 3)
      return ds2coords[index];
    };
     
    //--- Mutative Functions ---//

    void setType(DLFLFaceVertexType type) { fvtType = type; };
    void resetType( ) { fvtType = FVTNormal; };
     
    // Reset to original state
    void reset( ) {
      vertex = NULL; normal.reset(); //color = 1.0; texcoord.reset();
      epEPtr = NULL; fpFPtr = NULL;
      fvpNext = this; fvpPrev = this;
      fvtType = FVTNormal;
    };

    void setVertexPtr(DLFLVertexPtr vptr) { vertex = vptr; };
    void setEdgePtr(DLFLEdgePtr eptr) { epEPtr = eptr; };
    void setFacePtr(DLFLFacePtr fptr) { fpFPtr = fptr; };
    void setNormal(const Vector3d& n) { normal = normalized(n); };

    /* Check if this corner is a concave corner or not
     * Will not work properly for non-planar faces.
     * Will not work properly for faces which have edges which belong entirely to that face.
     * Will not work properly for highly skewed geometry */
    bool isConcaveCorner( ) const;

    /* Check if this corner is a winged corner or not
     * A winged corner is a degenerate corner 
     * i.e. the angle at the corner is 180 degrees */
    bool isWingedCorner( ) const;

    /* Find the next non-winged corner starting from this corner
     * If none can be found, returns NULL */
    DLFLFaceVertexPtr nextNonWingedCorner( ) const;

    /* Find the CLOSEST non-winged corner starting from this corner
     * If none can be found, returns NULL
     * Search both forward and backward till we find one. */
    DLFLFaceVertexPtr closestNonWingedCorner( ) const;

    // Get a vector along the edge starting at this corner
    Vector3d getEdgeVector( ) const;

    /* Get 2 vectors along the 2 edges which form this corner
     * Both vectors will originate at this corner */
    void getEdgeVectors(Vector3d& pvec, Vector3d& nvec);

    /* Compute the normal using adjacent vertices in this face
     * Normal will be adjusted to account for concave corners */
    void updateNormal( );
    
    Vector3d computeNormal( ) {
      updateNormal();
      return normal;
    };

    void setVertexCoords(const Vector3d& vec) {
      if ( vertex ) vertex->coords = vec;
    };
    void setVertexCoords(double x, double y, double z) {
      if ( vertex ) vertex->coords.set(x,y,z);
    };

    void setTexCoords(const Vector2d& tc) { texcoord = tc; };
    void setAuxCoords(const Vector3d& p) { auxcoords = p; };
    void setAuxNormal(const Vector3d& n) { auxnormal = n; };

    void setDS2Coords(const Vector3d& dsc0, const Vector3d& dsc1, const Vector3d& dsc2, const Vector3d& dsc3) { 
      ds2coords[0] = dsc0; ds2coords[1] = dsc1; ds2coords[2] = dsc2; ds2coords[3] = dsc3;
    };

    void setDS2Coord0(const Vector3d& dsc0) { ds2coords[0] = dsc0; };
    void setDS2Coord1(const Vector3d& dsc1) { ds2coords[1] = dsc1; };
    void setDS2Coord2(const Vector3d& dsc2) { ds2coords[2] = dsc2; };
    void setDS2Coord3(const Vector3d& dsc3) { ds2coords[3] = dsc3; };
     
    // Update the DLFLFaceVertexList of the DLFLVertex referred to by this DLFLFaceVertex
    void addSelfToVertex( );
    void deleteSelfFromVertex( );

    // Update the DLFLFaceVertexPtr of the DLFLEdge starting at this DLFLFaceVertex
    void addSelfToEdge( );
    void deleteSelfFromEdge( );

    // Remove the DLFLFaceVertexPtr from the DLFLFace to which it belongs
    void deleteSelfFromFace( );

    // List related functions

  private:

    // This function by itself can leave the DLFL object in an invalid state
    // It should be used with extreme care, ideally as part of a face reversal only.
    // To prevent unintended misuse, this is made private and classes which need
    // access to this function are made friends

    void reverse( ) {
      // Swap the previous and next pointers
      DLFLFaceVertexPtr temp = fvpNext;
      fvpNext = fvpPrev; fvpPrev = temp;
    };

    friend class DLFLFace;

  public :

    void setNext(DLFLFaceVertexPtr fvptr) { fvpNext = fvptr; };
    void setPrev(DLFLFaceVertexPtr fvptr) { fvpPrev = fvptr; };

    DLFLFaceVertexPtr next( ) const { return fvpNext; };
    DLFLFaceVertexPtr& next( ) { return fvpNext; };

    DLFLFaceVertexPtr prev( ) const { return fvpPrev; };
    DLFLFaceVertexPtr& prev( ) { return fvpPrev; };

    /* Next/prev facevertex around the vertex pointed to by this facevertex
     * Use the EdgePtr to do a 1-step vertex-trace
     * Returns this facevertex if EdgePtr is NULL */
    DLFLFaceVertexPtr vnext( );
    DLFLFaceVertexPtr vprev( );

    /* Make given DLFLFaceVertexPtr point to the next DLFLFaceVertexPtr
     * Used in list traversal */
    friend void advance(DLFLFaceVertexPtr& fvp) {
      fvp = fvp->fvpNext;
    };

    /* Make given DLFLFaceVertexPtr point to the previous DLFLFaceVertexPtr
     * Used in list traversal */
    friend void backward(DLFLFaceVertexPtr& fvp) {
      fvp = fvp->fvpPrev;
    };

    void print(bool printadjacent = false) const;

    void printPointers( ) const {
      cout << "DLFLFaceVertex : " << vertex << ", " << epEPtr << ", " << fpFPtr << ";"
	   << "< " << fvpPrev << ", " << fvpNext << " >" << endl;
    };

    // Write this face vertex in DLFL format and set it's index value
    void writeDLFL(ostream& o, uint newindex);

    /* Read normal, texcoord and color info for this face vertex from an input stream
     * in DLFL format. Returns the vertex index */
    uint readDLFL(istream& i);

    /*
    // "Render" this vertex. Simple calls the render() function for the vertex pointer
    // This is used for selection
    void render( ) const;

    // Do a glVertex on this vertex, using the normal and with color
    friend void glVertexCN(const DLFLFaceVertex& dfv);

    // Do a glVertex on this vertex, using the normal and without color
    friend void glVertexN(const DLFLFaceVertex& dfv);
     
    // Do a glVertex on this vertex, without using the normal and with color
    friend void glVertexC(const DLFLFaceVertex& dfv);
     
    // Do a glVertex on this face vertex, without using the normal and without color
    // This function is not named glVertex to allow using it in the
    // STL for_each algorithm, since glVertex is overloaded for DLFLVertex also
    friend void glVertexOFV(const DLFLFaceVertex& dfv);

    //-- Same as above functions but with texture coordinates also --//

    friend void glVertexCNT(const DLFLFaceVertex& dfv);
    friend void glVertexNT(const DLFLFaceVertex& dfv);
    friend void glVertexCT(const DLFLFaceVertex& dfv);
    friend void glVertexOFVT(const DLFLFaceVertex& dfv);

    //-- Versions of above functions which take pointers are arguments --//
    friend void glVertexCNp(DLFLFaceVertexPtr dfvp);
    friend void glVertexNp(DLFLFaceVertexPtr dfvp);
    friend void glVertexCp(DLFLFaceVertexPtr dfvp);
    friend void glVertexOFVp(DLFLFaceVertexPtr dfvp);

    friend void glVertexCNTp(DLFLFaceVertexPtr dfvp);
    friend void glVertexNTp(DLFLFaceVertexPtr dfvp);
    friend void glVertexCTp(DLFLFaceVertexPtr dfvp);
    friend void glVertexOFVTp(DLFLFaceVertexPtr dfvp);*/
  }; // end class DLFLFaceVertex

  bool coFacial(DLFLFaceVertexPtr fvptr1, DLFLFaceVertexPtr fvptr2);
  bool coFacial(const DLFLFaceVertex& fv1, const DLFLFaceVertex& fv2);
  
  bool areEqual(DLFLFaceVertexPtr fvptr1, DLFLFaceVertexPtr fvptr2);
  bool areEqual(const DLFLFaceVertex& fv1, const DLFLFaceVertex& fv2);

} // end namespace

#endif /* _DLFL_FACE_VERTEX_HH_ */

