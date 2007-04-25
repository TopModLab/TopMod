/* $Id: DLFLAux.cc,v 4.0 2003/12/26 01:58:52 vinod Exp $ */

// Auxiliary supporting subroutines for DLFL subroutines
#include "DLFLAux.hh"

// Print an array of doubles separated by tabs
void printArray(const DoubleArray& array)
{
  int n = array.size();
  for (int i=0; i < n; ++i)
     cout << array[i] << '\t';
  cout << endl;
}


// Print an array of Vector3ds separated by newlines
void printArray(const Vector3dArray& array)
{
  int n = array.size();
  for (int i=0; i < n; ++i)
     cout << array[i] << "  ";
  cout << endl;
}

// The Hermite basis vector
Vector4d Hv(double t)
{
  double t2 = sqr(t), t3 = cube(t);
  return Vector4d( 2.0*t3 - 3.0*t2 + 1.0,
                   -2.0*t3 + 3.0*t2,
                   t3 - 2.0*t2 + t,
                   t3 - t2 );
}

// Hermite basis vector for tangents
Vector4d dHv(double t)
{
  double t2 = sqr(t);
  return Vector4d( 6.0*t2 - 6.0*t,
                   -6.0*t2 + 6.0*t,
                   3.0*t2 - 4.0*t + 1,
                   3.0*t2 - 2.0*t );
}


// Derivative of dHv - second derivative of Hv
Vector4d ddHv(double t)
{
  return Vector4d( 12.0*t - 6.0,
                   -12.0*t + 6.0,
                   6.0*t - 4.0,
                   6.0*t - 2.0 );
}


// Determine if the given point is within the given polygon
// Last parameter is the normal to be used for finding projection plane
bool pointInPolygon(Vector3d p, Vector3dArray poly, const Vector3d& projn)
{
  bool ptin = false;

     // Determine the optimal plane for projecting the polygon
     // The largest component of the normal vector
     // determines the optimal projection plane
  int pp = findAxialProjectionPlane(projn);

     // The integer returned is the index of the component
     // corresponding to the normal of the projection plane
     // We don't actually have to project into the plane - we will just
     // use the other 2 coordinates for our computations

     // Do a 2D point in polygon test, using intersection counting method
     // The 2 coordinates that will be used in the calculation are (pp+1)%3 and (pp+2)%3
     // That is X will correspond to (pp+1)%3 and Y will correspond to (pp+2)%3
  int c = 0; // Count of no. of intersections of ray
  Vector3d prevp = poly[poly.size()-1]; // Previous point to current point. Initialized to last point
  Vector3d curp; // Current point
  int x = (pp+1)%3, y = (pp+2)%3; // Index for X and Y
  for (int i=0; i < poly.size(); ++i)
     {
       curp = poly[i];

          // Check if ray along positive X from p intersects
          // line segment between prep and curp
          // First check if Y coordinate is within the line segment
          // If it is then check if intersection is to the right.
       if ( ( (p[y] >= curp[y]) && (p[y] < prevp[y]) ) ||
            ( (p[y] >= prevp[y]) && (p[y] < curp[y]) ) )
          {
            if ( isNonZero(curp[y]-prevp[y]) ) /// Edge not along ray
               {
                 double interx = prevp[x] + (curp[x]-prevp[x]) * (p[y]-prevp[y]) / (curp[y]-prevp[y]);
                 if ( p[x] < interx ) c ^= 1;
               }
          }
       prevp = curp;
     }

     // For odd no. of intersections c will be 1, for even c will be 0
     // Odd no. means point is inside, even means point is outside
  if ( c ) ptin = true;

  return ptin;
}


// Find centroid of given set of points
Vector3d centroid(const Vector3dArray& poly)
{
  Vector3d cen;
  for (int i=poly.size()-1; i >= 0; --i)
     cen += poly[i];
  cen /= poly.size();
  return cen;
}


