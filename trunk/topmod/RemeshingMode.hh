/****************************************************************************
 **
 **
 ****************************************************************************/

#ifndef REMESHINGMODE_H
#define REMESHINGMODE_H

#include <QWidget>
#include <QPushButton>
#include <QBoxLayout>

#include "MainWindow.hh"

class MainWindow;
class DLFLWindow;

class QComboBox;
class QPushButton;
class QGroupBox;
class QBoxLayout;
class QCheckBox;
class QLabel;

class RemeshingMode : public QWidget {
	Q_OBJECT

public:
	RemeshingMode(QWidget *parent = 0);
	void addActions(QActionGroup *remeshingActionGroup, QActionGroup *actionGroup, QToolBar *toolBar, QStackedWidget *stackedWidget);

		//dual
	QBoxLayout *mDualLayout;
	QWidget *mDualWidget;
	QAction *mDualAction;
	
	//three conversion
	QWidget *mRootThreeWidget;
	QWidget *mDualVertexTruncationWidget;
	QWidget *mStellationWidget;
	QWidget *mDoubleStellationWidget;
	QWidget *mTwelveSixFourWidget;
	QWidget *mHoneycombWidget;
	QWidget *mVertexTruncationWidget;
	QWidget *mDualTwelveSixFourWidget;
	
	QBoxLayout *mRootThreeLayout;
	QBoxLayout *mDualVertexTruncationLayout;
	QBoxLayout *mStellationLayout;
	QBoxLayout *mDoubleStellationLayout;
	QBoxLayout *mTwelveSixFourLayout;
	QBoxLayout *mHoneycombLayout;
	QBoxLayout *mVertexTruncationLayout;
	QBoxLayout *mDualTwelveSixFourLayout;
	
	QAction *mRootThreeAction;
	QAction *mDualVertexTruncationAction;
	QAction *mStellationAction;
	QAction *mDoubleStellationAction;
	QAction *mTwelveSixFourAction;
	QAction *mHoneycombAction;
	QAction *mVertexTruncationAction;
	QAction *mDualTwelveSixFourAction;
	
	//four conversion
	QWidget *mLinearVertexWidget;
	QWidget *mCatmullClarkWidget;
	QWidget *mStellateEdgeRemovalWidget;
	QWidget *mDooSabinWidget;
	QWidget *mCornerCuttingWidget;
	QWidget *mSimplestWidget;
	
	QBoxLayout *mLinearVertexLayout;
	QBoxLayout *mCatmullClarkLayout;
	QBoxLayout *mStellateEdgeRemovalLayout;
	QBoxLayout *mDooSabinLayout;
	QBoxLayout *mCornerCuttingLayout;
	QBoxLayout *mSimplestLayout;

	QAction *mLinearVertexAction;
	QAction *mCatmullClarkAction;
	QAction *mStellateEdgeRemovalAction;
	QAction *mDooSabinAction;
	QAction *mCornerCuttingAction;
	QAction *mSimplestAction;
	
	
	//five conversion
	QWidget *mPentagonalizationWidget;
  QWidget *mCubicPentagonalizationWidget;
  QWidget *mDualPentagonalizationWidget;
	
	QBoxLayout *mPentagonalizationLayout;
  QBoxLayout *mCubicPentagonalizationLayout;
  QBoxLayout *mDualPentagonalizationLayout;

	QAction *mPentagonalizationAction;
  QAction *mCubicPentagonalizationAction;
  QAction *mDualPentagonalizationAction;

	//three preservation
	QWidget *mLoopStyleRemeshingWidget;
  QWidget *mLoopSubdivisionWidget;
  QWidget *mDualLoopStyleRemeshingWidget;
  QWidget *mDualLoopSubdivisionWidget;

	QBoxLayout *mLoopStyleRemeshingLayout;
  QBoxLayout *mLoopSubdivisionLayout;
  QBoxLayout *mDualLoopStyleRemeshingLayout;
  QBoxLayout *mDualLoopSubdivisionLayout;

	QAction *mLoopStyleRemeshingAction;
  QAction *mLoopSubdivisionAction;
  QAction *mDualLoopStyleRemeshingAction;
  QAction *mDualLoopSubdivisionAction;
	
