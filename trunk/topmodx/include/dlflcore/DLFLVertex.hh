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
*/

/**
 * \file DLFLVertex.hh
 */

#ifndef _DLFL_VERTEX_HH_
#define _DLFL_VERTEX_HH_

// Class to store a vertex for the DLFLObject class
// Based on the OBJVertex class

#include "DLFLCommon.hh"

namespace DLFL {
  
  class DLFLVertex {
  public :
    static void setLastID( uint id ) {
      if( id > suLastID )
	suLastID = id;
    };

  protected :
    static uint suLastID;                             // Distinct ID for each instance
    // The last assigned ID is stored in this
    // class variable

    // Generate a new unique ID
    static uint newID(void) {
      uint temp = suLastID;
      suLastID++;
      return temp;
    };
     
  public :
    Vector3d             coords;                     // Coordinates of vertex
    uint ismarked;
    uint		   isvisited;		       // flag for traverse - ozgur
    unsigned long         flags;                      // Variable for general use to store flags, etc.
    uint		   CHullIndex;		       // Index used for convex hull - Esan

  protected :
    uint                  uID;                        // ID for this DLFLVertex
    uint                  index;                      // Index for use in file output
    DLFLFaceVertexPtrList fvpList;                    // List of DLFLFaceVertexes which
    // refer to this DLFLVertex
    DLFLVertexType        vtType;                     // For use in subdivision surfaces
    Vector3d              auxcoords;                  // Coords for use during subdivs, etc.
    Vector3d              auxnormal;                  // Extra storage for normal
    Vector3d              normal;                     // Average normal at this vertex

    // Assign a unique ID for this instance
    void assignID(void) {
      uID = DLFLVertex :: newID();
      index = 0;
      ismarked = 0;
		 isvisited = 0;

    };

  public :
    // Default constructor
    DLFLVertex() 
      : coords(), flags(0), fvpList(), vtType(VTNormal), auxcoords(), auxnormal(), normal()
    { assignID(); }
    
    // 1 argument constructor
    DLFLVertex(const Vector3d& vec)
      : coords(vec), flags(0), fvpList(), vtType(VTNormal), auxcoords(), auxnormal(), normal()
    { assignID(); }

    // 3 argument constructor
    DLFLVertex(double x, double y, double z)
      : coords(x,y,z), flags(0), fvpList(), vtType(VTNormal), auxcoords(), auxnormal(), normal()
    { assignID(); }
  
    // Copy constructor
    DLFLVertex(const DLFLVertex& dv)
      : coords(dv.coords), flags(dv.flags),
	uID(dv.uID), index(dv.index), fvpList(dv.fvpList), vtType(dv.vtType),
	auxcoords(dv.auxcoords), auxnormal(dv.auxnormal), normal(dv.normal)
    {}
  
    // Destructor
    ~DLFLVertex() {}

    // Assignment operator
    DLFLVertex& operator = (const DLFLVertex& dv) {
      coords = dv.coords; flags = dv.flags;
      uID = dv.uID; index = dv.index; fvpList = dv.fvpList; vtType = dv.vtType;
      auxcoords = dv.auxcoords; auxnormal = dv.auxnormal; normal = dv.normal;
      return (*this);
    }

    DLFLVertexPtr copy(void) const {
      DLFLVertexPtr newdv = new DLFLVertex(*this);
      return newdv;
    }

    // Dump contents of this object
    void dump( ostream& o ) const;

    void reset(void) {
      coords.reset(); flags = 0; fvpList.clear(); vtType = VTNormal;
      auxcoords.reset(); auxnormal.reset(); normal.reset();
    }

    void makeUnique(void) {
      assignID();
    }

    friend void makeVertexUnique(DLFLVertexPtr dvp) {
      dvp->assignID();
    }

    //--- Query functions ---//

    DLFLVertexType getType(void) const {
      return vtType;
    }

    Vector3d getCoords(void) const {
      return coords;
    }

    DLFLFaceVertexPtrList getFaceVertexList(void) const {
      return fvpList;
    }

    // Number of Edges incident on this Vertex = no. of Faces adjacent to this Vertex
    // = size of the FaceVertex list = valence of Vertex
    uint numEdges(void) const {
      return fvpList.size();
    }
     
    uint numFaces(void) const {
      return fvpList.size();
    }

    uint valence(void) const {
      return fvpList.size();
    }
     
    uint getID(void) const {
      return uID;
    }

    uint getIndex(void) const {
      return index;
    }

    Vector3d getAuxCoords(void) const {
      return auxcoords;
    }

    Vector3d getAuxNormal(void) const {
      return auxnormal;
    }
     
    Vector3d getNormal(void) const {
      return normal;
    }

    //--- Mutative functions ---//

    void setType(DLFLVertexType type) {
      vtType = type;
    }

    void resetType(void) {
      vtType = VTNormal;
    }

