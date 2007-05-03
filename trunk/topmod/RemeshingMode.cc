/****************************************************************************
 **
 **
 ****************************************************************************/
#include <QtGui>

#include "RemeshingMode.hh"

RemeshingMode::RemeshingMode(QWidget *parent, QShortcutManager *sm)
		: QWidget(parent)
{		
	setParent(0);
	mParent = parent;
	
	((MainWindow*)mParent)->setRemeshingScheme(DLFLWindow::Dual);
	
	//setup stacked widget pages here, 
	setupDual();
	setupThreeConversion();
	setupFourConversion();
	setupFiveConversion();
	setupThreePreservation();
	setupFourPreservation();
	setupFivePreservation();			
	setupSixPreservation();
	setupMiscellaneous();
	
	mDualAction = new QAction(tr("Dual"),this);
	mDualAction->setCheckable(true);
	sm->registerAction(mDualAction, "Remeshing", "");
	mDualAction->setStatusTip(tr("Enter Dual Mode"));
	mDualAction->setToolTip(tr("Dual Mode"));
	connect(mDualAction, SIGNAL(triggered()), this, SLOT(triggerDual()));

	mRootThreeAction = new QAction(tr("Root-3"),this);
	mRootThreeAction->setCheckable(true);
	sm->registerAction(mRootThreeAction, "Remeshing", "");
	mRootThreeAction->setStatusTip(tr("Enter Root-3 Mode"));
	mRootThreeAction->setToolTip(tr("Root-3 Mode"));
	connect(mRootThreeAction, SIGNAL(triggered()), this, SLOT(triggerRootThree()));

	mDualVertexTruncationAction = new QAction(tr("Dual Vertex Truncation"),this);
	mDualVertexTruncationAction->setCheckable(true);
	sm->registerAction(mDualVertexTruncationAction, "Remeshing", "");
	mDualVertexTruncationAction->setStatusTip(tr("Enter Dual Vertex Truncation Mode"));
	mDualVertexTruncationAction->setToolTip(tr("Dual Vertex Truncation Mode"));
	connect(mDualVertexTruncationAction, SIGNAL(triggered()), this, SLOT(triggerDualVertexTruncation()));

	mStellationAction = new QAction(tr("Stellate"),this);
	mStellationAction->setCheckable(true);
	sm->registerAction(mStellationAction, "Remeshing", "");
	mStellationAction->setStatusTip(tr("Enter Stellate Mode"));
	mStellationAction->setToolTip(tr("Stellate Mode"));
	connect(mStellationAction, SIGNAL(triggered()), this, SLOT(triggerStellation()));

	mDoubleStellationAction = new QAction(tr("Double Stellate"),this);
	mDoubleStellationAction->setCheckable(true);
	sm->registerAction(mDoubleStellationAction, "Remeshing", "");
	mDoubleStellationAction->setStatusTip(tr("Enter Double Stellate Mode"));
	mDoubleStellationAction->setToolTip(tr("Double Stellate Mode"));
	connect(mDoubleStellationAction, SIGNAL(triggered()), this, SLOT(triggerDoubleStellation()));

	mTwelveSixFourAction = new QAction(tr("12.6.4"),this);
	mTwelveSixFourAction->setCheckable(true);
	sm->registerAction(mTwelveSixFourAction, "Remeshing", "");
	mTwelveSixFourAction->setStatusTip(tr("Enter 12.6.4 Mode"));
	mTwelveSixFourAction->setToolTip(tr("12.6.4 Mode"));
	connect(mTwelveSixFourAction, SIGNAL(triggered()), this, SLOT(triggerTwelveSixFour()));	

	mHoneycombAction = new QAction(tr("Honeycomb"),this);
	mHoneycombAction->setCheckable(true);
	sm->registerAction(mHoneycombAction, "Remeshing", "");
	mHoneycombAction->setStatusTip(tr("Enter Honeycomb Mode"));
	mHoneycombAction->setToolTip(tr("Honeycomb Mode"));
	connect(mHoneycombAction, SIGNAL(triggered()), this, SLOT(triggerHoneycomb()));	
	
	mVertexTruncationAction = new QAction(tr("Vertex Truncation"),this);
	mVertexTruncationAction->setCheckable(true);
	sm->registerAction(mVertexTruncationAction, "Remeshing", "");
	mVertexTruncationAction->setStatusTip(tr("Enter Vertex Truncation Mode"));
	mVertexTruncationAction->setToolTip(tr("Vertex Truncation Mode"));
	connect(mVertexTruncationAction, SIGNAL(triggered()), this, SLOT(triggerVertexTruncation()));	
	
	mDualTwelveSixFourAction = new QAction(tr("Dual 12.6.4"),this);
	mDualTwelveSixFourAction->setCheckable(true);
	sm->registerAction(mDualTwelveSixFourAction, "Remeshing", "");
	mDualTwelveSixFourAction->setStatusTip(tr("Enter Dual 12.6.4 Mode"));
	mDualTwelveSixFourAction->setToolTip(tr("Dual 12.6.4 Mode"));
	connect(mDualTwelveSixFourAction, SIGNAL(triggered()), this, SLOT(triggerDualTwelveSixFour()));	
	
	mLinearVertexAction = new QAction(tr("Linear Vertex Insertion"),this);
	mLinearVertexAction->setCheckable(true);
	sm->registerAction(mLinearVertexAction, "Remeshing", "");
	mLinearVertexAction->setStatusTip(tr("Enter Linear Vertex Insertion Mode"));
	mLinearVertexAction->setToolTip(tr("Linear Vertex Insertion Mode"));
	connect(mLinearVertexAction, SIGNAL(triggered()), this, SLOT(triggerLinearVertex()));	
	
	mCatmullClarkAction = new QAction(tr("Catmull Clark"),this);
	mCatmullClarkAction->setCheckable(true);
	sm->registerAction(mCatmullClarkAction, "Remeshing", "");
	mCatmullClarkAction->setStatusTip(tr("Enter Catmull Clark Mode"));
	mCatmullClarkAction->setToolTip(tr("Catmull Clark Mode"));
	connect(mCatmullClarkAction, SIGNAL(triggered()), this, SLOT(triggerCatmullClark()));	
	
	mStellateEdgeRemovalAction = new QAction(tr("Stellate with Edge Removal"),this);
	mStellateEdgeRemovalAction->setCheckable(true);
	sm->registerAction(mStellateEdgeRemovalAction, "Remeshing", "");
	mStellateEdgeRemovalAction->setStatusTip(tr("Enter Stellate with Edge Removal Mode"));
	mStellateEdgeRemovalAction->setToolTip(tr("Stellate with Edge Removal Mode"));
	connect(mStellateEdgeRemovalAction, SIGNAL(triggered()), this, SLOT(triggerStellateEdgeRemoval()));	
	
	mDooSabinAction = new QAction(tr("Doo Sabin"),this);
	mDooSabinAction->setCheckable(true);
	sm->registerAction(mDooSabinAction, "Remeshing", "");
	mDooSabinAction->setStatusTip(tr("Enter Doo Sabin Mode"));
	mDooSabinAction->setToolTip(tr("Doo Sabin Mode"));
	connect(mDooSabinAction, SIGNAL(triggered()), this, SLOT(triggerDooSabin()));	
	
	mCornerCuttingAction = new QAction(tr("Corner Cutting"),this);
	mCornerCuttingAction->setCheckable(true);
	sm->registerAction(mCornerCuttingAction, "Remeshing", "");
	mCornerCuttingAction->setStatusTip(tr("Enter Corner Cutting Mode"));
	mCornerCuttingAction->setToolTip(tr("Corner Cutting Mode"));
	connect(mCornerCuttingAction, SIGNAL(triggered()), this, SLOT(triggerCornerCutting()));	

	mSimplestAction = new QAction(tr("Simplest"),this);
	mSimplestAction->setCheckable(true);
	sm->registerAction(mSimplestAction, "Remeshing", "");
	mSimplestAction->setStatusTip(tr("Enter Simplest Mode"));
	mSimplestAction->setToolTip(tr("Simplest Mode"));
	connect(mSimplestAction, SIGNAL(triggered()), this, SLOT(triggerSimplest()));	

	mPentagonalizationAction = new QAction(tr("Pentagonalization"),this);
	mPentagonalizationAction->setCheckable(true);
	sm->registerAction(mPentagonalizationAction, "Remeshing", "");
	mPentagonalizationAction->setStatusTip(tr("Enter Pentagonalization Mode"));
	mPentagonalizationAction->setToolTip(tr("Pentagonalization Mode"));
	connect(mPentagonalizationAction, SIGNAL(triggered()), this, SLOT(triggerPentagonalization()));	

	mCubicPentagonalizationAction = new QAction(tr("Cubic Pentagonalization"),this);
	mCubicPentagonalizationAction->setCheckable(true);
	sm->registerAction(mCubicPentagonalizationAction, "Remeshing", "");
	mCubicPentagonalizationAction->setStatusTip(tr("Enter Cubic Pentagonalization Mode"));
	mCubicPentagonalizationAction->setToolTip(tr("Cubic Pentagonalization Mode"));
	connect(mCubicPentagonalizationAction, SIGNAL(triggered()), this, SLOT(triggerCubicPentagonalization()));	

	mDualPentagonalizationAction = new QAction(tr("Dual Pentagonalization"),this);
	mDualPentagonalizationAction->setCheckable(true);
	sm->registerAction(mDualPentagonalizationAction, "Remeshing", "");
	mDualPentagonalizationAction->setStatusTip(tr("Enter Dual Pentagonalization Mode"));
	mDualPentagonalizationAction->setToolTip(tr("Dual Pentagonalization Mode"));
	connect(mDualPentagonalizationAction, SIGNAL(triggered()), this, SLOT(triggerDualPentagonalization()));	

	mLoopStyleRemeshingAction = new QAction(tr("Loop Style Remeshing"),this);
	mLoopStyleRemeshingAction->setCheckable(true);
	sm->registerAction(mLoopStyleRemeshingAction, "Remeshing", "");
	mLoopStyleRemeshingAction->setStatusTip(tr("Enter Loop Style Remeshing Mode"));
	mLoopStyleRemeshingAction->setToolTip(tr("Loop Style Remeshing Mode"));
	connect(mLoopStyleRemeshingAction, SIGNAL(triggered()), this, SLOT(triggerLoopStyleRemeshing()));		

	mLoopSubdivisionAction = new QAction(tr("Loop Subdivision"),this);
	mLoopSubdivisionAction->setCheckable(true);
	sm->registerAction(mLoopSubdivisionAction, "Remeshing", "");
	mLoopSubdivisionAction->setStatusTip(tr("Enter Loop Subdivision Mode"));
	mLoopSubdivisionAction->setToolTip(tr("Loop Subdivision Mode"));
	connect(mLoopSubdivisionAction, SIGNAL(triggered()), this, SLOT(triggerLoopSubdivision()));		
	
	mDualLoopStyleRemeshingAction = new QAction(tr("Dual Loop Style Remeshing"),this);
	mDualLoopStyleRemeshingAction->setCheckable(true);
	sm->registerAction(mDualLoopStyleRemeshingAction, "Remeshing", "");
	mDualLoopStyleRemeshingAction->setStatusTip(tr("Enter Dual Loop Style Remeshing Mode"));
	mDualLoopStyleRemeshingAction->setToolTip(tr("Dual Loop Style Remeshing Mode"));
	connect(mDualLoopStyleRemeshingAction, SIGNAL(triggered()), this, SLOT(triggerDualLoopStyleRemeshing()));		
	
	mDualLoopSubdivisionAction = new QAction(tr("Dual Loop Subdivision"),this);
	mDualLoopSubdivisionAction->setCheckable(true);
	sm->registerAction(mDualLoopSubdivisionAction, "Remeshing", "");
	mDualLoopSubdivisionAction->setStatusTip(tr("Enter Dual Loop Subdivision Mode"));
	mDualLoopSubdivisionAction->setToolTip(tr("Dual Loop Subdivision Mode"));
	connect(mDualLoopSubdivisionAction, SIGNAL(triggered()), this, SLOT(triggerDualLoopSubdivision()));		
	
	mGlobalExtrudeAction = new QAction(tr("Global Extrude"),this);
	mGlobalExtrudeAction->setCheckable(true);
	sm->registerAction(mGlobalExtrudeAction, "Remeshing", "");
	mGlobalExtrudeAction->setStatusTip(tr("Enter Global Extrude Mode"));
	mGlobalExtrudeAction->setToolTip(tr("Global Extrude Mode"));
	connect(mGlobalExtrudeAction, SIGNAL(triggered()), this, SLOT(triggerGlobalExtrude()));

	mCheckerboardAction = new QAction(tr("Checkerboard"),this);
	mCheckerboardAction->setCheckable(true);
	sm->registerAction(mCheckerboardAction, "Remeshing", "");
	mCheckerboardAction->setStatusTip(tr("Enter Checkerboard Mode"));
	mCheckerboardAction->setToolTip(tr("Checkerboard Mode"));
	connect(mCheckerboardAction, SIGNAL(triggered()), this, SLOT(triggerCheckerboard()));

	mDualGlobalExtrudeAction = new QAction(tr("Dual Global Extrude"),this);
	mDualGlobalExtrudeAction->setCheckable(true);
	sm->registerAction(mDualGlobalExtrudeAction, "Remeshing", "");
	mDualGlobalExtrudeAction->setStatusTip(tr("Enter Dual Global Extrude Mode"));
	mDualGlobalExtrudeAction->setToolTip(tr("Dual Global Extrude Mode"));
	connect(mDualGlobalExtrudeAction, SIGNAL(triggered()), this, SLOT(triggerDualGlobalExtrude()));

	mDualCheckerboardAction = new QAction(tr("Dual Checkerboard"),this);
	mDualCheckerboardAction->setCheckable(true);
	sm->registerAction(mDualCheckerboardAction, "Remeshing", "");
	mDualCheckerboardAction->setStatusTip(tr("Enter Dual Checkerboard Mode"));
	mDualCheckerboardAction->setToolTip(tr("Dual Checkerboard Mode"));
	connect(mDualCheckerboardAction, SIGNAL(triggered()), this, SLOT(triggerDualCheckerboard()));

	mPentagonPreservingAction = new QAction(tr("Pentagon Preserving"),this);
	mPentagonPreservingAction->setCheckable(true);
	sm->registerAction(mPentagonPreservingAction, "Remeshing", "");
	mPentagonPreservingAction->setStatusTip(tr("Enter Pentagon Preserving Mode"));
	mPentagonPreservingAction->setToolTip(tr("Pentagon Preserving Mode"));
	connect(mPentagonPreservingAction, SIGNAL(triggered()), this, SLOT(triggerPentagonPreserving()));

	mDualPentagonPreservingAction = new QAction(tr("Dual Pentagon Preserving"),this);
	mDualPentagonPreservingAction->setCheckable(true);
	sm->registerAction(mDualPentagonPreservingAction, "Remeshing", "");
	mDualPentagonPreservingAction->setStatusTip(tr("Enter Dual Pentagon Preserving Mode"));
	mDualPentagonPreservingAction->setToolTip(tr("Dual Pentagon Preserving Mode"));
	connect(mDualPentagonPreservingAction, SIGNAL(triggered()), this, SLOT(triggerDualPentagonPreserving()));	
	
	mDualLoopStyleRemeshingSixAction = new QAction(tr("Dual Loop Style"),this);
	mDualLoopStyleRemeshingSixAction->setCheckable(true);
	sm->registerAction(mDualLoopStyleRemeshingSixAction, "Remeshing", "");
	mDualLoopStyleRemeshingSixAction->setStatusTip(tr("Enter Loop Style Remeshing Mode"));
	mDualLoopStyleRemeshingSixAction->setToolTip(tr("Dual Loop Style Remeshing Mode"));
	connect(mDualLoopStyleRemeshingSixAction, SIGNAL(triggered()), this, SLOT(triggerDualLoopStyleRemeshingSix()));	
	
	mLoopStyleRemeshingSixAction = new QAction(tr("Loop Style"),this);
	mLoopStyleRemeshingSixAction->setCheckable(true);
	sm->registerAction(mLoopStyleRemeshingSixAction, "Remeshing", "");
	mLoopStyleRemeshingSixAction->setStatusTip(tr("Enter Loop Style Remeshing Mode"));
	mLoopStyleRemeshingSixAction->setToolTip(tr("Loop Style Remeshing Mode"));
	connect(mLoopStyleRemeshingSixAction, SIGNAL(triggered()), this, SLOT(triggerLoopStyleRemeshingSix()));	
	
	mFractalAction = new QAction(tr("Fractal"),this);
	mFractalAction->setCheckable(true);
	sm->registerAction(mFractalAction, "Remeshing", "");
	mFractalAction->setStatusTip(tr("Enter Fractal Mode"));
	mFractalAction->setToolTip(tr("Fractal Mode"));
	connect(mFractalAction, SIGNAL(triggered()), this, SLOT(triggerFractal()));	
	
	mDoubleStellateMiscAction = new QAction(tr("Double Stellate"),this);
	mDoubleStellateMiscAction->setCheckable(true);
	sm->registerAction(mDoubleStellateMiscAction, "Remeshing", "");
	mDoubleStellateMiscAction->setStatusTip(tr("Enter Double Stellate Mode"));
	mDoubleStellateMiscAction->setToolTip(tr("Double Stellate Mode"));
	connect(mDoubleStellateMiscAction, SIGNAL(triggered()), this, SLOT(triggerDoubleStellateMisc()));	
	
	mDooSabinBCAction = new QAction(tr("Doo Sabin BC"),this);
	mDooSabinBCAction->setCheckable(true);
	sm->registerAction(mDooSabinBCAction, "Remeshing", "");
	mDooSabinBCAction->setStatusTip(tr("Enter Doo Sabin BC Mode"));
	mDooSabinBCAction->setToolTip(tr("Doo Sabin BC Mode"));
	connect(mDooSabinBCAction, SIGNAL(triggered()), this, SLOT(triggerDooSabinBC()));	
	
	mDooSabinBCNewAction = new QAction(tr("Doo Sabin BC New"),this);
	mDooSabinBCNewAction->setCheckable(true);
	sm->registerAction(mDooSabinBCNewAction, "Remeshing", "");
	mDooSabinBCNewAction->setStatusTip(tr("Enter Doo Sabin BC New Mode"));
	mDooSabinBCNewAction->setToolTip(tr("Doo Sabin BC New Mode"));
	connect(mDooSabinBCNewAction, SIGNAL(triggered()), this, SLOT(triggerDooSabinBCNew()));	
	
	mDomeAction = new QAction(tr("Dome"),this);
	mDomeAction->setCheckable(true);
	sm->registerAction(mDomeAction, "Remeshing", "");
	mDomeAction->setStatusTip(tr("Enter Dome Mode"));
	mDomeAction->setToolTip(tr("Dome Mode"));
	connect(mDomeAction, SIGNAL(triggered()), this, SLOT(triggerDome()));	
	
	mSubdivideFaceAction = new QAction(tr("Subdivide Face"),this);
	mSubdivideFaceAction->setCheckable(true);
	sm->registerAction(mSubdivideFaceAction, "Remeshing", "");
	mSubdivideFaceAction->setStatusTip(tr("Enter Subdivide Face Mode"));
	mSubdivideFaceAction->setToolTip(tr("Subdivide Face Mode"));
	connect(mSubdivideFaceAction, SIGNAL(triggered()), this, SLOT(triggerSubdivideFace()));	

}

