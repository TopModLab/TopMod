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
*/

/******************************************
 * include.hh
 *
 ******************************************/

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "stylesheeteditor.h"
#include "qshortcutmanager.h"
#include "qshortcutdialog.h"
#include "DLFLScriptEditor.hh"
#include "VerseTopMod.hh"

#include <iostream>
#include <array>
#include <QMainWindow>
#include <QWidget>
#include <QComboBox>
#include <QKeyEvent>
#include <QMenuBar>
#include <QMenu>
#include <QStyleOptionMenuItem>
//#include <QAssistantClient>
#include <QPen>
#include "GLWidget.h"

//the six modes are now separated into separate classes
#include "BasicsMode.h"
#include "ExtrusionsMode.h"
#include "RemeshingMode.h"
#include "HighgenusMode.h"
#include "ConicalMode.h"
#include "TexturingMode.h"
#include "ExperimentalModes.h"

#ifdef QCOMPLETER
//new command auto completion interface - like quicksilver inside topmod
#include "CommandCompleter.hh"
#endif

#include "DLFLLighting.hh"
#include <DLFLObject.hh>
#include <DLFLConvexHull.hh>

#include "include/WireframeRenderer.hh"
#include "include/NormalRenderer.hh"
#include "include/LitRenderer.hh"
#include "include/TexturedRenderer.hh"
#include "include/TexturedLitRenderer.hh"
#include "include/PatchRenderer.hh"
#include "include/ColorableRenderer.hh"
#include "TopModPreferences.h"

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
class ExtrusionsMode;
class RemeshingMode;
class ConicalMode;
class HighgenusMode;
class TexturingMode;
class ExperimentalMode;
#ifdef WITH_VERSE
class VerseTopMod;
#endif
class QAction;
class QMenu;
class GLWidget;
class QBoxLayout;
class QComboBox;
class QMenuBar;

using namespace DLFL;

