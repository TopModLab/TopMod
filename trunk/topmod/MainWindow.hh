/******************************************
* include.hh
	* 
	******************************************/

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "stylesheeteditor.hh"
#include "qshortcutmanager.hh"
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
// #include <QPainter>
#include <QPen>
#include "GLWidget.hh"

//the six modes are now separated into separate classes
#include "BasicsMode.hh"
#include "ExtrusionMode.hh"
#include "RemeshingMode.hh"
#include "HighgenusMode.hh"
#include "ConicalMode.hh"
#include "TexturingMode.hh"

#include "DLFLWindow.hh"
#include "DLFLObject.hh"
#include "include/WireframeRenderer.hh"
#include "include/ShadedRenderer.hh"
#include "include/NormalRenderer.hh"
#include "include/LitRenderer.hh"
#include "include/TexturedRenderer.hh"
#include "include/TexturedLitRenderer.hh"
#include "include/PatchRenderer.hh"
#include "TopModPreferences.hh"

class TopModPreferences;

class BasicsMode;
class ExtrusionMode;
class RemeshingMode;
class ConicalMode;
class HighgenusMode;
class TexturingMode;

// class QPainter;
class QPen;

class QAction;
class QMenu;
class GLWidget;
class QBoxLayout;
class QComboBox;
class QMenuBar;

class DLFLWindow;

class MainWindow : public QMainWindow {
	Q_OBJECT

	public:
		MainWindow(char *filename = NULL);

						// Create the renderers
		void createRenderers(void){
			wired = new WireframeRenderer();
		//wired->setRenderFlags(DLFLRender::ShowWireframe); // redundant?

			shaded = new ShadedRenderer();
			shaded->setRenderFlags(DLFLRenderer::ShowWireframe);

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
		};

		void destroyRenderers(void){
			delete shaded;
			delete normal;
			delete lit;
			delete textured;
			delete texturedlit;
			delete patch;
		};

		void setMode(int m);
		void setToolOptions(QWidget *optionsWidget);
		void setRemeshingScheme(int m);
		void loadFile(QString fileName);
	// void setCurrentFile(QString fileName);	

		DLFLWindow *getDLFLWindow(){ return mainWindow; };

	protected:
		void closeEvent( QCloseEvent *event );

	//Box Layouts for the entire window
		QBoxLayout *layout, *subTopLayout, *subLowLayout, *leftLayout, *rightLayout;
		QWidget * cWidget;  QHBoxLayout *modesLayout;

	// QPainter painter;
		int startX, startY, endX, endY;

		DLFLWindow *mainWindow;            // Window displaying the object

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

	// Default sizes
		static int MIN_W;                 // Minimum width for app window
		static int MIN_H;                // Minimum height for app window

		// Renderers
		static WireframeRendererPtr wired;            // WireframeRenderer
		static ShadedRendererPtr shaded;              // ShadedRenderer
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
		void createActions();
		void createMenus();
		void createToolBars();
		void createStatusBar();
		void readSettings();
		void writeSettings();
		bool maybeSave();
	// void loadFile(const QString &fileName);
		bool saveFile(QString fileName);
		void openFile(QString fileName);

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
		QMenu *settingsMenu;
		QMenu *languageMenu;

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

	//Edit Menu Actions
		QAction *mUndoAct;
		QAction *mRedoAct;

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
		QAction *showSilhouetteAct;
		QAction *showWireframeAct;
		QAction *objectOrientationAct;
		QAction *showNormalsAct;
		QAction *showGridAct;
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
	  QAction *selectFaceAct;
	  QAction *selectEdgeAct;
	  QAction *selectCornerAct;
	  QAction *clearSelectedModeAct;
	  QAction *exitSelectionModeAct;
	// 
	//   //Mode Switching Actions
	//   QAction *modeBasicsAct;
	//   QAction *modeExtrusionAct;
	// QAction *modeConicalAct;
	//   QAction *modeRemeshingAct;
	//   QAction *modeHighgenusAct;
	//   QAction *modeTexturingAct;

	//settings menu actions
		QAction *manageShortcutsAct;

