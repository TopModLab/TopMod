/******************************************
 * include.hh
 * 
 ******************************************/

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "stylesheeteditor.hh"
#include "qshortcutmanager.hh"
#include "qshortcutdialog.hh"
#include "DLFLScriptEditor.hh"
#include "VerseTopMod.hh"

#include <iostream>
#include <QMainWindow>
#include <QWidget>
#include <QComboBox>
#include <QKeyEvent>
#include <QMenuBar>
#include <QMenu>
#include <QStyleOptionMenuItem>
#include <QAssistantClient>
#include <QPen>
#include "GLWidget.hh"

//the six modes are now separated into separate classes
#include "BasicsMode.hh"
#include "ExtrusionMode.hh"
#include "RemeshingMode.hh"
#include "HighgenusMode.hh"
#include "ConicalMode.hh"
#include "TexturingMode.hh"

//new command auto completion interface - like quicksilver inside topmod
#include "CommandCompleter.hh"

#include "DLFLLighting.hh"
#include <DLFLObject.hh>
#include <DLFLConvexHull.hh>
#include <Light.hh>
#include <SpotLight.hh>
#include <PointLight.hh>
#include <AmbientLight.hh>

#include "include/WireframeRenderer.hh"
#include "include/NormalRenderer.hh"
#include "include/LitRenderer.hh"
#include "include/TexturedRenderer.hh"
#include "include/TexturedLitRenderer.hh"
#include "include/PatchRenderer.hh"
#include "TopModPreferences.hh"

// DLFLAux Library includes
#include <DLFLCast.hh>
#include <DLFLConnect.hh>
#include <DLFLConvexHull.hh>
#include <DLFLCrust.hh>
#include <DLFLDual.hh>
#include <DLFLExtrude.hh>
#include <DLFLMeshSmooth.hh>
#include <DLFLMultiConnect.hh>
#include <DLFLSculpting.hh>
#include <DLFLSubdiv.hh>


typedef StringStream * StringStreamPtr;
typedef list<StringStreamPtr> StringStreamPtrList;

class TopModPreferences;

class BasicsMode;
class ExtrusionMode;
class RemeshingMode;
class ConicalMode;
class HighgenusMode;
class TexturingMode;
class VerseTopMod;

class QAction;
class QMenu;
class GLWidget;
class QBoxLayout;
class QComboBox;
class QMenuBar;

using namespace DLFL;

class MainWindow : public QMainWindow {
	Q_OBJECT

	public:
				// Enumerations for the various operating modes
		enum Mode
		{
			NormalMode=0,
				SelectVertex=1, SelectEdge=2, SelectFace=3, SelectFaceVertex=4, // Simple selection
				MultiSelectVertex=5, MultiSelectEdge=6, MultiSelectFace=7, MultiSelectFaceVertex=8, SelectCheckerboard=9,// Multiple selection
				InsertEdge=11, DeleteEdge=12, SubdivideEdge=13, CollapseEdge=14, SpliceCorners=15, // Edge operations
				ConnectEdges=21, // Connect two half-edges
				ExtrudeFace=31, ExtrudeFaceDS=32, ExtrudeDualFace=33, ExtrudeFaceDodeca=34, ExtrudeFaceIcosa=35, ExtrudeMultipleFaces=36, // Extrusions
				StellateFace=41, DoubleStellateFace = 42,
				ConnectFaceVertices=51, ConnectFaces=52, BezierConnectFaces=53, HermiteConnectFaces=54, // Handles
				ReorderFace=61, SubdivideFace=62, // Face operations
				CrustModeling=71, // Crust modeling
				CutEdge=200, CutVertex=201, CutEdgeandVertex = 202, CutFace = 203, //Conical sculpting Ozgur
				TruncateEdge=204, MarkEdge=206, MarkVertex=207, ConvexHullMode=301,
				EditVertex=81, //brianb
				SelectEdgeLoop=82, SelectFaceLoop=83, SelectSimilarFaces=84 //dave
			};

				// Enumerations for Selection Masking (e.g. vertices, faces, edges, face-vertices, and eventually objects... ?)
			enum SelectionMask { MaskObject=0,MaskVertices,MaskEdges,MaskFaces,MaskFaceVertices };

