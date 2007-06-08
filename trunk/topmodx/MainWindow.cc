/****************************************************************************
 **
 **
 ****************************************************************************/

#include <QtGui>
#include <QtOpenGL>
#include "MainWindow.hh"

WireframeRendererPtr MainWindow::wired;              // Wireframe Renderer
NormalRendererPtr MainWindow::normal;                // NormalRenderer
LitRendererPtr MainWindow::lit;                        // LitRenderer
TexturedRendererPtr MainWindow::textured;          // TexturedRenderer
TexturedLitRendererPtr MainWindow::texturedlit; // TexturedLitRenderer
PatchRendererPtr MainWindow::patch;		       // PatchRenderer

//-- Parameters used in various operations on the DLFL object --//
//-- See header file for explanations --//

int MainWindow::drag_startx = 0;
int MainWindow::drag_starty = 0;
bool MainWindow::is_editing = false;

				// Edge deletion
bool MainWindow::delete_edge_cleanup = true;

				// Handles
int MainWindow::num_segments = 4;
int MainWindow::max_segments = -1;
bool MainWindow::symmetric_weights = true;
double MainWindow::nwt1 = 5.0;
double MainWindow::nwt2 = 5.0;
int MainWindow::num_extra_twists = 0;

				// Extrusions
double MainWindow::extrude_dist = 2.0;
double MainWindow::extrude_rot = 0.0;
double MainWindow::extrude_scale = 1.0;
int MainWindow::num_extrusions = 1;
double MainWindow::ds_ex_twist = 0.0;
bool MainWindow::dual_mesh_edges_check = false;
bool MainWindow::hexagonalize_dodeca_extrude = false;
double MainWindow::extrude_bendT = 0;
double MainWindow::extrude_bendB = 0;

				// Edge subdivision
int MainWindow::num_e_subdivs = 2;

				// Split valence 2 vertices
double MainWindow::vertex_split_offset=-0.1;

				// Crust modeling
double MainWindow::crust_thickness = 0.5;
double MainWindow::crust_scale_factor = 0.9;
bool MainWindow::crust_cleanup = true;

				// Wireframe modeling
double MainWindow::wireframe_thickness = 0.25;

				// Column modeling
double MainWindow::column_thickness = 0.25;
int MainWindow::column_segments = 4;

				// Subdivision
double MainWindow::twist_factor = 0.0;
double MainWindow::weight_factor = 0.0;
bool MainWindow::doo_sabin_check = true;
double MainWindow::vertex_cutting_offset = 0.25;
double MainWindow::pentagonal_offset = 0.0;
double MainWindow::pentagonal_scale = 0.75;
bool MainWindow::accurate_dual = true;
double MainWindow::checkerboard_thickness = 0.33;

			// Added by Doug
double MainWindow::star_offset = 0.0;
double MainWindow::fractal_offset = 1.0;

			// Added by Eric
double MainWindow::substellate_height = 0.0;
double MainWindow::substellate_curve = 0.0;

			// Added by bei&cansin dec 09
double MainWindow::domeLength_factor = 1.0 ;
double MainWindow::domeScale_factor = 1.0  ;
double MainWindow::dual1264_scale_factor = 0.7 ;

			// Added by bei dec 11
double MainWindow::dooSabinBCnewScale_factor = 1.0;
double MainWindow::dooSabinBCnewLength_factor = 1.0;

			// Added by bei 12-11-03
double MainWindow::loopLength_factor = 1.0;

			// Face subdivision
bool MainWindow::use_quads = true;

				// Tile texturing
int MainWindow::tile_tex_n = 2;

				// Multi-face handles
MainWindow::MFHAlgo MainWindow::mfh_algo = MainWindow::ConvexHull;
double MainWindow::mfh_scale_factor = 1.0;
double MainWindow::mfh_extrude_dist = 0.5;
bool MainWindow::mfh_use_max_offsets = false;
double MainWindow::mfh_min_adj_factor = 0.01;
bool MainWindow::mfh_make_connections = true;

				// Menger Sponge creation
double MainWindow::sponge_thickness = 0.67; // Thickness for Menger Sponge
double MainWindow::sponge_collapse_threshold = 0.0; // Threshold factor for edge collapse
bool MainWindow::sponge_fractional_thickness = false; // Thickness is fractional

				//conical sculpting initialization from ozgur
double MainWindow::cutOffsetE_factor = 0.25;
double MainWindow::cutOffsetV_factor = 0.25;
double MainWindow::pnormalBendS_factor = 0;
double MainWindow::pnormalBendT_factor = 0;
bool   MainWindow::global_cut = false;
int    MainWindow::numberofPeels = 90;
bool   MainWindow::selected_cut=true;

//-- Selection count, used as index into selection lists --//
int MainWindow::num_sel_verts = 0;
int MainWindow::num_sel_edges = 0;
int MainWindow::num_sel_faces = 0;
int MainWindow::num_sel_faceverts = 0;

bool MainWindow::deselect_verts = false;
bool MainWindow::deselect_edges = false;
bool MainWindow::deselect_faces = false;
bool MainWindow::deselect_faceverts = false;

// for face loop selection 
DLFLEdgePtr MainWindow::face_loop_start_edge = NULL;

MainWindow::MainWindow(char *filename) : object(), mode(NormalMode), undoList(), redoList(), 
	undolimit(20), useUndo(true), mIsModified(false), mIsPrimitive(false), mWasPrimitive(false) {
		//initialize the OpenGL Window GLWidget
		QGLFormat fmt;
		//initialize renderer
		createRenderers();

		//patch object initialization
		patchObject = new TMPatchObject( object.getID() );

		active = new GLWidget(500,600, VPPersp, lit, QColor(255,255,255,255),QColor(255,255,255,255) , &object, patchObject,fmt, this);
		active->switchTo(VPPersp);	
		active->redraw();
		active->setMinimumSize(400,400);
		active->setFocusPolicy(Qt::StrongFocus);
		
		
		//status bar
		mStatusBar = new QStatusBar(this);
		// setStatusBar(mStatusBar);
		setAttribute(Qt::WA_AcceptDrops, true);
		setWindowFlags(Qt::Window);
		setWindowTitle(tr("newfile[*] - TopMod"));
	  cWidget = new QWidget( );	
		//accept file drop events
		setAcceptDrops(true);
		/** Setup Main Layout and add the glwidget to it **/
	  layout = new QBoxLayout( QBoxLayout::TopToBottom, 0 );
	  layout->addWidget(active);
		layout->setMargin(0);
		// layout->addStretch(1);
	  cWidget->setLayout( layout );
	  setCentralWidget( active );

		//initialize light color
		plight.position.set(50,25,0);
		plight.warmcolor.set(1,1,0.6);
		plight.coolcolor.set(0.2,0.2,0.4);
		plight.intensity = 2.0;

		#ifdef WITH_PYTHON
			//the script editor widget will be placed into a QDockWidget
			//and will be dockable in the top and bottom sections of the main window	
			mScriptEditor = new DLFLScriptEditor( );
			mScriptEditorDockWidget = new QDockWidget(tr("Script Editor"), this);
			mScriptEditorDockWidget->setAllowedAreas(Qt::BottomDockWidgetArea | Qt::TopDockWidgetArea);
			mScriptEditorDockWidget->setWidget(mScriptEditor);
			addDockWidget(Qt::BottomDockWidgetArea, mScriptEditorDockWidget);
			mScriptEditorDockWidget->hide();
			mScriptEditorDockWidget->setMaximumHeight(200);
			connect( this, SIGNAL(loadedObject(DLFLObject*,QString)), 
				 mScriptEditor, SLOT(loadObject(DLFLObject*,QString)) );
			//connect( mScriptEditor, SIGNAL(cmdExecuted()), this, SLOT(recomputeAll()) );
			//connect( mScriptEditor, SIGNAL(cmdExecuted()), this->getActive(), SLOT(update()) );
		#endif


		#ifdef WITH_VERSE
			//verse script box
			mVerseDialog = VerseTopMod::Instance(this);
			mVerseDialogDockWidget = new QDockWidget(tr("Verse-TopMod"), this);
			mVerseDialogDockWidget->setAllowedAreas(Qt::BottomDockWidgetArea | Qt::TopDockWidgetArea);
			mVerseDialogDockWidget->setWidget(mVerseDialog);
			addDockWidget(Qt::BottomDockWidgetArea, mVerseDialogDockWidget);
			mVerseDialogDockWidget->hide();
			mVerseDialogDockWidget->setMaximumHeight(200);
			// VerseTopMod::Instance()->write("hey\n");
		#endif

	  //make a new instance of QShortcutManager
	  sm = new QShortcutManager();

		//instantiate toolbars
		mBasicsMode = new BasicsMode(this, sm);
		mExtrusionMode = new ExtrusionMode(this, sm);
		mConicalMode = new ConicalMode(this, sm);
		mRemeshingMode = new RemeshingMode(this, sm);
		mHighgenusMode = new HighgenusMode(this, sm);
		mTexturingMode = new TexturingMode(this, sm);

	  createActions();
	  createToolBars();
	  createMenus();
		//initialize the help file...
		initializeHelp();
		//style sheet editor
	  mStyleSheetEditor = new StyleSheetEditor;

		//preference dialog
		mSettings = new QSettings("TopMod", "Topological Mesh Modeler");
		readSettings();
		mPreferencesDialog = new TopModPreferences(mSettings, mStyleSheetEditor, sm, this);	
}

