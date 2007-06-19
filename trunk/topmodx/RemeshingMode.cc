/****************************************************************************
 **
 **
 ****************************************************************************/
#include <QtGui>

#include "RemeshingMode.hh"

RemeshingMode::RemeshingMode(QWidget *parent, QShortcutManager *sm, QWidget *actionList)
		: QWidget(parent)
{		
	setParent(0);
	mParent = parent;
	
	((MainWindow*)mParent)->setRemeshingScheme(MainWindow::Dual);
	
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
	
	mDualAction = new QAction(QIcon(":images/doosabin_extrude.png"),tr("Dual"),this);
	mDualAction->setCheckable(true);
	sm->registerAction(mDualAction, "Remeshing", "");
	mDualAction->setStatusTip(tr("Enter Dual Remeshing Mode"));
	mDualAction->setToolTip(tr("Dual Remeshing Mode"));
	connect(mDualAction, SIGNAL(triggered()), this, SLOT(triggerDual()));
	actionList->addAction(mDualAction);

	mRootThreeAction = new QAction(QIcon(":images/doosabin_extrude.png"),tr("Root-3"),this);
	mRootThreeAction->setCheckable(true);
	sm->registerAction(mRootThreeAction, "Remeshing", "");
	mRootThreeAction->setStatusTip(tr("Enter Root-3 Remeshing Mode"));
	mRootThreeAction->setToolTip(tr("Root-3 Remeshing Mode"));
	connect(mRootThreeAction, SIGNAL(triggered()), this, SLOT(triggerRootThree()));
	actionList->addAction(mRootThreeAction);

	mDualVertexTruncationAction = new QAction(QIcon(":images/doosabin_extrude.png"),tr("Dual Vertex Truncation"),this);
	mDualVertexTruncationAction->setCheckable(true);
	sm->registerAction(mDualVertexTruncationAction, "Remeshing", "");
	mDualVertexTruncationAction->setStatusTip(tr("Enter Dual Vertex Truncation Remeshing Mode"));
	mDualVertexTruncationAction->setToolTip(tr("Dual Vertex Truncation Remeshing Mode"));
	connect(mDualVertexTruncationAction, SIGNAL(triggered()), this, SLOT(triggerDualVertexTruncation()));
	actionList->addAction(mDualVertexTruncationAction);

	mStellationAction = new QAction(QIcon(":images/doosabin_extrude.png"),tr("Stellate"),this);
	mStellationAction->setCheckable(true);
	sm->registerAction(mStellationAction, "Remeshing", "");
	mStellationAction->setStatusTip(tr("Enter Stellate Remeshing Mode"));
	mStellationAction->setToolTip(tr("Stellate Remeshing Mode"));
	connect(mStellationAction, SIGNAL(triggered()), this, SLOT(triggerStellation()));
	actionList->addAction(mStellationAction);

	mDoubleStellationAction = new QAction(QIcon(":images/doosabin_extrude.png"),tr("Double Stellate"),this);
	mDoubleStellationAction->setCheckable(true);
	sm->registerAction(mDoubleStellationAction, "Remeshing", "");
	mDoubleStellationAction->setStatusTip(tr("Enter Double Stellate Remeshing Mode"));
	mDoubleStellationAction->setToolTip(tr("Double Stellate Remeshing Mode"));
	connect(mDoubleStellationAction, SIGNAL(triggered()), this, SLOT(triggerDoubleStellation()));
	actionList->addAction(mDoubleStellationAction);

	mTwelveSixFourAction = new QAction(QIcon(":images/doosabin_extrude.png"),tr("12.6.4"),this);
	mTwelveSixFourAction->setCheckable(true);
	sm->registerAction(mTwelveSixFourAction, "Remeshing", "");
	mTwelveSixFourAction->setStatusTip(tr("Enter 12.6.4 Remeshing Mode"));
	mTwelveSixFourAction->setToolTip(tr("12.6.4 Remeshing Mode"));
	connect(mTwelveSixFourAction, SIGNAL(triggered()), this, SLOT(triggerTwelveSixFour()));	
	actionList->addAction(mTwelveSixFourAction);

	mHoneycombAction = new QAction(QIcon(":images/doosabin_extrude.png"),tr("Honeycomb"),this);
	mHoneycombAction->setCheckable(true);
	sm->registerAction(mHoneycombAction, "Remeshing", "");
	mHoneycombAction->setStatusTip(tr("Enter Honeycomb Remeshing Mode"));
	mHoneycombAction->setToolTip(tr("Honeycomb Remeshing Mode"));
	connect(mHoneycombAction, SIGNAL(triggered()), this, SLOT(triggerHoneycomb()));	
	actionList->addAction(mHoneycombAction);
	
	mVertexTruncationAction = new QAction(QIcon(":images/doosabin_extrude.png"),tr("Vertex Truncation"),this);
	mVertexTruncationAction->setCheckable(true);
	sm->registerAction(mVertexTruncationAction, "Remeshing", "");
	mVertexTruncationAction->setStatusTip(tr("Enter Vertex Truncation Remeshing Mode"));
	mVertexTruncationAction->setToolTip(tr("Vertex Truncation Remeshing Mode"));
	connect(mVertexTruncationAction, SIGNAL(triggered()), this, SLOT(triggerVertexTruncation()));	
	actionList->addAction(mVertexTruncationAction);
	
	mDualTwelveSixFourAction = new QAction(QIcon(":images/doosabin_extrude.png"),tr("Dual 12.6.4"),this);
	mDualTwelveSixFourAction->setCheckable(true);
	sm->registerAction(mDualTwelveSixFourAction, "Remeshing", "");
	mDualTwelveSixFourAction->setStatusTip(tr("Enter Dual 12.6.4 Remeshing Mode"));
	mDualTwelveSixFourAction->setToolTip(tr("Dual 12.6.4 Remeshing Mode"));
	connect(mDualTwelveSixFourAction, SIGNAL(triggered()), this, SLOT(triggerDualTwelveSixFour()));	
	actionList->addAction(mDualTwelveSixFourAction);
	
	mLinearVertexAction = new QAction(QIcon(":images/doosabin_extrude.png"),tr("Linear Vertex Insertion"),this);
	mLinearVertexAction->setCheckable(true);
	sm->registerAction(mLinearVertexAction, "Remeshing", "");
	mLinearVertexAction->setStatusTip(tr("Enter Linear Vertex Insertion Remeshing Mode"));
	mLinearVertexAction->setToolTip(tr("Linear Vertex Insertion Remeshing Mode"));
	connect(mLinearVertexAction, SIGNAL(triggered()), this, SLOT(triggerLinearVertex()));	
	actionList->addAction(mLinearVertexAction);
	
	mCatmullClarkAction = new QAction(QIcon(":images/doosabin_extrude.png"),tr("Catmull Clark"),this);
	mCatmullClarkAction->setCheckable(true);
	sm->registerAction(mCatmullClarkAction, "Remeshing", "");
	mCatmullClarkAction->setStatusTip(tr("Enter Catmull Clark Remeshing Mode"));
	mCatmullClarkAction->setToolTip(tr("Catmull Clark Remeshing Mode"));
	connect(mCatmullClarkAction, SIGNAL(triggered()), this, SLOT(triggerCatmullClark()));	
	actionList->addAction(mCatmullClarkAction);
	
	mStellateEdgeRemovalAction = new QAction(QIcon(":images/doosabin_extrude.png"),tr("Stellate with Edge Removal"),this);
	mStellateEdgeRemovalAction->setCheckable(true);
	sm->registerAction(mStellateEdgeRemovalAction, "Remeshing", "");
	mStellateEdgeRemovalAction->setStatusTip(tr("Enter Stellate with Edge Removal Remeshing Mode"));
	mStellateEdgeRemovalAction->setToolTip(tr("Stellate with Edge Removal Remeshing Mode"));
	connect(mStellateEdgeRemovalAction, SIGNAL(triggered()), this, SLOT(triggerStellateEdgeRemoval()));	
	actionList->addAction(mStellateEdgeRemovalAction);
	
	mDooSabinAction = new QAction(QIcon(":images/doosabin_extrude.png"),tr("Doo Sabin"),this);
	mDooSabinAction->setCheckable(true);
	sm->registerAction(mDooSabinAction, "Remeshing", "");
	mDooSabinAction->setStatusTip(tr("Enter Doo Sabin Remeshing Mode"));
	mDooSabinAction->setToolTip(tr("Doo Sabin Remeshing Mode"));
	connect(mDooSabinAction, SIGNAL(triggered()), this, SLOT(triggerDooSabin()));	
	actionList->addAction(mDooSabinAction);
	
	mCornerCuttingAction = new QAction(QIcon(":images/doosabin_extrude.png"),tr("Corner Cutting"),this);
	mCornerCuttingAction->setCheckable(true);
	sm->registerAction(mCornerCuttingAction, "Remeshing", "");
	mCornerCuttingAction->setStatusTip(tr("Enter Corner Cutting Remeshing Mode"));
	mCornerCuttingAction->setToolTip(tr("Corner Cutting Remeshing Mode"));
	connect(mCornerCuttingAction, SIGNAL(triggered()), this, SLOT(triggerCornerCutting()));	
	actionList->addAction(mCornerCuttingAction);

	mSimplestAction = new QAction(QIcon(":images/doosabin_extrude.png"),tr("Simplest"),this);
	mSimplestAction->setCheckable(true);
	sm->registerAction(mSimplestAction, "Remeshing", "");
	mSimplestAction->setStatusTip(tr("Enter Simplest Remeshing Mode"));
	mSimplestAction->setToolTip(tr("Simplest Remeshing Mode"));
	connect(mSimplestAction, SIGNAL(triggered()), this, SLOT(triggerSimplest()));	
	actionList->addAction(mSimplestAction);

	mPentagonalizationAction = new QAction(QIcon(":images/doosabin_extrude.png"),tr("Pentagonalization"),this);
	mPentagonalizationAction->setCheckable(true);
	sm->registerAction(mPentagonalizationAction, "Remeshing", "");
	mPentagonalizationAction->setStatusTip(tr("Enter Pentagonalization Remeshing Mode"));
	mPentagonalizationAction->setToolTip(tr("Pentagonalization Remeshing Mode"));
	connect(mPentagonalizationAction, SIGNAL(triggered()), this, SLOT(triggerPentagonalization()));	
	actionList->addAction(mPentagonalizationAction);

	mCubicPentagonalizationAction = new QAction(QIcon(":images/doosabin_extrude.png"),tr("Cubic Pentagonalization"),this);
	mCubicPentagonalizationAction->setCheckable(true);
	sm->registerAction(mCubicPentagonalizationAction, "Remeshing", "");
	mCubicPentagonalizationAction->setStatusTip(tr("Enter Cubic Pentagonalization Remeshing Mode"));
	mCubicPentagonalizationAction->setToolTip(tr("Cubic Pentagonalization Remeshing Mode"));
	connect(mCubicPentagonalizationAction, SIGNAL(triggered()), this, SLOT(triggerCubicPentagonalization()));	
	actionList->addAction(mCubicPentagonalizationAction);

	mDualPentagonalizationAction = new QAction(QIcon(":images/doosabin_extrude.png"),tr("Dual Pentagonalization"),this);
	mDualPentagonalizationAction->setCheckable(true);
	sm->registerAction(mDualPentagonalizationAction, "Remeshing", "");
	mDualPentagonalizationAction->setStatusTip(tr("Enter Dual Pentagonalization Remeshing Mode"));
	mDualPentagonalizationAction->setToolTip(tr("Dual Pentagonalization Remeshing Mode"));
	connect(mDualPentagonalizationAction, SIGNAL(triggered()), this, SLOT(triggerDualPentagonalization()));	
	actionList->addAction(mDualPentagonalizationAction);

	mLoopStyleRemeshingAction = new QAction(QIcon(":images/doosabin_extrude.png"),tr("Loop Style Remeshing"),this);
	mLoopStyleRemeshingAction->setCheckable(true);
	sm->registerAction(mLoopStyleRemeshingAction, "Remeshing", "");
	mLoopStyleRemeshingAction->setStatusTip(tr("Enter Loop Style Remeshing Remeshing Mode"));
	mLoopStyleRemeshingAction->setToolTip(tr("Loop Style Remeshing Remeshing Mode"));
	connect(mLoopStyleRemeshingAction, SIGNAL(triggered()), this, SLOT(triggerLoopStyleRemeshing()));		
	actionList->addAction(mLoopStyleRemeshingAction);

	mLoopSubdivisionAction = new QAction(QIcon(":images/doosabin_extrude.png"),tr("Loop Subdivision"),this);
	mLoopSubdivisionAction->setCheckable(true);
	sm->registerAction(mLoopSubdivisionAction, "Remeshing", "");
	mLoopSubdivisionAction->setStatusTip(tr("Enter Loop Subdivision Remeshing Mode"));
	mLoopSubdivisionAction->setToolTip(tr("Loop Subdivision Remeshing Mode"));
	connect(mLoopSubdivisionAction, SIGNAL(triggered()), this, SLOT(triggerLoopSubdivision()));		
	actionList->addAction(mLoopSubdivisionAction);
	
	mDualLoopStyleRemeshingAction = new QAction(QIcon(":images/doosabin_extrude.png"),tr("Dual Loop Style Remeshing"),this);
	mDualLoopStyleRemeshingAction->setCheckable(true);
	sm->registerAction(mDualLoopStyleRemeshingAction, "Remeshing", "");
	mDualLoopStyleRemeshingAction->setStatusTip(tr("Enter Dual Loop Style Remeshing Remeshing Mode"));
	mDualLoopStyleRemeshingAction->setToolTip(tr("Dual Loop Style Remeshing Remeshing Mode"));
	connect(mDualLoopStyleRemeshingAction, SIGNAL(triggered()), this, SLOT(triggerDualLoopStyleRemeshing()));		
	actionList->addAction(mDualLoopStyleRemeshingAction);
	
	mDualLoopSubdivisionAction = new QAction(QIcon(":images/doosabin_extrude.png"),tr("Dual Loop Subdivision"),this);
	mDualLoopSubdivisionAction->setCheckable(true);
	sm->registerAction(mDualLoopSubdivisionAction, "Remeshing", "");
	mDualLoopSubdivisionAction->setStatusTip(tr("Enter Dual Loop Subdivision Remeshing Mode"));
	mDualLoopSubdivisionAction->setToolTip(tr("Dual Loop Subdivision Remeshing Mode"));
	connect(mDualLoopSubdivisionAction, SIGNAL(triggered()), this, SLOT(triggerDualLoopSubdivision()));		
	actionList->addAction(mDualLoopSubdivisionAction);
	
	mGlobalExtrudeAction = new QAction(QIcon(":images/doosabin_extrude.png"),tr("Global Extrude"),this);
	mGlobalExtrudeAction->setCheckable(true);
	sm->registerAction(mGlobalExtrudeAction, "Remeshing", "");
	mGlobalExtrudeAction->setStatusTip(tr("Enter Global Extrude Remeshing Mode"));
	mGlobalExtrudeAction->setToolTip(tr("Global Extrude Remeshing Mode"));
	connect(mGlobalExtrudeAction, SIGNAL(triggered()), this, SLOT(triggerGlobalExtrude()));
	actionList->addAction(mGlobalExtrudeAction);

	mCheckerboardAction = new QAction(QIcon(":images/doosabin_extrude.png"),tr("Checkerboard"),this);
	mCheckerboardAction->setCheckable(true);
	sm->registerAction(mCheckerboardAction, "Remeshing", "");
	mCheckerboardAction->setStatusTip(tr("Enter Checkerboard Remeshing Mode"));
	mCheckerboardAction->setToolTip(tr("Checkerboard Remeshing Mode"));
	connect(mCheckerboardAction, SIGNAL(triggered()), this, SLOT(triggerCheckerboard()));
	actionList->addAction(mCheckerboardAction);

	mDualGlobalExtrudeAction = new QAction(QIcon(":images/doosabin_extrude.png"),tr("Dual Global Extrude"),this);
	mDualGlobalExtrudeAction->setCheckable(true);
	sm->registerAction(mDualGlobalExtrudeAction, "Remeshing", "");
	mDualGlobalExtrudeAction->setStatusTip(tr("Enter Dual Global Extrude Remeshing Mode"));
	mDualGlobalExtrudeAction->setToolTip(tr("Dual Global Extrude Remeshing Mode"));
	connect(mDualGlobalExtrudeAction, SIGNAL(triggered()), this, SLOT(triggerDualGlobalExtrude()));
	actionList->addAction(mDualGlobalExtrudeAction);

	mDualCheckerboardAction = new QAction(QIcon(":images/doosabin_extrude.png"),tr("Dual Checkerboard"),this);
	mDualCheckerboardAction->setCheckable(true);
	sm->registerAction(mDualCheckerboardAction, "Remeshing", "");
	mDualCheckerboardAction->setStatusTip(tr("Enter Dual Checkerboard Remeshing Mode"));
	mDualCheckerboardAction->setToolTip(tr("Dual Checkerboard Remeshing Mode"));
	connect(mDualCheckerboardAction, SIGNAL(triggered()), this, SLOT(triggerDualCheckerboard()));
	actionList->addAction(mDualCheckerboardAction);

	mPentagonPreservingAction = new QAction(QIcon(":images/doosabin_extrude.png"),tr("Pentagon Preserving"),this);
	mPentagonPreservingAction->setCheckable(true);
	sm->registerAction(mPentagonPreservingAction, "Remeshing", "");
	mPentagonPreservingAction->setStatusTip(tr("Enter Pentagon Preserving Remeshing Mode"));
	mPentagonPreservingAction->setToolTip(tr("Pentagon Preserving Remeshing Mode"));
	connect(mPentagonPreservingAction, SIGNAL(triggered()), this, SLOT(triggerPentagonPreserving()));
	actionList->addAction(mPentagonPreservingAction);

	mDualPentagonPreservingAction = new QAction(QIcon(":images/doosabin_extrude.png"),tr("Dual Pentagon Preserving"),this);
	mDualPentagonPreservingAction->setCheckable(true);
	sm->registerAction(mDualPentagonPreservingAction, "Remeshing", "");
	mDualPentagonPreservingAction->setStatusTip(tr("Enter Dual Pentagon Preserving Remeshing Mode"));
	mDualPentagonPreservingAction->setToolTip(tr("Dual Pentagon Preserving Remeshing Mode"));
	connect(mDualPentagonPreservingAction, SIGNAL(triggered()), this, SLOT(triggerDualPentagonPreserving()));	
	actionList->addAction(mDualPentagonPreservingAction);
	
	mDualLoopStyleRemeshingSixAction = new QAction(QIcon(":images/doosabin_extrude.png"),tr("Dual Loop Style"),this);
	mDualLoopStyleRemeshingSixAction->setCheckable(true);
	sm->registerAction(mDualLoopStyleRemeshingSixAction, "Remeshing", "");
	mDualLoopStyleRemeshingSixAction->setStatusTip(tr("Enter Loop Style Remeshing Remeshing Mode"));
	mDualLoopStyleRemeshingSixAction->setToolTip(tr("Dual Loop Style Remeshing Remeshing Mode"));
	connect(mDualLoopStyleRemeshingSixAction, SIGNAL(triggered()), this, SLOT(triggerDualLoopStyleRemeshingSix()));	
	actionList->addAction(mDualLoopStyleRemeshingSixAction);
	
	mLoopStyleRemeshingSixAction = new QAction(QIcon(":images/doosabin_extrude.png"),tr("Loop Style"),this);
	mLoopStyleRemeshingSixAction->setCheckable(true);
	sm->registerAction(mLoopStyleRemeshingSixAction, "Remeshing", "");
	mLoopStyleRemeshingSixAction->setStatusTip(tr("Enter Loop Style Remeshing Remeshing Mode"));
	mLoopStyleRemeshingSixAction->setToolTip(tr("Loop Style Remeshing Remeshing Mode"));
	connect(mLoopStyleRemeshingSixAction, SIGNAL(triggered()), this, SLOT(triggerLoopStyleRemeshingSix()));	
	actionList->addAction(mLoopStyleRemeshingSixAction);
	
	mFractalAction = new QAction(QIcon(":images/doosabin_extrude.png"),tr("Fractal"),this);
	mFractalAction->setCheckable(true);
	sm->registerAction(mFractalAction, "Remeshing", "");
	mFractalAction->setStatusTip(tr("Enter Fractal Remeshing Mode"));
	mFractalAction->setToolTip(tr("Fractal Remeshing Mode"));
	connect(mFractalAction, SIGNAL(triggered()), this, SLOT(triggerFractal()));	
	actionList->addAction(mFractalAction);
	
	mDoubleStellateMiscAction = new QAction(QIcon(":images/doosabin_extrude.png"),tr("Double Stellate"),this);
	mDoubleStellateMiscAction->setCheckable(true);
	sm->registerAction(mDoubleStellateMiscAction, "Remeshing", "");
	mDoubleStellateMiscAction->setStatusTip(tr("Enter Double Stellate Remeshing Mode"));
	mDoubleStellateMiscAction->setToolTip(tr("Double Stellate Remeshing Mode"));
	connect(mDoubleStellateMiscAction, SIGNAL(triggered()), this, SLOT(triggerDoubleStellateMisc()));	
	actionList->addAction(mDoubleStellateMiscAction);

	mDooSabinBCAction = new QAction(QIcon(":images/doosabin_extrude.png"),tr("Doo Sabin BC"),this);
	mDooSabinBCAction->setCheckable(true);
	sm->registerAction(mDooSabinBCAction, "Remeshing", "");
	mDooSabinBCAction->setStatusTip(tr("Enter Doo Sabin BC Remeshing Mode"));
	mDooSabinBCAction->setToolTip(tr("Doo Sabin BC Remeshing Mode"));
	connect(mDooSabinBCAction, SIGNAL(triggered()), this, SLOT(triggerDooSabinBC()));	
	actionList->addAction(mDooSabinBCAction);
	
	mDooSabinBCNewAction = new QAction(QIcon(":images/doosabin_extrude.png"),tr("Doo Sabin BC New"),this);
	mDooSabinBCNewAction->setCheckable(true);
	sm->registerAction(mDooSabinBCNewAction, "Remeshing", "");
	mDooSabinBCNewAction->setStatusTip(tr("Enter Doo Sabin BC New Remeshing Mode"));
	mDooSabinBCNewAction->setToolTip(tr("Doo Sabin BC New Remeshing Mode"));
	connect(mDooSabinBCNewAction, SIGNAL(triggered()), this, SLOT(triggerDooSabinBCNew()));	
	actionList->addAction(mDooSabinBCNewAction);

	mDomeAction = new QAction(QIcon(":images/doosabin_extrude.png"),tr("Dome"),this);
	mDomeAction->setCheckable(true);
	sm->registerAction(mDomeAction, "Remeshing", "");
	mDomeAction->setStatusTip(tr("Enter Dome Remeshing Mode"));
	mDomeAction->setToolTip(tr("Dome Remeshing Mode"));
	connect(mDomeAction, SIGNAL(triggered()), this, SLOT(triggerDome()));	
	actionList->addAction(mDomeAction);
	
	mSubdivideFaceAction = new QAction(QIcon(":images/doosabin_extrude.png"),tr("Subdivide Face"),this);
	mSubdivideFaceAction->setCheckable(true);
	sm->registerAction(mSubdivideFaceAction, "Remeshing", "");
	mSubdivideFaceAction->setStatusTip(tr("Enter Subdivide Face Remeshing Mode"));
	mSubdivideFaceAction->setToolTip(tr("Subdivide Face Remeshing Mode"));
	connect(mSubdivideFaceAction, SIGNAL(triggered()), this, SLOT(triggerSubdivideFace()));	
	actionList->addAction(mSubdivideFaceAction);

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
					((MainWindow*)mParent),SLOT(performRemeshing()) );

	mDualLayout->addWidget(dualCreateButton);	
	mDualLayout->addStretch(1);
	mDualWidget->setWindowTitle("Cut by Edge");
	mDualWidget->setLayout(mDualLayout);
}

