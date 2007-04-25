/* $Id: DLFLViewport.cc,v 4.0 2003/12/26 01:58:53 vinod Exp $ */

// Source code for DLFLViewport class

#include "DLFLViewport.hh"

//-- Define static members --//

DLFLVertexPtrArray DLFLViewport::sel_vptr_array;
DLFLEdgePtrArray DLFLViewport::sel_eptr_array;
DLFLFacePtrArray DLFLViewport::sel_fptr_array;
DLFLFaceVertexPtrArray DLFLViewport::sel_fvptr_array;

// Subroutine for selecting a Vertex
DLFLVertexPtr DLFLViewport :: selectVertex(int mx, int my)
{
  GLuint selectBuf[1024];
  uint closest;
  GLuint dist;
  long hits, index;
  DLFLVertexPtr sel(NULL);
  
  glSelectBuffer(1024,selectBuf);
  glRenderMode(GL_SELECT);
  
  glInitNames(); glPushName(0);
  
  GLint vp[4];
  glGetIntegerv(GL_VIEWPORT, vp);
  viewport.camera.enterSelectionMode(mx,my,20,20,vp); // Reduced sensitivity for picking points
  
     // Make sure earlier matrices are preserved, since multiple windows
     // seem to be sharing the same matrix stacks
  
  glMatrixMode(GL_PROJECTION);
  glPushMatrix();
  
  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
  
  viewport.reshape();
  viewport.apply_transform();
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  object->renderVerticesForSelect();
  glFlush();
  
  glPopMatrix();
  
  glMatrixMode(GL_PROJECTION);
  glPopMatrix();
  
  viewport.camera.leaveSelectionMode();
  
  hits = glRenderMode(GL_RENDER);
  if ( hits > 0 )
     {
       closest = 0; dist = 0xffffffff;
       while ( hits )
          {
            index = (hits-1)*4;
            if ( selectBuf[index+1] < dist )
               {
                 dist = selectBuf[index+1];
                 closest = selectBuf[index+3];
               }
            hits--;
          }
          // closest now has the id of the selected vertex
       sel = DLFLObject::vparray[closest];
     }
  return sel;
}

   // Subroutine for selecting an Edge
DLFLEdgePtr DLFLViewport :: selectEdge(int mx, int my)
{
  GLuint selectBuf[1024];
  uint closest;
  GLuint dist;
  long hits, index;
  DLFLEdgePtr sel(NULL);
         
  glSelectBuffer(1024,selectBuf);
  glRenderMode(GL_SELECT);

  glInitNames(); glPushName(0);

  GLint vp[4];
  glGetIntegerv(GL_VIEWPORT, vp);
  viewport.camera.enterSelectionMode(mx,my,10,10,vp);

     // Make sure earlier matrices are preserved, since multiple windows
     // seem to be sharing the same matrix stacks
         
  glMatrixMode(GL_PROJECTION);
  glPushMatrix();

  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();

  viewport.reshape();
  viewport.apply_transform();
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  object->renderEdgesForSelect();
  glFlush();

  glPopMatrix();

  glMatrixMode(GL_PROJECTION);
  glPopMatrix();

  viewport.camera.leaveSelectionMode();

  hits = glRenderMode(GL_RENDER);
  if ( hits > 0 )
     {
       closest = 0; dist = 0xffffffff;
       while ( hits )
          {
            index = (hits-1)*4;
            if ( selectBuf[index+1] < dist )
               {
                 dist = selectBuf[index+1];
                 closest = selectBuf[index+3];
               }
            hits--;
          }
          // closest now has the id of the selected edge
       sel = DLFLObject::eparray[closest];
     }
  return sel;
}

   // Subroutine for selecting a Face
DLFLFacePtr DLFLViewport :: selectFace(int mx, int my)
{
  GLuint selectBuf[1024];
  uint closest;
  GLuint dist;
  long hits, index;
  DLFLFacePtr sel(NULL);
         
  glSelectBuffer(1024,selectBuf);
  glRenderMode(GL_SELECT);

  glInitNames(); glPushName(0);

  GLint vp[4];
  glGetIntegerv(GL_VIEWPORT, vp);
  viewport.camera.enterSelectionMode(mx,my,2.5,2.5,vp);

     // Make sure earlier matrices are preserved, since multiple windows
     // seem to be sharing the same matrix stacks
         
  glMatrixMode(GL_PROJECTION);
  glPushMatrix();

  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();

  viewport.reshape();
  viewport.apply_transform();
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  object->renderFacesForSelect();
  glFlush();

  glPopMatrix();

  glMatrixMode(GL_PROJECTION);
  glPopMatrix();

  viewport.camera.leaveSelectionMode();

  hits = glRenderMode(GL_RENDER);
  if ( hits > 0 )
     {
       closest = 0; dist = 0xffffffff;
       while ( hits )
          {
            index = (hits-1)*4;
            if ( selectBuf[index+1] < dist )
               {
                 dist = selectBuf[index+1];
                 closest = selectBuf[index+3];
               }
            hits--;
          }
          // closest now has the id of the selected face
       sel = DLFLObject::fparray[closest];
     }
  return sel;
}

   // Subroutine for selecting a FaceVertex (Corner) within a Face
