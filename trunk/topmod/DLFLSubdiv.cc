/* $Id: DLFLSubdiv.cc,v 4.8 2004/01/20 22:17:40 vinod Exp $ */

/*
  Subdivision subroutines for DLFL
*/

#include "DLFLObject.hh"
#include "DLFLAux.hh"

void DLFLObject :: loopSubDivide(void)
{
     // Perform Loop subdivision

     // For every edge compute the new point coordinates and store in the aux coord
  DLFLEdgePtrList :: iterator efirst, elast;
  DLFLEdgePtr ep;
  DLFLFaceVertexPtr fvp1, fvp2, fvp1p1, fvp1n2, fvp2p1, fvp2n2;
  Vector3d p1,p2,p1p1,p1n2,p2p1,p2n2;
  Vector3d newpt;
  int num_old_edges = 0;
  efirst = edge_list.begin(); elast = edge_list.end();
  while ( efirst != elast )
     {
       ep = (*efirst); ++efirst; ++num_old_edges;
       ep->getFaceVertexPointers(fvp1,fvp2);
       fvp1p1 = fvp1->prev(); fvp1n2 = (fvp1->next())->next();
       fvp2p1 = fvp2->prev(); fvp2n2 = (fvp2->next())->next();
       p1 = fvp1->getVertexCoords(); p2 = fvp2->getVertexCoords();
       p1p1 = fvp1p1->getVertexCoords(); p2p1 = fvp2p1->getVertexCoords();
       p1n2 = fvp1n2->getVertexCoords(); p2n2 = fvp2n2->getVertexCoords();
       newpt = (p1+p2)*3.0/8.0 + (p1p1+p1n2+p2p1+p2n2)*1.0/16.0;
       ep->setAuxCoords(newpt);
     }

     // For every vertex compute the new point coordinates and adjust the coordinate
  DLFLVertexPtrList :: iterator vfirst, vlast;
  DLFLVertexPtr vp;
  DLFLFaceVertexPtrArray fvparray;
  Vector3d op, p;
  int valence, num_old_verts=0;
  double beta;
  vfirst = vertex_list.begin(); vlast = vertex_list.end();
  while ( vfirst != vlast )
     {
       vp = (*vfirst); ++vfirst; ++num_old_verts;
       vp->getFaceVertices(fvparray);
       valence = fvparray.size();
       if ( valence > 0 )
          {
            p = vp->coords;
            op.reset();
            for (int i=0; i < valence; ++i)
               op += (fvparray[i]->next())->getVertexCoords();

            beta = ( 0.625 - sqr( 0.375 + 0.25 * cos( 2.0*M_PI/double(valence) ) ) ) / double(valence);

            vp->coords = op * beta + (1.0 - valence*beta)*p;
          }
     }

     // Subdivide each old edge and set coordinate of new point to be the aux coord
  int count=0;
  efirst = edge_list.begin(); elast = edge_list.end();
  while ( efirst != elast && count < num_old_edges )
     {
       ep = (*efirst); ++efirst; ++count;
       newpt = ep->getAuxCoords(); ep->resetAuxCoords();
       vp = subDivideEdge(ep); vp->coords = newpt;
     }

     // Connect newly created midpoints in each face
     // Go through all old vertices (before edge subdivision) and
     // go through all corners for each vertex and connect previous and next corners
  vfirst = vertex_list.begin(); vlast = vertex_list.end();
  count = 0;
  while ( vfirst != vlast && count < num_old_verts )
     {
       vp = (*vfirst); ++vfirst; ++count;
       vp->getFaceVertices(fvparray);
       for (int i=0; i < fvparray.size(); ++i)
          insertEdge(fvparray[i]->prev(),fvparray[i]->next());
     }
}

void DLFLObject :: checkerBoardRemeshing(double thickness)
{
     // Create a checkerboard remeshing for entire object.
     // The code is similar to that for creating the remeshing faces for the Menger Sponge
     // Thickness is a fraction of the edge length for each edge
     // It should be between 0.0 and 0.5. Values outside this range will produce weird results
     // Boundary values produce coincident vertices.
     // If thickness is negative a default value of 0.25 is assumed
     // If thickness is > 0.5 it is clamped to 0.5
  if ( thickness < 0.0 ) thickness = 0.25;
  if ( thickness > 0.5 ) thickness = 0.5;

     // Use the extrudeFacePlanarOffset method to create the central faces for each original face
  int num_old_verts = vertex_list.size();
  int num_old_faces = face_list.size();
  int num_old_edges = edge_list.size();
  int count;
  DLFLFacePtrList :: iterator ffirst, flast;
  DLFLFacePtr fp;

  ffirst = face_list.begin(); flast = face_list.end();
  count = 0;
  while ( ffirst != flast && count < num_old_faces )
     {
       fp = (*ffirst); ++ffirst; ++count;
       extrudeFacePlanarOffset(fp,0,0,thickness,true);
     }

     // Trisect all the old edges using the same fractional thickness
     // Set the type of the new points for use later
  DLFLEdgePtrList :: iterator efirst = edge_list.begin();
  DLFLEdgePtr ep;
  count = 0;
  while ( count < num_old_edges )
     {
       ep = (*efirst); ++efirst; ++count;
       trisectEdge(ep,thickness*ep->length(),false,true);
     }

     // Complete the remeshing by inserting edges
     // Go through all the old vertices (before extrude and trisect)
     // For each vertex, go through the face-vertex list. For each face-vertex
     // in that list, connect the previous and next face-vertices with an edge
     // After doing that, find edges which do not connect to the points created
     // by trisection and delete them
  DLFLVertexPtrList :: iterator vfirst, vlast;
  DLFLVertexPtr vp, ovp;
  DLFLFaceVertexPtrArray fvparray;
  DLFLFaceVertexPtr fvp;
  DLFLEdgePtrArray eparray;

  vfirst = vertex_list.begin(); vlast = vertex_list.end();
  count = 0;
  while ( count < num_old_verts )
     {
       vp = (*vfirst); ++vfirst; ++count;
       vp->getFaceVertices(fvparray);
       for (int i=0; i < fvparray.size(); ++i)
          {
            fvp = fvparray[i];
            insertEdge1(fvp->prev(),fvp->next());
          }
       vp->getEdges(eparray);
       for (int i=0; i < eparray.size(); ++i)
          {
            ep = eparray[i];
            ovp = ep->getOtherVertexPointer(vp);
            if ( ovp->getType() != VTNewPoint ) deleteEdge(ep);
            else ovp->resetType(); // Reset the type to avoid complications in subsequent operations
          }
     }
}

  
void DLFLObject :: simplestSubDivide(void)
{
     // Simplest subdivision (halfway)

     // Keep track of number of old vertices before edges are subdivided
  int num_old_verts = vertex_list.size();

     // Subdivide all the edges.
  subDivideAllEdges();

     // All old edges will have to be deleted after new edges have been inserted
     // Keep track of number of old edges before we insert any new edges
  int num_old_edges = edge_list.size();

     // Go through vertex list. For all old vertices (before edge subdivision)
     // go through the face-vertex list and insert edges between previous and
     // next corners to each face-vertex.

  DLFLVertexPtrList :: iterator vl_first, vl_last;
  vl_first = vertex_list.begin(); vl_last = vertex_list.end();

  DLFLFaceVertexPtrList fvplist;
  DLFLFaceVertexPtrList :: iterator fvp_first,fvp_last;
  DLFLFaceVertexPtr fvp1, fvp2;
  DLFLMaterialPtr matl = (face_list.front())->material();
  int num_verts = 0;
  while ( vl_first != vl_last && num_verts < num_old_verts )
     {
       fvplist = (*vl_first)->getFaceVertexList(); ++vl_first; ++num_verts;  

       fvp_first = fvplist.begin(); fvp_last = fvplist.end();
       while ( fvp_first != fvp_last )
          {
            fvp2 = fvp1 = (*fvp_first); ++fvp_first;

               // Insert an edge between previous and next corners
            fvp1 = fvp1->next(); fvp2 = fvp2->prev();
            insertEdge(fvp1,fvp2,false,matl);
          }
     }

     // Delete the old edges
  DLFLEdgePtrList :: iterator el_first = edge_list.begin();
  DLFLEdgePtrList :: iterator el_last = edge_list.end();
  DLFLEdgePtr ep = NULL;
  int num_edges = 0; 
  while ( el_first != el_last && num_edges < num_old_edges )
     {
       ep = (*el_first); ++el_first; ++num_edges;
       deleteEdge(ep,true);
     }
}

void DLFLObject :: vertexCuttingSubDivide(double offset)
{
     // Vertex-cutting subdivision

     // If the offset is 0.5 this is the same as simplest subdivision
  if ( areEqual(offset,0.5) )
     {
       simplestSubDivide();
       return;
     }

     // Subdivision makes sense only if the offset is non-zero
  if ( isNonZero(offset) == false ) return;

     // Keep track of number of old vertices before edges are subdivided
  int num_old_verts = vertex_list.size();

     // Go through edge list and sub-divide all the old edges
     // into 3 parts using the offset parameter for the length ratio
  DLFLEdgePtrList :: iterator first = edge_list.begin();
  DLFLEdgePtr ep;
  int num_edges = 0, num_old_edges = edge_list.size();
  Vector3d p1,p2;
  DLFLVertexPtr vp;

  while ( num_edges < num_old_edges )
     {
       ep = (*first); ++first; ++num_edges;

          // Get end points (coordinates) of the edge
       ep->getEndPoints(p1,p2);
  
          // Subdivide the edge once and set coordinates of new point
       vp = subDivideEdge(ep); vp->coords = p1 + offset*(p2-p1);

          // Get the last edge from the edge list.
          // This will be the edge connected to the second vertex, p2
       ep = edge_list.back();

          // Subdivide this edge and set coordinates of new point
       vp = subDivideEdge(ep); vp->coords = p2 + offset*(p1-p2);
     }


     // Go through vertex list. For all old vertices (before edge subdivision)
     // go through the face-vertex list and insert edges between previous and
     // next corners to each face-vertex.
     // All old corner edges will have to be deleted after new edges have been inserted

  DLFLVertexPtrList :: iterator vl_first, vl_last;
  vl_first = vertex_list.begin(); vl_last = vertex_list.end();

  DLFLFaceVertexPtrList fvplist;
  DLFLFaceVertexPtrList :: iterator fvp_first, fvp_last;
  DLFLFaceVertexPtr fvp1, fvp2;
  DLFLMaterialPtr matl = (face_list.front())->material();
  int num_verts = 0;

  while ( vl_first != vl_last && num_verts < num_old_verts )
     {
       vp = *vl_first; ++vl_first; ++num_verts; 

       fvplist = vp->getFaceVertexList();
       fvp_first = fvplist.begin(); fvp_last = fvplist.end();
       while ( fvp_first != fvp_last )
          {
            fvp2 = fvp1 = (*fvp_first); ++fvp_first;

               // Insert an edge between previous and next corners
            fvp1 = fvp1->next(); fvp2 = fvp2->prev();
            insertEdge(fvp1,fvp2,false,matl);
          }

          // Delete the old corner edges
       DLFLEdgePtr * ep = NULL;
       num_edges = vp->getEdges(&ep); 

       for ( int i=0; i < num_edges; i++ )
          deleteEdge(ep[i],true); 
       delete [] ep;
     }
}