	//four preservation
  QWidget *mGlobalExtrudeWidget;
  QWidget *mCheckerboardWidget;
  QWidget *mDualGlobalExtrudeWidget;
  QWidget *mDualCheckerboardWidget;

  QBoxLayout *mGlobalExtrudeLayout;
  QBoxLayout *mCheckerboardLayout;
  QBoxLayout *mDualGlobalExtrudeLayout;
  QBoxLayout *mDualCheckerboardLayout;

  QAction *mGlobalExtrudeAction;
  QAction *mCheckerboardAction;
  QAction *mDualGlobalExtrudeAction;
  QAction *mDualCheckerboardAction;
	
	//five preservation
	QWidget *mPentagonPreservingWidget;
  QWidget *mDualPentagonPreservingWidget;
	
	QBoxLayout *mPentagonPreservingLayout;
  QBoxLayout *mDualPentagonPreservingLayout;

	QAction *mPentagonPreservingAction;
  QAction *mDualPentagonPreservingAction;

	//six preservation
	QWidget *mDualLoopStyleRemeshingSixWidget;
  QWidget *mLoopStyleRemeshingSixWidget;

	QBoxLayout *mDualLoopStyleRemeshingSixLayout;
  QBoxLayout *mLoopStyleRemeshingSixLayout;

	QAction *mDualLoopStyleRemeshingSixAction;
  QAction *mLoopStyleRemeshingSixAction;	
	
	//miscellaneous
	QWidget *mFractalWidget;
  QWidget *mDoubleStellateMiscWidget;
  QWidget *mDooSabinBCWidget;
  QWidget *mDooSabinBCNewWidget;
  QWidget *mDomeWidget;
  QWidget *mSubdivideFaceWidget;
	
	QBoxLayout *mFractalLayout;
  QBoxLayout *mDoubleStellateMiscLayout;
  QBoxLayout *mDooSabinBCLayout;
  QBoxLayout *mDooSabinBCNewLayout;
  QBoxLayout *mDomeLayout;
  QBoxLayout *mSubdivideFaceLayout;
	
	QAction *mFractalAction;
  QAction *mDoubleStellateMiscAction;
  QAction *mDooSabinBCAction;
  QAction *mDooSabinBCNewAction;
  QAction *mDomeAction;
  QAction *mSubdivideFaceAction;
	
protected:
	void setupDual();
	void setupThreeConversion();
	void setupFourConversion();
	void setupFiveConversion();
	void setupThreePreservation();
	void setupFourPreservation();
	void setupFivePreservation();
	void setupSixPreservation();
	void setupMiscellaneous();	

public slots:

	void triggerDual();

	void triggerRootThree();
	void triggerDualVertexTruncation();
	void triggerStellation();
	void triggerDoubleStellation();
	void triggerTwelveSixFour();
	void triggerHoneycomb();
	void triggerVertexTruncation();
	void triggerDualTwelveSixFour();

	void triggerLinearVertex();
	void triggerCatmullClark();
	void triggerStellateEdgeRemoval();
	void triggerDooSabin();
	void triggerCornerCutting();
	void triggerSimplest();

	void triggerPentagonalization();
	void triggerCubicPentagonalization();
	void triggerDualPentagonalization();

	void triggerLoopStyleRemeshing();
	void triggerLoopSubdivision();
	void triggerDualLoopStyleRemeshing();
	void triggerDualLoopSubdivision();

	void triggerGlobalExtrude();
	void triggerCheckerboard();
	void triggerDualGlobalExtrude();
	void triggerDualCheckerboard();

	void triggerPentagonPreserving();
	void triggerDualPentagonPreserving();

	void triggerDualLoopStyleRemeshingSix();
	void triggerLoopStyleRemeshingSixWidget();

	void triggerFractal();
	void triggerDoubleStellateMisc();
	void triggerDooSabinBC();
	void triggerDooSabinBCNew();
	void triggerDome();
	void triggerSubdivideFace();
		
private:
	
	QWidget *mParent;
	

	
};

#endif