/*!
	\file MainWindow.hh
	\brief Definition of the MainWindow class

	\see MainWindow
*/

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	/**
	* Enumerations for the various operating modes.
	*/
	enum Mode
	{
		NormalMode = 0,					/**< normal mode. does not allow any selection in the viewport. */
		SelectVertex = 1,				/**< select one vertex. */
		SelectEdge = 2,					/**< select one edge. */
		SelectFace = 3,					/**< select one face. */
		SelectCorner = 4,				/**< select one corner, which requires selection of a face and a vertex. */
		MultiSelectVertex = 5,			/**< select multiple vertices. */
		MultiSelectEdge = 6,			/**< select multiple edges. */
		MultiSelectFace = 7,			/**< . */
		MultiSelectCorner = 8,			/**< . */
		SelectCheckerboard = 9,			/**< select alternating faces forming a checkerboard pattern. works well after Doo Sabin Remeshing. */
		SelectionWindow = 10,			/**< crossing window selection  - dave. */
		InsertEdge = 11,				/**< insert edge. */
		DeleteEdge = 12,				/**< delete an edge. */
		SubdivideEdge = 13,				/**< subdivide an edge into n segments. */
		CollapseEdge = 14,				/**< collapse an edge. */
		SpliceCorners = 15,				/**< select two corners and collapse the face in between. */
		ConnectEdges = 21,				/**< connect two half-edges. */
		ExtrudeFace = 31,				/**< cubical extrusion. */
		ExtrudeFaceDS = 32,				/**< doo sabin extrusion. */
		ExtrudeDualFace = 33,			/**< dual of cubical extrusion. */
		ExtrudeFaceDodeca = 34,			/**< dodecahedral extrusion. */
		ExtrudeFaceIcosa = 35,			/**< icosahedral extrusion. */
		ExtrudeMultipleFaces = 36,		/**< cubical extrusion for multiple faces at once. */
		StellateFace = 41,				/**< stellate extrusion. */
		DoubleStellateFace = 42,		/**< double stellate extrusion. */
		ExtrudeFaceDome = 43,			/**< dome extrusion. dave */
		ConnectFaceVertices = 51,		/**< add . */
		ConnectFaces = 52,				/**< add hole/handle operation. */
		BezierConnectFaces = 53,		/**< add handle operation. */
		HermiteConnectFaces = 54,		/**< add handle operation. */
		ReorderFace = 61,				/**< reorders the corners of a face. */
		SubdivideFace = 62,				/**< subdivides a face into n faces. */
		CrustModeling = 71,				/**< create a crust of the current model and poke holes in it. */
		CutEdge = 200,					/**< // ozgur. */
		CutVertex = 201,				/**< // ozgur. */
		CutEdgeandVertex = 202,			/**< // ozgur. */
		CutFace = 203,					/**< // ozgur. */
		TruncateEdge = 204,				/**< // ozgur. */
		MarkEdge = 206,					/**< // ozgur. */
		MarkVertex = 207,				/**< // ozgur. */
		ConvexHullMode = 301,			/**< create a convex hull or dual convex hull // ozgur. */
		EditVertex = 81,				/**< select and move individual vertices // brianb. */
		SelectEdgeLoop = 82,			/**< select an edge loop. shift select for multiple. */
		SelectEdgeRing = 85,			/**< select an edge ring. shift select for multiple. */
		SelectFaceLoop = 83,			/**< allows user to select one edge in the viewport and selects the corresponding face loop. shift select for multiple. */
		SelectSimilar = 84,				/**< select all sub objects with the same number of verts edges or faces whatever... . */
		SelectFacesByArea = 856,		/**< select all faces with the a similar surface area. */
		SelectFacesByColor = 857,		/**< select all faces with the same color material. */
		PaintFace = 858,				/**< paint a face as you click. */
		EyeDropper = 859				/**< grab the color of the selected face. */
	};

	/**
	* Enumerations for Selection Masking (e.g. vertices, faces, edges, face-vertices, and eventually objects... ?)
	*/
	enum SelectionMask
	{
		MaskObject = 0,
		MaskVertices,
		MaskEdges,
		MaskFaces,
		MaskCorners,
		MaskNone
	};

	/**
	* Enumerations for Extrusion operatoers
	*/
	enum ExtrusionMode
	{
		CubicalExtrude,
		DooSabinExtrude,
		DodecahedralExtrude,
		IcosahedralExtrude,
		OctahedralExtrude, //!< also called dual
		StellateExtrude,
		DoubleStellateExtrude,
		DomeExtrude
	};

	// Enumerations for various multi-face-handle algorithms
	enum MFHAlgo
	{
		ConvexHull = 0,
		ClosestEdge = 1
	};

	enum RemeshingScheme
	{
		Dual = 0,									/**< . */
		Root3 = 10,									/**< . */
		Triangulate = 15,							/**< . */
		DualVertexTrunc = 11,						/**< . */
		GlobalStellate = 12,						/**< . */
		Star = 13,									/**< . */
		Generic1264 = 14,							/**< . */
		Honeycomb = 20,								/**< . */
		VertexTrunc = 21,							/**< . */
		DualGeneric1264 = 22,						/**< . */
		LinearVertexInsertion = 30,					/**< . */
		CatmullClark = 31,							/**< . */
		ModifiedStellate = 32,						/**< . */
		DooSabin = 40,								/**< . */
		CornerCutting = 41,							/**< . */
		ModifiedCornerCutting = 43,					/**< . */
		Simplest = 42,								/**< . */
		Pentagonal = 50,							/**< . */
		CubicPentagonal = 51,						/**< . */
		DualPentagonal = 52,						/**< . */
		LoopStyle = 60,								/**< . */
		Loop = 61,									/**< . */
		Root4 = 62,									/**< . */
		DualLoop = 63,								/**< . */
		GlobalCubicExtrude = 70,					/**< . */
		CheckerBoard = 71,							/**< . */
		DualGlobalCubicExtrude = 80,				/**< . */
		DualCheckerBoard = 81,						/**< . */
		PentagonPreserving = 90,					/**< . */
		DualPentagonPreserving = 91,				/**< . */
		HexagonPreserving = 100,					/**< . */
		DualHexagonPreserving = 101,				/**< . */
		Fractal = 110,								/**< . */
		ModifiedDoubleStellate = 111,				/**< . */
		Dome = 112,									/**< . */
		DooSabinBC = 113,							/**< . */
		DooSabinBCNew = 114							/**< . */
	};

	enum SpinBoxMode
	{
		eFirstSpinBox = 0,	/**< the first spinbox in the current option panel will be controlled by the Y key. */
		eSecondSpinBox,		/**< the second spinbox in the current option panel will be controlled by the U key. */
		eThirdSpinBox,		/**< the third spinbox in the current option panel will be controlled by the I key. */
		eFourthSpinBox,		/**< the fourth spinbox in the current option panel will be controlled by the O key. */
		eFifthSpinBox,		/**< the fifth spinbox in the current option panel will be controlled by the P key. */
		eSixthSpinBox,		/**< the sixth spinbox in the current option panel will be controlled by the [ key. */
		eInvalidSpinBox		/**< no spinbox is being manipulated */
	};

	//-- Parameters used in various operations on the DLFL object --//

	// face area tolerance - dave
	static float sFaceAreaTolerance;
	// face color tolerance - dave 11/07
	static float sFaceColorTolerance;

	// Edge deletion
	static bool sDeleteEdgeCleanup; //!< Flag for point-sphere cleanup after edge deletion

	//!< Handles
	static int sNumSegments;			//!< No. of segments in a connection/handle
	static int sMaxSegments;			//!< Max. no. of segments to connect
	static bool sSymmetricWeights;		//!< Use symmetric weight factors
	static double sFaceNormalWeight1;	//!< Weight factor for normal of first face
	static double sFaceNormalWeight2;	//!< Weight factor for normal of second face
	static int sNumExtraTwists;			//!< No. of extra twists in handles

	//!< Extrusions
	static double sExtrudeDist;				//!< Distance for an extrude
	static double sExtrudeRot;				//!< Rotation for an extrude
	static double sExtrudeScale;			//!< Scaling for an extrude
	static int sNumExtrusions;				//!< No. of segments for an extrude
	static double sDooSabinExtrusionTwist;	//!< Twist factor for doo-sabin extrude
	static bool sDualMeshEdgesCheck;		//!< Flag to check for removal of dual mesh edges
	static bool sHexagonalizeDodecaExtrude;	//!< Flag to hexagonalize when doing dodeca extrude
	static double sExtrudeLength1;
	static double sExtrudeLength2;
	static double sExtrudeLength3;
	static double sExtrudeAngle;
	static double sExtrudeLength1_iCosA;
	static double sExtrudeLength2_iCosA;
	static double sExtrudeLength3_iCosA;
	static double sExtrudeAngle_iCosA;

	//!< Edge subdivision
	static int sNumEdgeSubdiv;			//!< No. of subdivisions for an edge

	//!< Split valence 2 vertices
	static double sVertexSplitOffset;	//!< Half of distance between new vertices


	//!< Crust modeling
	static double sCrustThickness;		//!< Thickness of crust
	static double sCrustScaleFactor;	//!< Scale factor for making crust
	static bool sCrustCleanup;			//!< Cleanup flag for crust modeling

	//!< Wire-frame modeling
	static double sWireframeThickness;	//!< Thickness of crust for wireframe
	static double sWireframe2Thickness;	//!< depth of crust for wireframe2
	static double sWireframe2Width;		//!< width of crust for wireframe2
	static bool sWireframeSplit;		//!< split valence-2 vertices or not?
	static double sCornerCuttingAlpha;

	//!< Column modeling
	static double sColumnThickness;	//!< Thickness of columns for column modeling
	static int sColumnSegments;		//!< Number of segments in each column for column modeling

	//!< Subdivision
	static double sTwistFactor;				//!< Twist factor for root-4 subdiv
	static double sWeightFactor;			//!< Weight factor for root-4 subdiv
	static bool sDooSabinCheck;				//!< Flag to check for repeating edges
	static double sVertexCuttingOffset;		//!< Offset value for vertex cutting
	static double sPentagonalOffset;		//!< Offset value for pentagonal subdivision (conversion)
	static double sPentagonalScale;			//!< Scale factor for pentagonal subdivision (preserving)
	static bool sAccurateDual;				//!< Flag to indicate that accurate dual method should be used
	static double sCheckerboardThickness;	//!< Fractional thickness for checkerboard remeshing
	static double sModifiedCornerCuttingThickness; //!< for modified corner cutting scheme same as Thickness of crust for wireframe

	//added by Ryan
	static double sPinchingFactor;		//!< scaling factor for pinched handles
	static double sPinchCenter;			//!< 0 to 1, the parametric cetner of the pinching factor
	static double sBubbleFactor;		//!< 0 to 0.5, the offset from the pinch center to the bezier control points

	static double sHoleHandlePinchingFactor;		//!< scaling factor for pinched handles
	static double sHoleHandlePinchCenter;
	static double sHoleHandlePinchWidth;

	//!< Added by Doug
	static double sStarOffset;		//!< Offset value for star subdivision
	static double sFractalOffset;	//!< Offset value for fractal subdivision

	//!< Added by Eric
	static double sSubstellateHeight;	//amount of first stellation extrusion
	static double sSubstellateCurve;	//amount of second stellation extrusion

	//!< added by dave
	static double sDomeExtrudeLengthFactor;		//!< Length for dome extrusion  //dave
	static double sDomeExtrudeScaleFactor;		//!< Scale factor for dome extrusion //dave
	static double sDomeExtrudeRotationFactor;	//!< Scale factor for dome extrusion //dave

	//!< Added by Bei & Cansin
	static double sDomeLengthFactor;			//!< Length for dome extrusion
	static double sDomeScaleFactor;				//!< Scale factor for dome extrusion
	static double sDual1264ScaleFactor;			//!< Scale factor for Dual 12-6-4 remeshing
	static double sDooSabinBCnewScaleFactor;	//!< Scale factor for new Doo-Sabin remeshing scheme
	static double sDooSabinBCnewLengthFactor;	//!< Length factor for new Doo-Sabin remeshing scheme
	static double sLoopLengthFactor;			//!< Length factor for Loop-style remeshing scheme

	//!< Face subdivision
	static bool sUseQuads; //!< Flag indicating if face subdivision should use quads or triangles

	//!< Tile texturing
	static int sTileTexSize; //!< Tiling size for tile texturing

	//!< Multi-face handles
	static MFHAlgo sMfhAlgorithm;		//!< Algorithm to use for multi-face handle
	static double sMfhScaleFactor;		//!< Scale factor for convex hull creation
	static double sMfhExtrudeDist;		//!< Extrude distance for convex hull creation
	static bool sMfhUseMaxOffsets;		//!< Flag indicating whether to compute and use the max. offset distances
	static double sMfhMinAdjFactor;		//!< Min. value of adjustment factor for iterative convex hull computation
	static bool sMfhMakeConnections;	//!< Should final connections be made or should we stop with convex hull?

	//!< Menger Sponge creation
	static double sSpongeThickness;			//!< Thickness for Menger Sponge
	static double sSpongeCollapseThreshold;	//!< Threshold angle for edge collapse
	static bool sSpongeFractionalThickness;	//!< Consider thickness to be fractional

	//!< Conical Sculpting Variables from Ozgur
	static double sCutOffsetEdgeFactor;
	static double sCutOffsetVertFactor;
	static double sPNormalBendS_Factor;
	static double sPNormalBendT_Factor;
	static bool sGlobalCut;
	static bool sSelectedCut;
	static int sNumOfPeels;
	static double sExtrudeBendT;
	static double sExtrudeBendB;

	static int sIncrementalSaveCount;

	//dave paint bucket tool - exp. 11.07
	static QColor sPaintBucketColor;

	QString mCurFilename;
	//document modified
	bool isModified();
	void setModified(bool isModified);