				// Enumerations for various multi-face-handle algorithms
			enum MFHAlgo { ConvexHull=0, ClosestEdge=1 };

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
			//document modified
				bool isModified();
				void setModified(bool isModified);


				protected :

					//-- Selection count, used as index into selection lists --//
				static int num_sel_verts;             // No. of selected vertices
				static int num_sel_edges;                // No. of selected edges
				static int num_sel_faces;                // No. of selected faces
				static int num_sel_faceverts;    // No. of selected face vertices

					//-- should we deselect or select the current faces? --//
				static bool deselect_verts;
				static bool deselect_edges;
				static bool deselect_faces;
				static bool deselect_faceverts;

					//face loop stuff // dave
				static DLFLEdgePtr face_loop_start_edge;
				// static DLFLFacePtr face_loop_marker;
				static bool face_loop_start;

					//-- Viewports --//
				GLWidget *active;       // Active viewport to handle events

				DLFLObject object;                            // The DLFL object
				TMPatchObject *patchObject;										// the patch object
				Mode mode;																		// Current operating mode
				SelectionMask selectionmask;            			// Current selection mask
				RemeshingScheme remeshingscheme;							// Current selected remeshing scheme
				PointLight plight;														// Light used to compute lighting

					//-- Lists to handle undo and redo --//
				StringStreamPtrList undoList;                    // List for Undo
				StringStreamPtrList redoList;                    // List for Redo
				int undolimit;                                  // Limit for undo
				bool useUndo;            // Flag to indicate if undo will be used

					// Initialize the viewports, etc.
				void initialize(int x, int y, int w, int h, DLFLRendererPtr rp);

					//-- Parameters used in various operations on the DLFL object --// // brianb
				static int  drag_startx;
				static int  drag_starty;
				static bool is_editing;

				public :
			//constructor
				MainWindow(char *filename = NULL);
			// Destructor
				~MainWindow() {
					clearUndoList();
					clearRedoList();
					delete active;
				}

				static void MainWindow::clearSelected()	{
					GLWidget::clearSelected();
					MainWindow::clearNumSelected();
				}

				static void startDrag(int x, int y) { // brianb
					drag_startx = x;
				drag_starty = y;
			}

			static void MainWindow::clearNumSelected() {
				num_sel_verts = 0;
				num_sel_edges = 0;
				num_sel_faces = 0;
				num_sel_faceverts = 0;
			}

			void getCheckerboardSelection(DLFLFacePtr fptr);
			void getEdgeLoopSelection(DLFLEdgePtr eptr);
			void getFaceLoopSelection(DLFLEdgePtr eptr, bool start, DLFLFacePtr face_loop_marker, bool select_face_loop);

						// Create the renderers
			void createRenderers();
			void destroyRenderers();

							// Switch to specified operating mode
			void setMode(Mode m);
			void setRemeshingScheme(RemeshingScheme scheme);
			void setSelectionMask(SelectionMask m);
			void setToolOptions(QWidget *optionsWidget);
			void loadFile(QString fileName);

		protected:
			void closeEvent( QCloseEvent *event );

		//Box Layouts for the entire window
			QBoxLayout *layout, *subTopLayout, *subLowLayout, *leftLayout, *rightLayout;
			QWidget * cWidget;  QHBoxLayout *modesLayout;

	#ifdef WITH_PYTHON
			DLFLScriptEditor *mScriptEditor;
			QDockWidget *mScriptEditorDockWidget;
	#endif

	#ifdef WITH_VERSE
			VerseTopMod *mVerseDialog;
			QDockWidget *mVerseDialogDockWidget;
	#endif	

			QDockWidget *mToolOptionsDockWidget;
			QStackedWidget *mToolOptionsStackedWidget;

		// Renderers
			static WireframeRendererPtr wired;            // WireframeRenderer
			static NormalRendererPtr normal;              // NormalRenderer
			static LitRendererPtr lit;                    // LitRenderer
			static TexturedRendererPtr textured;          // TexturedRenderer
			static TexturedLitRendererPtr texturedlit; 	// TexturedLitRenderer
			static PatchRendererPtr patch;		// PatchRenderer

