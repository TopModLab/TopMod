/****************************************************************************
 **
 **
 ****************************************************************************/
#include <QtGui>

#include "ExtrusionMode.hh"

ExtrusionMode::ExtrusionMode(QWidget *parent)
		: QWidget(parent)
{		
	this->setParent(parent);
	lastMode = DLFLWindow::ExtrudeFaceDS;

	//mode group. this is the main widget that goes into a TabWidget
  extrusionGroup = new QGroupBox(tr("Extrusion"));
	//boxLayout widget to add buttons to within the groupBox widget
	extrusionBoxLayout = new QVBoxLayout;

	//Populate a Q Combo Box with a list of strings
	extrusionLabel = new QLabel(tr("Select Extrusion"));
	extrusionComboBox = new QComboBox();
	QStringList extrusionList;
	extrusionList << "Doo Sabin Extrude" << "Cubical Extrude" << "Dodecahedral Extrude" << "Icosahedral Extrude" 
								<< "Octahedral Extrude" << "Stellate" << "Double Stellate";
								
	extrusionComboBox->addItems(extrusionList);
	//add the combobox to the layout
	extrusionBoxLayout->addWidget(extrusionLabel);
	extrusionBoxLayout->addWidget(extrusionComboBox);
	
	//create the stacked widget and all child widget pages
	dooSabinExtrudeWidget = new QWidget;
  cubicalExtrudeWidget = new QWidget;
  dodecahedralExtrudeWidget = new QWidget;
  icosehedralExtrudeWidget = new QWidget;
  octahedralExtrudeWidget = new QWidget;
  stellateExtrudeWidget = new QWidget;
  doubleStellateExtrudeWidget = new QWidget;

  stackedWidget = new QStackedWidget;
  stackedWidget->addWidget(dooSabinExtrudeWidget);
  stackedWidget->addWidget(cubicalExtrudeWidget);
  stackedWidget->addWidget(dodecahedralExtrudeWidget);
  stackedWidget->addWidget(icosehedralExtrudeWidget);
  stackedWidget->addWidget(octahedralExtrudeWidget);
  stackedWidget->addWidget(stellateExtrudeWidget);
  stackedWidget->addWidget(doubleStellateExtrudeWidget);

	//add the stackedwidget to the layout
  extrusionBoxLayout->addWidget(stackedWidget);

	//connect the combobox to the stacked widget
  connect(extrusionComboBox, SIGNAL(activated(int)),
          stackedWidget, SLOT(setCurrentIndex(int)));
	
	//setup stacked widget pages here, 
	//each in a separate function to make the code more readable
	setupDooSabinExtrude();
	setupCubicalExtrude();
	setupDodecahedralExtrude();
	setupIcosahedralExtrude();
	setupOctahedralExtrude();
	setupStellateExtrude();
	setupDoubleStellateExtrude();
 	
	//set the layout
	extrusionGroup->setLayout(extrusionBoxLayout);

	connect(extrusionComboBox, SIGNAL(currentIndexChanged(int)),
					this,SLOT(switchMode(int)) );	
}

void ExtrusionMode::insertTab(QTabWidget *tabWidget){
	tabWidget->addTab( extrusionGroup, QIcon(":/images/mode_extrusion.png"), "2");
  tabWidget->setTabToolTip(0,tr("Extrusions Mode"));
}

int ExtrusionMode::getLastMode(){
	return lastMode;
}

void ExtrusionMode::switchMode(int index){	
	switch(index){
		case 0: lastMode = DLFLWindow::ExtrudeFaceDS;
		break;
		case 1:	lastMode = DLFLWindow::ExtrudeFace;
		break;	
		case 2: lastMode = DLFLWindow::ExtrudeFaceDodeca;
		break;
		case 3: lastMode = DLFLWindow::ExtrudeFaceIcosa;
		break;
		case 4: lastMode = DLFLWindow::ExtrudeDualFace;
		break;
		case 5: lastMode = DLFLWindow::StellateFace;
		break;
		case 6: lastMode = DLFLWindow::DoubleStellateFace;
		break;
		default:
		break;
	};
	((MainWindow*)parentWidget())->setMode(lastMode);
}

