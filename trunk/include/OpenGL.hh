/* File Name : OpenGL.hh */
/* Header file for common OpenGL, GLUT functions */
/* To be included for programs. */
/* Function definitions in OpenGL.cc */

#ifndef _OPENGL_HH_

#define _OPENGL_HH_

#include <glut.h>
#include <stdlib.h>
#include <stdio.h>

// Macros for return values of event handlers
#define EVENT_NOT_HANDLED 0
#define EVENT_HANDLED 1
#define EVENT_NOT_USED 2

typedef unsigned int uint;

struct Color 
{
  public :
     
     float r,g,b;
};
  
void initGLUT(int width, int height, int * argcp, char ** argv);
void initGLUTDB(int width, int height, int * argcp, char ** argv); // Double buffer mode
void initGLUTDepth(int width, int height, int * argcp, char ** argv);
void initGLUTDBDepth(int width, int height, int * argcp, char ** argv); // Double buffer mode
int  createWindow(char * name);
void clearColor(Color color);                          // Set the color to use for clearing
void clearWindow(Color color);                         /* Clear the window using given color */
void clearWindow(void);                                // Clear the window
void setColor(Color color);                            /* Set color to a given color */

void registerDefaultCallBacks(void);
  
void defaultDisplayFunction(void);
void defaultReshapeFunction(int width, int height);
void defaultKeyboardFunction(unsigned char key, int x, int y);
void defaultMouseFunction(int button, int state, int x, int y);
void defaultVisibilityFunction(int state);

void convertYCoord(int& y);                            // Converts y coord from origin at top
                                                       // to origin at bottom

void exitFunction(int exitcode);                       /* Called when program exits */
                                                       /* Should be defined in app. prog. */

void drawLine(double x1, double y1, double x2, double y2);
void drawRectangle(double x1, double y1, double x2, double y2);
void drawFilledRectangle(double x1, double y1, double x2, double y2);
void draw3dRectangle(double x1, double y1, double x2, double y2, Color topleft, Color botright);

   /* Global variables */
extern unsigned char DIMENSION;

   /* Predefined colors as structs of (r,g,b) */
extern Color Black, White;
extern Color VyDkGray, DkGray, Gray, LtGray, VyLtGray; /* Gray Scale */
extern Color Red, Blue, Green, Yellow, Magenta, Cyan; /* Common colors */
  
#endif // #ifndef _OPENGL_HH_
