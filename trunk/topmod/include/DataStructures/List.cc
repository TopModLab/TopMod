// $Id: List.cc,v 1.1 2000/02/09 21:34:34 vinod Exp $

// Source code for the generic List class

#include "List.hh"

List :: List()                                  // Default constructor
  : node(NULL), length(0), useCopy(true)
{
     // Allocate memory for the control node
     // which will always be at the end of the list
  node = new PtrListNode;
  node->setPrev(node);  node->setNext(node);
}

Reference List :: front(void)                          // Value at front of list
{
  return *begin();
}

ConstReference List :: front(void) const               // Value at front of list (const)
{
  return *begin();
}

Reference List :: back(void)                           // Value at end of list
{
  Iterator temp = end();
  --temp;
  return *temp;
}

ConstReference List :: back(void) const                // Value at end of list (const)
{
  ConstIterator temp = end();
  --temp;
  return *temp;
}

   // Insert a value before a specified position
void List :: insertBefore(Iterator& pos, ConstReference data)// Insertion
{
     // Create a new node
  ListNodePtr temp = new PtrListNode;
  LinkPtr itr_node = pos.node;                         // Node referred to by Iterator

  if ( useCopy ) temp->setData(data->copy());
  else           temp->setData(data);
  
  temp->setNext(itr_node);
  temp->setPrev(itr_node->prev());

     // Update links of adjacent old nodes
  (itr_node->prev())->setNext(temp);
  itr_node->setPrev(temp);

  ++length;
}

   // Insert a value after a specified position
void List :: insertAfter(Iterator& pos, ConstReference data)// Insertion
{
     // Create a new node
  ListNodePtr temp = new PtrListNode;
  LinkPtr itr_node = pos.node;                         // Node referred to by Iterator

  if ( useCopy ) temp->setData(data->copy());
  else           temp->setData(data);
  
  temp->setNext(itr_node->next());
  temp->setPrev(itr_node);

     // Update links of adjacent old nodes
  (itr_node->next())->setPrev(temp);
  itr_node->setNext(temp);

  ++length;
}

   // Insert a range of values before a specified position
void List :: insertBefore(Iterator& pos,ConstIterator& first, ConstIterator& last)
{
     // Assumes that 'last' can be reached from 'first' through a finite
     // number of increment ( ++ ) operations
     // Otherwise this will go into an infinite loop!
  while ( first != last )
     {
       insertBefore(pos, *first);
       ++first;
     }
}

   // Insert a range of values after a specified position
void List :: insertAfter(Iterator& pos, ConstIterator& first, ConstIterator& last)
{
     // Assumes that 'last' can be reached from 'first' through a finite
     // number of increment ( ++ ) operations
     // Otherwise this will go into an infinite loop!

     // Insert in reverse order to preserve original sequence of first to last
  while ( last != first )
     {
       insertAfter(pos, *last);
       --last;
     }
}

void List :: push_front(ConstReference data)           // Insert at beginning 
{
  Iterator temp = begin();
  insertBefore(temp, data);
}

void List :: push_back(ConstReference data)            // Insert at end
{
  Iterator temp = end();
  insertBefore(temp, data);
}

void List :: erase(Iterator& pos)                      // Erase specified node
{
  LinkPtr itr_node = pos.node;  // Node referred to by Iterator

     // Free memory associated with the data field
     // Can we just say 'delete itr_node->data' ??

     // This needs to be modified, since we can't access the pointer directly
  if ( useCopy ) delete (itr_node->data());
  itr_node->setData(NULL);
  
     // Update adjacent links
  (itr_node->prev())->setNext(itr_node->next());
  (itr_node->next())->setPrev(itr_node->prev());

     // Delete the memory allocated for node
  delete itr_node;
  
  --length;
}

void List :: erase(ConstReference data)                // Erase node with specified data
{
  Iterator first = begin();
  Iterator last = end();

  while ( first != last )
     {
       if ( *first == data )
          {
            erase(first); break;
          }
       ++first;
     }
}

void List :: erase(Iterator& first, Iterator& last)    // Erase range of nodes
{
     // Assumes that 'last' can be reached from 'first' through a finite
     // number of increment ( ++ ) operations
  while (first != last)
     {
       erase(first); ++first;
     }
}

