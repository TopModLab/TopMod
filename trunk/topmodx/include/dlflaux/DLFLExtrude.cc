
#include "DLFLExtrude.hh"
#include <DLFLCore.hh>
#include "DLFLConnect.hh"

namespace DLFL {

	DLFLFacePtr duplicateFace(DLFLObjectPtr obj, DLFLFacePtr fptr, double offset, double rot, double sf) {
		// Duplicate the given face, use face normal for direction of offset
		Vector3d dir = fptr->computeNormal();
		return duplicateFace(obj,fptr,dir,offset,rot,sf);
	}

	DLFLFacePtr duplicateFace(DLFLObjectPtr obj, DLFLFacePtr fptr, const Vector3d& dir,
														double offset, double rot, double sf) {
		// Duplicate given face, offsetting, scaling and rotating if necessary
		DLFLFacePtr endface = NULL;

		// First compute the coordinates of the vertices of the new points and store
		// them in an array
		DLFLFaceVertexPtr head;
		head = fptr->front();
		if ( head ) {
			Vector3d ndir = normalized(dir);
			Vector3dArray newverts;

			fptr->getVertexCoords(newverts);

			// Scale the new vertices about their centroid if scale factor is not 1.0 or 0.0
			sf = Abs(sf);
			if ( isNonZero(sf) && ( Abs(sf-1.0) > ZERO ) ) 
				scale(newverts,sf);
       
			// Rotate the new vertices if rotation is not 0.0
			if ( isNonZero(rot) ) 
				rotate(newverts,ndir,rot*M_PI/180.0);
       
			// Translate the new vertices by given amount along given direction
			if ( Abs(offset) > ZERO ) {
				translate(newverts,ndir,offset);
			}

			obj->createFace(newverts,fptr->material());

			// Get pointer to the first newly created face (second from last)
			DLFLFacePtrList::reverse_iterator rfirst = obj->rbeginFace();
			++rfirst; endface = (*rfirst);
		}
		return endface;
	}

	DLFLFacePtr duplicateFacePlanarOffset(DLFLObjectPtr obj, DLFLFacePtr fptr,
																				double offset, double rot, double thickness,
																				bool fractionalthickness) {
		// Duplicate the given face, use face normal for direction of offset
		// Offset the vertices in the plane of the face along
		// the angular bisectors by specified thickness.
		Vector3d dir = fptr->computeNormal();
		return duplicateFacePlanarOffset(obj,fptr,dir,offset,rot,thickness,fractionalthickness);
	}