DLFLFaceVertexPtr DLFLViewport :: selectFaceVertex(DLFLFacePtr fp, int mx, int my)
{
  GLuint selectBuf[1024];
  uint closest;
  GLuint dist;
  long hits, index;
  DLFLFaceVertexPtr sel(NULL);
         
  glSelectBuffer(1024,selectBuf);
  glRenderMode(GL_SELECT);

  glInitNames(); glPushName(0);

  GLint vp[4];
  glGetIntegerv(GL_VIEWPORT, vp);
  viewport.camera.enterSelectionMode(mx,my,40,40,vp);

     // Make sure earlier matrices are preserved, since multiple windows
     // seem to be sharing the same matrix stacks
         
  glMatrixMode(GL_PROJECTION);
  glPushMatrix();

  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();

  viewport.reshape();
  viewport.apply_transform();
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  object->renderFaceVerticesForSelect(fp);
  glFlush();

  glPopMatrix();

  glMatrixMode(GL_PROJECTION);
  glPopMatrix();

  viewport.camera.leaveSelectionMode();

  hits = glRenderMode(GL_RENDER);
  if ( hits > 0 )
     {
       closest = 0; dist = 0xffffffff;
       while ( hits )
          {
            index = (hits-1)*4;
            if ( selectBuf[index+1] < dist )
               {
                 dist = selectBuf[index+1];
                 closest = selectBuf[index+3];
               }
            hits--;
          }
          // closest now has the id of the selected face vertex
       sel = DLFLObject::fvparray[closest];
     }
  return sel;
}

   // Draw the selected items
void DLFLViewport :: drawSelected(void)
{
  if ( !sel_vptr_array.empty() )
     {
          // Selected vertices are shown in red color
       glPointSize(5.0);
       glBegin(GL_POINTS);
       glColor3f(1,0,0);
       DLFLVertexPtrArray :: iterator first, last;
       first = sel_vptr_array.begin(); last = sel_vptr_array.end();
       while ( first != last )
          {
            (*first)->render(); ++first;
          }
       glEnd();
       glPointSize(1.0);
     }

  if ( !sel_eptr_array.empty() )
     {
          // Selected edges are shown in blue color
       glLineWidth(3.0);
       glColor3f(0,0,1);
       DLFLEdgePtrArray :: iterator first, last;
       first = sel_eptr_array.begin(); last = sel_eptr_array.end();
       while ( first != last )
          {
            (*first)->render(); ++first;
          }
       glLineWidth(1.5);
     }

  if ( !sel_fptr_array.empty() )
     {
          // Selected faces are shown in cyan color
       glLineWidth(3.0);
       glColor3f(0,1,1);
       DLFLFacePtrArray :: iterator first, last;
       first = sel_fptr_array.begin(); last = sel_fptr_array.end();
       while ( first != last )
          {
            (*first)->render_FVN(); ++first;
          }
       glLineWidth(1.5);
     }

  if ( !sel_fvptr_array.empty() )
     {
          // Selected corners (vertices) are shown in green color
       glPointSize(6.0);
       glColor3f(0,1,0);
       glBegin(GL_POINTS);
       DLFLFaceVertexPtrArray :: iterator first, last;
       first = sel_fvptr_array.begin(); last = sel_fvptr_array.end();
       while ( first != last )
          {
            (*first)->render(); ++first;
          }
       glEnd();
       glPointSize(1.5);
     }
}
  

   // Draw the contents of the viewport
