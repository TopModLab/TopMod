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
 * \file DLFLEdge.hh
 */

#ifndef _DLFL_EDGE_HH_
#define _DLFL_EDGE_HH_

// Class to define an edge
// And edge contains 2 pointers - one to each end of the edge
// The pointers are to FaceVertex structures
// An edge appears twice in the system unless it is a boundary edge of a 2D surface
// Each of the pointers points to the beginning Vertex of the edge in each of the 2 cases
// So each of the pointers will be to Face Nodes in different faces
// Definition of class FaceVertexPtr is not required for this class since only the pointer
// is stored

#include "DLFLCommon.hh"

namespace DLFL {

  class DLFLEdge {
  public :
    static void setLastID( uint id ) {
      if( id > suLastID )
				suLastID = id;
    };

  protected :
    static uint suLastID;                             // Distinct ID for each instance
    // The last assigned ID is stored in this
    // class variable
     
    DLFLFaceVertexPtr  fvpV1;
    DLFLFaceVertexPtr  fvpV2;
    uint uID;                                         // ID for this Edge
    DLFLEdgeType       etType;                        // For use in subdivision surfaces
    Vector3d           auxcoords;                     // Coords for use during subdivs, etc.
    Vector3d           auxnormal;                     // Extra storage for normal
    Vector3d           midpoint;                      // Midpoint of edge (not always current)
    Vector3d           normal;                        // Edge normal (at midpoint, not always current)

  public :

    unsigned long      flags;                         // Variable for general use to store flags, etc.
    uint               ismarked;
    uint isdummy;
    uint istodel;
    uint isvisited;

  protected:

    // Generate a new unique ID
    static uint newID(void) {
      uint temp = suLastID;
      suLastID++;
      return temp;
    }
     
    // Assign a unique ID for this instance
    void assignID(void) {
      uID = DLFLEdge :: newID();
      ismarked = 0;
			isvisited = 0;
    }

    // Update the mid point for this edge
    void updateMidPoint(void);

    // Update the edge normal - average of normals at the 4 corners adjacent to this edge
    void updateNormal(void);

  public :

    // Default constructor
    DLFLEdge()
      : fvpV1(NULL), fvpV2(NULL), etType(ETNormal), auxcoords(), auxnormal(), midpoint(), normal(), flags(0)
    { assignID(); }

    // 2 & 3 argument constructor
    DLFLEdge(DLFLFaceVertexPtr fvp1, DLFLFaceVertexPtr fvp2, bool update=true)
      : fvpV1(fvp1), fvpV2(fvp2), etType(ETNormal), auxcoords(), auxnormal(), midpoint(), normal(), flags(0)
    {
      if ( update ) {
				updateNormal();
      }
      updateMidPoint();
      assignID();
    }

    // Copy constructor
    DLFLEdge(const DLFLEdge& e)
      : fvpV1(e.fvpV1), fvpV2(e.fvpV2), uID(e.uID), etType(e.etType), auxcoords(e.auxcoords), auxnormal(e.auxnormal),
				midpoint(e.midpoint), normal(e.normal), flags(e.flags)
    {}

    // Destructor
    ~DLFLEdge()
    {}

    // Assignment operator
    DLFLEdge& operator = (const DLFLEdge& e) {
      fvpV1 = e.fvpV1; fvpV2 = e.fvpV2; uID = e.uID; etType = e.etType; auxcoords = e.auxcoords; auxnormal = e.auxnormal;
      midpoint = e.midpoint; normal = e.normal; flags = e.flags;
      return (*this);
    }

    // Copy function 
    DLFLEdgePtr copy(void) const {
      DLFLEdgePtr eptr = new DLFLEdge(*this);
      return eptr;
    }

    // Dump contents of object
    void dump(ostream& o) const;

    // Get/set the attribute flags
    DLFLEdgeType getType(void) const {
      return etType;
    }

    Vector3d getAuxCoords(void) const {
      return auxcoords;
    }

    Vector3d getAuxNormal(void) const {
      return auxnormal;
    }
     