			BasicsMode *mBasicsMode;
			ExtrusionMode *mExtrusionMode;
			ConicalMode *mConicalMode;
			RemeshingMode *mRemeshingMode;
			HighgenusMode *mHighgenusMode;
			TexturingMode *mTexturingMode;

	//Stuff for the shortcut manager test
			QShortcutManager *sm;

		private:
		//document modified
			bool mIsModified;
			bool mIsPrimitive;
			bool mWasPrimitive;

			void createActions();
			void createMenus();
			void createToolBars();
			void createStatusBar();
			void readSettings();
			void writeSettings();
			bool maybeSave();
			void initializeHelp();

			// help file viewer
			QAssistantClient *mAssistantClient;
			
			//top level and sublevel menus
			QMenuBar *menuBar;
			QMenu *fileMenu;
			QMenu *editMenu;
			QMenu *modesMenu;
			QMenu *displayMenu;
			QMenu *mViewMenu;
			QMenu *mShowIDsMenu;
			QMenu *rendererMenu;
			QMenu *primitivesMenu;
			QMenu *objectMenu;
			QMenu *selectionMenu;
			QMenu *mSelectionMaskMenu;
			QMenu *settingsMenu;
			QMenu *languageMenu;
			QMenu *mHelpMenu;

	#ifdef WITH_VERSE
			QMenu *mVerseMenu;
	#endif

			QMenu *mRemeshingMenu;
			QMenu *mToolsMenu;

		//File Menu Actions
			QAction *mNewAct;
			QAction *mOpenAct;
			QAction *mSaveAct;
			QAction *mSavePatchesAct;
			QAction *mSaveAsAct;
			QAction *loadTextureAct;
			QAction *printInfoAct;
			QAction *printFaceListAct;
			QAction *printVertexListAct;
			QAction *printEdgeListAct;
			QAction *mPrintCVListAct;
			QAction *mExitAct;
			
			QAction *mFullscreenAct;
			QAction *mPerformRemeshingAct;
			QAction *mQuickCommandAct;

		//Edit Menu Actions
			QAction *mUndoAct;
			QAction *mRedoAct;
			QAction *mClearUndoListAct;

		//view switching actions
			QAction *mTopViewAct;
			QAction *mBottomViewAct;
			QAction *mLeftViewAct;
			QAction *mRightViewAct;
			QAction *mFrontViewAct;
			QAction *mBackViewAct;
			QAction *mPerspViewAct;

		//Display Menu Actions
			QAction *showVerticesAct;
			QAction *mShowFaceIDsAct;
			QAction *mShowEdgeIDsAct;
			QAction *mShowVertexIDsAct;
			QAction *mShowSelectedIDsAct;
			QAction *showSilhouetteAct;
			QAction *showWireframeAct;
			QAction *objectOrientationAct;
			QAction *showNormalsAct;
			QAction *showGridAct;
			QAction *showHUDAct;
			QAction *showCoordinateAxesAct;

		//Renderer Menu Actions
			QAction *wireframeRendererAct;
			QAction *normalRendererAct;
			QAction *shadedRendererAct;
			QAction *lightedRendererAct;
			QAction *texturedRendererAct;
			QAction *texturedLightedAct;
			QAction *patchRendererAct;	

		//Primitives Menu Actions
			QAction *pCubeAct;
			QAction *pDodecahedronAct;
			QAction *pIcosahedronAct;
			QAction *pTetrahedronAct;
			QAction *pSoccerBallAct;
			QAction *pOctahedronAct;
			QAction *pGeodesicAct;

		//Object Menu Actions
			QAction *subdivideAllEdgesAct;
			QAction *planarizeAllFacesAct;
			QAction *makeObjectSphericalAct;
			QAction *makeObjectSmoothAct;
			QAction *createCrustAct;
			QAction *computeLightingAct;
			QAction *computeNormalsAndLightingAct;
			QAction *assignTextureCoordinatesAct;