void DLFLObject :: pentagonalSubDivide2(double scale_factor)
{
     // Pentagonal subdivision - Version 2

     // First sub-divide all edges in the object.
     // Type flag NEED NOT be set for new vertices
     // The head pointer in each face will not change during edge subdivision
     // So we don't need to use the type flag.
  subDivideAllEdges(2,false);

     // Go through each face and create a new face within each face using the
     // midpoints of the old edges, i.e. the newly created points after above
     // subdivision of all edges
  
  DLFLFacePtrList :: iterator fl_first, fl_last;
  DLFLFacePtr fp, ifp;
  DLFLFaceVertexPtr head,fvp,ifvp;
  DLFLFaceVertexPtrArray fvplist1, fvplist2; // List of face-vertices which have to be connected
  int num_faces, num_old_faces, face_valence;
  Vector3dArray vertex_coords;

     // Reserve space in the two arrays for making connections. The size will be twice the number
     // of original edges. i.e. number of edges after subdividing all edges
  int num_edges = edge_list.size();
  fvplist1.reserve(num_edges); fvplist2.reserve(num_edges);

  fl_first = face_list.begin(); fl_last = face_list.end();
  num_old_faces = face_list.size(); num_faces = 0;
     // New faces will be created in the loop, so make sure those aren't traversed
  while ( fl_first != fl_last && num_faces < num_old_faces ) 
     {
       fp = (*fl_first);

          // If a face had less than 3 sides originally we don't include it.
       face_valence = fp->size();
       if ( face_valence >= 6 )
          {
               // Start from a new vertex
               // The head of a face will always be an old vertex since edge subdivision
               // does not change the head pointer of any face
            head = fp->front(); head = head->next();

               // Collect coordinates of alternate vertices starting from fvp
               // for creating the new face
            vertex_coords.clear();
            vertex_coords.reserve(face_valence/2);
            fvp = head;
            vertex_coords.push_back(fvp->getVertexCoords());
            fvp = fvp->next(); fvp = fvp->next();
            while ( fvp != head )
               {
                 vertex_coords.push_back(fvp->getVertexCoords());
                 fvp = fvp->next(); fvp = fvp->next();
               }

               // Scale the coordinates of the new face by given scale factor
            scale(vertex_coords,scale_factor);

               // Create the new face from the collected and scaled midpoints
            createFace(vertex_coords);
            
               // Find the inward facing face of the two newly created ones
            ifp = face_list.back();
            
               // Go through original face and the newly created inward facing face
               // and store corners in the array for making connections afterwards.
               // Only corners which are tagged as new in the old face will be used
               // We can reuse the 'head' variable initialized above
               // ifp has to be traversed in the reverse direction
               // Reset the type of fvp so it doesn't affect other algorithms later on
            fvp = head; ifvp = ifp->front();
            fvplist1.push_back(fvp); fvplist2.push_back(ifvp);
            fvp->resetType();
            fvp = fvp->next(); fvp = fvp->next(); ifvp = ifvp->prev();
            while ( fvp != head )
               {
                 fvplist1.push_back(fvp); fvplist2.push_back(ifvp);
                 fvp->resetType();
                 fvp = fvp->next(); fvp = fvp->next(); ifvp = ifvp->prev();
               }
          }
       ++fl_first; ++num_faces;
     }

     // Go through fvplist1 and fvplist2 and insert edges between corresponding face-vertices
  for (int i=0; i < fvplist1.size(); ++i)
     {
       if ( fvplist1[i] != NULL && fvplist2[i] != NULL )
          insertEdge(fvplist1[i],fvplist2[i]);
       else
          cout << "NULL pointers found! i = " << i << " "
               << fvplist1[i] << " " << fvplist2[i] << endl;
     }
}

void DLFLObject :: pentagonalSubDivide(double offset)
{
     // Pentagonal subdivision

     // Commonly used variables
  DLFLFacePtrList :: iterator fl_first, fl_last;
  DLFLFacePtr fp;
  DLFLVertexPtr vp;
  int num_faces, num_old_faces;

     // First compute the coordinates of the new points and store them in the aux-coords
     // fields of each entity (vertex, edge, face)
  
     // Go through each face and compute the centroid and store it in the aux-coords field
     // Do the makeUnique also to make sure Face IDs are consecutive
  Vector3d cen;
  num_faces = 0;
  fl_first = face_list.begin(); fl_last = face_list.end();
  while ( fl_first != fl_last )
     {
       fp = (*fl_first); ++fl_first; ++num_faces;
       cen = fp->geomCentroid();
       fp->setAuxCoords(cen); fp->makeUnique();
     }
	
     // Subdivide all the edges into 3 equal parts.
  subDivideAllEdges(3);


     // Create point-spheres for each old Face. Insert edges between the point-sphere and
     // every other new vertex (created by edge subdivision) in each old face.
     // Also adjust the coordinates of the new vertices based on the offset.

  DLFLEdgePtr lastedge;
  DLFLFacePtr fptr1;
  DLFLFaceVertexPtr fvp1, fvp2;
  int numsides1;
  bool done;

  fl_first = face_list.begin(); fl_last = face_list.end();

  num_old_faces = num_faces; num_faces = 0;
  while ( fl_first != fl_last && num_faces < num_old_faces )
     {
       fp = (*fl_first); ++fl_first; ++num_faces;

       cen = fp->getAuxCoords();
       fvp1 = createPointSphere(cen,fp->material());

          // Get corner following the first corner in the face
          // The first corner will always be an old vertex of the face
          // Thus the corner following it will always be a new vertex
          // Adjacent faces will never use the same new vertex in an edge
          // because of the consistent rotation order

       fvp2 = (fp->firstVertex())->next();

          // Insert an edge between the point-sphere and the new corner
          // NOTE: Make sure point-sphere corner is given first
       lastedge = insertEdge2(fvp1,fvp2);

       done = false;
       while ( !done )
          {
            fvp1 = lastedge->getFaceVertexPtr1();

               // Get face in which fvp1 is there. This will always be the face
               // in which the next edge has to be inserted because the point-sphere corner
               // is always specified first in the insertEdge call
            fptr1 = fvp1->getFacePtr(); numsides1 = fptr1->size();

               // If the face has more than 5 sides insert another edge
            if ( numsides1 > 5 )
               {
                 fvp2 = fvp1->next();
				
                    // Update the position of the new vertex referred to by fvp2
                 vp = fvp2->getVertexPtr();
                 vp->coords += ( cen - vp->coords )*offset;

                    // Skip the next two corners to get the corner for next edge insertion
                 fvp2 = fvp2->next(); fvp2 = fvp2->next(); fvp2 = fvp2->next();

                    // Make sure point-sphere corner is specified first
                 lastedge = insertEdge1(fvp1,fvp2,false);
               }
            else
               {
                    // All edges in this face have been inserted.
                 done = true;
               }
          }
     }	 
}

