// brianb 

#ifndef _DLFL_LOCATOR_HH_

#define _DLFL_LOCATOR_HH_

// Class to act as a handle/transformation handle for DLFL verticies

#include "DLFLCommon.hh"
#include "DLFLVertex.hh"
// #ifdef __APPLE__
// #include <GL/glut.h>
// #else
// #include <GL/glut.h>
// #endif

class DLFLLocator
{
  protected :

     static uint suLastID;                             // Distinct ID for each instance
                                                       // The last assigned ID is stored in this
                                                       // class variable

        // Generate a new unique ID
     static uint newID(void)
       {
         uint temp = suLastID;
         suLastID++;
         return temp;
       }
     
  public :

     Vector3d              coords;                     // Coordinates of locator
     unsigned long         flags;                      // Variable for general use to store flags, etc.
     DLFLVertexPtr         vtx;                        // Associated vertex
     int                   selectedAxis;               // Index of selected axis -1=none,0=x,1=y,2=z,3=free
     bool                  renderSelection;

  protected :

     uint                  uID;                        // ID for this DLFLLocator

        // Assign a unique ID for this instance
     void assignID(void)
       {
         uID = DLFLLocator :: newID();
       }

  public :
     
        // Default constructor
     DLFLLocator()
       : coords(), flags(0)
       {
         vtx = NULL;
         assignID();
         selectedAxis = -1;
         renderSelection = false;
       }

        // 1 argument constructor
     DLFLLocator(const Vector3d& vec)
       : coords(vec), flags(0)
       {
         vtx = NULL;
         assignID();
         selectedAxis = -1;
         renderSelection = false;
       }

        // 3 argument constructor
     DLFLLocator(double x, double y, double z)
       : coords(x,y,z), flags(0)
       {
         vtx = NULL;
         assignID();
         selectedAxis = -1;
         renderSelection = false;
       }

        // Copy constructor
     DLFLLocator(const DLFLLocator& dl)
       : vtx(dl.vtx), coords(dl.coords), flags(dl.flags), uID(dl.uID), selectedAxis(dl.selectedAxis),
         renderSelection(dl.renderSelection)
       { }

        // Destructor
     ~DLFLLocator()
       {}

        // Assignment operator
     DLFLLocator& operator = (const DLFLLocator& dl)
       {
         vtx = dl.vtx;
         coords = dl.coords; flags = dl.flags;
         uID = dl.uID;
         return (*this);
       }

     DLFLLocatorPtr copy(void) const
       {
         DLFLLocatorPtr newdl = new DLFLLocator(*this);
         return newdl;
       }

        // Dump contents of this object
     void dump(ostream& o) const;

     void reset(void)
       {
         coords.reset(); flags = 0;
       }

     void makeUnique(void)
       {
         assignID();
       }

     friend void makeLocatorUnique(DLFLLocatorPtr dlp)
       {
         dlp->assignID();
       }

        //--- Query functions ---//

     Vector3d getCoords(void) const
       {
         return coords;
       }

     uint getID(void) const
       {
         return uID;
       }
     int getSelectedAxis() const
       {
         return selectedAxis;
       }

        //--- Mutative functions ---//
     void setRenderSelection(bool s)
       {
         renderSelection = s;
       }

     void setSelectedAxis(int axis)
       {
         if (axis >= 0 && axis <= 3)
           selectedAxis = axis;
         else
           selectedAxis = -1;
       }

     void setCoords(const Vector3d& p)
       {
         coords = p;
       }

     void setActiveVertex(DLFLVertexPtr vptr)
       {
         vtx = vptr;
       }

     DLFLVertexPtr getActiveVertex()
       {
         return vtx;
       }

        // Apply a transformation specified by the matrix to the coordinates
     void transform(const Matrix4x4& tmat)
       {
         Vector4d tp(coords); tp[3] = 1.0;
         tp = tmat * tp;
         tp /= tp[3];
         coords = tp;
       }

        // Print out this locator 
     void print(void) const
       {
         cout << "DLFLLocator " << uID << "," << vtx << " : " << coords << endl;
       }

     void render(void);
};

#endif /* #ifndef _DLFL_LOCATOR_HH_ */

// brianb
