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

/* $Id: Quaternion.hh,v 1.3 2002/11/17 18:53:45 vinod Exp $ */

#ifndef _QUATERNION_HH_

#define _QUATERNION_HH_

// Class for a Quaternion

#include "../Base/BaseObject.hh"
#include "Vector3d.hh"
#include "Matrix4x4.hh"

class Quaternion : public BaseObject
{
  protected :

     Vector3d   v3Vec;                                 // Vector component
     double     dScalar;                               // Scalar component

  public :

        // Default constructor - create an identity quaternion
     Quaternion()
       : BaseObject(), v3Vec(), dScalar(1.0)
       {}

        // Construct from a vector. Scalar is set to 0
     Quaternion(const Vector3d& vec)
       : BaseObject(), v3Vec(vec), dScalar(0.0)
       {}

        // Constructor from a vector and a scalar
     Quaternion(const Vector3d& vec, double scalar)
       : BaseObject(), v3Vec(vec), dScalar(scalar)
       {}

        // Same as above, but with reverse order
     Quaternion(double scalar, const Vector3d& vec)
       : BaseObject(), v3Vec(vec), dScalar(scalar)
       {}

        // Construct from 3/4 individual values. Scalar is set to 0 by default
     Quaternion(double x, double y, double z, double scalar=0.0)
       : BaseObject(), v3Vec(x,y,z), dScalar(scalar)
       {}

        // Copy constructor
     Quaternion(const Quaternion& quat)
       : BaseObject(quat), v3Vec(quat.v3Vec), dScalar(quat.dScalar)
       {}

        // Destructor
     ~Quaternion()
       {}

        // Assignment operator
     Quaternion& operator = (const Quaternion& quat)
       {
         BaseObject :: operator = (quat);
         v3Vec = quat.v3Vec; dScalar = quat.dScalar;
         return (*this);
       }

        // Assignment from Vector3d
     Quaternion& operator = (const Vector3d& vec)
       {
         v3Vec = vec; dScalar = 0.0;
         return (*this);
       }
     
        // Set the vector and scalar parts of the quaternion
     void set(const Vector3d& vec, double scalar=0.0)
       {
         v3Vec = vec; dScalar = scalar;
       }

     void set(double x, double y, double z, double scalar=0.0)
       {
         v3Vec.set(x,y,z); dScalar = scalar;
       }

        // Reset the quaternion to its default state - identity
     void reset(void)
       {
         v3Vec.reset(); dScalar = 1.0;
       }
     
        // Make a copy of the object
        // implement BaseObject class pure virtual function
     virtual BaseObject * copy(void) const
       {
         Quaternion * quat = new Quaternion(*this);
         return quat;
       }

        // Access the elements of the quaternion. Indices start at 0 and the
        // scalar is at index=3
     double& operator [] (uint index)
       {
         if ( index == 3 ) return dScalar;
         return v3Vec[index];
       }
     
     double operator [] (uint index) const
       {
         if ( index == 3 ) return dScalar;
         return v3Vec[index];
       }

        // Class member functions which return the identity quaternion
        // Identity quaternion is with the all vector components=0 and scalar=1
     static Quaternion identity(void)
       {
         Quaternion ident;  // Default constructor creates an identity quaternion
         return ident;
       }
     
     static Quaternion I(void)
       {
         Quaternion ident;  // Default constructor creates an identity quaternion
         return ident;
       }

        //--- Arithmetic assignment operators ---//

     void operator += (const Quaternion& quat)
       {
         dScalar += quat.dScalar;
         v3Vec += quat.v3Vec;
       }
     
     void operator -= (const Quaternion& quat)
       {
         dScalar -= quat.dScalar;
         v3Vec -= quat.v3Vec;
       }
     
     void operator *= (const Quaternion& quat)
       {
         Vector3d v;
         double s;

         s = dScalar*quat.dScalar - v3Vec*quat.v3Vec;
         v = quat.v3Vec*dScalar + v3Vec*quat.dScalar + v3Vec % quat.v3Vec;

         dScalar = s; v3Vec = v;
       }

     void operator *= (const Vector3d& vec)
       {
         Quaternion v2q(vec);

         this->operator *= (v2q);
       }
     
