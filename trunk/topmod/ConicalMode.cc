/****************************************************************************
 **
 **
 ****************************************************************************/
#include <QtGui>

#include "ConicalMode.hh"

ConicalMode::ConicalMode(QWidget *parent)
		: QWidget(parent)
{		
	this->setParent(parent);
	lastMode = DLFLWindow::CutEdge;
	
	//mode group. this is the main widget that goes into a TabWidget
  conicalGroup = new QGroupBox(tr("Conical"));
	//boxLayout widget to add buttons to within the groupBox widget
	conicalBoxLayout = new QVBoxLayout;

	//Populate a Q Combo Box with a list of strings
	conicalLabel = new QLabel(tr("Select Conical Operation"));
	conicalComboBox = new QComboBox();
	QStringList conicalList;
	conicalList << tr("Cut by Edge") << tr("Cut by Vertex") << tr("Cut by Edge/Vertex") << tr("Cut by Face") << tr("Truncate Edge") << tr("Truncate Vertex") << tr("Dual Convex Hull");
	conicalComboBox->addItems(conicalList);
	//add the combobox to the layout
	conicalBoxLayout->addWidget(conicalLabel);
	conicalBoxLayout->addWidget(conicalComboBox);
	
	//create the stacked widget and all child widget pages
	cutbyEdgeWidget = new QWidget;
  cutbyVertexWidget = new QWidget;
  cutbyEdgeVertexWidget = new QWidget;
  cutbyFaceWidget = new QWidget;
  truncateEdgeWidget = new QWidget;
  truncateVertexWidget = new QWidget;
  dualConvexHullWidget = new QWidget;

  stackedWidget = new QStackedWidget;
  stackedWidget->addWidget(cutbyEdgeWidget);
  stackedWidget->addWidget(cutbyVertexWidget);
  stackedWidget->addWidget(cutbyEdgeVertexWidget);
  stackedWidget->addWidget(cutbyFaceWidget);
  stackedWidget->addWidget(truncateEdgeWidget);
  stackedWidget->addWidget(truncateVertexWidget);
  stackedWidget->addWidget(dualConvexHullWidget);

	//add the stackedwidget to the layout
  conicalBoxLayout->addWidget(stackedWidget);

	//connect the combobox to the stacked widget
  connect(conicalComboBox, SIGNAL(activated(int)),
          stackedWidget, SLOT(setCurrentIndex(int)));
	
	//setup stacked widget pages here, 
	//each in a separate function to make the code more readable
	setupCutbyEdge();
	setupCutbyVertex();
	setupCutbyEdgeVertex();
	setupCutbyFace();
	setupTruncateEdge();
	setupTruncateVertex();
	setupDualConvexHull();

	//set the layout
	conicalGroup->setLayout(conicalBoxLayout);
	
	connect(conicalComboBox, SIGNAL(currentIndexChanged(int)),
					this,SLOT(switchMode(int)) );
}

void ConicalMode::insertTab(QTabWidget *tabWidget){
	tabWidget->addTab( conicalGroup, QIcon(":/images/mode_conical.png"), "3");
  tabWidget->setTabToolTip(0,tr("Conical Mode"));
}

int ConicalMode::getLastMode(){
	return lastMode;
}

void ConicalMode::switchMode(int index){	
	switch(index){
		case 0: lastMode = DLFLWindow::CutEdge;
		break;
		case 1: lastMode = DLFLWindow::CutVertex;
		break;	
		case 2: lastMode = DLFLWindow::CutFace;
		break;
		case 3: lastMode = DLFLWindow::CutEdgeandVertex;
		break;
		case 4: lastMode = DLFLWindow::TruncateEdge;
		break;
		case 5: lastMode = DLFLWindow::ConvexHullMode;
		break;
		default:
		break;
	};
	((MainWindow*)parentWidget())->setMode(lastMode);	
}