void ExtrusionMode::setLength(double value){
	dooSabinLengthSpinBox->setValue(value);
	cubicalLengthSpinBox->setValue(value);
	dodecahedralLengthSpinBox->setValue(value);
	icosahedralLengthSpinBox->setValue(value);
	octahedralLengthSpinBox->setValue(value);
	stellateLengthSpinBox->setValue(value);
	doubleStellateLengthSpinBox->setValue(value);
	
	((MainWindow*)parentWidget())->changeExtrudeLength(value);
}
void ExtrusionMode::setRotation(int value){
	dooSabinRotationSpinBox->setValue(value);
	cubicalRotationSpinBox->setValue(value);
	octahedralRotationSpinBox->setValue(value);
	
	((MainWindow*)parentWidget())->changeExtrudeRotation(value);
}

void ExtrusionMode::setSegments(int value){
	dooSabinSegmentsSpinBox->setValue(value);
	cubicalSegmentsSpinBox->setValue(value);
	dodecahedralSegmentsSpinBox->setValue(value);
	icosahedralSegmentsSpinBox->setValue(value);
	octahedralSegmentsSpinBox->setValue(value);

	((MainWindow*)parentWidget())->changeNumExtrusions(value);
}

void ExtrusionMode::setScale(double value){
	dooSabinScaleSpinBox->setValue(value);
	cubicalScaleSpinBox->setValue(value);
	dodecahedralScaleSpinBox->setValue(value);
	icosahedralScaleSpinBox->setValue(value);
	octahedralScaleSpinBox->setValue(value);
		
	((MainWindow*)parentWidget())->changeExtrudeScale(value);
}

void ExtrusionMode::setupDooSabinExtrude(){
	
	dooSabinExtrudeLayout = new QVBoxLayout;
	
	//length
	dooSabinLengthLabel = new QLabel(tr("Length"));
	dooSabinLengthSpinBox = new QDoubleSpinBox;
	dooSabinLengthSpinBox->setRange(-100.0, 100.0);
	dooSabinLengthSpinBox->setSingleStep(0.1);
	dooSabinLengthSpinBox->setValue(1.0);
	dooSabinLengthSpinBox->setDecimals(1);
	connect(dooSabinLengthSpinBox, SIGNAL(valueChanged(double)),
          this, SLOT(setLength(double)));

	dooSabinExtrudeLayout->addWidget(dooSabinLengthLabel);
  dooSabinExtrudeLayout->addWidget(dooSabinLengthSpinBox);

	//rotation
	dooSabinRotationLabel = new QLabel(tr("Rotation"));
	dooSabinRotationSpinBox = new QSpinBox;
	dooSabinRotationSpinBox->setRange(0, 360);
	dooSabinRotationSpinBox->setSingleStep(1);
	dooSabinRotationSpinBox->setValue(0.0);
	connect(dooSabinRotationSpinBox, SIGNAL(valueChanged(int)),
          this, SLOT(setRotation(int)));
	
	dooSabinExtrudeLayout->addWidget(dooSabinRotationLabel);
  dooSabinExtrudeLayout->addWidget(dooSabinRotationSpinBox);

	//scale
	dooSabinScaleLabel = new QLabel(tr("Scale"));
	dooSabinScaleSpinBox = new QDoubleSpinBox;
	dooSabinScaleSpinBox->setRange(0.0, 10.0);
	dooSabinScaleSpinBox->setSingleStep(0.01);
	dooSabinScaleSpinBox->setValue(1.0);
	dooSabinScaleSpinBox->setDecimals(2);
	connect(dooSabinScaleSpinBox, SIGNAL(valueChanged(double)),
          this, SLOT(setScale(double)));
	
	dooSabinExtrudeLayout->addWidget(dooSabinScaleLabel);
  dooSabinExtrudeLayout->addWidget(dooSabinScaleSpinBox);
	// dooSabinExtrudeLayout->addStretch(1);

	//segments
	dooSabinSegmentsLabel = new QLabel(tr("Segments"));
	dooSabinSegmentsSpinBox = new QSpinBox;
	dooSabinSegmentsSpinBox->setRange(1, 50);
	dooSabinSegmentsSpinBox->setSingleStep(1);
	dooSabinSegmentsSpinBox->setValue(1);
	connect(dooSabinSegmentsSpinBox, SIGNAL(valueChanged(int)),
          this, SLOT(setSegments(int)));	
	
	dooSabinExtrudeLayout->addWidget(dooSabinSegmentsLabel);
  dooSabinExtrudeLayout->addWidget(dooSabinSegmentsSpinBox);
	dooSabinExtrudeLayout->addStretch(1);

	// //bendT
	// QLabel *dooSabinBendTLabel = new QLabel(tr("Bend T"));
	// QDoubleSpinBox *dooSabinBendTSpinBox = new QDoubleSpinBox;
	// dooSabinBendTSpinBox->setRange(-100.0, 100.0);
	// dooSabinBendTSpinBox->setSingleStep(0.1);
	// dooSabinBendTSpinBox->setValue(0.0);
	// dooSabinBendTSpinBox->setDecimals(1);
	// dooSabinExtrudeLayout->addWidget(dooSabinBendTLabel);
	//   dooSabinExtrudeLayout->addWidget(dooSabinBendTSpinBox);
	// dooSabinExtrudeLayout->addStretch(1);
	// 
	// //bendB
	// QLabel *dooSabinBendBLabel = new QLabel(tr("Bend B"));
	// QDoubleSpinBox *dooSabinBendBSpinBox = new QDoubleSpinBox;
	// dooSabinBendBSpinBox->setRange(-100.0, 100.0);
	// dooSabinBendBSpinBox->setSingleStep(0.1);
	// dooSabinBendBSpinBox->setValue(0.0);
	// dooSabinBendBSpinBox->setDecimals(1);
	// dooSabinExtrudeLayout->addWidget(dooSabinBendBLabel);
	//   dooSabinExtrudeLayout->addWidget(dooSabinBendBSpinBox);
	
	//dooSabinExtrudeLayout->addStretch(1);
	dooSabinExtrudeWidget->setLayout(dooSabinExtrudeLayout);	
	
}

