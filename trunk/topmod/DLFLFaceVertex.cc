/* $Id: DLFLFaceVertex.cc,v 4.0 2003/12/26 01:58:52 vinod Exp $ */

// Non-inline function definitions for class DLFLFaceVertex

#include "DLFLFaceVertex.hh"
#include "DLFLFace.hh"
#include "DLFLEdge.hh"
#include "DLFLVertex.hh"
#include "DLFLAux.hh"
#include "DLFLRenderer.hh"

// Dump contents of this object
void DLFLFaceVertex :: dump(ostream& o) const
{
  o << "     DLFLFaceVertex" << endl
    << "       VertexPtr : " << vertex << endl
//    << "       Normal : " << normal << endl
//    << "       Color : " << color << endl
//    << "       TexCoord : " << texcoord << endl
    << "       EdgePtr : " << epEPtr << endl
    << "       FacePtr : " << fpFPtr << endl
//    << "       next : " << fvpNext << endl
//    << "       prev : " << fvpPrev << endl
//    << "       Type : " << fvtType << endl
    << endl;
}

void average(const DLFLFaceVertex& dfv1, const DLFLFaceVertex& dfv2, DLFLFaceVertex& ave)
{
     // Average 2 face vertexes. Coordinates, normals, texture coordinates and color are averaged
     // Assumes that all 3 face vertexes have valid vertex pointers
  ave.vertex->coords = ((dfv1.vertex->coords)+(dfv2.vertex->coords))/2.0;
  ave.normal = (dfv1.normal + dfv2.normal)/2.0;
  ave.color = (dfv1.color + dfv2.color)/2.0;
  ave.texcoord = (dfv1.texcoord + dfv2.texcoord)/2.0;
}

void average(DLFLFaceVertexPtr dfvp1, DLFLFaceVertexPtr dfvp2, DLFLFaceVertexPtr avep)
{
     // Average 2 face vertexes. Coordinates, normals, texture coordinates and color are averaged
     // Assumes that all 3 face vertexes have valid vertex pointers
  avep->vertex->coords = ((dfvp1->vertex->coords)+(dfvp2->vertex->coords))/2.0;
  avep->normal = (dfvp1->normal + dfvp2->normal)/2.0;
  avep->color = (dfvp1->color + dfvp2->color)/2.0;
  avep->texcoord = (dfvp1->texcoord + dfvp2->texcoord)/2.0;
}

DLFLVertexType DLFLFaceVertex :: getVertexType(void) const
{
  return vertex->getType();
}

DLFLFaceVertexPtr DLFLFaceVertex :: getOppositeCorner(void)
{
  return epEPtr->getOtherFaceVertexPtr(this);
}

uint DLFLFaceVertex :: getVertexID(void) const
{
  return vertex->getID();
}

uint DLFLFaceVertex :: getFaceID(void) const
{
  return fpFPtr->getID();
}

uint DLFLFaceVertex :: getEdgeID(void) const
{
  return epEPtr->getID();
}

Vector3d DLFLFaceVertex :: getVertexCoords(void) const
{
  return vertex->coords;
}

bool coFacial(DLFLFaceVertexPtr fvptr1, DLFLFaceVertexPtr fvptr2) // friend function
{
  return ( fvptr1->getFacePtr() == fvptr2->getFacePtr() );
}

bool coFacial(const DLFLFaceVertex& fv1, const DLFLFaceVertex& fv2) // friend function
{
  return ( fv1.getFacePtr() == fv2.getFacePtr() );
}

bool areEqual(DLFLFaceVertexPtr fvptr1, DLFLFaceVertexPtr fvptr2) // friend function
{
     // Test to see if 2 DLFLFaceVertexes are "equal"
     // They are "equal" iff they belong to the same Face AND they
     // refer to the same vertex

     // If either pointer is NULL returns false unless both are NULL in which
     // case it returns true
  if (fvptr1 == NULL)
     {
       if ( fvptr2 == NULL ) return true;
       return false;
     }
  else if ( fvptr2 == NULL ) return false;
  else
     {
          // Both pointers are not NULL
       if ( coFacial(fvptr1,fvptr2) &&
            ( fvptr1->getVertexID() == fvptr2->getVertexID() ) )
          return true;
     }
  return false;
}

