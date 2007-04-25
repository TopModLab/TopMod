// $Id: Paragraph.cc,v 1.1 2000/02/09 21:31:13 vinod Exp $

// Implementation for the Paragraph class

#include "Paragraph.hh"

//--- Friend functions - operators ---//

   // Extraction operator - input from an input stream
istream& operator >> (istream& i, Paragraph& paragraph)
{
     // Free the existing paragraph
  paragraph.destroy();

  char c;
  
     // Remove leading empty lines
  while ( i )
     {
          // Get the next character from the input stream
       i.get(c);

          // If it is a valid character, break out of loop
       if ( !(c == '\n') )
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

     // Add terminating NULL character to buffer
  buffer[buflen-1] = NULL;

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
       if ( c == '\n' )
          break;

          // Valid character, add it to end of buffer
       buffer[count] = c; count++;

          // Check if buffer is full
       if ( count >= buflen-1 )
          {
               // Append 'count' characters from buffer to paragraph
            paragraph.append(buffer,count);
            count = 0;
          }
     }

     // Append the characters in the buffer, if any, to paragraph
     // Terminating NULL is automatically added
  paragraph.append(buffer,count);

  return i;
}

// $Log: Paragraph.cc,v $
// Revision 1.1  2000/02/09 21:31:13  vinod
// Moved into String directory
//
// Revision 1.1.1.1  1999/01/30 22:31:19  vinod
// Utility Classes
//
// Revision 1.1.1.1  1998/05/03 21:44:53  vinod
// Utility classes, container classes
//
