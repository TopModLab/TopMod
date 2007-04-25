/****************************************************************************
 **
 **
 ****************************************************************************/

#ifndef EXTRUSIONMODE_H
#define EXTRUSIONMODE_H

#include <QWidget>

#include <QBoxLayout>
#include <QStackedWidget>
#include <QComboBox>

#include "MainWindow.hh"

class MainWindow;
class DLFLWindow;

class QComboBox;
class QStackedWidget;
class QGroupBox;
class QVBoxLayout;
class QCheckBox;
class QLabel;

class ExtrusionMode : public QWidget {
	Q_OBJECT

public:
	ExtrusionMode(QWidget *parent = 0);
	void insertTab(QTabWidget *tabWidget);
	int getLastMode();
	
protected:
	void setupDooSabinExtrude();
	void setupCubicalExtrude();
	void setupDodecahedralExtrude();
	void setupIcosahedralExtrude();
	void setupOctahedralExtrude();
	void setupStellateExtrude();
	void setupDoubleStellateExtrude();
	
public slots:
	void switchMode(int index);
	void setLength(double value);
	void setRotation(int value);
	void setSegments(int value);
	void setScale(double value);
	
private:
	
	int lastMode;
	
	QLabel *extrusionLabel;
	QGroupBox *extrusionGroup;
	QVBoxLayout *extrusionBoxLayout;
	QComboBox *extrusionComboBox;
	
	QStackedWidget *stackedWidget;
	
	QWidget *dooSabinExtrudeWidget;
	QWidget *cubicalExtrudeWidget;
	QWidget *dodecahedralExtrudeWidget;
	QWidget *icosehedralExtrudeWidget;
	QWidget *octahedralExtrudeWidget;
	QWidget *stellateExtrudeWidget;
	QWidget *doubleStellateExtrudeWidget;
	
	QVBoxLayout *dooSabinExtrudeLayout;
	QVBoxLayout *cubicalExtrudeLayout;
	QVBoxLayout *dodecahedralExtrudeLayout;
	QVBoxLayout *icosahedralExtrudeLayout; 
	QVBoxLayout *octahedralExtrudeLayout;
	QVBoxLayout *stellateExtrudeLayout; 
	QVBoxLayout *doubleStellateExtrudeLayout;
	
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