void DLFLObject :: honeycombSubDivide(void)
{
     // Honey-comb subdivision

     // Go through list of faces and create the inner faces for each face
  DLFLFacePtrList :: iterator fl_first, fl_last;
  DLFLEdgePtrList :: iterator el_first, el_last;
  DLFLVertexPtrList :: iterator vl_first, vl_last;
  DLFLFacePtr fp, ifp;
  Vector3dArray vertex_coords, new_vertex_coords;
  int num_verts, num_faces, num_edges;
  int num_old_verts, num_old_faces, num_old_edges;
  Vector3d p1,p2,p;
  Vector3d * midpoints = NULL;
  DLFLFaceVertexPtrArray fvplist1, fvplist2; // List of face-vertices which have to be connected
  DLFLFaceVertexPtr fvp1, fvp2, head1, head2;
  DLFLEdgePtr ep;
  DLFLVertexPtr vp;
  int edgeindex,eistart;
  
  num_old_verts = vertex_list.size();
  num_old_faces = face_list.size();
  num_old_edges = edge_list.size();

     // Apply make-unique on the edge_list to make sure all Edge IDs are consecutive
  makeEdgesUnique();
  
     // Reserve and create num_old_edges entries in the 2 temporary lists
  fvplist1.resize(num_old_edges,NULL); fvplist2.resize(num_old_edges,NULL);

     // Find starting edge ID to use as offset.
  eistart = (edge_list.front())->getID();

     // For each old face create new faces and update the 2 temporary lists using the edge IDs
     // as index in the lists
  fl_first = face_list.begin(); fl_last = face_list.end();
  num_faces = 0;
  while ( fl_first != fl_last && num_faces < num_old_faces )
     {
       fp = (*fl_first);

       fp->getVertexCoords(vertex_coords);
       num_verts = vertex_coords.size();

          // Find mid-points of all the edges in this face and store them
       midpoints = new Vector3d[num_verts];
       new_vertex_coords.reserve(num_verts);
       for (int i=0; i < num_verts; ++i)
          {
            p1 = vertex_coords[i];
            if ( i == num_verts-1 ) p2 = vertex_coords[0];
            else p2 = vertex_coords[i+1];
            midpoints[i] = (p1+p2)/2.0;
          }

          // Now using the mid-points compute new coordinates for the new polygon
       double coef, alpha;
       for (int i=0; i < num_verts; ++i)
          {
            p.reset();
            for (int j=0; j < num_verts; ++j)
               {
                 alpha = M_SQRT1_2 - 0.25 + 5.0/(4.0*num_verts);
                 if ( i == j ) coef = alpha;
                 else coef = (1.0 - alpha) * ( 3.0 + 2.0*cos(2.0*(i-j)*M_PI/num_verts) ) / (3.0*num_verts - 5.0);
                 p += coef*midpoints[j];
               }
            new_vertex_coords.push_back(p);
          }
            
       createFace(new_vertex_coords);

       delete [] midpoints; midpoints = NULL;
       new_vertex_coords.clear();
       vertex_coords.clear();
            
          // Get second newly inserted face - the one which is facing inwards
       ifp = face_list.back();

          // Go through current face and ifp and store the face-vertices in the
          // array of corners to make connections afterwards. Array is indexed
          // using the ID of the edge starting at each corner of the old face
          // offset to start at 0 using num_edges
          // ifp has to be traversed in the reverse direction
            
       fvp1 = head1 = fp->front(); fvp2 = head2 = ifp->front();

       ep = fvp1->getEdgePtr(); edgeindex = ep->getID() - eistart;
       if ( fvplist1[edgeindex] == NULL ) fvplist1[edgeindex] = fvp2;
       else fvplist2[edgeindex] = fvp2;
       fvp1 = fvp1->next(); fvp2 = fvp2->prev();

       while ( fvp1 != head1 && fvp2 != head2 )
          {
            ep = fvp1->getEdgePtr(); edgeindex = ep->getID() - eistart;
            if ( fvplist1[edgeindex] == NULL ) fvplist1[edgeindex] = fvp2;
            else fvplist2[edgeindex] = fvp2;
            fvp1 = fvp1->next(); fvp2 = fvp2->prev();
          }
       ++fl_first; ++num_faces;
     }

     // Go through the face_list,edge_list and vertex_list and destroy all the old faces, edges and vertices
  num_faces = 0; fl_first = face_list.begin(); fl_last = face_list.end();
  while ( fl_first != fl_last && num_faces < num_old_faces )
     {
       fp = (*fl_first); ++fl_first; ++num_faces;
       face_list.remove(fp); delete fp;
     }

  num_edges = 0; el_first = edge_list.begin(); el_last = edge_list.end();
  while ( el_first != el_last && num_edges < num_old_edges )
     {
       ep = (*el_first); ++el_first; ++num_edges;
       edge_list.remove(ep); delete ep;
     }

  num_verts = 0; vl_first = vertex_list.begin(); vl_last = vertex_list.end();
  while ( vl_first != vl_last && num_verts < num_old_verts )
     {
       vp = (*vl_first); ++vl_first; ++num_verts;
       vertex_list.remove(vp); delete vp;
     }
  
     // Go through fvplist1 and fvplist2 and insert edges between corresponding face-vertices
  for (int i=0; i < num_old_edges; ++i)
     {
       if ( fvplist1[i] != NULL && fvplist2[i] != NULL )
          insertEdge(fvplist1[i],fvplist2[i]);
       else
          cout << "NULL pointers found! i = " << i << " "
               << fvplist1[i] << " " << fvplist2[i] << endl;
     }
}

void DLFLObject :: dooSabinSubDivide(bool check)
{
     // Regular Doo-Sabin subdivision scheme

     // Go through list of faces and create new inner faces for each face
  DLFLFacePtrList :: iterator fl_first, fl_last;
  DLFLEdgePtrList :: iterator el_first, el_last;
  DLFLVertexPtrList :: iterator vl_first, vl_last;
  DLFLFacePtr fp, ifp;
  DLFLEdgePtrArray eplist1, eplist2; // List of edges to be connected
  DLFLFaceVertexPtr fvp1, fvp2, head1, head2;
  DLFLEdgePtr ep, iep;
  DLFLVertexPtr vp;
  Vector3dArray vertex_coords, new_vertex_coords;
  Vector3d p;
  int num_old_faces, num_old_edges, num_old_verts;
  int num_faces, num_edges, num_verts;
  int eistart, edgeindex;
  
  num_old_verts = vertex_list.size();
  num_old_faces = face_list.size();
  num_old_edges = edge_list.size();

     // Apply make-unique on the edge_list to make sure all Edge IDs are consecutive
  makeEdgesUnique();
  
     // Reserve and create num_old_edges entries in the 2 temporary lists
  eplist1.resize(num_old_edges,NULL); eplist2.resize(num_old_edges,NULL);
  
     // Find starting edge ID to use as offset.
  eistart = (edge_list.front())->getID();

  fl_first = face_list.begin(); fl_last = face_list.end(); num_faces = 0;
  while ( fl_first != fl_last && num_faces < num_old_faces )
     {
       fp = (*fl_first);

       fp->getVertexCoords(vertex_coords);

       num_verts = vertex_coords.size();
       new_vertex_coords.reserve(num_verts);
            
          // Compute new coordinates for the new polygon
       double coef;
       for (int i=0; i < num_verts; ++i)
          {
            p.reset();
            for (int j=0; j < num_verts; ++j)
               {
                 if ( i == j ) coef = 0.25 + 5.0/(4.0*num_verts);
                 else coef = ( 3.0 + 2.0*cos(2.0*(i-j)*M_PI/num_verts) ) / (4.0*num_verts);
                 p += coef*vertex_coords[j];
               }
            new_vertex_coords.push_back(p);
          }
       createFace(new_vertex_coords);
       new_vertex_coords.clear(); vertex_coords.clear();

          // Get the second newly inserted face, the one facing inwards
       ifp = face_list.back();
          // Set type of inner face so we can use the type to
          // determine which face to use for edge connects
       ifp->setType(FTNew);
            
          // Go through current face and ifp and store the face-vertices in the
          // array of corners to make connections afterwards. Array is indexed
          // using the ID of the edge starting at each corner of the old face
          // offset to start at 0 using num_edges
          // ifp has to be traversed in the reverse direction starting at the back
            
       fvp1 = head1 = fp->front(); fvp2 = head2 = ifp->back();

       ep = fvp1->getEdgePtr(); iep = fvp2->getEdgePtr();
       edgeindex = ep->getID() - eistart;
       if ( eplist1[edgeindex] == NULL ) eplist1[edgeindex] = iep;
       else                              eplist2[edgeindex] = iep;
       fvp1 = fvp1->next(); fvp2 = fvp2->prev();

       while ( fvp1 != head1 && fvp2 != head2 )
          {
            ep = fvp1->getEdgePtr(); iep = fvp2->getEdgePtr();
            edgeindex = ep->getID() - eistart;
            if ( eplist1[edgeindex] == NULL ) eplist1[edgeindex] = iep;
            else                              eplist2[edgeindex] = iep;
            fvp1 = fvp1->next(); fvp2 = fvp2->prev();
          }
       ++fl_first; ++num_faces;
     }

     // Go through the face_list,edge_list and vertex_list and destroy all the old faces, edges and vertices
  num_faces = 0; fl_first = face_list.begin(); fl_last = face_list.end();
  while ( fl_first != fl_last && num_faces < num_old_faces )
     {
       fp = (*fl_first); ++fl_first; ++num_faces;
       face_list.remove(fp); delete fp;
     }

  num_edges = 0; el_first = edge_list.begin(); el_last = edge_list.end();
  while ( el_first != el_last && num_edges < num_old_edges )
     {
       ep = (*el_first); ++el_first; ++num_edges;
       edge_list.remove(ep); delete ep;
     }

  num_verts = 0; vl_first = vertex_list.begin(); vl_last = vertex_list.end();
  while ( vl_first != vl_last && num_verts < num_old_verts )
     {
       vp = (*vl_first); ++vl_first; ++num_verts;
       vertex_list.remove(vp); delete vp;
     }

     // Go through eplist1,fplist1 and eplist2,fplist2 and connect corresponding half-edges
  DLFLFacePtr fp1, fp2, tfp1, tfp2;
  for (int i=0; i < num_old_edges; ++i)
     {
       if ( eplist1[i] != NULL && eplist2[i] != NULL )
          {
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

            connectEdgesWithoutLoopCheck(eplist1[i],fp1,eplist2[i],fp2,check);
          }
       else
          cout << "NULL pointers found! i = " << i << " "
               << eplist1[i] << " -- " << eplist2[i] << endl;
     }
}

