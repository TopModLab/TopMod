/* $Id: DLFLVertex.hh,v 4.2 2004/01/20 09:27:23 vinod Exp $ */

#ifndef _DLFL_VERTEX_HH_

#define _DLFL_VERTEX_HH_

// Class to store a vertex for the DLFLObject class
// Based on the OBJVertex class

#include "DLFLCommon.hh"

class DLFLVertex
{
  protected :

     static uint suLastID;                             // Distinct ID for each instance
                                                       // The last assigned ID is stored in this
                                                       // class variable

        // Generate a new unique ID
     static uint newID(void)
       {
         uint temp = suLastID;
         suLastID++;
         return temp;
       }
     
  public :

     Vector3d              coords;                     // Coordinates of vertex
     unsigned long         flags;                      // Variable for general use to store flags, etc.
     uint		   CHullIndex;		       // Index used for convex hull - Esan

  protected :

     uint                  uID;                        // ID for this DLFLVertex
     uint                  index;                      // Index for use in file output
     DLFLFaceVertexPtrList fvpList;                    // List of DLFLFaceVertexes which
                                                       // refer to this DLFLVertex
     DLFLVertexType        vtType;                     // For use in subdivision surfaces
     Vector3d              auxcoords;                  // Coords for use during subdivs, etc.

        // Assign a unique ID for this instance
     void assignID(void)
       {
         uID = DLFLVertex :: newID();
         index = 0;
       }

  public :
     
        // Default constructor
     DLFLVertex()
       : coords(), flags(0), fvpList(), vtType(VTNormal), auxcoords()
       {
         assignID();
       }

        // 1 argument constructor
     DLFLVertex(const Vector3d& vec)
       : coords(vec), flags(0), fvpList(), vtType(VTNormal), auxcoords()
       {
         assignID();
       }

        // 3 argument constructor
     DLFLVertex(double x, double y, double z)
       : coords(x,y,z), flags(0), fvpList(), vtType(VTNormal), auxcoords()
       {
         assignID();
       }

        // Copy constructor
     DLFLVertex(const DLFLVertex& dv)
       : coords(dv.coords), flags(dv.flags),
         uID(dv.uID), index(dv.index), fvpList(dv.fvpList), vtType(dv.vtType),
         auxcoords(dv.auxcoords)
       {}

        // Destructor
     ~DLFLVertex()
       {}

        // Assignment operator
     DLFLVertex& operator = (const DLFLVertex& dv)
       {
         coords = dv.coords; flags = dv.flags;
         uID = dv.uID; index = dv.index; fvpList = dv.fvpList; vtType = dv.vtType;
         auxcoords = dv.auxcoords;
         return (*this);
       }

     DLFLVertexPtr copy(void) const
       {
         DLFLVertexPtr newdv = new DLFLVertex(*this);
         return newdv;
       }

        // Dump contents of this object
     void dump(ostream& o) const;

     void reset(void)
       {
         coords.reset(); flags = 0; fvpList.clear(); vtType = VTNormal; auxcoords.reset();
       }

     void makeUnique(void)
       {
         assignID();
       }

     friend void makeVertexUnique(DLFLVertexPtr dvp)
       {
         dvp->assignID();
       }

        //--- Query functions ---//

     DLFLVertexType getType(void) const
       {
         return vtType;
       }

     Vector3d getCoords(void) const
       {
         return coords;
       }

     DLFLFaceVertexPtrList getFaceVertexList(void) const
       {
         return fvpList;
       }

        // Number of Edges incident on this Vertex = no. of Faces adjacent to this Vertex
        // = size of the FaceVertex list = valence of Vertex
     uint numEdges(void) const
       {
         return fvpList.size();
       }
     
     uint numFaces(void) const
       {
         return fvpList.size();
       }

     uint valence(void) const
       {
         return fvpList.size();
       }
     
     uint getID(void) const
       {
         return uID;
       }

     uint getIndex(void) const
       {
         return index;
       }

     Vector3d getAuxCoords(void) const
       {
         return auxcoords;
       }

        //--- Mutative functions ---//

     void setType(DLFLVertexType type)
       {
         vtType = type;
       }