protected :
	//-- Selection count, used as index into selection lists --//
	static int sNumSelVerts;			//!< No. of selected vertices
	static int sNumSelEdges;			//!< No. of selected edges
	static int sNumSelFaces;			//!< No. of selected faces
	static int sNumSelFaceVerts;		//!< No. of selected face vertices

	//-- should we deselect or select the current faces? --//
	static bool sDeselectVerts;
	static bool sDeselectEdges;
	static bool sDeselectFaces;
	static bool sDeselectFaceVerts;

	static DLFLEdgePtr sFaceLoopStartEdge;		//!< face loop edge pointer
	static DLFLEdgePtr sEdgeRingStartEdge;		//!< edge ring edge pointer
	static bool sFaceLoopStart;					//!< face loop start

	GLWidget *mActiveViewport;					//!< Active viewport to handle events

	DLFLObject mObject;							//!< The DLFL object
	//TMPatchObject *patchObject;				//!< the patch object
	Mode mOperationMode;						//!< Current operating mode
	ExtrusionMode mExtrusionMode;				//!< Current operating mode
	SelectionMask mSelectionMask;				//!< Current selection mask
	RemeshingScheme mRemeshingScheme;			//!< Current selected remeshing scheme
	PointLight mPointLight;						//!< Light used to compute lighting

	StringStreamPtrList mUndoList;				//!< List for Undo
	StringStreamPtrList mRedoList;				//!< List for Redo
	StringStreamPtrList mUndoMtlList;			//!< Mtl List for Undo
	StringStreamPtrList mRedoMtlList;			//!< Mtl List for Redo
	int mUndoLimit;								//!< Limit for undo
	bool mUseUndo;								//!< Flag to indicate if undo will be used

	void initialize(int x, int y, int w, int h, DLFLRendererPtr rp);	//!< Initialize the viewports, etc.

	// brianb
	static int sDragStartX;
	static int sDragStartY;
	double mStartDragX;
	double mStartDragY;
	static bool sIsEditing;

