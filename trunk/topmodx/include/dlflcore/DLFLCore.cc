/**
 * \file DLFLCore.cc
 */

#include "DLFLCore.hh"
#include <cmath>
#include <cassert>

namespace DLFL {

  /***************
   * Insert Edge *
   ***************/

  int insertEdge( DLFLObjectPtr obj, 
									 uint &faceId1, uint &vertId1,
									 uint &faceId2, uint &vertId2,
									 uint &faceId1b, uint &vertId1b,
									 uint &faceId2b, uint &vertId2b,
									 bool check,
									 bool set_type ) {
		int id = -1;

    if( !obj )
      return -1;
    DLFLFacePtr fptr1, fptr2;
    DLFLFaceVertexPtr fvptr1 = NULL, fvptr2 = NULL;
    DLFLFaceVertexPtr fvptr1b = NULL, fvptr2b = NULL;

    DLFLEdgePtr eptr;

    fptr1 = obj->findFace( faceId1 );
    fptr2 = obj->findFace( faceId2 );
		if( fptr1 && fptr2 ) {
			fvptr1 = fptr1->findFaceVertex( vertId1 );
			fvptr2 = fptr2->findFaceVertex( vertId2 );

			if( fvptr1 && fvptr2 ) {
				if( check )
					eptr = insertEdge( obj, fvptr1, fvptr2, set_type );
				else
					eptr = insertEdgeWithoutCheck( obj, fvptr1, fvptr2, set_type );
				if( eptr != 0 )
					id = eptr->getID( );

				faceId1 = fvptr1->getFaceID();
				faceId2 = fvptr2->getFaceID();
				vertId1 = fvptr1->getVertexID();
				vertId2 = fvptr2->getVertexID();

				fvptr1b = fvptr1->next();
				fvptr2b = fvptr2->next();
				faceId1b =  fvptr1b->getFaceID();
				faceId2b =  fvptr2b->getFaceID();
				vertId1b = fvptr1b->getVertexID();
				vertId2b = fvptr2b->getVertexID();
			}
		}
    return id;
  }

  DLFLEdgePtr insertEdge(  DLFLObjectPtr obj, 
													 DLFLFaceVertexPtr fvptr1, DLFLFaceVertexPtr fvptr2, 
													 bool set_type, DLFLMaterialPtr matl ) {
    if( coFacial(fvptr1, fvptr2) ) {
      if( fvptr1 != fvptr2 )
				return insertEdgeCoFacial( obj, fvptr1, fvptr2, set_type );
      return NULL;
    }
    return insertEdgeNonCoFacial( obj, fvptr1, fvptr2, matl );
  }

  DLFLEdgePtr insertEdgeCoFacial( DLFLObjectPtr obj, 
																	DLFLFaceVertexPtr fvptr1, DLFLFaceVertexPtr fvptr2, 
																	bool set_type ) {
    //Insert an edge between 2 corners in the same face.Doesn 't check if both
    // corners are in the same face or not
    // Insertion of the Edge will split the Face into 2 faces
    DLFLFacePtr fp = fvptr1->getFacePtr();
    DLFLMaterialPtr matl = fp->material();

    DLFLEdgePtr edgeptr;
    //Pointer to the new Edge
    DLFLFacePtr newfaceptr;
    //Pointers to the new Faces
    DLFLFaceVertexPtr fvp, temp;
    DLFLEdgePtr ep;

    //Only 1 new face is created.
    newfaceptr = new DLFLFace( matl);

    //Use same type as fp for the new face.Useful for doo - Sabin subdivision
    newfaceptr->setType(fp->getType());

    //Set type if caller requested it
    if (set_type) {
      //Reset types of the 2 FaceVertexes
      fvptr1->resetType();
      fvptr2->resetType();
      fp->setType(FTNew);
      newfaceptr->setType(FTNew);
    }
    //Create the new face.Add a copy of fvptr1 to the new face
    // Update edge which originally pointed to fvptr1 to point to new face vertex
    temp = newfaceptr->addVertex(fvptr1);

    //Copy needs to be made
    temp->addSelfToVertex();
    ep = fvptr1->getEdgePtr();

		if( ep == NULL)
			return NULL;

		ep->resetFaceVertexPtr(fvptr1);
    ep->setNullFaceVertexPtr(temp);

    //Remove face - vertices from fvptr1 to fvptr2
    // from old face and add it to the new face
    fvp = fvptr1->next();
    while (fvp != fvptr2) {
      //Remove fvp from old face and add it to the new face
      temp = fvp->next();
      fp->deleteVertexPtr(fvp);
      newfaceptr->addVertexPtr(fvp);
      fvp = temp;
    }

    //Remove fvptr2 from old face
    fp->deleteVertexPtr(fvptr2);

    //Add copy of fvptr2 to old face after fvptr1
    temp = fp->insertAfter(fvptr1, fvptr2, true);
    temp->addSelfToVertex();

    //Add fvptr2 to new face
    newfaceptr->addVertexPtr(fvptr2);

    //Update edge which originally pointed to fvptr2 to point to the copy
    ep = fvptr2->getEdgePtr();
    ep->resetFaceVertexPtr(fvptr2);
    ep->setNullFaceVertexPtr(temp);

    //Add new face to face list
    obj->addFacePtr(newfaceptr);

    //Create the new Edge and do necessary updates
    edgeptr = new DLFLEdge;
    edgeptr->setFaceVertexPointers(fvptr1, fvptr2);
    edgeptr->updateFaceVertices();
    obj->addEdgePtr(edgeptr);
    if( set_type )
      edgeptr->setType(ETNew);

    return edgeptr;
  }

