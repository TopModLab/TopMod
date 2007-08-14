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
	label = new QLabel(s);
	QDoubleSpinBox *spinbox = new QDoubleSpinBox;
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
	
	tileTexturingNumTilesSpinBox = createDoubleSpinBox(mTileTexturingLayout, tileTexturingNumTilesLabel, tr("Tiling Number"), 2, 8, 1, 2, 0, 0,0);
	connect(tileTexturingNumTilesSpinBox, SIGNAL(valueChanged(double)), ((MainWindow*)mParent), SLOT(changeTileTexNum(double)));

	QPushButton *tileTexturingAssignButton = new QPushButton(tr("Assign Texture\nCoordinates"), this);
	connect(tileTexturingAssignButton, SIGNAL(clicked()), ((MainWindow*)mParent), SLOT(assignTileTexCoords()));
	mTileTexturingLayout->addWidget(tileTexturingAssignButton,1,0,1,2);

	mTileTexturingLayout->setRowStretch(2,1);
	mTileTexturingLayout->setColumnStretch(2,1);
	mTileTexturingWidget->setWindowTitle(tr("Tile Texturing"));
	mTileTexturingWidget->setLayout(mTileTexturingLayout);
	
}

void TexturingMode::retranslateUi(){
	
}
