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

// $Id: Inlines.hh,v 1.4 2002/10/25 15:40:47 vinod Exp $

#ifndef _INLINES_HH_

#define _INLINES_HH_

// Frequently used operations defined as inline functions

#include "Constants.hh"
#include <math.h>

#ifndef __GNUG__
#include <bool.h>
#endif

typedef unsigned int uint;


//--- Inline absolute value functions ---//

#ifndef _INLINE_ABS_

#define _INLINE_ABS_

inline int Abs(int x)
{
  return (x < 0) ? -x : x;
}

inline float Abs(float x)
{
  return (x < 0.0) ? -x : x;
}

inline double Abs(double x)
{
  return (x < 0.0) ? -x : x;
}

#endif // #ifndef _INLINE_ABS_



//--- Checking against zero ---//

#ifndef _INLINE_ZERO_CHECKS_

#define _INLINE_ZERO_CHECKS_

inline bool isNonZero(int num)
{
  return ( ( num != 0 ) ? true : false );
}

inline bool isNonZero(double num)
{
  return ( ( (num < -ZERO) || (num > ZERO) ) ? true : false );
}

#endif // #ifndef _INLINE_ZERO_CHECKS_



//--- Floating point comparison ---//

#ifndef _INLINE_FP_EQUAL_

#define _INLINE_FP_EQUAL_

inline bool areEqual(float x1, float x2)
{
  return ( ( Abs(x1-x2) < ZERO ) ? true : false );
}

inline bool areEqual(double x1, double x2)
{
  return ( ( Abs(x1-x2) < ZERO ) ? true : false );
}

#endif // #ifndef _INLINE_FP_EQUAL_



//--- Checking for odd and even numbers ---//

#ifndef _INLINE_ODD_EVEN_

#define _INLINE_ODD_EVEN_

inline bool isOdd(int num)
{
  return ( (num % 2) ? true : false );
}

inline bool isEven(int num)
{
  return ( (num % 2) ? false : true );
}

inline bool isOdd(uint num)
{
  return ( (num % 2) ? true : false );
}

inline bool isEven(uint num)
{
  return ( (num % 2) ? false : true );
}

#endif // #ifndef _INLINE_ODD_EVEN_



//--- Mathematical subroutines ---//

#ifndef _INLINE_MATH_

#define _INLINE_MATH_

inline int sqr(int x)
{
  return x*x;
}

inline int cube(int x)
{
  return x*x*x;
}

inline uint sqr(uint x)
{
  return x*x;
}

inline uint cube(uint x)
{
  return x*x*x;
}

inline float sqr(float x)
{
  return x*x;
}

inline float cube(float x)
{
  return x*x*x;
}

inline double sqr(double x)
{
  return x*x;
}

inline double cube(double x)
{
  return x*x*x;
}

#endif // #ifndef _INLINE_MATH_



//--- Min and Max subroutines ---//

#ifndef _INLINE_MIN_MAX_

#define _INLINE_MIN_MAX_

inline int min(int x, int y)
{
  return ( (x < y) ? x : y );
}

inline int max(int x, int y)
{
  return ( (x > y) ? x : y );
}

inline uint min(uint x, uint y)
{
  return ( (x < y) ? x : y );
}

inline uint max(uint x, uint y)
{
  return ( (x > y) ? x : y );
}

inline float min(float x, float y)
{
  return ( (x < y) ? x : y );
}

inline float max(float x, float y)
{
  return ( (x > y) ? x : y );
}

inline double min(double x, double y)
{
  return ( (x < y) ? x : y );
}

inline double max(double x, double y)
{
  return ( (x > y) ? x : y );
}

#endif // #ifndef _INLINE_MIN_MAX_



//--- Swap two numbers ---//

#ifndef _INLINE_SWAP_

#define _INLINE_SWAP_

inline void swap(int& x, int& y)
{
  int t = x; x = y; y = t;
}

