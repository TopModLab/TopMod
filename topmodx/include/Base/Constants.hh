// $Id: Constants.hh,v 1.1 2000/02/09 21:17:47 vinod Exp $

#ifndef _CONSTANTS_HH_

#define _CONSTANTS_HH_

// Predefined constants, and macros
// To change the values of some of these macros, define them before including
// this file. Or dont include this file at all and define them on your own

// Zero '0' value to be used for comparison and assignment
#ifndef ZERO
#define ZERO 1.0e-10
#endif

// float infinity to be used for comparison and assignment
#ifndef FLT_INF
#define FLT_INF 1.0e35
#endif

// double infinity to be used for comparison and assignment
#ifndef DBL_INF
#define DBL_INF 1.0e100
#endif

#ifndef M_PI
#define M_PI 3.14159265358979323846   // pi
#endif

#ifndef M_SQRT1_2
#define M_SQRT1_2  0.707106781186547524401  // 1/sqrt(2)
#endif

#endif // #ifndef _CONSTANTS_HH_

// $Log: Constants.hh,v $
// Revision 1.1  2000/02/09 21:17:47  vinod
// Moved into Base directory
//
// Revision 1.1.1.1  1999/01/30 22:31:18  vinod
// Utility Classes
//
// Revision 1.2  1999/01/30 21:20:00  vinod
// *** empty log message ***
//
