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

/* $Id: Matrix3x3.hh,v 1.2 2000/02/09 22:15:06 vinod Exp $ */

#ifndef _MATRIX3X3_HH_

#define _MATRIX3X3_HH_

// Class for a 3x3 matrix. Built from Vector3d
// Row-major form is used. (ie) each row of the matrix
// is a Vector3d. This makes inversion easier, since elementary
// row operations are simplified

#include "../Base/BaseObject.hh"
#include "Vector3d.hh"

class Matrix3x3;
typedef Matrix3x3 * Matrix3x3Ptr;

// Typedefs for backward compatibility with older name
typedef Matrix3x3 Matrix3_3;
typedef Matrix3_3 * Matrix3_3Ptr;

class Matrix3x3 : public BaseObject
{
  protected :

     Vector3d   row[3];                                // 3 rows of the matrix

        // Copy values from another matrix
     void copyFrom(const Matrix3x3& mat)
       {
         for (int i=0; i < 3; ++i)
            row[i] = mat.row[i];
       }

  public :

        // Default constructor - creates an identity matrix
     Matrix3x3()
       : BaseObject()
       {
         row[0].set(1.0,0.0,0.0);
         row[1].set(0.0,1.0,0.0);
         row[2].set(0.0,0.0,1.0);
       }

        // 1 argument constructor - from scalar, set all elements to given value
     Matrix3x3(double scalar)
       : BaseObject()
       {
         row[0] = scalar; row[1] = scalar; row[2] = scalar;
       }
     
        // 3 argument constructor - from 3 Vector3ds
     Matrix3x3(const Vector3d& r0, const Vector3d& r1, const Vector3d& r2)
       : BaseObject()
       {
         row[0] = r0; row[1] = r1; row[2] = r2;
       }

        // Copy constructor
     Matrix3x3(const Matrix3x3& mat)
       : BaseObject()
       {
         copyFrom(mat);
       }

        // Destructor
     virtual ~Matrix3x3()
       {}

        // Assignment operator
     Matrix3x3& operator = (const Matrix3x3& mat)
       {
         copyFrom(mat);
         return (*this);
       }

        // Assignment from a scalar
     void operator = (double scalar)
       {
         row[0] = scalar; row[1] = scalar; row[2] = scalar;
       }

        // Set the rows of the matrix to the given Vector3ds
     void set(const Vector3d& r0, const Vector3d& r1, const Vector3d& r2)
       {
         row[0] = r0; row[1] = r1; row[2] = r2;
       }

     void set(double scalar)
       {
         row[0] = scalar; row[1] = scalar; row[2] = scalar;
       }
     
        // Reset the matrix to it's default state - identity
     void reset(void)
       {
         row[0].set(1.0,0.0,0.0);
         row[1].set(0.0,1.0,0.0);
         row[2].set(0.0,0.0,1.0);
       }
     
        // Make a copy of the object
        // implement BaseObject class pure virtual function
     virtual BaseObjectPtr copy(void) const
       {
         Matrix3x3Ptr mat = new Matrix3x3(*this);
         return mat;
       }

        // Access a row of the matrix - no range checks
     Vector3d& operator [] (uint index)
       {
         return row[index];
       }

        // Access a row of the matrix - no range checks : const version
     const Vector3d& operator [] (uint index) const
       {
         return row[index];
       }

        // Class member functions which return the identity matrix
     static Matrix3x3 identity(void)
       {
         Matrix3x3 imat;  // Default constructor creates an identity matrix
         return imat;
       }
     
     static Matrix3x3 I(void)
       {
         Matrix3x3 imat;  // Default constructor creates an identity matrix
         return imat;
       }
     
        //--- Arithmetic assignment operators ---//

     void operator += (const Matrix3x3& mat)
       {
         for (int i=0; i < 3; ++i)
            row[i] += mat.row[i];
       }
     
     void operator -= (const Matrix3x3& mat)
       {
         for (int i=0; i < 3; ++i)
            row[i] -= mat.row[i];
       }
     
     void operator *= (double scalar)
       {
         for (int i=0; i < 3; ++i)
            row[i] *= scalar;
       }
     
     void operator /= (double scalar)
       {
         for (int i=0; i < 3; ++i)
            row[i] /= scalar;
       }

        //--- Special functions - inverse, transpose, etc. ---//

        // Find the transpose of a given matrix
     void transpose(void)
       {
         for (int i=0; i < 3; ++i)
            for (int j=i+1; j < 3; ++j)
               swap(row[i][j],row[j][i]);
       }

     friend Matrix3x3 transpose(const Matrix3x3& mat)
       {
         Matrix3x3 trans(mat); trans.transpose();
         return trans;
       }

     friend double determinant(const Matrix3x3& mat)
       {
         double det = 0.0;

         det =   mat[0][0] * mat[1][1] * mat[2][2] 
               + mat[0][1] * mat[1][2] * mat[2][0] 
               + mat[0][2] * mat[1][0] * mat[2][1] 
               - mat[2][0] * mat[1][1] * mat[0][2] 
               - mat[2][1] * mat[1][2] * mat[0][0] 
               - mat[2][2] * mat[1][0] * mat[0][1];
         return det;
       }

        // Invert the matrix, using elementary row operations
     void invert(void);
     
        // Find the inverse of a given matrix
        // Using elementary row operations
     friend Matrix3x3 inverse(const Matrix3x3& mat)
       {
         Matrix3x3 inv(mat); inv.invert();
         return inv;
       }
         