// Find the optimal axial projection plane for a polygon with the specified normal vector
// Return value: 0 = YZ, 1 = ZX, 2 = XY (corresponding to vector components of normal)
int findAxialProjectionPlane(const Vector3d& n)
{
     // Pick either of XY, YZ or ZX planes depending
     // on the largest component of the normal vector
     // The normal vector need not be normalized
     // If the given normal is of zero length, default
     // projection plane (XY) is returned
  int ppn = 1;
  double x,y,z;

     // Make all components positive.
  x = Abs(n[0]); y = Abs(n[1]); z = Abs(n[2]);

  if ( x > y )
     {
       if ( y > z )      ppn = 0; // x is largest, pick YZ plane
       else if ( x > z ) ppn = 0; // x is largest, pick YZ plane
       else              ppn = 2; // z is largest, pick XY plane
     }
  else
     {
       if ( x > z )      ppn = 1; // y is largest, pick ZX plane
       else if ( y > z ) ppn = 1; // y is largest, pick ZX plane
       else              ppn = 2; // z is largest, pick XY plane
     }
  return ppn;
}


// Find a projection plane for 2 given planes (normals)
Vector3d findProjectionPlane(const Vector3d& n1, const Vector3d& n2)
{
  Vector3d nproj(n1+n2);                               // Normal for projection plane
  if ( Abs(n1*n2 + 1.0) < ZERO ) nproj = n1;           // If 2 planes are opposite, choose 1 of them arbitrarily
  normalize(nproj);
  return nproj;
}


// Project a polygon onto a plane defined by point and normal
void planarProject(Vector3dArray& poly, const Vector3d& p, const Vector3d& nproj)
{
  Vector3d pp;                                         // Projected point

  for (int i=poly.size()-1; i >= 0; --i)
     {
       pp = poly[i] - ( (poly[i] - p )*nproj ) * nproj;
       poly[i] = pp;
     }
}


// Translate the given polygon so that the centroid is moved to the given origin
Vector3d translate(Vector3dArray& poly, const Vector3d& origin)
{
  Vector3d delta = origin - centroid(poly);

  for (int i = poly.size()-1; i >= 0; --i)
     poly[i] += delta;

  return delta;
}


// Translate the given polygon along given direction by given amount
void translate(Vector3dArray& poly, const Vector3d& dir, double dist)
{
  Vector3d delta = dist * normalized(dir);

  for (int i = poly.size()-1; i >= 0; --i)
     poly[i] += delta;
}


// Scale the given polygon about it's centroid by the given scale factor
void scale(Vector3dArray& poly, double scale_factor)
{
  Vector3d origin = centroid(poly);

  for (int i = poly.size()-1; i >= 0; --i)
     {
       poly[i] -= origin; poly[i] *= scale_factor; poly[i] += origin;
     }
}


// Rotate the given polygon about it's centroid such that normal goes to new direction
void rotate(Vector3dArray& poly, const Vector3d& normal, const Vector3d& newnormal)
{
     // Translate centroid of polygon to origin
  Vector3d cen = translate(poly,Vector3d(0,0,0));

  Vector3d axis;
  double cost, sint, angle;

     // Rotation axis is cross product of normal and new normal
  axis = normalized(normal) % normalized(newnormal);

     // if the length of the rotation axis is 0, then the 2 normals
     // are either the same or in opposite directions. Don't do anything
     // in both those cases to avoid reducing all points to 0
  if ( normsqr(axis) > ZERO )
     {
       sint = normalize(axis);
       cost = normalized(normal) * normalized(newnormal);

       angle = atan2(sint,cost);

          // Construct a quaternion for the rotation
       Quaternion rot; rot.setAxisAndAngle(axis,angle); normalize(rot);
       Quaternion rotp;

          // Rotate all points using the quaternion
       for (int i=poly.size()-1; i >= 0; --i)
          {
            rotp = rot * poly[i] * conjugate(rot);
            poly[i].set(rotp[0],rotp[1],rotp[2]);
          }
     }

     // Translate back to old centroid
  translate(poly,-cen);
}


