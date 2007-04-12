/* $Id: SimpleOBJ.hh,v 1.1 2006/09/04 19:22:53 stuart Exp $ */

#ifndef _SIMPLE_OBJ_HH_

#define _SIMPLE_OBJ_HH_

// Class to represent a simple object in OBJ file format
// Has an array of vertices and an array of faces
// A Face is simply an array of pointers to face-vertices
// A FaceVertex has a pointer to a vertex and a normal
// Associated classes : OBJVertex, OBJFaceVertex, OBJFace
// Uses STL vector for the array of vertices and array of faces
// Because of that all functions have been made inline to avoid linker problems

#include <BaseObject.hh>
#include <StreamIO.hh>
#include <Color.hh>
#include <vector.h>
#include "OBJVertex.hh"
#include "OBJFace.hh"

class SimpleOBJ;
typedef SimpleOBJ * SimpleOBJPtr;

#define OBJVertexArray vector<OBJVertex> 
#define OBJFaceArray vector<OBJFace> 

// We need an array to store the normals
#define Vector3dArray vector<Vector3d>

class SimpleOBJ : public BaseObject
{
  protected :
     
     OBJVertexArray     ovarray;                       // Array of vertices
     OBJFaceArray       ofarray;                       // Array of faces

  public :
     
     RGBColor           color;                         // Color of object

  protected :
     
     bool               use_obj_color;                 // Flag to use object color

  public :

        // Default constructor
     SimpleOBJ()
       : BaseObject(), ovarray(0), ofarray(0),
         color(1), use_obj_color(false)
       {}

        // Copy constructor
     SimpleOBJ(const SimpleOBJ& sobj)
       : BaseObject(sobj), ovarray(sobj.ovarray), ofarray(sobj.ofarray),
         color(sobj.color), use_obj_color(sobj.use_obj_color)
       {}

        // Destructor
     virtual ~SimpleOBJ()
       {}

        // Assignment operator
     SimpleOBJ& operator = (const SimpleOBJ& sobj)
       {
         BaseObject :: operator = (sobj);
         ovarray = sobj.ovarray; ofarray = sobj.ofarray;
         color = sobj.color; use_obj_color = sobj.use_obj_color;
         return (*this);
       }

        // Make a copy. Implement base class pure virtual function
     virtual BaseObjectPtr copy(void) const
       {
         SimpleOBJPtr newsobj = new SimpleOBJ(*this);
         return newsobj;
       }

        //--- Member functions ---//

     uint numVertices(void) const
       {
         return ovarray.size();
       }

     uint numFaces(void) const
       {
         return ofarray.size();
       }

        // Reset the whole object
     void reset(void)
       {
         ovarray.clear(); ofarray.clear(); color = 1.0; use_obj_color = true;
       }

        // Access color
     void setColor(const Vector3d& col)
       {
         color = col;
       }

     void setColor(const RGBColor& col)
       {
         color = col;
       }

        // Set object color and propagate that color to all components
     void propagateColor(const Vector3d& col)
       {
         color = col;
         for (int i=0; i < ofarray.size(); ++i)
            ofarray[i].setColor(col);
       }

     void propagateColor(const RGBColor& col)
       {
         color = col;
         for (int i=0; i < ofarray.size(); ++i)
            ofarray[i].setColor(col);
       }

     RGBColor getColor(void) const
       {
         return color;
       }

     void useObjectColor(void)
       {
         use_obj_color = true;
         for (int i=0; i < ofarray.size(); ++i)
            ofarray[i].useObjectColor();
       }

     void useFaceColor(void)
       {
         use_obj_color = false;
         for (int i=0; i < ofarray.size(); ++i)
            ofarray[i].useFaceColor();
       }

     void useFaceVertexColor(void)
       {
         use_obj_color = false;
         for (int i=0; i < ofarray.size(); ++i)
            ofarray[i].useFaceVertexColor();
       }

     void useVertexColor(void)
       {
         use_obj_color = false;
         for (int i=0; i < ofarray.size(); ++i)
            ofarray[i].useVertexColor();
       }

        // Flat/smooth shading
     void useFlatShading(void)
       {
         for (int i=0; i < ofarray.size(); ++i)
            ofarray[i].useFlatShading();
       }
     
     void useSmoothShading(void)
       {
         for (int i=0; i < ofarray.size(); ++i)
            ofarray[i].useSmoothShading();
       }
     
        // Access the vertex array and the face array. No checks are done
     OBJVertex& vertex(uint index)
       {
         return ovarray[index];
       }

     OBJVertexPtr vertexptr(uint index)
       {
         return &(ovarray[index]);
       }
     
     OBJVertex vertex(uint index) const
       {
         return ovarray[index];
       }

     OBJFace& face(uint index)
       {
         return ofarray[index];
       }

     OBJFacePtr faceptr(uint index)
       {
         return &(ofarray[index]);
       }
     
     OBJFace face(uint index) const
       {
         return ofarray[index];
       }

        // Compute normals for each face
     void computeFaceNormals(void)
       {
         for (int i=0; i < ofarray.size(); ++i)
            ofarray[i].computeNormal();
       }

        // Set normals for each face vertex to be the normal for the Face
     void setVertexNormalsToFaceNormals(void)
       {
         for (int i=0; i < ofarray.size(); ++i)
            ofarray[i].setNormals();
       }

