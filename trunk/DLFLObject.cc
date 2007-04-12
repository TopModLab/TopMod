/* $Id: DLFLObject.cc,v 4.0 2003/12/26 01:58:52 vinod Exp $ */

// Non-inline function definitions for class DLFLObject

#include "DLFLAux.hh"
#include "DLFLObject.hh"

uint DLFLObject :: suLastID = 0;
Transformation DLFLObject :: tr;
int DLFLObject :: bezierOrder = 3;

void DLFLObject :: bezierDefaults()
{
  // Compute default values for all control points in the object

  // Compute face centroids / control points
  DLFLFacePtrList :: const_iterator ffirst=face_list.begin(), 
                                    flast=face_list.end();
  while (ffirst != flast)
  {
    (*ffirst)->geomCentroid();
    ++ffirst;
  }

  // Compute edge centroids / control points
  DLFLEdgePtrList :: const_iterator efirst=edge_list.begin(), 
                                    elast=edge_list.end();
  while (efirst != elast)
  {
    (*efirst)->geomCentroid();
    ++efirst;
  } 

  // Compute edge control points using face centroids and rotation order
  DLFLFaceVertexPtr fvhead, fvcurrent;
  DLFLEdgePtr ecurrent;
  ffirst=face_list.begin();
  flast=face_list.end();
  DLFLControlPointArray cpArray;
  DLFLVertexPtr edgeVertex;
  DLFLControlPoint edgeCentroid, faceCentroid;

  while (ffirst != flast)
  {
    fvhead = (*ffirst)->front(); 
    faceCentroid = (*ffirst)->getGeomCentroid();
    ++ffirst;

    if (fvhead)
    {
      fvcurrent = fvhead;
      do
      {
        ecurrent = fvcurrent->getEdgePtr();
        edgeCentroid = ecurrent->getCentroid();

        if (fvcurrent == ecurrent->getFaceVertexPtr1())
        {  // Compute foward edge control points
          edgeVertex = ecurrent->getFaceVertexPtr1()->getVertexPtr();

          cpArray.clear(); 
          cpArray.reserve(bezierOrder-1);
          float t;
          for (int i=1; i < bezierOrder; ++i)
          {
            t = i / (float)(bezierOrder);
            cpArray.push_back( 
               (1-t)*edgeVertex->getCoords() + t*edgeCentroid
                   );
          }
          ecurrent->setControlPointForwardArray1(cpArray);

          cpArray.clear(); 
          cpArray.reserve(bezierOrder-1);
          for (int i=1; i < bezierOrder; ++i)
          {
            t = i / (float)(bezierOrder);
            cpArray.push_back(
               (1-t)*edgeCentroid + t*faceCentroid
                   );
          }
          ecurrent->setControlPointForwardArray2(cpArray);
          
        }
        else
        {  // Compute backward edge control points
          edgeVertex = ecurrent->getFaceVertexPtr2()->getVertexPtr();

          float t;
          cpArray.clear(); 
          cpArray.reserve(bezierOrder-1);
          for (int i=1; i < bezierOrder; ++i)
          {
            t = i / (float)(bezierOrder);
            //cpArray[i-1] = 
            cpArray.push_back(
              (1-t)*edgeVertex->getCoords() + t*edgeCentroid
                 );
          }
          ecurrent->setControlPointBackwardArray1(cpArray);

          cpArray.clear(); 
          cpArray.reserve(bezierOrder-1);
          for (int i=1; i < bezierOrder; ++i)
          {
            t = i / (float)(bezierOrder);
            cpArray.push_back( 
                     (1-t)*edgeCentroid + t*faceCentroid
                  );
          }
          ecurrent->setControlPointBackwardArray2(cpArray);
        }

        fvcurrent = fvcurrent->next();        
      } while (fvcurrent != fvhead);
    }
  }

  // Compute face vertex control points using already computed control points/verts
  DLFLVertexPtr fvPtr;
  DLFLControlPoint fcp, ecpfw, ecpbk;
  ffirst=face_list.begin();
  flast=face_list.end();
  DLFLControlPoint2dArray cp2dArray;

  while (ffirst != flast)
  {
    fvhead = (*ffirst)->front();
    fcp = (*ffirst)->getGeomCentroid();
    ++ffirst;

    if (fvhead)
    {
      fvcurrent = fvhead;
      do
      {
        // Get vertex
        if (fvcurrent == fvcurrent->getEdgePtr()->getFaceVertexPtr1())
          fvPtr = fvcurrent->getEdgePtr()->getFaceVertexPtr1()->getVertexPtr();
        else
          fvPtr = fvcurrent->getEdgePtr()->getFaceVertexPtr2()->getVertexPtr();

        // Get edge control points (to avoid tests just recreate)
        ecpfw = (fvPtr->getCoords() + fvcurrent->next()->getVertexPtr()->getCoords())
                / 2.0;
        ecpbk = (fvPtr->getCoords() + fvcurrent->prev()->getVertexPtr()->getCoords()) 
                / 2.0;

        // Interpolate between vertex, both edge centroids, and the face centroid
        float s, t;
        cp2dArray.clear();
        cp2dArray.reserve((bezierOrder-1)*(bezierOrder-1));
        for (int j=1; j < bezierOrder; ++j)
        {
          s = j / (float)(bezierOrder);

          for (int i=1; i < bezierOrder; ++i)
          {
            t = i / (float)(bezierOrder);
            cp2dArray.push_back(
                                  (1-s)*(1-t)*fvPtr->getCoords() +
                                  (1-s)*(  t)*ecpfw +
                                  (  s)*(  t)*fcp +
                                  (  s)*(1-t)*ecpbk
                               );
          }
        }

        fvcurrent->setControlPoint2dArray(cp2dArray);


        fvcurrent = fvcurrent->next();
      } while (fvcurrent != fvhead);
    }
  }
}

