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
*
* Short description of this file
*
* name of .hh file containing function prototypes
*
*/


#include "DLFLMultiConnect.hh"

#include <DLFLCore.hh>
#include <DLFLCoreExt.hh>
#include <DLFLObject.hh>
#include "DLFLConvexHull.hh"
#include "DLFLConnect.hh"
#include "DLFLCrust.hh"
#include "DLFLExtrude.hh"

namespace DLFL {

	DLFLEdgePtr HalfEdgePair::last_connected1 = NULL;
	DLFLEdgePtr HalfEdgePair::last_connected2 = NULL;

	void tripleConnectFaces( DLFLObjectPtr obj, DLFLFacePtr fp1, DLFLFacePtr fp2, DLFLFacePtr fp3){ 
		// Connect 3 faces. Connects closest edges between each pair of faces
		// Only edges which make an angle of 45 deg or less with the normal to the plane
		// containing the centroids

		// Find centroids of the 3 faces
		Vector3d cen1, cen2, cen3, cen_cen;

		cen1 = fp1->geomCentroid(); cen2 = fp2->geomCentroid(); cen3 = fp3->geomCentroid();
		cen_cen = (cen1+cen2+cen3)/3.0;

		// Find normal to the plane containing the centroids
		// Direction is irrelevant
		Vector3d cpnormal = (cen2-cen1)%(cen3-cen1);
		normalize(cpnormal);

		// Find candidate edges in each face. Angle between edge and cpnormal
		// should be less than 45 degrees
		DLFLEdgePtrArray ep1, ep2, ep3;
		DLFLEdgePtrList cedges; // Use linked list, not array, to simplify erase operations
		DLFLFacePtrList cfaces; // Faces for candidate edges (we need half-edges to connect)

		//--- NOTE ---//
		// In the list of faces, the other face for the edge is stored, instead of the
		// face which defines the half-edge to be connected. This is because once two half-edges
		// are connected, that face pointer might no longer be valid, but the other side will
		// still be valid. This assumes that both sides of the edge are not the same face


		// Get all edges in the faces.
		fp1->getEdges(ep1); fp2->getEdges(ep2); fp3->getEdges(ep3);

		DLFLEdgePtr ep;
		//double cosangle = cos(80.0*M_PI/180.0);
		Vector3d evec;

		// Find candidate edges and add them to the cedges list
		for (int i=0; i < (int)ep1.size(); ++i) {
			ep = ep1[i];
			evec = normalized(ep->getEdgeVector());
			//       if ( Abs(evec * cpnormal) > cosangle )
			if ( true ) {
				cedges.push_back(ep);
				cfaces.push_back(ep->getOtherFacePointer(fp1)); // See NOTE above
			}
		}
		for (int i=0; i < (int)ep2.size(); ++i) {
			ep = ep2[i];
			evec = normalized(ep->getEdgeVector());
			//       if ( Abs(evec * cpnormal) > cosangle )
			if ( true ) {
				cedges.push_back(ep);
				cfaces.push_back(ep->getOtherFacePointer(fp2)); // See NOTE above
			}
		}
		for (int i=0; i < (int)ep3.size(); ++i) {
			ep = ep3[i];
			evec = normalized(ep->getEdgeVector());
			//       if ( Abs(evec * cpnormal) > cosangle )
			if ( 1 ) {
				cedges.push_back(ep);
				cfaces.push_back(ep->getOtherFacePointer(fp3));
			}
		}

		// cedges now contains all the candidate edges
		cout << "Candidate edges : ";
		DLFLEdgePtrList::iterator epfirst, eplast;
		epfirst = cedges.begin(); eplast = cedges.end();
		while ( epfirst != eplast ) {
			cout << (*epfirst)->getID() << " "; ++epfirst;
		}
		cout << endl;

		/*
		Find all possible connections among the candidate edges
			Add them to an array of HalfEdgePairs to sort and make
			connections later. Valid connections are all those between
			half-edges in different faces
		*/

			HalfEdgePairArray heparray;
		DLFLFacePtr fp;
		DLFLFacePtrList::iterator fpfirst;
		while ( !cedges.empty() ) {
			ep = cedges.front(); cedges.pop_front();
			fp = cfaces.front(); cfaces.pop_front();

			// Go through remaining edges. Find edge which is closest
			// but is not in the same face as ep
			epfirst = cedges.begin(); eplast = cedges.end();
			fpfirst = cfaces.begin();
			DLFLEdgePtr temp_ep;
			DLFLFacePtr temp_fp, ofp, temp_ofp;
			while ( epfirst != eplast ) {
				temp_ep = (*epfirst); temp_fp = (*fpfirst);
				ofp = ep->getOtherFacePointer(fp);
				temp_ofp = temp_ep->getOtherFacePointer(temp_fp);
				if ( ofp != temp_ofp ) { // Different faces
					HalfEdgePair hep(ep,temp_ep,fp,temp_fp);

		// Check if plane formed by the two half-edges will be parallel
		// to the plane formed by the centroid's of the two faces and
		// the overall centroid calculated above. Also check for planarity
		// If planarity is less than a specified value, discard the pair
				Vector3d fpcen, temp_fpcen;
				Vector3d n; // Normal to above mentioned plane
				double cosangle = cos(5.0*M_PI/180.0); // Tolerance for parallel planes check
				double min_planarity = cosangle;
				fpcen = ofp->geomCentroid(); temp_fpcen = temp_ofp->geomCentroid();
				n = normalized( (fpcen-cen_cen)%(temp_fpcen-cen_cen) );

				if ( (hep.planarity > min_planarity) && (Abs(n*hep.normal) < cosangle) ) {
			// Add this half-edge pair to the array
					heparray.push_back(hep);
				}
			}
			++epfirst; ++fpfirst;
		}
	}

	cout << "Possible connections" << endl;
	for (int i=0; i < (int)heparray.size(); ++i) {
		heparray[i].print();
	}

		// Sort heparray according to distance. Use STL stable_sort algorithm
	stable_sort(heparray.begin(), heparray.end(), greater_than);

	cout << "Possible connections: reverse sorted" << endl;
	for (int i=0; i < (int)heparray.size(); ++i) {
		heparray[i].print();
	}

	cout << "Making Connections" << endl;
		// Go through heparray and start making connections. Once a connection is
		// made, remove all other HalfEdgePairs which contain already connected edges
		// NOTE: Traversal is from the end of the array.
	HalfEdgePair hep;
	while ( !heparray.empty() ) {
		hep = heparray.back(); heparray.pop_back();
		hep.print();
			// Make connection

		connectEdges( obj, 
			hep.ep1, (hep.ep1)->getOtherFacePointer(hep.fp1),
			hep.ep2, (hep.ep2)->getOtherFacePointer(hep.fp2) );

			// Remove all HalfEdgePairs from heparray which contain any of the
			// two edges in hep
		HalfEdgePair::last_connected1 = hep.ep1;
		HalfEdgePair::last_connected2 = hep.ep2;
		heparray.erase(remove_if(heparray.begin(), heparray.end(), contains_connected),heparray.end());
	}
}

void multiConnectFaces(DLFLObjectPtr obj, DLFLFacePtrArray fp) {
		// Connect multiple faces. Connects closest edges between each pair of faces

		// Find centroids of the faces
	int num_faces = fp.size();
	Vector3dArray cen;
	Vector3d cen_cen;

	cen.resize(num_faces);
	for (int i=0; i < (int)num_faces; ++i) {
		cen[i] = fp[i]->geomCentroid();
		cen_cen += cen[i];
	}
	cen_cen /= num_faces;

	DLFLEdgePtrList cedges; // Use linked list, not array, to simplify erase operations
	DLFLFacePtrList cfaces; // Faces for candidate edges (we need half-edges to connect)

		//--- NOTE ---//
		// In the list of faces, the other face for the edge is stored, instead of the
		// face which defines the half-edge to be connected. This is because once two half-edges
		// are connected, that face pointer might no longer be valid, but the other side will
		// still be valid. This assumes that both sides of the edge are not the same face

	for (int i=0; i < num_faces; ++i) {
		DLFLEdgePtrArray eparray;
		fp[i]->getEdges(eparray);

		DLFLEdgePtr ep;

			// Add all edges to the cedges list
		for (int j=0; j < (int)eparray.size(); ++j) {
			ep = eparray[j];
			cedges.push_back(ep);
			cfaces.push_back(ep->getOtherFacePointer(fp[i])); // See NOTE above
		}
	}

		// cedges now contains all the candidate edges

		/*
	Find all possible connections among the candidate edges
		Add them to an array of HalfEdgePairs to sort and make
		connections later. Valid connections are all those between
		half-edges in different faces
		*/

		HalfEdgePairArray heparray;
	DLFLEdgePtr eptr;
	DLFLFacePtr fptr;
	DLFLFacePtrList::iterator fpfirst;
	DLFLEdgePtrList::iterator epfirst, eplast;
	double cosangle = cos(5.0*M_PI/180.0); // Tolerance for parallel planes check
	double min_planarity = cosangle;
	while ( !cedges.empty() ) {
		eptr = cedges.front(); cedges.pop_front();
		fptr = cfaces.front(); cfaces.pop_front();

			// Go through remaining edges. Find edge which is closest
			// but is not in the same face as ep
		epfirst = cedges.begin(); eplast = cedges.end();
		fpfirst = cfaces.begin();
		DLFLEdgePtr temp_ep;
		DLFLFacePtr temp_fp, ofp, temp_ofp;
		while ( epfirst != eplast ) {
			temp_ep = (*epfirst); temp_fp = (*fpfirst);
			ofp = eptr->getOtherFacePointer(fptr);
			temp_ofp = temp_ep->getOtherFacePointer(temp_fp);
			if ( ofp != temp_ofp ) { // Different faces
				HalfEdgePair hep(eptr,temp_ep,fptr,temp_fp);
		// Check if plane formed by the two half-edges will be parallel
		// to the plane formed by the centroid's of the two faces and
		// the overall centroid calculated above. Also check for planarity
		// If planarity is less than a specified value, discard the pair
			Vector3d fpcen, temp_fpcen;
			Vector3d n; // Normal to above mentioned plane
			fpcen = ofp->geomCentroid(); temp_fpcen = temp_ofp->geomCentroid();
			n = normalized( (fpcen-cen_cen)%(temp_fpcen-cen_cen) );

			if ( isNonZero(normsqr(n)) &&
				(hep.planarity > min_planarity) &&
			(Abs(n*hep.normal) < cosangle) ) {
			// Add this half-edge pair to the array
				heparray.push_back(hep);
			} else {
			// For debug
			}
		}
		++epfirst; ++fpfirst;
	}
}

		// Sort heparray according to distance. Use STL stable_sort algorithm
stable_sort(heparray.begin(), heparray.end(), greater_than);

		// Before inserting the new edges, keep count of old edges.
		// Newly inserted edges will then be checked for redundant ones
		// and cleaned up if necessary
int num_old_edges = obj->num_edges();

		// Go through heparray and start making connections. Once a connection is
		// made, remove all other HalfEdgePairs which contain already connected edges
		// NOTE: Traversal is from the end of the array.
HalfEdgePair hep;
while ( !heparray.empty() ) {
	hep = heparray.back(); heparray.pop_back();
			// Make connection

	connectEdges(obj,
		hep.ep1,(hep.ep1)->getOtherFacePointer(hep.fp1),
		hep.ep2,(hep.ep2)->getOtherFacePointer(hep.fp2));

			// Remove all HalfEdgePairs from heparray which contain any of the
			// two edges in hep
	HalfEdgePair::last_connected1 = hep.ep1;
	HalfEdgePair::last_connected2 = hep.ep2;
	heparray.erase(remove_if(heparray.begin(), heparray.end(), contains_connected),heparray.end());
}

		// Go through newly inserted edges and cleanup ones which are redundant
DLFLEdgePtrList::iterator el_first, el_last;
DLFLEdgePtr newep;
DLFLEdgePtrList newedges;

el_first = obj->beginEdge(); el_last = obj->endEdge();
advance(el_first,num_old_edges);
while ( el_first != el_last ) {
	newep = (*el_first); ++el_first;
	newedges.push_back(newep);
}
edgeCleanup( obj, newedges);
}

void multiConnectFaces(DLFLObjectPtr obj, DLFLFacePtrArray fparray, double scale_factor,
double extrude_dist, bool use_max_offsets) {
		// Connect multiple faces using convex hull method
		// scale specifies scale factor for vertices before convex hull is created
		// extrude specified distance faces should be extruded before convex hull is created

		// First get vertices from all selected faces.
		// If two vertices are geometrically concident, only one of them is used for convex hull
	Vector3dArray vertices;
	Vector3dArray face_vertices;
	Vector3d p1,p2,facenormal,extrudevec;
	DLFLFacePtr fp;
	int num_sel_faces = fparray.size();
	DoubleArray max_extrude_distances;

	max_extrude_distances.resize(num_sel_faces,extrude_dist);

	if ( use_max_offsets ) 
		findMaxOffsets(fparray,max_extrude_distances);

	for (int i=0; i < num_sel_faces; ++i) {
		fp = fparray[i];
		fp->getVertexCoords(face_vertices);
		facenormal = fp->computeNormal();
		extrudevec = max_extrude_distances[i] * facenormal;

			// Add points from face_vertices to vertices checking for coincident vertices
			// maxdist is the square of max distance between 2 points for them to be
			// considered coincident.
			// Also extrude the points if extrude_dist is non-zero
		double maxdist = 1.0e-4;
		for (int j=0; j < (int)face_vertices.size(); ++j) {
			p1 = face_vertices[j];
			bool addp1 = true;
			for (int k=0; k < (int)vertices.size(); ++k) {
				p2 = vertices[k];
				if ( normsqr(p1-p2) < maxdist ) {
					addp1 = false;
					break;
				}
			}
			if ( addp1 == true ) {
				if ( isNonZero(extrude_dist) ) p1 += extrudevec;
				vertices.push_back(p1);
			}
		}
	}

		// Scale the points for convex hull with centroid as origin using given scale factor
		// Scaling is done only if scale_factor is non-zero and not equal to 1
	scale_factor = Abs(scale_factor);
	if ( isNonZero(scale_factor) && isNonZero(scale_factor-1.0) )
		scale(vertices,scale_factor);

		// Find convex hull of the transformed points
	DLFLConvexHull convexhull;
	convexhull.createHull(vertices);
		// Do edge cleanup on convex hull to remove redundant edges
	edgeCleanup(&convexhull);

		// If convex hull is interior to the original object, it has to be reversed before
		// making further connections. This can be tested by checking normals of original faces
		// If the normals point towards the centroid of the convex hull, no reversal is needed.
		// Only one face needs to be checked
	Vector3d v1 = fparray[0]->normalCentroid();
	Vector3d v2 = fparray[0]->geomCentroid() - centroid(vertices); normalize(v2);
	if ( v1*v2 > 0.0 ) convexhull.reverse();

		// Go through selected faces and faces of convex hull and find matching ones
		// to make connections. A match is when a selected face and a face in the convex hull
		// are parallel to each other AND are facing each other.
	DLFLFacePtrArray chfparray, fp1array, fp2array;
	DLFLFacePtr sfp, chfp;
	Vector3d sfpn, chfpn;

	fp1array.reserve(num_sel_faces); fp2array.reserve(num_sel_faces);

	convexhull.getFaces(chfparray);
	int num_ch_faces = chfparray.size();

	for (int i=0; i < num_ch_faces; ++i) {
		chfp = chfparray[i];
		chfpn = chfp->computeNormal();
		for (int j=0; j < num_sel_faces; ++j) {
			sfp = fparray[j];
			sfpn = sfp->computeNormal();
	// Dot product should be -1
			if ( !isNonZero(1.0 + chfpn*sfpn) ) {
		// Matching faces found
				fp1array.push_back(sfp); fp2array.push_back(chfp);
				break;
			}
		}
	}

		// Before connecting the faces, combine the convexhull with this object
		// to get a single DLFLObject
	obj->splice(convexhull);

		// Go through the lists of matching faces and make the connections
		// Closest vertex connection method is used
	for (int i=0; i < (int)fp1array.size(); ++i) {
		connectFaces(obj,fp1array[i],fp2array[i]);
	}

}

void multiConnectFaces(DLFLObjectPtr obj, DLFLFacePtrArray fparray, double min_factor, bool make_connections) {
		// Connect multiple faces using convex hull method. min_factor determines
		// search stop criterion for finding suitable convex hull.

		// If difference in adjustment factor between 2 successive iterations
		// is less than min_factor and we don't have any points inside the convex
		// hull in the current iteration we stop. Adjustment factor will never go below
		// the min_factor.

		// Search for the convex hull which is the smallest one which contains all the
		// original points. We do a binary search starting from half the distance from
		// centroid of each face to the centroid of all the face-centroids and continuing
		// till we reach the stop criterion described above.

	vector<Vector3dArray> vertices; // 2D Array of vertices in their original positions
	Vector3dArray chvertices; // Translated/extruded vertices for computing convex hull
	Vector3dArray centroids; // Overall centroid
	Vector3d global_centroid; // Centroid of each face
	Vector3dArray adj_vect; // Normalized vectors from centroid of each face to global centroid
	DoubleArray adj_dist; // Distance from centroid of each face to global centroid
	DLFLFacePtr fp;
	int num_sel_faces = fparray.size();

	centroids.resize(num_sel_faces);
	vertices.resize(num_sel_faces);
	for (int i=0; i < num_sel_faces; ++i) {
		fp = fparray[i];
		fp->getVertexCoords(vertices[i]); // Get vertices of face
		centroids[i] = fp->geomCentroid(); // Find centroid of face
		global_centroid += centroids[i];
		chvertices.insert(chvertices.end(),vertices[i].begin(),vertices[i].end());
	}

		// If convex hull of initial vertices excludes any point, we don't proceed any further
	DLFLConvexHull convexhull;
	if ( convexhull.createHull(chvertices) == false ) {
		cout << "Initial convex hull excludes atleast one vertex!" << endl;
		cout << "Multi-connect faces aborted." << endl;
		return;
	}

	global_centroid /= num_sel_faces;

	adj_vect.resize(num_sel_faces);
	adj_dist.resize(num_sel_faces);
	for (int i=0; i < num_sel_faces; ++i) {
		adj_vect[i] = global_centroid - centroids[i];
		adj_dist[i] = normalize(adj_vect[i]);
	}

	bool converged = false;
	bool ptinside = false; // Was any point inside convex hull in current iteration?
	double efactor = 0.5; // Amount to move in terms of original distance
	double efactor_prev = 0.0; // efactor value in previous iteration
	double efactor_diff; // Change in efactor from this iteration to next iteration
	int max_iters = 10; // Max. no. of iterations
	int num_iters = 0;
	while ( !converged ) {
			// Go through original vertices face by face and move them by appropriate
			// distances to get new vertices for convex hull computation
		chvertices.clear();
		for (int i=0; i < num_sel_faces; ++i) {
			Vector3d dvec = efactor * adj_dist[i] * adj_vect[i]; // Adjustment vector
			for (int j=0; j < (int)vertices[i].size(); ++j) {
				Vector3d p = vertices[i][j] + dvec;
				chvertices.push_back(p);
			}
		}

			// We now have the vertices to compute the convex hull
		ptinside = !convexhull.createHull(chvertices);

		efactor_diff = 0.5 * Abs(efactor-efactor_prev);
		efactor_prev = efactor;

			// If difference in efactor is less than min_factor
			// AND we are not excluding any points in this iteration we stop iterating.
			// Otherwise clamp efactor_diff to min_factor
		if ( efactor_diff < min_factor ) {
			if ( ptinside == false ) converged = true;
			else efactor_diff = min_factor;
		}

		if ( ptinside ) {
	// Atleast 1 vertex was inside convex hull in this iteration
	// So move points away from global centroid. i.e. reduce efactor
			efactor = efactor - efactor_diff;
		} else {
	// All points are still outside convex hull in this iteration
	// So move points closer to global centroid. i.e. increase efactor
			efactor = efactor + efactor_diff;
		}
		num_iters++;
		if ( num_iters > max_iters ) break;
	}

		// If we haven't converged within max. no. of iterations, print out error message
		// and return.
	if ( converged == false ) {
		cout << "Could not find suitable convex hull within " << max_iters << " iterations." << endl;
		cout << "Multi-connext faces aborted." << endl;
		return;
	}

		// We have converged to the convex hull closest to the boundary convex hull
		// given the constraints.

		// Do edge cleanup on convex hull to remove redundant edges
	edgeCleanup(&convexhull);

		// If convex hull is interior to the original object, it has to be reversed before
		// making further connections. This can be tested by checking normals of original faces
		// If the normals point towards the centroid of the convex hull, no reversal is needed.
		// Only one face needs to be checked
	if ( fparray[0]->normalCentroid() * adj_vect[0] > 0.0 ) convexhull.reverse();

		// Go through selected faces and faces of convex hull and find matching ones
		// to make connections. A match is when a selected face and a face in the convex hull
		// are parallel to each other AND are facing each other.
	DLFLFacePtrArray chfparray, fp1array, fp2array;
	DLFLFacePtr sfp, chfp;
	Vector3d sfpn, chfpn;

	fp1array.reserve(num_sel_faces); fp2array.reserve(num_sel_faces);

	convexhull.getFaces(chfparray);
	int num_ch_faces = chfparray.size();

	for (int i=0; i < num_ch_faces; ++i) {
		chfp = chfparray[i];
		chfpn = chfp->computeNormal();
		for (int j=0; j < num_sel_faces; ++j) {
			sfp = fparray[j];
			sfpn = sfp->computeNormal();
	// Dot product should be -1
			if ( Abs(1.0 + chfpn*sfpn) < 1.0e-5 ) {
		// Matching faces found
				fp1array.push_back(sfp); fp2array.push_back(chfp);
				break;
			}
		}
	}

		// Before connecting the faces, combine the convexhull with this object
		// to get a single DLFLObject
	obj->splice(convexhull);

		// Go through the lists of matching faces and make the connections
		// Closest vertex connection method is used
	if ( make_connections )
	for (int i=0; i < (int)fp1array.size(); ++i) {
		connectFaces(obj,fp1array[i],fp2array[i]);
	}
}


