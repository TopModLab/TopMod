/* $Id: DLFLBase.cc,v 4.0 2003/12/26 01:58:52 vinod Exp $ */

// Basic operations on DLFL. Other operations are a combination of these operations

#include "DLFLObject.hh"
#include <math.h>
#include <assert.h>

DLFLVertexPtr DLFLObject :: findVertex(uint vid)
{
     // Find a vertex with the given vertex id. Return NULL if none exists
  DLFLVertexPtrList :: iterator first = vertex_list.begin(), last = vertex_list.end();
  DLFLVertexPtr sel = NULL;
  while ( first != last )
     {
       if ( (*first)->getID() == vid )
          {
            sel = (*first); break;
          }
       ++first;
     }
  return sel;
}

DLFLEdgePtr DLFLObject :: findEdge(uint eid)
{
     // Find an edge with the given edge id. Return NULL if none exists
  DLFLEdgePtrList :: iterator first = edge_list.begin(), last = edge_list.end();
  DLFLEdgePtr sel = NULL;
  while ( first != last )
     {
       if ( (*first)->getID() == eid )
          {
            sel = (*first); break;
          }
       ++first;
     }
  return sel;
}

DLFLFacePtr DLFLObject :: findFace(uint fid)
{
     // Find a face with the given face id. Return NULL if none exists
  DLFLFacePtrList :: iterator first = face_list.begin(), last = face_list.end();
  DLFLFacePtr sel = NULL;
  while ( first != last )
     {
       if ( (*first)->getID() == fid )
          {
            sel = (*first); break;
          }
       ++first;
     }
  return sel;
}

void DLFLObject :: addVertex(const DLFLVertex& vertex)
{
  addVertexPtr(vertex.copy());
}
     
void DLFLObject :: addVertex(DLFLVertexPtr vertexptr)
{
  addVertexPtr(vertexptr->copy());
}
     
void DLFLObject :: addEdge(const DLFLEdge& edge)
{
  addEdgePtr(edge.copy());
}
     
void DLFLObject :: addEdge(DLFLEdgePtr edgeptr)
{
  addEdgePtr(edgeptr->copy());
}
     
void DLFLObject :: addFace(const DLFLFace& face)
{
  addFacePtr(face.copy());
}

void DLFLObject :: addFace(DLFLFacePtr faceptr)
{
  addFacePtr(faceptr->copy());
}

void DLFLObject :: addMaterial(const DLFLMaterial& matl)
{
  addMaterialPtr(matl.copy());
}

void DLFLObject :: addMaterial(DLFLMaterialPtr matlptr)
{
  addMaterialPtr(matlptr->copy());
}

void DLFLObject :: deleteVertex(DLFLVertexPtr vertexptr)
{
     // A Vertex can be deleted only if it is a winged vertex - has only 2 edges incident on it
     // Deleting the vertex basically connects the 2 edges on either side, which involves, deleting
     // those 2 edges and adding a new edge.
  int valence = vertexptr->numEdges();

  if ( valence != 2 ) return;

     // Not yet implemented
}

void DLFLObject :: deleteVertex(uint vertex_index)
{
     // Find the VertexPtr for the given vertex_index from the VertexList and delete it
  if ( vertex_index > vertex_list.size() ) return;

  DLFLVertexPtrList :: iterator i = vertex_list.begin();
  advance(i,vertex_index);
  deleteVertex(*i);
}

