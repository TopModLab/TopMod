/****************************************************************************
 **
 **
 ****************************************************************************/
#include <QtGui>

#include "BasicsMode.hh"

BasicsMode::BasicsMode(QWidget *parent)
		: QWidget(parent)
{		
	this->setParent(parent);
	lastMode = DLFLWindow::InsertEdge;
	//mode group. this is the main widget that goes into a TabWidget
  basicsGroup = new QGroupBox(tr("Basics"));
	//boxLayout widget to add buttons to within the groupBox widget
	basicsLabel = new QLabel(tr("Select Basic Operation"));
	basicsBoxLayout = new QVBoxLayout;

	//Populate a Q Combo Box with a list of strings
	basicsComboBox = new QComboBox();
	QStringList basicsList;
	basicsList << tr("Insert Edge") << tr("Delete Edge") << tr("Collapse Edge") << tr("Subdivide Edge") 
						 << tr("Connect Edges") << tr("Splice Corners") << tr("Transforms");
						
	basicsComboBox->addItems(basicsList);
	//add the combobox to the layout
	basicsBoxLayout->addWidget(basicsLabel);
	basicsBoxLayout->addWidget(basicsComboBox);
	
	//create the stacked widget and all child widget pages
	insertEdgeWidget = new QWidget;
  deleteEdgeWidget = new QWidget;
  collapseEdgeWidget = new QWidget;
  subdivideEdgeWidget = new QWidget;
  connectEdgesWidget = new QWidget;
  spliceCornersWidget = new QWidget;
  transformsWidget = new QWidget;

  stackedWidget = new QStackedWidget;
  stackedWidget->addWidget(insertEdgeWidget);
  stackedWidget->addWidget(deleteEdgeWidget);
  stackedWidget->addWidget(collapseEdgeWidget);
  stackedWidget->addWidget(subdivideEdgeWidget);
  stackedWidget->addWidget(connectEdgesWidget);
  stackedWidget->addWidget(spliceCornersWidget);
  stackedWidget->addWidget(transformsWidget);

	//add the stackedwidget to the layout
  basicsBoxLayout->addWidget(stackedWidget);

	//connect the combobox to the stacked widget
  connect(basicsComboBox, SIGNAL(activated(int)),
          stackedWidget, SLOT(setCurrentIndex(int)));
	
	//setup stacked widget pages here, 
	//each in a separate function to make the code more readable
	//setupInsertEdge();
	setupDeleteEdge();
	//setupCollapseEdge();
	setupSubdivideEdge();
	//setupConnectEdges();
	//setupSpliceCorners();
	setupTransforms();
	
	//set the layout
	basicsGroup->setLayout(basicsBoxLayout);
	
	connect(basicsComboBox, SIGNAL(currentIndexChanged(int)),
					this,SLOT(switchMode(int)) );
}

void BasicsMode::insertTab(QTabWidget *tabWidget){
	tabWidget->addTab( basicsGroup, QIcon(":/images/mode_basics.png"), "1");
  tabWidget->setTabToolTip(0,tr("Basics Mode"));
	//currentIndexChanged
}

int BasicsMode::getLastMode(){
	return lastMode;
}

void BasicsMode::switchMode(int index){
		
	switch(index){
		case 0: lastMode = DLFLWindow::InsertEdge;
		break;
		case 1:	lastMode = DLFLWindow::DeleteEdge;
		break;	
		case 2: lastMode = DLFLWindow::CollapseEdge;
		break;
		case 3: lastMode = DLFLWindow::SubDivideEdge;
		break;
		case 4: lastMode = DLFLWindow::ConnectEdges;
		break;
		case 5: lastMode = DLFLWindow::SpliceCorners;
		break;
		case 6: lastMode = DLFLWindow::NormalMode;
		break;
		default:
		break;
	}
	((MainWindow*)parentWidget())->setMode(lastMode);
}

void BasicsMode::setupInsertEdge(){
	
	insertEdgeLayout = new QVBoxLayout;
	
	insertEdgeWidget->setLayout(insertEdgeLayout);	
	
}

void BasicsMode::setupDeleteEdge(){
	
	deleteEdgeLayout = new QVBoxLayout;
	//cleanup checkbox
	QCheckBox *cleanupCheckBox = new QCheckBox(tr("Cleanup"),this);
	connect(cleanupCheckBox, SIGNAL(stateChanged(int)),
          ((MainWindow*)parentWidget()), SLOT(toggleDeleteEdgeCleanupFlag(int)));
	deleteEdgeLayout->addWidget(cleanupCheckBox);	
	deleteEdgeLayout->addStretch(1);
	//set layout
	deleteEdgeWidget->setLayout(deleteEdgeLayout);	
	
}

void BasicsMode::setupCollapseEdge(){
	
	collapseEdgeLayout = new QVBoxLayout;
	
	collapseEdgeWidget->setLayout(collapseEdgeLayout);	
	
}

void BasicsMode::setupSubdivideEdge(){
	
	subdivideEdgeLayout = new QVBoxLayout;
	
	//number of subdivisions spinbox
	QLabel *numSubdivsLabel = new QLabel(tr("Number of Subdivisions"));
	QSpinBox *numSubdivsSpinBox = new QSpinBox;
	numSubdivsSpinBox->setRange(1, 10);
	numSubdivsSpinBox->setSingleStep(1);
	numSubdivsSpinBox->setValue(0);
	connect(numSubdivsSpinBox, SIGNAL(valueChanged(int)),
          ((MainWindow*)parentWidget()), SLOT(changeNumSubDivs(int)));
	
	subdivideEdgeLayout->addWidget(numSubdivsLabel);
  subdivideEdgeLayout->addWidget(numSubdivsSpinBox);
	subdivideEdgeLayout->addStretch(1);
	subdivideEdgeWidget->setLayout(subdivideEdgeLayout);	
	
}

