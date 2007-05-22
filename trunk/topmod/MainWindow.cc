/****************************************************************************
 **
 **
 ****************************************************************************/

#include <QtGui>
#include <QtOpenGL>
#include "GLWidget.hh"
#include "MainWindow.hh"

int MainWindow::MIN_W = 1200;                        // Minimum width for app window
int MainWindow::MIN_H = 630;                        // Minimum height for app window

ShadedRendererPtr MainWindow::shaded;                // ShadedRenderer
NormalRendererPtr MainWindow::normal;                // NormalRenderer
LitRendererPtr MainWindow::lit;                        // LitRenderer
TexturedRendererPtr MainWindow::textured;          // TexturedRenderer
TexturedLitRendererPtr MainWindow::texturedlit; // TexturedLitRenderer
PatchRendererPtr MainWindow::patch;		       // PatchRenderer

//DLFLWindowPtr MainWindow::mainWindowPtr;            // Pointer to the main window

MainWindow::MainWindow(char *filename) {
	
	// setUnifiedTitleAndToolBarOnMac(false);
	mStatusBar = new QStatusBar();
	setStatusBar(mStatusBar);
	// setDockOptions(QMainWindow::AllowNestedDocks);
	setAttribute(Qt::WA_AcceptDrops, true);
	// setAttribute(Qt::WA_MacMetalStyle, true);
	setWindowFlags(Qt::Window);
	setWindowTitle(tr("newfile[*] - TopMod"));
	
  cWidget = new QWidget( );
	
	//create a container widget to hold multiple glwidgets
	mainWindow = new DLFLWindow(0,0,1000,700,MainWindow::lit,this);
	//accept file drop events
	mainWindow->setAcceptDrops(true);
	
  /** Setup Layouts **/
  layout = new QBoxLayout( QBoxLayout::TopToBottom, 0 );

  layout->addWidget( mainWindow , 0 );
	layout->setMargin(0);

  cWidget->setLayout( layout );
  setCentralWidget( cWidget );

	
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
	#endif
	
	#ifdef WITH_VERSE
		//verse script box
		mVerseDialog = new VerseTopMod(this );
		mVerseDialogDockWidget = new QDockWidget(tr("Verse-TopMod"), this);
		mVerseDialogDockWidget->setAllowedAreas(Qt::BottomDockWidgetArea | Qt::TopDockWidgetArea);
		mVerseDialogDockWidget->setWidget(mVerseDialog);
		addDockWidget(Qt::BottomDockWidgetArea, mVerseDialogDockWidget);
		mVerseDialogDockWidget->hide();
		mVerseDialogDockWidget->setMaximumHeight(200);
	#endif
	
  //make a new instance of QShortcutManager
  sm = new QShortcutManager();

	mBasicsMode = new BasicsMode(this, sm);
	mExtrusionMode = new ExtrusionMode(this, sm);
	mConicalMode = new ConicalMode(this, sm);
	mRemeshingMode = new RemeshingMode(this, sm);
	mHighgenusMode = new HighgenusMode(this, sm);
	mTexturingMode = new TexturingMode(this, sm);

  createActions();
  createToolBars();
  createMenus();

	MainWindow::createRenderers();
  mainWindow->setRenderer(lit); // Default renderer is LightedRenderer
	
	//style sheet editor
  mStyleSheetEditor = new StyleSheetEditor(this);

	readSettings();
	// if (filename){
// 		mainWindow->openFile(QString(filename));
// 	}
	
}

void MainWindow::setMode(int m){
	
	mainWindow->setMode((DLFLWindow::Mode)m);
}

void MainWindow::setRemeshingScheme(int m){
	mainWindow->setRemeshingScheme((DLFLWindow::RemeshingScheme)m);
}

