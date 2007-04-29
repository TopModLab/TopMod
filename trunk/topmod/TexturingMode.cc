/****************************************************************************
 **
 **
 ****************************************************************************/
#include <QtGui>

#include "TexturingMode.hh"

TexturingMode::TexturingMode(QWidget *parent)
		: QWidget(parent)
{		
	this->setParent(parent);
	
	mTileTexturingWidget = new QWidget;
	setupTileTexturing();
	
	mTileTexturingAction = new QAction(tr("Tile Texturing"),this);
	mTileTexturingAction->setCheckable(true);
	// sm->registerAction(mTileTexturingAction, "Basics Modes", "9");
	mTileTexturingAction->setStatusTip(tr("Enter Tile Texturing Mode"));
	mTileTexturingAction->setToolTip(tr("Tile Texturing Mode"));
	connect(mTileTexturingAction, SIGNAL(triggered()), this, SLOT(triggerTileTexturing()));

}

void TexturingMode::triggerTileTexturing(){
	
	((MainWindow*)parentWidget())->setToolOptions(mTileTexturingWidget);
	((MainWindow*)parentWidget())->setMode(DLFLWindow::NormalMode);
	
}

void TexturingMode::addActions(QActionGroup *actionGroup, QToolBar *toolBar, QStackedWidget *stackedWidget){
	
	actionGroup->addAction(mTileTexturingAction);
	toolBar->addAction(mTileTexturingAction);
	stackedWidget->addWidget(mTileTexturingWidget);
	
}

void TexturingMode::setupTileTexturing(){
	
	mTileTexturingLayout = new QBoxLayout(QBoxLayout::LeftToRight);
	
	//number of segments
	QLabel *tileTexturingNumTilesLabel = new QLabel(tr("Tiling Number:"));
	QSpinBox *tileTexturingNumTilesSpinBox = new QSpinBox;
	tileTexturingNumTilesSpinBox->setRange(2, 8);
	tileTexturingNumTilesSpinBox->setSingleStep(1);
	tileTexturingNumTilesSpinBox->setValue(2);
	tileTexturingNumTilesSpinBox->setMaximumSize(60,25);
	
	connect(tileTexturingNumTilesSpinBox, SIGNAL(valueChanged(int)),
          ((MainWindow*)parentWidget()), SLOT(changeTileTexNum(int)));

	mTileTexturingLayout->addWidget(tileTexturingNumTilesLabel);
  mTileTexturingLayout->addWidget(tileTexturingNumTilesSpinBox);
	//create column button
	QPushButton *tileTexturingAssignButton = new QPushButton(tr("Assign Texture Coordinates"), this);
	
	connect(tileTexturingAssignButton, SIGNAL(clicked()),
          ((MainWindow*)parentWidget()), SLOT(assign_texture_coordinates()));

	mTileTexturingLayout->addWidget(tileTexturingAssignButton);
	mTileTexturingLayout->addStretch(1);
	mTileTexturingWidget->setLayout(mTileTexturingLayout);	
	
}
