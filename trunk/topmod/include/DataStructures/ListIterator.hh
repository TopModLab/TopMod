/* $Id: ListIterator.hh,v 1.1 2000/02/09 21:34:36 vinod Exp $ */

#ifndef _LIST_ITERATOR_HH_

#define _LIST_ITERATOR_HH_

//#include <bool.h>
#include "ListNode.hh"

typedef ListNode        Link;
typedef Link *          LinkPtr;

// Generic Iterator for the list
class Iterator
{
  protected :

     LinkPtr node;                                     // A node of the list

     Iterator(LinkPtr new_node)                        // 1 arg constructor
       : node(new_node)
       {}

     void increment(void)                              // Increment function
       {
         node = node->next();
       }
          
     void decrement(void)                              // Decrement function
       {
         node = node->prev();
       }

     Reference dereference(void) const                 // Dereference function
       {
         return node->data();
       }

  private :

     friend class List;
     friend class ConstIterator;
          
  public :

     Iterator()                                        // Default constructor
       : node(NULL)
       {}

     Iterator(const Iterator& itr)                     // Copy constructor
       : node(itr.node)
       {}

     ~Iterator()                                       // Destructor
       {
         node = NULL;
       }

     Iterator& operator = (const Iterator& itr)        // Assignment operator
       {
         node = itr.node;
         return (*this);
       }
          
        //--- Comparison operators ---//
          
     bool operator == (const Iterator& itr) const      // Equality operator
       {
         return node == itr.node;
       }
     
     bool operator != (const Iterator& itr) const      // Inequality operator
       {
         return node != itr.node;
       }
     
     Reference operator * () const                     // Dereference operator
       {
         return this->dereference();
       }
     
        //--- Manipulators ---//
          
     void operator ++ ()                               // Prefix increment
       {
         increment();
       }
          
     void operator ++ (int)                            // Postfix increment
       {
         increment();
       }

     void operator -- ()                               // Prefix decrement
       {
         decrement();
       }
          
     void operator -- (int)                            // Postfix decrement
       {
         decrement();
       }

}; // class Iterator
     

// Generic Iterator for the list - const version
class ConstIterator
{
  protected :

     LinkPtr node;                                     // A node of the list

     ConstIterator(LinkPtr new_node)                   // 1 arg constructor
       : node(new_node)
       {}

     void increment(void)                              // Increment function
       {
         node = node->next();
       }
          
     void decrement(void)                              // Decrement function
       {
         node = node->prev();
       }

     ConstReference dereference(void) const            // Dereference function
       {
         return node->data();
       }

  private :

     friend class List;
     
  public :

     ConstIterator()                                   // Default constructor
       : node(NULL)
       {}

     ConstIterator(const ConstIterator& itr)           // Copy constructor
       : node(itr.node)
       {}

     ConstIterator(const Iterator& itr)                // Construct from Iterator
       : node(itr.node)
       {}

     ~ConstIterator()                                  // Destructor
       {
         node = NULL;
       }

     ConstIterator& operator = (const ConstIterator& itr) // Assignment operator
       {
         node = itr.node;
         return (*this);
       }
          
     ConstIterator& operator = (const Iterator& itr)   // Assignment from Iterator
       {
         node = itr.node;
         return (*this);
       }
          
        //--- Comparison operators ---//
          
     bool operator == (const ConstIterator& itr) const // Equality operator
       {
         return node == itr.node;
       }
     
     bool operator != (const ConstIterator& itr) const // Inequality operator
       {
         return node != itr.node;
       }
     
     ConstReference operator * () const                // Dereference operator
       {
         return this->dereference();
       }
     
        //--- Manipulators ---//
          
     void operator ++ ()                               // Prefix increment
       {
         increment();
       }
          
     void operator ++ (int)                            // Postfix increment
       {
         increment();
       }

     void operator -- ()                               // Prefix decrement
       {
         decrement();
       }
          
     void operator -- (int)                            // Postfix decrement
       {
         decrement();
       }

}; // class ConstIterator

#endif /* #ifndef _LIST_ITERATOR_HH_ */


/*
  $Log: ListIterator.hh,v $
  Revision 1.1  2000/02/09 21:34:36  vinod
  Moved into DataStructures directory

  Revision 1.1  2000/01/15 01:22:51  vinod
  ListIterator class

*/
