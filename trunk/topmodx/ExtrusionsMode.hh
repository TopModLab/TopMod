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
	QLabel *dooSabinTwistLabel;
	QDoubleSpinBox *dooSabinTwistSpinBox;
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
	QLabel *domeRotationLabel;
	QLabel *domeScaleLabel;
	QDoubleSpinBox *domeHeightSpinBox;
	QDoubleSpinBox *domeRotationSpinBox;
	QDoubleSpinBox *domeScaleSpinBox;
	
	QPushButton *performDooSabinExtrusionButton;
	QPushButton *performCubicalExtrusionButton;
	QPushButton *performDodecahedralExtrusionButton;
	QPushButton *performIcosahedralExtrusionButton;
	QPushButton *performOctahedralExtrusionButton;
	QPushButton *performStellateExtrusionButton;
	QPushButton *performDoubleStellateExtrusionButton;
	QPushButton *domeButton;
	
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