void DLFLObject :: deleteEdge(DLFLEdgePtr edgeptr, bool cleanup)
{
  DLFLFaceVertexPtr fvpV1, fvpV2;
  DLFLFacePtr f1,f2;

  fvpV1 = NULL; fvpV2 = NULL;
  edgeptr->getFaceVertexPointers(fvpV1,fvpV2);

  f1 = fvpV1->getFacePtr(); f2 = fvpV2->getFacePtr();
  
  if ( f1 != f2 )
     {
          // Two edge sides belong to different faces

          // We'll keep one of the old faces and delete the other one.
          // We should keep the larger face and delete the smaller one
          // For now we'll assume that f1 is the larger face. Vertices from f2
          // will be added to f1. There will an overlap in face-vertices at the ends
          // of the edge

          // Reorder f1 so that it ends at fvpV1
       f1->reorder(fvpV1->next());

          // Reorder f2 so that it ends at fvpV2
       f2->reorder(fvpV2->next());

          // Remove fvpV1 from f1
       f1->deleteVertexPtr(fvpV1);

          // Go through vertices of f2 starting at the beginning and add all of them except fvpV2
          // Also remove them from f2
       DLFLFaceVertexPtr next, temp = NULL;
       temp = f2->front();
       while ( temp != fvpV2 )
          {
            next = temp->next();
            f2->deleteVertexPtr(temp);
            f1->addVertexPtr(temp);
            temp = next;
          }

          // Remove fvpV2 from f2
       f2->deleteVertexPtr(fvpV2);
       
          // Remove fvpV1 and fvpV2 from the face-vertex-lists of their vertices
       fvpV1->deleteSelfFromVertex();
       fvpV2->deleteSelfFromVertex();

          // Free memory for fvpV1 and fvpV2
       delete fvpV1; delete fvpV2;
       
          // The Edge can now be removed from the EdgeList
          // Free the pointer also since the edge_list owns the DLFLEdge pointed to by edgeptr
       edge_list.remove(edgeptr); delete edgeptr;

          // Destroy f2 and delete it from the face list
       f2->destroy();
       face_list.remove(f2); delete f2;
     }
  else
     {
          // Two edge sides belong to same face

          // Split the Face to get 2 new faces
          // Without loss of generality, f1 will be used to refer to the Face

          // 1 new face will be created.
       DLFLFacePtr nfp = new DLFLFace(f1->material());
       DLFLFaceVertexPtr fvpTemp, temp;
       
          // Reorder the face so that face starts at vertex after fvpV2
       f1->reorder(fvpV2->next());

          // Start at vertex after fvpV1, remove from face and add to new face till we reach fvpV2
          // Don't add fvpV2 unless fvpV2 is right after fvpV1, in which case we will have a point sphere
       fvpTemp = fvpV1->next();
       if ( fvpTemp == fvpV2 )
          {
               // Add fvpV2. nfp will be a point sphere. Add a copy since fvpV2 will be deleted later on
               // Since nfp will be a point sphere adding a copy will not affect any other pointers
            temp = nfp->addVertex(fvpV2);
            temp->addSelfToVertex();
            temp->setEdgePtr(NULL);
          }

          // Loop will execute only if nfp is not a point sphere
       while ( fvpTemp != fvpV2 )
          {
            temp = fvpTemp->next();
            f1->deleteVertexPtr(fvpTemp);
            nfp->addVertexPtr(fvpTemp);
            fvpTemp = temp;
          }
       
          // Remove fvpV2 from f1 and free the pointer. Remove fvpV2 from face-vertex-list of its vertex also
       f1->deleteVertexPtr(fvpV2); fvpV2->deleteSelfFromVertex(); delete fvpV2;

          // Now check if fvpV1 is the only vertex in f1. If so f1 becomes a point sphere
          // Don't delete fvpV1 from f1. Otherwise delete fvpV1 from f1 and free the pointer

       if ( fvpV1->next() != fvpV1 )
          {
               // Remove fvpV1 from the face-vertex-list of its vertex also
            f1->deleteVertexPtr(fvpV1); fvpV1->deleteSelfFromVertex(); delete fvpV1;
          }
       
          // The Edge can now be removed from the EdgeList
          // Free the pointer also, since edge_list owns the object pointed to by edgeptr
       edge_list.remove(edgeptr); delete edgeptr;
       
          // Add the new Face to the FaceList
       addFacePtr(nfp);

          // If f1 ends up being a point sphere, reset the EdgePtr field of it's only corner
       if ( f1->size() == 1 )
          {
            DLFLFaceVertexPtr fvp = f1->firstVertex();
               //DLFLVertexPtr vp = fvp->vertex;
               //vertex_list.remove(vp); delete vp;
            fvp->setEdgePtr(NULL);
               // f1 is now a PointSphere
          }

          // Check if we have to cleanup point-spheres
       if ( cleanup )
          {
               // If either f1 or nfp is a point-sphere, completely remove it from the
               // object and also the vertex in that face
            DLFLFaceVertexPtr fvp;
            DLFLVertexPtr vp;
            if ( f1->size() == 1 )
               {
                 fvp = f1->firstVertex(); vp = fvp->vertex;
                 vertex_list.remove(vp); delete vp;
                 f1->destroy(); face_list.remove(f1); delete f1;
               }
            if ( nfp->size() == 1 )
               {
                 fvp = nfp->firstVertex(); vp = fvp->vertex;
                 vertex_list.remove(vp); delete vp;
                 nfp->destroy(); face_list.remove(nfp); delete nfp;
               }
          }
     }
}