		//Selection Menu Actions
			QAction *selectVertexAct;
			QAction *editVertexAct;
			QAction *selectFaceAct;
			QAction *selectFaceLoopAct;
			QAction *selectMultipleFacesAct;
			QAction *selectSimilarFacesAct;
			QAction *selectCheckerboardFacesAct;
			QAction *selectAllAct;
			QAction *selectInverseFacesAct;
			QAction *selectEdgeAct;
			QAction *selectEdgeLoopAct;
			QAction *selectCornerAct;
			QAction *clearSelectedModeAct;
			QAction *exitSelectionModeAct;

		//selection mask actions
			QAction *mSelectVerticesMaskAct;
			QAction *mSelectFacesMaskAct;
			QAction *mSelectEdgesMaskAct;
			QAction *mSelectFaceVerticesMaskAct;

		//Language Menu actions;
			QAction *englishAct;
			QAction *spanishAct;
			QAction *germanAct;
			QAction *frenchAct;
			QAction *turkishAct;
			QAction *catalanAct;
			
		//help menu
			QAction *mAboutQtAct;
			QAction *mHelpAct;
			QAction *mAboutAct;

	#ifdef WITH_VERSE
		//verse menu actions
			QAction *mVerseConnectLocalhostAct;
			QAction *mVerseConnectAct;
			QAction *mVerseDisconnectAct;
			QAction *mVerseDisconnectAllAct;
			QAction *mVerseStartServerAct;
			QAction *mVerseKillServerAct;
	#endif

			QWidget *mToolOptionsTitleBarWidget;
			QBoxLayout *mToolOptionsTitleBarLayout;

		//QToolbars
			QToolBar *mEditToolBar;
			QToolBar *mSelectionMaskToolBar;
			QToolBar *mPrimitivesToolBar;
			QToolBar *mToolsToolBar;
			QToolBar *mExtrusionToolBar;
			QToolBar *mConicalToolBar;
			QToolBar *mHighgenusToolBar;
			QToolBar *mTexturingToolBar;
			QToolBar *mRemeshingToolBar;

		//QActionGroups
			QActionGroup *mToolsActionGroup;
			QActionGroup *mRendererActionGroup;
			QActionGroup *mRemeshingActionGroup;
			QActionGroup *mSelectionMaskActionGroup;

		//the status bar
			QStatusBar *mStatusBar;

	#ifdef WITH_PYTHON
			QAction *mShowScriptEditorAct;
	#endif

	#ifdef WITH_VERSE
			QAction *mShowVerseDialogAct;
	#endif

			StyleSheetEditor *mStyleSheetEditor;
			QAction *mEditStyleSheetAct;
			TopModPreferences *mPreferencesDialog;
			QAction *mPreferencesAct;
			QSettings *mSettings;

public slots:
			void about(); //TODO: topmod developer credits 
			void help(); //open the qtassistantclient help viewer
			void documentWasModified();
			
			void getCommand(); //this will open up the quicksilver like interface and accept a value from the user

			void showHideScriptEditor();
			void showHideVerseDialog();

			void load_texture();
		//renderers
			void useWireframeRenderer();
			void useNormalRenderer();
			void useLightedRenderer();
			void useTexturedRenderer();
			void useTexturedLitRenderer();
			void usePatchRenderer();

			void select_vertex();
			void edit_vertex();
			void select_face();
			void select_face_loop();
			void select_multiple_faces();
			void select_similar_faces();
			void select_checkerboard_faces();
			void select_edge();
			void select_edge_loop();
			void select_corner();
			void exit_selection_mode();
			void clear_selected();
			void selectionMaskVertices();
			void selectionMaskFaces();
			void selectionMaskEdges();
			void selectionMaskFaceVertices();
			void selectAll();
			void selectInverse();

	//Basics Widget
			void toggleDeleteEdgeCleanupFlag(int state);
			void changeNumSubDivs(int value);

			void changeTileTexNum(int value);
			void toggleUseQuadsFlag(int state);

	// Extrusion
			void changeExtrudeLength(double value);
			void changeExtrudeRotation(int value);
			void changeExtrudeScale(double value);
			void changeNumExtrusions(int value);
			void changeStellateLength(double value);
			void toggleDualMeshEdgesFlag(int state);
			void toggleHexagonalizeDodecaExtrudeFlag(int state);

