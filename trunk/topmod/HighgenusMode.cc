/****************************************************************************
 **
 **
 ****************************************************************************/
#include <QtGui>

#include "HighgenusMode.hh"

HighgenusMode::HighgenusMode(QWidget *parent)
		: QWidget(parent)
{		
	this->setParent(parent);
	lastMode = DLFLWindow::ConnectFaceVertices;
	//mode group. this is the main widget that goes into a TabWidget
  highgenusGroup = new QGroupBox(tr("High Genus Modeling"));
	//boxLayout widget to add buttons to within the groupBox widget
	highgenusBoxLayout = new QVBoxLayout;

	//Populate a Q Combo Box with a list of strings
	highgenusLabel = new QLabel(tr("Select High-Genus Operation"));
	highgenusComboBox = new QComboBox();
	QStringList highgenusList;
	highgenusList << tr("Add Hole/Handle") << tr("Add Hole/Handle (closest vertex)") << tr("Add Handle (shape interpolation)") 
								<< tr("Rind Modeling (Scaling)") << tr("Rind Modeling (Thickness)") << tr("Wireframe Modeling") << tr("Column Modeling") 
								<< tr("Sierpinsky") << tr("Multi-Face Handle") << tr("Menger Sponge");
	highgenusComboBox->addItems(highgenusList);
	//add the combobox to the layout
	highgenusBoxLayout->addWidget(highgenusLabel);
	highgenusBoxLayout->addWidget(highgenusComboBox);
	
	//create the stacked widget and all child widget pages
	addHoleHandleWidget = new QWidget;
  addHoleHandleCVWidget = new QWidget;
  addHandleSIWidget = new QWidget;
  rindModelingScalingWidget = new QWidget;
  rindModelingThicknessWidget = new QWidget;
  wireframeModelingWidget = new QWidget;
  columnModelingWidget = new QWidget;
	sierpinskyWidget = new QWidget;
	multiFaceHandleWidget = new QWidget;                
	mengerSpongeWidget = new QWidget;  

	stackedWidget = new QStackedWidget;

	stackedWidget->addWidget(addHoleHandleWidget);
  stackedWidget->addWidget(addHoleHandleCVWidget);
  stackedWidget->addWidget(addHandleSIWidget);
  stackedWidget->addWidget(rindModelingScalingWidget);
  stackedWidget->addWidget(rindModelingThicknessWidget);
  stackedWidget->addWidget(wireframeModelingWidget);
  stackedWidget->addWidget(columnModelingWidget);
  stackedWidget->addWidget(sierpinskyWidget);
  stackedWidget->addWidget(multiFaceHandleWidget);
  stackedWidget->addWidget(mengerSpongeWidget);

	//add the stackedwidget to the layout
  highgenusBoxLayout->addWidget(stackedWidget);

	//connect the combobox to the stacked widget
  connect(highgenusComboBox, SIGNAL(activated(int)),
          stackedWidget, SLOT(setCurrentIndex(int)));
	
	//setup stacked widget pages here, 
	//each in a separate function to make the code more readable
	setupAddHoleHandle();
	setupAddHoleHandleCV();
	setupAddHandleSI();
	setupRindModelingScaling();
	setupRindModelingThickness();
	setupWireframeModeling();
	setupColumnModeling();
	setupSierpinsky();
	setupMultiFaceHandle();
	setupMengerSponge();
	
	//set the layout
	highgenusGroup->setLayout(highgenusBoxLayout);
	
	connect(highgenusComboBox, SIGNAL(currentIndexChanged(int)),
					this,SLOT(switchMode(int)) );
}

void HighgenusMode::insertTab(QTabWidget *tabWidget){
	tabWidget->addTab( highgenusGroup, QIcon(":/images/mode_highgenus.png"), "5");
  tabWidget->setTabToolTip(0,tr("Highgenus Modeling"));
}

int HighgenusMode::getLastMode(){
	return lastMode;
}

