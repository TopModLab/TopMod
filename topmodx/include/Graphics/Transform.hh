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

/* $Id: Transform.hh,v 2.2 2000/05/05 20:50:10 vinod Exp $ */

#ifndef _TRANSFORM_HH_

#define _TRANSFORM_HH_

// Class for transformations (translation, scaling, rotation)

#include "../vecmat/Vector3d.hh"
#include "../vecmat/Matrix4x4.hh"
#include "../vecmat/Quaternion.hh"
#include <cmath>
/*
#ifdef __APPLE__
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#else
#include <GL/gl.h>
#include <GL/glu.h>
#endif
*/
class Transformation;
typedef Transformation * TransformationPtr;

class Transformation
{
	public :

	Matrix4x4 transform;                              // Combined transformation matrix

	public :

				// Default constructor
	Transformation()
		: transform()
		{}

				// Copy constructor
	Transformation(const Transformation& tr)
		: transform(tr.transform)
		{}

				// Construct from a matrix
	Transformation(const Matrix4x4& mat)
		: transform(mat)
		{}

				// Destructor
	~Transformation()
		{}

				// Assignment operator
	Transformation& operator = (const Transformation& tr)
	{
		transform = tr.transform;
		return (*this);
	}

				// Static functions to compute translation, rotation and scaling matrices
	static Matrix4x4 translation(const Vector3d& t)
	{
		Matrix4x4 transmat;
		transmat[0][3] = t[0];
		transmat[1][3] = t[1];
		transmat[2][3] = t[2];
		return transmat;
	}

	static Matrix4x4 translation(double tx, double ty, double tz)
	{
		Matrix4x4 transmat;
		transmat[0][3] = tx;
		transmat[1][3] = ty;
		transmat[2][3] = tz;
		return transmat;
	}

	static Matrix4x4 rotationX(double angle)          // Rotation about X axis
	{
		Matrix4x4 rotmat;
		double ct = cos(angle), st = sin(angle);
		rotmat[0].set( 1,  0,  0, 0);
		rotmat[1].set( 0, ct,-st, 0);
		rotmat[2].set( 0, st, ct, 0);
		rotmat[3].set( 0,  0,  0, 1);
		return rotmat;
	}

	static Matrix4x4 rotationY(double angle)          // Rotation about Y axis
	{
		Matrix4x4 rotmat;
		double ct = cos(angle), st = sin(angle);
		rotmat[0].set( ct, 0, st, 0);
		rotmat[1].set(  0, 1,  0, 0);
		rotmat[2].set(-st, 0, ct, 0);
		rotmat[3].set(  0, 0,  0, 1);
		return rotmat;
	}

	static Matrix4x4 rotationZ(double angle)          // Rotation about Z axis
	{
		Matrix4x4 rotmat;
		double ct = cos(angle), st = sin(angle);
		rotmat[0].set( ct,-st, 0, 0);
		rotmat[1].set( st, ct, 0, 0);
		rotmat[2].set(  0,  0, 1, 0);
		rotmat[3].set(  0,  0, 0, 1);
		return rotmat;
	}

	static Matrix4x4 scaling(const Vector3d& s)
	{
		Matrix4x4 scmat;
		scmat[0][0] = s[0];
		scmat[1][1] = s[1];
		scmat[2][2] = s[2];
		return scmat;
	}

	static Matrix4x4 scaling(double sx, double sy, double sz)
	{
		Matrix4x4 scmat;
		scmat[0][0] = sx;
		scmat[1][1] = sy;
		scmat[2][2] = sz;
		return scmat;
	}

				// Compute the matrix equivalent to gluLookAt for given eye, center and upvector
	static Matrix4x4 lookat(const Vector3d& eye, const Vector3d& center, const Vector3d& up)
	{
		Matrix4x4 lmat;
		Vector3d f = normalized(center - eye);
		Vector3d UP = normalized(up);
		Vector3d s = normalized(f % UP);
		Vector3d u = normalized(s % f);

		lmat[0] = s;
		lmat[1] = u;
		lmat[2] = -f;

		return lmat;
	}

				// Combining two transformations/transformation and matrix

				// Post-multiply with given transformation/matrix
	void operator *= (const Transformation& tr)
	{
		transform *= tr.transform;
	}

	void operator *= (const Matrix4x4& mat)
	{
		transform *= mat;
	}