void MainWindow::createActions() {
		
	//File Menu Actions
	mOpenAct = new QAction(QIcon(":images/open.png"),tr("&Open..."), this);
	sm->registerAction(mOpenAct, "File Menu", "CTRL+O");
	mOpenAct->setStatusTip(tr("Open an existing file"));
	connect(mOpenAct, SIGNAL(triggered()), this, SLOT(openFile()));

	mSaveAct = new QAction(QIcon(":images/saveas.png"),tr("&Save"), this);
	sm->registerAction(mSaveAct, "File Menu", "CTRL+S");
	mSaveAct->setStatusTip(tr("Save the document to disk"));
	connect(mSaveAct, SIGNAL(triggered()), this, SLOT(saveFile()));

	mSaveAsAct = new QAction(QIcon(":images/saveas.png"),tr("Save &As..."), this);
	sm->registerAction(mSaveAsAct, "File Menu", "CTRL+SHIFT+S");
	mSaveAsAct->setStatusTip(tr("Save the document under a new name"));
	connect(mSaveAsAct, SIGNAL(triggered()), this, SLOT(saveFileAs()));

	mSavePatchesAct = new QAction(QIcon(":images/saveas.png"),tr("Save &Patch OBJ..."), this);
	sm->registerAction(mSavePatchesAct, "File Menu", "");
	mSavePatchesAct->setStatusTip(tr("Save a bezier patch .obj file"));
	connect(mSavePatchesAct, SIGNAL(triggered()), this, SLOT(saveFileBezierOBJ()));

	loadTextureAct = new QAction(tr("Load &Texture..."), this);
	sm->registerAction(loadTextureAct, "File Menu", "CTRL+L");
	loadTextureAct->setStatusTip(tr("Load Texture from file"));
	connect(loadTextureAct, SIGNAL(triggered()), this, SLOT(load_texture()));

	printInfoAct = new QAction(tr("Print &Information"), this);
	sm->registerAction(printInfoAct, "File Menu", "CTRL+P");
	printInfoAct->setStatusTip(tr("Print Information to the console"));
	connect(printInfoAct, SIGNAL(triggered()), this, SLOT(printSummary()));

	printFaceListAct = new QAction(tr("Print &Face List"), this);
	sm->registerAction(printFaceListAct, "File Menu", "CTRL+ALT+P");
	printFaceListAct->setStatusTip(tr("Print Face List to the console"));
	// connect(printFaceListAct, SIGNAL(triggered()), this, SLOT(saveAs()));

	printVertexListAct = new QAction(tr("Save &As..."), this);
	sm->registerAction(printVertexListAct, "File Menu", "CTRL+SHIFT+P");
	printVertexListAct->setStatusTip(tr("Save the document under a new name"));
	// connect(printVertexListAct, SIGNAL(triggered()), this, SLOT(saveAs()));

	printEdgeListAct = new QAction(tr("Print &Edge List"), this);
	sm->registerAction(printEdgeListAct, "File Menu", "CTRL+ALT+SHIFT+P");
	printEdgeListAct->setStatusTip(tr("Print Edge list to the console"));
	// connect(printEdgeListAct, SIGNAL(triggered()), this, SLOT(saveAs()));

	mPrintCVListAct = new QAction(tr("Print &CV List"), this);
	sm->registerAction(mPrintCVListAct, "File Menu", "");
	mPrintCVListAct->setStatusTip(tr("Print CV list to the console"));
	// connect(mPrintCVListAct, SIGNAL(triggered()), this, SLOT(saveAs()));

	mExitAct = new QAction(tr("E&xit"), this);
	sm->registerAction(mExitAct, "File Menu", "CTRL+Q");
	mExitAct->setStatusTip(tr("Exit the application"));
	connect(mExitAct, SIGNAL(triggered()), this, SLOT(close()));

	//Edit Menu Actions
	mUndoAct = new QAction(QIcon(":images/undo.png"), tr("&Undo"), this);
	sm->registerAction(mUndoAct, "Edit Menu", "CTRL+Z");
	mUndoAct->setStatusTip(tr("Undo the last operation"));
	connect(mUndoAct, SIGNAL(triggered()), this, SLOT(undo()));

	mRedoAct = new QAction(QIcon(":images/redo.png"), tr("&Redo"), this);
	sm->registerAction(mRedoAct, "Edit Menu", "CTRL+SHIFT+Z");
	mUndoAct->setStatusTip(tr("Redo the last operation"));
	connect(mRedoAct, SIGNAL(triggered()), this, SLOT(redo()));
	
	mClearUndoListAct = new QAction(tr("&Clear Undo List"), this);
	sm->registerAction(mClearUndoListAct, "Edit Menu", "CTRL+SHIFT+Z");
	mClearUndoListAct->setStatusTip(tr("Clear the Undo List to free up some memory"));
	connect(mClearUndoListAct, SIGNAL(triggered()), this, SLOT(clearUndoList()));
	
	//View Menu Actions
	mPerspViewAct = new QAction( tr("&Reset Camera"), this);
	sm->registerAction(mPerspViewAct, "View Menu", "F");
	mPerspViewAct->setStatusTip(tr("Reset Camera Position to default"));
	connect(mPerspViewAct, SIGNAL(triggered()), this, SLOT(switchPerspView()));

	mTopViewAct = new QAction( tr("&Top View"), this);
	sm->registerAction(mTopViewAct, "View Menu", "");
	mTopViewAct->setStatusTip(tr("Switch to Top View"));
	connect(mTopViewAct, SIGNAL(triggered()), this, SLOT(switchTopView()));

	mBottomViewAct = new QAction( tr("&Bottom View"), this);
	sm->registerAction(mBottomViewAct, "View Menu", "");
	mBottomViewAct->setStatusTip(tr("Switch to Bottom View"));
	connect(mBottomViewAct, SIGNAL(triggered()), this, SLOT(switchBottomView()));
	
	mFrontViewAct = new QAction( tr("&Front View"), this);
	sm->registerAction(mFrontViewAct, "View Menu", "");
	mFrontViewAct->setStatusTip(tr("Switch to Front View"));
	connect(mFrontViewAct, SIGNAL(triggered()), this, SLOT(switchFrontView()));

	mBackViewAct = new QAction( tr("B&ack View"), this);
	sm->registerAction(mBackViewAct, "View Menu", "");
	mBackViewAct->setStatusTip(tr("Switch to Back View"));
	connect(mBackViewAct, SIGNAL(triggered()), this, SLOT(switchBackView()));

	mLeftViewAct = new QAction( tr("&Left View"), this);
	sm->registerAction(mLeftViewAct, "View Menu", "");
	mLeftViewAct->setStatusTip(tr("Switch to Left View"));
	connect(mLeftViewAct, SIGNAL(triggered()), this, SLOT(switchLeftView()));

	mRightViewAct = new QAction( tr("&Right View"), this);
	sm->registerAction(mRightViewAct, "View Menu", "");
	mRightViewAct->setStatusTip(tr("Switch to Right View"));
	connect(mRightViewAct, SIGNAL(triggered()), this, SLOT(switchRightView()));

	//Display Menu Actions
	mFullscreenAct = new QAction(tr("&Full Screen"), this);
	mFullscreenAct->setCheckable(true);
	sm->registerAction( mFullscreenAct, "Display Menu", "");
	mFullscreenAct->setStatusTip(tr("Toggle Full Screen"));
	//  connect(mFullscreenAct, SIGNAL(triggered()), glWidget, SLOT(toggleFullScreen()) );

	showVerticesAct = new QAction(tr("Show &Vertices"), this);
	showVerticesAct->setCheckable(true);
	sm->registerAction(showVerticesAct, "Display Menu", "V");
	// showVerticesAct->setStatusTip(tr("Copy the current selection's contents to the "
	connect(showVerticesAct, SIGNAL(triggered()), this->getActive(), SLOT(toggleVertices()));

	mShowFaceIDsAct = new QAction(tr("Show &Face IDs"), this);
	mShowFaceIDsAct->setCheckable(true);
	sm->registerAction(mShowFaceIDsAct, "Display Menu", "CTRL+F");
	connect(mShowFaceIDsAct, SIGNAL(triggered()), this->getActive(), SLOT(toggleFaceIDs()));

	mShowEdgeIDsAct = new QAction(tr("Show &Edge IDs"), this);
	mShowEdgeIDsAct->setCheckable(true);
	sm->registerAction(mShowEdgeIDsAct, "Display Menu", "CTRL+E");
	connect(mShowEdgeIDsAct, SIGNAL(triggered()), this->getActive(), SLOT(toggleEdgeIDs()));

	mShowVertexIDsAct = new QAction(tr("Show &Vertex IDs"), this);
	mShowVertexIDsAct->setCheckable(true);
	sm->registerAction(mShowVertexIDsAct, "Display Menu", "CTRL+V");
	connect(mShowVertexIDsAct, SIGNAL(triggered()), this->getActive(), SLOT(toggleVertexIDs()));

	mShowSelectedIDsAct = new QAction(tr("Show &Selected IDs"), this);
	mShowSelectedIDsAct->setCheckable(true);
	sm->registerAction(mShowSelectedIDsAct, "Display Menu", "SHIFT+S");
	connect(mShowSelectedIDsAct, SIGNAL(triggered()), this->getActive(), SLOT(toggleSelectedIDs()));

	showSilhouetteAct = new QAction(tr("Show &Silhouette"), this);
	showSilhouetteAct->setCheckable(true);
	sm->registerAction(showSilhouetteAct, "Display Menu", "S" );
	// showSilhouetteAct->setStatusTip((tr"Blah"));
	connect(showSilhouetteAct, SIGNAL(triggered()), this->getActive(), SLOT(toggleSilhouette()));

	showWireframeAct = new QAction(tr("Show &Wireframe"), this);
	showWireframeAct->setCheckable(true);
	showWireframeAct->setChecked(true);
	sm->registerAction(showWireframeAct, "Display Menu", "W");
	// showWireframeAct->setStatusTip(tr("Copy the current selection's contents to the "
	connect(showWireframeAct, SIGNAL(triggered()), this->getActive(), SLOT(toggleWireframe()));

	showCoordinateAxesAct = new QAction(tr("Show &Coordinate Axes"), this);
	showCoordinateAxesAct->setCheckable(true);
	sm->registerAction(showCoordinateAxesAct, "Display Menu", "X");
	// showCoordinateAxesAct->setStatusTip(tr("Copy the current selection's contents to the "
	connect(showCoordinateAxesAct, SIGNAL(triggered()), this->getActive(), SLOT(toggleAxes()));

	objectOrientationAct = new QAction(tr("Reverse Object"), this);
	objectOrientationAct->setCheckable(true);
	sm->registerAction(objectOrientationAct, "Display Menu", "R");
	// objectOrienationAct->setStatusTip(tr("Copy the current selection's contents to the "
	connect(objectOrientationAct, SIGNAL(triggered()), this->getActive(), SLOT(toggleObjectOrientation()));

	showNormalsAct = new QAction(tr("Show Normals"), this);
	showNormalsAct->setCheckable(true);
	sm->registerAction(showNormalsAct, "Display Menu", "N");
	// objectOrienationAct->setStatusTip(tr("Copy the current selection's contents to the "
	connect(showNormalsAct, SIGNAL(triggered()), this->getActive(), SLOT(toggleNormals()));

	showGridAct = new QAction(tr("Show &Grid"), this);
	showGridAct->setCheckable(true);
	sm->registerAction(showGridAct, "Display Menu", "G");
	// showGridAct->setStatusTip(tr("Copy the current selection's contents to the "
	connect(showGridAct, SIGNAL(triggered()), getActive(), SLOT(toggleGrid()));

	showHUDAct = new QAction(tr("Show &Heads Up Display"), this);
	showHUDAct->setCheckable(true);
	sm->registerAction(showHUDAct, "Display Menu", "H");
	showHUDAct->setStatusTip(tr("Show the Heads Up Display"));
	connect(showHUDAct, SIGNAL(triggered()), this->getActive(), SLOT(toggleHUD()));

	#ifdef WITH_PYTHON
		mShowScriptEditorAct = new QAction(tr("Show Script &Editor"), this);
		mShowScriptEditorAct->setCheckable(true);
		mShowScriptEditorAct->setStatusTip( tr("Show the script editor to execute DLFL commands") );
		connect(mShowScriptEditorAct, SIGNAL(triggered()), this, SLOT(showHideScriptEditor()));
		sm->registerAction(mShowScriptEditorAct, "Display Menu", "SHIFT+CTRL+E");
	#endif
	
	#ifdef WITH_VERSE
		mShowVerseDialogAct = new QAction(tr("Show Verse &Dialog"), this);
		mShowVerseDialogAct->setCheckable(true);
		mShowVerseDialogAct->setStatusTip( tr("Show the verse dialog to view verse server connection status") );
		connect(mShowVerseDialogAct, SIGNAL(triggered()), this, SLOT(showHideVerseDialog()));
		sm->registerAction(mShowVerseDialogAct, "Display Menu", "SHIFT+CTRL+V");
	#endif	

		//Renderer Menu Actions
		wireframeRendererAct = new QAction(tr("&Wireframe Renderer"), this);
		wireframeRendererAct->setCheckable(true);
		sm->registerAction(wireframeRendererAct, "Renderer Menu", "1");
		wireframeRendererAct->setStatusTip(tr("Switch the current renderer to Wireframe"));
		connect(wireframeRendererAct, SIGNAL(triggered()), this, SLOT(useWireframeRenderer()));	

	normalRendererAct = new QAction(tr("&Normal Renderer"), this);
	normalRendererAct->setCheckable(true);
	sm->registerAction(normalRendererAct, "Renderer Menu", "2");
	normalRendererAct->setStatusTip(tr("Switch the current renderer to Normal"));
	connect(normalRendererAct, SIGNAL(triggered()), this, SLOT(useNormalRenderer()));

	lightedRendererAct = new QAction(tr("&Lighted Renderer"), this);
	lightedRendererAct->setCheckable(true);
	sm->registerAction(lightedRendererAct, "Renderer Menu", "3");
	lightedRendererAct->setStatusTip(tr("Switch the current renderer to Lighted"));
	connect(lightedRendererAct, SIGNAL(triggered()), this, SLOT(useLightedRenderer()));
	
	texturedRendererAct = new QAction(tr("&Textured Renderer"), this);
	texturedRendererAct->setCheckable(true);
	sm->registerAction(texturedRendererAct, "Renderer Menu", "4");
	texturedRendererAct->setStatusTip(tr("Switch the current renderer to Textured"));
	connect(texturedRendererAct, SIGNAL(triggered()), this, SLOT(useTexturedRenderer()));

	texturedLightedAct = new QAction(tr("Te&xtured Lighted Renderer"), this);
	texturedLightedAct->setCheckable(true);
	sm->registerAction(texturedLightedAct, "Renderer Menu", "5");
	texturedLightedAct->setStatusTip(tr("Switch the current renderer to Textured Lit"));
	connect(texturedLightedAct, SIGNAL(triggered()), this, SLOT(useTexturedLitRenderer()));

	patchRendererAct = new QAction(tr("&Patch Renderer"), this);
	patchRendererAct->setCheckable(true);
	sm->registerAction(patchRendererAct, "Renderer Menu", "6");
	patchRendererAct->setStatusTip(tr("Switch the current renderer to Patch"));
	connect(patchRendererAct, SIGNAL(triggered()), this, SLOT(usePatchRenderer()));

	mRendererActionGroup = new QActionGroup(this);
	mRendererActionGroup->setExclusive(true);
	mRendererActionGroup->addAction(wireframeRendererAct);
	mRendererActionGroup->addAction(normalRendererAct);
	mRendererActionGroup->addAction(lightedRendererAct);
	mRendererActionGroup->addAction(texturedRendererAct);
	mRendererActionGroup->addAction(texturedLightedAct);
	mRendererActionGroup->addAction(patchRendererAct);
	lightedRendererAct->setChecked(true);

	//PRIMITIVES MENU ACTIONS
	pCubeAct = new QAction(QIcon(":/images/prim_cube.png"), tr("&Cube"), this);
	sm->registerAction(pCubeAct, "Primitives Menu", "CTRL+1");
	pCubeAct->setStatusTip(tr("Load a Cube"));
	connect(pCubeAct, SIGNAL(triggered()), this, SLOT(loadCube()));

	pOctahedronAct = new QAction(QIcon(":/images/prim_octahedron.png"), tr("&Octahedron"), this);
	sm->registerAction(pOctahedronAct, "Primitives Menu", "CTRL+2");
	pOctahedronAct->setStatusTip(tr("Load an octahedron"));
	connect(pOctahedronAct, SIGNAL(triggered()), this, SLOT(loadOctahedron()));

	pTetrahedronAct = new QAction(QIcon(":/images/prim_tetrahedron.png"), tr("&Tetrahedron"), this);
	sm->registerAction(pTetrahedronAct, "Primitives Menu", "CTRL+3");
	pTetrahedronAct->setStatusTip(tr("Load a tetrahedron"));
	connect(pTetrahedronAct, SIGNAL(triggered()), this, SLOT(loadTetrahedron()));

	pDodecahedronAct = new QAction(QIcon(":/images/prim_dodecahedron.png"), tr("&Dodecahedron"), this);
	sm->registerAction(pDodecahedronAct, "Primitives Menu", "CTRL+4");
	pDodecahedronAct->setStatusTip(tr("Load a dodecahedron"));
	connect(pDodecahedronAct, SIGNAL(triggered()), this, SLOT(loadDodecahedron()));

	pIcosahedronAct = new QAction(QIcon(":/images/prim_icosahedron.png"), tr("&Icosahedron"), this);
	sm->registerAction(pIcosahedronAct, "Primitives Menu", "CTRL+5");
	pIcosahedronAct->setStatusTip(tr("Load an icosahedron"));
	connect(pIcosahedronAct, SIGNAL(triggered()), this, SLOT(loadIcosahedron()));

	pSoccerBallAct = new QAction(QIcon(":/images/prim_soccerball.png"), tr("&Soccer ball"), this);
	sm->registerAction(pSoccerBallAct, "Primitives Menu", "CTRL+6");
	pSoccerBallAct->setStatusTip(tr("Load a soccer ball"));
	connect(pSoccerBallAct, SIGNAL(triggered()), this, SLOT(loadSoccerball()));

	pGeodesicAct = new QAction(QIcon(":/images/prim_geodesic.png"), tr("&Geodesic Dome"), this);
	sm->registerAction(pGeodesicAct, "Primitives Menu", "CTRL+7");
	pGeodesicAct->setStatusTip(tr("Load a geodesic dome"));
	connect(pGeodesicAct, SIGNAL(triggered()), this, SLOT(loadGeodesic()));
	
	//Object Menu Actions
	subdivideAllEdgesAct = new QAction(tr("Subdivide All &Edges"), this);
	sm->registerAction(subdivideAllEdgesAct, "Settings", "");
	// subdivideAllEdgesAct->setStatusTip(tr("Copy the current selection's contents to the "
	connect(subdivideAllEdgesAct, SIGNAL(triggered()), this, SLOT(subdivideAllEdges()));

	planarizeAllFacesAct = new QAction(tr("Planarize All &Faces"), this);
	sm->registerAction(planarizeAllFacesAct, "Settings", "CTRL+X");
	// planarizeAllFacesAct->setStatusTip(tr("Copy the current selection's contents to the "
	connect(planarizeAllFacesAct, SIGNAL(triggered()), this, SLOT(planarizeFaces()));

	makeObjectSphericalAct = new QAction(tr("Make &Object Spherical"), this);
	sm->registerAction(makeObjectSphericalAct, "Settings", "CTRL+X");
	// makeObjectSphericalAct->setStatusTip(tr("Copy the current selection's contents to the "
	connect(makeObjectSphericalAct, SIGNAL(triggered()), this, SLOT(spheralizeObject()));

	makeObjectSmoothAct = new QAction(tr("Make Object &Smooth"), this);
	sm->registerAction(makeObjectSmoothAct, "Settings", "CTRL+X");
	// makeObjectSmoothAct->setStatusTip(tr("Copy the current selection's contents to the "
	connect(makeObjectSmoothAct, SIGNAL(triggered()), this, SLOT(smoothMesh()));

	createCrustAct = new QAction(tr("&Create Crust"), this);
	sm->registerAction(createCrustAct, "Settings", "CTRL+X");
	// createCrustAct->setStatusTip(tr("Copy the current selection's contents to the "
	connect(createCrustAct, SIGNAL(triggered()), this, SLOT(createCrust()));

	computeLightingAct = new QAction(tr("Compute &Lighting"), this);
	sm->registerAction(computeLightingAct, "Settings", "CTRL+X");
	// computeLightingAct->setStatusTip(tr("Copy the current selection's contents to the "
	connect(computeLightingAct, SIGNAL(triggered()), this, SLOT(recomputeLighting()));

	computeNormalsAndLightingAct = new QAction(tr("Compute &Normals and Lighting"), this);
	sm->registerAction(computeNormalsAndLightingAct, "Settings", "CTRL+X");
	// computeNormalsAndLightingAct->setStatusTip(tr("Copy the current selection's contents to the "
	connect(computeNormalsAndLightingAct, SIGNAL(triggered()), this, SLOT(recomputeNormals()));

	assignTextureCoordinatesAct = new QAction(tr("Assign &Texture Coordinates"), this);
	sm->registerAction(assignTextureCoordinatesAct, "Settings", "CTRL+X");
	// assignTextureCoordinatesAct->setStatusTip(tr("Copy the current selection's contents to the "
	connect(assignTextureCoordinatesAct, SIGNAL(triggered()), this, SLOT(assignTileTexCoords()));

	// //SELECTION MENU ACTIONS
	selectVertexAct = new QAction(tr("Select &Vertex"), this);
	sm->registerAction(selectVertexAct, "Selection", "SHIFT+V");
	selectVertexAct->setStatusTip(tr("Select a Vertex"));
	connect(selectVertexAct, SIGNAL(triggered()), this, SLOT(select_vertex()));

	editVertexAct = new QAction(tr("Edit Verte&x"), this);
	sm->registerAction(editVertexAct, "Selection", "SHIFT+V");
	editVertexAct->setStatusTip(tr("Select and Move Vertices One at a time."));
	connect(editVertexAct, SIGNAL(triggered()), this, SLOT(edit_vertex()));
	
	selectFaceAct = new QAction(tr("Select &Face"), this);
	sm->registerAction(selectFaceAct, "Selection", "SHIFT+F");
	selectFaceAct->setStatusTip(tr("Select One Face. Just for practice. :)"));
	connect(selectFaceAct, SIGNAL(triggered()), this, SLOT(select_face() ) );

	selectFaceLoopAct = new QAction(tr("Select Face Loo&p"), this);
	sm->registerAction(selectFaceLoopAct, "Selection", "SHIFT+P");
	selectFaceLoopAct->setStatusTip(tr("Select a Face Loop."));
	connect(selectFaceLoopAct, SIGNAL(triggered()), this, SLOT(select_face_loop() ) );
	
	selectMultipleFacesAct = new QAction(tr("Select &Multiple Faces"), this);
	sm->registerAction(selectMultipleFacesAct, "Selection", "SHIFT+G");
	connect( selectMultipleFacesAct , SIGNAL( triggered() ), this, SLOT( select_multiple_faces() ) );

	selectSimilarFacesAct = new QAction(tr("Select &Similar Faces"), this);
	sm->registerAction(selectSimilarFacesAct, "Selection", "SHIFT+G");
	connect( selectSimilarFacesAct , SIGNAL( triggered() ), this, SLOT( select_similar_faces() ) );

	selectCheckerboardFacesAct = new QAction(tr("C&heckerboard Select Faces"), this);
	sm->registerAction(selectCheckerboardFacesAct, "Selection", "");
	connect( selectCheckerboardFacesAct , SIGNAL( triggered() ), this, SLOT( select_checkerboard_faces() ) );

	selectAllFacesAct = new QAction(tr("Select &All Faces"), this);
	sm->registerAction(selectAllFacesAct, "Selection", "CTRL+A");
	connect( selectAllFacesAct , SIGNAL( triggered() ), this->getActive(), SLOT( selectAllFaces() ) );

	selectInverseFacesAct = new QAction(tr("Select &Inverse Faces"), this);
	sm->registerAction(selectInverseFacesAct, "Selection", "CTRL+I");
	connect( selectInverseFacesAct , SIGNAL( triggered() ), this->getActive(), SLOT( selectInverseFaces() ) );

	selectEdgeAct = new QAction(tr("Select &Edge"), this);
	sm->registerAction(selectEdgeAct, "Selection", "SHIFT+E");
	selectEdgeAct->setStatusTip(tr("Select one Edge"));
	connect( selectEdgeAct , SIGNAL( triggered() ), this,SLOT( select_edge() ) );

	selectEdgeLoopAct = new QAction(tr("Select Edge &Loop"), this);
	sm->registerAction(selectEdgeLoopAct, "Selection", "SHIFT+L");
	selectEdgeLoopAct->setStatusTip(tr("Select an Edge Loop"));
	connect( selectEdgeLoopAct , SIGNAL( triggered() ), this,SLOT( select_edge_loop() ) );
	
	selectCornerAct = new QAction(tr("Select &Corner"), this);
	sm->registerAction(selectCornerAct, "Selection", "SHIFT+C");
	selectCornerAct->setStatusTip(tr("Select a Face Vertex"));
	connect( selectCornerAct , SIGNAL( triggered() ), this, SLOT( select_corner() ) );
	
	exitSelectionModeAct = new QAction(tr("E&xit Selection Mode"), this);
	sm->registerAction(exitSelectionModeAct, "Selection", "SHIFT+X");
	connect( exitSelectionModeAct , SIGNAL( triggered() ), this, SLOT( exit_selection_mode() ) );

	clearSelectedModeAct = new QAction(tr("&Clear Selected"), this);
	sm->registerAction(clearSelectedModeAct, "Selection", "Escape");
	connect( clearSelectedModeAct , SIGNAL( triggered() ), this, SLOT( clear_selected() ) );

	mSelectVerticesMaskAct = new QAction(tr("Select &Vertices"), this);
	sm->registerAction(mSelectVerticesMaskAct, "Selection", "");
	mSelectVerticesMaskAct->setStatusTip(tr("Select by Component type: Vertices"));
	connect( mSelectVerticesMaskAct , SIGNAL( triggered() ), this, SLOT( select_vertices() ) );

	mSelectEdgesMaskAct = new QAction(tr("Select &Edges"), this);
	sm->registerAction(mSelectEdgesMaskAct, "Selection", "");
	mSelectEdgesMaskAct->setStatusTip(tr("Select by Component type: Edges"));
	connect( mSelectEdgesMaskAct , SIGNAL( triggered() ), this, SLOT( select_edges() ) );

	mSelectFacesMaskAct = new QAction(tr("Select &Faces"), this);
	sm->registerAction(mSelectFacesMaskAct, "Selection", "");
	mSelectFacesMaskAct->setStatusTip(tr("Select by Component type: Faces"));
	connect( mSelectFacesMaskAct , SIGNAL( triggered() ), this, SLOT( select_faces() ) );

	mSelectFaceVerticesMaskAct = new QAction(tr("Select &Corner"), this);
	sm->registerAction(mSelectFaceVerticesMaskAct, "Selection", "");
	mSelectFaceVerticesMaskAct->setStatusTip(tr("Select by Component type: Face-Vertices"));
	connect( mSelectFaceVerticesMaskAct , SIGNAL( triggered() ), this, SLOT( select_face_vertices() ) );
 
	//SETTINGS ACTIONS	
	mPreferencesAct = new QAction(tr("&Preferences"), this);
	sm->registerAction(mPreferencesAct, "Settings", "CTRL+,");
	mPreferencesAct->setStatusTip(tr("Open the Preferences Dialog"));
	connect(mPreferencesAct, SIGNAL(triggered()), this, SLOT(openPreferences()));	

	//LANGUAGE MENU BAR ACTIONS
	englishAct = new QAction(tr("English"),this);
	//sm->connect( englishAct , SIGNAL( triggered() ), SLOT  ( configure() ) );
	sm->registerAction(englishAct, "Languages", "CTRL+F7");

	spanishAct = new QAction(tr("Spanish"),this);
	//sm->connect( spanishAct , SIGNAL( triggered() ), SLOT  ( configure() ) );
	sm->registerAction(spanishAct, "Languages", "CTRL+F8");

	germanAct = new QAction(tr("German"),this);
	//sm->connect( germanAct , SIGNAL( triggered() ), SLOT  ( configure() ) );
	sm->registerAction(germanAct, "Languages", "CTRL+F9");

	frenchAct = new QAction(tr("French"),this);
	//sm->connect( frenchAct , SIGNAL( triggered() ), SLOT  ( configure() ) );
	sm->registerAction(frenchAct, "Languages", "CTRL+F10");

	turkishAct = new QAction(tr("Turkish"),this);
	//sm->connect( turkishAct , SIGNAL( triggered() ), SLOT  ( configure() ) );
	sm->registerAction(turkishAct, "Languages", "CTRL+F11");

	catalanAct = new QAction(tr("Catalan"),this);
	// sm->connect( catalanAct , SIGNAL( triggered() ), SLOT  ( configure() ) );
	sm->registerAction(catalanAct, "Languages", "CTRL+F12");
	
	#ifdef WITH_VERSE
		//verse menu actions
		mVerseConnectLocalhostAct = new QAction(tr("Connect to localhost..."), this);
		mVerseConnectLocalhostAct->setStatusTip( tr("Connect to localhost") );
		connect(mVerseConnectLocalhostAct, SIGNAL(triggered()), mVerseDialog, SLOT(connectLocalhost()));
		sm->registerAction(mVerseConnectLocalhostAct, "Verse Menu", "");
	
		mVerseConnectAct = new QAction(tr("Connect to host..."), this);
		mVerseConnectAct->setStatusTip( tr("Connect to host...") );
		connect(mVerseConnectAct, SIGNAL(triggered()), mVerseDialog, SLOT(connectHost()));
		sm->registerAction(mVerseConnectAct, "Verse Menu", "");

		mVerseDisconnectAct = new QAction(tr("Disconnect session"), this);
		mVerseDisconnectAct->setStatusTip( tr("Disconnect Verse Session") );
		connect(mVerseDisconnectAct, SIGNAL(triggered()), mVerseDialog, SLOT(disconnectHost()));
		sm->registerAction(mVerseDisconnectAct, "Verse Menu", "");

		mVerseDisconnectAllAct = new QAction(tr("Disconnect All Sessions"), this);
		mVerseDisconnectAllAct->setStatusTip( tr("Disconnect All Sessions") );
		connect(mVerseDisconnectAllAct, SIGNAL(triggered()), mVerseDialog, SLOT(disconnectAll()));
		sm->registerAction(mVerseDisconnectAllAct, "Verse Menu", "");
		
		mVerseStartServerAct = new QAction(tr("Start Verse Server"), this);
		mVerseStartServerAct->setStatusTip( tr("Disconnect All Nodes") );
		connect(mVerseStartServerAct, SIGNAL(triggered()), mVerseDialog, SLOT(startServer()));
		sm->registerAction(mVerseStartServerAct, "Verse Menu", "");

		mVerseKillServerAct = new QAction(tr("Kill Verse Server"), this);
		mVerseKillServerAct->setStatusTip( tr("Kill the Local Verse server process") );
		connect(mVerseKillServerAct, SIGNAL(triggered()), mVerseDialog, SLOT(killServer()));
		sm->registerAction(mVerseKillServerAct, "Verse Menu", "");
	#endif
	
	mPerformRemeshingAct = new QAction(tr("Perform Remeshing"), this);
	mPerformRemeshingAct->setStatusTip( tr("Perform the current remeshing scheme") );
	connect(mPerformRemeshingAct, SIGNAL(triggered()), this, SLOT(performRemeshing()));
	sm->registerAction(mPerformRemeshingAct, "Remeshing Menu", "CTRL+R");
	
	//help menu actions
	mHelpAct = new QAction(tr("&Help Contents"), this);
	mHelpAct->setStatusTip( tr("View the help file") );
	connect(mHelpAct, SIGNAL(triggered()), this, SLOT(help()));
	sm->registerAction(mHelpAct, "Help Menu", "F1");

	mAboutAct = new QAction(tr("&About TopMod"), this);
	mAboutAct->setStatusTip( tr("About TopMod") );
	connect(mAboutAct, SIGNAL(triggered()), this, SLOT(about()));
	sm->registerAction(mAboutAct, "Help Menu", "");

	mAboutQtAct = new QAction(tr("About &Qt"), this);
	mAboutQtAct->setStatusTip( tr("About Qt") );
	connect(mAboutQtAct, SIGNAL(triggered()), qApp, SLOT(aboutQt()));
	sm->registerAction(mAboutQtAct, "Help Menu", "");
	
}

