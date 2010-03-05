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

/* $Id: Color.hh,v 2.2 2000/04/20 04:16:11 vinod Exp $ */

#ifndef _COLOR_HH_

#define _COLOR_HH_

// Class to encapsulate RGB colors. Uses a Vector3d to store the components
// Has member data which are references to the 3 components
// Provides a function, clamp() to clamp the values to lie between 0 and 1
// Does not automatically do the clamping. User has to do it explicitly

#include "../Base/BaseObject.hh"
#include "../vecmat/Vector3d.hh"
#include "../vecmat/Vector4d.hh"
#include "../Base/Inlines.hh"

#ifdef __APPLE__
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#else
#include <GL/gl.h>
#include <GL/glu.h>
#endif

class RGBColor;
typedef RGBColor * RGBColorPtr;

class RGBColor : public BaseObject
{
  public :

        // NOTE: Order Dependency. The line defining color *MUST* be before the line
        // defining r,g,b
     Vector3d color;                                   // Vector used to store the color
     double &r,&g,&b;                                  // References to the 3 components

  public :

     void clamp(void)                                  // Clamp components to [0,1]
       {
         if ( r > 1.0 ) r = 1.0;
         else if ( r < 0.0 ) r = 0.0;

         if ( g > 1.0 ) g = 1.0;
         else if ( g < 0.0 ) g = 0.0;

         if ( b > 1.0 ) b = 1.0;
         else if ( b < 0.0 ) b = 0.0;
       }

  public :
     
        // Default constructor
     RGBColor()
       : BaseObject(), color(), r(color[0]), g(color[1]), b(color[2])
       {}

        // 1-arg constructor
     RGBColor(const Vector3d& col)
       : BaseObject(), color(col), r(color[0]), g(color[1]), b(color[2])
       {}

        // 1-arg constructor, gray value
     RGBColor(double gray)
       : BaseObject(), color(gray), r(color[0]), g(color[1]), b(color[2])
       {}
     
        // 3-arg constructor
     RGBColor(double red, double green, double blue)
       : BaseObject(), color(red,green,blue), r(color[0]), g(color[1]), b(color[2])
       {}
     
        // Copy constructor
     RGBColor(const RGBColor& col)
       : BaseObject(col), color(col.color), r(color[0]), g(color[1]), b(color[2])
       {}

        // Destructor
     ~RGBColor()
       {}

        // Assignment operator
     RGBColor& operator = (const RGBColor& col)
       {
         BaseObject :: operator = (col);
         color = col.color;
         return (*this);
       }

        // Assignment from a Vector3d
     RGBColor& operator = (const Vector3d& col)
       {
         color = col;
         return (*this);
       }

        // Assignment from a scalar - gray value
     RGBColor& operator = (double gray)
       {
         color = gray;
         return (*this);
       }

        // Make a copy
     BaseObjectPtr copy(void) const
       {
         RGBColorPtr newcol = new RGBColor(*this);
         return newcol;
       }

     void set(double red, double green, double blue)
       {
         color.set(red,green,blue);
       }

        // Comparison
     bool operator == (const RGBColor& c) const
       {
         if ( Abs(r-c.r) < 1.0e-3 &&
              Abs(g-c.g) < 1.0e-3 &&
              Abs(b-c.b) < 1.0e-3 ) return true;
         return false;
       }

     bool operator != (const RGBColor& c) const
       {
         return !(operator == (c));
       }

        // Arithmetic-assignment operators
        // These operators don't clamp the values. User has to do them explicitly
     void operator += (const RGBColor& c)
       {
         color += c.color;
       }

     void operator -= (const RGBColor& c)
       {
         color -= c.color;
       }

     void operator *= (double scalar)
       {
         color *= scalar;
       }

     void operator /= (double scalar)
       {
         color /= scalar;
       }
     
        // Arithmetic operators
     RGBColor operator + (const RGBColor& c) const
       {
         RGBColor sum(*this); sum += c;
         return sum;
       }
       
     RGBColor operator - (const RGBColor& c) const
       {
         RGBColor diff(*this); diff -= c;
         return diff;
       }
       
     RGBColor operator * (double scalar) const
       {
         RGBColor prod(*this); prod *= scalar;
         return prod;
       }

     friend RGBColor operator * (double scalar, const RGBColor& col)
       {
         RGBColor prod(col); prod *= scalar;
         return prod;
       }

     friend RGBColor product(const RGBColor& col1, const RGBColor& col2)
       {
         RGBColor prod(product(col1.color,col2.color));
         return prod;
       }
     
     RGBColor operator / (double scalar) const
       {
         RGBColor quot(*this); quot /= scalar;
         return quot;
       }

        // Luminanace = sum of components
     friend double luminance(const RGBColor& c)
       {
         return (c.r+c.g+c.b);
       }

        // I/O stream operators
     friend ostream& operator << (ostream& o, const RGBColor& c)
       {
         o << "(" << c.r << "," << c.g << "," << c.b << ")";
         return o;
       }

     friend istream& operator >> (istream& i, RGBColor& c)
       {
         i >> c.color;
         return i;
       }

        // Overload OpenGL color function
     friend void glColor(const RGBColor& c)
       {
         glColor3d(c.r,c.g,c.b);
       }
};

#endif /* #ifndef _COLOR_HH_ */


/*
  $Log: Color.hh,v $
  Revision 2.2  2000/04/20 04:16:11  vinod
  bug fix in extraction operator

  Revision 2.1  2000/04/20 03:58:59  vinod
  Added set func, comparison operators, changed extraction operator

  Revision 2.0  2000/03/12 00:49:30  vinod
  Major revision number change

  Revision 1.2  2000/03/03 18:59:41  vinod
  None of functions/ctors/ops now automatically clamp

  Revision 1.1  2000/03/03 18:55:19  vinod
  Color class, moved from sep dir

  Revision 1.10  2000/02/29 06:33:54  vinod
  Fixed const correctness for operators

  Revision 1.9  2000/02/28 22:06:07  vinod
  Arithmetic-assignment ops don't clamp, added product fn, Made clamp() public

  Revision 1.8  2000/02/17 05:53:21  vinod
  Uses new include file names

  Revision 1.7  2000/02/11 09:43:41  vinod
  Bug fix: g and b were not references, because of incorrect declaration

  Revision 1.6  2000/02/11 09:38:09  vinod
  Moved clamp() before constructors, some of which now call it

  Revision 1.5  2000/02/08 05:17:06  vinod
  Missing GL include. Fixed typos, minor bugs

  Revision 1.4  2000/02/08 04:00:07  vinod
  Added assignment from Vector3d andd double operators

  Revision 1.3  2000/02/08 03:57:29  vinod
  Added gray-value ctor, pre-mult by scalar

  Revision 1.2  2000/02/07 22:14:53  vinod
  Added 3-arg constructor

  Revision 1.1.1.1  2000/02/07 22:08:50  vinod
  Color classes

*/
