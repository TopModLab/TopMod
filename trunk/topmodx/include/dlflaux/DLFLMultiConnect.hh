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

#ifndef _DLFLMULTI_CONNECT_H_
#define _DLFLMULTI_CONNECT_H_

#include <DLFLObject.hh>

namespace DLFL {

  class HalfEdgePair {
    // Class defines a connection as that between two half-edges
    // Also contains members to store other calculated information
    // about this connection such as distance between edges, planarity of
    // resulting quadrilateral, etc.
    // Contains comparison methods to allow sorting a list of EdgePairs
    // according to priority for connection.
  public :

    // Pointers to edges which were most recently connected
    static DLFLEdgePtr last_connected1, last_connected2; 

    DLFLEdgePtr ep1, ep2; // Edges to be connected
    DLFLFacePtr fp1, fp2; // Faces which define the half-edges to be connected
    // These faces define the half-edges that will remain valid even after
    // a connection has been made. i.e. the half-edges which should NOT be
    // connected. The other face adjacent to the edge is used to define the
    // half-edge that will actually be used to make the connection

    double mpdist;     // Distance between midpoints of the two edges
    double planarity; // Planarity of quad resulting from edge connection.
    // planarity value will be clamped to lie between 0 and 1.
    // 1 means the quad is perfectly planar
    // 0 means the two edges are perpendicular to each other
    Vector3d normal; // Average normal of quad resulting from edge connection.

    // Default constructor
    HalfEdgePair()
      : ep1(NULL), ep2(NULL), fp1(NULL), fp2(NULL), mpdist(0.0), planarity(0.0), normal()
    {}
     

    // Constructor
    HalfEdgePair(DLFLEdgePtr eptr1, DLFLEdgePtr eptr2,
		 DLFLFacePtr fptr1, DLFLFacePtr fptr2)
      : ep1(eptr1), ep2(eptr2), fp1(fptr1), fp2(fptr2), mpdist(0.0), planarity(0.0), normal()
    { computeMetrics(); }

    // Copy constructor
    HalfEdgePair(const HalfEdgePair& hep)
      : ep1(hep.ep1), ep2(hep.ep2), fp1(hep.fp1), fp2(hep.fp2),
	mpdist(hep.mpdist), planarity(hep.planarity), normal(hep.normal)
    {}

    // Destructor
    ~HalfEdgePair() {}

    // Assignment operator
    HalfEdgePair& operator = (const HalfEdgePair& hep) {
      ep1 = hep.ep1; ep2 = hep.ep2;
      fp1 = hep.fp1; fp2 = hep.fp2;
      mpdist = hep.mpdist; planarity = hep.planarity; normal = hep.normal;
      return (*this);
    }

    // Compute metrics for this pair
    void computeMetrics(void) {
      if ( ep1 && ep2 ) {
	mpdist = distBetween(ep1,ep2);

	// Compute the average normal for this quad formed by this pair
	// Takes average of normals at any two opposite corners formed with adjacent corners.
	// Rotation order is defined by associated face and not order of vertices in edge
	DLFLFaceVertexPtr e1fvp1, e1fvp2, e2fvp1, e2fvp2;

	e1fvp1 = ep1->getFaceVertexPtr(ep1->getOtherFacePointer(fp1)); e1fvp2 = e1fvp1->next();
	e2fvp1 = ep2->getFaceVertexPtr(ep2->getOtherFacePointer(fp2)); e2fvp2 = e2fvp1->next();
         
	Vector3d v11, v12, v21, v22; // Corners
	Vector3d av11, av12, av21, av22; // Adjusted coordinates of corners - see below
	Vector3d n11, n21; // Normals at 2 opposite corners

	v11 = e1fvp1->getVertexCoords(); v12 = e1fvp2->getVertexCoords();
	v21 = e2fvp1->getVertexCoords(); v22 = e2fvp2->getVertexCoords();

	// Adjust coordinates of the end points to take care of situations where
	// the two edges share a vertex (which will cause problems in
	// normal & planarity computation. Move the end points slightly inward
	// along its edge. Note that this is done only for the local computation
	av11 = 0.75*v11 + 0.25*v12; av12 = 0.25*v11 + 0.75*v12;
	av21 = 0.75*v21 + 0.25*v22; av22 = 0.25*v21 + 0.75*v22;

	// Use adjusted corners for computation
	n11 = normalized( (av22-av11) % (av12-av11) );
	n21 = normalized( (av12-av21) % (av22-av21) );

	normal = normalized(n11+n21);

	// Compute planarity. Dot product between normals at opposite corners
	// is taken to be a measure of planarity. If both normals are same,
	// dot product will be 1, otherwise it will be less than 1
	planarity = n11 * n21;
      }
    }