    // Reset type of vertex, all face-vertices and edges connected to this vertex
    void resetTypeDeep(void);

    friend void resetVertexType(DLFLVertexPtr dvp) {
      dvp->resetType();
    }

    void setFaceVertexList(const DLFLFaceVertexPtrList& list) {
      fvpList = list;
    }

    void setCoords(const Vector3d& p) {
      coords = p;
    }

    // Set the aux. coords
    void setAuxCoords(const Vector3d& p) {
      auxcoords = p;
    }

    void setAuxNormal(const Vector3d& n) {
      auxnormal = n;
    }
     
    // Add to the aux coords
    void addToAuxCoords(const Vector3d& p) {
      auxcoords += p;
    }

    void addToAuxNormal(const Vector3d& n) {
      auxnormal += n;
    }
     
    // Reset the aux coords
    void resetAuxCoords(void) {
      auxcoords.reset();
    }

    void resetAuxNormal(void) {
      auxnormal.reset();
    }
           
    // Set the texture coordinates for all FaceVertexes referring to this vertex
    void setTexCoords(const Vector2d& texcoord);

    // Set the color values for all FaceVertexes referring to this vertex
    void setColor(const RGBColor& color);

    // Set the normal for this vertex directly
    void setNormal(const Vector3d& n) {
      normal = n; normalize(normal);
    }

    // If flag is true recompute normals for all FaceVertexes referring to this vertex,
    // Update the vertex normal. Returns the vertex normal
    Vector3d updateNormal(bool recompute=true);

    Vector3d computeNormal(bool set=false);

    // Retained for backward compatibility
    // Same as updateNormal but doesn't recompute the corner normals
    Vector3d averageNormal(void) {
      return updateNormal(false);
    }

    // Get normals of all corners referring to this vertex. Returns average normal
    Vector3d getNormals(Vector3dArray& normals);

    // Set tex coordinates, color and normal info for all FaceVertexes referring to this vertex
    void setFaceVertexProps(const Vector2d& texcoord, const RGBColor& color, const Vector3d& normal);

    // Update the DLFLFaceVertexList by adding a new DLFLFaceVertexPtr
    void addToFaceVertexList(DLFLFaceVertexPtr fvptr) {
      fvpList.push_back(fvptr);
    }

    void deleteFromFaceVertexList(DLFLFaceVertexPtr fvptr) {
      fvpList.remove(fvptr);
    }

    // DLFL Vertex Trace
    void vertexTrace(void) const;
    friend void vertexTrace(DLFLVertexPtr vertexptr) {
      vertexptr->vertexTrace();
    }

    friend void vertexTrace(const DLFLVertex& vertex) {
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
    void getOrderedFaceVertices(DLFLFaceVertexPtrArray& fvparray);
     
    void getCorners(DLFLFaceVertexPtrArray& fvparray) {
      getFaceVertices(fvparray);
    }
    void getOrderedCorners(DLFLFaceVertexPtrArray& fvparray) {
      getOrderedFaceVertices(fvparray);
    }
     
    // Get the aux coords from all corners which share this Vertex
    void getCornerAuxCoords(Vector3dArray& coords) const;
    void getOrderedCornerAuxCoords(Vector3dArray& coords) const;

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
    void transform(const Matrix4x4& tmat) {
      Vector4d tp(coords); tp[3] = 1.0;
      tp = tmat * tp;
      tp /= tp[3];
      coords = tp;
    }

    // Print out this Vertex
    void print(void) const {
      cout << "DLFLVertex " << uID << "," << vtType << "," << valence() << " : " << coords << endl;
    }

    // Write this vertex in DLFL format and set it's index value
    void writeDLFL(ostream& o, uint newindex) {
      double x,y,z; coords.get(x,y,z);
      o << "v " << x << ' ' << y << ' ' << z << endl;
      index = newindex;
    }

    // Read a vertex from an input stream.
    // The 3 coordinates should be specified separated by spaces (as in OBJ format)
    friend istream& operator >> (istream& i, DLFLVertex& dv) {
      // Read x,y,z coordinates.
      double x,y,z;
      i >> x >> y >> z;
      dv.coords.set(x,y,z);
      return i;
    }

    // Write a vertex to an output stream. Writes in OBJ format
    friend ostream& operator << (ostream& o, const DLFLVertex& dv) {
      // Only coordinates are written.
      double x,y,z;
      dv.coords.get(x,y,z);
      o << "v " << x << ' ' << y << ' ' << z << endl;
      return o;
    }
    /*
    // Do a glVertex on this point
    friend void glVertex(const DLFLVertex& dv) {
    dv.render();
    }

    friend void glVertexP(const DLFLVertexPtr& dvp) {
    dvp->render();
    }
    */
  };
} // end namespace

#endif /* _DLFL_VERTEX_HH_ */