  DLFLEdgePtr insertEdgeNonCoFacial( DLFLObjectPtr obj, DLFLFaceVertexPtr fvptr1, DLFLFaceVertexPtr fvptr2, DLFLMaterialPtr matl ) {
    //Insert an edge between 2 corners belonging to different faces
    // Doesn 't check if the corners belong to different faces
    DLFLFacePtr fp1 = fvptr1->getFacePtr();
    DLFLFacePtr fp2 = fvptr2->getFacePtr();

    DLFLEdgePtr newedgeptr;
    //The new Edge
    DLFLFacePtr newfaceptr;
    //The new Face
    DLFLFaceVertexPtr fvp, temp;
    int numverts1 = 0, numverts2 = 0;
    //No.of verts in each face

    numverts1 = fp1->size();
    numverts2 = fp2->size();

    //Reorder face 1 so that it ends at fvptr1
    fp1->reorder(fvptr1->next());

    //Reorder face 2 so that it ends at fvptr2
    fp2->reorder(fvptr2->next());

    //"New" face will be the larger of the old faces
    // For now assume fp1 is the larger face
    newfaceptr = fp1;

    //If a material is specified use it, otherwise stick with material of fp1
    if (matl)
      newfaceptr->setMaterial(matl);

    //Add face - vertices from face 2 and remove them from face 2

    // Check if fp2 is a point - sphere.If so, fvptr2 should be there only once
    if (numverts2 > 1) {
      //Add copy of fvptr2.The edge which fvptr2 originally pointed to
      // must be updated to point to the new face vertex
      temp = newfaceptr->addVertex(fvptr2);
      temp->addSelfToVertex();
      DLFLEdgePtr ep = fvptr2->getEdgePtr();
      ep->resetFaceVertexPtr(fvptr2);
      ep->setNullFaceVertexPtr(temp);
    }
    //Go through all vertices in fp2 and add them to the new face and delete them from the old face
    fvp = fp2->front();

    do {
      temp = fvp->next();
      fp2->deleteVertexPtr(fvp);
      newfaceptr->addVertexPtr(fvp);
      fvp = temp;
    } while (fp2->front());

    //Check if fp1 is a point - sphere.If so, fvptr1 should be there only once
    if (numverts1 > 1) {
      //Add copy of fvptr1.The edge which fvptr1 originally pointed to
      // must be updated to point to the new face vertex
      temp = newfaceptr->addVertex(fvptr1);
      temp->addSelfToVertex();
      DLFLEdgePtr ep = fvptr1->getEdgePtr();

      ep->resetFaceVertexPtr(fvptr1);
      ep->setNullFaceVertexPtr(temp);
    }
    //Delete face 2 from the face list and free the pointer
    obj->removeFace(fp2);
    delete fp2;

    //Create the new Edge and do necessary updates
    newedgeptr = new DLFLEdge;
    newedgeptr->setFaceVertexPointers(fvptr1, fvptr2);
    obj->addEdgePtr(newedgeptr);
    newedgeptr->updateFaceVertices();

    return newedgeptr;
  }

  DLFLEdgePtr insertEdgeWithoutCheck( DLFLObjectPtr obj, DLFLFaceVertexPtr fvptr1, DLFLFaceVertexPtr fvptr2, bool set_type, DLFLMaterialPtr matl) {
    if( coFacial(fvptr1, fvptr2) )
      return insertEdgeCoFacial(obj,fvptr1, fvptr2, set_type );
    return insertEdgeNonCoFacial(obj, fvptr1, fvptr2, matl );
  }

  /***************
   * Delete Edge *
   ***************/

	std::vector<int> deleteEdgeID( DLFLObjectPtr obj, uint edgeId, bool cleanup ) {
		std::vector<int> faceids;

    DLFLEdgePtr eptr = obj->findEdge( edgeId );
		if( eptr == NULL ) {
			faceids.push_back(-1);
			return faceids;
		}

    DLFLFacePtrArray fpa = deleteEdge( obj, eptr, cleanup );
		if( fpa.size() > 0 ) {
			for( int i=0; i < fpa.size(); i++ )
				faceids.push_back( fpa[i]->getID() );
		}
		return faceids;
  }