bool areEqual(const DLFLFaceVertex& fv1, const DLFLFaceVertex& fv2) // friend function
{
     // Test to see if 2 DLFLFaceVertexes are "equal"
     // They are "equal" iff they belong to the same Face AND they
     // refer to the same Position

  if ( coFacial(fv1,fv2) &&
       ( fv1.getVertexID() == fv2.getVertexID() ) )
          return true;
  return false;
}

// Check if given corner is a concave corner or not.
// Will not work properly for non-planar faces.
// Will not work properly for faces which have edges which belong entirely to that face.
// Will not work properly for highly skewed geometry.
bool DLFLFaceVertex :: isConcaveCorner(void) const
{
     // If face is a triangle or a 2-gon or a point-sphere return false
  if ( fpFPtr->size() <= 3 ) return false;

     // Compute the normalized edge vectors starting from the specified corner
  Vector3d ppos = fvpPrev->getVertexCoords();
  Vector3d npos = fvpNext->getVertexCoords();
  Vector3d pos = getVertexCoords();
  Vector3d pvec = ppos-pos; normalize(pvec);
  Vector3d nvec = npos-pos; normalize(nvec);

     // If angle between the 2 edge vectors is 0 degrees or 180 degrees
     // the corner is assumed to be a convex corner
  double sinangle = norm(pvec % nvec);
  if ( sinangle < 1.0e-5 ) return false;

     // Compute a new point by adding the 2 edge vectors, scaled to a very small length.
     // The choice of the length of the vector is arbitrary and must be chosen to avoid
     // miscalculations because of rounding errors in floating point computations.
     // For a concave corner this point will end up lying outside the polygon
     // For a convex corner this point will end up lying inside the polygon
  Vector3d p = pos + (pvec + nvec) * 0.01;

     // Normal at this corner - for computing projection plane
     // not corrected for concavity
  Vector3d fvn = nvec % pvec; // pvec and nvec are already normalized

     // Check if p is inside the face
  Vector3dArray facepoints;
  fpFPtr->getVertexCoords(facepoints);

  if ( pointInPolygon(p,facepoints,fvn) ) return false;

  return true;
}

   // Check if this corner is a winged corner or not
   // A winged corner is a degenerate corner
   // i.e. the angle at the corner is 180 degrees
bool DLFLFaceVertex :: isWingedCorner(void) const
{
     // If face is a triangle or a 2-gon or a point-sphere return false
  if ( fpFPtr->size() <= 3 ) return false;

     // Compute the normalized edge vectors starting from the specified corner
  Vector3d ppos = fvpPrev->getVertexCoords();
  Vector3d npos = fvpNext->getVertexCoords();
  Vector3d pos = getVertexCoords();
  Vector3d pvec = ppos-pos; normalize(pvec);
  Vector3d nvec = npos-pos; normalize(nvec);

     // If angle between the 2 edge vectors is 0 degrees or 180 degrees
     // the corner is a winged corner
     // We can also check this by adding the two vectors and checking if
     // they sum to 0
  if ( norm(pvec+nvec) < 1.0e-5 ) return true;
  return false;
}

   // Find the next non-winged corner starting from this corner
   // If none can be found, returns NULL
DLFLFaceVertexPtr DLFLFaceVertex :: nextNonWingedCorner(void) const
{
  DLFLFaceVertexPtr fvp = next();
  while ( fvp->isWingedCorner() && fvp != this ) fvp = fvp->next();
  if ( fvp == this ) return NULL;
  return fvp;
}

   // Find the CLOSEST non-winged corner starting from this corner
   // If none can be found, returns NULL
   // Search both forward and backward till we find one.
