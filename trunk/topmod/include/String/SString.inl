// $Id: SString.inl,v 1.2 2002/10/17 01:36:19 vinod Exp $

// Undefine INLINE macro if it has been defined previously
#ifdef INLINE
#undef INLINE
#endif // #ifdef INLINE

// If we are compiling in debug mode, dont make functions inline
// So we should treat this file as a C++ file - include the header
// If we not in debug mode, then the header file should include this file
// at the end
// Note : For debug mode, the Makefile should make sure that this file is
// compiled! The C++ file for the class which includes both the header file
// and this file, can be used to ensure proper compilation

#ifdef STRING_DEBUG
#define INLINE
#include "SString.hh"
#else
#define INLINE inline
#endif // #ifdef STRING_DEBUG

// The header file should be included before this file
// (in both debug and non-debug modes)

#ifdef _STRING_HH_

#ifndef _SSTRING_INL_

#define _SSTRING_INL_

   // Free the internal string
INLINE void SString :: freeString(void)
{
  delete [] str; len = 0;
}

   // Default constructor
INLINE SString :: SString()
  : str(NULL), len(0)
{
     // Initialize to NULL string
}

   // Construct from a character array
INLINE SString :: SString(const char * char_array)
  : str(NULL), len(0)
{
  copyFrom(char_array);
}

   // Copy constructor
INLINE SString :: SString(const SString & string)
  : str(NULL), len(string.len)
{
  copyFrom(string);
}
     
   // Destructor
INLINE SString :: ~SString()
{
  freeString();
}

   // Assignment operator
INLINE SString& SString :: operator = (const SString& string)
{
  freeString();
  copyFrom(string);
  
  return (*this);
}

     
   //--- Member functions ---//
     
   // Make a copy of the string
   // implement BaseObject class pure virtual function
INLINE BaseObjectPtr SString :: copy(void) const
{
  SStringPtr newstring = new SString(*this);
  return newstring;
}
     
   // Destroy the string
INLINE void SString :: destroy(void)
{
  freeString();
}

   // Append another SString to end of string
INLINE void SString :: append(const SString& string)
{
  append(string.str);
}

   // Append first 'n' characters from another SString to end of string
INLINE void SString :: append(const SString& string, size_t num)
{
  append(string.str, num);
}

   // Is this a NULL string
INLINE bool SString :: isNull(void)
{
  return ( str == NULL );
}

INLINE bool SString :: isEmpty(void)
{
  return isNull();
}
     

   //--- Member functions - operators ---//

   // Assignment from a character array
INLINE SString& SString :: operator = (const char * char_array)
{
  freeString();
  copyFrom(char_array);

  return(*this);
}
     
   // Extract character at given index
INLINE char SString :: operator [] (size_t pos) const
{
  if ( pos > len ) return NULL;
  
  return str[pos];
}

   // Convert to character array - cast operator
INLINE SString :: operator const char * () const
{
  return str;
}

   // Expand string by concatenation
INLINE SString& SString :: operator += (const char * char_array)
{
  append(char_array);
  return(*this);
}

INLINE SString& SString :: operator += (const SString& string)
{
  append(string);
  return (*this);
}

INLINE SString& SString :: operator += (char c)
{
  append(c);
  return (*this);
}


   //--- Friend functions - operators ---//
     
   // Concatenation
INLINE SString operator + (const SString& string, const char * char_array)
{
  SString new_string(string);
  new_string += char_array;
  
  return new_string;
}
     
INLINE SString operator + (const char * char_array, const SString& string)
{
  return ( string + char_array );
}
     
INLINE SString operator + (const SString& string1, const SString& string2)
{
  SString new_string(string1);
  new_string += string2;

  return new_string;
}

   // Comparison of strings
INLINE bool operator == (const SString& string, const char * char_array)
{
  return ( !strcmp(string.str,char_array) );
}
     
INLINE bool operator == (const char * char_array, const SString& string)
{
  return ( string == char_array );
}
     
INLINE bool operator == (const SString& string1, const SString& string2)
{
  return ( !strcmp(string1.str,string2.str) );
}
     
INLINE bool operator != (const char * char_array, const SString& string)
{
  return ( !(char_array == string) );
}
     
INLINE bool operator != (const SString& string, const char * char_array)
{
  return ( !(string == char_array) );
}
     
INLINE bool operator != (const SString& string1, const SString& string2)
{
  return ( !(string1 == string2) );
}

   // Insertion operator - output to an output stream
INLINE ostream& operator << (ostream& o, const SString& string)
{
  if ( string.len > 0 ) o << string.str;

  return o;
}

   //--- Friend functions ---//
     
   // Find the length of the string
INLINE size_t length(const SString& string)
{
  return string.len;
}

INLINE size_t strlen(const SString& string)
{
  return string.len;
}

#endif // #ifndef _SSTRING_INL_

#endif // #ifdef _SSTRING_HH_

// $Log: SString.inl,v $
// Revision 1.2  2002/10/17 01:36:19  vinod
// Modified to use typedefs for pointers
//
// Revision 1.1  2000/02/09 21:31:13  vinod
// Moved into String directory
//
// Revision 1.1.1.1  1999/01/30 22:31:19  vinod
// Utility Classes
//
// Revision 1.3  1998/05/05 00:22:20  vinod
// Renamed base class to BaseObject to avoid conflicts with X11
//
// Revision 1.2  1998/05/03 22:05:29  vinod
// Changes char * to const char * in constructor
//
// Revision 1.1.1.1  1998/05/03 21:44:53  vinod
// Utility classes, container classes
//
