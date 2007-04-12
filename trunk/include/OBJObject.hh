/* $Id: OBJObject.hh,v 1.1 2006/09/04 19:22:53 stuart Exp $ */

#ifndef _OBJ_OBJECT_HH_

#define _OBJ_OBJECT_HH_

// Simplified (and slightly modified) Wavefront Object class
// Has an array of vertices, and an array of materials. There is always
// a default material. Each material has an array of faces

#include <stdio.h>
#include <Vector2d.hh>
#include <Vector3d.hh>
#include <Transform.hh>
#include "OBJCommon.hh"
#include "OBJVertex.hh"
#include "OBJMaterial.hh"

typedef vector<Vector3d> Vector3dArray;
typedef vector<Vector2d> Vector2dArray;

class OBJObject
{
  public :

     Vector3d           position;                      // Position of object
     Vector3d           scale;                         // Scale of object
     Quaternion         rotation;                      // Rotation of object

  protected :

     OBJVertexArray     vertex_array;                  // Array of vertices
     OBJMaterialList    matl_list;                     // List of materials
     OBJFaceArray       face_array;                    // Array of faces

     static Vector3dArray normals;                     // Array for reading normals
     static Vector2dArray texcoords;                   // Array for reading texture coords
     static Transformation tr;                         // For doing GL transformation

  public :

        // Default constructor
     OBJObject()
       : position(), scale(1), rotation(),
         vertex_array(), matl_list(), face_array()
       {
            // Create a default material with gray color
         matl_list.push_back(OBJMaterial("default",0.5,0.5,0.5));
       }

        // Copy constructor
     OBJObject(const OBJObject& obj)
       : position(obj.position), scale(obj.scale), rotation(obj.rotation),
         vertex_array(obj.vertex_array), matl_list(obj.matl_list), face_array(obj.face_array)
       {}

        // Destructor
     virtual ~OBJObject()
       {}

        // Assignment operator
     OBJObject& operator = (const OBJObject& obj)
       {
         position = obj.position; scale = obj.scale; rotation = obj.rotation;
         vertex_array = obj.vertex_array; matl_list = obj.matl_list; face_array = obj.face_array;
         return (*this);
       }

        //--- Member functions ---//

     uint numVertices(void) const
       {
         return vertex_array.size();
       }

     uint numFaces(void) const
       {
         return face_array.size();
       }

     uint numMaterials(void) const
       {
         return matl_list.size();
       }

        // Reset the whole object
     void reset(void)
       {
         position.reset(); scale.set(1,1,1); rotation.reset();
         vertex_array.clear(); face_array.clear(); matl_list.clear();
         matl_list.push_back(OBJMaterial("default",0.5,0.5,0.5));
       }

     void setColor(const RGBColor& col)
       {
            // matl_list[0] is always the default material
         matl_list.front().setColor(col);
       }

     RGBColor getColor(void) const
       {
            // Return color of matl_list[0]
         return matl_list.front().color;
       }

        // Access the vertex array and the face array. No checks are done
     OBJVertex& vertex(uint index)
       {
         return vertex_array[index];
       }

     OBJVertexPtr vertexptr(uint index)
       {
         return &(vertex_array[index]);
       }
     
     OBJVertex vertex(uint index) const
       {
         return vertex_array[index];
       }

     OBJFace& face(uint index)
       {
         return face_array[index];
       }

     OBJFacePtr faceptr(uint index)
       {
         return &(face_array[index]);
       }

     OBJFace face(uint index) const
       {
         return face_array[index];
       }

        // Apply GL transformations before rendering
     void transform(void) const
       {
         tr.reset();
         tr.scale(scale);
         tr.rotate(rotation);
         tr.translate(position);
         tr.apply();
       }

        // Apply the transformations for this object on the given vector
     void transform(Vector3d& p)
       {
         tr.reset();
         tr.scale(scale);
         tr.rotate(rotation);
         tr.translate(position);
         Vector4d tp(p); tp[3] = 1.0;
         tp = tr.matrix() * tp;
         tp /= tp[3];
         p = tp;
       }