	DLFLFacePtr duplicateFacePlanarOffset(DLFLObjectPtr obj, DLFLFacePtr fptr, const Vector3d& dir,
																				double offset, double rot, double thickness,
																				bool fractionalthickness) {
		// Duplicate given face, offsetting (along normal), and rotating if necessary
		// Offset the vertices in the plane of the face along
		// the angular bisectors by specified thickness.
		// Boolean flag indicates if thickness is absolute or fraction of edge length.
		DLFLFacePtr endface = NULL;

		// First compute the coordinates of the vertices of the new points and store
		// them in an array
		DLFLFaceVertexPtr head,fvp;
		head = fptr->front();
		if ( head ) {
			Vector3d ndir = normalized(dir);
			Vector3dArray newverts;

			fptr->getVertexCoords(newverts);

			// Offset the corners by thickness using the edge vectors to determine direction
			// If boolean flag is set thickness is assumed to be a fraction of edge length.
			// Traverse the face and the coordinate array simultaneously and adjust coordinates
			int i=0;//, numverts=newverts.size();
			Vector3d pvec, nvec, ovec;
			fvp = head;

			// Put if statement outside of loop for speed
			if ( fractionalthickness ) {
				// Thickness is relative to edge lengths, use fractions of edge vectors
				do {
					fvp->getEdgeVectors(pvec,nvec); // Edge vectors originating at fvp

					// If fvp is a winged corner we need to find the new coordinates differently
					if ( fvp->isWingedCorner() ) {
						// Find the next non-winged corner. It can be concave or convex
						DLFLFaceVertexPtr nwfvp = fvp->nextNonWingedCorner();
						if ( nwfvp == NULL ) {
							// This situation should not occur
							cout << "Something went wrong somewhere..." << endl;
							return NULL;
						}

						// Find the normal at this non-winged corner.
						// Normal will be adjusted for concave corners
						Vector3d nwfvpn = nwfvp->computeNormal();

						// Use this normal to find a vector starting at fvp
						// and pointing into the face and perpendicular to
						// the two edges coincident at fvp
						// This will be the offset vector direction
						ovec = nwfvpn % nvec; normalize(ovec);
						ovec *= thickness;
					} else {
						// Compute the offset vector using the edge vectors
						ovec = thickness*(pvec + nvec);

						// If this corner is a concave corner, flip the offset vector
						if ( fvp->isConcaveCorner() ) ovec = -ovec;
					}

					// Adjust the coordinates of the new vertex using the offset vector
					newverts[i] += ovec;
					fvp = fvp->next(); i++;
				} while ( fvp != head );
			} else {
				// Thichness is absolute, use the normalized edge vectors
				do {
					fvp->getEdgeVectors(pvec,nvec); // Edge vectors originating at fvp

					normalize(pvec); normalize(nvec);

					// If fvp is a winged corner we need to find the new coordinates differently
					if ( fvp->isWingedCorner() ) {
						// Find the next non-winged corner. It can be concave or convex
						DLFLFaceVertexPtr nwfvp = fvp->nextNonWingedCorner();
						if ( nwfvp == NULL ) {
							// This situation should not occur
							cout << "Something went wrong somewhere..." << endl;
							return NULL;
						}

						// Find the normal at this non-winged corner.
						// Normal will be adjusted for concave corners
						Vector3d nwfvpn = nwfvp->computeNormal();

						// Use this normal to find a vector starting at fvp
						// and pointing into the face and perpendicular to
						// the two edges coincident at fvp
						// This will be the offset vector direction
						ovec = nwfvpn % nvec;
						ovec *= thickness;
					} else {
						// Compute the offset vector using the edge vectors
						ovec = thickness*(pvec + nvec);

						// If this corner is a concave corner, flip the offset vector
						if ( fvp->isConcaveCorner() ) 
							ovec = -ovec;
					}

					// Adjust the coordinates of the new vertex using the offset vector
					newverts[i] += ovec;
					fvp = fvp->next(); i++;
				} while ( fvp != head );
			}

			// Rotate the new vertices if rotation is not 0.0
			if ( isNonZero(rot) ) 
				rotate(newverts,ndir,rot*M_PI/180.0);
       
			// Translate the new vertices by given amount along given direction
			if ( Abs(offset) > ZERO ) {
				translate(newverts,ndir,offset);
			}

			obj->createFace(newverts,fptr->material());

			// Get pointer to the first newly created face (second from last)
			DLFLFacePtrList::reverse_iterator rfirst = obj->rbeginFace();
			++rfirst; endface = (*rfirst);
		}
		return endface;
	}

	DLFLFacePtr extrudeFace(DLFLObjectPtr obj, DLFLFacePtr fptr, double d) {
		// Extrude the given face along its normal for a given distance
		Vector3d dir = fptr->computeNormal();
		normalize(dir);
		return extrudeFace(obj,fptr,d,dir,0.0,1.0);
	}

	DLFLFacePtr extrudeFace(DLFLObjectPtr obj, DLFLFacePtr fptr, double d, int num) {
		DLFLFacePtr exface = fptr;
		for (int i=0; i < num; ++i) {
			exface = extrudeFace(obj,exface,d);
		}
		return exface;
	}

	DLFLFacePtr extrudeFace(DLFLObjectPtr obj, DLFLFacePtr fptr, double d, double rot, double sf) {
		// Extrude the given face along its normal for a given distance
		// Rotate and scale the new face w.r.t. old face by given parameters
		Vector3d dir = fptr->computeNormal();
		normalize(dir);
		return extrudeFace(obj,fptr,d,dir,rot,sf);
	}

	DLFLFacePtr extrudeFace(DLFLObjectPtr obj, DLFLFacePtr fptr, double d, int num, double rot, double sf) {
		DLFLFacePtr exface = fptr;
		for (int i=0; i < num; ++i) {
			exface = extrudeFace(obj,exface,d,rot,sf);
		}
		return exface;
	}