void ExtrusionMode::setupCubicalExtrude(){
	
	cubicalExtrudeLayout = new QVBoxLayout;

	//length
	cubicalLengthLabel = new QLabel(tr("Length"));
	cubicalLengthSpinBox = new QDoubleSpinBox;
	cubicalLengthSpinBox->setRange(-100.0, 100.0);
	cubicalLengthSpinBox->setSingleStep(0.1);
	cubicalLengthSpinBox->setValue(1.0);
	cubicalLengthSpinBox->setDecimals(1);
	connect(cubicalLengthSpinBox, SIGNAL(valueChanged(double)),
          this, SLOT(setLength(double)));

	cubicalExtrudeLayout->addWidget(cubicalLengthLabel);
  cubicalExtrudeLayout->addWidget(cubicalLengthSpinBox);

	//rotation
	cubicalRotationLabel = new QLabel(tr("Rotation"));
	cubicalRotationSpinBox = new QSpinBox;
	cubicalRotationSpinBox->setRange(0, 360);
	cubicalRotationSpinBox->setSingleStep(1);
	cubicalRotationSpinBox->setValue(0.0);
	connect(cubicalRotationSpinBox, SIGNAL(valueChanged(int)),
          this, SLOT(setRotation(int)));

	cubicalExtrudeLayout->addWidget(cubicalRotationLabel);
  cubicalExtrudeLayout->addWidget(cubicalRotationSpinBox);

	//scale
	cubicalScaleLabel = new QLabel(tr("Scale"));
	cubicalScaleSpinBox = new QDoubleSpinBox;
	cubicalScaleSpinBox->setRange(0.0, 10.0);
	cubicalScaleSpinBox->setSingleStep(0.01);
	cubicalScaleSpinBox->setValue(1.0);
	cubicalScaleSpinBox->setDecimals(2);
	connect(cubicalScaleSpinBox, SIGNAL(valueChanged(double)),
          this, SLOT(setScale(double)));
	
	cubicalExtrudeLayout->addWidget(cubicalScaleLabel);
  cubicalExtrudeLayout->addWidget(cubicalScaleSpinBox);

	//segments
	cubicalSegmentsLabel = new QLabel(tr("Segments"));
	cubicalSegmentsSpinBox = new QSpinBox;
	cubicalSegmentsSpinBox->setRange(1, 50);
	cubicalSegmentsSpinBox->setSingleStep(1);
	cubicalSegmentsSpinBox->setValue(1);
	connect(cubicalSegmentsSpinBox, SIGNAL(valueChanged(int)),
          this, SLOT(setSegments(int)));

	cubicalExtrudeLayout->addWidget(cubicalSegmentsLabel);
  cubicalExtrudeLayout->addWidget(cubicalSegmentsSpinBox);

	// //bendT
	// QLabel *cubicalBendTLabel = new QLabel(tr("Bend T"));
	// QDoubleSpinBox *cubicalBendTSpinBox = new QDoubleSpinBox;
	// cubicalBendTSpinBox->setRange(-100.0, 100.0);
	// cubicalBendTSpinBox->setSingleStep(0.1);
	// cubicalBendTSpinBox->setValue(0.0);
	// cubicalBendTSpinBox->setDecimals(1);
	// cubicalExtrudeLayout->addWidget(cubicalBendTLabel);
	//   cubicalExtrudeLayout->addWidget(cubicalBendTSpinBox);
	// cubicalExtrudeLayout->addStretch(1);
	// 
	// //bendB
	// QLabel *cubicalBendBLabel = new QLabel(tr("Bend B"));
	// QDoubleSpinBox *cubicalBendBSpinBox = new QDoubleSpinBox;
	// cubicalBendBSpinBox->setRange(-100.0, 100.0);
	// cubicalBendBSpinBox->setSingleStep(0.1);
	// cubicalBendBSpinBox->setValue(0.0);
	// cubicalBendBSpinBox->setDecimals(1);
	// cubicalExtrudeLayout->addWidget(cubicalBendBLabel);
	//   cubicalExtrudeLayout->addWidget(cubicalBendBSpinBox);
	
	cubicalExtrudeLayout->addStretch(1);
	cubicalExtrudeWidget->setLayout(cubicalExtrudeLayout);
}

