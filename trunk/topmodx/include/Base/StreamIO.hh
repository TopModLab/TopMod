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

// $Id: StreamIO.hh,v 1.9 2003/10/22 02:10:26 vinod Exp $

#ifndef _STREAMIO_HH_

#define _STREAMIO_HH_

// Miscellaneous functions for operating on IO streams in C++

#ifndef __GNUG__
#include <iostream.h>
#include <iomanip.h>
#include <ctype.h>
#include <bool.h>
#else
#include <iostream>
#include <iomanip>
#include <ctype.h>

using namespace std;

#endif


inline void removeWhiteSpace(istream& i)
{
  char c;

  while ( i )
     {
       i.get(c);
       if ( c != ' ' && c != '\t' && c != '\n' )
          {
            i.putback(c);
            break;
          }
     }
}

inline void removeSpaces(istream& i)
{
  char c;

  while ( i )
     {
       i.get(c);
       if ( c != ' ' )
          {
            i.putback(c);
            break;
          }
     }
}

inline void removeSpacesAndTabs(istream& i)
{
  char c;

  while ( i )
     {
       i.get(c);
       if ( c != ' ' && c != '\t' )
          {
            i.putback(c);
            break;
          }
     }
}

inline bool readTillEOL(istream& i)
{
     // Read till newline character occurs or EOF is reached
     // newline character is also read from the stream
     // Returns false if EOF is reached before EOL, true otherwise
  char c;

  i.get(c);
  while ( i && c != '\n' )
     i.get(c);
  if ( c == '\n' ) return true;
  return false;
}

inline bool readTill(istream& i, char c)
{
     // Read till specified character occurs or EOL/EOF is reached
     // Specified character is left in the stream
     // If EOF/EOL occurs before c, returns false, true otherwise
  char ic = i.peek();
  while ( i && ic != c && ic != '\n' )
     {
       i.get(ic); ic = i.peek();
     }
  if ( ic == c ) return true;
  return false;
}

inline bool readTillNumeric(istream& i)
{
     // Read till a numeric digit (0-9) occurs or EOL/EOF is reached
     // If EOL/EOF is reached before a digit occurs, returns false
     // and leaves the EOL/EOF character in i. If a digit is reached
     // returns true and leaves the digit in i
  char ic = i.peek();
  while ( i && !isdigit(ic) && ic != '\n' )
     {
       i.get(ic); ic = i.peek();
     }
  if ( isdigit(ic) ) return true;
  return false;
}

inline bool readTillDigit(istream& i)
{
     // Read till a digit or plus/minus sign occurs or EOL/EOF is reached
     // If EOL/EOF is reached before a digit occurs, returns false
     // and leaves the EOL/EOF character in i. If a digit is reached
     // returns true and leaves the digit in i
  char ic = i.peek();
  while ( i && !isdigit(ic) && ic != '+' && ic != '-' && ic != '\n' )
     {
       i.get(ic); ic = i.peek();
     }
  if ( isdigit(ic) || ic == '+' || ic == '-' ) return true;
  return false;
}

inline bool readTillFloat(istream& i)
{
     // Same as previous subroutine, except that '.' is also allowed
  char ic = i.peek();
  while ( i && !isdigit(ic) && ic != '+' && ic != '-' && ic != '.' && ic != '\n' )
     {
       i.get(ic); ic = i.peek();
     }
  if ( isdigit(ic) || ic == '+' || ic == '-' || ic == '.' ) return true;
  return false;
}

#endif // #ifndef _STREAMIO_HH_

// $Log: StreamIO.hh,v $
// Revision 1.9  2003/10/22 02:10:26  vinod
// Moved using namespace std line into ifndef so it is included only for GNU compilers
//
// Revision 1.8  2003/10/22 01:22:07  vinod
// Added namespace specification
//
// Revision 1.7  2002/10/25 15:40:47  vinod
// Added preprocessor directies for conditional includes depending on compiler
//
// Revision 1.6  2000/07/20 00:45:20  vinod
// Added readTillNumeric
//
// Revision 1.5  2000/04/20 04:23:01  vinod
// Fixed readTillDigit and readTillFloat to recognize plus/minus signs also
//
// Revision 1.4  2000/04/19 20:42:44  vinod
// Added readTillFloat
//
// Revision 1.3  2000/04/19 19:32:15  vinod
// Modified readTill functions to return a true/false
// Added readTillDigit function
//
// Revision 1.2  2000/03/23 05:08:02  vinod
// Added readTill function to read till a given character
//
// Revision 1.1  2000/02/09 21:17:47  vinod
// Moved into Base directory
//
// Revision 1.4  1999/10/08 22:25:34  vinod
// Added readTillEOL
//
// Revision 1.3  1999/02/13 07:55:56  vinod
// Added 2 new functions, changed removeWhiteSpace to remove newlines and tabs also
//
// Revision 1.2  1999/02/11 07:20:39  vinod
// Made functions actually inline
//
// Revision 1.1.1.1  1999/01/30 22:31:19  vinod
// Utility Classes
//
// Revision 1.2  1999/01/30 21:19:46  vinod
// *** empty log message ***
//
