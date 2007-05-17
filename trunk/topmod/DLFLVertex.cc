/* $Id: DLFLVertex.cc,v 4.1 2004/01/13 01:57:33 vinod Exp $ */

// Non-inline function definitions for DLFLVertex class

#include "DLFLVertex.hh"
#include "DLFLFace.hh"
#include "DLFLEdge.hh"

uint DLFLVertex :: suLastID = 0;

// Dump contents of this object to an output stream
void DLFLVertex :: dump(ostream& o) const
{
  o << "DLFLVertex" << endl
//    << "  ID : " << uID << endl
//    << "  Coords : " << coords << endl
//    << "  Type : " << vtType << endl
    << "  fvpList" << endl;

  DLFLFaceVertexPtrList :: const_iterator first, last;
  int i=0;
  
  first = fvpList.begin(); last = fvpList.end();
  while ( first != last )
     {
       o << "    " << i++ << " : " << (*first) << endl;
       ++first;
     }
  o << endl;
}

   // Reset type of vertex, face-vertices and edges associated with this vertex
void DLFLVertex :: resetTypeDeep(void)
{
     // Reset vertex type
  resetType();

     // Go through face-vertex list and reset type of each face-vertex
     // and the edge starting at that face-vertex
  DLFLFaceVertexPtrList :: iterator first, last;
  DLFLFaceVertexPtr fvp;
  first = fvpList.begin(); last = fvpList.end();
  while ( first != last )
     {
       fvp = (*first); ++first;
       fvp->resetType();
       fvp->getEdgePtr()->resetType();
     }
}

   // Set the texture coordinates for all FaceVertexes referring to this vertex
void DLFLVertex :: setTexCoords(const Vector2d& texcoord)
{
  DLFLFaceVertexPtrList :: iterator first, last;

  first = fvpList.begin(); last = fvpList.end();
  while ( first != last )
     {
       (*first)->texcoord = texcoord;
       ++first;
     }
}

   // Set the color values for all FaceVertexes referring to this vertex
void DLFLVertex :: setColor(const RGBColor& color)
{
  DLFLFaceVertexPtrList :: iterator first, last;

  first = fvpList.begin(); last = fvpList.end();
  while ( first != last )
     {
       (*first)->color = color;
       ++first;
     }
}

   // Compute the normals for all FaceVertexes referring to this vertex
   // Update the vertex normal and return it
Vector3d DLFLVertex :: updateNormal(bool recompute)
{
  DLFLFaceVertexPtrList :: iterator first, last;

  first = fvpList.begin(); last = fvpList.end();
  normal.reset();
  if ( recompute )
     while ( first != last )
        {
          normal += (*first)->computeNormal();
          ++first;
        }
  else
     while ( first != last )
        {
          normal += (*first)->normal;
          ++first;
        }
     
  normalize(normal);
  return normal;
}

Vector3d DLFLVertex :: getNormals(Vector3dArray& normals)
{
     // Return normals at all corners in an Vector3dArray
  Vector3d avenormal;
  int numnormals = fvpList.size();
  if ( numnormals > 0 )
     {
       normals.clear(); normals.reserve(numnormals);

       DLFLFaceVertexPtrList :: const_iterator first, last;
       DLFLFaceVertexPtr fvp = NULL;
       first = fvpList.begin(); last = fvpList.end();
       while ( first != last )
          {
            fvp = (*first); ++first;
            normals.push_back(fvp->normal); avenormal += fvp->normal;
          }
       avenormal /= numnormals;
     }
  return avenormal;
}

   // Set tex coordinates, color and normal info for all FaceVertexes referring to this vertex
void DLFLVertex :: setFaceVertexProps(const Vector2d& texcoord, const RGBColor& color, const Vector3d& n)
{
  DLFLFaceVertexPtrList :: iterator first, last;

  first = fvpList.begin(); last = fvpList.end();
  while ( first != last )
     {
       (*first)->texcoord = texcoord;
       (*first)->color = color;
       (*first)->normal = n;
       ++first;
     }
}

void DLFLVertex :: vertexTrace(void) const
{
     // Output all edges incident on this DLFLVertex in the specific rotation order

     // Pick a DLFLFaceVertex from the list - first one
  DLFLFaceVertexPtr fvptr1 = fvpList.front();

  if ( fvptr1 == NULL ) return;

  DLFLEdgePtr e1,e2;
  DLFLFaceVertexPtr fvptr2;
  DLFLFacePtr f;

     // Find Edge starting at this DLFLFaceVertex
  e1 = fvptr1->getEdgePtr();
  e2 = e1;

  do
     {
       cout << (*e2) << " ";

          // Find the other DLFLFaceVertex for this DLFLEdge
       fvptr2 = e2->getOtherFaceVertexPtr(fvptr1);
       if ( fvptr2 == NULL ) return;

          // Find the Face to which the second DLFLFaceVertex belongs
       f = fvptr2->getFacePtr();

          // Find the DLFLFaceVertex following the second DLFLFaceVertex in this Face
       fvptr1 = f->nextFaceVertex(fvptr2);

          // Find Edge starting at this DLFLFaceVertex
       e2 = fvptr1->getEdgePtr();
     }
  while ( e2 != e1 );

  cout << endl;
}