void HighgenusMode::switchMode(int index){	
	switch(index){
		case 0: lastMode = DLFLWindow::ConnectFaceVertices;//add_hole_mode1(;
		break;
		case 1:	lastMode = DLFLWindow::ConnectFaces;//add_hole_mode2(;
		break;	
		case 2: lastMode = DLFLWindow::HermiteConnectFaces;//add_handle_mode(;
		break;
		case 3: lastMode = DLFLWindow::NormalMode;//crust_modeling_mode1(;
		break;
		case 4: lastMode = DLFLWindow::NormalMode;//crust_modeling_mode2(;
		break;
		case 5: lastMode = DLFLWindow::NormalMode;//wireframe_modeling_mode(;
		break;
		case 6: lastMode = DLFLWindow::NormalMode;//column_modeling_mode(;
		break;
		case 7: lastMode = DLFLWindow::NormalMode;//sierpinsky_mode(;
		break;
		case 8: lastMode = DLFLWindow::MultiSelectFace;//multi_face_handle_mode(;
		break;
		case 9: lastMode = DLFLWindow::NormalMode;//mengersponge_mode();
		break;
		default:
		break;
	};
	((MainWindow*)parentWidget())->setMode(lastMode);
}

void HighgenusMode::toggleCrustCleanupFlag(int state)
{
	rindModelingThicknessCleanupCheckBox->setChecked(state);
	rindModelingScalingCleanupCheckBox->setChecked(state);
	
	((MainWindow*)parentWidget())->toggleCrustCleanupFlag(state);
}

void HighgenusMode::setupAddHoleHandle(){
	
	addHoleHandleLayout = new QVBoxLayout;
	
	QLabel *addHoleHandleNumSegmentsLabel = new QLabel(tr("Number of Segments"));
	addHoleHandleNumSegmentsSpinBox = new QSpinBox;
	addHoleHandleNumSegmentsSpinBox->setRange(1, 100);
	addHoleHandleNumSegmentsSpinBox->setSingleStep(1);
	addHoleHandleNumSegmentsSpinBox->setValue(4);	
	connect(addHoleHandleNumSegmentsSpinBox, SIGNAL(valueChanged(int)),
          this, SLOT(numSegmentsValueChanged(int)));
	
	addHoleHandleLayout->addWidget(addHoleHandleNumSegmentsLabel);
  addHoleHandleLayout->addWidget(addHoleHandleNumSegmentsSpinBox);	
	
	QLabel *addHoleHandleNumSegmentsConnectLabel = new QLabel(tr("# Segments to Connect (0=all)"));
	addHoleHandleNumSegmentsConnectSpinBox = new QSpinBox;
	addHoleHandleNumSegmentsConnectSpinBox->setRange(0, 4);
	addHoleHandleNumSegmentsConnectSpinBox->setSingleStep(1);
	addHoleHandleNumSegmentsConnectSpinBox->setValue(0);	
	connect(addHoleHandleNumSegmentsConnectSpinBox, SIGNAL(valueChanged(int)),
          this, SLOT(numSegmentsConnectValueChanged(int)));
	
	addHoleHandleLayout->addWidget(addHoleHandleNumSegmentsConnectLabel);
  addHoleHandleLayout->addWidget(addHoleHandleNumSegmentsConnectSpinBox);	
	
	addHoleHandleLayout->addStretch(1);
	addHoleHandleWidget->setLayout(addHoleHandleLayout);	
	
}

//slot for num segments to reset the max segments range
void HighgenusMode::numSegmentsValueChanged(int value){
	
	((MainWindow*)parentWidget())->changeNumSegments(value);
	
	addHoleHandleNumSegmentsSpinBox->setValue(value);
	addHoleHandleCVNumSegmentsSpinBox->setValue(value);
	addHandleSINumSegmentsSpinBox->setValue(value);
	
	//set the # segments to connect range on all three hole/handle panels
	addHoleHandleNumSegmentsConnectSpinBox->setRange(0,value);
	addHoleHandleCVNumSegmentsConnectSpinBox->setRange(0,value);
	addHandleSINumSegmentsConnectSpinBox->setRange(0,value);
	
	if (addHoleHandleNumSegmentsConnectSpinBox->value() > value ||
			addHoleHandleCVNumSegmentsConnectSpinBox->value() > value ||
			addHandleSINumSegmentsConnectSpinBox->value() > value) {
				
		addHoleHandleNumSegmentsConnectSpinBox->setValue(value);
		addHoleHandleCVNumSegmentsConnectSpinBox->setValue(value);
		addHandleSINumSegmentsConnectSpinBox->setValue(value);
	}
}