QMenu* RemeshingMode::getMenu(){
	
	mRemeshingMenu = new QMenu(tr("Remeshing"));
	
	mRemeshingMenu->addAction(mDualAction);
	
	mThreeConversionMenu = new QMenu(tr("3-Conversion"));
	mThreeConversionMenu->addAction(mRootThreeAction);
	mThreeConversionMenu->addAction(mDualVertexTruncationAction);
	mThreeConversionMenu->addAction(mStellationAction);
	mThreeConversionMenu->addAction(mDoubleStellationAction);
	mThreeConversionMenu->addAction(mTwelveSixFourAction);
	mThreeConversionMenu->addAction(mHoneycombAction);
	mThreeConversionMenu->addAction(mVertexTruncationAction);
	mThreeConversionMenu->addAction(mDualTwelveSixFourAction);
	
	mRemeshingMenu->addMenu(mThreeConversionMenu);
	
	mFourConversionMenu = new QMenu(tr("4-Conversion"));
	mFourConversionMenu->addAction(mLinearVertexAction);
	mFourConversionMenu->addAction(mCatmullClarkAction);
	mFourConversionMenu->addAction(mStellateEdgeRemovalAction);
	mFourConversionMenu->addAction(mDooSabinAction);
	mFourConversionMenu->addAction(mCornerCuttingAction);
	mFourConversionMenu->addAction(mSimplestAction);
	
	mRemeshingMenu->addMenu(mFourConversionMenu);
	
	mFiveConversionMenu = new QMenu(tr("5-Conversion"));	
	mFiveConversionMenu->addAction(mPentagonalizationAction);
	mFiveConversionMenu->addAction(mCubicPentagonalizationAction);
	mFiveConversionMenu->addAction(mDualPentagonalizationAction);
	
	mRemeshingMenu->addMenu(mFiveConversionMenu);
	
	mThreePreservationMenu = new QMenu(tr("3-Preservation"));
	mThreePreservationMenu->addAction(mLoopStyleRemeshingAction);
	mThreePreservationMenu->addAction(mLoopSubdivisionAction);
	mThreePreservationMenu->addAction(mDualLoopStyleRemeshingAction);
	mThreePreservationMenu->addAction(mDualLoopSubdivisionAction);
	
	mRemeshingMenu->addMenu(mThreePreservationMenu);
	
	mFourPreservationMenu = new QMenu(tr("4-Preservation"));
	mFourPreservationMenu->addAction(mGlobalExtrudeAction);
	mFourPreservationMenu->addAction(mCheckerboardAction);
	mFourPreservationMenu->addAction(mDualGlobalExtrudeAction);
	mFourPreservationMenu->addAction(mDualCheckerboardAction);
	
	mRemeshingMenu->addMenu(mFourPreservationMenu);
	
	mFivePreservationMenu = new QMenu(tr("5-Preservation"));
	mFivePreservationMenu->addAction(mPentagonPreservingAction);
	mFivePreservationMenu->addAction(mDualPentagonPreservingAction);
	
	mRemeshingMenu->addMenu(mFivePreservationMenu);
	
	mSixPreservationMenu = new QMenu(tr("6-Preservation"));
	mSixPreservationMenu->addAction(mDualLoopStyleRemeshingSixAction);
	mSixPreservationMenu->addAction(mLoopStyleRemeshingSixAction);	
	
	mRemeshingMenu->addMenu(mSixPreservationMenu);
	
	mMiscellaneousMenu = new QMenu(tr("Miscellaneous"));
	mMiscellaneousMenu->addAction(mFractalAction);
	mMiscellaneousMenu->addAction(mDoubleStellateMiscAction);
	mMiscellaneousMenu->addAction(mDooSabinBCAction);
	mMiscellaneousMenu->addAction(mDooSabinBCNewAction);
	mMiscellaneousMenu->addAction(mDomeAction);
	mMiscellaneousMenu->addAction(mSubdivideFaceAction);

	mRemeshingMenu->addMenu(mMiscellaneousMenu);	
	
	return mRemeshingMenu;
}