void DLFLViewport :: draw(void)
{
  if ( !valid() )
     {
       valid(1);
       viewport.resize(w(),h());
       // grid.generate();
          // Do initialization for renderer
       renderer->initialize();
     }

  glClearColor(1.0,1.0,1.0,1);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glEnable(GL_DEPTH_TEST);

  glMatrixMode(GL_MODELVIEW);

  glPushMatrix();

     // Multiply by the transformation matrix.
  viewport.apply_transform();

     // Draw a grid if required. Use thin gray lines
  // if ( showgrid )
//      {
//        glLineWidth(0.5); glColor3f(0.3,0.3,0.3); grid.render();
//      }
         
     // Draw the axes if required. Use thick lines
  if ( showaxes )
     {
       glLineWidth(2.0);
       glBegin(GL_LINES);

       glColor3f(1.0,0.0,0.0);                     // X Axis - Red
       glVertex3f(0,0,0); glVertex3f(2,0,0);

       glColor3f(0.0,1.0,0.0);                     // Y Axis - Green
       glVertex3f(0,0,0); glVertex3f(0,2,0);

       glColor3f(0.0,0.0,1.0);                     // Z Axis - Blue
       glVertex3f(0,0,0); glVertex3f(0,0,2);

       glEnd();
       glLineWidth(1.5);
     }

     // Draw the actual object using the renderer.
     // Assumes that render flags have already been set for the renderer
     // Default color is slightly bluish
  glColor3f(0.2,0.5,0.85);
  if ( renderer ) renderer->render(object);

     // Show any selected items.
     // Adjust the depthrange so selected items are shown clearly
  glDepthRange(0,1-0.0005-0.0005);
  DLFLViewport :: drawSelected();
  glDepthRange(0,1);

  glPopMatrix();
  glFlush();
}


   // Handle mouse and focus events in this viewport
   // When the mouse enters this viewport, it sets the active viewport to be itself
   // Mouse events are handled only for navigational controls, not selection
   // Does not handle focus events
int DLFLViewport :: handle(int event)
{
  int x = Fl::event_x(), y = Fl::event_y();
  if ( event == FL_RELEASE || event == FL_DRAG )
     {
          // The mouse was dragged or released
          // If the ALT key was also pressed handle navigation
       if ( Fl::event_state(FL_ALT) )
          {
            if ( viewport.send_to_current(DLFLViewport::translateFLTKEvent(event),x,y) ) redraw();
            if ( event == FL_RELEASE ) fl_cursor(FL_CURSOR_DEFAULT);
            return 1;
          }
     }
  else if ( event == FL_PUSH )
     {
          // A mouse button was pressed
          // If the ALT key was also pressed handle navigation
       if ( Fl::event_state(FL_ALT) ) 
          {
            if ( Fl::event_button() == FL_LEFT_MOUSE ) // Left mouse was pressed
               {
                 if ( viewport.current() == VPPan ) // Implies middle mouse is already down
                    {
                      viewport.handle_pan(VPRelease,x,y); // Stop panning
                      viewport.handle_zoom(VPPush,x,y); // Start zooming
                      fl_cursor(FL_CURSOR_WE,FL_RED);
                    }
                 else                      // Middle mouse is not down
                    {
                      viewport.handle_rotate(VPPush,x,y); // Start rotating
                      fl_cursor(FL_CURSOR_CROSS,FL_RED);
                    }
               }
            else if ( Fl::event_button() == FL_RIGHT_MOUSE ) // Middle mouse was pressed
               {
                 if ( viewport.current() == VPRotate ) // Implies left mouse is already down
                    {
                      viewport.handle_rotate(VPRelease,x,y); // Stop rotating
                      viewport.handle_zoom(VPPush,x,y); // Start zooming
                      fl_cursor(FL_CURSOR_WE,FL_RED);
                    }
                 else                        // Left mouse is not down
                    {
                      viewport.handle_pan(VPPush,x,y); // Start panning
                      fl_cursor(FL_CURSOR_MOVE,FL_RED); 
                    }
               }
            return 1;
          }
     }

     // All other events are reported as not handled
  return 0;
}

/*
  $Log: DLFLViewport.cc,v $
  Revision 4.0  2003/12/26 01:58:53  vinod
  Major version sync.

  Revision 3.0  2003/09/19 16:06:47  vinod
  Major version update

  Revision 2.8  2002/10/30 03:43:12  vinod
  Added call to initialize renderer in draw method

  Revision 2.7  2002/10/25 03:19:45  vinod
  Fixed warnings about negative numbers for GLuint,
  too large a number for dist in selection methods

  Revision 2.6  2002/10/22 21:24:12  vinod
  Added check for NULL renderer

  Revision 2.5  2002/10/16 14:15:31  vinod
  Bug fix: Check for middle mouse in navigation

  Revision 2.4  2002/10/08 16:19:08  vinod
  Bug fix. Removed line defining active viewport

  Revision 2.3  2002/10/08 16:18:22  vinod
  Removed handling of FL_ENTER and FL_LEAVE events. Active viewport is no longer a member

  Revision 2.2  2002/10/02 16:43:52  vinod
  Selection lists are now implemented as arrays.

  Revision 2.1  2002/09/23 07:17:11  vinod
  Viewport class to handle display of object and mouse events

*/
