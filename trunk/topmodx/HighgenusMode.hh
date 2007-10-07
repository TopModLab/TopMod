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

#ifndef HIGHGENUSMODE_H
#define HIGHGENUSMODE_H

/*!
	\file HighgenusMode.hh
	\brief Definition of the HighgenusMode class
	
	\see HighgenusMode
*/

#include "MainWindow.hh"
class MainWindow;

class HighgenusMode : public QWidget {
	Q_OBJECT

public:
	HighgenusMode(QWidget *parent, QShortcutManager *sm, QWidget *actionList);
	void addActions(QActionGroup *actionGroup, QToolBar *toolBar, QStackedWidget *stackedWidget);
	QMenu* getMenu();
	void retranslateUi();
	
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
	
	QGridLayout *mAddHoleHandleLayout;
	QGridLayout *mAddHoleHandleCVLayout;
	QGridLayout *mAddHandleSILayout;
	QGridLayout *mRindModelingScalingLayout;
	QGridLayout *mRindModelingThicknessLayout;
	QGridLayout *mWireframeModelingLayout;
	QGridLayout *mWireframeModeling2Layout;
	QGridLayout *mColumnModelingLayout; 
	QGridLayout *mSierpinskyLayout;
	QGridLayout *mMultiFaceHandleLayout; 
	QGridLayout *mMengerSpongeLayout;
	

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
	
	QDoubleSpinBox *createDoubleSpinBox(QGridLayout *layout, QLabel *label, QString s, double low, double high, double step, double value, double decimals, int row, int col);
	
public slots:
	void numSegmentsValueChanged(double value);
	void changeMultiFaceAlgorithm(bool on);
	void toggleMultiFaceHandleUseMaxOffsetFlag(int state);
	void toggleSymmetricWeightsFlag(int state);
	
	void toggleCrustCleanupFlag(int state);
	void numSegmentsConnectValueChanged(double value);
	
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

	QLabel *addHoleHandleNumSegmentsConnectLabel;
	QLabel *addHoleHandleNumSegmentsLabel;
	QLabel *addHoleHandleCVNumSegmentsLabel;
	QLabel *addHoleHandleCVNumSegmentsConnectLabel;
	QLabel *addHandleSINumSegmentsLabel;
	QLabel *addHandleSINumSegmentsConnectLabel;
	QLabel *addHandleSIWeight1Label;
	QLabel *addHandleSIWeight2Label;
	QLabel *addHandleSITwistsConnectLabel;
	QLabel *rindModelingScalingLabel;
	QLabel *rindModelingThicknessLabel;
	QLabel *wireframeModelingThicknessLabel;
	QLabel *wireframeModeling2ThicknessLabel;
	QLabel *wireframeModeling2WidthLabel;
	QLabel *columnModelingThicknessLabel;
	QLabel *mengerSpongeThicknessLabel;
	QLabel *mengerSpongeThresholdLabel;
	QLabel *columnModelingNumSegmentsLabel;
	
	
	QGridLayout *mMultiFaceAlgorithmLayout;
	
	QButtonGroup *multiFaceAlgorithmButtonGroup;
	
	QCheckBox *rindModelingThicknessCleanupCheckBox;
	QCheckBox *rindModelingScalingCleanupCheckBox;

	QDoubleSpinBox *addHoleHandleNumSegmentsConnectSpinBox;
	QDoubleSpinBox *addHoleHandleNumSegmentsSpinBox;
	
	QDoubleSpinBox *addHoleHandleCVNumSegmentsSpinBox;
	QDoubleSpinBox *addHoleHandleCVNumSegmentsConnectSpinBox;
	
	QDoubleSpinBox *addHandleSINumSegmentsSpinBox;
	QDoubleSpinBox *addHandleSINumSegmentsConnectSpinBox;
	
	QDoubleSpinBox *addHandleSITwistsConnectSpinBox;
	QDoubleSpinBox *addHandleSIWeight1SpinBox;
	QDoubleSpinBox *addHandleSIWeight2SpinBox;
	
	QDoubleSpinBox *rindModelingScalingSpinBox;
	QDoubleSpinBox *rindModelingThicknessSpinBox;
	QDoubleSpinBox *wireframeModelingThicknessSpinBox;
	QDoubleSpinBox *wireframeModeling2ThicknessSpinBox;
	QDoubleSpinBox *wireframeModeling2WidthSpinBox;
	QDoubleSpinBox *columnModelingThicknessSpinBox;
	QDoubleSpinBox *columnModelingNumSegmentsSpinBox;
	QDoubleSpinBox *mengerSpongeThicknessSpinBox;
	QDoubleSpinBox *mengerSpongeThresholdSpinBox;
		
	QLabel *multiFaceHandleExtrudeDistanceLabel;
	QLabel *multiFaceHandleScaleLabel;
	QDoubleSpinBox *multiFaceHandleExtrudeDistanceSpinBox;
	QCheckBox *multiFaceHandleMaxOffsetsCheckBox;
	QDoubleSpinBox *multiFaceHandleScaleSpinBox;
	
	QCheckBox *addHandleSISymmetricWeightsCheckBox;
	QPushButton *rindModelingScalingCreateCrustButton;
	QPushButton *rindModelingThicknessCreateButton;
	QPushButton *wireframeModelingCreateButton;
	QPushButton *wireframeModeling2CreateButton;
	QPushButton *columnModelingCreateButton;
	QPushButton *sierpinskyButton;
	QGroupBox *multiFaceAlgorithmGroupBox;
	QPushButton *multiFaceHandleButton;
	QCheckBox *mengerSpongFractionalThicknessCheckBox;
	QPushButton *mengerSpongeButton;
	
	
};

#endif
