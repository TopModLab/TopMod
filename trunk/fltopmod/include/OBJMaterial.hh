/* $Id: OBJMaterial.hh,v 1.1 2006/09/04 19:22:53 stuart Exp $ */

#ifndef _OBJ_MATERIAL_HH_

#define _OBJ_MATERIAL_HH_

// class for a material for use in the OBJObject class
// Reads .mtl files for material specification (4/18/00 - not yet)
// Contains an array/list of faces which use this material

#include <Color.hh>
#include <StreamIO.hh>
#include <Inlines.hh>
#include <string.h>
#include <vector.h>
#include "OBJFace.hh"

class OBJMaterial
{
  public :

     char *          name;                             // Name of material
     RGBColor        color;                            // Material diffuse color
     OBJFacePtrList  faces;                            // Pointers to faces using this material

  public :

        // Default, 1 and 2 arg constructors
     OBJMaterial(const char * n, const RGBColor& c = RGBColor(0))
       : name(NULL), color(c), faces()
       {
         if ( n )
            {
              name = new char[strlen(n)+1]; strcpy(name,n);
            }
         else
            {
              name = new char[8]; strcpy(name,"default");
            }
       }

     OBJMaterial(const char * n, double r, double g, double b)
       : name(NULL), color(r,g,b), faces()
       {
         if ( n )
            {
              name = new char[strlen(n)+1]; strcpy(name,n);
            }
         else
            {
              name = new char[8]; strcpy(name,"default");
            }
       }

     OBJMaterial(const OBJMaterial& mat)
       : name(NULL), color(mat.color), faces(mat.faces)
       {
         name = new char[strlen(mat.name)+1]; strcpy(name,mat.name);
       }

     ~OBJMaterial()
       {
         delete [] name;
       }

     OBJMaterial& operator = (const OBJMaterial& mat)
       {
         delete [] name; name = NULL;
         name = new char[strlen(mat.name)+1]; strcpy(name,mat.name);
         color = mat.color; faces = mat.faces;
         return (*this);
       }

        //--- Member functions ---//

     void setColor(const RGBColor& c)
       {
         color = c;
       }

     void setColor(double r, double g, double b)
       {
         color.set(r,g,b);
       }

     void setName(const char * n)
       {
         if ( n )
            {
              delete [] name; name = NULL;
              name = new char[strlen(n)]; strcpy(name,n);
            }
       }

     void addFace(OBJFacePtr faceptr)
       {
         faces.push_back(faceptr);
       }

     void deleteFace(OBJFacePtr faceptr)
       {
         faces.remove(faceptr);
       }

     uint numFaces(void) const
       {
         return faces.size();
       }

        // Check if material color is same as given color
     bool operator == (const RGBColor& c) const
       {
         return (color == c);
       }

        // Check if material name is same as given string
     bool operator == (const char * n) const
       {
         if ( n && !strcasecmp(name,n) ) return true;
         return false;
       }

        // Compare 2 materials - check both name and color
     bool operator == (const OBJMaterial& mat) const
       {
         if ( (color == mat.color) &&
              !strcasecmp(name,mat.name) ) return true;
         return false;
       }

        //--- Rendering functions ---//

     void render(void) const
       {
            // Use the material properties and render the faces
         glColor(color);
         for_each(faces.begin(),faces.end(),facerender);
       }

     void renderFaces(void) const
       {
            // Render the faces without using the material properties
            // Uses the face vertex colors and normals
         for_each(faces.begin(),faces.end(),facerender_FVCN);
       }

     void plainRender(void) const
       {
            // Render the faces without using the material properties
            // or the face colors. Uses the Face normals
         for_each(faces.begin(),faces.end(),facerender_FVN);
       }

        // Same as above but with texturing
     void renderT(void) const
       {
            // Use the material properties and render the faces
         glColor(color);
         for_each(faces.begin(),faces.end(),facerender_FVNT);
       }

     void renderFacesT(void) const
       {
            // Render the faces without using the material properties
            // Uses the face vertex colors and normals
         for_each(faces.begin(),faces.end(),facerender_FVCNT);
       }

     void plainRenderT(void) const
       {
            // Render the faces without using the material properties
            // or the face colors. Uses the Face normals
         for_each(faces.begin(),faces.end(),facerender_FVNT);
       }

     void outline(void) const
       {
            // Use the material properties and outline the faces
         glColor(color);
         for_each(faces.begin(),faces.end(),faceoutline);
       }

     void outlineFaces(void) const
       {
            // Outline the faces without using the material properties
            // Uses the face vertex colors
         for_each(faces.begin(),faces.end(),faceoutline_FVC);
       }

     void plainOutline(void) const
       {
            // Outline the faces without using the material properties
            // or the face colors
         for_each(faces.begin(),faces.end(),faceoutline_FV);
       }

        //--- Friend versions of the above rendering functions ---//

        // Names have been changed to avoid conflicts during STL instantiation
        // because of functions with same name

     friend void matl_render(const OBJMaterial& matl)
       {
         matl.render();
       }

     friend void matl_renderFaces(const OBJMaterial& matl)
       {
         matl.renderFaces();
       }

     friend void matl_plainRender(const OBJMaterial& matl)
       {
         matl.plainRender();
       }

     friend void matl_renderT(const OBJMaterial& matl)
       {
         matl.renderT();
       }

     friend void matl_renderFacesT(const OBJMaterial& matl)
       {
         matl.renderFacesT();
       }

     friend void matl_plainRenderT(const OBJMaterial& matl)
       {
         matl.plainRenderT();
       }

     friend void matl_outline(const OBJMaterial& matl)
       {
         matl.outline();
       }

     friend void matl_outlineFaces(const OBJMaterial& matl)
       {
         matl.outlineFaces();
       }

     friend void matl_plainOutline(const OBJMaterial& matl)
       {
         matl.plainOutline();
       }

     friend ostream& operator << (ostream& o, const OBJMaterial& mat)
       {
         o << "Material : " << mat.name << ' ' << mat.color << ", "
           << mat.faces.size() << " faces" << endl;
         OBJFacePtrList :: const_iterator first, last;
         first = mat.faces.begin(); last = mat.faces.end();
         while ( first != last )
            {
              o << *(*first) << endl;
              ++first;
            }
         return o;
       }
};

#endif /* #ifndef _OBJ_MATERIAL_HH_ */

/*
  $Log: OBJMaterial.hh,v $
  Revision 1.1  2006/09/04 19:22:53  stuart
  Added includes and libs.

  Revision 1.4  2000/09/22 18:17:01  vinod
  Now uses list of OBJFacePtr. Overhaul of rendering functions

  Revision 1.3  2000/05/13 01:56:43  vinod
  Added member functions to access the face array

  Revision 1.2  2000/04/25 04:01:50  vinod
  Added functions for using texturing

  Revision 1.1  2000/04/19 21:52:24  vinod
  OBJObject class. Initial version. Modified from SimpleOBJ

*/