void DLFLObject :: dump(ostream& o) const
{
  o << "DLFLObject" << endl;

  int i=0;

  o << "VertexList" << endl;
  
  DLFLVertexPtrList :: const_iterator vfirst=vertex_list.begin(), vlast=vertex_list.end();
  
  while ( vfirst != vlast )
     {
       o << i << " : " << (*vfirst) << endl;
       (*vfirst)->dump(o);
       ++vfirst; ++i;
     }

  i = 0;
  o << "EdgeList" << endl;
  DLFLEdgePtrList :: const_iterator efirst=edge_list.begin(), elast=edge_list.end();
  while ( efirst != elast )
     {
       o << i << " : " << (*efirst) << endl;
       (*efirst)->dump(o);
       ++efirst; ++i;
     }
         
  i = 0;
  o << "FaceList" << endl;
  DLFLFacePtrList :: const_iterator ffirst=face_list.begin(), flast=face_list.end();
  while ( ffirst != flast )
     {
       o << i << " : " << (*ffirst) << endl;
       (*ffirst)->dump(o);
       ++ffirst; ++i;
     }
}

void DLFLObject :: splice(DLFLObject& object)
{
     // Combine 2 objects. The lists are simply spliced together.
     // Entities must be removed from the second object to prevent dangling pointers
     // when it is destroyed.
  vertex_list.splice(vertex_list.end(),object.vertex_list);
  edge_list.splice(edge_list.end(),object.edge_list);
  face_list.splice(face_list.end(),object.face_list);
  matl_list.splice(matl_list.end(),object.matl_list);
}

   // Reverse the orientation of all faces in the object
   // This also requires reversing all edges in the object
void DLFLObject :: reverse(void)
{
     // Reverse the edges first, since they depend on the ordering of the
     // original faces.
  DLFLEdgePtrList :: iterator efirst=edge_list.begin(), elast=edge_list.end();
  while ( efirst != elast )
     {
       (*efirst)->reverse();
       ++efirst;
     }
  
  DLFLFacePtrList :: iterator ffirst=face_list.begin(), flast=face_list.end();
  while ( ffirst != flast )
     {
       (*ffirst)->reverse();
       ++ffirst;
     }
}

void DLFLObject :: edgeCleanup(void)
{
  edgeCleanup(edge_list);
}

void DLFLObject :: edgeCleanup(const DLFLEdgePtrList& edges)
{
  DLFLEdgePtrList deletion_list; // List of edges to be deleted
  DLFLEdgePtrList::const_iterator first, last;
  DLFLEdgePtr edge;
  DLFLFacePtr fp1, fp2;

     // First go through all edges and find out ones which have to be deleted.
     // If we do deletion on the fly, some normal computation could be affected
     // causing errors
  first = edges.begin(); last = edges.end();
  while ( first != last )
     {
       edge = (*first); ++first;
       edge->getFacePointers(fp1,fp2);

       if ( fp1 != fp2 )
          {
            Vector3d fp1n = fp1->computeNormal();
            Vector3d fp2n = fp2->computeNormal();
            if ( Abs(fp1n*fp2n-1.0) < 1.0e-5 )
             {
                  // Normals of faces on both sides of edge are same
                  // This edge can be deleted
               deletion_list.push_back(edge);
             }
          }
       else
          deletion_list.push_back(edge);
     }

  first = deletion_list.begin(); last = deletion_list.end();
  while ( first != last )
     {
       edge = (*first); ++first;
       deleteEdge(edge,true);
     }
}