  DLFLFacePtrArray deleteEdge( DLFLObjectPtr obj, DLFLEdgePtr edgeptr, bool cleanup ) {
    DLFLFaceVertexPtr fvpV1, fvpV2;
    DLFLFacePtr f1, f2;

		DLFLFacePtrArray rfpa;

    fvpV1 = NULL;
    fvpV2 = NULL;
    edgeptr->getFaceVertexPointers(fvpV1, fvpV2);

    f1 = fvpV1->getFacePtr();
    f2 = fvpV2->getFacePtr();

    if (f1 != f2) {
      //Two edge sides belong to different faces

      // We 'll keep one of the old faces and delete the other one.
      // We should keep the larger face and delete the smaller one
      // For now we 'll assume that f1 is the larger face. Vertices from f2
      // will be added to f1.There will an overlap in face - vertices at the ends
      // of the edge

      // Reorder f1 so that it ends at fvpV1
      f1->reorder(fvpV1->next());

      //Reorder f2 so that it ends at fvpV2
      f2->reorder(fvpV2->next());

      //Remove fvpV1 from f1
      f1->deleteVertexPtr(fvpV1);

      //Go through vertices of f2 starting at the beginning and add all of them except fvpV2
      // Also remove them from f2
      DLFLFaceVertexPtr next, temp = NULL;
      temp = f2->front();
      while (temp != fvpV2) {
				next = temp->next();
				f2->deleteVertexPtr(temp);
				f1->addVertexPtr(temp);
				temp = next;
      }

      //Remove fvpV2 from f2
      f2->deleteVertexPtr(fvpV2);

      //Remove fvpV1 and fvpV2 from the face - vertex - lists of their vertices
      fvpV1->deleteSelfFromVertex();
      fvpV2->deleteSelfFromVertex();

      //Free memory for fvpV1 and fvpV2
      delete fvpV1;
      delete fvpV2;

      //The Edge can now be removed from the EdgeList
      // Free the pointer also since the edge_list owns the DLFLEdge pointed to by edgeptr
      obj->removeEdge(edgeptr);
      delete edgeptr;

      //Destroy f2 and delete it from the face list
      f2->destroy();
      obj->removeFace(f2);
      delete f2;
			rfpa.push_back(f1);
    } else {
      //Two edge sides belong to same face

      // Split the Face to get 2 new faces
      // Without loss of generality, f1 will be used to refer to the Face

      // 1 new face will be created.
      DLFLFacePtr nfp = new DLFLFace( f1->material());
			rfpa.push_back(f1);
			rfpa.push_back(nfp);
      DLFLFaceVertexPtr fvpTemp, temp;

      //Reorder the face so that face starts at vertex after fvpV2
      f1->reorder(fvpV2->next());

      //Start at vertex after fvpV1, remove from face and add to new face till we reach fvpV2
      // Don 't add fvpV2 unless fvpV2 is right after fvpV1, in which case we will have a point sphere
      fvpTemp = fvpV1->next();
      if (fvpTemp == fvpV2) {
				//Add fvpV2.nfp will be a point sphere.Add a copy since fvpV2 will be deleted later on
				// Since nfp will be a point sphere adding a copy will not affect any other pointers
				temp = nfp->addVertex(fvpV2);
				temp->addSelfToVertex();
				temp->setEdgePtr(NULL);
      }
      //Loop will execute only if nfp is not a point sphere
      while (fvpTemp != fvpV2) {
				temp = fvpTemp->next();
				f1->deleteVertexPtr(fvpTemp);
				nfp->addVertexPtr(fvpTemp);
				fvpTemp = temp;
      }

      //Remove fvpV2 from f1 and free the pointer.Remove fvpV2 from face - vertex - list of its vertex also
      f1->deleteVertexPtr(fvpV2);
      fvpV2->deleteSelfFromVertex();
      delete fvpV2;
			
      //Now check if fvpV1 is the only vertex in f1.If so f1 becomes a point sphere
      // Don 't delete fvpV1 from f1. Otherwise delete fvpV1 from f1 and free the pointer
      if (fvpV1->next() != fvpV1) {
				//Remove fvpV1 from the face - vertex - list of its vertex also
				f1->deleteVertexPtr(fvpV1);
				fvpV1->deleteSelfFromVertex();
				delete fvpV1;
      }
      //The Edge can now be removed from the EdgeList
      // Free the pointer also, since edge_list owns the object pointed to by edgeptr
      obj->removeEdge(edgeptr);
      delete edgeptr;
      
      //Add the new Face to the FaceList
      obj->addFacePtr(nfp);

      //If f1 ends up being a point sphere, reset the EdgePtr field of it 's only corner
      if (f1->size() == 1) {
				DLFLFaceVertexPtr fvp = f1->firstVertex();
				//DLFLVertexPtr vp = fvp->vertex;
				//obj->removeVertex(vp); delete vp;

				fvp->setEdgePtr(NULL);
				//f1 is now a PointSphere
      }
      //Check if we have to cleanup point - spheres
      if (cleanup) {
				//If either f1 or nfp is a point - sphere, completely remove it from the
				// object and also the vertex in that face
				DLFLFaceVertexPtr fvp;
				DLFLVertexPtr vp;

				if (f1->size() == 1) {
					fvp = f1->firstVertex();
					vp = fvp->vertex;
					obj->removeVertex(vp);
					delete vp;

					f1->destroy();
					obj->removeFace(f1);
					if(!rfpa.empty())
						rfpa.erase( rfpa.begin() );
					delete f1;
				}
				if (nfp->size() == 1) {
					fvp = nfp->firstVertex();
					vp = fvp->vertex;
					obj->removeVertex(vp);
					delete vp;

					nfp->destroy();
					obj->removeFace(nfp);
					if(!rfpa.empty())
						rfpa.erase( --(rfpa.end()) );
					delete nfp;
				}
      }
    }
		return rfpa;
  }