void ConicalMode::changeCutOffsetE(double value){
	
	cutbyEdgeOffsetSpinBox->setValue(value);	
	cutbyFaceOffsetSpinBox->setValue(value);	
	truncateEdgeOffsetSpinBox->setValue(value);
	truncateVertexOffsetSpinBox->setValue(value);	
	cutbyEdgeVertexOffsetESpinBox->setValue(value);	
	cutbyFaceOffsetSpinBox->setValue(value);	
	
  ((MainWindow*)parentWidget())->changeCutOffsetE(value);
}

void ConicalMode::changeCutOffsetV(double value){
	
	cutbyVertexOffsetSpinBox->setValue(value);	
	cutbyEdgeVertexOffsetVSpinBox->setValue(value);	
	
  ((MainWindow*)parentWidget())->changeCutOffsetV(value);
}

void ConicalMode::toggleGlobalCut(int state){    
	
	cutbyEdgeGlobalCheckBox->setChecked(state);
	cutbyVertexGlobalCheckBox->setChecked(state);
	truncateVertexGlobalCheckBox->setChecked(state);
	cutbyEdgeVertexGlobalCheckBox->setChecked(state);
	cutbyFaceGlobalCheckBox->setChecked(state);
	truncateEdgeGlobalCheckBox->setChecked(state);
	
  ((MainWindow*)parentWidget())->toggleGlobalCut(state);
}
void ConicalMode::toggleSelectedCut(int state){
	
	cutbyEdgeCutSelectedCheckBox->setChecked(state);
	cutbyVertexCutSelectedCheckBox->setChecked(state);
	truncateVertexCutSelectedCheckBox->setChecked(state);
	cutbyEdgeVertexCutSelectedCheckBox->setChecked(state);
	cutbyFaceCutSelectedCheckBox->setChecked(state);
	truncateEdgeCutSelectedCheckBox->setChecked(state);
	
  ((MainWindow*)parentWidget())->toggleSelectedCut(state);
}

void ConicalMode::changeTiltPlane1(double value){
	
	cutbyEdgeTiltPlane1SpinBox->setValue(value);	
	cutbyEdgeVertexTiltPlane1SpinBox->setValue(value);	
	
	((MainWindow*)parentWidget())->changeTiltPlane1(value);
}
void ConicalMode::changeTiltPlane2(double value){
	
	cutbyEdgeTiltPlane2SpinBox->setValue(value);
	cutbyEdgeVertexTiltPlane2SpinBox->setValue(value);	
	
	((MainWindow*)parentWidget())->changeCutOffsetE(value); 
}