public :
	/**
	* \brief Constructor
	*/
	MainWindow(char *filename = nullptr);

	/**
	* \brief Destructor
	*/
	~MainWindow() {
		clearUndoList();
		clearRedoList();
		delete mActiveViewport;
	}

	/**
	* \brief translation function. see tutorial at http://trolltech.com/developer/knowledgebase/faq.2007-05-23.5871663589/
	*/
	void changeEvent ( QEvent * event ) {
		if (event->type() == QEvent::LanguageChange) {
		    retranslateUi();
		}
		QMainWindow::changeEvent(event);
	}

	void retranslateUi();

	/**
	* \brief clears all currently selected vertices, edges, faces, whatever is selected
	*/
	void clearSelected()	{
		mActiveViewport->clearSelected();
		MainWindow::clearNumSelected();
	}

	/**
	* \brief current used for vertex editing, needs to be updated to allow generic and multiple subobject translation/rotation/scale editing
	*/
	static void startDrag(int x, int y) { // brianb
		sDragStartX = x;
		sDragStartY = y;
	}

	/**
	* \brief resets all subobject num_selected variables to 0
	*/
	static void clearNumSelected() {
		sNumSelVerts = 0;
		sNumSelEdges = 0;
		sNumSelFaces = 0;
		sNumSelFaceVerts = 0;
	}

	void getCheckerboardSelection(DLFLFacePtr fptr);	//!< \todo  needs to be moved to DLFL namespace
	void getEdgeLoopSelection(DLFLEdgePtr eptr);		//!< \todo  needs to be moved to DLFL namespace
	void getFaceLoopSelection(DLFLEdgePtr eptr, bool start, DLFLFacePtr face_loop_marker, bool select_face_loop);	//!< \todo  needs to be moved to DLFL namespace
	void getEdgeRingSelection(DLFLEdgePtr eptr, bool start, DLFLFacePtr face_loop_marker, bool select_face_loop);	//!< \todo  needs to be moved to DLFL namespace

	void createRenderers();			//!< allocate memory for the renderer pointers
	void destroyRenderers();		//!< delete memory allcated for the renderer pointers

	void setMode(Mode m);								//!< Switch to specified operating mode
	void setExtrusionMode(ExtrusionMode m);				//!< Switch to specified operating mode
	Mode getMode() { return mOperationMode; };			//!< returns the current operation mode enum... this needs to return a string eventually. don't know how to do that yet
	void setRemeshingScheme(RemeshingScheme scheme);	//!< switch the current remeshing scheme
	void setSelectionMask(SelectionMask m);				//!< set the current selection mask (verts, edges, faces, multiple?)
	void setToolOptions(QWidget *optionsWidget);		//!< set the current tool option widget to be displayed in mToolOptionsDockWidget
	void loadFile(QString fileName);					//!< load an OBJ or a DLFL file

	/**
	* \brief this will store pointers to the current mode's spinboxes so we can do keyboard interaction with them
	*/
	void setSpinBoxes(
		QDoubleSpinBox *one = nullptr, QDoubleSpinBox *two = nullptr,
		QDoubleSpinBox *three = nullptr, QDoubleSpinBox *four = nullptr,
		QDoubleSpinBox *five = nullptr, QDoubleSpinBox *six = nullptr);

#ifdef WITH_PYTHON
	DLFLScriptEditor *mScriptEditor;			//!< ScriptEditor Object by Stuart
	QDockWidget *mScriptEditorDockWidget;		//!< docked script editor window for Python Scripting interface by Stuart
#endif

#ifdef WITH_VERSE
	VerseTopMod *mVerseDialog;					//!< for a possible future implementation of the Verse protocol http://verse.blender.org
	QDockWidget *mVerseDialogDockWidget;		//!< for a possible future implementation of the Verse protocol http://verse.blender.org
#endif

	QDockWidget *mToolOptionsDockWidget;			//!< the floating window that displays the current tool's options (spinboxes, checkboxes, etc...)
	QStackedWidget *mToolOptionsStackedWidget;		//!< the widget that references each tool option widget and handles switching the display

	QDockWidget *mStartupDialogDockWidget;
	QWidget *mStartupDialogWidget;
	QGridLayout *mStartupDialogLayout;
	bool mShowStartupDialogAtStartup;
	QLabel *mQuickTimeLabel;
	QLabel *mDownloadQuickTimeLabel;

protected:
	void closeEvent(QCloseEvent *event);		//!< what will execute when the main window is closed (on application exit/quit)

	SpinBoxMode mSpinBoxMode;					//!< enum to store which spinbox mode we are in. e.g. 1, 2, 3, 4, 5, to allow mouse motion to update the values

	// Renderers
	static WireframeRendererPtr sWiredRenderer;			//!< WireframeRenderer
	static NormalRendererPtr sNormalRenderer;			//!< Normal Renderer - white color - like a "hidden line" view
	static LitRendererPtr sLitRenderer;					//!< Lit Renderer
	static TexturedRendererPtr sTexturedRenderer;		//!< Textured Renderer
	static TexturedLitRendererPtr sTexturedLitRenderer;	//!< Textured and Lit Renderer
	static PatchRendererPtr sPatchRenderer;				//!< Bezier Patch Display
	static ColorableRendererPtr sColorableRendererr;	//!< face colors... new ... by dave... 11/07

	BasicsMode *mBasicsMode;				//!< widget that holds all displayable option widgets for basic operating modes (InsertEdge, DeleteEdge, CollapseEdge, ConnectEdges, etc...)
	ExtrusionsMode *mExtrusionsMode;		//!< widget that holds all displayable option widgets for the extrusion operation modes
	ConicalMode *mConicalMode;				//!< for future implementation of Ozgur's Conical/Planar modeling modes.
	RemeshingMode *mRemeshingMode;			//!< widget that holds all displayable option widgets for all remeshing modes
	HighgenusMode *mHighgenusMode;			//!< high genus operation options (e.g. wireframe, sierpinsky, add handle)
	TexturingMode *mTexturingMode;			//!< \todo  texturing mode widgets (not working at the moment)
	ExperimentalMode *mExperimentalMode;	//!< experimental mode widgets like the new paint bucket tool dave 11.07

	QShortcutManager *mShortcutManager;					//!< Stuff for the shortcut manager test

	QStandardItemModel *mActionModel;		//!< stores all the actions in topmod. sent to CommandCompleter class in order to create an index of the possible actions based on the text and icon associated with each action
	QWidget *mActionListWidget;				//!< widget that stores all actions availabe in topmod for CommandCompleter autocompletion functionality

