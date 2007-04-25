/* $Id: DLFLFaceVertex.hh,v 4.0 2003/12/26 01:58:52 vinod Exp $ */

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

class DLFLFaceVertex
{
  public :

     DLFLVertexPtr      vertex;                        // Associated vertex pointer
     Vector3d           normal;                        // Normal
     RGBColor           color;                         // Color 
     Vector2d           texcoord;                      // Texture coordinate
     bool               backface;                      // Flag indicating this is part of a "back" face
                                                       // Used in reading OBJ files
                                                       // Default value is 'false'

  protected :

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
     TMPatchPtr         tmpp;                          // Pointer to the TMPatch corresponding to this corner

  public :

        // Default constructor
     DLFLFaceVertex(bool bf=false)
       : vertex(NULL), normal(), color(1), texcoord(), backface(bf), index(0),
         epEPtr(NULL), fpFPtr(NULL),
         fvpNext(NULL), fvpPrev(NULL),
         fvtType(FVTNormal), auxcoords(), auxnormal(), tmpp(NULL)
       {
         fvpNext = this; fvpPrev = this;
       }

        // 2 arg-constructor - copy the pointers
     DLFLFaceVertex(DLFLVertexPtr vptr, DLFLEdgePtr eptr, bool bf=false)
       : vertex(vptr), normal(), color(1), texcoord(), backface(bf), index(0),
         epEPtr(eptr), fpFPtr(NULL),
         fvpNext(NULL), fvpPrev(NULL),
         fvtType(FVTNormal), auxcoords(), auxnormal(), tmpp(NULL)
       {
         fvpNext = this; fvpPrev = this;
       }

        // Copy constructor
     DLFLFaceVertex(const DLFLFaceVertex& dfv)
       : vertex(dfv.vertex), normal(dfv.normal), color(dfv.color), texcoord(dfv.texcoord),
         backface(false), index(dfv.index),
         epEPtr(dfv.epEPtr), fpFPtr(dfv.fpFPtr), 
         fvpNext(NULL), fvpPrev(NULL), fvtType(dfv.fvtType), auxcoords(dfv.auxcoords), auxnormal(dfv.auxnormal), tmpp(dfv.tmpp)
       {
         fvpNext = this; fvpPrev = this;
       }

        // Destructor
     ~DLFLFaceVertex()
       {}

        // Assignment operator
     DLFLFaceVertex& operator = (const DLFLFaceVertex& dfv)
       {
         vertex = dfv.vertex; normal = dfv.normal; color = dfv.color; texcoord = dfv.texcoord;
         backface = dfv.backface; index = dfv.index;
         epEPtr = dfv.epEPtr; fpFPtr = dfv.fpFPtr;
         fvpNext = dfv.fvpNext; fvpPrev = dfv.fvpPrev;
         fvtType = dfv.fvtType; auxcoords = dfv.auxcoords; auxnormal = dfv.auxnormal; tmpp = dfv.tmpp;
         return (*this);
       }

     DLFLFaceVertexPtr copy(void) const
       {
         DLFLFaceVertexPtr fvptr = new DLFLFaceVertex(*this);
         return fvptr;
       }

        // Dump contents of this object
     void dump(ostream& o) const;

        // Average 2 facevertexes
        // Average the coordinates, normals, color and texture coordinates
        // Assumes that all 3 have valid vertex pointers
     friend void average(const DLFLFaceVertex& dfv1, const DLFLFaceVertex& dfv2, DLFLFaceVertex& ave);
     friend void average(DLFLFaceVertexPtr dfvp1, DLFLFaceVertexPtr dfvp2, DLFLFaceVertexPtr avep);

        // Query Functions
     uint getIndex(void) const
       {
         return index;
       }

     DLFLFaceVertexType getType(void) const
       {
         return fvtType;
       }

     DLFLVertexType getVertexType(void) const;

     DLFLVertexPtr getVertexPtr(void) const
       {
         return vertex;
       }

     DLFLEdgePtr getEdgePtr(void) const
       {
         return epEPtr;
       }

     DLFLFaceVertexPtr getOppositeCorner(void);
     
     DLFLFacePtr getFacePtr(void) const
       {
         return fpFPtr;
       }

     uint getVertexID(void) const;
     uint getFaceID(void) const;
     uint getEdgeID(void) const;

     Vector3d getNormal(void) const
       {
         return normal;
       }

     Vector3d getVertexCoords(void) const;

     Vector2d getTexCoords(void) const
       {
         return texcoord;
       }

     Vector3d getAuxCoords(void) const
       {
         return auxcoords;
       }

     Vector3d getAuxNormal(void) const
       {
         return auxnormal;
       }

     void getDS2Coords(Vector3d& dsc0, Vector3d& dsc1, Vector3d& dsc2, Vector3d& dsc3) const
       { 
         dsc0 = ds2coords[0]; dsc1 = ds2coords[1]; dsc2 = ds2coords[2]; dsc3 = ds2coords[3];
       }

