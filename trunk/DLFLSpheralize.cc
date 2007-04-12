
/* Spheralization algorithm */

#include "DLFLObject.hh"
#include "DLFLAux.hh"
#include <math.h>
#include <assert.h>

void DLFLObject :: spheralize(void)
{
  DLFLVertexPtrList :: iterator vl_first, vl_last;
  int num_vertices=0; 
  Vector3d center;
  DLFLVertexPtr vertexptr;
  Vector3d buffer;

  vl_first = vertex_list.begin(); vl_last = vertex_list.end();  

  while ( vl_first != vl_last )
     {
       vertexptr = (*vl_first); ++vl_first;

       center += vertexptr->coords; 
       num_vertices++; 
     }
  center /= num_vertices;

  vl_first = vertex_list.begin(); vl_last = vertex_list.end();

  while ( vl_first != vl_last )
     {
       vertexptr = (*vl_first); ++vl_first;

       Vector3d new_pos = vertexptr->coords; // This is the old position

       buffer = new_pos-center;
       normalize(buffer); 
       vertexptr->coords = buffer; /*+center;*/
     }
}