void MainWindow::createMenus(){
	#ifdef __APPLE__
		menuBar = new QMenuBar(0);
		setMenuBar(menuBar);
		// setUnifiedTitleAndToolBarOnMac(true);
	#else
		menuBar = new QMenuBar(this);
		setMenuBar(menuBar);
	#endif
	
	fileMenu = new QMenu(tr("&File"));
	menuBar->addMenu(fileMenu);
	fileMenu->setTearOffEnabled(true);
	
	fileMenu->addAction(mOpenAct);
	fileMenu->addAction(mSaveAct);
	fileMenu->addAction(mSaveAsAct);
	fileMenu->addAction(mSavePatchesAct);

	#ifdef WITH_VERSE
		fileMenu->addSeparator();
		mVerseMenu = new QMenu(tr("&Verse"));
		fileMenu->addMenu(mVerseMenu);
		mVerseMenu->addAction(mVerseStartServerAct);
		// mVerseMenu->addAction(mVerseKillServerAct);
		mVerseMenu->addSeparator();
		mVerseMenu->addAction(mVerseConnectLocalhostAct);
		mVerseMenu->addAction(mVerseConnectAct);
		// mVerseMenu->removeAction(mVerseConnectAct);
		mVerseMenu->addSeparator();
		// mVerseMenu->addAction(mVerseDisconnectAct);
		// mVerseMenu->addAction(mVerseDisconnectAllAct);
	#endif
	
	fileMenu->addSeparator();
	fileMenu->addAction(loadTextureAct);
	fileMenu->addAction(printInfoAct);
	fileMenu->addAction(printFaceListAct);
	fileMenu->addAction(printVertexListAct);
	fileMenu->addAction(printEdgeListAct);
	fileMenu->addAction(mPrintCVListAct);
	fileMenu->addSeparator();
	fileMenu->addAction(mExitAct);

	editMenu = new QMenu(tr("&Edit"));
	menuBar->addMenu(editMenu);
	editMenu->addAction(mUndoAct);
	editMenu->addAction(mRedoAct);
	editMenu->setTearOffEnabled(true);
	editMenu->addSeparator();
	editMenu->addAction(mClearUndoListAct);
	editMenu->addSeparator();
	editMenu->addAction(mPreferencesAct);
	
	mViewMenu = new QMenu(tr("&View"));
	mViewMenu->setTearOffEnabled(true);
	menuBar->addMenu(mViewMenu);
	mViewMenu->addAction(mPerspViewAct);
	// mViewMenu->addAction(mTopViewAct);
	// mViewMenu->addAction(mBottomViewAct);
	// mViewMenu->addAction(mLeftViewAct);
	// mViewMenu->addAction(mRightViewAct);
	// mViewMenu->addAction(mFrontViewAct);
	// mViewMenu->addAction(mBackViewAct);

	displayMenu = new QMenu(tr("&Display"));
	displayMenu->setTearOffEnabled(true);
	menuBar->addMenu(displayMenu);
	displayMenu->addAction(showVerticesAct);
	
	mShowIDsMenu = new QMenu(tr("&Show IDs"));
	displayMenu->addMenu(mShowIDsMenu);
	mShowIDsMenu->setTearOffEnabled(true);
	mShowIDsMenu->addAction(mShowFaceIDsAct);
	mShowIDsMenu->addAction(mShowEdgeIDsAct);
	mShowIDsMenu->addAction(mShowVertexIDsAct);
	mShowIDsMenu->addAction(mShowSelectedIDsAct);
	displayMenu->addAction(showSilhouetteAct);
	displayMenu->addAction(showWireframeAct);
	displayMenu->addAction(showCoordinateAxesAct);
	displayMenu->addAction(showGridAct);
	displayMenu->addAction(showHUDAct);
	displayMenu->addAction(objectOrientationAct);
	displayMenu->addAction(showNormalsAct);
	
	#ifdef WITH_PYTHON
		displayMenu->addAction(mShowScriptEditorAct);
	#endif
	
	#ifdef WITH_VERSE
		displayMenu->addAction(mShowVerseDialogAct);
	#endif
	
	displayMenu->addAction(mFullscreenAct);

	rendererMenu = new QMenu(tr("&Renderer"));
	rendererMenu->setTearOffEnabled(true);
	menuBar->addMenu(rendererMenu);
	rendererMenu->addAction(wireframeRendererAct);
	rendererMenu->addAction(normalRendererAct);
	rendererMenu->addAction(lightedRendererAct);
	rendererMenu->addAction(texturedRendererAct);
	// texturedRendererAct->setEnabled(false);
	rendererMenu->addAction(texturedLightedAct);
	// texturedLightedAct->setEnabled(false);
	rendererMenu->addSeparator()->setText(tr("Special Mode??"));
	rendererMenu->addAction(patchRendererAct);

	primitivesMenu = new QMenu(tr("&Primitives"));
	primitivesMenu->setTearOffEnabled(true);
	menuBar->addMenu(primitivesMenu);
	primitivesMenu->addAction(pCubeAct);
	primitivesMenu->addAction(pOctahedronAct);
	primitivesMenu->addAction(pTetrahedronAct);
	primitivesMenu->addAction(pDodecahedronAct);
	primitivesMenu->addAction(pIcosahedronAct);
	primitivesMenu->addAction(pSoccerBallAct);
	primitivesMenu->addAction(pGeodesicAct);
	
	mToolsMenu = new QMenu(tr("&Tools"));
	mToolsMenu->setTearOffEnabled(true);
	mToolsMenu->addMenu(mBasicsMode->getMenu());
	mToolsMenu->addMenu(mExtrusionMode->getMenu());
	// mToolsMenu->addMenu(mConicalMode->getMenu());
	mToolsMenu->addMenu(mHighgenusMode->getMenu());
	mToolsMenu->addMenu(mTexturingMode->getMenu());
	menuBar->addMenu(mToolsMenu);
	
	mRemeshingMenu = mRemeshingMode->getMenu();
	mRemeshingMenu->addAction(mPerformRemeshingAct);
	mRemeshingMenu->setTearOffEnabled(true);
	menuBar->addMenu(mRemeshingMenu);		

	objectMenu = new QMenu(tr("&Object"));
	objectMenu->setTearOffEnabled(true);
	menuBar->addMenu(objectMenu);
	objectMenu->addAction(subdivideAllEdgesAct);
	objectMenu->addAction(planarizeAllFacesAct);
	objectMenu->addAction(makeObjectSphericalAct);
	objectMenu->addAction(makeObjectSmoothAct);
	objectMenu->addAction(createCrustAct);
	objectMenu->addAction(computeLightingAct);
	objectMenu->addAction(computeNormalsAndLightingAct);
	objectMenu->addAction(assignTextureCoordinatesAct);

	selectionMenu = new QMenu(tr("&Selection"));
	menuBar->addMenu(selectionMenu);
	selectionMenu->addAction(selectVertexAct);
	selectionMenu->addAction(editVertexAct);
	selectionMenu->addAction(selectFaceAct);
	selectionMenu->addAction(selectFaceLoopAct);
	selectionMenu->addAction(selectMultipleFacesAct);
	selectionMenu->addAction(selectSimilarFacesAct);
	selectionMenu->addAction(selectCheckerboardFacesAct);
	selectionMenu->addAction(selectAllFacesAct);
	selectionMenu->addAction(selectInverseFacesAct);
	selectionMenu->addAction(selectEdgeAct);
	selectionMenu->addAction(selectEdgeLoopAct);
	selectionMenu->addAction(selectCornerAct);
	selectionMenu->addAction(exitSelectionModeAct);
	selectionMenu->addAction(clearSelectedModeAct);

	//selection Masks (vertices / edges / faces / face-vertices)
	mSelectionMaskMenu = new QMenu(tr("Selection &Masks"));
	selectionMenu->addMenu(mSelectionMaskMenu);
	mSelectionMaskMenu->addAction(mSelectVerticesMaskAct);
	mSelectionMaskMenu->addAction(mSelectFacesMaskAct);
	mSelectionMaskMenu->addAction(mSelectEdgesMaskAct);
	mSelectionMaskMenu->addAction(mSelectFaceVerticesMaskAct);
	
	mHelpMenu = new QMenu(tr("&Help"));
	menuBar->addMenu(mHelpMenu);
	mHelpMenu->addAction(mHelpAct);
	mHelpMenu->addAction(mAboutAct);
	mHelpMenu->addAction(mAboutQtAct);
	
	// settingsMenu = new QMenu(tr("Se&ttings"));
	// settingsMenu->setTearOffEnabled(true);
	// menuBar->addMenu(settingsMenu);
	// settingsMenu->addAction(manageShortcutsAct);
	// settingsMenu->addAction(mEditStyleSheetAct);
	// languageMenu = new QMenu(tr("&Language"));
	// languageMenu->setTearOffEnabled(true);
	// settingsMenu->addMenu(languageMenu);
	// languageMenu->addAction(englishAct);
	// languageMenu->addAction(spanishAct);
	// languageMenu->addAction(germanAct);
	// languageMenu->addAction(frenchAct);
	// languageMenu->addAction(turkishAct);
	// languageMenu->addAction(catalanAct);
	
}