  void deleteEdge(DLFLObjectPtr obj, uint edge_index, bool cleanup) {
    //Find the EdgePtr for the given edge_index from the EdgeList and delete it
    if (edge_index > obj->num_edges( ) )
      return;
    
    DLFLEdgePtrList::iterator i = obj->beginEdge( );
    advance(i, edge_index);
    deleteEdge(obj, *i, cleanup);
  }

  /*****************
   * Collapse Edge *
   *****************/

  int collapseEdgeID( DLFLObjectPtr obj, const uint edgeId, bool cleanup ) {
    DLFLEdgePtr ep = obj->findEdge( edgeId );

		if( ep == NULL ) return -1;
    
    DLFLVertexPtr vp;
    vp = collapseEdge( obj, ep, cleanup );

    if( vp )
      return vp->getID();
    else
      return -1;
  }

  DLFLVertexPtr collapseEdge( DLFLObjectPtr obj, DLFLEdgePtr edgeptr, bool cleanup ) {
    //Collapse an edge - merge two vertices into one after removing in - between edge
    if (edgeptr == NULL)
      return NULL;

    //If the edge is a self - loop, just delete the edge with cleanup
    if (edgeptr->isSelfLoop()) {
      deleteEdge(obj, edgeptr, true);
      return NULL;
    }
    DLFLFaceVertexPtr efvp1, efvp2;
    DLFLVertexPtr vp1, vp2;
    DLFLEdgePtr ep1next, ep1prev, ep2next, ep2prev;

    edgeptr->getFaceVertexPointers(efvp1, efvp2);
    vp1 = efvp1->getVertexPtr();
    vp2 = efvp2->getVertexPtr();

    //Adjust coordinates of vp1 to be midpoint of collapsed edge
    vp1->setCoords(edgeptr->getMidPoint());

    //Find edges adjacent to specified edge.
    // If cleanup is required some of them will be deleted later on
    ep1next = (efvp1->next())->getEdgePtr();
    ep1prev = (efvp1->prev())->getEdgePtr();
    ep2next = (efvp2->next())->getEdgePtr();
    ep2prev = (efvp2->prev())->getEdgePtr();

    //Vertex 1(vp1, efvp1) will remain and vertex 2(vp2, efvp2) will be removed

    // First adjust edge pointer of efvp1 and correspondingly adjust the face - vertex pointer
    // of the edge to which efvp1 will point after removal of vertex 2
    // Also remove the FVP after efvp1 from the face
    DLFLFaceVertexPtr efvp1_next = efvp1->next();
    DLFLEdgePtr efvp1_next_ep = efvp1_next->getEdgePtr();

    //Remove efvp1_next from Edge, Vertex and Face and delete the pointer itself
    efvp1_next->deleteSelfFromEdge();
    efvp1_next->deleteSelfFromVertex();
    efvp1_next->deleteSelfFromFace();
    delete efvp1_next;

    //Reset edge pointer of efvp1 and make the edge point back to efvp1
    efvp1->setEdgePtr(efvp1_next_ep);
    efvp1->addSelfToEdge();

    //Remove efvp2 from Vertex and Face and delete the pointer itself.
    // Removal from Edge is not necessary since the Edge itself is going to be deleted
    // For the same reason no edge pointers have to be adjusted
    efvp2->deleteSelfFromVertex();
    efvp2->deleteSelfFromFace();
    delete efvp2;

    //Go through all remaining face - vertices referring to vp2 and make them point
    // to vp1 instead.Since vp2 is going to be deleted anyway, these face - vertices
    // need not be removed from vp2 's list.
    DLFLFaceVertexPtrArray fvparray;
    vp2->getFaceVertices(fvparray);
    for (int i = 0; i < (int)fvparray.size(); ++i) {
      fvparray[i]->setVertexPtr(vp1);
      vp1->addToFaceVertexList(fvparray[i]);
    }

    //Delete Vertex 2(vp2) from Vertex list and free memory
    obj->removeVertex(vp2);
    delete vp2;

    //Delete edge to be collapsed from edge list and free memory
    obj->removeEdge(edgeptr);
    delete edgeptr;

    //Do cleanup of 2 - gons if boolean flag is true
    if (cleanup == true) {
      if (ep1next->getOtherVertexPointer(vp1) == ep1prev->getOtherVertexPointer(vp1)) {
				//Remove ep1next
				deleteEdge(obj, ep1next, true);
      }
      if (ep2next->getOtherVertexPointer(vp1) == ep2prev->getOtherVertexPointer(vp1)) {
				//Remove ep2prev
				deleteEdge(obj, ep2prev, true);
      }
    }

    return vp1;
  }

  void collapseEdge( DLFLObjectPtr obj, uint edge_index, bool cleanup ) {
    //Find the EdgePtr for the given edge_index from the EdgeList and collapse it
    if( edge_index > obj->num_edges() )
      return;
    
    DLFLEdgePtrList::iterator i = obj->beginEdge();
    advance(i, edge_index);
    collapseEdge(obj, *i, cleanup);
  }

  /******************
   * Subdivide Edge *
   ******************/