void RemeshingMode::addActions(QActionGroup *actionGroup, QToolBar *toolBar, QStackedWidget *stackedWidget){
	
	actionGroup->addAction(mDualAction);
	actionGroup->addAction(mRootThreeAction);
	actionGroup->addAction(mDualVertexTruncationAction);
	actionGroup->addAction(mStellationAction);
	actionGroup->addAction(mDoubleStellationAction);
	actionGroup->addAction(mTwelveSixFourAction);
	actionGroup->addAction(mHoneycombAction);
	actionGroup->addAction(mVertexTruncationAction);
	actionGroup->addAction(mDualTwelveSixFourAction);
	actionGroup->addAction(mLinearVertexAction);
	actionGroup->addAction(mCatmullClarkAction);
	actionGroup->addAction(mStellateEdgeRemovalAction);
	actionGroup->addAction(mDooSabinAction);
	actionGroup->addAction(mCornerCuttingAction);
	actionGroup->addAction(mSimplestAction);
	actionGroup->addAction(mPentagonalizationAction);
	actionGroup->addAction(mCubicPentagonalizationAction);
	actionGroup->addAction(mDualPentagonalizationAction);
	actionGroup->addAction(mLoopStyleRemeshingAction);
	actionGroup->addAction(mLoopSubdivisionAction);
	actionGroup->addAction(mDualLoopStyleRemeshingAction);
	actionGroup->addAction(mDualLoopSubdivisionAction);
	actionGroup->addAction(mGlobalExtrudeAction);
	actionGroup->addAction(mCheckerboardAction);
	actionGroup->addAction(mDualGlobalExtrudeAction);
	actionGroup->addAction(mDualCheckerboardAction);
	actionGroup->addAction(mPentagonPreservingAction);
	actionGroup->addAction(mDualPentagonPreservingAction);
	actionGroup->addAction(mDualLoopStyleRemeshingSixAction);
	actionGroup->addAction(mLoopStyleRemeshingSixAction);	
	actionGroup->addAction(mFractalAction);
	actionGroup->addAction(mDoubleStellateMiscAction);
	actionGroup->addAction(mDooSabinBCAction);
	actionGroup->addAction(mDooSabinBCNewAction);
	actionGroup->addAction(mDomeAction);
	
	actionGroup->addAction(mSubdivideFaceAction);
	
	toolBar->addAction(mDualAction);
	toolBar->addAction(mRootThreeAction);
	toolBar->addAction(mDualVertexTruncationAction);
	toolBar->addAction(mStellationAction);
	toolBar->addAction(mDoubleStellationAction);
	toolBar->addAction(mTwelveSixFourAction);
	toolBar->addSeparator();
	toolBar->addAction(mHoneycombAction);
	toolBar->addAction(mVertexTruncationAction);
	toolBar->addAction(mDualTwelveSixFourAction);
	toolBar->addAction(mLinearVertexAction);
	toolBar->addAction(mCatmullClarkAction);
	toolBar->addAction(mStellateEdgeRemovalAction);
	toolBar->addAction(mDooSabinAction);
	toolBar->addAction(mCornerCuttingAction);
	toolBar->addAction(mSimplestAction);
	toolBar->addSeparator();
	toolBar->addAction(mPentagonalizationAction);
	toolBar->addAction(mCubicPentagonalizationAction);
	toolBar->addAction(mDualPentagonalizationAction);
	toolBar->addSeparator();
	toolBar->addAction(mLoopStyleRemeshingAction);
	toolBar->addAction(mLoopSubdivisionAction);
	toolBar->addAction(mDualLoopStyleRemeshingAction);
	toolBar->addAction(mDualLoopSubdivisionAction);
	toolBar->addSeparator();
	toolBar->addAction(mGlobalExtrudeAction);
	toolBar->addAction(mCheckerboardAction);
	toolBar->addAction(mDualGlobalExtrudeAction);
	toolBar->addAction(mDualCheckerboardAction);
	toolBar->addSeparator();
	toolBar->addAction(mPentagonPreservingAction);
	toolBar->addAction(mDualPentagonPreservingAction);
	toolBar->addSeparator();
	toolBar->addAction(mDualLoopStyleRemeshingSixAction);
	toolBar->addAction(mLoopStyleRemeshingSixAction);	
	toolBar->addSeparator();
	toolBar->addAction(mFractalAction);
	toolBar->addAction(mDoubleStellateMiscAction);
	toolBar->addAction(mDooSabinBCAction);
	toolBar->addAction(mDooSabinBCNewAction);
	toolBar->addAction(mDomeAction);
	toolBar->addAction(mSubdivideFaceAction);
	
	stackedWidget->addWidget(mDualWidget);
	stackedWidget->addWidget(mRootThreeWidget);
	stackedWidget->addWidget(mDualVertexTruncationWidget);
	stackedWidget->addWidget(mStellationWidget);
	stackedWidget->addWidget(mDoubleStellationWidget);
	stackedWidget->addWidget(mTwelveSixFourWidget);
	stackedWidget->addWidget(mHoneycombWidget);
	stackedWidget->addWidget(mVertexTruncationWidget);
	stackedWidget->addWidget(mDualTwelveSixFourWidget);
	stackedWidget->addWidget(mLinearVertexWidget);
	stackedWidget->addWidget(mCatmullClarkWidget);
	stackedWidget->addWidget(mStellateEdgeRemovalWidget);
	stackedWidget->addWidget(mDooSabinWidget);
	stackedWidget->addWidget(mCornerCuttingWidget);
	stackedWidget->addWidget(mSimplestWidget);
	stackedWidget->addWidget(mPentagonalizationWidget);
	stackedWidget->addWidget(mCubicPentagonalizationWidget);
	stackedWidget->addWidget(mDualPentagonalizationWidget);
	stackedWidget->addWidget(mLoopStyleRemeshingWidget);
	stackedWidget->addWidget(mLoopSubdivisionWidget);
	stackedWidget->addWidget(mDualLoopStyleRemeshingWidget);
	stackedWidget->addWidget(mDualLoopSubdivisionWidget);
	stackedWidget->addWidget(mGlobalExtrudeWidget);
	stackedWidget->addWidget(mCheckerboardWidget);
	stackedWidget->addWidget(mDualGlobalExtrudeWidget);
	stackedWidget->addWidget(mDualCheckerboardWidget);
	stackedWidget->addWidget(mPentagonPreservingWidget);
	stackedWidget->addWidget(mDualPentagonPreservingWidget);
	stackedWidget->addWidget(mDualLoopStyleRemeshingSixWidget);
	stackedWidget->addWidget(mLoopStyleRemeshingSixWidget);	
	stackedWidget->addWidget(mFractalWidget);
	stackedWidget->addWidget(mDoubleStellateMiscWidget);
	stackedWidget->addWidget(mDooSabinBCWidget);
	stackedWidget->addWidget(mDooSabinBCNewWidget);
	stackedWidget->addWidget(mDomeWidget);
	stackedWidget->addWidget(mSubdivideFaceWidget);											
	
}

void RemeshingMode::setupDual(){
	
	mDualWidget = new QWidget;
	mDualLayout = new QBoxLayout(QBoxLayout::LeftToRight);
	mDualLayout->setMargin(0);
	
	QCheckBox *dualFasterCheckBox = new QCheckBox(tr("Use Faster Method (Less Accurate)"));					
	connect(dualFasterCheckBox, SIGNAL(stateChanged(int)),
					((MainWindow*)mParent),SLOT(toggleAccurateDualFlag(int)) );
	
	mDualLayout->addWidget(dualFasterCheckBox);
	
	//create crust button
	QPushButton *dualCreateButton = new QPushButton(tr("Create Dual"), this);
	connect(dualCreateButton, SIGNAL(clicked()),
					((MainWindow*)mParent),SLOT(perform_remeshing()) );

	mDualLayout->addWidget(dualCreateButton);	
	mDualLayout->addStretch(1);
	mDualWidget->setLayout(mDualLayout);
}