void DLFLObject :: deleteEdge(uint edge_index, bool cleanup)
{
     // Find the EdgePtr for the given edge_index from the EdgeList and delete it
  if ( edge_index > edge_list.size() ) return;

  DLFLEdgePtrList :: iterator i = edge_list.begin();
  advance(i,edge_index);
  deleteEdge(*i,cleanup);
}


void DLFLObject :: collapseEdge(DLFLEdgePtr edgeptr, bool cleanup)
{
     // Collapse an edge - merge two vertices into one after removing in-between edge
  if ( edgeptr == NULL ) return;

     // If the edge is a self-loop, just delete the edge with cleanup
  if ( edgeptr->isSelfLoop() )
     {
       deleteEdge(edgeptr,true);
       return;
     }

  DLFLFaceVertexPtr efvp1, efvp2;
  DLFLVertexPtr vp1, vp2;
  DLFLEdgePtr ep1next, ep1prev, ep2next, ep2prev;

  edgeptr->getFaceVertexPointers(efvp1,efvp2);
  vp1 = efvp1->getVertexPtr(); vp2 = efvp2->getVertexPtr();

     // Adjust coordinates of vp1 to be midpoint of collapsed edge
  vp1->setCoords(edgeptr->getMidPoint());

     // Find edges adjacent to specified edge.
     // If cleanup is required some of them will be deleted later on
  ep1next = (efvp1->next())->getEdgePtr(); ep1prev = (efvp1->prev())->getEdgePtr();
  ep2next = (efvp2->next())->getEdgePtr(); ep2prev = (efvp2->prev())->getEdgePtr();

     // Vertex 1 (vp1,efvp1) will remain and vertex 2 (vp2,efvp2) will be removed

     // First adjust edge pointer of efvp1 and correspondingly adjust the face-vertex pointer
     // of the edge to which efvp1 will point after removal of vertex 2
     // Also remove the FVP after efvp1 from the face
  DLFLFaceVertexPtr efvp1_next = efvp1->next();
  DLFLEdgePtr efvp1_next_ep = efvp1_next->getEdgePtr();

     // Remove efvp1_next from Edge, Vertex and Face and delete the pointer itself
  efvp1_next->deleteSelfFromEdge();
  efvp1_next->deleteSelfFromVertex();
  efvp1_next->deleteSelfFromFace();
  delete efvp1_next;

     // Reset edge pointer of efvp1 and make the edge point back to efvp1
  efvp1->setEdgePtr(efvp1_next_ep); efvp1->addSelfToEdge();

     // Remove efvp2 from Vertex and Face and delete the pointer itself.
     // Removal from Edge is not necessary since the Edge itself is going to be deleted
     // For the same reason no edge pointers have to be adjusted
  efvp2->deleteSelfFromVertex();
  efvp2->deleteSelfFromFace();
  delete efvp2;

     // Go through all remaining face-vertices referring to vp2 and make them point
     // to vp1 instead. Since vp2 is going to be deleted anyway, these face-vertices
     // need not be removed from vp2's list.
  DLFLFaceVertexPtrArray fvparray;
  vp2->getFaceVertices(fvparray);
  for (int i=0; i < fvparray.size(); ++i)
     {
       fvparray[i]->setVertexPtr(vp1);
       vp1->addToFaceVertexList(fvparray[i]);
     }

     // Delete Vertex 2 (vp2) from Vertex list and free memory
  vertex_list.remove(vp2); delete vp2;
  
     // Delete edge to be collapsed from edge list and free memory
  edge_list.remove(edgeptr); delete edgeptr;

     // Do cleanup of 2-gons if boolean flag is true
  if ( cleanup == true )
     {
       if ( ep1next->getOtherVertexPointer(vp1) == ep1prev->getOtherVertexPointer(vp1) )
          {
               // Remove ep1next
            deleteEdge(ep1next,true);
          }
       if ( ep2next->getOtherVertexPointer(vp1) == ep2prev->getOtherVertexPointer(vp1) )
          {
               // Remove ep2prev
            deleteEdge(ep2prev,true);
          }
     }
}