void ExtrusionMode::setupDodecahedralExtrude(){
	
	dodecahedralExtrudeLayout = new QVBoxLayout;
	
	//length
	dodecahedralLengthLabel = new QLabel(tr("Length:"));
	dodecahedralLengthSpinBox = new QDoubleSpinBox;
	dodecahedralLengthSpinBox->setRange(-100, 100);
	dodecahedralLengthSpinBox->setSingleStep(0.1);
	dodecahedralLengthSpinBox->setValue(1.0);
	dodecahedralLengthSpinBox->setDecimals(1);
	connect(dodecahedralLengthSpinBox, SIGNAL(valueChanged(double)),
          this, SLOT(setLength(double)));
	
	dodecahedralExtrudeLayout->addWidget(dodecahedralLengthLabel);
  dodecahedralExtrudeLayout->addWidget(dodecahedralLengthSpinBox);

	//scale
	dodecahedralScaleLabel = new QLabel(tr("Scale:"));
	dodecahedralScaleSpinBox = new QDoubleSpinBox;
	dodecahedralScaleSpinBox->setRange(0, 10);
	dodecahedralScaleSpinBox->setSingleStep(.01);
	dodecahedralScaleSpinBox->setValue(1);
	connect(dodecahedralScaleSpinBox, SIGNAL(valueChanged(double)),
          this, SLOT(setScale(double)));

	dodecahedralExtrudeLayout->addWidget(dodecahedralScaleLabel);
  dodecahedralExtrudeLayout->addWidget(dodecahedralScaleSpinBox);

	//segments
	dodecahedralSegmentsLabel = new QLabel(tr("Segments:"));
	dodecahedralSegmentsSpinBox = new QSpinBox;
	dodecahedralSegmentsSpinBox->setRange(1, 50);
	dodecahedralSegmentsSpinBox->setSingleStep(1);
	dodecahedralSegmentsSpinBox->setValue(1);
	connect(dodecahedralSegmentsSpinBox, SIGNAL(valueChanged(int)),
          this, SLOT(setSegments(int)));

	dodecahedralExtrudeLayout->addWidget(dodecahedralSegmentsLabel);
  dodecahedralExtrudeLayout->addWidget(dodecahedralSegmentsSpinBox);
	
	//mesh flat edges checkbox
	hexagonalizeCheckBox = new QCheckBox(tr("Hexagonalize"),this);
	connect(hexagonalizeCheckBox, SIGNAL(stateChanged(int)),
          ((MainWindow*)parentWidget()), SLOT(toggleHexagonalizeDodecaExtrudeFlag(int)));
	
	dodecahedralExtrudeLayout->addWidget(hexagonalizeCheckBox);

	dodecahedralExtrudeLayout->addStretch(1);
	dodecahedralExtrudeWidget->setLayout(dodecahedralExtrudeLayout);	
}

