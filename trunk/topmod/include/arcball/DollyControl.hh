/* $Id: DollyControl.hh,v 2.2 2000/05/02 19:09:04 vinod Exp $ */

#ifndef _DOLLY_CONTROL_HH_

#define _DOLLY_CONTROL_HH_

// Class for a translation controller

#include "../vecmat/Vector3d.hh"
#include "../vecmat/Matrix4x4.hh"

class DollyControl
{
  protected :

     Matrix4x4 mNow;                                   // Current transformation matrix
     bool dragging;
     double vNow, vDown;                               // Mouse translations/points
     double dNow, dDown;                               // Current dolly
     double scale_factor;                              // For scaling given coordinate value
     
  public :

     DollyControl(double sf=1.0)                       // Default constructor
       : mNow(), dragging(false), vNow(), vDown(), dNow(), dDown(),
         scale_factor(sf)
       {}

     DollyControl(const DollyControl& dc)              // Copy constructor
       : mNow(dc.mNow), dragging(dc.dragging), vNow(dc.vNow), vDown(dc.vDown),
         dNow(dc.dNow), dDown(dc.dDown), scale_factor(dc.scale_factor)
       {}

     ~DollyControl()                                   // Destructor
       {}

     DollyControl& operator = (const DollyControl& dc) // Assignment operator
       {
         mNow = dc.mNow; dragging = dc.dragging; vNow = dc.vNow; vDown = dc.vDown;
         dNow = dc.dNow; dDown = dc.dDown; scale_factor = dc.scale_factor;
         return (*this);
       }

     void reset(void)
       {
         dNow = 0.0; dDown = 0.0 ; mNow.reset();
       }

     void set_scale(double sf)
       {
         scale_factor = sf;
       }

     void reset_scale(void)
       {
         scale_factor = 1.0;
       }
     
     void mouse(const Vector3d& pos)                   // Specify mouse position
                                                       // only z coordinate is used
       {
         vNow = scale_factor*pos[2];
       }

     void mouse(double z)                              // Specify mouse position
       {
         vNow = scale_factor*z;
       }
     
     Matrix4_4 value(void) const                       // Get the translation matrix
       {
         return mNow;
       }

     double dolly_value(void) const                    // Get the dolly value
       {
         return dNow;
       }

     Vector3d dolly_vector(void) const                 // Get dolly value as vector
                                                       // dolly value is in z
       {
         return Vector3d(0,0,dNow);
       }
     
     void beginDrag(void)                              // Begin a drag
       {
         dragging = true; vDown = vNow;
       }
     
     void endDrag(void)                                // End a drag
       {
         dragging = false; dDown = dNow;
       }

     bool isDragging(void) const                       // Check dragging status
       {
         return dragging;
       }

     void update(void)                                 // Update the vectors and matrices
       {
         if ( dragging )
            {
              dNow = dDown; dNow += vNow; dNow -= vDown;

                 /*
                   NOTE : Matrix should be transposed before using in OpenGL
                 */
              mNow[0][3] = 0.0;
              mNow[1][3] = 0.0;
              mNow[2][3] = dNow;
            }
       }
};

#endif /* #ifndef _DOLLY_CONTROL_HH_ */


/*
  $Log: DollyControl.hh,v $
  Revision 2.2  2000/05/02 19:09:04  vinod
  Rotation matrix is now mathematically correct (transpose of GL)

  Revision 2.1  2000/03/31 09:10:24  vinod
  Dolly (movement along direction of sight) controller

*/