void List :: erase(void)                               // Erase all nodes
{
  Iterator temp1,temp2;
  temp1 = begin(); temp2 = end();
  erase( temp1, temp2);
}

void List :: pop_front(void)                           // Delete first node
{
  Iterator temp = begin();
  erase(temp);
}

void List :: pop_back(void)                            // Delete last node
{
  Iterator temp = end();
  --temp;        // end() returns NULL node at end of list
  erase(temp);
}

List :: List(const List& new_list)                     // Copy constructor
  : node(NULL), length(0), useCopy(new_list.useCopy)
{
     // Create control node and insert the new list at the beginning
  node = new PtrListNode;
  node->setPrev(node);  node->setNext(node);

  Iterator temp1;
  ConstIterator temp2,temp3;
  temp1 = begin();
  temp2 = new_list.begin();
  temp3 = new_list.end();
  insert(temp1,temp2,temp3);
}

List :: ~List()                                        // Destructor
{
     // Erase the whole list and delete the control node
  erase();
  delete node;
}

List& List :: operator = (const List& new_list)        // Assignment operator
{
     // Does not change useCopy flag.
  if ( this != &new_list )
     {
          // Erase all existing nodes and insert new list at beginning
       erase();
       
       Iterator first1 = begin();
       ConstIterator first2 = new_list.begin();
       ConstIterator last2 = new_list.end();
       
       insert(first1, first2, last2);
     }

  return *this;
}

Iterator List :: getNode(ConstReference data)          // Get node with specified data
{
  Iterator first = begin();
  Iterator last = end();

  while ( first != last )
     {
       if ( *first == data ) return first;
       ++first;
     }
  
  return Iterator();
}

ConstIterator List :: getNode(ConstReference data) const // const version
{
  ConstIterator first = begin();
  ConstIterator last = end();

  while ( first != last )
     {
       if ( *first == data ) return first;
       ++first;
     }

  return ConstIterator();
}

Value List :: get(uint i) const               // Get i'th element
{
  ConstIterator first = begin();
  ConstIterator last = end();
  uint count=0;
  Value element = NULL;
  
  while (first != last)
     {
       if ( count == i )
          {
            element = *first; break;
          }
       ++count; ++first;
     }
  return element;
}

void List :: set(uint i, ConstReference data)          // Set i'th element to given data
{
  Iterator first = begin();
  Iterator last = end();
  uint count=0;
  
  while (first != last)
     {
       if ( count == i )
          {
            first.node->setData(data); break;
          }
       ++count; ++first;
     }
}

Iterator List :: next(Iterator pos)                    // Get next valid node
{
  Iterator node = pos;
  ++node;
  if ( node == end() ) ++node;
  return node;
}

ConstIterator List :: next(ConstIterator pos) const    // const version
{
  ConstIterator node = pos;
  ++node;
  if ( node == end() ) ++node;
  return node;
}

Iterator List :: prev(Iterator pos)                    // Get prev valid node
{
  Iterator node = pos;
  --node;
  if ( node == end() ) --node;
  return node;
}

ConstIterator List :: prev(ConstIterator pos) const    // const version
{
  ConstIterator node = pos;
  --node;
  if ( node == end() ) --node;
  return node;
}

void List :: apply(FunctionPtr func)                   // Apply given function to all elements
{
  Iterator first = begin();
  Iterator last = end();

  while ( first != last )
     {
       func(*first);
       ++first;
     }
}

void List :: apply(ConstFunctionPtr func) const        // const version
{
  ConstIterator first = begin();
  ConstIterator last = end();

  while ( first != last )
     {
       func(*first);
       ++first;
     }
}

//--- end of class List ---//

// $Log: List.cc,v $
// Revision 1.1  2000/02/09 21:34:34  vinod
// Moved into DataStructures directory
//
// Revision 1.2  2000/01/15 01:23:36  vinod
// Major changes. Restructured
//
// Revision 1.1.1.1  1999/01/30 22:31:19  vinod
// Utility Classes
//
// Revision 1.1.1.1  1998/05/03 21:44:53  vinod
// Utility classes, container classes
//
