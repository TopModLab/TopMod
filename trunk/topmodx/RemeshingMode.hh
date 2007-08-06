/****************************************************************************
 **
 **
 ****************************************************************************/

#ifndef REMESHINGMODE_H
#define REMESHINGMODE_H

#include <QWidget>
#include <QPushButton>
#include <QGridLayout>

#include "MainWindow.hh"

class MainWindow;

/*!
	\file RemeshingMode.hh
	\brief Definition of the RemeshingMode class
	
	\see RemeshingMode
*/

class RemeshingMode : public QWidget {
	Q_OBJECT

public:
	RemeshingMode(QWidget *parent, QShortcutManager *sm, QWidget *actionList);
	void addActions(QActionGroup *actionGroup, QToolBar *toolBar, QStackedWidget *stackedWidget);
	QMenu *getMenu();
	void retranslateUi();

		//dual
	QGridLayout *mDualLayout;
	QWidget *mDualWidget;
	QAction *mDualAction;
	
	//three conversion
	QWidget *mRootThreeWidget;
	QWidget *mTriangulateWidget;
	QWidget *mDualVertexTruncationWidget;
	QWidget *mStellationWidget;
	QWidget *mDoubleStellationWidget;
	QWidget *mTwelveSixFourWidget;
	QWidget *mHoneycombWidget;
	QWidget *mVertexTruncationWidget;
	QWidget *mDualTwelveSixFourWidget;
	
	QGridLayout *mRootThreeLayout;
	QGridLayout *mTriangulateLayout;
	QGridLayout *mDualVertexTruncationLayout;
	QGridLayout *mStellationLayout;
	QGridLayout *mDoubleStellationLayout;
	QGridLayout *mTwelveSixFourLayout;
	QGridLayout *mHoneycombLayout;
	QGridLayout *mVertexTruncationLayout;
	QGridLayout *mDualTwelveSixFourLayout;
	
	QAction *mRootThreeAction;
	QAction *mTriangulateAction;
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
	
	QGridLayout *mLinearVertexLayout;
	QGridLayout *mCatmullClarkLayout;
	QGridLayout *mStellateEdgeRemovalLayout;
	QGridLayout *mDooSabinLayout;
	QGridLayout *mCornerCuttingLayout;
	QGridLayout *mSimplestLayout;

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
	
	QGridLayout *mPentagonalizationLayout;
  QGridLayout *mCubicPentagonalizationLayout;
  QGridLayout *mDualPentagonalizationLayout;

	QAction *mPentagonalizationAction;
  QAction *mCubicPentagonalizationAction;
  QAction *mDualPentagonalizationAction;

	//three preservation
	QWidget *mLoopStyleRemeshingWidget;
  QWidget *mLoopSubdivisionWidget;
  QWidget *mDualLoopStyleRemeshingWidget;
  QWidget *mDualLoopSubdivisionWidget;

	QGridLayout *mLoopStyleRemeshingLayout;
  QGridLayout *mLoopSubdivisionLayout;
  QGridLayout *mDualLoopStyleRemeshingLayout;
  QGridLayout *mDualLoopSubdivisionLayout;

	QAction *mLoopStyleRemeshingAction;
  QAction *mLoopSubdivisionAction;
  QAction *mDualLoopStyleRemeshingAction;
  QAction *mDualLoopSubdivisionAction;
	
	//four preservation
  QWidget *mGlobalExtrudeWidget;
  QWidget *mCheckerboardWidget;
  QWidget *mDualGlobalExtrudeWidget;
  QWidget *mDualCheckerboardWidget;

  QGridLayout *mGlobalExtrudeLayout;
  QGridLayout *mCheckerboardLayout;
  QGridLayout *mDualGlobalExtrudeLayout;
  QGridLayout *mDualCheckerboardLayout;

  QAction *mGlobalExtrudeAction;
  QAction *mCheckerboardAction;
  QAction *mDualGlobalExtrudeAction;
  QAction *mDualCheckerboardAction;
	
	//five preservation
	QWidget *mPentagonPreservingWidget;
  QWidget *mDualPentagonPreservingWidget;
	
	QGridLayout *mPentagonPreservingLayout;
  QGridLayout *mDualPentagonPreservingLayout;

	QAction *mPentagonPreservingAction;
  QAction *mDualPentagonPreservingAction;

	//six preservation
	QWidget *mDualLoopStyleRemeshingSixWidget;
  QWidget *mLoopStyleRemeshingSixWidget;

