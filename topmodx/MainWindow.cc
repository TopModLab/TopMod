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

/****************************************************************************
 **
 **
 ****************************************************************************/

#include <QtGui>
#include <QtOpenGL>
#include "MainWindow.h"

/*!
	\ingroup gui
	@{

	\class MainWindow
	\brief The Main Window of the application.

	\note

	\see MainWindow
*/

WireframeRendererPtr MainWindow::sWiredRenderer;				//!< WireframeRenderer Pointer
NormalRendererPtr MainWindow::sNormalRenderer;					//!< NormalRenderer Pointer
LitRendererPtr MainWindow::sLitRenderer;						//!< LitRenderer Pointer
TexturedRendererPtr MainWindow::sTexturedRenderer;				//!< TexturedRenderer Pointer
TexturedLitRendererPtr MainWindow::sTexturedLitRenderer;		//!< TexturedLitRenderer Pointer
PatchRendererPtr MainWindow::sPatchRenderer;					//!< PatchRenderer Pointer
ColorableRendererPtr MainWindow::sColorableRendererr;			//!< ColorableRenderer Pointer

//-- Parameters used in various operations on the DLFL object --//
//-- See header file for explanations --//

int MainWindow::sDragStartX = 0;
int MainWindow::sDragStartY = 0;
bool MainWindow::sIsEditing = false;

//face area - dave
float MainWindow::sFaceAreaTolerance = 0.05;
//face color tolerance
float MainWindow::sFaceColorTolerance = 0.00;

// Edge deletion
bool MainWindow::sDeleteEdgeCleanup = true;

// Handles
int MainWindow::sNumSegments = 10;
int MainWindow::sMaxSegments = -1;
bool MainWindow::sSymmetricWeights = true;
double MainWindow::sFaceNormalWeight1 = 25.0;
double MainWindow::sFaceNormalWeight2 = 25.0;
int MainWindow::sNumExtraTwists = 0;

// Extrusions
double MainWindow::sExtrudeDist = 2.0;
double MainWindow::sExtrudeRot = 0.0;
double MainWindow::sExtrudeScale = 1.0;
double MainWindow::sExtrudeAngle = 40.0;
double MainWindow::sExtrudeLength1 = 0.5;
double MainWindow::sExtrudeLength2 = 1.0;
double MainWindow::sExtrudeLength3 = 1.0;
double MainWindow::sExtrudeAngle_iCosA = 50.0;
double MainWindow::sExtrudeLength1_iCosA = 0.5;
double MainWindow::sExtrudeLength2_iCosA = 0.7;
double MainWindow::sExtrudeLength3_iCosA = 0.8;
int MainWindow::sNumExtrusions = 1;
double MainWindow::sDooSabinExtrusionTwist = 0.0;
bool MainWindow::sDualMeshEdgesCheck = false;
bool MainWindow::sHexagonalizeDodecaExtrude = false;
double MainWindow::sExtrudeBendT = 0;
double MainWindow::sExtrudeBendB = 0;

// Edge subdivision
int MainWindow::sNumEdgeSubdiv = 2;

// Split valence 2 vertices
double MainWindow::sVertexSplitOffset=-0.1;

// Crust modeling
double MainWindow::sCrustThickness = 0.5;
double MainWindow::sCrustScaleFactor = 0.9;
bool MainWindow::sCrustCleanup = true;

// Wireframe modeling
double MainWindow::sWireframeThickness = 0.25;
double MainWindow::sWireframe2Thickness = 0.25;
double MainWindow::sWireframe2Width = 0.25;
bool MainWindow::sWireframeSplit = true;
double MainWindow::sCornerCuttingAlpha = 9.0/16.0;


// Column modeling
double MainWindow::sColumnThickness = 0.25;
int MainWindow::sColumnSegments = 4;

// Subdivision
double MainWindow::sTwistFactor = 0.0;
double MainWindow::sWeightFactor = 0.0;
bool MainWindow::sDooSabinCheck = true;
double MainWindow::sVertexCuttingOffset = 0.25;
double MainWindow::sPentagonalOffset = 0.0;
double MainWindow::sPentagonalScale = 0.75;
bool MainWindow::sAccurateDual = true;
double MainWindow::sCheckerboardThickness = 0.33;
double MainWindow::sModifiedCornerCuttingThickness = 0.25;

// Added by Doug
double MainWindow::sStarOffset = 0.0;
double MainWindow::sFractalOffset = 1.0;

//Added by Ryan
double MainWindow::sPinchingFactor = 1;
double MainWindow::sPinchCenter = 0.5;
double MainWindow::sBubbleFactor = 1;
double MainWindow::sHoleHandlePinchingFactor = 1;
double MainWindow::sHoleHandlePinchCenter = 0.5;
double MainWindow::sHoleHandlePinchWidth = 1;

// Added by Eric
double MainWindow::sSubstellateHeight = 0.0;
double MainWindow::sSubstellateCurve = 0.0;

// Added by bei&cansin dec 09
double MainWindow::sDomeLengthFactor = 1.0 ;
double MainWindow::sDomeScaleFactor = 1.0  ;
double MainWindow::sDual1264ScaleFactor = 0.7 ;

//dave
// double MainWindow::dooSabinExtrudeTwist_factor = 0.0 ;
double MainWindow::sDomeExtrudeLengthFactor = 1.0 ;
double MainWindow::sDomeExtrudeScaleFactor = 1.0  ;
double MainWindow::sDomeExtrudeRotationFactor = 0.0  ;

// Added by bei dec 11
double MainWindow::sDooSabinBCnewScaleFactor = 1.0;
double MainWindow::sDooSabinBCnewLengthFactor = 1.0;

// Added by bei 12-11-03
double MainWindow::sLoopLengthFactor = 1.0;

// Face subdivision
bool MainWindow::sUseQuads = true;

// Tile texturing
int MainWindow::sTileTexSize = 2;

// Multi-face handles
MainWindow::MFHAlgo MainWindow::sMfhAlgorithm = MainWindow::ConvexHull;
double MainWindow::sMfhScaleFactor = 1.0;
double MainWindow::sMfhExtrudeDist = 0.5;
bool MainWindow::sMfhUseMaxOffsets = false;
double MainWindow::sMfhMinAdjFactor = 0.01;
bool MainWindow::sMfhMakeConnections = true;

// Menger Sponge creation
double MainWindow::sSpongeThickness = 0.67; // Thickness for Menger Sponge
double MainWindow::sSpongeCollapseThreshold = 0.0; // Threshold factor for edge collapse
bool MainWindow::sSpongeFractionalThickness = false; // Thickness is fractional

//conical sculpting initialization from ozgur
double MainWindow::sCutOffsetEdgeFactor = 0.25;
double MainWindow::sCutOffsetVertFactor = 0.25;
double MainWindow::sPNormalBendS_Factor = 0;
double MainWindow::sPNormalBendT_Factor = 0;
bool MainWindow::sGlobalCut = false;
int MainWindow::sNumOfPeels = 90;
bool MainWindow::sSelectedCut=true;

//-- Selection count, used as index into selection lists --//
int MainWindow::sNumSelVerts = 0;
int MainWindow::sNumSelEdges = 0;
int MainWindow::sNumSelFaces = 0;
int MainWindow::sNumSelFaceVerts = 0;

bool MainWindow::sDeselectVerts = false;
bool MainWindow::sDeselectEdges = false;
bool MainWindow::sDeselectFaces = false;
bool MainWindow::sDeselectFaceVerts = false;

// for face loop selection
DLFLEdgePtr MainWindow::sFaceLoopStartEdge = nullptr;
DLFLEdgePtr MainWindow::sEdgeRingStartEdge = nullptr;

//incremental save...
int MainWindow::sIncrementalSaveCount = 0;

//paint bucket static color:
QColor MainWindow::sPaintBucketColor = QColor(0.5,0.5,0.5);

/**
 * asdflkjasdf
 * asdfl;jkas;df
 **/
MainWindow::MainWindow(char *filename)
	: mObject(), mOperationMode(NormalMode)
	, mUndoList(), mUndoMtlList(), mRedoList(), mRedoMtlList()
	, mUndoLimit(20), mUseUndo(true)
	, mIsModified(false), mIsPrimitive(false), mWasPrimitive(false)
	, mSpinBoxMode(eInvalidSpinBox)
{
	// i18n stuff
	mTranslator_es = new QTranslator(this);
	mTranslator_fr = new QTranslator(this);
	mTranslator_de = new QTranslator(this);
	mTranslator_tr = new QTranslator(this);
	mTranslator_it = new QTranslator(this);
	mTranslator_hi = new QTranslator(this);
	mTranslator_ca = new QTranslator(this);

	//for auto save
	//auto save timer // and connect it to the saveFile slot
	mAutoSaveTimer = new QTimer(this);
	connect(mAutoSaveTimer, SIGNAL(timeout()), this, SLOT(saveFile(/*normals and texture options should go here... eventually*/)));

	//QSettings Path for windows
#ifdef WIN32
	QSettings::setPath(QSettings::IniFormat,QSettings::UserScope,QString("%APPDATA%"));
#endif

//#ifdef __APPLE__
	//mac icon
	this->setWindowIcon(QIcon(":/images/topmod.png"));
//#endif
	//initialize the OpenGL Window GLWidget
	QGLFormat fmt;
	//initialize renderer
	createRenderers();
	setMouseTracking(true);

	mActiveViewport = new GLWidget(500,600, sLitRenderer, QColor(255,255,255,255),QColor(255,255,255,255) , &mObject, fmt, this);
	// active->switchTo(VPPersp);
	setRenderer(sLitRenderer);
	mActiveViewport->redraw();
	mActiveViewport->setMinimumSize(400,400);
	mActiveViewport->setFocusPolicy(Qt::StrongFocus);
	setFocusPolicy(Qt::StrongFocus);
	// active->setResizeEnabled(true);
	//status bar
	mStatusBar = new QStatusBar(this);
	//statusbar is not working!!! it throws off glwidget's pick coordinates.... :(
	setStatusBar(mStatusBar);
	setAttribute(Qt::WA_AcceptDrops, true);
	setWindowFlags(Qt::Window);
	setWindowTitle(tr("newfile[*] - TopMod"));
	setCurrentFile("untitled");
	// cWidget = new QWidget();
	// QWidget *shwidget = new QWidget;
	// QVBoxLayout *vblayout = new QVBoxLayout;
	// vblayout->addWidget(active);
	// shwidget->setLayout(vblayout);
	//accept file drop events
	setAcceptDrops(true);
	/** Setup Main Layout and add the glwidget to it **/
	// layout = new QBoxLayout( QBoxLayout::LeftToRight, this );
	// layout->addWidget(active);
	// layout->setMargin(0);
	// vblayout->setMargin(0);
	// cWidget->setLayout( layout );
	setCentralWidget(mActiveViewport);
	//store user's desktop size
	QWidget *d = QApplication::desktop();
	int w=d->width();  // returns screen width
	int h=d->height(); // returns screen height

#ifdef WITH_PYTHON
	//the script editor widget will be placed into a QDockWidget
	//and will be dockable in the top and bottom sections of the main window
	mScriptEditorDockWidget = new QDockWidget(tr("Script Editor"), this);
	mScriptEditor = new DLFLScriptEditor( &mObject, mScriptEditorDockWidget );
	mScriptEditorDockWidget->setAllowedAreas(Qt::BottomDockWidgetArea | Qt::TopDockWidgetArea);
	mScriptEditorDockWidget->setWidget(mScriptEditor);
	addDockWidget(Qt::BottomDockWidgetArea, mScriptEditorDockWidget);
	mScriptEditorDockWidget->hide();
	mScriptEditorDockWidget->setFloating(true);
	// mScriptEditorDockWidget->setGeometry(20,h-320,500,300);
	//mScriptEditorDockWidget->setMaximumHeight(200);
	connect( this, SIGNAL(loadedObject(DLFLObject*,QString)),mScriptEditor, SLOT(loadObject(DLFLObject*,QString)) );
	connect( this, SIGNAL(echoCommand(QString)),mScriptEditor, SLOT(echoCommand(QString)) );
	connect( mScriptEditor, SIGNAL(makingChange()), this, SLOT(undoPush()) );
	connect( mScriptEditor, SIGNAL(cmdExecuted()), this, SLOT(recomputeAll()) );
	connect( mScriptEditor, SIGNAL(cmdExecuted()), this->getActive(), SLOT(update()) );
	connect( mScriptEditor, SIGNAL(requestObject(QString)), this, SLOT(openFile(QString)) );

	if( !Py_IsInitialized() )
		Py_Initialize( );
#endif

#ifdef WITH_VERSE
	mVerseDialog = VerseTopMod::Instance(this);
	mVerseDialogDockWidget = new QDockWidget(tr("Verse-TopMod"), this);
	mVerseDialogDockWidget->setAllowedAreas(Qt::BottomDockWidgetArea | Qt::TopDockWidgetArea);
	mVerseDialogDockWidget->setWidget(mVerseDialog);
	addDockWidget(Qt::BottomDockWidgetArea, mVerseDialogDockWidget);
	mVerseDialogDockWidget->hide();
	mVerseDialogDockWidget->setMaximumHeight(200);
#endif

	//Tool options dockwidget must be initialized before setting up the actions
	//the main tool options DockWidget
	mToolOptionsDockWidget = new QDockWidget(tr("Tool Options - Insert Edge"), this);
	mToolOptionsDockWidget->setAllowedAreas(Qt::RightDockWidgetArea);
	mToolOptionsStackedWidget = new QStackedWidget();
	mToolOptionsDockWidget->setWidget(mToolOptionsStackedWidget);
	// mToolOptionsDockWidget->move(width() , 150);
	// mToolOptionsTitleBarLayout = new QBoxLayout(QBoxLayout::LeftToRight);
	// mToolOptionsTitleBarWidget = new QWidget;
	// mToolOptionsTitleBarWidget->setLayout(mToolOptionsTitleBarLayout);
	// mToolOptionsDockWidget->setTitleBarWidget(mToolOptionsTitleBarWidget);
	// mToolOptionsDockWidget->titleBarWidget()->setContentsMargins(0,0,0,0);
	// mToolOptionsDockWidget->move(0,22);
	addDockWidget(Qt::RightDockWidgetArea, mToolOptionsDockWidget);

	//animated help dockwidget
	initializeAnimatedHelp();

	//make a new instance of QShortcutManager
	mShortcutManager = new QShortcutManager();
	mActionModel = new QStandardItemModel();
	mActionListWidget = new QWidget;

	//startup dialog
	mStartupDialogDockWidget = new QDockWidget(tr("Learning Movies"),this);

	createActions();

	//instantiate toolbars
	mBasicsMode = new BasicsMode(this, mShortcutManager, mActionListWidget);
	mExtrusionsMode = new ExtrusionsMode(this, mShortcutManager, mActionListWidget );
	mConicalMode = new ConicalMode(this, mShortcutManager);
	mRemeshingMode = new RemeshingMode(this, mShortcutManager, mActionListWidget);
	mHighgenusMode = new HighgenusMode(this, mShortcutManager, mActionListWidget);
	mTexturingMode = new TexturingMode(this, mShortcutManager, mActionListWidget);
	mExperimentalMode = new ExperimentalMode(this, mShortcutManager, mActionListWidget);


	#ifdef QCOMPLETER
	mCommandCompleter = new CommandCompleter(mActionListWidget, this);
	#endif
	createToolBars();
	createMenus();
	//initialize the help file...
	initializeHelp();
	//style sheet editor
	mStyleSheetEditor = new StyleSheetEditor;
	//preference dialog
	mSettings = new QSettings("TopMod");
	mPreferencesDialog = new TopModPreferences(mSettings, mStyleSheetEditor, mShortcutManager, this);

	// //testing shortcut context
	// for (int i = 0; i < mActionListWidget->actions().count(); i++){
	// 	mActionListWidget->actions().at(i)->setShortcutContext(Qt::WidgetShortcut);
	// }

	//reposition floating windows:
	// mToolOptionsDockWidget->setGeometry(10 + this->x()/*this->active->width()-mToolOptionsDockWidget->width()*/,this->y()+150,mToolOptionsDockWidget->width(),mToolOptionsDockWidget->height());
	// mToolOptionsDockWidget->setFixedSize(mToolOptionsDockWidget->width(),mToolOptionsDockWidget->height());

	//must happen after preference file is loaded
	createStartupDialog();

	retranslateUi();
	setExtrusionMode(CubicalExtrude);
	setMode(MainWindow::NormalMode);

	// std::cout << QT_VER << "\n";
}

// void MainWindow::setToolOptionsPo