void DLFLObject :: cornerCuttingSubDivide(void)
{
     // Corner-cutting subdivision scheme
     // Tension parameter is calculated based on number of vertices in face

     // Go through list of faces and create new inner faces for each face
  DLFLFacePtrList :: iterator fl_first, fl_last;
  DLFLEdgePtrList :: iterator el_first, el_last;
  DLFLVertexPtrList :: iterator vl_first, vl_last;
  DLFLFacePtr fp, ifp;
  DLFLEdgePtrArray eplist1, eplist2; // List of edges to be connected
  DLFLFaceVertexPtr fvp1, fvp2, head1, head2;
  DLFLEdgePtr ep, iep;
  DLFLVertexPtr vp;
  Vector3dArray vertex_coords, new_vertex_coords;
  Vector3d p;
  int num_old_faces, num_old_edges, num_old_verts;
  int num_faces, num_edges, num_verts;
  int eistart, edgeindex;
  
  num_old_verts = vertex_list.size();
  num_old_faces = face_list.size();
  num_old_edges = edge_list.size();

     // Apply make-unique on the edge_list to make sure all Edge IDs are consecutive
  makeEdgesUnique();
  
     // Reserve and create num_old_edges entries in the 2 temporary lists
  eplist1.resize(num_old_edges,NULL); eplist2.resize(num_old_edges,NULL);
  
     // Find starting edge ID to use as offset.
  eistart = (edge_list.front())->getID();

  fl_first = face_list.begin(); fl_last = face_list.end();
  num_faces = 0;
  while ( fl_first != fl_last && num_faces < num_old_faces )
     {
       fp = (*fl_first);

       fp->getVertexCoords(vertex_coords);

       num_verts = vertex_coords.size();
       new_vertex_coords.reserve(num_verts);
            
          // Compute new coordinates for the new polygon
       double coef, alpha;
          //alpha = 12.0/16.0;
       for (int i=0; i < num_verts; ++i)
          {
            p.reset();
            for (int j=0; j < num_verts; ++j)
               {
                 alpha = 0.25 + 5.0/(4.0*num_verts);
                 if ( i == j ) coef = alpha;
                 else coef = (1.0 - alpha) * ( 3.0 + 2.0*cos(2.0*(i-j)*M_PI/num_verts) ) / (3.0*num_verts - 5.0);
                 p += coef*vertex_coords[j];
               }
            new_vertex_coords.push_back(p);
          }
       createFace(new_vertex_coords);
       new_vertex_coords.clear(); vertex_coords.clear();

          // Get the second newly inserted face, the one facing inwards
       ifp = face_list.back();
          // Set type of inner face so we can use the type to
          // determine which face to use for edge connects
       ifp->setType(FTNew);
            
          // Go through current face and ifp and store the face-vertices in the
          // array of corners to make connections afterwards. Array is indexed
          // using the ID of the edge starting at each corner of the old face
          // offset to start at 0 using num_edges
          // ifp has to be traversed in the reverse direction starting at the back
            
       fvp1 = head1 = fp->front(); fvp2 = head2 = ifp->back();

       ep = fvp1->getEdgePtr(); iep = fvp2->getEdgePtr();
       edgeindex = ep->getID() - eistart;
       if ( eplist1[edgeindex] == NULL ) eplist1[edgeindex] = iep;
       else                              eplist2[edgeindex] = iep;
       fvp1 = fvp1->next(); fvp2 = fvp2->prev();

       while ( fvp1 != head1 && fvp2 != head2 )
          {
            ep = fvp1->getEdgePtr(); iep = fvp2->getEdgePtr();
            edgeindex = ep->getID() - eistart;
            if ( eplist1[edgeindex] == NULL ) eplist1[edgeindex] = iep;
            else                              eplist2[edgeindex] = iep;
            fvp1 = fvp1->next(); fvp2 = fvp2->prev();
          }
       ++fl_first; ++num_faces;
     }

     // Go through the face_list,edge_list and vertex_list and destroy all the old faces, edges and vertices
  num_faces = 0; fl_first = face_list.begin(); fl_last = face_list.end();
  while ( fl_first != fl_last && num_faces < num_old_faces )
     {
       fp = (*fl_first); ++fl_first; ++num_faces;
       face_list.remove(fp); delete fp;
     }

  num_edges = 0; el_first = edge_list.begin(); el_last = edge_list.end();
  while ( el_first != el_last && num_edges < num_old_edges )
     {
       ep = (*el_first); ++el_first; ++num_edges;
       edge_list.remove(ep); delete ep;
     }

  num_verts = 0; vl_first = vertex_list.begin(); vl_last = vertex_list.end();
  while ( vl_first != vl_last && num_verts < num_old_verts )
     {
       vp = (*vl_first); ++vl_first; ++num_verts;
       vertex_list.remove(vp); delete vp;
     }

     // Go through eplist1,fplist1 and eplist2,fplist2 and connect corresponding half-edges
  DLFLFacePtr fp1, fp2, tfp1, tfp2;
  for (int i=0; i < num_old_edges; ++i)
     {
       if ( eplist1[i] != NULL && eplist2[i] != NULL )
          {
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

            connectEdges(eplist1[i],fp1,eplist2[i],fp2);
          }
       else
          cout << "NULL pointers found! i = " << i << " "
               << eplist1[i] << " -- " << eplist2[i] << endl;
     }
}

void DLFLObject :: modifiedCornerCuttingSubDivide(double thickness)
{
     // (Modified) Corner-cutting subdivision scheme
     // Tension parameter is calculated based on number of vertices in face

     // Go through list of faces and create new inner faces for each face
  DLFLFacePtrList :: iterator fl_first, fl_last , fl_last_hole;
  DLFLEdgePtrList :: iterator el_first, el_last;
  DLFLVertexPtrList :: iterator vl_first, vl_last;
  DLFLFacePtr fp, ifp;
  DLFLEdgePtrArray eplist1, eplist2; // List of edges to be connected
  DLFLFaceVertexPtr fvp1, fvp2, head1, head2;
  DLFLEdgePtr ep, iep;
  DLFLVertexPtr vp;
  Vector3dArray vertex_coords, new_vertex_coords;
  Vector3d p;
  Vector3d v0, v1, v2, n1, n2, v0hat;
  float n1n2,x,t;
  int num_old_faces, num_old_edges, num_old_verts;
  int num_faces, num_edges, num_verts;
  int eistart, edgeindex;
  
  num_old_verts = vertex_list.size();
  num_old_faces = face_list.size();
  num_old_edges = edge_list.size();

     // Apply make-unique on the edge_list to make sure all Edge IDs are consecutive
  DLFLEdgePtrList :: iterator efirst=edge_list.begin(), elast=edge_list.end();

  makeEdgesUnique();
  
     // Reserve and create num_old_edges entries in the 2 temporary lists
  eplist1.resize(num_old_edges,NULL); eplist2.resize(num_old_edges,NULL);
  
     // Find starting edge ID to use as offset.
  eistart = (edge_list.front())->getID();

  fl_first = face_list.begin(); fl_last = face_list.end();
  num_faces = 0;
  while ( fl_first != fl_last && num_faces < num_old_faces )
     {
       fp = (*fl_first);

       fp->getVertexCoords(vertex_coords);

       num_verts = vertex_coords.size();
       new_vertex_coords.reserve(num_verts);
            
          // Compute new coordinates for the new polygon
          //double coef, alpha;
          //alpha = 12.0/16.0;
       for (int i=0; i < num_verts; ++i)
          {
            t = thickness/2.0;
            v0 = vertex_coords[i];
            if ( i > 0 ) v1 = vertex_coords[i-1];
            else v1 = vertex_coords[num_verts-1];
            if ( i < num_verts-1 ) v2 = vertex_coords[i+1];
            else v2 = vertex_coords[0];
            if ( norm(v1-v2) < 0.001 ) 
               {
                 if ( i < num_verts-2 ) v2 = vertex_coords[i+2]; 
                 else if ( i == num_verts-1 ) v2 = vertex_coords[0]; 
                 else v2 = vertex_coords[1];
               }
            n1 = normalized(v1-v0);
            n2 = normalized(v2-v0);
            n1n2 = n1*n2;
            x = sqrt(t*t/(1.0-n1n2*n1n2));
            v0hat = x*(n1+n2) + v0;
            p = v0hat;

            new_vertex_coords.push_back(p);
          }

       createFace(new_vertex_coords);
       new_vertex_coords.clear(); vertex_coords.clear();

          // Get the second newly inserted face, the one facing inwards
       ifp = face_list.back();
          // Set type of inner face so we can use the type to
          // determine which face to use for edge connects
       ifp->setType(FTNew);
          ////////////////////////////////////////////////////////////////////
       fl_last_hole = face_list.end();
       --fl_last_hole; --fl_last_hole; 
       // if (fp->getType() == FTHole)
				 (*fl_last_hole)->setType(FTHole);

          //////////////////////////////////////////////////////////////////////
            
            
          // Go through current face and ifp and store the face-vertices in the
          // array of corners to make connections afterwards. Array is indexed
          // using the ID of the edge starting at each corner of the old face
          // offset to start at 0 using num_edges
          // ifp has to be traversed in the reverse direction starting at the back
            
       fvp1 = head1 = fp->front(); fvp2 = head2 = ifp->back();
       
       ep = fvp1->getEdgePtr(); iep = fvp2->getEdgePtr();
       edgeindex = ep->getID() - eistart;
       if ( eplist1[edgeindex] == NULL ) eplist1[edgeindex] = iep;
       else                              eplist2[edgeindex] = iep;
       fvp1 = fvp1->next(); fvp2 = fvp2->prev();

       while ( fvp1 != head1 && fvp2 != head2 )
          {
            ep = fvp1->getEdgePtr(); iep = fvp2->getEdgePtr();
            edgeindex = ep->getID() - eistart;
            if ( eplist1[edgeindex] == NULL ) eplist1[edgeindex] = iep;
            else                              eplist2[edgeindex] = iep;
            fvp1 = fvp1->next(); fvp2 = fvp2->prev();
          }
       ++fl_first; ++num_faces;
     }

     // Go through the face_list,edge_list and vertex_list and destroy all the old faces, edges and vertices
  num_faces = 0; fl_first = face_list.begin(); fl_last = face_list.end();
  while ( fl_first != fl_last && num_faces < num_old_faces )
     {
       fp = (*fl_first); ++fl_first; ++num_faces;
       face_list.remove(fp); delete fp;
     }

  num_edges = 0; el_first = edge_list.begin(); el_last = edge_list.end();
  while ( el_first != el_last && num_edges < num_old_edges )
     {
       ep = (*el_first); ++el_first; ++num_edges;
       edge_list.remove(ep); delete ep;
     }

  num_verts = 0; vl_first = vertex_list.begin(); vl_last = vertex_list.end();
  while ( vl_first != vl_last && num_verts < num_old_verts )
     {
       vp = (*vl_first); ++vl_first; ++num_verts;
       vertex_list.remove(vp); delete vp;
     }

     // Go through eplist1,fplist1 and eplist2,fplist2 and connect corresponding half-edges
  DLFLFacePtr fp1, fp2, tfp1, tfp2;
  for (int i=0; i < num_old_edges; ++i)
     {
       if ( eplist1[i] != NULL && eplist2[i] != NULL )
          {
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

            connectEdges(eplist1[i],fp1,eplist2[i],fp2);
          }
       else
          cout << "NULL pointers found! i = " << i << " "
               << eplist1[i] << " -- " << eplist2[i] << endl;
     }
}

