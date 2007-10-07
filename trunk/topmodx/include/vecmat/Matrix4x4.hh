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

/* $Id: Matrix4x4.hh,v 1.3 2003/04/12 01:18:10 vinod Exp $ */

#ifndef _MATRIX4X4_HH_

#define _MATRIX4X4_HH_

// Class for a 4x4 matrix. Built from Vector4d
// Row-major form is used. (ie) each row of the matrix
// is a Vector4d. This makes inversion easier, since elementary
// row operations are simplified

#include "../Base/BaseObject.hh"
#include "Vector3d.hh"
#include "Vector4d.hh"
#include "Matrix3x3.hh"

class Matrix4x4;
typedef Matrix4x4 * Matrix4x4Ptr;

// Typedefs for backward compatibility with older name
typedef Matrix4x4 Matrix4_4;
typedef Matrix4_4 * Matrix4_4Ptr;

class Matrix4x4 : public BaseObject
{
  protected :

     Vector4d   row[4];                                // 4 rows of the matrix

        // Copy values from another matrix
     void copyFrom(const Matrix4x4& mat)
       {
         for (int i=0; i < 4; ++i)
            row[i] = mat.row[i];
       }

        // Copy from a 3x3 matrix. 4th row and column elements are all set to 0
     void copyFrom(const Matrix3_3& mat3)
       {
         for (int i=0; i < 3; ++i)
            row[i] = mat3[i];
       }
     
  public :

        // Default constructor - creates an identity matrix
     Matrix4x4()
       : BaseObject()
       {
         row[0].set(1.0,0.0,0.0,0.0);
         row[1].set(0.0,1.0,0.0,0.0);
         row[2].set(0.0,0.0,1.0,0.0);
         row[3].set(0.0,0.0,0.0,1.0);
       }

        // 1 argument constructor - from scalar, set all elements to given value
     Matrix4x4(double scalar)
       : BaseObject()
       {
         row[0] = scalar; row[1] = scalar; row[2] = scalar; row[3] = scalar;
       }
     
        // 4 argument constructor - from 4 Vector4ds
     Matrix4x4(const Vector4d& r0, const Vector4d& r1, const Vector4d& r2, const Vector4d& r3)
       : BaseObject()
       {
         row[0] = r0; row[1] = r1; row[2] = r2; row[3] = r3;
       }

        // Copy constructor
     Matrix4x4(const Matrix4x4& mat)
       : BaseObject()
       {
         copyFrom(mat);
       }

        // Constructor from a 3x3 matrix
     Matrix4x4(const Matrix3_3& mat3)
       : BaseObject()
       {
         copyFrom(mat3);
       }
     
        // Destructor
     virtual ~Matrix4x4()
       {}

        // Assignment operator
     Matrix4x4& operator = (const Matrix4x4& mat)
       {
         copyFrom(mat);
         return (*this);
       }

        // Assignment from a Matrix3_3
     Matrix4x4& operator = (const Matrix3_3& mat3)
       {
         copyFrom(mat3);
         return (*this);
       }
     
        // Assignment from a scalar
     void operator = (double scalar)
       {
         row[0] = scalar; row[1] = scalar; row[2] = scalar; row[3] = scalar;
       }

        // Set the rows of the matrix to the given Vector4ds
     void set(const Vector4d& r0, const Vector4d& r1, const Vector4d& r2, const Vector4d& r3)
       {
         row[0] = r0; row[1] = r1; row[2] = r2; row[3] = r3;
       }

     void set(double scalar)
       {
         row[0] = scalar; row[1] = scalar; row[2] = scalar; row[3] = scalar;
       }
     
        // Reset the matrix to it's default state - identity
     void reset(void)
       {
         row[0].set(1.0,0.0,0.0,0.0);
         row[1].set(0.0,1.0,0.0,0.0);
         row[2].set(0.0,0.0,1.0,0.0);
         row[3].set(0.0,0.0,0.0,1.0);
       }
     
        // Make a copy of the object
        // implement BaseObject class pure virtual function
     virtual BaseObjectPtr copy(void) const
       {
         Matrix4x4Ptr mat = new Matrix4x4(*this);
         return mat;
       }

        // Access a row of the matrix - no range checks
     Vector4d& operator [] (uint index)
       {
         return row[index];
       }

