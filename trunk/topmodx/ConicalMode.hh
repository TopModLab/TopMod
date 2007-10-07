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

#ifndef CONICALMODE_H
#define CONICALMODE_H

#include <QWidget>

#include <QGridLayout>
#include <QStackedWidget>
#include <QComboBox>

#include "MainWindow.hh"

class MainWindow;

class ConicalMode : public QWidget {
	Q_OBJECT

public:
	ConicalMode(QWidget *parent, QShortcutManager *sm);
	void addActions(QActionGroup *actionGroup, QToolBar *toolBar, QStackedWidget *stackedWidget);
	QMenu *getMenu();
	void retranslateUi();
	
	QDoubleSpinBox *createDoubleSpinBox(QGridLayout *layout, QLabel *label, QString s, double low, double high, double step, double value, double decimals, int row, int col);

	QWidget *mCutbyEdgeWidget;
	QWidget *mCutbyVertexWidget;
	QWidget *mCutbyEdgeVertexWidget;
	QWidget *mCutbyFaceWidget;
	QWidget *mTruncateEdgeWidget;
	QWidget *mTruncateVertexWidget; 
	QWidget *mDualConvexHullWidget; 

	QAction *mCutbyEdgeAction;
	QAction *mCutbyVertexAction;
	QAction *mCutbyEdgeVertexAction;
	QAction *mCutbyFaceAction;
	QAction *mTruncateEdgeAction;
	QAction *mTruncateVertexAction; 
	QAction *mDualConvexHullAction;
	
	QGridLayout *mCutbyEdgeLayout;
	QGridLayout *mCutbyVertexLayout;
	QGridLayout *mCutbyEdgeVertexLayout;
	QGridLayout *mCutbyFaceLayout;
	QGridLayout *mTruncateEdgeLayout;
	QGridLayout *mTruncateVertexLayout; 
	QGridLayout *mDualConvexHullLayout;
	
protected:
	void setupCutbyEdge();
	void setupCutbyVertex();
	void setupCutbyEdgeVertex();
	void setupCutbyFace();
	void setupTruncateEdge();
	void setupTruncateVertex();
	void setupDualConvexHull();
	
public slots:
	void changeCutOffsetE(double value);
	void changeCutOffsetV(double value);
	void toggleGlobalCut(int state);
	void toggleSelectedCut(int state);
	void changeTiltPlane1(double value);
	void changeTiltPlane2(double value);
	
	void triggerCutbyEdge();
	void triggerCutbyVertex();
	void triggerCutbyEdgeVertex();
	void triggerCutbyFace();
	void triggerTruncateEdge();
	void triggerTruncateVertex();
	void triggerDualConvexHull();

private: 

	QWidget *mParent;
	QMenu *mConicalMenu;

	QDoubleSpinBox *cutbyEdgeOffsetSpinBox;
	QDoubleSpinBox *cutbyEdgeTiltPlane1SpinBox;
	QDoubleSpinBox *cutbyEdgeTiltPlane2SpinBox;
	QDoubleSpinBox *cutbyVertexOffsetSpinBox;
	QDoubleSpinBox *cutbyEdgeVertexOffsetESpinBox;
	QDoubleSpinBox *cutbyEdgeVertexOffsetVSpinBox;
	QDoubleSpinBox *cutbyEdgeVertexTiltPlane1SpinBox;
	QDoubleSpinBox *cutbyEdgeVertexTiltPlane2SpinBox;
	QDoubleSpinBox *cutbyFaceOffsetSpinBox;
	QDoubleSpinBox *truncateEdgeOffsetSpinBox;
	QDoubleSpinBox *truncateVertexOffsetSpinBox;

	QCheckBox *cutbyEdgeGlobalCheckBox;
	QCheckBox *cutbyEdgeCutSelectedCheckBox;
	QCheckBox *cutbyVertexGlobalCheckBox;
	QCheckBox *cutbyVertexCutSelectedCheckBox;
	QCheckBox *cutbyEdgeVertexGlobalCheckBox;
	QCheckBox *cutbyEdgeVertexCutSelectedCheckBox;
	QCheckBox *cutbyFaceGlobalCheckBox;
	QCheckBox *cutbyFaceCutSelectedCheckBox;
	QCheckBox *truncateEdgeGlobalCheckBox;
	QCheckBox *truncateEdgeCutSelectedCheckBox;
	QCheckBox *truncateVertexGlobalCheckBox;
	QCheckBox *truncateVertexCutSelectedCheckBox;
	
	QLabel *cutbyEdgeOffsetLabel;
	QLabel *cutbyVertexOffsetLabel;
	QLabel *cutbyFaceOffsetLabel;
	
	QPushButton *performCuttingEdgeButton;
	QPushButton *performCuttingFaceButton;
	QPushButton *performCuttingVertexButton;
	QPushButton *dualConvexHullButton;
	QPushButton *convexHullButton;
};

#endif