void DLFLObject :: root4SubDivide(double a, double twist)
{
     // Root-4 subdivision
     // Go through list of faces and create the inner faces for each face
  DLFLFacePtrList :: iterator fl_first = face_list.begin();
  DLFLFacePtrList :: iterator fl_last = face_list.end();
  DLFLFacePtr faceptr;
  Vector3dArray new_vertex_coords,vertex_coords;
  Vector3d * midpoints;
  Vector3d p1,p2,p;
  DLFLFacePtrArray oldfaces, newfaces;                 // List of faces to be connected
  int numoldedges = edge_list.size();                  // Keep track of no. of edges in un-sub-divided object
                                                       // All old edges have to be deleted in the end
  int numoldverts = vertex_list.size();                // No. of old vertices which have to be moved
  int numoldfaces = face_list.size();
  int numfaces = 0;
  int num_verts;
  
  while ( fl_first != fl_last && numfaces < numoldfaces )
     {
       faceptr = (*fl_first);

       faceptr->getVertexCoords(vertex_coords);
       num_verts = vertex_coords.size();

          // Find mid-points of all the edges in this face and store them
       midpoints = new Vector3d[num_verts];
       new_vertex_coords.reserve(num_verts);
       for (int i=0; i < num_verts; ++i)
          {
            p1 = vertex_coords[i];
            if ( i == num_verts-1 ) p2 = vertex_coords[0];
            else p2 = vertex_coords[i+1];
            midpoints[i] = (1.0-twist)*p1 + twist*p2;
          }

          // Now using the mid-points compute new coordinates for the new polygon
       double coef, alpha;
       for (int i=0; i < num_verts; ++i)
          {
            p.reset();
            for (int j=0; j < num_verts; ++j)
               {
                 alpha = M_SQRT1_2 - 0.25 + 5.0/(4.0*num_verts);
                 if ( i == j ) coef = alpha;
                 else coef = (1.0 - alpha) * ( 3.0 + 2.0*cos(2.0*(i-j)*M_PI/num_verts) ) / (3.0*num_verts - 5.0);
                 p += coef*midpoints[j];
               }
            new_vertex_coords.push_back(p);
          }
            
       createFace(new_vertex_coords);

       delete [] midpoints; midpoints = NULL;
       new_vertex_coords.clear();
       vertex_coords.clear();

          // Old face and new face facing the old face have to be connected.
          // Store the pointers to the 2 faces in the 2 temporary arrays
       oldfaces.push_back(faceptr); newfaces.push_back(face_list.back());
       ++fl_first; ++numfaces;
     }

     // Go through list of faces to be connected and make the connections
  int numconn = oldfaces.size();
  DLFLFacePtr oldface=NULL, newface=NULL;
  for (int i=0; i < numconn; ++i)
     {
       oldface = oldfaces[i]; newface = newfaces[i];
       
          // Similar to extrude operation

          // Find the first face-vertices in the 2 faces to be connected
       DLFLFaceVertexPtr fvp1, fvp2;
       fvp1 = oldface->firstVertex(); fvp2 = newface->firstVertex();
       connectFaces(fvp1,fvp2);
     }
  oldfaces.clear(); newfaces.clear();
  
     // Go through the edge_list and delete all the old edges
  int count=0;
  DLFLEdgePtrList :: iterator el_first, el_last;
  DLFLEdgePtr eptr = NULL;
  el_first = edge_list.begin(); el_last = edge_list.end();
  
  while ( el_first != el_last )
     {
       eptr = (*el_first);
       ++el_first; ++count;
       deleteEdge(eptr);
       if ( count >= numoldedges ) break; // Done with old edges
     }

     // Go through vertex list and move all the old vertices
  count = 0;
  DLFLVertexPtrList :: iterator vl_first, vl_last;
  DLFLVertexPtr vptr = NULL;
  vl_first = vertex_list.begin(); vl_last = vertex_list.end();
  DLFLEdgePtr * iedges = NULL; // Edges indident on a vertex
  DLFLEdgePtr iedge = NULL; // Incident edge
  int numiedges = 0;
  Vector3d mp,psum,np;
  
  while ( vl_first != vl_last )
     {
       vptr = (*vl_first);
       ++vl_first; ++count;

          // Move vertex
       p = vptr->coords;
       
       numiedges = vptr->getEdges(&iedges);

       psum.reset();
       for (int j=0; j < numiedges; ++j)
          {
            iedge = iedges[j];
            mp = iedge->getMidPoint();
            psum += mp;
          }
       psum *= 2.0;
       psum -= numiedges*p;
       psum /= numiedges;

       np = psum*a + p*(1.0-a);

       vptr->coords = np;

       delete [] iedges; iedges = NULL;

       if ( count >= numoldverts ) break; // Done with old vertices
     }
}

  
void DLFLObject :: catmullClarkSubDivide(void)
{
     // Catmull-Clark subdivision surfaces implementation

     // Commonly used variables
  DLFLFacePtrList :: iterator fl_first, fl_last;
  DLFLEdgePtrList :: iterator el_first, el_last;
  DLFLVertexPtrList :: iterator vl_first, vl_last;
  DLFLFacePtr fp, efp1, efp2;
  DLFLEdgePtr ep;
  DLFLVertexPtr vp;
  DLFLFaceVertexPtr fvp;
  int num_faces, num_edges;
  int num_old_faces, num_old_edges;
  

     // Reset aux coords in each vertex
  vl_first = vertex_list.begin(); vl_last = vertex_list.end();
  while ( vl_first != vl_last )
     {
       vp = (*vl_first); ++vl_first;
       vp->resetAuxCoords();
     }
  
     // Compute the coordinates of the new points and store them in the aux-coords
     // fields of each class
  
     // Go through each face and compute the centroid and store it in the aux-coords field
     // Do the makeUnique also to make sure Face IDs are consecutive
     // Send the contribution to all vertexes belonging to this face
  Vector3d cen;
  num_faces = 0;
  fl_first = face_list.begin(); fl_last = face_list.end();
  while ( fl_first != fl_last )
     {
       fp = (*fl_first); ++fl_first; ++num_faces;
       cen = fp->geomCentroid();
       fp->setAuxCoords(cen); fp->makeUnique();

          // Send contribution of this face to all vertices in this face
       DLFLFaceVertexPtr current, head;
       current = head = fp->front();
       current->vertex->addToAuxCoords(cen); current = current->next();
       while ( current != head )
          {
            current->vertex->addToAuxCoords(cen); current = current->next();
          }
     }

     // Go through each edge and compute the new edge point coordinates
     // Send contribution of each edge to the vertexes at the ends
  Vector3d mp,afp,edgept;
  num_edges = 0;
  el_first = edge_list.begin(); el_last = edge_list.end();
  while ( el_first != el_last )
     {
       ep = (*el_first); ++el_first; ++num_edges;
       ep->getFacePointers(efp1,efp2);
       mp = ep->getMidPoint(); afp = ( efp1->getAuxCoords() + efp2->getAuxCoords() ) / 2.0;
       edgept = (mp + afp)/2.0;
       ep->setAuxCoords(edgept);

          // Send contribution of this edge to its 2 corners
       DLFLFaceVertexPtr efvp1, efvp2;
       ep->getFaceVertexPointers(efvp1,efvp2);
       efvp1->vertex->addToAuxCoords(2.0*mp); efvp2->vertex->addToAuxCoords(2.0*mp);
     }

     // Go through each vertex and compute the new vertex point coordinates
     // and change the coordinate to be the new one
  Vector3d vertexpt, ave_fep; // Contribution from faces and edges
  int n;
  vl_first = vertex_list.begin(); vl_last = vertex_list.end();
  while ( vl_first != vl_last )
     {
       vp = (*vl_first); ++vl_first;
       n = vp->valence();
       ave_fep = vp->getAuxCoords(); vp->resetAuxCoords();
       ave_fep /= double(n);
       vertexpt = ( ave_fep + (vp->coords)*(n-3.0) ) /double(n);
       vp->coords = vertexpt;
     }
  
     // Now create point-spheres for each Face. Store a pointer to the FaceVertex in
     // a temporary array indexed using the Face ID. For this makeUnique has to be
     // applied to the face list to make the Face IDs consecutive.
  DLFLVertexPtrArray psarray; // Array of point spheres (vertices)
  int fistart, faceindex;

  psarray.reserve(num_faces); psarray.resize(num_faces,NULL);
  fl_first = face_list.begin(); fl_last = face_list.end();
  fistart = (*fl_first)->getID();

  num_old_faces = num_faces; num_faces = 0;
  while ( fl_first != fl_last && num_faces < num_old_faces )
     {
       fp = (*fl_first); ++fl_first; ++num_faces;
       faceindex = fp->getID() - fistart;
       fvp = createPointSphere(fp->getAuxCoords(),fp->material());
          // Assign color and texture coords as centroid of old face
       fvp->color = fp->colorCentroid(); fvp->texcoord = fp->textureCentroid();
       fp->resetAuxCoords();
       psarray[faceindex] = fvp->vertex;
     }

     
     // Now subdivide all the edges. Get the newly created face-vertices
     // and add them to the table along with the vertex from above list of pointspheres
  DLFLFaceVertexPtrArray fvplist;
  DLFLVertexPtrArray vplist;
  DLFLFaceVertexPtrArray fvparray;
  int connindex, numconn;
  num_edges = 0; num_old_edges = edge_list.size();
  connindex = 0; numconn = 2*num_old_edges;
  
  fvplist.resize(numconn,NULL); vplist.resize(numconn,NULL);

  el_first = edge_list.begin(); el_last = edge_list.end();
  fvparray.reserve(2);
  while ( el_first != el_last && num_edges < num_old_edges )
     {
       ep = (*el_first); ++el_first; ++num_edges;

       edgept = ep->getAuxCoords(); ep->resetAuxCoords();
       vp = subDivideEdge(ep); vp->coords = edgept;
       vp->getFaceVertices(fvparray);

       for (int i=0; i < fvparray.size(); ++i)
          {
            fvp = fvparray[i];
            faceindex = (fvp->getFacePtr())->getID() - fistart;
            fvplist[connindex] = fvp; vplist[connindex] = psarray[faceindex];
            ++connindex;
          }
       fvparray.clear();
     }

     // psarray can be freed now
  psarray.clear();

      
     // Make all connections
  DLFLFaceVertexPtr fvp1, fvp2;
  for (int j=0; j < numconn; ++j)
     {
       fvp1 = fvplist[j];
       
          // Find the face-vertex referring to vp which is in the same face as fvp1
          // If only 1 face-vertex refers to vp, use that instead
       vp = vplist[j];
       fvp2 = vp->getFaceVertexInFace(fvp1->getFacePtr());
       insertEdge(fvp1,fvp2);
     }
  fvplist.clear(); vplist.clear();
}