        // Render the object
     void render(void) const
       {
         glPushMatrix();
         transform();
         for_each(matl_list.begin(),matl_list.end(),matl_render);
         glPopMatrix();
       }

     void outline(void) const
       {
         glPushMatrix();
         transform();
         for_each(matl_list.begin(),matl_list.end(),matl_outline);
         glPopMatrix();
       }

     void renderFaces(void) const
       {
         glPushMatrix();
         transform();
         for_each(matl_list.begin(),matl_list.end(),matl_renderFaces);
         glPopMatrix();
       }

     void outlineFaces(void) const
       {
         glPushMatrix();
         transform();
         for_each(matl_list.begin(),matl_list.end(),matl_outlineFaces);
         glPopMatrix();
       }

     void plainRender(void) const
       {
         glPushMatrix();
         transform();
         for_each(matl_list.begin(),matl_list.end(),matl_plainRender);
         glPopMatrix();
       }

     void plainOutline(void) const
       {
         glPushMatrix();
         transform();
         for_each(matl_list.begin(),matl_list.end(),matl_plainOutline);
         glPopMatrix();
       }

     void renderT(void) const
       {
         glPushMatrix();
         transform();
         for_each(matl_list.begin(),matl_list.end(),matl_renderT);
         glPopMatrix();
       }

     void renderFacesT(void) const
       {
         glPushMatrix();
         transform();
         for_each(matl_list.begin(),matl_list.end(),matl_renderFacesT);
         glPopMatrix();
       }

     void plainRenderT(void) const
       {
         glPushMatrix();
         transform();
         for_each(matl_list.begin(),matl_list.end(),matl_plainRenderT);
         glPopMatrix();
       }

  protected :

        // Find a material with the given color and return it's index
        // If not material is found, default material is returned
     OBJMaterialPtr findMaterial(const RGBColor& color);

        // Read the object definition from an input stream
     void readObject(istream& i);

  public :

        // Read/write the object from/to an input/output stream
     friend istream& operator >> (istream& i, OBJObject& obj)
       {
         obj.readObject(i);
         return i;
       }

     friend ostream& operator << (ostream& o, const OBJObject& obj)
       {
            // Print vertex array
         o << obj.vertex_array.size() << " vertices" << endl;
         for (int i=0; i < obj.vertex_array.size(); ++i)
            o << obj.vertex_array[i];
  
         o << endl;

            // Print the face array
         o << obj.numFaces() << " faces" << endl;
         for (int i=0; i < obj.face_array.size(); ++i)
            o << obj.face_array[i] << endl;

            // Print the faces using the material List
         o << obj.numMaterials() << " materials" << endl;
         OBJMaterialList :: const_iterator first, last;
         first = obj.matl_list.begin(); last = obj.matl_list.end();
         while ( first != last )
            {
              o << (*first) << endl;
              ++first;
            }
         o << endl;
         return o;
       }
};

#endif /* #ifndef _OBJ_OBJECT_HH_ */


/*
  $Log: OBJObject.hh,v $
  Revision 1.1  2006/09/04 19:22:53  stuart
  Added includes and libs.

  Revision 1.9  2000/09/22 18:18:21  vinod
  Overhaul of rendering functions

  Revision 1.8  2000/05/15 22:17:07  vinod
  Bug fix in transform(Vector3d&) - w wasn't being set to 1

  Revision 1.7  2000/05/13 01:57:06  vinod
  Added support for OBJIterator

  Revision 1.6  2000/05/12 04:58:05  vinod
  Added transform(Vector3d&) subroutine

  Revision 1.5  2000/05/08 19:26:58  vinod
  Added numMaterials() and getColor() methods

  Revision 1.4  2000/05/05 20:51:12  vinod
  Added position and scale

  Revision 1.3  2000/04/25 04:02:13  vinod
  Added functions for using texturing

  Revision 1.2  2000/04/20 04:40:08  vinod
  Added static variables for parser use.
  Moved parsing related functions into .cc file

  Revision 1.1  2000/04/19 21:52:24  vinod
  OBJObject class. Initial version. Modified from SimpleOBJ

*/