void RemeshingMode::triggerDual(){

	((MainWindow*)mParent)->setToolOptions(mDualWidget);
	((MainWindow*)mParent)->setRemeshingScheme(MainWindow::Dual);
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
					((MainWindow*)mParent),SLOT(performRemeshing()) );
	mRootThreeLayout->addWidget(rootThreeCreateButton);
	mRootThreeLayout->addStretch(1);
	mRootThreeWidget->setWindowTitle("Root-3 Remeshing");
	mRootThreeWidget->setLayout(mRootThreeLayout);
	
	//dual vertex truncation
	mDualVertexTruncationLayout = new QBoxLayout(QBoxLayout::LeftToRight);
	mDualVertexTruncationLayout->setMargin(0);
	QPushButton *dualVertexTruncationButton = new QPushButton(tr("Perform Remeshing"), this);
	connect(dualVertexTruncationButton, SIGNAL(clicked()),
					((MainWindow*)mParent),SLOT(performRemeshing()) );
	mDualVertexTruncationLayout->addWidget(dualVertexTruncationButton);
	mDualVertexTruncationLayout->addStretch(1);
	mDualVertexTruncationWidget->setWindowTitle("Dual Vertex Truncation");
	mDualVertexTruncationWidget->setLayout(mDualVertexTruncationLayout);

	//stellation
	mStellationLayout = new QBoxLayout(QBoxLayout::LeftToRight);
	mStellationLayout->setMargin(0);
	QPushButton *stellationButton = new QPushButton(tr("Perform Remeshing"), this);
	connect(stellationButton, SIGNAL(clicked()),
					((MainWindow*)mParent),SLOT(performRemeshing()) );
	mStellationLayout->addWidget(stellationButton);
	mStellationLayout->addStretch(1);
	mStellationWidget->setWindowTitle("Stellation Remeshing");
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
					((MainWindow*)mParent),SLOT(performRemeshing()) );
	mDoubleStellationLayout->addWidget(starButton);
	mDoubleStellationLayout->addStretch(1);
	mDoubleStellationWidget->setWindowTitle("Double Stellation Remeshing");
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
					((MainWindow*)mParent),SLOT(performRemeshing()) );
	mTwelveSixFourLayout->addWidget(twelveSixFourButton);
	mTwelveSixFourLayout->addStretch(1);
	mTwelveSixFourWidget->setWindowTitle("12.6.4");
	mTwelveSixFourWidget->setLayout(mTwelveSixFourLayout);
	
	//honeycomb
	mHoneycombLayout = new QBoxLayout(QBoxLayout::LeftToRight);
	mHoneycombLayout->setMargin(0);
	QPushButton *honeycombButton = new QPushButton(tr("Perform Remeshing"), this);
	connect(honeycombButton, SIGNAL(clicked()),
					((MainWindow*)mParent),SLOT(performRemeshing()) );
	mHoneycombLayout->addWidget(honeycombButton);
	mHoneycombLayout->addStretch(1);
	mHoneycombWidget->setWindowTitle("Honeycomb Remeshing");
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
					((MainWindow*)mParent),SLOT(performRemeshing()) );
	mVertexTruncationLayout->addWidget(vertexTruncationButton);
	mVertexTruncationLayout->addStretch(1);
	mVertexTruncationWidget->setWindowTitle("Vertex Truncation Remeshing");
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
					((MainWindow*)mParent),SLOT(performRemeshing()) );
					
	mDualTwelveSixFourLayout->addWidget(dualTwelveSixFourButton);
	mDualTwelveSixFourLayout->addStretch(1);
	mDualTwelveSixFourWidget->setWindowTitle("Dual 12.6.4 Remeshing");
	mDualTwelveSixFourWidget->setLayout(mDualTwelveSixFourLayout);
	
}

