
#include "DLFLExtrude.hh"
#include <DLFLCore.hh>
#include "DLFLConnect.hh"

namespace DLFL {

	DLFLFacePtr duplicateFace(DLFLObjectPtr obj, DLFLFacePtr fptr, double offset, double rot, double sf) {
		// Duplicate the given face, use face normal for direction of offset
		Vector3d dir = fptr->computeNormal();
		return duplicateFace(obj,fptr,dir,offset,rot,sf);
	}

	DLFLFacePtr duplicateFace(DLFLObjectPtr obj, DLFLFacePtr fptr, const Vector3d& dir, double offset, double rot, double sf) {p
		// Duplicate given face, offsetting, scaling and rotating if necessary
		DLFLFacePtr endface = NULL;

		// First compute the coordinates of the vertices of the new points and store
		// them in an array
		DLFLFaceVertexPtr head;
		head = fptr->front();
		if ( head ) {
			Vector3d ndir = normalized(dir);p-p-0o9i
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

	DLFLFacePtr duplicateFacePlanarOffset(DLFLObjectPtr obj, DLFLFacePtr fptr, double offset, double rot, double thickness, bool fractionalthickness) {
		// Duplicate the given face, use face normal for direction of offset
		// Offset the vertices in the plane of the face along
		// the angular bisectors by specified thickness.
		Vector3d dir = fptr->computeNormal();
		return duplicateFacePlanarOffset(obj,fptr,dir,offset,rot,thickness,fractionalthickness);
	}

	DLFLFacePtr duplicateFacePlanarOffset(DLFLObjectPtr obj, DLFLFacePtr fptr, const Vector3d& dir, double offset, double rot, double thickness, bool fractionalthickness) {
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

	DLFLFacePtr extrudeFacesTogether(DLFLObjectPtr obj, DLFLFacePtrArray fparray, double d) {

		//need an array of old vertices, new vertices, and which ones are outside, and which ones need to be connected
		DLFLVertexArray old_verts;
		DLFLVertexArray new_verts;
		
		//also need to delete edges inside the regions that will be kept together
		
		
		//then do

		//first compute the normals for each selected face
		Vector3d dir = fptr->computeNormal();
		//then normalize it
		normalize(dir);
		// return extrudeFace(obj,fptr,d,dir,0.0,1.0);
		
		
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

	DLFLFacePtr extrudeFace(DLFLObjectPtr obj, DLFLFacePtr fptr, double d, const Vector3d& dir, int num, double rot, double sf) {
		DLFLFacePtr exface = fptr;
		for (int i=0; i < num; ++i) {
			exface = extrudeFace(obj,exface,d,dir,rot,sf);
		}
		return exface;
	}

	DLFLFacePtr extrudeFacePlanarOffset(DLFLObjectPtr obj, DLFLFacePtr fptr, double d, double rot, double thickness, bool fractionalthickness) {
		Vector3d dir;
		if ( isNonZero(d) || isNonZero(rot) ) {
			dir = fptr->computeNormal();
			normalize(dir);
		}
		return extrudeFacePlanarOffset(obj,fptr,d,dir,rot,thickness,fractionalthickness);
	}

	DLFLFacePtr extrudeFacePlanarOffset(DLFLObjectPtr obj, DLFLFacePtr fptr, double d, const Vector3d& dir, double rot, double thickness, bool fractionalthickness) {
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

	DLFLFacePtr extrudeFaceDS(DLFLObjectPtr obj, DLFLFacePtr fptr, double d, double twist, double sf) {
		// Extrude the given face along its normal for a given distance
		Vector3d dir = fptr->computeNormal();
		normalize(dir);
		return extrudeFaceDS(obj,fptr,d,dir,twist,sf);
	}

	DLFLFacePtr extrudeFaceDS(DLFLObjectPtr obj, DLFLFacePtr fptr, double d, int num, double twist, double sf) {
		DLFLFacePtr exface = fptr;
		for (int i=0; i < num; ++i) {
			exface = extrudeFaceDS(obj,exface,d,twist,sf);
		}
		return exface;
	}

	DLFLFacePtr extrudeFaceDS(DLFLObjectPtr obj, DLFLFacePtr fptr, double d, const Vector3d& dir, double twist, double sf) {
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

	DLFLFacePtr extrudeFaceDS(DLFLObjectPtr obj, DLFLFacePtr fptr, double d, const Vector3d& dir, int num, double twist, double sf) {
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
	//eric new
	DLFLFacePtr extrudeFaceDodeca(DLFLObjectPtr obj, DLFLFacePtr fptr, double angle, int num, double ex_dist1, double ex_dist2, double ex_dist3, bool hexagonalize){
		// Dodecahedral extrusion
		DLFLFacePtr exface = fptr;
		DLFLEdgePtrArray ep_arr;
		if ( hexagonalize ) {
			DLFLEdgePtrArray :: iterator el_first, el_last;
			DLFLEdgePtr ep;

			exface = extrudeFaceDodeca(obj, exface, angle, ex_dist1, ex_dist2, ex_dist3);

			for (int i=0; i < num-1; ++i) {
				exface->getEdges(ep_arr);
				exface = extrudeFaceDodeca(obj, exface, angle, ex_dist1, ex_dist2, ex_dist3);

				el_first = ep_arr.begin(); el_last = ep_arr.end();
				while ( el_first != el_last ) {
					ep = (*el_first); ++el_first;
					deleteEdge(obj,ep,true);
				}
			}
		}
		else {
			for (int i=0; i < num; ++i)
				exface = extrudeFaceDodeca(obj, exface, angle, ex_dist1, ex_dist2, ex_dist3);
		}
		return exface;
	}

	DLFLFacePtr extrudeDodeca_Symmetric(DLFLObjectPtr obj, DLFLFacePtr fptr, double d, double rot, double sf, int modfactor) { 
		// int num_faces;
// 		int num_old_faces;
// 		int face_counter;
// 		int symmetry_id;
// 		int count_counter;
// 		int current_max_symm_group;
// 		DLFLFacePtr fp;
// 		DLFLFacePtrList :: iterator fl_first, fl_last, fl_exfirst, fl_exlast;
// 		DLFLFacePtr retface;
// 		Vector3d dir;
// 
// 		int num_extrudefaces;
// 		int num_oldexfaces;
// 		int num_excounter;
// 
// 		//these are used for finding the average edge length
// 		double av_edge_length;
// 		Vector3dArray base_verts;
// 		int face_sides;
// 		Vector3d tempv;
// 		int i;
// 
// 		num_faces = obj->num_faces();
// 
// 		face_counter = 0;
// 
// 		int modulus=modfactor;
// 		int modoffset;
// 		symmetry_id = fptr->material()->groupid;
// 		current_max_symm_group = max_symm_group;
// 		//printf("max_sym_group = %d, current group is %d\n", max_symm_group, symmetry_id);
// 
// 		fl_first = obj->beginFace();
// 		fl_last = obj->endFace();
// 
// 		while(face_counter < num_faces && fl_first != fl_last) {
// 			fp = (*fl_first);
// 
// 			if( symmetry_id == fp->material()->groupid) {
// 				//group matches, extrude this face
// 				dir = fp->computeNormal();
// 				normalize(dir);
// 				//FIND AV. LENGTH OF BASE EDGE************************************
// 				av_edge_length = 0;
// 				face_sides = fp->numFaceVertexes();
// 				fp->getVertexCoords(base_verts);
// 
// 				for(i=0; i < face_sides; i++) {
// 					if(i == face_sides - 1)
// 						tempv = base_verts[0];
// 					else
// 						tempv = base_verts[i+1];
// 
// 					tempv -= base_verts[i];
// 
// 					//find the length of one side
// 					av_edge_length +=  norm(tempv);
// 				}
// 				av_edge_length /= (double) face_sides;
// 				//******************************************************************
// 
// 				num_oldexfaces = obj->num_faces();
// 				//DLFLFacePtr endface = duplicateFace(fp,dir,  av_edge_length * d,rot,sf);
// 				DLFLFacePtr endface = extrudeFaceDodeca(obj, fp, 45.0, 1, 1, 1, 1, false);
// 
// 				retface = endface;
// 				if ( false && endface != NULL ) {
// 					count_counter = 2;
// 					modoffset = count_counter % modulus + 2;
// 					endface->symm_group = current_max_symm_group + modoffset;
// 					MakeNewMaterial(endface, current_max_symm_group + modoffset);
// 					if(max_symm_group < endface->symm_group)
// 						max_symm_group = endface->symm_group;
// 
// 					count_counter++;
// 					modoffset = count_counter % modulus + 2;
// 					fp->symm_group = current_max_symm_group + modoffset;
// 					MakeNewMaterial(fp, current_max_symm_group + modoffset);
// 					if(max_symm_group < fp->symm_group)
// 						max_symm_group = fp->symm_group;
// 						/*
// 									// Get the pointer to the second newly inserted face
// 					DLFLFacePtr nfp = face_list.back();
// 
// 									// The last face will be the one facing the old face
// 									// Find the first face-vertices in the 2 faces to be connected
// 					DLFLFaceVertexPtr fvp1, fvp2;
// 					fvp1 = fp->firstVertex(); fvp2 = nfp->firstVertex();
// 					connectFaces(fvp1,fvp2);
// 						*/
// 					num_extrudefaces = obj->num_faces();
// 					fl_exfirst = obj->beginFace();
// 					fl_exlast = obj->endFace();
// 
// 					count_counter = 3;
// 						//count_counter += num_extrudefaces - num_oldexfaces;
// 					num_excounter = 0;
// 					while(num_excounter < num_extrudefaces && fl_exfirst != fl_exlast) {
// 						if(num_excounter >= num_oldexfaces) {
// 							fp = (*fl_exfirst);
// 								//use the modulus to determine new face group
// 							modoffset = count_counter % modulus + 2;
// 
// 							if( count_counter == 3) {
// 								fp->symm_group = current_max_symm_group + 1;
// 								MakeNewMaterial(fp, current_max_symm_group + 1);
// 							}
// 							else {
// 								fp->symm_group = current_max_symm_group + modoffset;
// 								MakeNewMaterial(fp, current_max_symm_group + modoffset);
// 							}
// 
// 							if(max_symm_group < fp->symm_group)
// 								max_symm_group = fp->symm_group;
// 								//fp->symm_group = (symmetry_id-1) * (num_extrudefaces - num_oldexfaces) + count_counter + 1;
// 								//printf("symmetry count is %d\n", count_counter);
// 							count_counter++;
// 						}
// 						fl_exfirst++;
// 						num_excounter++;
// 					}
// 				}
// 			}
// 			++fl_first;
// 			face_counter++;
// 		}
// 	/* MAGICKAL REFERENCE CODE
// 		num_faces = 0;
// 		num_old_faces = face_list.size();
// 		fl_first = face_list.begin(); fl_last = face_list.end();
// 
// 			//Stellate all the faces again
// 		while ( num_faces < num_old_faces )
// 		{
// 			fp = (*fl_first);
// 			++fl_first;
// 			stellateFace(fp, 0.0);
// 			++num_faces;
// 		}
// 	*/
// 		return retface;
	}

	DLFLFacePtr extrudeFaceDodeca(DLFLObjectPtr obj, DLFLFacePtr fptr, double angle, double ex_dist1, double ex_dist2, double ex_dist3) {
	     // Dodecahedral extrusion
	     // Sequence of extrude, dual extrude and extrude followed by deletion of lateral edges
	  DLFLFacePtr exface = fptr;
	  DLFLEdgePtrArray ep_arr;
	  DLFLVertexPtrList :: iterator vl_first, vl_last, vl_current, vl_ring_1, vl_ring_2, vl_ring_3;
	  DLFLVertexPtr vp;
	  int num_old_verts = obj->num_vertices();
	  Vector3d anormal = fptr->computeNormal();
	  int face_sides = fptr->numFaceVertexes();

	  //double ddiv3 = d / 3.0;
	  //double sf_3 = pow(sf, 1.0/3.0);

	  Vector3dArray base_verts;
	  DLFLFaceVertexPtr head;
	  head = fptr->front();
	  //create an array of the original face's vertex positions
	  if ( head ){
	     DLFLFaceVertexPtr current = head;
	     base_verts.push_back(current->getAuxCoords());
	     current = current->next();
	     while ( current != head ) {
	       base_verts.push_back(current->getAuxCoords());
	       current = current->next();
	     }
	  }

	  fptr->getVertexCoords(base_verts);
	  exface = extrudeFace(obj, exface, 1.0, 0.0,  1.0);
	     // extrude and delete the unneeded edges
	  exface = extrudeDualFace(obj, exface, 1.0, 0.0, 1.0, true);
	  exface->getEdges( ep_arr);
	  exface = extrudeFace(obj, exface, 1.0, 0.0, 1.0);
	     // delete more unneeded edges, the same way dual extrude does
	  DLFLEdgePtrArray :: iterator el_first, el_last;
	  DLFLEdgePtr ep;
	  el_first = ep_arr.begin(); el_last = ep_arr.end();
	  while ( el_first != el_last ) {
			ep = (*el_first); ++el_first;
			deleteEdge(obj,ep,true);
		}
	  //new code for repositioning vertices
	  Vector3d tempv;

	  int vert_count=0;
	  int num_verts = obj->num_vertices();
	  int i;

	  vl_first = obj->beginVertex();
	  vl_last = obj->endVertex();

	  while(vert_count < num_old_verts) {
	    vl_first++;
	    vert_count++;
	  }

	  vl_ring_1 = vl_first;
	  vl_ring_2 = vl_first;
	  vl_ring_3 = vl_first;

	  for(i=0; i < face_sides; i++) {
	    //vl_ring_1++;
	    vl_ring_2++;
	    vl_ring_3++;
	  }

	  for(i=0; i < face_sides; i++) {
	     //vl_ring_2++;
	     vl_ring_3++;
	  }
	
	  Vector3d enext, enext_old, eprev, faceprev, facenext, finaldir, tocenter;
	  Vector3d midpoint_tier2;
	  Vector3d f_1, f_2, f_3, tedgeprev, tedgenext;
	  DLFLVertexPtrList :: iterator vl_temp1, vl_temp2;
	  int count, j;
	  //double alpha = -.4915;
	  //double alpha = -(116.57-90.)*M_PI/180.;
	  //double alpha = -(angle-90.)*M_PI/180.;
	  double alpha = -(angle)*M_PI/180.;
	  double l1, l_old, l_base;

	  vl_current = vl_ring_1;
	  //compute first side
	  l_base = 0;
	  for(i=0; i < face_sides; i++) {
	    if(i == face_sides - 1)
	      enext = base_verts[0];
	    else
	      enext = base_verts[i+1];

	    enext -= base_verts[i];
	    //find the length of one side
	    l_base +=  norm(enext);
	    vl_current++;
	  }
	  l_base /= (double) face_sides;

	  vl_current = vl_ring_1;
	  for(i=0; i < face_sides; i++) {

	    if(i==0)
	      eprev = base_verts[face_sides - 1];
	    else
	      eprev = base_verts[i-1];

	    if(i == face_sides - 1)
	      enext = base_verts[0];
	    else
	      enext = base_verts[i+1];

	    eprev -= base_verts[i];
	    enext -= base_verts[i];
	    //find the length of one side
	    //l1 = (norm(eprev) + norm(enext))*0.5;
	    l1 = l_base;
	    //l1 *= (ex_dist1/cos(alpha));  //defines the height based on the dihedral angle
	    l1 *= ex_dist1; //changes size for first set of lines, ignores dihedral angle

	    normalize(eprev);
	    normalize(enext);

	    //facenext
	    tempv = enext % anormal;
	    normalize(tempv);
	    facenext = cos(alpha) * tempv  +  sin(alpha) * anormal;
	    normalize(facenext);

	    //faceprev
	    tempv = anormal % eprev;
	    normalize(tempv);
	    faceprev = cos(alpha) * tempv + sin(alpha) * anormal;
	    normalize(faceprev);

	    finaldir = faceprev % facenext;
	    normalize(finaldir);

	    vp = (*vl_current);
	    tempv = vp->getCoords();
	    tempv = base_verts[i] + l1*finaldir;

	    vp->setCoords(tempv);
	    vl_current++;
	  }
	
	  vl_current = vl_ring_2;

	  //compute first side
	  for(i=0; i < face_sides; i++) {
	    if(i == face_sides - 1)
	      enext = base_verts[0];
	    else
	      enext = base_verts[i+1];
	
	    enext -= base_verts[i];
	    //find the length of one side
	    l1 = norm(enext);
	    /*l1 = (l1*0.5)*(1./cos(M_PI*54./180.)+tan(M_PI*54./180.) ) * ex_dist2; //PI/5  (36 degrees)*/
	    l1 = l_base * cos(M_PI*54./180.) * ex_dist2;
	    enext_old = enext;
	    normalize(enext);
	    //facenext
	    tempv = enext % anormal;
	    normalize(tempv);
	    facenext = cos(alpha) * tempv  +  sin(alpha) * anormal;
	    normalize(facenext);

	    finaldir = facenext % enext;
	    normalize(finaldir);

	    //new midpoint
	    //find vertex 1 on face prev
	    count = i;
	    vl_temp1 = vl_ring_1;
	    for(j = 0; j < count; j++)
	      vl_temp1++;
	    f_1 = (*vl_temp1)->getCoords();

	    //find vertex 3 on face next
	    vl_temp1 = vl_ring_1;
	    if(i == face_sides - 1)
	      count = 0;
	    else
	      count = i + 1;
	    for(j=0; j < count; j++)
	      vl_temp1++;
	    f_2 = (*vl_temp1)->getCoords();

	    //now f1 and f2 have the 2 vertices we need the midpoint of
	    midpoint_tier2 = ( f_1 + f_2 )/2.0;
	    vp = (*vl_current);
	    tempv = vp->getCoords();
	    //tempv = base_verts[i] + l1*finaldir + enext_old * 0.5;
	    tempv =  midpoint_tier2 + l1*finaldir;
	    vp->setCoords(tempv);
	    vl_current++;
	  }

		vl_current = vl_ring_3;
	  //compute first side
		for(i=0; i < face_sides; i++) {
			//find l1
			if(i==0) eprev = base_verts[face_sides - 1];
			else eprev = base_verts[i-1];

			if(i == face_sides - 1)	enext = base_verts[0];
			else enext = base_verts[i+1];

			eprev -= base_verts[i];
			enext -= base_verts[i];
			//find the length of one side
			l1 = (norm(eprev) + norm(enext))*0.5 *ex_dist3;
			l_old = l1 * ex_dist3; //stores l_old for 3rd set of lines

			//find vertex 1 on face prev
			if(i == 0) count = face_sides - 1;
			else count = i-1;
			
			vl_temp1 = vl_ring_2;
			
			for(j = 0; j < count; j++) vl_temp1++;
			
			f_1 = (*vl_temp1)->getCoords();
			//find vertex 2 on face prev
			vl_temp1 = vl_ring_1;
			count = i;
			for(j = 0; j < count; j++) vl_temp1++;
			f_2 = (*vl_temp1)->getCoords();

			//find vertex 3 on face prev
			vl_temp1 = vl_ring_2;
			count = i;
			for(j = 0; j < count; j++) vl_temp1++;
			f_3 = (*vl_temp1)->getCoords();

			eprev = f_2 - f_3;
			l1 = norm(eprev);

			tedgeprev = f_1 - f_2;
			tedgenext = f_3 - f_2;
			normalize(tedgeprev);
			normalize(tedgenext);

			faceprev = tedgenext % tedgeprev;
			normalize(faceprev);

			//find vertex 1 on face next
			vl_temp1 = vl_ring_2;
			count = i;
			for(j = 0; j < count; j++) vl_temp1++;
				f_1 = (*vl_temp1)->getCoords();
			
			//find vertex 2 on face next
			vl_temp1 = vl_ring_1;
			if(i == face_sides - 1)	count = 0;
			else count = i + 1;
			
			for(j=0; j < count; j++)	vl_temp1++;
			f_2 = (*vl_temp1)->getCoords();

			//find vertex 3 on face next
			vl_temp1 = vl_ring_2;
			if(i == face_sides - 1)
			count = 0;
			else
			count = i + 1;
			for(j=0; j < count; j++) vl_temp1++;
				f_3 = (*vl_temp1)->getCoords();

			enext = f_2 - f_1;
			tedgeprev = f_1 - f_2;
			tedgenext = f_3 - f_2;
			normalize(tedgeprev);
			normalize(tedgenext);

			facenext = tedgenext % tedgeprev;
			normalize(facenext);

			finaldir = faceprev % facenext;
			normalize(finaldir);

			vl_temp2 = vl_ring_2;
			count = i;
			for(j = 0; j < count; j++)
				vl_temp2++;

			vp = (*vl_current);
			tempv = (*vl_temp2)->getCoords();
			tempv += l_old*finaldir;
			vp->setCoords(tempv);
			vl_current++;
	  }
	  return exface;
	}

	DLFLFacePtr extrudeFaceIcosa(DLFLObjectPtr obj, DLFLFacePtr fptr, double angle, int num, double ex_dist1, double ex_dist2, double ex_dist3) {
	     // Icosahedral extrusion
	  DLFLFacePtr exface = fptr;
	  //double sf3 = pow(sf, 1.0/2.0);
	  for (int i=0; i < num; ++i)
			exface = extrudeFaceIcosa(obj, exface, angle, ex_dist1, ex_dist2, ex_dist3);
	  return exface;
	}
	
	DLFLFacePtr extrudeFaceIcosa(DLFLObjectPtr obj, DLFLFacePtr fptr, double angle, double ex_dist1, double ex_dist2, double ex_dist3) {
	  //old
	     // Icosahedral extrusion
	  DLFLFacePtr exface = fptr;
	  DLFLFacePtr otherface;
	  DLFLEdgePtrArray ep_arr;
	  DLFLVertexPtr vp;
	  DLFLVertexPtrList :: iterator vl_first, vl_last, vl_current, vl_ring_1, vl_ring_2, vl_ring_3;

	  int num_old_verts = obj->num_vertices();
	  int face_sides = fptr->numFaceVertexes();

	  double d = 1;
	  double rot = 0;
	  double sf = 1;
	  double ddiv3 = d / 3.0;
	  double stellate_amount = 0;
	  double center_to_vert;

	  Vector3d center_sphere, center_sphere2;
	  Vector3d center_normal;
	  Vector3d first_face_centroid;
	  Vector3d face_normal = fptr->computeNormal();
	  //GET FACE'S BASE VERTICES************************************************
	  Vector3dArray base_verts;
	  DLFLFaceVertexPtr head;
	  head = fptr->front();
	  //create an array of the original face's vertex positions
	  if ( head ) {
			DLFLFaceVertexPtr current = head;
			base_verts.push_back(current->getAuxCoords());
			current = current->next();
			while ( current != head ) {
				base_verts.push_back(current->getAuxCoords());
				current = current->next();
			}
	  }
	  fptr->getVertexCoords(base_verts);
	  //**************************************************************************
	  center_sphere2 = center_sphere = exface->geomCentroid();
	  center_normal = exface->computeNormal();
	  center_sphere += center_normal * (d / 3.0);
	  center_sphere2 += center_normal * (d / 2.0);
	     // dual extrude, then regular extrude
	  exface = extrudeDualFace(obj, exface, ddiv3, rot, sf, false);
	  exface->getEdges( ep_arr);
	  vp = obj->lastVertex();
	  center_to_vert = normsqr(vp->coords - center_sphere);
	     // now have the radius of the shape sphere
	  exface = extrudeFace(obj, exface, ddiv3*2.0);

	     // delete top edges from dual extrude
	  DLFLEdgePtrArray :: iterator el_first, el_last;
	  DLFLEdgePtr ep;
	  el_first = ep_arr.begin(); el_last = ep_arr.end();
	  while ( el_first != el_last ) {
			ep = (*el_first); ++el_first;
			deleteEdge(obj, ep,true);
		}
	  bool started = false;
	     // stellate the top faces
	  exface->getEdges(ep_arr);
	  el_first = ep_arr.begin(); el_last = ep_arr.end();
	  while ( el_first != el_last ) {
	    ep = (*el_first); ++el_first;
	    otherface = ep->getOtherFacePointer(exface);
	    //if ( started == false )
	    //   {
	    //     first_face_centroid = otherface->geomCentroid();
	    //     stellate_amount = center_to_vert - normsqr(first_face_centroid - center_sphere2);
	    //     started = true;
	    //   }
	       // stellateFace(otherface, 1/pow(sf, 1.0/3.0));
	    stellateFace(obj, otherface, stellate_amount/2.0);
	  }
	  //REPOSITIONING CALCULATIONS **************************************************************
	  int num_verts = obj->num_vertices();
	  int i;
	  int vert_count=0;
	  Vector3d tempv;
	  vl_first = obj->beginVertex();
	  vl_last = obj->endVertex();

	  while(vert_count < num_old_verts) {
	    vl_first++;
	    vert_count++;
	  }
	  vl_ring_1 = vl_first;
	  vl_ring_2 = vl_first;
	  vl_ring_3 = vl_first;
	
	  for(i=0; i < face_sides; i++) {
	    //vl_ring_1++;
	    vl_ring_2++;
	    vl_ring_3++;
	  }
	  for(i=0; i < face_sides; i++) {
	     //vl_ring_2++;
	     vl_ring_2++;
	  }
	  //RINGS ARE SET UP*****************************************
	  Vector3d vnext,  vprev, finaldir, facenext, faceprev, facenext_2, faceprev_2, vnext_2, vprev_2;
	  Vector3d halfv;
	  DLFLVertexPtrList :: iterator vl_temp;
	  int j, count;
	  //double alpha = -(116.57-90.)*M_PI/180.;
	  //alpha = -(50.)*M_PI/180.;
	  double alpha = -(angle)*M_PI/180.;
	  double length_base;
	  double l1;
	  //FIND AV. LENGTH OF BASE EDGE************************************
	  length_base = 0;
	  vl_current = vl_ring_1;
	  for(i=0; i < face_sides; i++) {
	    if(i == face_sides - 1)
	      vnext = base_verts[0];
	    else
	      vnext = base_verts[i+1];
	    vnext -= base_verts[i];
	    //find the length of one side
	    length_base +=  norm(vnext);
	    vl_current++;
	  }
	  length_base /= (double) face_sides;
	  normalize(face_normal);
	  //FIRST RING OF VERTICES*******************************************
	  vl_current = vl_ring_1;
	  for(i=0; i < face_sides; i++) {
	    if(i == face_sides - 1)
	      vnext = base_verts[0];
	    else
	      vnext = base_verts[i+1];

	    vnext -= base_verts[i];
	    halfv = vnext * 0.5;
	    l1 = length_base * ex_dist1 * sqrt(3.0)*0.5;
	    //l1 *= ex_dist1; //changes size for first set of lines, ignores dihedral angle
	    normalize(vnext);
	    //facenext
	    tempv = vnext % face_normal;
	    normalize(tempv);
	    facenext = cos(alpha) * tempv  +  sin(alpha) * face_normal;
	    normalize(facenext);
	    finaldir = facenext % vnext;
	    normalize(finaldir);
	    vp = (*vl_current);
	    //tempv = vp->getCoords();
	    tempv = base_verts[i] + halfv + finaldir * l1;
	    vp->setCoords(tempv);
	    vl_current++;
	  }
	  //SECOND RING OF VERTICES*******************************************
	  vl_current = vl_ring_2;
	  for(i=0; i < face_sides; i++) {
	    if(i==0)
	      vprev = base_verts[face_sides - 1];
	    else
	      vprev = base_verts[i-1];

	    if(i == face_sides - 1)
	      vnext = base_verts[0];
	    else
	      vnext = base_verts[i+1];

	    vnext -= base_verts[i];
	    vprev -= base_verts[i];
	    l1 = length_base * ex_dist2;
	    //l1 *= ex_dist1; //changes size for first set of lines, ignores dihedral angle
	    normalize(vprev);
	    normalize(vnext);
	    //faceprev
	    tempv = face_normal % vprev;
	    normalize(tempv);
	    faceprev = cos(alpha) * tempv + sin(alpha) * face_normal;
	    normalize(faceprev);
	    //facenext
	    tempv = vnext % face_normal;
	    normalize(tempv);
	    facenext = cos(alpha) * tempv  +  sin(alpha) * face_normal;
	    normalize(facenext);
	    //now have the 2 faces, need to get the secondary face directions
	    //find next vertex on first tier
	    vl_temp = vl_ring_1;
	    count = i;

	    for(j = 0; j < count; j++)
	      vl_temp++;
	    tempv = (*vl_temp)->getCoords();
	    vnext_2 = tempv - base_verts[i];

	    //find prev vertex on first tier
	    vl_temp = vl_ring_1;
	    if(i==0)
	      count = face_sides - 1;
	    else
	      count = i-1;

	    for(j = 0; j < count; j++)
	      vl_temp++;
	    tempv = (*vl_temp)->getCoords();
	    vprev_2 = tempv - base_verts[i];

	    facenext = -facenext;
	    tempv = vnext_2 % facenext;
	    normalize(tempv);
	    facenext_2 = cos(alpha) * tempv  +  sin(alpha) * facenext;
	    normalize(facenext_2);

	    faceprev = -faceprev;
	    tempv =   faceprev % vprev_2;
	    normalize(tempv);
	    faceprev_2 = cos(alpha) * tempv  +  sin(alpha) * faceprev;
	    normalize(faceprev_2);

	    finaldir = faceprev_2 % facenext_2;
	    normalize(finaldir);
	    //got to offset due to twisting problem
	    vl_temp = vl_ring_2;
	    if(i==0)
	      count = face_sides - 1;
	    else
	      count = i-1;
	    for(j = 0; j < count; j++)
	      vl_temp++;
	    vp = (*vl_temp);
	    //tempv = vp->getCoords();
	    tempv = base_verts[i] + finaldir * l1;
	    //tempv = base_verts[i] + facenext_2*l1;
	    vp->setCoords(tempv);
	    vl_current++;
	  }
	  //THIRD RING OF VERTICES*******************************************
	  vl_current = vl_ring_3;
	  for(i=0; i < face_sides; i++) {
	    //Vector3d GetRingPosition(DLFLVertexPtrList :: iterator vl_ring, int position, int numfaces)
	    vprev = GetRingPosition(vl_ring_2, i-1, face_sides);

	    vnext = GetRingPosition(vl_ring_2, i, face_sides);

	    if(i == face_sides-1)
	      vnext_2 = base_verts[0];
	    else
	      vnext_2 = base_verts[i+1];

	    vprev_2 = base_verts[i];
	    vnext -= GetRingPosition(vl_ring_1, i, face_sides);
	    vprev -= GetRingPosition(vl_ring_1, i, face_sides);
	    vnext_2 -= GetRingPosition(vl_ring_1, i, face_sides);
	    vprev_2 -= GetRingPosition(vl_ring_1, i, face_sides);

	    l1 = length_base * ex_dist3;
	    //l1 *= ex_dist1; //changes size for first set of lines, ignores dihedral angle
	    normalize(vprev);
	    normalize(vnext);
	    normalize(vprev_2);
	    normalize(vnext_2);
	    //faceprev
	    faceprev = vprev % vprev_2;
	    normalize(faceprev);
	    //facenext
	    facenext = vnext_2 % vnext;
	    normalize(facenext);
	    //now have the 2 faces, need to get the secondary face directions
	    facenext = -facenext;
	    tempv = vnext_2 % facenext;
	    normalize(tempv);
	    facenext_2 = cos(alpha) * tempv  +  sin(alpha) * facenext;
	    normalize(facenext_2);

	    faceprev = -faceprev;
	    tempv =   faceprev % vprev_2;
	    normalize(tempv);
	    faceprev_2 = cos(alpha) * tempv  +  sin(alpha) * faceprev;
	    normalize(faceprev_2);

	    finaldir = faceprev_2 % facenext_2;
	    normalize(finaldir);

	    //got to offset due to twisting problem
	    vl_temp = vl_ring_1;
	    if(i==0)
	      count = face_sides - 1;
	    else
	      count = i-1;
	      count = i;
	    for(j = 0; j < count; j++)
	      vl_temp++;
	    vp = (*vl_temp);
	    tempv = vp->getCoords();
	    vp = (*vl_current);
	    //tempv = vp->getCoords();
	    //tempv = base_verts[i] + finaldir * l1;
	    tempv = GetRingPosition(vl_ring_1, i, face_sides);
	    //tempv -= faceprev;
	    tempv += finaldir * l1;
	    vp->setCoords(tempv);
	    vl_current++;
	  }
	  return exface;
	}

	DLFLFacePtr extrudeFaceCubOcta(DLFLObjectPtr obj, DLFLFacePtr fptr, double angle, int num, double ex_dist1, double ex_dist2, double ex_dist3){
		// Cuboctahedral extrusion
	  DLFLFacePtr exface = fptr;
	  //double sf3 = pow(sf, 1.0/2.0);
	  for (int i=0; i < num; ++i)
			exface = extrudeFaceCubOcta(obj, exface, angle, ex_dist1, ex_dist2, ex_dist3);
	  return exface;
	}

	DLFLFacePtr extrudeFaceSmallRhombiCubOcta(DLFLObjectPtr obj, DLFLFacePtr fptr, double angle, int num, double ex_dist1, double ex_dist2, double ex_dist3) {
		
		// Small RhombiCuboctahedral extrusion
		DLFLFacePtr exface = fptr;
		//double sf3 = pow(sf, 1.0/2.0);
		for (int i=0; i < num; ++i)
			exface = extrudeFaceSmallRhombiCubOcta(obj, exface,angle, ex_dist1, ex_dist2, ex_dist3);
		return exface;
	}

	DLFLFacePtr extrudeFaceCubOcta(DLFLObjectPtr obj, DLFLFacePtr fptr, double angle, double ex_dist1, double ex_dist2, double ex_dist3) {
			// Cuboctahedral extrusion
		DLFLFacePtr exface = fptr;
		DLFLFacePtr otherface;
		DLFLEdgePtrArray ep_arr;
		DLFLVertexPtr vp;
		DLFLVertexPtrList :: iterator vl_first, vl_last, vl_current, vl_ring_1, vl_ring_2, vl_ring_3;

		int num_old_verts = obj->num_vertices();
		int face_sides = fptr->numFaceVertexes();

		double d = 1;
		double rot = 0;
		double sf = 1;
		double ddiv3 = d / 3.0;
		double stellate_amount;
		double center_to_vert;

		Vector3d center_sphere, center_sphere2;
		Vector3d center_normal;
		Vector3d first_face_centroid;
		Vector3d face_normal = fptr->computeNormal();

		//GET FACE'S BASE VERTICES************************************************
		Vector3dArray base_verts;
		DLFLFaceVertexPtr head;

		head = fptr->front();

		//create an array of the original face's vertex positions
		if ( head ) {
			DLFLFaceVertexPtr current = head;
			base_verts.push_back(current->getAuxCoords());
			current = current->next();
			while ( current != head )
			{
				base_verts.push_back(current->getAuxCoords());
				current = current->next();
			}
		}
		fptr->getVertexCoords(base_verts);
		//**************************************************************************
		center_sphere2 = center_sphere = exface->geomCentroid();
		center_normal = exface->computeNormal();
		center_sphere += center_normal * (d / 3.0);
		center_sphere2 += center_normal * (d / 2.0);
			// dual extrude, then regular extrude
		exface = extrudeDualFace(obj, exface, ddiv3, rot, sf, false);
		exface->getEdges( ep_arr);
		vp = obj->lastVertex();
		center_to_vert = normsqr(vp->coords - center_sphere);
			// now have the radius of the shape sphere
		exface = extrudeDualFace(obj, exface, ddiv3, rot, sf, false);

			// delete top edges from dual extrude
		DLFLEdgePtrArray :: iterator el_first, el_last;
		DLFLEdgePtr ep;
		el_first = ep_arr.begin(); el_last = ep_arr.end();
		while ( el_first != el_last ) {
			ep = (*el_first); ++el_first;
			deleteEdge(obj, ep,true);
		}
		//REPOSITIONING CALCULATIONS **************************************************************
		int num_verts = obj->num_vertices();
		int i;
		int vert_count=0;

		Vector3d tempv;
		vl_first = obj->beginVertex();
		vl_last = obj->endVertex();

		while(vert_count < num_old_verts) {
			vl_first++;
			vert_count++;
		}

		vl_ring_1 = vl_first;
		vl_ring_2 = vl_first;
		for(i=0; i < face_sides; i++) {
			//vl_ring_1++;
			vl_ring_2++;
		}

		//RINGS ARE SET UP*****************************************
		Vector3d vnext,  vprev, finaldir, facenext, faceprev, facenext_2, faceprev_2, vnext_2, vprev_2;
		Vector3d halfv, midpoint_tier2;

		DLFLVertexPtrList :: iterator vl_temp;
		int j, count;
		//double alpha = -(116.57-90.)*M_PI/180.;
		//alpha = -(50.)*M_PI/180.;
		double alpha = -(angle)*M_PI/180.;
		double length_base;
		double l1;
		//FIND AV. LENGTH OF BASE EDGE************************************
		length_base = 0;
		vl_current = vl_ring_1;
		for(i=0; i < face_sides; i++) {
			if(i == face_sides - 1)
				vnext = base_verts[0];
			else
				vnext = base_verts[i+1];
			vnext -= base_verts[i];
			//find the length of one side
			length_base +=  norm(vnext);
			vl_current++;
		}
		length_base /= (double) face_sides;
		normalize(face_normal);
		//FIRST RING OF VERTICES*******************************************
		vl_current = vl_ring_1;
		for(i=0; i < face_sides; i++) {
			if(i == face_sides - 1)
				vnext = base_verts[0];
			else
				vnext = base_verts[i+1];

			vnext -= base_verts[i];
			halfv = vnext * 0.5;

			l1 = length_base * ex_dist1 * sqrt(3.0)*0.5;
			//l1 *= ex_dist1; //changes size for first set of lines, ignores dihedral angle
			normalize(vnext);
			//facenext
			tempv = vnext % face_normal;
			normalize(tempv);
			facenext = cos(alpha) * tempv  +  sin(alpha) * face_normal;
			normalize(facenext);
			finaldir = facenext % vnext;
			normalize(finaldir);

			vp = (*vl_current);
			//tempv = vp->getCoords();
			tempv = base_verts[i] + halfv + finaldir * l1;
			vp->setCoords(tempv);
			vl_current++;
		}
		//SECOND RING OF VERTICES*******************************************
		vl_current = vl_ring_2;
			//compute first side
		for(i=0; i < face_sides; i++) {
			vnext = GetRingPosition(vl_ring_1, i+1, face_sides);
			vprev = GetRingPosition(vl_ring_1, i, face_sides);
			l1 = length_base * sqrt((double)2) * ex_dist2;
				//find midpoint between second tier verts
			midpoint_tier2 = ( vprev + vnext )/2.0;
				//find a vector from the starting vert to this midpoint
			if(i == face_sides - 1)
				tempv = base_verts[0];
			else
				tempv = base_verts[i+1];

			finaldir = midpoint_tier2 - tempv;
			normalize(finaldir);
			vp = (*vl_current);
			tempv +=  l1*finaldir;
			vp->setCoords(tempv);
			vl_current++;
		}
		return exface;
	}//END OF CUBOCTAHEDRON

	DLFLFacePtr extrudeFaceSmallRhombiCubOcta(DLFLObjectPtr obj, DLFLFacePtr fptr, double angle, double ex_dist1, double ex_dist2, double ex_dist3) {
	     // Cuboctahedral extrusion
	  DLFLFacePtr exface = fptr;
	  DLFLFacePtr otherface;
	  DLFLEdgePtrArray ep_arr_1, ep_arr_2;
	  DLFLVertexPtrList :: iterator vl_first, vl_last, vl_current, vl_ring_1, vl_ring_2, vl_ring_3;
	  DLFLVertexPtrList :: iterator vl_ring_1_2, vl_ring_2_2;
	  DLFLVertexPtr vp;

	  int num_old_verts = obj->num_vertices();
	  int face_sides = fptr->numFaceVertexes();
	  int i;

	  double d = 1;
	  double rot = 0;
	  double sf = 1;
	  double ddiv3 = d / 3.0;
	  double stellate_amount;

	  DLFLVertexPtr base_vps[face_sides];

	  Vector3d center_sphere, center_sphere2;
	  Vector3d center_normal;
	  Vector3d first_face_centroid;
	  Vector3d face_normal = fptr->computeNormal();

	  //GET FACE'S BASE VERTICES************************************************
	  Vector3dArray base_verts;
	  DLFLFaceVertexPtr head;

	  head = fptr->front();

	  if(ex_dist1 == 0)
	    ex_dist1 = .1;
	  if(ex_dist2 == 0)
	    ex_dist2 = .1;
	  if(ex_dist3 == 0)
	    ex_dist3 = .1;
	  if(angle == 0)
	    angle = .1;
	  //create an array of the original face's vertex positions
	  i=0;
	  if ( head ) {
	     DLFLFaceVertexPtr current = head;
	     base_vps[i] = current->getVertexPtr();
	     i++;
	     base_verts.push_back(current->getAuxCoords());
	     current = current->next();
	     while ( current != head ) {
	       base_vps[i] = current->getVertexPtr();
	       i++;
	       base_verts.push_back(current->getAuxCoords());
	       current = current->next();
	     }
	  }
	  fptr->getVertexCoords(base_verts);
	  //**************************************************************************
	  center_sphere2 = center_sphere = exface->geomCentroid();
	  center_normal = exface->computeNormal();
	  center_sphere += center_normal * (d / 3.0);
	  center_sphere2 += center_normal * (d / 2.0);
		// dual extrude, then regular extrude
	  //exface = extrudeDualFace(exface, ddiv3, rot, sf, false);
	  exface = extrudeFace(obj, exface, 1.0);
	  exface->getEdges( ep_arr_1);
	     // now have the radius of the shape sphere
	  exface = extrudeFace(obj, exface, 1.0);
	  exface->getEdges( ep_arr_2);
	  //final extrusion
	  exface = extrudeFace(obj, exface, 1.0);
	  DLFLEdgePtrArray :: iterator el_first, el_last;
	  DLFLEdgePtr ep;
	  el_first = ep_arr_1.begin(); el_last = ep_arr_1.end();
	  while ( el_first != el_last ) {
			ep = (*el_first); ++el_first;
			subdivideEdge(obj,ep);
	  }
	  el_first = ep_arr_2.begin(); el_last = ep_arr_2.end();
	  while ( el_first != el_last ) {
			ep = (*el_first); ++el_first;
			subdivideEdge(obj, ep);
	  }
	  //REPOSITIONING CALCULATIONS **************************************************************
	  int num_verts = obj->num_vertices();
	  int vert_count=0;
	  Vector3d tempv;
	  vl_first = obj->beginVertex();
	  vl_last = obj->endVertex();

	  while(vert_count < num_old_verts) {
	    vl_first++;
	    vert_count++;
	  }

	  vl_ring_1 = vl_first;
	  vl_ring_2 = vl_first;
	  vl_ring_3 = vl_first;

	  for(i=0; i < face_sides; i++) {
	    //vl_ring_1++;
	    vl_ring_2++;
	    vl_ring_3++;
	    vl_ring_3++;
	  }
	  vl_ring_1_2 = vl_ring_3;
	  vl_ring_2_2 = vl_ring_3;

	  for(i=0; i < face_sides; i++) {
	      vl_ring_1_2++;
	      vl_ring_2_2++;
	      vl_ring_2_2++;
	  }
	  //RINGS ARE SET UP*****************************************
	  Vector3d vnext,  vprev, finaldir, finaldir_2, facenext, faceprev, facenext_2, faceprev_2, vnext_2, vprev_2;
	  Vector3d halfv, midpoint_tier2;

	  DLFLVertexPtrList::iterator vl_temp;
	  DLFLVertexPtr vp2;
	  int j, count;
	  //double alpha = -(116.57-90.)*M_PI/180.;
	  //alpha = -(50.)*M_PI/180.;
	  double alpha = -(angle)*M_PI/180.;
	  double length_base;
	  double l1;

	  //FIND AV. LENGTH OF BASE EDGE************************************
	  length_base = 0;
	  vl_current = vl_ring_1;
	  for(i=0; i < face_sides; i++) {
	    if(i == face_sides - 1)
	      vnext = base_verts[0];
	    else
	      vnext = base_verts[i+1];
	    vnext -= base_verts[i];
	    //find the length of one side
	    length_base +=  norm(vnext);
	    vl_current++;
	  }
	  length_base /= (double) face_sides;
	  normalize(face_normal);
	  //make new edges
	  vl_current = vl_ring_1_2;
	  for(i=0; i < face_sides; i++) {
	    vp2 = base_vps[i];
	    vp = (*vl_current);
	    insertEdge(obj, vp->getFaceVertexInFace(exface), vp2->getFaceVertexInFace(exface));
	    vl_current++;
	  }

	  for(i=0; i < face_sides; i++) {
	      vp = GetRingVertex(vl_ring_1_2, i, face_sides);
	      vp2 = GetRingVertex(vl_ring_2_2, i, face_sides);
	      insertEdge(obj, vp->getFaceVertexInFace(exface), vp2->getFaceVertexInFace(exface));
	  }

	  vl_current = vl_ring_2_2;
	  for(i=0; i < face_sides; i++) {
	      vp2 = GetRingVertex(vl_ring_3, i, face_sides);
	      vp = (*vl_current);
	      insertEdge(obj,vp->getFaceVertexInFace(exface), vp2->getFaceVertexInFace(exface));
	      vl_current++;
	  }
	  //FIRST RING OF VERTICES*******************************************
	  vl_current = vl_ring_1_2;
	  for(i=0; i < face_sides; i++) {
	    if(i == face_sides - 1)
	      vnext = base_verts[0];
	    else
	      vnext = base_verts[i+1];
	    vnext -= base_verts[i];
	    l1 = length_base * ex_dist1;
	    //l1 *= ex_dist1; //changes size for first set of lines, ignores dihedral angle
	    normalize(vnext);
	    //facenext
	    tempv = vnext % face_normal;
	    normalize(tempv);
	    facenext = cos(alpha) * tempv  +  sin(alpha) * face_normal;
	    normalize(facenext);

	    finaldir = facenext % vnext;
	    normalize(finaldir);

	    vp = (*vl_current);
	    tempv = base_verts[i] + finaldir * l1;
	    vp->setCoords(tempv);

	    vp = GetRingVertex(vl_ring_1, i+1, face_sides);
	    if(i == face_sides - 1)
	      tempv = base_verts[0];
	    else
	      tempv = base_verts[i+1];
	    tempv += finaldir * l1;
	    vp->setCoords(tempv);

	    vl_current++;
	  }
	  //SECOND RING OF VERTICES*******************************************
	  for(i=0; i < face_sides; i++) {
	    vnext = GetRingPosition(vl_ring_1, i+1, face_sides);
	    tempv = GetRingPosition(vl_ring_1_2, i, face_sides);

	    vnext -= tempv;
	    normalize(vnext);

	    tempv = GetRingPosition(vl_ring_1_2, i, face_sides);
	    vprev = base_verts[i];

	    vprev -= tempv;
	    normalize(vprev);

	    face_normal = - vprev % vnext;
	    normalize(face_normal);

	    l1 = length_base * ex_dist2;
	    //l1 *= ex_dist1; //changes size for first set of lines, ignores dihedral angle
	    //facenext
	    tempv = vnext % face_normal;
	    normalize(tempv);
	    facenext = cos(alpha) * tempv  +  sin(alpha) * face_normal;
	    normalize(facenext);

	    finaldir = facenext % vnext;
	    normalize(finaldir);
	    vp = GetRingVertex(vl_ring_2, i+1, face_sides);

	    tempv = GetRingPosition(vl_ring_1, i+1, face_sides) + finaldir * l1;
	    vp->setCoords(tempv);

	    vp = GetRingVertex(vl_ring_2_2, i, face_sides);
	    tempv = GetRingPosition(vl_ring_1_2, i, face_sides);
	    tempv += finaldir * l1;
	    vp->setCoords(tempv);

	    vl_current++;
	  }
	  //THIRD RING OF VERTICES*******************************************
	  for(i=0; i < face_sides; i++) {
	    vnext = GetRingPosition(vl_ring_2, i+1, face_sides);
	    tempv = GetRingPosition(vl_ring_2_2, i, face_sides);

	    vnext -= tempv;
	    normalize(vnext);

	    vprev = GetRingPosition(vl_ring_1, i+1, face_sides);
	    tempv = GetRingPosition(vl_ring_2, i+1, face_sides);

	    vprev -= tempv;
	    normalize(vprev);

	    face_normal = - vprev % vnext;
	    normalize(face_normal);

	    //l1 *= ex_dist1; //changes size for first set of lines, ignores dihedral angle
	    //facenext
	    tempv = vnext % face_normal;
	    normalize(tempv);
	    facenext = cos(alpha) * tempv  +  sin(alpha) * face_normal;
	    normalize(facenext);

	    finaldir = facenext % vnext;
	    normalize(finaldir);
	    //now do faceprev
	    vnext = GetRingPosition(vl_ring_2, i, face_sides);
	    tempv = GetRingPosition(vl_ring_2_2, i-1, face_sides);
	    vnext -= tempv;
	    normalize(vnext);

	    vprev = GetRingPosition(vl_ring_1, i, face_sides);
	    tempv = GetRingPosition(vl_ring_2, i, face_sides);

	    vprev -= tempv;
	    normalize(vprev);

	    face_normal = - vprev % vnext;
	    normalize(face_normal);
	    //l1 *= ex_dist1; //changes size for first set of lines, ignores dihedral angle
	    //faceprev
	    tempv = vnext % face_normal;
	    normalize(tempv);
	    faceprev = cos(alpha) * tempv  +  sin(alpha) * face_normal;
	    normalize(faceprev);

	    finaldir_2 = faceprev % vnext;
	    normalize(finaldir_2);
	    //finaldir = facenext % vnext;
	    //normalize(finaldir);
	    /////////////////////
	    vp = GetRingVertex(vl_ring_3, i, face_sides);
	    l1 = length_base * ex_dist3;
	    tempv =  GetRingPosition(vl_ring_2_2, i, face_sides) + finaldir * l1;
	    tempv += GetRingPosition(vl_ring_2, i, face_sides) + finaldir_2 * l1;
	    tempv *= 0.5;
	    //got the average position, so that weird angles don't twist the rhombicuboctahedron
	    vp->setCoords(tempv);
	    vl_current++;
	  }
	  return exface;
	}//END OF SMALLRHOMBICUBOCTAHEDRON

	Vector3d GetRingPosition(DLFLVertexPtrList :: iterator vl_ring, int position, int numfaces) {
	  int count = position;
	  int j;
	  DLFLVertexPtrList :: iterator vl_temp;
	  DLFLVertexPtr vp;
	  vl_temp = vl_ring;

	  while(count >= numfaces)
	    count -= numfaces;
		while(count < 0)
	    count += numfaces;
	  for(j=0; j < count; j++)
	    vl_temp++;
	  vp = (*vl_temp);
	  return vp->getCoords();
	}

	DLFLVertexPtr GetRingVertex(DLFLVertexPtrList :: iterator vl_ring, int position, int numfaces) {
	  int count = position;
	  int j;

	  DLFLVertexPtrList :: iterator vl_temp;
	  DLFLVertexPtr vp;

	  vl_temp = vl_ring;
	
	  while(count >= numfaces)
	    count -= numfaces;
	  while(count < 0)
	    count += numfaces;

	  for(j=0; j < count; j++)
	    vl_temp++;
	  vp = (*vl_temp);
	  return vp;
	}

	// DLFLFacePtr extrudeFaceIcosa(DLFLObjectPtr obj, DLFLFacePtr fptr, double d, int num, double rot, double sf) {
// 		// Icosahedral extrusion
// 		DLFLFacePtr exface = fptr;
// 
// 		double sf3 = pow(sf, 1.0/2.0);
// 		for (int i=0; i < num; ++i) {
// 			exface = extrudeFaceIcosa(obj,exface,d, rot,sf3);
// 		}
// 		return exface;
// 	}
	//end eric new
	// is performed by extruding, dual extruding, then extruding again.
	// then delete the latitude edges
	// DLFLFacePtr extrudeFaceIcosa(DLFLObjectPtr obj, DLFLFacePtr fptr, double d, double rot, double sf) {
	// 	// Icosahedral extrusion
	// 	DLFLFacePtr exface = fptr;
	// 	DLFLFacePtr otherface;
	// 	DLFLEdgePtrArray ep_arr;
	// 	DLFLVertexPtr vp;
	// 
	// 	double ddiv3 = d / 3.0;
	// 	double stellate_amount;
	// 	double center_to_vert;
	// 
	// 	Vector3d center_sphere, center_sphere2;
	// 	Vector3d center_normal;
	// 	Vector3d first_face_centroid;
	// 
	// 	center_sphere2 = center_sphere = exface->geomCentroid();
	// 	center_normal = exface->computeNormal();
	// 	center_sphere += center_normal * (d / 3.0);
	// 	center_sphere2 += center_normal * (d / 2.0);
	// 
	// 	// dual extrude, then regular extrude
	// 	exface = extrudeDualFace(obj,exface, ddiv3, rot, sf, false);
	// 	exface->getEdges(ep_arr);
	// 	vp = obj->lastVertex();
	// 	center_to_vert = normsqr(vp->coords - center_sphere);
	// 
	// 	// now have the radius of the shape sphere
	// 
	// 	exface = extrudeFace(obj, exface, ddiv3*2.0);
	// 
	// 	// delete top edges from dual extrude
	// 	DLFLEdgePtrArray::iterator el_first, el_last;
	// 	DLFLEdgePtr ep;
	// 	el_first = ep_arr.begin(); el_last = ep_arr.end();
	// 	while ( el_first != el_last ) {
	// 		ep = (*el_first); ++el_first;
	// 		deleteEdge(obj,ep,true);
	// 	}
	// 
	// 	bool started = false;
	// 
	// 	// stellate the top faces
	// 	exface->getEdges(ep_arr);
	// 	el_first = ep_arr.begin(); el_last = ep_arr.end();
	// 	while ( el_first != el_last ) {
	// 		ep = (*el_first); ++el_first;
	// 		otherface = ep->getOtherFacePointer(exface);
	// 		if ( started == false ) {
	// 			first_face_centroid = otherface->geomCentroid();
	// 			stellate_amount = center_to_vert - normsqr(first_face_centroid - center_sphere2);
	// 			started = true;
	// 		}
	// 
	// 		// stellateFace(obj, otherface, 1/pow(sf, 1.0/3.0));
	// 		stellateFace(obj, otherface, stellate_amount/2.0);
	// 	}
	// 	return exface;
	// }
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
	//dave
  void extrudeFaceDome(DLFLObjectPtr obj, DLFLFacePtr fptr, double length, double sf) {	
    // DLFLFacePtrList::iterator fl_first, fl_last;
    // DLFLFacePtr fp;
    // int num_old_faces;
    // int num_faces;

    // num_old_faces = obj->num_faces();

    // Subdivide all the edges by 4
    // subdivideAllEdges(obj,4);

		// DLFLEdgePtrArray eptrarr;
		// fptr->getEdges(eptrarr);

    // num_faces = 0; fl_first = obj->beginFace(); fl_last = obj->endFace();
    // while ( fl_first != fl_last && num_faces < num_old_faces ) {
      // fp = (*fl_first); ++fl_first; ++num_faces;
      // fptr = extrudeFaceDS(obj,fptr, 0.0*length, 1, 0.0, 1.6*sf);
      fptr = extrudeFaceDS(obj,fptr, 0.3*length, 1, 0.0, 1.7*sf);
      fptr = extrudeFaceDS(obj,fptr, 0.18*length, 1, 0.0, 1.6*sf);
      fptr = extrudeFaceDS(obj,fptr, 0.1*length, 1, 0.0, 1.4*sf);
      fptr = extrudeFaceDS(obj,fptr, 0.05*length, 1, 0.0, 1.2*sf);
      fptr = extrudeFaceDS(obj,fptr, 0.025*length, 1, 0.0, 1.1*sf);
			stellateFace(obj,fptr, 0.01*length);//, 1, 0.0, 0.01*sf);		
    // }
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
