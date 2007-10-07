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

#ifndef _DLFLCORE_EXT_H_
#define _DLFLCORE_EXT_H_

#include "DLFLCommon.hh"
#include <Vector4d.hh>
#include <Quaternion.hh>

namespace DLFL {

  // Print an array of doubles separated by tabs
  void printArray(const DoubleArray& array);
  // Print an array of Vector3ds separated by newlines
  void printArray(const Vector3dArray& array);

  // The Hermite basis vector
  Vector4d Hv(double t);
  // Hermite basis vector for tangents
  Vector4d dHv(double t);
  // Derivative of dHv - second derivative of Hv
  Vector4d ddHv(double t);

  // Find centroid of given set of points
  Vector3d centroid(const Vector3dArray& poly);

  /* Determine if the given point is within the given polygon
   * Last parameter is the normal to be used for finding projection plane */
  bool pointInPolygon( Vector3d p, Vector3dArray poly, const Vector3d& projn );
  
  /* Find the optimal axial projection plane for a polygon with the specified normal vector
   * Return value: 0 = YZ, 1 = ZX, 2 = XY (corresponding to vector components of normal) */
  int findAxialProjectionPlane( const Vector3d& n );

  // Find a projection plane for 2 given planes (normals)
  Vector3d findProjectionPlane(const Vector3d& n1, const Vector3d& n2);

  // Project a polygon onto a plane defined by point and normal
  void planarProject(Vector3dArray& poly, const Vector3d& p, const Vector3d& nproj);
  // Project a point onto a plane defined by point and normal
  void planarProject(Vector3d& p, const Vector3d& point, const Vector3d& normal);
  // Project a line (defined by 2 points) onto a plane defined by point and normal
  void planarProject(Vector3d& p1, Vector3d& p2, const Vector3d& point, const Vector3d& normal);

  // Translate the given polygon so that the centroid is moved to the given origin
  // Returns amount by which polygon was translated
  Vector3d translate(Vector3dArray& poly, const Vector3d& origin);
  // Translate the given polygon along given direction by given amount
  void translate(Vector3dArray& poly, const Vector3d& dir, double dist);

  // Scale the given polygon about it's centroid by the given scale factor
  void scale(Vector3dArray& poly, double scale_factor);

  // Rotate the given polygon about it's centroid such tat normal goes to new direction
  void rotate(Vector3dArray& poly, const Vector3d& normal, const Vector3d& newnormal);
  // Rotate the given polygon around its normal by given angle
  void rotate(Vector3dArray& poly, const Vector3d& normal, double angle);

  // Resolve a planar polygon into distance and angle parameters for each vertex, based on
  // the centroid. The angles are w.r.t to the given xref vector (assumed to be normalized).
  // The yref vector is used to determine quadrant for correct angles
  // The distances are from the centroid
  void resolvePolygon(const Vector3dArray& poly, const Vector3d& xref, const Vector3d& yref,
		      DoubleArray& angle, DoubleArray& distance);

  // Reconstruct a polygon from its resolved parameters and reference axes
  // The reconstructed polygon will be centered at the origin
  void reconstructPolygon(Vector3dArray& poly, const Vector3d& xref, const Vector3d& yref,
			  const DoubleArray& angle, const DoubleArray& distance);

  // Linearly interpolate between 2 given sets of numbers using given parameter
  // Will do interpolation only upto smaller of 2 given arrays
  void linearInterpolate(const DoubleArray& start, const DoubleArray& end, DoubleArray& inter, double t);

  // Linearly interpolate between 2 given sets of Vector3ds using given parameter
  // Will do interpolation only upto smaller of 2 given arrays
  void linearInterpolate(const Vector3dArray& start, const Vector3dArray& end, Vector3dArray& inter, double t);

  // Do hermite interpolation given 2 end points and 2 end tangents. Compute point and tangent
  // at point for given parameter
  void hermiteInterpolate(const Vector3d& p1, const Vector3d& v1, const Vector3d& p2, const Vector3d& v2,
			  Vector3d& p, Vector3d& v, Vector3d& dvdt, double t);

  // Compute length of a Hermite curve using chord-length summation.
  // Curve is split into specified number of segments
  double hermiteCurveLength(const Vector3d& p1, const Vector3d& v1, const Vector3d& p2, const Vector3d& v2,
			    int numsegs);

  // Compute the centroid and average normal for a given array of points,
  // assuming they form a polygon and are specified in the correct order (RHS)
  void computeCentroidAndNormal(const Vector3dArray& p, Vector3d& centroid, Vector3d& normal);

  // Calculate doo-sabin coordinates for the given array of points,
  // assuming they form a polygon and are specified in the correct order
  void computeDooSabinCoords(Vector3dArray& points);
  
  // Calculate modified doo-sabin coordinates for the given array of points,
  // assuming they form a polygon and are specified in the correct order
  void computeModifiedDooSabinCoords(Vector3dArray& points);

  // Project a point onto a line along the normal from the line through the point
  Vector3d projectPointOnLine(const Vector3d& p, const Vector3d& p0, const Vector3d& p1);

  // Compute the distance of a point from a line
  double distPointLine(const Vector3d& p, const Vector3d& p0, const Vector3d& p1);

  // Project two lines onto a plane specified by point and normal
  // and fine their intersection
  Vector3d planarProjectAndIntersect(const Vector3d& p00, const Vector3d& p01,
				     const Vector3d& p10, const Vector3d& p11,
				     const Vector3d& point, const Vector3d& normal);
  /*
  // Compute lighting for a vertex with a given normal and lighting parameters
  // Returns a color
  Vector3d computeLighting(const Vector3d& p, const Vector3d& n, const RGBColor& basecolor,
			   double Ka, double Kd, double Ks, LightPtr lightptr);
  */
  // Find the intersection point between two coplanar lines specified by their end points
  Vector3d intersectCoplanarLines(const Vector3d& p00, const Vector3d& p01,
				  const Vector3d& p10, const Vector3d& p11);
} // end namespace

#endif // _DLFLCORE_EXT_H_