void RemeshingMode::triggerRootThree(){

	((MainWindow*)mParent)->setToolOptions(mRootThreeWidget);
	((MainWindow*)mParent)->setRemeshingScheme(MainWindow::Root3);
}

void RemeshingMode::triggerDualVertexTruncation(){

	((MainWindow*)mParent)->setToolOptions(mDualVertexTruncationWidget);
	((MainWindow*)mParent)->setRemeshingScheme(MainWindow::DualVertexTrunc);
}

void RemeshingMode::triggerStellation(){

	((MainWindow*)mParent)->setToolOptions(mStellationWidget);
	((MainWindow*)mParent)->setRemeshingScheme(MainWindow::GlobalStellate);
}

void RemeshingMode::triggerDoubleStellation(){

	((MainWindow*)mParent)->setToolOptions(mDoubleStellationWidget);
	((MainWindow*)mParent)->setRemeshingScheme(MainWindow::Star);
}

void RemeshingMode::triggerTwelveSixFour(){

	((MainWindow*)mParent)->setToolOptions(mTwelveSixFourWidget);
	((MainWindow*)mParent)->setRemeshingScheme(MainWindow::Generic1264);
}

void RemeshingMode::triggerHoneycomb(){

	((MainWindow*)mParent)->setToolOptions(mHoneycombWidget);
	((MainWindow*)mParent)->setRemeshingScheme(MainWindow::Honeycomb);
}