void MainWindow::createToolBars() {
	
	//the main tool options DockWidget
	mToolOptionsDockWidget = new QDockWidget(tr("Tool Options"),this);
	mToolOptionsDockWidget->setFeatures(QDockWidget::DockWidgetFloatable | QDockWidget::DockWidgetMovable /* | QDockWidget::DockWidgetClosable*/);
	mToolOptionsDockWidget->setAllowedAreas(Qt::TopDockWidgetArea);
	mToolOptionsStackedWidget = new QStackedWidget();
	// mToolOptionsDockWidget->setFloating(true);
	mToolOptionsDockWidget->hide();
	mToolOptionsDockWidget->setWidget(mToolOptionsStackedWidget);
	
	mToolOptionsDockWidget->setFloating(false);
	// mToolOptionsTitleBarLayout = new QBoxLayout(QBoxLayout::LeftToRight);
	// mToolOptionsTitleBarWidget = new QWidget;
	// mToolOptionsTitleBarWidget->setLayout(mToolOptionsTitleBarLayout);
	// mToolOptionsDockWidget->setTitleBarWidget(mToolOptionsTitleBarWidget);
	// mToolOptionsDockWidget->titleBarWidget()->setContentsMargins(0,0,0,0);
	// mToolOptionsDockWidget->move(0,22);
	// mToolOptionsDockWidget->setWindowTitle("Insert Edge Mode");
	addDockWidget(Qt::TopDockWidgetArea, mToolOptionsDockWidget);
	
	mEditToolBar = new QToolBar(tr("Edit"));
	addToolBar(Qt::TopToolBarArea,mEditToolBar);
	mEditToolBar->addAction(mOpenAct);
	mEditToolBar->addAction(mSaveAsAct);
	mEditToolBar->addAction(mUndoAct);
	mEditToolBar->addAction(mRedoAct);
	mEditToolBar->setIconSize(QSize(32,32));
	
	//selection masks toolbar
	mSelectionMaskToolBar = new QToolBar(tr("Selection Masks"));
	//addToolBar(Qt::TopToolBarArea,mSelectionMaskToolBar);
	mSelectionMaskToolBar->addAction(mSelectVerticesMaskAct);
	mSelectionMaskToolBar->addAction(mSelectFacesMaskAct);
	mSelectionMaskToolBar->addAction(mSelectEdgesMaskAct);
	mSelectionMaskToolBar->addAction(mSelectFaceVerticesMaskAct);
	
		
	mPrimitivesToolBar = new QToolBar(tr("Primitives"));
	// mPrimitivesToolBar->setFloatable(true);
	addToolBar(Qt::TopToolBarArea,mPrimitivesToolBar);
	mPrimitivesToolBar->setIconSize(QSize(32,32));
	mPrimitivesToolBar->addAction(pCubeAct);
	mPrimitivesToolBar->addAction(pOctahedronAct);
	mPrimitivesToolBar->addAction(pTetrahedronAct);
	mPrimitivesToolBar->addAction(pDodecahedronAct);
	mPrimitivesToolBar->addAction(pIcosahedronAct);
	mPrimitivesToolBar->addAction(pSoccerBallAct);
	mPrimitivesToolBar->addAction(pGeodesicAct);
	
	//basic tools - six buttons
	mToolsToolBar = new QToolBar(tr("Tools"));
	addToolBar(Qt::TopToolBarArea,mToolsToolBar);
	mToolsToolBar->setIconSize(QSize(32,32));
		
	addToolBarBreak();
	
	mExtrusionToolBar = new QToolBar(tr("Extrusion Tools"));
	addToolBar(Qt::TopToolBarArea,mExtrusionToolBar);
	mExtrusionToolBar->setIconSize(QSize(32,32));
	
	mConicalToolBar = new QToolBar(tr("Conical Tools"));
	// addToolBar(Qt::TopToolBarArea,mConicalToolBar);
	mConicalToolBar->setIconSize(QSize(32,32));
		
	mHighgenusToolBar = new QToolBar(tr("High Genus Tools"));
	addToolBar(Qt::TopToolBarArea,mHighgenusToolBar);
	mHighgenusToolBar->setIconSize(QSize(32,32));
	
	mTexturingToolBar = new QToolBar(tr("Texturing Tools"));
	addToolBar(Qt::TopToolBarArea,mTexturingToolBar);
	mTexturingToolBar->setIconSize(QSize(32,32));
	
	mRemeshingToolBar = new QToolBar(tr("Remeshing"));
	// addToolBar(Qt::TopToolBarArea,mRemeshingToolBar);
	mRemeshingToolBar->setIconSize(QSize(32,32));
	
	//tools ction group initialization
	mToolsActionGroup = new QActionGroup(this);
	mToolsActionGroup->setExclusive(true);
		
	mBasicsMode->addActions(mToolsActionGroup, mToolsToolBar, mToolOptionsStackedWidget);	
	mExtrusionMode->addActions(mToolsActionGroup, mExtrusionToolBar, mToolOptionsStackedWidget);
	mConicalMode->addActions(mToolsActionGroup, mConicalToolBar, mToolOptionsStackedWidget);
	
	mRemeshingActionGroup = new QActionGroup(this);
	mRemeshingMode->addActions(mToolsActionGroup, mRemeshingToolBar, mToolOptionsStackedWidget);
	
	mHighgenusMode->addActions(mToolsActionGroup, mHighgenusToolBar, mToolOptionsStackedWidget);
	mTexturingMode->addActions(mToolsActionGroup, mTexturingToolBar, mToolOptionsStackedWidget);

}

void MainWindow::setToolOptions(QWidget *optionsWidget) {
	mToolOptionsDockWidget->setWindowTitle(optionsWidget->windowTitle());
	mToolOptionsStackedWidget->setCurrentWidget(optionsWidget);
	// show or hide the dockwidget options
	if (optionsWidget->windowTitle() != "" && mToolOptionsDockWidget->isHidden())
		mToolOptionsDockWidget->show();
	else if (!mToolOptionsDockWidget->isHidden() && optionsWidget->windowTitle() == "")
		mToolOptionsDockWidget->hide();
}

void MainWindow::createStatusBar() {
  statusBar()->showMessage(tr("Welcome to TopMod"));
}

void MainWindow::readSettings() {
	mSettings->beginGroup("MainWindow");
  QPoint pos = mSettings->value("pos", QPoint(100, 100)).toPoint();
  QSize size = mSettings->value("size", QSize(800, 600)).toSize();
	mSettings->endGroup();
	
	resize(size.width(),size.height());
  move(pos);}

void MainWindow::writeSettings() {
	  
	mSettings->beginGroup("MainWindow");
	mSettings->setValue("pos", pos());
  mSettings->setValue("size", size());
	mSettings->endGroup();
	
	mSettings->beginGroup("ViewPortColors");
	
	mSettings->endGroup();
}

bool MainWindow::maybeSave() {
  if (this->isModified()) {
      int ret = QMessageBox::warning(this, tr("TopMod"),
                   tr("The document has been modified.\n"
                      "Do you want to save your changes?"),
                   QMessageBox::Yes | QMessageBox::Default,
                   QMessageBox::No,
                   QMessageBox::Cancel | QMessageBox::Escape);
      if (ret == QMessageBox::Yes)
          return this->saveFile();
      else if (ret == QMessageBox::Cancel)
          return false;
  }
  return true;
}

void MainWindow::loadFile(QString fileName) {
  openFile(fileName);
  this->setCurrentFile(fileName);
  statusBar()->showMessage(tr("File loaded"), 2000);
}

bool MainWindow::saveFile(QString fileName) {
  this->setCurrentFile(fileName);
  statusBar()->showMessage(tr("File saved"), 2000);
  return true;
}

void MainWindow::closeEvent(QCloseEvent *event) {
	//close the help file if it's open... not sure this is necessary
	if (mAssistantClient)
		mAssistantClient->closeAssistant();

	if (maybeSave()) {
	    writeSettings();
	    event->accept();
	} else event->ignore();
}

void MainWindow::openFile(QString fileName){
	QFile file(fileName);
	file.open(QIODevice::ReadOnly);
	QFileInfo info(file);
	QByteArray ba = info.absoluteFilePath().toLatin1();
	const char *filename = ba.data();
	mWasPrimitive = false;
	mIsPrimitive = false;
	if (!curFile.isEmpty()){
		undoPush();
		setModified(false);
	}

	readObject(filename);
#ifdef WITH_PYTHON
	DLFLObjectPtr obj = &object;
	if( obj )
		emit loadedObject(obj,fileName);
#endif
	recomputePatches();
	recomputeNormals();
	setCurrentFile(fileName);
	active->redraw();
}

void MainWindow::about() {
  QMessageBox::about(this, tr("About TopMod"),
		     tr("The About page for TopMod is still in the works \n stay tuned... \n"));
}

void MainWindow::initializeHelp(){
	mAssistantClient = new QAssistantClient(QLibraryInfo::location(QLibraryInfo::BinariesPath), this);
  // QStringList arguments;
  // arguments << "-profile" << QString("documentation") + QDir::separator() + QString("simpletextviewer.adp");
  // mAssistantClient->setArguments(arguments);
}

