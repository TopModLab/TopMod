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
*
* Short description of this file
*
* name of .hh file containing function prototypes
*
*/

// brianb

#include "DLFLLocator.hh"

uint DLFLLocator::suLastID = 0;


void DLFLLocator::render(void)
{
  if (!vtx)
    return;

  if (vtx)
    coords = vtx->getCoords();

  glDisable(GL_DEPTH_TEST);
  glLineWidth(2.0);

  // Render X
  if (renderSelection)
    glLoadName(0);
  if (getSelectedAxis() == 0)
    glColor3f(1.0,1.0,0.0);
  else
    glColor3f(1.0,0.0,0.0);     // X Axis - Red
  glBegin(GL_LINES);
    glVertex3f(coords[0],coords[1],coords[2]); glVertex3f(coords[0]+2,coords[1],coords[2]);
  glEnd();
  // glPushMatrix();
  //   glTranslatef(coords[0]+2,coords[1],coords[2]);
  //   glRotatef(90,0.0,1.0,0.0);
  // glutSolidCone(0.0625,0.125,100,1);
  // glPopMatrix();

  // Render Y
  if (renderSelection)
    glLoadName(1);
  if (getSelectedAxis() == 1)
    glColor3f(1.0,1.0,0.0);
  else
    glColor3f(0.0,1.0,0.0);     // Y Axis - Green
  glBegin(GL_LINES);
    glVertex3f(coords[0],coords[1],coords[2]); glVertex3f(coords[0],coords[1]+2,coords[2]);
  glEnd();
  // glPushMatrix();
  //   glTranslatef(coords[0],coords[1]+2,coords[2]);
  //   glRotatef(-90,1.0,0.0,0.0);
  //   glutSolidCone(0.0625,0.125,100,1);
  // glPopMatrix();

  // Render Z
  if (renderSelection)
    glLoadName(2);
  if (getSelectedAxis() == 2)
    glColor3f(1.0,1.0,0.0);
  else
    glColor3f(0.0,0.0,1.0);     // Z Axis - Blue
  glBegin(GL_LINES);
    glVertex3f(coords[0],coords[1],coords[2]); glVertex3f(coords[0],coords[1],coords[2]+2);
  glEnd();
  // glPushMatrix();
  //   glTranslatef(coords[0],coords[1],coords[2]+2);
  //   glutSolidCone(0.0625,0.125,100,1);
  // glPopMatrix();

  glLineWidth(1.0);

  // // Render cube
  // if (renderSelection)
  //   glLoadName(3);
  // if (getSelectedAxis() == 3)
  //   glColor3f(1.0,1.0,0.0);
  // else
  //   glColor3f(0.5,0.5,0.5);     // Cube - Grey
  // glPushMatrix();
  //   glTranslatef(coords[0],coords[1],coords[2]);
  // glutWireCube(0.4);
  // glPopMatrix();  

  // Render the vertex
  glColor3f(1.0,0.0,1.0);
  glPointSize(4.0);
  glBegin(GL_POINTS);
    // vtx->render();
  glEnd();

  glEnable(GL_DEPTH_TEST);
}
// brianb