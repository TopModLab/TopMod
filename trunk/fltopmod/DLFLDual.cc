/* $Id: DLFLDual.cc,v 4.0 2003/12/26 01:58:52 vinod Exp $ */

#include "DLFLObject.hh"

void DLFLObject :: createDual(bool accurate)
{
     // Using "correct" methods seems to be VERY slow
     // We will use it only if we want accurate dual
     // Otherwise we will use old method
  if ( accurate )
     {
          // Dual operation using face subdivision into quads,
          // followed by deleting old edges and cleanup of winged vertices

          // Keep count of old number of vertices
          // All old edges have to be deleted. But since old edges will be subdivided
          // we have to access the edges through the old vertices
       int num_old_verts = vertex_list.size();

          // Do quad subdivision of all faces
       subDivideAllFaces(true);

          // Go through all the old vertices and delete all edges connected to them
       int vcount = 0;
       DLFLVertexPtrList :: iterator vpfirst, vplast;
       DLFLVertexPtr vp;
       DLFLEdgePtrArray eparray;
       vpfirst = vertex_list.begin(); vplast = vertex_list.end();
       while ( vcount < num_old_verts )
          {
            vp = (*vpfirst); ++vpfirst; ++vcount;
            vp->getEdges(eparray);
            for (int i=0; i < eparray.size(); ++i)
               deleteEdge(eparray[i],true);
          }

          // Cleanup valence-2 vertices
       cleanupWingedVertices();
     }
  else
     {
          // Go through list of faces and create centroids
          // Also make sure face ids are consecutive

       DLFLFacePtrList :: iterator fl_first, fl_last;
       DLFLFacePtr fp;

       Vector3d cen;
       StringStream rw;

          // Traverse all faces, find centroid and output to stream
          // Also call makeUnique to ensure face ids are consecutive
       fl_first = face_list.begin(); fl_last = face_list.end();
       while ( fl_first != fl_last )
          {
            fp = (*fl_first); ++fl_first;
            fp->makeUnique();
            cen = fp->geomCentroid();	   
            rw << "v " << cen[0] << " " << cen[1] << " " << cen[2] << endl;
          }

          // Get ID of first face to determine offset for index into vertex list
       int fistart = (face_list.front())->getID();

          // Now traverse for all vertices, get all faces attached to it
          // and create dual faces accordingly and output to filestream
       DLFLVertexPtrList :: iterator vl_first, vl_last;
       DLFLVertexPtr vp;
       DLFLFaceVertexPtrArray fvparray;

       vl_first = vertex_list.begin(); vl_last = vertex_list.end();
       while ( vl_first != vl_last )
          {
            vp = (*vl_first); ++vl_first;
               // Get the face vertices pointing to this vertex in an array
            vp->getFaceVertices(fvparray);
            if ( fvparray.size() > 0 )
               {
                    // Start from one face vertex (first) and traverse
                    // the face vertices pointing to this vertex according to the
                    // rotation order. This wont always be the same as the order
                    // in the list of face vertices in the vertex.
                 DLFLFaceVertexPtr startfvp = fvparray[0];
                 DLFLFaceVertexPtr curfvp = startfvp;

                 int index=0;
                 fvparray[index++] = curfvp;

                    // Use vnext and not next, since we want rotation around vertex
                    // Use the same array to store the face vertices in the correct
                    // rotation order, since the size will be the same
                 curfvp = curfvp->vnext();
                 while ( curfvp != startfvp)
                    {
                      fvparray[index++] = curfvp;
                      curfvp = curfvp->vnext();
                    }

                    // Create the dual face corresponding to this vertex by
                    // going through the array created above, finding the corresponding face
                    // and using the ID to index into the list of vertices output earlier.
                    // Note that traversal here is in reverse because rotation order around
                    // vertex is opposite to that of the faces
                 rw << "f";
                 while ( index > 0 )
                    {
                      rw << " ";
                      rw << fvparray[--index]->getFaceID()-fistart+1;
                    }
               }
            fvparray.clear();
            rw << endl;
          }
       rw << endl << endl;
       rw << flush;
       readObjectAlt(rw);
     }
}

/*
  $Log: DLFLDual.cc,v $
  Revision 4.0  2003/12/26 01:58:52  vinod
  Major version sync.

  Revision 3.2  2003/12/07 01:51:12  vinod
  Delayed comment: added an accurate method of dual creation using face subdivision and cleanup of winged corners

  Revision 3.1  2003/12/07 01:48:39  vinod
  Added flag to createDual to allow choice of methods

  Revision 3.0  2003/09/19 16:06:47  vinod
  Major version update

  Revision 2.3  2002/10/25 16:27:43  vinod
  Removed unused variables

  Revision 2.2  2002/10/25 15:16:27  vinod
  Changed to use StringStream typedef instead of strstream

  Revision 2.1  2002/10/22 21:28:08  vinod
  Source code for Dual object creation.
  Modified from code written by Zeke Melek

*/
