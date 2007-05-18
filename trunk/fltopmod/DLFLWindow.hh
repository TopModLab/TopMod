/* $Id: DLFLWindow.hh,v 4.13 2004/04/18 18:19:41 vinod Exp $ */

#ifndef _DLFL_WINDOW_HH_

#define _DLFL_WINDOW_HH_

/*
  Window class which contains Viewports displaying a DLFLObject.
  By default has 4 viewports each with a different view.
  Handles keyboard input to switch between views.
  Handle mouse events for selection.
*/

#include "DLFLViewport.hh"
#include "DLFLConvexHull.hh"
#include "ShadedRenderer.hh"
#include <FL/Fl_Window.H>
#include <FL/Fl_Double_Window.H>
#include <FL/fl_file_chooser.H>

typedef StringStream * StringStreamPtr;
typedef list<StringStreamPtr> StringStreamPtrList;

class DLFLWindow;
typedef DLFLWindow * DLFLWindowPtr;

class DLFLWindow : public Fl_Double_Window
{
  public :

        // Enumerations for the various operating modes
     enum Mode
     {
       NormalMode=0,
       SelectVertex=1, SelectEdge=2, SelectFace=3, SelectFaceVertex=4, // Simple selection
       MultiSelectVertex=5, MultiSelectEdge=6, MultiSelectFace=7, MultiSelectFaceVertex=8, // Multiple selection
       InsertEdge=11, DeleteEdge=12, SubDivideEdge=13, CollapseEdge=14, SpliceCorners=15, // Edge operations
       ConnectEdges=21, // Connect two half-edges
       ExtrudeFace=31, ExtrudeFaceDS=32, ExtrudeDualFace=33, ExtrudeFaceDodeca=34, ExtrudeFaceIcosa=35, // Extrusions
       StellateFace=41, DoubleStellateFace = 42,
       ConnectFaceVertices=51, ConnectFaces=52, BezierConnectFaces=53, HermiteConnectFaces=54, // Handles
       ReorderFace=61, SubDivideFace=62, // Face operations
       CrustModeling=71 // Crust modeling
     };

        // Enumerations for various multi-face-handle algorithms
     enum MFHAlgo
     {
       ConvexHull=0, ClosestEdge=1 
     };

     enum RemeshingScheme
     {
       Dual=0,
       Root3=10, DualVertexTrunc=11, GlobalStellate=12, Star=13, Generic1264=14,
       Honeycomb=20, VertexTrunc=21, DualGeneric1264=22,
       LinearVertexInsertion=30, CatmullClark=31, ModifiedStellate=32,
       DooSabin=40, CornerCutting=41, Simplest=42,
       Pentagonal=50, CubicPentagonal=51, DualPentagonal=52,
       LoopStyle=60, Loop=61, Root4=62, DualLoop=63,
       GlobalCubicExtrude=70, CheckerBoard=71,
       DualGlobalCubicExtrude=80, DualCheckerBoard=81,
       PentagonPreserving=90, DualPentagonPreserving=91,
       HexagonPreserving=100, DualHexagonPreserving=101,
       Fractal=110, ModifiedDoubleStellate=111, Dome=112, DooSabinBC=113, DooSabinBCNew=114
     };

  public :

        //-- Parameters used in various operations on the DLFL object --//

        // Edge deletion
     static bool delete_edge_cleanup; // Flag for point-sphere cleanup after edge deletion

        // Handles
     static int num_segments; // No. of segments in a connection/handle
     static int max_segments;       // Max. no. of segments to connect
     static bool symmetric_weights;    // Use symmetric weight factors
     static double nwt1;     // Weight factor for normal of first face
     static double nwt2;    // Weight factor for normal of second face
     static int num_extra_twists;    // No. of extra twists in handles

        // Extrusions
     static double extrude_dist;            // Distance for an extrude
     static double extrude_rot;             // Rotation for an extrude
     static double extrude_scale;            // Scaling for an extrude
     static int num_extrusions;      // No. of segments for an extrude
     static double ds_ex_twist;  // Twist factor for doo-sabin extrude
     static bool dual_mesh_edges_check; // Flag to check for removal of dual mesh edges
     static bool hexagonalize_dodeca_extrude; // Flag to hexagonalize when doing dodeca extrude

        // Edge subdivision
     static int num_e_subdivs;      // No. of subdivisions for an edge

        // Split valence 2 vertices
     static double vertex_split_offset; // Half of distance between new vertices

        // Crust modeling
     static double crust_thickness;              // Thickness of crust
     static double crust_scale_factor; // Scale factor for making crust
     static bool crust_cleanup;     // Cleanup flag for crust modeling

        // Wire-frame modeling
     static double wireframe_thickness; // Thickness of crust for wireframe

        // Column modeling
     static double column_thickness; // Thickness of columns for column modeling
     static int column_segments; // Number of segments in each column for column modeling

        // Subdivision
     static double twist_factor;     // Twist factor for root-4 subdiv
     static double weight_factor;   // Weight factor for root-4 subdiv
     static bool doo_sabin_check; // Flag to check for repeating edges
     static double vertex_cutting_offset; // Offset value for vertex cutting
     static double pentagonal_offset; // Offset value for pentagonal subdivision (conversion)
     static double pentagonal_scale; // Scale factor for pentagonal subdivision (preserving)
     static bool accurate_dual; // Flag to indicate that accurate dual method should be used
     static double checkerboard_thickness; // Fractional thickness for checkerboard remeshing