//slot for num segments to set the max segments
void HighgenusMode::numSegmentsConnectValueChanged(int value){
	
	((MainWindow*)parentWidget())->changeMaxSegments(value);
	
	addHoleHandleNumSegmentsConnectSpinBox->setValue(value);
	addHoleHandleCVNumSegmentsConnectSpinBox->setValue(value);
	addHandleSINumSegmentsConnectSpinBox->setValue(value);	
}

void HighgenusMode::setupAddHoleHandleCV(){
	
	addHoleHandleCVLayout = new QVBoxLayout;

	QLabel *addHoleHandleCVNumSegmentsLabel = new QLabel(tr("Number of Segments"));
	addHoleHandleCVNumSegmentsSpinBox = new QSpinBox;
	addHoleHandleCVNumSegmentsSpinBox->setRange(1, 100);
	addHoleHandleCVNumSegmentsSpinBox->setSingleStep(1);
	addHoleHandleCVNumSegmentsSpinBox->setValue(4);	
	connect(addHoleHandleCVNumSegmentsSpinBox, SIGNAL(valueChanged(int)),
          this, SLOT(numSegmentsValueChanged(int)));

	addHoleHandleCVLayout->addWidget(addHoleHandleCVNumSegmentsLabel);
  addHoleHandleCVLayout->addWidget(addHoleHandleCVNumSegmentsSpinBox);	
	
	QLabel *addHoleHandleCVNumSegmentsConnectLabel = new QLabel(tr("# Segments to Connect (0=all)"));
	addHoleHandleCVNumSegmentsConnectSpinBox = new QSpinBox;
	addHoleHandleCVNumSegmentsConnectSpinBox->setRange(0, 4);
	addHoleHandleCVNumSegmentsConnectSpinBox->setSingleStep(1);
	addHoleHandleCVNumSegmentsConnectSpinBox->setValue(0);
	connect(addHoleHandleNumSegmentsConnectSpinBox, SIGNAL(valueChanged(int)),
          this, SLOT(numSegmentsConnectValueChanged(int)));
	
	addHoleHandleCVLayout->addWidget(addHoleHandleCVNumSegmentsConnectLabel);
  addHoleHandleCVLayout->addWidget(addHoleHandleCVNumSegmentsConnectSpinBox);	
	
	addHoleHandleCVLayout->addStretch(1);
	addHoleHandleCVWidget->setLayout(addHoleHandleCVLayout);	
}