void DLFLObject :: starSubDivide(double offset) // Doug
{
     // Star subdivision
  
     // Commonly used variables
  DLFLFacePtrList :: iterator fl_first, fl_last;
  DLFLFacePtr fp;
  DLFLVertexPtrList :: iterator vl_first, vl_last;
  DLFLVertexPtr vp;

  int num_faces, num_old_faces, num_orig_faces;
  int numVect1, numVect2;
  
  num_faces = 0;
  num_old_faces = face_list.size();
  num_orig_faces = num_old_faces;
  fl_first = face_list.begin(); fl_last = face_list.end();

  numVect1 = vertex_list.size();
  vl_first = vertex_list.begin();

  Vector3d NormList[num_orig_faces];

     //Stellate each face
  while ( num_faces < num_old_faces )
     {
       fp = (*fl_first); 
       ++fl_first;
       NormList[num_faces] = fp->normalCentroid();
       stellateFace(fp, 0.0);
       ++num_faces;
     }

  numVect2 = vertex_list.size();

  num_faces = 0;
  num_old_faces = face_list.size();
  fl_first = face_list.begin(); fl_last = face_list.end();
  
     //Stellate all the faces again
  while ( num_faces < num_old_faces )
     {
       fp = (*fl_first); 
       ++fl_first; 
       stellateFace(fp, 0.0);
       ++num_faces;
     }

  int NormCtr = 0;
  int VectCtr = 0;
  while ( VectCtr < numVect1 )
     {
       vp = (*vl_first);
       ++vl_first;
       ++VectCtr;
     }
  
  while ( VectCtr < numVect2 )
     {
       vp = (*vl_first);
       ++vl_first;
       ++VectCtr;
       vp->coords = vp->coords + offset*NormList[NormCtr];
       ++NormCtr;
     }

}

void DLFLObject :: sqrt3SubDivide(void) // Doug
{
     // Sqrt(3) subdivision

     // Commonly used variables
  DLFLFacePtrList :: iterator fl_first, fl_last;
  DLFLFacePtr fp;
  int num_faces, num_old_faces, num_old_edges;
  
  num_faces = 0;
  num_old_faces = face_list.size();
  num_old_edges = edge_list.size();

  fl_first = face_list.begin(); fl_last = face_list.end();

     //Stellate all the faces
  while ( num_faces < num_old_faces )
     {
       fp = (*fl_first); 
       ++fl_first; 
       stellateFace(fp, 0.0);
       ++num_faces;
     }
  
     // Delete the old edges
  DLFLEdgePtrList :: iterator el_first = edge_list.begin();
  DLFLEdgePtrList :: iterator el_last = edge_list.end();
  DLFLEdgePtr ep = NULL;
  int num_edges = 0; 
  
  while ( el_first != el_last && num_edges < num_old_edges )
     {
       ep = (*el_first); ++el_first; ++num_edges;
       deleteEdge(ep,true);
     }

  DLFLFaceVertexPtr fvp1, fvp2;

  num_faces = 0;
  num_old_faces = face_list.size();
  fl_first = face_list.begin(); fl_last = face_list.end();

  while ( num_faces < num_old_faces )
     {
       fp = (*fl_first);
       ++fl_first;
       ++num_faces;
       fvp1 = (fp->firstVertex())->next();
       fvp2 = fvp1->next()->next();
       insertEdge(fvp1,fvp2);
     }
}

void DLFLObject :: fractalSubDivide(double offset) // Doug
{
     // Fractal

  DLFLFacePtrList :: iterator fl_first, fl_last;
  DLFLFacePtr fp, fptr1;
  DLFLFaceVertexPtr fvp1, fvp2, fvp3;
  DLFLFaceVertexPtr fvpTest;
  DLFLEdgePtr lastedge;
  Vector3d dir;
  int num_faces, num_old_faces, numsides1;
  int numsidesALL, i;
  double d, length1, length2;
  bool done;

  num_faces = 0;
  
  num_old_faces = face_list.size();
  fl_first = face_list.begin(); fl_last = face_list.end();

  subDivideAllEdges();

  while ( num_faces < num_old_faces )
     {
       fp = (*fl_first); ++fl_first; ++num_faces;

       dir = fp->normalCentroid();

       DLFLMaterialPtr matl = fp->material();

       fvp2 = fp->firstVertex()->next();
       fvp3 = fvp2->next(); fvp3 = fvp3->next();


          // number of sides on the face
       numsidesALL = fp->size();

       fvpTest = fvp2;

       for (i=0; i < (numsidesALL/2); i++)
          {
            fvpTest = fvpTest->next();
          }

          //fvp2 and fvpTest should now be on opposites side of the face
          //get the distance btwn them divided by 2
       length1 = norm( fvp2->getVertexCoords() - fvpTest->getVertexCoords() )/2.0;

    
          // This should be the distance btwn two adjacent new vertices, but it is too big
       length2 = norm( fvp2->getVertexCoords() - fvp3->getVertexCoords() );
    
          // this first offset is too big atm, so using length1 for now 
       d = sqrt(length2*length2 - length1*length1);
       d = d*offset;


       fvp1 = createPointSphere(fp->geomCentroid()+d*dir,matl);

       insertEdge1(fvp2,fvp3,false);
       lastedge = insertEdge2(fvp1,fvp2,matl);

       done = false;
       while ( !done )
          {

            fvp1 = lastedge->getFaceVertexPtr1();

               // Get face in which fvp1 is there. This will always be the face
               // in which the next edge has to be inserted because the point-sphere corner
               // is always specified first in the insertEdge call
            fptr1 = fvp1->getFacePtr(); numsides1 = fptr1->size();

               // If the face fptr1 has more than 3 sides insert another edge
            if ( numsides1 > 3 ){

                 // Skip the next corner to get the corner for next edge insertion
              fvp2 = fvp1->next(); fvp2 = fvp2->next();
              fvp3 = fvp2->next(); fvp3 = fvp3->next();

                 // Make sure point-sphere corner is specified first
              insertEdge1(fvp2,fvp3,false);
              lastedge = insertEdge1(fvp1,fvp2,false);
            }
            else
               {
                    // All edges have been inserted.
                 done = true;
               }
          }
     }
}

void DLFLObject :: stellateSubDivide(void) // Eric
{
  DLFLFacePtrList :: iterator fl_first, fl_last;
  DLFLFacePtr fp;
  int num_faces, num_old_faces;
  int num_old_edges;
  Vector3d cen;
  Vector3d cen_to_edge;

  num_old_faces = face_list.size();
  num_old_edges = edge_list.size();

  num_faces = 0;
  fl_first = face_list.begin(); fl_last = face_list.end();
  while ( fl_first != fl_last && num_faces < num_old_faces )
     {
       fp = (*fl_first); ++fl_first; ++num_faces;
       cen = fp->geomCentroid();
       fp->setAuxCoords(cen); fp->makeUnique();

       stellateFace(fp, 0);
     }

     // Go through the edge_list and delete all the old edges
  int count=0;
  DLFLEdgePtrList :: iterator el_first, el_last;
  DLFLEdgePtr eptr = NULL;
  el_first = edge_list.begin(); el_last = edge_list.end();

  while ( el_first != el_last )
     {
       eptr = (*el_first);
       ++el_first; ++count;
       deleteEdge(eptr);
       if ( count >= num_old_edges ) break; // Done with old edges
     }
}

void DLFLObject :: twostellateSubDivide(double offset, double curve) // Eric
{
  DLFLFacePtrList :: iterator fl_first, fl_last;
  DLFLFacePtr fp;
  int num_faces, num_old_faces;
  int num_old_edges;
  Vector3d cen;

  num_old_faces = face_list.size();

  num_faces = 0;
  fl_first = face_list.begin(); fl_last = face_list.end();
  while ( fl_first != fl_last && num_faces < num_old_faces )
     {
       fp = (*fl_first); ++fl_first; ++num_faces;
       cen = fp->geomCentroid();
       fp->setAuxCoords(cen); fp->makeUnique();
       stellateFace(fp, offset);
     }

  num_old_faces = face_list.size();
  num_old_edges = edge_list.size();

  num_faces = 0;
  fl_first = face_list.begin(); fl_last = face_list.end();
  while ( fl_first != fl_last && num_faces < num_old_faces )
     {
       fp = (*fl_first); ++fl_first; ++num_faces;
       cen = fp->geomCentroid();
       fp->setAuxCoords(cen); fp->makeUnique();
       stellateFace(fp, curve);
     }

     // Go through the edge_list and delete all the old edges
  int count=0;
  DLFLEdgePtrList :: iterator el_first, el_last;
  DLFLEdgePtr eptr = NULL;
  el_first = edge_list.begin(); el_last = edge_list.end();
  while ( el_first != el_last )
     {
       eptr = (*el_first);
       ++el_first; ++count;
       deleteEdge(eptr);
       if ( count >= num_old_edges ) break; // Done with old edges
     }
}

