/*
*
* ***** BEGIN GPL LICENSE BLOCK *****
*
* This program is free software; you can redistribute it and/or
* modify it under the terms of the GNU General Public License
* as published by the Free Software Foundation; either version 2
* of the License, or (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program; if not, write to the Free Software  Foundation,
* Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
*
* The Original Code is Copyright (C) 2005 by xxxxxxxxxxxxxx
* All rights reserved.
*
* The Original Code is: all of this file.
*
* Contributor(s): none yet.
*
* ***** END GPL LICENSE BLOCK *****
*
* Short description of this file
*
* name of .hh file containing function prototypes
*
*/

/* $Id: DLFLStandardObjects.cc,v 4.0 2003/12/26 01:58:53 vinod Exp $ */

// Source code to create standard objects as DLFL objects
// These are static methods in the DLFLObject class
// All methods return a pointer to a newly created DLFLObject
// Call has to delete the pointer

#include <DLFLObject.hh>

using namespace DLFL;

struct mstriplet {
     int i, j, k;
};

static mstriplet msfilled[20] = {
  { 0, 0, 0 }, { 0, 0, 1 }, { 0, 0, 2 },
  { 0, 1, 0 },              { 0, 1, 2 },
  { 0, 2, 0 }, { 0, 2, 1 }, { 0, 2, 2 },
  { 1, 0, 0 },              { 1, 0, 2 },

  { 1, 2, 0 },              { 1, 2, 2 },
  { 2, 0, 0 }, { 2, 0, 1 }, { 2, 0, 2 },
  { 2, 1, 0 },              { 2, 1, 2 },
  { 2, 2, 0 }, { 2, 2, 1 }, { 2, 2, 2 }
};
  

   // Create a unit cube centered at origin
DLFLObjectPtr DLFLObject :: makeUnitCube(void)
{
  DLFLObjectPtr unitcube = new DLFLObject();
  Vector3dArray verts;
  Vector3d p;
  double x = 0.5;

  verts.resize(4);

     // Create top face
  p.set(x,x,x); verts[0] = p;
  p.set(x,x,-x); verts[1] = p;
  p.set(-x,x,-x); verts[2] = p;
  p.set(-x,x,x); verts[3] = p;
  unitcube->createFace(verts);
  DLFLFacePtr fp1 = unitcube->lastFace();

     // Create bottom face
  p.set(x,-x,x); verts[0] = p;
  p.set(-x,-x,x); verts[1] = p;
  p.set(-x,-x,-x); verts[2] = p;
  p.set(x,-x,-x); verts[3] = p;
  unitcube->createFace(verts);
  DLFLFacePtr fp2 = unitcube->lastFace();

     // fp1 and fp2 will be inward facing
     // Connect them using the first corners in each
  DLFLFaceVertexPtr fvp1, fvp2;
  fvp1 = fp1->firstVertex(); fvp2 = fp2->firstVertex();
  unitcube->connectFaces(fvp1,fvp2);

  return unitcube;
}

   // Create a cube of specified edge length centered at origin
DLFLObjectPtr DLFLObject :: makeUnitCube(double edgelength)
{
  DLFLObjectPtr unitcube = new DLFLObject();
  Vector3dArray verts;
  Vector3d p;
  double x = edgelength/2.0;

  verts.resize(4);

     // Create top face
  p.set(x,x,x); verts[0] = p;
  p.set(x,x,-x); verts[1] = p;
  p.set(-x,x,-x); verts[2] = p;
  p.set(-x,x,x); verts[3] = p;
  unitcube->createFace(verts);
  DLFLFacePtr fp1 = unitcube->lastFace();

     // Create bottom face
  p.set(x,-x,x); verts[0] = p;
  p.set(-x,-x,x); verts[1] = p;
  p.set(-x,-x,-x); verts[2] = p;
  p.set(x,-x,-x); verts[3] = p;
  unitcube->createFace(verts);
  DLFLFacePtr fp2 = unitcube->lastFace();

     // fp1 and fp2 will be inward facing
     // Connect them using the first corners in each
  DLFLFaceVertexPtr fvp1, fvp2;
  fvp1 = fp1->firstVertex(); fvp2 = fp2->firstVertex();
  unitcube->connectFaces(fvp1,fvp2);

  return unitcube;
}

   // Create a tetrahedron with edge length = 1 and centered at origin