void RemeshingMode::triggerVertexTruncation(){

	((MainWindow*)mParent)->setToolOptions(mVertexTruncationWidget);
	((MainWindow*)mParent)->setRemeshingScheme(MainWindow::VertexTrunc);
}

void RemeshingMode::triggerDualTwelveSixFour(){

	((MainWindow*)mParent)->setToolOptions(mDualTwelveSixFourWidget);
	((MainWindow*)mParent)->setRemeshingScheme(MainWindow::DualGeneric1264);
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
					((MainWindow*)mParent),SLOT(performRemeshing()) );
					
	mLinearVertexLayout->addWidget(linearVertexCreateButton);
	mLinearVertexLayout->addStretch(1);
	mLinearVertexWidget->setWindowTitle("Linear Vertex Insertion Remeshing");
	mLinearVertexWidget->setLayout(mLinearVertexLayout);

	//catmull clark
	mCatmullClarkLayout = new QBoxLayout(QBoxLayout::LeftToRight);
	mCatmullClarkLayout->setMargin(0);
	QPushButton *catmullClarkCreateButton = new QPushButton(tr("Perform Remeshing"), this);
	connect(catmullClarkCreateButton, SIGNAL(clicked()),
					((MainWindow*)mParent),SLOT(performRemeshing()) );
					
	mCatmullClarkLayout->addWidget(catmullClarkCreateButton);
	mCatmullClarkLayout->addStretch(1);
	mCatmullClarkWidget->setWindowTitle("Catmull-Clark Remeshing");
	mCatmullClarkWidget->setLayout(mCatmullClarkLayout);
	
	//stellate with edge removal
	mStellateEdgeRemovalLayout = new QBoxLayout(QBoxLayout::LeftToRight);
	mStellateEdgeRemovalLayout->setMargin(0);
	QPushButton *stellateEdgeRemovalCreateButton = new QPushButton(tr("Perform Remeshing"), this);
	connect(stellateEdgeRemovalCreateButton, SIGNAL(clicked()),
					((MainWindow*)mParent),SLOT(performRemeshing()) );
					
	mStellateEdgeRemovalLayout->addWidget(stellateEdgeRemovalCreateButton);
	mStellateEdgeRemovalLayout->addStretch(1);
	mStellateEdgeRemovalWidget->setWindowTitle("Stellate with Edge Removal Remeshing");
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
					((MainWindow*)mParent),SLOT(performRemeshing()) );
	mDooSabinLayout->addWidget(dooSabinCreateButton);
	mDooSabinLayout->addStretch(1);
	mDooSabinWidget->setWindowTitle("Doo Sabin Remeshing");
	mDooSabinWidget->setLayout(mDooSabinLayout);
	
	//corner cutting
	mCornerCuttingLayout = new QBoxLayout(QBoxLayout::LeftToRight);
	mCornerCuttingLayout->setMargin(0);
	QPushButton *cornerCuttingCreateButton = new QPushButton(tr("Perform Remeshing"), this);
	connect(cornerCuttingCreateButton, SIGNAL(clicked()),
					((MainWindow*)mParent),SLOT(performRemeshing()) );
	mCornerCuttingLayout->addWidget(cornerCuttingCreateButton);
	mCornerCuttingLayout->addStretch(1);
	mCornerCuttingWidget->setWindowTitle("Corner Cutting Remeshing");
	mCornerCuttingWidget->setLayout(mCornerCuttingLayout);
	
	//simplest
	mSimplestLayout = new QBoxLayout(QBoxLayout::LeftToRight);
	mSimplestLayout->setMargin(0);
	QPushButton *simplestCreateButton = new QPushButton(tr("Perform Remeshing"), this);
	connect(simplestCreateButton, SIGNAL(clicked()),
					((MainWindow*)mParent),SLOT(performRemeshing()) );
	mSimplestLayout->addWidget(simplestCreateButton);
	mSimplestLayout->addStretch(1);
	mSimplestWidget->setWindowTitle("Simplest Remeshing Scheme");
	mSimplestWidget->setLayout(mSimplestLayout);	
	
}