void ExtrusionMode::setupIcosahedralExtrude(){
	
	icosahedralExtrudeLayout = new QVBoxLayout;

	//length
	icosahedralLengthLabel = new QLabel(tr("Length:"));
	icosahedralLengthSpinBox = new QDoubleSpinBox;
	icosahedralLengthSpinBox->setRange(-100, 100);
	icosahedralLengthSpinBox->setSingleStep(0.1);
	icosahedralLengthSpinBox->setValue(1.0);
	icosahedralLengthSpinBox->setDecimals(1);
	connect(icosahedralLengthSpinBox, SIGNAL(valueChanged(double)),
          this, SLOT(setLength(double)));
	
	icosahedralExtrudeLayout->addWidget(icosahedralLengthLabel);
  icosahedralExtrudeLayout->addWidget(icosahedralLengthSpinBox);

	//scale
	icosahedralScaleLabel = new QLabel(tr("Scale:"));
	icosahedralScaleSpinBox = new QDoubleSpinBox;
	icosahedralScaleSpinBox->setRange(0, 10);
	icosahedralScaleSpinBox->setSingleStep(.01);
	icosahedralScaleSpinBox->setValue(1);
	connect(icosahedralScaleSpinBox, SIGNAL(valueChanged(double)),
          this, SLOT(setScale(double)));

	icosahedralExtrudeLayout->addWidget(icosahedralScaleLabel);
  icosahedralExtrudeLayout->addWidget(icosahedralScaleSpinBox);

	//segments
	icosahedralSegmentsLabel = new QLabel(tr("Segments:"));
	icosahedralSegmentsSpinBox = new QSpinBox;
	icosahedralSegmentsSpinBox->setRange(1, 50);
	icosahedralSegmentsSpinBox->setSingleStep(1);
	icosahedralSegmentsSpinBox->setValue(1);
	connect(icosahedralSegmentsSpinBox, SIGNAL(valueChanged(int)),
          this, SLOT(setSegments(int)));

	icosahedralExtrudeLayout->addWidget(icosahedralSegmentsLabel);
  icosahedralExtrudeLayout->addWidget(icosahedralSegmentsSpinBox);
	
	icosahedralExtrudeLayout->addStretch(1);
	icosehedralExtrudeWidget->setLayout(icosahedralExtrudeLayout);	
}