private:
	//document modified
	bool mIsModified;						//!< bool to stop program from exiting if file has been edited but not saved
	bool mIsPrimitive;						//!< bool to stop program from exiting if file has been edited but not saved
	bool mWasPrimitive;						//!< bool to stop program from exiting if file has been edited but not saved, used in DLFLUndo.cc
	bool mAutoSave, mIncrementalSave;
	int mIncrementalSaveMax;
	QTimer *mAutoSaveTimer;
	int mAutoSaveDelay;
	QString mSaveDirectory;
	bool mCommandCompleterIndexToggle;
	bool mSingleClickExtrude;


	void createActions();					//!< create all MainWindow actions for menu's and icons, also create operating mode actions in subclasses
	void createMenus();						//!< create top-level QMenuBar and add actions to each QMenu
	void createToolBars();					//!< create toolbars with large icons for one-click operations
	void createStatusBar();					//!< status bar will display extra info about what different operations or buttons do in TopMod interface
	bool maybeSave();						//!< save before exit?
	void initializeHelp();					//!< initialize the help files / create index / load html files

	void createStartupDialog();				//!< initialize the startup screen that will show links to beginner video tutorials / will include link to quicktime website and "disable checkbox"
	void initializeAnimatedHelp();			//!< initialize the in-context help animated screen captures. these will display in a small floatable window to the right

	//QAssistantClient *mAssistantClient;	//!< Qt help file viewer, will display html files created by DocBook xml transformation

	//top level and sublevel menus
	QMenuBar *mMenuBar;						//!< the main menubar pointer to which all qmenu's will be added
	QMenu *mFileMenu;						//!< the file menu which contains save, save as, quit etc...
	QMenu *mExportMenu;
	QMenu *mEditMenu;						//!< undo, redo, clear undo list, and also preferences (except on os/x)
	QMenu *mDisplayMenu;					//!< all viewport display functionality (HUD, IDs, renderer, verts, faces, normals, etc...)
	QMenu *mViewMenu;						//!< probably going to remove... but could contain choices for which view to use (persp or left / right /top/ bottom etc...)
	QMenu *mShowIDsMenu;					//!< submenu of mDisplayMenu
	QMenu *mRendererMenu;					//!< now a submenu of mDisplayMenu
	QMenu *mPrimitivesMenu;					//!< stores actions for one-click loading of primitive objects
	QMenu *mNewMenu;						//!< stores same actions as the primitives menu plus a "new file" command that loads a blank object
	QMenu *mObjectMenu;						//!< \todo  needs to be removed. not necessary
	QMenu *mSelectionMenu;					//!< all different selection options
	QMenu *mSelectionMaskMenu;				//!< \todo  figure out how to integrate a selection mask functionality into the current implementation of mode switching in TopMod
	QMenu *mLanguageMenu;					//!< \todo  add multi-lingual support using Qt's Linguist application and make dynamic language changing possible... could be very difficult to do...
	QMenu *mWindowMenu;						//!< menu choices for hiding and showing all floating windows and toolbars / also fullscreen mode
	QMenu *mHelpMenu;						//!< about TopMod, about Qt, Local documentation, Online documentation

	QMenu *mRightClickMenu;

#ifdef WITH_VERSE
	QMenu *mVerseMenu;
#endif

	QMenu *mRemeshingMenu;
	QMenu *mToolsMenu;

	//File Menu Actions
	QAction *mNewFileAct;
	QAction *mOpenAct;
	QAction *mSaveAct;
	QAction *mSavePatchesAct;
	QAction *mSaveLG3dAct;
	QAction *mSaveLG3dSelectedAct;
	QAction *mExportSTLAct;
	QAction *mScreenshotViewportAct;
	QAction *mScreenshotAppAct;
	QAction *mSaveAsAct;
	QAction *mLoadTextureAct;
	QAction *mPrintInfoAct;
	QAction *mPrintFaceListAct;
	QAction *mPrintVertexListAct;
	QAction *mPrintEdgeListAct;
	QAction *mPrintCVListAct;
	QAction *mExitAct;

	QAction *mFullscreenAct;
	QAction *mPerformRemeshingAct;
	QAction *mPerformExtrusionAct;
	QAction *mSubdivideSelectedFacesAct;
	QAction *mPaintSelectedFacesAct;
	QAction *mClearMaterialsAct;
	QAction *mExtrudeMultipleAct;			//!< temporary for now... not sure how to handle this in the future...
	QAction *mQuickCommandAct;

	//Edit Menu Actions
	QAction *mDeleteSelectedAct;			//!< delete the selected objects.
	QAction *mUndoAct;						//!< pop the previous model state off the undo stack
	QAction *mRedoAct;						//!< push the model back onto the undo stack
	QAction *mClearUndoListAct;				//!< clear the undo list to free up memory

	//view switching actions
	QAction *mTopViewAct;					//!< switch to top view
	QAction *mBottomViewAct;				//!< switch to bottom view
	QAction *mLeftViewAct;					//!< switch to left view
	QAction *mRightViewAct;					//!< switch to right view
	QAction *mFrontViewAct;					//!< switch to front view
	QAction *mBackViewAct;					//!< switch to back view
	QAction *mPerspViewAct;					//!< switch to perspective view or reset the current one

	QAction *mZoomInAct;
	QAction *mZoomOutAct;

	//Display Menu Actions
	QAction *mShowVerticesAct;
	QAction *mShowFaceIDsAct;
	QAction *mShowEdgeIDsAct;
	QAction *mShowVertexIDsAct;
	QAction *mShowSelectedIDsAct;
	QAction *mShowSilhouetteAct;
	QAction *mShowWireframeAct;
	QAction *mObjectOrientationAct;
	QAction *mShowNormalsAct;
	QAction *mShowGridAct;
	QAction *mShowHUDAct;
	QAction *mShowCoordinateAxesAct;
#ifdef GPU_OK
	QAction *mUseGPUAct;
