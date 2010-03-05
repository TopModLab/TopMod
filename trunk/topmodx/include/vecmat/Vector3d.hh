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

/* $Id: Vector3d.hh,v 1.6 2002/10/25 15:44:14 vinod Exp $ */

#ifndef _VECTOR_3D_HH_

#define _VECTOR_3D_HH_

// Class for a 3-D vector.
// Derived from BaseObject
// Assumes existence of classes Vector2d and Vector4d, which are 2-D and 4-D
// versions of this class.
// All the Vector classes are forward declared in Vector.hh, along with any
// function prototypes for subroutines using 2 different kinds of Vectors

// Functions which need the definition of the other Vector classes are defined in the
// source file, Vector3d.cc

#include "Vector.hh"

class Vector3d;
typedef Vector3d * Vector3dPtr;

class Vector3d : public BaseObject
{
  protected :

     double elem[3];                                   // 3 elements of the vector

        // Initialize the elements from a Vector2d or Vector4d
     void copyFrom(const Vector2d& vec);
     void copyFrom(const Vector4d& vec);

  public :

        // Default constructor
     Vector3d()
       : BaseObject()
       {
         elem[0] = elem[1] = elem[2] = 0.0;
       }

        // 1 argument constructor - intialize all elements with given value
     Vector3d(double val)
       : BaseObject()
       {
         elem[0] = elem[1] = elem[2] = val;
       }
     
        // 1 argument constructor - initialize with given array
        // Assumes array has atleast 3 elements
     Vector3d(double * arr)
       : BaseObject()
       {
         elem[0] = arr[0]; elem[1] = arr[1]; elem[2] = arr[2];
       }

        // 3 argument constructor
     Vector3d(double val1, double val2, double val3=0.0)
       : BaseObject()
       {
         elem[0] = val1; elem[1] = val2; elem[2] = val3;
       }
     
        // Copy constructor
     Vector3d(const Vector3d& vec)
       : BaseObject(vec)
       {
         elem[0] = vec.elem[0]; elem[1] = vec.elem[1]; elem[2] = vec.elem[2];
       }

        // Construct from a Vector2d - third element is set to 0
     Vector3d(const Vector2d& vec)
       : BaseObject()
       {
         copyFrom(vec);
       }
     
        // Construct from a Vector4d - copies first 3 elements
     Vector3d(const Vector4d& vec)
       : BaseObject()
       {
         copyFrom(vec);
       }
     
        // Destructor - does nothing
     virtual ~Vector3d()
       {}

        // Assignment operator
     Vector3d& operator = (const Vector3d& vec)
       {
         elem[0] = vec.elem[0]; elem[1] = vec.elem[1]; elem[2] = vec.elem[2];
         return (*this);
       }

        // Assignment from a scalar - all elements are set to the scalar value
     Vector3d& operator = (double scalar)
       {
         elem[0] = elem[1] = elem[2] = scalar;
         return (*this);
       }

        // Assignment from a Vector2d - third element set to 0
     Vector3d& operator = (const Vector2d& vec)
       {
         copyFrom(vec);
         return (*this);
       }

        // Assignment from a Vector4d - copies first 3 elements
     Vector3d& operator = (const Vector4d& vec)
       {
         copyFrom(vec);
         return (*this);
       }

        // Make a copy of the object
        // implement BaseObject class pure virtual function
     virtual BaseObjectPtr copy(void) const
       {
         Vector3dPtr vec = new Vector3d(*this);
         return vec;
       }

        // Set elements of vector to given values
     void set(double v1, double v2, double v3)
       {
         elem[0] = v1; elem[1] = v2; elem[2] = v3;
       }

     void set(double val)
       {
         elem[0] = elem[1] = elem[2] = val;
       }
     
        // Set elements of vector to default values
     void reset(void)
       {
         set(0.0);
       }
     