DLFLFaceVertexPtr DLFLFaceVertex :: closestNonWingedCorner(void) const
{
  DLFLFaceVertexPtr fvpn = next(), fvpp = prev(), fvp = NULL;
  while ( fvpn->isWingedCorner() &&
          fvpp->isWingedCorner() &&
          fvpp != fvpn )
     {
       fvpn = fvpn->next(); fvpp = fvpp->prev();
     }
  if ( fvpp == fvpn )
     {
       if ( ! fvpp->isWingedCorner() ) fvp = fvpp;
       else if ( ! fvpn->isWingedCorner() ) fvp = fvpn;
          // WARNING! If the code reaches this point, this function will return NULL
          // This will cause other parts of the program to fail
     }
  else if ( ! fvpp->isWingedCorner() ) fvp = fvpp;
  else fvp = fvpn;
  return fvp;
}

  
   // Get a vector along the edge starting at this corner
Vector3d DLFLFaceVertex :: getEdgeVector(void) const
{
  Vector3d pos = getVertexCoords();
  Vector3d npos = fvpNext->getVertexCoords();
  Vector3d evec = npos-pos;
  return evec;
}

   // Get 2 vectors along the 2 edges which form this corner
   // Both vectors will originate at this corner
void DLFLFaceVertex :: getEdgeVectors(Vector3d& pvec, Vector3d& nvec)
{
  nvec = getEdgeVector();
  pvec = - (fvpPrev->getEdgeVector()); // Reverse direction of edgevector from previous corner
}

void DLFLFaceVertex :: updateNormal(void)
{
     // If this is a winged corner, assign normal of nearest non-winged corner
     // Otherwise compute for this corner and adjust for concave corners
  if ( isWingedCorner() )
     {
       DLFLFaceVertexPtr fvp = closestNonWingedCorner();
       normal = fvp->computeNormal();
     }
  else
     {
          // compute the normal using adjacent vertices
       Vector3d pvec, nvec;

       getEdgeVectors(pvec,nvec); // Don't have to be normalized
       normal = nvec % pvec;
       normalize(normal);

          // If this corner is a concave corner w.r.t. the face it is in,
          // then reverse the normal.
       if ( isConcaveCorner() ) normal = -normal;
     }
}

void DLFLFaceVertex :: setVertexCoords(const Vector3d& vec)
{
  if ( vertex ) vertex->coords = vec;
}

void DLFLFaceVertex :: setVertexCoords(double x, double y, double z)
{
  if ( vertex ) vertex->coords.set(x,y,z);
}

void DLFLFaceVertex :: addSelfToVertex(void)
{
  if ( vertex ) vertex->addToFaceVertexList(this);
}

void DLFLFaceVertex :: deleteSelfFromVertex(void)
{
  if ( vertex ) vertex->deleteFromFaceVertexList(this);
}

void DLFLFaceVertex :: addSelfToEdge(void)
{
     // Add self to Edge if there is a NULL DLFLFaceVertex pointer in the Edge
  if ( epEPtr ) epEPtr->setNullFaceVertexPtr(this);
}

void DLFLFaceVertex :: deleteSelfFromEdge(void)
{
     // Set the DLFLFaceVertexPtr of Edge which refers to this DLFLFaceVertex to NULL
  if ( epEPtr ) epEPtr->resetFaceVertexPtr(this);
}

void DLFLFaceVertex :: deleteSelfFromFace(void)
{
     // Remove the DLFLFaceVertexPtr from the face to which it belongs.
     // Simply calls the deleteVertexPtr method in the face
     // This can't be properly implemented here since the head pointer of the DLFLFace might
     // need to be adjusted too
  fpFPtr->deleteFaceVertexPtr(this);
}