#endif
	QAction *mAntialiasingAct;

	QAction *mShowFaceCentroidsAct;

	//Renderer Menu Actions
	QAction *mWireframeRendererAct;
	QAction *mNormalRendererAct;
	QAction *mShadedRendererAct;
	QAction *mLightedRendererAct;
	QAction *mTexturedRendererAct;
	QAction *mTexturedLightedAct;
	QAction *mPatchRendererAct;
	QAction *mColorableRendererAct;

	//Primitives Menu Actions
	QAction *mPrimCubeAct;
	QAction *mPrimDodecahedronAct;
	QAction *mPrimIcosahedronAct;
	QAction *mPrimTetrahedronAct;
	QAction *mPrimSoccerBallAct;
	QAction *mPrimOctahedronAct;
	QAction *mPrimGeodesicAct;

	//Object Menu Actions
	QAction *mSubdivideAllEdgesAct;
	QAction *mPlanarizeAllFacesAct;
	QAction *makeObjectSphericalAct;
	QAction *mCleanup2gonsAct;
	QAction *mCleanupWingedVerticesAct;
	QAction *mSplitValence2VerticesAct;
	QAction *mMakeObjectSmoothAct;
	QAction *mMakeWireframeAct;
	QAction *mMakeColumnsAct;
	QAction *mMakeSierpinskiAct;
	QAction *mCreateCrustThicknessAct;
	QAction *mCreateCrustScalingAct;
	QAction *mPerformCuttingAct;
	QAction *mComputeLightingAct;
	QAction *mComputeNormalsAndLightingAct;
	QAction *mAssignTextureCoordinatesAct;

	//Selection Menu Actions
	QAction *mSelectVertexAct;
	QAction *mSelectMultipleVerticesAct;
	QAction *mSelectMultipleAct;
	QAction *mEditVertexAct;
	QAction *mSelectFaceAct;
	QAction *mSelectFaceLoopAct;
	QAction *mSelectEdgeRingAct;
	QAction *mSubdivideSelectedEdgesAct;		//!< subdivide the selected edges.
	QAction *mCollapseSelectedEdgesAct;			//!< collapse the selected edges.
	QAction *mSelectMultipleFacesAct;
	QAction *mSelectSimilarFacesAct;
	QAction *mSelectSimilarAct;
	QAction *mSelectFacesByAreaAct;
	QAction *mSelectFacesByColorAct;
	QAction *mSelectionWindowAct;
	QAction *mSelectCheckerboardFacesAct;
	QAction *mSelectAllAct;
	QAction *mGrowSelectionAct;
	QAction *mShrinkSelectionAct;
	QAction *mSelectInverseAct;
	QAction *mSelectEdgeAct;
	QAction *mSelectMultipleEdgesAct;
	QAction *mSelectEdgeLoopAct;
	QAction *mSelectCornerAct;
	QAction *mClearSelectedModeAct;
	QAction *mExitSelectionModeAct;
	QAction *mSelectEdgesFromFacesAct;
	QAction *mSelectEdgesFromVerticesAct;
	QAction *mSelectFacesFromEdgesAct;
	QAction *mSelectFacesFromVerticesAct;
	QAction *mSelectVerticesFromFacesAct;
	QAction *mSelectVerticesFromEdgesAct;

	//selection mask actions
	QAction *mSelectVerticesMaskAct;
	QAction *mSelectFacesMaskAct;
	QAction *mSelectEdgesMaskAct;
	QAction *mSelectCornersMaskAct;

	//Language Menu actions;
	QActionGroup *mLanguageActionGroup;
	QAction *mEnglishAct;
	QAction *mSpanishAct;
	QAction *mGermanAct;
	QAction *mFrenchAct;
	QAction *mHindiAct;
	QAction *mItalianAct;
	QAction *mTurkishAct;
	QAction *mCatalanAct;

	//help menu
	QAction *mAboutQtAct;
	QAction *mHelpAct;
	QAction *mTopModWebAct;
	QAction *mVideoTutorialsAct;
	QAction *mTopModResearchAct;
	QAction *mCheckForUpdatesAct;
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

	//Window Menu - toolbar display
	QAction *mEditToolBarAct;
	QAction *mSelectionMaskToolBarAct;
	QAction *mPrimitivesToolBarAct;
	QAction *mToolsToolBarAct;
	QAction *mExtrusionToolBarAct;
	QAction *mConicalToolBarAct;
	QAction *mHighgenusToolBarAct;
	QAction *mTexturingToolBarAct;
	QAction *mExperimentalToolBarAct;
	QAction *mRemeshingToolBarAct;
	QAction *mHideToolBarsAct;
	QAction *mShowToolBarsAct;

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
	QToolBar *mExperimentalToolBar;
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

	QAction *mShowToolOptionsAct;
	QAction *mShowStartupDialogAct;

	StyleSheetEditor *mStyleSheetEditor;
	QAction *mEditStyleSheetAct;
	TopModPreferences *mPreferencesDialog;
	QAction *mPreferencesAct;
	QSettings *mSettings;
	#ifdef QCOMPLETER
	CommandCompleter *mCommandCompleter;
	#endif
	QStringList mCommandList;

	//popup helper animations
	QMovie *mAnimatedHelpMovie;
	QLabel *mAnimatedHelpLabel;
	QWidget *mAnimatedHelpWidget;
	QVBoxLayout *mAnimatedHelpLayout;
	QDockWidget *mAnimatedHelpDockWidget;
	QAction *mShowAnimatedHelpAct;

	std::array<QDoubleSpinBox*, 6> mSpinBoxes = {};

	//startup dialog actions for loading movies
	QAction *mTutorialNavigationAct;
	QAction *mTutorialInterfaceAct;
	QAction *mTutorialBasicAct;
	QAction *mTutorialExtrusionAct;
	QAction *mTutorialRemeshingAct;
	QAction *mTutorialHighgenusAct;
	QAction *mTutorialTexturingAct;
	//associated toolbuttons
	QToolButton *mTutorialNavigationButton;
	QToolButton *mTutorialInterfaceButton;
	QToolButton *mTutorialBasicButton;
	QToolButton *mTutorialExtrusionButton;
	QToolButton *mTutorialRemeshingButton;
	QToolButton *mTutorialHighgenusButton;
	QToolButton *mTutorialTexturingButton;
	QCheckBox *mShowStartupDialogAtStartupCheckBox;

	//translation stuff for future
	QTranslator *mTranslator_es;		//!< \todo translation widget spanish
	QTranslator *mTranslator_fr;		//!< \todo translation widget french
	QTranslator *mTranslator_de;		//!< \todo translation widget german
	QTranslator *mTranslator_tr;		//!< \todo translation widget turkish
	QTranslator *mTranslator_ca;		//!< \todo translation widget catalan
	QTranslator *mTranslator_it;		//!< \todo translation widget italian
	QTranslator *mTranslator_hi;		//!< \todo translation widget hindi

	//for screenshots
	QPixmap mViewportPixmap;
	QPixmap mAppPixmap;

	//paint bucket tool dave //11.07
	// QColor mPaintBucketColor;