        // Get the elements of vector into given values
     void get(double& v1, double& v2, double& v3) const
       {
         v1 = elem[0]; v2 = elem[1]; v3 = elem[2];
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
     void fillArray(double arr[3]) const
       {
         arr[0] = elem[0]; arr[1] = elem[1]; arr[2] = elem[2];
       }

			double lengthsqr(){
				return elem[0]*elem[0] + elem[1]*elem[1] + elem[2]*elem[2];
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
     void operator += (const Vector3d& vec)            // Additive-assignment
       {
         elem[0] += vec.elem[0]; elem[1] += vec.elem[1]; elem[2] += vec.elem[2];
       }

     void operator -= (const Vector3d& vec)            // Subtractive-assignment
       {
         elem[0] -= vec.elem[0]; elem[1] -= vec.elem[1]; elem[2] -= vec.elem[2];
       }

     void operator *= (double scalar)                  // Multiplicative-assignment (scalar)
       {
         elem[0] *= scalar; elem[1] *= scalar; elem[2] *= scalar;
       }
     
     void operator /= (double scalar)                  // Divisive-assignment (scalar)
       {
         elem[0] /= scalar; elem[1] /= scalar; elem[2] /= scalar;
       }
     
     Vector3d operator + (const Vector3d& vec) const   // Addition
       {
         Vector3d sum(*this);
         sum += vec;
         return sum;
       }

     Vector3d operator - (const Vector3d& vec) const   // Subtraction
       {
         Vector3d diff(*this);
         diff -= vec;
         return diff;
       }

     double operator * (const Vector3d& vec) const     // Multiplication - dot product
       {
         double dotprod = elem[0]*vec.elem[0] + elem[1]*vec.elem[1] + elem[2]*vec.elem[2];
         return dotprod;
       }

     Vector3d operator % (const Vector3d& vec) const   // Multiplication - cross product
       {
         Vector3d crossp;

         crossp.elem[0] = elem[1]*vec.elem[2] - elem[2]*vec.elem[1];
         crossp.elem[1] = elem[2]*vec.elem[0] - elem[0]*vec.elem[2];
         crossp.elem[2] = elem[0]*vec.elem[1] - elem[1]*vec.elem[0];

         return crossp;
       }
     
        // Operators defined as friend's
     friend Vector3d operator - (const Vector3d& vec)  // Negation
       {
         Vector3d negv(-vec.elem[0],-vec.elem[1],-vec.elem[2]);
         return negv;
       }

     friend Vector3d operator * (double scalar, const Vector3d& vec) // Pre-multiply by scalar
       {
         Vector3d prod(vec);

         prod *= scalar;
         return prod;
       }

     friend Vector3d operator * (const Vector3d& vec, double scalar) // Post-multiply by scalar
       {
         Vector3d prod(vec);

         prod *= scalar;
         return prod;
       }

     friend Vector3d operator / (const Vector3d& vec, double scalar) // Divide by scalar
       {
         Vector3d prod(vec);

         prod /= scalar;
         return prod;
       }

     friend Vector3d product(const Vector3d& vec1, const Vector3d& vec2) // Elem-by-elem product
       {
         Vector3d prod(vec1[0]*vec2[0],vec1[1]*vec2[1],vec1[2]*vec2[2]);

         return prod;
       }
     
        // Boolean operators
     
     bool operator == (const Vector3d& vec) const      // Equality 
       {
         if ( (fabs(elem[0]-vec.elem[0]) > ZERO) ||
              (fabs(elem[1]-vec.elem[1]) > ZERO) ||
              (fabs(elem[2]-vec.elem[2]) > ZERO) )
            return false;
         return true;
       }

     bool operator != (const Vector3d& vec) const      // Inequality
       {
         return !( (*this) == vec );
       }

        // Other functions
     
     friend double normsqr(const Vector3d& vec)        // Square of norm of the vector
       {
         double nsq = sqr(vec.elem[0]) + sqr(vec.elem[1]) + sqr(vec.elem[2]);
         return nsq;
       }
     
     friend double norm(const Vector3d& vec)           // Norm of the vector
       {
         return sqrt(normsqr(vec));
       }

     friend double normalize(Vector3d& vec)            // Normalize. Returns previous norm
       {
         double n = norm(vec);
         if ( !areEqual(n,1.0) && (isNonZero(n) == true) ) vec /= n;
         return n;
       }

     friend Vector3d normalized(const Vector3d& vec)   // Return normalized vector
       {
         Vector3d nvec(vec);
         normalize(nvec);
         return nvec;
       }
     
     friend void swap(Vector3d& vec1, Vector3d& vec2)  // Swap elements of 2 Vector3ds
       {
         swap(vec1.elem[0],vec2.elem[0]);
         swap(vec1.elem[1],vec2.elem[1]);
         swap(vec1.elem[2],vec2.elem[2]);
       }

        // I/O Stream operators

     friend istream& operator >> (istream& i, Vector3d& vec) // Extraction operator
       {
            // Separating character can be anything
            // If only 1 number is there in the input stream before reaching EOL
            // both elements are set to that number.
            // If only 2 numbers are there, third element is set to 0
            // If EOL is reached before reading a number, vec is not changed
            // Caveat: Separator char need not be the same between each element
         removeWhiteSpace(i);
         if ( readTillFloat(i) )
            {
              i >> vec.elem[0];
              if ( readTillFloat(i) )
                 {
                   i >> vec.elem[1];
                   if ( readTillFloat(i) ) i >> vec.elem[2];
                   else                    vec.elem[2] = 0.0;
                 }
              else
                 vec.elem[2] = vec.elem[1] = vec.elem[0];
            }
         return i;
       }

     friend ostream& operator << (ostream& o, const Vector3d& vec) // Insertion operator
       {
            // User can set precision from 0 to 6
         o << setiosflags(ios::fixed) << setiosflags(ios::showpoint);

         int oldprec = o.precision();
         if ( oldprec < 0 ) o << setprecision(0);
         if ( oldprec > 6 ) o << setprecision(6);

         o << "["
           << vec.elem[0] << " "
           << vec.elem[1] << " "
           << vec.elem[2]
           << "]";
         
         o << setprecision(oldprec);
         return o;
       }
};

#endif // #ifndef _VECTOR_3D_HH_

/*
  $Log: Vector3d.hh,v $
  Revision 1.6  2002/10/25 15:44:14  vinod
  Added getCArray() methods

  Revision 1.5  2002/10/17 01:34:05  vinod
  Added pointer cast operator

  Revision 1.4  2000/04/19 20:41:49  vinod
  Changed extraction operators

  Revision 1.3  2000/02/09 22:18:31  vinod
  includes now refer to local directory

  Revision 1.2  2000/02/09 22:15:08  vinod
  Code modifications to take care of file/class renames

  Revision 1.1  2000/02/09 22:01:24  vinod
  Renamed

  Revision 1.1  2000/02/09 21:21:44  vinod
  Moved into VecMat directory

  Revision 1.5  2000/02/04 21:51:06  vinod
  Added check for norm=1 in normalize

  Revision 1.4  2000/01/21 22:12:16  vinod
  Added implicit 2 arg constructor

  Revision 1.3  2000/01/21 18:44:54  vinod
  Modified fillArray funcs to not use references

  Revision 1.2  1999/11/22 03:08:35  vinod
  Modified arg of fillArray functions to be a reference

  Revision 1.1  1999/10/18 05:31:36  vinod
  Vectors using statically allocated arrays

*/