  int subdivideEdgeID( DLFLObjectPtr obj, uint edgeId, bool set_type ) {
    DLFLEdgePtr ep = obj->findEdge( edgeId );

		if( ep == NULL ) return -1;
    
    DLFLVertexPtr vp;
    vp = subdivideEdge( obj, ep, set_type );

    if( vp )
      return vp->getID();
    else
      return -1; 
  }

  DLFLVertexPtr subdivideEdge( DLFLObjectPtr obj, DLFLEdgePtr edgeptr, bool set_type) {
    //Sub - divide an Edge into 2 Edges.A new Vertex will be added at the midpoint
    // belonging to both Faces on either side of the original Edge
    DLFLFaceVertexPtr fvpV1, fvpV2;
    DLFLFacePtr f1, f2;

    fvpV1 = NULL;
    fvpV2 = NULL;

    edgeptr->getFaceVertexPointers(fvpV1, fvpV2);

		if( fvpV1 == NULL || fvpV2 == NULL )
			return NULL;

    f1 = fvpV1->getFacePtr();
    f2 = fvpV2->getFacePtr();

		if( f1 == NULL || f2 == NULL )
			return NULL;

    //Edge subdivision will work whether the two Edge sides belong to different Faces
    // or not.
    DLFLVertexPtr nvp = new DLFLVertex;

    obj->addVertexPtr(nvp);

    //Create the 2 new FaceVertexes
    DLFLFaceVertexPtr newfv1, newfv2;

    newfv1 = new DLFLFaceVertex;
    newfv2 = new DLFLFaceVertex;

    //Set the VertexPtr and FacePtr fields of the new FaceVertexes
    newfv1->setVertexPtr(nvp);
    newfv1->setFacePtr(f1);
    newfv2->setVertexPtr(nvp);
    newfv2->setFacePtr(f2);

    DLFLFaceVertexType vt1,vt2;
    //Set type if caller requested it
    if (set_type) {
      //Always set it to new Edge point.Caller can change it to a new Face point
      // at the appropriate places
      nvp->setType(VTNewEdgePoint);
      newfv1->setType(FVTNew);
      newfv2->setType(FVTNew);
      vt1 = newfv1->getType();
      vt2 = newfv2->getType();
    }
    //Insert the new FaceVertexes into the respective
    // Faces at the appropriate locations
		//check if they are null - added by dave
		if (f1 && newfv1)
	    f1->insertAfter(fvpV1, newfv1, false);

		if (f2 && newfv2)
	    f2->insertAfter(fvpV2, newfv2, false);

    //Average the normals, coordinates, color and texture coordinates
    average(fvpV1, f1->nextFaceVertex(newfv1), newfv1);
    average(fvpV2, f2->nextFaceVertex(newfv2), newfv2);

    //Add the 2 new FaceVertexes to the list in Vertex
    nvp->addToFaceVertexList(newfv1);
    nvp->addToFaceVertexList(newfv2);

    //Create the 2 new Edges
    DLFLEdgePtr nep1, nep2;

    nep1 = new DLFLEdge;
    nep2 = new DLFLEdge;

    nep1->setFaceVertexPointers(fvpV1, newfv2);
    nep2->setFaceVertexPointers(newfv1, fvpV2);

    //Set type of new Edge if caller requested it
    if (set_type) {
      nep1->setType(ETNew);
      nep2->setType(ETNew);
    }
    //Remove the existing Edge from the EdgeList
    // Free the pointer also
    obj->removeEdge(edgeptr);
    delete edgeptr;

    //Add the 2 new Edges into the EdgeList
    obj->addEdgePtr(nep1);
    obj->addEdgePtr(nep2);

    //Update the EdgePtr fields of the 2 FaceVertexes
    newfv1->setEdgePtr(nep2);
    newfv2->setEdgePtr(nep1);
    fvpV1->setEdgePtr(nep1);
    fvpV2->setEdgePtr(nep2);

    return nvp;
  }

  DLFLVertexPtr subdivideEdge( DLFLObjectPtr obj, uint edge_index) {
    //Find the EdgePtr for the given edge_index from the EdgeList and sub - divide it
    if (edge_index >= obj->num_edges() )
      return NULL;

    DLFLEdgePtrList::iterator i = obj->beginEdge();
    advance(i, edge_index);
    return subdivideEdge(obj,*i);
  }

  vector<int> subdivideEdgeID( DLFLObjectPtr obj, int num_divs, uint edgeId, bool set_type ) {
    DLFLEdgePtr ep = obj->findEdge( edgeId );
    DLFLVertexPtrArray vpa;
    vector<int> vIDs;

    vpa = subdivideEdge( obj, num_divs, ep, set_type );

    if( vpa.size() > 0 ) {
      for(int i = 0; i < (int)vpa.size(); i++ ) {
				vIDs.push_back( vpa[i]->getID() );
      }
    }

    return vIDs;
  }