void MainWindow::createActions() {

	//File Menu Actions
	mNewFileAct = new QAction(/*QIcon(":images/folder.png"),*/tr("&New File..."), this);
	mShortcutManager->registerAction(mNewFileAct, "File Menu", "CTRL+N");
	mNewFileAct->setStatusTip(tr("Open a blank file and clear the undo list"));
	connect(mNewFileAct, SIGNAL(triggered()), this, SLOT(newFile()));
	mActionListWidget->addAction(mNewFileAct);

	mOpenAct = new QAction(QIcon(":images/folder.png"), tr("&Open..."), this);
	mShortcutManager->registerAction(mOpenAct, "File Menu", "CTRL+O");
	mOpenAct->setStatusTip(tr("Open an existing file"));
	connect(mOpenAct, SIGNAL(triggered()), this, SLOT(openFile()));
	mActionListWidget->addAction(mOpenAct);

	mSaveAct = new QAction(QIcon(":images/document-save.png"), tr("&Save"), this);
	mShortcutManager->registerAction(mSaveAct, "File Menu", "CTRL+S");
	mSaveAct->setStatusTip(tr("Save the document to disk"));
	connect(mSaveAct, SIGNAL(triggered()), this, SLOT(saveFile()));
	mActionListWidget->addAction(mSaveAct);

	mSaveAsAct = new QAction(QIcon(":images/document-save-as.png"), tr("Save &As..."), this);
	mShortcutManager->registerAction(mSaveAsAct, "File Menu", "CTRL+SHIFT+S");
	mSaveAsAct->setStatusTip(tr("Save the document under a new name"));
	connect(mSaveAsAct, SIGNAL(triggered()), this, SLOT(saveFileAs()));
	mActionListWidget->addAction(mSaveAsAct);

	mSavePatchesAct = new QAction(QIcon(":images/saveas.png"), tr("Save &Patch OBJ..."), this);
	mShortcutManager->registerAction(mSavePatchesAct, "File Menu", "");
	mSavePatchesAct->setStatusTip(tr("Save a bezier patch .obj file"));
	connect(mSavePatchesAct, SIGNAL(triggered()), this, SLOT(saveFileBezierOBJ()));
	mActionListWidget->addAction(mSavePatchesAct);

	mSaveLG3dAct = new QAction(QIcon(":images/saveas.png"), tr("Export LiveGrahpics3D..."), this);
	mShortcutManager->registerAction(mSaveLG3dAct, "File Menu", "CTRL+ALT+S");
	mSaveLG3dAct->setStatusTip(tr(
		"Export a LiveGraphics3D (*.m) file for embedding into the TopMod Wiki, "\
		"Warning: you cannot import this file back into TopMod"));
	connect(mSaveLG3dAct, SIGNAL(triggered()), this, SLOT(saveFileLG3d()));
	mActionListWidget->addAction(mSaveLG3dAct);

	mSaveLG3dSelectedAct = new QAction(
		QIcon(":images/saveas.png"), tr("Export LG3d (Sel. Faces)..."), this);
	mShortcutManager->registerAction(mSaveLG3dSelectedAct, "File Menu", "");
	mSaveLG3dSelectedAct->setStatusTip(tr(
		"Export a LiveGraphics3D (*.m) of the current selected faces file for embedding into the TopMod Wiki, "\
		"Warning: you cannot import this file back into TopMod"));
	connect(mSaveLG3dSelectedAct, SIGNAL(triggered()), this, SLOT(saveFileLG3dSelected()));
	mActionListWidget->addAction(mSaveLG3dSelectedAct);

	mExportSTLAct = new QAction(QIcon(":images/saveas.png"), tr("Export STL..."), this);
	mShortcutManager->registerAction(mExportSTLAct, "File Menu", "CTRL+ALT+SHIFT+S");
	mExportSTLAct->setStatusTip(tr(
		"Export a stereolithography (*.stl) file for use "\
		"with various rapid prototyping software and hardware"));
	connect(mExportSTLAct, SIGNAL(triggered()), this, SLOT(saveFileSTL()));
	mActionListWidget->addAction(mExportSTLAct);

	mScreenshotViewportAct = new QAction(
		QIcon(":images/camera-photo.png"), tr("Save Viewport Screenshot..."), this);
	mShortcutManager->registerAction(mScreenshotViewportAct, "File Menu", "0");
	mScreenshotViewportAct->setStatusTip(tr("Save a screenshot of the contents of the OpenGL viewport (*.png)"));
	connect(mScreenshotViewportAct, SIGNAL(triggered()), this, SLOT(viewportScreenshot()));
	mActionListWidget->addAction(mScreenshotViewportAct);

	mScreenshotAppAct = new QAction(QIcon(":images/image-x-generic.png"), tr("Save App Screenshot..."), this);
	mShortcutManager->registerAction(mScreenshotAppAct, "File Menu", "9");
	mScreenshotAppAct->setStatusTip(tr("Save a screenshot of the entire main application window (*.png)"));
	connect(mScreenshotAppAct, SIGNAL(triggered()), this, SLOT(appScreenshot()));
	mActionListWidget->addAction(mScreenshotAppAct);

	mLoadTextureAct = new QAction(tr("Load &Texture..."), this);
	mShortcutManager->registerAction(mLoadTextureAct, "File Menu", "CTRL+L");
	mLoadTextureAct->setStatusTip(tr("Load Texture from file"));
	connect(mLoadTextureAct, SIGNAL(triggered()), this, SLOT(load_texture()));
	mActionListWidget->addAction(mLoadTextureAct);

	mPrintInfoAct = new QAction(tr("Print &Information"), this);
	mShortcutManager->registerAction(mPrintInfoAct, "File Menu", "P");
	mPrintInfoAct->setStatusTip(tr("Print Information to the console"));
	connect(mPrintInfoAct, SIGNAL(triggered()), this, SLOT(printSummary()));
	mActionListWidget->addAction(mPrintInfoAct);

	mPrintFaceListAct = new QAction(tr("Print &Face List"), this);
	mShortcutManager->registerAction(mPrintFaceListAct, "File Menu", "CTRL+ALT+P");
	mPrintFaceListAct->setStatusTip(tr("Print Face List to the console"));
	// connect(printFaceListAct, SIGNAL(triggered()), this, SLOT(saveAs()));
	mActionListWidget->addAction(mPrintFaceListAct);

	mPrintVertexListAct = new QAction(tr("Print &Vertex List"), this);
	mShortcutManager->registerAction(mPrintVertexListAct, "File Menu", "CTRL+SHIFT+P");
	mPrintVertexListAct->setStatusTip(tr("Save the document under a new name"));
	// connect(printVertexListAct, SIGNAL(triggered()), this, SLOT(saveAs()));
	mActionListWidget->addAction(mPrintVertexListAct);

	mPrintEdgeListAct = new QAction(tr("Print &Edge List"), this);
	mShortcutManager->registerAction(mPrintEdgeListAct, "File Menu", "CTRL+ALT+SHIFT+P");
	mPrintEdgeListAct->setStatusTip(tr("Print Edge list to the console"));
	// connect(printEdgeListAct, SIGNAL(triggered()), this, SLOT(saveAs()));
	mActionListWidget->addAction(mPrintEdgeListAct);

	mPrintCVListAct = new QAction(tr("Print &CV List"), this);
	mShortcutManager->registerAction(mPrintCVListAct, "File Menu", "");
	mPrintCVListAct->setStatusTip(tr("Print CV list to the console"));
	// connect(mPrintCVListAct, SIGNAL(triggered()), this, SLOT(saveAs()));
	mActionListWidget->addAction(mPrintCVListAct);

	mExitAct = new QAction(tr("E&xit"), this);
	mShortcutManager->registerAction(mExitAct, "File Menu", "CTRL+Q");
	mExitAct->setStatusTip(tr("Exit the application"));
	connect(mExitAct, SIGNAL(triggered()), this, SLOT(close()));
	// mActionListWidget->addAction(mExitAct);

	//quick command quicksilver like interface
#ifdef QCOMPLETER
	mQuickCommandAct = new QAction(tr("Quick Command"), this);
	QShortcut *shortcut = new QShortcut(QKeySequence(Qt::Key_Space), this);
	connect(shortcut, SIGNAL(activated()), this, SLOT(getCommand()));
	mQuickCommandAct->setStatusTip(tr("Quick Command Access with Autocompletion"));
	connect(mQuickCommandAct, SIGNAL(triggered()), this, SLOT(getCommand()));
	// mActionListWidget->addAction(mQuickCommandAct);
#endif
	//Edit Menu Actions
	mDeleteSelectedAct = new QAction(/*QIcon(":images/edit-undo.png"),*/ tr("&Delete Selected"), this);
	mShortcutManager->registerAction(mDeleteSelectedAct, "Edit Menu", "Del");
	mDeleteSelectedAct->setStatusTip(tr("Delete Selected"));
	connect(mDeleteSelectedAct, SIGNAL(triggered()), this, SLOT(deleteSelected()));
	mActionListWidget->addAction(mDeleteSelectedAct);

	mUndoAct = new QAction(QIcon(":images/edit-undo.png"), tr("&Undo"), this);
	mShortcutManager->registerAction(mUndoAct, "Edit Menu", "CTRL+Z");
	mUndoAct->setStatusTip(tr("Undo the last operation"));
	connect(mUndoAct, SIGNAL(triggered()), this, SLOT(undo()));
	mActionListWidget->addAction(mUndoAct);

	mRedoAct = new QAction(QIcon(":images/edit-redo.png"), tr("&Redo"), this);
	mShortcutManager->registerAction(mRedoAct, "Edit Menu", "CTRL+SHIFT+Z");
	mRedoAct->setStatusTip(tr("Redo the last operation"));
	connect(mRedoAct, SIGNAL(triggered()), this, SLOT(redo()));
	mActionListWidget->addAction(mRedoAct);

	mClearUndoListAct = new QAction(tr("&Clear Undo List"), this);
	mShortcutManager->registerAction(mClearUndoListAct, "Edit Menu", "CTRL+SHIFT+Z");
	mClearUndoListAct->setStatusTip(tr("Clear the Undo List to free up some memory"));
	connect(mClearUndoListAct, SIGNAL(triggered()), this, SLOT(clearUndoList()));
	mActionListWidget->addAction(mClearUndoListAct);

	//View Menu Actions
	mPerspViewAct = new QAction(tr("&Reset Camera"), this);
	mShortcutManager->registerAction(mPerspViewAct, "View Menu", "F");
	mPerspViewAct->setStatusTip(tr("Reset Camera Position to default"));
	connect(mPerspViewAct, SIGNAL(triggered()), this, SLOT(switchPerspView()));
	mActionListWidget->addAction(mPerspViewAct);

	mZoomInAct = new QAction(tr("Zoom In"), this);
	mShortcutManager->registerAction(mZoomInAct, "Display", "ALT+=");
	mZoomInAct->setStatusTip(tr("Zoom in on the Model"));
	connect(mZoomInAct, SIGNAL(triggered()), mActiveViewport, SLOT(zoomIn()));
	// mActionListWidget->addAction(mZoomInAct);

	mZoomOutAct = new QAction(tr("Zoom Out"), this);
	mShortcutManager->registerAction(mZoomOutAct, "Display", "ALT+-");
	mZoomOutAct->setStatusTip(tr("Zoom in on the Model"));
	connect(mZoomOutAct, SIGNAL(triggered()), mActiveViewport, SLOT(zoomOut()));

	// mTopViewAct = new QAction( tr("&Top View"), this);
	// sm->registerAction(mTopViewAct, "View Menu", "");
	// mTopViewAct->setStatusTip(tr("Switch to Top View"));
	// connect(mTopViewAct, SIGNAL(triggered()), this, SLOT(switchTopView()));
	// // mActionListWidget->addAction(mTopViewAct);
	//
	// mBottomViewAct = new QAction( tr("&Bottom View"), this);
	// sm->registerAction(mBottomViewAct, "View Menu", "");
	// mBottomViewAct->setStatusTip(tr("Switch to Bottom View"));
	// connect(mBottomViewAct, SIGNAL(triggered()), this, SLOT(switchBottomView()));
	// // mActionListWidget->addAction(mBottomViewAct);
	//
	// mFrontViewAct = new QAction( tr("&Front View"), this);
	// sm->registerAction(mFrontViewAct, "View Menu", "");
	// mFrontViewAct->setStatusTip(tr("Switch to Front View"));
	// connect(mFrontViewAct, SIGNAL(triggered()), this, SLOT(switchFrontView()));
	// // mActionListWidget->addAction(mFrontViewAct);
	//
	// mBackViewAct = new QAction( tr("B&ack View"), this);
	// sm->registerAction(mBackViewAct, "View Menu", "");
	// mBackViewAct->setStatusTip(tr("Switch to Back View"));
	// connect(mBackViewAct, SIGNAL(triggered()), this, SLOT(switchBackView()));
	// // mActionListWidget->addAction(mBackViewAct);
	//
	// mLeftViewAct = new QAction( tr("&Left View"), this);
	// sm->registerAction(mLeftViewAct, "View Menu", "");
	// mLeftViewAct->setStatusTip(tr("Switch to Left View"));
	// connect(mLeftViewAct, SIGNAL(triggered()), this, SLOT(switchLeftView()));
	// // mActionListWidget->addAction(mLeftViewAct);
	//
	// mRightViewAct = new QAction( tr("&Right View"), this);
	// sm->registerAction(mRightViewAct, "View Menu", "");
	// mRightViewAct->setStatusTip(tr("Switch to Right View"));
	// connect(mRightViewAct, SIGNAL(triggered()), this, SLOT(switchRightView()));
	// // mActionListWidget->addAction(mRightViewAct);

	//Display Menu Actions
	mFullscreenAct = new QAction(QIcon(":images/view-fullscreen.png"), tr("&Full Screen"), this);
	mFullscreenAct->setCheckable(true);
	mShortcutManager->registerAction(mFullscreenAct, "Display Menu", "ALT+F");
	mFullscreenAct->setStatusTip(tr("Toggle Full Screen"));
	connect(mFullscreenAct, SIGNAL(triggered()), this, SLOT(toggleFullScreen()));
	mActionListWidget->addAction(mFullscreenAct);

	mShowVerticesAct = new QAction(tr("Show &Vertices"), this);
	mShowVerticesAct->setCheckable(true);
	mShortcutManager->registerAction(mShowVerticesAct, "Display Menu", "V");
	connect(mShowVerticesAct, SIGNAL(triggered()), this->getActive(), SLOT(toggleVertices()));
	mActionListWidget->addAction(mShowVerticesAct);

	mShowFaceIDsAct = new QAction(tr("Show &Face IDs"), this);
	mShowFaceIDsAct->setCheckable(true);
	mShortcutManager->registerAction(mShowFaceIDsAct, "Display Menu", "CTRL+F");
	connect(mShowFaceIDsAct, SIGNAL(triggered()), this->getActive(), SLOT(toggleFaceIDs()));
	mActionListWidget->addAction(mShowFaceIDsAct);

	mShowEdgeIDsAct = new QAction(tr("Show &Edge IDs"), this);
	mShowEdgeIDsAct->setCheckable(true);
	mShortcutManager->registerAction(mShowEdgeIDsAct, "Display Menu", "CTRL+E");
	connect(mShowEdgeIDsAct, SIGNAL(triggered()), this->getActive(), SLOT(toggleEdgeIDs()));
	mActionListWidget->addAction(mShowEdgeIDsAct);

	mShowVertexIDsAct = new QAction(tr("Show &Vertex IDs"), this);
	mShowVertexIDsAct->setCheckable(true);
	mShortcutManager->registerAction(mShowVertexIDsAct, "Display Menu", "CTRL+V");
	connect(mShowVertexIDsAct, SIGNAL(triggered()), this->getActive(), SLOT(toggleVertexIDs()));
	mActionListWidget->addAction(mShowVertexIDsAct);

	mShowSelectedIDsAct = new QAction(tr("Show &Selected IDs"), this);
	mShowSelectedIDsAct->setCheckable(true);
	mShortcutManager->registerAction(mShowSelectedIDsAct, "Display Menu", "SHIFT+D");
	connect(mShowSelectedIDsAct, SIGNAL(triggered()), this->getActive(), SLOT(toggleSelectedIDs()));
	mActionListWidget->addAction(mShowSelectedIDsAct);

	mShowSilhouetteAct = new QAction(tr("Show &Silhouette"), this);
	mShowSilhouetteAct->setCheckable(true);
	mShortcutManager->registerAction(mShowSilhouetteAct, "Display Menu", "S");
	connect(mShowSilhouetteAct, SIGNAL(triggered()), this->getActive(), SLOT(toggleSilhouette()));
	mActionListWidget->addAction(mShowSilhouetteAct);

	mShowWireframeAct = new QAction(tr("Show &Wireframe"), this);
	mShowWireframeAct->setCheckable(true);
	mShowWireframeAct->setChecked(true);
	mShortcutManager->registerAction(mShowWireframeAct, "Display Menu", "W");
	connect(mShowWireframeAct, SIGNAL(triggered()), this->getActive(), SLOT(toggleWireframe()));
	mActionListWidget->addAction(mShowWireframeAct);

	mShowCoordinateAxesAct = new QAction(tr("Show &Coordinate Axes"), this);
	mShowCoordinateAxesAct->setCheckable(true);
	mShortcutManager->registerAction(mShowCoordinateAxesAct, "Display Menu", "X");
	connect(mShowCoordinateAxesAct, SIGNAL(triggered()), this->getActive(), SLOT(toggleAxes()));
	mActionListWidget->addAction(mShowCoordinateAxesAct);

	mObjectOrientationAct = new QAction(tr("Reverse Object"), this);
	mObjectOrientationAct->setCheckable(true);
	mShortcutManager->registerAction(mObjectOrientationAct, "Display Menu", "R");
	connect(mObjectOrientationAct, SIGNAL(triggered()), this->getActive(), SLOT(toggleObjectOrientation()));
	mActionListWidget->addAction(mObjectOrientationAct);

	mShowNormalsAct = new QAction(tr("Show &Normals"), this);
	mShowNormalsAct->setCheckable(true);
	mShortcutManager->registerAction(mShowNormalsAct, "Display Menu", "N");
	connect(mShowNormalsAct, SIGNAL(triggered()), this->getActive(), SLOT(toggleNormals()));
	mActionListWidget->addAction(mShowNormalsAct);

	mShowFaceCentroidsAct = new QAction(tr("Show &Face Centroids"), this);
	mShowFaceCentroidsAct->setCheckable(true);
	mShortcutManager->registerAction(mShowFaceCentroidsAct, "Display Menu", "C");
	// objectOrienationAct->setStatusTip(tr("Copy the current selection's contents to the "
	connect(mShowFaceCentroidsAct, SIGNAL(triggered()), this->getActive(), SLOT(toggleFaceCentroids()));
	mActionListWidget->addAction(mShowFaceCentroidsAct);

	mShowGridAct = new QAction(tr("Show &Grid"), this);
	mShowGridAct->setCheckable(true);
	mShowGridAct->setEnabled(false);
	mShortcutManager->registerAction(mShowGridAct, "Display Menu", "G");
	// showGridAct->setStatusTip(tr("Copy the current selection's contents to the "
	connect(mShowGridAct, SIGNAL(triggered()), getActive(), SLOT(toggleGrid()));
	mActionListWidget->addAction(mShowGridAct);

	mShowHUDAct = new QAction(tr("Show &Heads Up Display"), this);
	mShowHUDAct->setCheckable(true);
	mShortcutManager->registerAction(mShowHUDAct, "Display Menu", "H");
	mShowHUDAct->setStatusTip(tr("Show the Heads Up Display"));
	connect(mShowHUDAct, SIGNAL(triggered()), this->getActive(), SLOT(toggleHUD()));
	mActionListWidget->addAction(mShowHUDAct);

#ifdef GPU_OK
	mUseGPUAct = new QAction(tr("&Use GPU Shading"), this);
	mUseGPUAct->setCheckable(true);
	mUseGPUAct->setChecked(true);
	mShortcutManager->registerAction(mUseGPUAct, "Display Menu", "L");
	mUseGPUAct->setStatusTip(tr("Use GPU Shading"));
	connect(mUseGPUAct, SIGNAL(triggered()), this->getActive(), SLOT(toggleGPU()));
	mActionListWidget->addAction(mUseGPUAct);
#endif

	mAntialiasingAct = new QAction(tr("Toggle &Antialiasing"), this);
	mAntialiasingAct->setCheckable(true);
	mAntialiasingAct->setChecked(false);
	mShortcutManager->registerAction(mAntialiasingAct, "Display Menu", "K");
	mAntialiasingAct->setStatusTip(tr("Toggle Antialiasing"));
	connect(mAntialiasingAct, SIGNAL(triggered()), this->getActive(), SLOT(toggleAntialiasing()));
	mActionListWidget->addAction(mAntialiasingAct);

#ifdef WITH_PYTHON
	mShowScriptEditorAct = mScriptEditorDockWidget->toggleViewAction();//new QAction(tr("Show Script &Editor"), this);
	mShowScriptEditorAct->setStatusTip( tr("Show the script editor to execute DLFL commands") );
	mShortcutManager->registerAction(mShowScriptEditorAct, "Window Menu", "SHIFT+CTRL+E");
	mActionListWidget->addAction(mShowScriptEditorAct);
#endif

#ifdef WITH_VERSE
	mShowVerseDialogAct = mVerseDialogDockWidget->toggleViewAction();//new QAction(tr("Show Verse &Dialog"), this);
	// mShowVerseDialogAct->setText(// true);
	mShowVerseDialogAct->setStatusTip( tr("Show the verse dialog to view verse server connection status") );
	// connect(mShowVerseDialogAct, SIGNAL(triggered()), this, SLOT(showHideVerseDialog()));
	mShortcutManager->registerAction(mShowVerseDialogAct, "Window Menu", "SHIFT+CTRL+V");
	mActionListWidget->addAction(mShowVerseDialogAct);
#endif

	mShowToolOptionsAct = mToolOptionsDockWidget->toggleViewAction();
	mShowToolOptionsAct->setStatusTip(tr("Show the tool options window"));
	mShortcutManager->registerAction(mShowToolOptionsAct, "Window Menu", "O");
	mActionListWidget->addAction(mShowToolOptionsAct);

	mShowStartupDialogAct = mStartupDialogDockWidget->toggleViewAction();
	mShowStartupDialogAct->setStatusTip(tr("Show the startup screen with links to video tutorials"));
	mShortcutManager->registerAction(mShowStartupDialogAct, "Window Menu", "");
	mActionListWidget->addAction(mShowStartupDialogAct);

	mShowAnimatedHelpAct = mAnimatedHelpDockWidget->toggleViewAction();// new QAction(tr("Show Animated Hel&p"), this);
																	   // mShowAnimatedHelpAct->setCheckable(true);
	mShowAnimatedHelpAct->setStatusTip(tr("Show/Hide the animated help window"));
	// connect(mShowAnimatedHelpAct, SIGNAL(triggered()), this, SLOT(showHideAnimatedHelp()));
	mShortcutManager->registerAction(mShowAnimatedHelpAct, "Window Menu", "SHIFT+H");
	mActionListWidget->addAction(mShowAnimatedHelpAct);

	//Renderer Menu Actions
	mWireframeRendererAct = new QAction(tr("&Wireframe Renderer"), this);
	mWireframeRendererAct->setCheckable(true);
	mShortcutManager->registerAction(mWireframeRendererAct, "Renderer Menu", "1");
	mWireframeRendererAct->setStatusTip(tr("Switch the current renderer to Wireframe"));
	connect(mWireframeRendererAct, SIGNAL(triggered()), this, SLOT(useWireframeRenderer()));
	mActionListWidget->addAction(mWireframeRendererAct);

	mNormalRendererAct = new QAction(tr("&Normal Renderer"), this);
	mNormalRendererAct->setCheckable(true);
	mShortcutManager->registerAction(mNormalRendererAct, "Renderer Menu", "2");
	mNormalRendererAct->setStatusTip(tr("Switch the current renderer to Normal"));
	connect(mNormalRendererAct, SIGNAL(triggered()), this, SLOT(useNormalRenderer()));
	mActionListWidget->addAction(mNormalRendererAct);

	mLightedRendererAct = new QAction(tr("&Lighted Renderer"), this);
	mLightedRendererAct->setCheckable(true);
	mShortcutManager->registerAction(mLightedRendererAct, "Renderer Menu", "3");
	mLightedRendererAct->setStatusTip(tr("Switch the current renderer to Lighted"));
	connect(mLightedRendererAct, SIGNAL(triggered()), this, SLOT(useLightedRenderer()));
	mActionListWidget->addAction(mLightedRendererAct);

	mTexturedRendererAct = new QAction(tr("&Textured Renderer"), this);
	mTexturedRendererAct->setCheckable(true);
	mShortcutManager->registerAction(mTexturedRendererAct, "Renderer Menu", "4");
	mTexturedRendererAct->setStatusTip(tr("Switch the current renderer to Textured"));
	connect(mTexturedRendererAct, SIGNAL(triggered()), this, SLOT(useTexturedRenderer()));
	mActionListWidget->addAction(mTexturedRendererAct);

	mTexturedLightedAct = new QAction(tr("Te&xtured Lighted Renderer"), this);
	mTexturedLightedAct->setCheckable(true);
	mShortcutManager->registerAction(mTexturedLightedAct, "Renderer Menu", "5");
	mTexturedLightedAct->setStatusTip(tr("Switch the current renderer to Textured Lit"));
	connect(mTexturedLightedAct, SIGNAL(triggered()), this, SLOT(useTexturedLitRenderer()));
	mActionListWidget->addAction(mTexturedLightedAct);

	mPatchRendererAct = new QAction(tr("&Patch Renderer"), this);
	mPatchRendererAct->setCheckable(true);
	mShortcutManager->registerAction(mPatchRendererAct, "Renderer Menu", "6");
	mPatchRendererAct->setStatusTip(tr("Switch the current renderer to Patch"));
	connect(mPatchRendererAct, SIGNAL(triggered()), this, SLOT(usePatchRenderer()));
	mActionListWidget->addAction(mPatchRendererAct);

	mColorableRendererAct = new QAction(tr("&Colorable Renderer"), this);
	mColorableRendererAct->setCheckable(true);
	mShortcutManager->registerAction(mColorableRendererAct, "Renderer Menu", "7");
	mColorableRendererAct->setStatusTip(tr("Switch the current renderer to Colorable"));
	connect(mColorableRendererAct, SIGNAL(triggered()), this, SLOT(useColorableRenderer()));
	mActionListWidget->addAction(mColorableRendererAct);

	mRendererActionGroup = new QActionGroup(this);
	mRendererActionGroup->setExclusive(true);
	mRendererActionGroup->addAction(mWireframeRendererAct);
	mRendererActionGroup->addAction(mNormalRendererAct);
	mRendererActionGroup->addAction(mLightedRendererAct);
	mRendererActionGroup->addAction(mTexturedRendererAct);
	mRendererActionGroup->addAction(mTexturedLightedAct);
	mRendererActionGroup->addAction(mPatchRendererAct);
	mRendererActionGroup->addAction(mColorableRendererAct);
	mLightedRendererAct->setChecked(true);

	//PRIMITIVES MENU ACTIONS
	mPrimCubeAct = new QAction(QIcon(":/images/prim_cube.png"), tr("&Cube"), this);
	mShortcutManager->registerAction(mPrimCubeAct, "Primitives Menu", "CTRL+1");
	mPrimCubeAct->setStatusTip(tr("Load a Cube"));
	connect(mPrimCubeAct, SIGNAL(triggered()), this, SLOT(loadCube()));
	mActionListWidget->addAction(mPrimCubeAct);

	mPrimOctahedronAct = new QAction(QIcon(":/images/prim_octahedron.png"), tr("&Octahedron"), this);
	mShortcutManager->registerAction(mPrimOctahedronAct, "Primitives Menu", "CTRL+2");
	mPrimOctahedronAct->setStatusTip(tr("Load an octahedron"));
	connect(mPrimOctahedronAct, SIGNAL(triggered()), this, SLOT(loadOctahedron()));
	mActionListWidget->addAction(mPrimOctahedronAct);

	mPrimTetrahedronAct = new QAction(QIcon(":/images/prim_tetrahedron.png"), tr("&Tetrahedron"), this);
	mShortcutManager->registerAction(mPrimTetrahedronAct, "Primitives Menu", "CTRL+3");
	mPrimTetrahedronAct->setStatusTip(tr("Load a tetrahedron"));
	connect(mPrimTetrahedronAct, SIGNAL(triggered()), this, SLOT(loadTetrahedron()));
	mActionListWidget->addAction(mPrimTetrahedronAct);

	mPrimDodecahedronAct = new QAction(QIcon(":/images/prim_dodecahedron.png"), tr("&Dodecahedron"), this);
	mShortcutManager->registerAction(mPrimDodecahedronAct, "Primitives Menu", "CTRL+4");
	mPrimDodecahedronAct->setStatusTip(tr("Load a dodecahedron"));
	connect(mPrimDodecahedronAct, SIGNAL(triggered()), this, SLOT(loadDodecahedron()));
	mActionListWidget->addAction(mPrimDodecahedronAct);

	mPrimIcosahedronAct = new QAction(QIcon(":/images/prim_icosahedron.png"), tr("&Icosahedron"), this);
	mShortcutManager->registerAction(mPrimIcosahedronAct, "Primitives Menu", "CTRL+5");
	mPrimIcosahedronAct->setStatusTip(tr("Load an icosahedron"));
	connect(mPrimIcosahedronAct, SIGNAL(triggered()), this, SLOT(loadIcosahedron()));
	mActionListWidget->addAction(mPrimIcosahedronAct);

	mPrimSoccerBallAct = new QAction(QIcon(":/images/prim_soccerball.png"), tr("&Soccer ball"), this);
	mShortcutManager->registerAction(mPrimSoccerBallAct, "Primitives Menu", "CTRL+6");
	mPrimSoccerBallAct->setStatusTip(tr("Load a soccer ball"));
	connect(mPrimSoccerBallAct, SIGNAL(triggered()), this, SLOT(loadSoccerball()));
	mActionListWidget->addAction(mPrimSoccerBallAct);

	mPrimGeodesicAct = new QAction(QIcon(":/images/prim_geodesic.png"), tr("&Geodesic Dome"), this);
	mShortcutManager->registerAction(mPrimGeodesicAct, "Primitives Menu", "CTRL+7");
	mPrimGeodesicAct->setStatusTip(tr("Load a geodesic dome"));
	connect(mPrimGeodesicAct, SIGNAL(triggered()), this, SLOT(loadGeodesic()));
	mActionListWidget->addAction(mPrimGeodesicAct);

	//Object Menu Actions
	mSubdivideAllEdgesAct = new QAction(tr("Subdivide All &Edges"), this);
	mShortcutManager->registerAction(mSubdivideAllEdgesAct, "Tools", "CTRL+D");
	// subdivideAllEdgesAct->setStatusTip(tr("Copy the current selection's contents to the "
	connect(mSubdivideAllEdgesAct, SIGNAL(triggered()), this, SLOT(subdivideAllEdges()));
	mActionListWidget->addAction(mSubdivideAllEdgesAct);

	mPlanarizeAllFacesAct = new QAction(tr("Planarize All &Faces"), this);
	mShortcutManager->registerAction(mPlanarizeAllFacesAct, "Tools", "CTRL+P");
	// planarizeAllFacesAct->setStatusTip(tr("Copy the current selection's contents to the "
	connect(mPlanarizeAllFacesAct, SIGNAL(triggered()), this, SLOT(planarizeFaces()));
	mActionListWidget->addAction(mPlanarizeAllFacesAct);

	makeObjectSphericalAct = new QAction(tr("Make &Object Spherical"), this);
	mShortcutManager->registerAction(makeObjectSphericalAct, "Tools", "CTRL+H");
	// makeObjectSphericalAct->setStatusTip(tr("Copy the current selection's contents to the "
	connect(makeObjectSphericalAct, SIGNAL(triggered()), this, SLOT(spheralizeObject()));
	mActionListWidget->addAction(makeObjectSphericalAct);

	mCleanup2gonsAct = new QAction(tr("Cleanup 2-gons"), this);
	mShortcutManager->registerAction(mCleanup2gonsAct, "Tools", "");
	// mCleanup2gonsAct->setStatusTip(tr("Copy the current selection's contents to the "
	connect(mCleanup2gonsAct, SIGNAL(triggered()), this, SLOT(cleanup2gons()));
	mActionListWidget->addAction(mCleanup2gonsAct);

	mCleanupWingedVerticesAct = new QAction(tr("Remove valence-2 vertices"), this);
	mShortcutManager->registerAction(mCleanupWingedVerticesAct, "Tools", "SHIFT+CTRL+V");
	connect(mCleanupWingedVerticesAct, SIGNAL(triggered()), this, SLOT(cleanupWingedVertices()));
	mActionListWidget->addAction(mCleanupWingedVerticesAct);

	mSplitValence2VerticesAct = new QAction(tr("Split valence-2 vertices"), this);
	mShortcutManager->registerAction(mSplitValence2VerticesAct, "Tools", "");
	connect(mSplitValence2VerticesAct, SIGNAL(triggered()), this, SLOT(splitValence2Vertices()));
	mActionListWidget->addAction(mSplitValence2VerticesAct);

	mMakeObjectSmoothAct = new QAction(tr("Make Object &Smooth"), this);
	mShortcutManager->registerAction(mMakeObjectSmoothAct, "Tools", "CTRL+M");
	// makeObjectSmoothAct->setStatusTip(tr("Copy the current selection's contents to the "
	connect(mMakeObjectSmoothAct, SIGNAL(triggered()), this, SLOT(smoothMesh()));
	mActionListWidget->addAction(mMakeObjectSmoothAct);

	mPerformCuttingAct = new QAction(tr("Perform Cutting"), this);
	mShortcutManager->registerAction(mPerformCuttingAct, "Tools", "CTRL+T");
	mPerformCuttingAct->setStatusTip(tr("Cut selected faces, edges, or vertices based on the current selection mask"));
	connect(mPerformCuttingAct, SIGNAL(triggered()), this, SLOT(performCutting()));
	mActionListWidget->addAction(mPerformCuttingAct);

	mCreateCrustScalingAct = new QAction(tr("&Create Crust (Scaling)"), this);
	mShortcutManager->registerAction(mCreateCrustScalingAct, "Tools", "CTRL+ALT+C");
	mCreateCrustScalingAct->setStatusTip(tr("Create a crust using the currently selected faces with scaling mode"));
	connect(mCreateCrustScalingAct, SIGNAL(triggered()), this, SLOT(crustModeling4()));
	mActionListWidget->addAction(mCreateCrustScalingAct);

	mCreateCrustThicknessAct = new QAction(tr("Create Crust (&Thickness)"), this);
	mShortcutManager->registerAction(mCreateCrustThicknessAct, "Tools", "CTRL+C");
	mCreateCrustThicknessAct->setStatusTip(tr("Create a crust using the currently selected faces with thickness mode"));
	connect(mCreateCrustThicknessAct, SIGNAL(triggered()), this, SLOT(crustModeling3()));
	mActionListWidget->addAction(mCreateCrustThicknessAct);

	mMakeWireframeAct = new QAction(tr("Create Wireframe"), this);
	mShortcutManager->registerAction(mMakeWireframeAct, "Tools", "CTRL+W");
	mMakeWireframeAct->setStatusTip(tr("Create a wireframe model using the current options"));
	connect(mMakeWireframeAct, SIGNAL(triggered()), this, SLOT(makeWireframe()));
	mActionListWidget->addAction(mMakeWireframeAct);

	mMakeColumnsAct = new QAction(tr("Create Columns"), this);
	mShortcutManager->registerAction(mMakeColumnsAct, "Tools", "CTRL+ALT+W");
	mMakeColumnsAct->setStatusTip(tr("Create a column model using the current options"));
	connect(mMakeColumnsAct, SIGNAL(triggered()), this, SLOT(makeWireframeWithColumns()));
	mActionListWidget->addAction(mMakeColumnsAct);

	mMakeSierpinskiAct = new QAction(tr("Create Sierpinski"), this);
	mShortcutManager->registerAction(mMakeSierpinskiAct, "Tools", "CTRL+K");
	mMakeSierpinskiAct->setStatusTip(tr("Create a sierpinski tetrahedra"));
	connect(mMakeSierpinskiAct, SIGNAL(triggered()), this, SLOT(multiConnectMidpoints()));
	mActionListWidget->addAction(mMakeSierpinskiAct);

	mComputeLightingAct = new QAction(tr("Compute &Lighting"), this);
	mShortcutManager->registerAction(mComputeLightingAct, "Tools", "");
	// computeLightingAct->setStatusTip(tr("Copy the current selection's contents to the "
	connect(mComputeLightingAct, SIGNAL(triggered()), getActive(), SLOT(recomputeLighting()));
	mActionListWidget->addAction(mComputeLightingAct);

	mComputeNormalsAndLightingAct = new QAction(tr("Compute &Normals and Lighting"), this);
	mShortcutManager->registerAction(mComputeNormalsAndLightingAct, "Tools", "");
	// computeNormalsAndLightingAct->setStatusTip(tr("Copy the current selection's contents to the "
	connect(mComputeNormalsAndLightingAct, SIGNAL(triggered()), getActive(), SLOT(recomputeNormals()));
	mActionListWidget->addAction(mComputeNormalsAndLightingAct);

	mAssignTextureCoordinatesAct = new QAction(tr("Assign &Texture Coordinates"), this);
	mShortcutManager->registerAction(mAssignTextureCoordinatesAct, "Tools", "");
	// assignTextureCoordinatesAct->setStatusTip(tr("Copy the current selection's contents to the "
	connect(mAssignTextureCoordinatesAct, SIGNAL(triggered()), this, SLOT(assignTileTexCoords()));
	mActionListWidget->addAction(mAssignTextureCoordinatesAct);

	// //SELECTION MENU ACTIONS
	mSelectionWindowAct = new QAction(tr("Selection Window"), this);
	mShortcutManager->registerAction(mSelectionWindowAct, "Selection", "SHIFT+W");
	connect(mSelectionWindowAct, SIGNAL(triggered()), this, SLOT(selection_window()));
	mActionListWidget->addAction(mSelectionWindowAct);

	mSelectVertexAct = new QAction(tr("Select &Vertex"), this);
	mShortcutManager->registerAction(mSelectVertexAct, "Selection", "");
	mSelectVertexAct->setStatusTip(tr("Select a Vertex"));
	connect(mSelectVertexAct, SIGNAL(triggered()), this, SLOT(select_vertex()));
	mActionListWidget->addAction(mSelectVertexAct);

	mSelectMultipleVerticesAct = new QAction(tr("Select Multiple Vertices"), this);
	mShortcutManager->registerAction(mSelectMultipleVerticesAct, "Selection", "");
	mSelectMultipleVerticesAct->setStatusTip(tr("Select multiple vertices"));
	connect(mSelectMultipleVerticesAct, SIGNAL(triggered()), this, SLOT(select_multiple_vertices()));
	mActionListWidget->addAction(mSelectMultipleVerticesAct);

	mSelectMultipleAct = new QAction(tr("Select Multiple"), this);
	mShortcutManager->registerAction(mSelectMultipleAct, "Selection", "SHIFT+M");
	mSelectMultipleAct->setStatusTip(tr("Select multiple"));
	connect(mSelectMultipleAct, SIGNAL(triggered()), this, SLOT(select_multiple()));
	mActionListWidget->addAction(mSelectMultipleAct);

	mEditVertexAct = new QAction(tr("Edit Verte&x"), this);
	mShortcutManager->registerAction(mEditVertexAct, "Selection", "");
	mEditVertexAct->setStatusTip(tr("Select and Move Vertices one at a time."));
	connect(mEditVertexAct, SIGNAL(triggered()), this, SLOT(edit_vertex()));
	mActionListWidget->addAction(mEditVertexAct);

	mSelectFaceAct = new QAction(tr("Select &Face"), this);
	mShortcutManager->registerAction(mSelectFaceAct, "Selection", "");
	mSelectFaceAct->setStatusTip(tr("Select One Face. Just for practice. :)"));
	connect(mSelectFaceAct, SIGNAL(triggered()), this, SLOT(select_face()));
	mActionListWidget->addAction(mSelectFaceAct);

	mSelectFaceLoopAct = new QAction(tr("Select Face Loo&p"), this);
	mShortcutManager->registerAction(mSelectFaceLoopAct, "Selection", "SHIFT+P");
	mSelectFaceLoopAct->setStatusTip(tr("Select a Face Loop."));
	connect(mSelectFaceLoopAct, SIGNAL(triggered()), this, SLOT(select_face_loop()));
	mActionListWidget->addAction(mSelectFaceLoopAct);

	mSelectMultipleFacesAct = new QAction(tr("Select &Multiple Faces"), this);
	mShortcutManager->registerAction(mSelectMultipleFacesAct, "Selection", "");
	connect(mSelectMultipleFacesAct, SIGNAL(triggered()), this, SLOT(select_multiple_faces()));
	mActionListWidget->addAction(mSelectMultipleFacesAct);

	mSelectSimilarFacesAct = new QAction(tr("Select &Similar Faces"), this);
	mShortcutManager->registerAction(mSelectSimilarFacesAct, "Selection", "SHIFT+G");
	connect(mSelectSimilarFacesAct, SIGNAL(triggered()), this, SLOT(select_similar_faces()));
	mActionListWidget->addAction(mSelectSimilarFacesAct);

	mSelectSimilarAct = new QAction(tr("Select Similar"), this);
	mShortcutManager->registerAction(mSelectSimilarAct, "Selection", "SHIFT+S");
	connect(mSelectSimilarAct, SIGNAL(triggered()), this, SLOT(select_similar()));
	mActionListWidget->addAction(mSelectSimilarAct);

	mSelectFacesByAreaAct = new QAction(tr("Select Faces By Surf. Area"), this);
	mShortcutManager->registerAction(mSelectFacesByAreaAct, "Selection", "CTRL+SHIFT+A");
	connect(mSelectFacesByAreaAct, SIGNAL(triggered()), this, SLOT(select_faces_by_area()));
	mActionListWidget->addAction(mSelectFacesByAreaAct);

	mSelectFacesByColorAct = new QAction(tr("Select Faces By Color"), this);
	mShortcutManager->registerAction(mSelectFacesByColorAct, "Selection", "CTRL+SHIFT+C");
	connect(mSelectFacesByColorAct, SIGNAL(triggered()), this, SLOT(select_faces_by_color()));
	mActionListWidget->addAction(mSelectFacesByColorAct);

	mSelectCheckerboardFacesAct = new QAction(tr("C&heckerboard Select Faces"), this);
	mShortcutManager->registerAction(mSelectCheckerboardFacesAct, "Selection", "SHIFT+K");
	connect(mSelectCheckerboardFacesAct, SIGNAL(triggered()), this, SLOT(select_checkerboard_faces()));
	mActionListWidget->addAction(mSelectCheckerboardFacesAct);

	mSelectAllAct = new QAction(tr("Select &All"), this);
	mShortcutManager->registerAction(mSelectAllAct, "Selection", "CTRL+A");
	connect(mSelectAllAct, SIGNAL(triggered()), this, SLOT(selectAll()));
	mActionListWidget->addAction(mSelectAllAct);

	mGrowSelectionAct = new QAction(tr("Grow Selection"), this);
	mShortcutManager->registerAction(mGrowSelectionAct, "Selection", "=");
	connect(mGrowSelectionAct, SIGNAL(triggered()), this, SLOT(growSelection()));
	mActionListWidget->addAction(mGrowSelectionAct);

	mShrinkSelectionAct = new QAction(tr("Shrink Selection"), this);
	mShortcutManager->registerAction(mShrinkSelectionAct, "Selection", "-");
	connect(mShrinkSelectionAct, SIGNAL(triggered()), this, SLOT(shrinkSelection()));
	mActionListWidget->addAction(mShrinkSelectionAct);

	mSelectInverseAct = new QAction(tr("Select &Inverse"), this);
	mShortcutManager->registerAction(mSelectInverseAct, "Selection", "CTRL+I");
	connect(mSelectInverseAct, SIGNAL(triggered()), this, SLOT(selectInverse()));
	mActionListWidget->addAction(mSelectInverseAct);

	mSelectEdgeAct = new QAction(tr("Select &Edge"), this);
	mShortcutManager->registerAction(mSelectEdgeAct, "Selection", "");
	mSelectEdgeAct->setStatusTip(tr("Select one Edge"));
	connect(mSelectEdgeAct, SIGNAL(triggered()), this, SLOT(select_edge()));
	mActionListWidget->addAction(mSelectEdgeAct);

	mSelectMultipleEdgesAct = new QAction(tr("Select Multiple Edges"), this);
	mShortcutManager->registerAction(mSelectMultipleEdgesAct, "Selection", "");
	mSelectMultipleEdgesAct->setStatusTip(tr("Select multiple edges"));
	connect(mSelectMultipleEdgesAct, SIGNAL(triggered()), this, SLOT(select_multiple_edges()));
	mActionListWidget->addAction(mSelectMultipleEdgesAct);

	mCollapseSelectedEdgesAct = new QAction(tr("Collapse Selected Edges"), this);
	mShortcutManager->registerAction(mCollapseSelectedEdgesAct, "Edit Menu", "ALT+C");
	mCollapseSelectedEdgesAct->setStatusTip(tr("Collapse Selected Edges"));
	connect(mCollapseSelectedEdgesAct, SIGNAL(triggered()), this, SLOT(collapseSelectedEdges()));
	mActionListWidget->addAction(mCollapseSelectedEdgesAct);

	mSelectEdgeLoopAct = new QAction(tr("Select Edge &Loop"), this);
	mShortcutManager->registerAction(mSelectEdgeLoopAct, "Selection", "SHIFT+L");
	mSelectEdgeLoopAct->setStatusTip(tr("Select an Edge Loop"));
	connect(mSelectEdgeLoopAct, SIGNAL(triggered()), this, SLOT(select_edge_loop()));
	mActionListWidget->addAction(mSelectEdgeLoopAct);

	mSelectEdgeRingAct = new QAction(tr("Select Edge &Ring"), this);
	mShortcutManager->registerAction(mSelectEdgeRingAct, "Selection", "SHIFT+R");
	mSelectEdgeRingAct->setStatusTip(tr("Select an edge ring."));
	connect(mSelectEdgeRingAct, SIGNAL(triggered()), this, SLOT(select_edge_ring()));
	mActionListWidget->addAction(mSelectEdgeRingAct);

	mSelectCornerAct = new QAction(tr("Select &Corner"), this);
	mShortcutManager->registerAction(mSelectCornerAct, "Selection", "SHIFT+C");
	mSelectCornerAct->setStatusTip(tr("Select a Corner"));
	connect(mSelectCornerAct, SIGNAL(triggered()), this, SLOT(select_corner()));
	mActionListWidget->addAction(mSelectCornerAct);

	mExitSelectionModeAct = new QAction(tr("E&xit Selection Mode"), this);
	mShortcutManager->registerAction(mExitSelectionModeAct, "Selection", "SHIFT+X");
	connect(mExitSelectionModeAct, SIGNAL(triggered()), this, SLOT(exit_selection_mode()));
	mActionListWidget->addAction(mExitSelectionModeAct);

	mClearSelectedModeAct = new QAction(tr("&Clear Selected"), this);
	mShortcutManager->registerAction(mClearSelectedModeAct, "Selection", "Escape");
	connect(mClearSelectedModeAct, SIGNAL(triggered()), this, SLOT(clear_selected()));
	mActionListWidget->addAction(mClearSelectedModeAct);

	mSelectEdgesFromFacesAct = new QAction(tr("Select Edges from Faces"), this);
	mShortcutManager->registerAction(mSelectEdgesFromFacesAct, "Selection", "");
	connect(mSelectEdgesFromFacesAct, SIGNAL(triggered()), this, SLOT(selectEdgesFromFaces()));
	mActionListWidget->addAction(mSelectEdgesFromFacesAct);

	mSelectEdgesFromVerticesAct = new QAction(tr("Select Edges from Vertices"), this);
	mShortcutManager->registerAction(mSelectEdgesFromVerticesAct, "Selection", "");
	connect(mSelectEdgesFromVerticesAct, SIGNAL(triggered()), this, SLOT(selectEdgesFromVertices()));
	mActionListWidget->addAction(mSelectEdgesFromVerticesAct);

	mSelectFacesFromEdgesAct = new QAction(tr("Select Faces from Edges"), this);
	mShortcutManager->registerAction(mSelectFacesFromEdgesAct, "Selection", "");
	connect(mSelectFacesFromEdgesAct, SIGNAL(triggered()), this, SLOT(selectFacesFromEdges()));
	mActionListWidget->addAction(mSelectFacesFromEdgesAct);

	mSelectFacesFromVerticesAct = new QAction(tr("Select Faces from Vertices"), this);
	mShortcutManager->registerAction(mSelectFacesFromVerticesAct, "Selection", "");
	connect(mSelectFacesFromVerticesAct, SIGNAL(triggered()), this, SLOT(selectFacesFromVertices()));
	mActionListWidget->addAction(mSelectFacesFromVerticesAct);

	mSelectVerticesFromEdgesAct = new QAction(tr("Select Vertices from Edges"), this);
	mShortcutManager->registerAction(mSelectVerticesFromEdgesAct, "Selection", "");
	connect(mSelectVerticesFromEdgesAct, SIGNAL(triggered()), this, SLOT(selectVerticesFromEdges()));
	mActionListWidget->addAction(mSelectVerticesFromEdgesAct);

	mSelectVerticesFromFacesAct = new QAction(tr("Select Vertices from Faces"), this);
	mShortcutManager->registerAction(mSelectVerticesFromFacesAct, "Selection", "");
	connect(mSelectVerticesFromFacesAct, SIGNAL(triggered()), this, SLOT(selectVerticesFromFaces()));
	mActionListWidget->addAction(mSelectVerticesFromFacesAct);

	//selection masks
	mSelectVerticesMaskAct = new QAction(QIcon(":images/selection-mask-vertices.png"), tr("Select &Vertices"), this);
	mSelectVerticesMaskAct->setCheckable(true);
	mShortcutManager->registerAction(mSelectVerticesMaskAct, "Selection", "SHIFT+V");
	mSelectVerticesMaskAct->setStatusTip(tr("Select by Component type: Vertices"));
	connect(mSelectVerticesMaskAct, SIGNAL(triggered()), this, SLOT(selectionMaskVertices()));
	mActionListWidget->addAction(mSelectVerticesMaskAct);

	mSelectEdgesMaskAct = new QAction(QIcon(":images/selection-mask-edges.png"), tr("Select &Edges"), this);
	mSelectEdgesMaskAct->setCheckable(true);
	mShortcutManager->registerAction(mSelectEdgesMaskAct, "Selection", "SHIFT+E");
	mSelectEdgesMaskAct->setStatusTip(tr("Select by Component type: Edges"));
	connect(mSelectEdgesMaskAct, SIGNAL(triggered()), this, SLOT(selectionMaskEdges()));
	mActionListWidget->addAction(mSelectEdgesMaskAct);

	mSelectFacesMaskAct = new QAction(QIcon(":images/selection-mask-faces.png"), tr("Select &Faces"), this);
	mSelectFacesMaskAct->setCheckable(true);
	mShortcutManager->registerAction(mSelectFacesMaskAct, "Selection", "SHIFT+F");
	mSelectFacesMaskAct->setStatusTip(tr("Select by Component type: Faces"));
	connect(mSelectFacesMaskAct, SIGNAL(triggered()), this, SLOT(selectionMaskFaces()));
	mActionListWidget->addAction(mSelectFacesMaskAct);

	mSelectCornersMaskAct = new QAction(QIcon(":images/selection-mask-corners.png"), tr("Select &Corner"), this);
	mSelectCornersMaskAct->setCheckable(true);
	mShortcutManager->registerAction(mSelectCornersMaskAct, "Selection", "");
	mSelectCornersMaskAct->setStatusTip(tr("Select by Component type: Corners"));
	connect(mSelectCornersMaskAct, SIGNAL(triggered()), this, SLOT(selectionMaskCorners()));
	mActionListWidget->addAction(mSelectCornersMaskAct);

	mSelectionMaskActionGroup = new QActionGroup(this);
	mSelectionMaskActionGroup->setExclusive(true);
	mSelectionMaskActionGroup->addAction(mSelectVerticesMaskAct);
	mSelectionMaskActionGroup->addAction(mSelectFacesMaskAct);
	mSelectionMaskActionGroup->addAction(mSelectEdgesMaskAct);
	mSelectionMaskActionGroup->addAction(mSelectCornersMaskAct);
	// mSelectCornersMaskAct->setChecked(true);

	//SETTINGS ACTIONS
	mPreferencesAct = new QAction(QIcon(":images/preferences-system.png"), tr("&Preferences"), this);
	mShortcutManager->registerAction(mPreferencesAct, "Settings", "CTRL+,");
	mPreferencesAct->setStatusTip(tr("Open the Preferences Dialog"));
	connect(mPreferencesAct, SIGNAL(triggered()), this, SLOT(openPreferences()));
	mActionListWidget->addAction(mPreferencesAct);

	//LANGUAGE MENU BAR ACTIONS
	mEnglishAct = new QAction(tr("English"), this);
	mEnglishAct->setCheckable(true);
	connect(mEnglishAct, SIGNAL(triggered()), this, SLOT(setLanguageEnglish()));
	mShortcutManager->registerAction(mEnglishAct, "Languages", "CTRL+ALT+1");
	mActionListWidget->addAction(mEnglishAct);

	mSpanishAct = new QAction(tr("Spanish"), this);
	mSpanishAct->setCheckable(true);
	connect(mSpanishAct, SIGNAL(triggered()), this, SLOT(setLanguageSpanish()));
	mShortcutManager->registerAction(mSpanishAct, "Languages", "CTRL+ALT+2");
	mActionListWidget->addAction(mSpanishAct);

	mGermanAct = new QAction(tr("German"), this);
	mGermanAct->setCheckable(true);
	connect(mGermanAct, SIGNAL(triggered()), this, SLOT(setLanguageGerman()));
	mShortcutManager->registerAction(mGermanAct, "Languages", "CTRL+ALT+3");
	mActionListWidget->addAction(mGermanAct);

	mFrenchAct = new QAction(tr("French"), this);
	mFrenchAct->setCheckable(true);
	connect(mFrenchAct, SIGNAL(triggered()), this, SLOT(setLanguageFrench()));
	mShortcutManager->registerAction(mFrenchAct, "Languages", "CTRL+ALT+4");
	mActionListWidget->addAction(mFrenchAct);

	mItalianAct = new QAction(tr("Italian"), this);
	mItalianAct->setCheckable(true);
	connect(mItalianAct, SIGNAL(triggered()), this, SLOT(setLanguageItalian()));
	mShortcutManager->registerAction(mItalianAct, "Languages", "CTRL+ALT+5");
	mActionListWidget->addAction(mItalianAct);

	mTurkishAct = new QAction(tr("Turkish"), this);
	mTurkishAct->setCheckable(true);
	connect(mTurkishAct, SIGNAL(triggered()), this, SLOT(setLanguageTurkish()));
	mShortcutManager->registerAction(mTurkishAct, "Languages", "CTRL+ALT+6");
	mActionListWidget->addAction(mTurkishAct);

	mCatalanAct = new QAction(tr("Catalan"), this);
	mCatalanAct->setCheckable(true);
	connect(mCatalanAct, SIGNAL(triggered()), this, SLOT(setLanguageCatalan()));
	mShortcutManager->registerAction(mCatalanAct, "Languages", "CTRL+ALT+7");
	mActionListWidget->addAction(mCatalanAct);

	mHindiAct = new QAction(tr("Hindi"), this);
	mHindiAct->setCheckable(true);
	connect(mHindiAct, SIGNAL(triggered()), this, SLOT(setLanguageHindi()));
	mShortcutManager->registerAction(mHindiAct, "Languages", "CTRL+ALT+8");
	mActionListWidget->addAction(mHindiAct);

	mLanguageActionGroup = new QActionGroup(this);
	mLanguageActionGroup->setExclusive(true);
	mLanguageActionGroup->addAction(mEnglishAct);
	mLanguageActionGroup->addAction(mSpanishAct);
	mLanguageActionGroup->addAction(mGermanAct);
	mLanguageActionGroup->addAction(mFrenchAct);
	mLanguageActionGroup->addAction(mItalianAct);
	mLanguageActionGroup->addAction(mTurkishAct);
	mLanguageActionGroup->addAction(mCatalanAct);
	mLanguageActionGroup->addAction(mHindiAct);
	mEnglishAct->setChecked(true);

#ifdef WITH_VERSE
	//verse menu actions
	mVerseConnectLocalhostAct = new QAction(tr("Connect to localhost..."), this);
	mVerseConnectLocalhostAct->setStatusTip( tr("Connect to localhost") );
	connect(mVerseConnectLocalhostAct, SIGNAL(triggered()), mVerseDialog, SLOT(connectLocalhost()));
	mShortcutManager->registerAction(mVerseConnectLocalhostAct, "Verse Menu", "");
	mActionListWidget->addAction(mVerseConnectLocalhostAct);

	mVerseConnectAct = new QAction(tr("Connect to host..."), this);
	mVerseConnectAct->setStatusTip( tr("Connect to host...") );
	connect(mVerseConnectAct, SIGNAL(triggered()), mVerseDialog, SLOT(connectHost()));
	mShortcutManager->registerAction(mVerseConnectAct, "Verse Menu", "");
	mActionListWidget->addAction(mVerseConnectAct);

	mVerseDisconnectAct = new QAction(tr("Disconnect session"), this);
	mVerseDisconnectAct->setStatusTip( tr("Disconnect Verse Session") );
	connect(mVerseDisconnectAct, SIGNAL(triggered()), mVerseDialog, SLOT(disconnectHost()));
	mShortcutManager->registerAction(mVerseDisconnectAct, "Verse Menu", "");
	mActionListWidget->addAction(mVerseDisconnectAct);

	mVerseDisconnectAllAct = new QAction(tr("Disconnect All Sessions"), this);
	mVerseDisconnectAllAct->setStatusTip( tr("Disconnect All Sessions") );
	connect(mVerseDisconnectAllAct, SIGNAL(triggered()), mVerseDialog, SLOT(disconnectAll()));
	mShortcutManager->registerAction(mVerseDisconnectAllAct, "Verse Menu", "");
	mActionListWidget->addAction(mVerseDisconnectAllAct);

	mVerseStartServerAct = new QAction(tr("Start Verse Server"), this);
	mVerseStartServerAct->setStatusTip( tr("Disconnect All Nodes") );
	connect(mVerseStartServerAct, SIGNAL(triggered()), mVerseDialog, SLOT(startServer()));
	mShortcutManager->registerAction(mVerseStartServerAct, "Verse Menu", "");
	mActionListWidget->addAction(mVerseStartServerAct);

	mVerseKillServerAct = new QAction(tr("Kill Verse Server"), this);
	mVerseKillServerAct->setStatusTip( tr("Kill the Local Verse server process") );
	connect(mVerseKillServerAct, SIGNAL(triggered()), mVerseDialog, SLOT(killServer()));
	mShortcutManager->registerAction(mVerseKillServerAct, "Verse Menu", "");
	mActionListWidget->addAction(mVerseKillServerAct);
#endif

	mPaintSelectedFacesAct = new QAction(tr("Paint Selected Faces"), this);
	mPaintSelectedFacesAct->setStatusTip(tr("Paint all Selected Faces"));
	connect(mPaintSelectedFacesAct, SIGNAL(triggered()), this, SLOT(paintSelectedFaces()));
	mShortcutManager->registerAction(mPaintSelectedFacesAct, "Tools Menu", "CTRL+SHIFT+B");
	mActionListWidget->addAction(mPaintSelectedFacesAct);

	mClearMaterialsAct = new QAction(tr("Clear Materials"), this);
	mClearMaterialsAct->setStatusTip(tr("Clear Materials"));
	connect(mClearMaterialsAct, SIGNAL(triggered()), this, SLOT(clearMaterials()));
	mShortcutManager->registerAction(mClearMaterialsAct, "Tools Menu", "CTRL+SHIFT+X");
	mActionListWidget->addAction(mClearMaterialsAct);

	mSubdivideSelectedFacesAct = new QAction(tr("Subdivide Selected Faces"), this);
	mSubdivideSelectedFacesAct->setStatusTip(tr("Subdivide all Selected Faces"));
	connect(mSubdivideSelectedFacesAct, SIGNAL(triggered()), this, SLOT(subdivideSelectedFaces()));
	mShortcutManager->registerAction(mSubdivideSelectedFacesAct, "Tools Menu", "CTRL+B");
	mActionListWidget->addAction(mSubdivideSelectedFacesAct);

	mSubdivideSelectedEdgesAct = new QAction(tr("Subdivide Selected Edges"), this);
	mSubdivideSelectedEdgesAct->setStatusTip(tr("Subdivide all Selected Edges"));
	connect(mSubdivideSelectedEdgesAct, SIGNAL(triggered()), this, SLOT(subdivideSelectedEdges()));
	mShortcutManager->registerAction(mSubdivideSelectedEdgesAct, "Tools Menu", "CTRL+SHIFT+E");
	mActionListWidget->addAction(mSubdivideSelectedEdgesAct);

	mPerformRemeshingAct = new QAction(tr("Perform Remeshing"), this);
	mPerformRemeshingAct->setStatusTip(tr("Perform the current remeshing scheme"));
	connect(mPerformRemeshingAct, SIGNAL(triggered()), this, SLOT(performRemeshing()));
	mShortcutManager->registerAction(mPerformRemeshingAct, "Remeshing Menu", "CTRL+R");
	mActionListWidget->addAction(mPerformRemeshingAct);

	mPerformExtrusionAct = new QAction(tr("Perform Extrusion"), this);
	mPerformExtrusionAct->setStatusTip(tr("Perform the current extrusion operator on the selected faces"));
	connect(mPerformExtrusionAct, SIGNAL(triggered()), this, SLOT(performExtrusion()));
	mShortcutManager->registerAction(mPerformExtrusionAct, "Tools Menu", "CTRL+X");
	mActionListWidget->addAction(mPerformExtrusionAct);

	mExtrudeMultipleAct = new QAction(tr("Extrude Multiple Faces"), this);
	mExtrudeMultipleAct->setCheckable(true);
	mExtrudeMultipleAct->setChecked(false);
	mExtrudeMultipleAct->setStatusTip(tr("Check this if you want to be able to select multiple faces and then hit CTRL+X to extrude the selected faces"));
	// connect(mExtrudeMultipleAct, SIGNAL(triggered()), this, SLOT(performExtrusion()));
	mShortcutManager->registerAction(mExtrudeMultipleAct, "Tools Menu", "M");
	mActionListWidget->addAction(mExtrudeMultipleAct);

	//help menu actions
	mHelpAct = new QAction(QIcon(":images/applications-internet.png"), tr("&Online User Manual"), this);
	mHelpAct->setStatusTip(tr("View the User Manual on the TopMod Wiki"));
	connect(mHelpAct, SIGNAL(triggered()), this, SLOT(help()));
	mShortcutManager->registerAction(mHelpAct, "Help Menu", "F1");
	mActionListWidget->addAction(mHelpAct);

	mVideoTutorialsAct = new QAction(QIcon(":images/applications-internet.png"), tr("&TopMod Video Tutorials on blip.tv"), this);
	mVideoTutorialsAct->setStatusTip(tr("Go to http://topmod.blip.tv"));
	connect(mVideoTutorialsAct, SIGNAL(triggered()), this, SLOT(topModBlip()));
	mShortcutManager->registerAction(mVideoTutorialsAct, "Help Menu", "");
	mActionListWidget->addAction(mVideoTutorialsAct);

	mTopModWebAct = new QAction(QIcon(":images/applications-internet.png"), tr("&TopMod on the Web"), this);
	mTopModWebAct->setStatusTip(tr("Go to the TopMod web page"));
	connect(mTopModWebAct, SIGNAL(triggered()), this, SLOT(topModWeb()));
	mShortcutManager->registerAction(mTopModWebAct, "Help Menu", "");
	mActionListWidget->addAction(mTopModWebAct);

	mTopModResearchAct = new QAction(QIcon(":images/applications-internet.png"), tr("&TopMod Research Papers"), this);
	mTopModResearchAct->setStatusTip(tr("Go to the TopMod Research web page"));
	connect(mTopModResearchAct, SIGNAL(triggered()), this, SLOT(topModResearch()));
	mShortcutManager->registerAction(mTopModResearchAct, "Help Menu", "");
	mActionListWidget->addAction(mTopModResearchAct);

	mCheckForUpdatesAct = new QAction(QIcon(":images/applications-internet.png"), tr("&Check for Updates"), this);
	mCheckForUpdatesAct->setStatusTip(tr("Check for updates to TopMod Online"));
	connect(mCheckForUpdatesAct, SIGNAL(triggered()), this, SLOT(checkForUpdates()));
	mShortcutManager->registerAction(mCheckForUpdatesAct, "Help Menu", "");
	mActionListWidget->addAction(mCheckForUpdatesAct);

	mAboutAct = new QAction(tr("&About TopMod"), this);
	mAboutAct->setStatusTip(tr("About TopMod"));
	connect(mAboutAct, SIGNAL(triggered()), this, SLOT(about()));
	mShortcutManager->registerAction(mAboutAct, "Help Menu", "");
	mActionListWidget->addAction(mAboutAct);

	mAboutQtAct = new QAction(tr("About &Qt"), this);
	mAboutQtAct->setStatusTip(tr("About Qt"));
	connect(mAboutQtAct, SIGNAL(triggered()), qApp, SLOT(aboutQt()));
	mShortcutManager->registerAction(mAboutQtAct, "Help Menu", "");
	mActionListWidget->addAction(mAboutQtAct);

	mHideToolBarsAct = new QAction(tr("Hide All ToolBars"), this);
	mHideToolBarsAct->setStatusTip(tr("Hide All ToolBars"));
	connect(mHideToolBarsAct, SIGNAL(triggered()), this, SLOT(hideAllToolBars()));
	mShortcutManager->registerAction(mHideToolBarsAct, "Window Menu", "T");
	mActionListWidget->addAction(mHideToolBarsAct);

	mShowToolBarsAct = new QAction(tr("Show All ToolBars"), this);
	mShowToolBarsAct->setStatusTip(tr("Show All ToolBars"));
	connect(mShowToolBarsAct, SIGNAL(triggered()), this, SLOT(showAllToolBars()));
	mShortcutManager->registerAction(mShowToolBarsAct, "Window Menu", "SHIFT+T");
	mActionListWidget->addAction(mShowToolBarsAct);
}