				// Pre-multiply with given transformation/matrix
				// The operator chosen is not the most intuitive, but the only one that makes
				// some kind of sense
	void operator /= (const Transformation& tr)
	{
		transform = tr.transform * transform;
	}

	void operator /= (const Matrix4x4& mat)
	{
		transform = mat * transform;
	}

	Transformation operator * (const Transformation& tr)
	{
		Transformation newtr(*this);
		newtr *= tr;
		return newtr;
	}

	Transformation operator * (const Matrix4x4& mat)
	{
		Transformation newtr(*this);
		newtr *= mat;
		return newtr;
	}

	friend Transformation operator * (const Matrix4x4& mat, const Transformation& tr)
	{
		Transformation newtr(mat);
		newtr *= tr;
		return newtr;
	}

	void invert(void)
	{
						// Invert the transformation matrix
		transform.invert();
	}
	
	// Apply the transformation matrix to a vector
	Vector3d applyTo(const Vector3d& vec) {
		return transform*vec;
	}

	// Apply transformations - pre-multiply by corresponding transformation matrices
	void translate(const Vector3d& t)
	{
		transform = Transformation :: translation(t) * transform;
	}

	void translate(double tx, double ty, double tz)
	{
		transform = Transformation :: translation(tx,ty,tz) * transform;
	}

	void rotateX(double angle)
	{
		transform = Transformation :: rotationX(angle) * transform;
	}

	void rotateY(double angle)
	{
		transform = Transformation :: rotationY(angle) * transform;
	}

	void rotateZ(double angle)
	{
		transform = Transformation :: rotationZ(angle) * transform;
	}

	void rotate(const Quaternion& quat)
	{
						// Rotate according to the rotation specified by the quaternion
		transform = quat.toMatrix4() * transform;
	}

	void scale(const Vector3d& s)
	{
		transform = Transformation :: scaling(s) * transform;
	}

	void scale(double sx, double sy, double sz)
	{
		transform = Transformation :: scaling(sx,sy,sz) * transform;
	}

				// Apply transformations - post-multiply
	void post_translate(const Vector3d& t)
	{
		transform *= Transformation :: translation(t);
	}

	void post_translate(double tx, double ty, double tz)
	{
		transform *= Transformation :: translation(tx,ty,tz);
	}

	void post_rotateX(double angle)
	{
		transform *= Transformation :: rotationX(angle);
	}

	void post_rotateY(double angle)
	{
		transform *= Transformation :: rotationY(angle);
	}

	void post_rotateZ(double angle)
	{
		transform *= Transformation :: rotationZ(angle);
	}

	void post_rotate(const Quaternion& quat)
	{
						// Rotate according to the rotation specified by the quaternion
		transform *= quat.toMatrix4();
	}

	void post_scale(const Vector3d& s)
	{
		transform *= Transformation :: scaling(s);
	}

	void post_scale(double sx, double sy, double sz)
	{
		transform *= Transformation :: scaling(sx,sy,sz);
	}

	// Apply the transformation in OpenGL. Calls only glMultMatrix
	void apply(void) const
	{
		double mat[16];
		transform.fillArrayColumnMajor(mat);
		//glMultMatrixd(mat);
	}

	void fillArrayColumnMajor( double mat[] ) {
		transform.fillArrayColumnMajor(mat);
	}

	// Get the transformation matrix
	Matrix4x4 matrix(void)
	{
		return transform;
	}

				// Reset the transformation matrix
	void reset(void)
	{
		transform.reset();
	}

				// Set the transformation matrix
	void set(const Matrix4x4 mat)
	{
		transform = mat;
	}
};

#endif /* #ifndef _TRANSFORM_HH_ */


/*
$Log: Transform.hh,v $
	Revision 2.2  2000/05/05 20:50:10  vinod
	Bug fix in rotation matrices : required transform

	Revision 2.1  2000/04/28 22:52:48  vinod
	Added Transformation::lookat subroutine, changed Matrix4_4 to Matrix4x4

	Revision 2.0  2000/03/12 00:49:31  vinod
	Major revision number change

	Revision 1.2  2000/03/03 00:50:33  vinod
	Remove BaseObject derivation, changed apply to use Column major

	Revision 1.1  2000/03/01 01:38:24  vinod
	Transformation class

*/
