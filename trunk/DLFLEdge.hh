/* $Id: DLFLEdge.hh,v 4.0 2003/12/26 01:58:52 vinod Exp $ */

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

class DLFLEdge
{
  protected :

     static uint suLastID;                             // Distinct ID for each instance
                                                       // The last assigned ID is stored in this
                                                       // class variable
     
     DLFLFaceVertexPtr  fvpV1;
     DLFLFaceVertexPtr  fvpV2;
     uint uID;                                         // ID for this Edge
     DLFLEdgeType       etType;                        // For use in subdivision surfaces
     Vector3d           auxcoords;                     // Coords for use during subdivs, etc.

     // Bezier surface info
     DLFLControlPoint      centroid;                   // Center control point
     DLFLControlPointArray cpfArray1;                  // Control points for forward edge
     DLFLControlPointArray cpfArray2;                  
     DLFLControlPointArray cpbArray1;                  // Control points for backward edge
     DLFLControlPointArray cpbArray2;

  public :

     unsigned long      flags;                         // Variable for general use to store flags, etc.

  protected:

        // Generate a new unique ID
     static uint newID(void)
       {
         uint temp = suLastID;
         suLastID++;
         return temp;
       }
     
        // Assign a unique ID for this instance
     void assignID(void)
       {
         uID = DLFLEdge :: newID();
       }
     

  public :

        // Default constructor
     DLFLEdge()
       : fvpV1(NULL), fvpV2(NULL), etType(ETNormal), auxcoords(), flags(0)
       {
         assignID();
       }

        // 2 argument constructor
     DLFLEdge(DLFLFaceVertexPtr fvp1, DLFLFaceVertexPtr fvp2)
       : fvpV1(fvp1), fvpV2(fvp2), etType(ETNormal), auxcoords(), flags(0)
       {
         assignID();
       }

        // Copy constructor
     DLFLEdge(const DLFLEdge& e)
       : fvpV1(e.fvpV1), fvpV2(e.fvpV2), uID(e.uID), etType(e.etType), auxcoords(e.auxcoords),
         flags(e.flags),
         cpfArray1(e.cpfArray1),
         cpbArray1(e.cpbArray1),
         cpfArray2(e.cpfArray2),
         cpbArray2(e.cpbArray2)
       {}

        // Destructor
     ~DLFLEdge()
       {}

        // Assignment operator
     DLFLEdge& operator = (const DLFLEdge& e)
       {
         fvpV1 = e.fvpV1; fvpV2 = e.fvpV2; uID = e.uID; etType = e.etType; auxcoords = e.auxcoords;
         flags = e.flags;
         cpfArray1 = e.cpfArray1;
         cpbArray1 = e.cpbArray1;
         cpfArray2 = e.cpfArray2;
         cpbArray2 = e.cpbArray2;
         return (*this);
       }

        // Copy function
     DLFLEdgePtr copy(void) const
       {
         DLFLEdgePtr eptr = new DLFLEdge(*this);
         return eptr;
       }

     void setControlPointForwardArray1(const DLFLControlPointArray& cpArray)
       {
         cpfArray1 = cpArray;
       }
     void setControlPointBackwardArray1(const DLFLControlPointArray& cpArray)
       {
         cpbArray1 = cpArray;
       }
     void setControlPointForwardArray2(const DLFLControlPointArray& cpArray)
       {
         cpfArray2 = cpArray;
       }
     void setControlPointBackwardArray2(const DLFLControlPointArray& cpArray)
       {
         cpbArray2 = cpArray;
       }

     DLFLControlPointArray getControlPointForwardArray1() const
       {
         return cpfArray1;
       }
     DLFLControlPointArray getControlPointBackwardArray1() const
       {
         return cpbArray1;
       }
     DLFLControlPointArray getControlPointForwardArray2() const
       {
         return cpfArray2;
       }
     DLFLControlPointArray getControlPointBackwardArray2() const
       {
         return cpbArray2;
       }

        // Dump contents of object
     void dump(ostream& o) const;

        // Get/set the attribute flags
     DLFLEdgeType getType(void) const
       {
         return etType;
       }

     Vector3d getAuxCoords(void) const
       {
         return auxcoords;
       }

     void geomCentroid(void);

     DLFLControlPoint getCentroid(void) const 
       {
         return centroid;
       }

     void setType(DLFLEdgeType type)
       {
         etType = type;
       }

     void resetType(void)
       {
         etType = ETNormal;
       }

     void setAuxCoords(const Vector3d& p)
       {
         auxcoords = p;
       }

     void addToAuxCoords(const Vector3d& p)
       {
         auxcoords += p;
       }

     void resetAuxCoords(void)
       {
         auxcoords.reset();
       }
     
     friend void resetEdgeType(DLFLEdgePtr dep)
       {
         dep->resetType();
       }

        // Query functions
     DLFLFaceVertexPtr getFaceVertexPtr1(void) const
       {
         return fvpV1;
       }