void RemeshingMode::triggerLinearVertex(){

	((MainWindow*)mParent)->setToolOptions(mLinearVertexWidget);
	((MainWindow*)mParent)->setRemeshingScheme(MainWindow::LinearVertexInsertion);
}

void RemeshingMode::triggerCatmullClark(){

	((MainWindow*)mParent)->setToolOptions(mCatmullClarkWidget);
	((MainWindow*)mParent)->setRemeshingScheme(MainWindow::CatmullClark);
}

void RemeshingMode::triggerStellateEdgeRemoval(){

	((MainWindow*)mParent)->setToolOptions(mStellateEdgeRemovalWidget);
	((MainWindow*)mParent)->setRemeshingScheme(MainWindow::ModifiedStellate);
}

void RemeshingMode::triggerDooSabin(){

	((MainWindow*)mParent)->setToolOptions(mDooSabinWidget);
	((MainWindow*)mParent)->setRemeshingScheme(MainWindow::DooSabin);
}

void RemeshingMode::triggerCornerCutting(){

	((MainWindow*)mParent)->setToolOptions(mCornerCuttingWidget);
	((MainWindow*)mParent)->setRemeshingScheme(MainWindow::CornerCutting);
}

void RemeshingMode::triggerSimplest(){

	((MainWindow*)mParent)->setToolOptions(mSimplestWidget);
	((MainWindow*)mParent)->setRemeshingScheme(MainWindow::Simplest);
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
					((MainWindow*)mParent),SLOT(performRemeshing()) );
					
	mPentagonalizationLayout->addWidget(pentagonalizationCreateButton);
	mPentagonalizationLayout->addStretch(1);
	mPentagonalizationWidget->setWindowTitle("Pentagonalization Remeshing");
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
					((MainWindow*)mParent),SLOT(performRemeshing()) );
					
	mCubicPentagonalizationLayout->addWidget(cubicPentagonalizationCreateButton);
	mCubicPentagonalizationLayout->addStretch(1);
	mCubicPentagonalizationWidget->setWindowTitle("Cubic Pentagonalization Remeshing");
	mCubicPentagonalizationWidget->setLayout(mCubicPentagonalizationLayout);
	
	//dual pentagonalization
	mDualPentagonalizationLayout = new QBoxLayout(QBoxLayout::LeftToRight);
	mDualPentagonalizationLayout->setMargin(0);
	QPushButton *dualPentagonalizationCreateButton = new QPushButton(tr("Perform Remeshing"), this);
	connect(dualPentagonalizationCreateButton, SIGNAL(clicked()),
					((MainWindow*)mParent),SLOT(performRemeshing()) );
	mDualPentagonalizationLayout->addWidget(dualPentagonalizationCreateButton);
	mDualPentagonalizationLayout->addStretch(1);
	mDualPentagonalizationWidget->setWindowTitle("Dual Pentagonalization Remeshing");
	mDualPentagonalizationWidget->setLayout(mDualPentagonalizationLayout);

}