     void resetType(void)
       {
         vtType = VTNormal;
       }

        // Reset type of vertex, all face-vertices and edges connected to this vertex
     void resetTypeDeep(void);

     friend void resetVertexType(DLFLVertexPtr dvp)
       {
         dvp->resetType();
       }

     void setFaceVertexList(const DLFLFaceVertexPtrList& list)
       {
         fvpList = list;
       }

     void setCoords(const Vector3d& p)
       {
         coords = p;
       }

        // Set the aux. coords
     void setAuxCoords(const Vector3d& p)
       {
         auxcoords = p;
       }

        // Add to the aux coords
     void addToAuxCoords(const Vector3d& p)
       {
         auxcoords += p;
       }

        // Reset the aux coords
     void resetAuxCoords(void)
       {
         auxcoords.reset();
       }
     
        // Set the texture coordinates for all FaceVertexes referring to this vertex
     void setTexCoords(const Vector2d& texcoord);

        // Set the color values for all FaceVertexes referring to this vertex
     void setColor(const RGBColor& color);

        // Set the normals for all FaceVertexes referring to this vertex
     void setNormal(const Vector3d& normal);

        // Compute normals for all FaceVertexes referring to this vertex, average them
        // and set their normals to be the average if flag is true. Returns the normal
     Vector3d computeNormal(bool set=false);

        // Compute the average of the normals of all facevertices referring to this vertex
        // Assumes that normals have already been computed for all facevertices
     Vector3d averageNormal(void);

        // Get normals of all corners referring to this vertex. Returns average normal
     Vector3d getNormals(Vector3dArray& normals);

        // Set tex coordinates, color and normal info for all FaceVertexes referring to this vertex
     void setFaceVertexProps(const Vector2d& texcoord, const RGBColor& color, const Vector3d& normal);

        // Update the DLFLFaceVertexList by adding a new DLFLFaceVertexPtr
     void addToFaceVertexList(DLFLFaceVertexPtr fvptr)
       {
         fvpList.push_back(fvptr);
       }

     void deleteFromFaceVertexList(DLFLFaceVertexPtr fvptr)
       {
         fvpList.remove(fvptr);
       }

        // DLFL Vertex Trace
     void vertexTrace(void) const;
     friend void vertexTrace(DLFLVertexPtr vertexptr)
       {
         vertexptr->vertexTrace();
       }

     friend void vertexTrace(const DLFLVertex& vertex)
       {
         vertex.vertexTrace();
       }

        // Create an array of Edge's incident on this Vertex. The number of Edges
        // is returned. Memory will be allocated inside this function, which should
        // be freed by the caller. Pass a pointer to the array (DLFLEdgePtr **)
        // Equivalent to a vertex trace
     int getEdges(DLFLEdgePtr ** edges) const;

        // Same as above but uses the STL Array
     void getEdges(DLFLEdgePtrArray& edges) const;

        // Get the Edge incident on this Vertex which connects to given Vertex
        // If no such edge exists, returns NULL
     DLFLEdgePtr getEdgeTo(DLFLVertexPtr vp);

        // Create an array of FaceVertexes which share this Vertex.
     void getFaceVertices(DLFLFaceVertexPtrArray& fvparray);

        // Create an array of Faces which share this Vertex
     void getFaces(DLFLFacePtrArray& fparray);

        // Get the FaceVertex belonging to the given face. If only 1 face-vertex
        // is there in the list, return that. If more than 1 exist but nothing
        // belongs to given face, return NULL
     DLFLFaceVertexPtr getFaceVertexInFace(DLFLFacePtr fp);

        // Get the FaceVertex which has the given Vertex before/after it in it's Face
        // If only 1 FaceVertex refers to this Vertex, will return that
        // If there are more than 1 and none of them satisfies the condition, returns NULL
     DLFLFaceVertexPtr getFaceVertexWithPrev(DLFLVertexPtr vp);
     DLFLFaceVertexPtr getFaceVertexWithNext(DLFLVertexPtr vp);

        // Get the FaceVertex which has the 'backface' flag set
        // If no such FaceVertex is found, returns NULL
     DLFLFaceVertexPtr getBackFaceVertex(void);