	//Language Menu actions;
		QAction *englishAct;
		QAction *spanishAct;
		QAction *germanAct;
		QAction *frenchAct;
		QAction *turkishAct;
		QAction *catalanAct;

	#ifdef WITH_VERSE
		//verse menu actions
		QAction *mVerseConnectLocalhostAct;
		QAction *mVerseConnectAct;
		QAction *mVerseDisconnectAct;
		QAction *mVerseDisconnectAllAct;
	#endif

	// QString curFile;

		QWidget *mToolOptionsTitleBarWidget;
		QBoxLayout *mToolOptionsTitleBarLayout;

	//QToolbars
		QToolBar *mEditToolBar;

		QToolBar *mPrimitivesToolBar;
		QToolBar *mToolsToolBar;
		QToolBar *mExtrusionToolBar;
		QToolBar *mConicalToolBar;
		QToolBar *mHighgenusToolBar;
		QToolBar *mTexturingToolBar;

		QToolBar *mRemeshingToolBar;

	//QActionGroups
		QActionGroup *mToolsActionGroup;
		QActionGroup *rendererActionGroup;
		QActionGroup *modesActionGroup;
		QActionGroup *mRemeshingActionGroup;

	//the status bar
		QStatusBar *mStatusBar;

		QAction *showScriptEditorAct;
	
	#ifdef WITH_VERSE
		QAction *mShowVerseDialogAct;
		// QInputDialog *mVerseAddressDialog;
	#endif
		QAction *mFullscreenAct;

		QAction *mPerformRemeshingAct;

		StyleSheetEditor *mStyleSheetEditor;
		QAction *mEditStyleSheetAct;

		TopModPreferences *mPreferencesDialog;
		QAction *mPreferencesAct;

		QSettings *mSettings;

	public slots:

		void newFile();
		void open();
		bool save();
		bool saveAs();
		void about();
		void documentWasModified();

		void showHideScriptEditor();
		void showHideVerseDialog();

		// #ifdef WITH_VERSE
		// void verseConnectHost();
		// #endif
		
		void open_file();
		void save_file();
		void save_file_with_normals();
		void save_file_with_tex_coords();
		void save_file_extended();
	/* stuart - bezier export */
		void save_file_patches();
		void load_texture();
		void print_info();
		void print_flist();
		void print_vlist();
		void print_elist();
		void print_cvlist();
		void quit();



		void undo();
		void redo();

		void toggle_vertices();
		void toggle_silhouette();
		void toggle_wireframe();
		void toggle_grid();
		void toggle_axes();

		void use_wireframe_renderer();
		void use_normal_renderer();
		void use_lighted_renderer();
		void use_shaded_renderer();
		void use_textured_renderer();
		void use_textured_lit_renderer();
		void use_patch_renderer();

		void subdivide_all_edges();
		void planarize_all_faces();
		void make_object_spherical();
		void make_object_smooth();
		void create_crust();
		void compute_lighting();
		void compute_normals_and_lighting();
		void assign_texture_coordinates();

		void select_vertex();
		void select_face();
		void select_edge();
		void select_corner();
		void exit_selection_mode();
		void clear_selected();

		void exit_modes();

		void subdivide_face();
	// void assign_tile_texture_coords();

		void crust_modeling1();
		void crust_modeling2();
		void wireframe_modeling();
		void column_modeling(); // Esan

		void perform_remeshing();

		void create_multi_face_handle();
		void mfh_use_closest_edge_algo();
		void mfh_use_convex_hull_algo();
		void create_sierpinsky_tetrahedra();
		void create_menger_sponge();

	//Basics Widget
		void toggleDeleteEdgeCleanupFlag(int state);
		void changeNumSubDivs(int value);
		void translatex(double value);
		void translatey(double value);
		void translatez(double value);
		void scalex(double value);
		void scaley(double value);
		void scalez(double value);
		void freeze_transforms();

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

	//conical operations from ozgur
		void perform_cutting();// ozgur

		void create_convexhull();    // ozgur
		void create_dual_convexhull(); //ozgur

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
		void paintEvent(QPaintEvent * event);

		void dragEnterEvent(QDragEnterEvent *event);
		void dropEvent(QDropEvent *event);
		
	};

#endif