    void setType(DLFLEdgeType type) {
      etType = type;
    }

    void resetType(void) {
      etType = ETNormal;
    }

    void setAuxCoords(const Vector3d& p) {
      auxcoords = p;
    }

    void setAuxNormal(const Vector3d& n) {
      auxnormal = n;
    }
     
    void addToAuxCoords(const Vector3d& p) {
      auxcoords += p;
    }

    void addToAuxNormal(const Vector3d& n) {
      auxnormal += n;
    }
     
    void resetAuxCoords(void) {
      auxcoords.reset();
    }

    void resetAuxNormal(void) {
      auxnormal.reset();
    }
           
    friend void resetEdgeType(DLFLEdgePtr dep) {
      dep->resetType();
    }

    // Query functions
    DLFLFaceVertexPtr getFaceVertexPtr1(void) const {
      return fvpV1;
    }

    DLFLFaceVertexPtr getFaceVertexPtr2(void) const {
      return fvpV2;
    }

    // Get the face-vertex belonging to given face
    // Returns NULL if edge is not adjacent to given face
    DLFLFaceVertexPtr getFaceVertexPtr(DLFLFacePtr fptr);

    DLFLFaceVertexPtr getOtherFaceVertexPtr(DLFLFaceVertexPtr fvptr) {
      // Return the other DLFLFaceVertexPtr for this edge.
      // Returns NULL if given DLFLFaceVertexPtr is not found in this edge
      if ( fvpV1 == fvptr ) return fvpV2;
      else if ( fvpV2 == fvptr ) return fvpV1;
      return NULL;
    }

    void getFaceVertexPointers(DLFLFaceVertexPtr& fvptr1, DLFLFaceVertexPtr& fvptr2) const {
      fvptr1 = fvpV1; fvptr2 = fvpV2;
    }

    void getCorners(DLFLFaceVertexPtr& fvp1, DLFLFaceVertexPtr& fvp2) const {
      getFaceVertexPointers(fvp1,fvp2);
    }

    uint getID(void) const {
      return uID;
    }

    Vector3d getMidPoint(bool update=false) {
			if( update )
				updateMidPoint();
      return midpoint;
    }

    Vector3d getNormal(bool update=false) {
			if( update )
				updateNormal();
      return normal;
    }

    void getEndPoints(Vector3d& p1, Vector3d& p2) const;
    double length(void) const;

    // Get a vector along the direction of the edge from endpoint 1 to endpoint 2
    Vector3d getEdgeVector(void) const;

    // Get the pointers to the 2 faces that this edge is adjacent to
    void getFacePointers(DLFLFacePtr& fptr1, DLFLFacePtr& fptr2);

    // Get pointer to face adjacent to this edge which is not the given face
    DLFLFacePtr getOtherFacePointer(DLFLFacePtr fptr);

    // Get the pointers to the 2 DLFLVertices that define this edge
    void getVertexPointers(DLFLVertexPtr& vp1, DLFLVertexPtr& vp2);

    // Get pointer to vertex which is at opposite end to given vertex pointer
    DLFLVertexPtr getOtherVertexPointer(DLFLVertexPtr vptr);

    // Get all 4 corners adjacent to this edge in CCW order such that those
    // corners will make a face (EF = Edge Face)
    void getEFCorners(DLFLFaceVertexPtrArray& corners);

    // Get the aux coords of all 4 corners adjacent to this edge in CCW order such
    // that those corners make a face (EF = Edge Face)
    void getEFCornersAuxCoords(Vector3dArray& coords);

    // NOTE : All the 4 functions below will return the distance from one
    // of the end points when the point is not in the rectangular region
    // parallel to the line segment
     
    // Distance of a point from the DLFLEdge in 3D
    double distFrom(double x, double y, double z) const;

    // For hit calculations with projections
    double distFromXY(double x, double y) const;
    double distFromYZ(double y, double z) const;
    double distFromZX(double z, double x) const;

    // Find distance between midpoints of two given edges
    friend double distBetween(DLFLEdgePtr ep1, DLFLEdgePtr ep2);

