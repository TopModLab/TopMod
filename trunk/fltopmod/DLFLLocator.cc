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
  glPushMatrix();
    glTranslatef(coords[0]+2,coords[1],coords[2]);
    glRotatef(90,0.0,1.0,0.0);
  glutSolidCone(0.0625,0.125,100,1);
  glPopMatrix();

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
  glPushMatrix();
    glTranslatef(coords[0],coords[1]+2,coords[2]);
    glRotatef(-90,1.0,0.0,0.0);
    glutSolidCone(0.0625,0.125,100,1);
  glPopMatrix();

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
  glPushMatrix();
    glTranslatef(coords[0],coords[1],coords[2]+2);
    glutSolidCone(0.0625,0.125,100,1);
  glPopMatrix();

  glLineWidth(3.0);

  // Render cube
  if (renderSelection)
    glLoadName(3);
  if (getSelectedAxis() == 3)
    glColor3f(1.0,1.0,0.0);
  else
    glColor3f(0.5,0.5,0.5);     // Cube - Grey
  glPushMatrix();
    glTranslatef(coords[0],coords[1],coords[2]);
  glutWireCube(0.4);
  glPopMatrix();  

  // Render the vertex
  glColor3f(1.0,0.0,1.0);
  glPointSize(4.0);
  glBegin(GL_POINTS);
    vtx->render();
  glEnd();

  glEnable(GL_DEPTH_TEST);
}