void HighgenusMode::setupAddHandleSI(){
	
	addHandleSILayout = new QVBoxLayout;
	
	//number of segments
	QLabel *addHandleSINumSegmentsLabel = new QLabel(tr("Number of Segments"));
	addHandleSINumSegmentsSpinBox = new QSpinBox;
	addHandleSINumSegmentsSpinBox->setRange(1, 100);
	addHandleSINumSegmentsSpinBox->setSingleStep(1);
	addHandleSINumSegmentsSpinBox->setValue(4);
	connect(addHandleSINumSegmentsSpinBox, SIGNAL(valueChanged(int)),
          this, SLOT(numSegmentsValueChanged(int)));

	addHandleSILayout->addWidget(addHandleSINumSegmentsLabel);
  addHandleSILayout->addWidget(addHandleSINumSegmentsSpinBox);	
	
	//number of segments to connect
	QLabel *addHandleSINumSegmentsConnectLabel = new QLabel(tr("# Segments to Connect (0=all)"));
	addHandleSINumSegmentsConnectSpinBox = new QSpinBox;
	addHandleSINumSegmentsConnectSpinBox->setRange(0, 4);
	addHandleSINumSegmentsConnectSpinBox->setSingleStep(1);
	addHandleSINumSegmentsConnectSpinBox->setValue(0);
	connect(addHoleHandleNumSegmentsConnectSpinBox, SIGNAL(valueChanged(int)),
          this, SLOT(numSegmentsConnectValueChanged(int)));
	
	addHandleSILayout->addWidget(addHandleSINumSegmentsConnectLabel);
  addHandleSILayout->addWidget(addHandleSINumSegmentsConnectSpinBox);
	
	//weight 1
	QLabel *addHandleSIWeight1Label = new QLabel(tr("Weight 1"));
	QDoubleSpinBox *addHandleSIWeight1SpinBox = new QDoubleSpinBox;
	addHandleSIWeight1SpinBox->setRange(-100.0, 100.0);
	addHandleSIWeight1SpinBox->setSingleStep(0.1);
	addHandleSIWeight1SpinBox->setValue(5.0);
	addHandleSIWeight1SpinBox->setDecimals(1);
	connect(addHandleSIWeight1SpinBox, SIGNAL(valueChanged(double)),
          ((MainWindow*)parentWidget()), SLOT(changeWeight1(double)));
	
	addHandleSILayout->addWidget(addHandleSIWeight1Label);
  addHandleSILayout->addWidget(addHandleSIWeight1SpinBox);
	
	//symmetric weights checkbox
	QCheckBox *addHandleSISymmetricWeightsCheckBox = new QCheckBox(tr("Symmetric Weights"),this);
	addHandleSISymmetricWeightsCheckBox->setChecked(Qt::Checked);
	connect(addHandleSISymmetricWeightsCheckBox, SIGNAL(stateChanged(int)),
          this, SLOT(toggleSymmetricWeightsFlag(int)));
				

	addHandleSILayout->addWidget(addHandleSISymmetricWeightsCheckBox);
	
	//weight 2 - must be disabled when the checkbox is unchecked...
	QLabel *addHandleSIWeight2Label = new QLabel(tr("Weight 2:"));
	addHandleSIWeight2SpinBox = new QDoubleSpinBox;
	addHandleSIWeight2SpinBox->setRange(-100.0, 100.0);
	addHandleSIWeight2SpinBox->setSingleStep(0.1);
	addHandleSIWeight2SpinBox->setValue(5.0);
	addHandleSIWeight2SpinBox->setDecimals(1);
	//disabled by default
	addHandleSIWeight2SpinBox->setEnabled(false);
	
	connect(addHandleSIWeight2SpinBox, SIGNAL(valueChanged(double)),
          ((MainWindow*)parentWidget()), SLOT(changeWeight2(double)));

	addHandleSILayout->addWidget(addHandleSIWeight2Label);
  addHandleSILayout->addWidget(addHandleSIWeight2SpinBox);

	//add twists?
	QLabel *addHandleSITwistsConnectLabel = new QLabel(tr("Extra Twists:"));
	addHandleSITwistsConnectSpinBox = new QSpinBox;
	addHandleSITwistsConnectSpinBox->setRange(0, 100);
	addHandleSITwistsConnectSpinBox->setSingleStep(1);
	addHandleSITwistsConnectSpinBox->setValue(0);	
	connect(addHandleSITwistsConnectSpinBox, SIGNAL(valueChanged(int)),
          ((MainWindow*)parentWidget()), SLOT(changeExtraTwists(int)));
	
	addHandleSILayout->addWidget(addHandleSITwistsConnectLabel);
  addHandleSILayout->addWidget(addHandleSITwistsConnectSpinBox);

	addHandleSILayout->addStretch(1);
	addHandleSIWidget->setLayout(addHandleSILayout);	
	
}

void HighgenusMode::toggleSymmetricWeightsFlag(int state){
	
	((MainWindow*)parentWidget())->toggleSymmetricWeightsFlag(state);
	
	if (state){
		addHandleSIWeight2SpinBox->setEnabled(false);
	}
	else {
		addHandleSIWeight2SpinBox->setEnabled(true);
	}
}