void RemeshingMode::triggerPentagonalization(){

	((MainWindow*)mParent)->setToolOptions(mPentagonalizationWidget);
	((MainWindow*)mParent)->setRemeshingScheme(MainWindow::Pentagonal);
}

void RemeshingMode::triggerCubicPentagonalization(){

	((MainWindow*)mParent)->setToolOptions(mCubicPentagonalizationWidget);
	((MainWindow*)mParent)->setRemeshingScheme(MainWindow::CubicPentagonal);
}

void RemeshingMode::triggerDualPentagonalization(){

	((MainWindow*)mParent)->setToolOptions(mDualPentagonalizationWidget);
	((MainWindow*)mParent)->setRemeshingScheme(MainWindow::DualPentagonal);
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
					((MainWindow*)mParent),SLOT(performRemeshing()) );
	
	mLoopStyleRemeshingLayout->addWidget(loopStyleRemeshingButton);
	mLoopStyleRemeshingLayout->addStretch(1);
	mLoopStyleRemeshingWidget->setWindowTitle("Loop Style Remeshing");
	mLoopStyleRemeshingWidget->setLayout(mLoopStyleRemeshingLayout);

	//loop subdivision
	mLoopSubdivisionLayout = new QBoxLayout(QBoxLayout::LeftToRight);
	mLoopSubdivisionLayout->setMargin(0);
	QPushButton *loopSubdivisionCreateButton = new QPushButton(tr("Perform Remeshing"), this);
	connect(loopSubdivisionCreateButton, SIGNAL(clicked()),
					((MainWindow*)mParent),SLOT(performRemeshing()) );
	mLoopSubdivisionLayout->addWidget(loopSubdivisionCreateButton);
	mLoopSubdivisionLayout->addStretch(1);
	mLoopSubdivisionWidget->setWindowTitle("Loop Subdivision Remeshing");
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
					((MainWindow*)mParent),SLOT(performRemeshing()) );
					
	mDualLoopStyleRemeshingLayout->addWidget(dualLoopStyleRemeshingButton);
	mDualLoopStyleRemeshingLayout->addStretch(1);
	mDualLoopStyleRemeshingWidget->setWindowTitle("Dual Loop Style Remeshing");
	mDualLoopStyleRemeshingWidget->setLayout(mDualLoopStyleRemeshingLayout);
	
	//dual of loop subdivision
	mDualLoopSubdivisionLayout = new QBoxLayout(QBoxLayout::LeftToRight);
	mDualLoopSubdivisionLayout->setMargin(0);
	QPushButton *dualLoopSubdivisionCreateButton = new QPushButton(tr("Perform Remeshing"), this);
	connect(dualLoopSubdivisionCreateButton, SIGNAL(clicked()),
					((MainWindow*)mParent),SLOT(performRemeshing()) );
					
	mDualLoopSubdivisionLayout->addWidget(dualLoopSubdivisionCreateButton);
	mDualLoopSubdivisionLayout->addStretch(1);
	mDualLoopSubdivisionWidget->setWindowTitle("Dual Loop Subdivision Remeshing");
	mDualLoopSubdivisionWidget->setLayout(mDualLoopSubdivisionLayout);

}