void MainWindow::help() {
	mAssistantClient->showPage("");
}

void MainWindow::documentWasModified() {
  setWindowModified(this->isModified());
}

void MainWindow::on_editStyleAction_triggered() {
  mStyleSheetEditor->show();
  mStyleSheetEditor->activateWindow();
}

void MainWindow::openPreferences() {
  mPreferencesDialog->display();
}

void MainWindow::showHideScriptEditor(){
	
#ifdef WITH_PYTHON
	if( mScriptEditorDockWidget->isVisible( ) )
	  mScriptEditorDockWidget->hide( );
	else {
	  mScriptEditorDockWidget->show( );
	  mScriptEditorDockWidget->setFocus();
	}
#endif
}

void MainWindow::showHideVerseDialog(){
#ifdef WITH_VERSE
	if( mVerseDialogDockWidget->isVisible( ) )
	  mVerseDialogDockWidget->hide( );
	else {
	  mVerseDialogDockWidget->show( );
	  mVerseDialogDockWidget->setFocus();
	}
#endif
}

#ifdef WITH_VERSE
void MainWindow::verseConnected(){
	mVerseMenu->insertAction(mVerseConnectLocalhostAct, mVerseDisconnectAct);
	mVerseMenu->removeAction(mVerseConnectLocalhostAct);
	mVerseMenu->removeAction(mVerseConnectAct);
}

void MainWindow::verseDisconnected(){
	mVerseMenu->insertAction(mVerseDisconnectAct,mVerseConnectLocalhostAct);
	mVerseMenu->insertAction(mVerseDisconnectAct,mVerseConnectAct);
	mVerseMenu->removeAction(mVerseDisconnectAct);
}
void MainWindow::verseStarted(){
	mVerseMenu->insertAction(mVerseStartServerAct, mVerseKillServerAct);
	mVerseMenu->removeAction(mVerseStartServerAct);	
}

void MainWindow::verseKilled(){
	mVerseMenu->insertAction(mVerseKillServerAct, mVerseStartServerAct);
	mVerseMenu->removeAction(mVerseKillServerAct);		
}
#endif

void MainWindow::createRenderers(){
	wired = new WireframeRenderer();
//wired->setRenderFlags(DLFLRender::ShowWireframe); // redundant?

	normal = new NormalRenderer();
	normal->setRenderFlags(DLFLRenderer::ShowWireframe);

	lit = new LitRenderer();
	lit->setRenderFlags(DLFLRenderer::ShowWireframe);

	textured = new TexturedRenderer();
	textured->setRenderFlags(DLFLRenderer::ShowWireframe);

	texturedlit = new TexturedLitRenderer();
	texturedlit->setRenderFlags(DLFLRenderer::ShowWireframe);

	patch = new PatchRenderer();
	patch->setRenderFlags(DLFLRenderer::ShowWireframe);
}

void MainWindow::destroyRenderers(){
	delete wired;
	delete normal;
	delete lit;
	delete textured;
	delete texturedlit;
	delete patch;
};

void MainWindow::setWarmLightColor(QColor c){
	plight.warmcolor.set(c.redF(),c.greenF(),c.blueF());
	recomputeLighting();
	redraw();
}

void MainWindow::setCoolLightColor(QColor c){
	plight.coolcolor.set(c.redF(),c.greenF(),c.blueF());
	recomputeLighting();
	redraw();
}

void MainWindow::setLightIntensity(double i){
	plight.intensity = i;
	recomputeLighting();
	redraw();
}

void MainWindow::setUndoLimit(int limit) {
	undolimit = limit;
}

void MainWindow::toggleUndo(void) {
	if ( useUndo ) useUndo = false;
	else useUndo = true;
}

void MainWindow::doDrag(int x, int y) { // brianb
	int drag_endx = x;
int drag_endy = y;

GLdouble obj_world[3],  // Object world coordinates
	obj_window[3], // Object window coordinates 
	ms_window[3],  // Mouse start drag window
	ms_world[3],   // Mouse start drag world
	me_window[3],  // Mouse end drag window
	me_world[3];   // Mouse end drag world
GLdouble modelMatrix[16], projMatrix[16];
GLint viewport[4];
GLint realy;
DLFLVertexPtr vptr;
Viewport* viewp;

switch ( mode ) {
	case EditVertex:
	if (GLWidget::numSelectedLocators() > 0)
	{
		if (!is_editing) {
			undoPush();
			is_editing = true;
		}
		vptr = active->getLocatorPtr()->getActiveVertex();

				// Save previous transformations
		glMatrixMode(GL_PROJECTION);
		glPushMatrix();
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();

				// Apply current transformation
		viewp = active->getViewport();
		viewp->reshape();
		viewp->apply_transform();

				// Get the info
		glGetIntegerv(GL_VIEWPORT, viewport);
		glGetDoublev(GL_MODELVIEW_MATRIX, modelMatrix);
		glGetDoublev(GL_PROJECTION_MATRIX, projMatrix);

		obj_world[0] = vptr->getCoords()[0]; 
		obj_world[1] = vptr->getCoords()[1]; 
		obj_world[2] = vptr->getCoords()[2];

				// Project object coordinates to window coordinates (to get accurate window depth)
		gluProject(obj_world[0],obj_world[1],obj_world[2],
			modelMatrix,projMatrix,viewport,
			&obj_window[0],&obj_window[1],&obj_window[2]);

				// Set start and end window coordinates using depth coordinate found above
		ms_window[0] = drag_startx;  ms_window[1] = drag_starty;  ms_window[2] = obj_window[2];
		me_window[0] = drag_endx;    me_window[1] = drag_endy;    me_window[2] = obj_window[2];

				// Unproject start drag window coordinates to world coordinates
		gluUnProject(ms_window[0],ms_window[1],ms_window[2],
			modelMatrix, projMatrix, viewport,
			&ms_world[0],&ms_world[1],&ms_world[2]);

				// Unproject end drag window coordinates to world coordinates
		gluUnProject(me_window[0],me_window[1],me_window[2],
			modelMatrix, projMatrix, viewport,
			&me_world[0],&me_world[1],&me_world[2]);

				// Switch on locked axis and update object world position
		switch (active->getLocatorPtr()->getSelectedAxis())
		{
			case 0: // X-axis
			obj_world[0] = obj_world[0] + me_world[0] - ms_world[0];     
			break;

			case 1: // Y-axis
			obj_world[1] = obj_world[1] + me_world[1] - ms_world[1];                     
			break;

			case 2: // Z-axis
			obj_world[2] = obj_world[2] + me_world[2] - ms_world[2];                 
			break;

			case 3:  // User can drag freely along viewing place
			default: 
			obj_world[0] = obj_world[0] + me_world[0] - ms_world[0];             
			obj_world[1] = obj_world[1] + me_world[1] - ms_world[1];
			obj_world[2] = obj_world[2] + me_world[2] - ms_world[2];
			break;
		}

		vptr->setCoords(Vector3d(obj_world[0],obj_world[1],obj_world[2]));

				// Update patches
				//object.bezierDefaults();

				// Reset drag start points
		startDrag(drag_endx,drag_endy);

				// Restore previous transformations
		glPopMatrix();
		glMatrixMode(GL_PROJECTION);
		glPopMatrix();

		redraw();
	}
	break;

	default:
	doSelection(x,y);
	break;
}
}// brianb

	// Do selection of various entities depending on current mode
void MainWindow::doSelection(int x, int y) {
	DLFLVertexPtr svptr = NULL;
	DLFLEdgePtr septr = NULL;
	DLFLFacePtr sfptr = NULL;
	DLFLFaceVertexPtr sfvptr = NULL;
	DLFLLocatorPtr slptr = NULL; // brianb

	DLFLEdgePtrArray septrarr;
	DLFLEdgePtrArray::iterator eit;
	DLFLFacePtrArray sfptrarr;
	DLFLFacePtrArray::iterator first, last;

	switch ( mode ) {

		case EditVertex:     // brianb
		slptr = active->getLocatorPtr();
		svptr = active->getLocatorPtr()->getActiveVertex();
		if (svptr == NULL) {
			svptr = active->selectVertex(x,y);
			slptr->setActiveVertex(svptr);
		}
								// Test for locator selection
		if (slptr->getActiveVertex() != NULL)	{
			slptr = active->selectLocator(x,y);
			if (slptr != NULL) {
				GLWidget::setSelectedLocator(0,slptr);
				startDrag(x,y);
			}
			else {
				active->getLocatorPtr()->setActiveVertex(NULL);
				GLWidget::clearSelectedLocators();
			} 
		}
		break;
		case SelectVertex:
		case MarkVertex:
		case CutVertex://ozgur
		svptr = active->selectVertex(x,y);
		GLWidget::setSelectedVertex(num_sel_verts,svptr);
		break;
		case MultiSelectVertex :
		svptr = active->selectVertex(x,y);
		if ( !GLWidget::isSelected(svptr) )
			GLWidget::setSelectedVertex(num_sel_verts,svptr);
		break;
		case DeleteEdge :
		case SubdivideEdge :
		case CollapseEdge :
		case SelectEdge :
		case CutEdge :
		case TruncateEdge :
		case MarkEdge ://ozgur
		septr = active->selectEdge(x,y);
		GLWidget::setSelectedEdge(num_sel_edges,septr);
		break;
		case CutEdgeandVertex://ozgur
		septr = active->selectEdge(x,y);
		svptr = active->selectVertex(x,y);
		GLWidget::setSelectedEdge(num_sel_edges,septr);
		GLWidget::setSelectedVertex(num_sel_verts,svptr);
		break;
		case MultiSelectEdge :
		septr = active->selectEdge(x,y);
		if ( !GLWidget::isSelected(septr) )
			GLWidget::setSelectedEdge(num_sel_edges,septr);
		break;
		case SelectEdgeLoop:
		if (QApplication::keyboardModifiers() != Qt::ShiftModifier){
			GLWidget::clearSelectedEdges();
		}
		septr = active->selectEdge(x,y);
		if (septr && QApplication::keyboardModifiers() == Qt::ControlModifier && GLWidget::isSelected(septr)){
			deselect_edges = true;
			GLWidget::clearSelectedEdge(septr);
			num_sel_edges--;
			getEdgeLoopSelection(septr);
			deselect_edges = false;
		}
		else if ( septr && !GLWidget::isSelected(septr)){
			GLWidget::setSelectedEdge(num_sel_edges,septr);
			num_sel_edges++;
			getEdgeLoopSelection(septr);
		}
		active->redraw();
		break;
		case SelectFace :
		case ExtrudeFace :
		case ExtrudeFaceDS :
		case ExtrudeDualFace :
		case ExtrudeFaceDodeca :
		case ExtrudeFaceIcosa :
		case StellateFace :
		case DoubleStellateFace :
		case CrustModeling :
		case ConnectFaces :
		case CutFace://ozgur
		sfptr = active->selectFace(x,y);
		GLWidget::setSelectedFace(num_sel_faces,sfptr);
		break;
		case SelectSimilarFaces :
		//clear selection if shift isn't down
		if (QApplication::keyboardModifiers() != Qt::ShiftModifier)
			GLWidget::clearSelectedFaces();
		sfptr = active->selectFace(x,y);
		if (sfptr){
			GLWidget::setSelectedFace(num_sel_faces,sfptr);
			num_sel_faces++;
			DLFLFacePtrArray sfptrarray;
			vector<DLFLFacePtr>::iterator it;
			DLFL::selectMatchingFaces(&object, sfptr, sfptrarray);
			for (it = sfptrarray.begin(); it != sfptrarray.end(); it++){
				if (!GLWidget::isSelected(*it)){
					GLWidget::setSelectedFace(num_sel_faces,*it);
					num_sel_faces++;
				}
			}
		}
		active->redraw();
		break;
		case SelectFaceLoop:
		if (QApplication::keyboardModifiers() != Qt::ShiftModifier){
			GLWidget::clearSelectedFaces();
		}
		septr = active->selectEdge(x,y);
		GLWidget::setSelectedEdge(num_sel_edges,septr);
		if ( septr ){
			if (QApplication::keyboardModifiers() == Qt::ControlModifier){ // deselect
				face_loop_start_edge = septr;
				getFaceLoopSelection(septr, true, NULL, false);
			}
			else { //select
				face_loop_start_edge = septr;
				getFaceLoopSelection(septr, true, NULL, true);
			}
		}
		active->redraw();
		break;
		case ExtrudeMultipleFaces :
		case MultiSelectFace :
		case SubdivideFace :
									// No duplicates allowed
		// sfptr = active->selectFace(x,y);
		// if ( !GLWidget::isSelected(sfptr) ){
		// 	GLWidget::setSelectedFace(num_sel_faces,sfptr);
		// 	num_sel_faces++;
		// }
		// else if ( GLWidget::isSelected(sfptr) && QApplication::keyboardModifiers() == Qt::ControlModifier) {
		// 	GLWidget::clearSelectedFace(sfptr);
		// 	num_sel_faces--;
		// }
		if ( QApplication::keyboardModifiers() == Qt::ControlModifier) {
			sfptrarr = active->deselectFaces(x,y);
			first = sfptrarr.begin(); last = sfptrarr.end();
			while ( first != last ){
				GLWidget::clearSelectedFace(*first);
				++first;
				num_sel_faces--;
			}
			active->redraw();
			sfptrarr.clear();
		}
		else {
			sfptrarr = active->selectFaces(x,y);
			first = sfptrarr.begin(); last = sfptrarr.end();
			while ( first != last ){
				GLWidget::setSelectedFace(num_sel_faces,*first);
				++first;
				num_sel_faces++;
			}
			active->redraw();
			sfptrarr.clear();
		}
		break;
		case SelectCheckerboard :
		if (QApplication::keyboardModifiers() != Qt::ShiftModifier){
			GLWidget::clearSelectedFaces();
		}
		//get one selected face
		sfptr = active->selectFace(x,y);
		if (sfptr && GLWidget::isSelected(sfptr) && QApplication::keyboardModifiers() == Qt::ControlModifier){
			deselect_edges = true;
			GLWidget::clearSelectedFace(sfptr);
			num_sel_faces--;
			getCheckerboardSelection(sfptr);
			deselect_edges = false;
		}
		else if (sfptr && !GLWidget::isSelected(sfptr) ){
			GLWidget::setSelectedFace(num_sel_faces,sfptr);
			num_sel_faces++;
			getCheckerboardSelection(sfptr);
		}		
		active->redraw();
		sfptrarr.clear();
		break;
		case SelectFaceVertex :
		case ReorderFace :
		case InsertEdge :
		case SpliceCorners :
		case ConnectFaceVertices :
		case BezierConnectFaces :
		case HermiteConnectFaces :
		sfptr = active->selectFace(x,y);
		GLWidget::setSelectedFace(num_sel_faces,sfptr);
		if ( sfptr )
		{
			sfvptr = active->selectFaceVertex(sfptr,x,y);
			GLWidget::setSelectedFaceVertex(num_sel_faceverts,sfvptr);
		}
		break;

		case MultiSelectFaceVertex :
		sfptr = active->selectFace(x,y);
		GLWidget::setSelectedFace(num_sel_faces,sfptr);
		if ( sfptr )
		{
			sfvptr = active->selectFaceVertex(sfptr,x,y);
			if ( !GLWidget::isSelected(sfvptr) )
				GLWidget::setSelectedFaceVertex(num_sel_faceverts,sfvptr);
		}
		break;

		case ConnectEdges :
		sfptr = active->selectFace(x,y);
		GLWidget::setSelectedFace(num_sel_faces,sfptr);
		if ( sfptr )
		{
			septr = active->selectEdge(x,y);
			GLWidget::setSelectedEdge(num_sel_edges,septr);
		}
		break;
	};	
	if ( svptr != NULL || septr != NULL || sfptr != NULL ) redraw();
}