        // Access a row of the matrix - no range checks : const version
     const Vector4d& operator [] (uint index) const
       {
         return row[index];
       }

        // Class member functions which return the identity matrix
     static Matrix4x4 identity(void)
       {
         Matrix4x4 imat;  // Default constructor creates an identity matrix
         return imat;
       }
     
     static Matrix4x4 I(void)
       {
         Matrix4x4 imat;  // Default constructor creates an identity matrix
         return imat;
       }
     
        //--- Arithmetic assignment operators ---//

     void operator += (const Matrix4x4& mat)
       {
         for (int i=0; i < 4; ++i)
            row[i] += mat.row[i];
       }
     
     void operator -= (const Matrix4x4& mat)
       {
         for (int i=0; i < 4; ++i)
            row[i] -= mat.row[i];
       }
     
     void operator *= (double scalar)
       {
         for (int i=0; i < 4; ++i)
            row[i] *= scalar;
       }
     
     void operator /= (double scalar)
       {
         for (int i=0; i < 4; ++i)
            row[i] /= scalar;
       }

        //--- Special functions - inverse, transpose, etc. ---//

     void transpose(void)
       {
         for (int i=0; i < 4; ++i)
            for (int j=i+1; j < 4; ++j)
               swap(row[i][j],row[j][i]);
       }
     
        // Find the transpose of a given matrix
     friend Matrix4x4 transpose(const Matrix4x4& mat)
       {
         Matrix4x4 trans(mat); trans.transpose();
         return trans;
       }

        // Find the 3x3 sub-matrix which is the co-factor for the given element
     Matrix3_3 cofactor(uint r, uint c) const;

     friend double determinant(const Matrix4x4& mat)
       {
         double det = 0.0;

         for (int i=0; i < 4; ++i)
            det += mat[0][i] * cofsign(0,i) * determinant(mat.cofactor(0,i));
         return det;
       }

        // Invert the matrix. Using elementary row operations
     void invert(void);
     
        // Find the inverse of a given matrix
        // Using elementary row operations
     friend Matrix4x4 inverse(const Matrix4x4& mat)
       {
         Matrix4x4 inv(mat); inv.invert();
         return inv;
       }
         
        //--- Arithmetic operators, implemented as friend functions ---//

        // Negation
     friend Matrix4x4 operator - (const Matrix4x4& mat)
       {
         Matrix4x4 neg;

         for (int i=0; i < 4; ++i)
            neg.row[i] = -mat.row[i];

         return neg;
       }
     
     friend Matrix4x4 operator + (const Matrix4x4& mat1, const Matrix4x4& mat2)
       {
         Matrix4x4 sum(mat1);
         sum += mat2;
         return sum;
       }
     
     friend Matrix4x4 operator - (const Matrix4x4& mat1, const Matrix4x4& mat2)
       {
         Matrix4x4 diff(mat1);
         diff -= mat2;
         return diff;
       }

        // Post-multiplication by a scalar
     friend Matrix4x4 operator * (const Matrix4x4& mat, double scalar)
       {
         Matrix4x4 prod(mat);
         prod *= scalar;
         return prod;
       }
     
        // Pre-multiplication by a scalar
     friend Matrix4x4 operator * (double scalar, const Matrix4x4& mat)
       {
         Matrix4x4 prod(mat);
         prod *= scalar;
         return prod;
       }

        // Division by a scalar
     friend Matrix4x4 operator / (const Matrix4x4& mat, double scalar)
       {
         Matrix4x4 quot(mat);
         quot /= scalar;
         return quot;
       }

        // Multiplication of 2 matrices - outer product
     friend Matrix4x4 operator * (const Matrix4x4& mat1, const Matrix4x4& mat2);
     
        // Element-by-element multiplication of 2 matrices
     friend Matrix4x4 product(const Matrix4x4& mat1, const Matrix4x4& mat2)
       {
         Matrix4x4 prod;

         prod.row[0] = product(mat1[0],mat2[0]);
         prod.row[1] = product(mat1[1],mat2[1]);
         prod.row[2] = product(mat1[2],mat2[2]);
         prod.row[3] = product(mat1[3],mat2[3]);
         
         return prod;
       }
     