void DLFLObject :: collapseEdge(uint edge_index, bool cleanup)
{
     // Find the EdgePtr for the given edge_index from the EdgeList and collapse it
  if ( edge_index > edge_list.size() ) return;

  DLFLEdgePtrList :: iterator i = edge_list.begin();
  advance(i,edge_index);
  collapseEdge(*i,cleanup);
}


DLFLEdgePtr DLFLObject :: insertEdge1(DLFLFaceVertexPtr fvptr1, DLFLFaceVertexPtr fvptr2,
                                      bool set_type)
{
     // Insert an edge between 2 corners in the same face. Doesn't check if both
     // corners are in the same face or not
     // Insertion of the Edge will split the Face into 2 faces
  DLFLFacePtr fp = fvptr1->getFacePtr();
  DLFLMaterialPtr matl = fp->material();

  DLFLEdgePtr edgeptr;                                 // Pointer to the new Edge
  DLFLFacePtr newfaceptr;                              // Pointers to the new Faces
  DLFLFaceVertexPtr fvp, temp;
  DLFLEdgePtr ep;
  
     // Only 1 new face is created.
  newfaceptr = new DLFLFace(matl);

     // Use same type as fp for the new face. Useful for doo-Sabin subdivision
  newfaceptr->setType(fp->getType());
  
     // Set type if caller requested it
  if ( set_type )
     {
          // Reset types of the 2 FaceVertexes
       fvptr1->resetType(); fvptr2->resetType();
       fp->setType(FTNew); newfaceptr->setType(FTNew);
     }

     // Create the new face. Add a copy of fvptr1 to the new face
     // Update edge which originally pointed to fvptr1 to point to new face vertex
  temp = newfaceptr->addVertex(fvptr1); // Copy needs to be made
  temp->addSelfToVertex();
  ep = fvptr1->getEdgePtr();
  ep->resetFaceVertexPtr(fvptr1);
  ep->setNullFaceVertexPtr(temp);

  
     // Remove face-vertices from fvptr1 to fvptr2
     // from old face and add it to the new face
  fvp = fvptr1->next();
  while ( fvp != fvptr2 )
     {
          // Remove fvp from old face and add it to the new face
       temp = fvp->next();
       fp->deleteVertexPtr(fvp);
       newfaceptr->addVertexPtr(fvp);
       fvp = temp;
     }

     // Remove fvptr2 from old face
  fp->deleteVertexPtr(fvptr2);

     // Add copy of fvptr2 to old face after fvptr1
  temp = fp->insertAfter(fvptr1,fvptr2,true);
  temp->addSelfToVertex();
  
     // Add fvptr2 to new face
  newfaceptr->addVertexPtr(fvptr2);

     // Update edge which originally pointed to fvptr2 to point to the copy
  ep = fvptr2->getEdgePtr();
  ep->resetFaceVertexPtr(fvptr2);
  ep->setNullFaceVertexPtr(temp);
       

     // Add new face to face list
  addFacePtr(newfaceptr);

     // Create the new Edge and do necessary updates
  edgeptr = new DLFLEdge;
  edgeptr->setFaceVertexPointers(fvptr1,fvptr2);
  edgeptr->updateFaceVertices();
  addEdgePtr(edgeptr);
  if ( set_type ) edgeptr->setType(ETNew);

  return edgeptr;
}