void MainWindow::createActions() {
		
	//File Menu Actions
	mOpenAct = new QAction(QIcon(":images/open.png"),tr("&Open..."), this);
	sm->registerAction(mOpenAct, "File Menu", "CTRL+O");
	mOpenAct->setStatusTip(tr("Open an existing file"));
	connect(mOpenAct, SIGNAL(triggered()), mainWindow, SLOT(openFile()));

	mSaveAct = new QAction(QIcon(":images/saveas.png"),tr("&Save"), this);
	sm->registerAction(mSaveAct, "File Menu", "CTRL+S");
	mSaveAct->setStatusTip(tr("Save the document to disk"));
	connect(mSaveAct, SIGNAL(triggered()), mainWindow, SLOT(saveFile()));

	mSaveAsAct = new QAction(QIcon(":images/saveas.png"),tr("Save &As..."), this);
	sm->registerAction(mSaveAsAct, "File Menu", "CTRL+SHIFT+S");
	mSaveAsAct->setStatusTip(tr("Save the document under a new name"));
	connect(mSaveAsAct, SIGNAL(triggered()), mainWindow, SLOT(saveFileAs()));

	mSavePatchesAct = new QAction(QIcon(":images/saveas.png"),tr("Save &Patch OBJ..."), this);
	sm->registerAction(mSavePatchesAct, "File Menu", "");
	mSavePatchesAct->setStatusTip(tr("Save a bezier patch .obj file"));
	connect(mSavePatchesAct, SIGNAL(triggered()), mainWindow, SLOT(saveFileBezierOBJ()));

	loadTextureAct = new QAction(tr("Load &Texture..."), this);
	sm->registerAction(loadTextureAct, "File Menu", "CTRL+L");
	loadTextureAct->setStatusTip(tr("Load Texture from file"));
	connect(loadTextureAct, SIGNAL(triggered()), this, SLOT(load_texture()));

	printInfoAct = new QAction(tr("Print &Information"), this);
	sm->registerAction(printInfoAct, "File Menu", "CTRL+P");
	printInfoAct->setStatusTip(tr("Print Information to the console"));
	connect(printInfoAct, SIGNAL(triggered()), mainWindow, SLOT(printSummary()));

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
	connect(mUndoAct, SIGNAL(triggered()), mainWindow, SLOT(undo()));

	mRedoAct = new QAction(QIcon(":images/redo.png"), tr("&Redo"), this);
	sm->registerAction(mRedoAct, "Edit Menu", "CTRL+SHIFT+Z");
	mUndoAct->setStatusTip(tr("Redo the last operation"));
	connect(mRedoAct, SIGNAL(triggered()), mainWindow, SLOT(redo()));

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
	connect(showVerticesAct, SIGNAL(triggered()), mainWindow, SLOT(toggleVertices()));

	mShowFaceIDsAct = new QAction(tr("Show &Face IDs"), this);
	mShowFaceIDsAct->setCheckable(true);
	sm->registerAction(mShowFaceIDsAct, "Display Menu", "CTRL+F");
	connect(mShowFaceIDsAct, SIGNAL(triggered()), mainWindow, SLOT(toggleFaceIDs()));

	mShowEdgeIDsAct = new QAction(tr("Show &Edge IDs"), this);
	mShowEdgeIDsAct->setCheckable(true);
	sm->registerAction(mShowEdgeIDsAct, "Display Menu", "CTRL+E");
	connect(mShowEdgeIDsAct, SIGNAL(triggered()), mainWindow, SLOT(toggleEdgeIDs()));

	mShowVertexIDsAct = new QAction(tr("Show &Vertex IDs"), this);
	mShowVertexIDsAct->setCheckable(true);
	sm->registerAction(mShowVertexIDsAct, "Display Menu", "CTRL+V");
	connect(mShowVertexIDsAct, SIGNAL(triggered()), mainWindow, SLOT(toggleVertexIDs()));

	showSilhouetteAct = new QAction(tr("Show &Silhouette"), this);
	showSilhouetteAct->setCheckable(true);
	sm->registerAction(showSilhouetteAct, "Display Menu", "S" );
	// showSilhouetteAct->setStatusTip((tr"Blah"));
	connect(showSilhouetteAct, SIGNAL(triggered()), mainWindow, SLOT(toggleSilhouette()));

	showWireframeAct = new QAction(tr("Show &Wireframe"), this);
	showWireframeAct->setCheckable(true);
	showWireframeAct->setChecked(true);
	sm->registerAction(showWireframeAct, "Display Menu", "W");
	// showWireframeAct->setStatusTip(tr("Copy the current selection's contents to the "
	connect(showWireframeAct, SIGNAL(triggered()), mainWindow, SLOT(toggleWireframe()));

	showCoordinateAxesAct = new QAction(tr("Show &Coordinate Axes"), this);
	showCoordinateAxesAct->setCheckable(true);
	sm->registerAction(showCoordinateAxesAct, "Display Menu", "X");
	// showCoordinateAxesAct->setStatusTip(tr("Copy the current selection's contents to the "
	connect(showCoordinateAxesAct, SIGNAL(triggered()), mainWindow, SLOT(toggleAxes()));

	objectOrientationAct = new QAction(tr("Reverse Object"), this);
	objectOrientationAct->setCheckable(true);
	sm->registerAction(objectOrientationAct, "Display Menu", "R");
	// objectOrienationAct->setStatusTip(tr("Copy the current selection's contents to the "
	connect(objectOrientationAct, SIGNAL(triggered()), mainWindow, SLOT(toggleObjectOrientation()));

	showNormalsAct = new QAction(tr("Show Normals"), this);
	showNormalsAct->setCheckable(true);
	sm->registerAction(showNormalsAct, "Display Menu", "N");
	// objectOrienationAct->setStatusTip(tr("Copy the current selection's contents to the "
	connect(showNormalsAct, SIGNAL(triggered()), mainWindow, SLOT(toggleNormals()));

	showGridAct = new QAction(tr("Show &Grid"), this);
	showGridAct->setCheckable(true);
	sm->registerAction(showGridAct, "Display Menu", "G");
	// showGridAct->setStatusTip(tr("Copy the current selection's contents to the "
	connect(showGridAct, SIGNAL(triggered()), mainWindow, SLOT(toggleGrid()));

	#ifdef WITH_PYTHON
		showScriptEditorAct = new QAction(tr("Show Script &Editor"), this);
		showScriptEditorAct->setCheckable(true);
		showScriptEditorAct->setStatusTip( tr("Show the script editor to execute DLFL commands") );
		connect(showScriptEditorAct, SIGNAL(triggered()), this, SLOT(showHideScriptEditor()));
		sm->registerAction(showScriptEditorAct, "Display Menu", "SHIFT+CTRL+E");
	#endif
	
	#ifdef WITH_VERSE
		mShowVerseDialogAct = new QAction(tr("Show Verse &Dialog"), this);
		mShowVerseDialogAct->setCheckable(true);
		mShowVerseDialogAct->setStatusTip( tr("Show the verse dialog to view verse server connection status") );
		connect(mShowVerseDialogAct, SIGNAL(triggered()), this, SLOT(showHideVerseDialog()));
		sm->registerAction(mShowVerseDialogAct, "Display Menu", "SHIFT+CTRL+V");
	#endif	

	//Renderer Menu Actions
	normalRendererAct = new QAction(tr("&Normal Renderer"), this);
	normalRendererAct->setCheckable(true);
	sm->registerAction(normalRendererAct, "Renderer Menu", "1");
	normalRendererAct->setStatusTip(tr("Switch the current renderer to Normal"));
	connect(normalRendererAct, SIGNAL(triggered()), this, SLOT(use_normal_renderer()));

	shadedRendererAct = new QAction(tr("&Shaded Renderer"), this);
	shadedRendererAct->setCheckable(true);
	sm->registerAction(shadedRendererAct, "Renderer Menu", "2");
	shadedRendererAct->setStatusTip(tr("Switch the current renderer to Shaded"));
	connect(shadedRendererAct, SIGNAL(triggered()), this, SLOT(use_shaded_renderer()));

	lightedRendererAct = new QAction(tr("&Lighted Renderer"), this);
	lightedRendererAct->setCheckable(true);
	sm->registerAction(lightedRendererAct, "Renderer Menu", "3");
	lightedRendererAct->setStatusTip(tr("Switch the current renderer to Lighted"));
	connect(lightedRendererAct, SIGNAL(triggered()), this, SLOT(use_lighted_renderer()));
	
	texturedRendererAct = new QAction(tr("&Textured Renderer"), this);
	texturedRendererAct->setCheckable(true);
	sm->registerAction(texturedRendererAct, "Renderer Menu", "4");
	texturedRendererAct->setStatusTip(tr("Switch the current renderer to Textured"));
	connect(texturedRendererAct, SIGNAL(triggered()), this, SLOT(use_textured_renderer()));

	texturedLightedAct = new QAction(tr("Te&xtured Lighted Renderer"), this);
	texturedLightedAct->setCheckable(true);
	sm->registerAction(texturedLightedAct, "Renderer Menu", "5");
	texturedLightedAct->setStatusTip(tr("Switch the current renderer to Textured Lit"));
	connect(texturedLightedAct, SIGNAL(triggered()), this, SLOT(use_textured_lit_renderer()));

	patchRendererAct = new QAction(tr("&Patch Renderer"), this);
	patchRendererAct->setCheckable(true);
	sm->registerAction(patchRendererAct, "Renderer Menu", "6");
	patchRendererAct->setStatusTip(tr("Switch the current renderer to Patch"));
	connect(patchRendererAct, SIGNAL(triggered()), this, SLOT(use_patch_renderer()));

	rendererActionGroup = new QActionGroup(this);
	rendererActionGroup->setExclusive(true);
	rendererActionGroup->addAction(normalRendererAct);
	rendererActionGroup->addAction(lightedRendererAct);
	rendererActionGroup->addAction(shadedRendererAct);
	rendererActionGroup->addAction(texturedRendererAct);
	rendererActionGroup->addAction(texturedLightedAct);
	rendererActionGroup->addAction(patchRendererAct);
	lightedRendererAct->setChecked(true);

	//PRIMITIVES MENU ACTIONS
	pCubeAct = new QAction(QIcon(":/images/prim_cube.png"), tr("&Cube"), this);
	sm->registerAction(pCubeAct, "Primitives Menu", "CTRL+1");
	pCubeAct->setStatusTip(tr("Load a Cube"));
	connect(pCubeAct, SIGNAL(triggered()), mainWindow, SLOT(loadCube()));

	pOctahedronAct = new QAction(QIcon(":/images/prim_octahedron.png"), tr("&Octahedron"), this);
	sm->registerAction(pOctahedronAct, "Primitives Menu", "CTRL+2");
	pOctahedronAct->setStatusTip(tr("Load an octahedron"));
	connect(pOctahedronAct, SIGNAL(triggered()), mainWindow, SLOT(loadOctahedron()));

	pTetrahedronAct = new QAction(QIcon(":/images/prim_tetrahedron.png"), tr("&Tetrahedron"), this);
	sm->registerAction(pTetrahedronAct, "Primitives Menu", "CTRL+3");
	pTetrahedronAct->setStatusTip(tr("Load a tetrahedron"));
	connect(pTetrahedronAct, SIGNAL(triggered()), mainWindow, SLOT(loadTetrahedron()));

	pDodecahedronAct = new QAction(QIcon(":/images/prim_dodecahedron.png"), tr("&Dodecahedron"), this);
	sm->registerAction(pDodecahedronAct, "Primitives Menu", "CTRL+4");
	pDodecahedronAct->setStatusTip(tr("Load a dodecahedron"));
	connect(pDodecahedronAct, SIGNAL(triggered()), mainWindow, SLOT(loadDodecahedron()));

	pIcosahedronAct = new QAction(QIcon(":/images/prim_icosahedron.png"), tr("&Icosahedron"), this);
	sm->registerAction(pIcosahedronAct, "Primitives Menu", "CTRL+5");
	pIcosahedronAct->setStatusTip(tr("Load an icosahedron"));
	connect(pIcosahedronAct, SIGNAL(triggered()), mainWindow, SLOT(loadIcosahedron()));

	pSoccerBallAct = new QAction(QIcon(":/images/prim_soccerball.png"), tr("&Soccer ball"), this);
	sm->registerAction(pSoccerBallAct, "Primitives Menu", "CTRL+6");
	pSoccerBallAct->setStatusTip(tr("Load a soccer ball"));
	connect(pSoccerBallAct, SIGNAL(triggered()), mainWindow, SLOT(loadSoccerball()));

	pGeodesicAct = new QAction(QIcon(":/images/prim_geodesic.png"), tr("&Geodesic Dome"), this);
	sm->registerAction(pGeodesicAct, "Primitives Menu", "CTRL+7");
	pGeodesicAct->setStatusTip(tr("Load a geodesic dome"));
	connect(pGeodesicAct, SIGNAL(triggered()), mainWindow, SLOT(loadGeodesic()));
	
	//Object Menu Actions
	subdivideAllEdgesAct = new QAction(tr("Subdivide All &Edges"), this);
	sm->registerAction(subdivideAllEdgesAct, "Settings", "");
	// subdivideAllEdgesAct->setStatusTip(tr("Copy the current selection's contents to the "
	connect(subdivideAllEdgesAct, SIGNAL(triggered()), this, SLOT(subdivide_all_edges()));

	planarizeAllFacesAct = new QAction(tr("Planarize All &Faces"), this);
	sm->registerAction(planarizeAllFacesAct, "Settings", "CTRL+X");
	// planarizeAllFacesAct->setStatusTip(tr("Copy the current selection's contents to the "
	connect(planarizeAllFacesAct, SIGNAL(triggered()), this, SLOT(planarize_all_faces()));

	makeObjectSphericalAct = new QAction(tr("Make &Object Spherical"), this);
	sm->registerAction(makeObjectSphericalAct, "Settings", "CTRL+X");
	// makeObjectSphericalAct->setStatusTip(tr("Copy the current selection's contents to the "
	connect(makeObjectSphericalAct, SIGNAL(triggered()), this, SLOT(make_object_spherical()));

	makeObjectSmoothAct = new QAction(tr("Make Object &Smooth"), this);
	sm->registerAction(makeObjectSmoothAct, "Settings", "CTRL+X");
	// makeObjectSmoothAct->setStatusTip(tr("Copy the current selection's contents to the "
	connect(makeObjectSmoothAct, SIGNAL(triggered()), this, SLOT(make_object_smooth()));

	createCrustAct = new QAction(tr("&Create Crust"), this);
	sm->registerAction(createCrustAct, "Settings", "CTRL+X");
	// createCrustAct->setStatusTip(tr("Copy the current selection's contents to the "
	connect(createCrustAct, SIGNAL(triggered()), this, SLOT(create_crust()));

	computeLightingAct = new QAction(tr("Compute &Lighting"), this);
	sm->registerAction(computeLightingAct, "Settings", "CTRL+X");
	// computeLightingAct->setStatusTip(tr("Copy the current selection's contents to the "
	connect(computeLightingAct, SIGNAL(triggered()), this, SLOT(compute_lighting()));

	computeNormalsAndLightingAct = new QAction(tr("Compute &Normals and Lighting"), this);
	sm->registerAction(computeNormalsAndLightingAct, "Settings", "CTRL+X");
	// computeNormalsAndLightingAct->setStatusTip(tr("Copy the current selection's contents to the "
	connect(computeNormalsAndLightingAct, SIGNAL(triggered()), this, SLOT(compute_normals_and_lighting()));

	assignTextureCoordinatesAct = new QAction(tr("Assign &Texture Coordinates"), this);
	sm->registerAction(assignTextureCoordinatesAct, "Settings", "CTRL+X");
	// assignTextureCoordinatesAct->setStatusTip(tr("Copy the current selection's contents to the "
	connect(assignTextureCoordinatesAct, SIGNAL(triggered()), this, SLOT(assign_texture_coordinates()));

	// //SELECTION MENU ACTIONS
	// selectVertexAct = new QAction(tr("Select &Vertex"), this);
	// sm->registerAction(selectVertexAct, "Settings", "CTRL+M");
	// // selectVertexAct->setStatusTip(tr("Copy the current selection's contents to the "
	// connect(selectVertexAct, SIGNAL(triggered()), this, SLOT(select_vertex()));
	// 
	// selectFaceAct = new QAction(tr("Select &Face"), this);
	// sm->registerAction(selectFaceAct, "Settings", "CTRL+M");
	// // selectFaceAct->setStatusTip(tr("Copy the current selection's contents to the "
	// connect(selectFaceAct, SIGNAL(triggered()), this, SLOT(select_face() ) );
	// 
	// selectEdgeAct = new QAction(tr("Select &Edge"), this);
	// sm->registerAction(selectEdgeAct, "Settings", "CTRL+M");
	// // selectCornerAct->setStatusTip(tr("Copy the current selection's contents to the "));
	// sm->connect( selectEdgeAct , SIGNAL( triggered() ), SLOT  ( select_edge() ) );
	// 
	// selectCornerAct = new QAction(tr("Select &Corner"), this);
	// sm->registerAction(selectCornerAct, "Settings", "CTRL+M");
	// // selectCornerAct->setStatusTip(tr("Copy the current selection's contents to the "));
	// sm->connect( selectCornerAct , SIGNAL( triggered() ), SLOT  ( select_corner() ) );
	// 
	// exitSelectionModeAct = new QAction(tr("Exit Selection Mode"), this);
	// //sm->registerAction(exitSelectionModeAct, "Settings", "CTRL+X");
	// sm->connect( exitSelectionModeAct , SIGNAL( triggered() ), SLOT  ( exit_selection_mode() ) );
 
	//SETTINGS ACTIONS
	manageShortcutsAct = new QAction(tr("Shortcuts..."),this);
	sm->connect( manageShortcutsAct , SIGNAL( triggered() ), SLOT  ( configure() ) );
	sm->registerAction(manageShortcutsAct, "Settings", "CTRL+M");

	mEditStyleSheetAct = new QAction(tr("Stylesheets..."),this);
	connect( mEditStyleSheetAct, SIGNAL( triggered() ), this, SLOT(on_editStyleAction_triggered()) );
	sm->registerAction(mEditStyleSheetAct, "Settings", "CTRL+N");

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
		mVerseConnectLocalhostAct = new QAction(tr("Connect to localhost"), this);
		mVerseConnectLocalhostAct->setStatusTip( tr("Connect to localhost") );
		connect(mVerseConnectLocalhostAct, SIGNAL(triggered()), mVerseDialog, SLOT(connectLocalhost()));
		sm->registerAction(mVerseConnectLocalhostAct, "Verse Menu", "");
	
		mVerseConnectAct = new QAction(tr("Connect to host..."), this);
		mVerseConnectAct->setStatusTip( tr("Connect to host...") );
		connect(mVerseConnectAct, SIGNAL(triggered()), mVerseDialog, SLOT(connectHost()));
		sm->registerAction(mVerseConnectAct, "Verse Menu", "");

		mVerseDisconnectAct = new QAction(tr("Disconnect"), this);
		mVerseDisconnectAct->setStatusTip( tr("Disconnect from Verse Server") );
		connect(mVerseDisconnectAct, SIGNAL(triggered()), mVerseDialog, SLOT(disconnectHost()));
		sm->registerAction(mVerseDisconnectAct, "Verse Menu", "");

		mVerseDisconnectAllAct = new QAction(tr("Disconnect All"), this);
		mVerseDisconnectAllAct->setStatusTip( tr("Disconnect All Nodes") );
		connect(mVerseDisconnectAllAct, SIGNAL(triggered()), mVerseDialog, SLOT(disconnectAll()));
		sm->registerAction(mVerseDisconnectAllAct, "Verse Menu", "");
	#endif
	
	mPerformRemeshingAct = new QAction(tr("Perform Remeshing"), this);
	mPerformRemeshingAct->setStatusTip( tr("Perform the current remeshing scheme") );
	connect(mPerformRemeshingAct, SIGNAL(triggered()), this, SLOT(perform_remeshing()));
	sm->registerAction(mPerformRemeshingAct, "Remeshing Menu", "CTRL+R");
	
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
		mVerseMenu->addAction(mVerseConnectLocalhostAct);
		mVerseMenu->addAction(mVerseConnectAct);
		mVerseMenu->addSeparator();
		mVerseMenu->addAction(mVerseDisconnectAct);
		mVerseMenu->addAction(mVerseDisconnectAllAct);
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
	displayMenu->addAction(showSilhouetteAct);
	displayMenu->addAction(showWireframeAct);
	displayMenu->addAction(showCoordinateAxesAct);
	displayMenu->addAction(showGridAct);
	displayMenu->addAction(objectOrientationAct);
	displayMenu->addAction(showNormalsAct);
	
	#ifdef WITH_PYTHON
		displayMenu->addAction(showScriptEditorAct);
	#endif
	
	#ifdef WITH_VERSE
		displayMenu->addAction(mShowVerseDialogAct);
	#endif
	
	displayMenu->addAction(mFullscreenAct);

	rendererMenu = new QMenu(tr("&Renderer"));
	rendererMenu->setTearOffEnabled(true);
	menuBar->addMenu(rendererMenu);
	rendererMenu->addAction(normalRendererAct);
	rendererMenu->addAction(shadedRendererAct);
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

	// selectionMenu = new QMenu(tr("&Selection"));
	// // menuBar->addMenu(selectionMenu);
	// selectionMenu->addAction(selectVertexAct);
	// selectionMenu->addAction(selectFaceAct);
	// selectionMenu->addAction(selectEdgeAct);
	// selectionMenu->addAction(selectCornerAct);
	// selectionMenu->addAction(exitSelectionModeAct);

	settingsMenu = new QMenu(tr("Se&ttings"));
	settingsMenu->setTearOffEnabled(true);
	menuBar->addMenu(settingsMenu);
	settingsMenu->addAction(manageShortcutsAct);
	settingsMenu->addAction(mEditStyleSheetAct);
	languageMenu = new QMenu(tr("&Language"));
	languageMenu->setTearOffEnabled(true);
	settingsMenu->addMenu(languageMenu);
	languageMenu->addAction(englishAct);
	languageMenu->addAction(spanishAct);
	languageMenu->addAction(germanAct);
	languageMenu->addAction(frenchAct);
	languageMenu->addAction(turkishAct);
	languageMenu->addAction(catalanAct);
	
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
	
	// mConicalToolBar = new QToolBar(tr("Conical Tools"));
	// addToolBar(Qt::TopToolBarArea,mConicalToolBar);
	// mConicalToolBar->setIconSize(QSize(32,32));
		
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
	// mConicalMode->addActions(mToolsActionGroup, mConicalToolBar, mToolOptionsStackedWidget);
	
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
  QSettings settings("TopMod", "Topological Mesh Modeler");

	settings.beginGroup("MainWindow");
  QPoint pos = settings.value("pos", QPoint(100, 100)).toPoint();
  QSize size = settings.value("size", QSize(800, 600)).toSize();
	settings.endGroup();
	
  resize(size);
  move(pos);
}