        // Post-multiplication by a Vector4d. Vector is assumed to be a column vector
     friend Vector4d operator * (const Matrix4x4& mat, const Vector4d& vec)
       {
         Vector4d prod;

         prod.set( mat.row[0]*vec, mat.row[1]*vec, mat.row[2]*vec, mat.row[3]*vec );
         return prod;
       }

			friend Vector3d operator*( const Matrix4x4& mat, const Vector3d& vec ) {
				Vector3d prod3;
				Vector4d prod4, vec4 = vec; vec4[3] = 1.0;
				prod4 = mat * vec4;
				prod3 = prod4;
				return prod3;
			}
     
        // Pre-multiplication by a Vector4d. Vector is assumed to be a row vector
     friend Vector4d operator * (const Vector4d& vec, const Matrix4x4& mat);

        // Multiplication of two Vector4ds to produce a Matrix4x4 - outer product
        // or tensor product of two Vectors.
        // Same as multiplying row vector (v1) with column vector (v2)
     friend Matrix4x4 operator ^ (const Vector4d& v1, const Vector4d& v2)
       {
         return Matrix4x4( v1[0]*v2, v1[1]*v2, v1[2]*v2, v1[3]*v2 );
       }
     
     friend void swap(Matrix4x4& mat1, Matrix4x4& mat2)
       {
         for (int i=0; i < 4; ++i)
            swap(mat1.row[i],mat2.row[i]);
       }

        // Multiply self with another matrix. Simply calls above defined friend function
     void operator *= (const Matrix4x4& mat)
       {
         Matrix4x4 prod = (*this) * mat;
         *this = prod;
       }

     void fillArrayRowMajor(double array[16]) const
       {
            // Fill an array with contents of the matrix
            // Row - major form -> Row 1 == { array[0], array[1], array[2], array[3] }
         int index=0;
         for (int i=0; i < 4; ++i)
            {
              row[i].get(array[index],array[index+1],array[index+2],array[index+3]);
              index += 4;
            }
       }

     void fillArrayColumnMajor(double array[16]) const
       {
            // Fill an array with contents of the matrix
            // Row - major form -> Column 1 == { array[0], array[1], array[2], array[3] }
         int index=0;
         for (int i=0; i < 4; ++i)
            {
              row[i].get(array[index],array[index+4],array[index+8],array[index+12]);
              index += 1;
            }
       }
            
     friend ostream& operator << (ostream& o, const Matrix4x4& mat)
       {
         o << "[ " << mat.row[0] << "," << endl
           << "  " << mat.row[1] << "," << endl
           << "  " << mat.row[2] << "," << endl
           << "  " << mat.row[3] << " ]" << endl;
         return o;
       }

     friend istream& operator >> (istream& i, Matrix4x4& mat)
       {
         char rowsep, dummy;
         i >> dummy >> mat.row[0] >> rowsep
           >> mat.row[1] >> rowsep
           >> mat.row[2] >> rowsep
           >> mat.row[3] >> dummy;
         return i;
       }
};

#endif /* #ifndef _MATRIX4X4_HH_ */


/*
  $Log: Matrix4x4.hh,v $
  Revision 1.3  2003/04/12 01:18:10  vinod
  MAJOR BUG FIX! Loop for determinant was missing last element!

  Revision 1.2  2000/02/09 22:15:06  vinod
  Code modifications to take care of file/class renames

  Revision 1.1  2000/02/09 22:01:23  vinod
  Renamed

  Revision 1.1  2000/02/09 21:21:43  vinod
  Moved into VecMat directory

  Revision 1.6  2000/02/09 07:03:29  vinod
  Moved invert fns, etc. and friend fns defined outside class to source file

  Revision 1.5  2000/01/27 00:10:22  vinod
  Added inplace transpose and invert functions

  Revision 1.4  2000/01/21 18:44:05  vinod
  Minor access problem in converting from Matrix3_3.
  Changed fillArray functions to use double instead of reference (not allowed)

  Revision 1.3  1999/11/22 03:08:03  vinod
  Added fillArray functions

  Revision 1.2  1999/10/26 05:00:38  vinod
  Added conversion from Matrix3_3

  Revision 1.1  1999/10/18 05:32:02  vinod
  Matrix classes using Vectors

*/
