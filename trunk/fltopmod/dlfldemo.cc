#include <Viewport.hh>
#include <Grid.hh>
#include <fstream.h>
#include <FL/Fl.H>
#include <FL/Fl_Gl_Window.H>
#include <FL/fl_draw.H>
#include <FL/Fl_Tile.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Menu_Bar.H>
#include <FL/fl_file_chooser.H>
#include <FL/Fl_Output.H>
#include <FL/Fl_Slider.H>
#include <FL/Fl_Value_Slider.H>
#include <FL/Fl_Value_Input.H>
#include <FL/Fl_Check_Button.H>
#include <Texture.hh>

#include "DLFLObject.hh"

#define OPTX 0
#define OPTY 0
#define OPTWIDTH 250
#define OPTHEIGHT 800

#define TITLEX OPTWIDTH
#define TITLEY 0
#define TITLEWIDTH 800
#define TITLEHEIGHT 30

#define MAINX OPTWIDTH
#define MAINY TITLEHEIGHT
#define MAINWIDTH TITLEWIDTH
#define MAINHEIGHT (OPTHEIGHT-TITLEHEIGHT)

#define WIDTH (OPTWIDTH + MAINWIDTH)
#define HEIGHT OPTHEIGHT

#define BORDER 2

#define MWIDTH 800
#define MHEIGHT 800
#define SWIDTH (MWIDTH >> 1)
#define SHEIGHT (MHEIGHT >> 1)
#define SBORDER 2

// Define macros for FLTK mouse buttons.
#define FL_LEFT_MOUSE 1
#define FL_MIDDLE_MOUSE 2
#define FL_RIGHT_MOUSE 3

enum RenderMode { Normal=0, Wireframe=1, Shaded=2, Silhouette=3, Textured=4,
                  Lit=5, LitOnTextured=6 };

typedef void (*Renderer)(void);

DLFLObject object;
Renderer renderer;
DLFLVertexPtr svptr = NULL;
DLFLEdgePtr septr = NULL;
DLFLEdgePtr septr2 = NULL;
DLFLFacePtr afptr1 = NULL;
DLFLFacePtr afptr2 = NULL;
DLFLFacePtr sfptr = NULL;
DLFLFacePtr sfptr2 = NULL;
DLFLFaceVertexPtr sfvptr = NULL;
DLFLFaceVertexPtr sfvptr2 = NULL;
DLFLFaceVertexPtr pfvptr = NULL;
DLFLFaceVertexPtr nfvptr = NULL;
DLFLMaterialPtr mptr = NULL;
bool firstvertexselected = false;                      // Used in edge insertion
bool secondvertexselected = false;
bool firstedgeselected = false;                        // Used in connecting edges
bool secondedgeselected = false;
GLuint texid;                                          // GL ID for the texture
Texture texture;                                       // Texture for the object

Fl_Output * title;                                     // Title for the window
Fl_Window * mainwindow;                                // Main Window
Fl_Window * titlewin;                                  // Window for the title
Fl_Window * optwin;                                    // Window for options

class Window;                                          // Forward declaration

Window * pwin;                                         // Perspective view window
Window * twin;                                         // Top view window
Window * fwin;                                         // Front view window
Window * rwin;                                         // Right view window

PointLight plight;

// Flags for the renderer
int drawgrid = 0;                                      // Toggle grid
int drawaxes = 0;                                      // Toggle axes
int wireframe = 0;                                     // Toggle wireframe
int silhouette = 0;                                    // Toggle silhouette
int lighting = 0;                                      // Toggle lit render
int showpoints = 0;                                    // Toggle vertices (only if silhouette=0)

// Parameters for various DLFL subroutines (connect, extrude, etc.)
int num_segments = 4;                                  // No. of segments in a connect
int max_segments = -1;                                 // Max. no. of segments to connect
double extrude_dist = 1.0;                             // Distance for an extrude
double extrude_rot = 0.0;                              // Rotation for an extrude
double extrude_scale = 1.0;                            // Scaling for an extrude
double num_extrusions = 1;                             // No. of segments for an extrude
double nwt1 = 5.0;                                     // Weight for normal of first face in hermite connections
double nwt2 = 5.0;                                     // Weight for normal of second face in hermite connections
int num_extra_twists = 0;                              // No. of extra twists in handles
int num_e_subdivs = 2;                                 // No. of sub-divisions for edge sub-division
double crust_thickness = 0.5;                          // Thickness of crust
double crust_scale_factor = 0.9;                       // Scale factor to generate thickness of crust
double crust_cleanup = true;                           // Cleanup flag for crust modeling
double twist_factor = 0.0;                             // Twist factor for root-4 subdivision, doo-sabin extrude
double weight_factor = 0.0;                            // Weight factor for root-4 subdivision

int CBW_Slider=0;
int CBW_ValueInput=1;

Fl_Value_Slider * cthick_slider;
Fl_Value_Slider * cscale_slider;
Fl_Value_Input * cthick_vinput;

void resetSelected(void)
{
     // Reset all selections
  svptr = NULL;
  septr = NULL;
  afptr1 = NULL;
  afptr2 = NULL;
  sfptr = NULL;
  sfptr2 = NULL;
  sfvptr = NULL;
  sfvptr2 = NULL;
  pfvptr = NULL;
  nfvptr = NULL;
  firstvertexselected = NULL;
  secondvertexselected = NULL;
  firstedgeselected = NULL;
  secondedgeselected = NULL;
}

void setUpTexturing(void)
{
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

  glGenTextures(1,&texid);
  glBindTexture(GL_TEXTURE_2D, texid);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
  if ( texture.isValid() ) glTexture(texture);
}

void drawWireFrame(void)
{
  glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
  glColor3f(0.1,0.1,0.2); glLineWidth(0.9);
  glDepthRange(0.0,1.0-0.0005);
  object.plainRender();
}

void drawSilhouette(void)
{
  glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
  glColor3f(0.0,0.0,0.0); glLineWidth(6.0);
  glDepthRange(0.1,1.0);
  object.plainRender();
}

void drawPoints(void)
{
  glPolygonMode(GL_FRONT_AND_BACK,GL_POINTS);
  glColor3f(1.0,0.0,0.0); glPointSize(5.0);
  glDepthRange(0.0,1.0-0.005);
  object.plainRender();
}

void litRenderer(void)
{
     // Render using light information

  glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
  object.renderFaces();

  if ( wireframe ) drawWireFrame();
  if ( silhouette ) drawSilhouette();
  else if ( showpoints ) drawPoints();
}

void litOnTexturedRenderer(void)
{
     // Texturing
  glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, texid);
  object.renderFacesT();
  glDisable(GL_TEXTURE_2D);

  if ( wireframe ) drawWireFrame();
  if ( silhouette ) drawSilhouette();
  else if ( showpoints ) drawPoints();
}

void normalRenderer(void)
{
     // Normal render
  glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
  glColor3f(0.25,0.25,0.25);
  object.plainRender();

  if ( wireframe ) drawWireFrame();
  if ( silhouette ) drawSilhouette();
  else if ( showpoints ) drawPoints();
}