void MainWindow::writeSettings() {
  QSettings settings("TopMod", "Topological Mesh Modeler");
  
	settings.beginGroup("MainWindow");
	settings.setValue("pos", pos());
  settings.setValue("size", size());
	settings.endGroup();
}

bool MainWindow::maybeSave() {
  if (mainWindow->isModified()) {
      int ret = QMessageBox::warning(this, tr("TopMod"),
                   tr("The document has been modified.\n"
                      "Do you want to save your changes?"),
                   QMessageBox::Yes | QMessageBox::Default,
                   QMessageBox::No,
                   QMessageBox::Cancel | QMessageBox::Escape);
      if (ret == QMessageBox::Yes)
          return mainWindow->saveFile();
      else if (ret == QMessageBox::Cancel)
          return false;
  }
  return true;
}

void MainWindow::loadFile(QString fileName) {
	openFile(fileName);
  mainWindow->setCurrentFile(fileName);
  statusBar()->showMessage(tr("File loaded"), 2000);
}

bool MainWindow::saveFile(QString fileName) {
  mainWindow->setCurrentFile(fileName);
  statusBar()->showMessage(tr("File saved"), 2000);
  return true;
}

void MainWindow::closeEvent(QCloseEvent *event) {
	if (maybeSave()) {
	    writeSettings();
	    event->accept();
	} else {
	    event->ignore();
	}
}

