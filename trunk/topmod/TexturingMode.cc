/****************************************************************************
 **
 **
 ****************************************************************************/
#include <QtGui>

#include "TexturingMode.hh"

TexturingMode::TexturingMode(QWidget *parent, QShortcutManager *sm)
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

}

QMenu* TexturingMode::getMenu(){
	
	mTexturingMenu = new QMenu(tr("Texturing"));
	
	mTexturingMenu->addAction(mTileTexturingAction);
	
	return mTexturingMenu;
	
}

void TexturingMode::triggerTileTexturing(){
	
	((MainWindow*)mParent)->setToolOptions(mTileTexturingWidget);
	((MainWindow*)mParent)->setMode(DLFLWindow::NormalMode);
	
}

void TexturingMode::addActions(QActionGroup *actionGroup, QToolBar *toolBar, QStackedWidget *stackedWidget){
	
	actionGroup->addAction(mTileTexturingAction);
	toolBar->addAction(mTileTexturingAction);
	stackedWidget->addWidget(mTileTexturingWidget);
	
}

void TexturingMode::setupTileTexturing(){
	
	mTileTexturingLayout = new QBoxLayout(QBoxLayout::LeftToRight);
	mTileTexturingLayout->setMargin(0);
	//number of segments
	QLabel *tileTexturingNumTilesLabel = new QLabel(tr("Tiling Number:"));
	QSpinBox *tileTexturingNumTilesSpinBox = new QSpinBox;
	tileTexturingNumTilesSpinBox->setRange(2, 8);
	tileTexturingNumTilesSpinBox->setSingleStep(1);
	tileTexturingNumTilesSpinBox->setValue(2);
	tileTexturingNumTilesSpinBox->setMaximumSize(60,25);
	
	connect(tileTexturingNumTilesSpinBox, SIGNAL(valueChanged(int)),
          ((MainWindow*)mParent), SLOT(changeTileTexNum(int)));

	mTileTexturingLayout->addWidget(tileTexturingNumTilesLabel);
  mTileTexturingLayout->addWidget(tileTexturingNumTilesSpinBox);
	//create column button
	QPushButton *tileTexturingAssignButton = new QPushButton(tr("Assign Texture Coordinates"), this);
	
	connect(tileTexturingAssignButton, SIGNAL(clicked()),
          ((MainWindow*)mParent), SLOT(assign_texture_coordinates()));

	mTileTexturingLayout->addWidget(tileTexturingAssignButton);
	mTileTexturingLayout->addStretch(1);
	mTileTexturingWidget->setLayout(mTileTexturingLayout);	
	
}