void HighgenusMode::setupRindModelingScaling(){
	
	rindModelingScalingLayout = new QVBoxLayout;
	
	//scale factor - 0.00-10.00
	QLabel *rindModelingScalingLabel = new QLabel(tr("Scale:"));
	QDoubleSpinBox *rindModelingScalingSpinBox = new QDoubleSpinBox;
	rindModelingScalingSpinBox->setRange(0.0, 10.0);
	rindModelingScalingSpinBox->setSingleStep(0.01);
	rindModelingScalingSpinBox->setValue(0.9);
	rindModelingScalingSpinBox->setDecimals(2);
	connect(rindModelingScalingSpinBox, SIGNAL(valueChanged(double)),
          ((MainWindow*)parentWidget()), SLOT(changeCrustScaleFactor(double)));

	rindModelingScalingLayout->addWidget(rindModelingScalingLabel);
  rindModelingScalingLayout->addWidget(rindModelingScalingSpinBox);
	
	//symmetric weights checkbox
	rindModelingScalingCleanupCheckBox = new QCheckBox(tr("Cleanup when peeling"),this);
	rindModelingScalingCleanupCheckBox->setChecked(Qt::Unchecked);
	connect(rindModelingScalingCleanupCheckBox, SIGNAL(stateChanged(int)),
          this, SLOT(toggleCrustCleanupFlag(int)));

	rindModelingScalingLayout->addWidget(rindModelingScalingCleanupCheckBox);
	//create crust button
	QPushButton *rindModelingScalingCreateCrustButton = new QPushButton("Create Crust", this);
	connect(rindModelingScalingCreateCrustButton, SIGNAL(clicked()),
          ((MainWindow*)parentWidget()), SLOT(crust_modeling1()));

	rindModelingScalingLayout->addWidget(rindModelingScalingCreateCrustButton);	
	//set layout and add stretch
	rindModelingScalingLayout->addStretch(1);
	rindModelingScalingWidget->setLayout(rindModelingScalingLayout);	
}

void HighgenusMode::setupRindModelingThickness(){

	rindModelingThicknessLayout = new QVBoxLayout;
	
	//thickness
	QLabel *rindModelingThicknessLabel = new QLabel(tr("Thickness:"));
	QDoubleSpinBox *rindModelingThicknessSpinBox = new QDoubleSpinBox;
	rindModelingThicknessSpinBox->setRange(-1.0, 1.0);
	rindModelingThicknessSpinBox->setSingleStep(0.01);
	rindModelingThicknessSpinBox->setValue(0.5);
	rindModelingThicknessSpinBox->setDecimals(2);
	connect(rindModelingThicknessSpinBox, SIGNAL(valueChanged(double)),
          ((MainWindow*)parentWidget()), SLOT(changeCrustThickness(double)));
	
	rindModelingThicknessLayout->addWidget(rindModelingThicknessLabel);
  rindModelingThicknessLayout->addWidget(rindModelingThicknessSpinBox);
	
	//symmetric weights checkbox
	rindModelingThicknessCleanupCheckBox = new QCheckBox(tr("Cleanup when peeling"),this);
	rindModelingThicknessCleanupCheckBox->setChecked(Qt::Unchecked);
	connect(rindModelingThicknessCleanupCheckBox, SIGNAL(stateChanged(int)),
          this, SLOT(toggleCrustCleanupFlag(int)));

	rindModelingThicknessLayout->addWidget(rindModelingThicknessCleanupCheckBox);
	//create crust button
	QPushButton *rindModelingThicknessCreateButton = new QPushButton("Create Crust", this);
	connect(rindModelingThicknessCreateButton, SIGNAL(clicked()),
          ((MainWindow*)parentWidget()), SLOT(crust_modeling2()));

	rindModelingThicknessLayout->addWidget(rindModelingThicknessCreateButton);	
	//set layout and add stretch
	rindModelingThicknessLayout->addStretch(1);
	rindModelingThicknessWidget->setLayout(rindModelingThicknessLayout);
}

void HighgenusMode::setupWireframeModeling(){
	
	wireframeModelingLayout = new QVBoxLayout;
	
	//thickness
	QLabel *wireframeModelingThicknessLabel = new QLabel(tr("Thickness:"));
	QDoubleSpinBox *wireframeModelingThicknessSpinBox = new QDoubleSpinBox;
	wireframeModelingThicknessSpinBox->setRange(0.0, 1.0);
	wireframeModelingThicknessSpinBox->setSingleStep(0.01);
	wireframeModelingThicknessSpinBox->setValue(0.25);
	wireframeModelingThicknessSpinBox->setDecimals(2);
	connect(wireframeModelingThicknessSpinBox, SIGNAL(valueChanged(double)),
          ((MainWindow*)parentWidget()), SLOT(changeWireframeThickness(double)));
	
	wireframeModelingLayout->addWidget(wireframeModelingThicknessLabel);
  wireframeModelingLayout->addWidget(wireframeModelingThicknessSpinBox);
	//create wireframe button
	QPushButton *wireframeModelingCreateButton = new QPushButton("Create Wireframe", this);
	connect(wireframeModelingCreateButton, SIGNAL(clicked()),
          ((MainWindow*)parentWidget()), SLOT(wireframe_modeling()));
	wireframeModelingLayout->addWidget(wireframeModelingCreateButton);	
	//set layout and add stretch
	wireframeModelingLayout->addStretch(1);
	wireframeModelingWidget->setLayout(wireframeModelingLayout);
}