void MainWindow::createMenus(){
#ifdef __APPLE__
	mMenuBar = new QMenuBar(0);
	setMenuBar(mMenuBar);
	// setUnifiedTitleAndToolBarOnMac(true);
#else
	mMenuBar = new QMenuBar(this);
	setMenuBar(mMenuBar);
#endif

	mFileMenu = new QMenu(tr("&File"));
	mMenuBar->addMenu(mFileMenu);
	mFileMenu->setTearOffEnabled(true);

	mNewMenu = new QMenu(tr("&New"));
	mNewMenu->setTearOffEnabled(true);
	mFileMenu->addMenu(mNewMenu);
	mNewMenu->addAction(mNewFileAct);
	mNewMenu->addAction(mPrimCubeAct);
	mNewMenu->addAction(mPrimOctahedronAct);
	mNewMenu->addAction(mPrimTetrahedronAct);
	mNewMenu->addAction(mPrimDodecahedronAct);
	mNewMenu->addAction(mPrimIcosahedronAct);
	mNewMenu->addAction(mPrimSoccerBallAct);
	mNewMenu->addAction(mPrimGeodesicAct);

	mExportMenu = new QMenu(tr("&Export"));
	mExportMenu->setIcon(QIcon(":images/document-save-as.png"));
	// mFileMenu->addAction(mNewFileAct);
	mFileMenu->addAction(mOpenAct);
	mFileMenu->addAction(mSaveAct);
	mFileMenu->addAction(mSaveAsAct);
	mFileMenu->addMenu(mExportMenu);
	mExportMenu->addAction(mSavePatchesAct);
	mExportMenu->addAction(mSaveLG3dAct);
	mExportMenu->addAction(mSaveLG3dSelectedAct);
	mExportMenu->addAction(mExportSTLAct);
	mFileMenu->addAction(mScreenshotViewportAct);
	mFileMenu->addAction(mScreenshotAppAct);
#ifdef WITH_VERSE
	mFileMenu->addSeparator();
	mVerseMenu = new QMenu(tr("&Verse"));
	mFileMenu->addMenu(mVerseMenu);
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
	mFileMenu->addSeparator();
	mFileMenu->addAction(mLoadTextureAct);
	// mFileMenu->addAction(printInfoAct);
	// mFileMenu->addAction(printFaceListAct);
	// mFileMenu->addAction(printVertexListAct);
	// mFileMenu->addAction(printEdgeListAct);
	// mFileMenu->addAction(mPrintCVListAct);
	mFileMenu->addSeparator();
	mFileMenu->addAction(mExitAct);

	mEditMenu = new QMenu(tr("&Edit"));
	mMenuBar->addMenu(mEditMenu);
	mEditMenu->addAction(mDeleteSelectedAct);
	mEditMenu->addSeparator();
	mEditMenu->addAction(mUndoAct);
	mEditMenu->addAction(mRedoAct);
	mEditMenu->setTearOffEnabled(true);
	mEditMenu->addSeparator();
	mEditMenu->addAction(mClearUndoListAct);
	mEditMenu->addSeparator();
	mEditMenu->addAction(mPreferencesAct);

	// mViewMenu = new QMenu(tr("&View"));
	// mViewMenu->setTearOffEnabled(true);
	// menuBar->addMenu(mViewMenu);
	// mViewMenu->addAction(mPerspViewAct);
	// mViewMenu->addAction(mTopViewAct);
	// mViewMenu->addAction(mBottomViewAct);
	// mViewMenu->addAction(mLeftViewAct);
	// mViewMenu->addAction(mRightViewAct);
	// mViewMenu->addAction(mFrontViewAct);
	// mViewMenu->addAction(mBackViewAct);

	mDisplayMenu = new QMenu(tr("&Display"));
	mDisplayMenu->setTearOffEnabled(true);
	mMenuBar->addMenu(mDisplayMenu);
	//this is now a submenu of Display to make everything on the menu bar more compact
	mRendererMenu = new QMenu(tr("&Renderer"));
	mRendererMenu->setTearOffEnabled(true);
	// menuBar->addMenu(rendererMenu);
	mDisplayMenu->addMenu(mRendererMenu);
	mRendererMenu->addAction(mWireframeRendererAct);
	mRendererMenu->addAction(mNormalRendererAct);
	mRendererMenu->addAction(mLightedRendererAct);
	mRendererMenu->addAction(mTexturedRendererAct);
	// texturedRendererAct->setEnabled(false);
	mRendererMenu->addAction(mTexturedLightedAct);
	// texturedLightedAct->setEnabled(false);
	mRendererMenu->addSeparator()->setText(tr("Special Mode??"));
	mRendererMenu->addAction(mPatchRendererAct);
	mRendererMenu->addAction(mColorableRendererAct);

	mDisplayMenu->addAction(mPerspViewAct);
	mDisplayMenu->addAction(mShowVerticesAct);
	//ID display submenu
	mShowIDsMenu = new QMenu(tr("&Show IDs"));
	mDisplayMenu->addMenu(mShowIDsMenu);
	mShowIDsMenu->setTearOffEnabled(true);
	mShowIDsMenu->addAction(mShowFaceIDsAct);
	mShowIDsMenu->addAction(mShowEdgeIDsAct);
	mShowIDsMenu->addAction(mShowVertexIDsAct);
	mShowIDsMenu->addAction(mShowSelectedIDsAct);
	//more view options
	mDisplayMenu->addAction(mShowSilhouetteAct);
	mDisplayMenu->addAction(mShowWireframeAct);
	mDisplayMenu->addAction(mShowCoordinateAxesAct);
	// mDisplayMenu->addAction(showGridAct); //removed for now
	mDisplayMenu->addAction(mShowHUDAct);
	#ifdef GPU_OK
	mDisplayMenu->addAction(mUseGPUAct);
	#endif
	mDisplayMenu->addAction(mAntialiasingAct);
	mDisplayMenu->addAction(mObjectOrientationAct);
	mDisplayMenu->addAction(mShowNormalsAct);
	mDisplayMenu->addAction(mShowFaceCentroidsAct);
	mDisplayMenu->addAction(mFullscreenAct);
	mDisplayMenu->addAction(mZoomInAct);
	mDisplayMenu->addAction(mZoomOutAct);

	mPrimitivesMenu = new QMenu(tr("&Primitives"));
	mPrimitivesMenu->setTearOffEnabled(true);
	mMenuBar->addMenu(mPrimitivesMenu);
	mPrimitivesMenu->addAction(mPrimCubeAct);
	mPrimitivesMenu->addAction(mPrimOctahedronAct);
	mPrimitivesMenu->addAction(mPrimTetrahedronAct);
	mPrimitivesMenu->addAction(mPrimDodecahedronAct);
	mPrimitivesMenu->addAction(mPrimIcosahedronAct);
	mPrimitivesMenu->addAction(mPrimSoccerBallAct);
	mPrimitivesMenu->addAction(mPrimGeodesicAct);

	mSelectionMenu = new QMenu(tr("&Selection"));
	mMenuBar->addMenu(mSelectionMenu);
	mSelectionMenu->addAction(mSelectAllAct);
	mSelectionMenu->addAction(mSelectInverseAct);
	mSelectionMenu->addAction(mSelectMultipleAct);
	mSelectionMenu->addAction(mSelectSimilarAct);
	mSelectionMenu->addAction(mSelectionWindowAct);
	mSelectionMenu->addAction(mGrowSelectionAct);
	mSelectionMenu->addAction(mShrinkSelectionAct);
	// mSelectionMenu->addAction(exitSelectionModeAct);
	mSelectionMenu->addAction(mClearSelectedModeAct);
	mSelectionMenu->addSeparator();
	// mSelectionMenu->addAction(selectEdgeAct);
	// mSelectionMenu->addAction(selectMultipleEdgesAct);
	mSelectionMenu->addAction(mSelectEdgeLoopAct);
	mSelectionMenu->addAction(mSelectEdgeRingAct);
	// mSelectionMenu->addAction(mCollapseSelectedEdgesAct);
	// mSelectionMenu->addAction(mSubdivideSelectedEdgesAct);
	mSelectionMenu->addAction(mSelectEdgesFromFacesAct);
	mSelectionMenu->addAction(mSelectEdgesFromVerticesAct);
	mSelectionMenu->addSeparator();
	// mSelectionMenu->addAction(selectFaceAct);
	mSelectionMenu->addAction(mSelectFaceLoopAct);
	// mSelectionMenu->addAction(selectMultipleFacesAct);
	// mSelectionMenu->addAction(selectSimilarFacesAct);
	mSelectionMenu->addAction(mSelectFacesByAreaAct);
	mSelectionMenu->addAction(mSelectFacesByColorAct);
	mSelectionMenu->addAction(mSelectCheckerboardFacesAct);
	mSelectionMenu->addAction(mSelectFacesFromEdgesAct);
	mSelectionMenu->addAction(mSelectFacesFromVerticesAct);
	mSelectionMenu->addSeparator();
	// mSelectionMenu->addAction(selectVertexAct);
	// mSelectionMenu->addAction(selectMultipleVerticesAct);
	mSelectionMenu->addAction(mEditVertexAct);
	mSelectionMenu->addAction(mSelectVerticesFromFacesAct);
	mSelectionMenu->addAction(mSelectVerticesFromEdgesAct);
	// mSelectionMenu->addAction(selectCornerAct);
	// mSelectionMenu->addSeparator();

	mToolsMenu = new QMenu(tr("&Tools"));
	mToolsMenu->setTearOffEnabled(true);
	mToolsMenu->addMenu(mBasicsMode->getMenu());
	mToolsMenu->addMenu(mExtrusionsMode->getMenu());
	mToolsMenu->addMenu(mConicalMode->getMenu());
	mToolsMenu->addMenu(mHighgenusMode->getMenu());
	mToolsMenu->addMenu(mTexturingMode->getMenu());
	mToolsMenu->addMenu(mExperimentalMode->getMenu());
	#ifdef QCOMPLETER
	mToolsMenu->addAction(mQuickCommandAct);
	#endif
	// mToolsMenu->addAction(mExtrudeMultipleAct);
	mToolsMenu->addAction(mPerformExtrusionAct);
	mToolsMenu->addAction(mSubdivideSelectedFacesAct);
	mToolsMenu->addAction(mSubdivideSelectedEdgesAct);
	mToolsMenu->addAction(mPaintSelectedFacesAct);
	mMenuBar->addMenu(mToolsMenu);

	mRemeshingMenu = mRemeshingMode->getMenu();
	mRemeshingMenu->addAction(mPerformRemeshingAct);
	mRemeshingMenu->setTearOffEnabled(true);
	mMenuBar->addMenu(mRemeshingMenu);

	mObjectMenu = new QMenu(tr("&Object"));
	mObjectMenu->setTearOffEnabled(true);
	mToolsMenu->addMenu(mObjectMenu);
	mObjectMenu->addAction(mMakeWireframeAct);
	mObjectMenu->addAction(mMakeColumnsAct);
	mObjectMenu->addAction(mMakeSierpinskiAct);
	mObjectMenu->addAction(mCreateCrustThicknessAct);
	mObjectMenu->addAction(mCreateCrustScalingAct);
	mObjectMenu->addAction(mPerformCuttingAct);
	mObjectMenu->addAction(mSubdivideAllEdgesAct);
	mObjectMenu->addAction(mPlanarizeAllFacesAct);
	mObjectMenu->addAction(makeObjectSphericalAct);
	mObjectMenu->addAction(mMakeObjectSmoothAct);
	mObjectMenu->addSeparator();
	mObjectMenu->addAction(mCleanupWingedVerticesAct);
	mObjectMenu->addAction(mCleanup2gonsAct);
	mObjectMenu->addAction(mSplitValence2VerticesAct);
	mObjectMenu->addSeparator();
	mToolsMenu->addAction(mClearMaterialsAct);
	mObjectMenu->addAction(mComputeLightingAct);
	mObjectMenu->addAction(mComputeNormalsAndLightingAct);
	mObjectMenu->addAction(mAssignTextureCoordinatesAct);

	//selection Masks (vertices / edges / faces / face-vertices)
	mSelectionMaskMenu = new QMenu(tr("Selection &Masks"));
	mSelectionMenu->addMenu(mSelectionMaskMenu);
	mSelectionMaskMenu->addAction(mSelectVerticesMaskAct);
	mSelectionMaskMenu->addAction(mSelectFacesMaskAct);
	mSelectionMaskMenu->addAction(mSelectEdgesMaskAct);
	mSelectionMaskMenu->addAction(mSelectCornersMaskAct);

	mWindowMenu = new QMenu(tr("&Window"));
	mMenuBar->addMenu(mWindowMenu);
	mWindowMenu->addAction(mShowStartupDialogAct);
	mWindowMenu->addAction(mShowToolOptionsAct);
	#ifdef WITH_PYTHON
	mWindowMenu->addAction(mShowScriptEditorAct);
	#endif
	mWindowMenu->addAction(mShowAnimatedHelpAct);
	#ifdef WITH_VERSE
	mWindowMenu->addAction(mShowVerseDialogAct);
	#endif
	// mWindowMenu->addAction(mShowToolBarsAct);
	// mWindowMenu->addAction(mHideToolBarsAct);
	mWindowMenu->addSeparator();
	mWindowMenu->addAction(mEditToolBarAct);
	mWindowMenu->addAction(mSelectionMaskToolBarAct);
	mWindowMenu->addAction(mPrimitivesToolBarAct);
	mWindowMenu->addAction(mToolsToolBarAct);
	mWindowMenu->addAction(mExtrusionToolBarAct);
	mWindowMenu->addAction(mConicalToolBarAct);
	mWindowMenu->addAction(mHighgenusToolBarAct);
	mWindowMenu->addAction(mTexturingToolBarAct);
	mWindowMenu->addAction(mExperimentalToolBarAct);
	mWindowMenu->addAction(mRemeshingToolBarAct);

	mHelpMenu = new QMenu(tr("&Help"));
	mMenuBar->addMenu(mHelpMenu);
	mHelpMenu->addAction(mHelpAct);
	mHelpMenu->addAction(mCheckForUpdatesAct);
	mHelpMenu->addAction(mTopModWebAct);
	mHelpMenu->addAction(mVideoTutorialsAct);
	mHelpMenu->addAction(mTopModResearchAct);
	mHelpMenu->addSeparator();
	mLanguageMenu = new QMenu(tr("&Language"));
	// mLanguageMenu->setTearOffEnabled(true);
	mHelpMenu->addMenu(mLanguageMenu);
	mLanguageMenu->addAction(mEnglishAct);
	mLanguageMenu->addAction(mSpanishAct);
	mLanguageMenu->addAction(mGermanAct);
	mLanguageMenu->addAction(mFrenchAct);
	mLanguageMenu->addAction(mItalianAct);
	mLanguageMenu->addAction(mTurkishAct);
	mLanguageMenu->addAction(mCatalanAct);
	mLanguageMenu->addAction(mHindiAct);
	mHelpMenu->addSeparator();
	mHelpMenu->addAction(mAboutAct);
	mHelpMenu->addAction(mAboutQtAct);

	mRightClickMenu = new QMenu();

}

