/****************************************************************************
 **
 **
 ****************************************************************************/

#ifndef HIGHGENUSMODE_H
#define HIGHGENUSMODE_H

#include "MainWindow.hh"

class MainWindow;
// class DLFLWindow;

class HighgenusMode : public QWidget {
	Q_OBJECT

public:
	HighgenusMode(QWidget *parent, QShortcutManager *sm, QWidget *actionList);
	void addActions(QActionGroup *actionGroup, QToolBar *toolBar, QStackedWidget *stackedWidget);
	QMenu* getMenu();
	
	QAction *mAddHoleHandleAction;
	QAction *mAddHoleHandleCVAction;
	QAction *mAddHandleSIAction;
	QAction *mRindModelingScalingAction; 
	QAction *mRindModelingThicknessAction;
	QAction *mWireframeModelingAction; 
	QAction *mWireframeModeling2Action; 
	QAction *mColumnModelingAction;	
	QAction *mSierpinskyAction;
	QAction *mMultiFaceHandleAction;
	QAction *mMengerSpongeAction;
	
	QWidget *mAddHoleHandleWidget;
	QWidget *mAddHoleHandleCVWidget;
	QWidget *mAddHandleSIWidget;
	QWidget *mRindModelingScalingWidget; 
	QWidget *mRindModelingThicknessWidget;
	QWidget *mWireframeModelingWidget; 
	QWidget *mWireframeModeling2Widget; 
	QWidget *mColumnModelingWidget;	
	QWidget *mSierpinskyWidget;
	QWidget *mMultiFaceHandleWidget;
	QWidget *mMengerSpongeWidget;
	
	QBoxLayout *mAddHoleHandleLayout;
	QBoxLayout *mAddHoleHandleCVLayout;
	QBoxLayout *mAddHandleSILayout;
	QBoxLayout *mRindModelingScalingLayout;
	QBoxLayout *mRindModelingThicknessLayout;
	QBoxLayout *mWireframeModelingLayout;
	QBoxLayout *mWireframeModeling2Layout;
	QBoxLayout *mColumnModelingLayout; 
	QBoxLayout *mSierpinskyLayout;
	QBoxLayout *mMultiFaceHandleLayout; 
	QBoxLayout *mMengerSpongeLayout;
	

protected:
	void setupAddHoleHandle();
	void setupAddHoleHandleCV();
	void setupAddHandleSI();
	void setupRindModelingScaling();
	void setupRindModelingThickness();
	void setupWireframeModeling();
	void setupWireframeModeling2();
	void setupColumnModeling();
	void setupSierpinsky();
	void setupMultiFaceHandle();
	void setupMengerSponge();
	
public slots:
	void numSegmentsValueChanged(int value);
	void changeMultiFaceAlgorithm(bool on);
	void toggleMultiFaceHandleUseMaxOffsetFlag(int state);
	void toggleSymmetricWeightsFlag(int state);
	
	void toggleCrustCleanupFlag(int state);
	void numSegmentsConnectValueChanged(int value);
	
	void triggerAddHoleHandle();
	void triggerAddHoleHandleCV();
	void triggerAddHandleSI();
	void triggerRindModelingScaling();
	void triggerRindModelingThickness();
	void triggerWireframeModeling();
	void triggerWireframeModeling2();
	void triggerColumnModeling();
	void triggerSierpinsky();
	void triggerMultiFaceHandle();
	void triggerMengerSponge();

private:
	
	QWidget *mParent;
	QMenu *mHighgenusMenu;

	QBoxLayout *mMultiFaceAlgorithmLayout;
	
	QButtonGroup *multiFaceAlgorithmButtonGroup;
	
	QCheckBox *rindModelingThicknessCleanupCheckBox;
	QCheckBox *rindModelingScalingCleanupCheckBox;

	QSpinBox *addHoleHandleNumSegmentsConnectSpinBox;
	QSpinBox *addHoleHandleNumSegmentsSpinBox;
	
	QSpinBox *addHoleHandleCVNumSegmentsSpinBox;
	QSpinBox *addHoleHandleCVNumSegmentsConnectSpinBox;
	
	QSpinBox *addHandleSINumSegmentsSpinBox;
	QSpinBox *addHandleSINumSegmentsConnectSpinBox;
	
	QSpinBox *addHandleSITwistsConnectSpinBox;
	QDoubleSpinBox *addHandleSIWeight2SpinBox;
	
	QLabel *multiFaceHandleExtrudeDistanceLabel;
	QLabel *multiFaceHandleScaleLabel;
	QDoubleSpinBox *multiFaceHandleExtrudeDistanceSpinBox;
	QCheckBox *multiFaceHandleMaxOffsetsCheckBox;
	QDoubleSpinBox *multiFaceHandleScaleSpinBox;
};

#endif