DLFLObjectPtr DLFLObject :: makeUnitTetrahedron(void)
{
  DLFLObjectPtr unittetra = new DLFLObject();
  Vector3dArray verts;
  double rt3 = sqrt(3.0), rt6 = sqrt(6.0);
  verts.resize(3);

     // Build the bottom face on the ZX plane. This faces downward
  verts[0].set(-rt3/6.0,0,0.5);
  verts[1].set(-rt3/6.0,0,-0.5);
  verts[2].set(rt3/3.0,0,0.0);

  unittetra->createFace(verts);

     // Get the upward facing face - the last in the face list
  DLFLFacePtr fp = unittetra->lastFace();

     // Triangulate this face - a new point will be created at centroid
  unittetra->subDivideFace(fp,false);

     // Adjust the coordinates of the new point to get the top point
  DLFLVertexPtr vp = unittetra->lastVertex();
  vp->coords.set(0,rt6/3.0,0);

     // Translate the whole object so centroid is at origin
  unittetra->position.set(0,-rt6/12.0,0);
  unittetra->freezeTransformations(); // Optimized version should be used

  return unittetra;
}

   // Create a level 1 Menger sponge using unit cubes
DLFLObjectPtr DLFLObject :: makeMengerSponge(void)
{
  DLFLObjectPtr mengersponge = new DLFLObject();

     // Make 20 unit cubes and splice them together
     // after suitably transforming them
  Vector3d opos;
  DLFLObjectPtr unitcube;

     // We'll first build the sponge with one corner at the origin
     // and in the +++ octant. Then we'll translate it to the correct position

  int i,j,k;
  for (int m=0; m < 20; ++m)
     {
       i = msfilled[m].i; j = msfilled[m].j; k = msfilled[m].k;
       opos.set(0.5+i*1.0,0.5+j*1.0,0.5+k*1.0);
       unitcube = DLFLObject::makeUnitCube();
       unitcube->position = opos;
       unitcube->freezeTransformations(); // An optimized version should be used
       mengersponge->splice(*unitcube);
       delete unitcube;
     }

     // Now translate this object to the correct position
  mengersponge->position.set(-1.5,-1.5,-1.5);
  mengersponge->freezeTransformations();
  return mengersponge;
}

   // Create a level 1 Menger sponge using cubes of specified length
DLFLObjectPtr DLFLObject :: makeMengerSponge(double edgelength)
{
  DLFLObjectPtr mengersponge = new DLFLObject();

     // Make 20 unit cubes and splice them together
     // after suitably transforming them
  Vector3d opos;
  DLFLObjectPtr unitcube;

     // We'll first build the sponge with one corner at the origin
     // and in the +++ octant. Then we'll translate it to the correct position

  int i,j,k;
  for (int m=0; m < 20; ++m)
     {
       i = msfilled[m].i; j = msfilled[m].j; k = msfilled[m].k;
       opos.set(edgelength/2.0+i*edgelength,
                edgelength/2.0+j*edgelength,
                edgelength/2.0+k*edgelength);
       unitcube = DLFLObject::makeUnitCube();
       unitcube->position = opos;
       unitcube->freezeTransformations(); // An optimized version should be used
       mengersponge->splice(*unitcube);
       delete unitcube;
     }

     // Now translate this object to the correct position
  mengersponge->position.set(-1.5*edgelength,-1.5*edgelength,-1.5*edgelength);
  mengersponge->freezeTransformations();
  return mengersponge;
}

   // Create a specified level Menger sponge using cubes of unit length
   // This is a recursive function