void MainWindow::createToolBars() {

	mEditToolBar = new QToolBar(tr("Edit"),this);
	//mEditToolBar->setFloatable(true);
	addToolBar(Qt::TopToolBarArea,mEditToolBar);
	//mEditToolBar->setFloatable(true);
	mEditToolBar->addAction(mNewFileAct);
	mEditToolBar->addAction(mOpenAct);
	mEditToolBar->addAction(mSaveAsAct);
	mEditToolBar->addAction(mUndoAct);
	mEditToolBar->addAction(mRedoAct);
	mEditToolBar->addAction(mScreenshotViewportAct);
	mEditToolBar->addAction(mScreenshotAppAct);
	mEditToolBar->setIconSize(QSize(32,32));

	//selection masks toolbar
	mSelectionMaskToolBar = new QToolBar(tr("Selection Masks"));
	// mSelectionMaskToolBar->hide();
	//mSelectionMaskToolBar->setFloatable(true);
	addToolBar(Qt::TopToolBarArea,mSelectionMaskToolBar);
	mSelectionMaskToolBar->addAction(mSelectVerticesMaskAct);
	mSelectionMaskToolBar->addAction(mSelectFacesMaskAct);
	mSelectionMaskToolBar->addAction(mSelectEdgesMaskAct);
	mSelectionMaskToolBar->addAction(mSelectCornersMaskAct);
	// mSelectionMaskToolBar->setOrientation(Qt::Vertical);

	mPrimitivesToolBar = new QToolBar(tr("Primitives"),this);
	//mPrimitivesToolBar->setFloatable(true);
	addToolBar(Qt::TopToolBarArea,mPrimitivesToolBar);
	mPrimitivesToolBar->setIconSize(QSize(32,32));
	mPrimitivesToolBar->addAction(mPrimCubeAct);
	mPrimitivesToolBar->addAction(mPrimOctahedronAct);
	mPrimitivesToolBar->addAction(mPrimTetrahedronAct);
	mPrimitivesToolBar->addAction(mPrimDodecahedronAct);
	mPrimitivesToolBar->addAction(mPrimIcosahedronAct);
	mPrimitivesToolBar->addAction(mPrimSoccerBallAct);
	mPrimitivesToolBar->addAction(mPrimGeodesicAct);

	//basic tools - six buttons
	mToolsToolBar = new QToolBar(tr("Tools"),this);
	//mToolsToolBar->setFloatable(true);
	addToolBar(Qt::TopToolBarArea,mToolsToolBar);
	mToolsToolBar->setIconSize(QSize(32,32));

	addToolBarBreak();

	mExtrusionToolBar = new QToolBar(tr("Extrusion Tools"),this);
	//mExtrusionToolBar->setFloatable(true);
	addToolBar(Qt::TopToolBarArea,mExtrusionToolBar);
	mExtrusionToolBar->setIconSize(QSize(32,32));

	mConicalToolBar = new QToolBar(tr("Conical Tools"),this);
	addToolBar(Qt::TopToolBarArea,mConicalToolBar);
	mConicalToolBar->setIconSize(QSize(32,32));

	mHighgenusToolBar = new QToolBar(tr("High Genus Tools"),this);
	//mHighgenusToolBar->setFloatable(true);
	addToolBar(Qt::TopToolBarArea,mHighgenusToolBar);
	mHighgenusToolBar->setIconSize(QSize(32,32));

	mExperimentalToolBar = new QToolBar(tr("Experimental Tools"),this);
	//mHighgenusToolBar->setFloatable(true);
	addToolBar(Qt::TopToolBarArea,mExperimentalToolBar);
	mExperimentalToolBar->setIconSize(QSize(32,32));

	mTexturingToolBar = new QToolBar(tr("Texturing Tools"),this);
	//mTexturingToolBar->setFloatable(true);
	addToolBar(Qt::TopToolBarArea,mTexturingToolBar);
	mTexturingToolBar->setIconSize(QSize(32,32));

	mRemeshingToolBar = new QToolBar(tr("Remeshing Tools"),this);
	mRemeshingToolBar->setAllowedAreas(Qt::RightToolBarArea);
	mRemeshingToolBar->setOrientation(Qt::Vertical);
	mRemeshingToolBar->setToolButtonStyle(Qt::ToolButtonTextOnly);
	//mRemeshingToolBar->setFloatable(false);
	// mRemeshingToolBar->setFloating(true);
	addToolBar(Qt::RightToolBarArea,mRemeshingToolBar);
	// mRemeshingToolBar->setIconSize(QSize(16,16));
	mRemeshingToolBar->hide();

	//tools ction group initialization
	mToolsActionGroup = new QActionGroup(this);
	mToolsActionGroup->setExclusive(true);

	mBasicsMode->addActions(mToolsActionGroup, mToolsToolBar, mToolOptionsStackedWidget);
	mExtrusionsMode->addActions(mToolsActionGroup, mExtrusionToolBar, mToolOptionsStackedWidget);
	mConicalMode->addActions(mToolsActionGroup, mConicalToolBar, mToolOptionsStackedWidget);

	mRemeshingActionGroup = new QActionGroup(this);
	mRemeshingMode->addActions(mToolsActionGroup, mRemeshingToolBar, mToolOptionsStackedWidget);

	mHighgenusMode->addActions(mToolsActionGroup, mHighgenusToolBar, mToolOptionsStackedWidget);
	mTexturingMode->addActions(mToolsActionGroup, mTexturingToolBar, mToolOptionsStackedWidget);
	mExperimentalMode->addActions(mToolsActionGroup, mExperimentalToolBar, mToolOptionsStackedWidget);

	//window menu toolbar display actions
	mEditToolBarAct = mEditToolBar->toggleViewAction();
	mSelectionMaskToolBarAct = mSelectionMaskToolBar->toggleViewAction();
	mPrimitivesToolBarAct = mPrimitivesToolBar->toggleViewAction();
	mToolsToolBarAct = mToolsToolBar->toggleViewAction();
	mExtrusionToolBarAct = mExtrusionToolBar->toggleViewAction();
	mConicalToolBarAct = mConicalToolBar->toggleViewAction();
	mHighgenusToolBarAct = mHighgenusToolBar->toggleViewAction();
	mTexturingToolBarAct = mTexturingToolBar->toggleViewAction();
	mExperimentalToolBarAct = mExperimentalToolBar->toggleViewAction();
	mRemeshingToolBarAct = mRemeshingToolBar->toggleViewAction();

}

void MainWindow::createStartupDialog(){

	//startup screen - links to video tutorials and free quicktime download

	// mStartupDialogDockWidget->setFeatures(QDockWidget::DockWidgetFloatable | QDockWidget::DockWidgetMovable /* | QDockWidget::DockWidgetClosable*/);
	mStartupDialogDockWidget->setAllowedAreas(Qt::NoDockWidgetArea);
	mStartupDialogDockWidget->hide();

	mStartupDialogWidget = new QWidget;
	mStartupDialogLayout = new QGridLayout;

	//actions for the startup screen
	mTutorialNavigationAct = new QAction(QIcon(":/images/tutorial_01.png"),tr("Navigation Basics"), this);
	connect(mTutorialNavigationAct, SIGNAL(triggered()), this, SLOT(loadNavigationTutorial()));
	mTutorialNavigationButton = new QToolButton;
	mTutorialNavigationButton->setFixedWidth(200);
	mTutorialNavigationButton->setIconSize(QSize(48,48));
	mTutorialNavigationButton->setDefaultAction(mTutorialNavigationAct);
	mTutorialNavigationButton->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
	mStartupDialogLayout->addWidget(mTutorialNavigationButton,1,0);

	mTutorialInterfaceAct = new QAction(QIcon(":/images/tutorial_02.png"),tr("Interface Basics"), this);
	connect(mTutorialInterfaceAct, SIGNAL(triggered()), this, SLOT(loadInterfaceTutorial()));
	mTutorialInterfaceButton = new QToolButton;
	mTutorialInterfaceButton->setFixedWidth(200);
	mTutorialInterfaceButton->setIconSize(QSize(48,48));
	mTutorialInterfaceButton->setDefaultAction(mTutorialInterfaceAct);
	mTutorialInterfaceButton->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
	mStartupDialogLayout->addWidget(mTutorialInterfaceButton,2,0);

	mTutorialBasicAct = new QAction(QIcon(":/images/tutorial_03.png"),tr("Basic Operations"), this);
	connect(mTutorialBasicAct, SIGNAL(triggered()), this, SLOT(loadBasicTutorial()));
	mTutorialBasicButton = new QToolButton;
	mTutorialBasicButton->setFixedWidth(200);
	mTutorialBasicButton->setIconSize(QSize(48,48));
	mTutorialBasicButton->setDefaultAction(mTutorialBasicAct);
	mTutorialBasicButton->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
	mStartupDialogLayout->addWidget(mTutorialBasicButton,3,0);

	mTutorialExtrusionAct = new QAction(QIcon(":/images/tutorial_04.png"),tr("Extrusion Operations"), this);
	connect(mTutorialExtrusionAct, SIGNAL(triggered()), this, SLOT(loadExtrusionTutorial()));
	mTutorialExtrusionButton = new QToolButton;
	mTutorialExtrusionButton->setFixedWidth(200);
	mTutorialExtrusionButton->setIconSize(QSize(48,48));
	mTutorialExtrusionButton->setDefaultAction(mTutorialExtrusionAct);
	mTutorialExtrusionButton->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
	mStartupDialogLayout->addWidget(mTutorialExtrusionButton,4,0);

	mTutorialRemeshingAct = new QAction(QIcon(":/images/tutorial_05.png"),tr("Remeshing Operations"), this);
	connect(mTutorialRemeshingAct, SIGNAL(triggered()), this, SLOT(loadRemeshingTutorial()));
	mTutorialRemeshingButton = new QToolButton;
	mTutorialRemeshingButton->setFixedWidth(200);
	mTutorialRemeshingButton->setIconSize(QSize(48,48));
	mTutorialRemeshingButton->setDefaultAction(mTutorialRemeshingAct);
	mTutorialRemeshingButton->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
	mStartupDialogLayout->addWidget(mTutorialRemeshingButton,1,1);

	mTutorialHighgenusAct = new QAction(QIcon(":/images/tutorial_06.png"),tr("High genus Operations"), this);
	connect(mTutorialHighgenusAct, SIGNAL(triggered()), this, SLOT(loadHighgenusTutorial()));
	mTutorialHighgenusButton = new QToolButton;
	mTutorialHighgenusButton->setFixedWidth(200);
	mTutorialHighgenusButton->setIconSize(QSize(48,48));
	mTutorialHighgenusButton->setDefaultAction(mTutorialHighgenusAct);
	mTutorialHighgenusButton->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
	mStartupDialogLayout->addWidget(mTutorialHighgenusButton,2,1);

	mTutorialTexturingAct = new QAction(QIcon(":/images/tutorial_07.png"),tr("Texturing Operations"), this);
	connect(mTutorialTexturingAct, SIGNAL(triggered()), this, SLOT(loadTexturingTutorial()));
	mTutorialTexturingButton = new QToolButton;
	mTutorialTexturingButton->setFixedWidth(200);
	mTutorialTexturingButton->setIconSize(QSize(48,48));
	mTutorialTexturingButton->setDefaultAction(mTutorialTexturingAct);
	mTutorialTexturingButton->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
	mStartupDialogLayout->addWidget(mTutorialTexturingButton,3,1);

	//show on startup? checkbox
	mShowStartupDialogAtStartupCheckBox = new QCheckBox(tr("Show this dialog at startup"));
	mShowStartupDialogAtStartupCheckBox->setChecked(mShowStartupDialogAtStartup);
	connect(mShowStartupDialogAtStartupCheckBox, SIGNAL(stateChanged(int)), this, SLOT(setShowStartupDialogAtStartup(int)));
	mStartupDialogLayout->addWidget(mShowStartupDialogAtStartupCheckBox,5,0);

	//download quicktime link label
	mQuickTimeLabel = new QLabel(tr("Quicktime 7.2 or greater recommended."));
	mDownloadQuickTimeLabel = new QLabel(tr(
		"<h5>Quicktime 7.2 or greater recommended.<br />"\
		"<a href=\"http://apple.com/quicktime/\">Download now.</a></h5>"));
	mDownloadQuickTimeLabel->setOpenExternalLinks(true);
	mStartupDialogLayout->addWidget(mDownloadQuickTimeLabel,5,1, Qt::AlignRight);

	mStartupDialogLayout->setColumnStretch(2,1);
	mStartupDialogLayout->setRowStretch(6,1);
	mStartupDialogWidget->setLayout(mStartupDialogLayout);
	mStartupDialogDockWidget->setWidget(mStartupDialogWidget);
	mStartupDialogDockWidget->setFloating(true);
	mStartupDialogDockWidget->move(
		this->width() / 2 + this->x() - mStartupDialogDockWidget->width() / 2,
		this->y() + height() / 2 - mStartupDialogDockWidget->height() / 2);
	if (!mShowStartupDialogAtStartup)
		mStartupDialogDockWidget->hide();
}

void MainWindow::setShowStartupDialogAtStartup(int b){
	mShowStartupDialogAtStartup = (bool)b;
}

bool MainWindow::getShowStartupDialogAtStartup(){
	return mShowStartupDialogAtStartup;
}

void MainWindow::loadNavigationTutorial(){
	QDesktopServices::openUrl(QUrl::fromLocalFile("video/tutorial_navigation.mov"));
}

void MainWindow::loadInterfaceTutorial(){
	QDesktopServices::openUrl(QUrl::fromLocalFile("video/tutorial_interface.mov"));
}

void MainWindow::loadBasicTutorial(){
	QDesktopServices::openUrl(QUrl::fromLocalFile("video/tutorial_basic.mov"));
}

void MainWindow::loadExtrusionTutorial(){
	QDesktopServices::openUrl(QUrl::fromLocalFile("video/tutorial_extrusion.mov"));
}

void MainWindow::loadRemeshingTutorial(){
	QDesktopServices::openUrl(QUrl::fromLocalFile("video/tutorial_remeshing.mov"));
}

void MainWindow::loadHighgenusTutorial(){
	QDesktopServices::openUrl(QUrl::fromLocalFile("video/tutorial_highgenus.mov"));
}

void MainWindow::loadTexturingTutorial(){
	QDesktopServices::openUrl(QUrl::fromLocalFile("video/tutorial_texturing.mov"));
}

void MainWindow::showAllToolBars(){
	mEditToolBar->show();
	mSelectionMaskToolBar->show();
	mPrimitivesToolBar->show();
	mToolsToolBar->show();
	mExtrusionToolBar->show();
	mConicalToolBar->show();
	mHighgenusToolBar->show();
	mTexturingToolBar->show();
	mExperimentalToolBar->show();
	mRemeshingToolBar->show();

}

void MainWindow::hideAllToolBars(){
	mEditToolBar->hide();
	mSelectionMaskToolBar->hide();
	mPrimitivesToolBar->hide();
	mToolsToolBar->hide();
	mExtrusionToolBar->hide();
	mConicalToolBar->hide();
	mHighgenusToolBar->hide();
	mTexturingToolBar->hide();
	mExperimentalToolBar->hide();
	mRemeshingToolBar->hide();

}

void MainWindow::setToolOptions(QWidget *optionsWidget)
{
	mToolOptionsDockWidget->setWindowTitle(tr("Tool Options - ") + optionsWidget->windowTitle());
	mToolOptionsStackedWidget->setCurrentWidget(optionsWidget);
	// Adjust option layout to be tight
	mToolOptionsStackedWidget->adjustSize();
	// show or hide the dockwidget options
	// if (optionsWidget->windowTitle() != "" && mToolOptionsDockWidget->isHidden())
		// mToolOptionsDockWidget->show();
	// else if (!mToolOptionsDockWidget->isHidden() && optionsWidget->windowTitle() == "")
		// mToolOptionsDockWidget->hide();
}

void MainWindow::createStatusBar()
{
	statusBar()->showMessage(tr("Welcome to TopMod"));
}

void MainWindow::closeEvent(QCloseEvent *event)
{
	//close the help file if it's open... not sure this is necessary
	// if (mAssistantClient)
	// 	mAssistantClient->closeAssistant();

	mPreferencesDialog->saveSettings();

	if (maybeSave())
	{
		event->accept();
	}
	else event->ignore();
}

void MainWindow::newFile()
{
	if (maybeSave())
	{
		clearUndoList();
		mObject.destroy();
		mActiveViewport->redraw();
	}
	else
	{
		return;
	}
}