    // Get the Edge normals - vectors along the surface perpendicular to the edge
    // Calculated as cross-product of surface normal at corner and edge vector
    // Will be opposite in direction for the two corners
    // Specify the corner where the normal is to be calculated. This defines the origin
    // for the edge vector. Checks to see if given corner belongs to Edge. If not returns
    // a zero vector as the normal
    Vector3d getEdgeNormal(DLFLFaceVertexPtr fvptr) const;

    // Compute and return the average of the normal at the two end vertices
    Vector3d averageVertexNormal(void) const;

    // Compute and return the average of the normal at the two end corners
    Vector3d averageCornerNormal(void) const;

    // Mutative functions
    void setFaceVertexPtr1(DLFLFaceVertexPtr fvptr, bool update=true) {
      fvpV1 = fvptr;
      if ( update ) {
				updateNormal();
      }
      updateMidPoint();
    }

    void setFaceVertexPtr2(DLFLFaceVertexPtr fvptr, bool update=true) {
      fvpV2 = fvptr;
      if ( update ) {
				updateNormal();
      }
      updateMidPoint();
    }

    void setFaceVertexPointers(DLFLFaceVertexPtr fvptr1, DLFLFaceVertexPtr fvptr2, bool update=true) {
      fvpV1 = fvptr1; fvpV2 = fvptr2;
      if ( update ) {
				updateNormal();
      }
      updateMidPoint();
    }
     
    void makeUnique(void) {
      assignID();
    }

    friend void makeEdgeUnique(DLFLEdgePtr dep) {
      dep->makeUnique();
    }

  private :
     
    // Reverse the edge. Updates the face-vertex pointers appropriately.
    // WARNING! This function will leave the object in an invalid state
    // unless ALL other edges and faces are also reversed. To prevent unintended
    // problems, this is made private and classes which need access to this
    // function are made friends
    void reverse(void);

    friend class DLFLObject;

  public :

    // Update the DLFLEdgePtr field of the 2 FaceVertex members
    void updateFaceVertices(void);

    // Update the FaceVertexPtr fields selectively

    // If any of the FaceVertexPtr fields are NULL set it to the given FaceVertexPtr
    void setNullFaceVertexPtr(DLFLFaceVertexPtr fvptr) {
      if ( fvpV1 == NULL ) fvpV1 = fvptr;
      else if ( fvpV2 == NULL ) fvpV2 = fvptr;
    }

    // If any of the FaceVertexPtr fields matches the given FaceVertexPtr, set it to NULL
    void resetFaceVertexPtr(DLFLFaceVertexPtr fvptr) {
      if ( fvpV1 == fvptr ) fvpV1 = NULL;
      else if ( fvpV2 == fvptr ) fvpV2 = NULL;
    }
     
    // Equality operator - two edges are equal if they have the same 2 end-points
    friend bool operator == (const DLFLEdge& e1, const DLFLEdge& e2);

    // Check if the DLFLEdge is a 'valid' edge - both FaceVertexes shouldn't be from the
    // same Face
    bool isValid(void) const;
     
    // Is this Edge a self-loop? Are the 2 end points the same?
    bool isSelfLoop(void) const;

    // Check if 2 edges intersect
    friend bool checkIntersection(DLFLEdgePtr ep1, DLFLEdgePtr ep2);

    // Print out the DLFLEdge
    friend ostream& operator << (ostream& o, const DLFLEdge& e);

    // Print out the Face IDs of the 2 FaceVertices
    void printFaceIDs(void) const;
    void printPointers(void) const;
    void print(void) const { printFaceIDs();}

    // Write out the edge in DLFL format
    void writeDLFL(ostream& o) const;

    // Write out the edge in DLFL format but in reverse order. Useful for crust modeling
    void writeDLFLReverse(ostream& o) const;

  };

  bool coFacial(DLFLEdgePtr ep1, DLFLEdgePtr ep2);

} // end namespace

#endif /* #ifndef _DLFL_EDGE_HH_ */