DLFLObjectPtr DLFLObject :: makeMengerSponge(int level)
{
  if ( level < 1 ) return NULL;
  if ( level == 1 ) return makeMengerSponge();
  
  DLFLObjectPtr mengersponge = new DLFLObject();

     // Make 20 Menger sponges of 1 lower level and splice them together
     // after suitably transforming them
  Vector3d opos;
  DLFLObjectPtr submengersponge; // Next lower level Menger sponge

     // We'll first build the sponge with one corner at the origin
     // and in the +++ octant. Then we'll translate it to the correct position

  double baselength = pow(3.0,level-1); // Edge length of each sub-Menger sponge at this level
  double baselength_2 = baselength/2.0;
  int i,j,k;
  for (int m=0; m < 20; ++m)
     {
       i = msfilled[m].i; j = msfilled[m].j; k = msfilled[m].k;
       opos.set(baselength_2 + i*baselength,
                baselength_2 + j*baselength,
                baselength_2 + k*baselength);
       submengersponge = DLFLObject::makeMengerSponge(level-1);
       submengersponge->position = opos;
       submengersponge->freezeTransformations(); // An optimized version should be used
       mengersponge->splice(*submengersponge);
       delete submengersponge;
     }

     // Now translate this object to the correct position
  mengersponge->position.set(-1.5*baselength,-1.5*baselength,-1.5*baselength);
  mengersponge->freezeTransformations();
  return mengersponge;
}

   // Create a Sierpinski tetrahedron with smallest edge length = 1 and centered at origin
DLFLObjectPtr DLFLObject :: makeSierpinskiTetrahedron(int level)
{
  if ( level < 0 ) return NULL;
  if ( level == 0 ) return makeUnitTetrahedron();

  DLFLObjectPtr stetra = new DLFLObject();

     // Make 4 copies of level-1 Sierpinski tetrahedron
     // and splice them together after suitable transformations
  double rt3_6 = sqrt(3.0)/6.0, rt6_12 = sqrt(6.0)/12.0;
  double baselength = pow(2.0,level-1); // Edge length of sub-Sierpinski tetra at this level
  DLFLObjectPtr substetra; // Next lower level Sierpinski tetra

  substetra = DLFLObject::makeSierpinskiTetrahedron(level-1);
  substetra->position.set(baselength*rt3_6,baselength*rt6_12,baselength*0.5);
  substetra->freezeTransformations();
  stetra->splice(*substetra);
  delete substetra;
  
  substetra = DLFLObject::makeSierpinskiTetrahedron(level-1);
  substetra->position.set(baselength*rt3_6,baselength*rt6_12,baselength*1.5);
  substetra->freezeTransformations();
  stetra->splice(*substetra);
  delete substetra;
  
  substetra = DLFLObject::makeSierpinskiTetrahedron(level-1);
  substetra->position.set(baselength*rt3_6*4.0,baselength*rt6_12,baselength);
  substetra->freezeTransformations();
  stetra->splice(*substetra);
  delete substetra;
  
  substetra = DLFLObject::makeSierpinskiTetrahedron(level-1);
  substetra->position.set(baselength*rt3_6*2.0,baselength*rt6_12*5.0,baselength);
  substetra->freezeTransformations();
  stetra->splice(*substetra);
  delete substetra;

  stetra->position.set(-baselength*rt3_6*2.0,-baselength*rt6_12,-baselength);
  stetra->freezeTransformations();
  return stetra;
}

/*
  $Log: DLFLStandardObjects.cc,v $
  Revision 4.0  2003/12/26 01:58:53  vinod
  Major version sync.

  Revision 3.2  2003/12/08 20:19:03  vinod
  Added methods for unit tetrahedron and Sierpinski tetrahedron.

  Revision 3.1  2003/12/08 07:48:56  vinod
  Subroutines for generating standard objects like cube procedurally

*/
