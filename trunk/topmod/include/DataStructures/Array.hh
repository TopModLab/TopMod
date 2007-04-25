/* $Id: Array.hh,v 1.3 2000/04/12 06:56:46 vinod Exp $ */

#ifndef _ARRAY_HH_

#define _ARRAY_HH_

// Generic Array class to store an array of BaseObject pointers
// Array is dynamic - automatically resizes when elements are added to the end
// A default length and length increment is defined. 'garbage collection' i.e.
// Freeing up unused space is also supported.
// The array will always have atleast ARRAY_MIN_SIZE elements in it
// This is a macro that can be redefined before including this file

// class Array is derived from BaseObject, so we can have an Array of Arrays

// The Array class stores BaseObject pointers. So objects of any class derived from BaseObject
// can be stored in an Array, without need for a derived Array class. The only functions
// that need to be overridden are the array access operators which return a reference
// to a BaseObjectPtr and which have to be recast to the derived class. Even this is not
// required, but is useful for clarity in resulting code

#include <stdlib.h>
#include <BaseObject.hh>
#include <Inlines.hh>
#include <iostream.h>

#ifndef ARRAY_MIN_SIZE
#define ARRAY_MIN_SIZE 10
#endif // #ifndef ARRAY_MIN_SIZE

#ifndef ARRAY_DEFAULT_INCREMENT
#define ARRAY_DEFAULT_INCREMENT 10
#endif // #ifndef ARRAY_DEFAULT_INCREMENT

class Array;
typedef Array * ArrayPtr;

typedef BaseObjectPtr ArrayNode;
typedef ArrayNode * ArrayNodePtr;

class Array : public BaseObject
{
  protected :

     ArrayNodePtr       elements;                      // Actual array
     uint               alloc_size;                    // Allocated size of array
     uint               length;                        // Used size of array
     uint               size_increment;                // Size increment for expansion

        // Copy elements from the given array. Pointers are copied directly (shallow copy)
        // Resizes if necessary. Memory is allocated only for 'array.size' elements
     void copy_from(const Array& array)
       {
         delete [] elements; elements = NULL;
         alloc_size = length = array.length;
         elements = new ArrayNode[alloc_size];
         for (register int i=0; i < length; ++i)
            elements[i] = array.elements[i];
       }

        // Reallocate memory and copy the array. Call this whenever the array contents
        // don't change but the alloc_size needs to be changed. Does not do limit checks
     void reallocate(void)
       {
         ArrayNodePtr new_elements = new ArrayNode[alloc_size];
         for (register int i=0; i < length; ++i)
            new_elements[i] = elements[i];
         delete [] elements;
         elements = new_elements;
       }

        // Shift all elements from start to last element of array by offset to the right
        // Assumes allocated space is enough for the right shift
        // Does not adjust the value of length
     void shift_right(uint offset, uint start=0)
       {
         register int i, end = length-1;
         for (i=end; i >= start; i--)             elements[i+offset] = elements[i];
         for (i=start+offset-1; i >= start; --i)  elements[i] = NULL;
       }

        // Shift all elements from start to last element of array by offset to the left
        // Does not adjust the value of length
     void shift_left(uint offset, uint start=0)
       {
         register int i, end = length-1;
         for (i=max(start,offset); i <= end; ++i) elements[i-offset] = elements[i];
         for (i=end-offset+1; i <= end; ++i)      elements[i] = NULL;
       }

  public :

        // Default and 1-arg constructor
     Array(uint initial_size=ARRAY_MIN_SIZE)
       : BaseObject(), elements(NULL),
         alloc_size(initial_size), length(0), size_increment(ARRAY_DEFAULT_INCREMENT)
       {
         elements = new ArrayNode[alloc_size];
         for (int i=0; i < alloc_size; ++i)
            elements[i] = NULL;
       }

        // Copy constructor. alloc_size will be equal to array.length
     Array(const Array& array)
       : BaseObject(array), elements(NULL),
         alloc_size(array.length), length(0), size_increment(array.size_increment)
       {
         copy_from(array);
       }

        // Destructor. Does not free objects pointed to by the pointers
     virtual ~Array()
       {
         delete [] elements;
       }

        // Assignment operator. alloc_size will be equal to array.length
     Array& operator = (const Array& array)
       {
         BaseObject :: operator = (array);
         alloc_size = array.length; length = 0; size_increment = array.size_increment;
         copy_from(array);
         return (*this);
       }

        // Copy function - base class pure virtual implementation
        // Since it uses the copy constructor, it does only a shallow copy.
     virtual BaseObjectPtr copy(void) const
       {
         ArrayPtr newarray = new Array(*this);
         return newarray;
       }

        //--- Member functions ---//

        // Is the array empty
     bool is_empty(void) const
       {
         return length == 0;
       }

        // Length of the array
     uint size(void)
       {
         return length;
       }

        // Allocated length of the array
     uint capacity(void)
       {
         return alloc_size;
       }

        // Size increment for the array
     uint increment(void)
       {
         return size_increment;
       }

        // Set the size increment
     void set_size_increment(uint increment)
       {
         size_increment = increment;
       }

        // Change alloc_size to given size and resizes the elements array
        // alloc_size is not allowed to go below ARRAY_MIN_SIZE
        // If new alloc_size is less than length, array is truncated so that length = alloc_size
     void resize(uint new_size)
       {
         alloc_size = new_size;
         if ( alloc_size < ARRAY_MIN_SIZE ) alloc_size = ARRAY_MIN_SIZE;
         if ( length > alloc_size ) length = alloc_size;
         reallocate();
       }