void ConicalMode::setupCutbyEdge(){
	
	cutbyEdgeLayout = new QVBoxLayout;
	
	QLabel *cutbyEdgeOffsetLabel = new QLabel(tr("Offset:"));
	cutbyEdgeOffsetSpinBox = new QDoubleSpinBox;
	cutbyEdgeOffsetSpinBox->setRange(0, 1);
	cutbyEdgeOffsetSpinBox->setSingleStep(0.01);
	cutbyEdgeOffsetSpinBox->setValue(0.25);	
	connect(cutbyEdgeOffsetSpinBox, SIGNAL(valueChanged(double)),
          this, SLOT(changeCutOffsetE(double)));
	
	cutbyEdgeLayout->addWidget(cutbyEdgeOffsetLabel);
  cutbyEdgeLayout->addWidget(cutbyEdgeOffsetSpinBox);

	QLabel *cutbyEdgeTiltPlane1Label = new QLabel(tr("Tilt plane:"));
	cutbyEdgeTiltPlane1SpinBox = new QDoubleSpinBox;
	cutbyEdgeTiltPlane1SpinBox->setRange(-1, 1);
	cutbyEdgeTiltPlane1SpinBox->setSingleStep(.01);
	cutbyEdgeTiltPlane1SpinBox->setValue(0);	
	connect(cutbyEdgeTiltPlane1SpinBox, SIGNAL(valueChanged(double)),
          this, SLOT(changeTiltPlane1(double)));
	
	cutbyEdgeLayout->addWidget(cutbyEdgeTiltPlane1Label);
  cutbyEdgeLayout->addWidget(cutbyEdgeTiltPlane1SpinBox);

	QLabel *cutbyEdgeTiltPlane2Label = new QLabel(tr("Tilt plane:"));
	cutbyEdgeTiltPlane2SpinBox = new QDoubleSpinBox;
	cutbyEdgeTiltPlane2SpinBox->setRange(-1, 1);
	cutbyEdgeTiltPlane2SpinBox->setSingleStep(.01);
	cutbyEdgeTiltPlane2SpinBox->setValue(0);
	connect(cutbyEdgeTiltPlane2SpinBox, SIGNAL(valueChanged(double)),
          this, SLOT(changeTiltPlane2(double)));
	
	cutbyEdgeLayout->addWidget(cutbyEdgeTiltPlane2Label);
  cutbyEdgeLayout->addWidget(cutbyEdgeTiltPlane2SpinBox);

	cutbyEdgeGlobalCheckBox = new QCheckBox(tr("global cut"),this);
	cutbyEdgeGlobalCheckBox->setChecked(Qt::Unchecked);
	connect(cutbyEdgeGlobalCheckBox, SIGNAL(stateChanged(int)),
          this, SLOT(toggleGlobalCut(int)));
				
	cutbyEdgeLayout->addWidget(cutbyEdgeGlobalCheckBox);
	
	cutbyEdgeCutSelectedCheckBox = new QCheckBox(tr("cut selected edges"),this);
	cutbyEdgeCutSelectedCheckBox->setChecked(Qt::Checked);
	connect(cutbyEdgeCutSelectedCheckBox, SIGNAL(stateChanged(int)),
          this, SLOT(toggleSelectedCut(int)));
				
	cutbyEdgeLayout->addWidget(cutbyEdgeCutSelectedCheckBox);
		
	QPushButton *performCuttingButton = new QPushButton(tr("Perform Cutting"), this);
	connect(performCuttingButton, SIGNAL(clicked()),
          ((MainWindow*)parentWidget()), SLOT(perform_cutting()));

	cutbyEdgeLayout->addWidget(performCuttingButton);

	cutbyEdgeLayout->addStretch(1);
	cutbyEdgeWidget->setLayout(cutbyEdgeLayout);	
	
}

void ConicalMode::setupCutbyVertex(){
	
	cutbyVertexLayout = new QVBoxLayout;
	
	QLabel *cutbyVertexOffsetLabel = new QLabel(tr("Offset:"));
	cutbyVertexOffsetSpinBox = new QDoubleSpinBox;
	cutbyVertexOffsetSpinBox->setRange(0, 1);
	cutbyVertexOffsetSpinBox->setSingleStep(0.01);
	cutbyVertexOffsetSpinBox->setValue(0.25);	
	connect(cutbyVertexOffsetSpinBox, SIGNAL(valueChanged(double)),
          this, SLOT(changeCutOffsetV(double)));
	
	cutbyVertexLayout->addWidget(cutbyVertexOffsetLabel);
  cutbyVertexLayout->addWidget(cutbyVertexOffsetSpinBox);

	cutbyVertexGlobalCheckBox = new QCheckBox(tr("global cut"),this);
	cutbyVertexGlobalCheckBox->setChecked(Qt::Unchecked);
	connect(cutbyVertexGlobalCheckBox, SIGNAL(stateChanged(int)),
          this, SLOT(toggleGlobalCut(int)));
				
	cutbyVertexLayout->addWidget(cutbyVertexGlobalCheckBox);
	
	cutbyVertexCutSelectedCheckBox = new QCheckBox(tr("cut selected edges"),this);
	cutbyVertexCutSelectedCheckBox->setChecked(Qt::Checked);
	connect(cutbyVertexCutSelectedCheckBox, SIGNAL(stateChanged(int)),
          this, SLOT(toggleSelectedCut(int)));
				
	cutbyVertexLayout->addWidget(cutbyVertexCutSelectedCheckBox);	
	
	QPushButton *performCuttingButton = new QPushButton(tr("Perform Cutting"), this);
	connect(performCuttingButton, SIGNAL(clicked()),
          ((MainWindow*)parentWidget()), SLOT(perform_cutting()));

	cutbyVertexLayout->addWidget(performCuttingButton);
	
	cutbyVertexLayout->addStretch(1);
	cutbyVertexWidget->setLayout(cutbyVertexLayout);	
	
}