void HighgenusMode::setupColumnModeling(){
	
	columnModelingLayout = new QVBoxLayout;	
	//thickness
	QLabel *columnModelingThicknessLabel = new QLabel(tr("Thickness:"));
	QDoubleSpinBox *columnModelingThicknessSpinBox = new QDoubleSpinBox;
	columnModelingThicknessSpinBox->setRange(0.0, 1.0);
	columnModelingThicknessSpinBox->setSingleStep(0.01);
	columnModelingThicknessSpinBox->setValue(0.25);
	columnModelingThicknessSpinBox->setDecimals(2);
	connect(columnModelingThicknessSpinBox, SIGNAL(valueChanged(double)),
          ((MainWindow*)parentWidget()), SLOT(changeColumnThickness(double)));
	
	columnModelingLayout->addWidget(columnModelingThicknessLabel);
  columnModelingLayout->addWidget(columnModelingThicknessSpinBox);
	//number of segments
	QLabel *columnModelingNumSegmentsLabel = new QLabel(tr("Number of Segments:"));
	QSpinBox *columnModelingNumSegmentsSpinBox = new QSpinBox;
	columnModelingNumSegmentsSpinBox->setRange(4, 100);
	columnModelingNumSegmentsSpinBox->setSingleStep(1);
	columnModelingNumSegmentsSpinBox->setValue(0);
	connect(columnModelingNumSegmentsSpinBox, SIGNAL(valueChanged(int)),
          ((MainWindow*)parentWidget()), SLOT(changeColumnSegments(int)));

	columnModelingLayout->addWidget(columnModelingNumSegmentsLabel);
  columnModelingLayout->addWidget(columnModelingNumSegmentsSpinBox);
	//create column button
	QPushButton *columnModelingCreateButton = new QPushButton("Create Wireframe with Columns", this);
	connect(columnModelingCreateButton, SIGNAL(clicked()),
          ((MainWindow*)parentWidget()), SLOT(column_modeling()));
	columnModelingLayout->addWidget(columnModelingCreateButton);	
	//set layout and add stretch
	columnModelingLayout->addStretch(1);
	columnModelingWidget->setLayout(columnModelingLayout);	
}

void HighgenusMode::setupSierpinsky(){
	
	sierpinskyLayout = new QVBoxLayout;	
	//create column button
	QPushButton *sierpinskyButton = new QPushButton("Create Sierpinsky Tetrahedra", this);
	connect(sierpinskyButton, SIGNAL(clicked()),
          ((MainWindow*)parentWidget()), SLOT(create_sierpinsky_tetrahedra()));

	sierpinskyLayout->addWidget(sierpinskyButton);
	
	sierpinskyLayout->addStretch(1);
	sierpinskyWidget->setLayout(sierpinskyLayout);	
}