void DLFLObject :: edgeCleanup(const DLFLEdgePtrArray& edges)
{
  DLFLEdgePtrList deletion_list; // List of edges to be deleted
  DLFLEdgePtrList::iterator first, last;
  DLFLEdgePtr edge;
  DLFLFacePtr fp1, fp2;

     // First go through all edges and find out ones which have to be deleted.
     // If we do deletion on the fly, some normal computation could be affected
     // causing errors
  for (int i=0; i < edges.size(); ++i)
     {
       edge = edges[i];
       edge->getFacePointers(fp1,fp2);

       if ( fp1 != fp2 )
          {
            Vector3d fp1n = fp1->computeNormal();
            Vector3d fp2n = fp2->computeNormal();
            if ( Abs(fp1n*fp2n-1.0) < 1.0e-5 )
             {
                  // Normals of faces on both sides of edge are same
                  // This edge can be deleted
               deletion_list.push_back(edge);
             }
          }
       else
          deletion_list.push_back(edge);
     }

  first = deletion_list.begin(); last = deletion_list.end();
  while ( first != last )
     {
       edge = (*first); ++first;
       deleteEdge(edge,true);
     }
}

   // Removes one of the edges in 2-gons
void DLFLObject :: cleanup2gons(void)
{
     // Go through list of faces. If a 2-gon is found,
     // delete one of the edges.
     // We don't check for infinite loop, since if an edge belongs
     // to a 2-gon deleting it will not create a new face
  DLFLFacePtrList :: iterator ffirst=face_list.begin(), flast=face_list.end();
  DLFLFacePtr fp;
  while ( ffirst != flast )
     {
       fp = (*ffirst); ++ffirst;
       if ( fp->size() == 2 )
          {
               // Face is a 2-gon.
               // Get one of the edges in the face and delete it
            DLFLFaceVertexPtr fvp = fp->front();
            DLFLEdgePtr ep = fvp->getEdgePtr();
            deleteEdge(ep,true);
          }
     }
}


   // Removes all winged (valence-2) vertices
void DLFLObject :: cleanupWingedVertices(void)
{
     // Go through list of vertices. If a valence-2 vertex is found,
     // find the 2 edges incident on that vertex. Insert an edge
     // between the other vertices of the 2 edges belonging to
     // the same face. Delete the edges incident on the valence-2 vertex
     // with cleanup to get rid of the point sphere.
  DLFLVertexPtrList :: iterator vfirst=vertex_list.begin(), vlast=vertex_list.end();
  DLFLVertexPtr vp;
  DLFLFaceVertexPtrArray fvparray;
  DLFLEdgePtrArray eparray;
  DLFLFaceVertexPtr fvp, pfvp, nfvp;
  while ( vfirst != vlast )
     {
       vp = (*vfirst); ++vfirst;
          // If vp is a valence 2 vertex, it will be removed, so increment the iterator now itself
       if ( vp->valence() == 2 )
          {
            vp->getFaceVertices(fvparray);
            vp->getEdges(eparray);

               // Pick the first face vertex in the array
            fvp = fvparray[0];

               // Find the previous and next corners to fvp
            pfvp = fvp->prev(); nfvp = fvp->next();

            if ( pfvp == nfvp )
               {
                    // We have a disconnected 2-gon (consisting of 2 faces, 2 vertices and 2 edges)
                    // This component will be left as is. Deleting this component is tricky, since
                    // we are traversing the vertex list and can't remove vertices arbitrarily
                    // We can keep track of such cases and handle them outside the loop
               }
            else
               {
                    // Insert an edge between pfvp and nfvp
                    // We know that both belong to the same face
                 insertEdge1(pfvp,nfvp);

                    // Now delete the 2 edges incident on the valence-2 vertex with cleanup
                 deleteEdge(eparray[0],true);
                 deleteEdge(eparray[1],true);
               }

          }
     }
}

bool DLFLObject :: edgeExists(DLFLVertexPtr vptr1, DLFLVertexPtr vptr2)
{
     // Check if an edge exists between two given vertices
  bool edgeexists = false;

     // Try to find an edge from vptr1 to vptr2
  if ( vptr1->getEdgeTo(vptr2) != NULL ) edgeexists = true;

  return edgeexists;
}

void DLFLObject :: splitValence2Vertices(double offset)
{
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

  vl_first = vertex_list.begin(); vl_last = vertex_list.end();
  while ( vl_first != vl_last )
     {
       vp = (*vl_first); ++vl_first;
       if ( vp->valence() == 2 ) splitvp.push_back(vp);
     }

  vl_first = splitvp.begin(); vl_last = splitvp.end();
  while ( vl_first != vl_last )
     {
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
       DLFLVertexPtr newvp = subDivideEdge(e2);

          // Adjust coordinates of vp and newvp using vp as origin and separated
          // by given offset. Vertices are moved along directions calculated
          // using the edge vectors
       Vector3d origin = vp->coords;

       if ( offset > 0.0 )
          {
            vp->coords = origin + offset * normalized(e1vec-e2vec);
            newvp->coords = origin + offset * normalized(e2vec-e1vec);
          }
       else
          {
            vp->coords = origin + 0.1 * e1len * normalized(e1vec-e2vec);
            newvp->coords = origin + 0.1 * e2len * normalized(e2vec-e1vec);
          }

          // Set type of vp and newvp so they don't get split again
       vp->setType(VTNewEdgePoint); newvp->setType(VTNewPoint);
     }
}