void ExtrusionMode::setupOctahedralExtrude(){
	
	octahedralExtrudeLayout = new QVBoxLayout;
	
	//length
	octahedralLengthLabel = new QLabel(tr("Length"));
	octahedralLengthSpinBox = new QDoubleSpinBox;
	octahedralLengthSpinBox->setRange(-100.0, 100.0);
	octahedralLengthSpinBox->setSingleStep(0.1);
	octahedralLengthSpinBox->setValue(1.0);
	octahedralLengthSpinBox->setDecimals(1);
	connect(octahedralLengthSpinBox, SIGNAL(valueChanged(double)),
          this, SLOT(setLength(double)));

	octahedralExtrudeLayout->addWidget(octahedralLengthLabel);
  octahedralExtrudeLayout->addWidget(octahedralLengthSpinBox);
	// octahedralExtrudeLayout->addStretch(1);

	//rotation
	octahedralRotationLabel = new QLabel(tr("Rotation"));
	octahedralRotationSpinBox = new QSpinBox;
	octahedralRotationSpinBox->setRange(0, 360);
	octahedralRotationSpinBox->setSingleStep(1);
	octahedralRotationSpinBox->setValue(0.0);
	connect(octahedralRotationSpinBox, SIGNAL(valueChanged(int)),
          this, SLOT(setRotation(int)));

	octahedralExtrudeLayout->addWidget(octahedralRotationLabel);
  octahedralExtrudeLayout->addWidget(octahedralRotationSpinBox);
	// octahedralExtrudeLayout->addStretch(1);

	//scale
	octahedralScaleLabel = new QLabel(tr("Scale"));
	octahedralScaleSpinBox = new QDoubleSpinBox;
	octahedralScaleSpinBox->setRange(0.0, 10.0);
	octahedralScaleSpinBox->setSingleStep(0.01);
	octahedralScaleSpinBox->setValue(1.0);
	octahedralScaleSpinBox->setDecimals(2);
	connect(octahedralScaleSpinBox, SIGNAL(valueChanged(double)),
          this, SLOT(setScale(double)));

	octahedralExtrudeLayout->addWidget(octahedralScaleLabel);
  octahedralExtrudeLayout->addWidget(octahedralScaleSpinBox);
	// octahedralExtrudeLayout->addStretch(1);

	//segments
	octahedralSegmentsLabel = new QLabel(tr("Segments"));
	octahedralSegmentsSpinBox = new QSpinBox;
	octahedralSegmentsSpinBox->setRange(1, 50);
	octahedralSegmentsSpinBox->setSingleStep(1);
	octahedralSegmentsSpinBox->setValue(1);
	connect(octahedralSegmentsSpinBox, SIGNAL(valueChanged(int)),
          this, SLOT(setSegments(int)));

	octahedralExtrudeLayout->addWidget(octahedralSegmentsLabel);
  octahedralExtrudeLayout->addWidget(octahedralSegmentsSpinBox);
	// octahedralExtrudeLayout->addStretch(1);
	
	//mesh flat edges checkbox
	meshFlatEdgesCheckBox = new QCheckBox(tr("Mesh Flat Edges"),this);
	connect(meshFlatEdgesCheckBox, SIGNAL(stateChanged(int)),
          ((MainWindow*)parentWidget()), SLOT(toggleDualMeshEdgesFlag(int)));
	octahedralExtrudeLayout->addWidget(meshFlatEdgesCheckBox);

	octahedralExtrudeLayout->addStretch(1);
	octahedralExtrudeWidget->setLayout(octahedralExtrudeLayout);	
}

void ExtrusionMode::setupStellateExtrude(){
	
	stellateExtrudeLayout = new QVBoxLayout;
	
	//length
	stellateLengthLabel = new QLabel(tr("Length"));
	stellateLengthSpinBox = new QDoubleSpinBox;
	stellateLengthSpinBox->setRange(-100.0, 100.0);
	stellateLengthSpinBox->setSingleStep(0.1);
	stellateLengthSpinBox->setValue(1.0);
	stellateLengthSpinBox->setDecimals(1);
	connect(stellateLengthSpinBox, SIGNAL(valueChanged(double)),
          this, SLOT(changeStellateLength(double)));

	stellateExtrudeLayout->addWidget(stellateLengthLabel);
	stellateExtrudeLayout->addWidget(stellateLengthSpinBox);

	stellateExtrudeLayout->addStretch(1);	
	stellateExtrudeWidget->setLayout(stellateExtrudeLayout);
}

void ExtrusionMode::setupDoubleStellateExtrude(){
	
	doubleStellateExtrudeLayout = new QVBoxLayout;

	//length
	doubleStellateLengthLabel = new QLabel(tr("Length"));
	doubleStellateLengthSpinBox = new QDoubleSpinBox;
	doubleStellateLengthSpinBox->setRange(-100.0, 100.0);
	doubleStellateLengthSpinBox->setSingleStep(0.1);
	doubleStellateLengthSpinBox->setValue(1.0);
	doubleStellateLengthSpinBox->setDecimals(1);
	connect(doubleStellateLengthSpinBox, SIGNAL(valueChanged(double)),
          this, SLOT(changeStellateLength(double)));
	
	doubleStellateExtrudeLayout->addWidget(doubleStellateLengthLabel);
	doubleStellateExtrudeLayout->addWidget(doubleStellateLengthSpinBox);
	
	doubleStellateExtrudeLayout->addStretch(1);
	doubleStellateExtrudeWidget->setLayout(doubleStellateExtrudeLayout);	
}