    // Comparison between two HalfEdgePairs. Uses distance to determine relative ordering
    friend bool less_than(const HalfEdgePair& hep1, const HalfEdgePair& hep2);
/*
    friend bool less_than(const HalfEdgePair& hep1, const HalfEdgePair& hep2) {
      if ( hep1.mpdist < hep2.mpdist ) return true;
      return false;
    }
*/
    // Comparison between two HalfEdgePairs. Uses distance to determine relative ordering
    friend bool greater_than(const HalfEdgePair& hep1, const HalfEdgePair& hep2);
/*
    friend bool greater_than(const HalfEdgePair& hep1, const HalfEdgePair& hep2) {
      if ( hep1.mpdist > hep2.mpdist ) return true;
      return false;
    }
*/
    // Checks to see if given HalfEdgePair contains an edge which
    // was most recently connected. Uses the stored static variables
    // to do the comparison. Assumes variables have been set.
    friend bool contains_connected(const HalfEdgePair& hep);
/*
    friend bool contains_connected(const HalfEdgePair& hep) {
      if ( hep.ep1 == HalfEdgePair::last_connected1 ||
	   hep.ep1 == HalfEdgePair::last_connected2 ||
	   hep.ep2 == HalfEdgePair::last_connected1 ||
	   hep.ep2 == HalfEdgePair::last_connected2 ) return true;
      return false;
    }
*/

    void print(void) const {
      if ( ep1 && ep2 )
	cout << "{" << ep1->getID() << "," << (ep1->getOtherFacePointer(fp1))->getID() << "}"
	     << " <--> "
	     << "{" << ep2->getID() << "," << (ep2->getOtherFacePointer(fp2))->getID() << "}"
	     << " : " << mpdist << "," << planarity << "," << normal << endl;
    }
  };

  typedef vector<HalfEdgePair> HalfEdgePairArray;
  typedef list<HalfEdgePair> HalfEdgePairList;

  // a struct for a Plane to simplify storage and access
  struct Plane {
  public :
    Vector3d origin, normal;
     
    Plane()
      : origin(), normal()
    {}
     
    Plane(const Plane& plane)
      : origin(plane.origin), normal(plane.normal)
    {}
     
    ~Plane()
    {}
     
    Plane& operator = (const Plane& plane) {
      origin = plane.origin; normal = plane.normal;
      return (*this);
    }

    // Find intersection of this Plane with a ray starting at p in direction v.
    // Computes parametric distance along v with p as origin and returns it as parameter
    // Returns false if there is no intersection, true otherwise
    bool intersect(const Vector3d& p, const Vector3d& v, double& t) {
      double vn = v * normal;
      if ( !isNonZero(vn) ) return false;

      t = ((origin - p)*normal)/vn;

      return true;
    }
  };

  typedef vector<Plane> PlaneArray;
  typedef vector<PlaneArray> PlaneArrayArray;

  void tripleConnectFaces( DLFLObjectPtr obj, DLFLFacePtr fp1, DLFLFacePtr fp2, DLFLFacePtr fp3);
  void multiConnectFaces( DLFLObjectPtr obj, DLFLFacePtrArray fp);
  void multiConnectFaces( DLFLObjectPtr obj, DLFLFacePtrArray fparray, double scale_factor, double extrude_dist, bool use_max_offsets=false);
  void multiConnectFaces( DLFLObjectPtr obj, DLFLFacePtrArray fparray, double min_factor, bool make_connections=true);
  void findMaxOffsets( DLFLFacePtrArray fparray, DoubleArray& maxoffsets);
  void multiConnectMidpoints( DLFLObjectPtr obj );
  void multiConnectCrust( DLFLObjectPtr obj, double scale_factor=0.5 );
  void modifiedMultiConnectCrust(DLFLObjectPtr obj, double scale_factor=0.5 );
  void createSponge( DLFLObjectPtr obj, double thickness, double collapse_threshold_factor=0.0 );

  bool less_than(const HalfEdgePair& hep1, const HalfEdgePair& hep2);
  bool greater_than(const HalfEdgePair& hep1, const HalfEdgePair& hep2);
  bool contains_connected(const HalfEdgePair& hep);

} // end namespace

#endif // _DLFLMULTI_CONNECT_H_