        // Added by Doug
     static double star_offset; // Offset value for star subdivision
     static double fractal_offset; // Offset value for fractal subdivision

        // Added by Eric
     static double substellate_height; //amount of first stellation extrusion
     static double substellate_curve; //amount of second stellation extrusion

        // Added by Bei & Cansin
     static double domeLength_factor;     // Length for dome extrusion
     static double domeScale_factor; // Scale factor for dome extrusion
     static double dual1264_scale_factor; // Scale factor for Dual 12-6-4 remeshing
     static double dooSabinBCnewScale_factor; // Scale factor for new Doo-Sabin remeshing scheme
     static double dooSabinBCnewLength_factor; // Length factor for new Doo-Sabin remeshing scheme
     static double loopLength_factor; // Length factor for Loop-style remeshing scheme

        // Face subdivision
     static bool use_quads; // Flag indicating if face subdivision should use quads or triangles

        // Tile texturing
     static int tile_tex_n; // Tiling size for tile texturing

        // Multi-face handles
     static MFHAlgo mfh_algo; // Algorithm to use for multi-face handle
     static double mfh_scale_factor; // Scale factor for convex hull creation
     static double mfh_extrude_dist; // Extrude distance for convex hull creation
     static bool mfh_use_max_offsets; // Flag indicating whether to compute and use the max. offset distances
     static double mfh_min_adj_factor; // Min. value of adjustment factor for iterative convex hull computation
     static bool mfh_make_connections; // Should final connections be made or should we stop with convex hull?

        // Menger Sponge creation
     static double sponge_thickness; // Thickness for Menger Sponge
     static double sponge_collapse_threshold; // Threshold angle for edge collapse
     static bool sponge_fractional_thickness; // Consider thickness to be fractional

  protected :

        //-- Selection count, used as index into selection lists --//
     static int num_sel_verts;             // No. of selected vertices
     static int num_sel_edges;                // No. of selected edges
     static int num_sel_faces;                // No. of selected faces
     static int num_sel_faceverts;    // No. of selected face vertices

     static void clearNumSelected(void)
       {
         num_sel_verts = 0;
         num_sel_edges = 0;
         num_sel_faces = 0;
         num_sel_faceverts = 0;
       }

     static void clearSelected(void)
       {
         DLFLViewport::clearSelected();
         DLFLWindow::clearNumSelected();
       }

  protected :

        //-- Viewports --//
     DLFLViewportPtr top;                              // Top viewport
     DLFLViewportPtr persp;                    // Perspective viewport
     DLFLViewportPtr front;                          // Front viewport
     DLFLViewportPtr right;                          // Right viewport
     DLFLViewportPtr active;       // Active viewport to handle events
     
     DLFLObject object;                             // The DLFL object
     Mode mode;                              // Current operating mode
     RemeshingScheme remeshingscheme; // Current selected remeshing scheme
     PointLight plight;              // Light used to compute lighting

        //-- Lists to handle undo and redo --//
     StringStreamPtrList undoList;                    // List for Undo
     StringStreamPtrList redoList;                    // List for Redo
     int undolimit;                                  // Limit for undo
     bool useUndo;            // Flag to indicate if undo will be used

        // Initialize the viewports, etc.
     void initialize(int x, int y, int w, int h, DLFLRendererPtr rp)
       {
            // Create the 4 viewports. They will automatically be added
            // as children to this window
            // begin() is called in the constructor of Fl_Window and Fl_Gl_Window
            // So make sure end() is called for each viewport so that subsequently
            // created viewports are not made children of that viewport.

         int sw=w, sh=h;

         top = new DLFLViewport(0,0,sw,sh,rp,&object);
         top->switchTo(VPTop);
         top->box(FL_EMBOSSED_BOX);
         top->end();
           
         persp = new DLFLViewport(0,0,sw,sh,rp,&object);
         persp->switchTo(VPPersp);
         persp->box(FL_EMBOSSED_BOX);
         persp->end();
         
         front = new DLFLViewport(0,0,sw,sh,rp,&object);
         front->switchTo(VPFront);
         front->box(FL_EMBOSSED_BOX);
         front->end();

         right = new DLFLViewport(0,0,sw,sh,rp,&object);
         right->switchTo(VPRight);
         right->box(FL_EMBOSSED_BOX);
         right->end();

            // End addition of children to this window
         end();

         top->hide();
         front->hide();
         right->hide();

         active = persp;

         resizable(active);
         
         plight.position.set(50,25,0);
         plight.warmcolor.set(1,1,0.6);
         plight.coolcolor.set(0.2,0.2,0.5);
         plight.intensity = 2.0;
       }

  public :

        // Constructor
     DLFLWindow(int x, int y, int w, int h)
       : Fl_Double_Window(x,y,w,h,NULL), top(NULL), persp(NULL), front(NULL), right(NULL), active(NULL),
         object(), mode(NormalMode), plight(), undoList(), redoList(), undolimit(20), useUndo(true)
       {
            // Default renderer is NULL
         initialize(x,y,w,h,NULL);
       }

        // Constructor
     DLFLWindow(int x, int y, int w, int h, DLFLRendererPtr rp)
       : Fl_Double_Window(x,y,w,h,NULL), top(NULL), persp(NULL), front(NULL), right(NULL), active(NULL),
         object(), mode(NormalMode), plight(), undoList(), redoList(), undolimit(20), useUndo(true)
       {
         initialize(x,y,w,h,rp);
       }

        // Destructor
     ~DLFLWindow()
       {
         clearUndoList();
         clearRedoList();
            // Delete the 4 viewports
         delete top;
         delete persp;
         delete front;
         delete right;
       }

