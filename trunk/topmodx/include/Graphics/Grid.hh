/*
*
* ***** BEGIN GPL LICENSE BLOCK *****
*
* This program is free software; you can redistribute it and/or
* modify it under the terms of the GNU General Public License
* as published by the Free Software Foundation; either version 2
* of the License, or (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program; if not, write to the Free Software  Foundation,
* Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
*
* The Original Code is Copyright (C) 2005 by xxxxxxxxxxxxxx
* All rights reserved.
*
* The Original Code is: all of this file.
*
* Contributor(s): none yet.
*
* ***** END GPL LICENSE BLOCK *****
*/

/* $Id: Grid.hh,v 2.2 2002/10/25 15:42:05 vinod Exp $ */

#ifndef _GRID_HH_

#define _GRID_HH_

// Class for a grid of lines along one of the three planes
// Uses OpenGL display lists. Spacing and size of grid can be controlled

// Grid extends from -size/2 to size/2 along both axis in the plane
// No. of squares in a row of the grid is specified by subdiv

#ifdef __APPLE__
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#else
#include <GL/gl.h>
#include <GL/glu.h>
#endif

typedef unsigned int uint;

enum GridPlane { XY=0, YZ=1, ZX=2 };

class Grid
{
  protected :

     GridPlane plane;                                  // Plane for the grid
     float size;                                       // Size (extent of grid)
     uint subdiv;                                      // No. of subdivisions
     GLuint disp_list_id;                              // OpenGL display list id
     bool visible;                                     // Visibility
     
     void createList(void)
       {
         int i;
         float sx,ex,sy,ey,sz,ez;                      // Start/end coordinates
         float dd,delta;
         
         glNewList(disp_list_id, GL_COMPILE);
         {
           dd = size/float(subdiv);
           glBegin(GL_LINES);
           switch ( plane )
              {
                case XY :
                             sx = sy = -size/2.0; ex = ey = -sx;
                             for (i=0; i <= subdiv; ++i)
                                {
                                  delta = float(i)*dd;
                                  glVertex3f(sx+delta,sy,0.0); glVertex3f(sx+delta,ey,0.0);
                                  glVertex3f(sx,sy+delta,0.0); glVertex3f(ex,sy+delta,0.0);
                                }
                             break;
                case YZ :
                             sy = sz = -size/2.0; ey = ez = -sy;
                             for (i=0; i <= subdiv; ++i)
                                {
                                  delta = i*dd;
                                  glVertex3f(0.0,sy+delta,sz); glVertex3f(0.0,sy+delta,ez);
                                  glVertex3f(0.0,sy,sz+delta); glVertex3f(0.0,ey,sz+delta);
                                }
                             break;
                case ZX :
                             sz = sx = -size/2.0; ez = ex = -sz;
                             for (i=0; i <= subdiv; ++i)
                                {
                                  delta = i*dd;
                                  glVertex3f(sx,0.0,sz+delta); glVertex3f(ex,0.0,sz+delta);
                                  glVertex3f(sx+delta,0.0,sz); glVertex3f(sx+delta,0.0,ez);
                                }
                             break;
              }
           glEnd();
         }
         glEndList();
       }
     
  public :

     Grid(GridPlane p = ZX, float s=20.0, uint num=10)
       : plane(p), size(s), subdiv(num), disp_list_id(0), visible(true)
       {
         disp_list_id = glGenLists(1);
         createList();
       }
     
     Grid(const Grid& grid)
       : plane(grid.plane), size(grid.size), subdiv(grid.subdiv),
         disp_list_id(0), visible(grid.visible)
       {
         disp_list_id = glGenLists(1);
         createList();
       }
     
     ~Grid()
       {
            // Free the associated display list
         glDeleteLists(disp_list_id,1);
       }
     
     void operator = (const Grid& grid)
       {
         plane = grid.plane; size = grid.size; subdiv = grid.subdiv;
         disp_list_id = 0; visible = grid.visible;
         createList();
       }

        //--- Member functions ---//

     void setPlane(GridPlane p)                        // Set the grid plane
       {
         plane = p;
         createList();
       }
     
     void setSize(float s)                             // Set the grid size
       {
         size = s;
         createList();
       }
     
     void setNumSubDivisions(uint num)                 // Set no. of subdivisions
       {
         subdiv = num;
         createList();
       }

     void set(GridPlane p, float s, uint num)          // Set all 3 parameters
       {
         plane = p; size = s; subdiv = num;
         createList();
       }
     
     void turnOff(void)                                // Make grid invisible
       {
         visible = false;
       }

     void turnOn(void)                                 // Make grid visible
       {
         visible = true;
       }

     void toggle(void)                                 // Toggle grid visibility
       {
         if ( visible ) visible = false;
         else visible = true;
       }

        // Call this function once for after view has been set-up
        // This function must be called for grid to be displayed
     void generate(void)                               // Regenerate display list with a new id
       {
         glDeleteLists(disp_list_id,1);
         disp_list_id = glGenLists(1);
         createList();
       }

     GridPlane getPlane(void) const                    // Get the grid plane
       {
         return plane;
       }
     
     float getSize(void) const                         // Get the grid size
       {
         return size;
       }

     uint getNumSubDivisions(void) const               // Get no. of subdivisions
       {
         return subdiv;
       }

     GLuint getDisplayListID(void) const               // Get the display list ID
       {
         return disp_list_id;
       }
     
     void render(void) const                           // Render the grid
       {
         if ( visible ) glCallList(disp_list_id);
       }
};

#endif /* #ifndef _GRID_HH_ */


/*
  $Log: Grid.hh,v $
  Revision 2.2  2002/10/25 15:42:05  vinod
  Fixed include location for GL headers

  Revision 2.1  2000/04/05 00:27:18  vinod
  Grid object class

*/
