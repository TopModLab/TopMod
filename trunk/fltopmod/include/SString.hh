// $Id: SString.hh,v 1.1 2006/09/04 19:22:53 stuart Exp $

#ifndef _STRING_HH_

#define _STRING_HH_

#include <string.h>
#include <bool.h>
#include <iostream.h>
#include "BaseObject.hh"

// A Simple String class
class SString;
typedef SString * SStringPtr;

class SString : public BaseObject
{
  protected :

        //--- Member data ---//
     
     char *     str;                                   // The actual string itself
     size_t     len;                                   // Length of the string

        //--- Utility functions ---//

        // Free the internal string
     void freeString(void);

        // Copy from another SString
     void copyFrom(const SString& string);

        // Copy from a character array
     void copyFrom(const char * char_array);

  public :
     
        // Default constructor
     SString();

        // Construct from a character array
     SString(const char * char_array);

        // Copy constructor
     SString(const SString & string);
     
        // Destructor
     virtual ~SString();

        // Assignment operator
     SString& operator = (const SString& string);

     
        //--- Member functions ---//
     
        // Make a copy of the string
        // implement BaseObject class pure virtual function
     virtual BaseObjectPtr copy(void) const;

        // Destroy the string
     void destroy(void);

        // Append a character array to end of string
     void append(const char * char_array);

        // Append first 'n' characters from a character array to end of string
     void append(const char * char_array, size_t num);
     
        // Append another SString to end of string
     void append(const SString& string);

        // Append first 'n' characters from another SString to end of string
     void append(const SString& string, size_t num);
     
        // Append a character to the end of the string
     void append(char c);
     
        // Is this a NULL string
     bool isNull(void);
     bool isEmpty(void);

     
        //--- Member functions - operators ---//

        // Assignment from a character array
     SString& operator = (const char * char_array);
     
        // Extract character at given index
     char operator [] (size_t pos) const;

        // Convert to character array - cast operator
     operator const char * () const;

        // Expand string by concatenation
     SString& operator += (const char * char_array);
     SString& operator += (const SString& string);
     SString& operator += (char c);

     
        //--- Friend functions - operators ---//
     
        // Concatenation
     friend SString operator + (const SString& string, const char * char_array);
     friend SString operator + (const char * char_array, const SString& string);
     friend SString operator + (const SString& string1, const SString& string2);

        // Comparison of strings
     friend bool operator == (const SString& string, const char * char_array);
     friend bool operator == (const char * char_array, const SString& string);
     friend bool operator == (const SString& string1, const SString& string2);

     friend bool operator != (const char * char_array, const SString& string);
     friend bool operator != (const SString& string, const char * char_array);
     friend bool operator != (const SString& string1, const SString& string2);

        // Insertion operator - output to an output stream
     friend ostream& operator << (ostream& o, const SString& string);

        // Extraction operator - input from an input stream
     friend istream& operator >> (istream& i, SString& string);

     
        //--- Friend functions ---//
     
        // Find the length of the string
     friend size_t length(const SString& string);
     friend size_t strlen(const SString& string);
};

// Include the inline function definitions unless we are compiling in debug mode
#ifndef STRING_DEBUG
#include "SString.inl"
#endif // #ifndef STRING_DEBUG

#endif // #ifndef _STRING_HH_

// $Log: SString.hh,v $
// Revision 1.1  2006/09/04 19:22:53  stuart
// Added includes and libs.
//
// Revision 1.2  2002/10/17 01:36:04  vinod
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