DLFLFaceVertexPtr DLFLFaceVertex :: vnext(void)
{
  DLFLFaceVertexPtr vn = this;
  if ( epEPtr )
     {
       vn = epEPtr->getOtherFaceVertexPtr(vn);
       vn = vn->next();
     }
  return vn;
}

DLFLFaceVertexPtr DLFLFaceVertex :: vprev(void)
{
  DLFLFaceVertexPtr vn = this;
  if ( epEPtr )
     {
       vn = vn->prev();
       vn = vn->epEPtr->getOtherFaceVertexPtr(vn);
     }
  return vn;
}

void DLFLFaceVertex :: print(bool printadjacent) const
{
  cout << "DLFLFaceVertex : "
       << "Vertex (" << vertex->getID() << ") "
       << "Face (" << fpFPtr->getID() << ") "
       << "Edge (" << epEPtr->getID() << ") " << endl;
  if ( printadjacent )
     {
       cout << "Previous "; fvpPrev->print();
       cout << "Next "; fvpNext->print();
     }
}

void DLFLFaceVertex :: writeDLFL(ostream& o, uint newindex)
{
  o << "fv " << vertex->getIndex() << ' '
    << normal << ' '
    << texcoord << ' ' << endl;
  index = newindex;
}

uint DLFLFaceVertex :: readDLFL(istream& i)
{
     // Assume that the "fv" characters have already been read
     // Read the vertex index and return it, so the caller can set the vertex pointer
     // using the index to reference the vertex list
  uint vindex;
  i >> vindex >> normal >> texcoord;
  return vindex;
}

void DLFLFaceVertex :: render(void) const
{
  if ( vertex ) vertex->render();
}

   // Do a glVertex on this vertex, using the normal and with color
void glVertexCN(const DLFLFaceVertex& dfv)
{
  if ( dfv.vertex )
     {
       double x,y,z; dfv.normal.get(x,y,z);
       glNormal3f(x,y,z);
       glColor(dfv.color);
       glVertex(*(dfv.vertex));
     }
}
     
   // Do a glVertex on this vertex, using the normal and without color
void glVertexN(const DLFLFaceVertex& dfv)
{
  if ( dfv.vertex )
     {
       double x,y,z; dfv.normal.get(x,y,z);
       glNormal3f(x,y,z);
       glVertex(*(dfv.vertex));
     }
}
     
   // Do a glVertex on this vertex, without using the normal and with color
void glVertexC(const DLFLFaceVertex& dfv)
{
  if ( dfv.vertex )
     {
       glColor(dfv.color);
       glVertex(*(dfv.vertex));
     }
}
     
   // Do a glVertex on this face vertex, without using the normal and without color
   // This function is not named glVertex to allow using it in the
   // STL for_each algorithm, since glVertex is overloaded for DLFLVertex also
void glVertexOFV(const DLFLFaceVertex& dfv)
{
  if ( dfv.vertex ) glVertex(*(dfv.vertex));
}


   //-- Same as above functions but with texture coordinates also --//

void glVertexCNT(const DLFLFaceVertex& dfv)
{
  if ( dfv.vertex )
     {
       double x,y,z; dfv.normal.get(x,y,z);
       glNormal3f(x,y,z);
       glColor(dfv.color);
       if ( DLFLRenderer::isReversed() )
          glTexCoord2d(1.0-dfv.texcoord[0],1.0-dfv.texcoord[1]);
       else
          glTexCoord2d(1.0-dfv.texcoord[0],1.0-dfv.texcoord[1]);
       glVertex(*(dfv.vertex));
     }
}
     
void glVertexNT(const DLFLFaceVertex& dfv)
{
  if ( dfv.vertex )
     {
       double x,y,z; dfv.normal.get(x,y,z);
       glNormal3f(x,y,z);
       if ( DLFLRenderer::isReversed() )
          glTexCoord2d(1.0-dfv.texcoord[0],1.0-dfv.texcoord[1]);
       else
          glTexCoord2d(dfv.texcoord[0],dfv.texcoord[1]);
       glVertex(*(dfv.vertex));
     }
}
     
