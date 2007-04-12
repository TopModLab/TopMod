/* $Id: DLFLAux.hh,v 4.0 2003/12/26 01:58:52 vinod Exp $ */

#ifndef _DLFL_AUX_HH_

#define _DLFL_AUX_HH_

// Auxiliary subroutines for DLFL

#include "DLFLCommon.hh"
#include <Vector4d.hh>
#include <Quaternion.hh>

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


// Determine if the given point is within the given polygon
// Last parameter is the normal to be used for finding projection plane
bool pointInPolygon(Vector3d p, Vector3dArray poly, const Vector3d& projn);

// Find centroid of given set of points
Vector3d centroid(const Vector3dArray& poly);

// Find the optimal axial projection plane for a polygon with the specified normal vector
// Return value: 0 = YZ, 1 = ZX, 2 = XY (corresponding to vector components of normal)
int findAxialProjectionPlane(const Vector3d& n);

// Find a projection plane for 2 given planes (normals)
Vector3d findProjectionPlane(const Vector3d& n1, const Vector3d& n2);


// Project a polygon onto a plane defined by point and normal
void planarProject(Vector3dArray& poly, const Vector3d& p, const Vector3d& nproj);


// Translate the given polygon so that the centroid is moved to the given origin
// Returns amount by which polygon was translated
Vector3d translate(Vector3dArray& poly, const Vector3d& origin);


// Translate the given polygon along given direction by given amount
void translate(Vector3dArray& poly, const Vector3d& dir, double dist);

// Scale the given polygon about it's centroid by the given scale factor
void scale(Vector3dArray& poly, double scale_factor);


// Rotate the given polygon about it's centroid such that normal goes to new direction
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

#endif /* #ifndef _DLFL_AUX_HH_ */


/*
  $Log: DLFLAux.hh,v $
  Revision 4.0  2003/12/26 01:58:52  vinod
  Major version sync.

  Revision 3.3  2003/11/14 02:39:21  vinod
  Removed isConcaveCorner method. Added pointInPolygon method

  Revision 3.2  2003/11/07 05:37:26  vinod
  Added isConcaveCorner and findAxialProjectPlane methods

  Revision 3.1  2003/11/06 17:21:01  vinod
  Added second translate method for moving polygons along a direction

  Revision 3.0  2003/09/19 16:06:47  vinod
  Major version update

  Revision 2.6  2003/04/29 08:40:09  vinod
  Included dvdt calculation for Hermite interpolation

  Revision 2.5  2003/04/04 06:27:30  vinod
  Added methods to compute length of Hermite curve

  Revision 2.4  2001/12/05 18:12:48  vinod
  Added linearInterpolate method for Vector arrays

  Revision 2.3  2001/09/19 03:56:26  vinod
  translate returns delta

  Revision 2.2  2001/09/13 23:08:28  vinod
  Added rotate

  Revision 2.1  2001/08/07 07:15:45  vinod
  Added polygon scaling

  Revision 2.0  2001/07/25 05:12:41  vinod
  Major version sync

  Revision 1.2  2001/05/17 04:58:26  vinod
  Added printArray, Moved Hv from DLFLObject.cc, Added dHv
  Changed planarProject to do projection for one plane only
  Added findProjectionPlane, rotate, reconstructPolygon, hermiteInterpolate

  Revision 1.1  2001/05/15 21:12:05  vinod
  Auxiliary subroutines for DLFL

*/
