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

#ifndef EXPERIMENTALMODE_H
#define EXPERIMENTALMODE_H

#include <QWidget>

#include <QGridLayout>
#include <QStackedWidget>
#include <QComboBox>

#include "MainWindow.hh"

class MainWindow;

class ExperimentalMode : public QWidget {
	Q_OBJECT

public:
	ExperimentalMode(QWidget *parent, QShortcutManager *sm,  QWidget *actionList);
	void addActions(QActionGroup *actionGroup, QToolBar *toolBar, QStackedWidget *stackedWidget);
	QMenu *getMenu();
	void retranslateUi();
	
	QDoubleSpinBox *createDoubleSpinBox(QGridLayout *layout, QLabel *label, QString s, double low, double high, double step, double value, double decimals, int row, int col);

	QPushButton* addColorPreference(QLabel *label, QString text, QColor color, QGridLayout *layout, QStyle *style, int row, int col);

	void setButtonColor(QColor color, QPushButton *button);
	void setPaintBucketColor(QColor c);

	QWidget *mPaintBucketWidget;
	QAction *mPaintBucketAct;
	QGridLayout *mPaintBucketLayout;
	
protected:
	void setupPaintBucket();
	
public slots:
	// void changeCutOffsetE(double value);
	void triggerPaintBucket();
	void setPaintBucketColor();

private: 

	QWidget *mParent;
	QMenu *mExperimentalMenu;

	//Face Painting Color
	QColor mPaintBucketColor, mPaintBucketColorDefault;
	QPushButton *mPaintBucketColorButton;
	QPushButton *mPaintSelectedFacesButton;
	QPushButton *mResetMaterialsButton;
	QLabel *mPaintBucketColorLabel;


	QStyle* colorPickerStyle;
	QPalette p;

};
#endif