void DLFLObject :: subDivideFace(DLFLFacePtr faceptr, bool usequads)
{
     // Subdivide a face into quads or triangles depending on flag
     // If quads are used, edges are subdivided, otherwise existing corners are used
  Vector3d geomcen, normalcen;
  Vector2d texcen;
  RGBColor colorcen;

  faceptr->getCentroids(geomcen,texcen,colorcen,normalcen);

  DLFLFaceVertexPtrArray endpoints;
  if ( usequads )
     {
          // Subdivide all edges in this face and store the corresponding newly created corners
          // which will be used as the end points of the new edges
       DLFLVertexPtr vp = NULL;
       DLFLEdgePtrArray edges;
  
       faceptr->getEdges(edges);
       endpoints.resize(edges.size(),NULL);
       for (int i=0; i < edges.size(); ++i)
          {
            vp = subDivideEdge(edges[i]);
            endpoints[i] = vp->getFaceVertexInFace(faceptr);
          }
     }
  else
     {
          // Use existing corners in face as end points of the new edges
       faceptr->getCorners(endpoints);
     }

     // Create point-sphere at centroid and set properties of face-vertex to
     // that of centroid of face.
  DLFLFaceVertexPtr cenfvp = createPointSphere(geomcen,faceptr->material());
  cenfvp->normal = normalcen; cenfvp->color = colorcen; cenfvp->texcoord = texcen;

     // Insert edges between corner in centroid and corners in the endpoints array
     // NOTE: Make sure centroid corner is specified first in the insertEdge call
  for (int i=0; i < endpoints.size(); ++i)
     {
       insertEdge(cenfvp,endpoints[i]);
     }
}

void DLFLObject :: subDivideFace(uint face_index, bool usequads)
{
     // Find the Face with the given face_index from the FaceList and sub-divide it
  if ( face_index > face_list.size() ) return;
  
  DLFLFacePtrList :: iterator i = face_list.begin();
  advance(i,face_index);
  subDivideFace(*i,usequads);
}


void DLFLObject :: subDivideFaces(DLFLFacePtrArray fparray, bool usequads)
{
     // Subdivide the given list of faces into quads or triangles
     // For triangles, we can just go through the list and subdivide each of them
     // For quads, since edges are subdivided and new points created, we want
     // to make sure that edges shared between two faces in the list are not
     // subdivided twice.

     // If there is only 1 face do regular face subdivision and return
  if ( fparray.size() == 1 )
     {
       subDivideFace(fparray[0],usequads);
       return;
     }
  if ( usequads )
     {
       Vector3d geomcen, normalcen;
       Vector2d texcen;
       RGBColor colorcen;
       DLFLVertexPtr vp;
       DLFLFaceVertexPtr cenfvp;
       DLFLFaceVertexPtrArray corners, vcorners;
       DLFLEdgePtrArray edges, vedges;
       DLFLFacePtr faceptr;
       DLFLEdgePtrList eptypereset; // List of edges whose type has to be reset
       DLFLFaceVertexPtrList fvptypereset; // List of corners whose type has to be reset
       for (int i=0; i < fparray.size(); ++i)
          {
            faceptr = fparray[i];

               // Subdivide all edges in this face, set type of new vertex when subdividing.
               // If an edge is a new edge, then we will not subdivide that edge
               // since it was created as a result of a previous edge subdivision
            faceptr->getEdges(edges);
            for (int i=0; i < edges.size(); ++i)
               {
                 if ( edges[i]->getType() != ETNew )
                    {
                      vp = subDivideEdge(edges[i],true);
                         // Find the edges and the corners associated with this vertex
                         // Add them to the list for resetting type later
                         // We can make use of the fact that there will only 2 edges
                         // and 2 corners associated with this vertex
                      vp->getEdges(vedges); vp->getFaceVertices(vcorners);
                      eptypereset.push_back(vedges[0]); eptypereset.push_back(vedges[1]); 
                      fvptypereset.push_back(vcorners[0]); fvptypereset.push_back(vcorners[1]); 
                    }
               }

               // Create point-sphere at centroid and set properties of face-vertex to
               // that of centroid of face.
            faceptr->getCentroids(geomcen,texcen,colorcen,normalcen);
            cenfvp = createPointSphere(geomcen,faceptr->material());
            cenfvp->normal = normalcen; cenfvp->color = colorcen; cenfvp->texcoord = texcen;

               // Insert edges between corner in centroid and the new corners in face
               // NOTE: Make sure centroid corner is specified first in the insertEdge call
            faceptr->getCorners(corners);
            for (int i=0; i < corners.size(); ++i)
               {
                 if ( corners[i]->getType() == FVTNew )
                    {
                         // Reset the type of the corner as well as the vertex
                         // Get both edges related to this corner and add them
                         // to the list for resetting type later.
                      corners[i]->resetType();
                      corners[i]->getVertexPtr()->resetType();
                      insertEdge(cenfvp,corners[i]);
                    }
               }
          }

          // Reset type of edges which were created by subdivision
       DLFLEdgePtrList::iterator epfirst = eptypereset.begin(), eplast = eptypereset.end();
       while ( epfirst != eplast )
          {
            (*epfirst)->resetType(); ++epfirst;
          }

          // Reset type of corners which were created by subdivision
       DLFLFaceVertexPtrList::iterator fvpfirst = fvptypereset.begin(), fvplast = fvptypereset.end();
       while ( fvpfirst != fvplast )
          {
            (*fvpfirst)->resetType(); ++fvpfirst;
          }
     }
  else
     {
          // Go through list and subdivide each face
       for (int i=0; i < fparray.size(); ++i)
          {
            subDivideFace(fparray[i],false);
          }
     }
}