//----- Begin Additions by Bei & Cansin -----//

void DLFLObject :: setOldVertexType()
{
  DLFLVertexPtrList::iterator first;
  int num_old_vertex, num_vertex;

  num_old_vertex = vertex_list.size();
  num_vertex = 0;
  first = vertex_list.begin();
  while ( num_vertex < num_old_vertex )
     {
       (*first)->setType(VTOld);
       num_vertex++;
       first++;
     }
}

void DLFLObject :: setNewSubVertexType()
{
  DLFLVertexPtrList::iterator first;
  int num_old_vertex, num_vertex;

  num_old_vertex = vertex_list.size();
  num_vertex = 0;
  first = vertex_list.begin();
  while ( num_vertex < num_old_vertex )
     {
       if ((*first)->getType() != VTOld)
          (*first)->setType(VTNewSub);
       num_vertex++;
       first++;
     }	
}

void DLFLObject :: setNormalVertexType()
{
  DLFLVertexPtrList::iterator first;
  int num_old_vertex, num_vertex;

  num_old_vertex = vertex_list.size();
  num_vertex = 0;
  first = vertex_list.begin();
  while ( num_vertex < num_old_vertex )
     {
       (*first)->setType(VTNormal);
       num_vertex++;
       first++;
     }
}

void DLFLObject::domeSubDivide(double length, double sf)
{	
  DLFLFacePtrList :: iterator fl_first, fl_last;
  DLFLFacePtr fp;
  int num_old_faces;
  int num_faces;

  num_old_faces = face_list.size();

     // Subdivide all the edges by 4
  subDivideAllEdges(4);

  num_faces = 0; fl_first = face_list.begin(); fl_last = face_list.end();
  while ( fl_first != fl_last && num_faces < num_old_faces )
     {
       fp = (*fl_first); ++fl_first; ++num_faces;
       fp = extrudeFaceDS(fp, 0.0*length, 1, 0.0, 1.6*sf);
       fp = extrudeFaceDS(fp, 0.3*length, 1, 0.0, 1.7*sf);
       fp = extrudeFaceDS(fp, 0.18*length, 1, 0.0, 1.6*sf);
       fp = extrudeFaceDS(fp, 0.1*length, 1, 0.0, 1.4*sf);
       fp = extrudeFaceDS(fp, 0.05*length, 1, 0.0, 1.2*sf);
       fp = extrudeFaceDS(fp, 0.025*length, 1, 0.0, 1.1*sf);
       fp = extrudeFaceDS(fp, 0.01*length, 1, 0.0, 0.01*sf);		
     }
}

void DLFLObject :: dual1264SubDivide(double sf)
{
  DLFLFacePtrList :: iterator fl_first, fl_last;
  DLFLEdgePtrList :: iterator el_first, el_last;
  DLFLVertexPtrList :: iterator vl_first, vl_last;
  DLFLEdgePtrArray eplist1, eplist2; // List of edges to be connected
  DLFLFacePtr fp, ifp;
  DLFLFaceVertexPtr head, current, fvp1, fvp2, head1, head2;
  DLFLEdgePtr ep, iep;
  DLFLVertexPtr vp;
  Vector3d vert1, vert2;
  Vector3dArray new_vertex_coords;
  int num_old_faces, num_old_edges, num_old_verts;
  int num_faces, num_edges, num_verts;
  int eistart, edgeindex;

  num_old_verts = vertex_list.size();
  num_old_faces = face_list.size();
  num_old_edges = edge_list.size();

     // Apply make-unique on the edge_list to make sure all Edge IDs are consecutive
  DLFLEdgePtrList :: iterator efirst=edge_list.begin(), elast=edge_list.end();
  while ( efirst != elast )
     {
       (*efirst)->makeUnique();
       ++efirst;
     }
  
     // Reserve and create num_old_edges entries in the 2 temporary lists
  eplist1.reserve(num_old_edges); eplist2.reserve(num_old_edges);
  eplist1.resize(num_old_edges,NULL); eplist2.resize(num_old_edges,NULL);

     // Find starting edge ID to use as offset.
  eistart = (edge_list.front())->getID();

  fl_first = face_list.begin(); fl_last = face_list.end(); num_faces = 0;
  while ( fl_first != fl_last && num_faces < num_old_faces )
     {
       fp = (*fl_first); ++fl_first; ++num_faces;
        
          // First compute the coordinates of the vertices of the new points and store them in an array
       head = fp->front();
       if ( head )
          {
            current = head;
			
            vert1 = current->getVertexCoords();
            vert2 = current->next()->getVertexCoords();
            new_vertex_coords.push_back(vert1 * 2/3 + vert2 * 1/3);
            new_vertex_coords.push_back(vert1 * 1/3 + vert2 * 2/3);

            current = current->next();
            while ( current != head )
               {
                 vert1 = current->getVertexCoords();
                 vert2 = current->next()->getVertexCoords();
                 new_vertex_coords.push_back(vert1 * 2/3 + vert2 * 1/3);
                 new_vertex_coords.push_back(vert1 * 1/3 + vert2 * 2/3);

                 current = current->next();
               }

               // Scale the new vertices about their centroid if scale factor is not 1.0 or 0.0
            sf = Abs(sf);
            if ( isNonZero(sf) && ( Abs(sf-1.0) > ZERO ) ) 
               scale(new_vertex_coords, sf);

            createFace(new_vertex_coords,fp->material());

            new_vertex_coords.clear();
            ifp = face_list.back();
            ifp->setType(FTNew);

            fvp1 = head1 = fp->front(); fvp2 = head2 = ifp->back();
            
            ep = fvp1->getEdgePtr(); iep = fvp2->getEdgePtr();
            edgeindex = ep->getID() - eistart;
            if ( eplist1[edgeindex] == NULL )
               eplist1[edgeindex] = iep;
            else
               eplist2[edgeindex] = iep;
            fvp2 = fvp2->prev();

            fvp1 = fvp1->next(); fvp2 = fvp2->prev();
            while ( fvp1 != head1 && fvp2 != head2 )
               {
                 ep = fvp1->getEdgePtr(); iep = fvp2->getEdgePtr();
                 edgeindex = ep->getID() - eistart;
                 if ( eplist1[edgeindex] == NULL )
                    eplist1[edgeindex] = iep;
                 else
                    eplist2[edgeindex] = iep;
                 fvp2 = fvp2->prev();

                 fvp1 = fvp1->next(); fvp2 = fvp2->prev();
               }
          }
     }

     // Go through the face_list,edge_list and vertex_list and destroy all the old faces, edges and vertices
  num_faces = 0; fl_first = face_list.begin(); fl_last = face_list.end();
  while ( fl_first != fl_last && num_faces < num_old_faces )
     {
       fp = (*fl_first); ++fl_first; ++num_faces;
       face_list.remove(fp); delete fp;
     }

  num_edges = 0; el_first = edge_list.begin(); el_last = edge_list.end();
  while ( el_first != el_last && num_edges < num_old_edges )
     {
       ep = (*el_first); ++el_first; ++num_edges;
       edge_list.remove(ep); delete ep;
     }

  num_verts = 0; vl_first = vertex_list.begin(); vl_last = vertex_list.end();
  while ( vl_first != vl_last && num_verts < num_old_verts )
     {
       vp = (*vl_first); ++vl_first; ++num_verts;
       vertex_list.remove(vp); delete vp;
     }
	
     // Go through eplist1,fplist1 and eplist2,fplist2 and connect corresponding half-edges
  DLFLFacePtr fp1, fp2, tfp1, tfp2;
  for (int i=0; i < num_old_edges; ++i)
     {
       if ( eplist1[i] != NULL && eplist2[i] != NULL )
          {
            eplist1[i]->getFacePointers(tfp1,tfp2);
            if ( tfp1->getType() == FTNew ) fp1 = tfp1;
            else if ( tfp2->getType() == FTNew )
               fp1 = tfp2;
            else
               cout << i << " : " << "Face not found for half-edge!" << endl;

            eplist2[i]->getFacePointers(tfp1,tfp2);
            if ( tfp1->getType() == FTNew )
               fp2 = tfp1;
            else if ( tfp2->getType() == FTNew )
               fp2 = tfp2;
            else
               cout << i << " : " << "Face not found for half-edge!" << endl;

            connectEdgesWithoutLoopCheck(eplist1[i],fp1,eplist2[i],fp2);
          }
       else
          cout << "NULL pointers found! i = " << i << " " << eplist1[i] << " -- " << eplist2[i] << endl;
     }	
}

