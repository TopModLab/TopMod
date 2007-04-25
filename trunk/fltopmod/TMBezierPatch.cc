/* $Id$ */

#include "TMBezierPatch.hh"

void TMBezierPatch :: fillGLArray(void)
{
     // Fill array to be sent to OpenGL
     // Assumes that memory has already been allocated
     // Note: Traversal order is row major, which is why loop variables are interchanged
  for (int i=0; i < patchsize; ++i)
     for (int j=0; j < patchsize; ++j)
        {
          glcparray[arrayIndex(i,j,0)] = patch[i][j].position[0];
          glcparray[arrayIndex(i,j,1)] = patch[i][j].position[1];
          glcparray[arrayIndex(i,j,2)] = patch[i][j].position[2];
        }
}

void TMBezierPatch :: updatePositions(void)
{
     // Compute the positions of all the points in this patch
     // Assumes that the parametric coordinates of all patch points have been set
     // Lower left (p00) is linked to vertex control point
     // Lower right (p10) is linked to edge control point 1
     // Upper left (p01) is linked to edge control point 2
     // Upper right (p11) is linked to face control point
  Vector3d p00, p10, p01, p11;
  p00 = vcp.position; p10 = ecp1.position;
  p01 = ecp2.position; p11 = fcp.position;

  double u,v;
  Vector3d p;

  for (int row=0; row < patchsize; ++row)
     for (int col=0; col < patchsize; ++col)
        {
          u = patch[row][col].u; v = patch[row][col].v;
          patch[row][col].position = (1.0-u)*(1.0-v)*p00 + u*(1.0-v)*p10 + u*v*p11 + (1.0-u)*v*p01;
        }

     // Fill the array to be sent to OpenGL
  fillGLArray();
}

void TMBezierPatch :: glSetup(void) const
{
     // Setup GL stuff for rendering this patch

     // Define the mapping function (evaluator)
     // The map has to be defined every time we render since we have multiple patches
  glMap2d(GL_MAP2_VERTEX_3,  // Map vertex information
          0.0,               // Minimum U
          1.0,               // Maximum U
          3,                 // U-stride
          patchsize,         // U-order
          0.0,               // Minimum V
          1.0,               // Maximum V
          3*patchsize,       // V-stride
          patchsize,         // V-order
          &glcparray[0]);

     // Enable the map (evaluator)
  glEnable(GL_MAP2_VERTEX_3);

     // Define the grid for the map
  glMapGrid2d(patchsize,     // U segments
              0.0,           // Start U
              1.0,           // End U
              patchsize,     // V segments
              0.0,           // Start V
              1.0);          // End V
}

void TMBezierPatch :: render(void) const
{
     // Render this Bezier patch as filled polygons

  glSetup();
  glEvalMesh2(GL_FILL, 0, patchsize, 0, patchsize);
  glDisable(GL_MAP2_VERTEX_3);
}

void TMBezierPatch :: renderWireframe(void) const
{
     // Render this Bezier patch as a wireframe

  glSetup();
  glLineWidth(0.75);
  glEvalMesh2(GL_LINE, 0, patchsize, 0, patchsize);
  glLineWidth(1.0);
  glDisable(GL_MAP2_VERTEX_3);
}

void TMBezierPatch :: renderPoints(void) const
{
     // Render this Bezier patch as points

  glSetup();
  glPointSize(1.25);
  glEvalMesh2(GL_POINT, 0, patchsize, 0, patchsize);
  glPointSize(1.0);
  glDisable(GL_MAP2_VERTEX_3);
}


/*
  $Log$
*/