	DLFLFacePtr extrudeFace(DLFLObjectPtr obj, DLFLFacePtr fptr, double d, const Vector3d& dir) {
		return extrudeFace(obj,fptr,d,dir,0.0,1.0);
	}

	DLFLFacePtr extrudeFace(DLFLObjectPtr obj, DLFLFacePtr fptr, double d, const Vector3d& dir, int num) {
		DLFLFacePtr exface = fptr;
		for (int i=0; i < num; ++i) {
			exface = extrudeFace(obj,exface,d,dir);
		}
		return exface;
	}
  
	DLFLFacePtr extrudeFace(DLFLObjectPtr obj, DLFLFacePtr fptr, double d, const Vector3d& dir, double rot, double sf) {
		DLFLFacePtr endface = duplicateFace(obj,fptr,dir,d,rot,sf);
		if ( endface != NULL ) {
			// Get the pointer to the second newly inserted face
			DLFLFacePtr nfp = obj->lastFace();
       
			// The last face will be the one facing the old face
			// Find the first face-vertices in the 2 faces to be connected
			DLFLFaceVertexPtr fvp1, fvp2;
			fvp1 = fptr->firstVertex(); fvp2 = nfp->firstVertex();
			connectFaces(obj,fvp1,fvp2);
		}
		return endface;
	}

	DLFLFacePtr extrudeFace(DLFLObjectPtr obj, DLFLFacePtr fptr, double d, const Vector3d& dir,
													int num, double rot, double sf) {
		DLFLFacePtr exface = fptr;
		for (int i=0; i < num; ++i) {
			exface = extrudeFace(obj,exface,d,dir,rot,sf);
		}
		return exface;
	}

	DLFLFacePtr extrudeFacePlanarOffset(DLFLObjectPtr obj, DLFLFacePtr fptr,
																			double d, double rot, double thickness,
																			bool fractionalthickness) {
		Vector3d dir;
		if ( isNonZero(d) || isNonZero(rot) ) {
			dir = fptr->computeNormal();
			normalize(dir);
		}
		return extrudeFacePlanarOffset(obj,fptr,d,dir,rot,thickness,fractionalthickness);
	}

	DLFLFacePtr extrudeFacePlanarOffset(DLFLObjectPtr obj, DLFLFacePtr fptr, double d, const Vector3d& dir,
																			double rot, double thickness,
																			bool fractionalthickness) {
		DLFLFacePtr endface = duplicateFacePlanarOffset(obj,fptr,dir,d,rot,thickness,fractionalthickness);
		if ( endface != NULL ) {
			// Get the pointer to the second newly inserted face
			DLFLFacePtr nfp = obj->lastFace();
       
			// The last face will be the one facing the old face
			// Find the first face-vertices in the 2 faces to be connected
			DLFLFaceVertexPtr fvp1, fvp2;
			fvp1 = fptr->firstVertex(); fvp2 = nfp->firstVertex();
			connectFaces(obj,fvp1,fvp2);
		}
		return endface;
	}

	DLFLFacePtr extrudeFaceDS(DLFLObjectPtr obj, DLFLFacePtr fptr, double d,
														double twist, double sf) {
		// Extrude the given face along its normal for a given distance
		Vector3d dir = fptr->computeNormal();
		normalize(dir);
		return extrudeFaceDS(obj,fptr,d,dir,twist,sf);
	}

	DLFLFacePtr extrudeFaceDS(DLFLObjectPtr obj, DLFLFacePtr fptr, double d,
														int num, double twist, double sf) {
		DLFLFacePtr exface = fptr;
		for (int i=0; i < num; ++i) {
			exface = extrudeFaceDS(obj,exface,d,twist,sf);
		}
		return exface;
	}