inline void swap(uint& x, uint& y)
{
  uint t = x; x = y; y = t;
}

inline void swap(float& x, float& y)
{
  float t = x; x = y; y = t;
}

inline void swap(double& x, double& y)
{
  double t = x; x = y; y = t;
}

#endif // #ifndef _INLINE_SWAP_



//--- Conversion between radians and angles ---//

#ifndef _INLINE_DEG_RAD_

#define _INLINE_DEG_RAD_

inline float deg2rad(float deg)
{
  return deg*M_PI/180.0;
}

inline double deg2rad(double deg)
{
  return deg*M_PI/180.0;
}

inline float rad2deg(float rad)
{
  return rad*180.0/M_PI;
}

inline double rad2deg(double rad)
{
  return rad*180.0/M_PI;
}

#endif // #ifndef _INLINE_DEG_RAD_



//--- Sign of numbers ---//

#ifndef _INLINE_SIGN_

#define _INLINE_SIGN_

inline int sign(int x)
{
  return ( (x < 0) ? -1 : ((x > 0) ? 1 : 0) );
}

inline float sign(float x)
{
  return ( (x < 0.0) ? -1.0 : ((x > 0.0) ? 1.0 : 0.0) );
}

inline double sign(double x)
{
  return ( (x < 0.0) ? -1.0 : ((x > 0.0) ? 1.0 : 0.0) );
}

// For matrices - sign of cofactor. 1 if i+j is even, -1 if i+j is odd
inline int cofsign(int i, int j)
{
  return ( ((i+j)%2) ? -1 : 1 );
}

inline int cofsign(uint i, uint j)
{
  return ( ((i+j)%2) ? -1 : 1 );
}

#endif // #ifndef _INLINE_SIGN_



//--- Wrap a given integer to lie between 0 and size-1 ---//
// Assumes that the specified size is > 0

#ifndef _INLINE_WRAP_

#define _INLINE_WRAP_

inline int wrap(int i, int size)
{
  int retval = (i%size);
  return ( (retval >= 0) ? retval : size+retval );
}

#endif // #ifndef _INLINE_WRAP_


#endif // #ifndef _INLINES_HH_ 

// $Log: Inlines.hh,v $
// Revision 1.4  2002/10/25 15:40:47  vinod
// Added preprocessor directies for conditional includes depending on compiler
//
// Revision 1.3  2000/04/19 06:48:17  vinod
// Removed unecessary wrap overload. Bug fix on #ifndef
//
// Revision 1.2  2000/04/18 05:44:41  vinod
// Added wrap function
//
// Revision 1.1  2000/02/09 21:17:47  vinod
// Moved into Base directory
//
// Revision 1.11  2000/02/09 06:42:13  vinod
// Added macros to prevent multiple definitions of specific blocks of fns
//
// Revision 1.10  2000/02/04 21:47:11  vinod
// Added Abs and areEqual functions
//
// Revision 1.9  2000/01/21 18:42:54  vinod
// *** empty log message ***
//
// Revision 1.8  1999/10/18 04:35:01  vinod
// Added cofsign function
//
// Revision 1.7  1999/04/30 05:34:45  vinod
// Made sign functions inline
//
// Revision 1.6  1999/04/30 05:29:48  vinod
// Added sign functions
//
// Revision 1.5  1999/04/03 21:40:54  vinod
// Added functions for conversion between degrees and radians
//
// Revision 1.4  1999/04/02 07:34:59  vinod
// Added functions to check odd/even nos
//
// Revision 1.3  1999/04/01 06:46:29  vinod
// Added swap functions
//
// Revision 1.2  1999/02/11 07:20:39  vinod
// Made functions actually inline
//
// Revision 1.1.1.1  1999/01/30 22:31:19  vinod
// Utility Classes
//
// Revision 1.4  1999/01/30 21:19:44  vinod
// *** empty log message ***
//