void DLFLObject :: subDivideFaces(DLFLFacePtrList fplist, bool usequads)
{
     // NOTE: fplist parameter is to passed by value to ensure copy will be made
     // This is to allow subdivision of all faces in the mesh, in which case
     // the entire face list will be passed to this subroutine.

     // Subdivide the given list of faces into quads or triangles
     // For triangles, we can just go through the list and subdivide each of them
     // For quads, since edges are subdivided and new points created, we want
     // to make sure that edges shared between two faces in the list are not
     // subdivided twice.

     // If there is only 1 face do regular face subdivision and return
  if ( fplist.size() == 1 )
     {
       subDivideFace(fplist.front());
       return;
     }
  if ( usequads )
     {
       Vector3d geomcen, normalcen;
       Vector2d texcen;
       RGBColor colorcen;
       DLFLVertexPtr vp;
       DLFLFaceVertexPtr cenfvp;
       DLFLFaceVertexPtrArray corners, vcorners;
       DLFLEdgePtrArray edges, vedges;
       DLFLFacePtr faceptr;
       DLFLEdgePtrList eptypereset; // List of edges whose type has to be reset
       DLFLFaceVertexPtrList fvptypereset; // List of corners whose type has to be reset
       DLFLFacePtrList::iterator fpfirst, fplast;
       fpfirst = fplist.begin(); fplast = fplist.end();
       while ( fpfirst != fplast )
          {
            faceptr = (*fpfirst); ++fpfirst;

               // Subdivide all edges in this face, set type of new vertex when subdividing.
               // If an edge is a new edge, then we will not subdivide that edge
               // since it was created as a result of a previous edge subdivision
            faceptr->getEdges(edges);
            for (int i=0; i < edges.size(); ++i)
               {
                 if ( edges[i]->getType() != ETNew )
                    {
                      vp = subDivideEdge(edges[i],true);
                         // Find the edges and the corners associated with this vertex
                         // Add them to the list for resetting type later
                         // We can make use of the fact that there will only 2 edges
                         // and 2 corners associated with this vertex
                      vp->getEdges(vedges); vp->getFaceVertices(vcorners);
                      eptypereset.push_back(vedges[0]); eptypereset.push_back(vedges[1]); 
                      fvptypereset.push_back(vcorners[0]); fvptypereset.push_back(vcorners[1]); 
                    }
               }

               // Create point-sphere at centroid and set properties of face-vertex to
               // that of centroid of face.
            faceptr->getCentroids(geomcen,texcen,colorcen,normalcen);
            cenfvp = createPointSphere(geomcen,faceptr->material());
            cenfvp->normal = normalcen; cenfvp->color = colorcen; cenfvp->texcoord = texcen;

               // Insert edges between corner in centroid and the new corners in face
               // NOTE: Make sure centroid corner is specified first in the insertEdge call
            faceptr->getCorners(corners);
            for (int i=0; i < corners.size(); ++i)
               {
                 if ( corners[i]->getType() == FVTNew )
                    {
                         // Reset the type of the corner as well as the vertex
                         // Get both edges related to this corner and add them
                         // to the list for resetting type later.
                      corners[i]->resetType();
                      corners[i]->getVertexPtr()->resetType();
                      insertEdge(cenfvp,corners[i]);
                    }
               }
          }

          // Reset type of edges which were created by subdivision
       DLFLEdgePtrList::iterator epfirst = eptypereset.begin(), eplast = eptypereset.end();
       while ( epfirst != eplast )
          {
            (*epfirst)->resetType(); ++epfirst;
          }

          // Reset type of corners which were created by subdivision
       DLFLFaceVertexPtrList::iterator fvpfirst = fvptypereset.begin(), fvplast = fvptypereset.end();
       while ( fvpfirst != fvplast )
          {
            (*fvpfirst)->resetType(); ++fvpfirst;
          }
     }
  else
     {
          // Go through list and subdivide each face
       DLFLFacePtrList::iterator fpfirst, fplast;
       fpfirst = fplist.begin(); fplast = fplist.end();
       while ( fpfirst != fplast )
          {
            subDivideFace(*fpfirst,false); ++fpfirst;
          }
     }
}

