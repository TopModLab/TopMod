
/* Rough outline of planarization algorithm */
#include "DLFLObject.hh"
#include "DLFLAux.hh"
#include <math.h>
#include <assert.h>


void DLFLObject::planarize(void)
{
  DLFLFacePtrList::iterator fl_first, fl_last;
  DLFLFacePtr faceptr;
  
  fl_first = face_list.begin(); fl_last = face_list.end();

     /* Loop through all faces */
  while ( fl_first != fl_last )
     {
       faceptr = (*fl_first); ++fl_first;

          /*
            Find the average normal for the face and the centroid
            Store these in the member variables of the face for later use
          */
       faceptr->normal = faceptr->computeNormal();
       faceptr->centroid = faceptr->geomCentroid();

          /*
            If the normal and point to define the plane are calculated
            using some other method, calculate them and store them in
            the 'normal' and 'centroid' member variables in the face
          */
     }


  DLFLVertexPtrList::iterator vl_first, vl_last;
  DLFLVertexPtr vertexptr;
  DLFLFaceVertexPtrArray fvparray;
  Vector3dArray p_array, n_array; // Arrays of point and normals for each face adjacent to a vertex
  int num_faces;
  
  vl_first = vertex_list.begin(); vl_last = vertex_list.end();

     /* Loop through all vertices */
  while ( vl_first != vl_last )
     {
       vertexptr = (*vl_first); ++vl_first;

          /*
            Get all the corners for this vertex
          */
       fvparray.clear(); // The local array must be cleared before calling getFaceVertices
       vertexptr->getFaceVertices(fvparray);
       num_faces = fvparray.size();
       
          /*
            Go through the array of face-vertices and find the normal and centroid
            for each face
          */
       p_array.clear(); n_array.clear(); // The local arrays must be cleared for each Vertex

          // If we resize the array before filling it, we can avoid unnecessary memory allocation
          // thus speeding up the process a little bit.
       //p_array.resize(num_faces,Vector3d(0)); n_array.resize(num_faces,Vector3d(0));
       p_array.resize(num_faces,Vector3d()); n_array.resize(num_faces,Vector3d());
       for (int i=0; i <num_faces; ++i)
          {
            DLFLFacePtr fp = fvparray[i]->getFacePtr();

               // Get the normal and centroid for this face (which we calculated above)
               // and store them in our local arrays
            n_array[i] = fp->normal;
            p_array[i] = fp->centroid;
	    //cout << "Before p = " << p_array[i] << " n = " << n_array[i] << endl;
	    //cout << "After p = " << p_array[i] << " n = " << n_array[i] << endl << endl; 
	    
	    
          }

       Vector3d new_pos = vertexptr->coords; // This is the old position


      Vector3d buffer1,buffer2; 
       float t, deltat; int m; 
       m=100; t=0.3;deltat=t/(1.1*m); 
       for (int j=0; j <m; ++j)
       {
       t-=deltat; 
       for (int i=0; i <num_faces; ++i)
          {
       buffer1=new_pos-p_array[i];
       new_pos-=t*(n_array[i]*buffer1)*n_array[i];
          }
	  }
       
       /*
       buffer1=new_pos-p_array[0];
       buffer2=n_array[0];
       t=0;  
       if(fabs((n_array[0]*buffer2))>0.000001)
       t=(n_array[0]*buffer1)/(n_array[0]*buffer2);
       new_pos-=t*buffer2;
       
       if(num_faces>0)
       {
       buffer1=new_pos-p_array[1];
       buffer2=n_array[1]-(n_array[0]*n_array[1])*n_array[0];
       if(fabs((buffer2*buffer2))<0.000001) 
       buffer2=n_array[2];
       t=0;  
       if(fabs((n_array[1]*buffer2))>0.000001) 
       t=(n_array[1]*buffer1)/(n_array[1]*buffer2);
       new_pos-=t*buffer2;
       }
       
       else if(num_faces>1)
       {
       buffer1=new_pos-p_array[2];
       buffer2=n_array[0]%n_array[1];
       if(fabs((buffer2*buffer2))<0.000001) 
       buffer2=n_array[2]-(n_array[0]*n_array[2])*n_array[0];
       else if(fabs((buffer2*buffer2))<0.000001)
       buffer2=n_array[2]-(n_array[1]*n_array[2])*n_array[1];
       else if(fabs((buffer2*buffer2))<0.000001)
       buffer2=n_array[2]; 
       t=0;  
       if(fabs((n_array[2]*buffer2))>0.000001) 
       t=(n_array[2]*buffer1)/(n_array[2]*buffer2);
       new_pos-=t*buffer2;
       }
       */
       
      
          /*
            Use the 2 arrays (n_array and p_array) to compute the new position for this vertex
          */
       
       vertexptr->coords = new_pos;
     }
}

