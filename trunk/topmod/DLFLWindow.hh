/* $Id: DLFLWindow.hh,v 4.13 2004/04/18 18:19:41 vinod Exp $ */

#ifndef _DLFL_WINDOW_HH_

#define _DLFL_WINDOW_HH_

/*
  Window class which contains Viewports displaying a DLFLObject.
  By default has 4 viewports each with a different view.
  Handles keyboard input to switch between views.
  Handle mouse events for selection.
*/
#include <QTabWidget>
#include <QByteArray>
#include <QFileInfo>
#include <QFile>

#include "GLWidget.hh"
#include "DLFLConvexHull.hh"
#include "include/ShadedRenderer.hh"

typedef StringStream * StringStreamPtr;
typedef list<StringStreamPtr> StringStreamPtrList;

class DLFLWindow : public QWidget {
	Q_OBJECT        // must include this if you use Qt signals/slots
  
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
		  CrustModeling=71, // Crust modeling
			CutEdge=200, CutVertex=201, CutEdgeandVertex = 202, CutFace = 203, //Conical sculpting Ozgur
			TruncateEdge=204, MarkEdge=206, MarkVertex=207, ConvexHullMode=301
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
		
			// Conical Sculpting Variables from Ozgur
		static double cutOffsetE_factor;
    static double cutOffsetV_factor;
    static double pnormalBendS_factor;
    static double pnormalBendT_factor;
    static bool global_cut;
    static bool selected_cut;
    static int numberofPeels;
    static double extrude_bendT;
    static double extrude_bendB;

		QString curFile;
  protected :

		   //-- Selection count, used as index into selection lists --//
		static int num_sel_verts;             // No. of selected vertices
		static int num_sel_edges;                // No. of selected edges
		static int num_sel_faces;                // No. of selected faces
		static int num_sel_faceverts;    // No. of selected face vertices

		   //-- Viewports --//
		GLWidget *top;                              // Top viewport
		GLWidget *persp;                    // Perspective viewport
		GLWidget *front;                          // Front viewport
		GLWidget *right;                          // Right viewport
		GLWidget *active;       // Active viewport to handle events

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
		void initialize(int x, int y, int w, int h, DLFLRendererPtr rp);

private:
	QVBoxLayout *layout;

  public :

		   // Constructor
		DLFLWindow(int x, int y, int w, int h, QWidget *parent=0)
		  :  QWidget(parent), top(NULL), persp(NULL), front(NULL), right(NULL), active(NULL),
		    object(), mode(NormalMode), plight(), undoList(), redoList(), undolimit(20), useUndo(true)
		  {
		       // Default renderer is NULL
		    initialize(x,y,w,h,NULL);
		  }

		   // Constructor
		DLFLWindow(int x, int y, int w, int h, DLFLRendererPtr rp, QWidget *parent=0)
		  :  QWidget(parent), top(NULL), persp(NULL), front(NULL), right(NULL), active(NULL),
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
		
			static void DLFLWindow::clearSelected(void)
			{
				GLWidget::clearSelected();
				DLFLWindow::clearNumSelected();
			}

			static void DLFLWindow::clearNumSelected(void)
			{
				num_sel_verts = 0;
				num_sel_edges = 0;
				num_sel_faces = 0;
				num_sel_faceverts = 0;
			}

		// Handle keyboard and mouse events
		void mousePressEvent(QMouseEvent *event);
		void mouseReleaseEvent(QMouseEvent *event);
		void mouseMoveEvent(QMouseEvent *event);
		// Mode getMode(){ return mode; }
		// void setMode(Mode m){ mode = m; }
public slots:

		// Read the DLFL object from a file
		void readObject(const char * filename);
		void readObjectQFile(QString file);
		// Read the DLFL object from a file - use alternate OBJ reader for OBJ files
		void readObjectAlt(const char * filename);
		void readObjectOBJ(const char * filename);
		void readObjectDLFL(const char * filename);
		// Write the DLFL object to a file
		void writeObject(const char * filename, bool with_normals=false, bool with_tex_coords=false);
		void writeObjectOBJ(const char * filename, bool with_normals=false, bool with_tex_coords=false);
		void writeObjectDLFL(const char * filename);
		void setUndoLimit(int limit);
		void toggleUndo(void);

		void clearUndoList(void);      // Erase all elements on Undo list
		void clearRedoList(void);      // Erase all elements on Redo list
		void undoPush(void);         // Put current object onto undo list
		void undo(void);                           // Undo last operation
		void redo(void);              // Redo previously undone operation