	QGridLayout *mDualLoopStyleRemeshingSixLayout;
  QGridLayout *mLoopStyleRemeshingSixLayout;

	QAction *mDualLoopStyleRemeshingSixAction;
  QAction *mLoopStyleRemeshingSixAction;	
	
	//miscellaneous
	QWidget *mFractalWidget;
  QWidget *mDoubleStellateMiscWidget;
  QWidget *mDooSabinBCWidget;
  QWidget *mDooSabinBCNewWidget;
  QWidget *mDomeWidget;
  QWidget *mSubdivideFaceWidget;
	
	QGridLayout *mFractalLayout;
  QGridLayout *mDoubleStellateMiscLayout;
  QGridLayout *mDooSabinBCLayout;
  QGridLayout *mDooSabinBCNewLayout;
  QGridLayout *mDomeLayout;
  QGridLayout *mSubdivideFaceLayout;
	
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

	QDoubleSpinBox *createDoubleSpinBox(QGridLayout *layout, QLabel *label, QString s, double low, double high, double step, double value, double decimals, int row, int col);

public slots:

	void triggerDual();

	void triggerRootThree();
	void triggerTriangulate();
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
	void triggerLoopStyleRemeshingSix();

	void triggerFractal();
	void triggerDoubleStellateMisc();
	void triggerDooSabinBC();
	void triggerDooSabinBCNew();
	void triggerDome();
	void triggerSubdivideFace();
		
private:
	
	QWidget *mParent;
	
	QMenu *mRemeshingMenu;
	QMenu *mThreeConversionMenu;
	QMenu *mFourConversionMenu;
	QMenu *mFiveConversionMenu;	
	QMenu *mThreePreservationMenu;
	QMenu *mFourPreservationMenu;
	QMenu *mFivePreservationMenu;
	QMenu *mSixPreservationMenu;
	QMenu *mMiscellaneousMenu;
	
	QDoubleSpinBox *starSpinBox;
	QDoubleSpinBox *twelveSixFourSpinBox;
	QDoubleSpinBox *vertexTruncationSpinBox;
	QDoubleSpinBox *dualTwelveSixFourSpinBox;
	QDoubleSpinBox *pentagonalizationSpinBox;
	QDoubleSpinBox *cubicPentagonalizationSpinBox;
	QDoubleSpinBox *loopStyleRemeshingSpinBox;
	QDoubleSpinBox *dualLoopStyleRemeshingTwistSpinBox;
	QDoubleSpinBox *dualLoopStyleRemeshingWeightSpinBox;
	QDoubleSpinBox *pentagonPreservingSpinBox;
	QDoubleSpinBox *dualLoopStyleRemeshingSixTwistSpinBox;
	QDoubleSpinBox *dualLoopStyleRemeshingSixWeightSpinBox;
	QDoubleSpinBox *loopStyleRemeshingSixSpinBox;
	QDoubleSpinBox *fractalMultiplierSpinBox;
	QDoubleSpinBox *doubleStellateMiscHeightSpinBox;
	QDoubleSpinBox *doubleStellateMiscCurveSpinBox;
	QDoubleSpinBox *dooSabinBCNewScaleSpinBox;
	QDoubleSpinBox *dooSabinBCNewLengthSpinBox;
	QDoubleSpinBox *domeHeightSpinBox;
	QDoubleSpinBox *domeScaleSpinBox;

	QLabel *starLabel;
	QLabel *twelveSixFourLabel;
	QLabel *vertexTruncationLabel;
	QLabel *dualTwelveSixFourLabel;
	QLabel *pentagonalizationLabel;
	QLabel *cubicPentagonalizationLabel;
	QLabel *loopStyleRemeshingLabel;
	QLabel *dualLoopStyleRemeshingTwistLabel;
	QLabel *dualLoopStyleRemeshingWeightLabel;
	QLabel *pentagonPreservingLabel;
	QLabel *dualLoopStyleRemeshingSixTwistLabel;
	QLabel *dualLoopStyleRemeshingSixWeightLabel;
	QLabel *loopStyleRemeshingSixLabel;
	QLabel *fractalMultiplierLabel;
	QLabel *doubleStellateMiscHeightLabel;
	QLabel *doubleStellateMiscCurveLabel;
	QLabel *dooSabinBCNewLengthLabel;
	QLabel *dooSabinBCNewScaleLabel;
	QLabel *domeHeightLabel;
	QLabel *domeScaleLabel;
	
};

#endif
