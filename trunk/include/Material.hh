/* $Id: Material.hh,v 1.1 2006/09/04 19:22:52 stuart Exp $ */

#ifndef _MATERIAL_HH_

#define _MATERIAL_HH_

// class for a material. Currently has only the color.

#include <Color.hh>
#include <list.hh>

class Material;
typedef Material * MaterialPtr;

typedef list<Material> MaterialList;

class Material
{
  public :

     RGBColor color;                                   // Diffuse color of material

     Material()
       : color(0,0,0)
       {}

     Material(double r, double g, double b)
       : color(r,g,b)
       {}

     Material(const RGBColor& col)
       : color(col)
       {}

     Material(const Material& mat)
       : color(mat.color)
       {}

     ~Material()
       {}

     Material& operator = (const Material& mat)
       {
         color = mat.color;
         return (*this);
       }

        //-- IOstream operators. Currently the material is just the color
        //-- Color is specified as a triplet of 3 numbers in the range 0 to 1
        //-- separated by spaces
     ostream& operator << (ostream& o, const Material& mat)
       {
         o << mat.color.r << ' ' << mat.color.g << ' ' << mat.color.b << ' ';
         return o;
       }

     istream& operator >> (istream& i, Material& mat)
       {
         double r,g,b;
         i >> r >> g >> b;
         mat.color.set(r,g,b);
         return i;
       }

        // Do a glColor using this material
     friend void glColor(const Material& mat)
       {
         glColor(mat.color);
       }
};

#endif /* #ifndef _MATERIAL_HH_ */


/*
  $Log: Material.hh,v $
  Revision 1.1  2006/09/04 19:22:52  stuart
  Added includes and libs.

*/