void DLFLObject :: subDivideAllFaces(bool usequads)
{
     // NOTE: A copy of the list needs to be made since subdivision
     // will add new faces. Parameter has to passed by value to ensure copy will be made
  subDivideFaces(face_list,usequads);
}

/*
  Tile Texturing, written by Avneet, Ergun
*/
void DLFLObject :: assignTileTexEdgeFlags(int n)
{
     // Assign flags to edge corners based on tiling factor
     // Flags are stored in the first component of the texture coordinate in each face vertex
  int randomvariable1, randomvariable2;
  DLFLEdgePtrList :: const_iterator first = edge_list.begin(), last = edge_list.end();
  while ( first != last )
     {
       DLFLFaceVertexPtr fvp1,fvp2;     
       (*first)->getFaceVertexPointers(fvp1,fvp2);

       randomvariable1 = rand() % (n/2);
       randomvariable2 = rand() % 2;

       fvp1->texcoord[0] = 2*randomvariable1 + randomvariable2;
       fvp2->texcoord[0] = 2*randomvariable1 + ((randomvariable2+1) % 2);

       ++first;
     }
}

/*
  Tile Texturing, written by Avneet, Ergun
*/
void DLFLObject :: assignTileTexCoords(int n)
{
     // Compute the texture coordinates for each corner in each face

     // Assign edge flags first
  assignTileTexEdgeFlags(n);

     // Compute texture coordinates using the edge flags
  DLFLFaceVertexPtrArray corners;
  int flag[4];
  double u,v;
	 
  DLFLFacePtrList :: const_iterator first = face_list.begin(), last = face_list.end();
  while ( first != last )
     {
       (*first)->getCorners(corners);

       for (int i=0; i < corners.size(); ++i)
          {
            flag[i] = int(corners[i]->texcoord[0]);
          }
       for (int i=0; i < corners.size(); ++i)
          {
            u = (float)( n*flag[0] + flag[1]  + (int)( ((i+1)%4) / 2 ) ) / (n*n);

               // Subtract from 1.0 since image origin is at top-left, instead of bottom-left
            v = 1.0 - (float)( n*flag[2] + flag[3]  + (int)(i/2) ) / (n*n); 
		   
            corners[i]->texcoord.set(u,v);
          }
       ++first;
     }
}

void DLFLObject :: randomAssignTexCoords(void)
{
     // FOR QUADS ONLY - randomly assign texture coordinates from a unit square to the 4 corners
  DLFLFacePtrList :: iterator first, last;
  first = face_list.begin(); last = face_list.end();
  while ( first != last )
     {
       (*first)->randomAssignTexCoords();
       ++first;
     }
}

void DLFLObject :: createFace(const Vector3dArray& verts, DLFLMaterialPtr matl, bool set_type)
{
     // Create 2 new faces with the given vertex coordinates. The 2 faces will have the same
     // vertices and share the same edges, but will have opposite rotation orders.
     // This essentially creates a 2 manifold with 2 faces with no volume.
  int numverts = verts.size();
  DLFLFacePtr newface1, newface2;
  DLFLVertexPtr vptr, tempvptr;
  DLFLFaceVertex fv;
  DLFLFaceVertexPtr fvptr;

  if ( matl == NULL ) matl = firstMaterial();
  
  newface1 = new DLFLFace; newface2 = new DLFLFace;

  if ( set_type )
     {
       newface1->setType(FTNew); newface2->setType(FTNew);
     }
  
  for (int i=0; i < numverts; ++i)
     {
       vptr = new DLFLVertex(verts[i]); if ( i == 0 ) tempvptr = vptr;
       addVertexPtr(vptr);
       fv.setVertexPtr(vptr);
       newface1->addVertex(fv); newface2->addVertex(fv);
     }

     // Reverse one of the new faces
  newface2->reverse();

     // These updates have to be done before the reorder
  newface1->addFaceVerticesToVertices(); newface2->addFaceVerticesToVertices();

     // Reorder the second new face so that the first vertex is the first vertex in the array
  DLFLFaceVertexPtrList fvplist = tempvptr->getFaceVertexList();
  fvptr = fvplist.front();
  if ( fvptr->getFacePtr() != newface2 ) fvptr = fvplist.back();
  newface2->reorder(fvptr);
  
     // Add the new faces to the list
  addFacePtr(newface1); addFacePtr(newface2);
  newface1->setMaterial(matl); newface2->setMaterial(matl);

  newface1->computeNormal(); newface2->computeNormal();

     // Create the new edges
     // Traverse the 2 faces in opposite orders
  DLFLFaceVertexPtr head1, current1;
  DLFLFaceVertexPtr head2, current2;
  DLFLEdgePtr eptr;

  head1 = newface1->front(); head2 = newface2->back();

     // Assume that head1 and head2 are not NULL
  current1 = head1; current2 = head2;
  eptr = new DLFLEdge(current1,current2);
  eptr->updateFaceVertices();
  addEdgePtr(eptr);
  current1 = current1->next(); current2 = current2->prev();
  while ( current1 != head1 && current2 != head2 )
     {
       eptr = new DLFLEdge(current1,current2);
       eptr->updateFaceVertices();
       addEdgePtr(eptr);
       current1 = current1->next(); current2 = current2->prev();
     }
}