void MainWindow::newFile() { }

void MainWindow::open() {
	mainWindow->openFile();
}

void MainWindow::openFile(QString fileName) {
	mainWindow->openFile(fileName);
}

bool MainWindow::save() {
	mainWindow->saveFile();
}

bool MainWindow::saveAs() {
	mainWindow->saveFileAs();
}

void MainWindow::about() {
  QMessageBox::about(this, tr("About TopMod"),
		     tr("The <b>Application</b> example demonstrates how to "
			"write modern GUI applications using Qt, with a menu bar, "
			"toolbars, and a status bar."));
}

void MainWindow::documentWasModified() {
  setWindowModified(mainWindow->isModified());
}

void MainWindow::on_editStyleAction_triggered() {
  mStyleSheetEditor->show();
  mStyleSheetEditor->activateWindow();
}

void MainWindow::mousePressEvent(QMouseEvent *event) {
	if ( event->buttons() == Qt::RightButton ){
		// QMessageBox::about(this, tr("About TopMod"),tr("hey you bastard = %1").arg(QApplication::keyboardModifiers()));
		// modesMenu->popup(event->pos());
		startX = event->x();
		startY = event->y();
		// editMenu->popup(QPoint(event->x()-editMenu->width(),event->y()));
	}
}

void MainWindow::mouseMoveEvent(QMouseEvent *event) {
	if ( event->buttons() == Qt::RightButton ){
		// QMessageBox::about(this, tr("About TopMod"),tr("hey you bastard = %1").arg(startX));
		// modesMenu->popup(event->pos());
		endX = event->x();
		endY = event->y();
		update();
		
		// startX = event->x();
		// startY = event->y();
		// editMenu->popup(QPoint(event->x()-editMenu->width(),event->y()));
	}
}