  DLFLVertexPtrArray subdivideEdge( DLFLObjectPtr obj, int num_divs, DLFLEdgePtr edgeptr, bool set_type ) {
    DLFLVertexPtrArray newVerts;
    //Sub - divide an edge into several edges.Does this by repeatedly calling
    // subdivideEdge(edgeptr, set_type) which subdivides into 2 parts.
    // Coordinates of the new points are set using the returned vertex pointer
    if (num_divs < 2)
      return newVerts;
    if (num_divs > 2) {
      DLFLVertexPtr newvptr;
      DLFLEdgePtr eptr;
      Vector3d p1, p2, p, dp;

      edgeptr->getEndPoints(p1, p2);
      dp = (p2 - p1) / double (num_divs);
      eptr = edgeptr;
      for (int i = 1; i < num_divs; ++i) {
				p = p1 + double (i) * dp;
				newvptr = subdivideEdge(obj, eptr, set_type);
				newvptr->coords = p;
				newVerts.push_back(newvptr);
	
				//Find the last edge from the edge_list.
				// NOTE:This is not an arbitrary choice
				// The last edge in the list will be the edge from midpoint
				// to the second vertex(p1) of the original edge
				eptr = obj->lastEdge( );
      }
    } else {
      //Simple call the regular subdivideEdge
      DLFLVertexPtr newvptr = subdivideEdge(obj, edgeptr, set_type);
      newVerts.push_back(newvptr);
    }

    return newVerts;
  }

  void subdivideEdge( DLFLObjectPtr obj, int num_divs, uint edge_index) {
    //Find the EdgePtr for the given edge_index from the EdgeList and sub - divide it
    if (edge_index < obj->num_edges()) {
      DLFLEdgePtrList::iterator i = obj->beginEdge();
      advance(i, edge_index);
      subdivideEdge(obj,num_divs, *i);
    }
  }

  void trisectEdge( DLFLObjectPtr obj, DLFLEdgePtr edgeptr, double num, bool scale, bool set_type) {
    //Split the edge into 3. New points will be calculated based on scale factor
    // with respect to mid point of edge OR offset from end points
    // Boolean flag indicates if specified number is a scale factor or offset distance
    // Offset distance is clamped to lie between 0.01 and edgelength / 2 - 0.01
    // Scale factor is clamped to lie between 0.01 and 0.99(inclusive)
    //Store the end points of the edge
    DLFLVertexPtr evp1, evp2;
    edgeptr->getVertexPointers(evp1, evp2);

    //Find coordinates of the new points
    Vector3d ep1, ep2, mp1, mp2, mid;
    ep1 = evp1->coords;
    ep2 = evp2->coords;

    if (scale == true) {
      //Given number is a scale factor
      // Clamp scale factor to lie between 0.01 and 0.99
      if (num > 0.99)
				num = 0.99;
      else if (num < 0.01)
				num = 0.01;

      mid = (ep1 + ep2) * 0.5;
      mp1 = num * ep1 + (1.0 - num) * mid;
      mp2 = num * ep2 + (1.0 - num) * mid;
    } else {
      //Given number is an offset distance from end points
      // Clamp offset distance to be between 0.01 and(edge_length / 2) - 0.01
      double maxoffset = edgeptr->length() / 2.0 - 0.01;

      if (num < 0.01)
				num = 0.01;
      else if (num > maxoffset)
				num = maxoffset;

      Vector3d evec = ep2 - ep1;

      normalize(evec);
      mp1 = ep1 + num * evec;
      mp2 = ep2 - num * evec;
    }

    DLFLVertexPtr mvp1, mvp2;

    //Subdivide edge into 2 first
    // Adjust coordinates of midpoint
    mvp1 = subdivideEdge(obj,edgeptr);
    mvp1->coords = mp1;

    //Subdivide the last edge in the list into 2 again
    // Adjust coordinates of midpoint
    mvp2 = subdivideEdge(obj,obj->lastEdge());
    mvp2->coords = mp2;

    if (set_type) {
      mvp1->setType(VTNewPoint);
      mvp2->setType(VTNewPoint);
    }
  }

  void subdivideAllEdges( DLFLObjectPtr obj, int num_divs, bool set_type) {
    //Go through edge list and sub - divide all the old edges
    DLFLEdgePtrList::iterator first;
    DLFLEdgePtr ep;
    int num_edges, num_old_edges;

    num_old_edges = obj->num_edges();
    num_edges = 0;
    first = obj->beginEdge();
    while (num_edges < num_old_edges) {
      ep = (*first);
      ++first;
      ++num_edges;
      subdivideEdge(obj,num_divs, ep, set_type);
    }
  }

  void subdivideNonLoopingEdges( DLFLObjectPtr obj, int num_divs, bool set_type) {
    //Go through edge list and sub - divide all the old edges which are not self - loops
    DLFLEdgePtrList::iterator first;
    DLFLEdgePtr ep;
    int num_edges, num_old_edges;

    num_old_edges = obj->num_edges();
    num_edges = 0;
    first = obj->beginEdge();
    while (num_edges < num_old_edges) {
      ep = (*first);
      ++first;
      ++num_edges;
      if (!(ep->isSelfLoop()))
				subdivideEdge(obj,num_divs, ep, set_type);
    }
  }

  void spliceCorners( DLFLObjectPtr obj, DLFLFaceVertexPtr fvptr1, DLFLFaceVertexPtr fvptr2) {
    DLFLEdgePtr ep = insertEdge(obj,fvptr1, fvptr2);
    if (ep != NULL)
      collapseEdge(obj,ep);
  }