int DLFLVertex :: getEdges(DLFLEdgePtr ** edges) const
{
     // Create an array of DLFLEdge's incident on this DLFLVertex. The number of DLFLEdges
     // is returned. Memory will be allocated inside this function, which should
     // be freed by the caller. Pass a pointer to the array (DLFLEdge ** or DLFLEdgePtr *)
     // Equivalent to a vertex trace
     // In case of an error -1 is returned, but memory is not freed

  int num_edges = fvpList.size();
  int i = 0;

  if ( num_edges <= 0 ) return -1;

     // Allocate memory for the DLFLEdgePtr array
  *edges = new DLFLEdgePtr[num_edges];
  
  DLFLFaceVertexPtrList :: const_iterator first, last;
  DLFLFaceVertexPtr fvp = NULL;
  first = fvpList.begin(); last = fvpList.end();
  while ( first != last )
     {
       fvp = (*first); ++first;
       edges[0][i++] = fvp->getEdgePtr();
     }
  return num_edges;
}

void DLFLVertex :: getEdges(DLFLEdgePtrArray& edges) const
{
     // Return edges in an DLFLEdgePtrArray

  edges.clear(); edges.reserve(fvpList.size());

  DLFLFaceVertexPtrList :: const_iterator first, last;
  DLFLFaceVertexPtr fvp = NULL;
  first = fvpList.begin(); last = fvpList.end();
  while ( first != last )
     {
       fvp = (*first); ++first;
       edges.push_back(fvp->getEdgePtr());
     }
}

DLFLEdgePtr DLFLVertex :: getEdgeTo(DLFLVertexPtr vp)
{
     // Get the Edge incident on this Vertex which connects to given Vertex
     // If no such edge exists, returns NULL
  DLFLEdgePtr ep, retep = NULL;
  DLFLFaceVertexPtrList :: iterator first, last;
  DLFLFaceVertexPtr fvp = NULL, ofvp;
  first = fvpList.begin(); last = fvpList.end();
  while ( first != last )
     {
       fvp = (*first); ++first;
       ep = fvp->getEdgePtr();
       if ( ep != NULL )
          {
            ofvp = ep->getOtherFaceVertexPtr(fvp);
            if ( ofvp && ofvp->vertex == vp )
               {
                 retep = ep; break;
               }
          }
     }
  return retep;
}
  
void DLFLVertex :: getFaceVertices(DLFLFaceVertexPtrArray& fvparray)
{
     // Go through the face-vertex-pointer list and add each
     // face vertex pointer to the array
  fvparray.clear(); fvparray.reserve(fvpList.size());
  DLFLFaceVertexPtrList :: iterator first, last;
  DLFLFaceVertexPtr fvp = NULL;
  first = fvpList.begin(); last = fvpList.end();
  while ( first != last )
     {
       fvp = (*first); ++first;
       fvparray.push_back(fvp);
     }
}

void DLFLVertex :: getOrderedFaceVertices(DLFLFaceVertexPtrArray& fvparray)
{
     // Get the face vertices associated with this vertex in the clockwise rotation order
  fvparray.clear();

  DLFLFaceVertexPtr fvpstart = fvpList.front();
  if ( fvpstart == NULL ) return;

  fvparray.reserve(fvpList.size());
  DLFLFaceVertexPtr fvp = fvpstart;

  do
     {
       fvparray.push_back(fvp);
       fvp = fvp->vnext();
     }
  while ( fvp != fvpstart );
}


void DLFLVertex :: getCornerAuxCoords(Vector3dArray& coords) const
{
  coords.clear(); coords.reserve(fvpList.size());
  DLFLFaceVertexPtrList :: const_iterator first, last;
  DLFLFaceVertexPtr fvp = NULL;
  first = fvpList.begin(); last = fvpList.end();
  while ( first != last )
     {
       fvp = (*first); ++first;
       coords.push_back(fvp->getAuxCoords());
     }
}

void DLFLVertex :: getOrderedCornerAuxCoords(Vector3dArray& coords) const
{
     // Get the aux coords of face vertices associated with this vertex in the clockwise rotation order
  coords.clear();

  DLFLFaceVertexPtr fvpstart = fvpList.front();
  if ( fvpstart == NULL ) return;

  coords.reserve(fvpList.size());
  DLFLFaceVertexPtr fvp = fvpstart;

  do
     {
       coords.push_back(fvp->getAuxCoords());
       fvp = fvp->vnext();
     }
  while ( fvp != fvpstart );
}