// Rotate the given polygon around its normal by given angle
void rotate(Vector3dArray& poly, const Vector3d& normal, double angle)
{
     // Translate centroid of polygon to origin
  Vector3d cen = translate(poly,Vector3d(0,0,0));

  Vector3d axis = normalized(normal);

  if ( angle < 0.0 ) angle += 2.0*M_PI;

     // Construct a quaternion for the rotation
  Quaternion rot; rot.setAxisAndAngle(axis,angle); normalize(rot);
  Quaternion rotp;

     // Rotate all points using the quaternion
  for (int i=poly.size()-1; i >= 0; --i)
     {
       rotp = rot * poly[i] * conjugate(rot);
       poly[i].set(rotp[0],rotp[1],rotp[2]);
     }

     // Translate back to old centroid
  translate(poly,-cen);
}


// Resolve a planar polygon into distance and angle parameters for each vertex, based on
// the centroid. The angles are w.r.t to the given xref vector (assumed to be normalized).
// The yref vector is used to determine quadrant for correct angles.
// xref and yref should be mutually perpendicular and lie on the plane of the polygon
// The distances are from the centroid
void resolvePolygon(const Vector3dArray& poly, const Vector3d& xref, const Vector3d& yref,
                    DoubleArray& angle, DoubleArray& distance)
{
  Vector3d cen = centroid(poly);
  Vector3d vec,p;
  double theta, ptheta, dist;
  double cost, sint;
  int n = poly.size();

  angle.resize(n,0.0); distance.resize(n,0.0);

     // First resolve first point
  p = poly[0]; vec = p-cen;
  dist = normalize(vec);
  cost = vec * xref; sint = vec * yref;
  theta = atan2(sint,cost);

     // We want the first angle to always be positive
     // atan2 returns angle in the range -pi to pi
  if ( theta < 0.0 ) theta += 2.0*M_PI;
  
  angle[0] = theta ; distance[0] = dist;
  ptheta = theta;
       
  for (int i=1; i < n; ++i)
     {
       p = poly[i]; vec = p-cen;

       dist = normalize(vec);
       cost = vec * xref; sint = vec * yref;
       theta = atan2(sint,cost);

          // We want the angles to be in ascending order
          // So if angle is smaller than previous angle, add 2pi
          // **NOTE** We might have to add several multiples of 2pi
          // to make the angle greater than the previous angle!!
       while ( theta < ptheta ) theta += 2.0*M_PI;

          // Store the actual angles
       angle[i] = theta; distance[i] = dist;
       ptheta = theta;
     }
}


// Reconstruct a polygon from its resolved parameters and reference axes
// The reconstructed polygon will be centered at the origin
void reconstructPolygon(Vector3dArray& poly, const Vector3d& xref, const Vector3d& yref,
                        const DoubleArray& angle, const DoubleArray& distance)
{
  int n = angle.size();
  poly.resize(n);
  for (int i=0; i < n; ++i)
     {
       poly[i] = distance[i] * ( xref*cos(angle[i]) + yref*sin(angle[i]) );
     }
}


// Linearly interpolate between 2 given sets of numbers using given parameter
// Will do interpolation only upto smaller of 2 given arrays
void linearInterpolate(const DoubleArray& start, const DoubleArray& end, DoubleArray& inter, double t)
{
  int n = start.size();
  if ( end.size() < n ) n = end.size();

  for (int i=0; i < n; ++i)
     inter[i] = start[i]*(1.0-t) + end[i]*t;
}


// Linearly interpolate between 2 given sets of Vector3ds using given parameter
// Will do interpolation only upto smaller of 2 given arrays
void linearInterpolate(const Vector3dArray& start, const Vector3dArray& end, Vector3dArray& inter, double t)
{
  int n = start.size();
  if ( end.size() < n ) n = end.size();

  for (int i=0; i < n; ++i)
     inter[i] = start[i]*(1.0-t) + end[i]*t;
}