  DLFLObjectPtr readObjectFile( char* filename ) {
    DLFLObjectPtr obj;
    ifstream file;

    file.open( filename );
    if( !file ) {
      return NULL;
    }

    obj = new DLFLObject( );
    
    char* ext = strrchr( filename, '.' );

    if( strcasecmp(ext,".obj") == 0 ) {
      obj->readObject( file );
      obj->setFilename( filename );
    } else if( strcasecmp(ext,".dlfl") == 0 ) {
      obj->readDLFL( file );
      obj->setFilename( filename );
    } else {
      delete obj;
      obj = NULL;
    }

    obj->computeNormals( );

    file.close( );
    return obj;
  }

  bool writeObjectFile( DLFLObject *obj, char* filename ) {
    ofstream file;
    if( filename == NULL )
      filename = obj->getFilename( );
    file.open( filename );
    if( !file )
      return false;
     
    char* ext = strrchr( filename, '.' );
    bool wrote = false;
    if( strcasecmp(ext,".obj") == 0 ) {
      obj->writeObject( file, true, true );
      //obj->setFilename( filename );
    } else if( strcasecmp(ext,".dlfl") == 0 ) {
      obj->writeDLFL( file, false );
      //obj->setFilename( filename );
    }	else if( strcasecmp(ext,".m") == 0 ) {
      obj->writeLG3d( file, false );
      //obj->setFilename( filename );
    }	else if( strcasecmp(ext,".stl") == 0 ) {
      obj->writeSTL( file );
      //obj->setFilename( filename );
    }
    return wrote;
  }

  /**
   * Cleanup
   */

  void edgeCleanup( DLFLObjectPtr obj ) {
    edgeCleanup( obj, obj->getEdgeList() );
  }

  void edgeCleanup( DLFLObjectPtr obj, const DLFLEdgePtrList& edges) {
    DLFLEdgePtrList deletion_list; // List of edges to be deleted
    DLFLEdgePtrList::const_iterator first, last;
    DLFLEdgePtr edge;
    DLFLFacePtr fp1, fp2;

    // First go through all edges and find out ones which have to be deleted.
    // If we do deletion on the fly, some normal computation could be affected
    // causing errors
    first = edges.begin(); last = edges.end();
    while ( first != last ) {
      edge = (*first); ++first;
      edge->getFacePointers(fp1,fp2);

      if ( fp1 != fp2 ) {
				Vector3d fp1n = fp1->computeNormal();
				Vector3d fp2n = fp2->computeNormal();
				if ( Abs(fp1n*fp2n-1.0) < 1.0e-5 ) {
					// Normals of faces on both sides of edge are same
					// This edge can be deleted
					deletion_list.push_back(edge);
				}
      } else
				deletion_list.push_back(edge);
    }

    first = deletion_list.begin(); last = deletion_list.end();
    while ( first != last ) {
      edge = (*first); ++first;
      deleteEdge(obj,edge,true);
    }
  }
  void edgeCleanup( DLFLObjectPtr obj, const DLFLEdgePtrArray& edges) {
    DLFLEdgePtrList deletion_list; // List of edges to be deleted
    DLFLEdgePtrList::iterator first, last;
    DLFLEdgePtr edge;
    DLFLFacePtr fp1, fp2;

    // First go through all edges and find out ones which have to be deleted.
    // If we do deletion on the fly, some normal computation could be affected
    // causing errors
    for (int i=0; i < edges.size(); ++i) {
      edge = edges[i];
      edge->getFacePointers(fp1,fp2);

      if ( fp1 != fp2 ) {
				Vector3d fp1n = fp1->computeNormal();
				Vector3d fp2n = fp2->computeNormal();
				if ( Abs(fp1n*fp2n-1.0) < 1.0e-5 ) {
					// Normals of faces on both sides of edge are same
					// This edge can be deleted
					deletion_list.push_back(edge);
				}
      } else
				deletion_list.push_back(edge);
    }

    first = deletion_list.begin(); last = deletion_list.end();
    while ( first != last ) {
      edge = (*first); ++first;
      deleteEdge(obj,edge,true);
    }
  }

  // Removes one of the edges in 2-gons
  void cleanup2gons( DLFLObjectPtr obj ) {
    // Go through list of faces. If a 2-gon is found,
    // delete one of the edges.
    // We don't check for infinite loop, since if an edge belongs
    // to a 2-gon deleting it will not create a new face
    DLFLFacePtrList face_list = obj->getFaceList( );
    DLFLFacePtrList::iterator ffirst=face_list.begin(), flast=face_list.end();
    DLFLFacePtr fp;
    while ( ffirst != flast ) {
      fp = (*ffirst); ++ffirst;
      if ( fp && fp->size() == 2 ) {
				// Face is a 2-gon.
				// Get one of the edges in the face and delete it
				DLFLFaceVertexPtr fvp = fp->front();
				DLFLEdgePtr ep = fvp->getEdgePtr();
				deleteEdge(obj,ep,true);
      }
    }
  }