     void operator *= (double scalar)
       {
         dScalar *= scalar;
         v3Vec *= scalar;
       }
     
     void operator /= (double scalar)
       {
         dScalar /= scalar;
         v3Vec /= scalar;
       }

        // Arithmetic operators and various other functions, implemented as friend functions

        // Square of the length of a quaternion
     friend double lengthsqr(const Quaternion& quat)
       {
         double lensqr = quat.v3Vec*quat.v3Vec + quat.dScalar*quat.dScalar;
         return lensqr;
       }
     
        // Length of a quaternion
     friend double length(const Quaternion& quat)
       {
         return sqrt(lengthsqr(quat));
       }

        // For consistency define norm and normsqr also
     friend double normsqr(const Quaternion& quat)
       {
         return lengthsqr(quat);
       }

     friend double norm(const Quaternion& quat)
       {
         return length(quat);
       }

        // Compute conjugate of this quaternion which -v,s
     friend Quaternion conjugate(const Quaternion& quat)
       {
         return Quaternion(-quat.v3Vec,quat.dScalar);
       }
     
        // Normalize a quaternion - make it a unit quaternion
        // Returns the original length of the quaternion
        // If length is 0, nothing is changed
     friend double normalize(Quaternion& quat)
       {
         double len = length(quat);
         if ( isNonZero(len) == true )
            quat /= len;
         return len;
       }

     friend Quaternion normalized(const Quaternion& quat)
       {
         Quaternion unitq = quat;
         normalize(unitq);
         return unitq;
       }

        // Arithmetic operators

        // Negation
     friend Quaternion operator - (const Quaternion& q)
       {
         Quaternion neg(-q.dScalar, -q.v3Vec);
         return neg;
       }
     
     friend Quaternion operator + (const Quaternion& q1, const Quaternion& q2)
       {
         Quaternion sum = q1;
         sum += q2;
         return sum;
       }

     friend Quaternion operator - (const Quaternion& q1, const Quaternion& q2)
       {
         Quaternion diff = q1;
         diff -= q2;
         return diff;
       }

        // Post-multiplication by a scalar
     friend Quaternion operator * (const Quaternion& q, double scalar)
       {
         Quaternion prod = q;
         prod *= scalar;
         return prod;
       }

        // Pre-multiplication by a scalara
     friend Quaternion operator * (double scalar, const Quaternion& q)
       {
         Quaternion prod = q;
         prod *= scalar;
         return prod;
       }

        // Division by a scalar
     friend Quaternion operator / (const Quaternion& q, double scalar)
       {
         Quaternion quot = q;
         quot /= scalar;
         return quot;
       }

        // Multiplication of 2 quaternions
     friend Quaternion operator * (const Quaternion& q1, const Quaternion& q2)
       {
         Quaternion prod = q1;
         prod *= q2;
         return prod;
       }

        // Post-multiplication of a quaternion by a Vector3d
        // Same as above, except Vector3d is promoted to quaternion with scalar value 0.0
     friend Quaternion operator * (const Quaternion& q, const Vector3d& v)
       {
         Quaternion prod = q;
         prod *= v;
         return prod;
       }
     
        // Pre-multiplication of a quaternion by a Vector3d
     friend Quaternion operator * (const Vector3d& v, const Quaternion& q)
       {
         Quaternion prod(v);
         prod *= q;
         return prod;
       }

        // Convert to a rotation matrix. Assumes that quaternion has been normalized
     Matrix3_3 toMatrix(void) const
       {
         Matrix3_3 mat;
         double x,y,z,s;

         v3Vec.get(x,y,z); s = dScalar;

         mat[0].set( 1.0 - 2.0*(y*y + z*z), 2.0*(x*y - s*z), 2.0*(x*z + s*y) );
         mat[1].set( 2.0*(x*y + s*z), 1.0 - 2.0*(x*x + z*z), 2.0*(y*z - s*x) );
         mat[2].set( 2.0*(x*z - s*y), 2.0*(y*z + s*x), 1.0 - 2.0*(x*x + y*y) );

         return mat;
       }

     Matrix4_4 toMatrix4(void) const
       {
         Matrix4_4 mat4 = toMatrix();
         mat4[3][3] = 1.0;
         return mat4;
       }