void RemeshingMode::triggerLoopStyleRemeshing(){

	((MainWindow*)mParent)->setToolOptions(mLoopStyleRemeshingWidget);
	((MainWindow*)mParent)->setRemeshingScheme(MainWindow::LoopStyle);
}

void RemeshingMode::triggerLoopSubdivision(){

	((MainWindow*)mParent)->setToolOptions(mLoopSubdivisionWidget);
	((MainWindow*)mParent)->setRemeshingScheme(MainWindow::Loop);
}

void RemeshingMode::triggerDualLoopStyleRemeshing(){

	((MainWindow*)mParent)->setToolOptions(mDualLoopStyleRemeshingWidget);
	((MainWindow*)mParent)->setRemeshingScheme(MainWindow::Root4);
}

void RemeshingMode::triggerDualLoopSubdivision(){

	((MainWindow*)mParent)->setToolOptions(mDualLoopSubdivisionWidget);
	((MainWindow*)mParent)->setRemeshingScheme(MainWindow::DualLoop);
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
					((MainWindow*)mParent),SLOT(performRemeshing()) );
					
	mGlobalExtrudeLayout->addWidget(globalExtrudeButton);
	mGlobalExtrudeLayout->addStretch(1);
	mGlobalExtrudeWidget->setWindowTitle("Global Extrude Remeshing");
	mGlobalExtrudeWidget->setLayout(mGlobalExtrudeLayout);

	//checkerboard
	mCheckerboardLayout = new QBoxLayout(QBoxLayout::LeftToRight);
	mCheckerboardLayout->setMargin(0);
	QPushButton *checkerboardCreateButton = new QPushButton(tr("Perform Remeshing"), this);
	connect(checkerboardCreateButton, SIGNAL(clicked()),
					((MainWindow*)mParent),SLOT(performRemeshing()) );
					
	mCheckerboardLayout->addWidget(checkerboardCreateButton);
	mCheckerboardLayout->addStretch(1);
	mCheckerboardWidget->setWindowTitle("Checkerboard Remeshing");
	mCheckerboardWidget->setLayout(mCheckerboardLayout);
	
	//dual of global extrude
	mDualGlobalExtrudeLayout = new QBoxLayout(QBoxLayout::LeftToRight);
	mDualGlobalExtrudeLayout->setMargin(0);
	QPushButton *dualGlobalExtrudeButton = new QPushButton(tr("Perform Remeshing"), this);
	connect(dualGlobalExtrudeButton, SIGNAL(clicked()),
					((MainWindow*)mParent),SLOT(performRemeshing()) );
					
	mDualGlobalExtrudeLayout->addWidget(dualGlobalExtrudeButton);
	mDualGlobalExtrudeLayout->addStretch(1);
	mDualGlobalExtrudeWidget->setWindowTitle("Dual Global Extrude Remeshing");
	mDualGlobalExtrudeWidget->setLayout(mDualGlobalExtrudeLayout);
	
	//dual of checkerboard remeshing
	mDualCheckerboardLayout = new QBoxLayout(QBoxLayout::LeftToRight);
	mDualCheckerboardLayout->setMargin(0);
	QPushButton *dualCheckerboardCreateButton = new QPushButton(tr("Perform Remeshing"), this);
	connect(dualCheckerboardCreateButton, SIGNAL(clicked()),
					((MainWindow*)mParent),SLOT(performRemeshing()) );
					
	mDualCheckerboardLayout->addWidget(dualCheckerboardCreateButton);
	mDualCheckerboardLayout->addStretch(1);
	mDualCheckerboardWidget->setWindowTitle("Dual Checkerboard Remeshing");
	mDualCheckerboardWidget->setLayout(mDualCheckerboardLayout);
	
}

void RemeshingMode::triggerGlobalExtrude(){

	((MainWindow*)mParent)->setToolOptions(mGlobalExtrudeWidget);
	((MainWindow*)mParent)->setRemeshingScheme(MainWindow::GlobalCubicExtrude);
}

void RemeshingMode::triggerCheckerboard(){

	((MainWindow*)mParent)->setToolOptions(mCheckerboardWidget);
	((MainWindow*)mParent)->setRemeshingScheme(MainWindow::CheckerBoard);
}

void RemeshingMode::triggerDualGlobalExtrude(){

	((MainWindow*)mParent)->setToolOptions(mDualGlobalExtrudeWidget);
	((MainWindow*)mParent)->setRemeshingScheme(MainWindow::DualGlobalCubicExtrude);
}