			void changeNumSegments(int value);
			void changeMaxSegments(int value);
			void changeNumSegments2(int value);
			void changeMaxSegments2(int value);
			void changeWeight1(double value);
			void toggleSymmetricWeightsFlag(int state);
			void changeWeight2(double value);
			void changeExtraTwists(int value);

			void changeCrustScaleFactor(double value);
			void changeCrustThickness(double value);
			void toggleCrustCleanupFlag(int state);

			void changeWireframeThickness(double value);

			void changeColumnThickness(double value); // Esan
			void changeColumnSegments(int value); // Esan

			void changeMultiFaceHandleScaleFactor(double value);
			void changeMultiFaceHandleExtrudeDist(double value);
			void toggleMultiFaceHandleUseMaxOffsetFlag(int state);

			void changeMengerSpongeThickness(double value);
			void toggleMengerSpongeFractionalThicknessFlag(int state);
			void changeMengerSpongeCollapseThreshold(double value);

			void on_editStyleAction_triggered(); // stylesheet editor
			void openPreferences();

			void changeCutOffsetE(double value);   //ozgur
			void changeCutOffsetV(double value);   //ozgur
			void changePNormalBendS(double value); //ozgur
			void changePNormalBendT(double value); //ozgur
			void toggleGlobalCut(int state);    //ozgur
			void toggleSelectedCut(int state);  //ozgur
			void changeTiltPlane1(double value); //dave
			void changeTiltPlane2(double value); //dave

	//remeshing slot functions
			void toggleDooSabinEdgeFlag(int state);
			void changeRoot4Twist(double value);
			void changeRoot4Weight(double value);
			void changeVertexCuttingOffset(double value);
			void changePentagonalOffset(double value);
			void changePentagonalScaleFactor(double value);
			void changeStarOffset(double value); // Doug
			void changeFractalOffset(double value); // Doug
			void changeSubStellateAmount(double value); // Eric
			void changeSubStellateCurve(double value); // Eric
			void changeDooSabinBCnewScale(double value); // Bei & Cansin
			void changeDooSabinBCnewLength(double value); // Bei & Cansin
			void changeLoopStyleSubdivisionLength(double value); // Bei & Cansin
			void changeDual1264Scale(double value); // Bei & Cansin
			void changeDomeSubdivisionLength(double value); // Bei & Cansin
			void changeDomeSubdivisionScale(double value); // Bei & Cansin
			void toggleAccurateDualFlag(int state);

		//right click menu support coming soon...
			void mousePressEvent(QMouseEvent * event);
			void mouseReleaseEvent(QMouseEvent * event);
			void mouseMoveEvent(QMouseEvent * event);

			void dragEnterEvent(QDragEnterEvent *event);
			void dropEvent(QDropEvent *event);

		#ifdef WITH_VERSE
			void verseConnected();
			void verseDisconnected();
			void verseStarted();
			void verseKilled();
		#endif				

			void switchPerspView();
			void switchTopView();
			void switchBottomView();
			void switchRightView();
			void switchLeftView();
			void switchFrontView();
			void switchBackView();

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
			void toggleUndo();

			void clearUndoList();      // Erase all elements on Undo list
			void clearRedoList();      // Erase all elements on Redo list
			void undoPush();         // Put current object onto undo list
			void undo();                           // Undo last operation
			void redo();              // Redo previously undone operation

				// Change the renderer for all viewports
			void setRenderer(DLFLRendererPtr rp);
				// void setPatchRenderer(); // just testing out the signals/slots functionality of Qt

				// Do appropriate selection depending on current mode
				// Mouse position is passed. Selected entities will be added
				// to the selection lists in the GLWidget class, which are assumed
				// to have been cleared before calling this function
			void doSelection(int x, int y);
			void doDrag(int x, int y);

				// Override show() method to show subwindows also
				// void show();
				// Return pointer to the active GLWidget
			GLWidget *getActive();

				//set lighting colors... warm and cool
			void setWarmLightColor(QColor c);
			void setCoolLightColor(QColor c);
			void setLightIntensity(double i);

				// Override redraw() method to mark subwindows also for redraw
			void redraw();
				// Override resize() method to properly resize subwindows
			// void resize(int x, int y, int w, int h);

				//--- Methods to perform various operations ---//