void DLFLVertex :: getFaces(DLFLFacePtrArray& fparray)
{
     // Go through the face-vertex-pointer list and add
     // face pointer of each face vertex pointer to the array
  fparray.clear(); fparray.reserve(fvpList.size());
  DLFLFaceVertexPtrList :: iterator first, last;
  DLFLFaceVertexPtr fvp = NULL;
  first = fvpList.begin(); last = fvpList.end();
  while ( first != last )
     {
       fvp = (*first); ++first;
       fparray.push_back(fvp->getFacePtr());
     }
}

DLFLFaceVertexPtr DLFLVertex :: getFaceVertexInFace(DLFLFacePtr fp)
{
     // Get the FaceVertex belonging to the given face. If only 1 face-vertex
     // is there in the list, return that. If more than 1 exist but nothing
     // belongs to given face, return NULL
  DLFLFaceVertexPtrList :: iterator first, last;
  DLFLFaceVertexPtr fvp, retfvp = NULL;
  first = fvpList.begin(); last = fvpList.end();
  if ( fvpList.size() == 1 ) retfvp = (*first);
  else
     {
       while ( first != last )
          {
            fvp = (*first); ++first;
            if ( fvp->getFacePtr() == fp )
               {
                 retfvp = fvp;
                 break;
               }
          }
     }
  return retfvp;
}

DLFLFaceVertexPtr DLFLVertex :: getFaceVertexWithPrev(DLFLVertexPtr vp)
{
     // Get the FaceVertex which has the given Vertex before it in it's Face
     // If only 1 FaceVertex refers to this Vertex, will return that
     // If there are more than 1 and none of them satisfies the condition, returns NULL
  DLFLFaceVertexPtrList :: iterator first, last;
  DLFLFaceVertexPtr fvp, retfvp = NULL;
  first = fvpList.begin(); last = fvpList.end();
  if ( fvpList.size() == 1 ) retfvp = (*first);
  else
     {
       while ( first != last )
          {
            fvp = (*first); ++first;
            if ( fvp->prev()->vertex == vp )
               {
                 retfvp = fvp;
                 break;
               }
          }
     }
  return retfvp;
}

DLFLFaceVertexPtr DLFLVertex :: getFaceVertexWithNext(DLFLVertexPtr vp)
{
     // Get the FaceVertex which has the given Vertex after it in it's Face
     // If only 1 FaceVertex refers to this Vertex, will return that
     // If there are more than 1 and none of them satisfies the condition, returns NULL
  DLFLFaceVertexPtrList :: iterator first, last;
  DLFLFaceVertexPtr fvp, retfvp = NULL;
  first = fvpList.begin(); last = fvpList.end();
  if ( fvpList.size() == 1 ) retfvp = (*first);
  else
     {
       while ( first != last )
          {
            fvp = (*first); ++first;
            if ( fvp->next()->vertex == vp )
               {
                 retfvp = fvp;
                 break;
               }
          }
     }
  return retfvp;
}

DLFLFaceVertexPtr DLFLVertex :: getBackFaceVertex(void)
{
     // Get the FaceVertex which has the 'backface' flag set
     // If no such FaceVertex is found, returns NULL
  DLFLFaceVertexPtrList :: iterator first, last;
  DLFLFaceVertexPtr fvp, retfvp = NULL;
  first = fvpList.begin(); last = fvpList.end();
  while ( first != last )
     {
       fvp = (*first); ++first;
       if ( fvp->backface == true )
          {
            retfvp = fvp; break;
          }
     }
  return retfvp;
}


/*
  $Log: DLFLVertex.cc,v $
  Revision 4.1  2004/01/13 01:57:33  vinod
  Added getFaces method

  Revision 4.0  2003/12/26 01:58:53  vinod
  Major version sync.

  Revision 3.1  2003/10/10 06:40:47  vinod
  Added getNormals method

  Revision 3.0  2003/09/19 16:06:47  vinod
  Major version update

  Revision 2.6  2003/09/19 16:04:24  vinod
  Added resetTypeDeep method

  Revision 2.5  2002/12/30 17:29:02  vinod
  Added STL array version of getEdges method. Rewrote earlier getEdges

  Revision 2.4  2002/04/28 22:14:23  vinod
  Added getEdgeTo, getFaceVertexWithPrev/Next and getBackFaceVertex

  Revision 2.3  2001/11/21 00:17:38  vinod
  Added set flag to computeNormal, added averageNormal method

  Revision 2.2  2001/10/06 16:40:58  vinod
  Added getFaceVertexInFace

  Revision 2.1  2001/09/28 17:45:14  vinod
  Added getFaceVertices method

  Revision 2.0  2001/07/25 05:12:46  vinod
  Major version sync

  Revision 1.3  2000/11/28 19:15:29  vinod
  Added computeNormal

  Revision 1.2  2000/11/21 19:19:15  vinod
  Added dump() method

  Revision 1.1.1.1  2000/11/02 19:32:42  vinod
  New DLFL Sources

*/
