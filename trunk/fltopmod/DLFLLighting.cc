/* $Id: DLFLLighting.cc,v 4.0 2003/12/26 01:58:52 vinod Exp $ */

#ifndef _DLFL_LIGHTING_HH_

#define _DLFL_LIGHTING_HH_

// Subroutines for lighting computations in DLFL

#include "DLFLFace.hh"
#include "DLFLObject.hh"

#pragma set woff 1174

void DLFLFace :: computeLighting(LightPtr lightptr)
{
  if ( head )
     {
          //double Ka = matl_ptr->Ka;
       double Kd = matl_ptr->Kd;
          //double Ks = matl_ptr->Ks;
       RGBColor basecolor = matl_ptr->color;
       RGBColor fvcolor;
       Vector3d normal, pos;

       DLFLFaceVertexPtr current = head;
       normal = current->getNormal();
       pos = current->getVertexCoords();

       fvcolor = lightptr->illuminate(pos,normal);
       fvcolor *= Kd;
       fvcolor += (1.0-Kd)*basecolor;

       current->color = fvcolor;

       current = current->next();
       while ( current != head )
          {
            normal = current->getNormal();
            pos = current->getVertexCoords();

            fvcolor = lightptr->illuminate(pos,normal);
            fvcolor *= Kd;
            fvcolor += (1.0-Kd)*basecolor;

            current->color = fvcolor;

            current = current->next();
          }
     }
}

void DLFLObject :: computeNormals(void)
{
  DLFLVertexPtrList :: iterator first, last;

  first = vertex_list.begin(); last = vertex_list.end();
  while ( first != last )
     {
       (*first)->updateNormal();
       ++first;
     }
}

void DLFLObject :: computeLighting(LightPtr lightptr)
{
  DLFLFacePtrList :: iterator first, last;
  DLFLFacePtr faceptr;

  first = face_list.begin(); last = face_list.end();
  while ( first != last )
     {
       faceptr = (*first);
       faceptr->computeLighting(lightptr);
       ++first;
     }

  TMPatchFacePtrList :: iterator pfirst = patch_list.begin(), plast = patch_list.end();
  TMPatchFacePtr pfp = NULL;
  while ( pfirst != plast )
     {
       pfp = (*pfirst); ++pfirst;
       pfp->computeLighting(lightptr);
     }
}

#pragma reset woff 1174

#endif /* #ifndef _DLFL_LIGHTING_HH_ */

/*
  $Log: DLFLLighting.cc,v $
  Revision 4.0  2003/12/26 01:58:52  vinod
  Major version sync.

  Revision 3.0  2003/09/19 16:06:47  vinod
  Major version update

  Revision 2.1  2003/01/05 21:11:26  vinod
  Commented out unused variables

  Revision 2.0  2001/07/25 05:12:44  vinod
  Major version sync

  Revision 1.2  2001/07/16 04:33:11  vinod
  Modified to use new list structure

  Revision 1.1  2000/11/28 19:18:12  vinod
  Lighting/normal computations for DLFL

*/