DLFLEdgePtr DLFLObject :: insertEdge2(DLFLFaceVertexPtr fvptr1, DLFLFaceVertexPtr fvptr2,
                                      DLFLMaterialPtr matl)
{
     // Insert an edge between 2 corners belonging to different faces
     // Doesn't check if the corners belong to different faces
  DLFLFacePtr fp1 = fvptr1->getFacePtr();
  DLFLFacePtr fp2 = fvptr2->getFacePtr();

  DLFLEdgePtr newedgeptr;                              // The new Edge
  DLFLFacePtr newfaceptr;                              // The new Face
  DLFLFaceVertexPtr fvp, temp;
  int numverts1=0, numverts2=0;                        // No. of verts in each face

  numverts1 = fp1->size(); numverts2 = fp2->size();

     // Reorder face 1 so that it ends at fvptr1
  fp1->reorder(fvptr1->next());

     // Reorder face 2 so that it ends at fvptr2
  fp2->reorder(fvptr2->next());

     // "New" face will be the larger of the old faces
     // For now assume fp1 is the larger face
  newfaceptr = fp1;

     // If a material is specified use it, otherwise stick with material of fp1
  if ( matl ) newfaceptr->setMaterial(matl);

     // Add face-vertices from face 2 and remove them from face 2

     // Check if fp2 is a point-sphere. If so, fvptr2 should be there only once
  if ( numverts2 > 1 )
     {
          // Add copy of fvptr2. The edge which fvptr2 originally pointed to
          // must be updated to point to the new face vertex
       temp = newfaceptr->addVertex(fvptr2);
       temp->addSelfToVertex();
       DLFLEdgePtr ep = fvptr2->getEdgePtr();
       ep->resetFaceVertexPtr(fvptr2);
       ep->setNullFaceVertexPtr(temp);
     }
  

     // Go through all vertices in fp2 and add them to the new face and delete them from the old face
  fvp = fp2->front();
  do
     {
       temp = fvp->next();
       fp2->deleteVertexPtr(fvp);
       newfaceptr->addVertexPtr(fvp);
       fvp = temp;
     }
  while ( fp2->front() );

     // Check if fp1 is a point-sphere. If so, fvptr1 should be there only once
  if ( numverts1 > 1 )
     {
          // Add copy of fvptr1. The edge which fvptr1 originally pointed to
          // must be updated to point to the new face vertex
       temp = newfaceptr->addVertex(fvptr1);
       temp->addSelfToVertex();
       DLFLEdgePtr ep = fvptr1->getEdgePtr();
       ep->resetFaceVertexPtr(fvptr1);
       ep->setNullFaceVertexPtr(temp);
     }

     // Delete face 2 from the face list and free the pointer
  face_list.remove(fp2); delete fp2;


     // Create the new Edge and do necessary updates
  newedgeptr = new DLFLEdge;
  newedgeptr->setFaceVertexPointers(fvptr1,fvptr2);
  addEdgePtr(newedgeptr);
  newedgeptr->updateFaceVertices();

  return newedgeptr;
}

DLFLEdgePtr DLFLObject :: insertEdge(DLFLFaceVertexPtr fvptr1, DLFLFaceVertexPtr fvptr2, bool set_type,
                                     DLFLMaterialPtr matl)
{
  if ( coFacial(fvptr1,fvptr2) )
     {
       if ( fvptr1 != fvptr2 ) return insertEdge1(fvptr1,fvptr2,set_type);
       return NULL;
     }
  return insertEdge2(fvptr1,fvptr2,matl);
}

DLFLEdgePtr DLFLObject :: insertEdgeWithoutCheck(DLFLFaceVertexPtr fvptr1, DLFLFaceVertexPtr fvptr2, bool set_type,
                                                 DLFLMaterialPtr matl)
{
  if ( coFacial(fvptr1,fvptr2) ) return insertEdge1(fvptr1,fvptr2,set_type);
  return insertEdge2(fvptr1,fvptr2,matl);
}