void glVertexCT(const DLFLFaceVertex& dfv)
{
  if ( dfv.vertex )
     {
       glColor(dfv.color);
       if ( DLFLRenderer::isReversed() )
          glTexCoord2d(1.0-dfv.texcoord[0],1.0-dfv.texcoord[1]);
       else
          glTexCoord2d(dfv.texcoord[0],dfv.texcoord[1]);
       glVertex(*(dfv.vertex));
     }
}

void glVertexOFVT(const DLFLFaceVertex& dfv)
{
  if ( dfv.vertex )
     {
       if ( DLFLRenderer::isReversed() )
          glTexCoord2d(1.0-dfv.texcoord[0],1.0-dfv.texcoord[1]);
       else
          glTexCoord2d(dfv.texcoord[0],dfv.texcoord[1]);
       glVertex(*(dfv.vertex));
     }
}


void glVertexCNp(DLFLFaceVertexPtr dfvp)
{
  glVertexCN(*dfvp);
}

void glVertexNp(DLFLFaceVertexPtr dfvp)
{
  glVertexN(*dfvp);
}

void glVertexCp(DLFLFaceVertexPtr dfvp)
{
  glVertexC(*dfvp);
}

void glVertexOFVp(DLFLFaceVertexPtr dfvp)
{
  glVertexOFV(*dfvp);
}

void glVertexCNTp(DLFLFaceVertexPtr dfvp)
{
  glVertexCNT(*dfvp);
}

void glVertexNTp(DLFLFaceVertexPtr dfvp)
{
  glVertexNT(*dfvp);
}

void glVertexCTp(DLFLFaceVertexPtr dfvp)
{
  glVertexCT(*dfvp);
}

void glVertexOFVTp(DLFLFaceVertexPtr dfvp)
{
  glVertexOFVT(*dfvp);
}

/*
  $Log: DLFLFaceVertex.cc,v $
  Revision 4.0  2003/12/26 01:58:52  vinod
  Major version sync.

  Revision 3.4  2003/12/06 08:03:54  vinod
  Added closestNonWingedCorner method. computeNormal now checks for winged corners

  Revision 3.3  2003/11/21 15:25:45  vinod
  Added isWingedCorner and nextNonWingedCorner methods

  Revision 3.2  2003/11/16 06:48:19  vinod
  Added getEdgeVector and getEdgeVectors methods.
  Modified computeNormal to use the getEdgeVectors method

  Revision 3.1  2003/11/14 02:40:59  vinod
  Added isConcaveCorner method. computeNormal now uses this method to correct for concave corners

  Revision 3.0  2003/09/19 16:06:47  vinod
  Major version update

  Revision 2.5  2003/03/10 07:33:52  vinod
  Added deleteSelfFromFace for consistency

  Revision 2.4  2002/12/27 07:17:21  vinod
  Changed computeNormal to fix direction of normal if necessary for valence 2 vertices

  Revision 2.3  2001/11/08 06:50:19  vinod
  Remove color data from DLFL format

  Revision 2.2  2001/09/28 05:50:03  vinod
  Added print method

  Revision 2.1  2001/08/07 07:16:31  vinod
  Added vnext and vprev methods

  Revision 2.0  2001/07/25 05:12:43  vinod
  Major version sync

  Revision 1.6  2001/07/18 07:24:54  vinod
  Added NULL pointer checks

  Revision 1.5  2001/07/12 05:09:52  vinod
  Modified code to use the prev and next pointers instead of using STL methods

  Revision 1.4  2001/04/08 21:18:08  vinod
  Added writeDLFL, getVertexType

  Revision 1.3  2000/11/28 19:14:09  vinod
  Added computeNormal

  Revision 1.2  2000/11/21 19:19:49  vinod
  Added dump() method

  Revision 1.1.1.1  2000/11/02 19:32:43  vinod
  New DLFL Sources

*/