	DLFLFacePtr extrudeFaceDS(DLFLObjectPtr obj, DLFLFacePtr fptr, double d, const Vector3d& dir,
														double twist, double sf) {
		// Extrude the given face along the given direction for a given distance
		DLFLFacePtr endface = NULL;

		// First compute the coordinates of the vertices of the new points and store
		// them in an array
		DLFLFaceVertexPtr head;
		head = fptr->front();
		if ( head ) {
			Vector3d ndir = normalized(dir);
			Vector3dArray oldverts,newverts;

			fptr->getVertexCoords(oldverts);
			uint numverts = oldverts.size();           // No. of vertices in original face

			// New vertices will be computed using the twist factor
			newverts.resize(numverts,d*ndir);
			for (int i=0; i < numverts-1; ++i) {
				newverts[i] += (1.0-twist)*oldverts[i] + twist*oldverts[i+1];
			}
			newverts[numverts-1] += (1.0-twist)*oldverts[numverts-1] + twist*oldverts[0];

			double coef;
			Vector3d p;
			for (int i=0; i < numverts; ++i) {
				p.reset();
				for (int j=0; j < numverts; ++j) {
					if ( i == j ) 
						coef = 0.25 + 5.0/(4.0*numverts);
					else 
						coef = ( 3.0 + 2.0*cos(2.0*(i-j)*M_PI/numverts) ) / (4.0*numverts);
					p += coef*newverts[j];
				}
				oldverts[i] = p;
			}

			// Scale the new vertices about their centroid if scale factor is not 1.0 or 0.0
			sf = Abs(sf);
			if ( isNonZero(sf) && ( Abs(sf-1.0) > ZERO ) ) 
				scale(oldverts,sf);
       
			obj->createFace(oldverts,fptr->material());

			// Get pointer to the first newly created face (second from last)
			DLFLFacePtrList::reverse_iterator rfirst = obj->rbeginFace();
			++rfirst; endface = (*rfirst);

			// Get the pointer to the second newly inserted face
			DLFLFacePtr nfp = obj->lastFace();
       
			// The last face will be the one facing the old face
			// Find the first face-vertices in the 2 faces to be connected
			DLFLFaceVertexPtr fvp1, fvp2;
			fvp1 = fptr->firstVertex(); fvp2 = nfp->firstVertex();
			connectFaces(obj,fvp1,fvp2);
		}
		return endface;
	}

	DLFLFacePtr extrudeFaceDS(DLFLObjectPtr obj, DLFLFacePtr fptr, double d, const Vector3d& dir,
														int num, double twist, double sf) {
		DLFLFacePtr exface = fptr;
		for (int i=0; i < num; ++i) {
			exface = extrudeFaceDS(obj,exface,d,dir,twist,sf);
		}
		return exface;
	}

	DLFLFacePtr extrudeDualFace(DLFLObjectPtr obj, DLFLFacePtr fptr, double d, double rot, double sf, bool mesh) {
		// Extrude the given face along its normal for a given distance
		// Rotate and scale the new face w.r.t. old face by given parameters
		Vector3d dir = fptr->computeNormal();
		normalize(dir);
		return extrudeDualFace(obj,fptr,d,dir,rot,sf,mesh);
	}

	DLFLFacePtr extrudeDualFace(DLFLObjectPtr obj, DLFLFacePtr fptr, double d, int num, double rot, double sf, bool mesh) {
		DLFLFacePtr exface = fptr;
		for (int i=0; i < num; ++i) {
			exface = extrudeDualFace(obj,exface,d,rot,sf,mesh);
		}
		return exface;
	}