		// Change the renderer for all viewports
		void setRenderer(DLFLRendererPtr rp);
		// void setPatchRenderer(); // just testing out the signals/slots functionality of Qt

		// Do appropriate selection depending on current mode
		// Mouse position is passed. Selected entities will be added
		// to the selection lists in the GLWidget class, which are assumed
		// to have been cleared before calling this function
		void doSelection(int x, int y);

		// Override show() method to show subwindows also
		void show(void);
		// Return pointer to the active GLWidget
		GLWidget *getActive(void);

		// Override redraw() method to mark subwindows also for redraw
		void redraw(void);
		// Override resize() method to properly resize subwindows
		void resize(int x, int y, int w, int h);


		//--- Methods to perform various operations ---//

			// Display overlays
		void toggleWireframe(void);
		void toggleSilhouette(void);
		void toggleVertices(void);
		void turnOffOverlays(void);
		void toggleObjectOrientation(void);
		void toggleNormals(void);
			// Switch to specified operating mode
		void setMode(Mode m);
		void setRemeshingScheme(RemeshingScheme scheme);

			// Geometric transformations
		void translatex(double x);
		void translatey(double y);
		void translatez(double z);
		void scalex(double x);
		void scaley(double y);
		void scalez(double z);
		void freezeTransforms(void);

		// Global operations (don't require selection)
		void recomputeNormals(void);
		void recomputeLighting(void);
		void recomputePatches(void);
		void subDivideAllEdges(void);
		void subDivideSelectedFaces(void);
		void subDivideAllFaces(void);
		void createMultiFaceHandle(void);
		void multiConnectMidpoints(void);
		void multiConnectCrust(void);
		void modifiedMultiConnectCrust(void);
		void createSponge(void);
		void planarizeFaces(void);

		void spheralizeObject(void);
		void smoothMesh(void);
		void performRemeshing(void); // Generic method for all remeshing schemes
		void subDivideCatmullClark(void);
		void subDivideDooSabin(void);
		void subDivideHoneycomb(void);
		void subDivideRoot4(void);
		void subDivideCornerCutting(void);
		void subDivideLinearVertexInsertion(void);
		void subDivideSimplest(void);
		void subDivideVertexCutting(void);
		void subDividePentagonal(void);
		void subDivideCubicPentagonal(void);
		void subDivideDualPentagonal(void);
		void subDividePentagonPreserving(void);
		void subDivideDualPentagonPreserving(void);
		void subDivideDualHexagonPreserving(void);
		void subDivideRoot3(void);
		void subDivideLoop(void);
		void subDivideDualLoop(void);
		void subDivide1264(void);
		void subDivideDual1264(void);
		void subDivideCheckerBoard(void);
		void subDivideDualCheckerBoard(void);
		void subDivideStar(void);
		void subDivideSqrt3(void);
		void subDivideFractal(void);
		void subStellate1(void);
		void subStellate2(void);
		void subDivideDome(void);
		void subDivideDooSabinBC(void);
		void subDivideDooSabinBCNew(void);
		void subDivideLoopStyle(void);
		void globalStellate(void);
		void splitValence2Vertices(void);
		void cleanupWingedVertices(void);
		void createDual(void);
		void createCrust(bool use_scaling=false);
		void makeWireframe(void);
		void makeWireframeWithColumns(void);
		void makeUnitCube(double edgelength);
		void makeUnitTetrahedron(double edgelength);
		void makeMengerSponge(int level);
		void makeSierpinskiTetrahedron(int level);
		void randomAssignTexCoords(void);
		void assignTileTexCoords(void);
		void edgeCleanup(void);
		void printSummary(void);
		void printVertexList(void);
		void printEdgeList(void);
		void printFaceList(void);

		// Axes and grid
		void toggleAxes(void);
		void toggleGrid(void);

		// File handling
		void openFile(void); 
		void openFile(QString fileName);
		void saveFile(bool with_normals=false, bool with_tex_coords=false);
		void openFileOBJ(void);
		void saveFileOBJ(bool with_normals=false, bool with_tex_coords=false);
		void openFileDLFL(void);
		void saveFileDLFL(void);
		void loadFile(const QString &fileName);
		void setCurrentFile(const QString &fileName);

		void cleanupForExit(void);
		void testConvexHull(void);

		//conical functions from ozgur
		void performCutting();
		void createConvexHull();
		void createDualConvexHull();
		
		//primitive slot functions finally work
		void loadCube();
		void loadOctahedron();
		void loadTetrahedron();
		void loadDodecahedron();
		void loadIcosahedron();
		void loadSoccerball();
		void loadGeodesic();		
 
};

#endif /* #ifndef _DLFL_WINDOW_HH_ */