        // Render the object
     void render(void) const
       {
         if ( use_obj_color )
            {
              glColor(color);
            }
         for (int i=0; i < ofarray.size(); ++i)
            ofarray[i].render();
       }

     void outline(void) const
       {
         if ( use_obj_color )
            {
              glColor(color);
            }
         for (int i=0; i < ofarray.size(); ++i)
            ofarray[i].outline();
       }

  protected :
     
        // Read the object definition from an input stream
     void readObject(istream& i)
       {
         char c,c2;

         if ( !i )
            {
              cerr  << "Incomplete OBJ file." << endl;
              return;
            }

            // Clear the object first
         reset();

         Vector3dArray normals(0);
         OBJVertex tempvertex;
         OBJFaceVertex tempfacevertex;
         OBJFace tempface;
         Vector3d tempnormal;
         RGBColor currentcolor;
         
            // Read each line and set the Vertex, Normal or Face
         while ( i )
            {
              removeWhiteSpace(i);
              double x,y,z;
              i.get(c); i.get(c2);
              if ( c == 'c' && c2 == ' ' )
                 {
                      // Read a color specification and store it into current color
                   i >> currentcolor.r >> currentcolor.g >> currentcolor.b;
                 }
              else if ( c == 'v' )
                 {
                      // Read a vertex/normal specification
                   i >> x >> y >> z;
                   if ( c2 == ' ' )
                      {
                        tempvertex.color = currentcolor;
                        tempvertex.coords.set(x,y,z); ovarray.push_back(tempvertex);
                        tempvertex.reset();
                      }
                   else if ( c2 == 'n' )
                      {
                        tempnormal.set(x,y,z); normals.push_back(tempnormal);
                        tempnormal.reset();
                      }
                 }
              else if ( c == 'f' && c2 == ' ' )
                 {
                      // Read each vertex specification (v/vt/vn)
                   c = i.peek();
                   while ( c != '\n' )
                      {
                        int v,vt,vn;
                        i >> v; vn = -1; c = i.peek();
                        if ( c == '/' )
                           {
                             i.get(c); c = i.peek();
                             if ( c != '/' )
                                {
                                  i >> vt; c = i.peek();
                                }
                             if ( c == '/' )
                                {
                                  i.get(c); i >> vn; c = i.peek();
                                }
                           }

                           // We have v,vt and vn now
                        tempfacevertex.color = currentcolor;
                        tempfacevertex.vertex = &ovarray[v-1];
                        if ( vn > 0 ) tempfacevertex.normal = normals[vn-1];

                           // Add this new face vertex to the tempface
                        tempface.push_back(tempfacevertex);
                        tempfacevertex.reset();
                      }

                      // Add the newly created face to the face array of the object
                   tempface.updateVertices();
                   tempface.color = currentcolor;
                   ofarray.push_back(tempface);
                   tempface.reset();
                 }
              readTillEOL(i);
            }
         color = currentcolor;
       }

        // Read/write the object from/to an input/output stream
     friend istream& operator >> (istream& i, SimpleOBJ& obj)
       {
         obj.readObject(i);
         return i;
       }

     friend ostream& operator << (ostream& o, const SimpleOBJ& obj)
       {
            // Print vertex list first
         for (int i=0; i < obj.ovarray.size(); ++i)
            o << obj.ovarray[i];
  
         o << endl;
  
            // Print the face list
         for (i=0; i < obj.ofarray.size(); ++i)
            o << obj.ofarray[i];

         o << endl;
         return o;
       }
};

#endif /* #ifndef _SIMPLE_OBJ_HH_ */


/*
  $Log: SimpleOBJ.hh,v $
  Revision 1.1  2006/09/04 19:22:53  stuart
  Added includes and libs.

  Revision 1.17  2000/02/24 00:33:12  vinod
  Made color public

  Revision 1.16  2000/02/24 00:31:21  vinod
  Added propagateColor and changed setColor

  Revision 1.15  2000/02/24 00:23:13  vinod
  Added functions to get VertexPtr and FacePtr

  Revision 1.14  2000/02/17 08:10:54  vinod
  Typo

  Revision 1.13  2000/02/17 08:06:11  vinod
  Removed read till first 'v'

  Revision 1.12  2000/02/17 06:11:49  vinod
  Added color reading to file parser

  Revision 1.11  2000/02/14 20:54:22  vinod
  Added Face vertex array to each vertex and related functions

  Revision 1.10  2000/02/11 09:23:09  vinod
  Changed default to not use object color

  Revision 1.9  2000/02/11 09:03:05  vinod
  Made setColor propagate to vertices

  Revision 1.8  2000/02/08 10:19:01  vinod
  Typo

  Revision 1.7  2000/02/08 10:18:08  vinod
  Added useFaceVertexColor to SimpleOBJ class

  Revision 1.6  2000/02/08 08:41:10  vinod
  Added computeFaceNormals, setVertexNormalsToFaceNormals

  Revision 1.5  2000/02/08 05:17:26  vinod
  Minor bug fix

  Revision 1.4  2000/02/08 05:13:37  vinod
  Added readObject function

  Revision 1.3  2000/02/08 05:04:41  vinod
  Changed private to protected. Allowed derivationSimpleOBJ.hh

  Revision 1.2  2000/02/08 04:44:32  vinod
  Uses RGBColor instead of Vector4d. Fixed bug in extraction operator

  Revision 1.1.1.1  2000/02/01 07:37:28  vinod
  Class for a simple OBJ format object

*/