void RemeshingMode::triggerDual(){

	((MainWindow*)mParent)->setToolOptions(mDualWidget);
	((MainWindow*)mParent)->setRemeshingScheme(DLFLWindow::Dual);
}

void RemeshingMode::setupThreeConversion(){

	mRootThreeWidget = new QWidget;
	mDualVertexTruncationWidget = new QWidget;
	mStellationWidget = new QWidget;
	mDoubleStellationWidget = new QWidget;
	mTwelveSixFourWidget = new QWidget;
	mHoneycombWidget = new QWidget;
	mVertexTruncationWidget = new QWidget;
	mDualTwelveSixFourWidget = new QWidget;

	//create the panels for each of these three conversion schemes
	//root 3
	mRootThreeLayout = new QBoxLayout(QBoxLayout::LeftToRight);
	mRootThreeLayout->setMargin(0);
	QPushButton *rootThreeCreateButton = new QPushButton(tr("Perform Root 3 Remeshing"), this);
	connect(rootThreeCreateButton, SIGNAL(clicked()),
					((MainWindow*)mParent),SLOT(perform_remeshing()) );
	mRootThreeLayout->addWidget(rootThreeCreateButton);
	mRootThreeLayout->addStretch(1);
	mRootThreeWidget->setLayout(mRootThreeLayout);
	
	//dual vertex truncation
	mDualVertexTruncationLayout = new QBoxLayout(QBoxLayout::LeftToRight);
	mDualVertexTruncationLayout->setMargin(0);
	QPushButton *dualVertexTruncationButton = new QPushButton(tr("Perform Remeshing"), this);
	connect(dualVertexTruncationButton, SIGNAL(clicked()),
					((MainWindow*)mParent),SLOT(perform_remeshing()) );
	mDualVertexTruncationLayout->addWidget(dualVertexTruncationButton);
	mDualVertexTruncationLayout->addStretch(1);
	mDualVertexTruncationWidget->setLayout(mDualVertexTruncationLayout);

	//stellation
	mStellationLayout = new QBoxLayout(QBoxLayout::LeftToRight);
	mStellationLayout->setMargin(0);
	QPushButton *stellationButton = new QPushButton(tr("Perform Remeshing"), this);
	connect(stellationButton, SIGNAL(clicked()),
					((MainWindow*)mParent),SLOT(perform_remeshing()) );
	mStellationLayout->addWidget(stellationButton);
	mStellationLayout->addStretch(1);
	mStellationWidget->setLayout(mStellationLayout);

	//double stellate
	mDoubleStellationLayout = new QBoxLayout(QBoxLayout::LeftToRight);
	mDoubleStellationLayout->setMargin(0);
	//scale factor - 0.00-10.00
	QLabel *starLabel = new QLabel(tr("Offset:"));
	QDoubleSpinBox *starSpinBox = new QDoubleSpinBox;
	starSpinBox->setRange(0.0, 1.0);
	starSpinBox->setSingleStep(0.01);
	starSpinBox->setValue(0.0);
	starSpinBox->setDecimals(2);
	connect(starSpinBox, SIGNAL(valueChanged(double)),
					((MainWindow*)mParent),SLOT(changeStarOffset(double)) );	
	
	mDoubleStellationLayout->addWidget(starLabel);
  mDoubleStellationLayout->addWidget(starSpinBox);
	//create button
	QPushButton *starButton = new QPushButton(tr("Perform Remeshing"), this);
	connect(starButton, SIGNAL(clicked()),
					((MainWindow*)mParent),SLOT(perform_remeshing()) );
	mDoubleStellationLayout->addWidget(starButton);
	mDoubleStellationLayout->addStretch(1);
	mDoubleStellationWidget->setLayout(mDoubleStellationLayout);
	
	//12.6.4
	mTwelveSixFourLayout = new QBoxLayout(QBoxLayout::LeftToRight);
	mTwelveSixFourLayout->setMargin(0);
	//scale factor - 0.00-10.00
	QLabel *twelveSixFourLabel = new QLabel(tr("Offset:"));
	QDoubleSpinBox *twelveSixFourSpinBox = new QDoubleSpinBox;
	twelveSixFourSpinBox->setRange(0.0, 1.0);
	twelveSixFourSpinBox->setSingleStep(0.01);
	twelveSixFourSpinBox->setValue(0.7);
	twelveSixFourSpinBox->setDecimals(2);
	connect(twelveSixFourSpinBox, SIGNAL(valueChanged(double)),
					((MainWindow*)mParent),SLOT(changeSubStellateCurve(double)) );	
	
	mTwelveSixFourLayout->addWidget(twelveSixFourLabel);
  mTwelveSixFourLayout->addWidget(twelveSixFourSpinBox);
	//create button
	QPushButton *twelveSixFourButton = new QPushButton(tr("Perform Remeshing"), this);
	connect(twelveSixFourButton, SIGNAL(clicked()),
					((MainWindow*)mParent),SLOT(perform_remeshing()) );
	mTwelveSixFourLayout->addWidget(twelveSixFourButton);
	mTwelveSixFourLayout->addStretch(1);
	mTwelveSixFourWidget->setLayout(mTwelveSixFourLayout);
	
	//honeycomb
	mHoneycombLayout = new QBoxLayout(QBoxLayout::LeftToRight);
	mHoneycombLayout->setMargin(0);
	QPushButton *honeycombButton = new QPushButton(tr("Perform Remeshing"), this);
	connect(honeycombButton, SIGNAL(clicked()),
					((MainWindow*)mParent),SLOT(perform_remeshing()) );
	mHoneycombLayout->addWidget(honeycombButton);
	mHoneycombLayout->addStretch(1);
	mHoneycombWidget->setLayout(mHoneycombLayout);
	
	//vertexTruncation
	mVertexTruncationLayout = new QBoxLayout(QBoxLayout::LeftToRight);
	mVertexTruncationLayout->setMargin(0);
	//scale factor
	QLabel *vertexTruncationLabel = new QLabel(tr("Offset:"));
	QDoubleSpinBox *vertexTruncationSpinBox = new QDoubleSpinBox;
	vertexTruncationSpinBox->setRange(0.0, 0.5);
	vertexTruncationSpinBox->setSingleStep(0.01);
	vertexTruncationSpinBox->setValue(0.25);
	vertexTruncationSpinBox->setDecimals(2);
	connect(vertexTruncationSpinBox, SIGNAL(valueChanged(double)),
					((MainWindow*)mParent),SLOT(changeVertexCuttingOffset(double)) );	
	
	mVertexTruncationLayout->addWidget(vertexTruncationLabel);
  mVertexTruncationLayout->addWidget(vertexTruncationSpinBox);
	QPushButton *vertexTruncationButton = new QPushButton(tr("Perform Remeshing"), this);
	connect(vertexTruncationButton, SIGNAL(clicked()),
					((MainWindow*)mParent),SLOT(perform_remeshing()) );
	mVertexTruncationLayout->addWidget(vertexTruncationButton);
	mVertexTruncationLayout->addStretch(1);
	mVertexTruncationWidget->setLayout(mVertexTruncationLayout);
		
	//dual 12.6.4
	mDualTwelveSixFourLayout = new QBoxLayout(QBoxLayout::LeftToRight);
	mDualTwelveSixFourLayout->setMargin(0);
	//scale factor
	QLabel *dualTwelveSixFourLabel = new QLabel(tr("Scale:"));
	QDoubleSpinBox *dualTwelveSixFourSpinBox = new QDoubleSpinBox;
	dualTwelveSixFourSpinBox->setRange(0.0, 1.0);
	dualTwelveSixFourSpinBox->setSingleStep(0.01);
	dualTwelveSixFourSpinBox->setValue(0.06);
	dualTwelveSixFourSpinBox->setDecimals(2);
	connect(dualTwelveSixFourSpinBox, SIGNAL(valueChanged(double)),
					((MainWindow*)mParent),SLOT(changeDual1264Scale(double)) );	
	
	mDualTwelveSixFourLayout->addWidget(dualTwelveSixFourLabel);
  mDualTwelveSixFourLayout->addWidget(dualTwelveSixFourSpinBox);
	//create button
	QPushButton *dualTwelveSixFourButton = new QPushButton(tr("Perform Remeshing"), this);
	connect(dualTwelveSixFourButton, SIGNAL(clicked()),
					((MainWindow*)mParent),SLOT(perform_remeshing()) );
					
	mDualTwelveSixFourLayout->addWidget(dualTwelveSixFourButton);
	mDualTwelveSixFourLayout->addStretch(1);
	mDualTwelveSixFourWidget->setLayout(mDualTwelveSixFourLayout);
	
}

void RemeshingMode::triggerRootThree(){

	((MainWindow*)mParent)->setToolOptions(mRootThreeWidget);
	((MainWindow*)mParent)->setRemeshingScheme(DLFLWindow::Root3);
}

void RemeshingMode::triggerDualVertexTruncation(){

	((MainWindow*)mParent)->setToolOptions(mDualVertexTruncationWidget);
	((MainWindow*)mParent)->setRemeshingScheme(DLFLWindow::DualVertexTrunc);
}

void RemeshingMode::triggerStellation(){

	((MainWindow*)mParent)->setToolOptions(mStellationWidget);
	((MainWindow*)mParent)->setRemeshingScheme(DLFLWindow::GlobalStellate);
}

void RemeshingMode::triggerDoubleStellation(){

	((MainWindow*)mParent)->setToolOptions(mDoubleStellationWidget);
	((MainWindow*)mParent)->setRemeshingScheme(DLFLWindow::Star);
}

void RemeshingMode::triggerTwelveSixFour(){

	((MainWindow*)mParent)->setToolOptions(mTwelveSixFourWidget);
	((MainWindow*)mParent)->setRemeshingScheme(DLFLWindow::Generic1264);
}

void RemeshingMode::triggerHoneycomb(){

	((MainWindow*)mParent)->setToolOptions(mHoneycombWidget);
	((MainWindow*)mParent)->setRemeshingScheme(DLFLWindow::Honeycomb);
}

