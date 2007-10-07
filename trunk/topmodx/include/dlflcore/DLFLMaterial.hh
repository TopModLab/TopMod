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
*/

#ifndef _DLFL_MATERIAL_HH_
#define _DLFL_MATERIAL_HH_

// class for a material for use in the DLFLObject class
// Contains an array/list of faces which use this material

#include "DLFLCommon.hh"
#include "DLFLFace.hh"
#include <Graphics/Color.hh>

namespace DLFL {

class DLFLMaterial {
public :

  char *          name;                     // Name of material
  RGBColor        color;                    // Material diffuse color
  DLFLFacePtrList faces;                    // Pointers to faces using this material
  double          Ka;                       // Ambient coefficient
  double          Kd;                       // Diffuse coefficient
  double          Ks;                       // Specular coefficient

public :

  // Default, 1 and 2 arg constructors
  DLFLMaterial(const char * n, const RGBColor& c = RGBColor(0))
    : name(NULL), color(c), faces(), Ka(0.1), Kd(0.3), Ks(0.8) {
    if ( n ) {
      name = new char[strlen(n)+1]; strcpy(name,n);
    } else {
      name = new char[8]; strcpy(name,"default");
    }
  }

  DLFLMaterial(const char * n, double r, double g, double b)
    : name(NULL), color(r,g,b), faces(), Ka(0.1), Kd(0.3), Ks(0.8) {
    if ( n ) {
      name = new char[strlen(n)+1]; strcpy(name,n);
    } else {
      name = new char[8]; strcpy(name,"default");
    }
  }

  DLFLMaterial(const DLFLMaterial& mat)
    : name(NULL), color(mat.color), faces(mat.faces), Ka(mat.Ka), Kd(mat.Kd), Ks(mat.Ks) {
    name = new char[strlen(mat.name)+1]; strcpy(name,mat.name);
  }

  ~DLFLMaterial() {
    faces.clear();
    delete [] name;
  }

  DLFLMaterial& operator = (const DLFLMaterial& mat) {
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

  void setColor(const RGBColor& c) {
    color = c;
  }

  void setColor(double r, double g, double b) {
    color.set(r,g,b);
  }

  void setName(const char * n) {
    if ( n ) {
      delete [] name; name = NULL;
      name = new char[strlen(n)]; strcpy(name,n);
    }
  }

  void addFace(DLFLFacePtr faceptr) {
    faces.push_back(faceptr);
  }

  void deleteFace(DLFLFacePtr faceptr) {
    if( faces.size() > 0 )
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

  /*friend ostream& operator << (ostream& o, const DLFLMaterial& mat) {
    o << "Material : " << mat.name << ' ' << mat.color << ", "
      << mat.faces.size() << " faces" << endl;
    DLFLFacePtrList :: const_iterator first, last;
    first = mat.faces.begin(); last = mat.faces.end();
    while ( first != last ) {
      o << *(*first) << endl;
      ++first;
    }
    return o;
    };*/
};

} // end namespace

#endif /* _DLFL_MATERIAL_HH_ */