void HighgenusMode::setupMultiFaceHandle(){
	
	multiFaceHandleLayout = new QVBoxLayout;
	
	QGroupBox *multiFaceAlgorithmGroupBox = new QGroupBox(tr("Use Convex Hull Algorithm"));
	multiFaceAlgorithmGroupBox->setCheckable(true);
	multiFaceAlgorithmGroupBox->setChecked(false);
	
	QVBoxLayout *multiFaceAlgorithmLayout = new QVBoxLayout;
	QButtonGroup *multiFaceAlgorithmButtonGroup = new QButtonGroup();
	// QRadioButton *convexHullRadioButton = new QRadioButton(tr("Use Convex Hull Algorithm") );
	// QRadioButton *closestEdgeRadioButton = new QRadioButton(tr("Closest Edge") );
	// convexHullRadioButton->setChecked(true);
// 	multiFaceAlgorithmButtonGroup->addButton(convexHullRadioButton);
// 	multiFaceAlgorithmButtonGroup->addButton(closestEdgeRadioButton);
// 	
	// connect(multiFaceAlgorithmButtonGroup, SIGNAL(buttonClicked(int)),
	//           this, SLOT(changeMultiFaceAlgorithm(int)));
	// connect(multiFaceAlgorithmButtonGroup, SIGNAL(buttonClicked(1)),
	//           this, SLOT(changeMultiFaceAlgorithm(int)));

	// multiFaceAlgorithmLayout->addWidget(convexHullRadioButton);
	// multiFaceAlgorithmLayout->addWidget(closestEdgeRadioButton);

	connect(multiFaceAlgorithmGroupBox, SIGNAL(toggled(bool)),
	          this, SLOT(changeMultiFaceAlgorithm(bool)));
	
	//scaling
	multiFaceHandleScaleLabel = new QLabel(tr("Scale Factor:"));
	multiFaceHandleScaleSpinBox = new QDoubleSpinBox;
	multiFaceHandleScaleSpinBox->setRange(0.0, 5.0);
	multiFaceHandleScaleSpinBox->setSingleStep(0.01);
	multiFaceHandleScaleSpinBox->setValue(1.0);
	multiFaceHandleScaleSpinBox->setDecimals(2);
	connect(multiFaceHandleScaleSpinBox, SIGNAL(valueChanged(double)),
          ((MainWindow*)parentWidget()), SLOT(changeMultiFaceHandleScaleFactor(double)));
	
	multiFaceAlgorithmLayout->addWidget(multiFaceHandleScaleLabel);
  multiFaceAlgorithmLayout->addWidget(multiFaceHandleScaleSpinBox);	
	
	//extrude distance
	multiFaceHandleExtrudeDistanceLabel = new QLabel(tr("Extrude Distance Factor:"));
	multiFaceHandleExtrudeDistanceSpinBox = new QDoubleSpinBox;
	multiFaceHandleExtrudeDistanceSpinBox->setRange(-2.0, 2.0);
	multiFaceHandleExtrudeDistanceSpinBox->setSingleStep(0.1);
	multiFaceHandleExtrudeDistanceSpinBox->setValue(0.5);
	multiFaceHandleExtrudeDistanceSpinBox->setDecimals(2);
	connect(multiFaceHandleExtrudeDistanceSpinBox, SIGNAL(valueChanged(double)),
          ((MainWindow*)parentWidget()), SLOT(changeMultiFaceHandleExtrudeDist(double)));
	
	multiFaceAlgorithmLayout->addWidget(multiFaceHandleExtrudeDistanceLabel);
  multiFaceAlgorithmLayout->addWidget(multiFaceHandleExtrudeDistanceSpinBox);
	
	//use max. offsets
	multiFaceHandleMaxOffsetsCheckBox = new QCheckBox(tr("Use max offsets"),this);
	connect(multiFaceHandleMaxOffsetsCheckBox, SIGNAL(stateChanged(int)),
          this, SLOT(toggleMultiFaceHandleUseMaxOffsetFlag(int)));

	multiFaceAlgorithmLayout->addWidget(multiFaceHandleMaxOffsetsCheckBox);

	multiFaceAlgorithmGroupBox->setLayout(multiFaceAlgorithmLayout);	
	//add the groupbox to the multiface layout
	multiFaceHandleLayout->addWidget(multiFaceAlgorithmGroupBox);

	//create column button
	QPushButton *multiFaceHandleButton = new QPushButton("Connect Selected Faces", this);
	connect(multiFaceHandleButton, SIGNAL(clicked()),
          ((MainWindow*)parentWidget()), SLOT(create_multi_face_handle()));

	multiFaceHandleLayout->addWidget(multiFaceHandleButton);
		
	multiFaceHandleLayout->addStretch(1);
	multiFaceHandleWidget->setLayout(multiFaceHandleLayout);
}

void HighgenusMode::toggleMultiFaceHandleUseMaxOffsetFlag(int state){
	
	((MainWindow*)parentWidget())->toggleMultiFaceHandleUseMaxOffsetFlag(state);
	
	if (state){
		multiFaceHandleExtrudeDistanceSpinBox->setEnabled(false);
		multiFaceHandleExtrudeDistanceLabel->setEnabled(false);
	}
	else {
		multiFaceHandleExtrudeDistanceLabel->setEnabled(true);
		multiFaceHandleExtrudeDistanceSpinBox->setEnabled(true);
	}
}