void RemeshingMode::triggerVertexTruncation(){

	((MainWindow*)mParent)->setToolOptions(mVertexTruncationWidget);
	((MainWindow*)mParent)->setRemeshingScheme(DLFLWindow::VertexTrunc);
}

void RemeshingMode::triggerDualTwelveSixFour(){

	((MainWindow*)mParent)->setToolOptions(mDualTwelveSixFourWidget);
	((MainWindow*)mParent)->setRemeshingScheme(DLFLWindow::DualGeneric1264);
}

void RemeshingMode::setupFourConversion(){
	
  mLinearVertexWidget = new QWidget;
  mCatmullClarkWidget = new QWidget;
  mStellateEdgeRemovalWidget = new QWidget;
  mDooSabinWidget = new QWidget;
	mCornerCuttingWidget = new QWidget;
  mSimplestWidget = new QWidget;

	//linear vertex insertion
	mLinearVertexLayout = new QBoxLayout(QBoxLayout::LeftToRight);
	mLinearVertexLayout->setMargin(0);
	QPushButton *linearVertexCreateButton = new QPushButton(tr("Perform Remeshing"), this);
	connect(linearVertexCreateButton, SIGNAL(clicked()),
					((MainWindow*)mParent),SLOT(perform_remeshing()) );
					
	mLinearVertexLayout->addWidget(linearVertexCreateButton);
	mLinearVertexLayout->addStretch(1);
	mLinearVertexWidget->setLayout(mLinearVertexLayout);

	//catmull clark
	mCatmullClarkLayout = new QBoxLayout(QBoxLayout::LeftToRight);
	mCatmullClarkLayout->setMargin(0);
	QPushButton *catmullClarkCreateButton = new QPushButton(tr("Perform Remeshing"), this);
	connect(catmullClarkCreateButton, SIGNAL(clicked()),
					((MainWindow*)mParent),SLOT(perform_remeshing()) );
					
	mCatmullClarkLayout->addWidget(catmullClarkCreateButton);
	mCatmullClarkLayout->addStretch(1);
	mCatmullClarkWidget->setLayout(mCatmullClarkLayout);
	
	//stellate with edge removal
	mStellateEdgeRemovalLayout = new QBoxLayout(QBoxLayout::LeftToRight);
	mStellateEdgeRemovalLayout->setMargin(0);
	QPushButton *stellateEdgeRemovalCreateButton = new QPushButton(tr("Perform Remeshing"), this);
	connect(stellateEdgeRemovalCreateButton, SIGNAL(clicked()),
					((MainWindow*)mParent),SLOT(perform_remeshing()) );
					
	mStellateEdgeRemovalLayout->addWidget(stellateEdgeRemovalCreateButton);
	mStellateEdgeRemovalLayout->addStretch(1);
	mStellateEdgeRemovalWidget->setLayout(mStellateEdgeRemovalLayout);
	
	//doo sabin
	mDooSabinLayout = new QBoxLayout(QBoxLayout::LeftToRight);
	mDooSabinLayout->setMargin(0);
	QCheckBox *dooSabinCheckBox = new QCheckBox(tr("Check for multiple edges"));
	mDooSabinLayout->addWidget(dooSabinCheckBox);
	//connect the checkbox
	connect(dooSabinCheckBox, SIGNAL(stateChanged(int)),
					((MainWindow*)mParent),SLOT(toggleDooSabinEdgeFlag(int)) );
	QPushButton *dooSabinCreateButton = new QPushButton(tr("Perform Remeshing"), this);
	connect(dooSabinCreateButton, SIGNAL(clicked()),
					((MainWindow*)mParent),SLOT(perform_remeshing()) );
	mDooSabinLayout->addWidget(dooSabinCreateButton);
	mDooSabinLayout->addStretch(1);
	mDooSabinWidget->setLayout(mDooSabinLayout);
	
	//corner cutting
	mCornerCuttingLayout = new QBoxLayout(QBoxLayout::LeftToRight);
	mCornerCuttingLayout->setMargin(0);
	QPushButton *cornerCuttingCreateButton = new QPushButton(tr("Perform Remeshing"), this);
	connect(cornerCuttingCreateButton, SIGNAL(clicked()),
					((MainWindow*)mParent),SLOT(perform_remeshing()) );
	mCornerCuttingLayout->addWidget(cornerCuttingCreateButton);
	mCornerCuttingLayout->addStretch(1);
	mCornerCuttingWidget->setLayout(mCornerCuttingLayout);
	
	//simplest
	mSimplestLayout = new QBoxLayout(QBoxLayout::LeftToRight);
	mSimplestLayout->setMargin(0);
	QPushButton *simplestCreateButton = new QPushButton(tr("Perform Remeshing"), this);
	connect(simplestCreateButton, SIGNAL(clicked()),
					((MainWindow*)mParent),SLOT(perform_remeshing()) );
	mSimplestLayout->addWidget(simplestCreateButton);
	mSimplestLayout->addStretch(1);
	mSimplestWidget->setLayout(mSimplestLayout);	
	
}

void RemeshingMode::triggerLinearVertex(){

	((MainWindow*)mParent)->setToolOptions(mLinearVertexWidget);
	((MainWindow*)mParent)->setRemeshingScheme(DLFLWindow::LinearVertexInsertion);
}

void RemeshingMode::triggerCatmullClark(){

	((MainWindow*)mParent)->setToolOptions(mCatmullClarkWidget);
	((MainWindow*)mParent)->setRemeshingScheme(DLFLWindow::CatmullClark);
}

void RemeshingMode::triggerStellateEdgeRemoval(){

	((MainWindow*)mParent)->setToolOptions(mStellateEdgeRemovalWidget);
	((MainWindow*)mParent)->setRemeshingScheme(DLFLWindow::ModifiedStellate);
}

void RemeshingMode::triggerDooSabin(){

	((MainWindow*)mParent)->setToolOptions(mDooSabinWidget);
	((MainWindow*)mParent)->setRemeshingScheme(DLFLWindow::DooSabin);
}

void RemeshingMode::triggerCornerCutting(){

	((MainWindow*)mParent)->setToolOptions(mCornerCuttingWidget);
	((MainWindow*)mParent)->setRemeshingScheme(DLFLWindow::CornerCutting);
}

void RemeshingMode::triggerSimplest(){

	((MainWindow*)mParent)->setToolOptions(mSimplestWidget);
	((MainWindow*)mParent)->setRemeshingScheme(DLFLWindow::Simplest);
}

void RemeshingMode::setupFiveConversion(){
	
	//create the stacked widget and all child widget pages
  mPentagonalizationWidget = new QWidget;
  mCubicPentagonalizationWidget = new QWidget;
  mDualPentagonalizationWidget = new QWidget;

	//pentagonalization
	mPentagonalizationLayout = new QBoxLayout(QBoxLayout::LeftToRight);
	mPentagonalizationLayout->setMargin(0);
	//offset spinbox
	QLabel *pentagonalizationLabel = new QLabel(tr("Offset:"));
	QDoubleSpinBox *pentagonalizationSpinBox = new QDoubleSpinBox;
	pentagonalizationSpinBox->setRange(0.0, 1.0);
	pentagonalizationSpinBox->setSingleStep(0.01);
	pentagonalizationSpinBox->setValue(0.0);
	pentagonalizationSpinBox->setDecimals(2);
	connect(pentagonalizationSpinBox, SIGNAL(valueChanged(double)),
					((MainWindow*)mParent),SLOT(changePentagonalOffset(double)) );
	
	mPentagonalizationLayout->addWidget(pentagonalizationLabel);
  mPentagonalizationLayout->addWidget(pentagonalizationSpinBox);
	QPushButton *pentagonalizationCreateButton = new QPushButton(tr("Perform Remeshing"), this);
	connect(pentagonalizationCreateButton, SIGNAL(clicked()),
					((MainWindow*)mParent),SLOT(perform_remeshing()) );
					
	mPentagonalizationLayout->addWidget(pentagonalizationCreateButton);
	mPentagonalizationLayout->addStretch(1);
	mPentagonalizationWidget->setLayout(mPentagonalizationLayout);

	//cubic Pentagonalization
	mCubicPentagonalizationLayout = new QBoxLayout(QBoxLayout::LeftToRight);
	mCubicPentagonalizationLayout->setMargin(0);
	//offset spinbox
	QLabel *cubicPentagonalizationLabel = new QLabel(tr("Offset:"));
	QDoubleSpinBox *cubicPentagonalizationSpinBox = new QDoubleSpinBox;
	cubicPentagonalizationSpinBox->setRange(0.0, 1.0);
	cubicPentagonalizationSpinBox->setSingleStep(0.01);
	cubicPentagonalizationSpinBox->setValue(0.0);
	cubicPentagonalizationSpinBox->setDecimals(2);
	connect(cubicPentagonalizationSpinBox, SIGNAL(valueChanged(double)),
					((MainWindow*)mParent),SLOT(changePentagonalOffset(double)) );
					
	mCubicPentagonalizationLayout->addWidget(cubicPentagonalizationLabel);
  mCubicPentagonalizationLayout->addWidget(cubicPentagonalizationSpinBox);
	QPushButton *cubicPentagonalizationCreateButton = new QPushButton(tr("Perform Remeshing"), this);
	connect(cubicPentagonalizationCreateButton, SIGNAL(clicked()),
					((MainWindow*)mParent),SLOT(perform_remeshing()) );
					
	mCubicPentagonalizationLayout->addWidget(cubicPentagonalizationCreateButton);
	mCubicPentagonalizationLayout->addStretch(1);
	mCubicPentagonalizationWidget->setLayout(mCubicPentagonalizationLayout);
	
	//dual pentagonalization
	mDualPentagonalizationLayout = new QBoxLayout(QBoxLayout::LeftToRight);
	mDualPentagonalizationLayout->setMargin(0);
	QPushButton *dualPentagonalizationCreateButton = new QPushButton(tr("Perform Remeshing"), this);
	connect(dualPentagonalizationCreateButton, SIGNAL(clicked()),
					((MainWindow*)mParent),SLOT(perform_remeshing()) );
	mDualPentagonalizationLayout->addWidget(dualPentagonalizationCreateButton);
	mDualPentagonalizationLayout->addStretch(1);
	mDualPentagonalizationWidget->setLayout(mDualPentagonalizationLayout);

}

void RemeshingMode::triggerPentagonalization(){

	((MainWindow*)mParent)->setToolOptions(mPentagonalizationWidget);
	((MainWindow*)mParent)->setRemeshingScheme(DLFLWindow::Pentagonal);
}