        // Read the DLFL object from a file
     void readObject(const char * filename)
       {
         ifstream file;
         file.open(filename);
         if ( strstr(filename,".dlfl") || strstr(filename,".DLFL") )
            object.readDLFL(file);
         else if ( strstr(filename,".obj") || strstr(filename,".OBJ") )
            object.readObject(file);
         file.close();
       }

        // Read the DLFL object from a file - use alternate OBJ reader for OBJ files
     void readObjectAlt(const char * filename)
       {
         ifstream file;
         file.open(filename);
         if ( strstr(filename,".dlfl") || strstr(filename,".DLFL") )
            object.readDLFL(file);
         else if ( strstr(filename,".obj") || strstr(filename,".OBJ") )
            object.readObjectAlt(file);
         file.close();
       }

     void readObjectOBJ(const char * filename)
       {
         ifstream file;
         file.open(filename);
         object.readObject(file);
         file.close();
       }

     void readObjectDLFL(const char * filename)
       {
         ifstream file;
         file.open(filename);
         object.readDLFL(file);
         file.close();
       }

        // Write the DLFL object to a file
     void writeObject(const char * filename, bool with_normals=false, bool with_tex_coords=false)
       {
         ofstream file;
         file.open(filename);
         if ( strstr(filename,".dlfl") || strstr(filename,".DLFL") )
            object.writeDLFL(file);
         else if ( strstr(filename,".obj") || strstr(filename,".OBJ") )
            object.objWrite(file,with_normals,with_tex_coords);
         file.close();
       }

     void writeObjectOBJ(const char * filename, bool with_normals=false, bool with_tex_coords=false)
       {
         ofstream file;
         file.open(filename);
         object.objWrite(file,with_normals,with_tex_coords);
         file.close();
       }

     void writeObjectDLFL(const char * filename)
       {
         ofstream file;
         file.open(filename);
         object.writeDLFL(file);
         file.close();
       }

     void setUndoLimit(int limit)
       {
         undolimit = limit;
       }

     void toggleUndo(void)
       {
         if ( useUndo ) useUndo = false;
         else useUndo = true;
       }

     void clearUndoList(void);      // Erase all elements on Undo list
     void clearRedoList(void);      // Erase all elements on Redo list
     void undoPush(void);         // Put current object onto undo list
     void undo(void);                           // Undo last operation
     void redo(void);              // Redo previously undone operation

        // Change the renderer for all viewports
     void setRenderer(DLFLRendererPtr rp)
       {
         top->setRenderer(rp);
         persp->setRenderer(rp);
         front->setRenderer(rp);
         right->setRenderer(rp);
       }

        // Do appropriate selection depending on current mode
        // Mouse position is passed. Selected entities will be added
        // to the selection lists in the DLFLViewport class, which are assumed
        // to have been cleared before calling this function
     void doSelection(int x, int y);

        // Override show() method to show subwindows also
     void show(void)
       {
         Fl_Double_Window::show();
         active->show();
       }

        // Return pointer to the active DLFLViewport
     DLFLViewportPtr getActive(void)
       {
         return active;
       }

        // Override redraw() method to mark subwindows also for redraw
     void redraw(void)
       {
         Fl_Double_Window::redraw();
         active->redraw();
       }
     
        // Override resize() method to properly resize subwindows
     void resize(int x, int y, int w, int h)
       {
         Fl_Double_Window::resize(x,y,w,h);
       }

        // Handle keyboard and mouse events
     int handle(int event);

        //--- Methods to perform various operations ---//

        // Display overlays
     void toggleWireframe(void)         // Toggle display of wireframe
       {
         active->toggleWireframe();
       }

     void toggleSilhouette(void)       // Toggle display of silhouette
       {
         active->toggleSilhouette();
       }

     void toggleVertices(void)             // Toggle display of points
       {
         active->toggleVertices();
       }

     void turnOffOverlays(void)               // Turn off all overlays
       {
         active->setRenderFlags(0);
       }

     void toggleObjectOrientation(void)    // Reverse displayed object
       {
         active->toggleObjectOrientation();
       }

        // Switch to specified operating mode
     void setMode(Mode m)
       {
         mode = m;
         switch ( mode )
            {
              case SelectVertex :
                           DLFLWindow::num_sel_verts = 0;
                           break;
              case SelectEdge :
                           DLFLWindow::num_sel_edges = 0;
                           break;
              case SelectFace :
                           DLFLWindow::num_sel_faces = 0;
                           break;
              case SelectFaceVertex :
                           DLFLWindow::num_sel_faceverts = 0;
                           break;
              default :
                              // Nothing to be done for other modes except clearing selection lists
                           DLFLWindow::clearSelected();
                           break;
            }
       }

     void setRemeshingScheme(RemeshingScheme scheme)
       {
         remeshingscheme = scheme;
       }

        // Geometric transformations
     void translatex(double x)
       {
         object.position[0] = x;
       }

     void translatey(double y)
       {
         object.position[1] = y;
       }

     void translatez(double z)
       {
         object.position[2] = z;
       }

     void scalex(double x)
       {
         object.scale_factor[0] = x;
       }

     void scaley(double y)
       {
         object.scale_factor[1] = y;
       }

     void scalez(double z)
       {
         object.scale_factor[2] = z;
       }

     void freezeTransforms(void)
       {
         object.freezeTransformations();
         object.position.reset();
         object.scale_factor.set(1,1,1);
         object.rotation.reset();
       }