void MainWindow::dragEnterEvent(QDragEnterEvent *event) {
	// QMessageBox::about(this,tr("mimetype"),tr("%1").arg(event->mimeData()->formats().join(",")));
	// if (event->mimeData()->hasFormat("text/uri-list"))
	// 	event->acceptProposedAction();
}

void MainWindow::dropEvent(QDropEvent *event) {
	//textBrowser->setPlainText(event->mimeData()->text());
	//mimeTypeCombo->clear();
	//mimeTypeCombo->addItems(event->mimeData()->formats());
	// QMessageBox::about(this,tr("mimetype"),tr("%1").arg(event->mimeData()->data("text/uri-list").data));

	// event->acceptProposedAction();
}

	// Handle keyboard and mouse events
void MainWindow::mousePressEvent(QMouseEvent *event) {

	if ( event->buttons() == Qt::LeftButton && mode != NormalMode ){
		doSelection(event->x(),this->size().height()-event->y());
	}
	// else if ( event->buttons() == Qt::RightButton && QApplication::keyboardModifiers() == Qt::ShiftModifier){
	// 	// event->ignore();
	// 	if (!active->isBrushVisible()) active->showBrush();
	// 	mBrushStartX = event->x();
	// 	// mBrushStartY = event->y();
	// }
	else event->ignore();
}


void MainWindow::mouseMoveEvent(QMouseEvent *event) {
	// if (active->isBrushVisible()) active->redraw();
	if ( mode != NormalMode )
		// doSelection(event->x(),this->size().height()-event->y() );
		doDrag(event->x(),this->size().height()-event->y() );
	// else if ( event->buttons() == Qt::RightButton && QApplication::keyboardModifiers() == Qt::ShiftModifier){
	// 	// event->ignore();
	// 	setBrushSize(mBrushSize+event->x()-mBrushStartX);
	// }
	else event->ignore();
}
void MainWindow::mouseReleaseEvent(QMouseEvent *event)  {
					// The mouse was dragged or released
					// Send this event to the subroutine handling the current event, if any
					// If we are not currently doing anything, nothing will happen
					// If ALT key is pressed we are mainpulating the view which is handled by the viewport
	if ( mode != NormalMode )
	{
		switch ( mode )
		{
			case EditVertex :       // brianb
			is_editing = false;
			if ( GLWidget::numSelectedVertices() >= 1 )	{
				DLFLVertexPtr vp = GLWidget::getSelectedVertex(0);
				vp->print();
				// GLWidget::clearSelectedVertices();
				// num_sel_verts = 0;
				redraw();
			}
			break;
			case SelectVertex :
			if ( GLWidget::numSelectedVertices() >= 1 )
			{
				DLFLVertexPtr vp = GLWidget::getSelectedVertex(0);
				vp->print();
				GLWidget::clearSelectedVertices();
				num_sel_verts = 0;
				redraw();
			}
			break;
			case SelectEdge :
			if ( GLWidget::numSelectedEdges() >= 1 ) {
				DLFLEdgePtr septr = GLWidget::getSelectedEdge(0);
				if (septr) {
																						//septr->print();
																						//num_sel_edges = 0;
				}
				GLWidget::clearSelectedEdges();
				redraw();
			}
			break;
			case SelectEdgeLoop:
			if ( GLWidget::numSelectedEdges() >= 1 ){
				DLFLEdgePtr septr = GLWidget::getSelectedEdge(0);
				if (septr)
					getEdgeLoopSelection(septr);
			}
			// GLWidget::clearSelectedEdges();
			active->redraw();			
			break;
			case SelectFaceLoop:
			if ( GLWidget::numSelectedEdges() >= 1 ){
				DLFLEdgePtr septr = GLWidget::getSelectedEdge(0);
				if (septr){
					if (QApplication::keyboardModifiers() == Qt::ControlModifier){ // deselect
						face_loop_start_edge = septr;
						getFaceLoopSelection(septr, true, NULL, false);
					}
					else { //select
						face_loop_start_edge = septr;
						getFaceLoopSelection(septr, true, NULL, true);
					}
				}
			}
			GLWidget::clearSelectedEdges();
			active->redraw();
			break;
			case SelectFace :
			if ( GLWidget::numSelectedFaces() >= 1 )
			{
				DLFLFacePtr fp = GLWidget::getSelectedFace(0);
				fp->print();
				GLWidget::clearSelectedFaces();
				num_sel_faces = 0;
				redraw();
			}
			break;
			case SelectSimilarFaces :
			if ( GLWidget::numSelectedFaces() >= 1 ){
				DLFLFacePtr sfptr = GLWidget::getSelectedFace(0);			
				if (sfptr){
					DLFLFacePtrArray sfptrarray;
					vector<DLFLFacePtr>::iterator it;
					DLFL::selectMatchingFaces(&object, sfptr, sfptrarray);
					for (it = sfptrarray.begin(); it != sfptrarray.end(); it++){
						if (!GLWidget::isSelected(*it)){
							GLWidget::setSelectedFace(num_sel_faces,*it);
							num_sel_faces++;
						}
					}
					redraw();
				}
			}
			break;
			case SelectFaceVertex :
			if ( GLWidget::numSelectedFaceVertices() >= 1 )
			{
				DLFLFaceVertexPtr fvp = GLWidget::getSelectedFaceVertex(0);
				fvp->print();
				GLWidget::clearSelectedFaceVertices();
				GLWidget::clearSelectedFaces();
				num_sel_faceverts = 0; num_sel_faces = 0;
				redraw();
			}
			break;
						case InsertEdge :
						if ( GLWidget::numSelectedFaceVertices() >= 2 )
						{
							DLFLFaceVertexPtr sfvptr1, sfvptr2;
							sfvptr1 = GLWidget::getSelectedFaceVertex(0);
							sfvptr2 = GLWidget::getSelectedFaceVertex(1);
							if ( sfvptr1 && sfvptr2 )
							{
								DLFLMaterialPtr mptr = sfvptr1->getFacePtr()->material();
								undoPush();
								setModified(true);
								//object.insertEdge(sfvptr1,sfvptr2,false,mptr);
								DLFL::insertEdge(&object,sfvptr1,sfvptr2,false,mptr);
								GLWidget::clearSelectedFaces();
								GLWidget::clearSelectedFaceVertices();
								num_sel_faceverts = 0; num_sel_faces = 0;
								recomputePatches();
								recomputeNormals();
								redraw();   
							}
						}
						else if ( GLWidget::numSelectedFaceVertices() == 1 )
						{
							num_sel_faceverts=1; num_sel_faces=1;
						}
						break;
						case DeleteEdge :
						if ( GLWidget::numSelectedEdges() >= 1 )
						{
							DLFLEdgePtr septr = GLWidget::getSelectedEdge(0);
							if ( septr )
							{
								undoPush();
								setModified(true);
								//object.deleteEdge(septr,MainWindow::delete_edge_cleanup);
								DLFL::deleteEdge( &object, septr, MainWindow::delete_edge_cleanup);
								recomputePatches();
								recomputeNormals();
							}
							GLWidget::clearSelectedEdges();
							redraw();
						}
						break;
						case SubdivideEdge :
						if ( GLWidget::numSelectedEdges() >= 1 )
						{
							DLFLEdgePtr septr = GLWidget::getSelectedEdge(0);
							if ( septr )
							{
								undoPush();
								setModified(true);
								//object.subdivideEdge(num_e_subdivs,septr);
								DLFL::subdivideEdge(&object, num_e_subdivs,septr);
								recomputePatches();
								recomputeNormals();
							}
							GLWidget::clearSelectedEdges();
							redraw();
						}
						break;
						case CollapseEdge :
						if ( GLWidget::numSelectedEdges() >= 1 )
						{
							DLFLEdgePtr septr = GLWidget::getSelectedEdge(0);
							if ( septr )
							{
								undoPush();
								setModified(true);
								//object.collapseEdge(septr);
								DLFL::collapseEdge(&object,septr);
								recomputePatches();
								recomputeNormals();
							}
							GLWidget::clearSelectedEdges();
							redraw();
						}
						break;
						case SpliceCorners :
						if ( GLWidget::numSelectedFaceVertices() >= 2 )
						{
							DLFLFaceVertexPtr sfvptr1, sfvptr2;
							sfvptr1 = GLWidget::getSelectedFaceVertex(0);
							sfvptr2 = GLWidget::getSelectedFaceVertex(1);
							if ( sfvptr1 && sfvptr2 )
							{
								DLFLMaterialPtr mptr = sfvptr1->getFacePtr()->material();
								undoPush();
								setModified(true);
								//object.spliceCorners(sfvptr1,sfvptr2);
								DLFL::spliceCorners(&object,sfvptr1,sfvptr2);
								GLWidget::clearSelectedFaces();
								GLWidget::clearSelectedFaceVertices();
								num_sel_faceverts = 0; num_sel_faces = 0;
								recomputePatches();
								recomputeNormals();
								redraw();   
							}
						}
						else if ( GLWidget::numSelectedFaceVertices() == 1 )
						{
							num_sel_faceverts=1; num_sel_faces=1;
						}
						break;
						case ConnectFaces :
						if ( GLWidget::numSelectedFaces() >= 2 )
						{
							DLFLFacePtr sfptr1, sfptr2;
							sfptr1 = GLWidget::getSelectedFace(0);
							sfptr2 = GLWidget::getSelectedFace(1);
							if ( sfptr1 && sfptr2 )
							{
								undoPush();
								setModified(true);
								DLFL::connectFaces(&object,sfptr1,sfptr2,num_segments);
								recomputePatches();
								recomputeNormals();
								GLWidget::clearSelectedFaces();
								redraw();   
							}
						}
						else if ( GLWidget::numSelectedFaces() == 1 )
						{
							num_sel_faces = 1;
						}
						break;
						case ConnectFaceVertices :
						if ( GLWidget::numSelectedFaceVertices() >= 2 )
						{
							DLFLFaceVertexPtr sfvptr1, sfvptr2;
							sfvptr1 = GLWidget::getSelectedFaceVertex(0);
							sfvptr2 = GLWidget::getSelectedFaceVertex(1);
							if ( sfvptr1 && sfvptr2 )
							{
								undoPush();
								setModified(true);
								DLFL::connectFaces(&object,sfvptr1,sfvptr2,num_segments, max_segments);
								GLWidget::clearSelectedFaces();
								GLWidget::clearSelectedFaceVertices();
								num_sel_faceverts = 0; num_sel_faces = 0;
								recomputePatches();
								recomputeNormals();
								redraw();   
							}
						}
						else if ( GLWidget::numSelectedFaceVertices() == 1 )
						{
							num_sel_faceverts = 1; num_sel_faces = 1;
						}
						break;
						case ConnectEdges :
						if ( GLWidget::numSelectedEdges() >= 2 )
						{
							DLFLEdgePtr septr1, septr2;
							DLFLFacePtr sfptr1, sfptr2;
							septr1 = GLWidget::getSelectedEdge(0);
							septr2 = GLWidget::getSelectedEdge(1);
							sfptr1 = GLWidget::getSelectedFace(0);
							sfptr2 = GLWidget::getSelectedFace(1);
							if ( septr1 && septr2 )
							{
								undoPush();
								setModified(true);
								DLFL::connectEdges(&object,septr1,sfptr1,septr2,sfptr2);
								GLWidget::clearSelectedEdges();
								GLWidget::clearSelectedFaces();
								num_sel_edges = 0; num_sel_faces = 0;
								redraw();   
							}
						}
						else if ( GLWidget::numSelectedEdges() == 1 )
						{
							num_sel_edges = 1; num_sel_faces = 1;
						}
						break;
						case ExtrudeFace :
						if ( GLWidget::numSelectedFaces() >= 1 )
						{
							DLFLFacePtr sfptr = GLWidget::getSelectedFace(0);
							if ( sfptr )
							{
								undoPush();
								setModified(true);
								DLFL::extrudeFace(&object,sfptr,extrude_dist,num_extrusions,extrude_rot,extrude_scale);
								recomputePatches();
								recomputeNormals();
							}
							GLWidget::clearSelectedFaces();
							redraw();
						}
						break;
						case ExtrudeMultipleFaces :
						if ( GLWidget::numSelectedFaces() >= 1 )
						{
							DLFLFacePtrArray sfptrarr = GLWidget::getSelectedFaces();
							if ( sfptrarr[0] )
							{
								undoPush();
								setModified(true);
								vector<DLFLFacePtr>::iterator it;
								for(it = sfptrarr.begin(); it != sfptrarr.end(); it++) {
									DLFL::extrudeFace(&object,*it,extrude_dist,num_extrusions,extrude_rot,extrude_scale);
									recomputePatches();
									recomputeNormals();						
								}
							}
							GLWidget::clearSelectedFaces();
							redraw();
						}
						break;
						case ExtrudeFaceDS :
						if ( GLWidget::numSelectedFaces() >= 1 )
						{
							DLFLFacePtr sfptr = GLWidget::getSelectedFace(0);
							if ( sfptr )
							{
								undoPush();
								setModified(true);
								DLFL::extrudeFaceDS(&object,sfptr,extrude_dist,num_extrusions,
									ds_ex_twist,extrude_scale);
								recomputePatches();
								recomputeNormals();
							}
							GLWidget::clearSelectedFaces();
							redraw();
						}
						break;
						case ExtrudeDualFace :
						if ( GLWidget::numSelectedFaces() >= 1 )
						{
							DLFLFacePtr sfptr = GLWidget::getSelectedFace(0);
							if ( sfptr )
							{
								undoPush();
								setModified(true);
								DLFL::extrudeDualFace(&object,sfptr,extrude_dist,num_extrusions,
									extrude_rot,extrude_scale,
									dual_mesh_edges_check);
								recomputePatches();
								recomputeNormals();
							}
							GLWidget::clearSelectedFaces();
							redraw();
						}
						break;
						case ExtrudeFaceDodeca :
						if ( GLWidget::numSelectedFaces() >= 1 )
						{
							DLFLFacePtr sfptr = GLWidget::getSelectedFace(0);
							if ( sfptr )
							{
								undoPush();
								setModified(true);
								DLFL::extrudeFaceDodeca(&object,sfptr,extrude_dist,num_extrusions,
									ds_ex_twist,extrude_scale,
									hexagonalize_dodeca_extrude);
								recomputePatches();
								recomputeNormals();
							}
							GLWidget::clearSelectedFaces();
							redraw();
						}
						break;
						case ExtrudeFaceIcosa :
						if ( GLWidget::numSelectedFaces() >= 1 )
						{
							DLFLFacePtr sfptr = GLWidget::getSelectedFace(0);
							if ( sfptr )
							{
								undoPush();
								setModified(true);
								DLFL::extrudeFaceIcosa(&object,sfptr,extrude_dist,num_extrusions,
									ds_ex_twist,extrude_scale);
								recomputePatches();
								recomputeNormals();
							}
							GLWidget::clearSelectedFaces();
							redraw();
						}
						break;
						case StellateFace :
						if ( GLWidget::numSelectedFaces() >= 1 )
						{
							DLFLFacePtr sfptr = GLWidget::getSelectedFace(0);
							if ( sfptr )
							{
								undoPush();
								setModified(true);
								DLFL::stellateFace(&object,sfptr,extrude_dist);
								recomputePatches();
								recomputeNormals();
							}
							GLWidget::clearSelectedFaces();
							redraw();
						}
						break;
						case DoubleStellateFace :
						if ( GLWidget::numSelectedFaces() >= 1 )
						{
							DLFLFacePtr sfptr = GLWidget::getSelectedFace(0);
							if ( sfptr )
							{
								undoPush();
								setModified(true);
								DLFL::doubleStellateFace(&object,sfptr,extrude_dist);
								recomputePatches();
								recomputeNormals();
							}
							GLWidget::clearSelectedFaces();
							redraw();
						}
						break;
						case CrustModeling :
						if ( GLWidget::numSelectedFaces() >= 1 )
						{
							DLFLFacePtr sfptr = GLWidget::getSelectedFace(0);
							if ( sfptr )
							{
																													// No undo for hole punching in crust modeling mode
																													// because the ids in the recreated object
																													// will be different
								if ( QApplication::keyboardModifiers() == Qt::ShiftModifier )
								{
									DLFL::tagMatchingFaces(&object,sfptr);
									DLFL::punchHoles(&object);
									recomputePatches();
									recomputeNormals();
								}
								else
									DLFL::cmMakeHole(&object,sfptr,crust_cleanup);
			//                                                  recomputeNormals();
							}
							GLWidget::clearSelectedFaces();
							redraw();
						}
						break;
						case BezierConnectFaces :
						if ( GLWidget::numSelectedFaceVertices() >= 2 )
						{
							DLFLFaceVertexPtr sfvptr1, sfvptr2;
							sfvptr1 = GLWidget::getSelectedFaceVertex(0);
							sfvptr2 = GLWidget::getSelectedFaceVertex(1);
							if ( sfvptr1 && sfvptr2 )
							{
								undoPush();
								setModified(true);
								DLFL::bezierConnectFaces(&object,sfvptr1,sfvptr2,
									num_segments,nwt1,nwt2);
								GLWidget::clearSelectedFaces();
								GLWidget::clearSelectedFaceVertices();
								num_sel_faceverts = 0; num_sel_faces = 0;
								recomputePatches();
								recomputeNormals();
								redraw();   
							}
						}
						else if ( GLWidget::numSelectedFaceVertices() == 1 )
						{
							num_sel_faceverts = 1; num_sel_faces = 1;
						}
						break;
						case HermiteConnectFaces :
						if ( GLWidget::numSelectedFaceVertices() >= 2 )
						{
							DLFLFaceVertexPtr sfvptr1, sfvptr2;
							sfvptr1 = GLWidget::getSelectedFaceVertex(0);
							sfvptr2 = GLWidget::getSelectedFaceVertex(1);
							if ( sfvptr1 && sfvptr2 )
							{
								undoPush();
								setModified(true);
								if ( symmetric_weights )
									DLFL::hermiteConnectFaces(&object,sfvptr1,sfvptr2,
									num_segments,nwt1,nwt1,
									max_segments,num_extra_twists);
								else
									DLFL::hermiteConnectFaces(&object,sfvptr1,sfvptr2,
									num_segments,nwt1,nwt2,
									max_segments,num_extra_twists);
								GLWidget::clearSelectedFaces();
								GLWidget::clearSelectedFaceVertices();
								num_sel_faceverts = 0; num_sel_faces = 0;
								recomputePatches();
								recomputeNormals();
								redraw();
							}
						}
						else if ( GLWidget::numSelectedFaceVertices() == 1 )
						{
							num_sel_faceverts = 1; num_sel_faces = 1;
						}
						break;
						case ReorderFace :
						if ( GLWidget::numSelectedFaceVertices() >= 1 )
						{
							DLFLFaceVertexPtr sfvptr = GLWidget::getSelectedFaceVertex(0);
							if ( sfvptr ) sfvptr->getFacePtr()->reorder(sfvptr);
							redraw();
						}
						break;
						case SubdivideFace :
						case MultiSelectFace :
						// num_sel_faces++;
						break;
			//from ozgur
						case CutEdge :
						if ( GLWidget::numSelectedEdges() >= 1 )
						{
							DLFLEdgePtr septr = GLWidget::getSelectedEdge(0);
							if ( septr )
							{
								undoPush();
								septr->ismarked = 1- septr->ismarked;
																					// DLFL::cutEdge( &object, septr, peelDistance_factor,pnormalBendS_factor,pnormalBendT_factor, peel_all_edges);
							}
							GLWidget::clearSelectedEdges();
							redraw();
						}
						break;
						case TruncateEdge :
						if ( GLWidget::numSelectedEdges() >= 1 )
						{
							DLFLEdgePtr septr = GLWidget::getSelectedEdge(0);
							if ( septr )
							{
								undoPush();
								septr->ismarked = 1- septr->ismarked;
																					// DLFL::cutEdge( &object, septr, peelDistance_factor,pnormalBendS_factor,pnormalBendT_factor, peel_all_edges);
							}
							GLWidget::clearSelectedEdges();
							redraw();
						}
						break;
						case CutVertex :
						if ( GLWidget::numSelectedVertices() >= 1 )
						{
							DLFLVertexPtr svptr = GLWidget::getSelectedVertex(0);
							if ( svptr )
							{
								undoPush();
								svptr->ismarked = 1 - svptr->ismarked;
							}
							// GLWidget::clearSelectedVertices();
							redraw();
						}
						break;

						case CutEdgeandVertex :
						if ( GLWidget::numSelectedEdges() >= 1 )
						{	
							if ( GLWidget::numSelectedEdges() >= 1 ) {
								DLFLVertexPtr svptr = GLWidget::getSelectedVertex(0);
								DLFLEdgePtr septr = GLWidget::getSelectedEdge(0);
								if ( septr ) {
									if (svptr) {
										undoPush();
										septr->ismarked = 1- septr->ismarked;
									// DLFL::cutEdge( &object, septr, peelDistance_factor,pnormalBendS_factor,pnormalBendT_factor, peel_all_edges);
									}
								}
								GLWidget::clearSelectedEdges();
								GLWidget::clearSelectedVertices();
								redraw();
							}
						}
						break;
						case MarkEdge:
						if ( GLWidget::numSelectedEdges() >= 1 )
						{
							DLFLEdgePtr septr = GLWidget::getSelectedEdge(0);
							if ( septr )
							{
								undoPush();
								septr->ismarked = 1- septr->ismarked;
							}
							GLWidget::clearSelectedEdges();
							redraw();
						}
						break;

						case MarkVertex :
						if ( GLWidget::numSelectedVertices() >= 1 )
						{
							DLFLVertexPtr svptr = GLWidget::getSelectedVertex(0);
							if ( svptr )
							{
								undoPush();
								svptr->ismarked = 1 - svptr->ismarked;
							}
							GLWidget::clearSelectedVertices();
							redraw();
						}
						break;

						case CutFace :
						if ( GLWidget::numSelectedFaces() >= 1 )
						{
							DLFLFacePtr sfptr = GLWidget::getSelectedFace(0);
							if ( sfptr )
							{
								undoPush();
								sfptr->ismarked = 1 - sfptr->ismarked;
							}
							GLWidget::clearSelectedFaces();
							redraw();
						}
						break;
					}//end switch (mode)

				}//end if (mode != NormalMode)
				// else if ( event->buttons() == Qt::RightButton && QApplication::keyboardModifiers() == Qt::ShiftModifier){
				// 	// event->ignore();
				// 	mBrushStartX = 0;
				// 	// mBrushStartY = 0;
				// }
				else event->ignore();
}//end function mousereleaseevent