void RemeshingMode::triggerCubicPentagonalization(){

	((MainWindow*)mParent)->setToolOptions(mCubicPentagonalizationWidget);
	((MainWindow*)mParent)->setRemeshingScheme(DLFLWindow::CubicPentagonal);
}

void RemeshingMode::triggerDualPentagonalization(){

	((MainWindow*)mParent)->setToolOptions(mDualPentagonalizationWidget);
	((MainWindow*)mParent)->setRemeshingScheme(DLFLWindow::DualPentagonal);
}

void RemeshingMode::setupThreePreservation(){
	
	//create the stacked widget and all child widget pages
  mLoopStyleRemeshingWidget = new QWidget;
  mLoopSubdivisionWidget = new QWidget;
  mDualLoopStyleRemeshingWidget = new QWidget;
  mDualLoopSubdivisionWidget = new QWidget;

	//loop-style remeshing
	mLoopStyleRemeshingLayout = new QBoxLayout(QBoxLayout::LeftToRight);
	mLoopStyleRemeshingLayout->setMargin(0);
	//offset spinbox
	QLabel *loopStyleRemeshingLabel = new QLabel(tr("Length:"));
	QDoubleSpinBox *loopStyleRemeshingSpinBox = new QDoubleSpinBox;
	loopStyleRemeshingSpinBox->setRange(0.0, 1.0);
	loopStyleRemeshingSpinBox->setSingleStep(0.01);
	loopStyleRemeshingSpinBox->setValue(1.0);
	loopStyleRemeshingSpinBox->setDecimals(2);
	connect(loopStyleRemeshingSpinBox, SIGNAL(valueChanged(double)),
					((MainWindow*)mParent),SLOT(changeLoopStyleSubdivisionLength(double)) );
	
	mLoopStyleRemeshingLayout->addWidget(loopStyleRemeshingLabel);
  mLoopStyleRemeshingLayout->addWidget(loopStyleRemeshingSpinBox);
	QPushButton *loopStyleRemeshingButton = new QPushButton(tr("Perform Remeshing"), this);
	connect(loopStyleRemeshingButton, SIGNAL(clicked()),
					((MainWindow*)mParent),SLOT(perform_remeshing()) );
	
	mLoopStyleRemeshingLayout->addWidget(loopStyleRemeshingButton);
	mLoopStyleRemeshingLayout->addStretch(1);
	mLoopStyleRemeshingWidget->setLayout(mLoopStyleRemeshingLayout);

	//loop subdivision
	mLoopSubdivisionLayout = new QBoxLayout(QBoxLayout::LeftToRight);
	mLoopSubdivisionLayout->setMargin(0);
	QPushButton *loopSubdivisionCreateButton = new QPushButton(tr("Perform Remeshing"), this);
	connect(loopSubdivisionCreateButton, SIGNAL(clicked()),
					((MainWindow*)mParent),SLOT(perform_remeshing()) );
	mLoopSubdivisionLayout->addWidget(loopSubdivisionCreateButton);
	mLoopSubdivisionLayout->addStretch(1);
	mLoopSubdivisionWidget->setLayout(mLoopSubdivisionLayout);
	
	//dual of loop-style remeshing
	mDualLoopStyleRemeshingLayout = new QBoxLayout(QBoxLayout::LeftToRight);
	mDualLoopStyleRemeshingLayout->setMargin(0);
	
	QLabel *dualLoopStyleRemeshingTwistLabel = new QLabel(tr("Twist:"));
	QDoubleSpinBox *dualLoopStyleRemeshingTwistSpinBox = new QDoubleSpinBox;
	dualLoopStyleRemeshingTwistSpinBox->setRange(0.0, 1.0);
	dualLoopStyleRemeshingTwistSpinBox->setSingleStep(0.01);
	dualLoopStyleRemeshingTwistSpinBox->setValue(0.0);
	dualLoopStyleRemeshingTwistSpinBox->setDecimals(2);
	connect(dualLoopStyleRemeshingTwistSpinBox, SIGNAL(valueChanged(double)),
					((MainWindow*)mParent),SLOT(changeRoot4Twist(double)) );	
	
	mDualLoopStyleRemeshingLayout->addWidget(dualLoopStyleRemeshingTwistLabel);
  mDualLoopStyleRemeshingLayout->addWidget(dualLoopStyleRemeshingTwistSpinBox);

	QLabel *dualLoopStyleRemeshingWeightLabel = new QLabel(tr("Weight:"));
	QDoubleSpinBox *dualLoopStyleRemeshingWeightSpinBox = new QDoubleSpinBox;
	dualLoopStyleRemeshingWeightSpinBox->setRange(0.0, 1.0);
	dualLoopStyleRemeshingWeightSpinBox->setSingleStep(0.01);
	dualLoopStyleRemeshingWeightSpinBox->setValue(0.0);
	dualLoopStyleRemeshingWeightSpinBox->setDecimals(2);
	connect(dualLoopStyleRemeshingWeightSpinBox, SIGNAL(valueChanged(double)),
					((MainWindow*)mParent),SLOT(changeRoot4Weight(double)) );	
	
	mDualLoopStyleRemeshingLayout->addWidget(dualLoopStyleRemeshingWeightLabel);
  mDualLoopStyleRemeshingLayout->addWidget(dualLoopStyleRemeshingWeightSpinBox);

	QPushButton *dualLoopStyleRemeshingButton = new QPushButton(tr("Perform Remeshing"), this);
	connect(dualLoopStyleRemeshingButton, SIGNAL(clicked()),
					((MainWindow*)mParent),SLOT(perform_remeshing()) );
					
	mDualLoopStyleRemeshingLayout->addWidget(dualLoopStyleRemeshingButton);
	mDualLoopStyleRemeshingLayout->addStretch(1);
	mDualLoopStyleRemeshingWidget->setLayout(mDualLoopStyleRemeshingLayout);
	
	//dual of loop subdivision
	mDualLoopSubdivisionLayout = new QBoxLayout(QBoxLayout::LeftToRight);
	mDualLoopSubdivisionLayout->setMargin(0);
	QPushButton *dualLoopSubdivisionCreateButton = new QPushButton(tr("Perform Remeshing"), this);
	connect(dualLoopSubdivisionCreateButton, SIGNAL(clicked()),
					((MainWindow*)mParent),SLOT(perform_remeshing()) );
					
	mDualLoopSubdivisionLayout->addWidget(dualLoopSubdivisionCreateButton);
	mDualLoopSubdivisionLayout->addStretch(1);
	mDualLoopSubdivisionWidget->setLayout(mDualLoopSubdivisionLayout);

}

void RemeshingMode::triggerLoopStyleRemeshing(){

	((MainWindow*)mParent)->setToolOptions(mLoopStyleRemeshingWidget);
	((MainWindow*)mParent)->setRemeshingScheme(DLFLWindow::LoopStyle);
}

void RemeshingMode::triggerLoopSubdivision(){

	((MainWindow*)mParent)->setToolOptions(mLoopSubdivisionWidget);
	((MainWindow*)mParent)->setRemeshingScheme(DLFLWindow::Loop);
}

void RemeshingMode::triggerDualLoopStyleRemeshing(){

	((MainWindow*)mParent)->setToolOptions(mDualLoopStyleRemeshingWidget);
	((MainWindow*)mParent)->setRemeshingScheme(DLFLWindow::Root4);
}

void RemeshingMode::triggerDualLoopSubdivision(){

	((MainWindow*)mParent)->setToolOptions(mDualLoopSubdivisionWidget);
	((MainWindow*)mParent)->setRemeshingScheme(DLFLWindow::DualLoop);
}

void RemeshingMode::setupFourPreservation(){
	
  mGlobalExtrudeWidget = new QWidget;
  mCheckerboardWidget = new QWidget;
  mDualGlobalExtrudeWidget = new QWidget;
  mDualCheckerboardWidget = new QWidget;

	//global extrude
	mGlobalExtrudeLayout = new QBoxLayout(QBoxLayout::LeftToRight);
	mGlobalExtrudeLayout->setMargin(0);
	QPushButton *globalExtrudeButton = new QPushButton(tr("Perform Remeshing"), this);
	connect(globalExtrudeButton, SIGNAL(clicked()),
					((MainWindow*)mParent),SLOT(perform_remeshing()) );
					
	mGlobalExtrudeLayout->addWidget(globalExtrudeButton);
	mGlobalExtrudeLayout->addStretch(1);
	mGlobalExtrudeWidget->setLayout(mGlobalExtrudeLayout);

	//checkerboard
	mCheckerboardLayout = new QBoxLayout(QBoxLayout::LeftToRight);
	mCheckerboardLayout->setMargin(0);
	QPushButton *checkerboardCreateButton = new QPushButton(tr("Perform Remeshing"), this);
	connect(checkerboardCreateButton, SIGNAL(clicked()),
					((MainWindow*)mParent),SLOT(perform_remeshing()) );
					
	mCheckerboardLayout->addWidget(checkerboardCreateButton);
	mCheckerboardLayout->addStretch(1);
	mCheckerboardWidget->setLayout(mCheckerboardLayout);
	
	//dual of global extrude
	mDualGlobalExtrudeLayout = new QBoxLayout(QBoxLayout::LeftToRight);
	mDualGlobalExtrudeLayout->setMargin(0);
	QPushButton *dualGlobalExtrudeButton = new QPushButton(tr("Perform Remeshing"), this);
	connect(dualGlobalExtrudeButton, SIGNAL(clicked()),
					((MainWindow*)mParent),SLOT(perform_remeshing()) );
					
	mDualGlobalExtrudeLayout->addWidget(dualGlobalExtrudeButton);
	mDualGlobalExtrudeLayout->addStretch(1);
	mDualGlobalExtrudeWidget->setLayout(mDualGlobalExtrudeLayout);
	
	//dual of checkerboard remeshing
	mDualCheckerboardLayout = new QBoxLayout(QBoxLayout::LeftToRight);
	mDualCheckerboardLayout->setMargin(0);
	QPushButton *dualCheckerboardCreateButton = new QPushButton(tr("Perform Remeshing"), this);
	connect(dualCheckerboardCreateButton, SIGNAL(clicked()),
					((MainWindow*)mParent),SLOT(perform_remeshing()) );
					
	mDualCheckerboardLayout->addWidget(dualCheckerboardCreateButton);
	mDualCheckerboardLayout->addStretch(1);
	mDualCheckerboardWidget->setLayout(mDualCheckerboardLayout);
	
}