DLFLVertexPtr DLFLObject :: subDivideEdge(DLFLEdgePtr edgeptr, bool set_type)
{
     // Sub-divide an Edge into 2 Edges. A new Vertex will be added at the midpoint
     // belonging to both Faces on either side of the original Edge
  DLFLFaceVertexPtr fvpV1, fvpV2;
  DLFLFacePtr f1,f2;

  fvpV1 = NULL; fvpV2 = NULL;

  edgeptr->getFaceVertexPointers(fvpV1,fvpV2);

  f1 = fvpV1->getFacePtr();  f2 = fvpV2->getFacePtr();

     // Edge subdivision will work whether the two Edge sides belong to different Faces
     // or not.
  DLFLVertexPtr nvp = new DLFLVertex;
       
  addVertexPtr(nvp);

     // Create the 2 new FaceVertexes
  DLFLFaceVertexPtr newfv1, newfv2;

  newfv1 = new DLFLFaceVertex; newfv2 = new DLFLFaceVertex;

     // Set the VertexPtr and FacePtr fields of the new FaceVertexes
  newfv1->setVertexPtr(nvp); newfv1->setFacePtr(f1);
  newfv2->setVertexPtr(nvp); newfv2->setFacePtr(f2);

     // Set type if caller requested it
  if ( set_type )
     {
          // Always set it to new Edge point. Caller can change it to a new Face point
          // at the appropriate places
       nvp->setType(VTNewEdgePoint);
       newfv1->setType(FVTNew);
       newfv2->setType(FVTNew);
     }
       
     // Insert the new FaceVertexes into the respective
     // Faces at the appropriate locations
  f1->insertAfter(fvpV1,newfv1,false);
  f2->insertAfter(fvpV2,newfv2,false);

     // Average the normals, coordinates, color and texture coordinates
  average(fvpV1,f1->nextFaceVertex(newfv1),newfv1); average(fvpV2,f2->nextFaceVertex(newfv2),newfv2);

       // Add the 2 new FaceVertexes to the list in Vertex
  nvp->addToFaceVertexList(newfv1); nvp->addToFaceVertexList(newfv2);

     // Create the 2 new Edges
  DLFLEdgePtr nep1, nep2;

  nep1 = new DLFLEdge; nep2 = new DLFLEdge;

  nep1->setFaceVertexPointers(fvpV1,newfv2);
  nep2->setFaceVertexPointers(newfv1,fvpV2);

     // Set type of new Edge if caller requested it
  if ( set_type )
     {
       nep1->setType(ETNew); nep2->setType(ETNew);
     }
       
     // Remove the existing Edge from the EdgeList
     // Free the pointer also
  edge_list.remove(edgeptr); delete edgeptr;

     // Add the 2 new Edges into the EdgeList
  addEdgePtr(nep1); addEdgePtr(nep2);

     // Update the EdgePtr fields of the 2 FaceVertexes
  newfv1->setEdgePtr(nep2); newfv2->setEdgePtr(nep1);
  fvpV1->setEdgePtr(nep1); fvpV2->setEdgePtr(nep2);

  return nvp;
}

DLFLVertexPtr DLFLObject :: subDivideEdge(uint edge_index)
{
     // Find the EdgePtr for the given edge_index from the EdgeList and sub-divide it
  if ( edge_index >= edge_list.size() ) return NULL;

  DLFLEdgePtrList :: iterator i = edge_list.begin();
  advance(i,edge_index);
  return subDivideEdge(*i);
}

void DLFLObject :: subDivideEdge(int num_divs, DLFLEdgePtr edgeptr, bool set_type)
{
     // Sub-divide an edge into several edges. Does this by repeatedly calling
     // subDivideEdge(edgeptr,set_type) which subdivides into 2 parts.
     // Coordinates of the new points are set using the returned vertex pointer
  if ( num_divs < 2 ) return;
  if ( num_divs > 2 )
     {
       DLFLVertexPtr newvptr;
       DLFLEdgePtr eptr;
       Vector3d p1,p2,p, dp;

       edgeptr->getEndPoints(p1,p2);
       dp = (p2-p1)/double(num_divs);
       eptr = edgeptr;
       for (int i=1; i < num_divs; ++i)
          {
            p = p1 + double(i)*dp;
            newvptr = subDivideEdge(eptr,set_type);
            newvptr->coords = p;

               // Find the last edge from the edge_list.
               // NOTE: This is not an arbitrary choice
               // The last edge in the list will be the edge from midpoint
               // to the second vertex (p1) of the original edge
            eptr = edge_list.back();
          }
     }
  else
     {
          // Simple call the regular subDivideEdge
       subDivideEdge(edgeptr,set_type);
     }
}

void DLFLObject :: subDivideEdge(int num_divs, uint edge_index)
{
     // Find the EdgePtr for the given edge_index from the EdgeList and sub-divide it
  if ( edge_index < edge_list.size() )
     {
       DLFLEdgePtrList :: iterator i = edge_list.begin();
       advance(i,edge_index);
       subDivideEdge(num_divs,*i);
     }
}

