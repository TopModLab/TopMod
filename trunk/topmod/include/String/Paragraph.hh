// $Id: Paragraph.hh,v 1.2 2002/10/17 01:35:51 vinod Exp $

#ifndef _PARAGRAPH_HH_

#define _PARAGRAPH_HH_

#include "SString.hh"

// Paragraph class - derived from class SString
// Only difference is that spaces and tab characters are allowed
// in Paragraphs which are not in strings
// So nothing to add, except the extraction operator

class Paragraph;
typedef Paragraph * ParagraphPtr;

class Paragraph : public SString
{
  protected :

        //--- Member data ---//

  public :

        // Default constructor
     Paragraph()
       : SString()
       {
            // nothing to do
       }

        // Construct from character array
     Paragraph(const char * char_array)
       : SString(char_array)
       {
            // nothing to do
       }

        // Construct from a SString
     Paragraph(const SString& string)
       : SString(string)
       {
            // nothing to do
       }
     
        // Copy constructor
     Paragraph(const Paragraph& para)
       : SString(para)
       {
            // nothing to do
       }

        // Destructor
     virtual ~Paragraph()
       {
            // nothing to do
       }

        // Assignment operator
     Paragraph& operator = (const Paragraph& para)
       {
         SString :: operator = (para);

         return (*this);
       }

     
        //--- Member functions ---//

        // Make a copy of the paragraph
        // implement BaseObject class pure virtual function
     virtual BaseObjectPtr copy(void) const
       {
         ParagraphPtr newpara = new Paragraph(*this);
         return newpara;
       }
     
        // Nothing else to add to the member functions inherited
        // from base class SString

     
        //--- Friend functions - operators ---//

        // Since SString class friend functions all work using references
        // they will automatically work with Paragraphs also
        // Extraction operator has to be redefined
     
        // Extraction operator - input from an input stream
     friend istream& operator >> (istream& i, Paragraph& paragraph);
};

#endif // #ifndef _PARAGRAPH_HH_

// $Log: Paragraph.hh,v $
// Revision 1.2  2002/10/17 01:35:51  vinod
// Modified to use typedefs for pointers
//
// Revision 1.1  2000/02/09 21:31:14  vinod
// Moved into String directory
//
// Revision 1.1.1.1  1999/01/30 22:31:19  vinod
// Utility Classes
//
// Revision 1.4  1998/05/05 00:22:19  vinod
// Renamed base class to BaseObject to avoid conflicts with X11
//
// Revision 1.3  1998/05/03 23:28:39  vinod
// Added constructor from SString
//
// Revision 1.2  1998/05/03 22:05:07  vinod
// Added constructor from char *
//
// Revision 1.1.1.1  1998/05/03 21:44:53  vinod
// Utility classes, container classes
//