void RemeshingMode::triggerGlobalExtrude(){

	((MainWindow*)mParent)->setToolOptions(mGlobalExtrudeWidget);
	((MainWindow*)mParent)->setRemeshingScheme(DLFLWindow::GlobalCubicExtrude);
}

void RemeshingMode::triggerCheckerboard(){

	((MainWindow*)mParent)->setToolOptions(mCheckerboardWidget);
	((MainWindow*)mParent)->setRemeshingScheme(DLFLWindow::CheckerBoard);
}

void RemeshingMode::triggerDualGlobalExtrude(){

	((MainWindow*)mParent)->setToolOptions(mDualGlobalExtrudeWidget);
	((MainWindow*)mParent)->setRemeshingScheme(DLFLWindow::DualGlobalCubicExtrude);
}

void RemeshingMode::triggerDualCheckerboard(){

	((MainWindow*)mParent)->setToolOptions(mDualCheckerboardWidget);
	((MainWindow*)mParent)->setRemeshingScheme(DLFLWindow::DualCheckerBoard);
}

void RemeshingMode::setupFivePreservation(){

	//create the stacked widget and all child widget pages
  mPentagonPreservingWidget = new QWidget;
  mDualPentagonPreservingWidget = new QWidget;

	//Pentagon Preserving
	mPentagonPreservingLayout = new QBoxLayout(QBoxLayout::LeftToRight);
	mPentagonPreservingLayout->setMargin(0);
	QLabel *pentagonPreservingLabel = new QLabel(tr("Scale Factor:"));
	QDoubleSpinBox *pentagonPreservingSpinBox = new QDoubleSpinBox;
	pentagonPreservingSpinBox->setRange(0.0, 1.0);
	pentagonPreservingSpinBox->setSingleStep(0.01);
	pentagonPreservingSpinBox->setValue(0.75);
	pentagonPreservingSpinBox->setDecimals(2);
	connect(pentagonPreservingSpinBox, SIGNAL(valueChanged(double)),
					((MainWindow*)mParent),SLOT(changePentagonalScaleFactor(double)) );	
	
	mPentagonPreservingLayout->addWidget(pentagonPreservingLabel);
  mPentagonPreservingLayout->addWidget(pentagonPreservingSpinBox);

	QPushButton *pentagonPreservingButton = new QPushButton(tr("Perform Remeshing"), this);
	connect(pentagonPreservingButton, SIGNAL(clicked()),
					((MainWindow*)mParent),SLOT(perform_remeshing()) );
					
	mPentagonPreservingLayout->addWidget(pentagonPreservingButton);
	mPentagonPreservingLayout->addStretch(1);
	mPentagonPreservingWidget->setLayout(mPentagonPreservingLayout);

	//dual of Pentagon Preserving
	mDualPentagonPreservingLayout = new QBoxLayout(QBoxLayout::LeftToRight);
	mDualPentagonPreservingLayout->setMargin(0);
	QPushButton *dualPentagonPreservingCreateButton = new QPushButton(tr("Perform Remeshing"), this);
	connect(dualPentagonPreservingCreateButton, SIGNAL(clicked()),
					((MainWindow*)mParent),SLOT(perform_remeshing()) );
					
	mDualPentagonPreservingLayout->addWidget(dualPentagonPreservingCreateButton);
	mDualPentagonPreservingLayout->addStretch(1);
	mDualPentagonPreservingWidget->setLayout(mDualPentagonPreservingLayout);
	
}

void RemeshingMode::triggerPentagonPreserving(){

	((MainWindow*)mParent)->setToolOptions(mPentagonPreservingWidget);
	((MainWindow*)mParent)->setRemeshingScheme(DLFLWindow::PentagonPreserving);
}

void RemeshingMode::triggerDualPentagonPreserving(){

	((MainWindow*)mParent)->setToolOptions(mDualPentagonPreservingWidget);
	((MainWindow*)mParent)->setRemeshingScheme(DLFLWindow::DualPentagonPreserving);
}

void RemeshingMode::setupSixPreservation(){
	
  mDualLoopStyleRemeshingSixWidget = new QWidget;
  mLoopStyleRemeshingSixWidget = new QWidget;

	//dual of loop-style remeshing
	mDualLoopStyleRemeshingSixLayout = new QBoxLayout(QBoxLayout::LeftToRight);
	mDualLoopStyleRemeshingSixLayout->setMargin(0);
	
	QLabel *dualLoopStyleRemeshingSixTwistLabel = new QLabel(tr("Twist:"));
	QDoubleSpinBox *dualLoopStyleRemeshingSixTwistSpinBox = new QDoubleSpinBox;
	dualLoopStyleRemeshingSixTwistSpinBox->setRange(0.0, 1.0);
	dualLoopStyleRemeshingSixTwistSpinBox->setSingleStep(0.01);
	dualLoopStyleRemeshingSixTwistSpinBox->setValue(0.0);
	dualLoopStyleRemeshingSixTwistSpinBox->setDecimals(2);
	connect(dualLoopStyleRemeshingSixTwistSpinBox, SIGNAL(valueChanged(double)),
					((MainWindow*)mParent),SLOT(changeRoot4Twist(double)) );	
	
	mDualLoopStyleRemeshingSixLayout->addWidget(dualLoopStyleRemeshingSixTwistLabel);
  mDualLoopStyleRemeshingSixLayout->addWidget(dualLoopStyleRemeshingSixTwistSpinBox);

	QLabel *dualLoopStyleRemeshingSixWeightLabel = new QLabel(tr("Weight:"));
	QDoubleSpinBox *dualLoopStyleRemeshingSixWeightSpinBox = new QDoubleSpinBox;
	dualLoopStyleRemeshingSixWeightSpinBox->setRange(0.0, 1.0);
	dualLoopStyleRemeshingSixWeightSpinBox->setSingleStep(0.01);
	dualLoopStyleRemeshingSixWeightSpinBox->setValue(0.0);
	dualLoopStyleRemeshingSixWeightSpinBox->setDecimals(2);
	connect(dualLoopStyleRemeshingSixWeightSpinBox, SIGNAL(valueChanged(double)),
					((MainWindow*)mParent),SLOT(changeRoot4Weight(double)) );	
	
	mDualLoopStyleRemeshingSixLayout->addWidget(dualLoopStyleRemeshingSixWeightLabel);
  mDualLoopStyleRemeshingSixLayout->addWidget(dualLoopStyleRemeshingSixWeightSpinBox);

	QPushButton *dualLoopStyleRemeshingSixButton = new QPushButton(tr("Perform Remeshing"), this);
	connect(dualLoopStyleRemeshingSixButton, SIGNAL(clicked()),
					((MainWindow*)mParent),SLOT(perform_remeshing()) );
					
	mDualLoopStyleRemeshingSixLayout->addWidget(dualLoopStyleRemeshingSixButton);
	mDualLoopStyleRemeshingSixLayout->addStretch(1);
	mDualLoopStyleRemeshingSixWidget->setLayout(mDualLoopStyleRemeshingSixLayout);
	
	//loop-style remeshing
	mLoopStyleRemeshingSixLayout = new QBoxLayout(QBoxLayout::LeftToRight);
	mLoopStyleRemeshingSixLayout->setMargin(0);

	QLabel *loopStyleRemeshingSixLabel = new QLabel(tr("Length:"));
	QDoubleSpinBox *loopStyleRemeshingSixSpinBox = new QDoubleSpinBox;
	loopStyleRemeshingSixSpinBox->setRange(0.0, 1.0);
	loopStyleRemeshingSixSpinBox->setSingleStep(0.01);
	loopStyleRemeshingSixSpinBox->setValue(1.0);
	loopStyleRemeshingSixSpinBox->setDecimals(2);
	connect(loopStyleRemeshingSixSpinBox, SIGNAL(valueChanged(double)),
					((MainWindow*)mParent),SLOT(changeLoopStyleSubdivisionLength(double)) );	
	
	mLoopStyleRemeshingSixLayout->addWidget(loopStyleRemeshingSixLabel);
  mLoopStyleRemeshingSixLayout->addWidget(loopStyleRemeshingSixSpinBox);
	QPushButton *loopStyleRemeshingSixButton = new QPushButton(tr("Perform Remeshing"), this);
	connect(loopStyleRemeshingSixButton, SIGNAL(clicked()),
					((MainWindow*)mParent),SLOT(perform_remeshing()) );
					
	mLoopStyleRemeshingSixLayout->addWidget(loopStyleRemeshingSixButton);
	mLoopStyleRemeshingSixLayout->addStretch(1);
	mLoopStyleRemeshingSixWidget->setLayout(mLoopStyleRemeshingSixLayout);

}

void RemeshingMode::triggerDualLoopStyleRemeshingSix(){

	((MainWindow*)mParent)->setToolOptions(mDualLoopStyleRemeshingSixWidget);
	((MainWindow*)mParent)->setRemeshingScheme(DLFLWindow::HexagonPreserving);
}

void RemeshingMode::triggerLoopStyleRemeshingSixWidget(){

	((MainWindow*)mParent)->setToolOptions(mLoopStyleRemeshingSixWidget);
	((MainWindow*)mParent)->setRemeshingScheme(DLFLWindow::DualHexagonPreserving);
}