					// Display overlays
			// void toggleWireframe();
			// void toggleSilhouette();
			// void toggleVertices();
			// void toggleFaceIDs( );
			// void toggleVertexIDs( );
			// void toggleEdgeIDs( );
			// void turnOffOverlays();
			// void toggleObjectOrientation();
			// void toggleNormals();

					// Geometric transformations
			void translatex(double x);
			void translatey(double y);
			void translatez(double z);
			void scalex(double x);
			void scaley(double y);
			void scalez(double z);
			void freezeTransforms();

				// Global operations (don't require selection)
		#ifdef WITH_PYTHON
			// run after an operation is done via python
			void recomputeAll() { 
				recomputePatches();
				recomputeNormals();
			};
		#endif
			void recomputeNormals();
			void recomputeLighting();
			void recomputePatches();
			void subdivideAllEdges();
			void subdivideSelectedFaces();
			void subdivideAllFaces();
			void createMultiFaceHandle();
			void multiConnectMidpoints();
			void multiConnectCrust();
			void modifiedMultiConnectCrust();
			void createSponge();
			void planarizeFaces();
	
			void spheralizeObject();
			void smoothMesh();
			void performRemeshing(); // Generic method for all remeshing schemes
			void subdivideCatmullClark();
			void subdivideDooSabin();
			void subdivideHoneycomb();
			void subdivideRoot4();
			void subdivideCornerCutting();
			void subdivideLinearVertexInsertion();
			void subdivideSimplest();
			void subdivideVertexCutting();
			void subdividePentagonal();
			void subdivideCubicPentagonal();
			void subdivideDualPentagonal();
			void subdividePentagonPreserving();
			void subdivideDualPentagonPreserving();
			void subdivideDualHexagonPreserving();
			void subdivideRoot3();
			void subdivideLoop();
			void subdivideDualLoop();
			void subdivide1264();
			void subdivideDual1264();
			void subdivideCheckerBoard();
			void subdivideDualCheckerBoard();
			void subdivideStar();
			void subdivideSqrt3();
			void subdivideFractal();
			void subStellate1();
			void subStellate2();
			void subdivideDome();
			void subdivideDooSabinBC();
			void subdivideDooSabinBCNew();
			void subdivideLoopStyle();
			void globalStellate();
			void splitValence2Vertices();
			void cleanupWingedVertices();
			void createDual();
			void crustModeling1();
			void crustModeling2();
			void crustModeling3();
			void crustModeling4();
			void createCrust(bool use_scaling=false);
			void createCrust2(bool use_scaling=false);
			void makeWireframe();
			void makeWireframe2();
			void makeWireframeWithColumns();
			// void makeUnitCube(double edgelength);
			// void makeUnitTetrahedron(double edgelength);
			// void makeMengerSponge(int level);
			// void makeSierpinskiTetrahedron(int level);
			void randomAssignTexCoords();
			void assignTileTexCoords();
			void edgeCleanup();
			void printSummary();
			void printVertexList();
			void printEdgeList();
			void printCVList();
			void printFaceList();
			
			void mfh_use_closest_edge_algo();
			void mfh_use_convex_hull_algo();
			//conical functions from ozgur
			void performCutting();
			void createConvexHull();
			void createDualConvexHull();
			void testConvexHull();

				// File handling
			void openFile(); 
			void openFile(QString fileName);
			bool saveFile(QString fileName);
			bool saveFile(bool with_normals=false, bool with_tex_coords=false);
			bool saveFileAs(bool with_normals=false, bool with_tex_coords=false);
			void openFileOBJ();
			void saveFileOBJ(bool with_normals=false, bool with_tex_coords=false);
			void openFileDLFL();
			void saveFileDLFL();
			bool saveFileBezierOBJ( );
			void writePatchOBJ(const char *filename);
			void setCurrentFile(QString fileName);

				//primitive slot functions finally work
			void loadCube();
			void loadOctahedron();
			void loadTetrahedron();
			void loadDodecahedron();
			void loadIcosahedron();
			void loadSoccerball();
			void loadGeodesic();
			signals:
			#ifdef WITH_PYTHON
			void loadedObject( DLFLObject *obj, QString fileName );
			#endif


		};

#endif