        // Global operations (don't require selection)
     void recomputeNormals(void)     // Recompute normals and lighting
       {
         object.computeNormals();
         object.computeLighting(&plight);
       }

     void recomputeLighting(void)                // Recompute lighting
       {
         object.computeLighting(&plight);
       }

     void recomputePatches(void) // Recompute the patches for patch rendering
     {
       object.updatePatches();
     }
       
     void subDivideAllEdges(void)              // Sub-divide all edges
       {
         undoPush();
         object.subDivideAllEdges(DLFLWindow::num_e_subdivs);
         DLFLWindow::clearSelected();
       }

     void subDivideSelectedFaces(void) // Subdivide all selected faces
       {
         undoPush();
         DLFLFacePtrArray fparray;
         fparray.resize(DLFLViewport::numSelectedFaces());
         for (int i=0; i < DLFLViewport::numSelectedFaces(); ++i)
            {
              fparray[i] = DLFLViewport::getSelectedFace(i);
            }
         object.subDivideFaces(fparray,use_quads);
         DLFLWindow::clearSelected();
       }

     void subDivideAllFaces(void) // Subdivide all the faces
       {
         undoPush();
         object.subDivideAllFaces(use_quads);
         DLFLWindow::clearSelected();
       }

     void createMultiFaceHandle(void) // Create multi-face handle between selected faes
       {
         int numsel = DLFLViewport::numSelectedFaces();
         if ( numsel < 3 ) return;
         undoPush();
         DLFLFacePtrArray sel_faces;
         DLFLFacePtr sfptr;
         for (int i=0; i < numsel; ++i)
            {
              sfptr = DLFLViewport::getSelectedFace(i);
              if ( sfptr ) sel_faces.push_back(sfptr);
            }
         switch ( DLFLWindow::mfh_algo )
            {
              case ConvexHull :
                           object.multiConnectFaces(sel_faces,DLFLWindow::mfh_scale_factor,
                                                    DLFLWindow::mfh_extrude_dist,
                                                    DLFLWindow::mfh_use_max_offsets);
                           break;
              case ClosestEdge :
                           object.multiConnectFaces(sel_faces);
                           break;
              default :
                           break;
            }
         recomputePatches();
         recomputeNormals();
         DLFLWindow::clearSelected();
       }

     void multiConnectMidpoints(void)
       {
            // Multi-connect midpoints after simplest-subdivision without edge deletion
         undoPush();
         object.multiConnectMidpoints();
         recomputePatches();
         recomputeNormals();
         DLFLWindow::clearSelected();
       }

     void multiConnectCrust(void)
       {
            // Multi-connect after creating crust
         undoPush();
         object.multiConnectCrust();
         recomputePatches();
         recomputeNormals();
         DLFLWindow::clearSelected();
       }

     void modifiedMultiConnectCrust(void)
       {
            // Modified multi-connect after creating crust
         undoPush();
         object.modifiedMultiConnectCrust();
         recomputePatches();
         recomputeNormals();
         DLFLWindow::clearSelected();
       }

     void createSponge(void)
       {
         undoPush();
         object.createSponge(DLFLWindow::sponge_thickness,
                             DLFLWindow::sponge_collapse_threshold);
         recomputePatches();
         recomputeNormals();
         DLFLWindow::clearSelected();
       }

     void planarizeFaces(void)                  // Planarize all faces
       {
         undoPush();
         object.planarize();
         recomputePatches();
         recomputeNormals();
       }

     void spheralizeObject(void)         // Spheralize object vertices
       {
         undoPush();
         object.spheralize();
         recomputePatches();
         recomputeNormals();
       }

     void smoothMesh(void)                          // Smooth the mesh
       {
         undoPush();
         object.meshsmooth();
         recomputePatches();
         recomputeNormals();
       }

     void performRemeshing(void); // Generic method for all remeshing schemes

     void subDivideCatmullClark(void)     // Catmull-Clark subdivision
       {
         undoPush();
         object.catmullClarkSubDivide();
         recomputePatches();
         recomputeNormals();
         DLFLWindow::clearSelected();
       }

     void subDivideDooSabin(void)             // Doo-Sabin subdivision
       {
         undoPush();
         object.dooSabinSubDivide(doo_sabin_check);
         recomputePatches();
         recomputeNormals();
         DLFLWindow::clearSelected();
       }

     void subDivideHoneycomb(void)            // Honeycomb subdivision
       {
         undoPush();
         object.honeycombSubDivide();
         recomputePatches();
         recomputeNormals();
         DLFLWindow::clearSelected();
       }

     void subDivideRoot4(void)                   // Root-4 subdivision
       {
         undoPush();
         object.root4SubDivide(DLFLWindow::weight_factor,DLFLWindow::twist_factor);
         recomputePatches();
         recomputeNormals();
         DLFLWindow::clearSelected();
       }

     void subDivideCornerCutting(void)   // Corner-cutting subdivision
       {
         undoPush();
         object.cornerCuttingSubDivide();
         recomputePatches();
         recomputeNormals();
         DLFLWindow::clearSelected();
       }

     void subDivideLinearVertexInsertion(void) // Bi-linear Vertex-insertion remeshing
       {
         undoPush();
         object.subDivideAllFaces(true);
         recomputePatches();
         recomputeNormals();
         DLFLWindow::clearSelected();
       }

     void subDivideSimplest(void)        // Corner-cutting subdivision
       {
         undoPush();
         object.simplestSubDivide();
         recomputePatches();
         recomputeNormals();
         DLFLWindow::clearSelected();
       }

