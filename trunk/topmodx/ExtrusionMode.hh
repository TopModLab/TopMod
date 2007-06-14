/****************************************************************************
 **
 **
 ****************************************************************************/

#ifndef EXTRUSIONMODE_H
#define EXTRUSIONMODE_H

#include <QWidget>

#include "MainWindow.hh"

class MainWindow;

class ExtrusionMode : public QWidget {
	Q_OBJECT

public:
	ExtrusionMode(QWidget *parent , QShortcutManager *sm, QWidget *actionList);
	void addActions(QActionGroup *actionGroup, QToolBar *toolBar, QStackedWidget *stackedWidget);
	QMenu* getMenu();
	
	QAction *mDooSabinExtrudeAction;
	QAction *mCubicalExtrudeAction;
	QAction *mDodecahedralExtrudeAction;
	QAction *mIcosahedralExtrudeAction;
	QAction *mOctahedralExtrudeAction;
	QAction *mStellateExtrudeAction;
	QAction *mDoubleStellateExtrudeAction;	

	QWidget *mDooSabinExtrudeWidget;
	QWidget *mCubicalExtrudeWidget;
	QWidget *mDodecahedralExtrudeWidget;
	QWidget *mIcosahedralExtrudeWidget;
	QWidget *mOctahedralExtrudeWidget;
	QWidget *mStellateExtrudeWidget;
	QWidget *mDoubleStellateExtrudeWidget;
	
	QBoxLayout *mDooSabinExtrudeLayout;
	QBoxLayout *mCubicalExtrudeLayout;
	QBoxLayout *mDodecahedralExtrudeLayout;
	QBoxLayout *mIcosahedralExtrudeLayout;
	QBoxLayout *mOctahedralExtrudeLayout;
	QBoxLayout *mStellateExtrudeLayout; 
	QBoxLayout *mDoubleStellateExtrudeLayout;
	
protected:
	void setupDooSabinExtrude();
	void setupCubicalExtrude();
	void setupDodecahedralExtrude();
	void setupIcosahedralExtrude();
	void setupOctahedralExtrude();
	void setupStellateExtrude();
	void setupDoubleStellateExtrude();
	
public slots:

	void setLength(double value);
	void setRotation(int value);
	void setSegments(int value);
	void setScale(double value);
	
	void triggerDooSabinExtrude();
	void triggerCubicalExtrude();
	void triggerDodecahedralExtrude();
	void triggerIcosahedralExtrude();
	void triggerOctahedralExtrude();
	void triggerStellateExtrude();
	void triggerDoubleStellateExtrude();
	
private:
	
	QWidget *mParent;
	QMenu *mExtrusionMenu;

	//doo sabin extrude
	QLabel *dooSabinLengthLabel;
	QDoubleSpinBox *dooSabinLengthSpinBox;
	QLabel *dooSabinRotationLabel;
	QSpinBox *dooSabinRotationSpinBox;
	QLabel *dooSabinScaleLabel;
	QDoubleSpinBox *dooSabinScaleSpinBox;
	QLabel *dooSabinSegmentsLabel;
	QSpinBox *dooSabinSegmentsSpinBox;
	
	//cubic extrude
	QLabel *cubicalLengthLabel;
	QDoubleSpinBox *cubicalLengthSpinBox;
	QLabel *cubicalRotationLabel;
	QSpinBox *cubicalRotationSpinBox;
	QLabel *cubicalScaleLabel;
	QDoubleSpinBox *cubicalScaleSpinBox;
	QLabel *cubicalSegmentsLabel;
	QSpinBox *cubicalSegmentsSpinBox;
	
	//dodecahedral extrude
	QLabel *dodecahedralLengthLabel;
	QDoubleSpinBox *dodecahedralLengthSpinBox;
	QLabel *dodecahedralScaleLabel;
	QDoubleSpinBox *dodecahedralScaleSpinBox;
	QLabel *dodecahedralSegmentsLabel;
	QSpinBox *dodecahedralSegmentsSpinBox;
	QCheckBox *hexagonalizeCheckBox;
		
	//icosahedral extrude
	QLabel *icosahedralLengthLabel;
	QDoubleSpinBox *icosahedralLengthSpinBox;
	QLabel *icosahedralScaleLabel;
	QDoubleSpinBox *icosahedralScaleSpinBox;
	QLabel *icosahedralSegmentsLabel;
	QSpinBox *icosahedralSegmentsSpinBox;
	
	//octahedral extrude
	QLabel *octahedralLengthLabel;
	QDoubleSpinBox *octahedralLengthSpinBox;
	QLabel *octahedralRotationLabel;
	QSpinBox *octahedralRotationSpinBox;
	QLabel *octahedralScaleLabel;
	QDoubleSpinBox *octahedralScaleSpinBox;
	QLabel *octahedralSegmentsLabel;
	QSpinBox *octahedralSegmentsSpinBox;
	QCheckBox *meshFlatEdgesCheckBox;
	
	//stellate extrude
	QLabel *stellateLengthLabel;
	QDoubleSpinBox *stellateLengthSpinBox;
	
	//double stellate
	QLabel *doubleStellateLengthLabel;
	QDoubleSpinBox *doubleStellateLengthSpinBox;
	
};

#endif