        // Apply a transformation specified by the matrix to the coordinates
     void transform(const Matrix4x4& tmat)
       {
         Vector4d tp(coords); tp[3] = 1.0;
         tp = tmat * tp;
         tp /= tp[3];
         coords = tp;
       }

        // Print out this Vertex
     void print(void) const
       {
         cout << "DLFLVertex " << uID << "," << vtType << "," << valence() << " : " << coords << endl;
       }

     void render(void) const
       {
         glVertex3dv(coords.getCArray());
       }

        // Write this vertex in DLFL format and set it's index value
     void writeDLFL(ostream& o, uint newindex)
       {
         double x,y,z; coords.get(x,y,z);
         o << "v " << x << ' ' << y << ' ' << z << endl;
         index = newindex;
       }

        // Read a vertex from an input stream.
        // The 3 coordinates should be specified separated by spaces (as in OBJ format)
     friend istream& operator >> (istream& i, DLFLVertex& dv)
       {
            // Read x,y,z coordinates.
         double x,y,z;
         i >> x >> y >> z;
         dv.coords.set(x,y,z);
         return i;
       }

        // Write a vertex to an output stream. Writes in OBJ format
     friend ostream& operator << (ostream& o, const DLFLVertex& dv)
       {
            // Only coordinates are written.
         double x,y,z;
         dv.coords.get(x,y,z);
         o << "v " << x << ' ' << y << ' ' << z << endl;
         return o;
       }

        // Do a glVertex on this point
     friend void glVertex(const DLFLVertex& dv)
       {
         dv.render();
       }

     friend void glVertexP(const DLFLVertexPtr& dvp)
       {
         dvp->render();
       }

};

#endif /* #ifndef _DLFL_VERTEX_HH_ */

/*
  $Log: DLFLVertex.hh,v $
  Revision 4.2  2004/01/20 09:27:23  vinod
  Added CHullIndex member variable for column modeling

  Revision 4.1  2004/01/13 01:57:33  vinod
  Added getFaces method

  Revision 4.0  2003/12/26 01:58:53  vinod
  Major version sync.

  Revision 3.4  2003/12/01 09:00:12  vinod
  Added transform method

  Revision 3.3  2003/11/29 19:12:25  vinod
  Added valence to output for print method

  Revision 3.2  2003/10/20 00:29:01  vinod
  print method now outputs vertex type

  Revision 3.1  2003/10/10 06:40:47  vinod
  Added getNormals method

  Revision 3.0  2003/09/19 16:06:47  vinod
  Major version update

  Revision 2.10  2003/09/19 16:04:24  vinod
  Added resetTypeDeep method

  Revision 2.9  2003/04/04 07:19:16  vinod
  Added 'flags' member variable for general use

  Revision 2.8  2003/03/10 07:34:42  vinod
  Added getCoords and setCoords methods

  Revision 2.7  2002/12/30 17:28:45  vinod
  Added STL array version of getEdges method

  Revision 2.6  2002/10/25 03:02:58  vinod
  Modified to use getCArray method for Vector classes
  instead of pointer cast operators which don't seem to have problems with GCC 3 and above

  Revision 2.5  2002/04/28 22:14:23  vinod
  Added getEdgeTo, getFaceVertexWithPrev/Next and getBackFaceVertex

  Revision 2.4  2001/11/21 00:17:38  vinod
  Added set flag to computeNormal, added averageNormal method

  Revision 2.3  2001/10/07 07:13:01  vinod
  Added addToAuxCoords, resetAuxCoords methods

  Revision 2.2  2001/10/06 16:40:51  vinod
  Added getFaceVertexInFace

  Revision 2.1  2001/09/28 17:45:14  vinod
  Added getFaceVertices method

  Revision 2.0  2001/07/25 05:12:46  vinod
  Major version sync

  Revision 1.4  2001/04/08 21:21:00  vinod
  Added writeDLFL, auxcoords

  Revision 1.3  2000/11/28 19:15:22  vinod
  Added computeNormal

  Revision 1.2  2000/11/21 19:19:15  vinod
  Added dump() method

  Revision 1.1.1.1  2000/11/02 19:32:43  vinod
  New DLFL Sources

*/