void wireframeRenderer(void)
{
     // Wireframe render
  drawWireFrame();
  if ( showpoints ) drawPoints();
}

void shadedRenderer(void)
{
     // Wireframe on shaded
  glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
  object.render();

  if ( wireframe ) drawWireFrame();
  if ( silhouette ) drawSilhouette();
  else if ( showpoints ) drawPoints();
}

void silhouetteRenderer(void)
{
     // Silhouette
  glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
  object.render();

  drawSilhouette();
  if ( wireframe ) drawWireFrame();
}

void texturedRenderer(void)
{
     // With texturing
  glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
  glColor3f(1.0,1.0,1.0);
  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, texid);
  object.plainRenderT();
  glDisable(GL_TEXTURE_2D);

  if ( wireframe ) drawWireFrame();
  if ( silhouette ) drawSilhouette();
  else if ( showpoints ) drawPoints();
}

void setRenderMode(RenderMode rmode)
{
     // Change the renderer depending on the Render mode
  switch ( rmode )
     {
       case Normal :
                    renderer = normalRenderer;
                    break;
       case Wireframe :
                    renderer = wireframeRenderer;
                    break;
       case Shaded :
                    renderer = shadedRenderer;
                    break;
       case Silhouette :
                    renderer = silhouetteRenderer;
                    break;
       case Textured :
                    renderer = texturedRenderer;
                    break;
       case Lit :
                    renderer = litRenderer;
                    break;
       case LitOnTextured :
                    renderer = litOnTexturedRenderer;
                    break;
       default :
                    renderer = normalRenderer;
     }
}

// Subroutine to translate FL events to Viewport events
inline VPMouseEvent fl_to_vp(int flevent)
{
  return ( (flevent == FL_PUSH) ? VPPush :
           ( (flevent == FL_RELEASE) ? VPRelease :
             ( (flevent == FL_DRAG) ? VPDrag : VPUnknown ) ) );
}


void setObject(const char * filename)
{
  ifstream file;
  file.open(filename);
  cout << setprecision(3);
  if ( strstr(filename,"dlfl") || strstr(filename,"DLFL") )
     object.readDLFL(file);
  else if ( strstr(filename,"obj") || strstr(filename,"OBJ") )
     object.readObject(file);
  file.close();

  object.print();
  cout << endl;
}

enum Mode { NormalMode=0,
            SelectVertex=1, SelectEdge=2, SelectFace=3, SelectFaceVertex=4,
            InsertEdge=11, DeleteEdge=12, RemoveEdge=13, SubDivideEdge=14, ConnectEdges=15,
            ExtrudeFace=21, ExtrudeFaceDS=22,
            ConnectFaceVertices=31, ConnectFaces=32, BezierConnectFaces=33, HermiteConnectFaces=34,
            ReorderFace=41,
            CrustModeling=51 };

class Window : public Fl_Gl_Window
{
  protected :

     Grid grid;
     bool infocus;
     static Mode smode;

  public :
     
     Viewport viewport;

  public :

     Window(int x, int y, int w, int h, const char *l=0)
       : Fl_Gl_Window(x,y,w,h,l), grid(ZX,20.0,20), infocus(false), viewport(w,h)
       {}

     ~Window()
       {}

     void switchTo(VPView view)
       {
            // Change grid orientation for front/back and left/right views
         switch ( view )
            {
              case VPFront :
              case VPBack :
                           grid.setPlane(XY);
                           break;
              case VPLeft :
              case VPRight :
                           grid.setPlane(YZ);
                           break;
              default :
                           grid.setPlane(ZX);
            }
         viewport.switchTo(view);
         if ( view == VPPersp )
               // Change camera settings to zoom in closer in perspective view
            viewport.setPerspView(10,10,10,0,0,0,0,1,0);
       }