     void subDivideVertexCutting(void)   // Vertex-cutting subdivision
       {
         undoPush();
         object.vertexCuttingSubDivide(DLFLWindow::vertex_cutting_offset);
         recomputePatches();
         recomputeNormals();
         DLFLWindow::clearSelected();
       }

     void subDividePentagonal(void)          // Pentagonal subdivision
       {
         undoPush();
         object.pentagonalSubDivide(DLFLWindow::pentagonal_offset);
         recomputePatches();
         recomputeNormals();
         DLFLWindow::clearSelected();
       }

     void subDivideCubicPentagonal(void) // Cubic Pentagonal remeshing scheme
       {
            // Implemented as Pentagonal + Dual + Dual
         undoPush();
         object.pentagonalSubDivide(DLFLWindow::pentagonal_offset);
         object.createDual(true); // Use accurate method
         object.createDual(true); // Use accurate method
         recomputePatches();
         recomputeNormals();
         DLFLWindow::clearSelected();
       }

     void subDivideDualPentagonal(void) // Dual-Pentagonal subdivision
       {
            // Implemented as Dual + Pentagonal + Dual
         undoPush();
         object.createDual(true); // Use accurate method
         object.pentagonalSubDivide(DLFLWindow::pentagonal_offset);
         object.createDual(true); // Use accurate method
         recomputePatches();
         recomputeNormals();
         DLFLWindow::clearSelected();
       }

     void subDividePentagonPreserving(void) // Pentagon preserving remeshing
       {
         undoPush();
         object.pentagonalSubDivide2(DLFLWindow::pentagonal_scale);
         recomputePatches();
         recomputeNormals();
         DLFLWindow::clearSelected();
       }

     void subDivideDualPentagonPreserving(void) // Dual Pentagon preserving remeshing
       {
            // Implemented as Dual + Pentagonal + Dual
         undoPush();
         object.createDual(true); // Use accurate method
         object.pentagonalSubDivide2(DLFLWindow::pentagonal_scale);
         object.createDual(true); // Use accurate method
         recomputePatches();
         recomputeNormals();
         DLFLWindow::clearSelected();
       }

     void subDivideDualHexagonPreserving(void) // Dual Hexagon Preserving remeshing
       {
            // Implemented as Dual + Root4 + Dual
         undoPush();
         object.createDual(true); // Use accurate method
         object.root4SubDivide(DLFLWindow::weight_factor,DLFLWindow::twist_factor);
         object.createDual(true); // Use accurate method
         recomputePatches();
         recomputeNormals();
         DLFLWindow::clearSelected();
       }

     void subDivideRoot3(void)                     // Root-3 remeshing
       {
            // Implemented as Dual + Honeycomb + Dual
         undoPush();
         object.createDual(true); // Use accurate method
         object.honeycombSubDivide();
         object.createDual(true); // Use accurate method
         recomputePatches();
         recomputeNormals();
         DLFLWindow::clearSelected();
       }

     void subDivideLoop(void)                      // Loop subdivision
       {
         undoPush();
         object.loopSubDivide();
         recomputePatches();
         recomputeNormals();
         DLFLWindow::clearSelected();
       }

     void subDivideDualLoop(void)          // Dual of Loop subdivision
       {
            // Implemented as Dual + Loop + Dual
         undoPush();
         object.createDual(true); // Use accurate method
         object.loopSubDivide();
         object.createDual(true); // Use accurate method
         recomputePatches();
         recomputeNormals();
         DLFLWindow::clearSelected();
       }

     void subDivide1264(void)                      // 12-6-4 remeshing
       {
            // Implemented as Dual + Dual 12-6-4 + Dual
         undoPush();
         object.createDual(true); // Use accurate method
         object.dual1264SubDivide(DLFLWindow::dual1264_scale_factor);
         object.createDual(true); // Use accurate method
         recomputePatches();
         recomputeNormals();
         DLFLWindow::clearSelected();
       }

     void subDivideDual1264(void) // Dual of 12-6-4 remeshing - Bei & Cansin
       {
         undoPush();
         object.dual1264SubDivide(DLFLWindow::dual1264_scale_factor);
         recomputePatches();
         recomputeNormals();
         DLFLWindow::clearSelected();
       }

     void subDivideCheckerBoard(void)       // Checker board remeshing
       {
         undoPush();
         object.checkerBoardRemeshing(DLFLWindow::checkerboard_thickness);
         recomputePatches();
         recomputeNormals();
         DLFLWindow::clearSelected();
       }

     void subDivideDualCheckerBoard(void) // Dual Checker board remeshing
       {
            // Implemented as Dual + Checker board + Dual
         undoPush();
         object.createDual(true); // Use accurate method
         object.checkerBoardRemeshing(DLFLWindow::checkerboard_thickness);
         object.createDual(true); // Use accurate method
         recomputePatches();
         recomputeNormals();
         DLFLWindow::clearSelected();
       }

     void subDivideStar(void)               // Star subdivision - Doug
       {
         undoPush();
         object.starSubDivide(DLFLWindow::star_offset);
         recomputePatches();
         recomputeNormals();
         DLFLWindow::clearSelected();
       }

     void subDivideSqrt3(void)           // sqrt(3) subdivision - Doug
       {
         undoPush();
         object.sqrt3SubDivide();
         recomputePatches();
         recomputeNormals();
         DLFLWindow::clearSelected();
       }

     void subDivideFractal(void)                     // fractal - Doug
       {
         undoPush();
         object.fractalSubDivide(DLFLWindow::fractal_offset);
         recomputePatches();
         recomputeNormals();
         DLFLWindow::clearSelected();
       }

