/****************************************************************************
 **
 **
 ****************************************************************************/

#ifndef EXTRUSIONMODE_H
#define EXTRUSIONMODE_H

#include <QWidget>

/*!
	\file ExtrusionsMode.hh
	\brief Definition of the ExtrusionsMode class
	
	\see ExtrusionsMode
*/

#include "MainWindow.hh"

class MainWindow;

class ExtrusionsMode : public QWidget {
	Q_OBJECT

public:
	ExtrusionsMode(QWidget *parent , QShortcutManager *sm, QWidget *actionList);
	void addActions(QActionGroup *actionGroup, QToolBar *toolBar, QStackedWidget *stackedWidget);
	QMenu* getMenu();
	void retranslateUi();
	
	QAction *mDooSabinExtrudeAction;
	QAction *mCubicalExtrudeAction;
	QAction *mDodecahedralExtrudeAction;
	QAction *mIcosahedralExtrudeAction;
	QAction *mOctahedralExtrudeAction;
	QAction *mStellateExtrudeAction;
	QAction *mDoubleStellateExtrudeAction;	
	QAction *mDomeExtrudeAction;	

	QWidget *mDooSabinExtrudeWidget;
	QWidget *mCubicalExtrudeWidget;
	QWidget *mDodecahedralExtrudeWidget;
	QWidget *mIcosahedralExtrudeWidget;
	QWidget *mOctahedralExtrudeWidget;
	QWidget *mStellateExtrudeWidget;
	QWidget *mDoubleStellateExtrudeWidget;
	QWidget *mDomeExtrudeWidget;
	
	QGridLayout *mDooSabinExtrudeLayout;
	QGridLayout *mCubicalExtrudeLayout;
	QGridLayout *mDodecahedralExtrudeLayout;
	QGridLayout *mIcosahedralExtrudeLayout;
	QGridLayout *mOctahedralExtrudeLayout;
	QGridLayout *mStellateExtrudeLayout; 
	QGridLayout *mDoubleStellateExtrudeLayout;
	QGridLayout *mDomeExtrudeLayout;
	
	QWidget *mParent;
	QMenu *mExtrusionMenu;

	//doo sabin extrude
	QLabel *dooSabinLengthLabel;
	QDoubleSpinBox *dooSabinLengthSpinBox;
	QLabel *dooSabinRotationLabel;
	QDoubleSpinBox *dooSabinRotationSpinBox;
	QLabel *dooSabinScaleLabel;
	QDoubleSpinBox *dooSabinScaleSpinBox;
	QLabel *dooSabinSegmentsLabel;
	QDoubleSpinBox *dooSabinSegmentsSpinBox;
	
	//cubic extrude
	QLabel *cubicalLengthLabel;
	QDoubleSpinBox *cubicalLengthSpinBox;
	QLabel *cubicalRotationLabel;
	QDoubleSpinBox *cubicalRotationSpinBox;
	QLabel *cubicalScaleLabel;
	QDoubleSpinBox *cubicalScaleSpinBox;
	QLabel *cubicalSegmentsLabel;
	QDoubleSpinBox *cubicalSegmentsSpinBox;
	
	//dodecahedral extrude
	QLabel *dodecahedralLengthLabel;
	QDoubleSpinBox *dodecahedralLengthSpinBox;
	QLabel *dodecahedralScaleLabel;
	QDoubleSpinBox *dodecahedralScaleSpinBox;
	QLabel *dodecahedralLength1Label;
	QDoubleSpinBox *dodecahedralLength1SpinBox;
	QLabel *dodecahedralLength2Label;
	QDoubleSpinBox *dodecahedralLength2SpinBox;
	QLabel *dodecahedralLength3Label;
	QDoubleSpinBox *dodecahedralLength3SpinBox;
	QLabel *dodecahedralAngleLabel;
	QDoubleSpinBox *dodecahedralAngleSpinBox;
	QLabel *dodecahedralSegmentsLabel;
	QDoubleSpinBox *dodecahedralSegmentsSpinBox;
	QCheckBox *hexagonalizeCheckBox;
		
	//icosahedral extrude
	QLabel *icosahedralLengthLabel;
	QDoubleSpinBox *icosahedralLengthSpinBox;
	QLabel *icosahedralScaleLabel;
	QDoubleSpinBox *icosahedralScaleSpinBox;
	QLabel *icosahedralSegmentsLabel;
	QDoubleSpinBox *icosahedralSegmentsSpinBox;
	QLabel *icosahedralLength1Label;
	QDoubleSpinBox *icosahedralLength1SpinBox;
	QLabel *icosahedralLength2Label;
	QDoubleSpinBox *icosahedralLength2SpinBox;
	QLabel *icosahedralLength3Label;
	QDoubleSpinBox *icosahedralLength3SpinBox;
	QLabel *icosahedralAngleLabel;
	QDoubleSpinBox *icosahedralAngleSpinBox;
	
	
	//octahedral extrude
	QLabel *octahedralLengthLabel;
	QDoubleSpinBox *octahedralLengthSpinBox;
	QLabel *octahedralRotationLabel;
	QDoubleSpinBox *octahedralRotationSpinBox;
	QLabel *octahedralScaleLabel;
	QDoubleSpinBox *octahedralScaleSpinBox;
	QLabel *octahedralSegmentsLabel;
	QDoubleSpinBox *octahedralSegmentsSpinBox;
	QCheckBox *meshFlatEdgesCheckBox;
	
	//stellate extrude
	QHBoxLayout *stellateLengthLayout;
	QLabel *stellateLengthLabel;
	QDoubleSpinBox *stellateLengthSpinBox;
	
	//double stellate
	QHBoxLayout *doubleStellateLengthLayout;
	QLabel *doubleStellateLengthLabel;
	QDoubleSpinBox *doubleStellateLengthSpinBox;
	
	//dome extrude
	QLabel *domeHeightLabel;
	QLabel *domeScaleLabel;
	QDoubleSpinBox *domeHeightSpinBox;
	QDoubleSpinBox *domeScaleSpinBox;
	
	
protected:
	void setupDooSabinExtrude();
	void setupCubicalExtrude();
	void setupDodecahedralExtrude();
	void setupIcosahedralExtrude();
	void setupOctahedralExtrude();
	void setupStellateExtrude();
	void setupDoubleStellateExtrude();
	void setupDomeExtrude();
	QDoubleSpinBox *createDoubleSpinBox(QGridLayout *layout, QLabel *label, QString s, double low, double high, double step, double value, double decimals, int row, int col);
	
	
public slots:

	void setLength(double value);
	void setLength1(double value);
	void setLength2(double value);
	void setLength3(double value);
	void setAngle(double value);
	void setLength1Icosa(double value);
	void setLength2Icosa(double value);
	void setLength3Icosa(double value);
	void setAngleIcosa(double value);
	void setRotation(double value);
	void setSegments(double value);
	void setScale(double value);
	
	void triggerDooSabinExtrude();
	void triggerCubicalExtrude();
	void triggerDodecahedralExtrude();
	void triggerIcosahedralExtrude();
	void triggerOctahedralExtrude();
	void triggerStellateExtrude();
	void triggerDoubleStellateExtrude();	
	void triggerDomeExtrude();	
	
};

#endif