  // Removes all winged (valence-2) vertices
  void cleanupWingedVertices( DLFLObjectPtr obj ) {
    // Go through list of vertices. If a valence-2 vertex is found,
    // find the 2 edges incident on that vertex. Insert an edge
    // between the other vertices of the 2 edges belonging to
    // the same face. Delete the edges incident on the valence-2 vertex
    // with cleanup to get rid of the point sphere.
    DLFLVertexPtrList vertex_list = obj->getVertexList( );
    DLFLVertexPtrList::iterator vfirst=vertex_list.begin(), vlast=vertex_list.end();
    DLFLVertexPtr vp;
    DLFLFaceVertexPtrArray fvparray;
    DLFLEdgePtrArray eparray;
    DLFLFaceVertexPtr fvp, pfvp, nfvp;
    while ( vfirst != vlast ) {
      vp = (*vfirst); ++vfirst;
      // If vp is a valence 2 vertex, it will be removed, so increment the iterator now itself
      if ( vp->valence() == 2 ) {
				vp->getFaceVertices(fvparray);
				vp->getEdges(eparray);

				// Pick the first face vertex in the array
				fvp = fvparray[0];

				// Find the previous and next corners to fvp
				pfvp = fvp->prev(); nfvp = fvp->next();

				if ( pfvp == nfvp ) {
					// We have a disconnected 2-gon (consisting of 2 faces, 2 vertices and 2 edges)
					// This component will be left as is. Deleting this component is tricky, since
					// we are traversing the vertex list and can't remove vertices arbitrarily
					// We can keep track of such cases and handle them outside the loop
				} else {
					// Insert an edge between pfvp and nfvp
					// We know that both belong to the same face
					insertEdgeCoFacial(obj,pfvp,nfvp);

					// Now delete the 2 edges incident on the valence-2 vertex with cleanup
					deleteEdge(obj,eparray[0],true);
					deleteEdge(obj,eparray[1],true);
				}

      }
    }
  }


  void splitValence2Vertices( DLFLObjectPtr obj, double offset) {
    // Split all valence 2 vertices into 2 vertices separated by given offset
    // If offset is negative the vertices are moved by a distance proportional to the
    // length of the edges.
    // Intended for use primarily after a Doo-Sabin type subdivision
    // where the original object has a valence 1 vertex
    // NOTE: Splitting a valence 2 vertex produces 2 new valence 2 vertices
    // They should not be split further. So we first create a list of valence 2
    // vertices and then go through that list and split them

    DLFLVertexPtrList::iterator vl_first, vl_last;
    DLFLVertexPtr vp = NULL;
    DLFLVertexPtrList splitvp;

    DLFLVertexPtrList vertex_list = obj->getVertexList();
    vl_first = vertex_list.begin(); vl_last = vertex_list.end();
    while ( vl_first != vl_last ) {
      vp = (*vl_first); ++vl_first;
      if ( vp->valence() == 2 ) 
				splitvp.push_back(vp);
    }

    vl_first = splitvp.begin(); vl_last = splitvp.end();
    while ( vl_first != vl_last ) {
      vp = (*vl_first); ++vl_first;
      DLFLEdgePtrArray eparray;
      vp->getEdges(eparray); // There will be 2 edges in the array

      DLFLEdgePtr e1 = eparray[0], e2 = eparray[1];
      DLFLVertexPtr e1v1, e1v2, e2v1, e2v2;

      e1->getVertexPointers(e1v1,e1v2);
      e2->getVertexPointers(e2v1,e2v2);

      Vector3d e1vec, e2vec;
      double e1len, e2len;
      e1vec = e1v2->coords - e1v1->coords; e1len = normalize(e1vec);
      e2vec = e2v2->coords - e2v1->coords; e2len = normalize(e2vec);
       
      if ( e1v2 == vp ) e1vec = -e1vec;
      if ( e2v2 == vp ) e2vec = -e2vec;

      // e1vec and e2vec will now be unit vectors along the 2 edges
      // with vp as the origin

      // Subdivide e2
      DLFLVertexPtr newvp = subdivideEdge(obj,e2);

      // Adjust coordinates of vp and newvp using vp as origin and separated
      // by given offset. Vertices are moved along directions calculated
      // using the edge vectors
      Vector3d origin = vp->coords;

      if ( offset > 0.0 ) {
				vp->coords = origin + offset * normalized(e1vec-e2vec);
				newvp->coords = origin + offset * normalized(e2vec-e1vec);
      } else {
				vp->coords = origin + 0.1 * e1len * normalized(e1vec-e2vec);
				newvp->coords = origin + 0.1 * e2len * normalized(e2vec-e1vec);
      }

      // Set type of vp and newvp so they don't get split again
      vp->setType(VTNewEdgePoint); newvp->setType(VTNewPoint);
    }
  }

	uint* createVertex( double x, double y, double z, DLFLObjectPtr &obj, bool set_type ) {
		Vector3d pos(x,y,z);
		DLFLFaceVertexPtr fv = obj->createPointSphere( pos );
		uint *id = new uint[2];
		id[0] = fv->getFaceID();
		id[1] = fv->getVertexID();
		return id;
	}

	void removeVertex( const DLFLObjectPtr &obj, uint vertId, uint faceId ) {
		if( !obj ) return;

		DLFLFacePtr fptr1 = obj->findFace( faceId );
		DLFLFaceVertexPtr fvptr1 = fptr1->findFaceVertex( vertId );
		obj->removePointSphere( fvptr1 );
	}
} // end namespace DLFL
