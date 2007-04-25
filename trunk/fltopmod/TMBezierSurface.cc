/* $Id$ */

#include "TMBezierSurface.hh"

void TMBezierSurface :: createSurface(void)
{
     // Clear out existing patches
  patcharray.clear();

     // Create bezier patches for the object
  if ( object == NULL ) return;
  if ( object->num_vertices() == 0 || object->num_faces() == 0 || object->num_edges() == 0 ) return;

     // Make IDs consecutive for vertices, edges and faces in the object
  object->makeUnique();

     // Get the edges, faces and vertices from the object into local arrays
  DLFLVertexPtrArray vparray;
  DLFLFacePtrArray fparray;
  DLFLEdgePtrArray eparray;

  object->getVertices(vparray);
  object->getFaces(fparray);
  object->getEdges(eparray);

     // Store the base IDs for correct indexing into array
  int vbid = vparray[0]->getID();
  int fbid = fparray[0]->getID();
  int ebid = eparray[0]->getID();

     // Create the arrays of control points
  TMVertexControlPointArray vcparray;
  TMFaceControlPointArray fcparray;
  TMEdgeControlPointArray ecparray;

     // Resize the arrays in advance
  vcparray.resize(vparray.size());
  fcparray.resize(fparray.size());
  ecparray.resize(eparray.size());

     // Set up links between the control points and the corresponding entities in the object
  for (int i=0; i < vcparray.size(); ++i)
     vcparray[i].setVertex(vparray[i]);

  for (int i=0; i < fcparray.size(); ++i)
     fcparray[i].setFace(fparray[i]);

  for (int i=0; i < ecparray.size(); ++i)
     ecparray[i].setEdge(eparray[i]);
  
     // Create the patches
  
     // Go through every face and create patches for each face
  for (int i=0; i < fparray.size(); ++i)
     {
       DLFLFacePtr fp = fparray[i];

          // Create a patch for every corner of the face
       DLFLFaceVertexPtr fvhead, fvcurrent;
       DLFLVertexPtr vertex;
       DLFLEdgePtr edge;
       fvcurrent = fvhead = fp->front();
       do
          {
               // Create a patch for this corner
            TMBezierPatch patch(type);

               // Determine the 4 control points connected to this corner
            TMVertexControlPoint tmvcp;
            TMFaceControlPoint tmfcp;
            TMEdgeControlPoint tmecp1, tmecp2;

            tmvcp = vcparray[fvcurrent->vertex->getID() - vbid];
            tmfcp = fcparray[i];
            tmecp1 = ecparray[fvcurrent->getEdgePtr()->getID() - ebid];
            tmecp2 = ecparray[fvcurrent->prev()->getEdgePtr()->getID() - ebid];

               // Store the control points in the patch
            patch.setControlPoints(tmvcp,tmfcp,tmecp1,tmecp2);

               // Update positions of the points in the patch
            patch.updatePositions();

               // Add the patch to the array of patches for this surface
            patcharray.push_back(patch);

               // Go to the next corner in this face
            fvcurrent = fvcurrent->next();
          }
       while ( fvcurrent != fvhead );
     }
}

/*
  $Log$
*/