DLFLFaceVertexPtr DLFLObject :: createPointSphere(const Vector3d& v, DLFLMaterialPtr matl)
{
     // Create a point sphere - a face with only 1 vertex
  DLFLFacePtr newface = new DLFLFace();

  if ( matl == NULL ) matl = firstMaterial();

  DLFLVertexPtr vp = new DLFLVertex(v);
  DLFLFaceVertexPtr fvp = new DLFLFaceVertex(vp,NULL);
  fvp->addSelfToVertex();
  newface->addVertexPtr(fvp);
  newface->setMaterial(matl);

  addVertexPtr(vp);
  addFacePtr(newface);

  return fvp;
}

void DLFLObject :: boundingBox(Vector3d& min, Vector3d& max) const
{
  DLFLVertexPtrList :: const_iterator vf,vl;
  double minx,miny,minz;
  double maxx,maxy,maxz;
  double x,y,z;
  DLFLVertexPtr vptr;
  
  vf = vertex_list.begin(); vl = vertex_list.end();
  vptr = (*vf); ++vf;

  vptr->coords.get(minx,miny,minz);
  maxx = minx; maxy = miny; maxz = minz;
  
  while ( vf != vl )
     {
       vptr = (*vf); ++vf;
       vptr->coords.get(x,y,z);

       (x < minx) ? minx = x : ( (x > maxx) ? maxx = x : 1);
       (y < miny) ? miny = y : ( (y > maxy) ? maxy = y : 1);
       (z < minz) ? minz = z : ( (z > maxz) ? maxz = z : 1);
     }
  min.set(minx,miny,minz); max.set(maxx,maxy,maxz);
}