bool MainWindow::maybeSave()
{
	if (this->isModified())
	{
		int ret = QMessageBox::warning(
			this, tr("TopMod"),
			tr("The document has been modified.\nDo you want to save your changes?"),
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

void MainWindow::loadFile(QString fileName)
{
	openFile(fileName);
	this->setCurrentFile(fileName);
	statusBar()->showMessage(tr("File loaded"), 2000);
}

bool MainWindow::saveFile(const QString &fileName)
{
	this->setCurrentFile(fileName);
	statusBar()->showMessage(tr("File saved"), 2000);
	return true;
}

void MainWindow::openFile(const QString &fileName)
{
	QFile file(fileName);
	file.open(QIODevice::ReadOnly);
	QFileInfo info(file);
	QByteArray ba = info.absoluteFilePath().toLatin1();
	const char *filename = ba.data();
	mWasPrimitive = false;
	mIsPrimitive = false;
	if (!mCurFilename.isEmpty())
	{
		undoPush();
		setModified(false);
	}

	readObject(filename);
#ifdef WITH_PYTHON
	DLFLObjectPtr obj = &mObject;
	if (obj)
		emit loadedObject(obj, fileName);
#endif
	mActiveViewport->createPatchObject();
	mActiveViewport->recomputePatches();
	mActiveViewport->recomputeNormals();
	setCurrentFile(fileName);
	mActiveViewport->redraw();
}

void MainWindow::about()
{
//TODO: Define version number in a header file
#ifndef VER
#define VER "3.0.0"
#endif // VER

	QString s = QString(
		"TopMod Topological Mesh Modeler v%1 <br />" \
		"<a href=\"http://www.topmod3d.org/\">TopMod Home Page</a><br />" \
		"<a href=\"https://github.com/TopModLab/TopMod/releases\">" \
		"Check for Updates.</a>").arg(VER);
	QMessageBox::about(this, tr("About TopMod"), s);
}

void MainWindow::initializeHelp()
{
// mAssistantClient = new QAssistantClient(QLibraryInfo::location(QLibraryInfo::BinariesPath), this);
//   QStringList arguments;
//   arguments << "-profile" << QString("userdoc") + QDir::separator() + QString("topmod.adp");
//   mAssistantClient->setArguments(arguments);
}

/**
* \brief show the help file in the QAssistant help file viewer
*/
void MainWindow::help()
{
// mAssistantClient->openAssistant();//showPage(QString("userdoc/index.html"));
	QDesktopServices::openUrl(QUrl("http://www.topmod3d.org/wiki"));
}

/**
* \brief will eventually check for updates on the TopMod website and download the necessary files
*/
void MainWindow::checkForUpdates()
{
	QDesktopServices::openUrl(QUrl("http://code.google.com/p/topmod/downloads/list"));
}

/**
* \brief opens the current TopMod homepage in the user's default web browser
*/
void MainWindow::topModWeb()
{
	QDesktopServices::openUrl(QUrl("http://www.topmod3d.org/"));
}

void MainWindow::topModBlip()
{
	QDesktopServices::openUrl(QUrl("http://topmod.blip.tv/"));
}

void MainWindow::topModResearch()
{
	QDesktopServices::openUrl(QUrl("http://www.topmod3d.org/wiki/index.php?title=Research"));
}

void MainWindow::documentWasModified()
{
	setWindowModified(this->isModified());
}

void MainWindow::on_editStyleAction_triggered()
{
	mStyleSheetEditor->show();
	mStyleSheetEditor->activateWindow();
}

void MainWindow::openPreferences()
{
	mPreferencesDialog->display();
}

#ifdef WITH_VERSE
void MainWindow::verseConnected()
{
	mVerseMenu->insertAction(mVerseConnectLocalhostAct, mVerseDisconnectAct);
	mVerseMenu->removeAction(mVerseConnectLocalhostAct);
	mVerseMenu->removeAction(mVerseConnectAct);
}
void MainWindow::verseDisconnected()
{
	mVerseMenu->insertAction(mVerseDisconnectAct, mVerseConnectLocalhostAct);
	mVerseMenu->insertAction(mVerseDisconnectAct, mVerseConnectAct);
	mVerseMenu->removeAction(mVerseDisconnectAct);
}
void MainWindow::verseStarted()
{
	mVerseMenu->insertAction(mVerseStartServerAct, mVerseKillServerAct);
	mVerseMenu->removeAction(mVerseStartServerAct);
}
void MainWindow::verseKilled()
{
	mVerseMenu->insertAction(mVerseKillServerAct, mVerseStartServerAct);
	mVerseMenu->removeAction(mVerseKillServerAct);
}
#endif

void MainWindow::createRenderers()
{
	sWiredRenderer = new WireframeRenderer();
	// wired->setRenderFlags(DLFLRenderer::ShowWireframe);

	sNormalRenderer = new NormalRenderer();
	// normal->setRenderFlags(DLFLRenderer::ShowWireframe);

	sLitRenderer = new LitRenderer();
	// lit->setRenderFlags(DLFLRenderer::ShowWireframe);

	sTexturedRenderer = new TexturedRenderer();
	// textured->setRenderFlags(DLFLRenderer::ShowWireframe);

	sTexturedLitRenderer = new TexturedLitRenderer();
	// texturedlit->setRenderFlags(DLFLRenderer::ShowWireframe);

	sPatchRenderer = new PatchRenderer();
	// patch->setRenderFlags(DLFLRenderer::ShowWireframe);
	sColorableRendererr = new ColorableRenderer();
}

void MainWindow::destroyRenderers()
{
	delete sWiredRenderer;
	delete sNormalRenderer;
	delete sLitRenderer;
	delete sTexturedRenderer;
	delete sTexturedLitRenderer;
	delete sPatchRenderer;
	delete sColorableRendererr;
}

void MainWindow::setUndoLimit(int limit)
{
	mUndoLimit = limit;
}

void MainWindow::toggleUndo(void)
{
	if (mUseUndo) mUseUndo = false;
	else mUseUndo = true;
}

void MainWindow::doDrag(int x, int y) // brianb
{
	int drag_endx = x;
	int drag_endy = y;

	GLdouble obj_world[3];  // Object world coordinates
	GLdouble obj_window[3]; // Object window coordinates
	GLdouble ms_window[3];  // Mouse start drag window
	GLdouble ms_world[3];   // Mouse start drag world
	GLdouble me_window[3];  // Mouse end drag window
	GLdouble me_world[3];   // Mouse end drag world

	GLdouble modelMatrix[16], projMatrix[16];
	GLint viewport[4];
	GLint realy;
	DLFLVertexPtr vptr;
	// Viewport* viewp;

	switch (mOperationMode)
	{
	case EditVertex:
	{
		if (GLWidget::numSelectedLocators() > 0)
		{
			if (!sIsEditing)
			{
				undoPush();
				sIsEditing = true;
			}
			vptr = mActiveViewport->getLocatorPtr()->getActiveVertex();

			// Save previous transformations
			glMatrixMode(GL_PROJECTION);
			glPushMatrix();
			glMatrixMode(GL_MODELVIEW);
			glPushMatrix();

			// Apply current transformation
			// viewp = active->getViewport();
			// viewp->reshape();
			// viewp->apply_transform();

			// Get the info
			glGetIntegerv(GL_VIEWPORT, viewport);
			glGetDoublev(GL_MODELVIEW_MATRIX, modelMatrix);
			glGetDoublev(GL_PROJECTION_MATRIX, projMatrix);

			obj_world[0] = vptr->getCoords()[0];
			obj_world[1] = vptr->getCoords()[1];
			obj_world[2] = vptr->getCoords()[2];

			// Project object coordinates to window coordinates (to get accurate window depth)
			gluProject(obj_world[0], obj_world[1], obj_world[2],
					   modelMatrix, projMatrix, viewport,
					   &obj_window[0], &obj_window[1], &obj_window[2]);

			// Set start and end window coordinates using depth coordinate found above
			ms_window[0] = sDragStartX;  ms_window[1] = sDragStartY;  ms_window[2] = obj_window[2];
			me_window[0] = drag_endx;    me_window[1] = drag_endy;    me_window[2] = obj_window[2];

			// Unproject start drag window coordinates to world coordinates
			gluUnProject(ms_window[0], ms_window[1], ms_window[2],
						 modelMatrix, projMatrix, viewport,
						 &ms_world[0], &ms_world[1], &ms_world[2]);

			// Unproject end drag window coordinates to world coordinates
			gluUnProject(me_window[0], me_window[1], me_window[2],
						 modelMatrix, projMatrix, viewport,
						 &me_world[0], &me_world[1], &me_world[2]);

			// Switch on locked axis and update object world position
			switch (mActiveViewport->getLocatorPtr()->getSelectedAxis())
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

			vptr->setCoords(Vector3d(obj_world[0], obj_world[1], obj_world[2]));

			// Reset drag start points
			startDrag(drag_endx, drag_endy);

			// Restore previous transformations
			glPopMatrix();
			glMatrixMode(GL_PROJECTION);
			glPopMatrix();

			redraw();
		}
		break;
	}
	case SelectionWindow:
	{
		// std::cout << "masking!\n";

		//draw the window with qPainter
		mActiveViewport->showSelectionWindow();
		mActiveViewport->setSelectionWindowStartX(sDragStartX);// mapFromGlobal(QCursor::pos()).x());
		mActiveViewport->setSelectionWindowStartY(sDragStartY);// mapFromGlobal(QCursor::pos()).y());
		//find center of rectangle
		// int w = abs(mapFromGlobal(QCursor::pos()).x()-drag_startx);
		// int h =	abs(mapFromGlobal(QCursor::pos()).y()-(active->height()-drag_starty));

		int w = max(1, abs(x - sDragStartX));
		int h = max(1, abs(y - sDragStartY));

		// int cx = (w)/2+drag_startx;
		// //invert this???
		// int cy = (h)/2+(active->height()-drag_starty);

		int cx = (x + sDragStartX) / 2;
		int cy = (y + sDragStartY) / 2;

		// cout << "cx = " << cx << "\t\tcy = " << cy << "\t\tw = " << w << "\t\th = " << h << "\n";

		if (mSelectionMask == MainWindow::MaskVertices)
		{
			DLFLVertexPtrArray svptrarr = mActiveViewport->selectVertices(cx, cy, w, h);
			for (auto svPtr : svptrarr)
			{
				if (!mActiveViewport->isSelected(svPtr))
				{
					mActiveViewport->setSelectedVertex(svPtr);
					sNumSelVerts++;
				}
			}
			mActiveViewport->redraw();
		}
		else if (mSelectionMask == MainWindow::MaskEdges)
		{
			DLFLEdgePtrArray septrarr = mActiveViewport->selectEdges(cx, cy, w, h);
			for (auto sePtr : septrarr)
			{
				if (!mActiveViewport->isSelected(sePtr))
				{
					mActiveViewport->setSelectedEdge(sePtr);
					sNumSelEdges++;
				}
			}
			mActiveViewport->redraw();
		}
		else if (mSelectionMask == MainWindow::MaskFaces)
		{
			// std::cout << "faces!\n";
			DLFLFacePtrArray sfptrarr = mActiveViewport->selectFaces(cx, cy, w, h);
			for (auto sfPtr : sfptrarr)
			{
				if (!mActiveViewport->isSelected(sfPtr))
				{
					mActiveViewport->setSelectedFace(sfPtr);
					sNumSelFaces++;
				}
			}
			mActiveViewport->redraw();
		}
		else if (mSelectionMask == MainWindow::MaskCorners)
		{

		}
		break;
	}
	default:
	{
		doSelection(x, y);
		break;
	}
	}
}// brianb

// Do selection of various entities depending on current mode
void MainWindow::doSelection(int x, int y)
{
	DLFLVertexPtr selectedVertPtr = nullptr;
	DLFLEdgePtr selectedEdgePtr = nullptr;
	DLFLFacePtr selectedFacePtr = nullptr;
	DLFLFaceVertexPtr selectedFaceVertPtr = nullptr;
	DLFLLocatorPtr selectedLocatorPtr = nullptr; // brianb

	switch (mOperationMode)
	{
	case EditVertex:     // brianb
	{
		selectedLocatorPtr = mActiveViewport->getLocatorPtr();
		selectedVertPtr = mActiveViewport->getLocatorPtr()->getActiveVertex();
		if (selectedVertPtr == nullptr)
		{
			selectedVertPtr = mActiveViewport->selectVertex(x, y);
			selectedLocatorPtr->setActiveVertex(selectedVertPtr);
		}
		// Test for locater selection
		if (selectedLocatorPtr->getActiveVertex() != nullptr)
		{
			selectedLocatorPtr = mActiveViewport->selectLocator(x, y);
			if (selectedLocatorPtr != nullptr)
			{
				mActiveViewport->setSelectedLocator(0, selectedLocatorPtr);
				startDrag(x, y);
			}
			else
			{
				mActiveViewport->getLocatorPtr()->setActiveVertex(nullptr);
				mActiveViewport->clearSelectedLocators();
			}
		}
		break;
	}
	case SelectVertex:
	case MarkVertex:
	case CutVertex://ozgur
		if (QApplication::keyboardModifiers() != Qt::ShiftModifier)
		{
			mActiveViewport->clearSelectedVertices();
		}
		selectedVertPtr = mActiveViewport->selectVertex(x, y);
		mActiveViewport->setSelectedVertex(selectedVertPtr);
		break;
	case MultiSelectVertex:
		selectedVertPtr = mActiveViewport->selectVertex(x, y);
		if (QApplication::keyboardModifiers() == Qt::ControlModifier)
		{
			if (mActiveViewport->isSelected(selectedVertPtr))
			{
				mActiveViewport->clearSelectedVertex(selectedVertPtr);
				sNumSelVerts--;
			}
			mActiveViewport->redraw();
		}
		else
		{
			selectedVertPtr = mActiveViewport->selectVertex(x, y);
			if (!mActiveViewport->isSelected(selectedVertPtr))
			{
				mActiveViewport->setSelectedVertex(selectedVertPtr);
				sNumSelVerts++;
			}
			mActiveViewport->redraw();
		}
		// svptr = active->selectVertex(x,y);
		// if ( !active->isSelected(svptr) )
		// 	active->setSelectedVertex(num_sel_verts,svptr);
		break;
	case DeleteEdge:
	case SubdivideEdge:
	case CollapseEdge:
	case SelectEdge:
	case CutEdge:
	case TruncateEdge:
	case MarkEdge://ozgur
		if (QApplication::keyboardModifiers() != Qt::ShiftModifier)
		{
			mActiveViewport->clearSelectedEdges();
		}
		selectedEdgePtr = mActiveViewport->selectEdge(x, y);
		mActiveViewport->setSelectedEdge(selectedEdgePtr);
		// cout << "x = " << x << "\t\ty = " << y << "\n";
		break;
	case CutEdgeandVertex://ozgur
		selectedEdgePtr = mActiveViewport->selectEdge(x, y);
		selectedVertPtr = mActiveViewport->selectVertex(x, y);
		mActiveViewport->setSelectedEdge(selectedEdgePtr);
		mActiveViewport->setSelectedVertex(selectedVertPtr);
		break;
	case MultiSelectEdge:
		selectedEdgePtr = mActiveViewport->selectEdge(x, y);
		if (QApplication::keyboardModifiers() == Qt::ControlModifier)
		{
			if (mActiveViewport->isSelected(selectedEdgePtr))
			{
				mActiveViewport->clearSelectedEdge(selectedEdgePtr);
				sNumSelEdges--;
			}
			mActiveViewport->redraw();
		}
		else
		{
			selectedEdgePtr = mActiveViewport->selectEdge(x, y);
			if (!mActiveViewport->isSelected(selectedEdgePtr))
			{
				mActiveViewport->setSelectedEdge(selectedEdgePtr);
				sNumSelEdges++;
			}
			mActiveViewport->redraw();
		}
		// septr = active->selectEdge(x,y);
		// if ( !active->isSelected(septr) )
		// 	active->setSelectedEdge(num_sel_edges,septr);
		break;
	case SelectEdgeLoop:
		if (QApplication::keyboardModifiers() != Qt::ShiftModifier)
		{
			mActiveViewport->clearSelectedEdges();
		}
		selectedEdgePtr = mActiveViewport->selectEdge(x, y);
		if (selectedEdgePtr &&
			QApplication::keyboardModifiers() == Qt::ControlModifier &&
			mActiveViewport->isSelected(selectedEdgePtr))
		{
			sDeselectEdges = true;
			mActiveViewport->clearSelectedEdge(selectedEdgePtr);
			sNumSelEdges--;
			getEdgeLoopSelection(selectedEdgePtr);
			sDeselectEdges = false;
		}
		else if (selectedEdgePtr && !mActiveViewport->isSelected(selectedEdgePtr))
		{
			// active->setSelectedEdge(num_sel_edges,septr);
			mActiveViewport->setSelectedEdge(selectedEdgePtr);
			sNumSelEdges++;
			getEdgeLoopSelection(selectedEdgePtr);
		}
		mActiveViewport->redraw();
		break;
	case SelectFace:
	case CrustModeling:
	case ConnectFaces:
	case CutFace://ozgur
	case PaintFace://dave
	case EyeDropper://dave
	{
		if (QApplication::keyboardModifiers() != Qt::ShiftModifier)
		{
			mActiveViewport->clearSelectedFaces();
		}
		selectedFacePtr = mActiveViewport->selectFace(x, y);
		// active->setSelectedFace(num_sel_faces,sfptr);
		mActiveViewport->setSelectedFace(selectedFacePtr);
		break;
		// case SelectSimilarFaces :
		// 	//clear selection if shift isn't down
		// 	if (QApplication::keyboardModifiers() != Qt::ShiftModifier)
		// 		active->clearSelectedFaces();
		// 	sfptr = active->selectFace(x,y);
		// 	if (sfptr){
		// 		if (!active->isSelected(sfptr)){
		// 			// active->setSelectedFace(num_sel_faces,sfptr);
		// 			active->setSelectedFace(sfptr);
		// 			num_sel_faces++;
		// 		}
		// 		DLFLFacePtrArray sfptrarray;
		// 		DLFL::selectMatchingFaces(&object, sfptr, sfptrarray);
		// 		for (auto sfPtr : sfptrarray){
		// 			if (!active->isSelected(sfPtr)){
		// 				active->setSelectedFace(sfPtr);
		// 				num_sel_faces++;
		// 			}
		// 		}
		// 	}
		// 	active->redraw();
		// 	break;
	}
	case SelectSimilar:
	{
		// std::cout << active->getSelectionMaskString() << " = select similar\n";

		switch (mSelectionMask)
		{
		case MaskFaces:
		{
			// std::cout<< "Mask faces select sim\n";
			//clear selection if shift isn't down
			if (QApplication::keyboardModifiers() != Qt::ShiftModifier)
				mActiveViewport->clearSelectedFaces();
			selectedFacePtr = mActiveViewport->selectFace(x, y);
			if (selectedFacePtr)
			{
				if (!mActiveViewport->isSelected(selectedFacePtr))
				{
					// active->setSelectedFace(num_sel_faces,sfptr);
					mActiveViewport->setSelectedFace(selectedFacePtr);
					sNumSelFaces++;
				}
				DLFLFacePtrArray sfptrarray;
				DLFL::selectMatchingFaces(&mObject, selectedFacePtr, sfptrarray);
				for (auto sfPtr : sfptrarray)
				{
					if (!mActiveViewport->isSelected(sfPtr))
					{
						mActiveViewport->setSelectedFace(sfPtr);
						sNumSelFaces++;
					}
				}
			}
			break;
		}
		case MaskEdges:
		{
			//clear selection if shift isn't down
			if (QApplication::keyboardModifiers() != Qt::ShiftModifier)
				mActiveViewport->clearSelectedEdges();
			selectedEdgePtr = mActiveViewport->selectEdge(x, y);
			if (selectedEdgePtr)
			{
				if (!mActiveViewport->isSelected(selectedEdgePtr))
				{
					mActiveViewport->setSelectedEdge(selectedEdgePtr);
					sNumSelEdges++;
				}
				DLFLEdgePtrArray septrarray;
				DLFL::selectMatchingEdges(&mObject, selectedEdgePtr, septrarray);
				for (auto sePtr : septrarray)
				{
					if (!mActiveViewport->isSelected(sePtr))
					{
						mActiveViewport->setSelectedEdge(sePtr);
						sNumSelEdges++;
					}
				}
			}
			break;
		}
		case MaskVertices:
		{
			//clear selection if shift isn't down
			if (QApplication::keyboardModifiers() != Qt::ShiftModifier)
				mActiveViewport->clearSelectedVertices();
			selectedVertPtr = mActiveViewport->selectVertex(x, y);
			if (selectedVertPtr)
			{
				if (!mActiveViewport->isSelected(selectedVertPtr))
				{
					mActiveViewport->setSelectedVertex(selectedVertPtr);
					sNumSelVerts++;
				}
				DLFLVertexPtrArray svptrarray;
				DLFL::selectMatchingVertices(&mObject, selectedVertPtr, svptrarray);
				for (auto svPtr : svptrarray)
				{
					if (!mActiveViewport->isSelected(svPtr))
					{
						mActiveViewport->setSelectedVertex(svPtr);
						sNumSelVerts++;
					}
				}
			}
			mActiveViewport->redraw();
			break;
		}
		default:
			break;
		};
		break;
	}
	case SelectFacesByArea:
	{
		//clear selection if shift isn't down
		if (QApplication::keyboardModifiers() != Qt::ShiftModifier)
			mActiveViewport->clearSelectedFaces();
		selectedFacePtr = mActiveViewport->selectFace(x, y);
		if (selectedFacePtr)
		{
			if (!mActiveViewport->isSelected(selectedFacePtr))
			{
				// active->setSelectedFace(num_sel_faces,sfptr);
				mActiveViewport->setSelectedFace(selectedFacePtr);
				sNumSelFaces++;
			}
			DLFLFacePtrArray sfptrarray;
			DLFL::selectFacesByArea(&mObject, selectedFacePtr, sfptrarray, MainWindow::sFaceAreaTolerance);
			for (auto sfPtr : sfptrarray)
			{
				if (!mActiveViewport->isSelected(sfPtr))
				{
					mActiveViewport->setSelectedFace(sfPtr);
					sNumSelFaces++;
				}
			}
		}
		mActiveViewport->redraw();
		break;
	}
	case SelectFacesByColor:
	{
		//clear selection if shift isn't down
		if (QApplication::keyboardModifiers() != Qt::ShiftModifier)
			mActiveViewport->clearSelectedFaces();
		selectedFacePtr = mActiveViewport->selectFace(x, y);
		if (selectedFacePtr)
		{
			if (!mActiveViewport->isSelected(selectedFacePtr))
			{
				// active->setSelectedFace(num_sel_faces,sfptr);
				mActiveViewport->setSelectedFace(selectedFacePtr);
				sNumSelFaces++;
			}
			DLFLFacePtrArray sfptrarray;
			DLFL::selectFacesByColor(&mObject, selectedFacePtr, sfptrarray, MainWindow::sFaceColorTolerance);
			for (auto sfPtr : sfptrarray)
			{
				if (!mActiveViewport->isSelected(sfPtr))
				{
					mActiveViewport->setSelectedFace(sfPtr);
					sNumSelFaces++;
				}
			}
		}
		mActiveViewport->redraw();
		break;
	}
	case SelectFaceLoop:
	{
		if (QApplication::keyboardModifiers() != Qt::ShiftModifier)
		{
			mActiveViewport->clearSelectedFaces();
		}
		selectedEdgePtr = mActiveViewport->selectEdge(x, y);
		mActiveViewport->setSelectedEdge(selectedEdgePtr);
		if (selectedEdgePtr)
		{
			if (QApplication::keyboardModifiers() == Qt::ControlModifier)
			{ // deselect
				sFaceLoopStartEdge = selectedEdgePtr;
				getFaceLoopSelection(selectedEdgePtr, true, nullptr, false);
			}
			else
			{ //select
				sFaceLoopStartEdge = selectedEdgePtr;
				getFaceLoopSelection(selectedEdgePtr, true, nullptr, true);
			}
		}
		mActiveViewport->redraw();
		break;
	}
	case SelectEdgeRing:
	{
		if (QApplication::keyboardModifiers() != Qt::ShiftModifier)
		{
			mActiveViewport->clearSelectedEdges();
		}
		selectedEdgePtr = mActiveViewport->selectEdge(x, y);
		mActiveViewport->setSelectedEdge(selectedEdgePtr);
		if (selectedEdgePtr)
		{
			if (QApplication::keyboardModifiers() == Qt::ControlModifier)
			{ // deselect
				sEdgeRingStartEdge = selectedEdgePtr;
				getEdgeRingSelection(selectedEdgePtr, true, nullptr, false);
			}
			else
			{ //select
				sEdgeRingStartEdge = selectedEdgePtr;
				getEdgeRingSelection(selectedEdgePtr, true, nullptr, true);
			}
		}
		mActiveViewport->redraw();
		break;
	}
	case ExtrudeFace:
	case ExtrudeFaceDS:
	case ExtrudeDualFace:
	case ExtrudeFaceDodeca:
	case ExtrudeFaceIcosa:
	case StellateFace:
	case DoubleStellateFace:
	case ExtrudeFaceDome:
	case ExtrudeMultipleFaces:
	case MultiSelectFace:
	case SubdivideFace:
	{
		selectedFacePtr = mActiveViewport->selectFace(x, y);
		if (QApplication::keyboardModifiers() == Qt::ControlModifier)
		{
			if (mActiveViewport->isSelected(selectedFacePtr))
			{
				mActiveViewport->clearSelectedFace(selectedFacePtr);
				sNumSelFaces--;
			}
			mActiveViewport->redraw();
		}
		else
		{
			selectedFacePtr = mActiveViewport->selectFace(x, y);
			if (!mActiveViewport->isSelected(selectedFacePtr))
			{
				mActiveViewport->setSelectedFace(sNumSelFaces, selectedFacePtr);
				sNumSelFaces++;
			}
			mActiveViewport->redraw();
		}
		break;
	}
	case SelectCheckerboard:
	{
		if (QApplication::keyboardModifiers() != Qt::ShiftModifier)
		{
			mActiveViewport->clearSelectedFaces();
		}
		//get one selected face
		selectedFacePtr = mActiveViewport->selectFace(x, y);
		if (selectedFacePtr &&
			mActiveViewport->isSelected(selectedFacePtr) &&
			QApplication::keyboardModifiers() == Qt::ControlModifier)
		{
			mActiveViewport->clearSelectedFace(selectedFacePtr);
			sNumSelFaces--;
			getCheckerboardSelection(selectedFacePtr);
		}
		else if (selectedFacePtr && !mActiveViewport->isSelected(selectedFacePtr))
		{
			mActiveViewport->setSelectedFace(selectedFacePtr);
			sNumSelFaces++;
			getCheckerboardSelection(selectedFacePtr);
		}
		mActiveViewport->redraw();
		break;
	}
	case SelectCorner:
	case ReorderFace:
	case InsertEdge:
	case SpliceCorners:
	case ConnectFaceVertices:
	case BezierConnectFaces:
	case HermiteConnectFaces:
	{
		selectedFacePtr = mActiveViewport->selectFace(x, y);
		mActiveViewport->setSelectedFace(sNumSelFaces, selectedFacePtr);
		if (selectedFacePtr)
		{
			selectedFaceVertPtr = mActiveViewport->selectFaceVertex(selectedFacePtr, x, y);
			mActiveViewport->setSelectedFaceVertex(sNumSelFaceVerts, selectedFaceVertPtr);
		}
		break;
	}
	case MultiSelectCorner:
	{
		selectedFacePtr = mActiveViewport->selectFace(x, y);
		mActiveViewport->setSelectedFace(selectedFacePtr);
		if (selectedFacePtr)
		{
			selectedFaceVertPtr = mActiveViewport->selectFaceVertex(selectedFacePtr, x, y);
			if (!mActiveViewport->isSelected(selectedFaceVertPtr))
				mActiveViewport->setSelectedFaceVertex(selectedFaceVertPtr);
		}
		break;
	}
	case ConnectEdges:
	{
		selectedFacePtr = mActiveViewport->selectFace(x, y);
		mActiveViewport->setSelectedFace(sNumSelFaces, selectedFacePtr);
		if (selectedFacePtr)
		{
			selectedEdgePtr = mActiveViewport->selectEdge(x, y);
			mActiveViewport->setSelectedEdge(sNumSelEdges, selectedEdgePtr);
		}
		break;
	}
	case SelectionWindow:
	{
		if (QApplication::keyboardModifiers() != Qt::ShiftModifier)
		{
			mActiveViewport->clearSelectedEdges();
			mActiveViewport->clearSelectedVertices();
			mActiveViewport->clearSelectedFaces();
		}
		startDrag(x, y);

		break;
	}
	}
	redraw();
	// if ( svptr != nullptr || septr != nullptr || sfptr != nullptr ) redraw();
}

void MainWindow::dragEnterEvent(QDragEnterEvent *event)
{
	/*QMessageBox::about(this,tr("mimetype"),tr("%1").arg(event->mimeData()->formats().join(",")));
	if (event->mimeData()->hasFormat("text/uri-list"))
	event->acceptProposedAction();*/
}

void MainWindow::dropEvent(QDropEvent *event)
{
	/*textBrowser->setPlainText(event->mimeData()->text());
	mimeTypeCombo->clear();
	mimeTypeCombo->addItems(event->mimeData()->formats());
	QMessageBox::about(this, tr("mimetype"), tr("%1").arg(event->mimeData()->data("text/uri-list").data));

	event->acceptProposedAction();*/
}

// Handle keyboard and mouse events
void MainWindow::mousePressEvent(QMouseEvent *event)
{
	//experimental for crossing window selection
	// if (event->buttons() == Qt::LeftButton && mode == SelectionWindow) {}
	if (event->buttons() == Qt::LeftButton && mOperationMode != NormalMode)
	{
		doSelection(event->x(), height() - mStatusBar->height() - event->y());
	}
	// else if ( event->buttons() == Qt::RightButton && QApplication::keyboardModifiers() == Qt::ShiftModifier){
	// 	// event->ignore();
	// 	if (!active->isBrushVisible()) active->showBrush();
	// 	mBrushStartX = event->x();
	// 	// mBrushStartY = event->y();
	// }
	else if (event->buttons() == Qt::RightButton)
	{
		getRightClickMenu();
		mRightClickMenu->popup(mapToGlobal(event->pos()));
	}
	else
	{
		event->ignore();
	}
}

/**
* \brief load custom right click menu options based on the current mode of operation
*
*/
void MainWindow::getRightClickMenu()
{
	mRightClickMenu->clear();
	//add generic items for now to test it out

	switch (mOperationMode)
	{
	case NormalMode:
	// mRightClickMenu->addAction(createWireframeAct);
		break;
	case SelectVertex:
		break;
	case SelectEdge:
		mRightClickMenu->addAction(mSubdivideSelectedEdgesAct);
		break;
	case SelectFace:
		mRightClickMenu->addAction(mSubdivideSelectedFacesAct);
		mRightClickMenu->addAction(mPaintSelectedFacesAct);
		break;
	case SelectCorner:
		break;
	case MultiSelectVertex:
		break;
	case MultiSelectEdge:
		mRightClickMenu->addAction(mSubdivideSelectedEdgesAct);
		break;
	case MultiSelectFace:
		mRightClickMenu->addAction(mSubdivideSelectedFacesAct);
		mRightClickMenu->addAction(mPaintSelectedFacesAct);
		break;
	case MultiSelectCorner:
		break;
	case InsertEdge:
		break;
	case DeleteEdge:
		break;
	case SubdivideEdge:
		break;
	case CollapseEdge:
		mRightClickMenu->addAction(mSubdivideSelectedEdgesAct);
		mRightClickMenu->addAction(mCollapseSelectedEdgesAct);
		break;
	case SpliceCorners:
		break;
	case ConnectEdges:
		mRightClickMenu->addAction(mSubdivideSelectedEdgesAct);
		mRightClickMenu->addAction(mCollapseSelectedEdgesAct);
		break;
	case ExtrudeFace:
	case ExtrudeFaceDS:
	case ExtrudeDualFace:
	case ExtrudeFaceDodeca:
	case ExtrudeFaceIcosa:
	case ExtrudeMultipleFaces:
	case StellateFace:
	case DoubleStellateFace:
	case ExtrudeFaceDome:
		mRightClickMenu->addAction(mPerformExtrusionAct);
		mRightClickMenu->addAction(mSubdivideSelectedFacesAct);
		mRightClickMenu->addAction(mPaintSelectedFacesAct);
		break;
	case ConnectFaceVertices:
	case ConnectFaces:
	case BezierConnectFaces:
	case HermiteConnectFaces:
		mRightClickMenu->addMenu(mHighgenusMode->getMenu());
		break;
	case ReorderFace:
		break;
	case SubdivideFace:
		mRightClickMenu->addAction(mSubdivideSelectedFacesAct);
		break;
	case CrustModeling:
		mRightClickMenu->addAction(mCreateCrustThicknessAct);
		mRightClickMenu->addAction(mCreateCrustScalingAct);
		mRightClickMenu->addSeparator();
		break;
	case CutEdge:
	case CutVertex:
	case CutFace:
		mRightClickMenu->addAction(mPerformCuttingAct);
		mRightClickMenu->addSeparator();
		break;
	case TruncateEdge:
		break;
	case MarkEdge:
		break;
	case MarkVertex:
		break;
	case ConvexHullMode:
		break;
	case EditVertex:
		break;
	case SelectEdgeLoop:
	case SelectEdgeRing:
		mRightClickMenu->addAction(mSubdivideSelectedEdgesAct);
		mRightClickMenu->addAction(mCollapseSelectedEdgesAct);
		// mRightClickMenu->addAction(mDeleteSelectedAct);
		break;
	case SelectFaceLoop:
	// SelectSimilarFaces :
	case SelectFacesByArea:
	case SelectFacesByColor:
	case SelectCheckerboard:
		mRightClickMenu->addAction(mSubdivideSelectedFacesAct);
		mRightClickMenu->addAction(mPaintSelectedFacesAct);
		break;

	default:
		break;
	}

	mRightClickMenu->addSeparator();
	mRightClickMenu->addAction(mSelectAllAct);
	mRightClickMenu->addAction(mSelectInverseAct);
	mRightClickMenu->addAction(mSelectMultipleAct);
	mRightClickMenu->addAction(mSelectSimilarAct);
	mRightClickMenu->addAction(mSelectionWindowAct);
	mRightClickMenu->addAction(mGrowSelectionAct);
	mRightClickMenu->addAction(mShrinkSelectionAct);
	mRightClickMenu->addAction(mClearSelectedModeAct);
	mRightClickMenu->addSeparator();

	switch (mSelectionMask)
	{
	case MaskVertices:
		mRightClickMenu->addAction(mDeleteSelectedAct);
		// mRightClickMenu->addAction(selectVertexAct);
		// mRightClickMenu->addAction(selectMultipleVerticesAct);
		mRightClickMenu->addAction(mEditVertexAct);
		// mRightClickMenu->addAction(mEditVertexAct);
		mRightClickMenu->addAction(mSelectEdgesFromVerticesAct);
		mRightClickMenu->addAction(mSelectFacesFromVerticesAct);
		break;
	case MaskEdges:
		mRightClickMenu->addAction(mDeleteSelectedAct);
		mRightClickMenu->addAction(mCollapseSelectedEdgesAct);
		mRightClickMenu->addSeparator();
		// mRightClickMenu->addAction(selectEdgeAct);
		// mRightClickMenu->addAction(selectMultipleEdgesAct);
		mRightClickMenu->addAction(mSelectEdgeLoopAct);
		mRightClickMenu->addAction(mSelectEdgeRingAct);
		mRightClickMenu->addAction(mSelectVerticesFromEdgesAct);
		mRightClickMenu->addAction(mSelectFacesFromEdgesAct);
		break;
	case MaskFaces: //face stuff
		// mRightClickMenu->addAction(mDeleteSelectedAct);
		// mRightClickMenu->addAction(selectFaceAct);
		mRightClickMenu->addAction(mSelectFaceLoopAct);
		// mRightClickMenu->addAction(selectMultipleFacesAct);
		// mRightClickMenu->addAction(selectSimilarFacesAct);
		mRightClickMenu->addAction(mSelectFacesByAreaAct);
		mRightClickMenu->addAction(mSelectFacesByColorAct);
		mRightClickMenu->addAction(mSelectCheckerboardFacesAct);
		mRightClickMenu->addAction(mSelectEdgesFromFacesAct);
		mRightClickMenu->addAction(mSelectVerticesFromFacesAct);
		break;
	case MaskCorners:
		// mRightClickMenu->addAction(mDeleteSelectedAct);
		// mRightClickMenu->addAction(// selectCornerAct);
		break;
	default:
		break;
	}

	// mRightClickMenu->addSeparator();
	// mRightClickMenu->addAction(mEditVertexAct);
	mRightClickMenu->addSeparator();
	mRightClickMenu->addMenu(mToolsMenu);
	mRightClickMenu->addMenu(mRemeshingMenu);
	mRightClickMenu->addMenu(mSelectionMaskMenu);
}


void MainWindow::mouseMoveEvent(QMouseEvent *event)
{
	// if (active->isBrushVisible()) active->redraw();
	if (mOperationMode != NormalMode && event->buttons() == Qt::LeftButton)
	{
		// doSelection(event->x(),this->size().height()-event->y() );
		doDrag(event->x(), this->size().height() - mStatusBar->height() - event->y());
	}
	else if (mSpinBoxMode < eInvalidSpinBox && mSpinBoxes[mSpinBoxMode] != nullptr)
	{
		double d = (event->x() - mStartDragX) / 20;
		mSpinBoxes[mSpinBoxMode]->setValue(mSpinBoxes[mSpinBoxMode]->value() + d);
	}
	else
	{
		event->ignore();
	}
}

//if the user holds down certain keys, allow them to
//change various properties in the scene
void MainWindow::keyPressEvent(QKeyEvent *event)
{
	mStartDragX = mapFromGlobal(QCursor::pos()).x();
	mStartDragY = mapFromGlobal(QCursor::pos()).y();

	switch (event->key())
	{
	case Qt::Key_Y:
		mSpinBoxMode = eFirstSpinBox;
		break;
	case Qt::Key_U:
		mSpinBoxMode = eSecondSpinBox;
		break;
	case Qt::Key_I:
		mSpinBoxMode = eThirdSpinBox;
		break;
	case Qt::Key_O:
		mSpinBoxMode = eFourthSpinBox;
		break;
	case Qt::Key_P:
		mSpinBoxMode = eFifthSpinBox;
		break;
	default:
		mSpinBoxMode = eInvalidSpinBox;
		break;
	}
}

void MainWindow::keyReleaseEvent(QKeyEvent *event)
{
	mSpinBoxMode = eInvalidSpinBox;
}

void MainWindow::setSpinBoxes(
	QDoubleSpinBox *one, QDoubleSpinBox *two,
	QDoubleSpinBox *three, QDoubleSpinBox *four,
	QDoubleSpinBox *five, QDoubleSpinBox *six)
{
	mSpinBoxes = { one, two, three, four, five, six };
}

void MainWindow::mouseReleaseEvent(QMouseEvent *event)
{
	QString cmd;
	// The mouse was dragged or released
	// Send this event to the subroutine handling the current event, if any
	// If we are not currently doing anything, nothing will happen
	// If ALT key is pressed we are mainpulating the view which is handled by the viewport
	if (mOperationMode != NormalMode)
	{
		switch (mOperationMode)
		{
		case EditVertex:       // brianb
			sIsEditing = false;
			if (mActiveViewport->numSelectedVertices() > 0)
			{
				DLFLVertexPtr vp = mActiveViewport->getSelectedVertex(0);
				vp->print();
				// active->clearSelectedVertices();
				// num_sel_verts = 0;
				redraw();
			}
			break;
		case SelectVertex:

			if (mActiveViewport->numSelectedVertices() > 0)
			{
				// DLFLVertexPtr vp = active->getSelectedVertex(0);
				// vp->print();
				// active->clearSelectedVertices();
				// num_sel_verts = 0;
				// redraw();
			}
			break;
		case SelectEdge:
		case CutEdge:
			// if ( active->numSelectedEdges() > 0 ) {
				// DLFLEdgePtr septr = active->getSelectedEdge(0);
				// if (septr) {
					//septr->print();
					//num_sel_edges = 0;
				// }
				// active->clearSelectedEdges();
				// redraw();
			// }
			break;
		case SelectEdgeLoop:
			if (mActiveViewport->numSelectedEdges() > 0)
			{
				DLFLEdgePtr septr = mActiveViewport->getSelectedEdge(0);
				if (septr)
					getEdgeLoopSelection(septr);
			}
			// active->clearSelectedEdges();
			mActiveViewport->redraw();
			break;
		case SelectEdgeRing:
			if (mActiveViewport->numSelectedEdges() > 0)
			{
				DLFLEdgePtr septr = mActiveViewport->getSelectedEdge(0);
				if (septr)
				{
					if (QApplication::keyboardModifiers() == Qt::ControlModifier)
					{ // deselect
						sEdgeRingStartEdge = septr;
						getEdgeRingSelection(septr, true, nullptr, false);
					}
					else
					{ //select
						sEdgeRingStartEdge = septr;
						getEdgeRingSelection(septr, true, nullptr, true);
					}
				}
			}
			mActiveViewport->redraw();
			break;
		case SelectFaceLoop:
			if (mActiveViewport->numSelectedEdges() > 0)
			{
				DLFLEdgePtr septr = mActiveViewport->getSelectedEdge(0);
				if (septr)
				{
					if (QApplication::keyboardModifiers() == Qt::ControlModifier)
					{ // deselect
						sFaceLoopStartEdge = septr;
						getFaceLoopSelection(septr, true, nullptr, false);
					}
					else
					{ //select
						sFaceLoopStartEdge = septr;
						getFaceLoopSelection(septr, true, nullptr, true);
					}
				}
			}
			mActiveViewport->clearSelectedEdges();
			mActiveViewport->redraw();
			break;
		case SelectFace:
			// if ( active->numSelectedFaces() > 0 )
			// 	{
			// 		DLFLFacePtr fp = active->getSelectedFace(0);
			// 		// fp->print();
			// 		// active->clearSelectedFaces();
			// 		// num_sel_faces = 0;
			// 		redraw();
			// 	}
			break;
		case PaintFace:
			if (mActiveViewport->numSelectedFaces() > 0)
			{
				DLFLFacePtr fp = mActiveViewport->getSelectedFace(0);
				// first search for the material in the existing list
				// DLFLMaterialPtr m = object.findMaterial(RGBColor(paint_bucket_color.redF(),paint_bucket_color.greenF(),paint_bucket_color.blueF() ));
				fp->setMaterial(mObject.addMaterial(RGBColor(sPaintBucketColor.redF(), sPaintBucketColor.greenF(), sPaintBucketColor.blueF())));
				// if ( m ){
				//
				// }
				// else {
				//
				// }
				mActiveViewport->clearSelectedFaces();
				redraw();
			}
			break;
		case EyeDropper:
			if (mActiveViewport->numSelectedFaces() > 0)
			{
				DLFLFacePtr fp = mActiveViewport->getSelectedFace(0);
				//grab the color from the face... set it in the interface
				setPaintBucketColor(QColor(fp->material()->color.r, fp->material()->color.g, fp->material()->color.b));
				//set the color over in the experimental mode interface thing...
				mExperimentalMode->setPaintBucketColor(QColor(fp->material()->color.r, fp->material()->color.g, fp->material()->color.b));
				mActiveViewport->clearSelectedFaces();
				redraw();
			}
			break;
		case SelectSimilar:
			if (mSelectionMask == MainWindow::MaskFaces)
			{
				// cout << "select similar faces mouse release\n";
				if (mActiveViewport->numSelectedFaces() > 0)
				{
					DLFLFacePtr selectedFacePtr = mActiveViewport->getSelectedFace(0);
					if (selectedFacePtr)
					{
						DLFLFacePtrArray sfptrarray;
						DLFL::selectMatchingFaces(&mObject, selectedFacePtr, sfptrarray);
						for (auto sfPtr : sfptrarray)
						{
							if (!mActiveViewport->isSelected(sfPtr))
							{
								mActiveViewport->setSelectedFace(sfPtr);
								sNumSelFaces++;
							}
						}
					}
				}
			}
			else if (mSelectionMask == MainWindow::MaskEdges)
			{
				if (mActiveViewport->numSelectedEdges() > 0)
				{
					DLFLEdgePtr selectedEdgePtr = mActiveViewport->getSelectedEdge(0);
					if (selectedEdgePtr)
					{
						DLFLEdgePtrArray septrarray;
						DLFL::selectMatchingEdges(&mObject, selectedEdgePtr, septrarray);
						for (auto sePtr : septrarray)
						{
							if (!mActiveViewport->isSelected(sePtr))
							{
								mActiveViewport->setSelectedEdge(sePtr);
								sNumSelEdges++;
							}
						}
					}
				}
			}
			else if (mSelectionMask == MainWindow::MaskVertices)
			{
				if (mActiveViewport->numSelectedVertices() > 0)
				{
					DLFLVertexPtr svptr = mActiveViewport->getSelectedVertex(0);
					if (svptr)
					{
						if (!mActiveViewport->isSelected(svptr))
						{
							mActiveViewport->setSelectedVertex(svptr);
							sNumSelVerts++;
						}
						DLFLVertexPtrArray svptrarray;
						DLFL::selectMatchingVertices(&mObject, svptr, svptrarray);
						for (auto svPtr : svptrarray)
						{
							if (!mActiveViewport->isSelected(svPtr))
							{
								mActiveViewport->setSelectedVertex(svPtr);
								sNumSelVerts++;
							}
						}
					}
				}
			}
			redraw();
			break;
		case SelectFacesByArea:
			if (mActiveViewport->numSelectedFaces() > 0)
			{
				DLFLFacePtr sfptr = mActiveViewport->getSelectedFace(0);
				if (sfptr)
				{
					DLFLFacePtrArray sfptrarray;
					DLFL::selectFacesByArea(&mObject, sfptr, sfptrarray, MainWindow::sFaceAreaTolerance);
					for (auto sfPtr : sfptrarray)
					{
						if (!mActiveViewport->isSelected(sfPtr))
						{
							mActiveViewport->setSelectedFace(sfPtr);
							sNumSelFaces++;
						}
					}
					redraw();
				}
			}
			break;
		case SelectCorner:
			if (mActiveViewport->numSelectedCorners() > 0)
			{
				DLFLFaceVertexPtr fvp = mActiveViewport->getSelectedFaceVertex(0);
				fvp->print();
				mActiveViewport->clearSelectedCorners();
				mActiveViewport->clearSelectedFaces();
				sNumSelFaceVerts = 0; // num_sel_faces = 0;
				redraw();
			}
			break;
		case InsertEdge:
		// std::cout << "/ners = "<<active->numSelectedCorners() << "\n";
			if (mActiveViewport->numSelectedCorners() >= 2)
			{
				DLFLFaceVertexPtr sfvptr1, sfvptr2;
				sfvptr1 = mActiveViewport->getSelectedFaceVertex(0);
				sfvptr2 = mActiveViewport->getSelectedFaceVertex(1);
				if (sfvptr1 && sfvptr2)
				{
					DLFLMaterialPtr mptr = sfvptr1->getFacePtr()->material();
					undoPush();
					setModified(true);
#if WITH_PYTHON
					cmd = QString("insertEdge((");
					cmd += QString().setNum(sfvptr1->getFaceID()) + QString(",");
					cmd += QString().setNum(sfvptr1->getVertexID()) + QString("),(");
					cmd += QString().setNum(sfvptr2->getFaceID()) + QString(",");
					cmd += QString().setNum(sfvptr2->getVertexID()) + QString("))");
					emit echoCommand(cmd);
					if (Py_IsInitialized())
					{
						PyRun_SimpleString("from dlfl import *");
						PyRun_SimpleString(cmd.toLocal8Bit().constData());
					}
#else
					DLFL::insertEdge(&mObject, sfvptr1, sfvptr2, false, mptr);
#endif
					mActiveViewport->clearSelectedFaces();
					mActiveViewport->clearSelectedCorners();
					sNumSelFaceVerts = 0; // num_sel_faces = 0;
					mActiveViewport->recomputePatches();
					mActiveViewport->recomputeNormals();
					redraw();
				}
			}
			else if (mActiveViewport->numSelectedCorners() == 1)
			{
				sNumSelFaceVerts = 1; sNumSelFaces = 1;
			}
			break;
		case DeleteEdge:
			if (mActiveViewport->numSelectedEdges() > 0)
			{
				DLFLEdgePtr septr = mActiveViewport->getSelectedEdge(0);
				if (septr)
				{
					undoPush();
					setModified(true);
#if WITH_PYTHON
					cmd = QString("deleteEdge(");
					cmd += QString().setNum(septr->getID()) + QString(")");
					emit echoCommand(cmd);
#endif
					DLFL::deleteEdge(&mObject, septr, MainWindow::sDeleteEdgeCleanup);
					mActiveViewport->recomputePatches();
					mActiveViewport->recomputeNormals();
				}
				mActiveViewport->clearSelectedEdges();
				redraw();
			}
			break;
		case SubdivideEdge:
			if (mActiveViewport->numSelectedEdges() > 0)
			{
				DLFLEdgePtr septr = mActiveViewport->getSelectedEdge(0);
				if (septr)
				{
					undoPush();
					setModified(true);
#if WITH_PYTHON
					cmd = QString("subdivideEdge(");
					cmd += QString().setNum(septr->getID()) + QString(")");
					emit echoCommand(cmd);
#endif
					DLFL::subdivideEdge(&mObject, sNumEdgeSubdiv, septr);
					mActiveViewport->recomputePatches();
					mActiveViewport->recomputeNormals();
				}
				mActiveViewport->clearSelectedEdges();
				redraw();
			}
			break;
		case CollapseEdge:
			if (mActiveViewport->numSelectedEdges() > 0)
			{
				DLFLEdgePtr septr = mActiveViewport->getSelectedEdge(0);
				if (septr)
				{
					undoPush();
					setModified(true);
#if WITH_PYTHON
					cmd = QString("collapseEdge(");
					cmd += QString().setNum(septr->getID()) + QString(")");
					emit echoCommand(cmd);
#endif
					DLFL::collapseEdge(&mObject, septr);
					mActiveViewport->recomputePatches();
					mActiveViewport->recomputeNormals();
				}
				mActiveViewport->clearSelectedEdges();
				redraw();
			}
			break;
		case SpliceCorners:
			if (mActiveViewport->numSelectedCorners() >= 2)
			{
				DLFLFaceVertexPtr sfvptr1, sfvptr2;
				sfvptr1 = mActiveViewport->getSelectedFaceVertex(0);
				sfvptr2 = mActiveViewport->getSelectedFaceVertex(1);
				if (sfvptr1 && sfvptr2)
				{
					DLFLMaterialPtr mptr = sfvptr1->getFacePtr()->material();
					undoPush();
					setModified(true);
					//object.spliceCorners(sfvptr1,sfvptr2);
					DLFL::spliceCorners(&mObject, sfvptr1, sfvptr2);
					mActiveViewport->clearSelectedFaces();
					mActiveViewport->clearSelectedCorners();
					sNumSelFaceVerts = 0; sNumSelFaces = 0;
					mActiveViewport->recomputePatches();
					mActiveViewport->recomputeNormals();
					redraw();
				}
			}
			else if (mActiveViewport->numSelectedCorners() == 1)
			{
				sNumSelFaceVerts = 1; sNumSelFaces = 1;
			}
			break;
		case ConnectFaces:
			if (mActiveViewport->numSelectedFaces() >= 2)
			{
				DLFLFacePtr sfptr1, sfptr2;
				sfptr1 = mActiveViewport->getSelectedFace(0);
				sfptr2 = mActiveViewport->getSelectedFace(1);
				if (sfptr1 && sfptr2)
				{
					undoPush();
					setModified(true);
					DLFL::connectFaces(&mObject, sfptr1, sfptr2, sNumSegments);
					mActiveViewport->recomputePatches();
					mActiveViewport->recomputeNormals();
					mActiveViewport->clearSelectedFaces();
					redraw();
				}
			}
			else if (mActiveViewport->numSelectedFaces() == 1)
			{
				sNumSelFaces = 1;
			}
			break;
		case ConnectFaceVertices:
			if (mActiveViewport->numSelectedCorners() >= 2)
			{
				DLFLFaceVertexPtr sfvptr1, sfvptr2;
				sfvptr1 = mActiveViewport->getSelectedFaceVertex(0);
				sfvptr2 = mActiveViewport->getSelectedFaceVertex(1);
				if (sfvptr1 && sfvptr2)
				{
					undoPush();
					setModified(true);
					DLFL::connectFaces(&mObject, sfvptr1, sfvptr2, sNumSegments, sMaxSegments);
					mActiveViewport->clearSelectedFaces();
					mActiveViewport->clearSelectedCorners();
					sNumSelFaceVerts = 0; sNumSelFaces = 0;
					mActiveViewport->recomputePatches();
					mActiveViewport->recomputeNormals();
					redraw();
				}
			}
			else if (mActiveViewport->numSelectedCorners() == 1)
			{
				sNumSelFaceVerts = 1; sNumSelFaces = 1;
			}
			break;
		case ConnectEdges:
			if (mActiveViewport->numSelectedEdges() >= 2)
			{
				DLFLEdgePtr septr1, septr2;
				DLFLFacePtr sfptr1, sfptr2;
				septr1 = mActiveViewport->getSelectedEdge(0);
				septr2 = mActiveViewport->getSelectedEdge(1);
				sfptr1 = mActiveViewport->getSelectedFace(0);
				sfptr2 = mActiveViewport->getSelectedFace(1);
				if (septr1 && septr2)
				{
					undoPush();
					setModified(true);
					DLFL::connectEdges(&mObject, septr1, sfptr1, septr2, sfptr2);
					mActiveViewport->clearSelectedEdges();
					mActiveViewport->clearSelectedFaces();
					sNumSelEdges = 0; sNumSelFaces = 0;
					redraw();
				}
			}
			else if (mActiveViewport->numSelectedEdges() == 1)
			{
				sNumSelEdges = 1; sNumSelFaces = 1;
			}
			break;
		case ExtrudeFace:
			if (mSingleClickExtrude)
			{
				if (mActiveViewport->numSelectedFaces() > 0)
				{
					DLFLFacePtr sfptr = mActiveViewport->getSelectedFace(0);
					if (sfptr)
					{
						undoPush();
						setModified(true);
						DLFL::extrudeFace(&mObject, sfptr, sExtrudeDist, sNumExtrusions, sExtrudeRot, sExtrudeScale);
						mActiveViewport->recomputePatches();
						mActiveViewport->recomputeNormals();
					}
					mActiveViewport->clearSelectedFaces();
					redraw();
				}
			}
			break;
		case ExtrudeMultipleFaces:
			// if (!mExtrudeMultipleAct->isChecked()){
			// 	if ( active->numSelectedFaces() > 0)
			// 		{
			// 			DLFLFacePtrArray sfptrarr = active->getSelectedFaces();
			// 			if ( sfptrarr[0] )
			// 				{
			// 					undoPush();
			// 					setModified(true);
			// 					for(auto sfPtr : sfptrarr) {
			// 						DLFL::extrudeFace(&object,sfPtr,extrude_dist,num_extrusions,extrude_rot,extrude_scale);
			// 					}
			// 					active->recomputePatches();
			// 					active->recomputeNormals();
			// 				}
			// 			active->clearSelectedFaces();
			// 			redraw();
			// 		}
			// 	}
			break;
		case ExtrudeFaceDS:
			if (mSingleClickExtrude && mActiveViewport->numSelectedFaces() > 0)
			{
				DLFLFacePtr sfptr = mActiveViewport->getSelectedFace(0);
				if (sfptr)
				{
					undoPush();
					setModified(true);
					DLFL::extrudeFaceDS(&mObject, sfptr, sExtrudeDist, sNumExtrusions, sDooSabinExtrusionTwist, sExtrudeScale);
					mActiveViewport->recomputePatches();
					mActiveViewport->recomputeNormals();
				}
				mActiveViewport->clearSelectedFaces();
				redraw();
			}
			break;
		case ExtrudeDualFace:
			if (mSingleClickExtrude && mActiveViewport->numSelectedFaces() > 0)
			{
				DLFLFacePtr sfptr = mActiveViewport->getSelectedFace(0);
				if (sfptr)
				{
					undoPush();
					setModified(true);
					DLFL::extrudeDualFace(&mObject, sfptr, sExtrudeDist, sNumExtrusions, sExtrudeRot, sExtrudeScale, sDualMeshEdgesCheck);
					mActiveViewport->recomputePatches();
					mActiveViewport->recomputeNormals();
				}
				mActiveViewport->clearSelectedFaces();
				redraw();
			}
			break;
		case ExtrudeFaceDodeca:
			if (mSingleClickExtrude && mActiveViewport->numSelectedFaces() > 0)
			{
				DLFLFacePtr sfptr = mActiveViewport->getSelectedFace(0);
				if (sfptr)
				{
					undoPush();
					setModified(true);
					DLFL::extrudeFaceDodeca(
						&mObject, sfptr, sExtrudeAngle, sNumExtrusions,
						sExtrudeLength1, sExtrudeLength2, sExtrudeLength3,
						sHexagonalizeDodecaExtrude);
					// DLFL::extrudeFaceDodeca(&object,sfptr,extrude_dist,num_extrusions, ds_ex_twist,extrude_scale, hexagonalize_dodeca_extrude);
					mActiveViewport->recomputePatches();
					mActiveViewport->recomputeNormals();
				}
				mActiveViewport->clearSelectedFaces();
				redraw();

			}
			break;
		case ExtrudeFaceIcosa:
			if (mSingleClickExtrude && mActiveViewport->numSelectedFaces() > 0)
			{
				DLFLFacePtr sfptr = mActiveViewport->getSelectedFace(0);
				if (sfptr)
				{
					undoPush();
					setModified(true);
					DLFL::extrudeFaceIcosa(&mObject, sfptr, sExtrudeAngle_iCosA, sNumExtrusions, sExtrudeLength1_iCosA, sExtrudeLength2_iCosA, sExtrudeLength3_iCosA);
					// DLFL::extrudeFaceIcosa(&object,sfptr,extrude_dist,num_extrusions, ds_ex_twist,extrude_scale);
					mActiveViewport->recomputePatches();
					mActiveViewport->recomputeNormals();
				}
				mActiveViewport->clearSelectedFaces();
				redraw();
			}
			break;
		case StellateFace:
			if (mSingleClickExtrude && mActiveViewport->numSelectedFaces() > 0)
			{
				DLFLFacePtr sfptr = mActiveViewport->getSelectedFace(0);
				if (sfptr)
				{
					undoPush();
					setModified(true);
					DLFL::stellateFace(&mObject, sfptr, sExtrudeDist);
					mActiveViewport->recomputePatches();
					mActiveViewport->recomputeNormals();
				}
				mActiveViewport->clearSelectedFaces();
				redraw();
			}
			break;
		case DoubleStellateFace:
			if (mSingleClickExtrude && mActiveViewport->numSelectedFaces() > 0)
			{
				DLFLFacePtr sfptr = mActiveViewport->getSelectedFace(0);
				if (sfptr)
				{
					undoPush();
					setModified(true);
					DLFL::doubleStellateFace(&mObject, sfptr, sExtrudeDist);
					mActiveViewport->recomputePatches();
					mActiveViewport->recomputeNormals();
				}
				mActiveViewport->clearSelectedFaces();
				redraw();
			}
			break;
		case ExtrudeFaceDome:
			if (mSingleClickExtrude && mActiveViewport->numSelectedFaces() > 0)
			{
				DLFLFacePtr sfptr = mActiveViewport->getSelectedFace(0);
				if (sfptr)
				{
					undoPush();
					setModified(true);
					DLFL::extrudeFaceDome(&mObject, sfptr, sExtrudeDist, sExtrudeRot, sExtrudeScale);
					mActiveViewport->recomputePatches();
					mActiveViewport->recomputeNormals();
				}
				mActiveViewport->clearSelectedFaces();
				redraw();
			}
			break;
		case CrustModeling:
			if (mActiveViewport->numSelectedFaces() > 0)
			{
				DLFLFacePtr sfptr = mActiveViewport->getSelectedFace(0);
				if (sfptr)
				{
					// No undo for hole punching in crust modeling mode
					// because the ids in the recreated object
					// will be different
					if (QApplication::keyboardModifiers() == Qt::ShiftModifier)
					{
						DLFL::tagMatchingFaces(&mObject, sfptr);
						DLFL::punchHoles(&mObject);
						mActiveViewport->recomputePatches();
						mActiveViewport->recomputeNormals();
					}
					else
						DLFL::cmMakeHole(&mObject, sfptr, sCrustCleanup);
					// active->recomputeNormals();
				}
				mActiveViewport->clearSelectedFaces();
				redraw();
			}
			break;
		case BezierConnectFaces:
			if (mActiveViewport->numSelectedCorners() >= 2)
			{
				DLFLFaceVertexPtr sfvptr1 = mActiveViewport->getSelectedFaceVertex(0);
				DLFLFaceVertexPtr sfvptr2 = mActiveViewport->getSelectedFaceVertex(1);
				if (sfvptr1 != nullptr && sfvptr2 != nullptr)
				{
					undoPush();
					setModified(true);
					DLFL::bezierConnectFaces(
						&mObject, sfvptr1, sfvptr2, sNumSegments, sFaceNormalWeight1, sFaceNormalWeight2);
					mActiveViewport->clearSelectedFaces();
					mActiveViewport->clearSelectedCorners();
					sNumSelFaceVerts = 0; sNumSelFaces = 0;
					mActiveViewport->recomputePatches();
					mActiveViewport->recomputeNormals();
					redraw();
				}
			}
			else if (mActiveViewport->numSelectedCorners() == 1)
			{
				sNumSelFaceVerts = 1; sNumSelFaces = 1;
			}
			break;
		case HermiteConnectFaces:
			if (mActiveViewport->numSelectedCorners() >= 2)
			{
				DLFLFaceVertexPtr sfvptr1, sfvptr2;
				sfvptr1 = mActiveViewport->getSelectedFaceVertex(0);
				sfvptr2 = mActiveViewport->getSelectedFaceVertex(1);
				if (sfvptr1 && sfvptr2)
				{
					undoPush();
					setModified(true);
					if (sSymmetricWeights)
						DLFL::hermiteConnectFaces(&mObject, sfvptr1, sfvptr2,
												  sNumSegments, sFaceNormalWeight1, sFaceNormalWeight1,
												  sMaxSegments, sNumExtraTwists);
					else
						DLFL::hermiteConnectFaces(&mObject, sfvptr1, sfvptr2,
												  sNumSegments, sFaceNormalWeight1, sFaceNormalWeight2,
												  sMaxSegments, sNumExtraTwists);
					mActiveViewport->clearSelectedFaces();
					mActiveViewport->clearSelectedCorners();
					sNumSelFaceVerts = 0; sNumSelFaces = 0;
					mActiveViewport->recomputePatches();
					mActiveViewport->recomputeNormals();
					redraw();
				}
			}
			else if (mActiveViewport->numSelectedCorners() == 1)
			{
				sNumSelFaceVerts = 1; sNumSelFaces = 1;
			}
			break;
		case ReorderFace:
			if (mActiveViewport->numSelectedCorners() > 0)
			{
				DLFLFaceVertexPtr sfvptr = mActiveViewport->getSelectedFaceVertex(0);
				if (sfvptr) sfvptr->getFacePtr()->reorder(sfvptr);
				redraw();
			}
			break;
		case SubdivideFace:
		case MultiSelectFace:
			// num_sel_faces++;
			break;
		case MultiSelectEdge:
			break;
		case MultiSelectVertex:
			break;
		case MultiSelectCorner:
			break;
			//from ozgur
		// case CutEdge :
			// if ( active->numSelectedEdges() > 0 )
			// 	{
			// 		DLFLEdgePtr septr = active->getSelectedEdge(0);
			// 		if ( septr )
			// 			{
			// 				undoPush();
			// 				septr->isMarked = 1- septr->isMarked;
			// 				// DLFL::cutEdge( &object, septr, peelDistance_factor,pnormalBendS_factor,pnormalBendT_factor, peel_all_edges);
			// 			}
			// 		active->clearSelectedEdges();
			// 		redraw();
			// 	}
			// break;
		case TruncateEdge:
			if (mActiveViewport->numSelectedEdges() > 0)
			{
				DLFLEdgePtr septr = mActiveViewport->getSelectedEdge(0);
				if (septr)
				{
					undoPush();
					septr->isMarked = !septr->isMarked;
					// DLFL::cutEdge( &object, septr, peelDistance_factor,pnormalBendS_factor,pnormalBendT_factor, peel_all_edges);
				}
				mActiveViewport->clearSelectedEdges();
				redraw();
			}
			break;
		case CutVertex:
			if (mActiveViewport->numSelectedVertices() > 0)
			{
				DLFLVertexPtr svptr = mActiveViewport->getSelectedVertex(0);
				if (svptr)
				{
					undoPush();
					svptr->isMarked = !svptr->isMarked;
				}
			// active->clearSelectedVertices();
				redraw();
			}
			break;
		case CutEdgeandVertex:
			if (mActiveViewport->numSelectedVertices() > 0 &&
				mActiveViewport->numSelectedEdges() > 0)
			{
				DLFLVertexPtr svptr = mActiveViewport->getSelectedVertex(0);
				DLFLEdgePtr septr = mActiveViewport->getSelectedEdge(0);
				if (septr)
				{
					if (svptr)
					{
						undoPush();
						septr->isMarked = !septr->isMarked;
						// DLFL::cutEdge( &object, septr, peelDistance_factor,pnormalBendS_factor,pnormalBendT_factor, peel_all_edges);
					}
				}
				mActiveViewport->clearSelectedEdges();
				mActiveViewport->clearSelectedVertices();
				redraw();
			}
			break;
		case MarkEdge:
			if (mActiveViewport->numSelectedEdges() > 0)
			{
				DLFLEdgePtr septr = mActiveViewport->getSelectedEdge(0);
				if (septr)
				{
					undoPush();
					septr->isMarked = !septr->isMarked;
				}
				mActiveViewport->clearSelectedEdges();
				redraw();
			}
			break;
		case MarkVertex:
			if (mActiveViewport->numSelectedVertices() > 0)
			{
				DLFLVertexPtr svptr = mActiveViewport->getSelectedVertex(0);
				if (svptr)
				{
					undoPush();
					svptr->isMarked = !svptr->isMarked;
				}
				mActiveViewport->clearSelectedVertices();
				redraw();
			}
			break;
		case CutFace:
			// if ( active->numSelectedFaces() > 0 )
			// 	{
			// 		DLFLFacePtr sfptr = active->getSelectedFace(0);
			// 		if ( sfptr )
			// 			{
			// 				undoPush();
			// 				sfptr->isMarked = 1 - sfptr->isMarked;
			// 			}
			// 		active->clearSelectedFaces();
			// 		redraw();
			// 	}
			break;
		case SelectionWindow:
			mActiveViewport->hideSelectionWindow();

			break;
		}//end switch (mode)

	}
	else
	{
		event->ignore();
	}
}//end function mousereleaseevent


void MainWindow::testConvexHull(void)
{
	// For testing convex hull subroutine
	// Vector3dArray vertices;
	// vertices.resize(8);
	// vertices[0].set(0,0,0);
	// vertices[1].set(0,10,0);
	// vertices[2].set(10,10,0);
	// vertices[3].set(10,0,0);
	// vertices[4].set(0,0,10);
	// vertices[5].set(0,10,10);
	// vertices[6].set(10,10,10);
	// vertices[7].set(10,0,10);
	// DLFLConvexHull convexhull;
	// convexhull.createHull(vertices);
	//
	// ofstream file;
	// file.open("convexhull.dlfl");
	// convexhull.writeDLFL(file);
	// file.close();
}

void MainWindow::performRemeshing(void)
{
	setModified(true);

	switch (mRemeshingScheme)
	{
	case Dual:
		createDual();
		break;
	case Root3:
		subdivideRoot3();
		break;
	case Triangulate:
		triangulate();
		break;
	case DualVertexTrunc:
		subdivideSqrt3();
		break;
	case GlobalStellate:
		globalStellate();
		break;
	case Star:
		subdivideStar();
		break;
	case Generic1264:
		subdivide1264();
		break;
	case Honeycomb:
		subdivideHoneycomb();
		break;
	case VertexTrunc:
		subdivideVertexCutting();
		break;
	case DualGeneric1264:
		subdivideDual1264();
		break;
	case LinearVertexInsertion:
		subdivideLinearVertexInsertion();
		break;
	case CatmullClark:
		subdivideCatmullClark();
		break;
	case ModifiedStellate:
		subStellate1();
		break;
	case DooSabin:
		subdivideDooSabin();
		break;
	case CornerCutting:
		subdivideCornerCutting();
		break;
	case ModifiedCornerCutting:
		subdivideModifiedCornerCutting();
		break;
	case Simplest:
		subdivideSimplest();
		break;
	case Pentagonal:
		subdividePentagonal();
		break;
	case CubicPentagonal:
		subdivideCubicPentagonal();
		break;
	case DualPentagonal:
		subdivideDualPentagonal();
		break;
	case LoopStyle:
		subdivideLoopStyle();
		break;
	case Loop:
		subdivideLoop();
		break;
	case Root4:
		subdivideRoot4();
		break;
	case DualLoop:
		subdivideDualLoop();
		break;
	case GlobalCubicExtrude:
		// cout << "NOT YET IMPLEMENTED!!!" << endl;
		break;
	case CheckerBoard:
		subdivideCheckerBoard();
		break;
	case DualGlobalCubicExtrude:
		// cout << "NOT YET IMPLEMENTED!!!" << endl;
		break;
	case DualCheckerBoard:
		subdivideDualCheckerBoard();
		break;
	case PentagonPreserving:
		subdividePentagonPreserving();
		break;
	case DualPentagonPreserving:
		subdivideDualPentagonPreserving();
		break;
	case HexagonPreserving:
		subdivideRoot4();
		break;
	case DualHexagonPreserving:
		subdivideDualHexagonPreserving();
		break;
	case Fractal:
		subdivideFractal();
		break;
	case ModifiedDoubleStellate:
		subStellate2();
		break;
	case Dome:
		subdivideDome();
		break;
	case DooSabinBC:
		subdivideDooSabinBC();
		break;
	case DooSabinBCNew:
		subdivideDooSabinBCNew();
		break;
	}
	setMode(mOperationMode);
	redraw();
}

void MainWindow::performExtrusion()
{
	if ( mActiveViewport->numSelectedFaces() < 1 )
	{
		return;
	}

	DLFLFacePtrArray& sfptrarr = mActiveViewport->getSelectedFaces();
	if (sfptrarr[0])
	{
		undoPush();
		setModified(true);
		for (auto sfPtr : sfptrarr)
		{
			switch (mExtrusionMode)
			{
			case DooSabinExtrude:
				DLFL::extrudeFaceDS(&mObject, sfPtr, sExtrudeDist, sNumExtrusions, sDooSabinExtrusionTwist, sExtrudeScale);
				break;
			case CubicalExtrude:
				DLFL::extrudeFace(&mObject, sfPtr, sExtrudeDist, sNumExtrusions, sExtrudeRot, sExtrudeScale);
				break;
				// case IcosahedralExtrude: DLFL::extrudeFaceIcosa(&object,sfPtr,extrude_dist,num_extrusions, ds_ex_twist,extrude_scale);
			case IcosahedralExtrude:
				// std::cout<< extrude_angle_icosa  << "\t" << num_extrusions  << "\t" << extrude_length1_icosa  << "\t" << extrude_length2_icosa << "\t" << extrude_length3_icosa <<"\n";
				DLFL::extrudeFaceIcosa(&mObject, sfPtr, sExtrudeAngle_iCosA, sNumExtrusions, sExtrudeLength1_iCosA, sExtrudeLength2_iCosA, sExtrudeLength3_iCosA);
				// DLFL::extrudeFaceCubOcta(&object, sfPtr, extrude_angle_icosa,num_extrusions, extrude_length1_icosa,extrude_length2_icosa,extrude_length3_icosa);
				break;
				// DLFLFacePtr extrudeFaceDodeca(DLFLObjectPtr obj, DLFLFacePtr fptr, double angle, int num, double ex_dist1, double ex_dist2, double ex_dist3, bool hexagonalize);
			case DodecahedralExtrude:
				DLFL::extrudeFaceDodeca(&mObject, sfPtr, sExtrudeAngle, sNumExtrusions, sExtrudeLength1, sExtrudeLength2, sExtrudeLength3, sHexagonalizeDodecaExtrude);
				// case DodecahedralExtrude: DLFL::extrudeFaceDodeca(&object,sfPtr,extrude_dist,num_extrusions, ds_ex_twist,extrude_scale, hexagonalize_dodeca_extrude);
				// DLFL::extrudeFaceSmallRhombiCubOcta(&object,sfPtr,extrude_angle,num_extrusions, extrude_length1,extrude_length2,extrude_length3);
				break;
			case OctahedralExtrude:
				DLFL::extrudeDualFace(&mObject, sfPtr, sExtrudeDist, sNumExtrusions, sExtrudeRot, sExtrudeScale, sDualMeshEdgesCheck);
				break;
			case StellateExtrude:
				DLFL::stellateFace(&mObject, sfPtr, sExtrudeDist);
				break;
			case DoubleStellateExtrude:
				DLFL::doubleStellateFace(&mObject, sfPtr, sExtrudeDist);
				break;
			case DomeExtrude:
				DLFL::extrudeFaceDome(&mObject, sfPtr, sDomeExtrudeLengthFactor, sDomeExtrudeRotationFactor, sDomeExtrudeScaleFactor);
				break;
			default:
				DLFL::extrudeFace(&mObject, sfPtr, sExtrudeDist, sNumExtrusions, sExtrudeRot, sExtrudeScale);
				break;
			};
		}
		mActiveViewport->recomputePatches();
		mActiveViewport->recomputeNormals();
	}
	mActiveViewport->clearSelectedFaces();
	redraw();
}

// Change the renderer for all viewports
void MainWindow::setRenderer(DLFLRendererPtr rp)
{
	mActiveViewport->setRenderer(rp);
}

// Return pointer to the active GLWidget
GLWidget *MainWindow::getActive()
{
	return mActiveViewport;
}

// Override redraw() method to mark subwindows also for redraw
void MainWindow::redraw()
{
	mActiveViewport->redraw();
}

//--- Methods to perform various operations ---//

// Switch to specified operating mode
void MainWindow::setMode(Mode m)
{
	mOperationMode = m;

	switch (mOperationMode)
	{
		/*case BezierMode: // brianb
		object.bezierDefaults();
		break;*/
	case EditVertex:    // brianb
	case SelectVertex:
	case MultiSelectVertex:
	case CutVertex:
	case MarkVertex:
		setSelectionMask(MainWindow::MaskVertices);
		MainWindow::sNumSelVerts = 0;
		break;
	case SelectEdge:
	case SelectEdgeLoop:
	case SelectEdgeRing:
	case MultiSelectEdge:
	case DeleteEdge:
	case SubdivideEdge:
	case CollapseEdge:
	case ConnectEdges:
	case CutEdge:
	case TruncateEdge:
	case MarkEdge:
		setSelectionMask(MainWindow::MaskEdges);
		// MainWindow::num_sel_edges = 0;
		break;
	case SelectFace:
	case SelectFaceLoop:
	case MultiSelectFace:
	case SelectCheckerboard:
	case ExtrudeFace:
	case ExtrudeFaceDS:
	case ExtrudeDualFace:
	case ExtrudeFaceDodeca:
	case ExtrudeFaceIcosa:
	case ExtrudeMultipleFaces:
	case StellateFace:
	case DoubleStellateFace:
	case ExtrudeFaceDome:
	case ConnectFaces:
	case ReorderFace:
	case SubdivideFace:
	case CrustModeling:
	case CutFace:
	case SelectFacesByArea:
	case SelectFacesByColor:
	case PaintFace:
	case EyeDropper:
		setSelectionMask(MainWindow::MaskFaces);
		// MainWindow::num_sel_faces = 0;
		break;
	case SelectSimilar:
		//do nothing...
		break;
	/*case SelectSimilar:
	{
		switch (selectionmask)
		{
		case MaskEdges:
			break;
		case MaskFaces:
			break;
		case MaskVertices:

			break;
		default:
			break;
		};
		break;
	}*/
	case SelectCorner:
	case MultiSelectCorner:
	case InsertEdge:
	case SpliceCorners:
	case ConnectFaceVertices:
	case BezierConnectFaces:
	case HermiteConnectFaces:
		setSelectionMask(MainWindow::MaskCorners);
		// MainWindow::num_sel_faceverts = 0;
		break;
	case SelectionWindow:
		// set
		break;
	case NormalMode:
		setSelectionMask(MainWindow::MaskNone);
	default:
		// Nothing to be done for other modes except clearing selection lists
		setSelectionMask(MainWindow::MaskNone);
		// MainWindow::clearSelected();
		break;
	}
	// if (mode != MultiSelectFace)
		// active->hideBrush();
	// else active->showBrush();

	QString s;
	//this switch statement is for setting the string for the Heads up display
	switch (mOperationMode)
	{
	case NormalMode: s = tr("Normal Mode");
		break;
	case SelectVertex: s = tr("Select Vertex");
		break;
	case SelectEdge: s = tr("Select Edge");
		break;
	case SelectFace: s = tr("Select Face");
		break;
	case SelectCorner: s = tr("Select Corner");
		break;
	case MultiSelectVertex: s = tr("Multi-Select Vertex");
		break;
	case MultiSelectEdge: s = tr("Multi-Select Edge");
		break;
	case MultiSelectFace: s = tr("Multi-Select Face");
		break;
	case MultiSelectCorner: s = tr("Multi-Select Corner");
		break;
	case SelectCheckerboard: s = tr("Select Checkerboard");
		break;
	case InsertEdge: s = tr("Insert Edge");
		break;
	case DeleteEdge: s = tr("Delete Edge");
		break;
	case SubdivideEdge: s = tr("Subdivide Edge");
		break;
	case CollapseEdge: s = tr("Collapse Edge");
		break;
	case SpliceCorners: s = tr("Splice Corners");
		break;
	case ConnectEdges: s = tr("Connect Edges");
		break;
	case ExtrudeFace: s = tr("Cubical Extrusion");
		break;
	case ExtrudeFaceDS: s = tr("Doo Sabin Extrusion");
		break;
	case ExtrudeDualFace: s = tr("Dual Extrusion");
		break;
	case ExtrudeFaceDodeca: s = tr("Dodecahedral Extrusion");
		break;
	case ExtrudeFaceIcosa: s = tr("Icosahedral Extrusion");
		break;
	case ExtrudeMultipleFaces: s = tr("Extrude Multiple Faces");
		break;
	case StellateFace: s = tr("Stellate Face");
		break;
	case DoubleStellateFace: s = tr("Double Stellate Face");
		break;
	case ExtrudeFaceDome: s = tr("Dome Extrusion");
		break;
	case ConnectFaceVertices: s = tr("Connect Corners");
		break;
	case ConnectFaces: s = tr("Connect Faces");
		break;
	case BezierConnectFaces: s = tr("Bezier Connect Faces");
		break;
	case HermiteConnectFaces: s = tr("Hermite Connect Faces");
		break;
	case ReorderFace: s = tr("Reorder Face");
		break;
	case SubdivideFace: s = tr("Subdivide Face");
		break;
	case CrustModeling: s = tr("Crust Modeling");
		break;
	case CutEdge: s = tr("Cut Edge");
		break;
	case CutVertex: s = tr("Cut Vertex");
		break;
	case CutEdgeandVertex: s = tr("Cut Edge and Vertex");
		break;
	case CutFace: s = tr("Cut Face");
		break;
	case TruncateEdge: s = tr("Truncate Edge");
		break;
	case MarkEdge: s = tr("Mark Edge");
		break;
	case MarkVertex: s = tr("Mark Vertex");
		break;
	case ConvexHullMode: s = tr("Convex Hull Mode");
		break;
	case EditVertex: s = tr("Edit Vertex");
		break;
	case SelectEdgeLoop: s = tr("Select Edge Loop");
		break;
	case SelectEdgeRing: s = tr("Select Edge Ring");
		break;
	case SelectFaceLoop: s = tr("Select Face Loop");
		break;
	case SelectSimilar: s = tr("Select Similar");
		break;
	case SelectFacesByArea: s = tr("Select Faces By Surf. Area");
		break;
	case SelectionWindow: s = tr("Selection Window");
		break;
	default:	s = tr("-");
		break;
	}
	mActiveViewport->setModeString(s);
	redraw();
}

void MainWindow::setExtrusionMode(ExtrusionMode mode){
	mExtrusionMode = mode;
	QString modeStr;
	switch(mode){
	case DooSabinExtrude:
		modeStr = "Doo Sabin";
		break;
	case CubicalExtrude:
		modeStr = "Cubical";
		break;
	case DodecahedralExtrude:
		modeStr = "Dodecahedral";
		break;
	case IcosahedralExtrude:
		modeStr = "Icosahedral";
		break;
	case OctahedralExtrude:
		modeStr = "Octahedral";
		break;
		// case Dual:
		// break;
	case StellateExtrude:
		modeStr = "Stellate";
		break;
	case DoubleStellateExtrude:
		modeStr = "Double Stellate";
		break;
	case DomeExtrude:
		modeStr = "Dome";
		break;
	default:
		break;
	}
	mActiveViewport->setExtrusionModeString(modeStr);
}

void MainWindow::setSelectionMask(SelectionMask mask)
{
	if (mSelectionMask == mask)
	{
		return;
	}

	mSelectionMask = mask;
	// active->clearSelected();
	mActiveViewport->repaint();
	//make sure we clear all other selected objects here...
	switch (mSelectionMask)
	{
	case MaskVertices:
		mActiveViewport->clearSelectedEdges();
		mActiveViewport->clearSelectedFaces();
		mActiveViewport->clearSelectedCorners();
		// mSelectVerticesMaskAct->activate(QAction::Trigger);
		mSelectVerticesMaskAct->setChecked(true);
		mSelectEdgesMaskAct->setChecked(false);
		mSelectFacesMaskAct->setChecked(false);
		mSelectCornersMaskAct->setChecked(false);
		mActiveViewport->setSelectionMaskString(tr("Vertices"));
		break;
	case MaskEdges:
		mActiveViewport->clearSelectedVertices();
		mActiveViewport->clearSelectedFaces();
		mActiveViewport->clearSelectedCorners();
		// mSelectEdgesMaskAct->activate(QAction::Trigger);
		mSelectVerticesMaskAct->setChecked(false);
		mSelectEdgesMaskAct->setChecked(true);
		mSelectFacesMaskAct->setChecked(false);
		mSelectCornersMaskAct->setChecked(false);
		mActiveViewport->setSelectionMaskString(tr("Edges"));
		break;
	case MaskFaces:
		mActiveViewport->clearSelectedEdges();
		mActiveViewport->clearSelectedVertices();
		mActiveViewport->clearSelectedCorners();
		// mSelectFacesMaskAct->activate(QAction::Trigger);
		mSelectVerticesMaskAct->setChecked(false);
		mSelectEdgesMaskAct->setChecked(false);
		mSelectFacesMaskAct->setChecked(true);
		mSelectCornersMaskAct->setChecked(false);
		mActiveViewport->setSelectionMaskString(tr("Faces"));
		break;
	case MaskCorners:
		mActiveViewport->clearSelectedEdges();
		mActiveViewport->clearSelectedFaces();
		mActiveViewport->clearSelectedVertices();
		// mSelectCornersMaskAct->activate(QAction::Trigger);
		mSelectVerticesMaskAct->setChecked(false);
		mSelectEdgesMaskAct->setChecked(false);
		mSelectFacesMaskAct->setChecked(false);
		mSelectCornersMaskAct->setChecked(true);
		mActiveViewport->setSelectionMaskString(tr("Corners"));
		break;
	default:
		mActiveViewport->clearSelectedEdges();
		mActiveViewport->clearSelectedVertices();
		mActiveViewport->clearSelectedCorners();
		mActiveViewport->clearSelectedFaces();
		mSelectVerticesMaskAct->setChecked(false);
		mSelectEdgesMaskAct->setChecked(false);
		mSelectFacesMaskAct->setChecked(false);
		mSelectCornersMaskAct->setChecked(false);
		mActiveViewport->setSelectionMaskString(tr("None"));

		break;
	}
}

void MainWindow::setRemeshingScheme(RemeshingScheme scheme)
{
	mRemeshingScheme = scheme;
	QString s;

	switch (mRemeshingScheme)
	{
	case Dual:
		s = tr("Dual");
		break;
	case Root3:
		s = tr("Root-3");
		break;
	case DualVertexTrunc:
		s = tr("Dual Vertex Truncation");
		break;
	case GlobalStellate:
		s = tr("Global Stellate");
		break;
	case Star:
		s = tr("Star");
		break;
	case Generic1264:
		s = tr("Generic 12.6.4");
		break;
	case Honeycomb:
		s = tr("Honeycomb");
		break;
	case VertexTrunc:
		s = tr("Vertex Truncation");
		break;
	case DualGeneric1264:
		s = tr("Dual Generic 12.6.4");
		break;
	case LinearVertexInsertion:
		s = tr("Linear Vertex Truncation");
		break;
	case CatmullClark:
		s = tr("Catmull-Clark");
		break;
	case ModifiedStellate:
		s = tr("Modified Stellate");
		break;
	case DooSabin:
		s = tr("Doo Sabin");
		break;
	case CornerCutting:
		s = tr("Corner Cutting");
		break;
	case ModifiedCornerCutting:
		s = tr("Modified Corner Cutting");
		break;
	case Simplest:
		s = tr("Simplest");
		break;
	case Pentagonal:
		s = tr("Pentagonal");
		break;
	case CubicPentagonal:
		s = tr("Cubic Pentagonal");
		break;
	case DualPentagonal:
		s = tr("Dual Pentagonal");
		break;
	case LoopStyle:
		s = tr("Loop Style");
		break;
	case Loop:
		s = tr("Loop");
		break;
	case Root4:
		s = tr("Root4");
		break;
	case DualLoop:
		s = tr("Dual Loop");
		break;
	case GlobalCubicExtrude:
		s = tr("Global Cubic Extrude");
		break;
	case CheckerBoard:
		s = tr("Checkerboard");
		break;
	case DualGlobalCubicExtrude:
		s = tr("Dual Global Cubic Extrude");
		break;
	case DualCheckerBoard:
		s = tr("Dual Checkerboard");
		break;
	case PentagonPreserving:
		s = tr("Pentagon Preserving");
		break;
	case DualPentagonPreserving:
		s = tr("Dual Pentagon Preserving");
		break;
	case HexagonPreserving:
		s = tr("Hexagon Preserving");
		break;
	case DualHexagonPreserving:
		s = tr("Dual Hexagon Preserving");
		break;
	case Fractal:
		s = tr("Fractal");
		break;
	case ModifiedDoubleStellate:
		s = tr("Modified Double Stellate");
		break;
	case Dome:
		s = tr("Dome");
		break;
	case DooSabinBC:
		s = tr("Doo Sabin BC");
		break;
	case DooSabinBCNew:
		s = tr("Doo Sabin BC New");
		break;
	default: s = tr("None");
		break;
	}
	mActiveViewport->setRemeshingSchemeString(s);
}

// Read the DLFL object from a file
void MainWindow::readObject(const char * filename, const char *mtlfilename)
{
	mActiveViewport->clearSelected();
	ifstream file, mtlfile;
	file.open(filename);
	mtlfile.open(mtlfilename);

	if (strstr(filename, ".dlfl") || strstr(filename, ".DLFL"))
		mObject.readDLFL(file, mtlfile);
	else if (strstr(filename, ".obj") || strstr(filename, ".OBJ"))
		mObject.readObject(file, mtlfile);
	file.close();
}

// Read the DLFL object from a file
void MainWindow::readObjectQFile(const QString &filename)
{
	mActiveViewport->clearSelected();
	QFile file(filename);
	file.open(QIODevice::ReadOnly | QIODevice::Text);

	QByteArray ba = file.readAll();
	const char *filecontents = ba.data();
	string str(filecontents);
	istringstream filestring(str);

	ifstream mtlfile;
	// mtlfile = 0;

	if (filename.endsWith(".dlfl", Qt::CaseInsensitive))
		mObject.readDLFL(filestring, mtlfile);
	else if (filename.endsWith(".obj", Qt::CaseInsensitive))
		mObject.readObject(filestring, mtlfile);
	file.close();

#ifdef WITH_PYTHON
	DLFLObjectPtr obj = &mObject;
	if (obj)
		emit loadedObject(obj, filename);
#endif
	mActiveViewport->createPatchObject();

	// std::cout << "readObjectQFile end\n";
}

// Read the DLFL object from a file - use alternate OBJ reader for OBJ files
void MainWindow::readObjectAlt(const char * filename)
{
	// active->clearSelected();
	// ifstream file;
	// file.open(filename);
	// if ( strstr(filename,".dlfl") || strstr(filename,".DLFL") )
	// 	object.readDLFL(file);
	// else if ( strstr(filename,".obj") || strstr(filename,".OBJ") )
	// 	object.readObjectAlt(file);
	// file.close();
}

// Write the DLFL object to a file
void MainWindow::writeObject(
	const char * filename, const char* mtlfilename,
	bool with_normals, bool with_tex_coords)
{
	ofstream file;
	ofstream mtlfile;
	file.open(filename);
	mtlfile.open(mtlfilename);

	std::cout << mtlfilename << " = mtlfilename in writeObject function\n";

	if (strstr(filename, ".dlfl") || strstr(filename, ".DLFL"))
	{
		mObject.writeDLFL(file, mtlfile);
	}
	else if (strstr(filename, ".obj") || strstr(filename, ".OBJ"))
	{
		mObject.writeObject(file, mtlfile, with_normals, with_tex_coords);
	}
	file.close();
	mtlfile.close();
}

// Write the DLFL object to a file
void MainWindow::writeMTL(const char * filename)
{
	ofstream file;
	file.open(filename);
	mObject.writeMTL(file);
	file.close();
}

/* stuart - bezier export */
void MainWindow::writePatchOBJ(const char *filename)
{
	if (mActiveViewport->patchobject() != nullptr)
	{
		ofstream file;
		file.open(filename);
		mActiveViewport->patchobject()->objPatchWrite(file);
	}
}

/* dave - lg3d export */
void MainWindow::writeLG3d(const char *filename, bool selected)
{
	// if( active->patchobject() != nullptr ) {
	ofstream file;
	file.open(filename);
	mObject.writeLG3d(file, selected);
	file.close();
	// }
}

/* dave - lg3d export */
void MainWindow::writeSTL(const char *filename)
{
	ofstream file;
	file.open(filename);
	mObject.writeSTL(file);
	file.close();
}

// void MainWindow::writeObjectDLFL(const char * filename) {
// 	ofstream file;
// 	file.open(filename);
// 	object.writeDLFL(file);
// 	file.close();
// }

// File handling
void MainWindow::openFile(void)
{
	QString fileName = QFileDialog::getOpenFileName(
		this, tr("Open File..."), mSaveDirectory,
		tr("All Supported Files (*.obj *.dlfl);;Wavefront OBJ Files (*.obj);;DLFL Files (*.dlfl);;All Files (*)"),
		0, QFileDialog::DontUseSheet);

	if (!fileName.isEmpty())
	{
		if (!mCurFilename.isEmpty())
		{
			undoPush();
			setModified(false);
		}
		QByteArray ba = fileName.toLatin1();
		const char *filename = ba.data();
		mWasPrimitive = false;
		mIsPrimitive = false;

		mSaveDirectory = QFileInfo(fileName).absoluteDir().absolutePath();
		QByteArray ba2 = mSaveDirectory.toLatin1();
		const char *dirname = ba2.data();
		mObject.setDirname(dirname);

		QString mtlfile = mSaveDirectory + "/" + QFileInfo(fileName).baseName() + ".mtl";
		QByteArray ba3 = mtlfile.toLatin1();
		const char *mtlfilename = ba3.data();

		setCurrentFile(fileName);
		std::cout << "filename for DLFL reading = " << filename << endl;
		readObject(filename, mtlfilename);

#ifdef WITH_PYTHON
		// Emit and send to python script editor
		DLFLObjectPtr obj = &mObject;
		if (obj)
			emit loadedObject(obj, fileName);
#endif
		mActiveViewport->recomputePatches();
		mActiveViewport->recomputeNormals();
		mActiveViewport->redraw();
	}
}

bool MainWindow::saveFile(bool with_normals, bool with_tex_coords)
{
	if (mCurFilename != "untitled")
	{
		statusBar()->showMessage(tr("Saving File..."), 3000);
		QString curFileTemp(mCurFilename);

		if (!mCurFilename.isEmpty())
		{
			if (mIncrementalSave)
			{
				if (sIncrementalSaveCount >= mIncrementalSaveMax)
				{
					//go back to zero, start overwriting files...
					sIncrementalSaveCount = 0;
					//insert 00 at the end of hte name before the last dot
					if (curFileTemp.lastIndexOf(".") > -1)
						curFileTemp.insert(curFileTemp.lastIndexOf("."), "000");
					else curFileTemp.append("000");
				}
				else
				{
					//add the leading zeros for numbers less than 10
					QString t;
					if (sIncrementalSaveCount < 10)
						t = QString("00%1").arg(sIncrementalSaveCount);
					else t = QString("0%1").arg(sIncrementalSaveCount);
					curFileTemp.insert(curFileTemp.lastIndexOf("."), t);
				}
			}
			//add in the directory name
			QString fullpath = mSaveDirectory + "/" + curFileTemp;
			QByteArray ba = fullpath.toLatin1();
			const char *filename = ba.data();

			//materials test dave 11.07
			QString mtlFileName = fullpath;
			mtlFileName.replace(QString(".obj"), QString(".mtl"), Qt::CaseInsensitive);
			mtlFileName.replace(QString(".dlfl"), QString(".mtl"), Qt::CaseInsensitive);
			QByteArray ba2 = mtlFileName.toLatin1();
			const char *mtlfilename = ba2.data();
			// writeMTL(mtlfilename);

			writeObject(filename, mtlfilename, with_normals, with_tex_coords);


			if (mIncrementalSave)
				sIncrementalSaveCount++;
			setModified(false);
			// statusBar()->clearMessage();
			return true;
		}
		else
		{
			QString fileName = QFileDialog::getSaveFileName(
				this, tr("Save File As..."),
				mSaveDirectory + "/" + curFileTemp,
				tr("All Supported Files (*.obj *.dlfl);;Wavefront OBJ Files (*.obj);;DLFL Files (*.dlfl);;All Files (*)"),
				0, QFileDialog::DontUseSheet);

			if (!fileName.isEmpty())
			{
				//for incremental save test - dave
				sIncrementalSaveCount = 0;
				if (mIncrementalSave)
				{
					//insert 000's
					std::cout << fileName.toLatin1().data() << "\n";
					if (fileName.lastIndexOf(".") > -1)
						fileName.insert(fileName.lastIndexOf("."), "000");
					else fileName.append("000");
				}

				if (!fileName.endsWith(".obj", Qt::CaseInsensitive) ||
					!fileName.endsWith(".dlfl", Qt::CaseInsensitive))
					fileName.append(".obj");
				setCurrentFile(fileName);

				mSaveDirectory = QFileInfo(fileName).absoluteDir().absolutePath();
				mPreferencesDialog->setSaveDirectory(mSaveDirectory);

				QByteArray ba = fileName.toLatin1();
				const char *filename = ba.data();

				//materials test dave 11.07
				QString mtlFileName = fileName;
				mtlFileName.replace(QString(".obj"), QString(".mtl"), Qt::CaseInsensitive);
				mtlFileName.replace(QString(".dlfl"), QString(".mtl"), Qt::CaseInsensitive);
				QByteArray ba2 = mtlFileName.toLatin1();
				const char *mtlfilename = ba2.data();
				// writeMTL(mtlfilename);

				writeObject(filename, mtlfilename, with_normals, with_tex_coords);


				if (mIncrementalSave)
					sIncrementalSaveCount++;
				setModified(false);
				// statusBar()->clearMessage();
				return true;
			}
		}
	}
	// statusBar()->clearMessage();
	return false;
}

bool MainWindow::saveFileAs(bool with_normals, bool with_tex_coords)
{

	statusBar()->showMessage(tr("Saving File..."));

	QString fileName = QFileDialog::getSaveFileName(
		this, tr("Save File As..."),
		mSaveDirectory + "/" + mCurFilename,
		tr("All Supported Files (*.obj *.dlfl);;Wavefront OBJ Files (*.obj);;DLFL Files (*.dlfl);;All Files (*)"),
		0, QFileDialog::DontUseSheet);

	if (!fileName.isEmpty())
	{
		//reset the incremental save count no matter what...?
		sIncrementalSaveCount = 0;
		if (mIncrementalSave)
		{
			//account for leading zeros for digits 0-9
			if (fileName.lastIndexOf(".") > -1)
				fileName.insert(fileName.lastIndexOf("."), "000");
			else fileName.append("000");
		}
		if (!fileName.endsWith(".obj", Qt::CaseInsensitive) ||
			!fileName.endsWith(".dlfl", Qt::CaseInsensitive))
			fileName.append(".dlfl");
		setCurrentFile(fileName);

		mSaveDirectory = QFileInfo(fileName).absoluteDir().absolutePath();
		mPreferencesDialog->setSaveDirectory(mSaveDirectory);

		QByteArray ba = fileName.toLatin1();
		const char *filename = ba.data();

		//materials test dave 11.07
		QString mtlFileName = fileName;
		mtlFileName.replace(QString(".obj"), QString(".mtl"), Qt::CaseInsensitive);
		mtlFileName.replace(QString(".dlfl"), QString(".mtl"), Qt::CaseInsensitive);
		QByteArray ba2 = mtlFileName.toLatin1();
		const char *mtlfilename = ba2.data();
		// writeMTL(mtlfilename);

		writeObject(filename, mtlfilename, with_normals, with_tex_coords);

		if (mIncrementalSave)
			sIncrementalSaveCount++;

		statusBar()->clearMessage();
		return true;
	}
	statusBar()->clearMessage();
	return false;
}

void MainWindow::setCurrentFile(const QString &fileName)
{

	mCurFilename = QFileInfo(fileName).fileName();

	//dir name for mtl loading...??? dave 11.07
	QString dirName = QFileInfo(fileName).absolutePath();
	QByteArray ba2 = dirName.toLatin1();
	const char *dirname = ba2.data();
	mObject.setDirname(dirname);

	//base file name for the mtllib thing...
	QString baseName = QFileInfo(fileName).baseName();
	QByteArray ba = baseName.toLatin1();
	const char *filename = ba.data();
	mObject.setFilename(filename);

	QString shownName;
	if (mCurFilename.isEmpty())
		shownName = "untitled.obj";
	else
		shownName = mCurFilename;

	setWindowTitle(tr("%1[*] - %2").arg(shownName).arg(tr("TopMod")));
	setModified(false);
}


/* stuart - bezier export */
bool MainWindow::saveFileBezierOBJ()
{
	QString fileName = QFileDialog::getSaveFileName(
		this, tr("Save Bezier Patch (OBJ)..."),
		mSaveDirectory + "/" + mCurFilename,
		tr("Wavefront OBJ Files (*.obj);;All Files (*)"),
		0, QFileDialog::DontUseSheet);

	if (!fileName.isEmpty())
	{
		QByteArray ba = fileName.toLatin1();
		const char *filename = ba.data();
		writePatchOBJ(filename);
		return true;
	}
	return false;
}

/* dave - lg3d export */
bool MainWindow::saveFileLG3d()
{
	QString fileName = QFileDialog::getSaveFileName(
		this, tr("Export to LiveGraphics3D  (M)..."),
		mSaveDirectory + "/" + mCurFilename,
		tr("Mathematica Graphics3D Files (*.m);;All Files (*)"),
		0, QFileDialog::DontUseSheet);
	if (!fileName.isEmpty())
	{
		QByteArray ba = fileName.toLatin1();
		const char *filename = ba.data();
		writeLG3d(filename, false);
		return true;
	}
	return false;
}

bool MainWindow::saveFileLG3dSelected()
{
	QString fileName = QFileDialog::getSaveFileName(
		this, tr("Export Selected Faces to LiveGraphics3D  (M)..."),
		mSaveDirectory + "/" + mCurFilename,
		tr("Mathematica Graphics3D Files (*.m);;All Files (*)"),
		0, QFileDialog::DontUseSheet);
	if (!fileName.isEmpty())
	{
		QByteArray ba = fileName.toLatin1();
		const char *filename = ba.data();
		writeLG3d(filename, true);
		return true;
	}
	return false;
}

/* dave - stl export */
bool MainWindow::saveFileSTL()
{

//triangulate the mesh first - this isn't the best solution but will work for now.
// globalStellate();
//testing out a custom triangulate function
	triangulate();

	QString fileName = QFileDialog::getSaveFileName(
		this, tr("Export STL..."),
		mSaveDirectory + "/" + mCurFilename,
		tr("STL Files (*.stl);;All Files (*)"),
		0, QFileDialog::DontUseSheet);
	if (!fileName.isEmpty())
	{
		QByteArray ba = fileName.toLatin1();
		const char *filename = ba.data();
		writeSTL(filename);
		return true;
	}
	return false;
}

/* dave - png opengl viewport screenshot export */
bool MainWindow::viewportScreenshot()
{
	// viewportPixmap = QPixmap::grabWidget(active,0,0,active->width(),active->height());
	// viewportPixmap = QPixmap::grabWindow(active->winId(),/*mapToGlobal(active->pos()).x()*/0,/*mapToGlobal(active->pos()).y()*/0,active->width(),active->height());
	QImage image = mActiveViewport->grabFrameBuffer(true);
	QString format = "png";
	QString initialPath = mSaveDirectory + tr("/untitled.") + format;

	QString fileName = QFileDialog::getSaveFileName(
		this, tr("Save Viewport Screenshot As"),
		initialPath,
		tr("%1 Files (*.%2);;All Files (*)").arg(format.toUpper()).arg(format),
		0, QFileDialog::DontUseSheet);
	if (!fileName.isEmpty())
	{
		image.save(fileName, format.toLatin1());
		return true;
	}
	return false;
}

bool MainWindow::appScreenshot()
{
	// appPixmap = QPixmap::grabWidget(this);
	mAppPixmap = QPixmap::grabWindow(this->winId());

	QString format = "png";
	QString initialPath = mSaveDirectory + tr("/untitled.") + format;

	QString fileName = QFileDialog::getSaveFileName(
		this, tr("Save App Screenshot As"), initialPath,
		tr("%1 Files (*.%2);;All Files (*)").arg(format.toUpper()).arg(format),
		0, QFileDialog::DontUseSheet);
	if (!fileName.isEmpty())
	{
		mAppPixmap.save(fileName, format.toLatin1());
		return true;
	}
	return false;
}

void MainWindow::loadCube()
{
	if (isModified())
		undoPush();
	setModified(false);
	mIsPrimitive = true;
	mWasPrimitive = true;
	setCurrentFile(tr("cube.obj"));
	readObjectQFile(":/cube.obj");

	//iterate through faces and color them red... just to test the renderer out...
	// DLFLFacePtrArray fparray;
	// object.getFaces(fparray);
	// for (auto fPtr : fparray){
	// 	fPtr->material()->setColor(1.0,0.0,0.0);
	// }//end for loop



	//active->recomputePatches();
	mActiveViewport->recomputeNormals();
	mActiveViewport->redraw();
}

void MainWindow::loadOctahedron()
{
	if (isModified())
		undoPush();
	setModified(false);
	mIsPrimitive = true;
	mWasPrimitive = true;
	setCurrentFile(tr("octahedron.obj"));
	readObjectQFile(":/octahedron.obj");
	//active->recomputePatches();
	mActiveViewport->recomputeNormals();
	mActiveViewport->redraw();
}

void MainWindow::loadTetrahedron()
{
	if (isModified())
		undoPush();
	setModified(false);
	mIsPrimitive = true;
	mWasPrimitive = true;
	setCurrentFile(tr("tetrahedron.obj"));
	readObjectQFile(":/tetrahedron.obj");
	//active->recomputePatches();
	mActiveViewport->recomputeNormals();
	mActiveViewport->redraw();
}

void MainWindow::loadDodecahedron()
{
	if (isModified())
		undoPush();
	setModified(false);
	mIsPrimitive = true;
	mWasPrimitive = true;
	setCurrentFile(tr("dodecahedron.obj"));
	readObjectQFile(":/dodecahedron.obj");
	//active->recomputePatches();
	mActiveViewport->recomputeNormals();
	mActiveViewport->redraw();
}

void MainWindow::loadIcosahedron()
{
	if (isModified())
		undoPush();
	setModified(false);
	mIsPrimitive = true;
	mWasPrimitive = true;
	setCurrentFile(tr("icosahedron.obj"));
	readObjectQFile(":/icosahedron.obj");
	//active->recomputePatches();
	mActiveViewport->recomputeNormals();
	mActiveViewport->redraw();
}

void MainWindow::loadSoccerball()
{
	if (isModified())
		undoPush();
	setModified(false);
	mIsPrimitive = true;
	mWasPrimitive = true;
	setCurrentFile(tr("soccerball.obj"));
	readObjectQFile(":/soccerball.obj");
	//active->recomputePatches();
	mActiveViewport->recomputeNormals();
	mActiveViewport->redraw();
}

void MainWindow::loadGeodesic()
{
	if (isModified())
		undoPush();
	setModified(false);
	mIsPrimitive = true;
	mWasPrimitive = true;
	setCurrentFile(tr("geodesic.obj"));
	readObjectQFile(":/geodesic.obj");
	//active->recomputePatches();
	mActiveViewport->recomputeNormals();
	mActiveViewport->redraw();
}

bool MainWindow::isModified()
{
	return mIsModified;
}

void MainWindow::setModified(bool isModified)
{
	mIsModified = isModified;
	setWindowModified(mIsModified);
}

void MainWindow::switchPerspView()
{
	mActiveViewport->resetCamera();
	// active->switchTo(VPPersp);
	mActiveViewport->redraw();
}

void MainWindow::switchTopView()
{
	// active->switchTo(VPTop);
	mActiveViewport->redraw();
}

void MainWindow::switchBottomView()
{
	// active->switchTo(VPBottom);
	mActiveViewport->redraw();
}

void MainWindow::switchRightView()
{
	// active->switchTo(VPRight);
	mActiveViewport->redraw();
}

void MainWindow::switchLeftView()
{
	// active->switchTo(VPLeft);
	mActiveViewport->redraw();
}

void MainWindow::switchFrontView()
{
	// active->switchTo(VPFront);
	mActiveViewport->redraw();
}

void MainWindow::switchBackView()
{
	// active->switchTo(VPBack);
	mActiveViewport->redraw();
}

//recurse through selected faces to get a list of checkerboard selection for the entire object
void MainWindow::getCheckerboardSelection(DLFLFacePtr selectedFacePtr)
{
	if (selectedFacePtr)
	{
		int numShared = 0;
		DLFLFacePtrArray fparray;
		selectedFacePtr->getNeighboringFaces(fparray);
		for (auto fPtr : fparray)
		{
			if (selectedFacePtr->sharesOneVertex(fPtr) && !mActiveViewport->isSelected(fPtr))
			{
				numShared++;
				if (sDeselectEdges)
				{
					mActiveViewport->clearSelectedFace(fPtr);
					sNumSelFaces--;
				}
				else
				{
					mActiveViewport->setSelectedFace(fPtr);
					sNumSelFaces++;
				}
				getCheckerboardSelection(fPtr);
			}
		}//end for loop
	}
}

//recurse through selected edge to get a list of
void MainWindow::getEdgeLoopSelection(DLFLEdgePtr selectedEdgePtr)
{
	if (selectedEdgePtr)
	{
		//first check to see if edge only connects to only three other edges
		DLFLVertexPtr vp1, vp2;
		DLFLEdgePtrArray eparray;
		selectedEdgePtr->getVertexPointers(vp1, vp2);
		if (vp1->valence() == 4 && vp2->valence() == 4)
		{
			//get edges connected to vertex 1
			eparray.clear();
			vp1->getEdges(eparray);
			//loop through them to find the one that doesn't share a face
			for (auto ePtr : eparray)
			{
				if (!coFacial(ePtr, selectedEdgePtr))
				{
					if (sDeselectEdges)
					{
						mActiveViewport->clearSelectedEdge(ePtr);
						sNumSelEdges--;
						getEdgeLoopSelection(ePtr);
					}
					else if (!mActiveViewport->isSelected(ePtr))
					{
						mActiveViewport->setSelectedEdge(ePtr);
						sNumSelEdges++;
						getEdgeLoopSelection(ePtr);
					}
				}
			}//end for loop
			//get edges connected to vertex 2
			eparray.clear();
			vp2->getEdges(eparray);
			//loop through them to find the one that doesn't share a face
			for (auto ePtr : eparray)
			{
				if (!coFacial(ePtr, selectedEdgePtr))
				{
					if (sDeselectEdges)
					{
						mActiveViewport->clearSelectedEdge(ePtr);
						sNumSelEdges--;
						getEdgeLoopSelection(ePtr);
					}
					else if (!mActiveViewport->isSelected(ePtr))
					{
						mActiveViewport->setSelectedEdge(ePtr);
						sNumSelEdges++;
						getEdgeLoopSelection(ePtr);
					}
				}
			}//end for loop
			return;
		}//end if vp->valence
		return;
	}
}

//recurse through selected edge to get a list of
void MainWindow::getFaceLoopSelection(
	DLFLEdgePtr selectedEdgePtr, bool start,
	DLFLFacePtr face_loop_marker, bool select_face_loop)
{
	if ((selectedEdgePtr == sFaceLoopStartEdge) && !start)
		return;

	int idx = 0;
	DLFLEdgePtrArray edges;
	DLFLFacePtr fptr1, fptr2;
	//get the two faces corresponding to this edge ptr
	selectedEdgePtr->getFacePointers(fptr1, fptr2);
	//check if the two faces are quads
	if (fptr1 && fptr1->numFaceVertexes() == 4 && !(fptr1 == face_loop_marker))
	{
		if (!mActiveViewport->isSelected(fptr1))
		{
			mActiveViewport->setSelectedFace(sNumSelFaces, fptr1);
			sNumSelFaces++;
		}
		fptr1->getEdges(edges);
		idx = 0;
		for (auto ePtr : edges)
		{
			if (ePtr == selectedEdgePtr)
			{
				getFaceLoopSelection(edges[(idx + 2) % 4], false, fptr1, select_face_loop);
			}
			idx++;
		}//end for loop
	}
	if (fptr2 && fptr2->numFaceVertexes() == 4 && !(fptr2 == face_loop_marker))
	{
		if (!mActiveViewport->isSelected(fptr2))
		{
			mActiveViewport->setSelectedFace(sNumSelFaces, fptr2);
			sNumSelFaces++;
		}
		fptr2->getEdges(edges);
		idx = 0;
		for (auto ePtr : edges)
		{
			if (ePtr == selectedEdgePtr)
			{
				getFaceLoopSelection(edges[(idx + 2) % 4], false, fptr2, select_face_loop);
				// return;
			}
			idx++;
		}//end for loop
	}
}

//recurse through selected edge to get a list of
void MainWindow::getEdgeRingSelection(
	DLFLEdgePtr selectedEdgePtr, bool start,
	DLFLFacePtr face_loop_marker, bool select_face_loop)
{
	if ((selectedEdgePtr == sEdgeRingStartEdge) && !start)
		return;

	int idx = 0;
	DLFLEdgePtrArray edges;
	DLFLFacePtr fptr1, fptr2;
	if (!mActiveViewport->isSelected(selectedEdgePtr))
	{
		mActiveViewport->setSelectedEdge(selectedEdgePtr);
		sNumSelEdges++;
	}
	//get the two faces corresponding to this edge ptr
	selectedEdgePtr->getFacePointers(fptr1, fptr2);
	//check if the two faces are quads
	if (fptr1 && fptr1->numFaceVertexes() == 4 && !(fptr1 == face_loop_marker))
	{
		fptr1->getEdges(edges);
		idx = 0;
		for (auto ePtr : edges)
		{
			if (ePtr == selectedEdgePtr)
			{
				getEdgeRingSelection(edges[(idx + 2) % 4], false, fptr1, select_face_loop);
			}
			idx++;
		}//end for loop
	}
	if (fptr2 && fptr2->numFaceVertexes() == 4 && !(fptr2 == face_loop_marker))
	{
		fptr2->getEdges(edges);
		idx = 0;
		for (auto ePtr : edges)
		{
			if (ePtr == selectedEdgePtr)
			{
				getEdgeRingSelection(edges[(idx + 2) % 4], false, fptr2, select_face_loop);
			}
			idx++;
		}//end for loop
	}
}

void MainWindow::setLanguageSpanish()
{
	changeLanguage("es");
}

void MainWindow::setLanguageFrench()
{
	changeLanguage("fr");
}

void MainWindow::setLanguageGerman()
{
	changeLanguage("de");
}

void MainWindow::setLanguageTurkish()
{
	changeLanguage("tr");
}

void MainWindow::setLanguageItalian()
{
	changeLanguage("it");
}

void MainWindow::setLanguageHindi()
{
	changeLanguage("hi");
}

void MainWindow::setLanguageCatalan()
{
	changeLanguage("ca");
}

void MainWindow::setLanguageEnglish()
{
	changeLanguage("en");
}

void MainWindow::changeLanguage(const QString & string)
{

	if (string == QString("es"))
	{
		mTranslator_es->load(QString(":lang/topmod_es"));
		QCoreApplication::installTranslator(mTranslator_es);
	}
	if (string == QString("fr"))
	{
		mTranslator_fr->load(QString(":lang/topmod_fr"));
		QCoreApplication::installTranslator(mTranslator_fr);
	}
	if (string == QString("de"))
	{
		mTranslator_de->load(QString(":lang/topmod_de"));
		QCoreApplication::installTranslator(mTranslator_de);
	}
	if (string == QString("tr"))
	{
		mTranslator_tr->load(QString(":lang/topmod_tr"));
		QCoreApplication::installTranslator(mTranslator_tr);
	}
	if (string == QString("it"))
	{
		mTranslator_it->load(QString(":lang/topmod_it"));
		QCoreApplication::installTranslator(mTranslator_it);
	}
	if (string == QString("hi"))
	{
		mTranslator_hi->load(QString(":lang/topmod_hi"));
		QCoreApplication::installTranslator(mTranslator_hi);
	}
	if (string == QString("ca"))
	{
		mTranslator_ca->load(QString(":lang/topmod_ca"));
		QCoreApplication::installTranslator(mTranslator_ca);
	}
	if (string == QString("en"))
	{
		QCoreApplication::removeTranslator(mTranslator_es);
		QCoreApplication::removeTranslator(mTranslator_fr);
		QCoreApplication::removeTranslator(mTranslator_de);
		QCoreApplication::removeTranslator(mTranslator_tr);
		QCoreApplication::removeTranslator(mTranslator_it);
		QCoreApplication::removeTranslator(mTranslator_hi);
		QCoreApplication::removeTranslator(mTranslator_ca);
	}
}

/**
* \brief realtime app translation function
*
* \todo all translatable text will eventually go in this function, but that would take forever! we'll see if there's another
* way to handle that. it's probably going to be a matter of putting all the QAction->setText() and
* QAction->setTooltip() calls into one function. This is going to be a pain but could be beneficial in the long run
* I'm testing it out right now. we'll see how this goes...
*
*/
void MainWindow::retranslateUi()
{
	//retranslate all the operating mode classes
	mBasicsMode->retranslateUi();
	mExtrusionsMode->retranslateUi();
	mRemeshingMode->retranslateUi();
	mTexturingMode->retranslateUi();
	mHighgenusMode->retranslateUi();
	mConicalMode->retranslateUi();
	mExperimentalMode->retranslateUi();

	mStartupDialogDockWidget->setWindowTitle(tr("Learning Movies"));
	mToolOptionsDockWidget->setWindowTitle(mToolOptionsDockWidget->windowTitle());

#ifdef WITH_PYTHON
	mScriptEditor->retranslateUi();
	mScriptEditorDockWidget->setWindowTitle(tr("Script Editor"));
	mShowScriptEditorAct->setStatusTip(tr("Show the script editor to execute DLFL commands"));
#endif

	//from createMenus()
	mFileMenu->setTitle(tr("&File"));
#ifdef WITH_VERSE
	mVerseMenu->setTitle(tr("&Verse"));
#endif
	mEditMenu->setTitle(tr("&Edit"));
	mDisplayMenu->setTitle(tr("&Display"));
	mRendererMenu->setTitle(tr("&Renderer"));
	mShowIDsMenu->setTitle(tr("&Show IDs"));
	mNewMenu->setTitle(tr("&New"));
	mPrimitivesMenu->setTitle(tr("&Primitives"));
	mSelectionMenu->setTitle(tr("&Selection"));
	mToolsMenu->setTitle(tr("&Tools"));
	mObjectMenu->setTitle(tr("&Object"));
	mSelectionMaskMenu->setTitle(tr("Selection &Masks"));
	mWindowMenu->setTitle(tr("&Window"));
	mHelpMenu->setTitle(tr("&Help"));
	mLanguageMenu->setTitle(tr("&Language"));


	//from creatActions()!
	mNewFileAct->setText(tr("&New File..."));
	mNewFileAct->setStatusTip(tr("Open a blank file and clear the undo list"));
	mOpenAct->setText(tr("&Open..."));
	mOpenAct->setStatusTip(tr("Open an existing file"));
	mSaveAct->setText(tr("&Save"));
	mSaveAct->setStatusTip(tr("Save the document to disk"));
	mSaveAsAct->setText(tr("Save &As..."));
	mSaveAsAct->setStatusTip(tr("Save the document under a new name"));
	mSavePatchesAct->setText(tr("Save &Patch OBJ..."));
	mSavePatchesAct->setStatusTip(tr("Save a bezier patch .obj file"));
	mSaveLG3dAct->setText(tr("Export LiveGrahpics3D..."));
	mSaveLG3dAct->setStatusTip(tr(
		"Export a LiveGraphics3D (*.m) file for embedding into the TopMod Wiki, "\
		"Warning: you cannot import this file back into TopMod"));
	mSaveLG3dSelectedAct->setText(tr("Export LG3d (Sel. Faces)..."));
	mSaveLG3dSelectedAct->setStatusTip(tr(
		"Export a LiveGraphics3D (*.m) of the current selected faces file "\
		"for embedding into the TopMod Wiki, "\
		"Warning: you cannot import this file back into TopMod"));
	mExportSTLAct->setText(tr("Export STL..."));
	mExportSTLAct->setStatusTip(tr(
		"Export a stereolithography (*.stl) file "
		"for use with various rapid prototyping software and hardware"));
	mScreenshotAppAct->setText(tr("Save App Screenshot..."));
	mScreenshotAppAct->setStatusTip(tr("Save a screenshot of the entire main application window (*.png)"));
	mScreenshotViewportAct->setText(tr("Save Viewport Screenshot..."));
	mScreenshotViewportAct->setStatusTip(tr("Save a screenshot of the contents of the OpenGL viewport (*.png)"));
	mLoadTextureAct->setText(tr("Load &Texture..."));
	mLoadTextureAct->setStatusTip(tr("Load Texture from file"));
	mPrintInfoAct->setText(tr("Print &Information"));
	mPrintInfoAct->setStatusTip(tr("Print Information to the console"));
	mPrintFaceListAct->setText(tr("Print &Face List"));
	mPrintFaceListAct->setStatusTip(tr("Print Face List to the console"));
	mPrintVertexListAct->setText(tr("Print &Vertex List"));
	mPrintVertexListAct->setStatusTip(tr("Save the document under a new name"));
	mPrintEdgeListAct->setText(tr("Print &Edge List"));
	mPrintEdgeListAct->setStatusTip(tr("Print Edge list to the console"));
	mPrintCVListAct->setText(tr("Print &CV List"));
	mPrintCVListAct->setStatusTip(tr("Print CV list to the console"));
	mExitAct->setText(tr("E&xit"));
	mExitAct->setStatusTip(tr("Exit the application"));


	//quick command quicksilver like interface
#ifdef QCOMPLETER
	mQuickCommandAct->setText(tr("Quick Command"));
	mQuickCommandAct->setStatusTip(tr("Quick Command Access with Autocompletion"));
#endif
	//Edit Menu Actions
	mDeleteSelectedAct->setText(/*QIcon(":images/edit-undo.png"),*/ tr("&Delete Selected"));
	mDeleteSelectedAct->setStatusTip(tr("Delete Selected"));
	mUndoAct->setText(tr("&Undo"));
	mRedoAct->setText(tr("&Redo"));
	mClearUndoListAct->setText(tr("&Clear Undo List"));
	//View Menu Actions
	mPerspViewAct->setText(tr("&Reset Camera"));
	mZoomOutAct->setText(tr("Zoom Out"));
	mZoomInAct->setText(tr("Zoom In"));
	// mTopViewAct->setText( tr("&Top View"));
	// 	mBottomViewAct->setText( tr("&Bottom View"));
	// 	mFrontViewAct->setText( tr("&Front View"));
	// 	mBackViewAct->setText( tr("B&ack View"));
	// 	mLeftViewAct->setText( tr("&Left View"));
	// 	mRightViewAct->setText( tr("&Right View"));

	//Display Menu Actions
	mFullscreenAct->setText(tr("&Full Screen"));
	mFullscreenAct->setStatusTip(tr("Toggle Full Screen"));
	mShowVerticesAct->setText(tr("Show &Vertices"));
	mShowFaceIDsAct->setText(tr("Show &Face IDs"));
	mShowEdgeIDsAct->setText(tr("Show &Edge IDs"));
	mShowVertexIDsAct->setText(tr("Show &Vertex IDs"));
	mShowSelectedIDsAct->setText(tr("Show &Selected IDs"));
	mShowSilhouetteAct->setText(tr("Show &Silhouette"));
	mShowWireframeAct->setText(tr("Show &Wireframe"));
	mShowCoordinateAxesAct->setText(tr("Show &Coordinate Axes"));
	mObjectOrientationAct->setText(tr("Reverse Object"));
	mShowNormalsAct->setText(tr("Show &Normals"));
	mShowFaceCentroidsAct->setText(tr("Show &Face Centroids"));
	mShowHUDAct->setText(tr("Show &Heads Up Display"));
	mShowHUDAct->setStatusTip(tr("Show the Heads Up Display"));
#ifdef GPU_OK
	mUseGPUAct->setText(tr("&Use GPU Shading"));
	mUseGPUAct->setStatusTip(tr("Use GPU Shading"));
#endif
	mAntialiasingAct->setText(tr("Toggle &Antialiasing"));
	mAntialiasingAct->setStatusTip(tr("Toggle Antialiasing"));

#ifdef WITH_PYTHON
	mShowScriptEditorAct->setStatusTip(tr("Show the script editor to execute DLFL commands"));
#endif
#ifdef WITH_VERSE
	mShowVerseDialogAct->setStatusTip(tr("Show the verse dialog to view verse server connection status"));
#endif

	mShowToolOptionsAct->setStatusTip(tr("Show the tool options window"));
	mShowStartupDialogAct->setStatusTip(tr("Show the startup screen with links to video tutorials"));
	mShowAnimatedHelpAct->setStatusTip(tr("Show/Hide the animated help window"));

	//Renderer Menu Actions
	mWireframeRendererAct->setText(tr("&Wireframe Renderer"));
	mWireframeRendererAct->setStatusTip(tr("Switch the current renderer to Wireframe"));
	mNormalRendererAct->setText(tr("&Normal Renderer"));
	mNormalRendererAct->setStatusTip(tr("Switch the current renderer to Normal"));
	mLightedRendererAct->setText(tr("&Lighted Renderer"));
	mLightedRendererAct->setStatusTip(tr("Switch the current renderer to Lighted"));
	mTexturedRendererAct->setText(tr("&Textured Renderer"));
	mTexturedRendererAct->setStatusTip(tr("Switch the current renderer to Textured"));
	mTexturedLightedAct->setText(tr("Te&xtured Lighted Renderer"));
	mTexturedLightedAct->setStatusTip(tr("Switch the current renderer to Textured Lit"));
	mPatchRendererAct->setText(tr("&Patch Renderer"));
	mPatchRendererAct->setStatusTip(tr("Switch the current renderer to Patch"));
	mColorableRendererAct->setText(tr("&Colorable Renderer"));
	mColorableRendererAct->setStatusTip(tr("Switch the current renderer to Colorable"));

	//PRIMITIVES MENU ACTIONS
	mPrimCubeAct->setText(tr("&Cube"));
	mPrimCubeAct->setStatusTip(tr("Load a Cube"));
	mPrimOctahedronAct->setText(tr("&Octahedron"));
	mPrimOctahedronAct->setStatusTip(tr("Load an octahedron"));
	mPrimTetrahedronAct->setText(tr("&Tetrahedron"));
	mPrimTetrahedronAct->setStatusTip(tr("Load a tetrahedron"));
	mPrimDodecahedronAct->setText(tr("&Dodecahedron"));
	mPrimDodecahedronAct->setStatusTip(tr("Load a dodecahedron"));
	mPrimIcosahedronAct->setText(tr("&Icosahedron"));
	mPrimIcosahedronAct->setStatusTip(tr("Load an icosahedron"));
	mPrimSoccerBallAct->setText(tr("&Soccer ball"));
	mPrimSoccerBallAct->setStatusTip(tr("Load a soccer ball"));
	mPrimGeodesicAct->setText(tr("&Geodesic Dome"));
	mPrimGeodesicAct->setStatusTip(tr("Load a geodesic dome"));
	//Object Menu Actions
	mSubdivideAllEdgesAct->setText(tr("Subdivide All &Edges"));
	mPlanarizeAllFacesAct->setText(tr("Planarize All &Faces"));
	makeObjectSphericalAct->setText(tr("Make &Object Spherical"));
	mCleanup2gonsAct->setText(tr("Cleanup 2-gons"));
	mCleanupWingedVerticesAct->setText(tr("Remove valence-2 vertices"));
	mSplitValence2VerticesAct->setText(tr("Split valence-2 vertices"));
	mMakeObjectSmoothAct->setText(tr("Make Object &Smooth"));
	mPerformCuttingAct->setText(tr("Perform Cutting"));
	mPerformCuttingAct->setStatusTip(tr("Cut selected faces, edges, or vertices based on the current selection mask"));
	mCreateCrustScalingAct->setText(tr("&Create Crust (Scaling)"));
	mCreateCrustScalingAct->setStatusTip(tr("Create a crust using the currently selected faces with scaling mode"));
	mCreateCrustThicknessAct->setText(tr("Create Crust (&Thickness)"));
	mCreateCrustThicknessAct->setStatusTip(tr("Create a crust using the currently selected faces with thickness mode"));
	mMakeWireframeAct->setText(tr("Create Wireframe"));
	mMakeWireframeAct->setStatusTip(tr("Create a wireframe model using the current options"));
	mMakeColumnsAct->setText(tr("Create Columns"));
	mMakeColumnsAct->setStatusTip(tr("Create a column model using the current options"));
	mMakeSierpinskiAct->setText(tr("Create Sierpinski"));
	mMakeSierpinskiAct->setStatusTip(tr("Create a sierpinski tetrahedra"));
	mComputeLightingAct->setText(tr("Compute &Lighting"));
	mComputeNormalsAndLightingAct->setText(tr("Compute &Normals and Lighting"));
	mAssignTextureCoordinatesAct->setText(tr("Assign &Texture Coordinates"));
	//SELECTION MENU ACTIONS
	mSelectVertexAct->setText(tr("Select &Vertex"));
	mSelectVertexAct->setStatusTip(tr("Select a Vertex"));
	mSelectMultipleVerticesAct->setText(tr("Select Multiple Vertices"));
	mSelectMultipleVerticesAct->setStatusTip(tr("Select multiple vertices"));
	mEditVertexAct->setText(tr("Edit Verte&x"));
	mEditVertexAct->setStatusTip(tr("Select and Move Vertices One at a time."));
	mSelectFaceAct->setText(tr("Select &Face"));
	mSelectFaceAct->setStatusTip(tr("Select One Face. Just for practice. :)"));
	mSelectFaceLoopAct->setText(tr("Select Face Loo&p"));
	mSelectFaceLoopAct->setStatusTip(tr("Select a Face Loop."));
	mSelectEdgeRingAct->setText(tr("Select Edge &Ring"));
	mSelectEdgeRingAct->setStatusTip(tr("Select an Edge Ring."));
	mSelectMultipleFacesAct->setText(tr("Select &Multiple Faces"));
	mSelectSimilarFacesAct->setText(tr("Select &Similar Faces"));
	mSelectFacesByAreaAct->setText(tr("Select Faces By Surf. Area"));
	mSelectFacesByColorAct->setText(tr("Select Faces By Color"));
	mSelectionWindowAct->setText(tr("Selection Window"));
	mSelectCheckerboardFacesAct->setText(tr("C&heckerboard Select Faces"));
	mSelectAllAct->setText(tr("Select &All"));
	mGrowSelectionAct->setText(tr("Grow Selection"));
	mShrinkSelectionAct->setText(tr("Shrink Selection"));
	mSelectInverseAct->setText(tr("Select &Inverse"));
	mSelectEdgeAct->setText(tr("Select &Edge"));
	mSelectEdgeAct->setStatusTip(tr("Select one Edge"));
	mCollapseSelectedEdgesAct->setText(tr("Collapse Selected Edges"));
	mCollapseSelectedEdgesAct->setStatusTip(tr("Collapse Selected Edges"));
	mSelectMultipleEdgesAct->setText(tr("Select Multiple Edges"));
	mSelectMultipleEdgesAct->setStatusTip(tr("Select multiple edges"));
	mSelectEdgeLoopAct->setText(tr("Select Edge &Loop"));
	mSelectEdgeLoopAct->setStatusTip(tr("Select an Edge Loop"));
	mSelectCornerAct->setText(tr("Select &Corner"));
	mSelectCornerAct->setStatusTip(tr("Select a Corner"));
	mExitSelectionModeAct->setText(tr("E&xit Selection Mode"));
	mClearSelectedModeAct->setText(tr("&Clear Selected"));
	mSelectVerticesMaskAct->setText(tr("Select &Vertices"));
	mSelectVerticesMaskAct->setStatusTip(tr("Select by Component type: Vertices"));
	mSelectEdgesMaskAct->setText(tr("Select &Edges"));
	mSelectEdgesMaskAct->setStatusTip(tr("Select by Component type: Edges"));
	mSelectFacesMaskAct->setText(tr("Select &Faces"));
	mSelectFacesMaskAct->setStatusTip(tr("Select by Component type: Faces"));
	mSelectCornersMaskAct->setText(tr("Select &Corner"));
	mSelectCornersMaskAct->setStatusTip(tr("Select by Component type: Corners"));

	//SETTINGS ACTIONS
	mPreferencesAct->setText(tr("&Preferences"));
	mPreferencesAct->setStatusTip(tr("Open the Preferences Dialog"));

	//LANGUAGE MENU BAR ACTIONS
	mEnglishAct->setText(tr("English"));
	mSpanishAct->setText(tr("Spanish"));
	mGermanAct->setText(tr("German"));
	mFrenchAct->setText(tr("French"));
	mItalianAct->setText(tr("Italian"));
	mTurkishAct->setText(tr("Turkish"));
	mCatalanAct->setText(tr("Catalan"));
	mHindiAct->setText(tr("Hindi"));

#ifdef WITH_VERSE
	//verse menu actions
	mVerseConnectLocalhostAct->setText(tr("Connect to localhost..."));
	mVerseConnectLocalhostAct->setStatusTip(tr("Connect to localhost"));
	mVerseConnectAct->setText(tr("Connect to host..."));
	mVerseConnectAct->setStatusTip(tr("Connect to host..."));
	mVerseDisconnectAct->setText(tr("Disconnect session"));
	mVerseDisconnectAct->setStatusTip(tr("Disconnect Verse Session"));
	mVerseDisconnectAllAct->setText(tr("Disconnect All Sessions"));
	mVerseDisconnectAllAct->setStatusTip(tr("Disconnect All Sessions"));
	mVerseStartServerAct->setText(tr("Start Verse Server"));
	mVerseStartServerAct->setStatusTip(tr("Disconnect All Nodes"));
	mVerseKillServerAct->setText(tr("Kill Verse Server"));
	mVerseKillServerAct->setStatusTip(tr("Kill the Local Verse server process"));
#endif

	mSubdivideSelectedFacesAct->setText(tr("Subdivide Selected Faces"));
	mSubdivideSelectedFacesAct->setStatusTip(tr("Subdivide all Selected Faces"));
	mPaintSelectedFacesAct->setText(tr("Paint Selected Faces"));
	mPaintSelectedFacesAct->setStatusTip(tr("Paint all Selected Faces"));
	mClearMaterialsAct->setText(tr("Clear Materials"));
	mClearMaterialsAct->setStatusTip(tr("Clear Materials"));
	mSubdivideSelectedEdgesAct->setText(tr("Subdivide Selected Edges"));
	mSubdivideSelectedEdgesAct->setStatusTip(tr("Subdivide all Selected Edges"));
	mPerformRemeshingAct->setText(tr("Perform Remeshing"));
	mPerformRemeshingAct->setStatusTip(tr("Perform the current remeshing scheme"));
	mPerformExtrusionAct->setText(tr("Perform Extrusion"));
	mPerformExtrusionAct->setStatusTip(tr("Perform the current extrusion operator on the selected faces"));
	mExtrudeMultipleAct->setText(tr("Extrude Multiple Faces"));
	mExtrudeMultipleAct->setStatusTip(tr(
		"Check this if you want to be able to select multiple faces "\
		"and then hit CTRL+X to extrude the selected faces"));

	//help menu actions
	mHelpAct->setText(tr("&Online User Manual"));
	mHelpAct->setStatusTip(tr("View the User Manual on the TopMod Wiki"));
	mTopModWebAct->setText(tr("&TopMod on the Web"));
	mTopModWebAct->setStatusTip(tr("Go to the TopMod web page"));
	mVideoTutorialsAct->setText(tr("&TopMod Video Tutorials on blip.tv"));
	mVideoTutorialsAct->setStatusTip(tr("Go to http://topmod.blip.tv"));
	mTopModResearchAct->setText(tr("&TopMod Research Papers"));
	mTopModResearchAct->setStatusTip(tr("Go to the TopMod Research web page"));
	mCheckForUpdatesAct->setText(tr("&Check for Updates"));
	mCheckForUpdatesAct->setStatusTip(tr("Check for updates to TopMod Online"));
	mAboutAct->setText(tr("&About TopMod"));
	mAboutAct->setStatusTip(tr("About TopMod"));
	mAboutQtAct->setText(tr("About &Qt"));
	mAboutQtAct->setStatusTip(tr("About Qt"));
	mHideToolBarsAct->setText(tr("Hide All ToolBars"));
	mHideToolBarsAct->setStatusTip(tr("Hide All ToolBars"));
	mShowToolBarsAct->setText(tr("Show All ToolBars"));
	mShowToolBarsAct->setStatusTip(tr("Show All ToolBars"));

	//from createAnimatedHelp


	//from creatToolbars()
	mEditToolBar->setWindowTitle(tr("Edit"));
	mSelectionMaskToolBar->setWindowTitle(tr("Selection Masks"));
	mPrimitivesToolBar->setWindowTitle(tr("Primitives"));
	mToolsToolBar->setWindowTitle(tr("Tools"));
	mExtrusionToolBar->setWindowTitle(tr("Extrusion Tools"));
	mConicalToolBar->setWindowTitle(tr("Conical Tools"));
	mHighgenusToolBar->setWindowTitle(tr("High Genus Tools"));
	mTexturingToolBar->setWindowTitle(tr("Texturing Tools"));
	mRemeshingToolBar->setWindowTitle(tr("Remeshing Tools"));
	mExperimentalToolBar->setWindowTitle(tr("Experimental Tools"));

	//createStartupDialog
	mTutorialNavigationAct->setText(tr("Navigation Basics"));
	mTutorialInterfaceAct->setText(tr("Interface Basics"));
	mTutorialBasicAct->setText(tr("Basic Operations"));
	mTutorialExtrusionAct->setText(tr("Extrusion Operations"));
	mTutorialRemeshingAct->setText(tr("Remeshing Operations"));
	mTutorialHighgenusAct->setText(tr("High genus Operations"));
	mTutorialTexturingAct->setText(tr("Texturing Operations"));
	mShowStartupDialogAtStartupCheckBox->setText(tr("Show this dialog at startup"));
	mQuickTimeLabel->setText(tr("Quicktime 7.2 or greater recommended."));
	mDownloadQuickTimeLabel->setText(tr(
		"<h5>Quicktime 7.2 or greater recommended.<br />"\
		"<a href=\"http://apple.com/quicktime/\">Download now.</a></h5>"));

}//end retranslateUi
