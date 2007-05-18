/* $Id: OBJFace.hh,v 1.1 2006/09/04 19:22:53 stuart Exp $ */

#ifndef _OBJ_FACE_HH_

#define _OBJ_FACE_HH_

// Class for a Face to be used in the OBJObject class

#include <GL/gl.h>
#include "OBJCommon.hh"
#include "OBJFaceVertex.hh"

class OBJFace
{
  protected :

     OBJFaceVertexList  fv_list;                       // List of face vertices
     OBJMaterialPtr     matl_ptr;                      // Pointer to material for this face

  public :

        // Default constructor
     OBJFace()
       : fv_list(), matl_ptr(NULL)
       {}

        // Copy constructor
     OBJFace(const OBJFace& face)
       : fv_list(face.fv_list), matl_ptr(face.matl_ptr)
       {}

        // Destructor
     ~OBJFace();

        // Assignment operator
     OBJFace& operator = (const OBJFace& face);


        //--- Member functions ---//

     uint size(void) const                             // No. of vertices in this face
       {
         return fv_list.size();
       }

     uint numFaceVertexes(void) const
       {
         return fv_list.size();
       }

     OBJMaterialPtr material(void)
       {
         return matl_ptr;
       }

     void reset(void)
       {
         fv_list.clear(); matl_ptr = NULL;
       }

        // Add a new FaceVertex to the end of the array
     void addVertex(const OBJFaceVertex& ofv)
       {
         fv_list.push_back(ofv);
       }

        // Set the material pointer
     void setMaterial(OBJMaterialPtr mptr)
       {
         matl_ptr = mptr;
       }

        // Access the face-vertex specified by index. No range checks done
     OBJFaceVertexPtr facevertexptr(uint index)
       {
         OBJFaceVertexList :: iterator i = fv_list.begin();
         advance(i,index);
         return &(*i);
       }
     
     OBJFaceVertex& facevertex(uint index)
       {
         OBJFaceVertexList :: iterator i = fv_list.begin();
         advance(i,index);
         return (*i);
       }
     
     OBJFaceVertex facevertex(uint index) const
       {
         OBJFaceVertexList :: const_iterator i = fv_list.begin();
         advance(i,index);
         return (*i);
       }

        // Access the vertex specified by index. No range checks done
     OBJVertexPtr vertexptr(uint index)
       {
         return facevertex(index).vertex;
       }
     
     OBJVertex& vertex(uint index)
       {
         return *(facevertex(index).vertex);
       }

     OBJVertex vertex(uint index) const
       {
         return *(facevertex(index).vertex);
       }

        // Access the coordinates of the vertex specified by index. No range checks done
     Vector3d& vertex_coords(uint index)
       {
         return vertexptr(index)->coords;
       }

     Vector3d vertex_coords(uint index) const
       {
         return vertex(index).coords;
       }

        // Access color and normal of face vertex specified by index
     RGBColor& fv_color(uint index)
       {
         return facevertex(index).color;
       }

     RGBColor fv_color(uint index) const
       {
         return facevertex(index).color;
       }
     
     Vector3d& fv_normal(uint index)
       {
         return facevertex(index).normal;
       }

     Vector3d fv_normal(uint index) const
       {
         return facevertex(index).normal;
       }

        // Set the color of all face vertices of this face
     void setColor(const RGBColor& col)
       {
         OBJFaceVertexList :: iterator first, last;
         first = fv_list.begin();
         last = fv_list.end();
         while ( first != last )
            {
              (*first).color = col;
              ++first;
            }
       }

     void setColor(double r, double g, double b)
       {
         OBJFaceVertexList :: iterator first, last;
         first = fv_list.begin();
         last = fv_list.end();
         while ( first != last )
            {
              (*first).color.color.set(r,g,b);
              ++first;
            }
       }

        // Update the facevertices of the vertices which this face contains
     void update(void)
       {
         OBJFaceVertexList :: iterator first, last;
         first = fv_list.begin();
         last = fv_list.end();
         while ( first != last )
            {
              (*first).update();
              ++first;
            }
       }

     void updateMaterial(void);

        //--- Rendering subroutines ---//

        // Call glBegin with the appropriate macro depending on no of vertices
        // This is for filled polygons.
     static void glBeginP(int num)
       {
         switch ( num )
            {
              case 0 :
                           return;
              case 1 :
                           glBegin(GL_POINTS); break;
              case 2 :
                           glBegin(GL_LINES); break;
              case 3 :
                           glBegin(GL_TRIANGLES); break;
              case 4 :
                           glBegin(GL_QUADS); break;
              default :
                           glBegin(GL_POLYGON);
            }
       }

        // Call glBegin with the appropriate macro depending on no of vertices
        // This is for outlined polygons.
     static void glBeginO(int num)
       {
         switch ( num )
            {
              case 0 :
                           return;
              case 1 :
                           glBegin(GL_POINTS); break;
              case 2 :
                           glBegin(GL_LINES); break;
              default :
                           glBegin(GL_LINE_LOOP);
            }
       }

     void render_FVN(void) const                       // Render using FV normals
       {
         OBJFace :: glBeginP(fv_list.size());
         for_each(fv_list.begin(),fv_list.end(),glVertexN);
         glEnd();
       }

     void render_FVC(void) const                       // Render using FV colors
       {
         int num_vertices = fv_list.size();
         OBJFace :: glBeginP(fv_list.size());
         for_each(fv_list.begin(),fv_list.end(),glVertexC);
         glEnd();
       }