        // Increase the allocated size by specified number (default 1) of size_increments
     void expand(uint num_increments=1)
       {
         if ( num_increments ) resize(alloc_size + num_increments * size_increment);
       }

        // Increase the allocated size so that it is >= given size
     void expand_to(uint new_alloc_size)
       {
         uint num_increments = 1 + (new_alloc_size - alloc_size)/size_increment;
         expand(num_increments);
       }

        // Decrease the allocated size by specified number (default 1) of size_increments
     void shrink(uint num_decrements=1)
       {
         if ( num_decrements ) resize(alloc_size - num_decrements * size_increment);
       }

        // Decrease the allocated size so that it is <= 1 increment more than given size
     void shrink_to(uint new_alloc_size)
       {
         uint num_decrements = (alloc_size - new_alloc_size)/size_increment;
         shrink(num_decrements);
       }

        // Decrease allocated size to be same as used size, without going below ARRAY_MIN_SIZE
        // Use this if no more insertions will be done on the array, to save memory
     void shrink_wrap(void)
       {
         uint new_alloc_size = alloc_size;
         if ( new_alloc_size > length ) new_alloc_size = length;
         if ( new_alloc_size < ARRAY_MIN_SIZE ) new_alloc_size = ARRAY_MIN_SIZE;
         if ( new_alloc_size != alloc_size )
            {
              alloc_size = new_alloc_size;
              reallocate();
            }
       }

        //--- Array element access. These can be overidden for derived classes ---//
     
     ArrayNode& operator [] (uint index)
       {
         if ( index >= length )
            {
                 // Use exceptions in future
              cerr << "Array :: operator [] : Index out of range!" << endl;
            }
         return elements[index];
       }

     const ArrayNode& operator [] (uint index) const
       {
         if ( index >= length )
            {
                 // Use exceptions in future
              cerr << "Array :: operator [] : Index out of range!" << endl;
            }
         return elements[index];
       }

        // Free all elements of the array. Boolean flag indicates if
        // the array delete operator should be used (default is false)
     void free(bool array_delete=false)
       {
         int i;
         
         if ( array_delete )
            for (i=0; i < length; ++i)
               {
                 delete [] elements[i]; elements[i] = NULL;
               }
         else
            for (i=0; i < length; ++i)
               {
                 delete elements[i]; elements[i] = NULL;
               }
       }

        // Free element at given index. Boolean flag indicates if the array form
        // of the delete operator should be used (default is false)
     void free(uint index, bool array_delete = false)
       {
         if ( index < length )
            {
              if ( array_delete ) delete [] elements[index];
              else delete elements[index];
              elements[index] = NULL;
            }
       }

        // Add an element at the end of the array. Expands array if required
     void push_back(const ArrayNode& elem)
       {
         uint newsize = length+1;
         if ( newsize >= alloc_size ) expand();
         elements[newsize-1] = elem; length = newsize;
       }

        // Add an element to the beginning of the array. Expands array if required
     void push_front(const ArrayNode& elem)
       {
         uint newsize = length+1;
         if ( newsize >= alloc_size ) expand();
         shift_right(1); // shift elements to the right by 1 cell
         elements[0] = elem; length = newsize;
       }

        // Insert the given element at the given position. 
     void insert(uint index, const ArrayNode& elem)
       {
         if ( index < length )
            {
              uint newsize = length + 1;
              if ( newsize >= alloc_size ) expand();
              shift_right(1,index);
              elements[index] = elem; length = newsize; 
            }
       }

        // Remove the last element of the array and return it
     ArrayNode pop_back(void)
       {
         --length;
         ArrayNode temp = elements[length];
         elements[length] = NULL;
         return temp;
       }
     
        // Remove the first element of the array and return it
     ArrayNode pop_front(void)
       {
         --length;
         ArrayNode temp = elements[0];
         shift_left(1); // shift elements to the left by 1 cell
         return temp;
       }

        // Delete element at specified index. Does not free the pointer, simply
        // resets the pointer to NULL. shifts elements to the right of the given
        // index to the left by 1.
     void erase(uint index)
       {
         if ( index < length )
            {
              shift_left(1,index+1);
              --length;
            }
       }

        // Delete all elements in the array. Resizes the array so alloc_size is ARRAY_MIN_SIZE
        // Does not the free the pointers, just resets them to NULL
     void erase(void)
       {
         length = 0;
         if ( alloc_size > ARRAY_MIN_SIZE )
            {
              alloc_size = ARRAY_MIN_SIZE;
              delete [] elements;
              elements = new ArrayNode[alloc_size];
            }
         for (register int i=0; i < alloc_size; ++i)  elements[i] = NULL;
       }
};
     
#endif /* #ifndef _ARRAY_HH_ */


/*
  $Log: Array.hh,v $
  Revision 1.3  2000/04/12 06:56:46  vinod
  Changed variable name from size to length to allow the function name to be
  called size.
  Bug fixes in push_back, free, fixed typos.

  Revision 1.2  2000/04/12 05:24:59  vinod
  Major cleanup. Corrected/implemented most commonly used functionality.
  Removed auto_expand, auto_free and ptr_is_array. Added/modified functions
  for memory management (resize, expand, shrink, etc.)

  Revision 1.1  2000/02/09 21:34:34  vinod
  Moved into DataStructures directory

  Revision 1.1  2000/02/02 00:46:33  vinod
  Array class

*/