     void subStellate1(void)            // stellate subdivision - Eric
       {
         undoPush();
         object.stellateSubDivide();
         recomputePatches();
         recomputeNormals();
         DLFLWindow::clearSelected();
       }

     void subStellate2(void)            // stellate subdivision - Eric
       {
         undoPush();
         object.twostellateSubDivide(DLFLWindow::substellate_height, DLFLWindow::substellate_curve);
         recomputePatches();
         recomputeNormals();
         DLFLWindow::clearSelected();
       }

     void subDivideDome(void)                          // Bei & Cansin
       {
         undoPush();
         object.domeSubDivide(DLFLWindow::domeLength_factor, DLFLWindow::domeScale_factor);
         recomputePatches();
         recomputeNormals();
         DLFLWindow::clearSelected();
       }

     void subDivideDooSabinBC(void) // Doo-Sabin(BC) subdivision - Bei & Cansin
       {
         undoPush();
         object.dooSabinSubDivideBC(DLFLWindow::doo_sabin_check);
         recomputePatches();
         recomputeNormals();
         DLFLWindow::clearSelected();
       }

     void subDivideDooSabinBCNew(void) // Doo-Sabin(BCNew) Bei & Cansin
       {
         undoPush();
         object.dooSabinSubDivideBCNew(DLFLWindow::dooSabinBCnewScale_factor,
                                       DLFLWindow::dooSabinBCnewLength_factor);
         recomputePatches();
         recomputeNormals();
         DLFLWindow::clearSelected();
       }

     void subDivideLoopStyle(void)     // Loop-style subdivision - Bei
       {
         undoPush();
         object.loopStyleSubDivide(DLFLWindow::loopLength_factor);
         recomputePatches();
         recomputeNormals();
         DLFLWindow::clearSelected();
       }

     void globalStellate(void)
       {
            // Does not use length parameter for now. Uses subDivideFace method with triangles
         undoPush();
         object.subDivideAllFaces(false);
         recomputePatches();
         recomputeNormals();
         DLFLWindow::clearSelected();
       }

     void splitValence2Vertices(void)      // Split Valence 2 vertices
       {
         undoPush();
         object.splitValence2Vertices(DLFLWindow::vertex_split_offset);
         recomputePatches();
         recomputeNormals();
         DLFLWindow::clearSelected();
       }

     void cleanupWingedVertices(void)     // Remove valence 2 vertices
       {
         undoPush();
         object.cleanupWingedVertices();
         recomputePatches();
         recomputeNormals();
         DLFLWindow::clearSelected();
       }

     void createDual(void)                       // Create dual object
       {
         undoPush();
         object.createDual(DLFLWindow::accurate_dual);
         recomputePatches();
         recomputeNormals();
         DLFLWindow::clearSelected();
       }

     void createCrust(bool use_scaling=false)        // Create a crust
       {
         undoPush();
         if ( use_scaling ) object.createCrustWithScaling(DLFLWindow::crust_scale_factor);
         else object.createCrust(DLFLWindow::crust_thickness);
         recomputePatches();
         recomputeNormals();
         DLFLWindow::clearSelected();
       }

     void makeWireframe(void)                    // Create a wireframe
       {
         undoPush();
         object.makeWireframe(DLFLWindow::wireframe_thickness);
         recomputePatches();
         recomputeNormals();
         DLFLWindow::clearSelected();
       }

     void makeWireframeWithColumns(void) // Create a wireframe using columns
       {
         undoPush();
         object.makeWireframeWithColumns(DLFLWindow::column_thickness, DLFLWindow::column_segments);
         recomputePatches();
         recomputeNormals();
         DLFLWindow::clearSelected();
       }

     void makeUnitCube(double edgelength)
       {
         undoPush();
         DLFLObjectPtr unitcube = DLFLObject::makeUnitCube(edgelength);
         object.reset();
         object.splice(*unitcube);
         delete unitcube;
         recomputePatches();
         recomputeNormals();
         DLFLWindow::clearSelected();
       }

     void makeUnitTetrahedron(double edgelength)
       {
         undoPush();
         DLFLObjectPtr unittetra = DLFLObject::makeUnitTetrahedron();
         object.reset();
         object.splice(*unittetra);
         delete unittetra;
         recomputePatches();
         recomputeNormals();
         DLFLWindow::clearSelected();
       }

     void makeMengerSponge(int level)
       {
         undoPush();
         DLFLObjectPtr mengersponge = DLFLObject::makeMengerSponge(level);
         object.reset();
         object.splice(*mengersponge);
         delete mengersponge;
         recomputePatches();
         recomputeNormals();
         DLFLWindow::clearSelected();
       }

     void makeSierpinskiTetrahedron(int level)
       {
         undoPush();
         DLFLObjectPtr stetra = DLFLObject::makeSierpinskiTetrahedron(level);
         object.reset();
         object.splice(*stetra);
         delete stetra;
         recomputePatches();
         recomputeNormals();
         DLFLWindow::clearSelected();
       }

     void randomAssignTexCoords(void) // Randomly assign texture coordinates
       {
         object.randomAssignTexCoords();
       }

     void assignTileTexCoords(void) // Assign texture coordinates for tile texturing
       {
         object.assignTileTexCoords(DLFLWindow::tile_tex_n);
       }

     void edgeCleanup(void) // Cleanup redundant edges
       {
         undoPush();
         object.edgeCleanup();
         recomputePatches();
         recomputeNormals();
         DLFLWindow::clearSelected();
       }
     