	DLFLFacePtr extrudeDualFace(DLFLObjectPtr obj, DLFLFacePtr fptr, double d, const Vector3d& dir, double rot, double sf, bool mesh) {
		// Extrude the given face along the given direction for a given distance
		DLFLFacePtr endface = NULL;

		// First compute the coordinates of the vertices of the new points and store
		// them in an array
		DLFLFaceVertexPtr head;
		head = fptr->front();
		if ( head ) {
			Vector3d ndir = normalized(dir);
			Vector3dArray newverts;

			// If mesh flag is true, edges in the original face will be deleted.
			// Store those edges in a temporary array
			DLFLEdgePtrArray ep_arr;
			if ( mesh ) fptr->getEdges(ep_arr);

			// New face will contain the midpoints of edges of old face, appropriately transformed
			DLFLFaceVertexPtr current = head;
			newverts.push_back(current->getEdgePtr()->getMidPoint() + d*ndir);
			current = current->next();
			while ( current != head ) {
				newverts.push_back(current->getEdgePtr()->getMidPoint() + d*ndir);
				current = current->next();
			}

			// Scale the new vertices about their centroid if scale factor is not 1.0 or 0.0
			sf = Abs(sf);
			if ( isNonZero(sf) && ( Abs(sf-1.0) > ZERO ) ) 
				scale(newverts,sf);
       
			// Rotate the new vertices if rotation is not 0.0
			if ( isNonZero(rot) ) 
				rotate(newverts,ndir,rot*M_PI/180.0);

			// Create the new face(s)
			obj->createFace(newverts,fptr->material());

			// Get pointers to the newly created faces
			DLFLFacePtrList::reverse_iterator rfirst = obj->rbeginFace();
			DLFLFacePtr nfp = (*rfirst);
			++rfirst;
			endface = (*rfirst);

			// The last face (nfp) will be the one facing the old face
			// Find the first face-vertices in the 2 faces to be connected
			DLFLFaceVertexPtr fvp1, fvp2;
			fvp1 = fptr->firstVertex(); fvp2 = nfp->firstVertex();
			dualConnectFaces(obj,fvp1,fvp2);

			// If the mesh flag is true delete the edges in the original face
			if ( mesh ) {
				DLFLEdgePtrArray::iterator el_first, el_last;
				DLFLEdgePtr ep;

				el_first = ep_arr.begin(); el_last = ep_arr.end();
				while ( el_first != el_last ) {
					ep = (*el_first); ++el_first;
					deleteEdge(obj, ep,true); 
				}
			}
		}
		return endface;
	}

	DLFLFacePtr extrudeDualFace(DLFLObjectPtr obj, DLFLFacePtr fptr, double d, const Vector3d& dir, int num, double rot, double sf, bool mesh) {
		DLFLFacePtr exface = fptr;
		for (int i=0; i < num; ++i) {
			exface = extrudeDualFace(obj,exface,d,dir,rot,sf,mesh);
		}
		return exface;
	}


	void stellateFace(DLFLObjectPtr obj, DLFLFacePtr fptr, double d) {
		// Stellate the given face along its normal for a given distance
		Vector3d dir = fptr->computeNormal();
		normalize(dir);
		stellateFace(obj,fptr,d,dir);
	}

	void stellateFace(DLFLObjectPtr obj, DLFLFacePtr fptr, double d, const Vector3d& dir) {
		// Stellation is like extrusion but creates a cone instead of a cylinder
		DLFLMaterialPtr matl = fptr->material();
		DLFLEdgePtr lastedge;
		DLFLFacePtr fptr1;
		DLFLFaceVertexPtr fvp1, fvp2;
		int numsides1;
		bool done;

		// Create the point sphere which will be the tip of the cone
		fvp1 = obj->createPointSphere(fptr->geomCentroid()+d*dir,matl);
	   
		fvp2 = fptr->firstVertex();

		// Insert an edge between the given corners
		// NOTE: Make sure point-sphere corner is specified first
		lastedge = insertEdgeNonCoFacial(obj,fvp1,fvp2,matl);

		done = false;
		while ( !done ) {
			fvp1 = lastedge->getFaceVertexPtr1();

			// Get face in which fvp1 is there. This will always be the face
			// in which the next edge has to be inserted because the point-sphere corner
			// is always specified first in the insertEdge call
			fptr1 = fvp1->getFacePtr(); numsides1 = fptr1->size();

			// If the face fptr1 has more than 3 sides insert another edge
			if ( numsides1 > 3 ) {
				// Skip the next corner to get the corner for next edge insertion
				fvp2 = fvp1->next(); fvp2 = fvp2->next();

				// Make sure point-sphere corner is specified first
				lastedge = insertEdgeCoFacial(obj,fvp1,fvp2,false);
			} else {
				// All edges have been inserted.
				done = true;
			}
		}
	}

	void doubleStellateFace(DLFLObjectPtr obj, DLFLFacePtr fptr, double d) {
		// Original version by Eric. Modified by Vinod.
		// Double stellation
		// Zero-length stellatation followed by another zero-length stellation
		// of the new faces. Midpoint is then moved by specified distance along normal.
		Vector3d dir = fptr->computeNormal();

		// Do zero-length stellation
		stellateFace(obj,fptr,0.0);

		// Get newly added vertex
		DLFLVertexPtr vp = obj->lastVertex();

		// Find faces adjacent to new vertex
		DLFLFacePtrArray newfaces;
		vp->getFaces(newfaces);

		// Do zero-length stellation on new faces
		for (int i=0; i < newfaces.size(); ++i) 
			stellateFace(obj,newfaces[i],0.0);

		// Adjust position of centroid of original face - created by first stellation
		vp->coords += dir * d;
	}
    