     void toMatrix(double array[16]) const
       {
            // Similar to toMatrix4, but fills a given array of 16 elements with the
            // rotation matrix corresponding to this quaternion in column major form
         Matrix4_4 mat4 = toMatrix4();
         mat4.fillArrayColumnMajor(array);
       }
     
     friend Matrix3_3 toMatrix(const Quaternion& quat)
       {
         return quat.toMatrix();
       }

     friend Matrix4_4 toMatrix4(const Quaternion& quat)
       {
         return quat.toMatrix4();
       }
     
        //--- Conversion to and from axis and angle ---//
        //--- All angles are specified in radians ---//

        // Get the angle of rotation. Returns value between 0 and PI
     double getAngle(void) const
       {
         return 2.0 * acos(dScalar);
       }
     
        // Get the angle of rotation. Returns value between 0 and PI
        // Computes angle after normalizing the quaternion
     friend double getAngle(const Quaternion& quat)
       {
            // Make the quaternion a unit quaternion first
         Quaternion nq = normalized(quat);

         return nq.getAngle();
       }
     
        // Get the axis of rotation. Returns unit axis
     friend Vector3d getAxis(const Quaternion& quat)
       {
            // Make the quaternion a unit quaternion first
         Quaternion nq = normalized(quat);
         
         Vector3d axis = normalized(nq.v3Vec);
         return axis;
       }
     
        // Get the axis and angle.
     void getAxisAndAngle(Vector3d& axis, double& theta) const
       {
         Quaternion nq = normalized(*this);

         theta = 2.0 * acos(nq.dScalar);
         axis = normalized(nq.v3Vec);
       }

        // Set the axis and angle.
     void setAxisAndAngle(const Vector3d& axis, double theta)
       {
         dScalar = cos(theta/2.0);
         v3Vec = normalized(axis);
         v3Vec *= sin(theta/2.0);
       }

        // Change the angle to a new value. Use same axis as before
     void setAngle(double theta)
       {
         dScalar = cos(theta/2.0);
         normalize(v3Vec);
         v3Vec *= sin(theta/2.0);
       }

        // Scale the angle by the given scale factor
     void scaleAngle(double scale_factor)
       {
         setAngle(getAngle()*scale_factor);
       }
     
        //--- Insertion and extraction operators ---//
     friend ostream& operator << (ostream& o, const Quaternion& quat)
       {
         o << "{ " << quat.v3Vec << "," << quat.dScalar << " }";
         return o;
       }

     friend istream& operator >> (istream& i, Quaternion& quat)
       {
         char c;
         i >> c >> quat.v3Vec >> c >> quat.dScalar >> c;
         return i;
       }
};

#endif /* #ifndef _QUATERNION_HH_ */


/*
  $Log: Quaternion.hh,v $
  Revision 1.3  2002/11/17 18:53:45  vinod
  Fixed bug in friend multiplication operator
  Caught by Shannon - result was order independent, but should be order dependent

  Revision 1.2  2000/02/09 22:15:07  vinod
  Code modifications to take care of file/class renames

  Revision 1.1  2000/02/09 21:25:35  vinod
  Moved into VecMat directory

  Revision 1.10  2000/01/21 18:44:33  vinod
  Minor bug fixes. Changed toMatrix to not use references

  Revision 1.9  2000/01/13 23:35:16  vinod
  Added assignment from Vector3d

  Revision 1.8  1999/11/22 03:23:14  vinod
  Added conjugate function

  Revision 1.7  1999/11/22 03:08:54  vinod
  Added toMatrix function which fills array

  Revision 1.6  1999/10/26 05:01:50  vinod
  Added function to get rotation matrix as a Matrix4_4
  Added function to scale angle.
  Fixed const correctness

  Revision 1.5  1999/10/20 19:05:36  vinod
  Modified to use Matrix3_3 instead of Matrix3d. Other minor changes

  Revision 1.4  1999/04/04 07:08:26  vinod
  Added reset function

  Revision 1.3  1999/04/04 06:16:07  vinod
  Minor bug fixes

  Revision 1.2  1999/04/04 05:55:55  vinod
  Added insertion and extraction operators

  Revision 1.1  1999/04/03 22:23:45  vinod
  Quaternion class

*/