public slots:

	bool viewportScreenshot(); //!< take a screenshot of just the opengl viewport
	bool appScreenshot(); //!< take a screenshot of the whole app (for forum posting and bug reports)

	//brand new stuff - dave - 9/12/07
	void setAutoSave(int value);
	void setAutoSaveDelay(double value);
	void setIncrementalSave(int value);
	void setCommandCompleterIndexToggle(int value);
	void setSingleClickExtrude(int value);
	void setIncrementalSaveMax(double value);
	void setSaveDirectory(QString s);
	void checkSaveDirectory();


	// i18n stuff
	void changeLanguage(const QString &string);
	void setLanguageSpanish();
	void setLanguageFrench();
	void setLanguageGerman();
	void setLanguageTurkish();
	void setLanguageItalian();
	void setLanguageCatalan();
	void setLanguageHindi();
	void setLanguageEnglish();

	void about();					//!< \todo  topmod developer credits
	void help();					//!< open the qtassistantclient help viewer
	void checkForUpdates();			//!< check for updates on the topMod home page
	void topModWeb();				//!< open the TopMod web page in the default browser
	void topModBlip();				//!< open the http://topmod.blip.tv web page
	void topModResearch();			//!< open the TopMod research web page in the default browser
	void documentWasModified();
	void toggleFullScreen();		//!< fullscreen mode in the mainwindow class... this is a test...

	void getCommand();				//!< this will open up the quicksilver like interface and accept a value from the user

	void setAnimatedHelpImage();

	void showAllToolBars();
	void hideAllToolBars();

	bool getShowStartupDialogAtStartup();
	void setShowStartupDialogAtStartup(int b);
	//load the seven basic tutorial movies
	void loadNavigationTutorial();
	void loadInterfaceTutorial();
	void loadBasicTutorial();
	void loadExtrusionTutorial();
	void loadRemeshingTutorial();
	void loadHighgenusTutorial();
	void loadTexturingTutorial();

	void load_texture();
	//renderers
	void useWireframeRenderer();
	void useNormalRenderer();
	void useLightedRenderer();
	void useTexturedRenderer();
	void useTexturedLitRenderer();
	void usePatchRenderer();
	void useColorableRenderer();

	void select_vertex();
	void select_multiple_vertices();
	void select_multiple();
	void edit_vertex();
	void select_face();
	void select_face_loop();
	void select_edge_ring();
	void select_multiple_faces();
	void select_similar_faces();
	void select_similar();
	void select_faces_by_area();
	void select_faces_by_color();
	void selection_window();
	void select_checkerboard_faces();
	void select_edge();
	void select_multiple_edges();
	void select_edge_loop();
	void select_corner();
	void exit_selection_mode();
	void clear_selected();
	void selectionMaskVertices();
	void selectionMaskFaces();
	void selectionMaskEdges();
	void selectionMaskCorners();
	void selectAll();
	void growSelection();
	void shrinkSelection();
	void selectInverse();
	void selectEdgesFromFaces();
	void selectEdgesFromVertices();
	void selectFacesFromEdges();
	void selectFacesFromVertices();
	void selectVerticesFromFaces();
	void selectVerticesFromEdges();
	void reorderSelectedFaces();

	void setPaintBucketColor(QColor c);
	void paintSelectedFaces();
	void clearMaterials();

	void deleteSelected();				//!< delete selected objects
	void collapseSelectedEdges();		//!< collapse selected edges

	//Basics Widget
	void toggleDeleteEdgeCleanupFlag(int state);
	void changeNumSubDivs(double value);
	void changeCornerCuttingAlpha(double value);

	void changeTileTexNum(double value);
	void toggleUseQuadsFlag(int state);

	void changeFaceAreaTolerance(double value);	//!< dave - facial area calculation

	// Extrusion
	void changeExtrudeLength(double value);
	void changeExtrudeRotation(double value);
	void changeExtrudeScale(double value);
	void changeNumExtrusions(double value);
	void changeStellateLength(double value);
	void toggleDualMeshEdgesFlag(int state);
	void toggleHexagonalizeDodecaExtrudeFlag(int state);
	void changeExtrudeLength1(double value);
	void changeExtrudeLength2(double value);
	void changeExtrudeLength3(double value);
	void changeExtrudeAngle(double value);
	void changeExtrudeLength1Icosa(double value);
	void changeExtrudeLength2Icosa(double value);
	void changeExtrudeLength3Icosa(double value);
	void changeExtrudeAngleIcosa(double value);

	void changeValence2SplitOffset(double value);
	void toggleWireframeSplit(int state);

	void changeNumSegments(double value);
	void changeMaxSegments(double value);
	void changeNumSegments2(double value);
	void changeMaxSegments2(double value);
	void changeWeight1(double value);
	void toggleSymmetricWeightsFlag(int state);
	void changeWeight2(double value);
	void changeExtraTwists(double value);

	//pinching
	void changePinch(double value);							//Ryan
	void changePinchCenter(double value);					//Ryan
	void changeBubble(double value);						//Ryan
	void changeHoleHandlePinchValue(double value);			//Ryan
	void changeHoleHandlePinchCenterValue(double value);	//Ryan
	void changeHoleHandlePinchWidthValue(double value);		//Ryan

	void changeCrustScaleFactor(double value);
	void changeCrustThickness(double value);
	void toggleCrustCleanupFlag(int state);

	void changeWireframeThickness(double value);
	void changeWireframe2Width(double value);
	void changeWireframe2Thickness(double value);
	void changeModifiedCornerCuttingThickness(double value);

	void changeColumnThickness(double value);	// Esan
	void changeColumnSegments(double value);	// Esan

	void changeMultiFaceHandleScaleFactor(double value);
	void changeMultiFaceHandleExtrudeDist(double value);
	void toggleMultiFaceHandleUseMaxOffsetFlag(int state);

	void changeMengerSpongeThickness(double value);
	void toggleMengerSpongeFractionalThicknessFlag(int state);
	void changeMengerSpongeCollapseThreshold(double value);

	void on_editStyleAction_triggered(); // stylesheet editor
	void openPreferences();

	void changeCutOffsetE(double value);	//ozgur
	void changeCutOffsetV(double value);	//ozgur
	void changePNormalBendS(double value);	//ozgur
	void changePNormalBendT(double value);	//ozgur
	void toggleGlobalCut(int state);		//ozgur
	void toggleSelectedCut(int state);		//ozgur
	void changeTiltPlane1(double value);	//dave
	void changeTiltPlane2(double value);	//dave

	//remeshing slot functions
	void toggleDooSabinEdgeFlag(int state);
	void changeRoot4Twist(double value);
	void changeRoot4Weight(double value);
	void changeVertexCuttingOffset(double value);
	void changePentagonalOffset(double value);
	void changePentagonalScaleFactor(double value);
	void changeStarOffset(double value);					// Doug
	void changeFractalOffset(double value);					// Doug
	void changeSubStellateAmount(double value);				// Eric
	void changeSubStellateCurve(double value);				// Eric
	void changeDooSabinBCnewScale(double value);			// Bei & Cansin
	void changeDooSabinBCnewLength(double value);			// Bei & Cansin
	void changeLoopStyleSubdivisionLength(double value);	// Bei & Cansin
	void changeDual1264Scale(double value);					// Bei & Cansin
	void changeDomeSubdivisionLength(double value);			// Bei & Cansin
	void changeDomeSubdivisionScale(double value);			// Bei & Cansin
	void changeDomeExtrudeLength(double value);				// dave
	void changeDomeExtrudeRotation(double value);			// dave
	void changeDomeExtrudeScale(double value);				// dave
	void changeDooSabinExtrudeTwist(double value);			// dave
	void toggleAccurateDualFlag(int state);

	//right click menu support coming soon...
	void mousePressEvent(QMouseEvent * event);
	void mouseReleaseEvent(QMouseEvent * event);
	void mouseMoveEvent(QMouseEvent * event);

	void keyPressEvent(QKeyEvent *event);
	void keyReleaseEvent(QKeyEvent *event);

	void dragEnterEvent(QDragEnterEvent *event);
	void dropEvent(QDropEvent *event);

	void getRightClickMenu();

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

	// void writeObjectOBJ(const char * filename, bool with_normals=false, bool with_tex_coords=false);
	// void writeObjectDLFL(const char * filename);
	void setUndoLimit(int limit);
	void toggleUndo();

	void clearUndoList();		// Erase all elements on Undo list
	void clearRedoList();		// Erase all elements on Redo list
	void undoPush();			// Put current object onto undo list
	void undo();				// Undo last operation
	void redo();				// Redo previously undone operation

	// Change the renderer for all viewports
	void setRenderer(DLFLRendererPtr rp);
	// void setPatchRenderer(); // just testing out the signals/slots functionality of Qt

	// Do appropriate selection depending on current mode
	// Mouse position is passed. Selected entities will be added
	// to the selection lists in the GLWidget class, which are assumed
	// to have been cleared before calling this function
	void doSelection(int x, int y);
	void doDrag(int x, int y);

	// Return pointer to the active GLWidget
	GLWidget *getActive();

	void redraw();		//!< Override redraw() method to mark subwindows also for redraw

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
		mActiveViewport->recomputePatches();
		mActiveViewport->recomputeNormals();
	};