void BasicsMode::setupConnectEdges(){
	
	connectEdgesLayout = new QVBoxLayout;
	
	connectEdgesWidget->setLayout(connectEdgesLayout);	
	
}

void BasicsMode::setupSpliceCorners(){
	
	spliceCornersLayout = new QVBoxLayout;
	
	spliceCornersWidget->setLayout(spliceCornersLayout);
	
}

void BasicsMode::setupTransforms(){
	
	transformsLayout = new QVBoxLayout;

	//x transform
	QLabel *xPosLabel = new QLabel(tr("X Transform"));
	xPosSpinBox = new QDoubleSpinBox;
	xPosSpinBox->setRange(-100.0, 100.0);
	xPosSpinBox->setSingleStep(0.5);
	xPosSpinBox->setValue(0.0);
	xPosSpinBox->setDecimals(1);
	connect(xPosSpinBox, SIGNAL(valueChanged(double)),
          ((MainWindow*)parentWidget()), SLOT(translatex(double)));

	transformsLayout->addWidget(xPosLabel);
  transformsLayout->addWidget(xPosSpinBox);

	//y transform
	QLabel *yPosLabel = new QLabel(tr("Y Transform"));
	yPosSpinBox = new QDoubleSpinBox;
	yPosSpinBox->setRange(-100.0, 100.0);
	yPosSpinBox->setSingleStep(0.5);
	yPosSpinBox->setValue(0.0);
	yPosSpinBox->setDecimals(1);
	connect(yPosSpinBox, SIGNAL(valueChanged(double)),
          ((MainWindow*)parentWidget()), SLOT(translatey(double)));

	transformsLayout->addWidget(yPosLabel);
  transformsLayout->addWidget(yPosSpinBox);

	//z transform
	QLabel *zPosLabel = new QLabel(tr("Z Transform"));
	zPosSpinBox = new QDoubleSpinBox;
	zPosSpinBox->setRange(-100.0, 100.0);
	zPosSpinBox->setSingleStep(0.5);
	zPosSpinBox->setValue(0.0);
	zPosSpinBox->setDecimals(1);
	connect(zPosSpinBox, SIGNAL(valueChanged(double)),
          ((MainWindow*)parentWidget()), SLOT(translatez(double)));

	transformsLayout->addWidget(zPosLabel);
  transformsLayout->addWidget(zPosSpinBox);
	
	//x scale
	QLabel *xScaleLabel = new QLabel(tr("X Scale"));
	xScaleSpinBox = new QDoubleSpinBox;
	xScaleSpinBox->setRange(0.1, 10.0);
	xScaleSpinBox->setSingleStep(0.1);
	xScaleSpinBox->setValue(1.0);
	xScaleSpinBox->setDecimals(1);	
	connect(xScaleSpinBox, SIGNAL(valueChanged(double)),
          ((MainWindow*)parentWidget()), SLOT(scalex(double)));

	transformsLayout->addWidget(xScaleLabel);
  transformsLayout->addWidget(xScaleSpinBox);
	
	//y scale
	QLabel *yScaleLabel = new QLabel(tr("Y Scale"));
	yScaleSpinBox = new QDoubleSpinBox;
	yScaleSpinBox->setRange(0.1, 10.0);
	yScaleSpinBox->setSingleStep(0.1);
	yScaleSpinBox->setValue(1.0);
	yScaleSpinBox->setDecimals(1);
	connect(yScaleSpinBox, SIGNAL(valueChanged(double)),
          ((MainWindow*)parentWidget()), SLOT(scaley(double)));

	transformsLayout->addWidget(yScaleLabel);
  transformsLayout->addWidget(yScaleSpinBox);

	//z scale
	QLabel *zScaleLabel = new QLabel(tr("Z Scale"));
	zScaleSpinBox = new QDoubleSpinBox;
	zScaleSpinBox->setRange(0.1, 10.0);
	zScaleSpinBox->setSingleStep(0.1);
	zScaleSpinBox->setValue(1.0);
	zScaleSpinBox->setDecimals(1);
	connect(zScaleSpinBox, SIGNAL(valueChanged(double)),
          ((MainWindow*)parentWidget()), SLOT(scalez(double)));
	transformsLayout->addWidget(zScaleLabel);
  transformsLayout->addWidget(zScaleSpinBox);

	QPushButton *freezeTransformsButton = new QPushButton("&Freeze Transforms", this);
	connect(freezeTransformsButton, SIGNAL(clicked()),
          this, SLOT(freeze_transforms()));
	
	transformsLayout->addWidget(freezeTransformsButton);	
	transformsLayout->addStretch(1);
	//set the layout
	transformsWidget->setLayout(transformsLayout);	
}

void BasicsMode::freeze_transforms()
{
	((MainWindow*)parentWidget())->freeze_transforms();
	xPosSpinBox->setValue(0.0);
	yPosSpinBox->setValue(0.0);
	zPosSpinBox->setValue(0.0);
	xScaleSpinBox->setValue(1.0);
	yScaleSpinBox->setValue(1.0);
	zScaleSpinBox->setValue(1.0);
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