     void printSummary(void)              // Print summary information
       {
         object.print();
       }

     void printVertexList(void)                   // Print vertex list
       {
         object.printVertexList();
       }

     void printEdgeList(void)                       // Print edge list
       {
         object.printEdgeList();
       }
     
     void printFaceList(void)                       // Print face list
       {
         object.printFaces();
       }

        // Axes and grid
     void toggleAxes(void)        // Toggle display of coordinate axes
       {
         active->toggleAxes();
       }

     void toggleGrid(void)                   // Toggle display of grid
       {
         active->toggleGrid();
       }

        // File handling
     void openFile(void)
       {
         char * filename = fl_file_chooser("Read Object","*.{dlfl,obj}","");
         if ( filename )
            {
              undoPush();
              readObject(filename);
              recomputePatches();
              recomputeNormals();
            }         
       }
     
     void saveFile(bool with_normals=false, bool with_tex_coords=false)
       {
         char * filename = fl_file_chooser("Save Object","*.{dlfl,obj}","");
         if ( filename ) writeObject(filename,with_normals,with_tex_coords);
       }

     void openFileOBJ(void)
       {
         char * filename = fl_file_chooser("Read Object (OBJ)","*.obj","");
         if ( filename )
            {
              undoPush();
              readObjectOBJ(filename);
              recomputePatches();
              recomputeNormals();
            }         
       }
     
     void saveFileOBJ(bool with_normals=false, bool with_tex_coords=false)
       {
         char * filename = fl_file_chooser("Save Object (OBJ)","*.obj","");
         if ( filename ) writeObjectOBJ(filename,with_normals,with_tex_coords);
       }

     void openFileDLFL(void)
       {
         char * filename = fl_file_chooser("Read Object (DLFL)","*.dlfl","");
         if ( filename )
            {
              undoPush();
              readObjectDLFL(filename);
              recomputePatches();
              recomputeNormals();
            }
       }
     
     void saveFileDLFL(void)
       {
         char * filename = fl_file_chooser("Save Object (DLFL)","*.dlfl","");
         if ( filename ) writeObjectDLFL(filename);
       }

     void cleanupForExit(void) // Do memory cleanup if any before exit
       {
            // Nothing to be done
       }

     void testConvexHull(void);
};

#endif /* #ifndef _DLFL_WINDOW_HH_ */