void MainWindow::testConvexHull(void) {
		// For testing convex hull subroutine
	Vector3dArray vertices;
	vertices.resize(8);
	vertices[0].set(0,0,0);
	vertices[1].set(0,10,0);
	vertices[2].set(10,10,0);
	vertices[3].set(10,0,0);
	vertices[4].set(0,0,10);
	vertices[5].set(0,10,10);
	vertices[6].set(10,10,10);
	vertices[7].set(10,0,10);
	DLFLConvexHull convexhull;
	convexhull.createHull(vertices);

	ofstream file;
	file.open("convexhull.dlfl");
	convexhull.writeDLFL(file);
	file.close();
}

void MainWindow::performRemeshing(void) {
	switch ( remeshingscheme )
	{
		case Dual :
		createDual();
		break;
		case Root3 :
		subdivideRoot3();
		break;
		case DualVertexTrunc :
		subdivideSqrt3();
		break;
		case GlobalStellate :
		globalStellate();
		break;
		case Star :
		subdivideStar();
		break;
		case Generic1264 :
		subdivide1264();
		break;
		case Honeycomb :
		subdivideHoneycomb();
		break;
		case VertexTrunc :
		subdivideVertexCutting();
		break;
		case DualGeneric1264 :
		subdivideDual1264();
		break;
		case LinearVertexInsertion :
		subdivideLinearVertexInsertion();
		break;
		case CatmullClark :
		subdivideCatmullClark();
		break;
		case ModifiedStellate :
		subStellate1();
		break;
		case DooSabin :
		subdivideDooSabin();
		break;
		case CornerCutting :
		subdivideCornerCutting();
		break;
		case Simplest :
		subdivideSimplest();
		break;
		case Pentagonal :
		subdividePentagonal();
		break;
		case CubicPentagonal :
		subdivideCubicPentagonal();
		break;
		case DualPentagonal :
		subdivideDualPentagonal();
		break;
		case LoopStyle :
		subdivideLoopStyle();
		break;
		case Loop :
		subdivideLoop();
		break;
		case Root4 :
		subdivideRoot4();
		break;
		case DualLoop :
		subdivideDualLoop();
		break;
		case GlobalCubicExtrude :
		cout << "NOT YET IMPLEMENTED!!!" << endl;
		break;
		case CheckerBoard :
		subdivideCheckerBoard();
		break;
		case DualGlobalCubicExtrude :
		cout << "NOT YET IMPLEMENTED!!!" << endl;
		break;
		case DualCheckerBoard :
		subdivideDualCheckerBoard();
		break;
		case PentagonPreserving :
		subdividePentagonPreserving();
		break;
		case DualPentagonPreserving :
		subdivideDualPentagonPreserving();
		break;
		case HexagonPreserving :
		subdivideRoot4();
		break;
		case DualHexagonPreserving :
		subdivideDualHexagonPreserving();
		break;
		case Fractal :
		subdivideFractal();
		break;
		case ModifiedDoubleStellate :
		subStellate2();
		break;
		case Dome :
		subdivideDome();
		break;
		case DooSabinBC :
		subdivideDooSabinBC();
		break;
		case DooSabinBCNew :
		subdivideDooSabinBCNew();
		break;
	}
	setMode(mode);
	redraw();
}

			// Change the renderer for all viewports
void MainWindow::setRenderer(DLFLRendererPtr rp) {
	active->setRenderer(rp);
}

// // Override show() method to show subwindows also
// void MainWindow::show(void) {
// 	active->show();
// }

// Return pointer to the active GLWidget
GLWidget *MainWindow::getActive() {
	return active;
}

// Override redraw() method to mark subwindows also for redraw
void MainWindow::redraw() {
	active->redraw();
}

//--- Methods to perform various operations ---//

// Switch to specified operating mode
void MainWindow::setMode(Mode m) {
	mode = m;
	switch ( mode )	{
		// case BezierMode: // brianb
		//       object.bezierDefaults();
		//       break;
		case EditVertex :    // brianb
		case SelectVertex :
		MainWindow::num_sel_verts = 0;
		break;
		case SelectEdge :
		case SelectEdgeLoop :
		MainWindow::num_sel_edges = 0;
		break;
		case SelectFace :
		case SelectFaceLoop :
		MainWindow::num_sel_faces = 0;
		break;
		case SelectFaceVertex :
		MainWindow::num_sel_faceverts = 0;
		break;
		case NormalMode:
		default :
									// Nothing to be done for other modes except clearing selection lists
		MainWindow::clearSelected();
		break;
	}
	if (mode != MultiSelectFace)
		active->hideBrush();
	else active->showBrush();
}

void MainWindow::setSelectionMask(SelectionMask m){
	selectionmask = m;
	switch(selectionmask){
		case MaskVertices :
		break;
		case MaskEdges :
		break;
		case MaskFaces :
		break;
		case MaskFaceVertices :
		break;
		default:
		break;
	}
}

void MainWindow::setRemeshingScheme(RemeshingScheme scheme) {
	remeshingscheme = scheme;
}


	// Read the DLFL object from a file
void MainWindow::readObject(const char * filename) {
	active->clearSelected();
	ifstream file;
	file.open(filename);
	if ( strstr(filename,".dlfl") || strstr(filename,".DLFL") )
		object.readDLFL(file);
	else if ( strstr(filename,".obj") || strstr(filename,".OBJ") )
		object.readObject(file);
	file.close();
}

	// Read the DLFL object from a file
void MainWindow::readObjectQFile(QString filename) {
	active->clearSelected();
	QFile file(filename);
	file.open(QIODevice::ReadOnly);

	QByteArray ba = file.readAll();
	const char *filecontents = ba.data();
	string str(filecontents);
	istringstream filestring(str);

	if ( filename.indexOf(".dlfl") == filename.length()-4 || filename.indexOf(".dlfl") == filename.length()-4 )
		object.readDLFL(filestring);
	else if ( filename.indexOf(".OBJ") == filename.length()-4 || filename.indexOf(".obj") == filename.length()-4 )
		object.readObject(filestring);
	file.close();

#ifdef WITH_PYTHON
	DLFLObjectPtr obj = &object;
	if( obj )
		emit loadedObject(obj,filename);
#endif
}

	// Read the DLFL object from a file - use alternate OBJ reader for OBJ files
void MainWindow::readObjectAlt(const char * filename) {
	active->clearSelected();
	ifstream file;
	file.open(filename);
	if ( strstr(filename,".dlfl") || strstr(filename,".DLFL") )
		object.readDLFL(file);
	else if ( strstr(filename,".obj") || strstr(filename,".OBJ") )
		object.readObjectAlt(file);
	file.close();
}

void MainWindow::readObjectOBJ(const char * filename) {
	active->clearSelected();
	ifstream file;
	file.open(filename);
	object.readObject(file);
	file.close();
}

void MainWindow::readObjectDLFL(const char * filename) {
	active->clearSelected();
	ifstream file;
	file.open(filename);
	object.readDLFL(file);
	file.close();
}

	// Write the DLFL object to a file