void ConicalMode::setupCutbyEdgeVertex(){
	
	cutbyEdgeVertexLayout = new QVBoxLayout;
	
	QLabel *cutbyEdgeVertexOffsetELabel = new QLabel(tr("Offset (Edge):"));
	cutbyEdgeVertexOffsetESpinBox = new QDoubleSpinBox;
	cutbyEdgeVertexOffsetESpinBox->setRange(0, 1);
	cutbyEdgeVertexOffsetESpinBox->setSingleStep(0.01);
	cutbyEdgeVertexOffsetESpinBox->setValue(0.25);	
	connect(cutbyEdgeVertexOffsetESpinBox, SIGNAL(valueChanged(double)),
          this, SLOT(changeCutOffsetE(double)));
	
	cutbyEdgeVertexLayout->addWidget(cutbyEdgeVertexOffsetELabel);
  cutbyEdgeVertexLayout->addWidget(cutbyEdgeVertexOffsetESpinBox);

	QLabel *cutbyEdgeVertexOffsetVLabel = new QLabel(tr("Offset (Vertex):"));
	cutbyEdgeVertexOffsetVSpinBox = new QDoubleSpinBox;
	cutbyEdgeVertexOffsetVSpinBox->setRange(0, 1);
	cutbyEdgeVertexOffsetVSpinBox->setSingleStep(0.01);
	cutbyEdgeVertexOffsetVSpinBox->setValue(0.25);	
	connect(cutbyEdgeVertexOffsetVSpinBox, SIGNAL(valueChanged(double)),
          this, SLOT(changeCutOffsetV(double)));
	
	cutbyEdgeVertexLayout->addWidget(cutbyEdgeVertexOffsetVLabel);
  cutbyEdgeVertexLayout->addWidget(cutbyEdgeVertexOffsetVSpinBox);

	QLabel *cutbyEdgeVertexTiltPlane1Label = new QLabel(tr("Tilt plane:"));
	cutbyEdgeVertexTiltPlane1SpinBox = new QDoubleSpinBox;
	cutbyEdgeVertexTiltPlane1SpinBox->setRange(-1, 1);
	cutbyEdgeVertexTiltPlane1SpinBox->setSingleStep(.01);
	cutbyEdgeVertexTiltPlane1SpinBox->setValue(0);	
	connect(cutbyEdgeVertexTiltPlane1SpinBox, SIGNAL(valueChanged(double)),
          this, SLOT(changeTiltPlane1(double)));
	
	cutbyEdgeVertexLayout->addWidget(cutbyEdgeVertexTiltPlane1Label);
  cutbyEdgeVertexLayout->addWidget(cutbyEdgeVertexTiltPlane1SpinBox);

	QLabel *cutbyEdgeVertexTiltPlane2Label = new QLabel(tr("Tilt plane:"));
	cutbyEdgeVertexTiltPlane2SpinBox = new QDoubleSpinBox;
	cutbyEdgeVertexTiltPlane2SpinBox->setRange(-1, 1);
	cutbyEdgeVertexTiltPlane2SpinBox->setSingleStep(0.01);
	cutbyEdgeVertexTiltPlane2SpinBox->setValue(0);	
	connect(cutbyEdgeVertexTiltPlane2SpinBox, SIGNAL(valueChanged(double)),
          this, SLOT(changeTiltPlane2(double)));
	
	cutbyEdgeVertexLayout->addWidget(cutbyEdgeVertexTiltPlane2Label);
  cutbyEdgeVertexLayout->addWidget(cutbyEdgeVertexTiltPlane2SpinBox);

	cutbyEdgeVertexGlobalCheckBox = new QCheckBox(tr("global cut"),this);
	cutbyEdgeVertexGlobalCheckBox->setChecked(Qt::Unchecked);
	connect(cutbyEdgeVertexGlobalCheckBox, SIGNAL(stateChanged(int)),
          this, SLOT(toggleGlobalCut(int)));
				
	cutbyEdgeVertexLayout->addWidget(cutbyEdgeVertexGlobalCheckBox);
	
	cutbyEdgeVertexCutSelectedCheckBox = new QCheckBox(tr("cut selected edges"),this);
	cutbyEdgeVertexCutSelectedCheckBox->setChecked(Qt::Checked);
	connect(cutbyEdgeVertexCutSelectedCheckBox, SIGNAL(stateChanged(int)),
          this, SLOT(toggleSelectedCut(int)));
				
	cutbyEdgeVertexLayout->addWidget(cutbyEdgeVertexCutSelectedCheckBox);

	QPushButton *performCuttingButton = new QPushButton(tr("Perform Cutting"), this);
	connect(performCuttingButton, SIGNAL(clicked()),
          ((MainWindow*)parentWidget()), SLOT(perform_cutting()));

	cutbyEdgeVertexLayout->addWidget(performCuttingButton);

	cutbyEdgeVertexLayout->addStretch(1);
	cutbyEdgeVertexWidget->setLayout(cutbyEdgeVertexLayout);	
	
}