	// Define

void findMaxOffsets( DLFLFacePtrArray fparray, DoubleArray& maxoffsets) {
		/*
	Find the maximum distance by which each of the given
		faces can be offset without any of the points going inside
		the convex hull of all the points. The distance will be
		along the normal to each face.
		*/

		// First find the planes formed when each face is extruded along
		// its normal. The end plane is not included. All faces are assumed
		// to be planar, i.e. the average normal will be the same as the
		// normal at each corner.
		// Each of the planes can thus be defined as a (point,normal) pair
		// where the mid-point of each edge will be the point and the normal
		// is computed by the cross-product of the edge vector and face normal
		// The direction of the edge vector and hence the plane normal is not
		// important since we are only going to calculate distance from the
		// plane

		// Each face will have a variable number of Planes
		int numfaces = fparray.size();
	PlaneArrayArray allfaceplanes;
	allfaceplanes.resize(numfaces);

	DLFLEdgePtrArray edges;
	Vector3d facenormal;
	for (int i=0; i < numfaces; ++i) {
		int numedges;
		PlaneArray faceplanes;

		fparray[i]->getEdges(edges); facenormal = fparray[i]->computeNormal();
		numedges = edges.size();

		faceplanes.resize(numedges);

		Vector3d origin, edgevec, normal;
		for (int j=0; j < numedges; ++j) {
			origin = edges[j]->getMidPoint();
			edgevec = edges[j]->getEdgeVector(); normalize(edgevec);
			normal = edgevec % facenormal; normalize(normal);
			faceplanes[j].origin = origin;
			faceplanes[j].normal = normal;
		}
		allfaceplanes[i] = faceplanes;
	}

		// Now we have computed Planes for all faces
		// Go through each face, find distance of each point in face
		// from Planes of other faces. Find the minimum of these
		// distances
	maxoffsets.clear();
	maxoffsets.resize(numfaces);
	Vector3d facepoint;
	for (int i=0; i < numfaces; ++i) {
		facenormal = fparray[i]->normal; // Normal would have been computed in previous loop

		double mindist = 1.0e3;
		for (int j=0; j < numfaces; ++j) {
			if ( i != j ) {
				PlaneArray faceplanes = allfaceplanes[j];
				Vector3dArray facepoints;
				fparray[i]->getVertexCoords(facepoints);
				for (int k=0; k < (int)faceplanes.size(); ++k) {
					Plane plane = faceplanes[k];
					for (int m=0; m < (int)facepoints.size(); ++m) {
						facepoint = facepoints[m];
						double t = 0.0;

				// Find intersection of ray (centroid,facenormal) with plane
						if ( plane.intersect(facepoint,facenormal,t) ) {
							if ( Abs(t) < Abs(mindist) ) mindist = t;
						}
					}
				}
			}
		}
		maxoffsets[i] = mindist;
	}
}

void multiConnectMidpoints( DLFLObjectPtr obj ) {
		// Subdivide each edge. Connect midpoints to form new faces.
		// Connect corners in each face which share the same mid-point.

		// First part is similar to Simplest Subdivision scheme,
		// except that the old edges are not deleted.

		// Keep track of number of old vertices before edges are subdivided
	int num_old_verts = obj->num_vertices();

		// Sub-divide all edges. Make sure type is set.
	subdivideNonLoopingEdges(obj,2,true);

		// Go through vertex list. For all old vertices (before edge subdivision)
		// go through the face-vertex list and insert edges between previous and
		// next corners to each face-vertex.

	DLFLVertexPtrList::iterator vl_first, vl_last;
	vl_first = obj->beginVertex(); vl_last = obj->endVertex();

	DLFLFaceVertexPtrList fvplist;
	DLFLFaceVertexPtrList::iterator fvp_first,fvp_last;
	DLFLFaceVertexPtr fvp1, fvp2;
	DLFLMaterialPtr matl = (obj->firstFace())->material();
	int num_verts = 0;
	while ( vl_first != vl_last && num_verts < num_old_verts ) {
		fvplist = (*vl_first)->getFaceVertexList(); ++vl_first; ++num_verts;  

		fvp_first = fvplist.begin(); fvp_last = fvplist.end();
		while ( fvp_first != fvp_last ) {
			fvp2 = fvp1 = (*fvp_first); ++fvp_first;

	// Insert an edge between previous and next corners
	// Adjust for self-loops. If there is a self loop, go one more step
	// in the same direction
			if ( fvp1->getVertexPtr() == fvp1->next()->getVertexPtr() ) fvp1 = (fvp1->next())->next();
			else fvp1 = fvp1->next();
			if ( fvp2->getVertexPtr() == fvp2->prev()->getVertexPtr() ) fvp2 = (fvp2->prev())->prev();
			else fvp2 = fvp2->prev();

	// If self loops are there, we could end up inserting an edge between
	// two vertices twice. If that happens, second insertion will be
	// between two corners which are adjacent to each other.
	// Check for that and do insertion only if the corners are not adjacent
			if ( fvp1->next() != fvp2 && fvp1->prev() != fvp2 )
				insertEdge(obj,fvp1,fvp2,false,matl);
		}
	}

		// Go through the new vertices (midpoints). Find corners which refer to these
		// midpoints which are in the newly created middle faces. This can be found
		// by checking if next and previous vertices are also new midpoints.
	vl_first = obj->beginVertex(); vl_last = obj->endVertex();
	advance(vl_first,num_old_verts);
	DLFLFaceVertexPtr fvp,fvplink[2]; // Corners which are to be connected for each midpoint
	int corner_index; // Index into the above array
	while ( vl_first != vl_last ) {
		fvplist = (*vl_first)->getFaceVertexList(); ++vl_first;

		fvp_first = fvplist.begin(); fvp_last = fvplist.end();
		corner_index = 0;
		while ( fvp_first != fvp_last && corner_index < 2 ) {
			fvp = (*fvp_first); ++fvp_first;

	// Look at previous and next vertices. If both are new, then
	// pick this one and store in array
			fvp1 = fvp->prev(); fvp2 = fvp->next();
			if ( fvp1->getVertexPtr()->getType() == VTNewEdgePoint &&
				fvp2->getVertexPtr()->getType() == VTNewEdgePoint )
				fvplink[corner_index++] = fvp;
		}

			// Insert an edge between the two corners picked above
		insertEdge(obj,fvplink[0],fvplink[1],false);
	}

		// Reset type of all Vertices, face-vertices and edges
		// so we don't have any conflicts with subsequent operations
		// We only need to do this for the new vertices as above
	vl_first = obj->beginVertex(); vl_last = obj->endVertex();
	advance(vl_first,num_old_verts);
	while ( vl_first != vl_last ) {
		(*vl_first)->resetTypeDeep(); ++vl_first;
	}
}

void multiConnectCrust( DLFLObjectPtr obj, double scale_factor) {
		// Scale surface by given scale factor to form crust.
		// Then subdivide outer faces using zero-length extrusions
		// with same scale factor
		// Connect corresponding faces in inner shell and outer shell

		// Clamp scale factor to lie between 0.1 and 1.0.
		// Take absolute value
	scale_factor = Abs(scale_factor);
	if ( scale_factor < 0.1 ) scale_factor = 0.1;
	else if ( scale_factor > 1.0 ) scale_factor = 1.0;

		// First create scaled crust.
	createCrustWithScaling(obj,scale_factor);

		// The crust-modeling arrays would have been filled now.
		// Go through those arrays and punch holes after doing zero-length extrusion
		// crustfp1 contains old faces, crustfp2 contains new faces from inner shell
	int num_holes = crustfp1.size();
	DLFLFacePtr fp1, exfp1, fp2;
	DLFLFaceVertexPtr fvp1, fvp2;
	Vector3d vec; // For dummy direction vector
	for (int i=0; i < num_holes; ++i) {
		fp1 = crustfp1[i]; fp2 = crustfp2[i];

			// Do zero length extrusion with scaling for fp1
		exfp1 = extrudeFace(obj,fp1,0.0,vec,0.0,scale_factor);

			// Connect the end face of the extrusion and the inner face in the crust
		fvp1 = exfp1->firstVertex(); fvp2 = fp2->firstVertex();
		connectFaces(obj,fvp1,fvp2,1);
	}
}

void modifiedMultiConnectCrust(DLFLObjectPtr obj, double scale_factor) {
		// A modification of the multiConnectCrust method
		// After zero-length extrusion, old edges are trisected
		// and the new points are connected to the extruded faces.
		// The old support edges for the extrusion are then deleted.

		// Clamp scale factor to lie between 0.1 and 1.0.
		// Take absolute value
	scale_factor = Abs(scale_factor);
	if ( scale_factor < 0.1 ) scale_factor = 0.1;
	else if ( scale_factor > 1.0 ) scale_factor = 1.0;

	int num_old_edges = obj->num_edges();
	int num_old_verts = obj->num_vertices();
	int count = 0;

		// First create scaled crust.
	createCrustWithScaling(obj,scale_factor);

		// The crust-modeling arrays would have been filled now.
		// Do zero length extrusions of the faces in the outer shell
		// crustfp1 contains old faces, crustfp2 contains new faces from inner shell
		// Replace the crustfp1 array to contain the extruded end faces
		// Don't punch holes yet
	int num_holes = crustfp1.size();
	DLFLFacePtr fp1, exfp1, fp2;
	Vector3d vec; // For dummy direction vector
	for (int i=0; i < num_holes; ++i) {
		fp1 = crustfp1[i]; fp2 = crustfp2[i];

			// Do zero length extrusion with scaling for fp1
		exfp1 = extrudeFace(obj,fp1,0.0,vec,0.0,scale_factor);

			// Replace fp1 with exfp1 in crustfp1
		crustfp1[i] = exfp1;
	}

		// Trisect all the old edges (before creating crust and extruding)
		// Set type of new points
	DLFLEdgePtrList::iterator efirst, elast;
	DLFLEdgePtr ep;
	count = 0;
	efirst = obj->beginEdge(); elast = obj->endEdge();
	while ( count < num_old_edges ) {
		ep = (*efirst); ++efirst; ++count;
		trisectEdge(obj,ep,scale_factor,true,true);
	}

		// Go through all the old vertices (before crust, extrude and trisect)
		// For each vertex, go through the face-vertex list. For each face-vertex
		// in that list, connect the previous and next face-vertices with an edge
		// After doing that, find edges which do not connect to the points created
		// by trisection and delete them
	DLFLVertexPtrList::iterator vfirst, vlast;
	DLFLVertexPtr vp, ovp;
	DLFLFaceVertexPtrArray fvparray;
	DLFLFaceVertexPtr fvp;
	DLFLEdgePtrArray eparray;
	count = 0;
	vfirst = obj->beginVertex(); vlast = obj->endVertex();
	while ( count < num_old_verts ) {
		vp = (*vfirst); ++vfirst; ++count;
		vp->getFaceVertices(fvparray);
		for (int i=0; i < (int)fvparray.size(); ++i) {
			fvp = fvparray[i];
			insertEdgeCoFacial(obj,fvp->prev(),fvp->next());
		}
		vp->getEdges(eparray);
		for (int i=0; i < (int)eparray.size(); ++i) {
			ep = eparray[i];
			ovp = ep->getOtherVertexPointer(vp);
			if ( ovp->getType() != VTNewPoint ) deleteEdge(obj,ep);
			else ovp->resetType(); // Reset the type to allow proper recursive operation
		}
	}

		// Punch the holes now
	DLFLFaceVertexPtr fvp1, fvp2;
	for (int i=0; i < num_holes; ++i) {
		fp1 = crustfp1[i]; fp2 = crustfp2[i];
		fvp1 = fp1->firstVertex(); fvp2 = fp2->firstVertex();
		connectFaces(obj,fvp1,fvp2,1);
	}
}

void createSponge( DLFLObjectPtr obj, double thickness, double collapse_threshold_factor) {
		// New multi-connect with crust.
	bool fractional_thickness = false;
		// Make sure all Edge IDs and Face IDs are consecutive
	obj->makeEdgesUnique();
	obj->makeFacesUnique();

	DLFLEdgePtrArray eplist1, eplist2; // List of edges to be connected (for inner shell)
	DLFLFacePtrArray fplist1, fplist2; // List of faces to be connected (between outer and inner shell)
	int num_faces, num_edges;//, num_verts;
	int eistart, edgeindex, fistart;//, faceindex;
	int num_old_verts = obj->num_vertices();
	int num_old_faces = obj->num_faces();
	int num_old_edges = obj->num_edges();
	int count;

		// Reserve and create num_old_edges entries in the 2 temporary edge lists
	eplist1.resize(num_old_edges,NULL); eplist2.resize(num_old_edges,NULL);

		// Reserve and create num_old_faces entries in the 2 temporary face lists
	fplist1.resize(num_old_faces,NULL); fplist2.resize(num_old_faces,NULL);

		// Find starting edge ID to use as offset.
	eistart = (obj->firstEdge())->getID();
	fistart = (obj->firstFace())->getID();

	DLFLFacePtrList::iterator fl_first, fl_last;
	DLFLFacePtr fp, newfp1, newfp2;
	fl_first = obj->beginFace(); fl_last = obj->endFace();
	num_faces = 0;
	while ( fl_first != fl_last && num_faces < num_old_faces ) {
		fp = (*fl_first); ++fl_first; ++num_faces;

			// Create face for inner shell
		duplicateFacePlanarOffset(obj,fp,-thickness,0.0,thickness,fractional_thickness);

			// Get the two newly inserted faces
		DLFLFacePtrList::reverse_iterator temp = obj->rbeginFace();
		newfp1 = (*temp); ++temp; newfp2 = (*temp);

			// With respect to the outer shell, newfp1 faces inwards and newfp2 faces outwards
			// When creating the inner shell we want to reverse the surface
			// The edge connections will be made keeping this in mind

			// Set type of OUTER face so we can use the type to
			// determine which face to use for edge connects
		newfp2->setType(FTNew);

			// Store edges which are to be connected in the temporary array
			// using the adjusted edge ID of corresponding edge in original mesh.
			// newfp2 (the OUTER face) will be used for edge connections
		DLFLEdgePtrArray eparray1, eparray2;
		DLFLEdgePtr ep1, ep2;

		fp->getEdges(eparray1); newfp2->getEdges(eparray2);

			// Both fp and newfp2 MUST be of the same size. We wont check for that
		for (int i=0; i < (int)eparray1.size(); ++i) {
			ep1 = eparray1[i]; ep2 = eparray2[i];

			edgeindex = ep1->getID() - eistart;
			if ( eplist1[edgeindex] == NULL ) eplist1[edgeindex] = ep2;
			else                              eplist2[edgeindex] = ep2;
		}

			// Store faces which are to be connected in the temporary array
			// using the adjusted face ID of the face in the original mesh.
			// newfp1 (the INNER face) will be used for face connections
			// fplist1 will be used, since the other matching faces will only
			// be created later on and there is no possibility of conflict
		fplist1[fp->getID() - fistart] = newfp1;

			// Create the face for the outer shell
			// We use extrude now so we can easily insert the support edges later
		newfp2 = extrudeFacePlanarOffset(obj,fp,0.0,0.0,thickness,fractional_thickness);

			// newfp2 is used for make the face connection with the inner shell
			// Store newfp2 in fplist2
		fplist2[fp->getID() - fistart] = newfp2;
	}


		// Trisect all the old edges (before creating inner shell and extruding)
		// Set type of new points
	count = 0;
	DLFLEdgePtrList::iterator el_first, el_last;
	DLFLEdgePtr ep;
	el_first = obj->beginEdge(); el_last = obj->endEdge();
	if ( fractional_thickness ) {
		while ( count < num_old_edges ) {
			ep = (*el_first); ++el_first; ++count;
			trisectEdge(obj,ep,thickness*ep->length(),false,true);
		}
	} else {
		while ( count < num_old_edges ) {
			ep = (*el_first); ++el_first; ++count;
			trisectEdge(obj,ep,thickness,false,true);
		}
	}

		// Create the support edges on the outer surface
		// Go through all the old vertices (before crust, extrude and trisect)
		// For each vertex, go through the face-vertex list. For each face-vertex
		// in that list, connect the previous and next face-vertices with an edge
		// After doing that, find edges which do not connect to the points created
		// by trisection and delete them
	DLFLVertexPtrList::iterator vfirst, vlast;
	DLFLVertexPtr vp, ovp;
	DLFLFaceVertexPtrArray fvparray;
	DLFLFaceVertexPtr fvp;
	DLFLEdgePtrArray eparray;
	count = 0;
	vfirst = obj->beginVertex(); vlast = obj->endVertex();
	while ( count < num_old_verts ) {
		vp = (*vfirst); ++vfirst; ++count;
		vp->getFaceVertices(fvparray);
		for (int i=0; i < (int)fvparray.size(); ++i) {
			fvp = fvparray[i];
			insertEdgeCoFacial(obj,fvp->prev(),fvp->next());
		}
		vp->getEdges(eparray);
		for (int i=0; i < (int)eparray.size(); ++i) {
			ep = eparray[i];
			ovp = ep->getOtherVertexPointer(vp);
			if ( ovp->getType() != VTNewPoint ) deleteEdge(obj,ep);
			else ovp->resetType(); // Reset the type to allow proper recursive operation
		}
	}

		// Make the edge connections for the inner shell
		// Keep track of number of edges since the new edges will have to be collapsed
	num_edges = obj->num_edges(); // Don't use num_old_edges variable

		// Go through eplist1 and eplist2 and connect corresponding half-edges
		// The correct half-edge is determined by the type tag which was set previously
	DLFLFacePtr fp1, fp2, tfp1, tfp2;
	for (int i=0; i < num_old_edges; ++i) {
		if ( eplist1[i] != NULL && eplist2[i] != NULL ) {
	// Find the faces adjacent to the edges which are of type FTNew
	// These will be the inner faces
			eplist1[i]->getFacePointers(tfp1,tfp2);
			if ( tfp1->getType() == FTNew ) fp1 = tfp1;
			else if ( tfp2->getType() == FTNew ) fp1 = tfp2;
			else cout << i << " : " << "Face not found for half-edge!" << endl;

			eplist2[i]->getFacePointers(tfp1,tfp2);
			if ( tfp1->getType() == FTNew ) fp2 = tfp1;
			else if ( tfp2->getType() == FTNew ) fp2 = tfp2;
			else cout << i << " : " << "Face not found for half-edge!" << endl;

			connectEdges(obj,eplist1[i],fp1,eplist2[i],fp2);
		} else {
			cout << "NULL pointers found! i = " << i << " "
				<< eplist1[i] << " -- " << eplist2[i] << endl;
		}
	}

		// All edges which were inserted above should be collapsed if the length is
		// lesser than a threshold.
		// This threshold is computed from the threshold factor given above and the
		// sponge thickness. If threshold factor is 0, use a default value of 0.01
		// for threshold length. Threshold factor is a proportion of thickness.
	double collapse_threshold_length = 0.01;
	if ( collapse_threshold_factor > 0.001 )
		collapse_threshold_length = thickness * collapse_threshold_factor;

		// Edges which are part of self-intersections should also be collapsed
		// Since geometry is used to determine self-intersections, first make a list
		// of edges to be collapsed and then do the collapsing
	DLFLEdgePtrList colledges;
	el_first = obj->beginEdge(); el_last = obj->endEdge();
	advance(el_first,num_edges); // Advance to start of edges inserted above
	while ( el_first != el_last ) {
		ep = (*el_first); ++el_first;

		if ( ep->length() < collapse_threshold_length ) {
			ep->setType(ETCollapse);
			colledges.push_back(ep);
		} else {
	// Check if this edge is a part of a self-intersection
	// Find all edges at the two ends of this edge and check every pair
	// for intersection, excluding this edge itself
			DLFLVertexPtr vp1,vp2;
			ep->getVertexPointers(vp1,vp2);
			DLFLEdgePtrArray eparr1, eparr2;
			vp1->getEdges(eparr1); vp2->getEdges(eparr2);
			DLFLEdgePtr tep1, tep2;
			for (int i=0; i < (int)eparr1.size(); ++i) {
				tep1 = eparr1[i];
				if ( tep1 != ep ) {
					for (int j=0; j < (int)eparr2.size(); ++j) {
						tep2 = eparr2[j];
						if ( tep2 != ep && tep2 != tep1 ) {
		// Check if tep1 and tep2 intersect
							if ( checkIntersection(tep1,tep2) ) {
								ep->setType(ETCollapse);
								colledges.push_back(ep);
								i = eparr1.size(); // To make sure we break out of outer loop also
								break;
							}
						}
					}
				}
			}
		}
	}

		// Go through list of edges to be collapsed and collapse them
		// DON'T do cleanup when collapsing. The edges which form 2-gons or self-loops
		// will also be collapsed in this loop
	el_first = colledges.begin(); el_last = colledges.end();
	while ( el_first != el_last ) {
		ep = (*el_first); ++el_first;
		collapseEdge(obj,ep,false);
	}

		// Make the face connections between the outer shell and the inner shell
	DLFLFaceVertexPtr fvp1, fvp2;
	for (int i=0; i < num_old_faces; ++i) {
		fp1 = fplist1[i]; fp2 = fplist2[i];
		if ( fp1 != NULL && fp2 != NULL ) {
			fvp1 = fp1->firstVertex(); fvp2 = fp2->firstVertex();
			connectFaces(obj,fvp1,fvp2,1);
		}
	}

		// Cleanup any remaining 2-gons
		// cleanup2gons(obj);
}


  bool less_than(const HalfEdgePair& hep1, const HalfEdgePair& hep2)
  {
    if ( hep1.mpdist < hep2.mpdist ) return true;
    return false;
  }

  bool greater_than(const HalfEdgePair& hep1, const HalfEdgePair& hep2)
  {
    if ( hep1.mpdist > hep2.mpdist ) return true;
    return false;
  }

  bool contains_connected(const HalfEdgePair& hep)
  {
    if ( hep.ep1 == HalfEdgePair::last_connected1 ||
         hep.ep1 == HalfEdgePair::last_connected2 ||
         hep.ep2 == HalfEdgePair::last_connected1 ||
         hep.ep2 == HalfEdgePair::last_connected2 ) return true;
    return false;
  }

} // end namespace