	//--- Additions by Eric ---//

	DLFLFacePtr extrudeFaceDodeca(DLFLObjectPtr obj, DLFLFacePtr fptr, double d, int num, double rot, double sf, bool hexagonalize) {
		// Dodecahedral extrusion
		DLFLFacePtr exface = fptr;
		DLFLEdgePtrArray ep_arr;

		double sf3 = pow(sf, 1.0/2.0);
		if ( hexagonalize ) {
			exface = extrudeFaceDodeca(obj, exface, d, rot, sf3);

			for (int i=0; i < num-1; ++i) {
				exface->getEdges(ep_arr);
				exface = extrudeHexTileFace(obj, exface, d, rot, sf3);
			}
		} else {
			for (int i=0; i < num; ++i) {
				exface = extrudeFaceDodeca(obj, exface,d, rot, sf3);
			}
		}
		return exface;
	}

	DLFLFacePtr extrudeHexTileFace(DLFLObjectPtr obj, DLFLFacePtr fptr, double d, double rot, double sf) {
		// Hexagonal extrusion for multiple dodecahedral extrusion.
		DLFLFacePtr exface = fptr;
		DLFLEdgePtrArray ep_arr;
		double ddiv3 = d / 3.0;
		double sf_2 = sqrt(sf);

		// extrude and delete the unneeded edges
		exface = extrudeDualFace(obj, exface, ddiv3, rot, sf_2, true);
		exface->getEdges(ep_arr);

		exface = extrudeFace(obj, exface, ddiv3, 0.0, sf_2);

		// delete more unneeded edges, the same way dual extrude does
		DLFLEdgePtrArray::iterator el_first, el_last;
		DLFLEdgePtr ep;
		el_first = ep_arr.begin(); el_last = ep_arr.end();
		while ( el_first != el_last ) {
			ep = (*el_first); ++el_first;
			deleteEdge(obj,ep,true);
		}
		return exface;
	}

	DLFLFacePtr extrudeFaceDodeca(DLFLObjectPtr obj, DLFLFacePtr fptr, double d, double rot, double sf) {
		// Dodecahedral extrusion
		// Sequence of extrude, dual extrude and extrude followed by deletion of lateral edges
		DLFLFacePtr exface = fptr;
		DLFLEdgePtrArray ep_arr;

		double ddiv3 = d / 3.0;
		double sf_3 = pow(sf, 1.0/3.0);

		exface = extrudeFace(obj,exface,ddiv3,0.0,sf_3);

		// extrude and delete the unneeded edges
		exface = extrudeDualFace(obj,exface, ddiv3, rot, sf_3, true);
		exface->getEdges(ep_arr);

		exface = extrudeFace(obj,exface, ddiv3, 0.0, sf_3);

		// delete more unneeded edges, the same way dual extrude does
		DLFLEdgePtrArray::iterator el_first, el_last;
		DLFLEdgePtr ep;
		el_first = ep_arr.begin(); el_last = ep_arr.end();
		while ( el_first != el_last ) {
			ep = (*el_first); ++el_first;
			deleteEdge(obj,ep,true);
		}
		return exface;
	}

	DLFLFacePtr extrudeFaceIcosa(DLFLObjectPtr obj, DLFLFacePtr fptr, double d, int num, double rot, double sf) {
		// Icosahedral extrusion
		DLFLFacePtr exface = fptr;

		double sf3 = pow(sf, 1.0/2.0);
		for (int i=0; i < num; ++i) {
			exface = extrudeFaceIcosa(obj,exface,d, rot,sf3);
		}
		return exface;
	}