     void render_FVCN(void) const                      // Render using FV colors and normals
       {
         OBJFace :: glBeginP(fv_list.size());
         for_each(fv_list.begin(),fv_list.end(),glVertexCN);
         glEnd();
       }

     void render_FV(void) const                        // Render without using FV colors or normals
       {
         OBJFace :: glBeginP(fv_list.size());
         for_each(fv_list.begin(),fv_list.end(),glVertexOFV);
         glEnd();
       }

        //--- Same as above but with texturing also ---//
     
     void render_FVNT(void) const
       {
         OBJFace :: glBeginP(fv_list.size());
         for_each(fv_list.begin(),fv_list.end(),glVertexNT);
         glEnd();
       }

     void render_FVCT(void) const
       {
         OBJFace :: glBeginP(fv_list.size());
         for_each(fv_list.begin(),fv_list.end(),glVertexCT);
         glEnd();
       }

     void render_FVCNT(void) const
       {
         OBJFace :: glBeginP(fv_list.size());
         for_each(fv_list.begin(),fv_list.end(),glVertexCNT);
         glEnd();
       }

     void render_FVT(void) const
       {
         OBJFace :: glBeginP(fv_list.size());
         for_each(fv_list.begin(),fv_list.end(),glVertexOFVT);
         glEnd();
       }

     void render(void) const                           // Default render - render_FVN
       {
         render_FVN();
       }

        //-- Friend versions of above functions --//
     
     void outline_FVC(void) const                      // Render outline using FV colors
       {
         OBJFace :: glBeginO(fv_list.size());
         for_each(fv_list.begin(),fv_list.end(),glVertexC);
         glEnd();
       }

     void outline_FV(void) const                       // Render outline
       {
         OBJFace :: glBeginO(fv_list.size());
         for_each(fv_list.begin(),fv_list.end(),glVertexOFV);
         glEnd();
       }

     void outline(void) const                          // Default outline - outline_FV
       {
         outline_FV();
       }

        //--- These friend versions of above functions allow use of STL algorithms ---//

     friend void render_FVN(const OBJFace& face)
       {
         face.render_FVN();
       }

     friend void render_FVC(const OBJFace& face)
       {
         face.render_FVC();
       }

     friend void render_FVCN(const OBJFace& face)
       {
         face.render_FVCN();
       }

     friend void render_FV(const OBJFace& face)
       {
         face.render_FV();
       }

     friend void render_FVNT(const OBJFace& face)
       {
         face.render_FVNT();
       }

     friend void render_FVCT(const OBJFace& face)
       {
         face.render_FVCT();
       }

     friend void render_FVCNT(const OBJFace& face)
       {
         face.render_FVCNT();
       }

     friend void render_FVT(const OBJFace& face)
       {
         face.render_FVT();
       }

     friend void render(const OBJFace& face)
       {
         face.render();
       }

     friend void outline_FVC(const OBJFace& face)
       {
         face.outline_FVC();
       }

     friend void outline_FV(const OBJFace& face)
       {
         face.outline_FV();
       }

     friend void outline(const OBJFace& face)
       {
         face.outline();
       }


        //--- The OBJFacePtr versions are for use by the OBJMaterial class ---//

        // The function names have been changed to prevent STL instantiation errors
        // because of conflicts with other functions with similar names (OBJMaterial)

     friend void facerender_FVN(const OBJFacePtr& face)
       {
         face->render_FVN();
       }

     friend void facerender_FVC(const OBJFacePtr& face)
       {
         face->render_FVC();
       }

     friend void facerender_FVCN(const OBJFacePtr& face)
       {
         face->render_FVCN();
       }

     friend void facerender_FV(const OBJFacePtr& face)
       {
         face->render_FV();
       }

     friend void facerender_FVNT(const OBJFacePtr& face)
       {
         face->render_FVNT();
       }

     friend void facerender_FVCT(const OBJFacePtr& face)
       {
         face->render_FVCT();
       }

     friend void facerender_FVCNT(const OBJFacePtr& face)
       {
         face->render_FVCNT();
       }

     friend void facerender_FVT(const OBJFacePtr& face)
       {
         face->render_FVT();
       }

     friend void facerender(const OBJFacePtr& face)
       {
         face->render();
       }

     friend void faceoutline_FVC(const OBJFacePtr& face)
       {
         face->outline_FVC();
       }

     friend void faceoutline_FV(const OBJFacePtr& face)
       {
         face->outline_FV();
       }

     friend void faceoutline(const OBJFacePtr& face)
       {
         face->outline();
       }

     friend ostream& operator << (ostream& o, const OBJFace& face)
       {
         o << "Face (" << face.size() << ") : ";
         OBJFaceVertexList :: const_iterator first, last;
         first = face.fv_list.begin();
         last = face.fv_list.end();
         while ( first != last )
            {
              o << (*first).vertex->coords << ", ";
              ++first;
            }
         return o;
       }
};

#endif /* #ifndef _OBJ_FACE_HH_ */

/*
  $Log: OBJFace.hh,v $
  Revision 1.1  2006/09/04 19:22:53  stuart
  Added includes and libs.

  Revision 1.5  2000/09/25 17:59:55  vinod
  Changed face-vertex list to linked-list from array

  Revision 1.4  2000/09/22 18:15:47  vinod
  Added matl_ptr, added rendering functions

  Revision 1.3  2000/05/13 01:56:14  vinod
  Added numFaceVertexes() member function

  Revision 1.2  2000/04/25 04:01:38  vinod
  Added functions for using texturing

  Revision 1.1  2000/04/19 21:52:23  vinod
  OBJObject class. Initial version. Modified from SimpleOBJ

*/
