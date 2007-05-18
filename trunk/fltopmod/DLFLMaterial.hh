/* $Id: DLFLMaterial.hh,v 4.0 2003/12/26 01:58:52 vinod Exp $ */

#ifndef _DLFL_MATERIAL_HH_

#define _DLFL_MATERIAL_HH_

// class for a material for use in the DLFLObject class
// Contains an array/list of faces which use this material

#include "DLFLCommon.hh"
#include "DLFLFace.hh"

class DLFLMaterial
{
  public :

     char *          name;                             // Name of material
     RGBColor        color;                            // Material diffuse color
     DLFLFacePtrList faces;                            // Pointers to faces using this material
     double          Ka;                               // Ambient coefficient
     double          Kd;                               // Diffuse coefficient
     double          Ks;                               // Specular coefficient

  public :

        // Default, 1 and 2 arg constructors
     DLFLMaterial(const char * n, const RGBColor& c = RGBColor(0))
       : name(NULL), color(c), faces(), Ka(0.1), Kd(0.3), Ks(0.8)
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

     DLFLMaterial(const char * n, double r, double g, double b)
       : name(NULL), color(r,g,b), faces(), Ka(0.1), Kd(0.3), Ks(0.8)
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

     DLFLMaterial(const DLFLMaterial& mat)
       : name(NULL), color(mat.color), faces(mat.faces), Ka(mat.Ka), Kd(mat.Kd), Ks(mat.Ks)
       {
         name = new char[strlen(mat.name)+1]; strcpy(name,mat.name);
       }

     ~DLFLMaterial()
       {
         faces.clear();
         delete [] name;
       }

     DLFLMaterial& operator = (const DLFLMaterial& mat)
       {
         delete [] name; name = NULL;
         name = new char[strlen(mat.name)+1]; strcpy(name,mat.name);
         color = mat.color; faces = mat.faces; Ka = mat.Ka; Kd = mat.Kd; Ks = mat.Ks;
         return (*this);
       }

     DLFLMaterialPtr copy(void) const
       {
         DLFLMaterialPtr mpcopy = new DLFLMaterial(*this);
         return mpcopy;
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

     void addFace(DLFLFacePtr faceptr)
       {
         faces.push_back(faceptr);
       }

     void deleteFace(DLFLFacePtr faceptr)
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

        // Same as above, but function form
     bool equals(const RGBColor& c) const
       {
         return (color == c);
       }

        // Check if material name is same as given string
     bool operator == (const char * n) const
       {
         if ( n && !strcasecmp(name,n) ) return true;
         return false;
       }

        // Same as above but function form
     bool equals(const char * n) const
       {
         if ( n && !strcasecmp(name,n) ) return true;
         return false;
       }

        // Compare 2 materials - check both name and color
     bool operator == (const DLFLMaterial& mat) const
       {
         if ( (color == mat.color) &&
              !strcasecmp(name,mat.name) ) return true;
         return false;
       }

        // Same as above but function form
     bool equals(const DLFLMaterial& mat) const
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

     friend void matl_render(DLFLMaterialPtr matl)
       {
         matl->render();
       }

     friend void matl_renderFaces(DLFLMaterialPtr matl)
       {
         matl->renderFaces();
       }

     friend void matl_plainRender(DLFLMaterialPtr matl)
       {
         matl->plainRender();
       }

     friend void matl_renderT(DLFLMaterialPtr matl)
       {
         matl->renderT();
       }

     friend void matl_renderFacesT(DLFLMaterialPtr matl)
       {
         matl->renderFacesT();
       }

     friend void matl_plainRenderT(DLFLMaterialPtr matl)
       {
         matl->plainRenderT();
       }

     friend void matl_outline(DLFLMaterialPtr matl)
       {
         matl->outline();
       }

     friend void matl_outlineFaces(DLFLMaterialPtr matl)
       {
         matl->outlineFaces();
       }

     friend void matl_plainOutline(DLFLMaterialPtr matl)
       {
         matl->plainOutline();
       }

     friend ostream& operator << (ostream& o, const DLFLMaterial& mat)
       {
         o << "Material : " << mat.name << ' ' << mat.color << ", "
           << mat.faces.size() << " faces" << endl;
         DLFLFacePtrList :: const_iterator first, last;
         first = mat.faces.begin(); last = mat.faces.end();
         while ( first != last )
            {
              o << *(*first) << endl;
              ++first;
            }
         return o;
       }
};

#endif /* #ifndef _DLFL_MATERIAL_HH_ */

/*
  $Log: DLFLMaterial.hh,v $
  Revision 4.0  2003/12/26 01:58:52  vinod
  Major version sync.

  Revision 3.0  2003/09/19 16:06:47  vinod
  Major version update

  Revision 2.1  2002/10/25 03:00:29  vinod
  Added faces.clear() in destructor

  Revision 2.0  2001/07/25 05:12:44  vinod
  Major version sync

  Revision 1.3  2000/12/13 22:51:23  ergun
  Changed default coefficients

  Revision 1.2  2000/11/28 19:14:45  vinod
  Added lighting coefficients

  Revision 1.1.1.1  2000/11/02 19:32:43  vinod
  New DLFL Sources

*/