void MainWindow::mouseReleaseEvent(QMouseEvent *event) {
	//if ( event->buttons() == Qt::RightButton ){
		// QMessageBox::about(this, tr("About TopMod"),tr("hey you bastard = %1").arg(QApplication::keyboardModifiers()));
		// modesMenu->popup(event->pos());
		// startX = event->x();
		// startY = event->y();
		// editMenu->popup(QPoint(event->x()-editMenu->width(),event->y()));
		startX = -1; startY = -1;
		endX = startX; endY = startY;
		update();
	//}
}

void MainWindow::paintEvent(QPaintEvent *event){

	// if (startX != -1){
	// 	QPainter painter(this);
	// 	painter.begin(this);
	//     painter.setRenderHint(QPainter::Antialiasing);
	// 
	// 	QPen pen(Qt::black, 5, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
	// 	// QPen pen(Qt::black, 5, Qt::NoPen, Qt::RoundCap, Qt::RoundJoin);
	// 	
	//  	painter.setPen(pen);
	// 	painter.drawLine( startX, startY, endX, endY);
	// 	painter.end();
	// }
	// else {
	// 	QPainter painter(this);
	// 	painter.begin(this);
	//     painter.setRenderHint(QPainter::Antialiasing);
	// 	QPen pen(Qt::NoPen);
	//  	painter.setPen(pen);
	// 	//painter.drawLine( startX, startY, endX, endY);
	// 	painter.end();
	// }

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
