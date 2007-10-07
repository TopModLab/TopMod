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

// $Id: BaseObject.hh,v 1.5 2002/10/25 15:59:19 vinod Exp $

#ifndef _OBJECT_HH_

#define _OBJECT_HH_

// Abstract base class for all classes
// Can use this class to build container classes, which use object pointers
// So the same container classes can be used with any derived classes also
// without need for additional code - share the same code

// BaseObject has no member data. It only has protected constructors (to prevent
// instantiation), virtual destructors and an assignment operator

#ifndef __GNUG__
#include <bool.h>
#include <iostream.h>
#else
#include <iostream>
#endif

#include <stdlib.h>

class BaseObject;
typedef BaseObject * BaseObjectPtr;

class BaseObject
{
  protected :

        // Default constructor - protected to prevent instantiation
     BaseObject()
       {
            // Nothing to do
       }

        // Copy constructor - protected to prevent instantiation
     BaseObject(const BaseObject&)
       {
            // Nothing to do
       }
     
  public :

        // Assignment operator
     BaseObject& operator = (const BaseObject&)
       {
            // Nothing to do

         return *this;
       }

        // Destructor
     virtual ~BaseObject()
       {
            // Nothing to do
       }

        // Derived class should give a meaningful implementation
        // for the following functions.
        // Classes such as List which use BaseObject pointers
        // will use these functions, for memory management
        // These functions may be made pure virtual later
     
        // Make a copy of the BaseObject and return a pointer to the new one
     virtual BaseObjectPtr copy(void) const = 0;
};

// A reference class for use in container classes
// Based on http://www.cs.southwestern.edu/~owensb/Algo/STL/ref2.html

class BaseObjectReference
{
  protected :

     BaseObjectPtr referent;                           // What does this object refer to?
     bool          owner;                              // Does this object owns the referent?

  public :

        //--- Constructors ---//

     BaseObjectReference()
       : referent(NULL), owner(true)
       {}

     BaseObjectReference(const BaseObject& obj)
       : referent(obj.copy()), owner(true)
       {}

     BaseObjectReference(BaseObjectPtr obj)
       : referent(obj), owner(false)
       {}

     BaseObjectReference(const BaseObjectReference& ref)
       : referent(ref.referent), owner(ref.owner)
       {
         if ( owner && ref.referent ) referent = ref.referent->copy();
       }

        //--- Destructor ---//

     ~BaseObjectReference()
       {
         if ( owner ) delete referent;
       }

        //--- Assignment operator ---//

     BaseObjectReference& operator = (const BaseObjectReference& ref)
       {
         if ( owner ) delete referent;
         owner = true;
         if ( ref.referent ) referent = ref.referent->copy();
         return (*this);
       }

        //--- Operators ---//

     BaseObjectPtr operator -> () const
       {
         return referent;
       }

     operator BaseObject& () const
       {
         return *referent;
       }

     operator BaseObject * () const
       {
         return referent;
       }

     BaseObject& operator * () const
       {
         return *referent;
       }
};

#endif // #ifndef _OBJECT_HH_

// $Log: BaseObject.hh,v $
// Revision 1.5  2002/10/25 15:59:19  vinod
// Removed namespace reference
//
// Revision 1.4  2002/10/25 15:40:47  vinod
// Added preprocessor directies for conditional includes depending on compiler
//
// Revision 1.3  2000/07/20 00:44:36  vinod
// Added BaseObjectReference class
//
// Revision 1.2  2000/02/15 21:57:39  vinod
// Remove unused parameters
//
// Revision 1.1  2000/02/09 21:17:46  vinod
// Moved into Base directory
//
// Revision 1.2  1999/10/18 05:33:26  vinod
// Added typedef. Made copy function const qualified
//
// Revision 1.1.1.1  1999/01/30 22:31:19  vinod
// Utility Classes
//
// Revision 1.1  1998/12/13 21:31:32  vinod
// Abstract Base class
//
// Revision 1.1.1.1  1998/05/03 21:44:53  vinod
// Utility classes, container classes
//