void RemeshingMode::setupMiscellaneous(){
	
  mFractalWidget = new QWidget;
  mDoubleStellateMiscWidget = new QWidget;
  mDooSabinBCWidget = new QWidget;
  mDooSabinBCNewWidget = new QWidget;
  mDomeWidget = new QWidget;
  mSubdivideFaceWidget = new QWidget;

	//Fractal
	mFractalLayout = new QBoxLayout(QBoxLayout::LeftToRight);
	mFractalLayout->setMargin(0);
	
	QLabel *fractalMultiplierLabel = new QLabel(tr("Multiplier:"));
	QDoubleSpinBox *fractalMultiplierSpinBox = new QDoubleSpinBox;
	fractalMultiplierSpinBox->setRange(0.0, 2.0);
	fractalMultiplierSpinBox->setSingleStep(0.01);
	fractalMultiplierSpinBox->setValue(1.0);
	fractalMultiplierSpinBox->setDecimals(2);
	connect(fractalMultiplierSpinBox, SIGNAL(valueChanged(double)),
					((MainWindow*)mParent),SLOT(changeFractalOffset(double)) );	
	
	mFractalLayout->addWidget(fractalMultiplierLabel);
  mFractalLayout->addWidget(fractalMultiplierSpinBox);

	QPushButton *fractalButton = new QPushButton(tr("Perform Remeshing"), this);
	connect(fractalButton, SIGNAL(clicked()),
					((MainWindow*)mParent),SLOT(perform_remeshing()) );
					
	mFractalLayout->addWidget(fractalButton);
	mFractalLayout->addStretch(1);
	mFractalWidget->setLayout(mFractalLayout);
	
	//double stellate with Edge Removal
	mDoubleStellateMiscLayout = new QBoxLayout(QBoxLayout::LeftToRight);
	mDoubleStellateMiscLayout->setMargin(0);

	QLabel *doubleStellateMiscHeightLabel = new QLabel(tr("Height:"));
	QDoubleSpinBox *doubleStellateMiscHeightSpinBox = new QDoubleSpinBox;
	doubleStellateMiscHeightSpinBox->setRange(-1.0, 1.0);
	doubleStellateMiscHeightSpinBox->setSingleStep(0.01);
	doubleStellateMiscHeightSpinBox->setValue(0.0);
	doubleStellateMiscHeightSpinBox->setDecimals(2);
	connect(doubleStellateMiscHeightSpinBox, SIGNAL(valueChanged(double)),
					((MainWindow*)mParent),SLOT(changeSubStellateAmount(double)) );	
	
	mDoubleStellateMiscLayout->addWidget(doubleStellateMiscHeightLabel);
  mDoubleStellateMiscLayout->addWidget(doubleStellateMiscHeightSpinBox);

	QLabel *doubleStellateMiscCurveLabel = new QLabel(tr("Curve:"));
	QDoubleSpinBox *doubleStellateMiscCurveSpinBox = new QDoubleSpinBox;
	doubleStellateMiscCurveSpinBox->setRange(-1.0, 1.0);
	doubleStellateMiscCurveSpinBox->setSingleStep(0.01);
	doubleStellateMiscCurveSpinBox->setValue(0.0);
	doubleStellateMiscCurveSpinBox->setDecimals(2);
	connect(doubleStellateMiscCurveSpinBox, SIGNAL(valueChanged(double)),
					((MainWindow*)mParent),SLOT(changeSubStellateCurve(double)) );	
	
	mDoubleStellateMiscLayout->addWidget(doubleStellateMiscCurveLabel);
  mDoubleStellateMiscLayout->addWidget(doubleStellateMiscCurveSpinBox);

	QPushButton *doubleStellateMiscButton = new QPushButton(tr("Perform Remeshing"), this);
	connect(doubleStellateMiscButton, SIGNAL(clicked()),
					((MainWindow*)mParent),SLOT(perform_remeshing()) );
					
	mDoubleStellateMiscLayout->addWidget(doubleStellateMiscButton);
	mDoubleStellateMiscLayout->addStretch(1);
	mDoubleStellateMiscWidget->setLayout(mDoubleStellateMiscLayout);
	
	//doo sabin bc
	mDooSabinBCLayout = new QBoxLayout(QBoxLayout::LeftToRight);
	mDooSabinBCLayout->setMargin(0);
	QPushButton *dooSabinBCButton = new QPushButton(tr("Perform Remeshing"), this);
	connect(dooSabinBCButton, SIGNAL(clicked()),
					((MainWindow*)mParent),SLOT(perform_remeshing()) );
					
	mDooSabinBCLayout->addWidget(dooSabinBCButton);
	mDooSabinBCLayout->addStretch(1);
	mDooSabinBCWidget->setLayout(mDooSabinBCLayout);
	
	//doo sabin bc new
	mDooSabinBCNewLayout = new QBoxLayout(QBoxLayout::LeftToRight);
	mDooSabinBCNewLayout->setMargin(0);

	QLabel *dooSabinBCNewScaleLabel = new QLabel(tr("Scale:"));
	QDoubleSpinBox *dooSabinBCNewScaleSpinBox = new QDoubleSpinBox;
	dooSabinBCNewScaleSpinBox->setRange(0.0,2.0);
	dooSabinBCNewScaleSpinBox->setSingleStep(0.01);
	dooSabinBCNewScaleSpinBox->setValue(1.0);
	dooSabinBCNewScaleSpinBox->setDecimals(2);
	connect(dooSabinBCNewScaleSpinBox, SIGNAL(valueChanged(double)),
					((MainWindow*)mParent),SLOT(changeDooSabinBCnewScale(double)) );	
	
	mDooSabinBCNewLayout->addWidget(dooSabinBCNewScaleLabel);
  mDooSabinBCNewLayout->addWidget(dooSabinBCNewScaleSpinBox);

	QLabel *dooSabinBCNewLengthLabel = new QLabel(tr("Length:"));
	QDoubleSpinBox *dooSabinBCNewLengthSpinBox = new QDoubleSpinBox;
	dooSabinBCNewLengthSpinBox->setRange(0.0, 1.0);
	dooSabinBCNewLengthSpinBox->setSingleStep(0.01);
	dooSabinBCNewLengthSpinBox->setValue(1.0);
	dooSabinBCNewLengthSpinBox->setDecimals(2);	
	connect(dooSabinBCNewLengthSpinBox, SIGNAL(valueChanged(double)),
					((MainWindow*)mParent),SLOT(changeDooSabinBCnewLength(double)) );	
	
	mDooSabinBCNewLayout->addWidget(dooSabinBCNewLengthLabel);
  mDooSabinBCNewLayout->addWidget(dooSabinBCNewLengthSpinBox);

	QPushButton *dooSabinBCNewButton = new QPushButton(tr("Perform Remeshing"), this);
	connect(dooSabinBCNewButton, SIGNAL(clicked()),
					((MainWindow*)mParent),SLOT(perform_remeshing()) );
					
	mDooSabinBCNewLayout->addWidget(dooSabinBCNewButton);
	mDooSabinBCNewLayout->addStretch(1);
	mDooSabinBCNewWidget->setLayout(mDooSabinBCNewLayout);
	
	//dome
	mDomeLayout = new QBoxLayout(QBoxLayout::LeftToRight);
	mDomeLayout->setMargin(0);

	QLabel *domeHeightLabel = new QLabel(tr("Height:"));
	QDoubleSpinBox *domeHeightSpinBox = new QDoubleSpinBox;
	domeHeightSpinBox->setRange(0.0, 2.0);
	domeHeightSpinBox->setSingleStep(0.01);
	domeHeightSpinBox->setValue(1.0);
	domeHeightSpinBox->setDecimals(2);
	connect(domeHeightSpinBox, SIGNAL(valueChanged(double)),
					((MainWindow*)mParent),SLOT(changeDomeSubdivisionLength(double)) );	
	
	mDomeLayout->addWidget(domeHeightLabel);
  mDomeLayout->addWidget(domeHeightSpinBox);

	QLabel *domeScaleLabel = new QLabel(tr("Scale:"));
	QDoubleSpinBox *domeScaleSpinBox = new QDoubleSpinBox;
	domeScaleSpinBox->setRange(0.0, 2.0);
	domeScaleSpinBox->setSingleStep(0.01);
	domeScaleSpinBox->setValue(1.0);
	domeScaleSpinBox->setDecimals(2);
	connect(domeScaleSpinBox, SIGNAL(valueChanged(double)),
					((MainWindow*)mParent),SLOT(changeDomeSubdivisionScale(double)) );	
	
	mDomeLayout->addWidget(domeScaleLabel);
  mDomeLayout->addWidget(domeScaleSpinBox);

	QPushButton *domeButton = new QPushButton(tr("Perform Remeshing"), this);
	connect(domeButton, SIGNAL(clicked()),
					((MainWindow*)mParent),SLOT(perform_remeshing()) );
	mDomeLayout->addWidget(domeButton);
	mDomeLayout->addStretch(1);
	mDomeWidget->setLayout(mDomeLayout);
	
	//subdivide face
	mSubdivideFaceLayout = new QBoxLayout(QBoxLayout::LeftToRight);
	mSubdivideFaceLayout->setMargin(0);
	QCheckBox *subdivideFaceCheckBox = new QCheckBox(tr("Use Quads (off -> triangles)"));
	connect(subdivideFaceCheckBox, SIGNAL(stateChanged(int)),
					((MainWindow*)mParent),SLOT(toggleUseQuadsFlag(int)) );

	mSubdivideFaceLayout->addWidget(subdivideFaceCheckBox);
	
	subdivideFaceCheckBox->setChecked(true);
	QPushButton *subdivideFaceButton = new QPushButton(tr("Subdivide Selected Faces"), this);
	connect(subdivideFaceButton, SIGNAL(clicked()),
					((MainWindow*)mParent),SLOT(subdivide_face()) );
					
	mSubdivideFaceLayout->addWidget(subdivideFaceButton);
	mSubdivideFaceLayout->addStretch(1);
	mSubdivideFaceWidget->setLayout(mSubdivideFaceLayout);
	
}

void RemeshingMode::triggerFractal(){

	((MainWindow*)mParent)->setToolOptions(mFractalWidget);
	((MainWindow*)mParent)->setRemeshingScheme(DLFLWindow::Fractal);
}

void RemeshingMode::triggerDoubleStellateMisc(){

	((MainWindow*)mParent)->setToolOptions(mDoubleStellateMiscWidget);
	((MainWindow*)mParent)->setRemeshingScheme(DLFLWindow::ModifiedDoubleStellate);
}

void RemeshingMode::triggerDooSabinBC(){

	((MainWindow*)mParent)->setToolOptions(mDooSabinBCWidget);
	((MainWindow*)mParent)->setRemeshingScheme(DLFLWindow::DooSabinBC);
}

void RemeshingMode::triggerDooSabinBCNew(){

	((MainWindow*)mParent)->setToolOptions(mDooSabinBCNewWidget);
	((MainWindow*)mParent)->setRemeshingScheme(DLFLWindow::DooSabinBCNew);
}

void RemeshingMode::triggerDome(){

	((MainWindow*)mParent)->setToolOptions(mDomeWidget);
	((MainWindow*)mParent)->setRemeshingScheme(DLFLWindow::Dome);
}

void RemeshingMode::triggerSubdivideFace(){

	((MainWindow*)mParent)->setToolOptions(mSubdivideFaceWidget);
	((MainWindow*)mParent)->setMode(DLFLWindow::SubDivideFace);
}
