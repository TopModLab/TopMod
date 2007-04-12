/* $Id$ */

#ifndef _TM_BEZIER_PATCH_HH_

#define _TM_BEZIER_PATCH_HH_

#include "TMVertexControlPoint.hh"
#include "TMFaceControlPoint.hh"
#include "TMEdgeControlPoint.hh"
#include "TMBezierPatchPoint.hh"

class TMBezierPatch;
typedef vector<TMBezierPatch> TMBezierPatchArray;

   // Enum for type of patch. The value of the constant is the order of the surface
enum TMPatchType { Linear=1, Cubic=3, Quintic=5 };
     
class TMBezierPatch
{
  protected :

     int patchsize;                                    // Size of patch: 2, 4 or 6
     vector<TMBezierPatchPointArray> patch;            // The points in the patch
     TMVertexControlPoint vcp;                         // Vertex control point
     TMEdgeControlPoint ecp1, ecp2;                    // Edge control points
     TMFaceControlPoint fcp;                           // Face control point
     GLdouble * glcparray;                             // Array of points sent to OpenGL

  public :

        // Default and 1-arg constructor
     TMBezierPatch(TMPatchType type=Linear)
       : patchsize(int(type)+1), patch(), vcp(), fcp(), ecp1(), ecp2(), glcparray(NULL)
       {
         resizePatch();
       }

        // Copy constructor
     TMBezierPatch(const TMBezierPatch& tmbp)
       : patchsize(tmbp.patchsize), patch(tmbp.patch), vcp(tmbp.vcp), fcp(tmbp.fcp), ecp1(tmbp.ecp1), ecp2(tmbp.ecp2), glcparray(NULL)
       {
         allocateGLArray();
         fillGLArray();
       }

        // Destructor
     ~TMBezierPatch()
       {
         delete [] glcparray;
       }

        // Assignment operatpr
     TMBezierPatch& operator = (const TMBezierPatch& tmbp)
       {
         patchsize = tmbp.patchsize;
         patch = tmbp.patch;
         vcp = tmbp.vcp; fcp = tmbp.fcp; ecp1 = tmbp.ecp1; ecp2 = tmbp.ecp2;
         allocateGLArray();
         fillGLArray();
         return (*this);
       }

  protected :

        // Set the parametric coords for every point in the patch
     void setPatchParametricCoords(void)
       {
         double du = 1.0/(patchsize-1.0);
         double dv = 1.0/(patchsize-1.0);
         for (int row=0; row < patchsize; ++row)
            for (int col=0; col < patchsize; ++col)
               {
                    // NOTE: row corresponds to v and col corresponds to u
                 patch[row][col].u = col * du;
                 patch[row][col].v = row * dv;
               }
       }

     void resizePatch(void)
       {
         patch.clear();
         patch.resize(patchsize);
         for (int row=0; row < patchsize; ++row)
            patch[row].resize(patchsize);
         setPatchParametricCoords();
         allocateGLArray();
       }

        // Compute the size of the array to be sent to OpenGL
     int arraySize(void) const
       {
            // Compute and return the size of the array to be sent to OpenGL
         return patchsize*patchsize*3;
       }

        // Allocate memory for the array to be sent to OpenGL
     void allocateGLArray()
       {
         if ( glcparray ) delete [] glcparray;
         glcparray = NULL;
         glcparray = new GLdouble[arraySize()];
       }

        // Compute the index into the array to be sent to OpenGL for given row, column and component
     int arrayIndex(int row, int col, int component) const
       {
         return (row*patchsize + col) * 3 + component;
       }
     
        // Fill the array to be sent to OpenGL for rendering
     void fillGLArray(void);
         
  public :
     
     void setPatchType(TMPatchType type)
       {
         patchsize = int(type) + 1;
         resizePatch();
       }

     TMBezierPatchPointArray& operator [] (int row)
       {
         return patch[row];
       }

     TMBezierPatchPoint& operator () (int row, int col)
       {
         return patch[row][col];
       }

     const TMBezierPatchPointArray& operator [] (int row) const
       {
         return patch[row];
       }

     const TMBezierPatchPoint& operator () (int row, int col) const
       {
         return patch[row][col];
       }

     void setControlPoints(const TMVertexControlPoint& tmvcp,
                           const TMFaceControlPoint& tmfcp,
                           const TMEdgeControlPoint& tmecp1,
                           const TMEdgeControlPoint& tmecp2)
       {
         vcp = tmvcp; fcp = tmfcp; ecp1 = tmecp1; ecp2 = tmecp2;
       }

        // Update positions of the patch points using the control points
     void updatePositions(void);

  protected :

        // Do GL setup before rendering patch
     void glSetup(void) const;

  public :

        // Render this surface as filled polygons
     void render(void) const;

        // Render this surface as wireframe
     void renderWireframe(void) const;

        // Render this surface as points
     void renderPoints(void) const;
};


#endif /* #ifndef _TM_BEZIER_PATCH_HH_ */


/*
  $Log$
*/