/*
  $Log: DLFLWindow.hh,v $
  Revision 4.13  2004/04/18 18:19:41  vinod
  Added toggleObjectOrientation method

  Revision 4.12  2004/02/24 20:41:44  vinod
  Added option to turn off undo using a hotkey

  Revision 4.11  2004/01/20 22:17:19  vinod
  Added Loop and DualLoop subdivision schemes

  Revision 4.10  2004/01/20 17:43:28  vinod
  Changed method for Catmull Clark to catmullClarkSubDivide for consistency

  Revision 4.9  2004/01/20 09:28:08  vinod
  Added variables and methods for column modeling

  Revision 4.8  2004/01/20 07:24:38  vinod
  Added checkerBoardRemeshing scheme.
  Changed name of subhw to dual1264.
  Added wrapper functions for several remeshing schemes with proper handling of undo.

  Revision 4.7  2004/01/20 00:56:38  vinod
  Changed names of linear and loop remeshing schemes. Renumbered remeshinc scheme enumerations

  Revision 4.6  2004/01/19 20:20:50  vinod
  Implemented BiLinearVertexInsertion. Added remeshing schemes by Bei & Cansin

  Revision 4.5  2004/01/19 18:05:25  vinod
  Added enum for remeshing schemes.
  Added variable for keeping track of selected remeshing scheme.
  Added method to set remeshing scheme.
  Added generic method to perform remeshing based on selected scheme.

  Revision 4.4  2004/01/13 02:46:56  vinod
  Added hexagonalize flag for dodecahedral extrude

  Revision 4.3  2004/01/13 02:14:54  vinod
  Added double stellation, dodecahedral and icosahedral extrusion modes
  Added wrappers for stellate and double stellate remeshing schemes
  Added scale factor variable for pentagonal subdivision (preserving) scheme.

  Revision 4.2  2004/01/02 21:01:35  vinod
  Additions by Doug: parameters and methods for star, sqrt3 and fractal subdivision schemes

  Revision 4.1  2003/12/28 05:26:57  vinod
  Added boolean flag for cleanup after edge deletion.
  Removed the RemoveEdge mode.

  Revision 4.0  2003/12/26 01:58:53  vinod
  Major version sync.

  Revision 3.17  2003/12/08 20:19:46  vinod
  Added methods for making unit tetra and Sierpinski tetra

  Revision 3.16  2003/12/08 17:05:19  vinod
  Added methods to create standard objects unit cube and Menger Sponge

  Revision 3.15  2003/12/07 01:49:03  vinod
  Added flag for choosing dual creation method

  Revision 3.14  2003/12/07 00:38:25  vinod
  Changed point light position and intensity

  Revision 3.13  2003/12/06 08:04:36  vinod
  Changed light parameters. Normals are recomputed after a file is read

  Revision 3.12  2003/12/01 09:00:53  vinod
  Added translate, scale and freezeTransforms methods

  Revision 3.11  2003/11/29 20:36:35  vinod
  Added subDivideAllFaces hotkey and method

  Revision 3.10  2003/11/29 19:13:23  vinod
  Added cleanupWingedVertices method.
  Modified face subdivision operation to use new subDivideFaces method

  Revision 3.9  2003/11/29 04:20:52  vinod
  Added boolean flag for symmetric hermitian handle weights

  Revision 3.8  2003/11/27 00:22:59  vinod
  Changed threshold angle to threshold factor in Menger sponge creation

  Revision 3.7  2003/11/26 18:26:49  vinod
  Changed collapse threshold in Menger sponge creation to angle instead of length

  Revision 3.6  2003/11/23 22:45:37  vinod
  Added menger sponge edge collapse threshold parameter

  Revision 3.5  2003/11/21 15:23:43  vinod
  Added fractional thickness flag for menger sponge.
  Normals are now recomputed for most operations

  Revision 3.4  2003/11/16 06:48:40  vinod
  Added sponge_thickness class variable

  Revision 3.3  2003/10/25 22:53:55  vinod
  Fixed call to multiConnectFaces

  Revision 3.2  2003/10/21 06:45:04  vinod
  Added createSponge method

  Revision 3.1  2003/09/26 17:12:02  vinod
  Added multiConnectCrust and modifiedMultiConnectCrust

  Revision 3.0  2003/09/19 16:06:47  vinod
  Major version update

  Revision 2.12  2003/09/10 15:16:54  vinod
  Added multiConnectMidpoints

  Revision 2.11  2003/09/06 19:49:22  vinod
  Added enumeration and option for choosing between multi-face-handle algorithms

  Revision 2.10  2003/08/24 23:11:43  vinod
  Added 2nd pentagonal subdivision scheme with hotkey

  Revision 2.9  2003/08/24 20:27:45  vinod
  Added variables for new multiConnect method.

  Revision 2.8  2003/07/07 16:12:16  vinod
  Added mfh_use_max_offsets option for multi-connect

  Revision 2.7  2003/04/25 15:45:16  vinod
  Added scale factor and extrude distance variables
  for multiConnectFaces method

  Revision 2.6  2003/04/14 16:55:45  vinod
  Changed min. no. of faces for multi-connect to 3 from 2

  Revision 2.5  2003/04/14 16:50:08  vinod
  Added createMultiFaceHandle method

  Revision 2.4  2003/04/14 15:45:57  vinod
  Added testConvexHull method

  Revision 2.3  2003/04/04 06:28:41  vinod
  Added edgeCleanup option

  Revision 2.2  2003/03/10 07:35:17  vinod
  Added CollapseEdge mode

  Revision 2.1  2003/02/01 19:02:58  vinod
  Added boolean options for normal and tex coords in file save methods

  Revision 2.0  2003/01/25 08:49:31  vinod
  *** empty log message ***

  Revision 1.24  2003/01/25 08:08:00  vinod
  Added MultiSelect modes, subDivideSelectedFaces method

  Revision 1.23  2003/01/20 21:43:36  vinod
  Name change from skeleton modeling to wireframe modeling

  Revision 1.22  2002/12/30 17:29:24  vinod
  Added splitValence2Vertices method and variables

  Revision 1.21  2002/12/30 04:09:26  vinod
  Added variable and method for skeleton modeling

  Revision 1.20  2002/12/18 16:23:10  vinod
  Added face subdivision and tile texturing stuff

  Revision 1.19  2002/12/17 03:53:40  vinod
  Fixed comment logs

  Revision 1.18  2002/12/17 03:18:37  vinod
  See comments for previous revision

  Revision 1.17  2002/12/17 02:54:32  vinod
  Additions by Paul
  ExtrudeDualFace, StellateFace modes
  Vertex cutting and Pentagonal subdivision schemes

  Revision 1.16  2002/12/17 02:44:59  vinod
  DLFLWindow.hh

  Revision 1.15  2002/12/14 20:59:46  vinod
  Added readObjectAlt method

  Revision 1.14  2002/11/07 08:19:54  vinod
  Added edge check flag for dooSabin subdivision

  Revision 1.13  2002/10/31 06:07:25  vinod
  Added simplest subdivision scheme

  Revision 1.12  2002/10/30 04:37:25  vinod
  Added method to access the active viewport

  Revision 1.11  2002/10/30 03:43:44  vinod
  Added filter for dlfl and obj formats in file save and open

  Revision 1.10  2002/10/28 06:50:23  vinod
  Changed base class from Fl_Window to FL_Double_Window

  Revision 1.9  2002/10/25 15:34:45  vinod
  Removed unnecessary includes of fstream, stack

  Revision 1.8  2002/10/25 03:05:20  vinod
  Added macros to use sstream instead of strstream include file
  for GCC. Changed typedef to StringStream from Stream. Undo and redo lists use StringStreamPtr

  Revision 1.7  2002/10/22 21:26:14  vinod
  Added undo capability, createDual

  Revision 1.6  2002/10/17 02:21:59  vinod
  Bug fix: wrong variable: ipfile vs filename

  Revision 1.5  2002/10/16 14:16:43  vinod
  Added methods for various operations

  Revision 1.4  2002/10/09 14:57:09  vinod
  Added static members to keep track of number of selected items

  Revision 1.3  2002/10/08 16:22:17  vinod
  Overrides for show, redraw, resize methods.
  Added member variable for active viewport
  ShadedRenderer is now default renderer.
  Added initialize method to do initialization
  Viewports are all now same size. Only perspective is shown
  by default and is the default active viewport.

  Revision 1.2  2002/10/07 08:24:23  vinod
  Added read/write object from file, setRenderer

  Revision 1.1  2002/10/02 16:41:25  vinod
  Window class to display object. Handles keyboard events and selection

*/