     Vector3d getDS2Coord(uint index) const
       {
            // Assumes index is within range (0 to 3)
         return ds2coords[index];
       }

     TMPatchPtr getPatchPtr(void) const
       {
         return tmpp;
       }

     friend bool coFacial(DLFLFaceVertexPtr fvptr1, DLFLFaceVertexPtr fvptr2);
     friend bool coFacial(const DLFLFaceVertex& fv1, const DLFLFaceVertex& fv2);

     friend bool areEqual(DLFLFaceVertexPtr fvptr1, DLFLFaceVertexPtr fvptr2);
     friend bool areEqual(const DLFLFaceVertex& fv1, const DLFLFaceVertex& fv2);
     
        //--- Mutative Functions ---//

     void setType(DLFLFaceVertexType type)
       {
         fvtType = type;
       }

     void resetType(void)
       {
         fvtType = FVTNormal;
       }
     
        // Reset to original state
     void reset(void)
       {
         vertex = NULL; normal.reset(); color = 1.0; texcoord.reset();
         epEPtr = NULL; fpFPtr = NULL;
         fvpNext = this; fvpPrev = this;
         fvtType = FVTNormal;
       }

     void setVertexPtr(DLFLVertexPtr vptr)
       {
         vertex = vptr;
       }

     void setEdgePtr(DLFLEdgePtr eptr)
       {
         epEPtr = eptr;
       }

     void setFacePtr(DLFLFacePtr fptr)
       {
         fpFPtr = fptr;
       }
     
     void setNormal(const Vector3d& n)
       {
         normal = normalized(n);
       }

     void setPatchPtr(TMPatchPtr p)
       {
         tmpp = p;
       }

        // Check if this corner is a concave corner or not
        // Will not work properly for non-planar faces.
        // Will not work properly for faces which have edges which belong entirely to that face.
        // Will not work properly for highly skewed geometry .
     bool isConcaveCorner(void) const;

        // Check if this corner is a winged corner or not
        // A winged corner is a degenerate corner
        // i.e. the angle at the corner is 180 degrees
     bool isWingedCorner(void) const;

        // Find the next non-winged corner starting from this corner
        // If none can be found, returns NULL
     DLFLFaceVertexPtr nextNonWingedCorner(void) const;

        // Find the CLOSEST non-winged corner starting from this corner
        // If none can be found, returns NULL
        // Search both forward and backward till we find one.
     DLFLFaceVertexPtr closestNonWingedCorner(void) const;

        // Get a vector along the edge starting at this corner
     Vector3d getEdgeVector(void) const;

        // Get 2 vectors along the 2 edges which form this corner
        // Both vectors will originate at this corner
     void getEdgeVectors(Vector3d& pvec, Vector3d& nvec);

        // Compute the normal using adjacent vertices in this face
        // Normal will be adjusted to account for concave corners
     void updateNormal(void);
     Vector3d computeNormal(void)
     {
	  // Deprecated
       updateNormal();
       return normal;
     }

     void setVertexCoords(const Vector3d& vec);
     void setVertexCoords(double x, double y, double z);

     void setTexCoords(const Vector2d& tc)
       {
         texcoord = tc;
       }

     void setAuxCoords(const Vector3d& p)
       {
         auxcoords = p;
       }

     void setAuxNormal(const Vector3d& n)
       {
         auxnormal = n;
       }

     void setDS2Coords(const Vector3d& dsc0, const Vector3d& dsc1, const Vector3d& dsc2, const Vector3d& dsc3)
       { 
         ds2coords[0] = dsc0; ds2coords[1] = dsc1; ds2coords[2] = dsc2; ds2coords[3] = dsc3;
       }

     void setDS2Coord0(const Vector3d& dsc0)
       {
         ds2coords[0] = dsc0;
       }

     void setDS2Coord1(const Vector3d& dsc1)
       {
         ds2coords[1] = dsc1;
       }

     void setDS2Coord2(const Vector3d& dsc2)
       {
         ds2coords[2] = dsc2;
       }

     void setDS2Coord3(const Vector3d& dsc3)
       {
         ds2coords[3] = dsc3;
       }
     
        // Update the DLFLFaceVertexList of the DLFLVertex referred to by this DLFLFaceVertex
     void addSelfToVertex(void);
     void deleteSelfFromVertex(void);

        // Update the DLFLFaceVertexPtr of the DLFLEdge starting at this DLFLFaceVertex
     void addSelfToEdge(void);
     void deleteSelfFromEdge(void);

        // Remove the DLFLFaceVertexPtr from the DLFLFace to which it belongs
     void deleteSelfFromFace(void);

        // List related functions

  private:

        // This function by itself can leave the DLFL object in an invalid state
        // It should be used with extreme care, ideally as part of a face reversal only.
        // To prevent unintended misuse, this is made private and classes which need
        // access to this function are made friends