// Do hermite interpolation given 2 end points and 2 end tangents. Compute point and tangent
// at point for given parameter
void hermiteInterpolate(const Vector3d& p1, const Vector3d& v1, const Vector3d& p2, const Vector3d& v2,
                        Vector3d& p, Vector3d& v, Vector3d& dvdt, double t)
{
  Vector4d hv,dhv,ddhv,G;
  hv = Hv(t); dhv = dHv(t); ddhv = ddHv(t);

  G.set(p1[0],p2[0],v1[0],v2[0]);  p[0] = hv * G;  v[0] = dhv * G; dvdt[0] = ddhv * G;
  G.set(p1[1],p2[1],v1[1],v2[1]);  p[1] = hv * G;  v[1] = dhv * G; dvdt[1] = ddhv * G;
  G.set(p1[2],p2[2],v1[2],v2[2]);  p[2] = hv * G;  v[2] = dhv * G; dvdt[2] = ddhv * G;
}


// Compute length of a Hermite curve using chord-length summation.
// Curve is split into specified number of segments
double hermiteCurveLength(const Vector3d& p1, const Vector3d& v1, const Vector3d& p2, const Vector3d& v2,
                          int numsegs)
{
  double length = 0.0, t;
  Vector3d p, p0;
  Vector4d hv,Gx,Gy,Gz;

  Gx.set(p1[0],p2[0],v1[0],v2[0]);
  Gy.set(p1[1],p2[1],v1[1],v2[1]);
  Gz.set(p1[2],p2[2],v1[2],v2[2]);

  p0 = p1;
  for (int i=1; i < numsegs-1; ++i)
     {
       t = double(i)/double(numsegs);
       hv = Hv(t);

       p[0] = hv * Gx; p[1] = hv * Gy; p[2] = hv * Gz;

       length += norm(p-p0);
       p0 = p;
     }
  length += norm(p2-p0);

  return length;
}

/*
  $Log: DLFLAux.cc,v $
  Revision 4.0  2003/12/26 01:58:52  vinod
  Major version sync.

  Revision 3.3  2003/11/14 02:39:38  vinod
  Removed isConcaveCorner method. Added pointInPolygon method

  Revision 3.2  2003/11/07 05:37:41  vinod
  Added isConcaveCorner and findAxialProjectPlane methods

  Revision 3.1  2003/11/06 17:21:01  vinod
  Added second translate method for moving polygons along a direction

  Revision 3.0  2003/09/19 16:06:47  vinod
  Major version update

  Revision 2.9  2003/04/29 08:40:09  vinod
  Included dvdt calculation for Hermite interpolation

  Revision 2.8  2003/04/04 06:27:30  vinod
  Added methods to compute length of Hermite curve

  Revision 2.7  2001/12/05 18:12:48  vinod
  Added linearInterpolate method for Vector arrays

  Revision 2.6  2001/10/16 04:52:17  vinod
  Modified resolvePolygon to account for cases where several multiples of 2pi
  might have to be added to make angles monotonically increasing

  Revision 2.5  2001/10/15 23:23:21  vinod
  Added check for un-solvable rotations - when axis of rotation can't be
  determined, in rotate method for rotation from one normal to a new normal
  Modified resolvePolygon to make sure all angles are positive and
  monotonously increasing. And now stores actual angles instead of
  differences.

  Revision 2.4  2001/10/13 18:16:56  vinod
  Added code to ensure all angles in resolvePolygon are positive

  Revision 2.3  2001/09/19 03:57:18  vinod
  translate now returns the amount by which the polygon was translated
  Fixed bug in rotate methods which took polygon back to original centroid

  Revision 2.2  2001/09/15 22:33:04  vinod
  Added rotate by angle

  Revision 2.1  2001/08/07 07:15:53  vinod
  Added polygon scaling

  Revision 2.0  2001/07/25 05:12:41  vinod
  Major version sync

  Revision 1.3  2001/05/17 07:41:34  vinod
  Fixed axis/angle calculation in rotate

  Revision 1.2  2001/05/17 04:59:16  vinod
  Added printArray, Moved Hv from DLFLObject.cc, Added dHv
  Changed planarProject to do projection for one plane only
  Added findProjectionPlane, rotate, reconstructPolygon, hermiteInterpolate

  Revision 1.1  2001/05/15 21:09:39  vinod
  Auxiliary subroutines for DLFL

*/
