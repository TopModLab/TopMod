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

/****************************************************************************
 **
 **
 ****************************************************************************/
#include <QtGui>

#include "TexturingMode.hh"

/*!
	\ingroup gui
	@{
	
	\class TexturingMode
	\brief Texturing Operations GUI elements.
	
	\note 
	
	\see TexturingMode
*/

/*!
* \brief Constructor
* 
* @param parent the MainWindow widget
* @param sm the shortcut manager class for adding a custom shortcut to each menu action or icon
* @param actionList the master list of actions for use with the CommandCompleter class
* 
*/
TexturingMode::TexturingMode(QWidget *parent, QShortcutManager *sm, QWidget *actionList)
		: QWidget(parent)
{		
	setParent(0);
	mParent = parent;
	
	mTileTexturingWidget = new QWidget;
	setupTileTexturing();
	
	mTileTexturingAction = new QAction(tr("Tile Texturing"),this);
	mTileTexturingAction->setIcon(QIcon(":images/mode_texturing.png"));
	mTileTexturingAction->setCheckable(true);
	sm->registerAction(mTileTexturingAction, "Texturing Operations", "");
	mTileTexturingAction->setStatusTip(tr("Enter Tile Texturing Mode"));
	mTileTexturingAction->setToolTip(tr("Tile Texturing Mode"));
	connect(mTileTexturingAction, SIGNAL(triggered()), this, SLOT(triggerTileTexturing()));
	actionList->addAction(mTileTexturingAction);

}

QMenu* TexturingMode::getMenu(){
	
	mTexturingMenu = new QMenu(tr("Texturing"));
	
	mTexturingMenu->addAction(mTileTexturingAction);
	
	return mTexturingMenu;
	
}

void TexturingMode::triggerTileTexturing(){
	
	((MainWindow*)mParent)->setToolOptions(mTileTexturingWidget);
	((MainWindow*)mParent)->setMode(MainWindow::NormalMode);
	
}

void TexturingMode::addActions(QActionGroup *actionGroup, QToolBar *toolBar, QStackedWidget *stackedWidget){
	
	actionGroup->addAction(mTileTexturingAction);
	toolBar->addAction(mTileTexturingAction);
	stackedWidget->addWidget(mTileTexturingWidget);
	
}

QDoubleSpinBox *TexturingMode::createDoubleSpinBox(QGridLayout *layout, QLabel *label, QString s, double low, double high, double step, double value, double decimals, int row, int col){
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

void TexturingMode::setupTileTexturing(){

	mTileTexturingLayout = new QGridLayout;
	mTileTexturingLayout->setVerticalSpacing(1);
	mTileTexturingLayout->setHorizontalSpacing(1);
	// mTileTexturingLayout->setMargin(0);
	
	tileTexturingNumTilesLabel = new QLabel(this);
	tileTexturingNumTilesSpinBox = createDoubleSpinBox(mTileTexturingLayout, tileTexturingNumTilesLabel, tr("Tiling Number"), 2, 8, 1, 2, 0, 0,0);
	connect(tileTexturingNumTilesSpinBox, SIGNAL(valueChanged(double)), ((MainWindow*)mParent), SLOT(changeTileTexNum(double)));

	tileTexturingAssignButton = new QPushButton(tr("Assign Texture\nCoordinates"), this);
	connect(tileTexturingAssignButton, SIGNAL(clicked()), ((MainWindow*)mParent), SLOT(assignTileTexCoords()));
	mTileTexturingLayout->addWidget(tileTexturingAssignButton,1,0,1,2);

	mTileTexturingLayout->setRowStretch(2,1);
	mTileTexturingLayout->setColumnStretch(2,1);
	mTileTexturingWidget->setWindowTitle(tr("Tile Texturing"));
	mTileTexturingWidget->setLayout(mTileTexturingLayout);
	
}

void TexturingMode::retranslateUi(){
	
	mTexturingMenu->setTitle(tr("Texturing"));
	mTileTexturingAction->setText(tr("Tile Texturing"));
	mTileTexturingAction->setStatusTip(tr("Enter Tile Texturing Mode"));
	mTileTexturingAction->setToolTip(tr("Tile Texturing Mode"));
	// tileTexturingNumTilesLabel->setText(tr("Tiling Number"));
	tileTexturingAssignButton->setText(tr("Assign Texture\nCoordinates"));
	mTileTexturingWidget->setWindowTitle(tr("Tile Texturing"));
}
