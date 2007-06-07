
#include "DLFLSubdiv.hh"
#include <DLFLCore.hh>
#include <DLFLCoreExt.hh>
#include "DLFLExtrude.hh"
#include "DLFLConnect.hh"

namespace DLFL {

  void loopSubdivide( DLFLObjectPtr obj ) {
    // Perform Loop subdivision

    // For every edge compute the new point coordinates and store in the aux coord
    DLFLEdgePtrList::iterator efirst, elast;
    DLFLEdgePtr ep;
    DLFLFaceVertexPtr fvp1, fvp2, fvp1p1, fvp1n2, fvp2p1, fvp2n2;
    Vector3d p1,p2,p1p1,p1n2,p2p1,p2n2;
    Vector3d newpt;
    int num_old_edges = 0;
    efirst = obj->beginEdge(); elast = obj->endEdge();
    while ( efirst != elast ) {
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
    DLFLVertexPtrList::iterator vfirst, vlast;
    DLFLVertexPtr vp;
    DLFLFaceVertexPtrArray fvparray;
    Vector3d op, p;
    int valence, num_old_verts=0;
    double beta;
    vfirst = obj->beginVertex(); vlast = obj->endVertex();
    while ( vfirst != vlast ) {
      vp = (*vfirst); ++vfirst; ++num_old_verts;
      vp->getFaceVertices(fvparray);
      valence = fvparray.size();
      if ( valence > 0 ) {
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
    efirst = obj->beginEdge(); elast = obj->endEdge();
    while ( efirst != elast && count < num_old_edges ) {
      ep = (*efirst); ++efirst; ++count;
      newpt = ep->getAuxCoords(); ep->resetAuxCoords();
      vp = subdivideEdge(obj,ep); vp->coords = newpt;
    }

    // Connect newly created midpoints in each face
    // Go through all old vertices (before edge subdivision) and
    // go through all corners for each vertex and connect previous and next corners
    vfirst = obj->beginVertex(); vlast = obj->endVertex();
    count = 0;
    while ( vfirst != vlast && count < num_old_verts ) {
      vp = (*vfirst); ++vfirst; ++count;
      vp->getFaceVertices(fvparray);
      for (int i=0; i < fvparray.size(); ++i)
	insertEdge(obj,fvparray[i]->prev(),fvparray[i]->next());
    }
  }

  void checkerBoardRemeshing(DLFLObjectPtr obj, double thickness) {
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
    int num_old_verts = obj->num_vertices( );
    int num_old_faces = obj->num_faces( );
    int num_old_edges = obj->num_edges( );
    int count;
    DLFLFacePtrList::iterator ffirst, flast;
    DLFLFacePtr fp;

    ffirst = obj->beginFace(); flast = obj->endFace();
    count = 0;
    while ( ffirst != flast && count < num_old_faces ) {
      fp = (*ffirst); ++ffirst; ++count;
      extrudeFacePlanarOffset(obj,fp,0,0,thickness,true);
    }

    // Trisect all the old edges using the same fractional thickness
    // Set the type of the new points for use later
    DLFLEdgePtrList::iterator efirst = obj->beginEdge();
    DLFLEdgePtr ep;
    count = 0;
    while ( count < num_old_edges ) {
      ep = (*efirst); ++efirst; ++count;
      trisectEdge(obj,ep,thickness*ep->length(),false,true);
    }

    // Complete the remeshing by inserting edges
    // Go through all the old vertices (before extrude and trisect)
    // For each vertex, go through the face-vertex list. For each face-vertex
    // in that list, connect the previous and next face-vertices with an edge
    // After doing that, find edges which do not connect to the points created
    // by trisection and delete them
    DLFLVertexPtrList::iterator vfirst, vlast;
    DLFLVertexPtr vp, ovp;
    DLFLFaceVertexPtrArray fvparray;
    DLFLFaceVertexPtr fvp;
    DLFLEdgePtrArray eparray;

    vfirst = obj->beginVertex(); vlast = obj->endVertex();
    count = 0;
    while ( count < num_old_verts ) {
      vp = (*vfirst); ++vfirst; ++count;
      vp->getFaceVertices(fvparray);
      for (int i=0; i < fvparray.size(); ++i) {
	fvp = fvparray[i];
	insertEdgeCoFacial(obj,fvp->prev(),fvp->next());
      }
      vp->getEdges(eparray);
      for (int i=0; i < eparray.size(); ++i) {
	ep = eparray[i];
	ovp = ep->getOtherVertexPointer(vp);
	if ( ovp->getType() != VTNewPoint ) deleteEdge(obj,ep);
	else ovp->resetType(); // Reset the type to avoid complications in subsequent operations
      }
    }
  }

  void simplestSubdivide( DLFLObjectPtr obj ) {
    // Simplest subdivision (halfway)

    // Keep track of number of old vertices before edges are subdivided
    int num_old_verts = obj->num_vertices();

    // Subdivide all the edges.
    subdivideAllEdges(obj);

    // All old edges will have to be deleted after new edges have been inserted
    // Keep track of number of old edges before we insert any new edges
    int num_old_edges = obj->num_edges();

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
	fvp1 = fvp1->next(); fvp2 = fvp2->prev();
	insertEdge(obj,fvp1,fvp2,false,matl);
      }
    }

    // Delete the old edges
    DLFLEdgePtrList::iterator el_first = obj->beginEdge();
    DLFLEdgePtrList::iterator el_last = obj->endEdge();
    DLFLEdgePtr ep = NULL;
    int num_edges = 0; 
    while ( el_first != el_last && num_edges < num_old_edges ) {
      ep = (*el_first); ++el_first; ++num_edges;
      deleteEdge(obj,ep,true);
    }
  }

  void vertexCuttingSubdivide(DLFLObjectPtr obj,double offset) {
    // Vertex-cutting subdivision

    // If the offset is 0.5 this is the same as simplest subdivision
    if ( areEqual(offset,0.5) ) {
      simplestSubdivide(obj);
      return;
    }

    // Subdivision makes sense only if the offset is non-zero
    if ( isNonZero(offset) == false ) return;

    // Keep track of number of old vertices before edges are subdivided
    int num_old_verts = obj->num_vertices();

    // Go through edge list and sub-divide all the old edges
    // into 3 parts using the offset parameter for the length ratio
    DLFLEdgePtrList::iterator first = obj->beginEdge();
    DLFLEdgePtr ep;
    int num_edges = 0, num_old_edges = obj->num_edges();
    Vector3d p1,p2;
    DLFLVertexPtr vp;

    while ( num_edges < num_old_edges ) {
      ep = (*first); ++first; ++num_edges;

      // Get end points (coordinates) of the edge
      ep->getEndPoints(p1,p2);
  
      // Subdivide the edge once and set coordinates of new point
      vp = subdivideEdge(obj,ep); vp->coords = p1 + offset*(p2-p1);

      // Get the last edge from the edge list.
      // This will be the edge connected to the second vertex, p2
      ep = obj->lastEdge();

      // Subdivide this edge and set coordinates of new point
      vp = subdivideEdge(obj,ep); vp->coords = p2 + offset*(p1-p2);
    }


    // Go through vertex list. For all old vertices (before edge subdivision)
    // go through the face-vertex list and insert edges between previous and
    // next corners to each face-vertex.
    // All old corner edges will have to be deleted after new edges have been inserted

    DLFLVertexPtrList::iterator vl_first, vl_last;
    vl_first = obj->beginVertex(); vl_last = obj->endVertex();

    DLFLFaceVertexPtrList fvplist;
    DLFLFaceVertexPtrList::iterator fvp_first, fvp_last;
    DLFLFaceVertexPtr fvp1, fvp2;
    DLFLMaterialPtr matl = (obj->firstFace())->material();
    int num_verts = 0;

    while ( vl_first != vl_last && num_verts < num_old_verts ) {
      vp = *vl_first; ++vl_first; ++num_verts; 

      fvplist = vp->getFaceVertexList();
      fvp_first = fvplist.begin(); fvp_last = fvplist.end();
      while ( fvp_first != fvp_last ) {
	fvp2 = fvp1 = (*fvp_first); ++fvp_first;

	// Insert an edge between previous and next corners
	fvp1 = fvp1->next(); fvp2 = fvp2->prev();
	insertEdge(obj,fvp1,fvp2,false,matl);
      }

      // Delete the old corner edges
      DLFLEdgePtr * ep = NULL;
      num_edges = vp->getEdges(&ep); 

      for ( int i=0; i < num_edges; i++ )
	deleteEdge(obj,ep[i],true); 
      delete [] ep;
    }
  }

  void pentagonalSubdivide2(DLFLObjectPtr obj,double scale_factor) {
    // Pentagonal subdivision - Version 2

    // First sub-divide all edges in the object.
    // Type flag NEED NOT be set for new vertices
    // The head pointer in each face will not change during edge subdivision
    // So we don't need to use the type flag.
    subdivideAllEdges(obj,2,false);

    // Go through each face and create a new face within each face using the
    // midpoints of the old edges, i.e. the newly created points after above
    // subdivision of all edges
  
    DLFLFacePtrList::iterator fl_first, fl_last;
    DLFLFacePtr fp, ifp;
    DLFLFaceVertexPtr head,fvp,ifvp;
    DLFLFaceVertexPtrArray fvplist1, fvplist2; // List of face-vertices which have to be connected
    int num_faces, num_old_faces, face_valence;
    Vector3dArray vertex_coords;

    // Reserve space in the two arrays for making connections. The size will be twice the number
    // of original edges. i.e. number of edges after subdividing all edges
    int num_edges = obj->num_edges();
    fvplist1.reserve(num_edges); fvplist2.reserve(num_edges);

    fl_first = obj->beginFace(); fl_last = obj->endFace();
    num_old_faces = obj->num_faces(); num_faces = 0;
    // New faces will be created in the loop, so make sure those aren't traversed
    while ( fl_first != fl_last && num_faces < num_old_faces ) {
      fp = (*fl_first);

      // If a face had less than 3 sides originally we don't include it.
      face_valence = fp->size();
      if ( face_valence >= 6 ) {
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
	while ( fvp != head ) {
	  vertex_coords.push_back(fvp->getVertexCoords());
	  fvp = fvp->next(); fvp = fvp->next();
	}

	// Scale the coordinates of the new face by given scale factor
	scale(vertex_coords,scale_factor);

	// Create the new face from the collected and scaled midpoints
	obj->createFace(vertex_coords);
            
	// Find the inward facing face of the two newly created ones
	ifp = obj->lastFace();

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
	while ( fvp != head ) {
	  fvplist1.push_back(fvp); fvplist2.push_back(ifvp);
	  fvp->resetType();
	  fvp = fvp->next(); fvp = fvp->next(); ifvp = ifvp->prev();
	}
      }
      ++fl_first; ++num_faces;
    }

    // Go through fvplist1 and fvplist2 and insert edges between corresponding face-vertices
    for (int i=0; i < fvplist1.size(); ++i) {
      if ( fvplist1[i] != NULL && fvplist2[i] != NULL )
	insertEdge(obj,fvplist1[i],fvplist2[i]);
      else
	cout << "NULL pointers found! i = " << i << " "
	     << fvplist1[i] << " " << fvplist2[i] << endl;
    }
  }

  void pentagonalSubdivide(DLFLObjectPtr obj,double offset) {
    // Pentagonal subdivision

    // Commonly used variables
    DLFLFacePtrList::iterator fl_first, fl_last;
    DLFLFacePtr fp;
    DLFLVertexPtr vp;
    int num_faces, num_old_faces;

    // First compute the coordinates of the new points and store them in the aux-coords
    // fields of each entity (vertex, edge, face)
  
    // Go through each face and compute the centroid and store it in the aux-coords field
    // Do the makeUnique also to make sure Face IDs are consecutive
    Vector3d cen;
    num_faces = 0;
    fl_first = obj->beginFace(); fl_last = obj->endFace();
    while ( fl_first != fl_last ) {
      fp = (*fl_first); ++fl_first; ++num_faces;
      cen = fp->geomCentroid();
      fp->setAuxCoords(cen); fp->makeUnique();
    }
	
    // Subdivide all the edges into 3 equal parts.
    subdivideAllEdges(obj,3);


    // Create point-spheres for each old Face. Insert edges between the point-sphere and
    // every other new vertex (created by edge subdivision) in each old face.
    // Also adjust the coordinates of the new vertices based on the offset.

    DLFLEdgePtr lastedge;
    DLFLFacePtr fptr1;
    DLFLFaceVertexPtr fvp1, fvp2;
    int numsides1;
    bool done;

    fl_first = obj->beginFace(); fl_last = obj->endFace();

    num_old_faces = num_faces; num_faces = 0;
    while ( fl_first != fl_last && num_faces < num_old_faces ) {
      fp = (*fl_first); ++fl_first; ++num_faces;

      cen = fp->getAuxCoords();
      fvp1 = obj->createPointSphere(cen,fp->material());

      // Get corner following the first corner in the face
      // The first corner will always be an old vertex of the face
      // Thus the corner following it will always be a new vertex
      // Adjacent faces will never use the same new vertex in an edge
      // because of the consistent rotation order

      fvp2 = (fp->firstVertex())->next();

      // Insert an edge between the point-sphere and the new corner
      // NOTE: Make sure point-sphere corner is given first
      lastedge = insertEdgeNonCoFacial(obj,fvp1,fvp2);

      done = false;
      while ( !done ) {
	fvp1 = lastedge->getFaceVertexPtr1();

	// Get face in which fvp1 is there. This will always be the face
	// in which the next edge has to be inserted because the point-sphere corner
	// is always specified first in the insertEdge call
	fptr1 = fvp1->getFacePtr(); numsides1 = fptr1->size();

	// If the face has more than 5 sides insert another edge
	if ( numsides1 > 5 ) {
	  fvp2 = fvp1->next();
				
	  // Update the position of the new vertex referred to by fvp2
	  vp = fvp2->getVertexPtr();
	  vp->coords += ( cen - vp->coords )*offset;

	  // Skip the next two corners to get the corner for next edge insertion
	  fvp2 = fvp2->next(); fvp2 = fvp2->next(); fvp2 = fvp2->next();

	  // Make sure point-sphere corner is specified first
	  lastedge = insertEdgeCoFacial(obj,fvp1,fvp2,false);
	} else {
	  // All edges in this face have been inserted.
	  done = true;
	}
      }
    }	 
  }

  void honeycombSubdivide(DLFLObjectPtr obj) {
    // Honey-comb subdivision

    // Go through list of faces and create the inner faces for each face
    DLFLFacePtrList::iterator fl_first, fl_last;
    DLFLEdgePtrList::iterator el_first, el_last;
    DLFLVertexPtrList::iterator vl_first, vl_last;
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
  
    num_old_verts = obj->num_vertices();
    num_old_faces = obj->num_faces();
    num_old_edges = obj->num_edges();

    // Apply make-unique on the obj->num_edges to make sure all Edge IDs are consecutive
    obj->makeEdgesUnique();
  
    // Reserve and create num_old_edges entries in the 2 temporary lists
    fvplist1.resize(num_old_edges,NULL); 
    fvplist2.resize(num_old_edges,NULL);

    // Find starting edge ID to use as offset.
    eistart = (obj->firstEdge())->getID();

    // For each old face create new faces and update the 2 temporary lists using the edge IDs
    // as index in the lists
    fl_first = obj->beginFace(); fl_last = obj->endFace();
    num_faces = 0;
    while ( fl_first != fl_last && num_faces < num_old_faces ) {
      fp = (*fl_first);

      fp->getVertexCoords(vertex_coords);
      num_verts = vertex_coords.size();

      // Find mid-points of all the edges in this face and store them
      midpoints = new Vector3d[num_verts];
      new_vertex_coords.reserve(num_verts);
      for (int i=0; i < num_verts; ++i) {
	p1 = vertex_coords[i];
	if ( i == num_verts-1 ) 
	  p2 = vertex_coords[0];
	else 
	  p2 = vertex_coords[i+1];
	midpoints[i] = (p1+p2)/2.0;
      }

      // Now using the mid-points compute new coordinates for the new polygon
      double coef, alpha;
      for (int i=0; i < num_verts; ++i) {
	p.reset();
	for (int j=0; j < num_verts; ++j) {
	  alpha = M_SQRT1_2 - 0.25 + 5.0/(4.0*num_verts);
	  if ( i == j ) coef = alpha;
	  else coef = (1.0 - alpha) * ( 3.0 + 2.0*cos(2.0*(i-j)*M_PI/num_verts) ) / (3.0*num_verts - 5.0);
	  p += coef*midpoints[j];
	}
	new_vertex_coords.push_back(p);
      }
            
      obj->createFace(new_vertex_coords);

      delete [] midpoints; midpoints = NULL;
      new_vertex_coords.clear();
      vertex_coords.clear();
            
      // Get second newly inserted face - the one which is facing inwards
      ifp = obj->lastFace();

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

      while ( fvp1 != head1 && fvp2 != head2 ) {
	ep = fvp1->getEdgePtr(); edgeindex = ep->getID() - eistart;
	if ( fvplist1[edgeindex] == NULL ) fvplist1[edgeindex] = fvp2;
	else fvplist2[edgeindex] = fvp2;
	fvp1 = fvp1->next(); fvp2 = fvp2->prev();
      }
      ++fl_first; ++num_faces;
    }

    // Go through the face_list,obj->num_edges and vertex_list and 
    // destroy all the old faces, edges and vertices
    num_faces = 0; 
    fl_first = obj->beginFace(); fl_last = obj->endFace();
    while ( fl_first != fl_last && num_faces < num_old_faces ) {
      fp = (*fl_first); ++fl_first; ++num_faces;
      obj->removeFace(fp); delete fp;
    }

    num_edges = 0; 
    el_first = obj->beginEdge(); el_last = obj->endEdge();
    while ( el_first != el_last && num_edges < num_old_edges ) {
      ep = (*el_first); ++el_first; ++num_edges;
      obj->removeEdge(ep); delete ep;
    }

    num_verts = 0; 
    vl_first = obj->beginVertex(); vl_last = obj->endVertex();
    while ( vl_first != vl_last && num_verts < num_old_verts ) {
      vp = (*vl_first); ++vl_first; ++num_verts;
      obj->removeVertex(vp); delete vp;
    }
  
    // Go through fvplist1 and fvplist2 and insert edges between corresponding face-vertices
    for (int i=0; i < num_old_edges; ++i) {
      if ( fvplist1[i] != NULL && fvplist2[i] != NULL )
	insertEdge(obj,fvplist1[i],fvplist2[i]);
      else
	cout << "NULL pointers found! i = " << i << " "
	     << fvplist1[i] << " " << fvplist2[i] << endl;
    }
  }

  void dooSabinSubdivide(DLFLObjectPtr obj,bool check) {
    // Regular Doo-Sabin subdivision scheme

    // Go through list of faces and create new inner faces for each face
    DLFLFacePtrList::iterator fl_first, fl_last;
    DLFLEdgePtrList::iterator el_first, el_last;
    DLFLVertexPtrList::iterator vl_first, vl_last;
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
  
    num_old_verts = obj->num_vertices();
    num_old_faces = obj->num_faces();
    num_old_edges = obj->num_edges();

    // Apply make-unique on the obj->num_edges to make sure all Edge IDs are consecutive
    obj->makeEdgesUnique();
  
    // Reserve and create num_old_edges entries in the 2 temporary lists
    eplist1.resize(num_old_edges,NULL); eplist2.resize(num_old_edges,NULL);
  
    // Find starting edge ID to use as offset.
    eistart = (obj->lastEdge())->getID();

    fl_first = obj->beginFace(); fl_last = obj->endFace(); num_faces = 0;
    while ( fl_first != fl_last && num_faces < num_old_faces ) {
      fp = (*fl_first);

      fp->getVertexCoords(vertex_coords);

      num_verts = vertex_coords.size();
      new_vertex_coords.reserve(num_verts);
            
      // Compute new coordinates for the new polygon
      double coef;
      for (int i=0; i < num_verts; ++i) {
	p.reset();
	for (int j=0; j < num_verts; ++j) {
	  if ( i == j ) coef = 0.25 + 5.0/(4.0*num_verts);
	  else coef = ( 3.0 + 2.0*cos(2.0*(i-j)*M_PI/num_verts) ) / (4.0*num_verts);
	  p += coef*vertex_coords[j];
	}
	new_vertex_coords.push_back(p);
      }
      obj->createFace(new_vertex_coords);
      new_vertex_coords.clear(); vertex_coords.clear();

      // Get the second newly inserted face, the one facing inwards
      ifp = obj->lastFace();
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
      if ( eplist1[edgeindex] == NULL ) 
	eplist1[edgeindex] = iep;
      else                              
	eplist2[edgeindex] = iep;
      fvp1 = fvp1->next(); fvp2 = fvp2->prev();

      while ( fvp1 != head1 && fvp2 != head2 ) {
	ep = fvp1->getEdgePtr(); iep = fvp2->getEdgePtr();
	edgeindex = ep->getID() - eistart;
	if ( eplist1[edgeindex] == NULL ) 
	  eplist1[edgeindex] = iep;
	else                              
	  eplist2[edgeindex] = iep;
	fvp1 = fvp1->next(); fvp2 = fvp2->prev();
      }
      ++fl_first; ++num_faces;
    }

    // Go through the face_list,obj->num_edges and vertex_list 
    // and destroy all the old faces, edges and vertices
    num_faces = 0; 
    fl_first = obj->beginFace(); fl_last = obj->endFace();
    while ( fl_first != fl_last && num_faces < num_old_faces ) {
      fp = (*fl_first); ++fl_first; ++num_faces;
      obj->removeFace(fp); delete fp;
    }

    num_edges = 0; 
    el_first = obj->beginEdge(); el_last = obj->endEdge();
    while ( el_first != el_last && num_edges < num_old_edges ) {
      ep = (*el_first); ++el_first; ++num_edges;
      obj->removeEdge(ep); delete ep;
    }

    num_verts = 0; 
    vl_first = obj->beginVertex(); vl_last = obj->endVertex();
    while ( vl_first != vl_last && num_verts < num_old_verts ) {
      vp = (*vl_first); ++vl_first; ++num_verts;
      obj->removeVertex(vp); delete vp;
    }

    // Go through eplist1,fplist1 and eplist2,fplist2 and connect corresponding half-edges
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

	connectEdgesWithoutLoopCheck(obj,eplist1[i],fp1,eplist2[i],fp2,check);
      } else
	cout << "NULL pointers found! i = " << i << " "
	     << eplist1[i] << " -- " << eplist2[i] << endl;
    }
  }

  void cornerCuttingSubdivide(DLFLObjectPtr obj) {
    // Corner-cutting subdivision scheme
    // Tension parameter is calculated based on number of vertices in face

    // Go through list of faces and create new inner faces for each face
    DLFLFacePtrList::iterator fl_first, fl_last;
    DLFLEdgePtrList::iterator el_first, el_last;
    DLFLVertexPtrList::iterator vl_first, vl_last;
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
  
    num_old_verts = obj->num_vertices();
    num_old_faces = obj->num_faces();
    num_old_edges = obj->num_edges();

    // Apply make-unique on the obj->num_edges to make sure all Edge IDs are consecutive
    obj->makeEdgesUnique();
  
    // Reserve and create num_old_edges entries in the 2 temporary lists
    eplist1.resize(num_old_edges,NULL); eplist2.resize(num_old_edges,NULL);
  
    // Find starting edge ID to use as offset.
    eistart = (obj->firstEdge())->getID();

    fl_first = obj->beginFace(); fl_last = obj->endFace();
    num_faces = 0;
    while ( fl_first != fl_last && num_faces < num_old_faces ) {
      fp = (*fl_first);

      fp->getVertexCoords(vertex_coords);

      num_verts = vertex_coords.size();
      new_vertex_coords.reserve(num_verts);
            
      // Compute new coordinates for the new polygon
      double coef, alpha;
      //alpha = 12.0/16.0;
      for (int i=0; i < num_verts; ++i) {
	p.reset();
	for (int j=0; j < num_verts; ++j) {
	  alpha = 0.25 + 5.0/(4.0*num_verts);
	  if ( i == j ) coef = alpha;
	  else coef = (1.0 - alpha) * ( 3.0 + 2.0*cos(2.0*(i-j)*M_PI/num_verts) ) / (3.0*num_verts - 5.0);
	  p += coef*vertex_coords[j];
	}
	new_vertex_coords.push_back(p);
      }
      obj->createFace(new_vertex_coords);
      new_vertex_coords.clear(); vertex_coords.clear();

      // Get the second newly inserted face, the one facing inwards
      ifp = obj->lastFace();
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
      if ( eplist1[edgeindex] == NULL ) 
	eplist1[edgeindex] = iep;
      else                              
	eplist2[edgeindex] = iep;
      fvp1 = fvp1->next(); fvp2 = fvp2->prev();

      while ( fvp1 != head1 && fvp2 != head2 ) {
	ep = fvp1->getEdgePtr(); iep = fvp2->getEdgePtr();
	edgeindex = ep->getID() - eistart;
	if ( eplist1[edgeindex] == NULL ) 
	  eplist1[edgeindex] = iep;
	else                              
	  eplist2[edgeindex] = iep;
	fvp1 = fvp1->next(); fvp2 = fvp2->prev();
      }
      ++fl_first; ++num_faces;
    }

    // Go through the face_list,obj->num_edges and vertex_list and destroy all the old faces, edges and vertices
    num_faces = 0; fl_first = obj->beginFace(); fl_last = obj->endFace();
    while ( fl_first != fl_last && num_faces < num_old_faces ) {
      fp = (*fl_first); ++fl_first; ++num_faces;
      obj->removeFace(fp); delete fp;
    }

    num_edges = 0; el_first = obj->beginEdge(); el_last = obj->endEdge();
    while ( el_first != el_last && num_edges < num_old_edges ) {
      ep = (*el_first); ++el_first; ++num_edges;
      obj->removeEdge(ep); delete ep;
    }

    num_verts = 0; vl_first = obj->beginVertex(); vl_last = obj->endVertex();
    while ( vl_first != vl_last && num_verts < num_old_verts ) {
      vp = (*vl_first); ++vl_first; ++num_verts;
      obj->removeVertex(vp); delete vp;
    }

    // Go through eplist1,fplist1 and eplist2,fplist2 and connect corresponding half-edges
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
      } else
	cout << "NULL pointers found! i = " << i << " "
	     << eplist1[i] << " -- " << eplist2[i] << endl;
    }
  }

  void modifiedCornerCuttingSubdivide(DLFLObjectPtr obj,double thickness) {
    // (Modified) Corner-cutting subdivision scheme
    // Tension parameter is calculated based on number of vertices in face

    // Go through list of faces and create new inner faces for each face
    DLFLFacePtrList::iterator fl_first, fl_last , fl_last_hole;
    DLFLEdgePtrList::iterator el_first, el_last;
    DLFLVertexPtrList::iterator vl_first, vl_last;
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
  
    num_old_verts = obj->num_vertices();
    num_old_faces = obj->num_faces();
    num_old_edges = obj->num_edges();

    // Apply make-unique on the obj->num_edges to make sure all Edge IDs are consecutive
    DLFLEdgePtrList::iterator efirst=obj->beginEdge(), elast=obj->endEdge();

    obj->makeEdgesUnique();
  
    // Reserve and create num_old_edges entries in the 2 temporary lists
    eplist1.resize(num_old_edges,NULL); eplist2.resize(num_old_edges,NULL);
  
    // Find starting edge ID to use as offset.
    eistart = (obj->firstEdge())->getID();

    fl_first = obj->beginFace(); fl_last = obj->endFace();
    num_faces = 0;
    while ( fl_first != fl_last && num_faces < num_old_faces ) {
      fp = (*fl_first);

      fp->getVertexCoords(vertex_coords);

      num_verts = vertex_coords.size();
      new_vertex_coords.reserve(num_verts);
            
      // Compute new coordinates for the new polygon
      //double coef, alpha;
      //alpha = 12.0/16.0;
      for (int i=0; i < num_verts; ++i) {
	t = thickness/2.0;
	v0 = vertex_coords[i];
	if ( i > 0 ) 
	  v1 = vertex_coords[i-1];
	else 
	  v1 = vertex_coords[num_verts-1];
	if 
	  ( i < num_verts-1 ) v2 = vertex_coords[i+1];
	else 
	  v2 = vertex_coords[0];
	if ( norm(v1-v2) < 0.001 ) {
	  if ( i < num_verts-2 ) 
	    v2 = vertex_coords[i+2]; 
	  else if ( i == num_verts-1 ) 
	    v2 = vertex_coords[0]; 
	  else 
	    v2 = vertex_coords[1];
	}
	n1 = normalized(v1-v0);
	n2 = normalized(v2-v0);
	n1n2 = n1*n2;
	x = sqrt(t*t/(1.0-n1n2*n1n2));
	v0hat = x*(n1+n2) + v0;
	p = v0hat;

	new_vertex_coords.push_back(p);
      }

      obj->createFace(new_vertex_coords);
      new_vertex_coords.clear(); vertex_coords.clear();

      // Get the second newly inserted face, the one facing inwards
      ifp = obj->lastFace();
      // Set type of inner face so we can use the type to
      // determine which face to use for edge connects
      ifp->setType(FTNew);
      ////////////////////////////////////////////////////////////////////
      fl_last_hole = obj->endFace();
      --fl_last_hole; --fl_last_hole; 
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
      if ( eplist1[edgeindex] == NULL ) 
	eplist1[edgeindex] = iep;
      else
	eplist2[edgeindex] = iep;
      fvp1 = fvp1->next(); fvp2 = fvp2->prev();

      while ( fvp1 != head1 && fvp2 != head2 ) {
	ep = fvp1->getEdgePtr(); iep = fvp2->getEdgePtr();
	edgeindex = ep->getID() - eistart;
	if ( eplist1[edgeindex] == NULL ) 
	  eplist1[edgeindex] = iep;
	else
	  eplist2[edgeindex] = iep;
	fvp1 = fvp1->next(); fvp2 = fvp2->prev();
      }
      ++fl_first; ++num_faces;
    }

    // Go through the face_list,obj->num_edges and vertex_list and destroy all the old faces, edges and vertices
    num_faces = 0; fl_first = obj->beginFace(); fl_last = obj->endFace();
    while ( fl_first != fl_last && num_faces < num_old_faces ) {
      fp = (*fl_first); ++fl_first; ++num_faces;
      obj->removeFace(fp); delete fp;
    }

    num_edges = 0; el_first = obj->beginEdge(); el_last = obj->endEdge();
    while ( el_first != el_last && num_edges < num_old_edges ) {
      ep = (*el_first); ++el_first; ++num_edges;
      obj->removeEdge(ep); delete ep;
    }

    num_verts = 0; vl_first = obj->beginVertex(); vl_last = obj->endVertex();
    while ( vl_first != vl_last && num_verts < num_old_verts ) {
      vp = (*vl_first); ++vl_first; ++num_verts;
      obj->removeVertex(vp); delete vp;
    }

    // Go through eplist1,fplist1 and eplist2,fplist2 and connect corresponding half-edges
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
      } else
	cout << "NULL pointers found! i = " << i << " "
	     << eplist1[i] << " -- " << eplist2[i] << endl;
    }
  }

  void root4Subdivide(DLFLObjectPtr obj, double a, double twist) {
    // Root-4 subdivision
    // Go through list of faces and create the inner faces for each face
    DLFLFacePtrList::iterator fl_first = obj->beginFace();
    DLFLFacePtrList::iterator fl_last = obj->endFace();
    DLFLFacePtr faceptr;
    Vector3dArray new_vertex_coords,vertex_coords;
    Vector3d * midpoints;
    Vector3d p1,p2,p;
    DLFLFacePtrArray oldfaces, newfaces;                 // List of faces to be connected
    int numoldedges = obj->num_edges();                  // Keep track of no. of edges in un-sub-divided object
    // All old edges have to be deleted in the end
    int numoldverts = obj->num_vertices();                // No. of old vertices which have to be moved
    int numoldfaces = obj->num_faces();
    int numfaces = 0;
    int num_verts;
  
    while ( fl_first != fl_last && numfaces < numoldfaces ) {
      faceptr = (*fl_first);

      faceptr->getVertexCoords(vertex_coords);
      num_verts = vertex_coords.size();

      // Find mid-points of all the edges in this face and store them
      midpoints = new Vector3d[num_verts];
      new_vertex_coords.reserve(num_verts);
      for (int i=0; i < num_verts; ++i) {
	p1 = vertex_coords[i];
	if ( i == num_verts-1 ) p2 = vertex_coords[0];
	else p2 = vertex_coords[i+1];
	midpoints[i] = (1.0-twist)*p1 + twist*p2;
      }

      // Now using the mid-points compute new coordinates for the new polygon
      double coef, alpha;
      for (int i=0; i < num_verts; ++i) {
	p.reset();
	for (int j=0; j < num_verts; ++j) {
	  alpha = M_SQRT1_2 - 0.25 + 5.0/(4.0*num_verts);
	  if ( i == j ) coef = alpha;
	  else coef = (1.0 - alpha) * ( 3.0 + 2.0*cos(2.0*(i-j)*M_PI/num_verts) ) / (3.0*num_verts - 5.0);
	  p += coef*midpoints[j];
	}
	new_vertex_coords.push_back(p);
      }
            
      obj->createFace(new_vertex_coords);

      delete [] midpoints; midpoints = NULL;
      new_vertex_coords.clear();
      vertex_coords.clear();

      // Old face and new face facing the old face have to be connected.
      // Store the pointers to the 2 faces in the 2 temporary arrays
      oldfaces.push_back(faceptr); newfaces.push_back(obj->lastFace());
      ++fl_first; ++numfaces;
    }

    // Go through list of faces to be connected and make the connections
    int numconn = oldfaces.size();
    DLFLFacePtr oldface=NULL, newface=NULL;
    for (int i=0; i < numconn; ++i) {
      oldface = oldfaces[i]; newface = newfaces[i];
       
      // Similar to extrude operation

      // Find the first face-vertices in the 2 faces to be connected
      DLFLFaceVertexPtr fvp1, fvp2;
      fvp1 = oldface->firstVertex(); fvp2 = newface->firstVertex();
      connectFaces(obj,fvp1,fvp2);
    }
    oldfaces.clear(); newfaces.clear();
  
    // Go through the obj->num_edges and delete all the old edges
    int count=0;
    DLFLEdgePtrList::iterator el_first, el_last;
    DLFLEdgePtr eptr = NULL;
    el_first = obj->beginEdge(); el_last = obj->endEdge();
  
    while ( el_first != el_last ) {
      eptr = (*el_first);
      ++el_first; ++count;
      deleteEdge(obj,eptr);
      if ( count >= numoldedges ) break; // Done with old edges
    }

    // Go through vertex list and move all the old vertices
    count = 0;
    DLFLVertexPtrList::iterator vl_first, vl_last;
    DLFLVertexPtr vptr = NULL;
    vl_first = obj->beginVertex(); vl_last = obj->endVertex();
    DLFLEdgePtr * iedges = NULL; // Edges indident on a vertex
    DLFLEdgePtr iedge = NULL; // Incident edge
    int numiedges = 0;
    Vector3d mp,psum,np;
  
    while ( vl_first != vl_last ) {
      vptr = (*vl_first);
      ++vl_first; ++count;

      // Move vertex
      p = vptr->coords;
       
      numiedges = vptr->getEdges(&iedges);

      psum.reset();
      for (int j=0; j < numiedges; ++j) {
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

  
  void catmullClarkSubdivide( DLFLObjectPtr obj ) {
    // Catmull-Clark subdivision surfaces implementation

    // Commonly used variables
    DLFLFacePtrList::iterator fl_first, fl_last;
    DLFLEdgePtrList::iterator el_first, el_last;
    DLFLVertexPtrList::iterator vl_first, vl_last;
    DLFLFacePtr fp, efp1, efp2;
    DLFLEdgePtr ep;
    DLFLVertexPtr vp;
    DLFLFaceVertexPtr fvp;
    int num_faces, num_edges;
    int num_old_faces, num_old_edges;
  

    // Reset aux coords in each vertex
    vl_first = obj->beginVertex(); vl_last = obj->endVertex();
    while ( vl_first != vl_last ) {
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
    fl_first = obj->beginFace(); fl_last = obj->endFace();
    while ( fl_first != fl_last ) {
      fp = (*fl_first); ++fl_first; ++num_faces;
      cen = fp->geomCentroid();
      fp->setAuxCoords(cen); fp->makeUnique();

      // Send contribution of this face to all vertices in this face
      DLFLFaceVertexPtr current, head;
      current = head = fp->front();
      current->vertex->addToAuxCoords(cen); current = current->next();
      while ( current != head ) {
	current->vertex->addToAuxCoords(cen); current = current->next();
      }
    }

    // Go through each edge and compute the new edge point coordinates
    // Send contribution of each edge to the vertexes at the ends
    Vector3d mp,afp,edgept;
    num_edges = 0;
    el_first = obj->beginEdge(); el_last = obj->endEdge();
    while ( el_first != el_last ) {
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
    vl_first = obj->beginVertex(); vl_last = obj->endVertex();
    while ( vl_first != vl_last ) {
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
    fl_first = obj->beginFace(); fl_last = obj->endFace();
    fistart = (*fl_first)->getID();

    num_old_faces = num_faces; num_faces = 0;
    while ( fl_first != fl_last && num_faces < num_old_faces ) {
      fp = (*fl_first); ++fl_first; ++num_faces;
      faceindex = fp->getID() - fistart;
      fvp = obj->createPointSphere(fp->getAuxCoords(),fp->material());
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
    num_edges = 0; num_old_edges = obj->num_edges();
    connindex = 0; numconn = 2*num_old_edges;
  
    fvplist.resize(numconn,NULL); vplist.resize(numconn,NULL);

    el_first = obj->beginEdge(); el_last = obj->endEdge();
    fvparray.reserve(2);
    while ( el_first != el_last && num_edges < num_old_edges ) {
      ep = (*el_first); ++el_first; ++num_edges;

      edgept = ep->getAuxCoords(); ep->resetAuxCoords();
      vp = subdivideEdge(obj,ep); vp->coords = edgept;
      vp->getFaceVertices(fvparray);

      for (int i=0; i < fvparray.size(); ++i) {
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
    for (int j=0; j < numconn; ++j) {
      fvp1 = fvplist[j];
       
      // Find the face-vertex referring to vp which is in the same face as fvp1
      // If only 1 face-vertex refers to vp, use that instead
      vp = vplist[j];
      fvp2 = vp->getFaceVertexInFace(fvp1->getFacePtr());
      insertEdge(obj,fvp1,fvp2);
    }
    fvplist.clear(); vplist.clear();
  }

  void starSubdivide(DLFLObjectPtr obj,double offset) { // Doug
    // Star subdivision
  
    // Commonly used variables
    DLFLFacePtrList::iterator fl_first, fl_last;
    DLFLFacePtr fp;
    DLFLVertexPtrList::iterator vl_first, vl_last;
    DLFLVertexPtr vp;

    int num_faces, num_old_faces, num_orig_faces;
    int numVect1, numVect2;
  
    num_faces = 0;
    num_old_faces = obj->num_faces();
    num_orig_faces = num_old_faces;
    fl_first = obj->beginFace(); fl_last = obj->endFace();

    numVect1 = obj->num_vertices();
    vl_first = obj->beginVertex();

    Vector3d NormList[num_orig_faces];

    //Stellate each face
    while ( num_faces < num_old_faces ) {
      fp = (*fl_first); 
      ++fl_first;
      NormList[num_faces] = fp->normalCentroid();
      stellateFace(obj,fp, 0.0);
      ++num_faces;
    }

    numVect2 = obj->num_vertices();

    num_faces = 0;
    num_old_faces = obj->num_faces();
    fl_first = obj->beginFace(); fl_last = obj->endFace();
  
    //Stellate all the faces again
    while ( num_faces < num_old_faces ) {
      fp = (*fl_first); 
      ++fl_first; 
      stellateFace(obj,fp, 0.0);
      ++num_faces;
    }

    int NormCtr = 0;
    int VectCtr = 0;
    while ( VectCtr < numVect1 ) {
      vp = (*vl_first);
      ++vl_first;
      ++VectCtr;
    }
  
    while ( VectCtr < numVect2 ) {
      vp = (*vl_first);
      ++vl_first;
      ++VectCtr;
      vp->coords = vp->coords + offset*NormList[NormCtr];
      ++NormCtr;
    }

  }

  void sqrt3Subdivide( DLFLObjectPtr obj ) { // Doug
    // Sqrt(3) subdivision

    // Commonly used variables
    DLFLFacePtrList::iterator fl_first, fl_last;
    DLFLFacePtr fp;
    int num_faces, num_old_faces, num_old_edges;
  
    num_faces = 0;
    num_old_faces = obj->num_faces();
    num_old_edges = obj->num_edges();

    fl_first = obj->beginFace(); fl_last = obj->endFace();

    //Stellate all the faces
    while ( num_faces < num_old_faces ) {
      fp = (*fl_first); 
      ++fl_first; 
      stellateFace(obj,fp, 0.0);
      ++num_faces;
    }
  
    // Delete the old edges
    DLFLEdgePtrList::iterator el_first = obj->beginEdge();
    DLFLEdgePtrList::iterator el_last = obj->endEdge();
    DLFLEdgePtr ep = NULL;
    int num_edges = 0; 
  
    while ( el_first != el_last && num_edges < num_old_edges ) {
      ep = (*el_first); ++el_first; ++num_edges;
      deleteEdge(obj,ep,true);
    }

    DLFLFaceVertexPtr fvp1, fvp2;

    num_faces = 0;
    num_old_faces = obj->num_faces();
    fl_first = obj->beginFace(); fl_last = obj->endFace();

    while ( num_faces < num_old_faces ) {
      fp = (*fl_first);
      ++fl_first;
      ++num_faces;
      fvp1 = (fp->firstVertex())->next();
      fvp2 = fvp1->next()->next();
      insertEdge(obj,fvp1,fvp2);
    }
  }

  void fractalSubdivide(DLFLObjectPtr obj, double offset) { // Doug
    // Fractal

    DLFLFacePtrList::iterator fl_first, fl_last;
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
  
    num_old_faces = obj->num_faces();
    fl_first = obj->beginFace(); fl_last = obj->endFace();

    subdivideAllEdges(obj);

    while ( num_faces < num_old_faces ) {
      fp = (*fl_first); ++fl_first; ++num_faces;

      dir = fp->normalCentroid();

      DLFLMaterialPtr matl = fp->material();

      fvp2 = fp->firstVertex()->next();
      fvp3 = fvp2->next(); fvp3 = fvp3->next();


      // number of sides on the face
      numsidesALL = fp->size();

      fvpTest = fvp2;

      for (i=0; i < (numsidesALL/2); i++) {
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

      fvp1 = obj->createPointSphere(fp->geomCentroid()+d*dir,matl);

      insertEdgeCoFacial(obj,fvp2,fvp3,false);
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
	  fvp3 = fvp2->next(); fvp3 = fvp3->next();

	  // Make sure point-sphere corner is specified first
	  insertEdgeCoFacial(obj,fvp2,fvp3,false);
	  lastedge = insertEdgeCoFacial(obj,fvp1,fvp2,false);
	} else {
	  // All edges have been inserted.
	  done = true;
	}
      }
    }
  }

  void stellateSubdivide( DLFLObjectPtr obj ) { // Eric
    DLFLFacePtrList::iterator fl_first, fl_last;
    DLFLFacePtr fp;
    int num_faces, num_old_faces;
    int num_old_edges;
    Vector3d cen;
    Vector3d cen_to_edge;

    num_old_faces = obj->num_faces();
    num_old_edges = obj->num_edges();

    num_faces = 0;
    fl_first = obj->beginFace(); fl_last = obj->endFace();
    while ( fl_first != fl_last && num_faces < num_old_faces ) {
      fp = (*fl_first); ++fl_first; ++num_faces;
      cen = fp->geomCentroid();
      fp->setAuxCoords(cen); fp->makeUnique();

      stellateFace(obj, fp, 0);
    }

    // Go through the obj->num_edges and delete all the old edges
    int count=0;
    DLFLEdgePtrList::iterator el_first, el_last;
    DLFLEdgePtr eptr = NULL;
    el_first = obj->beginEdge(); el_last = obj->endEdge();

    while ( el_first != el_last ) {
      eptr = (*el_first);
      ++el_first; ++count;
      deleteEdge(obj,eptr);
      if ( count >= num_old_edges ) break; // Done with old edges
    }
  }

  void twostellateSubdivide(DLFLObjectPtr obj, double offset, double curve) { // Eric
    DLFLFacePtrList::iterator fl_first, fl_last;
    DLFLFacePtr fp;
    int num_faces, num_old_faces;
    int num_old_edges;
    Vector3d cen;

    num_old_faces = obj->num_faces();

    num_faces = 0;
    fl_first = obj->beginFace(); fl_last = obj->endFace();
    while ( fl_first != fl_last && num_faces < num_old_faces ) {
      fp = (*fl_first); ++fl_first; ++num_faces;
      cen = fp->geomCentroid();
      fp->setAuxCoords(cen); fp->makeUnique();
      stellateFace(obj,fp, offset);
    }

    num_old_faces = obj->num_faces();
    num_old_edges = obj->num_edges();

    num_faces = 0;
    fl_first = obj->beginFace(); fl_last = obj->endFace();
    while ( fl_first != fl_last && num_faces < num_old_faces ) {
      fp = (*fl_first); ++fl_first; ++num_faces;
      cen = fp->geomCentroid();
      fp->setAuxCoords(cen); fp->makeUnique();
      stellateFace(obj,fp, curve);
    }

    // Go through the obj->num_edges and delete all the old edges
    int count=0;
    DLFLEdgePtrList::iterator el_first, el_last;
    DLFLEdgePtr eptr = NULL;
    el_first = obj->beginEdge(); el_last = obj->endEdge();
    while ( el_first != el_last ) {
      eptr = (*el_first);
      ++el_first; ++count;
      deleteEdge(obj,eptr);
      if ( count >= num_old_edges ) break; // Done with old edges
    }
  }

  //----- Begin Additions by Bei & Cansin -----//

  void setOldVertexType(DLFLObjectPtr obj) {
    DLFLVertexPtrList::iterator first;
    int num_old_vertex, num_vertex;

    num_old_vertex = obj->num_vertices();
    num_vertex = 0;
    first = obj->beginVertex();
    while ( num_vertex < num_old_vertex ) {
      (*first)->setType(VTOld);
      num_vertex++;
      first++;
    }
  }

  void setNewSubVertexType(DLFLObjectPtr obj) {
    DLFLVertexPtrList::iterator first;
    int num_old_vertex, num_vertex;

    num_old_vertex = obj->num_vertices();
    num_vertex = 0;
    first = obj->beginVertex();
    while ( num_vertex < num_old_vertex ) {
      if ((*first)->getType() != VTOld)
	(*first)->setType(VTNewSub);
      num_vertex++;
      first++;
    }	
  }

  void setNormalVertexType(DLFLObjectPtr obj) {
    DLFLVertexPtrList::iterator first;
    int num_old_vertex, num_vertex;

    num_old_vertex = obj->num_vertices();
    num_vertex = 0;
    first = obj->beginVertex();
    while ( num_vertex < num_old_vertex ) {
      (*first)->setType(VTNormal);
      num_vertex++;
      first++;
    }
  }

  void domeSubdivide(DLFLObjectPtr obj, double length, double sf) {	
    DLFLFacePtrList::iterator fl_first, fl_last;
    DLFLFacePtr fp;
    int num_old_faces;
    int num_faces;

    num_old_faces = obj->num_faces();

    // Subdivide all the edges by 4
    subdivideAllEdges(obj,4);

    num_faces = 0; fl_first = obj->beginFace(); fl_last = obj->endFace();
    while ( fl_first != fl_last && num_faces < num_old_faces ) {
      fp = (*fl_first); ++fl_first; ++num_faces;
      fp = extrudeFaceDS(obj,fp, 0.0*length, 1, 0.0, 1.6*sf);
      fp = extrudeFaceDS(obj,fp, 0.3*length, 1, 0.0, 1.7*sf);
      fp = extrudeFaceDS(obj,fp, 0.18*length, 1, 0.0, 1.6*sf);
      fp = extrudeFaceDS(obj,fp, 0.1*length, 1, 0.0, 1.4*sf);
      fp = extrudeFaceDS(obj,fp, 0.05*length, 1, 0.0, 1.2*sf);
      fp = extrudeFaceDS(obj,fp, 0.025*length, 1, 0.0, 1.1*sf);
      fp = extrudeFaceDS(obj,fp, 0.01*length, 1, 0.0, 0.01*sf);		
    }
  }

  void dual1264Subdivide(DLFLObjectPtr obj, double sf) {
    DLFLFacePtrList::iterator fl_first, fl_last;
    DLFLEdgePtrList::iterator el_first, el_last;
    DLFLVertexPtrList::iterator vl_first, vl_last;
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

    num_old_verts = obj->num_vertices();
    num_old_faces = obj->num_faces();
    num_old_edges = obj->num_edges();

    // Apply make-unique on the obj->num_edges to make sure all Edge IDs are consecutive
    DLFLEdgePtrList::iterator efirst=obj->beginEdge(), elast=obj->endEdge();
    while ( efirst != elast ) {
      (*efirst)->makeUnique();
      ++efirst;
    }
  
    // Reserve and create num_old_edges entries in the 2 temporary lists
    eplist1.reserve(num_old_edges); eplist2.reserve(num_old_edges);
    eplist1.resize(num_old_edges,NULL); eplist2.resize(num_old_edges,NULL);

    // Find starting edge ID to use as offset.
    eistart = (obj->firstEdge())->getID();

    fl_first = obj->beginFace(); fl_last = obj->endFace(); num_faces = 0;
    while ( fl_first != fl_last && num_faces < num_old_faces ) {
      fp = (*fl_first); ++fl_first; ++num_faces;
        
      // First compute the coordinates of the vertices of the new points and store them in an array
      head = fp->front();
      if ( head ) {
	current = head;
			
	vert1 = current->getVertexCoords();
	vert2 = current->next()->getVertexCoords();
	new_vertex_coords.push_back(vert1 * 2/3 + vert2 * 1/3);
	new_vertex_coords.push_back(vert1 * 1/3 + vert2 * 2/3);

	current = current->next();
	while ( current != head ) {
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

	obj->createFace(new_vertex_coords,fp->material());

	new_vertex_coords.clear();
	ifp = obj->lastFace();
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
	while ( fvp1 != head1 && fvp2 != head2 ) {
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

    // Go through the face_list,obj->num_edges and vertex_list and destroy all the old faces, edges and vertices
    num_faces = 0; fl_first = obj->beginFace(); fl_last = obj->endFace();
    while ( fl_first != fl_last && num_faces < num_old_faces ) {
      fp = (*fl_first); ++fl_first; ++num_faces;
      obj->removeFace(fp); delete fp;
    }

    num_edges = 0; el_first = obj->beginEdge(); el_last = obj->endEdge();
    while ( el_first != el_last && num_edges < num_old_edges ) {
      ep = (*el_first); ++el_first; ++num_edges;
      obj->removeEdge(ep); delete ep;
    }

    num_verts = 0; vl_first = obj->beginVertex(); vl_last = obj->endVertex();
    while ( vl_first != vl_last && num_verts < num_old_verts ) {
      vp = (*vl_first); ++vl_first; ++num_verts;
      obj->removeVertex(vp); delete vp;
    }
	
    // Go through eplist1,fplist1 and eplist2,fplist2 and connect corresponding half-edges
    DLFLFacePtr fp1, fp2, tfp1, tfp2;
    for (int i=0; i < num_old_edges; ++i) {
      if ( eplist1[i] != NULL && eplist2[i] != NULL ) {
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

	connectEdgesWithoutLoopCheck(obj,eplist1[i],fp1,eplist2[i],fp2);
      } else
	cout << "NULL pointers found! i = " << i << " " << eplist1[i] << " -- " << eplist2[i] << endl;
    }	
  }

  void loopStyleSubdivide(DLFLObjectPtr obj,double length)  { // bei 12-11-03
    DLFLFacePtrList::iterator fl_first, fl_last;
    DLFLVertexPtrList::iterator vl_first, vl_last;
    DLFLFaceVertexPtrArray fvparray;
    DLFLFacePtr fp;
    DLFLVertexPtr vp;
    DLFLFaceVertexPtr fvp1, fvp2;
    Vector3d oldpos, newpos;
    int num_old_faces, num_faces, num_old_vertice, num_vertice, num_faceVertice;

    num_old_vertice = obj->num_vertices();

    // mark the original vertices
    setOldVertexType(obj);
    // subdivide all edges by 2
    subdivideAllEdges(obj,2);
    // mark the newly created vertices by subdivision
    setNewSubVertexType(obj);

    // TODO: put new code here below
    num_old_faces = obj->num_faces();
	
    num_faces = 0; fl_first = obj->beginFace(); fl_last = obj->endFace();
    while(fl_first != fl_last && num_faces < num_old_faces) {
      fp = (*fl_first); ++fl_first; ++num_faces;
		
      fvp1 = fp->front();		
      if ( (fvp1->getVertexType() != VTNewSub) )
	fvp1 = fvp1->next();
      vp = fvp1->getVertexPtr();

      fvp2 = fvp1->prev()->prev();		
      while( fvp2->getVertexPtr() != vp ) {
	insertEdge(obj,fvp1, fvp2);
	fvp1 = fvp2;
	fvp2 = fvp1->prev()->prev();
      }
      insertEdge(obj,fvp1, fvp2);
    }

    // move the original vertices to a propriate position decided by length factor
    num_vertice = 0; vl_first = obj->beginVertex(); vl_last = obj->endVertex();
    while (vl_first != vl_last && num_vertice < num_old_vertice) {
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
    setNormalVertexType(obj);
  }

  void dooSabinSubdivideBCNew(DLFLObjectPtr obj,double sf, double length) { // bei dec 11
    DLFLFacePtrList::iterator fl_first, fl_last;
    DLFLEdgePtrList::iterator el_first, el_last;
    DLFLVertexPtrList::iterator vl_first, vl_last;
    DLFLFaceVertexPtrArray fvparray;
    DLFLFacePtr fp;
    DLFLEdgePtr ep, ep1, ep2;
    DLFLFaceVertexPtr fvp, fvp1, fvp2;
    DLFLVertexPtr vp;
    Vector3d oldpos, newpos;
    int num_old_faces, num_old_edges, num_old_vertice;
    int num_faces, num_edges, num_vertice, num_faceVertice;

    num_old_vertice = obj->num_vertices();

    // mark the original vertices
    setOldVertexType(obj);
    // subdivide all edges by 2
    subdivideAllEdges(obj);
    // mark the newly created vertices by subdivision
    setNewSubVertexType(obj);

    num_old_faces = obj->num_faces(); num_old_edges = obj->num_edges();

    // doo-sabin extrusion
    num_faces = 0; fl_first = obj->beginFace(); fl_last = obj->endFace();
    while ( fl_first != fl_last && num_faces < num_old_faces ) {
      fp = (*fl_first); ++fl_first; ++num_faces;
      extrudeFaceDS(obj,fp, 0.0, 1, 0.0, sf);		
    }

    // delete old edges
    num_edges = 0; el_first = obj->beginEdge(); el_last = obj->endEdge();
    while ( el_first != el_last && num_edges < num_old_edges ) {
      ep = (*el_first); ++el_first; ++num_edges;
      deleteEdge(obj,ep);
    }

    // delete old edges while insert an new edge between two newly extrude faces
    num_old_edges = obj->num_edges();
    num_edges = 0; el_first = obj->beginEdge(); el_last = obj->endEdge();
    while ( el_first != el_last && num_edges < num_old_edges ) {
      ep = (*el_first); ++el_first; ++num_edges;
      fvp1 = ep->getFaceVertexPtr1(); 
      fvp2 = ep->getFaceVertexPtr2();

      if ((fvp1->getVertexType() == VTNewSub)) {
	fvp = fvp1;
	fvp1 = fvp->prev(); ep1 = fvp1->getEdgePtr();
	fvp2 = fvp->next(); ep2 = fvp->getEdgePtr();
	insertEdge(obj,fvp1, fvp2);
	deleteEdge(obj,ep1); deleteEdge(obj,ep2);
      } else if ((fvp2->getVertexType() == VTNewSub)) {
	fvp = fvp2;
	fvp1 = fvp->prev();  ep1 = fvp1->getEdgePtr();
	fvp2 = fvp->next();  ep2 = fvp->getEdgePtr();
	insertEdge(obj, fvp1, fvp2);
	deleteEdge(obj, ep1); 
	deleteEdge(obj, ep2);
      }
    }

    // move the original vertices to a propriate position decided by length factor
    num_vertice = 0;
    vl_first = obj->beginVertex();
    vl_last = obj->endVertex();
    while (vl_first != vl_last && num_vertice < num_old_vertice) {
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
    setNormalVertexType(obj);
  }

  void dooSabinSubdivideBC(DLFLObjectPtr obj, bool check) {
    subdivideAllEdges(obj);
    dooSabinSubdivide(obj);
  }

  // Face Subdivision //

  void subdivideFace(DLFLObjectPtr obj, DLFLFacePtr faceptr, bool usequads) {
    // Subdivide a face into quads or triangles depending on flag
    // If quads are used, edges are subdivided, otherwise existing corners are used
    Vector3d geomcen, normalcen;
    Vector2d texcen;
    RGBColor colorcen;

    faceptr->getCentroids(geomcen,texcen,colorcen,normalcen);

    DLFLFaceVertexPtrArray endpoints;
    if ( usequads ) {
      // Subdivide all edges in this face and store the corresponding newly created corners
      // which will be used as the end points of the new edges
      DLFLVertexPtr vp = NULL;
      DLFLEdgePtrArray edges;
  
      faceptr->getEdges(edges);
      endpoints.resize(edges.size(),NULL);
      for (int i=0; i < edges.size(); ++i) {
	vp = subdivideEdge(obj,edges[i]);
	endpoints[i] = vp->getFaceVertexInFace(faceptr);
      }
    } else {
      // Use existing corners in face as end points of the new edges
      faceptr->getCorners(endpoints);
    }

    // Create point-sphere at centroid and set properties of face-vertex to
    // that of centroid of face.
    DLFLFaceVertexPtr cenfvp = obj->createPointSphere(geomcen,faceptr->material());
    cenfvp->normal = normalcen; cenfvp->color = colorcen; cenfvp->texcoord = texcen;

    // Insert edges between corner in centroid and corners in the endpoints array
    // NOTE: Make sure centroid corner is specified first in the insertEdge call
    for (int i=0; i < endpoints.size(); ++i) {
      insertEdge(obj,cenfvp,endpoints[i]);
    }
  }

  void subdivideFace(DLFLObjectPtr obj, uint face_index, bool usequads) {
    // Find the Face with the given face_index from the FaceList and sub-divide it
    if ( face_index > obj->num_faces() ) return;
  
    DLFLFacePtrList::iterator i = obj->beginFace();
    advance(i,face_index);
    subdivideFace(obj, *i,usequads);
  }


  void subdivideFaces(DLFLObjectPtr obj, DLFLFacePtrArray fparray, bool usequads) {
    // Subdivide the given list of faces into quads or triangles
    // For triangles, we can just go through the list and subdivide each of them
    // For quads, since edges are subdivided and new points created, we want
    // to make sure that edges shared between two faces in the list are not
    // subdivided twice.

    // If there is only 1 face do regular face subdivision and return
    if ( fparray.size() == 1 ) {
      subdivideFace(obj, fparray[0],usequads);
      return;
    }
    if ( usequads ) {
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
      for (int i=0; i < fparray.size(); ++i) {
	faceptr = fparray[i];

	// Subdivide all edges in this face, set type of new vertex when subdividing.
	// If an edge is a new edge, then we will not subdivide that edge
	// since it was created as a result of a previous edge subdivision
	faceptr->getEdges(edges);
	for (int i=0; i < edges.size(); ++i) {
	  if ( edges[i]->getType() != ETNew ) {
	    vp = subdivideEdge(obj,edges[i],true);
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
	cenfvp = obj->createPointSphere(geomcen,faceptr->material());
	cenfvp->normal = normalcen; cenfvp->color = colorcen; cenfvp->texcoord = texcen;

	// Insert edges between corner in centroid and the new corners in face
	// NOTE: Make sure centroid corner is specified first in the insertEdge call
	faceptr->getCorners(corners);
	for (int i=0; i < corners.size(); ++i) {
	  if ( corners[i]->getType() == FVTNew ) {
	    // Reset the type of the corner as well as the vertex
	    // Get both edges related to this corner and add them
	    // to the list for resetting type later.
	    corners[i]->resetType();
	    corners[i]->getVertexPtr()->resetType();
	    insertEdge(obj,cenfvp,corners[i]);
	  }
	}
      }

      // Reset type of edges which were created by subdivision
      DLFLEdgePtrList::iterator epfirst = eptypereset.begin(), eplast = eptypereset.end();
      while ( epfirst != eplast ) {
	(*epfirst)->resetType(); ++epfirst;
      }

      // Reset type of corners which were created by subdivision
      DLFLFaceVertexPtrList::iterator fvpfirst = fvptypereset.begin(), fvplast = fvptypereset.end();
      while ( fvpfirst != fvplast ) {
	(*fvpfirst)->resetType(); ++fvpfirst;
      }
    } else {
      // Go through list and subdivide each face
      for (int i=0; i < fparray.size(); ++i) {
	subdivideFace(obj, fparray[i],false);
      }
    }
  }

  void subdivideFaces(DLFLObjectPtr obj, DLFLFacePtrList fplist, bool usequads) {
    // NOTE: fplist parameter is to passed by value to ensure copy will be made
    // This is to allow subdivision of all faces in the mesh, in which case
    // the entire face list will be passed to this subroutine.

    // Subdivide the given list of faces into quads or triangles
    // For triangles, we can just go through the list and subdivide each of them
    // For quads, since edges are subdivided and new points created, we want
    // to make sure that edges shared between two faces in the list are not
    // subdivided twice.

    // If there is only 1 face do regular face subdivision and return
    if ( fplist.size() == 1 ) {
      subdivideFace(obj, fplist.front());
      return;
    }
    if ( usequads ) {
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
      while ( fpfirst != fplast ) {
	faceptr = (*fpfirst); ++fpfirst;

	// Subdivide all edges in this face, set type of new vertex when subdividing.
	// If an edge is a new edge, then we will not subdivide that edge
	// since it was created as a result of a previous edge subdivision
	faceptr->getEdges(edges);
	for (int i=0; i < edges.size(); ++i) {
	  if ( edges[i]->getType() != ETNew ) {
	    vp = subdivideEdge(obj,edges[i],true);
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
	cenfvp = obj->createPointSphere(geomcen,faceptr->material());
	cenfvp->normal = normalcen; cenfvp->color = colorcen; cenfvp->texcoord = texcen;

	// Insert edges between corner in centroid and the new corners in face
	// NOTE: Make sure centroid corner is specified first in the insertEdge call
	faceptr->getCorners(corners);
	for (int i=0; i < corners.size(); ++i) {
	  if ( corners[i]->getType() == FVTNew ) {
	    // Reset the type of the corner as well as the vertex
	    // Get both edges related to this corner and add them
	    // to the list for resetting type later.
	    corners[i]->resetType();
	    corners[i]->getVertexPtr()->resetType();
	    insertEdge(obj,cenfvp,corners[i]);
	  }
	}
      }

      // Reset type of edges which were created by subdivision
      DLFLEdgePtrList::iterator epfirst = eptypereset.begin(), eplast = eptypereset.end();
      while ( epfirst != eplast ) {
	(*epfirst)->resetType(); ++epfirst;
      }

      // Reset type of corners which were created by subdivision
      DLFLFaceVertexPtrList::iterator fvpfirst = fvptypereset.begin(), fvplast = fvptypereset.end();
      while ( fvpfirst != fvplast ) {
	(*fvpfirst)->resetType(); ++fvpfirst;
      }
    } else {
      // Go through list and subdivide each face
      DLFLFacePtrList::iterator fpfirst, fplast;
      fpfirst = fplist.begin(); fplast = fplist.end();
      while ( fpfirst != fplast ) {
	subdivideFace(obj, *fpfirst,false); ++fpfirst;
      }
    }
  }

  void subdivideAllFaces(DLFLObjectPtr obj, bool usequads) {
    // NOTE: A copy of the list needs to be made since subdivision
    // will add new faces. Parameter has to passed by value to ensure copy will be made
    subdivideFaces( obj, obj->getFaceList(),usequads );
  }

} // end namespace