void ConicalMode::setupCutbyFace(){
	
	cutbyFaceLayout = new QVBoxLayout;

	QLabel *cutbyFaceOffsetLabel = new QLabel(tr("Offset:"));
	cutbyFaceOffsetSpinBox = new QDoubleSpinBox;
	cutbyFaceOffsetSpinBox->setRange(0, 1);
	cutbyFaceOffsetSpinBox->setSingleStep(0.01);
	cutbyFaceOffsetSpinBox->setValue(0.25);	
	connect(cutbyFaceOffsetSpinBox, SIGNAL(valueChanged(double)),
          this, SLOT(changeCutOffsetE(double)));
	
	cutbyFaceLayout->addWidget(cutbyFaceOffsetLabel);
  cutbyFaceLayout->addWidget(cutbyFaceOffsetSpinBox);

	cutbyFaceGlobalCheckBox = new QCheckBox(tr("global cut"),this);
	cutbyFaceGlobalCheckBox->setChecked(Qt::Unchecked);
	connect(cutbyFaceGlobalCheckBox, SIGNAL(stateChanged(int)),
          this, SLOT(toggleGlobalCut(int)));
	cutbyFaceLayout->addWidget(cutbyFaceGlobalCheckBox);
	
	cutbyFaceCutSelectedCheckBox = new QCheckBox(tr("cut selected faces"),this);
	cutbyFaceCutSelectedCheckBox->setChecked(Qt::Checked);
	connect(cutbyFaceCutSelectedCheckBox, SIGNAL(stateChanged(int)),
          this, SLOT(toggleSelectedCut(int)));
				
	cutbyFaceLayout->addWidget(cutbyFaceCutSelectedCheckBox);

	QPushButton *performCuttingButton = new QPushButton(tr("Perform Cutting"), this);
	connect(performCuttingButton, SIGNAL(clicked()),
          ((MainWindow*)parentWidget()), SLOT(perform_cutting()));

	cutbyFaceLayout->addWidget(performCuttingButton);

	cutbyFaceLayout->addStretch(1);
	cutbyFaceWidget->setLayout(cutbyFaceLayout);	
	
}