void MainWindow::writeObject(const char * filename, bool with_normals, bool with_tex_coords) {
	ofstream file;
	file.open(filename);
	if ( strstr(filename,".dlfl") || strstr(filename,".DLFL") )
		object.writeDLFL(file);
	else if ( strstr(filename,".obj") || strstr(filename,".OBJ") )
		object.writeObject(file,with_normals,with_tex_coords);
	file.close();
}

void MainWindow::writeObjectOBJ(const char * filename, bool with_normals, bool with_tex_coords) {
	ofstream file;
	file.open(filename);
	// object.writeObject(file,with_normals,with_tex_coords);
	file.close();
}

/* stuart - bezier export */
void MainWindow::writePatchOBJ( const char *filename ) {
	ofstream file;
	file.open(filename);
	// object.objPatchWrite(file);
}

void MainWindow::writeObjectDLFL(const char * filename) {
	ofstream file;
	file.open(filename);
	object.writeDLFL(file);
	file.close();
}

// File handling
void MainWindow::openFile(void) {
	QString fileName = QFileDialog::getOpenFileName(this,
		tr("Open File..."),
		"$HOME",
		tr("All Supported Files (*.obj *.dlfl);;Wavefront Files (*.obj);;DLFL Files (*.dlfl);;All Files (*)"));
	if (!fileName.isEmpty()){
		if (!curFile.isEmpty()){
			undoPush();
			setModified(false);
		}
		QByteArray ba = fileName.toLatin1();
		const char *filename = ba.data();
		mWasPrimitive = false;
		mIsPrimitive = false;
		readObject(filename);
#ifdef WITH_PYTHON
		// Emit and send to python script editor
		DLFLObjectPtr obj = &object;
		if( obj )
			emit loadedObject(obj,fileName);
#endif
		recomputePatches();
		recomputeNormals();
		setCurrentFile(fileName);
		active->redraw();
	}
}


bool MainWindow::saveFile(bool with_normals, bool with_tex_coords) {
	if (!curFile.isEmpty() && !mIsPrimitive){
		QByteArray ba = curFile.toLatin1();
		const char *filename = ba.data();
		writeObject(filename,with_normals,with_tex_coords);
		return true;
	}
	else {
		QString fileName = QFileDialog::getSaveFileName(this,
			tr("Save File As..."),
			curFile,
			tr("All Supported Files (*.obj *.dlfl);;Wavefront Files (*.obj);;DLFL Files (*.dlfl);;All Files (*)"));
		if (!fileName.isEmpty()){
			if (!fileName.indexOf(".obj") || !fileName.indexOf(".dlfl") || !fileName.indexOf(".OBJ") || !fileName.indexOf(".DLFL"))
				fileName.append(".dlfl");
			setCurrentFile(fileName);	
			QByteArray ba = fileName.toLatin1();
			const char *filename = ba.data();
			writeObject(filename,with_normals,with_tex_coords);
			return true;
		}
	}
	return false;
}

bool MainWindow::saveFileAs(bool with_normals, bool with_tex_coords) {
	QString fileName = QFileDialog::getSaveFileName(this,
		tr("Save File As..."),
		curFile,
		tr("All Supported Files (*.obj *.dlfl);;Wavefront Files (*.obj);;DLFL Files (*.dlfl);;All Files (*)"));
	if (!fileName.isEmpty()){
		if (!fileName.indexOf(".obj") || !fileName.indexOf(".dlfl") || !fileName.indexOf(".OBJ") || !fileName.indexOf(".DLFL"))
			fileName.append(".dlfl");
		setCurrentFile(fileName);				
		QByteArray ba = fileName.toLatin1();
		const char *filename = ba.data();
		writeObject(filename,with_normals,with_tex_coords);
		return true;
	}
	return false;
}

void MainWindow::openFileOBJ(void) {	
	// QString fileName = QFileDialog::getOpenFileName(this,
	// 																								tr("Open File..."),
	// 																								"$HOME",
	// 																								tr("Wavefront Files (*.obj);;All Files (*)"));
	// if (!fileName.isEmpty()) {
	// 	
	// 	setCurrentFile(fileName);
	// 	undoPush();
	// 	setModified(true);
	// 	QByteArray ba = fileName.toLatin1();
	// 	const char *filename = ba.data();
	// 	readObjectOBJ(filename);
	// 	recomputePatches();
	// 	recomputeNormals();
	// }
}

void MainWindow::saveFileOBJ(bool with_normals, bool with_tex_coords) {
	// QString fileName = QFileDialog::getSaveFileName(this,
	// 																								tr("Save File As..."),
	// 																								curFile,
	// 																								tr("Wavefront Files (*.obj);;All Files (*)"));
	// if (!fileName.isEmpty()){
	// 	QByteArray ba = fileName.toLatin1();
	// 	const char *filename = ba.data();
	// 	writeObjectOBJ(filename,with_normals,with_tex_coords);
	// }
}

/* stuart - bezier export */
bool MainWindow::saveFileBezierOBJ( ) {
	QString fileName = QFileDialog::getSaveFileName(this,
		tr("Save Bezier Patch (OBJ)..."),
		curFile,
		tr("Wavefront Files (*.obj);;All Files (*)"));
	if (!fileName.isEmpty()){
		QByteArray ba = fileName.toLatin1();
		const char *filename = ba.data();
		writePatchOBJ(filename);
		return true;
	}
	return false;
}

void MainWindow::openFileDLFL(void) {
	// QString fileName = QFileDialog::getOpenFileName(this,
	// 																								tr("Open File..."),
	// 																								"$HOME",
	// 																								tr("DLFL Files (*.dlfl);;All Files (*)"));
	// if (!fileName.isEmpty()){
	// 	setCurrentFile(fileName);
	// 	undoPush();
	// 	setModified(true);
	// 	QByteArray ba = fileName.toLatin1();
	// 	const char *filename = ba.data();
	// 	readObjectDLFL(filename);
	// 	recomputePatches();
	// 	recomputeNormals();
	// }
}

void MainWindow::saveFileDLFL(void) {

	// QString fileName = QFileDialog::getSaveFileName(this,
	// 																								tr("Save File As..."),
	// 																								curFile,
	// 																								tr("DLFL Files (*.dlfl);;All Files (*)"));
	// if (!fileName.isEmpty()){
	// 	QByteArray ba = fileName.toLatin1();
	// 	const char *filename = ba.data();
	// 	writeObjectDLFL(filename);
	// }
}

void MainWindow::setCurrentFile(QString fileName) {

	curFile = fileName;
	QString shownName;
	if (curFile.isEmpty())
		shownName = "untitled.obj";
	else
		shownName = QFileInfo(curFile).fileName();

	setWindowTitle( tr("%1[*] - %2").arg(shownName).arg(tr("TopMod")));
	setWindowModified(false);
}

// void MainWindow::cleanupForExit(void) // Do memory cleanup if any before exit {
// // Nothing to be done
// }

void MainWindow::loadCube(){	
	if (isModified())
		undoPush();
	setModified(false);
	mIsPrimitive = true;
	mWasPrimitive = true;
	setCurrentFile(tr("cube.obj"));
	readObjectQFile(":/cube.obj");
	recomputePatches();
	recomputeNormals();
	active->redraw();
}

void MainWindow::loadOctahedron(){	
	if (isModified())
		undoPush();
	setModified(false);
	mIsPrimitive = true;
	mWasPrimitive = true;
	setCurrentFile(tr("octahedron.obj"));
	readObjectQFile(":/octahedron.obj");
	recomputePatches();
	recomputeNormals();
	active->redraw();
}

void MainWindow::loadTetrahedron(){	
	if (isModified())
		undoPush();
	setModified(false);
	mIsPrimitive = true;
	mWasPrimitive = true;
	setCurrentFile(tr("tetrahedron.obj"));
	readObjectQFile(":/tetrahedron.obj");
	recomputePatches();
	recomputeNormals();
	active->redraw();
}

void MainWindow::loadDodecahedron(){	
	if (isModified())
		undoPush();
	setModified(false);
	mIsPrimitive = true;
	mWasPrimitive = true;
	setCurrentFile(tr("dodecahedron.obj"));
	readObjectQFile(":/dodecahedron.obj");
	recomputePatches();
	recomputeNormals();
	active->redraw();
}

void MainWindow::loadIcosahedron(){	
	if (isModified())
		undoPush();
	setModified(false);	
	mIsPrimitive = true;
	mWasPrimitive = true;
	setCurrentFile(tr("icosahedron.obj"));
	readObjectQFile(":/icosahedron.obj");
	recomputePatches();
	recomputeNormals();
	active->redraw();
}

void MainWindow::loadSoccerball(){	
	if (isModified())
		undoPush();
	setModified(false);
	mIsPrimitive = true;
	mWasPrimitive = true;
	setCurrentFile(tr("soccerball.obj"));
	readObjectQFile(":/soccerball.obj");
	recomputePatches();
	recomputeNormals();
	active->redraw();
}

void MainWindow::loadGeodesic(){	
	if (isModified())
		undoPush();
	setModified(false);
	mIsPrimitive = true;
	mWasPrimitive = true;
	setCurrentFile(tr("geodesic.obj"));
	readObjectQFile(":/geodesic.obj");
	recomputePatches();
	recomputeNormals();
	active->redraw();
}

bool MainWindow::isModified(){
	return mIsModified;
}

void MainWindow::setModified(bool isModified){
	mIsModified = isModified;
	setWindowModified(mIsModified);	
}

void MainWindow::switchPerspView(){
	active->switchTo(VPPersp);
	active->redraw();
}

void MainWindow::switchTopView(){
	active->switchTo(VPTop);
	active->redraw();
}

void MainWindow::switchBottomView(){
	active->switchTo(VPBottom);
	active->redraw();
}

void MainWindow::switchRightView(){
	active->switchTo(VPRight);
	active->redraw();
}

void MainWindow::switchLeftView(){
	active->switchTo(VPLeft);
	active->redraw();
}

void MainWindow::switchFrontView(){
	active->switchTo(VPFront);
	active->redraw();
}

void MainWindow::switchBackView(){
	active->switchTo(VPBack);
	active->redraw();
}

//recurse through selected faces to get a list of checkerboard selection for the entire object
void MainWindow::getCheckerboardSelection(DLFLFacePtr fptr) {
	// fparray.clear();
	// sfptr = active->selectFace(x,y);
	if (fptr){
		int numShared = 0;
		DLFLFacePtrArray fparray;
		vector<DLFLFacePtr>::iterator it;
		fptr->getNeighboringFaces(fparray);
		for ( it = fparray.begin(); it != fparray.end(); it++){
			if (fptr->sharesOneVertex((*it)) && !GLWidget::isSelected(*it)){
				numShared++;
				if (deselect_edges){
					GLWidget::clearSelectedFace(*it);
					num_sel_faces--;
				}
				else {
					GLWidget::setSelectedFace(num_sel_faces,*it);
					num_sel_faces++;					
				}
				getCheckerboardSelection((*it));
			}
		}//end for loop
		if (numShared == 0) return; //break out of recursive loop is there are no one vertex sharing faces
	}		
}

//recurse through selected edge to get a list of 
void MainWindow::getEdgeLoopSelection(DLFLEdgePtr eptr) {
	if (eptr){
		//first check to see if edge only connects to only three other edges
		DLFLVertexPtr vp1, vp2;
		DLFLEdgePtrArray eparray;
		vector<DLFLEdgePtr>::iterator it;
		eptr->getVertexPointers(vp1,vp2);
		if (vp1->valence() == 4 && vp2->valence() == 4){
			//get edges conected to vertex 1
			eparray.clear();
			vp1->getEdges(eparray);
			//loop through them to find the one that doesn't share a face
			for (it = eparray.begin(); it != eparray.end(); it++){
				if ( !coFacial(*it,eptr) ){
					if (deselect_edges){
						GLWidget::clearSelectedEdge(*it);
						num_sel_edges--;
						getEdgeLoopSelection(*it);
					}
					else if (!GLWidget::isSelected(*it)){
						GLWidget::setSelectedEdge(num_sel_edges,*it);
						num_sel_edges++;
						getEdgeLoopSelection(*it);
					}
				}
			}//end for loop
			//get edges conected to vertex 2
			eparray.clear();
			vp2->getEdges(eparray);
			//loop through them to find the one that doesn't share a face
			for (it = eparray.begin(); it != eparray.end(); it++){
				if ( !coFacial(*it,eptr) ){
					if (deselect_edges){
						GLWidget::clearSelectedEdge(*it);
						num_sel_edges--;
						getEdgeLoopSelection(*it);
					}
					else if (!GLWidget::isSelected(*it)){
						GLWidget::setSelectedEdge(num_sel_edges,*it);
						num_sel_edges++;
						getEdgeLoopSelection(*it);
					}
				}
			}//end for loop
			return;
		}//end if vp->valence
		return;
	}	
}

//recurse through selected edge to get a list of 
void MainWindow::getFaceLoopSelection(DLFLEdgePtr eptr, bool start, DLFLFacePtr face_loop_marker, bool select_face_loop) {
	if ( (eptr == face_loop_start_edge) && !start) 
		return;
		
	int idx = 0;
	DLFLEdgePtrArray edges;
	vector<DLFLEdgePtr>::iterator it;
	DLFLFacePtr fptr1, fptr2;
	//get the two faces corresponding to this edge ptr
	eptr->getFacePointers(fptr1,fptr2);
	//check if the two faces are quads
	if (fptr1 && fptr1->numFaceVertexes() == 4 && !(fptr1 == face_loop_marker) ){
		// cout << select_face_loop << "\t" << !GLWidget::isSelected(fptr1) << std::endl;
			if (/*select_face_loop &&*/ !GLWidget::isSelected(fptr1)){
					GLWidget::setSelectedFace(num_sel_faces,fptr1);
					num_sel_faces++;
			}
			/*else if (GLWidget::isSelected(fptr1)){
				GLWidget::clearSelectedFace(fptr1);
				num_sel_faces--;
			}*/
			fptr1->getEdges(edges);
			idx =0;
			for (it = edges.begin(); it != edges.end(); it++){
				if (*it == eptr){
					getFaceLoopSelection(edges[(idx+2)%4],false,fptr1, select_face_loop);
				}
				idx++;
			}//end for loop
	}
	if (fptr2 && fptr2->numFaceVertexes() == 4 && !(fptr2 == face_loop_marker) ){
			if (/*select_face_loop &&*/!GLWidget::isSelected(fptr2)){
				GLWidget::setSelectedFace(num_sel_faces,fptr2);
				num_sel_faces++;
			}
			/*else if (GLWidget::isSelected(fptr2)){
				GLWidget::clearSelectedFace(fptr2);
				num_sel_faces--;				
			}*/
			fptr2->getEdges(edges);
			idx =0;
			for (it = edges.begin(); it != edges.end(); it++){
				if (*it == eptr){
					getFaceLoopSelection(edges[(idx+2)%4], false, fptr2, select_face_loop);
					// return;
				}
				idx++;
			}//end for loop
		// }//end if fptr2	
	}
}