void RemeshingMode::triggerDualCheckerboard(){

	((MainWindow*)mParent)->setToolOptions(mDualCheckerboardWidget);
	((MainWindow*)mParent)->setRemeshingScheme(MainWindow::DualCheckerBoard);
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
					((MainWindow*)mParent),SLOT(performRemeshing()) );
					
	mPentagonPreservingLayout->addWidget(pentagonPreservingButton);
	mPentagonPreservingLayout->addStretch(1);
	mPentagonPreservingWidget->setWindowTitle("Pentagon Preserving Remeshing");
	mPentagonPreservingWidget->setLayout(mPentagonPreservingLayout);

	//dual of Pentagon Preserving
	mDualPentagonPreservingLayout = new QBoxLayout(QBoxLayout::LeftToRight);
	mDualPentagonPreservingLayout->setMargin(0);
	QPushButton *dualPentagonPreservingCreateButton = new QPushButton(tr("Perform Remeshing"), this);
	connect(dualPentagonPreservingCreateButton, SIGNAL(clicked()),
					((MainWindow*)mParent),SLOT(performRemeshing()) );
					
	mDualPentagonPreservingLayout->addWidget(dualPentagonPreservingCreateButton);
	mDualPentagonPreservingLayout->addStretch(1);
	mDualPentagonPreservingWidget->setWindowTitle("Dual Pentagon Preserving Remeshing");
	mDualPentagonPreservingWidget->setLayout(mDualPentagonPreservingLayout);
	
}

void RemeshingMode::triggerPentagonPreserving(){

	((MainWindow*)mParent)->setToolOptions(mPentagonPreservingWidget);
	((MainWindow*)mParent)->setRemeshingScheme(MainWindow::PentagonPreserving);
}

void RemeshingMode::triggerDualPentagonPreserving(){

	((MainWindow*)mParent)->setToolOptions(mDualPentagonPreservingWidget);
	((MainWindow*)mParent)->setRemeshingScheme(MainWindow::DualPentagonPreserving);
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
					((MainWindow*)mParent),SLOT(performRemeshing()) );
					
	mDualLoopStyleRemeshingSixLayout->addWidget(dualLoopStyleRemeshingSixButton);
	mDualLoopStyleRemeshingSixLayout->addStretch(1);
	mDualLoopStyleRemeshingSixWidget->setWindowTitle("Dual Loop Style Six Remeshing");
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
					((MainWindow*)mParent),SLOT(performRemeshing()) );
					
	mLoopStyleRemeshingSixLayout->addWidget(loopStyleRemeshingSixButton);
	mLoopStyleRemeshingSixLayout->addStretch(1);
	mLoopStyleRemeshingSixWidget->setWindowTitle("Loop Style Six Remeshing");
	mLoopStyleRemeshingSixWidget->setLayout(mLoopStyleRemeshingSixLayout);

}

void RemeshingMode::triggerDualLoopStyleRemeshingSix(){

	((MainWindow*)mParent)->setToolOptions(mDualLoopStyleRemeshingSixWidget);
	((MainWindow*)mParent)->setRemeshingScheme(MainWindow::HexagonPreserving);
}

void RemeshingMode::triggerLoopStyleRemeshingSix(){

	((MainWindow*)mParent)->setToolOptions(mLoopStyleRemeshingSixWidget);
	((MainWindow*)mParent)->setRemeshingScheme(MainWindow::DualHexagonPreserving);
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
					((MainWindow*)mParent),SLOT(performRemeshing()) );
					
	mFractalLayout->addWidget(fractalButton);
	mFractalLayout->addStretch(1);
	mFractalWidget->setWindowTitle("Fractal Remeshing");
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
					((MainWindow*)mParent),SLOT(performRemeshing()) );
					
	mDoubleStellateMiscLayout->addWidget(doubleStellateMiscButton);
	mDoubleStellateMiscLayout->addStretch(1);
	mDoubleStellateMiscWidget->setWindowTitle("Cut by Edge");
	mDoubleStellateMiscWidget->setLayout(mDoubleStellateMiscLayout);
	
	//doo sabin bc
	mDooSabinBCLayout = new QBoxLayout(QBoxLayout::LeftToRight);
	mDooSabinBCLayout->setMargin(0);
	QPushButton *dooSabinBCButton = new QPushButton(tr("Perform Remeshing"), this);
	connect(dooSabinBCButton, SIGNAL(clicked()),
					((MainWindow*)mParent),SLOT(performRemeshing()) );
					
	mDooSabinBCLayout->addWidget(dooSabinBCButton);
	mDooSabinBCLayout->addStretch(1);
	mDooSabinBCWidget->setWindowTitle("Doo Sabin BC Remeshing");
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
					((MainWindow*)mParent),SLOT(performRemeshing()) );
					
	mDooSabinBCNewLayout->addWidget(dooSabinBCNewButton);
	mDooSabinBCNewLayout->addStretch(1);
	mDooSabinBCNewWidget->setWindowTitle("Doo Sabin BC New Remeshing");
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
					((MainWindow*)mParent),SLOT(performRemeshing()) );
	mDomeLayout->addWidget(domeButton);
	mDomeLayout->addStretch(1);
	mDomeWidget->setWindowTitle("Dome Remeshing");
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
					((MainWindow*)mParent),SLOT(subdivideSelectedFaces()) );
					
	mSubdivideFaceLayout->addWidget(subdivideFaceButton);
	mSubdivideFaceLayout->addStretch(1);
	mSubdivideFaceWidget->setWindowTitle("Subdivide Selected Faces");
	mSubdivideFaceWidget->setLayout(mSubdivideFaceLayout);
	
}

void RemeshingMode::triggerFractal(){

	((MainWindow*)mParent)->setToolOptions(mFractalWidget);
	((MainWindow*)mParent)->setRemeshingScheme(MainWindow::Fractal);
}

void RemeshingMode::triggerDoubleStellateMisc(){

	((MainWindow*)mParent)->setToolOptions(mDoubleStellateMiscWidget);
	((MainWindow*)mParent)->setRemeshingScheme(MainWindow::ModifiedDoubleStellate);
}

void RemeshingMode::triggerDooSabinBC(){

	((MainWindow*)mParent)->setToolOptions(mDooSabinBCWidget);
	((MainWindow*)mParent)->setRemeshingScheme(MainWindow::DooSabinBC);
}

void RemeshingMode::triggerDooSabinBCNew(){

	((MainWindow*)mParent)->setToolOptions(mDooSabinBCNewWidget);
	((MainWindow*)mParent)->setRemeshingScheme(MainWindow::DooSabinBCNew);
}

void RemeshingMode::triggerDome(){

	((MainWindow*)mParent)->setToolOptions(mDomeWidget);
	((MainWindow*)mParent)->setRemeshingScheme(MainWindow::Dome);
}

void RemeshingMode::triggerSubdivideFace(){

	((MainWindow*)mParent)->setToolOptions(mSubdivideFaceWidget);
	((MainWindow*)mParent)->setMode(MainWindow::SubdivideFace);
}
