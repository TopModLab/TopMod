/* $Id$ */

#ifndef _TM_BEZIER_SURFACE_HH_

#define _TM_BEZIER_SURFACE_HH_

#include "TMBezierPatch.hh"
#include "DLFLObject.hh"

class TMBezierSurface
{
  protected :

     TMPatchType type;
     TMBezierPatchArray patcharray;
     DLFLObjectPtr object;

  public :

        // Default and 1-arg constructor
     TMBezierSurface(TMPatchType t=Linear)
       : type(t), patcharray(), object(NULL)
       {}

        // 1-arg and 2-arg constructor
     TMBezierSurface(DLFLObjectPtr obj, TMPatchType t=Linear)
       : type(t), patcharray(), object(obj)
       {
         createSurface();
       }

        // Copy constructor
     TMBezierSurface(const TMBezierSurface& tmbs)
       : type(tmbs.type), patcharray(tmbs.patcharray), object(tmbs.object)
       {}

        // Destructor
     ~TMBezierSurface()
       {}

        // Assignment operator
     TMBezierSurface& operator = (const TMBezierSurface& tmbs)
       {
         type = tmbs.type;
         patcharray = tmbs.patcharray;
         object = tmbs.object;
         return (*this);
       }

     void setPatchType(TMPatchType t)
       {
         type = t;
         for (int i=0; i < patcharray.size(); ++i)
            patcharray[i].setPatchType(t);
       }

        // Create surface - object should have already been set
     void createSurface();

        // Set the DLFLObject to which this surface is attached
     void setObject(DLFLObjectPtr obj)
       {
         object = obj;
         createSurface();
       }

        // Render the surface as filled polygons
     void render(void) const
       {
         for (int i=0; i < patcharray.size(); ++i)
            patcharray[i].render();
       }

        // Render the surface as wireframe
     void renderWireframe(void) const
       {
         for (int i=0; i < patcharray.size(); ++i)
            patcharray[i].renderWireframe();
       }

        // Render the surface as points
     void renderPoints(void) const
       {
         for (int i=0; i < patcharray.size(); ++i)
            patcharray[i].renderPoints();
       }
};


#endif /* #ifndef _TM_BEZIER_SURFACE_HH_ */


/*
  $Log$
*/