     void reverse(void)
       {
            // Swap the previous and next pointers
         DLFLFaceVertexPtr temp = fvpNext;
         fvpNext = fvpPrev; fvpPrev = temp;
       }

     friend class DLFLFace;

  public :

     void setNext(DLFLFaceVertexPtr fvptr)
       {
         fvpNext = fvptr;
       }

     void setPrev(DLFLFaceVertexPtr fvptr)
       {
         fvpPrev = fvptr;
       }

     DLFLFaceVertexPtr next(void) const
       {
         return fvpNext;
       }

     DLFLFaceVertexPtr& next(void)
       {
         return fvpNext;
       }

     DLFLFaceVertexPtr prev(void) const
       {
         return fvpPrev;
       }

     DLFLFaceVertexPtr& prev(void)
       {
         return fvpPrev;
       }

        // Next/prev facevertex around the vertex pointed to by this facevertex
        // Use the EdgePtr to do a 1-step vertex-trace
        // Returns this facevertex if EdgePtr is NULL
     DLFLFaceVertexPtr vnext(void);
     DLFLFaceVertexPtr vprev(void);

        // Make given DLFLFaceVertexPtr point to the next DLFLFaceVertexPtr
        // Used in list traversal
     friend void advance(DLFLFaceVertexPtr& fvp)
       {
         fvp = fvp->fvpNext;
       }

        // Make given DLFLFaceVertexPtr point to the previous DLFLFaceVertexPtr
        // Used in list traversal
     friend void backward(DLFLFaceVertexPtr& fvp)
       {
         fvp = fvp->fvpPrev;
       }

     void print(bool printadjacent = false) const;

     void printPointers(void) const
       {
         cout << "DLFLFaceVertex : " << vertex << ", " << epEPtr << ", " << fpFPtr << ";"
              << "< " << fvpPrev << ", " << fvpNext << " >" << endl;
       }

        // Write this face vertex in DLFL format and set it's index value
     void writeDLFL(ostream& o, uint newindex);

        // Read normal, texcoord and color info for this face vertex from an input stream
        // in DLFL format. Returns the vertex index
     uint readDLFL(istream& i);

        // "Render" this vertex. Simple calls the render() function for the vertex pointer
        // This is used for selection
     void render(void) const;

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
     friend void glVertexOFVTp(DLFLFaceVertexPtr dfvp);
};

#endif /* #ifndef _DLFL_FACE_VERTEX_HH_ */

/*
  $Log: DLFLFaceVertex.hh,v $
  Revision 4.0  2003/12/26 01:58:52  vinod
  Major version sync.

  Revision 3.5  2003/12/08 22:36:41  vinod
  Removed class name qualifier for closestNonWingedCorner method. IRIX complains. g++ doesn't

  Revision 3.4  2003/12/06 08:03:54  vinod
  Added closestNonWingedCorner method. computeNormal now checks for winged corners

  Revision 3.3  2003/11/21 15:25:39  vinod
  Added isWingedCorner and nextNonWingedCorner methods

  Revision 3.2  2003/11/16 06:45:28  vinod
  Added getEdgeVector and getEdgeVectors methods

  Revision 3.1  2003/11/14 02:40:06  vinod
  Added isConcaveCorner method

  Revision 3.0  2003/09/19 16:06:47  vinod
  Major version update

  Revision 2.7  2003/04/30 18:04:39  vinod
  Made reverse method private and added comment

  Revision 2.6  2003/03/10 07:33:52  vinod
  Added deleteSelfFromFace for consistency

  Revision 2.5  2003/01/30 07:02:24  vinod
  *** empty log message ***

  Revision 2.4  2002/04/28 22:13:32  vinod
  Changed copy constructor to set backface flag to false and not copy it

  Revision 2.3  2002/04/23 21:34:09  vinod
  Added backface flag

  Revision 2.2  2001/09/28 05:49:57  vinod
  Added print method

  Revision 2.1  2001/08/07 07:16:26  vinod
  Added vnext and vprev methods

  Revision 2.0  2001/07/25 05:12:44  vinod
  Major version sync

  Revision 1.7  2001/07/16 04:32:40  vinod
  Added advance, changed copy constructor

  Revision 1.6  2001/07/12 05:09:19  vinod
  Added reverse method

  Revision 1.5  2001/07/11 06:07:39  vinod
  Changed copy constructor to copy previous and next pointers also

  Revision 1.4  2001/04/08 21:17:54  vinod
  Added writeDLFL, auxcoords, getVertexType

  Revision 1.3  2000/11/28 19:14:00  vinod
  Added computeNormal

  Revision 1.2  2000/11/21 19:19:49  vinod
  Added dump() method

  Revision 1.1.1.1  2000/11/02 19:32:43  vinod
  New DLFL Sources

*/