	// is performed by extruding, dual extruding, then extruding again.
	// then delete the latitude edges
	DLFLFacePtr extrudeFaceIcosa(DLFLObjectPtr obj, DLFLFacePtr fptr, double d, double rot, double sf) {
		// Icosahedral extrusion
		DLFLFacePtr exface = fptr;
		DLFLFacePtr otherface;
		DLFLEdgePtrArray ep_arr;
		DLFLVertexPtr vp;

		double ddiv3 = d / 3.0;
		double stellate_amount;
		double center_to_vert;

		Vector3d center_sphere, center_sphere2;
		Vector3d center_normal;
		Vector3d first_face_centroid;

		center_sphere2 = center_sphere = exface->geomCentroid();
		center_normal = exface->computeNormal();
		center_sphere += center_normal * (d / 3.0);
		center_sphere2 += center_normal * (d / 2.0);

		// dual extrude, then regular extrude
		exface = extrudeDualFace(obj,exface, ddiv3, rot, sf, false);
		exface->getEdges(ep_arr);
		vp = obj->lastVertex();
		center_to_vert = normsqr(vp->coords - center_sphere);

		// now have the radius of the shape sphere

		exface = extrudeFace(obj, exface, ddiv3*2.0);

		// delete top edges from dual extrude
		DLFLEdgePtrArray::iterator el_first, el_last;
		DLFLEdgePtr ep;
		el_first = ep_arr.begin(); el_last = ep_arr.end();
		while ( el_first != el_last ) {
			ep = (*el_first); ++el_first;
			deleteEdge(obj,ep,true);
		}

		bool started = false;

		// stellate the top faces
		exface->getEdges(ep_arr);
		el_first = ep_arr.begin(); el_last = ep_arr.end();
		while ( el_first != el_last ) {
			ep = (*el_first); ++el_first;
			otherface = ep->getOtherFacePointer(exface);
			if ( started == false ) {
				first_face_centroid = otherface->geomCentroid();
				stellate_amount = center_to_vert - normsqr(first_face_centroid - center_sphere2);
				started = true;
			}

			// stellateFace(obj, otherface, 1/pow(sf, 1.0/3.0));
			stellateFace(obj, otherface, stellate_amount/2.0);
		}
		return exface;
	}

	//--- End additions by Eric ---//

	void extrudeFace(DLFLObjectPtr obj, DLFLFacePtr fptr) {
		// Extrude the given face using previously computed coordinates.
		DLFLFaceVertexPtr head;
		head = fptr->front();
		if ( head ) {
			Vector3dArray newverts;

			DLFLFaceVertexPtr current = head;
			newverts.push_back(current->getAuxCoords());
			current = current->next();
			while ( current != head ) {
				newverts.push_back(current->getAuxCoords());
				current = current->next();
			}
			obj->createFace(newverts,fptr->material());

			// Get the pointer to the second newly inserted face
			DLFLFacePtr nfp;
			DLFLFacePtrList::reverse_iterator rfirst = obj->rbeginFace();
			nfp = (*rfirst);

			// The last face will be the one facing the old face
			// Find the first face-vertices in the 2 faces to be connected
			DLFLFaceVertexPtr fvp1, fvp2;
			fvp1 = fptr->firstVertex(); fvp2 = nfp->firstVertex();
			connectFaces(obj,fvp1,fvp2);
		}
	}

	/* API
		 uint extrudeFace(DLFLObjectPtr obj, uint faceID, double d, int num, double rot, double sf ) {
		 DLFLFacePtr fp = extrudeFace( obj, obj->findFace(faceID), d, num, rot, sf );
		 return fp->getID();
		 }

		 uint extrudeFaceDS(DLFLObjectPtr obj, uint faceID, double d, int num, double twist, double sf ) { }

		 uint extrudeDualFace(DLFLObjectPtr obj, uint faceID, double d, int num, double rot, double sf, bool mesh ) { }

		 uint extrudeFaceDodeca(DLFLObjectPtr obj, uint faceID, double d, int num, double rot, double sf, bool hexagonalize ) { }

		 uint extrudeFaceIcosa(DLFLObjectPtr obj, uint faceID, double d, int num, double rot, double sf ) { }

		 void stellateFace(DLFLObjectPtr obj, uint faceID, double d) { }

		 void doubleStellateFace(DLFLObjectPtr obj, uint faceID, double d) { }
		 end API */

} // end namespace DLFL
