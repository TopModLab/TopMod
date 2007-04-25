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
	lastMode = DLFLWindow::NormalMode;
	//mode group. this is the main widget that goes into a TabWidget
  texturingGroup = new QGroupBox(tr("Texturing"));
	//boxLayout widget to add buttons to within the groupBox widget
	texturingBoxLayout = new QVBoxLayout;

	//Populate a Q Combo Box with a list of strings
	texturingLabel = new QLabel(tr("Select Texturing Operation"));
	texturingComboBox = new QComboBox();
	QStringList texturingList;
	texturingList << tr("Tile Texturing");
	texturingComboBox->addItems(texturingList);
	//add the combobox to the layout
	texturingBoxLayout->addWidget(texturingLabel);
	texturingBoxLayout->addWidget(texturingComboBox);
	
	//create the stacked widget and all child widget pages
	tileTexturingWidget = new QWidget;

  stackedWidget = new QStackedWidget;
  stackedWidget->addWidget(tileTexturingWidget);

	//add the stackedwidget to the layout
  texturingBoxLayout->addWidget(stackedWidget);
	//connect the combobox to the stacked widget
  connect(texturingComboBox, SIGNAL(activated(int)),
          stackedWidget, SLOT(setCurrentIndex(int)));
	
	//setup stacked widget pages here, 
	setupTileTexturing();
	
	connect(texturingComboBox, SIGNAL(currentIndexChanged(int)),
					this,SLOT(switchMode(int)) );

	//set the layout
	texturingGroup->setLayout(texturingBoxLayout);
}

void TexturingMode::insertTab(QTabWidget *tabWidget){
	tabWidget->addTab( texturingGroup, QIcon(":/images/mode_texturing.png"), "6");
  tabWidget->setTabToolTip(0,tr("Texturing Mode"));
}

int TexturingMode::getLastMode(){
	return lastMode;
}

void TexturingMode::switchMode(int index){	
	switch(index){
		case 0: ((MainWindow*)parentWidget())->setMode(DLFLWindow::NormalMode);
		break;
		default:
		break;
	}
}

void TexturingMode::setupTileTexturing(){
	
	tileTexturingLayout = new QVBoxLayout;
	
	//number of segments
	QLabel *tileTexturingNumTilesLabel = new QLabel(tr("Tiling Number:"));
	QSpinBox *tileTexturingNumTilesSpinBox = new QSpinBox;
	tileTexturingNumTilesSpinBox->setRange(2, 8);
	tileTexturingNumTilesSpinBox->setSingleStep(1);
	tileTexturingNumTilesSpinBox->setValue(2);
	connect(tileTexturingNumTilesSpinBox, SIGNAL(valueChanged(int)),
          ((MainWindow*)parentWidget()), SLOT(changeTileTexNum(int)));

	tileTexturingLayout->addWidget(tileTexturingNumTilesLabel);
  tileTexturingLayout->addWidget(tileTexturingNumTilesSpinBox);
	//create column button
	QPushButton *tileTexturingAssignButton = new QPushButton(tr("Assign Texture Coordinates"), this);
	tileTexturingLayout->addWidget(tileTexturingAssignButton);
	
	tileTexturingLayout->addStretch(1);
	tileTexturingWidget->setLayout(tileTexturingLayout);	
	
}
// 
// //mouse events
// void MainWindow::mousePressEvent(QMouseEvent *event) {
// 	if ( event->buttons() == Qt::RightButton ){
// 		event->ignore();
// 	}
// }
// 
// void MainWindow::mouseMoveEvent(QMouseEvent *event) {
// 	if ( event->buttons() == Qt::RightButton ){
// 		event->ignore();
// 	}
// }
// 
// void MainWindow::mouseReleaseEvent(QMouseEvent *event) {
// 	if ( event->buttons() == Qt::RightButton ){
// 		event->ignore();
// 	}
// }