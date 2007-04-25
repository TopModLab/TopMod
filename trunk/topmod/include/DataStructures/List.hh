// $Id: List.hh,v 1.1 2000/02/09 21:34:35 vinod Exp $

#ifndef _LIST_HH_

#define _LIST_HH_

#include "../Base/BaseObject.hh"
//#include <bool.h>
#include "ListNode.hh"
#include "ListIterator.hh"

// Linked list of BaseObject pointers

typedef unsigned int uint;
typedef size_t Size;

// The generic List class. The node is a pointer to a ListNode which is an ABC
// This list uses the PtrListNode as the node of the list
// Derived lists can use some other node derived from ListNode or PtrListNode

// This list is derived from BaseObject to allow list of lists

class List;
typedef List * ListPtr;

typedef void (*FunctionPtr)(Reference data);
typedef void (*ConstFunctionPtr)(ConstReference data);

class List : public BaseObject
{
  protected :

        //--- List class member data ---//
     
     LinkPtr  node;                                    // Pointer to first node
     Size     length;                                  // Length of list
     bool     useCopy;                                 // Flag to indicate if the copy() function
                                                       // should be used during insertion
                                                       // Default is true
     
  public :
     
        //--- List class member functions ---//

     List();                                           // Default constructor
     List(const List& new_list);                       // Copy constructor
     ~List();                                          // Destructor
     List& operator = (const List& new_list);          // Assignment operator

     
     virtual BaseObjectPtr copy(void) const            // Copy function - implement base class fn
       {
         ListPtr newlist = new List(*this);
         return newlist;
       }
     
     void setCopyFlag(void)                            // Set the copy flag to true
       {
         useCopy = true;
       }

     void resetCopyFlag(void)                          // Reset the copy flag to false
       {
         useCopy = false;
       }
     
     bool empty(void) const                            // Is list empty
       {
         return length == 0;
       }

     Size size(void) const                             // Size of list
       {
         return length;
       }

     Iterator begin(void)                              // Beginning of list
       {
         return Iterator(node->next());
       }

     ConstIterator begin(void) const                   // Beginning of list
       {
         return ConstIterator(node->next());
       }

     Iterator end(void)                                // End of list
       {
         return Iterator(node);
       }

     ConstIterator end(void) const                     // End of list
       {
         return ConstIterator(node);
       }

     Reference front(void);                            // Value at front of list
     ConstReference front(void) const;                 // Value at front of list

     Reference back(void);                             // Value at end of list
     ConstReference back(void) const;                  // Value at end of list

        //-- Insertion functions --//

        // Insert a single value
     void insertBefore(Iterator& pos, ConstReference data); // Insertion before
     void insertAfter(Iterator& pos, ConstReference data); // Insertion after
     void insert(Iterator& pos, ConstReference data)   // Default insertion
       {
         insertAfter(pos,data);
       }

        // Insert a range of values - assumes last can be reached from first
        // through finite number of ++ operations
     void insertBefore(Iterator& pos, ConstIterator& first, ConstIterator& last);
     void insertAfter(Iterator& pos, ConstIterator& first, ConstIterator& last);
     void insert(Iterator& pos, ConstIterator& first, ConstIterator& last)
       {
         insertAfter(pos,first,last);
       }

     void push_front(ConstReference data);             // Insert at beginning 
     void push_back(ConstReference data);              // Insert at end

     
        //-- Deletion functions --//

     void erase(Iterator& pos);                        // Erase specified node
     void erase(ConstReference data);                  // Erase node with specified data
     void erase(Iterator& first, Iterator& last);      // Erase range of nodes
     void erase(void);                                 // Erase all nodes
     
     void pop_front(void);                             // Delete first node
     void pop_back(void);                              // Delete last node

        //-- Search functions --//

     Iterator getNode(ConstReference data);            // Get node with specified data
     ConstIterator getNode(ConstReference data) const; // const version
     
        //-- Array type access functions --//

     Value get(uint i) const;                 // Get i'th element
     void set(uint i, ConstReference data);            // Set i'th element to given data

        //-- Access list as circular list --//

     Iterator next(Iterator pos);                      // Get next valid node
     ConstIterator next(ConstIterator pos) const;      // const version

     Iterator prev(Iterator pos);                      // Get prev valid node
     ConstIterator prev(ConstIterator pos) const;      // const version
     
        //-- Functions to access whole list --//

     void apply(FunctionPtr func);                     // Apply given function to all elements
     void apply(ConstFunctionPtr func) const;          // const version
};

#endif // #ifndef _LIST_HH_ 

// $Log: List.hh,v $
// Revision 1.1  2000/02/09 21:34:35  vinod
// Moved into DataStructures directory
//
// Revision 1.2  2000/01/15 01:23:36  vinod
// Major changes. Restructured
//
// Revision 1.1.1.1  1999/01/30 22:31:19  vinod
// Utility Classes
//
// Revision 1.5  1998/12/15 23:44:29  vinod
// Moved inline functions to header file
//
// Revision 1.4  1998/12/15 23:05:56  vinod
// *** empty log message ***
//
// Revision 1.3  1998/12/13 21:29:38  vinod
// Modifications to allow derived lists
//
// Revision 1.2  1998/05/05 00:22:18  vinod
// Renamed base class to BaseObject to avoid conflicts with X11
//
// Revision 1.1.1.1  1998/05/03 21:44:53  vinod
// Utility classes, container classes
//