     DLFLFaceVertexPtr getFaceVertexPtr2(void) const
       {
         return fvpV2;
       }

        // Get the face-vertex belonging to given face
        // Returns NULL if edge is not adjacent to given face
     DLFLFaceVertexPtr getFaceVertexPtr(DLFLFacePtr fptr);

     DLFLFaceVertexPtr getOtherFaceVertexPtr(DLFLFaceVertexPtr fvptr)
       {
            // Return the other DLFLFaceVertexPtr for this edge.
            // Returns NULL if given DLFLFaceVertexPtr is not found in this edge
         if ( fvpV1 == fvptr ) return fvpV2;
         else if ( fvpV2 == fvptr ) return fvpV1;
         return NULL;
       }

     void getFaceVertexPointers(DLFLFaceVertexPtr& fvptr1, DLFLFaceVertexPtr& fvptr2) const
       {
         fvptr1 = fvpV1; fvptr2 = fvpV2;
       }
     
     uint getID(void) const
       {
         return uID;
       }

        // Check if two given edges are co-facial
     friend bool coFacial(DLFLEdgePtr ep1, DLFLEdgePtr ep2);

     Vector3d getMidPoint(void) const;
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

        // Mutative functions
     void setFaceVertexPtr1(DLFLFaceVertexPtr fvptr)
       {
         fvpV1 = fvptr;
       }

     void setFaceVertexPtr2(DLFLFaceVertexPtr fvptr)
       {
         fvpV2 = fvptr;
       }

     void setFaceVertexPointers(DLFLFaceVertexPtr fvptr1, DLFLFaceVertexPtr fvptr2)
       {
         fvpV1 = fvptr1; fvpV2 = fvptr2;
       }
     
     void makeUnique(void)
       {
         assignID();
       }

     friend void makeEdgeUnique(DLFLEdgePtr dep)
       {
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
     void setNullFaceVertexPtr(DLFLFaceVertexPtr fvptr)
       {
         if ( fvpV1 == NULL ) fvpV1 = fvptr;
         else if ( fvpV2 == NULL ) fvpV2 = fvptr;
       }

        // If any of the FaceVertexPtr fields matches the given FaceVertexPtr, set it to NULL
     void resetFaceVertexPtr(DLFLFaceVertexPtr fvptr)
       {
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
     void print(void) const
       {
         printFaceIDs();
       }

        // Write out the edge in DLFL format
     void writeDLFL(ostream& o) const;

        // Write out the edge in DLFL format but in reverse order. Useful for crust modeling
     void writeDLFLReverse(ostream& o) const;

        // Render the DLFLEdge as a line segment
     void render(void) const;

     friend void renderP(const DLFLEdgePtr& dep)
       {
         dep->render();
       }

     friend void render(const DLFLEdge& de)
       {
         de.render();
       }
};

#endif /* #ifndef _DLFL_EDGE_HH_ */


/*
  $Log: DLFLEdge.hh,v $
  Revision 4.0  2003/12/26 01:58:52  vinod
  Major version sync.

  Revision 3.1  2003/11/23 17:37:32  vinod
  Added checkIntersection method

  Revision 3.0  2003/09/19 16:06:47  vinod
  Major version update

  Revision 2.13  2003/09/19 16:04:04  vinod
  Added self-loop checking method

  Revision 2.12  2003/04/30 18:04:17  vinod
  Added reverse method

  Revision 2.11  2003/04/04 07:19:37  vinod
  Added 'flags' member variable for general use

  Revision 2.10  2003/03/12 07:36:56  vinod
  Added getOtherVertexPointer method

  Revision 2.9  2003/01/30 07:01:38  vinod
  Added coFacial, getOtherFacePointer, distBetween methods

  Revision 2.8  2003/01/27 05:11:06  vinod
  Added getEdgeVector method

  Revision 2.7  2002/12/30 17:27:51  vinod
  Added getVertexPointers() and length() methods

  Revision 2.6  2002/12/27 06:32:29  vinod
  Added getEdgeNormal method

  Revision 2.5  2001/11/21 00:14:25  vinod
  Added writeDLFLReverse

  Revision 2.4  2001/11/02 00:33:10  vinod
  Added getEndPoints

  Revision 2.3  2001/10/07 07:13:01  vinod
  Added addToAuxCoords, resetAuxCoords methods

  Revision 2.2  2001/09/28 05:48:53  vinod
  Added method to find face-vertex belonging to a given face

  Revision 2.1  2001/09/15 22:35:50  vinod
  Added getMidPoint

  Revision 2.0  2001/07/25 05:12:43  vinod
  Major version sync

  Revision 1.4  2001/04/25 19:51:37  vinod
  Added getFacePointers

  Revision 1.3  2001/04/08 21:16:38  vinod
  Added writeDLFL, auxcoords

  Revision 1.2  2000/11/21 19:19:01  vinod
  Added dump() method

  Revision 1.1.1.1  2000/11/02 19:32:43  vinod
  New DLFL Sources

*/