#endif

	// void recomputeNormals();
	// void recomputeLighting();
	// void recomputePatches();
	void subdivideAllEdges();
	void subdivideSelectedEdges();
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
	void performRemeshing();	//!< Generic method for all remeshing schemes
	void performExtrusion();	//!< Generic method for all extrusion schemes on multiple faces
	//void getExtrudeMultiple();	//!< are we in multi select mode or not?
	void subdivideCatmullClark();
	void subdivideDooSabin();
	void subdivideHoneycomb();
	void subdivideRoot4();
	void subdivideCornerCutting();
	void subdivideModifiedCornerCutting();
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
	void triangulate();
	void splitValence2Vertices();
	void cleanupWingedVertices();
	void cleanup2gons();
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
	void openFile(const QString &fileName);
	bool saveFile(const QString &fileName);
	void newFile();
	bool saveFile(bool with_normals=true, bool with_tex_coords=true);
	bool saveFileAs(bool with_normals=true, bool with_tex_coords=true);
	void setCurrentFile(const QString &fileName);

	// Read the DLFL object from a file
	void readObject(const char * filename, const char *mtlfilename = nullptr);
	void readObjectQFile(const QString &file);
	// Read the DLFL object from a file - use alternate OBJ reader for OBJ files
	void readObjectAlt(const char * filename);
	// Write the DLFL object to a file
	void writeObject(const char * filename, const char *mtlfilename = nullptr, bool with_normals=true, bool with_tex_coords=true);
	void writeMTL(const char * filename);

	//exporters
	bool saveFileBezierOBJ( );
	bool saveFileLG3d( );
	bool saveFileLG3dSelected();
	bool saveFileSTL( );
	void writePatchOBJ(const char *filename);
	void writeLG3d(const char *filename, bool selected = false);
	void writeSTL(const char *filename);

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
	void loadedObject( DLFLObject *obj, QString fileName ); // echo command for loading object
#endif
	void echoCommand( QString cmd ); // all other commands

};

#endif