void ConicalMode::setupTruncateEdge(){
	
	truncateEdgeLayout = new QVBoxLayout;
	
	QLabel *truncateEdgeOffsetLabel = new QLabel(tr("Offset:"));
	truncateEdgeOffsetSpinBox = new QDoubleSpinBox;
	truncateEdgeOffsetSpinBox->setRange(0, 1);
	truncateEdgeOffsetSpinBox->setSingleStep(0.01);
	truncateEdgeOffsetSpinBox->setValue(0.25);	
	connect(truncateEdgeOffsetSpinBox, SIGNAL(valueChanged(double)),
          this, SLOT(changeCutOffsetE(double)));
	
	truncateEdgeLayout->addWidget(truncateEdgeOffsetLabel);
  truncateEdgeLayout->addWidget(truncateEdgeOffsetSpinBox);

	truncateEdgeGlobalCheckBox = new QCheckBox(tr("global cut"),this);
	truncateEdgeGlobalCheckBox->setChecked(Qt::Unchecked);
	connect(truncateEdgeGlobalCheckBox, SIGNAL(stateChanged(int)),
          this, SLOT(toggleGlobalCut(int)));
	truncateEdgeLayout->addWidget(truncateEdgeGlobalCheckBox);
	
	truncateEdgeCutSelectedCheckBox = new QCheckBox(tr("cut selected vertices"),this);
	truncateEdgeCutSelectedCheckBox->setChecked(Qt::Checked);
	connect(truncateEdgeCutSelectedCheckBox, SIGNAL(stateChanged(int)),
          this, SLOT(toggleSelectedCut(int)));
				
	truncateEdgeLayout->addWidget(truncateEdgeCutSelectedCheckBox);	
	
	QPushButton *performCuttingButton = new QPushButton(tr("Perform Cutting"), this);
	connect(performCuttingButton, SIGNAL(clicked()),
          ((MainWindow*)parentWidget()), SLOT(perform_cutting()));

	truncateEdgeLayout->addWidget(performCuttingButton);

	truncateEdgeLayout->addStretch(1);
	truncateEdgeWidget->setLayout(truncateEdgeLayout);	
	
}

void ConicalMode::setupTruncateVertex(){
	
	truncateVertexLayout = new QVBoxLayout;
	
	QLabel *truncateVertexOffsetLabel = new QLabel(tr("Offset:"));
	truncateVertexOffsetSpinBox = new QDoubleSpinBox;
	truncateVertexOffsetSpinBox->setRange(0, 1);
	truncateVertexOffsetSpinBox->setSingleStep(0.01);
	truncateVertexOffsetSpinBox->setValue(0.25);	
	connect(truncateVertexOffsetSpinBox, SIGNAL(valueChanged(double)),
          this, SLOT(changeCutOffsetE(double)));
	
	truncateVertexLayout->addWidget(truncateVertexOffsetLabel);
  truncateVertexLayout->addWidget(truncateVertexOffsetSpinBox);

	truncateVertexGlobalCheckBox = new QCheckBox(tr("global cut"),this);
	truncateVertexGlobalCheckBox->setChecked(Qt::Unchecked);
	connect(truncateVertexGlobalCheckBox, SIGNAL(stateChanged(int)),
          this, SLOT(toggleGlobalCut(int)));
	truncateVertexLayout->addWidget(truncateVertexGlobalCheckBox);
	
	truncateVertexCutSelectedCheckBox = new QCheckBox(tr("cut selected vertices"),this);
	truncateVertexCutSelectedCheckBox->setChecked(Qt::Checked);
	connect(truncateVertexCutSelectedCheckBox, SIGNAL(stateChanged(int)),
          this, SLOT(toggleSelectedCut(int)));
				
	truncateVertexLayout->addWidget(truncateVertexCutSelectedCheckBox);	

	QPushButton *performCuttingButton = new QPushButton(tr("Perform Cutting"), this);
	connect(performCuttingButton, SIGNAL(clicked()),
          ((MainWindow*)parentWidget()), SLOT(perform_cutting()));

	truncateVertexLayout->addWidget(performCuttingButton);
	
	truncateVertexLayout->addStretch(1);
	truncateVertexWidget->setLayout(truncateVertexLayout);
	
}

void ConicalMode::setupDualConvexHull(){
	
	dualConvexHullLayout = new QVBoxLayout;

	QPushButton *convexHullButton = new QPushButton(tr("Create Convex Hull"), this);
	connect(convexHullButton, SIGNAL(clicked()),
          ((MainWindow*)parentWidget()), SLOT(create_convexhull()));

	dualConvexHullLayout->addWidget(convexHullButton);
	
	QPushButton *dualConvexHullButton = new QPushButton(tr("Create Dual Convex Hull"), this);
	connect(dualConvexHullButton, SIGNAL(clicked()),
          ((MainWindow*)parentWidget()), SLOT(create_dual_convexhull()));

	dualConvexHullLayout->addWidget(dualConvexHullButton);
	
	dualConvexHullLayout->addStretch(1);
	dualConvexHullWidget->setLayout(dualConvexHullLayout);	
}