        //--- Arithmetic operators, implemented as friend functions ---//

        // Negation
     friend Matrix3x3 operator - (const Matrix3x3& mat)
       {
         Matrix3x3 neg;

         for (int i=0; i < 3; ++i)
            neg.row[i] = -mat.row[i];

         return neg;
       }
     
     friend Matrix3x3 operator + (const Matrix3x3& mat1, const Matrix3x3& mat2)
       {
         Matrix3x3 sum(mat1);
         sum += mat2;
         return sum;
       }
     
     friend Matrix3x3 operator - (const Matrix3x3& mat1, const Matrix3x3& mat2)
       {
         Matrix3x3 diff(mat1);
         diff -= mat2;
         return diff;
       }

        // Post-multiplication by a scalar
     friend Matrix3x3 operator * (const Matrix3x3& mat, double scalar)
       {
         Matrix3x3 prod(mat);
         prod *= scalar;
         return prod;
       }
     
        // Pre-multiplication by a scalar
     friend Matrix3x3 operator * (double scalar, const Matrix3x3& mat)
       {
         Matrix3x3 prod(mat);
         prod *= scalar;
         return prod;
       }

        // Division by a scalar
     friend Matrix3x3 operator / (const Matrix3x3& mat, double scalar)
       {
         Matrix3x3 quot(mat);
         quot /= scalar;
         return quot;
       }

        // Multiplication of 2 matrices - outer product
     friend Matrix3x3 operator * (const Matrix3x3& mat1, const Matrix3x3& mat2);

        // Element-by-element multiplication of 2 matrices
     friend Matrix3x3 product(const Matrix3x3& mat1, const Matrix3x3& mat2)
       {
         Matrix3x3 prod;

         prod.row[0] = product(mat1[0],mat2[0]);
         prod.row[1] = product(mat1[1],mat2[1]);
         prod.row[2] = product(mat1[2],mat2[2]);

         return prod;
       }
     
        // Post-multiplication by a Vector3d. Vector is assumed to be a column vector
     friend Vector3d operator * (const Matrix3x3& mat, const Vector3d& vec)
       {
         Vector3d prod;

         prod.set( mat.row[0]*vec, mat.row[1]*vec, mat.row[2]*vec );
         return prod;
       }
     
        // Pre-multiplication by a Vector3d. Vector is assumed to be a row vector
     friend Vector3d operator * (const Vector3d& vec, const Matrix3x3& mat);

        // Multiplication of two Vector3ds to produce a Matrix3x3 - outer product
        // or tensor product of two Vectors.
        // Same as multiplying row vector (v1) with column vector (v2)
     friend Matrix3x3 operator ^ (const Vector3d& v1, const Vector3d& v2)
       {
         return Matrix3x3( v1[0]*v2, v1[1]*v2, v1[2]*v2 );
       }
     
     friend void swap(Matrix3x3& mat1, Matrix3x3& mat2)
       {
         for (int i=0; i < 3; ++i)
            swap(mat1.row[i],mat2.row[i]);
       }

        // Multiply self with another matrix. Simply calls above defined friend function
     void operator *= (const Matrix3x3& mat)
       {
         Matrix3x3 prod = (*this) * mat;
         *this = prod;
       }
     
     void fillArrayRowMajor(double array[9]) const
       {
            // Fill an array with contents of the matrix
            // Row - major form -> Row 1 == { array[0], array[1], array[2] }
         int index=0;
         for (int i=0; i < 3; ++i)
            {
              row[i].get(array[index],array[index+1],array[index+2]);
              index += 3;
            }
       }

     void fillArrayColumnMajor(double array[9]) const
       {
            // Fill an array with contents of the matrix
            // Row - major form -> Column 1 == { array[0], array[1], array[2] }
         int index=0;
         for (int i=0; i < 3; ++i)
            {
              row[i].get(array[index],array[index+4],array[index+8]);
              index += 1;
            }
       }

     friend ostream& operator << (ostream& o, const Matrix3x3& mat)
       {
         o << "[ " << mat.row[0] << "," << endl
           << "  " << mat.row[1] << "," << endl
           << "  " << mat.row[2] << " ]" << endl;
         return o;
       }

     friend istream& operator >> (istream& i, Matrix3x3& mat)
       {
         char rowsep, dummy;
         i >> dummy >> mat.row[0] >> rowsep
           >> mat.row[1] >> rowsep
           >> mat.row[2] >> dummy;
         return i;
       }
};
     
#endif /* #ifndef _MATRIX3X3_HH_ */


/*
  $Log: Matrix3x3.hh,v $
  Revision 1.2  2000/02/09 22:15:06  vinod
  Code modifications to take care of file/class renames

  Revision 1.1  2000/02/09 22:01:22  vinod
  Renamed

  Revision 1.1  2000/02/09 21:21:42  vinod
  Moved into VecMat directory

  Revision 1.4  2000/02/09 07:03:28  vinod
  Moved invert fns, etc. and friend fns defined outside class to source file

  Revision 1.3  2000/02/01 06:24:16  vinod
  Minor bug fix, in invert()

  Revision 1.2  2000/01/27 03:48:52  vinod
  Added in-place inversion and transpose functions

  Revision 1.1  1999/10/18 05:32:02  vinod
  Matrix classes using Vectors

*/