void DLFLObject :: loopStyleSubDivide(double length)  // bei 12-11-03
{
  DLFLFacePtrList :: iterator fl_first, fl_last;
  DLFLVertexPtrList :: iterator vl_first, vl_last;
  DLFLFaceVertexPtrArray fvparray;
  DLFLFacePtr fp;
  DLFLVertexPtr vp;
  DLFLFaceVertexPtr fvp1, fvp2;
  Vector3d oldpos, newpos;
  int num_old_faces, num_faces, num_old_vertice, num_vertice, num_faceVertice;

  num_old_vertice = vertex_list.size();

     // mark the original vertices
  setOldVertexType();
     // subdivide all edges by 2
  subDivideAllEdges(2);
     // mark the newly created vertices by subdivision
  setNewSubVertexType();

     // TODO: put new code here below
  num_old_faces = face_list.size();
	
  num_faces = 0; fl_first = face_list.begin(); fl_last = face_list.end();
  while(fl_first != fl_last && num_faces < num_old_faces)
     {
       fp = (*fl_first); ++fl_first; ++num_faces;
		
       fvp1 = fp->front();		
       if ( (fvp1->getVertexType() != VTNewSub) )
          fvp1 = fvp1->next();
       vp = fvp1->getVertexPtr();

       fvp2 = fvp1->prev()->prev();		
       while( fvp2->getVertexPtr() != vp )
          {
            insertEdge(fvp1, fvp2);
            fvp1 = fvp2;
            fvp2 = fvp1->prev()->prev();
          }
       insertEdge(fvp1, fvp2);
     }

     // move the original vertices to a propriate position decided by length factor
  num_vertice = 0; vl_first = vertex_list.begin(); vl_last = vertex_list.end();
  while (vl_first != vl_last && num_vertice < num_old_vertice)
     {
       vp = (* vl_first); ++ vl_first; ++ num_vertice;
       if (vp->getType() != VTOld)
          continue;
       vp->getFaceVertices(fvparray);
       num_faceVertice = fvparray.size();
       newpos.reset();
       for (int i = 0; i < num_faceVertice; i++)
          newpos += fvparray[i]->next()->getVertexCoords();
       newpos = newpos/num_faceVertice;
       oldpos = vp->getCoords();
       newpos = length* oldpos + (1-length)* newpos;

       vp->setCoords(newpos);
     }

     // change the type of all the verteices back to VTNormal
  setNormalVertexType();
}

void DLFLObject :: dooSabinSubDivideBCNew(double sf, double length) // bei dec 11
{
  DLFLFacePtrList :: iterator fl_first, fl_last;
  DLFLEdgePtrList :: iterator el_first, el_last;
  DLFLVertexPtrList :: iterator vl_first, vl_last;
  DLFLFaceVertexPtrArray fvparray;
  DLFLFacePtr fp;
  DLFLEdgePtr ep, ep1, ep2;
  DLFLFaceVertexPtr fvp, fvp1, fvp2;
  DLFLVertexPtr vp;
  Vector3d oldpos, newpos;
  int num_old_faces, num_old_edges, num_old_vertice;
  int num_faces, num_edges, num_vertice, num_faceVertice;

  num_old_vertice = vertex_list.size();

     // mark the original vertices
  setOldVertexType();
     // subdivide all edges by 2
  subDivideAllEdges();
     // mark the newly created vertices by subdivision
  setNewSubVertexType();

  num_old_faces = face_list.size(); num_old_edges = edge_list.size();

     // doo-sabin extrusion
  num_faces = 0; fl_first = face_list.begin(); fl_last = face_list.end();
  while ( fl_first != fl_last && num_faces < num_old_faces )
     {
       fp = (*fl_first); ++fl_first; ++num_faces;
       extrudeFaceDS(fp, 0.0, 1, 0.0, sf);		
     }

     // delete old edges
  num_edges = 0; el_first = edge_list.begin(); el_last = edge_list.end();
  while ( el_first != el_last && num_edges < num_old_edges )
     {
       ep = (*el_first); ++el_first; ++num_edges;
       deleteEdge(ep);
     }

     // delete old edges while insert an new edge between two newly extrude faces
  num_old_edges = edge_list.size();
  num_edges = 0; el_first = edge_list.begin(); el_last = edge_list.end();
  while ( el_first != el_last && num_edges < num_old_edges )
     {
       ep = (*el_first); ++el_first; ++num_edges;
       fvp1 = ep->getFaceVertexPtr1(); 
       fvp2 = ep->getFaceVertexPtr2();

       if ((fvp1->getVertexType() == VTNewSub))
          {
            fvp = fvp1;
            fvp1 = fvp->prev(); ep1 = fvp1->getEdgePtr();
            fvp2 = fvp->next(); ep2 = fvp->getEdgePtr();
            insertEdge(fvp1, fvp2);
            deleteEdge(ep1); deleteEdge(ep2);
          }
       else if ((fvp2->getVertexType() == VTNewSub))
          {
            fvp = fvp2;
            fvp1 = fvp->prev();  ep1 = fvp1->getEdgePtr();
            fvp2 = fvp->next();  ep2 = fvp->getEdgePtr();
            insertEdge(fvp1, fvp2);
            deleteEdge(ep1); deleteEdge(ep2);
          }
     }

     // move the original vertices to a propriate position decided by length factor
  num_vertice = 0;	vl_first = vertex_list.begin();	vl_last	= vertex_list.end();
  while (vl_first != vl_last && num_vertice < num_old_vertice)
     {
       vp = (* vl_first); ++ vl_first; ++ num_vertice;
       if (vp->getType() != VTOld)
          continue;
       vp->getFaceVertices(fvparray);
       num_faceVertice = fvparray.size();
       newpos.reset();
       for (int i = 0; i < num_faceVertice; i++)
          newpos += fvparray[i]->next()->getVertexCoords();
       newpos = newpos/num_faceVertice;
       oldpos = vp->getCoords();
       newpos = length* oldpos + (1-length)* newpos;

       vp->setCoords(newpos);
     }

     // change the type of all the verteices back to VTNormal
  setNormalVertexType();
}

void DLFLObject :: dooSabinSubDivideBC(bool check)
{
  subDivideAllEdges();
  dooSabinSubDivide();
}

//---- End additions by Bwi & Cansin ----//


/*
  $Log: DLFLSubdiv.cc,v $
  Revision 4.8  2004/01/20 22:17:40  vinod
  Added Loop subdivision scheme

  Revision 4.7  2004/01/20 17:43:59  vinod
  Texture coords are now linearly interpolated  for Catmull-Clark subdivision

  Revision 4.6  2004/01/20 07:25:20  vinod
  Added checkerBoardRemeshing scheme. Changed name of subhw to dual1264

  Revision 4.5  2004/01/20 05:44:07  vinod
  Modified pentagonal preservation scheme to not use type flag, which causes problems for later operations.

  Revision 4.4  2004/01/20 00:57:41  vinod
  Changed name of Bei and Cansin's Loop scheme

  Revision 4.3  2004/01/19 20:20:11  vinod
  Added remeshing schemes by Bei & Cansin

  Revision 4.2  2004/01/13 02:13:16  vinod
  Added stellate and double stellate remeshing schemes.

  Revision 4.1  2004/01/02 21:00:55  vinod
  Additions by Doug: star, sqrt3 and fractal subdivision

  Revision 4.0  2003/12/26 01:58:53  vinod
  Major version sync.

  Revision 3.1  2003/10/20 00:30:06  vinod
  Modified subdiv algos to use make????Unique methods if possible

  Revision 3.0  2003/09/19 16:06:47  vinod
  Major version update

  Revision 2.24  2003/08/24 23:12:00  vinod
  Added 2nd pentagonal subdivision scheme

  Revision 2.23  2003/01/05 21:12:31  vinod
  Removed unused variables

  Revision 2.22  2002/12/30 04:08:40  vinod
  Added modified corner cutting subdiv scheme (Esan)

  Revision 2.21  2002/12/27 06:35:22  vinod
  Modified loops to include check for counter in loop conditional itself

  Revision 2.20  2002/12/17 03:53:40  vinod
  Fixed comment logs

  Revision 2.19  2002/12/17 03:18:37  vinod
  See comments for previous revision

  Revision 2.18  2002/12/17 02:55:53  vinod
  Additions by Paul
  Vertex cutting and Pentagonal subdivision schemes

  Revision 2.17  2002/12/17 02:44:59  vinod
  DLFLWindow.hh

  Revision 2.16  2002/11/24 00:52:27  vinod
  Changed Doo-Sabin subdivision to use connectEdges version withou loopcheck

  Revision 2.15  2002/11/07 08:19:34  vinod
  Added edge check flag for dooSabin subdivision

  Revision 2.14  2002/10/31 06:07:00  vinod
  Added simplest subdivision scheme

  Revision 2.13  2002/10/22 21:23:43  vinod
  Bug fix for taking care of C++ std. for counter variable scope in for loops

  Revision 2.12  2002/10/17 02:24:57  vinod
  Typo in comment

  Revision 2.11  2001/12/22 23:10:28  vinod
  Added weight factor for root-4 subdivision

  Revision 2.10  2001/12/17 17:35:02  vinod
  Modified root4 subdivision to allow twist, uses honeycomb type equation

  Revision 2.9  2001/10/08 00:50:11  vinod
  Slightly optimized catmull-clark subdivision

  Revision 2.8  2001/10/08 00:19:39  vinod
  Completed coordinate calculation for catmull-clark subdivision scheme

  Revision 2.7  2001/10/06 19:00:48  vinod
  Made corner-cutting subdivision a separate method

  Revision 2.6  2001/10/06 18:38:29  vinod
  Changed coefficient calculation for doo-Sabin

  Revision 2.5  2001/10/06 16:43:51  vinod
  Added proper coordinate calculation for dooSabin
  Completed implementation and debugging of catmull-clark subdivision method

  Revision 2.4  2001/09/28 17:46:53  vinod
  Rewrote catmull-clark subdivision using pointspheres

  Revision 2.3  2001/09/28 05:55:24  vinod
  Added Doo Sabin subdivision scheme.
  Modified honeycomb subdivision to produce smoother surfaces
  using new coordinate calculation algorithm.
  Added call to makeUnique on edge list to make sure Edge IDs are
  consecutive in honeycomb subdivision method

  Revision 2.2  2001/09/22 20:18:58  vinod
  Completed implementation of honeycomb subdivision scheme

  Revision 2.1  2001/09/19 04:00:23  vinod
  Added honeycomb and root4 subdivision schemes

  Revision 2.0  2001/07/25 05:12:45  vinod
  Major version sync

  Revision 1.4  2001/07/16 04:34:02  vinod
  Modified to use new list structure. Moved dooSabin to separate file

  Revision 1.3  2001/04/08 21:20:38  vinod
  *** empty log message ***

  Revision 1.2  2000/11/22 07:41:35  vinod
  Now uses the single face version of insertEdge

  Revision 1.1  2000/11/21 19:22:25  vinod
  Subdivision subroutines for DLFL

*/