void DLFLObject :: trisectEdge(DLFLEdgePtr edgeptr, double num, bool scale, bool set_type)
{
     // Split the edge into 3. New points will be calculated based on scale factor
     // with respect to mid point of edge OR offset from end points
     // Boolean flag indicates if specified number is a scale factor or offset distance
     // Offset distance is clamped to lie between 0.01 and edgelength/2 - 0.01
     // Scale factor is clamped to lie between 0.01 and 0.99 (inclusive)

     // Store the end points of the edge
  DLFLVertexPtr evp1, evp2;
  edgeptr->getVertexPointers(evp1,evp2);

     // Find coordinates of the new points
  Vector3d ep1, ep2, mp1, mp2, mid;
  ep1 = evp1->coords; ep2 = evp2->coords;

  if ( scale == true )
     {
          // Given number is a scale factor
          // Clamp scale factor to lie between 0.01 and 0.99
       if ( num > 0.99 ) num = 0.99;
       else if ( num < 0.01 ) num = 0.01;

       mid = (ep1+ep2)*0.5;
       mp1 = num*ep1 + (1.0-num)*mid;
       mp2 = num*ep2 + (1.0-num)*mid;
     }
  else
     {
          // Given number is an offset distance from end points
          // Clamp offset distance to be between 0.01 and (edge_length/2)-0.01
       double maxoffset = edgeptr->length()/2.0 - 0.01;
       if ( num < 0.01 ) num = 0.01;
       else if ( num > maxoffset ) num = maxoffset;

       Vector3d evec = ep2 - ep1; normalize(evec);
       mp1 = ep1 + num*evec;
       mp2 = ep2 - num*evec;
     }

  DLFLVertexPtr mvp1, mvp2;

     // Subdivide edge into 2 first
     // Adjust coordinates of midpoint
  mvp1 = subDivideEdge(edgeptr); mvp1->coords = mp1;

     // Subdivide the last edge in the list into 2 again
     // Adjust coordinates of midpoint
  mvp2 = subDivideEdge(lastEdge()); mvp2->coords = mp2;

  if ( set_type )
     {
       mvp1->setType(VTNewPoint); mvp2->setType(VTNewPoint);
     }
}

void DLFLObject :: subDivideAllEdges(int num_divs, bool set_type)
{
     // Go through edge list and sub-divide all the old edges
  DLFLEdgePtrList :: iterator first;
  DLFLEdgePtr ep;
  int num_edges, num_old_edges;

  num_old_edges = edge_list.size(); num_edges = 0;
  first = edge_list.begin();
  while ( num_edges < num_old_edges )
     {
       ep = (*first); ++first; ++num_edges;
       subDivideEdge(num_divs,ep,set_type);
     }
}

void DLFLObject :: subDivideNonLoopingEdges(int num_divs, bool set_type)
{
     // Go through edge list and sub-divide all the old edges which are not self-loops
  DLFLEdgePtrList :: iterator first;
  DLFLEdgePtr ep;
  int num_edges, num_old_edges;

  num_old_edges = edge_list.size(); num_edges = 0;
  first = edge_list.begin();
  while ( num_edges < num_old_edges )
     {
       ep = (*first); ++first; ++num_edges;
       if ( !(ep->isSelfLoop()) ) subDivideEdge(num_divs,ep,set_type);
     }
}

void DLFLObject :: boundaryWalk(uint face_index)
{
     // Find the Face with the given face_index from the FaceList and do a boundary walk on it
  uint i=0;
  DLFLFacePtrList :: iterator first = face_list.begin();
  DLFLFacePtrList :: iterator last = face_list.end();
  DLFLFacePtr faceptr = NULL;

  if ( face_index > face_list.size() ) return;
  while ( first != last )
     {
       if ( i == face_index )
          {
            faceptr = (*first);
            break;
          }
       ++first; ++i;
     }
  faceptr->boundaryWalk();
}
  
