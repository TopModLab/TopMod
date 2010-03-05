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

/* $Id: Vector2d.hh,v 1.6 2002/10/25 15:44:14 vinod Exp $ */

#ifndef _VECTOR_2D_HH_

#define _VECTOR_2D_HH_

// Class for a 2-D vector.
// Derived from BaseObject
// Assumes existence of classes Vector3d and Vector4d, which are 3-D and 4-D
// versions of this class.
// All the Vector classes are forward declared in Vector.hh, along with any
// function prototypes for subroutines using 2 different kinds of Vectors

// Functions which need the definition of the other Vector classes are defined in the
// source file, Vector2d.cc

#include "Vector.hh"

class Vector2d;
typedef Vector2d * Vector2dPtr;

class Vector2d : public BaseObject
{
  protected :

     double elem[2];                                   // 2 elements of the vector

        // Initialize the elements from a Vector3d or Vector4d
     void copyFrom(const Vector3d& vec);
     void copyFrom(const Vector4d& vec);

  public :

        // Default constructor
     Vector2d()
       : BaseObject()
       {
         elem[0] = elem[1] = 0.0;
       }

        // 1 argument constructor - intialize all elements with given value
     Vector2d(double val)
       : BaseObject()
       {
         elem[0] = elem[1] = val;
       }
     
        // 1 argument constructor - initialize with given array
        // Assumes array has atleast 2 elements
     Vector2d(double * arr)
       : BaseObject()
       {
         elem[0] = arr[0]; elem[1] = arr[1];
       }

        // 2 argument constructor
     Vector2d(double val1, double val2)
       : BaseObject()
       {
         elem[0] = val1; elem[1] = val2;
       }
     
        // Copy constructor
     Vector2d(const Vector2d& vec)
       : BaseObject(vec)
       {
         elem[0] = vec.elem[0]; elem[1] = vec.elem[1];
       }

        // Construct from a Vector3d - copies first 2 elements
     Vector2d(const Vector3d& vec)
       : BaseObject()
       {
         copyFrom(vec);
       }
     
        // Construct from a Vector4d - copies first 2 elements
     Vector2d(const Vector4d& vec)
       : BaseObject()
       {
         copyFrom(vec);
       }
     
        // Destructor - does nothing
     virtual ~Vector2d()
       {}

        // Assignment operator
     Vector2d& operator = (const Vector2d& vec)
       {
         elem[0] = vec.elem[0]; elem[1] = vec.elem[1];
         return (*this);
       }

        // Assignment from a scalar - both elements are set to the scalar value
     Vector2d& operator = (double scalar)
       {
         elem[0] = elem[1] = scalar;
         return (*this);
       }

        // Assignment from a Vector3d - copies first 2 elements
     Vector2d& operator = (const Vector3d& vec)
       {
         copyFrom(vec);
         return (*this);
       }

        // Assignment from a Vector4d - copies first 2 elements
     Vector2d& operator = (const Vector4d& vec)
       {
         copyFrom(vec);
         return (*this);
       }

        // Make a copy of the object
        // implement BaseObject class pure virtual function
     virtual BaseObjectPtr copy(void) const
       {
         Vector2dPtr vec = new Vector2d(*this);
         return vec;
       }

        // Set elements of vector to given values
     void set(double v1, double v2)
       {
         elem[0] = v1; elem[1] = v2;
       }

     void set(double val)
       {
         elem[0] = elem[1] = val;
       }
     
        // Set elements of vector to default values
     void reset(void)
       {
         set(0.0);
       }
     
        // Get the elements of vector into given values
     void get(double& v1, double& v2) const
       {
         v1 = elem[0]; v2 = elem[1];
       }
     
        // Get the vector as a C array (double *)
     const double * getCArray(void) const
       {
         return (const double *)elem;
       }
     
     double * getCArray(void)
       {
         return (double *)elem;
       }
     
        // Fill a 2D array with elements of the vector
     void fillArray(double arr[2]) const
       {
         arr[0] = elem[0]; arr[1] = elem[1];
       }

#ifndef __GNUG__
        /* GCC 3.0 and later have problems with having this and the [] operator */
        // Pointer cast operator
     operator const double * (void) const
       {
         return (const double *)elem;
       }

     operator double * (void)
       {
         return (double *)elem;
       }
#endif

        // Element access operator - doesn't check for valid indices
     double& operator [] (uint index)
       {
         return elem[index];
       }

     double operator [] (uint index) const
       {
         return elem[index];
       }

        // Arithmetic operators
     void operator += (const Vector2d& vec)            // Additive-assignment
       {
         elem[0] += vec.elem[0]; elem[1] += vec.elem[1];
       }

     void operator -= (const Vector2d& vec)            // Subtractive-assignment
       {
         elem[0] -= vec.elem[0]; elem[1] -= vec.elem[1];
       }

     void operator *= (double scalar)                  // Multiplicative-assignment (scalar)
       {
         elem[0] *= scalar; elem[1] *= scalar;
       }
     
     void operator /= (double scalar)                  // Divisive-assignment (scalar)
       {
         elem[0] /= scalar; elem[1] /= scalar;
       }
     
