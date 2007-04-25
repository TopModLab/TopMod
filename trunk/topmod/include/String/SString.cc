// $Id: SString.cc,v 1.1 2000/02/09 21:31:12 vinod Exp $

// Source code for the Simple String class

#include "SString.hh"

// Include the inline function definitions

#include "SString.inl"

// Non-inline functions begin here

//--- Private member functions ---//

   // Copy from another SString
void SString :: copyFrom(const SString& string)
{
  if ( string.len > 0 )
     {
       len = string.len;
       str = new char[len+1];
       strcpy(str,string.str);
     }
}

   // Copy from a character array
void SString :: copyFrom(const char * char_array)
{
  if ( char_array )
     {
       len = strlen(char_array);
       str = new char[len+1];
       strcpy(str,char_array);
     }
}

//--- Member functions ---//

   // Append a character array to end of string
void SString :: append(const char * char_array)
{
  if ( char_array == NULL ) return;
  
  char * new_string = NULL;
  size_t calen = strlen(char_array);
  
  new_string = new char[ len + calen + 1 ];

  register unsigned int i;

     // Copy original string
  for (i=1; i <= len; i++)
     new_string[i-1] = str[i-1];

     // Append character array
  for (i=1; i <= calen; i++)
     new_string[len+i-1] = char_array[i-1];

     // Append trailing NULL
  new_string[len+calen] = char(0);
  
  freeString();
  copyFrom(new_string);

  delete [] new_string;
}

   // Append first 'n' characters from a character array to end of string
void SString :: append(const char * char_array, size_t num)
{
  if ( char_array == NULL ) return;

  size_t calen = strlen(char_array);

  if ( num > calen ) num = calen;

  char * new_string = NULL;

  new_string = new char[ len + num + 1 ];

  register unsigned int i;

     // Copy original string
  for (i=1; i <= len; i++)
     new_string[i-1] = str[i-1];

     // Append character array
  for (i=1; i <= num; i++)
     new_string[len+i-1] = char_array[i-1];
  
     // Append trailing NULL
  new_string[len+num] = char(0);

  freeString();
  copyFrom(new_string);

  delete [] new_string;
}

   // Append a character to end of string
void SString :: append(char c)
{
  char * new_string = NULL;

  new_string = new char[len + 1];

  register unsigned int i;

     // Copy original string
  for (i=1; i <= len; i++)
     new_string[i-1] = str[i-1];
  
     // Append character
  new_string[len] = c;

     // Append trailing NULL
  new_string[len+1] = char(0);

  freeString();
  copyFrom(new_string);

  delete [] new_string;
}

//--- Friend functions - operators ---//

        // Extraction operator - input from an input stream
istream& operator >> (istream& i, SString& string)
{
     // Free the existing string
  string.destroy();

  char c;
  
     // Remove leading white space
  while ( i )
     {
          // Get the next character from the input stream
       i.get(c);

          // If it is a valid character, break out of loop
       if ( !( (c == ' ') || (c == '\n') || (c == '\t') ) )
          break;
     }

     // If we came out of loop because the input stream became empty
     // just return without doing anything
  if ( !i ) return i;

     // Allocate a temporary buffer to store the characters from
     // the input stream
  const int buflen=25;
  int count;
  char * buffer = new char[buflen];

     // Make first and last characters a NULL character
  buffer[0] = char(0); buffer[buflen-1] = char(0);

  count = 0;
     // Valid character, add it to beginning of buffer
  buffer[count] = c; count++;

     // No need to check if buffer is full, since any reasonable value for
     // the buffer length, will be greater than 1

     // Read the characters till next white-space character
  while ( i )
     {
          // Get the next character from the input stream
       i.get(c);

          // If invalid character break;
       if ( (c == ' ') || (c == '\n') || (c == '\t') )
          break;

          // Valid character, add it to end of buffer
       buffer[count] = c; count++;

          // Check if buffer is full
       if ( count >= buflen-1 )
          {
               // Append 'count' characters from buffer to string
            string.append(buffer,count);
            count = 0;
          }
     }

     // Append the characters in the buffer, if any, to string
     // The append function automatically adds a trailing NULL character
  string.append(buffer,count);

  return i;
}

// $Log: SString.cc,v $
// Revision 1.1  2000/02/09 21:31:12  vinod
// Moved into String directory
//
// Revision 1.1.1.1  1999/01/30 22:31:19  vinod
// Utility Classes
//
// Revision 1.1.1.1  1998/05/03 21:44:53  vinod
// Utility classes, container classes
//