void DLFLObject :: vertexTrace(uint vertex_index)
{
     // Find the Vertex with the given index in the VertexList and do a vertex trace on it
  uint i=0;
  DLFLVertexPtrList :: iterator first = vertex_list.begin();
  DLFLVertexPtrList :: iterator last = vertex_list.end();
  DLFLVertexPtr vertexptr = NULL;

  if ( vertex_index > vertex_list.size() ) return;
  while ( first != last )
     {
       if ( i == vertex_index )
          {
            vertexptr = (*first);
            break;
          }
       ++first; ++i;
     }
  vertexptr->vertexTrace();
}


/*
  $Log: DLFLBase.cc,v $
  Revision 4.0  2003/12/26 01:58:52  vinod
  Major version sync.

  Revision 3.3  2003/11/16 06:44:23  vinod
  Bug fix in trisectEdge. Typo when calculating edge vector

  Revision 3.2  2003/10/20 00:28:39  vinod
  Added check for self loops in edgeCollapse.
  Modified trisectEdge to allow trisecting using offset distance to calculate mid points

  Revision 3.1  2003/09/26 15:31:09  vinod
  Added trisectEdge method

  Revision 3.0  2003/09/19 16:06:47  vinod
  Major version update

  Revision 2.18  2003/09/19 16:03:35  vinod
  Added subDivideNonLoopingEdges method

  Revision 2.17  2003/08/24 23:10:49  vinod
  Added set_type parameter to subDivideAllEdges

  Revision 2.16  2003/03/12 07:36:30  vinod
  Added 2-gon cleanup option to collapseEdge

  Revision 2.15  2003/03/10 07:08:29  vinod
  Added collapseEdge method

  Revision 2.14  2002/12/17 03:53:40  vinod
  Fixed comment logs

  Revision 2.13  2002/12/17 03:18:37  vinod
  See comments for previous revision

  Revision 2.12  2002/12/17 03:09:39  vinod
  Clarified comment for multiple edge subdivide

  Revision 2.11  2002/12/17 02:44:59  vinod
  DLFLWindow.hh

  Revision 2.10  2002/04/02 22:43:56  vinod
  Removed unused debug code

  Revision 2.9  2001/11/11 21:55:19  vinod
  Added subDivideAllEdges

  Revision 2.8  2001/11/02 00:34:11  vinod
  Added option for multiple subdivisions in edge subdivide

  Revision 2.7  2001/10/22 22:47:00  vinod
  Added option to cleanup pointspheres in deleteEdge

  Revision 2.6  2001/10/06 18:37:47  vinod
  Added insertEdgeWithoutCheck. Added check for same corner in insertEdge

  Revision 2.5  2001/10/06 16:38:23  vinod
  Removed redundant code

  Revision 2.4  2001/09/28 05:48:28  vinod
  Set proper face-type for new face created during insertEdge

  Revision 2.3  2001/09/22 20:17:29  vinod
  Fixed bug in insertEdge when setting material pointer

  Revision 2.2  2001/09/15 22:35:27  vinod
  Modified insertEdge and deleteEdge subroutines to not create new faces
  unnecessarily and to use the modified FaceVertex data structures.

  Revision 2.1  2001/08/07 07:15:01  vinod
  subDivideEdge now returns the new Vertex

  Revision 2.0  2001/07/25 05:12:42  vinod
  Major version sync

  Revision 1.6  2001/07/25 04:46:29  vinod
  Removed unused variables. Modified insertEdge to take care of point-spheres
  Changed looping in insertEdge to make point-sphere modifications easier

  Revision 1.5  2001/07/20 03:56:29  vinod
  More changes from nextFaceVertex and prevFaceVertex to next() and prev()
  Modifications to allow Point Spheres.
  Removed unnecessary creation of temporary objects

  Revision 1.4  2001/07/18 07:11:57  vinod
  Cleaned up addVertex, addFace, addMaterial, addEdge functions
  to use the Ptr versions
  Modified deleteEdge function to use the next() and prev()
  functions of FaceVertex

  Revision 1.3  2001/07/16 04:31:12  vinod
  Modified to use next() and prev()

  Revision 1.2  2001/07/11 06:08:10  vinod
  insertEdge now returns the EdgePtr

  Revision 1.1  2001/05/14 19:46:01  vinod
  DLFL base operations

*/