void HighgenusMode::changeMultiFaceAlgorithm(bool on){
	
	if (!on) {
		((MainWindow*)parentWidget())->mfh_use_closest_edge_algo();	
		// //set all three widgets to be disabled
		// multiFaceHandleScaleLabel->setEnabled(false);
		// multiFaceHandleExtrudeDistanceLabel->setEnabled(false);
		// multiFaceHandleExtrudeDistanceSpinBox->setEnabled(false);
		// multiFaceHandleMaxOffsetsCheckBox->setEnabled(false);
		// multiFaceHandleScaleSpinBox->setEnabled(false);		
		
	}
	else { 		
		((MainWindow*)parentWidget())->mfh_use_convex_hull_algo();	
		// 
		// multiFaceHandleScaleLabel->setEnabled(false);
		// multiFaceHandleScaleSpinBox->setEnabled(true);
		// multiFaceHandleMaxOffsetsCheckBox->setEnabled(true);
		// 
		// //check if the checkbox is checked first or not, then enable widgets accordingly
		// if (multiFaceHandleMaxOffsetsCheckBox->checkState() == Qt::Checked){
		// 	multiFaceHandleExtrudeDistanceLabel->setEnabled(true);
		// 	multiFaceHandleExtrudeDistanceSpinBox->setEnabled(true);
		// }
	}
}

void HighgenusMode::setupMengerSponge(){
	
	mengerSpongeLayout = new QVBoxLayout;
	
	//thickness
	QLabel *mengerSpongeThicknessLabel = new QLabel(tr("Thickness:"));
	QDoubleSpinBox *mengerSpongeThicknessSpinBox = new QDoubleSpinBox;
	mengerSpongeThicknessSpinBox->setRange(0.01, 1.0);
	mengerSpongeThicknessSpinBox->setSingleStep(0.01);
	mengerSpongeThicknessSpinBox->setValue(0.67);
	mengerSpongeThicknessSpinBox->setDecimals(2);
	connect(mengerSpongeThicknessSpinBox, SIGNAL(valueChanged(double)),
          ((MainWindow*)parentWidget()), SLOT(changeMengerSpongeThickness(double)));

	mengerSpongeLayout->addWidget(mengerSpongeThicknessLabel);
  mengerSpongeLayout->addWidget(mengerSpongeThicknessSpinBox);	
	
	//threshold
	QLabel *mengerSpongeThresholdLabel = new QLabel(tr("Edge Collapse Threshold Factor:"));
	QDoubleSpinBox *mengerSpongeThresholdSpinBox = new QDoubleSpinBox;
	mengerSpongeThresholdSpinBox->setRange(0.0, 10.0);
	mengerSpongeThresholdSpinBox->setSingleStep(0.1);
	mengerSpongeThresholdSpinBox->setValue(0.0);
	mengerSpongeThresholdSpinBox->setDecimals(1);
	connect(mengerSpongeThresholdSpinBox, SIGNAL(valueChanged(double)),
          ((MainWindow*)parentWidget()), SLOT(changeMengerSpongeCollapseThreshold(double)));

	mengerSpongeLayout->addWidget(mengerSpongeThresholdLabel);
  mengerSpongeLayout->addWidget(mengerSpongeThresholdSpinBox);	
	
	//fractional thickness checkbox
	QCheckBox *mengerSpongFractionalThicknessCheckBox = new QCheckBox(tr("Fractional Thickness"),this);
	connect(mengerSpongFractionalThicknessCheckBox, SIGNAL(stateChanged(int)),
          ((MainWindow*)parentWidget()), SLOT(toggleMengerSpongeFractionalThicknessFlag(int)));
	mengerSpongeLayout->addWidget(mengerSpongFractionalThicknessCheckBox);
	
	//create menger sponge button
	QPushButton *mengerSpongeButton = new QPushButton("Create Menger Sponge", this);
	connect(mengerSpongeButton, SIGNAL(clicked()),
          ((MainWindow*)parentWidget()), SLOT(create_menger_sponge()));

	mengerSpongeLayout->addWidget(mengerSpongeButton);
	
	mengerSpongeLayout->addStretch(1);
	mengerSpongeWidget->setLayout(mengerSpongeLayout);
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