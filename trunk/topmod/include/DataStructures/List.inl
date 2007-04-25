// $Id: List.inl,v 1.1 2000/02/09 21:34:35 vinod Exp $

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

#ifdef LIST_DEBUG
#define INLINE
#include "List.hh"
#else
#define INLINE inline
#endif // #ifdef LIST_DEBUG

// The header file should be included before this file
// (in both debug and non-debug modes)

#ifdef _LIST_HH_

#ifndef _LIST_INL_

#define _LIST_INL_

// Some Macros for the various types and classes defined in class List
// to simplify writing and improve clarity

#define Reference List::Reference
#define ConstReference List::ConstReference
#define ListIterator List::Iterator
#define ListConstIterator List::ConstIterator

//--- class List::Iterator ---//

   //--- Comparison operators ---//
          
INLINE bool ListIterator :: operator == (const ListIterator& itr) const // Equality operator
{
  return node == itr.node;
}

INLINE bool ListIterator :: operator != (const ListIterator& itr) const // Inequality operator
{
  return node != itr.node;
}

//--- end of class List::Iterator ---//


//--- class List::ConstIterator ---//

   //--- Comparison operators ---//
          
INLINE bool ListConstIterator :: operator == (const ListConstIterator& citr) const
{                                                      // Equality operator
  return node == citr.node;
}

INLINE bool ListConstIterator :: operator != (const ListConstIterator& citr) const
{                                                      // Inequality operator
  return node != citr.node;
}

//--- end of class List::ConstIterator ---//


//--- class List ---//

INLINE List :: List()                                  // Default constructor
  : node(NULL), length(0)
{
     // Allocate memory for the control node
     // which will always be at the end of the list
  node = new Link;
  node->prev = node;  node->next = node;
}

INLINE Reference List :: front(void)                   // Value at front of list
{
  return *begin();
}

INLINE ConstReference List :: front(void) const        // Value at front of list
{
  return *begin();
}

INLINE Reference List :: back(void)                    // Value at end of list
{
  ListIterator temp = end();
  --temp;
  return *temp;
}

INLINE ConstReference List :: back(void) const         // Value at end of list
{
  ListConstIterator temp = end();
  --temp;
  return *temp;
}

   // Insert a value at a specified position
INLINE void List :: insert(ListIterator& pos, ConstReference data) // Insertion
{
     // Create a new node
  LinkPtr temp = new Link;
  LinkPtr itr_node = pos.node;  // Node referred to by Iterator

     // Doubly linked list with control node - equivalent to a circular list

     // Set links of new node
     // Store a copy of the data
  temp->data = data->copy();
  temp->next = itr_node;
  temp->prev = itr_node->prev;

     // Update links of adjacent old nodes
  (itr_node->prev)->next = temp;
  itr_node->prev = temp;

  ++length;
}

   // Insert a range of values at a specified position
INLINE void List :: insert(ListIterator& pos,
                           ListConstIterator& first, ListConstIterator& last) // Insertion
{
     // Assumes that 'last' can be reached from 'first' through a finite
     // number of increment ( ++ ) operations
     // Otherwise this will go into an infinite loop!
  while ( first != last )
     {
       insert(pos, first.dereference());
       first.increment();
     }
}

INLINE void List :: push_front(ConstReference data)    // Insert at beginning 
{
  ListIterator temp = begin();
  insert( temp, data );
}

INLINE void List :: push_back(ConstReference data)     // Insert at end
{
  ListIterator temp = end();
  insert( temp, data );
}

INLINE void List :: erase(ListIterator& pos)            // Erase specified node
{
  LinkPtr itr_node = pos.node;  // Node referred to by Iterator

     // Free memory associated with the data field
     // Can we just say 'delete itr_node->data' ??
  delete (itr_node->data);
  itr_node->data = NULL;
  
     // Update adjacent links
  (itr_node->prev)->next = itr_node->next;
  (itr_node->next)->prev = itr_node->prev;

     // Delete the memory allocated for node
  delete itr_node;
  
  --length;
}

INLINE void List :: erase(ListIterator& first, ListIterator& last) // Erase range of nodes
{
     // Assumes that 'last' can be reached from 'first' through a finite
     // number of increment ( ++ ) operations
  while (first != last)
     {
       erase(first); ++first;
     }
}

INLINE void List :: erase(void)                        // Erase all nodes
{
  ListIterator temp1,temp2;
  temp1 = begin(); temp2 = end();
  erase( temp1, temp2);
}

INLINE void List :: pop_front(void)                    // Delete first node
{
  ListIterator temp = begin();
  erase(temp);
}

INLINE void List :: pop_back(void)                     // Delete last node
{
  ListIterator temp = end();
  --temp;        // end() returns NULL node at end of list
  erase(temp);
}

INLINE List :: List(const List& new_list)              // Copy constructor
  : node(NULL), length(0)
{
     // Create control node and insert the new list at the beginning
  node = new Link;
  node->prev = node;  node->next = node;
  ListIterator temp1;
  ListConstIterator temp2,temp3;
  temp1 = begin();
  temp2 = new_list.begin();
  temp3 = new_list.end();
  insert(temp1,temp2,temp3);
}

INLINE List :: ~List()                                 // Destructor
{
     // Erase the whole list and delete the control node
  erase();
  delete node;
}

INLINE List& List :: operator = (const List& new_list) // Assignment operator
{
  if ( this != &new_list )
     {
       ListIterator first1 = begin();
       ListIterator last1 = end();
       ListConstIterator first2 = new_list.begin();
       ListConstIterator last2 = new_list.end();

       while ( (first1 != last1) && (first2 != last2) )
          {
            first1.dereference() = first2.dereference();  ++first1;  ++first2;
          }

       if (first2 == last2)     erase(first1, last1);
       else                     insert(last1, first2, last2);
     }

  return *this;
}
     
//--- end of class List ---//

#endif // #ifndef _LIST_INL_

#endif // #ifdef _LIST_HH_

// $Log: List.inl,v $
// Revision 1.1  2000/02/09 21:34:35  vinod
// Moved into DataStructures directory
//
// Revision 1.1.1.1  1999/01/30 22:31:19  vinod
// Utility Classes
//
// Revision 1.3  1998/12/15 23:44:31  vinod
// Moved inline functions to header file
//
// Revision 1.2  1998/12/13 21:29:39  vinod
// Modifications to allow derived lists
//
// Revision 1.1.1.1  1998/05/03 21:44:53  vinod
// Utility classes, container classes
//