/*
  $Log: DLFLObject.cc,v $
  Revision 4.0  2003/12/26 01:58:52  vinod
  Major version sync.

  Revision 3.3  2003/11/29 20:37:17  vinod
  Added subDivideFaces method with linked list parameter.
  Added subDivideAllFaces method

  Revision 3.2  2003/11/29 19:12:08  vinod
  Added cleanupWingedVertices and subDivideFaces methods

  Revision 3.1  2003/11/21 15:25:14  vinod
  Added cleanup2gons method

  Revision 3.0  2003/09/19 16:06:47  vinod
  Major version update

  Revision 2.35  2003/07/23 06:25:07  vinod
  Fixed 0 comparison and made small changes in edgeCleanup methods

  Revision 2.34  2003/04/30 18:05:47  vinod
  Moved reverse method from header file. Fixed it to reverse edges also

  Revision 2.33  2003/04/25 15:44:32  vinod
  Added splice method

  Revision 2.32  2003/04/18 21:30:23  vinod
  Removed debugging print statements

  Revision 2.31  2003/04/04 06:28:14  vinod
  Added edgeCleanup method

  Revision 2.30  2003/01/25 19:43:35  vinod
  Moved connect and extrude methods to separate files

  Revision 2.29  2003/01/05 21:12:09  vinod
  Removed unused variables

  Revision 2.28  2002/12/30 17:28:15  vinod
  Added splitValence2Vertices

  Revision 2.27  2002/12/19 23:16:02  vinod
  Adjusted calculation of v texture coordinate for tile texturing
  to account for reversed orientation of image in Fl_Image class.

  Revision 2.26  2002/12/18 16:26:32  vinod
  Removed unused variables in stellateFace

  Revision 2.25  2002/12/18 16:22:54  vinod
  Added face subdivision and tile texturing methods

  Revision 2.24  2002/12/17 03:53:40  vinod
  Fixed comment logs

  Revision 2.23  2002/12/17 03:18:37  vinod
  See comments for previous revision

  Revision 2.22  2002/12/17 02:58:34  vinod
  Additions by Paul
  dualConnectFaces, extrudeDualFace, stellateFace

  Revision 2.21  2002/12/17 02:44:59  vinod
  DLFLWindow.hh

  Revision 2.20  2002/11/24 00:52:02  vinod
  Added connectEdges version without loopcheck

  Revision 2.19  2002/10/31 06:20:21  vinod
  Fixed connectFaces closest vertex mode to choose proper closest vertices in 2 faces

  Revision 2.18  2002/10/25 16:27:54  vinod
  Removed unused variables

  Revision 2.17  2002/10/22 21:22:50  vinod
  Bug fix for taking care of C++ std. for counter variable scope in for loops

  Revision 2.16  2002/04/28 22:15:15  vinod
  Added variant of edgeExists to check two DLFLVertexes

  Revision 2.15  2001/12/21 19:02:23  vinod
  Made extrudeDS use full doo-sabin type interpolation

  Revision 2.14  2001/12/17 17:35:39  vinod
  Added extrudeFaceDS - dooSabin type extrude

  Revision 2.13  2001/12/12 18:21:32  vinod
  Added option of choosing closest corners for creating hole/handle.
  Added option for multiple extrusions of the same face

  Revision 2.12  2001/12/05 18:15:02  vinod
  Added multiple segment option for connectFaces

  Revision 2.11  2001/11/02 00:34:48  vinod
  Added option for extra twists and multiple edge subdivisions

  Revision 2.10  2001/10/22 22:51:05  vinod
  Added parameter to vary number of connectionst in hermitian handle code
  Added material parameter to createFace in several places.

  Revision 2.9  2001/10/15 23:25:07  vinod
  Added checks for unsolvable rotations - when axis of rotation can't be
  determined.
  Changed calculation of rotation plane - bisector plane of the 2
  polygons. Made changes to adjust for change in resolvePolygon, which now
  calculates and stores actual angles instead of differences.

  Revision 2.8  2001/10/13 18:17:30  vinod
  Added cast to double for numsegs and i. Removed redundant normalize(v)

  Revision 2.7  2001/10/06 18:37:04  vinod
  Added check flag to see if edge already exists in connecEdges

  Revision 2.6  2001/10/06 16:39:57  vinod
  Added missing addSelfToVertex in createPointSphere

  Revision 2.5  2001/09/28 17:45:45  vinod
  createPointSphere now returns the FaceVertex

  Revision 2.4  2001/09/28 05:51:46  vinod
  Removed extra calls to add faces to materials. Added connectEdges method

  Revision 2.3  2001/09/22 20:18:00  vinod
  Changed extrude to use face_list.back() instead of using reverse_iterators

  Revision 2.2  2001/09/19 03:57:55  vinod
  Added rotation and scaling for extrude

  Revision 2.1  2001/09/15 22:40:11  vinod
  Added set_type parameter to createFace

  Revision 2.0  2001/07/25 05:12:44  vinod
  Major version sync

  Revision 1.14  2001/07/16 04:34:49  vinod
  Modified to use new list structure. Added createPointSphere

  Revision 1.13  2001/07/11 06:07:03  vinod
  insertEdge functions now return the EdgePtr. Added randomAssignTexCoords

  Revision 1.12  2001/05/17 07:40:46  vinod
  Bug fix numverts/numsegs in hermite. Cleaned up extrude

  Revision 1.11  2001/05/17 05:11:42  vinod
  Moved Hv to DLFLAux.cc
  hermiteConnect and bezierConnect now take numsegs and weights as parameters
  Both now use createFace. hermite does proper interpolation without change
  in size of face. bezier does old style interpolation

  Revision 1.10  2001/05/15 21:08:54  vinod
  Added extrudeFace(DLFLFacePtr) which uses pre-computed aux coords

  Revision 1.9  2001/05/15 19:39:29  vinod
  Added createFace. extrude now uses createFace and computes normal before extrude along normal

  Revision 1.8  2001/05/14 20:01:43  vinod
  Moved base operations to DLFLBase.cc

  Revision 1.7  2001/04/25 20:02:40  vinod
  Added extrudeFace(DLFLFacePtr), bezier/hermiteConnectFaces

  Revision 1.6  2001/04/08 21:20:17  vinod
  Added findVertex/Edge/Face, extrudeFace

  Revision 1.5  2000/12/13 22:23:01  vinod
  Added deleteVertex

  Revision 1.4  2000/11/22 07:42:02  vinod
  Added connectFaces subroutine

  Revision 1.3  2000/11/21 19:21:01  vinod
  Added dump() method. Split insertEdge into 2 methods.
  Moved subdiv subroutines to separate file

  Revision 1.2  2000/11/02 22:17:53  vinod
  Update insertEdge to set material for the new faces correctly

  Revision 1.1.1.1  2000/11/02 19:32:43  vinod
  New DLFL Sources

*/
