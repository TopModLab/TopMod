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
*
* Short description of this file
*
* name of .hh file containing function prototypes
*
*/

#include <QtGui>

#include "ExperimentalModes.hh"

/*!
	\ingroup gui
	@{
	
	\class ExperimentalMode
	\brief experimental stuff like paint bucket
	
	\note 
	
	\see PaintBucket
*/

/*!
* \brief Constructor
* 
* @param parent the MainWindow widget
* @param sm the shortcut manager class for adding a custom shortcut to each menu action or icon
* @param actionList the master list of actions for use with the CommandCompleter class
* 
*/
ExperimentalMode::ExperimentalMode(QWidget *parent, QShortcutManager *sm, QWidget *actionList)
		: QWidget(parent)
{		
	setParent(0);
	mParent = parent;
	
	mPaintBucketWidget = new QWidget;
	
	setupPaintBucket();
	
	
	
	mPaintBucketAct = new QAction(QIcon(":images/color-fill.png"),tr("Paint Bucket"),this);
	mPaintBucketAct->setCheckable(true);
	sm->registerAction(mPaintBucketAct, "Experimental", "");
	mPaintBucketAct->setStatusTip(tr("Enter Paint Bucket Mode"));
	mPaintBucketAct->setToolTip(tr("Paint Bucket Mode"));
	connect(mPaintBucketAct, SIGNAL(triggered()), this, SLOT(triggerPaintBucket()));
	actionList->addAction(mPaintBucketAct);
	
}

QMenu* ExperimentalMode::getMenu(){
	mExperimentalMenu = new QMenu(tr("Experimental"));
	
	mExperimentalMenu->addAction(mPaintBucketAct);
	
	return mExperimentalMenu;
	
}

void ExperimentalMode::addActions(QActionGroup *actionGroup, QToolBar *toolBar, QStackedWidget *stackedWidget){
	
	actionGroup->addAction(mPaintBucketAct);
	
	toolBar->addAction(mPaintBucketAct);
	
	stackedWidget->addWidget(mPaintBucketWidget);
	
}

void ExperimentalMode::triggerPaintBucket(){
	
	((MainWindow*)mParent)->setToolOptions(mPaintBucketWidget);
	((MainWindow*)mParent)->setMode(MainWindow::PaintFace);
}


QDoubleSpinBox *ExperimentalMode::createDoubleSpinBox(QGridLayout *layout, QLabel *label, QString s, double low, double high, double step, double value, double decimals, int row, int col){
	label->setText(s);
	QDoubleSpinBox *spinbox = new QDoubleSpinBox(this);
	spinbox->setAccelerated(true);
	spinbox->setRange(low, high);
	spinbox->setSingleStep(step);
	spinbox->setValue(value);
	spinbox->setDecimals(decimals);
	spinbox->setMaximumSize(75,25);
	layout->addWidget(label,row,col);
  layout->addWidget(spinbox,row,col+1);

	return spinbox;
}

QPushButton* ExperimentalMode::addColorPreference(QLabel *label, QString text, QColor color, QGridLayout *layout, QStyle *style, int row, int col){
	
	label->setText(text);
	QPushButton *button = new QPushButton(this);
	button->setMaximumSize(QSize(100,25));
	button->setAutoFillBackground(true);
	button->setStyle(style);
	QPalette p = button->palette();
	p.setColor(button->backgroundRole(), color);
	button->setPalette(p);	
	layout->addWidget(label,row,col);
	layout->addWidget(button,row,col+1);
	return button;
}


void ExperimentalMode::setupPaintBucket(){
	
	colorPickerStyle = new QPlastiqueStyle;
	
	mPaintBucketLayout = new QGridLayout;
	mPaintBucketLayout->setVerticalSpacing(1);
	mPaintBucketLayout->setHorizontalSpacing(1);

	//viewport background color
	mPaintBucketColorLabel = new QLabel;
	mPaintBucketColorButton = addColorPreference(mPaintBucketColorLabel, tr("Paint Bucket\nColor:"), mPaintBucketColor, mPaintBucketLayout, colorPickerStyle, 0, 0);
	connect(mPaintBucketColorButton,SIGNAL(clicked()),this,SLOT(setPaintBucketColor()));
	
	//paint selcted faces 
	mPaintSelectedFacesButton = new QPushButton(tr("Paint Selected Faces"), this);
	connect(mPaintSelectedFacesButton, SIGNAL(clicked()), ((MainWindow*)mParent), SLOT(paintSelectedFaces()));
	mPaintBucketLayout->addWidget(mPaintSelectedFacesButton,1,0);

	//paint selcted faces 
	mResetMaterialsButton = new QPushButton(tr("Reset Materials"), this);
	connect(mResetMaterialsButton, SIGNAL(clicked()), ((MainWindow*)mParent), SLOT(clearMaterials()));
	mPaintBucketLayout->addWidget(mResetMaterialsButton,2,0);
	
	mPaintBucketLayout->setRowStretch(3,1);
	mPaintBucketLayout->setColumnStretch(2,1);
	mPaintBucketWidget->setWindowTitle(tr("Paint Bucket"));
	mPaintBucketWidget->setLayout(mPaintBucketLayout);	
}

void ExperimentalMode::setPaintBucketColor(){
	mPaintBucketColor.setRgba(QColorDialog::getRgba(mPaintBucketColor.rgba()));
	if (mPaintBucketColor.isValid()){
		((MainWindow*)mParent)->setPaintBucketColor(mPaintBucketColor);
		setButtonColor(mPaintBucketColor, mPaintBucketColorButton);
	}
}
void ExperimentalMode::setPaintBucketColor(QColor c){
	mPaintBucketColor = c;
	setButtonColor(mPaintBucketColor,mPaintBucketColorButton);
}

void ExperimentalMode::setButtonColor(QColor color, QPushButton *button){
	p = button->palette();
	p.setColor(QPalette::Button, color);
	button->setPalette(p);
}



void ExperimentalMode::retranslateUi(){
	//actions
	mPaintBucketAct->setText(tr("Paint Bucket"));
	mPaintBucketAct->setStatusTip(tr("Enter Paint Bucket Mode"));
	mPaintBucketAct->setToolTip(tr("Paint Bucket Mode"));

	//menus
	mExperimentalMenu->setTitle(tr("Experimental"));
	
	
	//!< \todo add buttons and spinbox labels to retranslate function for highgenusmode.cc
	// mPaintBucketColorButton
	mPaintSelectedFacesButton->setText(tr("Paint Selected Faces"));
	mPaintBucketColorLabel->setText(tr("Paint Bucket\nColor"));
	mPaintBucketWidget->setWindowTitle(tr("Paint Bucket"));

}