     Vector2d operator + (const Vector2d& vec) const   // Addition
       {
         Vector2d sum(*this);
         sum += vec;
         return sum;
       }

     Vector2d operator - (const Vector2d& vec) const   // Subtraction
       {
         Vector2d diff(*this);
         diff -= vec;
         return diff;
       }

     double operator * (const Vector2d& vec) const     // Multiplication - dot product
       {
         double dotprod = elem[0]*vec.elem[0] + elem[1]*vec.elem[1];
         return dotprod;
       }

        // Operators defined as friend's
     friend Vector2d operator - (const Vector2d& vec)  // Negation
       {
         Vector2d negv(-vec.elem[0],-vec.elem[1]);
         return negv;
       }

     friend Vector2d operator * (double scalar, const Vector2d& vec) // Pre-multiply by scalar
       {
         Vector2d prod(vec);

         prod *= scalar;
         return prod;
       }

     friend Vector2d operator * (const Vector2d& vec, double scalar) // Post-multiply by scalar
       {
         Vector2d prod(vec);

         prod *= scalar;
         return prod;
       }

     friend Vector2d operator / (const Vector2d& vec, double scalar) // Divide by scalar
       {
         Vector2d prod(vec);

         prod /= scalar;
         return prod;
       }

     friend Vector2d product(const Vector2d& vec1, const Vector2d& vec2) // Elem-by-elem product
       {
         Vector2d prod(vec1[0]*vec2[0],vec1[1]*vec2[1]);

         return prod;
       }
     
        // Boolean operators
     
     bool operator == (const Vector2d& vec) const      // Equality 
       {
         if ( (fabs(elem[0]-vec.elem[0]) > ZERO) ||
              (fabs(elem[1]-vec.elem[1]) > ZERO) )
            return false;
         return true;
       }

     bool operator != (const Vector2d& vec) const      // Inequality
       {
         return !( (*this) == vec );
       }

        // Other functions
     
     friend double normsqr(const Vector2d& vec)        // Square of norm of the vector
       {
         double nsq = sqr(vec.elem[0]) + sqr(vec.elem[1]);
         return nsq;
       }
     
     friend double norm(const Vector2d& vec)           // Norm of the vector
       {
         return sqrt(normsqr(vec));
       }

     friend double normalize(Vector2d& vec)            // Normalize. Returns previous norm
       {
         double n = norm(vec);
         if ( !areEqual(n,1.0) && (isNonZero(n) == true) ) vec /= n;
         return n;
       }

     friend Vector2d normalized(const Vector2d& vec)   // Return normalized vector
       {
         Vector2d nvec(vec);
         normalize(nvec);
         return nvec;
       }
     
     friend void swap(Vector2d& vec1, Vector2d& vec2)  // Swap elements of 2 Vector2ds
       {
         swap(vec1.elem[0],vec2.elem[0]);
         swap(vec1.elem[1],vec2.elem[1]);
       }

        // I/O Stream operators

     friend istream& operator >> (istream& i, Vector2d& vec) // Extraction operator
       {
            // Separating character can be anything
            // If only 1 number is there in the input stream before reaching EOL
            // both elements are set to that number
            // If EOL is reached before reading a number, vec is not changed
            // Caveat: Separator char need not be the same between each element
         removeWhiteSpace(i);
         if ( readTillFloat(i) )
            {
              i >> vec.elem[0];
              if ( readTillFloat(i) ) i >> vec.elem[1];
              else                    vec.elem[1] = vec.elem[0];
            }
         return i;
       }

     friend ostream& operator << (ostream& o, const Vector2d& vec) // Insertion operator
       {
            // User can set precision from 0 to 6
         o << setiosflags(ios::fixed) << setiosflags(ios::showpoint);

         int oldprec = o.precision();
         if ( oldprec < 0 ) o << setprecision(0);
         if ( oldprec > 6 ) o << setprecision(6);

         o << "["
           << vec.elem[0] << ' '
           << vec.elem[1]
           << "]";
         
         o << setprecision(oldprec);
         return o;
       }
};

#endif // #ifndef _VECTOR_2D_HH_

/*
  $Log: Vector2d.hh,v $
  Revision 1.6  2002/10/25 15:44:14  vinod
  Added getCArray() methods

  Revision 1.5  2002/10/17 01:34:05  vinod
  Added pointer cast operator

  Revision 1.4  2000/04/19 20:41:49  vinod
  Changed extraction operators

  Revision 1.3  2000/02/09 22:18:31  vinod
  includes now refer to local directory

  Revision 1.2  2000/02/09 22:15:07  vinod
  Code modifications to take care of file/class renames

  Revision 1.1  2000/02/09 22:01:24  vinod
  Renamed

  Revision 1.1  2000/02/09 21:21:43  vinod
  Moved into VecMat directory

  Revision 1.4  2000/02/04 21:51:06  vinod
  Added check for norm=1 in normalize

  Revision 1.3  2000/01/21 18:44:54  vinod
  Modified fillArray funcs to not use references

  Revision 1.2  1999/11/22 03:08:34  vinod
  Modified arg of fillArray functions to be a reference

  Revision 1.1  1999/10/18 05:31:35  vinod
  Vectors using statically allocated arrays

*/