     DLFLVertexPtr selectVertex(int mx, int my)
       {
         GLuint selectBuf[1024];
         uint closest;
         GLuint dist;
         long hits, index;
         DLFLVertexPtr sel(NULL);
         
         glSelectBuffer(1024,selectBuf);
         glRenderMode(GL_SELECT);

         glInitNames(); glPushName(-1);

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
         object.renderVerticesForSelect();
         glFlush();

         glPopMatrix();

         glMatrixMode(GL_PROJECTION);
         glPopMatrix();

         viewport.camera.leaveSelectionMode();

         hits = glRenderMode(GL_RENDER);
         if ( hits > 0 )
            {
              closest = 0; dist = 4294967295;
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

     DLFLEdgePtr selectEdge(int mx, int my)
       {
         GLuint selectBuf[1024];
         uint closest;
         GLuint dist;
         long hits, index;
         DLFLEdgePtr sel(NULL);
         
         glSelectBuffer(1024,selectBuf);
         glRenderMode(GL_SELECT);

         glInitNames(); glPushName(-1);

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
         object.renderEdgesForSelect();
         glFlush();

         glPopMatrix();

         glMatrixMode(GL_PROJECTION);
         glPopMatrix();

         viewport.camera.leaveSelectionMode();

         hits = glRenderMode(GL_RENDER);
         if ( hits > 0 )
            {
              closest = 0; dist = 4294967295;
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

     DLFLFacePtr selectFace(int mx, int my)
       {
         GLuint selectBuf[1024];
         uint closest;
         GLuint dist;
         long hits, index;
         DLFLFacePtr sel(NULL);
         
         glSelectBuffer(1024,selectBuf);
         glRenderMode(GL_SELECT);

         glInitNames(); glPushName(-1);

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
         object.renderFacesForSelect();
         glFlush();

         glPopMatrix();

         glMatrixMode(GL_PROJECTION);
         glPopMatrix();

         viewport.camera.leaveSelectionMode();

         hits = glRenderMode(GL_RENDER);
         if ( hits > 0 )
            {
              closest = 0; dist = 4294967295;
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

     DLFLFaceVertexPtr selectFaceVertex(DLFLFacePtr fp, int mx, int my)
       {
         GLuint selectBuf[1024];
         uint closest;
         GLuint dist;
         long hits, index;
         DLFLFaceVertexPtr sel(NULL);
         
         glSelectBuffer(1024,selectBuf);
         glRenderMode(GL_SELECT);

         glInitNames(); glPushName(-1);

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
         object.renderFaceVerticesForSelect(fp);
         glFlush();

         glPopMatrix();

         glMatrixMode(GL_PROJECTION);
         glPopMatrix();

         viewport.camera.leaveSelectionMode();

         hits = glRenderMode(GL_RENDER);
         if ( hits > 0 )
            {
              closest = 0; dist = 4294967295;
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

     void doSelection(int x, int y)
       {
         switch ( smode )
            {
              case SelectVertex :
                           svptr = selectVertex(x,y);
                           if ( svptr )
                              {
                                cout << "Selected Vertex: "; svptr->print();
                              }
                           break;
              case DeleteEdge :
              case RemoveEdge :
              case SubDivideEdge :
              case SelectEdge :
                           septr = selectEdge(x,y);
                           if ( septr )
                              {
                                cout << "Selected Edge: " << septr->getID() << endl;
                                if ( smode == SelectEdge ) septr->getFacePointers(afptr1,afptr2);
                              }
                           break;
              case SelectFace :
              case ExtrudeFace :
              case ExtrudeFaceDS :
              case CrustModeling :
                           sfptr = selectFace(x,y);
                           if ( sfptr )
                              {
                                cout << "Selected Face: " << sfptr->getID() << endl;
                                if ( smode == SelectFace )
                                   {
                                     sfptr->print(); cout << endl;
                                   }
                              }
                           break;
              case ConnectFaces :
                              // We have to select 2 faces We will use the closest vertex connection subroutine
                           if ( firstvertexselected == false )
                              {
                                   // Select first face
                                sfptr = selectFace(x,y);
                              }
                           else if ( secondvertexselected == false )
                              {
                                   // Select second face
                                sfptr2 = selectFace(x,y);
                              }
                           break;
              case SelectFaceVertex :
              case ReorderFace :
                           sfptr = selectFace(x,y);
                           if ( sfptr )
                              {
                                sfvptr = selectFaceVertex(sfptr,x,y);
                                if ( sfvptr )
                                   {
                                        //pfvptr = sfptr->prevFaceVertex(sfvptr);
                                        //nfvptr = sfptr->nextFaceVertex(sfvptr);
                                     pfvptr = sfvptr->prev();
                                     nfvptr = sfvptr->next();
                                   }
                              }
                           break;
              case ConnectEdges :
                              // Select 2 half-edges (Edge,Face pair)
                           if ( firstedgeselected == false )
                              {
                                sfptr = selectFace(x,y);
                                if ( sfptr ) septr = selectEdge(x,y);
                              }
                           else if ( secondedgeselected == false )
                              {
                                sfptr2 = selectFace(x,y);
                                if ( sfptr2 ) septr2 = selectEdge(x,y);
                              }
                           break;
              case InsertEdge :
              case ConnectFaceVertices :
              case BezierConnectFaces :
              case HermiteConnectFaces :
                              // We have to select 2 face vertices
                           if ( firstvertexselected == false )
                              {
                                   // Select first face/facevertex
                                sfptr = selectFace(x,y);
                                if ( sfptr ) sfvptr = selectFaceVertex(sfptr,x,y);
                              }
                           else if ( secondvertexselected == false )
                              {
                                   // Select second face/facevertex
                                sfptr2 = selectFace(x,y);
                                if ( sfptr2 ) sfvptr2 = selectFaceVertex(sfptr2,x,y);
                              }
                           break;
            }
       }
     
     int handle(int event)
       {
         int x = Fl::event_x(), y = Fl::event_y();
         if ( event == FL_FOCUS )
            {
              infocus = true;
              return 1;
            }
         else if ( event == FL_ENTER )
            {
              infocus = true; Fl::focus(this);
              return 1;
            }
         else if ( event == FL_LEAVE )
            {
              infocus = false;
              return 1;
            }
         else if ( event == FL_KEYBOARD )
            {
                 // Do several things depending on which key was pressed
              int key = Fl::event_key();
              bool ctrl = Fl::event_state(FL_CTRL);
              bool alt = Fl::event_state(FL_ALT);
              bool shift = Fl::event_state(FL_SHIFT);
              
              if ( ctrl )
                 {
                   switch ( key )
                      {
                        case 'p' :
                                     cout << "Toggle vertex display" << endl;
                                     title->value("Toggle vertex display");
                                     titlewin->redraw(); 
                                     showpoints ^= 1;
                                     break;
                        case 's' :
                                     if ( Fl::event_state(FL_SHIFT) )
                                        renderer = silhouetteRenderer;
                                     else
                                        {
                                          cout << "Toggle silhouette" << endl;
                                          title->value("Toggle Silhouette");
                                          titlewin->redraw();
                                          silhouette ^= 1;
                                        }
                                     break;
                        case 'w' :
                                     if ( Fl::event_state(FL_SHIFT) )
                                        renderer = wireframeRenderer;
                                     else
                                        {
                                          cout << "Toggle wireframe" << endl;
                                          title->value("Toggle wireframe");
                                          titlewin->redraw();
                                          wireframe ^= 1;
                                        }
                                     break;
                        case 'l' :
                                     if ( renderer == texturedRenderer || renderer == litOnTexturedRenderer )
                                        renderer = litOnTexturedRenderer;
                                     else
                                        renderer = litRenderer;
                                     break;
                        case 'n' :
                                     if ( Fl::event_state(FL_SHIFT) )
                                        renderer = shadedRenderer;
                                     else
                                        renderer = normalRenderer;
                                     break;
                        case 't' :
                                     if ( renderer == litRenderer || renderer == litOnTexturedRenderer )
                                        renderer = litOnTexturedRenderer;
                                     else
                                        renderer = texturedRenderer;
                                     break;

                        case 'v' :
                                        // Go into vertex selection mode
                                     cout << "Select a Vertex" << endl;
                                     title->value("Select a Vertex");
                                     titlewin->redraw();
                                     smode = SelectVertex;
                                     break;
                        case 'e' :
                                        // Go into edge selection mode
                                     cout << "Select an Edge" << endl;
                                     title->value("Select an Edge");
                                     titlewin->redraw();
                                     smode = SelectEdge;
                                     break;
                        case 'f' :
                                        // Go into face selection mode
                                     cout << "Select a Face" << endl;
                                     title->value("Select a Face");
                                     titlewin->redraw();
                                     smode = SelectFace;
                                     break;
                        case 'c' :
                                        // Go into face-vertex selection mode
                                     cout << "Select a FaceVertex" << endl;
                                     title->value("Select a FaceVertex(Corner)");
                                     titlewin->redraw();
                                     smode = SelectFaceVertex;
                                     break;
                        case 'u' :
                                        // Go out of selection mode
                                     smode = NormalMode;
                                     break;
                      }
                   if ( key == 'l' || key == 'p' || key == 's' ||
                        key == 'w' || key == 'n' || key == 't' ) redraw();
                 }
              else if ( alt )
                 {
                   ofstream ofile;
                   ifstream ifile;
                   char * ipfile, * opfile;
                   switch (key)
                      {
                        case 'e' :
                                     cout << "Select an Edge for deletion (point-spheres will be cleaned-up)" << endl;
                                     title->value("Select an Edge for deletion (with clean-up)");
                                     titlewin->redraw();
                                     smode = DeleteEdge;
                                     break;
                        case 'd' :
                                     cout << "Select an Edge for deletion (point-spheres will not be cleaned-up)" << endl;
                                     title->value("Select an Edge for deletion (without clean-up)");
                                     titlewin->redraw();
                                     smode = RemoveEdge;
                                     break;
                        case 's' :
                                     cout << "Select an Edge to subdivide" << endl;
                                     title->value("Select an Edge to subdivide");
                                     titlewin->redraw();
                                     smode = SubDivideEdge;
                                     break;
                        case 'i' :
                                        // Insert Edge Mode
                                     cout << "Select 2 FaceVertexes for Edge Insertion" << endl;
                                     title->value("Select 2 FaceVertexes for Edge Insertion");
                                     titlewin->redraw();
                                     smode = InsertEdge;
                                     break;
                        case 'h' :
                                        // Connect Faces (create hole/handle)
                                     if ( shift )
                                        {
                                          cout << "Select 2 Faces (closest vertex method)" << endl;
                                          title->value("Select 2 Faces for creating Hole/Handle (closest vertex method)");
                                          smode = ConnectFaces;
                                        }
                                     else
                                        {
                                          cout << "Select 2 FaceVertexes" << endl;
                                          title->value("Select 2 FaceVertexes for creating Hole/Handle");
                                          smode = ConnectFaceVertices;
                                        }
                                     titlewin->redraw();
                                     break;
                        case 'x' :
                                        // Extrude a face
                                     if ( shift )
                                        {
                                          cout << "Select a Face to extrude (Doo-Sabin extrude)" << endl;
                                          title->value("Select a Face to extrude (Doo-Sabin)");
                                          smode = ExtrudeFaceDS;
                                        }
                                     else
                                        {
                                          cout << "Select a Face to extrude" << endl;
                                          title->value("Select a Face to extrude");
                                          smode = ExtrudeFace;
                                        }
                                     titlewin->redraw();
                                     break;
                        case 't' :
                                        // Crust Modeling
                                     cout << "Crust Modeling Mode" << endl;
                                     cout << "Select faces to create holes" << endl;
                                     title->value("Crust Modeling : Select a face");
                                     titlewin->redraw();
                                     if ( shift )
                                        object.createCrustWithScaling(crust_scale_factor);
                                     else
                                        object.createCrust(crust_thickness);
                                     smode = CrustModeling;
                                     break;
                        case 'b' :
                                        // Bezier Connect Faces (create handle)
                                     cout << "Select 2 FaceVertexes" << endl;
                                     title->value("Select 2 FaceVertexes for creating Bezier Handle");
                                     titlewin->redraw();
                                     smode = BezierConnectFaces;
                                     break;
                        case 'm' :
                                        // Hermite Connect Faces (create handle)
                                     cout << "Select 2 FaceVertexes" << endl;
                                     title->value("Select 2 FaceVertexes for creating Hermite Handle");
                                     titlewin->redraw();
                                     smode = HermiteConnectFaces;
                                     break;
                        case 'c' :
                                        // Connect 2 half-edges
                                     cout << "Select 2 Half-Edges (Edge,Face pair)" << endl;
                                     title->value("Select 2 half-edges for connecting");
                                     titlewin->redraw();
                                     smode = ConnectEdges;
                                     break;
                        case 'r' :
                                        // Reorder face
                                     cout << "Select a FaceVertex" << endl;
                                     title->value("Select a FaceVertex to reorder the corresponding Face with");
                                     titlewin->redraw();
                                     smode = ReorderFace;
                                     break;
                        case 'w' :
                                     opfile = fl_file_chooser("Write DLFL","*.dlfl","dlfl.dlfl");
                                     ofile.open(opfile);
                                     if ( ofile ) object.writeDLFL(ofile);
                                     ofile.close();
                                     Fl :: focus(pwin);
                                     break;
                        case 'o' :
                                     ipfile = fl_file_chooser("Read DLFL","*.dlfl","dlfl.dlfl");
                                     ifile.open(ipfile);
                                     if ( ifile ) object.readDLFL(ifile);
                                     ifile.close();
                                     Fl :: focus(pwin);
                                     smode = NormalMode;
                                     break;
                        case 'f' :
                                     ipfile = fl_file_chooser("Read OBJ (Alternate)","*.obj","test.obj");
                                     ifile.open(ipfile);
                                     if ( ifile ) object.readObjectAlt(ifile);
                                     ifile.close();
                                     Fl :: focus(pwin);
                                     smode = NormalMode;
                                     break;
                      }
                      // Unselect any previously selected face/facevertex/edge/vertex
                   resetSelected();
                 }
              else if ( key == 'a' && infocus )
                 {
                   title->value("Subdividing all edges");
                   titlewin->redraw();
                   mainwindow->redraw();
                   object.subDivideAllEdges(num_e_subdivs);
                   resetSelected();
                   redraw();
                   smode = NormalMode;
                 }
	      else if ( key == '3' && infocus )
                 {
                   title->value("Planarize All Faces");
                   titlewin->redraw();
                   mainwindow->redraw();
                   object.planarize();
                   resetSelected();
                   redraw();
                   smode = NormalMode;
                 }
	     else if ( key == '2' && infocus )
                 {
                   title->value("Make Object Spherical");
                   titlewin->redraw();
                   mainwindow->redraw();
                   object.spheralize();
                   resetSelected();
                   redraw();
                   smode = NormalMode;
                 }
	    else if ( key == '1' && infocus )
                 {
                   title->value("Make Object Smooth");
                   titlewin->redraw();
                   mainwindow->redraw();
                   object.meshsmooth();
                   resetSelected();
                   redraw();
                   smode = NormalMode;
                 }
              else if ( key == 's' )
                 {
                   title->value("Catmull-clark subdivision");
                   titlewin->redraw();
                   mainwindow->redraw();
                   object.subDivide();
                   object.computeNormals();
                   object.computeLighting(&plight);
                   resetSelected();
                   redraw();
                   smode = NormalMode;
                 }
              else if ( key == 'h' )
                 {
                   title->value("Honeyomb subdivision");
                   titlewin->redraw();
                   mainwindow->redraw();
                   object.honeycombSubDivide();
                   object.computeNormals();
                   object.computeLighting(&plight);
                   resetSelected();
                   redraw();
                   smode = NormalMode;
                 }
              else if ( key == 'd' )
                 {
                   title->value("Doo-Sabin subdivision");
                   titlewin->redraw();
                   mainwindow->redraw();
                   object.dooSabinSubDivide();
                   object.computeNormals();
                   object.computeLighting(&plight);
                   resetSelected();
                   redraw();
                   smode = NormalMode;
                 }
              else if ( key == 'c' )
                 {
                   title->value("Corner-Cutting subdivision");
                   titlewin->redraw();
                   mainwindow->redraw();
                   object.cornerCuttingSubDivide();
                   object.computeNormals();
                   object.computeLighting(&plight);
                   resetSelected();
                   redraw();
                   smode = NormalMode;
                 }
              else if ( key == '4' )
                 {
                   title->value("Root-4 subdivision");
                   titlewin->redraw();
                   mainwindow->redraw();
                   object.root4SubDivide(weight_factor,twist_factor);
                   object.computeNormals();
                   object.computeLighting(&plight);
                   resetSelected();
                   redraw();
                   smode = NormalMode;
                 }
              else if ( key == 'r' )
                 {
                   title->value("Crust creation");
                   titlewin->redraw();
                   mainwindow->redraw();
                   if ( shift )
                      object.createCrustWithScaling(crust_scale_factor);
                   else
                      object.createCrust(crust_thickness);
                   object.computeNormals();
                   object.computeLighting(&plight);
                   resetSelected();
                   redraw();
                 }
              else if ( key == 'n' )
                 {
                   object.computeNormals();
                   object.computeLighting(&plight);
                   redraw();
                 }
              else if ( key == 'l' )
                 {
                   object.computeLighting(&plight);
                   redraw();
                 }
              else if ( key == 't' )
                 {
                   object.randomAssignTexCoords();
                   redraw();
                 }
              else if ( key == 'w' )
                 {
                   char * opfile;
                   opfile = fl_file_chooser("Write OBJ","*.obj","dlfl.obj");
                   ofstream ofile(opfile);
                   if ( ofile ) object.objWrite(ofile,false,true);
                   ofile.close();
                   Fl :: focus(pwin);
                 }
              else if ( key == 'o' )
                 {
                   char * ipfile;
                   ipfile = fl_file_chooser("Read OBJ","*.obj","dlfl.obj");
                   ifstream ifile(ipfile);
                   if ( ifile ) object.readObject(ifile);
                   ifile.close();
                   Fl :: focus(pwin);
                   smode = NormalMode;
                 }
              else if ( key == 'p' ) object.print();
              else if ( key == 'v' ) object.printVertexList();
              else if ( key == 'e' ) object.printEdgeList();
              else if ( key == 'f' ) object.printFaces();
              else if ( key == 'x' )
                 {
                   drawaxes ^= 1; redraw();
                 }
              else if ( key == 'g' )
                 {
                   drawgrid ^= 1; redraw();
                 }
              else if ( key == 'q' )
                 exit(0);
              if ( infocus ) return 1;
              else return 0;
            }
         else if ( event == FL_RELEASE || event == FL_DRAG )
            {
                 // The mouse was dragged or released
                 // Send this event to the subroutine handling the current event, if any
                 // If we are not currently doing anything, nothing will happen
              if ( !Fl::event_state(FL_ALT) )
                 {
                   if ( smode != NormalMode )
                      {
                           // We are doing some kind of selection or insertion
                        if ( event == FL_DRAG )
                           {
                             doSelection(x,h()-y);
                             if ( svptr != NULL || septr != NULL || sfptr != NULL ) redraw();
                           }
                        else if ( event == FL_RELEASE )
                           {
                                // If we are inserting an edge and have
                                // both corners selected insert the edge
                             if ( smode == InsertEdge )
                                {
                                  if ( firstvertexselected == false && sfvptr )
                                     firstvertexselected = true;
                                  else if ( secondvertexselected == false && sfvptr2 )
                                     secondvertexselected = true;
                                  if ( firstvertexselected && secondvertexselected )
                                     {
                                       if ( sfvptr && sfvptr2 )
                                          {
                                            mptr = sfptr->material();
                                            object.insertEdge(sfvptr,sfvptr2,false,mptr);
                                            smode = NormalMode;

                                               // RESET selected faces/facevertices!!!!
                                               // They are no longer valid!!
                                            sfptr = sfptr2 = NULL;
                                            sfvptr = sfvptr2 = NULL;

                                            redraw();   
                                          }
                                     }
                                }
                             else if ( smode == DeleteEdge )
                                {
                                     // With cleanup
                                  if ( septr ) object.deleteEdge(septr,true);
                                  septr = NULL;
                                  redraw();
                                }
                             else if ( smode == RemoveEdge )
                                {
                                     // Without cleanup
                                  if ( septr ) object.deleteEdge(septr,false);
                                  septr = NULL;
                                  redraw();
                                }
                             else if ( smode == SubDivideEdge )
                                {
                                  if ( septr ) object.subDivideEdge(num_e_subdivs,septr);
                                  septr = NULL;
                                  redraw();
                                }
                             else if ( smode == ConnectFaces )
                                {
                                  if ( firstvertexselected == false && sfptr )
                                     firstvertexselected = true;
                                  else if ( secondvertexselected == false && sfptr2 )
                                     secondvertexselected = true;
                                  if ( firstvertexselected && secondvertexselected )
                                     {
                                       if ( sfptr && sfptr2 )
                                          {
                                            object.connectFaces(sfptr,sfptr2,num_segments);
                                            smode = NormalMode;

                                               // RESET selected faces/facevertices!!!!
                                               // They are no longer valid!!
                                            sfptr = sfptr2 = NULL;
                                            redraw();   
                                          }
                                     }
                                }
                             else if ( smode == ConnectFaceVertices )
                                {
                                  if ( firstvertexselected == false && sfvptr )
                                     firstvertexselected = true;
                                  else if ( secondvertexselected == false && sfvptr2 )
                                     secondvertexselected = true;
                                  if ( firstvertexselected && secondvertexselected )
                                     {
                                       if ( sfvptr && sfvptr2 )
                                          {
                                            object.connectFaces(sfvptr,sfvptr2,num_segments);
                                            smode = NormalMode;

                                               // RESET selected faces/facevertices!!!!
                                               // They are no longer valid!!
                                            sfptr = sfptr2 = NULL;
                                            sfvptr = sfvptr2 = NULL;

                                            redraw();   
                                          }
                                     }
                                }
                             else if ( smode == ConnectEdges )
                                {
                                  if ( firstedgeselected == false && septr )
                                     firstedgeselected = true;
                                  else if ( secondedgeselected == false && septr2 )
                                     secondedgeselected = true;
                                  if ( firstedgeselected && secondedgeselected )
                                     {
                                       if ( septr && septr2 )
                                          {
                                            object.connectEdges(septr,sfptr,septr2,sfptr2);
                                            smode = NormalMode;

                                               // RESET selected faces/edges
                                            sfptr = sfptr2 = NULL;
                                            septr = septr2 = NULL;

                                            redraw();
                                          }
                                     }
                                }
                             else if ( smode == ExtrudeFace )
                                {
                                  if ( sfptr ) object.extrudeFace(sfptr,extrude_dist,num_extrusions,
                                                                  extrude_rot,extrude_scale);
                                  sfptr = NULL;
                                  redraw();
                                }
                             else if ( smode == ExtrudeFaceDS )
                                {
                                  if ( sfptr ) object.extrudeFaceDS(sfptr,extrude_dist,num_extrusions,
                                                                    twist_factor,extrude_scale);
                                  sfptr = NULL;
                                  redraw();
                                }
                             else if ( smode == CrustModeling )
                                {
                                  if ( sfptr )
                                     {
                                       object.cmMakeHole(sfptr,crust_cleanup);
                                       sfptr = NULL;
                                       redraw();
                                     }
                                }
                             else if ( smode == BezierConnectFaces )
                                {
                                  if ( firstvertexselected == false && sfvptr )
                                     firstvertexselected = true;
                                  else if ( secondvertexselected == false && sfvptr2 )
                                     secondvertexselected = true;
                                  if ( firstvertexselected && secondvertexselected )
                                     {
                                       if ( sfvptr && sfvptr2 )
                                          {
                                            object.bezierConnectFaces(sfvptr,sfvptr2,num_segments,nwt1,nwt2);
                                            smode = NormalMode;

                                               // RESET selected faces/facevertices!!!!
                                               // They are no longer valid!!
                                            sfptr = sfptr2 = NULL;
                                            sfvptr = sfvptr2 = NULL;
                                            redraw();   
                                          }
                                     }
                                }
                             else if ( smode == HermiteConnectFaces )
                                {
                                  if ( firstvertexselected == false && sfvptr )
                                     firstvertexselected = true;
                                  else if ( secondvertexselected == false && sfvptr2 )
                                     secondvertexselected = true;
                                  if ( firstvertexselected && secondvertexselected )
                                     {
                                       if ( sfvptr && sfvptr2 )
                                          {
                                            object.hermiteConnectFaces(sfvptr,sfvptr2,num_segments,nwt1,nwt2,
                                                                       max_segments,num_extra_twists);
                                            smode = NormalMode;

                                               // RESET selected faces/facevertices!!!!
                                               // They are no longer valid!!
                                            sfptr = sfptr2 = NULL;
                                            sfvptr = sfvptr2 = NULL;

                                            redraw();   
                                          }
                                     }
                                }
                             else if ( smode == ReorderFace )
                                {
                                  if ( sfvptr )
                                     {
                                       sfptr->reorder(sfvptr);
                                          // No need to reset any selections. They should still be valid
                                       redraw();
                                     }
                                }
                           }
                      }
                 }
              else
                 {
                   if ( viewport.send_to_current(fl_to_vp(event),x,y) ) redraw();
                   if ( event == FL_RELEASE ) fl_cursor(FL_CURSOR_DEFAULT);
                 }
              return 1; // Capture all drag/release events
            }
         else if ( event == FL_PUSH )
            {
              if ( Fl::event_state(FL_ALT) ) 
                 {
                      // A button was pushed with the ALT key pressed
                   if ( Fl::event_button() == FL_LEFT_MOUSE )// Left mouse was pressed
                      {
                        if ( viewport.current() == VPPan )  // Implies middle mouse is already down
                           {
                             viewport.handle_pan(VPRelease,x,y); // Stop panning
                             viewport.handle_zoom(VPPush,x,y); // Start zooming
                             fl_cursor(FL_CURSOR_WE,FL_RED);
                           }
                        else                                // Middle mouse is not down
                           {
                             viewport.handle_rotate(VPPush,x,y); // Start rotating
                             fl_cursor(FL_CURSOR_CROSS,FL_RED);
                           }
                      }
                   else                                     // Middle mouse was pressed
                      {
                        if ( viewport.current() == VPRotate ) // Implies left mouse is already down
                           {
                             viewport.handle_rotate(VPRelease,x,y); // Stop rotating
                             viewport.handle_zoom(VPPush,x,y);  // Start zooming
                             fl_cursor(FL_CURSOR_WE,FL_RED);
                           }
                        else                                // Left mouse is not down
                           {
                             viewport.handle_pan(VPPush,x,y);   // Start panning
                             fl_cursor(FL_CURSOR_MOVE,FL_RED);
                           }
                      }
                 }
              else if ( smode != NormalMode )
                 {
                      // We are doing some kind of selection
                   doSelection(x,h()-y);
                   if ( svptr != NULL || septr != NULL || sfptr != NULL ) redraw();
                 }
              return 1; // Capture all push events
            }

         return Fl_Gl_Window::handle(event);
       }

     void draw(void)
       {
         if ( !valid() )
            {
              valid(1);
              viewport.resize(w(),h());
              grid.generate();
              setUpTexturing();
            }

         glClearColor(1.0,1.0,1.0,1);
         glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

         glEnable(GL_DEPTH_TEST);

         glMatrixMode(GL_MODELVIEW);

         glPushMatrix();

             // Multiply by the transformation matrix.
         viewport.apply_transform();

            // Draw a grid in the ZX plane
         if ( drawgrid )
            {
              glLineWidth(0.5); glColor3f(0.3,0.3,0.3); grid.render();
            }
         
            // Draw the axes
         if ( drawaxes )
            {
              glLineWidth(2.0);
              glBegin(GL_LINES);

              glColor3f(0.0,1.0,1.0);                     // X Axis
              glVertex3f(0,0,0); glVertex3f(5,0,0);

              glColor3f(1.0,0.0,1.0);                     // Y Axis
              glVertex3f(0,0,0); glVertex3f(0,5,0);

              glColor3f(1.0,1.0,0.0);                     // Z Axis
              glVertex3f(0,0,0); glVertex3f(0,0,5);

              glEnd();
              glLineWidth(1.0);
            }

            // Draw sphere
         glColor3f(0.2,0.5,0.85);
         renderer();

            // If we have anything selected, draw it in blue
            // Adjust the depthrange so selected items are shown clearly
         glDepthRange(0,1-0.0005-0.0005);
         if ( svptr )
            {
              glPointSize(5.0);
              glBegin(GL_POINTS);
              glColor3f(1,1,0);
              svptr->render();
              glEnd();
              glPointSize(1.0);
            }
         if ( septr )
            {
              glLineWidth(3.0);
              glColor3f(0,0,1);
              septr->render();
              glLineWidth(1.0);
              if ( afptr1 || afptr2 )
                 {
                   glColor3f(0,1,0.5);
                   glLineWidth(2.0);
                   if ( afptr1 ) afptr1->render_FVN();
                   if ( afptr2 ) afptr2->render_FVN();
                 }
            }
         if ( sfptr )
            {
              glColor3f(0,1,1);
              glLineWidth(2.0);
              sfptr->render_FVN();
              if ( sfvptr )
                 {
                   glPointSize(6.0);
                   glBegin(GL_POINTS);
                   glColor3f(1,0,0); sfvptr->render();

                   if ( pfvptr )
                      {
                        glColor3f(0,1,0); pfvptr->render();
                      }
                   if ( nfvptr )
                      {
                        glColor3f(0,0,1); nfvptr->render();
                      }
                   glEnd();
                   glPointSize(1.0);
                 }
            }
         if ( sfptr2 )
            {
              glColor3f(0,1,1);
              glLineWidth(2.0);
              sfptr2->render_FVN();
              if ( sfvptr2 )
                 {
                   glPointSize(6.0);
                   glBegin(GL_POINTS);
                   glColor3f(1,0,0);
                   sfvptr2->render();
                   glEnd();
                   glPointSize(1.0);
                 }
              if ( septr2 )
                 {
                   glLineWidth(3.0);
                   glColor3f(0,0,1);
                   septr2->render();
                   glLineWidth(1.0);
                 }
            }
         glDepthRange(0,1);
         glPopMatrix();

         glFlush();
       }
};

Mode Window::smode = NormalMode;

void changeNumSegments(Fl_Widget * widget, void * data)
{
     // Call back for the numsegs value slider
  Fl_Value_Slider * slider = (Fl_Value_Slider *)widget;
  num_segments = slider->value();
  Fl :: focus(optwin);
}

void changeMaxSegments(Fl_Widget * widget, void * data)
{
     // Call back for the maxsegs value slider
  Fl_Value_Slider * slider = (Fl_Value_Slider *)widget;
  max_segments = slider->value();
  Fl :: focus(optwin);
}

void changeNWeight1(Fl_Widget * widget, void * data)
{
     // Call back for the nweight1 value slider
  Fl_Value_Slider * slider = (Fl_Value_Slider *)widget;
  nwt1 = slider->value();
  Fl :: focus(optwin);
}

void changeNWeight2(Fl_Widget * widget, void * data)
{
     // Call back for the nweight2  value slider
  Fl_Value_Slider * slider = (Fl_Value_Slider *)widget;
  nwt2 = slider->value();
  Fl :: focus(optwin);
}

void changeExtrudeDistance(Fl_Widget * widget, void * data)
{
     // Call back for the exlen value slider
  Fl_Value_Slider * slider = (Fl_Value_Slider *)widget;
  extrude_dist = slider->value();
  Fl :: focus(optwin);
}

void changeExtrudeScale(Fl_Widget * widget, void * data)
{
     // Call back for the exscale value slider
  Fl_Value_Slider * slider = (Fl_Value_Slider *)widget;
  extrude_scale = slider->value();
  Fl :: focus(optwin);
}

void changeExtrudeRotation(Fl_Widget * widget, void * data)
{
     // Call back for the exrot value slider
  Fl_Value_Slider * slider = (Fl_Value_Slider *)widget;
  extrude_rot = slider->value();
  Fl :: focus(optwin);
}

void changeNumExtrusions(Fl_Widget * widget, void * data)
{
     // Call back for the numex value slider
  Fl_Value_Slider * slider = (Fl_Value_Slider *)widget;
  num_extrusions = slider->value();
  Fl :: focus(optwin);
}

void changeNumExtraTwists(Fl_Widget * widget, void * data)
{
     // Call back for the maxsegs value slider
  Fl_Value_Slider * slider = (Fl_Value_Slider *)widget;
  num_extra_twists = slider->value();
  Fl :: focus(optwin);
}

void changeNumSubDivs(Fl_Widget * widget, void * data)
{
     // Call back for the maxsegs value slider
  Fl_Value_Slider * slider = (Fl_Value_Slider *)widget;
  num_e_subdivs = slider->value();
  Fl :: focus(optwin);
}

void changeCrustThickness(Fl_Widget * widget, void * data)
{
     // Call back for the crust thickness slider
  int user_data = *((int *)data);
  if ( user_data == CBW_ValueInput )
     {
       Fl_Value_Input * vinput = (Fl_Value_Input *)widget;
       crust_thickness = vinput->value();
       cthick_slider->value(crust_thickness);
     }
  else if ( user_data == CBW_Slider )
     {
       Fl_Value_Slider * slider = (Fl_Value_Slider *)widget;
       crust_thickness = slider->value();
          //cthick_vinput->value(crust_thickness);
     }
  Fl :: focus(optwin);
}

void changeCrustScaleFactor(Fl_Widget * widget, void * data)
{
     // Call back for the cthick_slider value slider
  Fl_Value_Slider * slider = (Fl_Value_Slider *)widget;
  crust_scale_factor = slider->value();
  Fl :: focus(optwin);
}

void changeTwistFactor(Fl_Widget * widget, void * data)
{
     // Call back for the twistfactor value slider
  Fl_Value_Slider * slider = (Fl_Value_Slider *)widget;
  twist_factor = slider->value();
  Fl :: focus(optwin);
}

void changeWeightFactor(Fl_Widget * widget, void * data)
{
     // Call back for the weightfactor value slider
  Fl_Value_Slider * slider = (Fl_Value_Slider *)widget;
  weight_factor = slider->value();
  Fl :: focus(optwin);
}

void changeCleanupFlag(Fl_Widget * widget, void * data)
{
     // Call back for the  cmcleanup check button
  if ( crust_cleanup ) crust_cleanup = false;
  else crust_cleanup = true;
  Fl :: focus(optwin);
}

int main (int argc, char **argv) 
{
  if ( argc > 1 )
     {
       setObject(argv[1]);
       argc--; argv++;
       if ( argc > 1 )
          {
            texture.readFrom(argv[1]);
            argc--; argv++;
          }
     }
  else
     setObject("test.obj");

  renderer = shadedRenderer;
  wireframe = 1;

  plight.position.set(10,0,0);
  plight.warmcolor.set(1,1,0.7);
  plight.coolcolor.set(0.2,0.2,0.4);
  plight.intensity = 1.0;

  mainwindow = new Fl_Window(WIDTH, HEIGHT);
  mainwindow->position(100,100);
  mainwindow->resizable(*mainwindow);

     /* Create options window */
  Fl_Group optgrp(OPTX,OPTY,OPTWIDTH,OPTHEIGHT);
  
  optwin = new Fl_Window(BORDER,BORDER,OPTWIDTH-BORDER,OPTHEIGHT-BORDER,"Options");

  Fl_Value_Slider numsegs(5,20,240,20,"Number of segments for handles");
  numsegs.type(FL_HORIZONTAL);
  numsegs.range(1,10);
  numsegs.step(1,1);
  numsegs.value(num_segments);
  numsegs.align(FL_ALIGN_TOP|FL_ALIGN_LEFT);
  numsegs.callback(changeNumSegments);

  Fl_Value_Slider nweight1(5,70,240,20,"Normal weight for first face");
  nweight1.type(FL_HORIZONTAL);
  nweight1.range(-10.0,10.0);
  nweight1.step(1,10);
  nweight1.value(nwt1);
  nweight1.align(FL_ALIGN_TOP|FL_ALIGN_LEFT);
  nweight1.callback(changeNWeight1);

  Fl_Value_Slider nweight2(5,120,240,20,"Normal weight for second face");
  nweight2.type(FL_HORIZONTAL);
  nweight2.range(-10.0,10.0);
  nweight2.step(1,10);
  nweight2.value(nwt2);
  nweight2.align(FL_ALIGN_TOP|FL_ALIGN_LEFT);
  nweight2.callback(changeNWeight2);

  Fl_Value_Slider exlen(5,170,240,20,"Length of extrusions");
  exlen.type(FL_HORIZONTAL);
  exlen.range(-1.0,2.0);
  exlen.step(1,100);
  exlen.value(extrude_dist);
  exlen.align(FL_ALIGN_TOP|FL_ALIGN_LEFT);
  exlen.callback(changeExtrudeDistance);
  
  Fl_Value_Slider exrot(5,220,240,20,"Rotation (twist) of extrusions");
  exrot.type(FL_HORIZONTAL);
  exrot.range(-100.0,100.0);
  exrot.step(1,2);
  exrot.value(extrude_rot);
  exrot.align(FL_ALIGN_TOP|FL_ALIGN_LEFT);
  exrot.callback(changeExtrudeRotation);
  
  Fl_Value_Slider exscale(5,270,240,20,"Scale factor for extrusions");
  exscale.type(FL_HORIZONTAL);
  exscale.range(0.1,2.0);
  exscale.step(1,100);
  exscale.value(extrude_scale);
  exscale.align(FL_ALIGN_TOP|FL_ALIGN_LEFT);
  exscale.callback(changeExtrudeScale);
  
  Fl_Value_Slider numex(5,320,240,20,"No. of extrusions");
  numex.type(FL_HORIZONTAL);
  numex.range(1,10);
  numex.step(1,1);
  numex.value(num_extrusions);
  numex.align(FL_ALIGN_TOP|FL_ALIGN_LEFT);
  numex.callback(changeNumExtrusions);
  
  Fl_Value_Slider maxsegs(5,370,240,20,"Max. number of segments to connect");
  maxsegs.type(FL_HORIZONTAL);
  maxsegs.range(-1,10);
  maxsegs.step(1,1);
  maxsegs.value(num_segments);
  maxsegs.align(FL_ALIGN_TOP|FL_ALIGN_LEFT);
  maxsegs.callback(changeMaxSegments);

  Fl_Value_Slider numtwists(5,420,240,20,"Number of extra twists in handles");
  numtwists.type(FL_HORIZONTAL);
  numtwists.range(-1,10);
  numtwists.step(1,1);
  numtwists.value(num_extra_twists);
  numtwists.align(FL_ALIGN_TOP|FL_ALIGN_LEFT);
  numtwists.callback(changeNumExtraTwists);

  Fl_Value_Slider numdivs(5,470,240,20,"Number of edge sub-divisions");
  numdivs.type(FL_HORIZONTAL);
  numdivs.range(2,5);
  numdivs.step(1,1);
  numdivs.value(num_e_subdivs);
  numdivs.align(FL_ALIGN_TOP|FL_ALIGN_LEFT);
  numdivs.callback(changeNumSubDivs);

     /*
  cthick_vinput = new Fl_Value_Input(5,470,50,20,"");
  cthick_vinput->type(FL_HORIZONTAL);
  cthick_vinput->range(-1.0,1.0);
  cthick_vinput->value(crust_thickness);
  cthick_vinput->align(FL_ALIGN_TOP|FL_ALIGN_LEFT);
  cthick_vinput->callback(changeCrustThickness,&CBW_ValueInput);
     */
  
  cthick_slider = new Fl_Value_Slider(5,520,240,20,"Crust thickness");
  cthick_slider->type(FL_HORIZONTAL);
  cthick_slider->range(-1.0,1.0);
  cthick_slider->step(1,100);
  cthick_slider->value(crust_thickness);
  cthick_slider->align(FL_ALIGN_TOP|FL_ALIGN_LEFT);
  cthick_slider->callback(changeCrustThickness,&CBW_Slider);

  cscale_slider = new Fl_Value_Slider(5,570,240,20,"Crust Scale Factor");
  cscale_slider->type(FL_HORIZONTAL);
  cscale_slider->range(-1.0,1.0);
  cscale_slider->step(1,100);
  cscale_slider->value(crust_scale_factor);
  cscale_slider->align(FL_ALIGN_TOP|FL_ALIGN_LEFT);
  cscale_slider->callback(changeCrustScaleFactor);

  Fl_Value_Slider twistfactor(5,620,240,20,"Twist factor (Root4 SubD, Doo-Sabin Extr.)");
  twistfactor.type(FL_HORIZONTAL);
  twistfactor.range(0.0,1.0);
  twistfactor.step(1,100);
  twistfactor.value(twist_factor);
  twistfactor.align(FL_ALIGN_TOP|FL_ALIGN_LEFT);
  twistfactor.callback(changeTwistFactor);

  Fl_Value_Slider weightfactor(5,670,240,20,"Weight factor (Root4 SubD)");
  weightfactor.type(FL_HORIZONTAL);
  weightfactor.range(0.0,1.0);
  weightfactor.step(1,100);
  weightfactor.value(weight_factor);
  weightfactor.align(FL_ALIGN_TOP|FL_ALIGN_LEFT);
  weightfactor.callback(changeWeightFactor);

  Fl_Check_Button cmcleanup(5,710,30,30,"Cleanup (Crust Modeling)");
  cmcleanup.type(FL_HORIZONTAL);
  cmcleanup.value(crust_cleanup);
  cmcleanup.align(FL_ALIGN_RIGHT);
  cmcleanup.callback(changeCleanupFlag);

  optwin->end();
  optgrp.end();


  Fl_Tile tile(TITLEX,TITLEY,TITLEWIDTH,TITLEHEIGHT+MAINHEIGHT);

  Fl_Group titlegrp(TITLEX,TITLEY,TITLEWIDTH,TITLEHEIGHT);
  titlewin = new Fl_Window(TITLEX+BORDER, TITLEY+BORDER, TITLEWIDTH-BORDER, TITLEHEIGHT-BORDER);
  title = new Fl_Output(0,0,TITLEWIDTH-BORDER,TITLEHEIGHT-BORDER);
  title->box(FL_FLAT_BOX);
  title->textsize(20);
  title->value("DLFL Demo");
  titlewin->resizable();
  titlewin->end();
  titlegrp.end();

     // Perspective view
  Fl_Group pgrp(MAINX,MAINY,MAINWIDTH,MAINHEIGHT);
  pwin = new Window(MAINX+BORDER, MAINY+BORDER, MAINWIDTH-BORDER, MAINHEIGHT-BORDER);
  pwin->mode(FL_RGB|FL_DOUBLE|FL_DEPTH);
  pwin->switchTo(VPPersp);
  pwin->end();
  pgrp.end();

     // Top view
  Fl_Group tgrp(MAINX,MAINY,MAINWIDTH,MAINHEIGHT);
  twin = new Window(MAINX+BORDER, MAINY+BORDER, MAINWIDTH-BORDER, MAINHEIGHT-BORDER);
  twin->mode(FL_RGB|FL_DOUBLE|FL_DEPTH);
  twin->switchTo(VPTop); twin->viewport.camera.setOrthographicViewVolume(10,10);
  twin->end();
  tgrp.end();
  tgrp.hide();
  
     // Front view
  Fl_Group fgrp(MAINX,MAINY,MAINWIDTH,MAINHEIGHT);
  fwin = new Window(MAINX+BORDER, MAINY+BORDER, MAINWIDTH-BORDER, MAINHEIGHT-BORDER);
  fwin->mode(FL_RGB|FL_DOUBLE|FL_DEPTH);
  fwin->switchTo(VPFront); fwin->viewport.camera.setOrthographicViewVolume(10,10);
  fwin->end();
  fgrp.end();
  fgrp.hide();

     // Right side view
  Fl_Group rgrp(MAINX,MAINY,MAINWIDTH,MAINHEIGHT);
  rwin = new Window(MAINX+BORDER, MAINY+BORDER, MAINWIDTH-BORDER, MAINHEIGHT-BORDER);
  rwin->mode(FL_RGB|FL_DOUBLE|FL_DEPTH);
  rwin->switchTo(VPRight); rwin->viewport.camera.setOrthographicViewVolume(10,10);
  rwin->end();
  rgrp.end();
  rgrp.hide();

  tile.end();

  mainwindow->resizable(tile);
  mainwindow->end();
  mainwindow->show(argc,argv);

  pwin->show(); titlewin->show(); optwin->show();
  return Fl::run();
}
