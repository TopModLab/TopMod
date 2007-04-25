/* $Id: ListNode.hh,v 1.1 2000/02/09 21:34:35 vinod Exp $ */

#ifndef _LIST_NODE_HH_

#define _LIST_NODE_HH_

#include "../Base/BaseObject.hh"
#include <stddef.h>

typedef BaseObjectPtr   Value;
typedef Value&          Reference;
typedef const Value&    ConstReference;

// Class for a node of the list. Declared outside the list class to allow other
// classes to be derived from this (which can redefine the next() and prev() operators

// Abstract Base Class for list node - defines interface only
class ListNode;
typedef ListNode *      ListNodePtr;

class ListNode
{
  public :

     ListNode()
       {}

     ListNode(const ListNode&)
       {}

     virtual ~ListNode()
       {}

     ListNode& operator = (const ListNode&)
       {
         return (*this);
       }

     virtual ListNodePtr prev(void) = 0;
     virtual ListNodePtr next(void) = 0;
     virtual Reference data(void) = 0;

     virtual void setPrev(ListNodePtr) = 0;
     virtual void setNext(ListNodePtr) = 0;
     virtual void setData(ConstReference) = 0;
     
}; // class ListNode


// List node for list of pointers - derived from ListNode ABC
// This node stores the next and prev pointers separately

class PtrListNode;
typedef PtrListNode * PtrListNodePtr;

class PtrListNode : public ListNode
{
  protected :

     Value       bopData;                              // Pointer to actual data
     ListNodePtr lnpNext;                              // Pointer to next node
     ListNodePtr lnpPrev;                              // Pointer to previous node

  public :
     
        // Default constructor
     PtrListNode()
       : ListNode(), bopData(NULL), lnpNext(NULL), lnpPrev(NULL)
       {}

        // Copy constructor
     PtrListNode(const PtrListNode& pln)
       : ListNode(pln), bopData(pln.bopData), lnpNext(pln.lnpNext), lnpPrev(pln.lnpPrev)
       {}
     
        // Destructor
     ~PtrListNode()
       {}

        // Assignment operator
     PtrListNode& operator = (const PtrListNode& pln)
       {
         ListNode :: operator = (pln);
         bopData = pln.bopData; lnpNext = pln.lnpNext; lnpPrev = pln.lnpPrev;
         return (*this);
       }
     
     ListNodePtr next(void)                            // Return pointer to next node
       {
         return lnpNext;
       }

     ListNodePtr prev(void)                            // Return pointer to prev node
       {
         return lnpPrev;
       }

     Reference data(void)                              // Return the pointer to the data
       {
         return bopData;
       }

     void setNext(ListNodePtr lnp)
       {
         lnpNext = lnp;
       }

     void setPrev(ListNodePtr lnp)
       {
         lnpPrev = lnp;
       }

     void setData(ConstReference bop)
       {
         bopData = bop;
       }

}; // class PtrListNode

#endif /* #ifndef _LIST_NODE_HH_ */


/*
  $Log: ListNode.hh,v $
  Revision 1.1  2000/02/09 21:34:35  vinod
  Moved into DataStructures directory

  Revision 1.1  2000/01/15 01:22:39  vinod
  ListNode class

*/
