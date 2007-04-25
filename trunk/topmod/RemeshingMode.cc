/****************************************************************************
 **
 **
 ****************************************************************************/
#include <QtGui>

#include "RemeshingMode.hh"

RemeshingMode::RemeshingMode(QWidget *parent)
		: QWidget(parent)
{		
	lastMode = DLFLWindow::NormalMode;
	((MainWindow*)parentWidget())->setRemeshingScheme(DLFLWindow::Dual);
	
	//mode group. this is the main widget that goes into a TabWidget
  remeshingGroup = new QGroupBox(tr("Remeshing"));
	//boxLayout widget to add buttons to within the groupBox widget
	remeshingBoxLayout = new QVBoxLayout;

	//Populate a Q Combo Box with a list of strings
	remeshingLabel = new QLabel(tr("Select Remeshing Scheme"));
	remeshingComboBox = new QComboBox();
	QStringList remeshingList;
	remeshingList << tr("Dual") << tr("3-Conversion Schemes") << tr("4-Conversion Schemes") << tr("5-Conversion Schemes") << tr("3-Preservation Schemes") 
								<< tr("4-Preservation Schemes") << tr("5-Preservation Schemes") << tr("6-Preservation Schemes") << tr("Miscellaneous Schemes");
	remeshingComboBox->addItems(remeshingList);
	//add the combobox to the layout
	remeshingBoxLayout->addWidget(remeshingLabel);
	remeshingBoxLayout->addWidget(remeshingComboBox);
	
	//create the stacked widget and all child widget pages
	dualWidget = new QWidget;
  threeConversionWidget = new QWidget;
  fourConversionWidget = new QWidget;
  fiveConversionWidget = new QWidget;
  threePreservationWidget = new QWidget;
  fourPreservationWidget = new QWidget;
  fivePreservationWidget = new QWidget;
  sixPreservationWidget = new QWidget;
  miscellaneousWidget = new QWidget;

  stackedWidget = new QStackedWidget;
  stackedWidget->addWidget(dualWidget);
  stackedWidget->addWidget(threeConversionWidget);
  stackedWidget->addWidget(fourConversionWidget);
  stackedWidget->addWidget(fiveConversionWidget);
  stackedWidget->addWidget(threePreservationWidget);
  stackedWidget->addWidget(fourPreservationWidget);
  stackedWidget->addWidget(fivePreservationWidget);
  stackedWidget->addWidget(sixPreservationWidget);
  stackedWidget->addWidget(miscellaneousWidget);

	//add the stackedwidget to the layout
  remeshingBoxLayout->addWidget(stackedWidget);

	//connect the combobox to the stacked widget
  connect(remeshingComboBox, SIGNAL(activated(int)),
          stackedWidget, SLOT(setCurrentIndex(int)));
	
	//setup stacked widget pages here, 
	setupDual();
	setupThreeConversion();
	setupFourConversion();
	setupFiveConversion();
	setupThreePreservation();
	setupFourPreservation();
	setupFivePreservation();			
	setupSixPreservation();
	setupMiscellaneous();
	
	//set the layout
	remeshingGroup->setLayout(remeshingBoxLayout);
	
	connect(remeshingComboBox, SIGNAL(currentIndexChanged(int)),
					this,SLOT(switchMode(int)) );
}

void RemeshingMode::insertTab(QTabWidget *tabWidget){
	tabWidget->addTab( remeshingGroup, QIcon(":/images/mode_remeshing.png"), "4");
  tabWidget->setTabToolTip(0,tr("Remeshing Modes"));
}

int RemeshingMode::getLastMode(){
	return lastMode;
}



void RemeshingMode::switchMode(int index){	
	switch(index){
		case 0: lastMode = DLFLWindow::NormalMode;//dual();
						((MainWindow*)parentWidget())->setRemeshingScheme(DLFLWindow::Dual);
		break;
		case 1:	lastMode = DLFLWindow::NormalMode;
						((MainWindow*)parentWidget())->setRemeshingScheme(lastThreeConversionScheme);
		break;	
		case 2: lastMode = DLFLWindow::NormalMode;
						((MainWindow*)parentWidget())->setRemeshingScheme(lastFourConversionScheme);
		break;
		case 3: lastMode = DLFLWindow::NormalMode;
						((MainWindow*)parentWidget())->setRemeshingScheme(lastFiveConversionScheme);
		break;
		case 4: lastMode = DLFLWindow::NormalMode;
						((MainWindow*)parentWidget())->setRemeshingScheme(lastThreePreservationScheme);
		break;
		case 5: lastMode = DLFLWindow::NormalMode;
						((MainWindow*)parentWidget())->setRemeshingScheme(lastFourPreservationScheme);
		break;
		case 6: lastMode = DLFLWindow::NormalMode;
						((MainWindow*)parentWidget())->setRemeshingScheme(lastFivePreservationScheme);
		break;
		case 7: lastMode = DLFLWindow::NormalMode;
						((MainWindow*)parentWidget())->setRemeshingScheme(lastSixPreservationScheme);
		break;
		case 8: lastMode = lastMiscellaneousMode;
						((MainWindow*)parentWidget())->setRemeshingScheme(lastMiscellaneousScheme);
		break;
		default: lastMode = DLFLWindow::NormalMode;
		break;
	}
	((MainWindow*)parentWidget())->setMode(lastMode);
	// ((MainWindow*)parentWidget())->setRemeshingScheme(lastMode);
}

void RemeshingMode::setupDual(){
	
	dualLayout = new QVBoxLayout;
	
	QCheckBox *dualFasterCheckBox = new QCheckBox(tr("Use Faster Method (Less Accurate)"));					
	connect(dualFasterCheckBox, SIGNAL(stateChanged(int)),
					((MainWindow*)parentWidget()),SLOT(toggleAccurateDualFlag(int)) );
	
	dualLayout->addWidget(dualFasterCheckBox);
	
	//create crust button
	QPushButton *dualCreateButton = new QPushButton(tr("Create Dual"), this);
	connect(dualCreateButton, SIGNAL(clicked()),
					((MainWindow*)parentWidget()),SLOT(perform_remeshing()) );

	dualLayout->addWidget(dualCreateButton);
	
	dualLayout->addStretch(1);
	dualWidget->setLayout(dualLayout);
}

void RemeshingMode::setupThreeConversion(){
	
	lastThreeConversionScheme = DLFLWindow::Root3;
	threeConversionLayout = new QVBoxLayout;
	
	QLabel *threeConversionLabel = new QLabel(tr("Select Remeshing Scheme"));
	QComboBox *threeConversionComboBox = new QComboBox();
	QStringList threeConversionList;
	threeConversionList << tr("Root 3") << tr("Dual of Vertex-Truncation") << tr("Stellation") << tr("Double Stellation (Star)") << tr("12.6.4") 
											<< tr("Honeycomb (dual of root 3)") << tr("Vertex-Truncation") << tr("Dual of 12.6.4") ;
	threeConversionComboBox->addItems(threeConversionList);
	//add the combobox to the layout
	threeConversionLayout->addWidget(threeConversionLabel);
	threeConversionLayout->addWidget(threeConversionComboBox);
	
	//create the stacked widget and all child widget pages
  QWidget *rootThreeWidget = new QWidget;
  QWidget *dualVertexTruncationWidget = new QWidget;
  QWidget *stellationWidget = new QWidget;
  QWidget *starWidget = new QWidget;
  QWidget *twelveSixFourWidget = new QWidget;
  QWidget *honeycombWidget = new QWidget;
  QWidget *vertexTruncationWidget = new QWidget;
  QWidget *dualTwelveSixFourWidget = new QWidget;

	//create the panels for each of these three conversion schemes
	//root 3
	QVBoxLayout *rootThreeLayout = new QVBoxLayout;
	QPushButton *rootThreeCreateButton = new QPushButton(tr("Perform Remeshing"), this);
	connect(rootThreeCreateButton, SIGNAL(clicked()),
					((MainWindow*)parentWidget()),SLOT(perform_remeshing()) );
	rootThreeLayout->addWidget(rootThreeCreateButton);
	rootThreeLayout->addStretch(1);
	rootThreeWidget->setLayout(rootThreeLayout);
	
	//dual vertex truncation
	QVBoxLayout *dualVertexTruncationLayout = new QVBoxLayout;
	QPushButton *dualVertexTruncationButton = new QPushButton(tr("Perform Remeshing"), this);
	connect(dualVertexTruncationButton, SIGNAL(clicked()),
					((MainWindow*)parentWidget()),SLOT(perform_remeshing()) );
	dualVertexTruncationLayout->addWidget(dualVertexTruncationButton);
	dualVertexTruncationLayout->addStretch(1);
	dualVertexTruncationWidget->setLayout(dualVertexTruncationLayout);
		
	//stellation
	QVBoxLayout *stellationLayout = new QVBoxLayout;
	QPushButton *stellationButton = new QPushButton(tr("Perform Remeshing"), this);
	connect(stellationButton, SIGNAL(clicked()),
					((MainWindow*)parentWidget()),SLOT(perform_remeshing()) );
	stellationLayout->addWidget(stellationButton);
	stellationLayout->addStretch(1);
	stellationWidget->setLayout(stellationLayout);

	//double stellate
	QVBoxLayout *starLayout = new QVBoxLayout;
	//scale factor - 0.00-10.00
	QLabel *starLabel = new QLabel(tr("Offset:"));
	QDoubleSpinBox *starSpinBox = new QDoubleSpinBox;
	starSpinBox->setRange(0.0, 1.0);
	starSpinBox->setSingleStep(0.01);
	starSpinBox->setValue(0.0);
	starSpinBox->setDecimals(2);
	connect(starSpinBox, SIGNAL(valueChanged(double)),
					((MainWindow*)parentWidget()),SLOT(changeStarOffset(double)) );	
	
	starLayout->addWidget(starLabel);
  starLayout->addWidget(starSpinBox);
	//create button
	QPushButton *starButton = new QPushButton(tr("Perform Remeshing"), this);
	connect(starButton, SIGNAL(clicked()),
					((MainWindow*)parentWidget()),SLOT(perform_remeshing()) );
	starLayout->addWidget(starButton);
	starLayout->addStretch(1);
	starWidget->setLayout(starLayout);
	
	//12.6.4
	QVBoxLayout *twelveSixFourLayout = new QVBoxLayout;
	//scale factor - 0.00-10.00
	QLabel *twelveSixFourLabel = new QLabel(tr("Offset:"));
	QDoubleSpinBox *twelveSixFourSpinBox = new QDoubleSpinBox;
	twelveSixFourSpinBox->setRange(0.0, 1.0);
	twelveSixFourSpinBox->setSingleStep(0.01);
	twelveSixFourSpinBox->setValue(0.7);
	twelveSixFourSpinBox->setDecimals(2);
	connect(twelveSixFourSpinBox, SIGNAL(valueChanged(double)),
					((MainWindow*)parentWidget()),SLOT(changeSubStellateCurve(double)) );	
	
	twelveSixFourLayout->addWidget(twelveSixFourLabel);
  twelveSixFourLayout->addWidget(twelveSixFourSpinBox);
	//create button
	QPushButton *twelveSixFourButton = new QPushButton(tr("Perform Remeshing"), this);
	connect(twelveSixFourButton, SIGNAL(clicked()),
					((MainWindow*)parentWidget()),SLOT(perform_remeshing()) );
	twelveSixFourLayout->addWidget(twelveSixFourButton);
	twelveSixFourLayout->addStretch(1);
	twelveSixFourWidget->setLayout(twelveSixFourLayout);
	
	//honeycomb
	QVBoxLayout *honeycombLayout = new QVBoxLayout;
	QPushButton *honeycombButton = new QPushButton(tr("Perform Remeshing"), this);
	connect(honeycombButton, SIGNAL(clicked()),
					((MainWindow*)parentWidget()),SLOT(perform_remeshing()) );
	honeycombLayout->addWidget(honeycombButton);
	honeycombLayout->addStretch(1);
	honeycombWidget->setLayout(honeycombLayout);
	
	//vertexTruncation
	QVBoxLayout *vertexTruncationLayout = new QVBoxLayout;
	//scale factor
	QLabel *vertexTruncationLabel = new QLabel(tr("Offset:"));
	QDoubleSpinBox *vertexTruncationSpinBox = new QDoubleSpinBox;
	vertexTruncationSpinBox->setRange(0.0, 0.5);
	vertexTruncationSpinBox->setSingleStep(0.01);
	vertexTruncationSpinBox->setValue(0.25);
	vertexTruncationSpinBox->setDecimals(2);
	connect(vertexTruncationSpinBox, SIGNAL(valueChanged(double)),
					((MainWindow*)parentWidget()),SLOT(changeVertexCuttingOffset(double)) );	
	
	vertexTruncationLayout->addWidget(vertexTruncationLabel);
  vertexTruncationLayout->addWidget(vertexTruncationSpinBox);
	QPushButton *vertexTruncationButton = new QPushButton(tr("Perform Remeshing"), this);
	connect(vertexTruncationButton, SIGNAL(clicked()),
					((MainWindow*)parentWidget()),SLOT(perform_remeshing()) );
	vertexTruncationLayout->addWidget(vertexTruncationButton);
	vertexTruncationLayout->addStretch(1);
	vertexTruncationWidget->setLayout(vertexTruncationLayout);
		
	//dual 12.6.4
	QVBoxLayout *dualTwelveSixFourLayout = new QVBoxLayout;
	//scale factor
	QLabel *dualTwelveSixFourLabel = new QLabel(tr("Scale:"));
	QDoubleSpinBox *dualTwelveSixFourSpinBox = new QDoubleSpinBox;
	dualTwelveSixFourSpinBox->setRange(0.0, 1.0);
	dualTwelveSixFourSpinBox->setSingleStep(0.01);
	dualTwelveSixFourSpinBox->setValue(0.06);
	dualTwelveSixFourSpinBox->setDecimals(2);
	connect(dualTwelveSixFourSpinBox, SIGNAL(valueChanged(double)),
					((MainWindow*)parentWidget()),SLOT(changeDual1264Scale(double)) );	
	
	dualTwelveSixFourLayout->addWidget(dualTwelveSixFourLabel);
  dualTwelveSixFourLayout->addWidget(dualTwelveSixFourSpinBox);
	//create button
	QPushButton *dualTwelveSixFourButton = new QPushButton(tr("Perform Remeshing"), this);
	connect(dualTwelveSixFourButton, SIGNAL(clicked()),
					((MainWindow*)parentWidget()),SLOT(perform_remeshing()) );
					
	dualTwelveSixFourLayout->addWidget(dualTwelveSixFourButton);
	dualTwelveSixFourLayout->addStretch(1);
	dualTwelveSixFourWidget->setLayout(dualTwelveSixFourLayout);
	
	//add the widgets to a stack controlled by the qcombobox
  QStackedWidget *threeConversionStackedWidget = new QStackedWidget;
  threeConversionStackedWidget->addWidget(rootThreeWidget);
  threeConversionStackedWidget->addWidget(dualVertexTruncationWidget);
  threeConversionStackedWidget->addWidget(stellationWidget);
  threeConversionStackedWidget->addWidget(starWidget);
  threeConversionStackedWidget->addWidget(twelveSixFourWidget);
  threeConversionStackedWidget->addWidget(honeycombWidget);
  threeConversionStackedWidget->addWidget(vertexTruncationWidget);
  threeConversionStackedWidget->addWidget(dualTwelveSixFourWidget);
	//add the stackedwidget to the layout
  threeConversionLayout->addWidget(threeConversionStackedWidget);
	//connect the combobox to the stacked widget
  connect(threeConversionComboBox, SIGNAL(activated(int)),
          threeConversionStackedWidget, SLOT(setCurrentIndex(int)));	

	threeConversionLayout->addStretch(1);
	threeConversionWidget->setLayout(threeConversionLayout);
	
	//connect the combobox signal to the mode switching slot
	connect(threeConversionComboBox, SIGNAL(currentIndexChanged(int)),
					this,SLOT(switchThreeConversionMode(int)) );
}

void RemeshingMode::switchThreeConversionMode(int index){	
	
	switch(index){
		case 0: lastThreeConversionScheme = DLFLWindow::Root3;//root3_subdivision();
		break;
		case 1:	lastThreeConversionScheme = DLFLWindow::DualVertexTrunc;//sqrt3_subdivision();
		break;	
		case 2: lastThreeConversionScheme = DLFLWindow::GlobalStellate;//global_stellation();
		break;
		case 3: lastThreeConversionScheme = DLFLWindow::Star;//star_subdivision();
		break;
		case 4: lastThreeConversionScheme = DLFLWindow::Generic1264;//generic1264_subdivision();
		break;
		case 5: lastThreeConversionScheme = DLFLWindow::Honeycomb;//honeycomb_subdivision();
		break;
		case 6: lastThreeConversionScheme = DLFLWindow::VertexTrunc;//vertex_cutting_subdivision();
		break;
		case 7: lastThreeConversionScheme = DLFLWindow::DualGeneric1264;//dual_generic1264_subdivision();
		break;
		default:
		break;
	}
	((MainWindow*)parentWidget())->setRemeshingScheme(lastThreeConversionScheme);
}

void RemeshingMode::setupFourConversion(){
	
	lastFourConversionScheme = DLFLWindow::LinearVertexInsertion;
	fourConversionLayout = new QVBoxLayout;
	
	fourConversionLabel = new QLabel(tr("Select Remeshing Scheme"));
	fourConversionComboBox = new QComboBox();
	QStringList fourConversionList;
	fourConversionList << tr("Linear Vertex Insertion") << tr("Catmull-Clark") << tr("Stellate with edge removal") << tr("Doo-Sabin") << tr("Corner-Cutting") 
											<< tr("Simplest");
											
	fourConversionComboBox->addItems(fourConversionList);
	//add the combobox to the layout
	fourConversionLayout->addWidget(fourConversionLabel);
	fourConversionLayout->addWidget(fourConversionComboBox);
	
	//create the stacked widget and all child widget pages
  QWidget *linearVertexWidget = new QWidget;
  QWidget *catmullClarkWidget = new QWidget;
  QWidget *stellateEdgeRemovalWidget = new QWidget;
  QWidget *dooSabinWidget = new QWidget;
  QWidget *cornerCuttingWidget = new QWidget;
  QWidget *simplestWidget = new QWidget;

	//create the panels for each of these four conversion schemes

	//linear vertex insertion
	QVBoxLayout *linearVertexLayout = new QVBoxLayout;
	QPushButton *linearVertexCreateButton = new QPushButton(tr("Perform Remeshing"), this);
	connect(linearVertexCreateButton, SIGNAL(clicked()),
					((MainWindow*)parentWidget()),SLOT(perform_remeshing()) );
					
	linearVertexLayout->addWidget(linearVertexCreateButton);
	linearVertexLayout->addStretch(1);
	linearVertexWidget->setLayout(linearVertexLayout);

	//catmull clark
	QVBoxLayout *catmullClarkLayout = new QVBoxLayout;
	QPushButton *catmullClarkCreateButton = new QPushButton(tr("Perform Remeshing"), this);
	connect(catmullClarkCreateButton, SIGNAL(clicked()),
					((MainWindow*)parentWidget()),SLOT(perform_remeshing()) );
					
	catmullClarkLayout->addWidget(catmullClarkCreateButton);
	catmullClarkLayout->addStretch(1);
	catmullClarkWidget->setLayout(catmullClarkLayout);
	
	//stellate with edge removal
	QVBoxLayout *stellateEdgeRemovalLayout = new QVBoxLayout;
	QPushButton *stellateEdgeRemovalCreateButton = new QPushButton(tr("Perform Remeshing"), this);
	connect(stellateEdgeRemovalCreateButton, SIGNAL(clicked()),
					((MainWindow*)parentWidget()),SLOT(perform_remeshing()) );
					
	stellateEdgeRemovalLayout->addWidget(stellateEdgeRemovalCreateButton);
	stellateEdgeRemovalLayout->addStretch(1);
	stellateEdgeRemovalWidget->setLayout(stellateEdgeRemovalLayout);
	
	//doo sabin
	QVBoxLayout *dooSabinLayout = new QVBoxLayout;
	QCheckBox *dooSabinCheckBox = new QCheckBox(tr("Check for multiple edges"));
	dooSabinLayout->addWidget(dooSabinCheckBox);
	//connect the checkbox
	connect(dooSabinCheckBox, SIGNAL(stateChanged(int)),
					((MainWindow*)parentWidget()),SLOT(toggleDooSabinEdgeFlag(int)) );
	QPushButton *dooSabinCreateButton = new QPushButton(tr("Perform Remeshing"), this);
	connect(dooSabinCreateButton, SIGNAL(clicked()),
					((MainWindow*)parentWidget()),SLOT(perform_remeshing()) );
	dooSabinLayout->addWidget(dooSabinCreateButton);
	dooSabinLayout->addStretch(1);
	dooSabinWidget->setLayout(dooSabinLayout);
	
	//corner cutting
	QVBoxLayout *cornerCuttingLayout = new QVBoxLayout;
	QPushButton *cornerCuttingCreateButton = new QPushButton(tr("Perform Remeshing"), this);
	connect(cornerCuttingCreateButton, SIGNAL(clicked()),
					((MainWindow*)parentWidget()),SLOT(perform_remeshing()) );
	cornerCuttingLayout->addWidget(cornerCuttingCreateButton);
	cornerCuttingLayout->addStretch(1);
	cornerCuttingWidget->setLayout(cornerCuttingLayout);
	
	//simplest
	QVBoxLayout *simplestLayout = new QVBoxLayout;
	QPushButton *simplestCreateButton = new QPushButton(tr("Perform Remeshing"), this);
	connect(simplestCreateButton, SIGNAL(clicked()),
					((MainWindow*)parentWidget()),SLOT(perform_remeshing()) );
	simplestLayout->addWidget(simplestCreateButton);
	simplestLayout->addStretch(1);
	simplestWidget->setLayout(simplestLayout);	
	
	//add the widgets to a stack controlled by the qcombobox
  QStackedWidget *fourConversionStackedWidget = new QStackedWidget;
	fourConversionStackedWidget->addWidget(linearVertexWidget);
  fourConversionStackedWidget->addWidget(catmullClarkWidget);
  fourConversionStackedWidget->addWidget(stellateEdgeRemovalWidget);
  fourConversionStackedWidget->addWidget(dooSabinWidget);
  fourConversionStackedWidget->addWidget(cornerCuttingWidget);
  fourConversionStackedWidget->addWidget(simplestWidget);
	//add the stackedwidget to the layout
  fourConversionLayout->addWidget(fourConversionStackedWidget);
	//connect the combobox to the stacked widget
  connect(fourConversionComboBox, SIGNAL(activated(int)),
          fourConversionStackedWidget, SLOT(setCurrentIndex(int)));	
	
	fourConversionLayout->addStretch(1);
	fourConversionWidget->setLayout(fourConversionLayout);	
	
	//connect the combobox signal to the mode switching slot
	connect(fourConversionComboBox, SIGNAL(currentIndexChanged(int)),
					this,SLOT(switchFourConversionMode(int)) );
}

void RemeshingMode::switchFourConversionMode(int index){	
	
	switch(index){
		case 0: lastFourConversionScheme = DLFLWindow::LinearVertexInsertion;
		break;
		case 1:	lastFourConversionScheme = DLFLWindow::CatmullClark;
		break;	
		case 2: lastFourConversionScheme = DLFLWindow::ModifiedStellate;
		break;
		case 3: lastFourConversionScheme = DLFLWindow::DooSabin;
		break;
		case 4: lastFourConversionScheme = DLFLWindow::CornerCutting;
		break;
		case 5: lastFourConversionScheme = DLFLWindow::Simplest;
		break;
		default:
		break;
	}
	((MainWindow*)parentWidget())->setRemeshingScheme(lastFourConversionScheme);
}

void RemeshingMode::setupFiveConversion(){
	
	lastFiveConversionScheme = DLFLWindow::Pentagonal;
	fiveConversionLayout = new QVBoxLayout;
	
	QLabel *fiveConversionLabel = new QLabel(tr("Select Remeshing Scheme"));
	QComboBox *fiveConversionComboBox = new QComboBox();
	QStringList fiveConversionList;
	fiveConversionList << tr("Pentagonalization") << tr("Cubic Pentagonalization") << tr("Dual-Pentagonalization");
	
	fiveConversionComboBox->addItems(fiveConversionList);
	//add the combobox to the layout
	fiveConversionLayout->addWidget(fiveConversionLabel);
	fiveConversionLayout->addWidget(fiveConversionComboBox);
	
	//create the stacked widget and all child widget pages
  QWidget *pentagonalizationWidget = new QWidget;
  QWidget *cubicPentagonalizationWidget = new QWidget;
  QWidget *dualPentagonalizationWidget = new QWidget;

	//create the panels for each of these five conversion schemes

	//pentagonalization
	QVBoxLayout *pentagonalizationLayout = new QVBoxLayout;
	//offset spinbox
	QLabel *pentagonalizationLabel = new QLabel(tr("Offset:"));
	QDoubleSpinBox *pentagonalizationSpinBox = new QDoubleSpinBox;
	pentagonalizationSpinBox->setRange(0.0, 1.0);
	pentagonalizationSpinBox->setSingleStep(0.01);
	pentagonalizationSpinBox->setValue(0.0);
	pentagonalizationSpinBox->setDecimals(2);
	connect(pentagonalizationSpinBox, SIGNAL(valueChanged(double)),
					((MainWindow*)parentWidget()),SLOT(changePentagonalOffset(double)) );
	
	pentagonalizationLayout->addWidget(pentagonalizationLabel);
  pentagonalizationLayout->addWidget(pentagonalizationSpinBox);
	QPushButton *pentagonalizationCreateButton = new QPushButton(tr("Perform Remeshing"), this);
	connect(pentagonalizationCreateButton, SIGNAL(clicked()),
					((MainWindow*)parentWidget()),SLOT(perform_remeshing()) );
					
	pentagonalizationLayout->addWidget(pentagonalizationCreateButton);
	pentagonalizationLayout->addStretch(1);
	pentagonalizationWidget->setLayout(pentagonalizationLayout);

	//cubic Pentagonalization
	QVBoxLayout *cubicPentagonalizationLayout = new QVBoxLayout;
	//offset spinbox
	QLabel *cubicPentagonalizationLabel = new QLabel(tr("Offset:"));
	QDoubleSpinBox *cubicPentagonalizationSpinBox = new QDoubleSpinBox;
	cubicPentagonalizationSpinBox->setRange(0.0, 1.0);
	cubicPentagonalizationSpinBox->setSingleStep(0.01);
	cubicPentagonalizationSpinBox->setValue(0.0);
	cubicPentagonalizationSpinBox->setDecimals(2);
	connect(cubicPentagonalizationSpinBox, SIGNAL(valueChanged(double)),
					((MainWindow*)parentWidget()),SLOT(changePentagonalOffset(double)) );
					
	cubicPentagonalizationLayout->addWidget(cubicPentagonalizationLabel);
  cubicPentagonalizationLayout->addWidget(cubicPentagonalizationSpinBox);
	QPushButton *cubicPentagonalizationCreateButton = new QPushButton(tr("Perform Remeshing"), this);
	connect(cubicPentagonalizationCreateButton, SIGNAL(clicked()),
					((MainWindow*)parentWidget()),SLOT(perform_remeshing()) );
					
	cubicPentagonalizationLayout->addWidget(cubicPentagonalizationCreateButton);
	cubicPentagonalizationLayout->addStretch(1);
	cubicPentagonalizationWidget->setLayout(cubicPentagonalizationLayout);
	
	//dual pentagonalization
	QVBoxLayout *dualPentagonalizationLayout = new QVBoxLayout;
	QPushButton *dualPentagonalizationCreateButton = new QPushButton(tr("Perform Remeshing"), this);
	connect(dualPentagonalizationCreateButton, SIGNAL(clicked()),
					((MainWindow*)parentWidget()),SLOT(perform_remeshing()) );
	dualPentagonalizationLayout->addWidget(dualPentagonalizationCreateButton);
	dualPentagonalizationLayout->addStretch(1);
	dualPentagonalizationWidget->setLayout(dualPentagonalizationLayout);
	
	//add the widgets to a stack controlled by the qcombobox
  QStackedWidget *fiveConversionStackedWidget = new QStackedWidget;
	fiveConversionStackedWidget->addWidget(pentagonalizationWidget);
  fiveConversionStackedWidget->addWidget(cubicPentagonalizationWidget);
  fiveConversionStackedWidget->addWidget(dualPentagonalizationWidget);

	//add the stackedwidget to the layout
  fiveConversionLayout->addWidget(fiveConversionStackedWidget);
	//connect the combobox to the stacked widget
  connect(fiveConversionComboBox, SIGNAL(activated(int)),
          fiveConversionStackedWidget, SLOT(setCurrentIndex(int)));
	
	fiveConversionLayout->addStretch(1);
	fiveConversionWidget->setLayout(fiveConversionLayout);	
	
	//connect the combobox signal to the mode switching slot
	connect(fiveConversionComboBox, SIGNAL(currentIndexChanged(int)),
					this,SLOT(switchFiveConversionMode(int)) );
}

void RemeshingMode::switchFiveConversionMode(int index){	
	
	switch(index){
		case 0: lastFiveConversionScheme = DLFLWindow::Pentagonal;
		break;
		case 1:	lastFiveConversionScheme = DLFLWindow::CubicPentagonal;
		break;	
		case 2: lastFiveConversionScheme = DLFLWindow::DualPentagonal;
		break;
		default:
		break;
	}
	((MainWindow*)parentWidget())->setRemeshingScheme(lastFiveConversionScheme);
}

void RemeshingMode::setupThreePreservation(){
	
	lastThreePreservationScheme = DLFLWindow::LoopStyle;
	threePreservationLayout = new QVBoxLayout;
	
	QLabel *threePreservationLabel = new QLabel(tr("Select Remeshing Scheme"));
	QComboBox *threePreservationComboBox = new QComboBox();
	QStringList threePreservationList;
	threePreservationList << tr("Loop-Style Remeshing") << tr("Loop Subdivision") << tr("Dual of Loop-Style Remeshing") << tr("Dual of Loop Subdivision");
	
	threePreservationComboBox->addItems(threePreservationList);
	//add the combobox to the layout
	threePreservationLayout->addWidget(threePreservationLabel);
	threePreservationLayout->addWidget(threePreservationComboBox);
	
	//create the stacked widget and all child widget pages
  QWidget *loopStyleRemeshingWidget = new QWidget;
  QWidget *loopSubdivisionWidget = new QWidget;
  QWidget *dualLoopStyleRemeshingWidget = new QWidget;
  QWidget *dualLoopSubdivisionWidget = new QWidget;

	//loop-style remeshing
	QVBoxLayout *loopStyleRemeshingLayout = new QVBoxLayout;
	//offset spinbox
	QLabel *loopStyleRemeshingLabel = new QLabel(tr("Length:"));
	QDoubleSpinBox *loopStyleRemeshingSpinBox = new QDoubleSpinBox;
	loopStyleRemeshingSpinBox->setRange(0.0, 1.0);
	loopStyleRemeshingSpinBox->setSingleStep(0.01);
	loopStyleRemeshingSpinBox->setValue(1.0);
	loopStyleRemeshingSpinBox->setDecimals(2);
	connect(loopStyleRemeshingSpinBox, SIGNAL(valueChanged(double)),
					((MainWindow*)parentWidget()),SLOT(changeLoopStyleSubdivisionLength(double)) );
	
	loopStyleRemeshingLayout->addWidget(loopStyleRemeshingLabel);
  loopStyleRemeshingLayout->addWidget(loopStyleRemeshingSpinBox);
	QPushButton *loopStyleRemeshingButton = new QPushButton(tr("Perform Remeshing"), this);
	connect(loopStyleRemeshingButton, SIGNAL(clicked()),
					((MainWindow*)parentWidget()),SLOT(perform_remeshing()) );
	
	loopStyleRemeshingLayout->addWidget(loopStyleRemeshingButton);
	loopStyleRemeshingLayout->addStretch(1);
	loopStyleRemeshingWidget->setLayout(loopStyleRemeshingLayout);

	//loop subdivision
	QVBoxLayout *loopSubdivisionLayout = new QVBoxLayout;
	QPushButton *loopSubdivisionCreateButton = new QPushButton(tr("Perform Remeshing"), this);
	connect(loopSubdivisionCreateButton, SIGNAL(clicked()),
					((MainWindow*)parentWidget()),SLOT(perform_remeshing()) );
	loopSubdivisionLayout->addWidget(loopSubdivisionCreateButton);
	loopSubdivisionLayout->addStretch(1);
	loopSubdivisionWidget->setLayout(loopSubdivisionLayout);
	
	//dual of loop-style remeshing
	QVBoxLayout *dualLoopStyleRemeshingLayout = new QVBoxLayout;
	
	QLabel *dualLoopStyleRemeshingTwistLabel = new QLabel(tr("Twist:"));
	QDoubleSpinBox *dualLoopStyleRemeshingTwistSpinBox = new QDoubleSpinBox;
	dualLoopStyleRemeshingTwistSpinBox->setRange(0.0, 1.0);
	dualLoopStyleRemeshingTwistSpinBox->setSingleStep(0.01);
	dualLoopStyleRemeshingTwistSpinBox->setValue(0.0);
	dualLoopStyleRemeshingTwistSpinBox->setDecimals(2);
	connect(dualLoopStyleRemeshingTwistSpinBox, SIGNAL(valueChanged(double)),
					((MainWindow*)parentWidget()),SLOT(changeRoot4Twist(double)) );	
	
	dualLoopStyleRemeshingLayout->addWidget(dualLoopStyleRemeshingTwistLabel);
  dualLoopStyleRemeshingLayout->addWidget(dualLoopStyleRemeshingTwistSpinBox);

	QLabel *dualLoopStyleRemeshingWeightLabel = new QLabel(tr("Weight:"));
	QDoubleSpinBox *dualLoopStyleRemeshingWeightSpinBox = new QDoubleSpinBox;
	dualLoopStyleRemeshingWeightSpinBox->setRange(0.0, 1.0);
	dualLoopStyleRemeshingWeightSpinBox->setSingleStep(0.01);
	dualLoopStyleRemeshingWeightSpinBox->setValue(0.0);
	dualLoopStyleRemeshingWeightSpinBox->setDecimals(2);
	connect(dualLoopStyleRemeshingWeightSpinBox, SIGNAL(valueChanged(double)),
					((MainWindow*)parentWidget()),SLOT(changeRoot4Weight(double)) );	
	
	dualLoopStyleRemeshingLayout->addWidget(dualLoopStyleRemeshingWeightLabel);
  dualLoopStyleRemeshingLayout->addWidget(dualLoopStyleRemeshingWeightSpinBox);

	QPushButton *dualLoopStyleRemeshingButton = new QPushButton(tr("Perform Remeshing"), this);
	connect(dualLoopStyleRemeshingButton, SIGNAL(clicked()),
					((MainWindow*)parentWidget()),SLOT(perform_remeshing()) );
					
	dualLoopStyleRemeshingLayout->addWidget(dualLoopStyleRemeshingButton);
	dualLoopStyleRemeshingLayout->addStretch(1);
	dualLoopStyleRemeshingWidget->setLayout(dualLoopStyleRemeshingLayout);
	
	//dual of loop subdivision
	QVBoxLayout *dualLoopSubdivisionLayout = new QVBoxLayout;
	QPushButton *dualLoopSubdivisionCreateButton = new QPushButton(tr("Perform Remeshing"), this);
	connect(dualLoopSubdivisionCreateButton, SIGNAL(clicked()),
					((MainWindow*)parentWidget()),SLOT(perform_remeshing()) );
					
	dualLoopSubdivisionLayout->addWidget(dualLoopSubdivisionCreateButton);
	dualLoopSubdivisionLayout->addStretch(1);
	dualLoopSubdivisionWidget->setLayout(dualLoopSubdivisionLayout);
	
	//add the widgets to a stack controlled by the qcombobox
  QStackedWidget *threePreservationStackedWidget = new QStackedWidget;
	threePreservationStackedWidget->addWidget(loopStyleRemeshingWidget);
	threePreservationStackedWidget->addWidget(loopSubdivisionWidget);
  threePreservationStackedWidget->addWidget(dualLoopStyleRemeshingWidget);
	threePreservationStackedWidget->addWidget(dualLoopSubdivisionWidget);

	//add the stackedwidget to the layout
  threePreservationLayout->addWidget(threePreservationStackedWidget);
	//connect the combobox to the stacked widget
  connect(threePreservationComboBox, SIGNAL(activated(int)),
          threePreservationStackedWidget, SLOT(setCurrentIndex(int)));	

	threePreservationLayout->addStretch(1);
	threePreservationWidget->setLayout(threePreservationLayout);
	
	//connect the combobox signal to the mode switching slot
	connect(threePreservationComboBox, SIGNAL(currentIndexChanged(int)),
					this,SLOT(switchThreePreservationMode(int)) );
}

void RemeshingMode::switchThreePreservationMode(int index){	
	
	switch(index){
		case 0: lastThreePreservationScheme = DLFLWindow::LoopStyle;
		break;
		case 1:	lastThreePreservationScheme = DLFLWindow::Loop;
		break;	
		case 2: lastThreePreservationScheme = DLFLWindow::Root4;
		break;
		case 3: lastThreePreservationScheme = DLFLWindow::DualLoop;
		break;
		default:
		break;
	}
	((MainWindow*)parentWidget())->setRemeshingScheme(lastThreePreservationScheme);
}

void RemeshingMode::setupFourPreservation(){
	
	lastFourPreservationScheme = DLFLWindow::GlobalCubicExtrude;
	fourPreservationLayout = new QVBoxLayout;
	
	QLabel *fourPreservationLabel = new QLabel(tr("Select Remeshing Scheme"));
	QComboBox *fourPreservationComboBox = new QComboBox();
	QStringList fourPreservationList;
	fourPreservationList << tr("Global Extrude") << tr("Checkerboard Remeshing") << tr("Dual of Global Extrude") << tr("Dual of Checkerboard Remeshing");
	
	fourPreservationComboBox->addItems(fourPreservationList);
	//add the combobox to the layout
	fourPreservationLayout->addWidget(fourPreservationLabel);
	fourPreservationLayout->addWidget(fourPreservationComboBox);
	
	//create the stacked widget and all child widget pages
  QWidget *globalExtrudeWidget = new QWidget;
  QWidget *checkerboardWidget = new QWidget;
  QWidget *dualGlobalExtrudeWidget = new QWidget;
  QWidget *dualCheckerboardWidget = new QWidget;

	//global extrude
	QVBoxLayout *globalExtrudeLayout = new QVBoxLayout;
	QPushButton *globalExtrudeButton = new QPushButton(tr("Perform Remeshing"), this);
	connect(globalExtrudeButton, SIGNAL(clicked()),
					((MainWindow*)parentWidget()),SLOT(perform_remeshing()) );
					
	globalExtrudeLayout->addWidget(globalExtrudeButton);
	globalExtrudeLayout->addStretch(1);
	globalExtrudeWidget->setLayout(globalExtrudeLayout);

	//checkerboard
	QVBoxLayout *checkerboardLayout = new QVBoxLayout;
	QPushButton *checkerboardCreateButton = new QPushButton(tr("Perform Remeshing"), this);
	connect(checkerboardCreateButton, SIGNAL(clicked()),
					((MainWindow*)parentWidget()),SLOT(perform_remeshing()) );
					
	checkerboardLayout->addWidget(checkerboardCreateButton);
	checkerboardLayout->addStretch(1);
	checkerboardWidget->setLayout(checkerboardLayout);
	
	//dual of global extrude
	QVBoxLayout *dualGlobalExtrudeLayout = new QVBoxLayout;
	QPushButton *dualGlobalExtrudeButton = new QPushButton(tr("Perform Remeshing"), this);
	connect(dualGlobalExtrudeButton, SIGNAL(clicked()),
					((MainWindow*)parentWidget()),SLOT(perform_remeshing()) );
					
	dualGlobalExtrudeLayout->addWidget(dualGlobalExtrudeButton);
	dualGlobalExtrudeLayout->addStretch(1);
	dualGlobalExtrudeWidget->setLayout(dualGlobalExtrudeLayout);
	
	//dual of checkerboard remeshing
	QVBoxLayout *dualCheckerboardLayout = new QVBoxLayout;
	QPushButton *dualCheckerboardCreateButton = new QPushButton(tr("Perform Remeshing"), this);
	connect(dualCheckerboardCreateButton, SIGNAL(clicked()),
					((MainWindow*)parentWidget()),SLOT(perform_remeshing()) );
					
	dualCheckerboardLayout->addWidget(dualCheckerboardCreateButton);
	dualCheckerboardLayout->addStretch(1);
	dualCheckerboardWidget->setLayout(dualCheckerboardLayout);
	
	//add the widgets to a stack controlled by the qcombobox
  QStackedWidget *fourPreservationStackedWidget = new QStackedWidget;
	fourPreservationStackedWidget->addWidget(globalExtrudeWidget);
	fourPreservationStackedWidget->addWidget(checkerboardWidget);
  fourPreservationStackedWidget->addWidget(dualGlobalExtrudeWidget);
	fourPreservationStackedWidget->addWidget(dualCheckerboardWidget);

	//add the stackedwidget to the layout
  fourPreservationLayout->addWidget(fourPreservationStackedWidget);
	//connect the combobox to the stacked widget
  connect(fourPreservationComboBox, SIGNAL(activated(int)),
          fourPreservationStackedWidget, SLOT(setCurrentIndex(int)));
	
	fourPreservationLayout->addStretch(1);
	fourPreservationWidget->setLayout(fourPreservationLayout);
	
	//connect the combobox signal to the mode switching slot
	connect(fourPreservationComboBox, SIGNAL(currentIndexChanged(int)),
					this,SLOT(switchFourPreservationMode(int)) );
}

void RemeshingMode::switchFourPreservationMode(int index){	
	
	switch(index){
		case 0: lastFourPreservationScheme = DLFLWindow::GlobalCubicExtrude;
		break;
		case 1:	lastFourPreservationScheme = DLFLWindow::CheckerBoard;
		break;	
		case 2: lastFourPreservationScheme = DLFLWindow::DualGlobalCubicExtrude;
		break;
		case 3: lastFourPreservationScheme = DLFLWindow::DualCheckerBoard;
		break;
		default:
		break;
	}
	((MainWindow*)parentWidget())->setRemeshingScheme(lastFourPreservationScheme);
}

void RemeshingMode::setupFivePreservation(){
	
	lastFivePreservationScheme = DLFLWindow::PentagonPreserving;
	fivePreservationLayout = new QVBoxLayout;
	
	QLabel *fivePreservationLabel = new QLabel(tr("Select Remeshing Scheme"));
	QComboBox *fivePreservationComboBox = new QComboBox();
	QStringList fivePreservationList;
	fivePreservationList << tr("Pentagon Preserving") << tr("Dual of Pentagon Preserving");
	
	fivePreservationComboBox->addItems(fivePreservationList);
	//add the combobox to the layout
	fivePreservationLayout->addWidget(fivePreservationLabel);
	fivePreservationLayout->addWidget(fivePreservationComboBox);
	
	//create the stacked widget and all child widget pages
  QWidget *pentagonPreservingWidget = new QWidget;
  QWidget *dualPentagonPreservingWidget = new QWidget;

	//Pentagon Preserving
	QVBoxLayout *pentagonPreservingLayout = new QVBoxLayout;
	QLabel *pentagonPreservingLabel = new QLabel(tr("Scale Factor:"));
	QDoubleSpinBox *pentagonPreservingSpinBox = new QDoubleSpinBox;
	pentagonPreservingSpinBox->setRange(0.0, 1.0);
	pentagonPreservingSpinBox->setSingleStep(0.01);
	pentagonPreservingSpinBox->setValue(0.75);
	pentagonPreservingSpinBox->setDecimals(2);
	connect(pentagonPreservingSpinBox, SIGNAL(valueChanged(double)),
					((MainWindow*)parentWidget()),SLOT(changePentagonalScaleFactor(double)) );	
	
	pentagonPreservingLayout->addWidget(pentagonPreservingLabel);
  pentagonPreservingLayout->addWidget(pentagonPreservingSpinBox);

	QPushButton *pentagonPreservingButton = new QPushButton(tr("Perform Remeshing"), this);
	connect(pentagonPreservingButton, SIGNAL(clicked()),
					((MainWindow*)parentWidget()),SLOT(perform_remeshing()) );
					
	pentagonPreservingLayout->addWidget(pentagonPreservingButton);
	pentagonPreservingLayout->addStretch(1);
	pentagonPreservingWidget->setLayout(pentagonPreservingLayout);

	//dual of Pentagon Preserving
	QVBoxLayout *dualPentagonPreservingLayout = new QVBoxLayout;
	QPushButton *dualPentagonPreservingCreateButton = new QPushButton(tr("Perform Remeshing"), this);
	connect(dualPentagonPreservingCreateButton, SIGNAL(clicked()),
					((MainWindow*)parentWidget()),SLOT(perform_remeshing()) );
					
	dualPentagonPreservingLayout->addWidget(dualPentagonPreservingCreateButton);
	dualPentagonPreservingLayout->addStretch(1);
	dualPentagonPreservingWidget->setLayout(dualPentagonPreservingLayout);
	
	//add the widgets to a stack controlled by the qcombobox
  QStackedWidget *fivePreservationStackedWidget = new QStackedWidget;
	fivePreservationStackedWidget->addWidget(pentagonPreservingWidget);
	fivePreservationStackedWidget->addWidget(dualPentagonPreservingWidget);

	//add the stackedwidget to the layout
  fivePreservationLayout->addWidget(fivePreservationStackedWidget);
	//connect the combobox to the stacked widget
  connect(fivePreservationComboBox, SIGNAL(activated(int)),
          fivePreservationStackedWidget, SLOT(setCurrentIndex(int)));
	
	fivePreservationLayout->addStretch(1);
	fivePreservationWidget->setLayout(fivePreservationLayout);
	
	//connect the combobox signal to the mode switching slot
	connect(fivePreservationComboBox, SIGNAL(currentIndexChanged(int)),
					this,SLOT(switchFivePreservationMode(int)) );
}

void RemeshingMode::switchFivePreservationMode(int index){	
	
	switch(index){
		case 0: lastFivePreservationScheme = DLFLWindow::PentagonPreserving;
		break;
		case 1:	lastFivePreservationScheme = DLFLWindow::DualPentagonPreserving;
		break;	
		default:
		break;
	}
	((MainWindow*)parentWidget())->setRemeshingScheme(lastFivePreservationScheme);
}

void RemeshingMode::setupSixPreservation(){
	
	lastSixPreservationScheme = DLFLWindow::HexagonPreserving;
	sixPreservationLayout = new QVBoxLayout;
	
	QLabel *sixPreservationLabel = new QLabel(tr("Select Remeshing Scheme"));
	QComboBox *sixPreservationComboBox = new QComboBox();
	QStringList sixPreservationList;
	sixPreservationList << tr("Dual of Loop-Style Remeshing") << tr("Loop-Style Remeshing");
	
	sixPreservationComboBox->addItems(sixPreservationList);
	//add the combobox to the layout
	sixPreservationLayout->addWidget(sixPreservationLabel);
	sixPreservationLayout->addWidget(sixPreservationComboBox);
	
	//create the stacked widget and all child widget pages
  QWidget *dualLoopStyleRemeshingSixWidget = new QWidget;
  QWidget *loopStyleRemeshingSixWidget = new QWidget;

	//dual of loop-style remeshing
	QVBoxLayout *dualLoopStyleRemeshingSixLayout = new QVBoxLayout;
	
	QLabel *dualLoopStyleRemeshingSixTwistLabel = new QLabel(tr("Twist:"));
	QDoubleSpinBox *dualLoopStyleRemeshingSixTwistSpinBox = new QDoubleSpinBox;
	dualLoopStyleRemeshingSixTwistSpinBox->setRange(0.0, 1.0);
	dualLoopStyleRemeshingSixTwistSpinBox->setSingleStep(0.01);
	dualLoopStyleRemeshingSixTwistSpinBox->setValue(0.0);
	dualLoopStyleRemeshingSixTwistSpinBox->setDecimals(2);
	connect(dualLoopStyleRemeshingSixTwistSpinBox, SIGNAL(valueChanged(double)),
					((MainWindow*)parentWidget()),SLOT(changeRoot4Twist(double)) );	
	
	dualLoopStyleRemeshingSixLayout->addWidget(dualLoopStyleRemeshingSixTwistLabel);
  dualLoopStyleRemeshingSixLayout->addWidget(dualLoopStyleRemeshingSixTwistSpinBox);

	QLabel *dualLoopStyleRemeshingSixWeightLabel = new QLabel(tr("Weight:"));
	QDoubleSpinBox *dualLoopStyleRemeshingSixWeightSpinBox = new QDoubleSpinBox;
	dualLoopStyleRemeshingSixWeightSpinBox->setRange(0.0, 1.0);
	dualLoopStyleRemeshingSixWeightSpinBox->setSingleStep(0.01);
	dualLoopStyleRemeshingSixWeightSpinBox->setValue(0.0);
	dualLoopStyleRemeshingSixWeightSpinBox->setDecimals(2);
	connect(dualLoopStyleRemeshingSixWeightSpinBox, SIGNAL(valueChanged(double)),
					((MainWindow*)parentWidget()),SLOT(changeRoot4Weight(double)) );	
	
	dualLoopStyleRemeshingSixLayout->addWidget(dualLoopStyleRemeshingSixWeightLabel);
  dualLoopStyleRemeshingSixLayout->addWidget(dualLoopStyleRemeshingSixWeightSpinBox);

	QPushButton *dualLoopStyleRemeshingSixButton = new QPushButton(tr("Perform Remeshing"), this);
	connect(dualLoopStyleRemeshingSixButton, SIGNAL(clicked()),
					((MainWindow*)parentWidget()),SLOT(perform_remeshing()) );
					
	dualLoopStyleRemeshingSixLayout->addWidget(dualLoopStyleRemeshingSixButton);
	dualLoopStyleRemeshingSixLayout->addStretch(1);
	dualLoopStyleRemeshingSixWidget->setLayout(dualLoopStyleRemeshingSixLayout);
	
	//loop-style remeshing
	QVBoxLayout *loopStyleRemeshingSixLayout = new QVBoxLayout;

	QLabel *loopStyleRemeshingSixLabel = new QLabel(tr("Length:"));
	QDoubleSpinBox *loopStyleRemeshingSixSpinBox = new QDoubleSpinBox;
	loopStyleRemeshingSixSpinBox->setRange(0.0, 1.0);
	loopStyleRemeshingSixSpinBox->setSingleStep(0.01);
	loopStyleRemeshingSixSpinBox->setValue(1.0);
	loopStyleRemeshingSixSpinBox->setDecimals(2);
	connect(loopStyleRemeshingSixSpinBox, SIGNAL(valueChanged(double)),
					((MainWindow*)parentWidget()),SLOT(changeLoopStyleSubdivisionLength(double)) );	
	
	loopStyleRemeshingSixLayout->addWidget(loopStyleRemeshingSixLabel);
  loopStyleRemeshingSixLayout->addWidget(loopStyleRemeshingSixSpinBox);
	QPushButton *loopStyleRemeshingSixButton = new QPushButton(tr("Perform Remeshing"), this);
	connect(loopStyleRemeshingSixButton, SIGNAL(clicked()),
					((MainWindow*)parentWidget()),SLOT(perform_remeshing()) );
					
	loopStyleRemeshingSixLayout->addWidget(loopStyleRemeshingSixButton);
	loopStyleRemeshingSixLayout->addStretch(1);
	loopStyleRemeshingSixWidget->setLayout(loopStyleRemeshingSixLayout);
	
	//add the widgets to a stack controlled by the qcombobox
  QStackedWidget *sixPreservationStackedWidget = new QStackedWidget;
  sixPreservationStackedWidget->addWidget(dualLoopStyleRemeshingSixWidget);
	sixPreservationStackedWidget->addWidget(loopStyleRemeshingSixWidget);

	//add the stackedwidget to the layout
  sixPreservationLayout->addWidget(sixPreservationStackedWidget);
	//connect the combobox to the stacked widget
  connect(sixPreservationComboBox, SIGNAL(activated(int)),
          sixPreservationStackedWidget, SLOT(setCurrentIndex(int)));

	sixPreservationLayout->addStretch(1);
	sixPreservationWidget->setLayout(sixPreservationLayout);
	
	//connect the combobox signal to the mode switching slot
	connect(sixPreservationComboBox, SIGNAL(currentIndexChanged(int)),
					this,SLOT(switchSixPreservationMode(int)) );
}

void RemeshingMode::switchSixPreservationMode(int index){	
	
	switch(index){
		case 0: lastSixPreservationScheme = DLFLWindow::HexagonPreserving;
		break;
		case 1:	lastSixPreservationScheme = DLFLWindow::DualHexagonPreserving;
		break;	
		default:
		break;
	}
	((MainWindow*)parentWidget())->setRemeshingScheme(lastSixPreservationScheme);
}

void RemeshingMode::setupMiscellaneous(){
	
	lastMiscellaneousScheme = DLFLWindow::Fractal;
	lastMiscellaneousMode = DLFLWindow::NormalMode;
	miscellaneousLayout = new QVBoxLayout;
	
	QLabel *miscellaneousLabel = new QLabel(tr("Select Remeshing Scheme"));
	QComboBox *miscellaneousComboBox = new QComboBox();
	QStringList miscellaneousList;
	miscellaneousList << tr("Fractal") << tr("Double Stellate with Edge Removal") << tr("Doo-Sabin (BC)") 
										<< tr("Doo-Sabin (BC New)") << tr("Dome") << tr("Subdivide Face");
	
	miscellaneousComboBox->addItems(miscellaneousList);
	//add the combobox to the layout
	miscellaneousLayout->addWidget(miscellaneousLabel);
	miscellaneousLayout->addWidget(miscellaneousComboBox);
	
	//create the stacked widget and all child widget pages
  QWidget *fractalWidget = new QWidget;
  QWidget *doubleStellateMiscWidget = new QWidget;
  QWidget *dooSabinBCWidget = new QWidget;
  QWidget *dooSabinBCNewWidget = new QWidget;
  QWidget *domeWidget = new QWidget;
  QWidget *subdivideFaceWidget = new QWidget;

	//Fractal
	QVBoxLayout *fractalLayout = new QVBoxLayout;
	
	QLabel *fractalMultiplierLabel = new QLabel(tr("Multiplier:"));
	QDoubleSpinBox *fractalMultiplierSpinBox = new QDoubleSpinBox;
	fractalMultiplierSpinBox->setRange(0.0, 2.0);
	fractalMultiplierSpinBox->setSingleStep(0.01);
	fractalMultiplierSpinBox->setValue(1.0);
	fractalMultiplierSpinBox->setDecimals(2);
	connect(fractalMultiplierSpinBox, SIGNAL(valueChanged(double)),
					((MainWindow*)parentWidget()),SLOT(changeFractalOffset(double)) );	
	
	fractalLayout->addWidget(fractalMultiplierLabel);
  fractalLayout->addWidget(fractalMultiplierSpinBox);

	QPushButton *fractalButton = new QPushButton(tr("Perform Remeshing"), this);
	connect(fractalButton, SIGNAL(clicked()),
					((MainWindow*)parentWidget()),SLOT(perform_remeshing()) );
					
	fractalLayout->addWidget(fractalButton);
	fractalLayout->addStretch(1);
	fractalWidget->setLayout(fractalLayout);
	
	//double stellate with Edge Removal
	QVBoxLayout *doubleStellateMiscLayout = new QVBoxLayout;

	QLabel *doubleStellateMiscHeightLabel = new QLabel(tr("Height:"));
	QDoubleSpinBox *doubleStellateMiscHeightSpinBox = new QDoubleSpinBox;
	doubleStellateMiscHeightSpinBox->setRange(-1.0, 1.0);
	doubleStellateMiscHeightSpinBox->setSingleStep(0.01);
	doubleStellateMiscHeightSpinBox->setValue(0.0);
	doubleStellateMiscHeightSpinBox->setDecimals(2);
	connect(doubleStellateMiscHeightSpinBox, SIGNAL(valueChanged(double)),
					((MainWindow*)parentWidget()),SLOT(changeSubStellateAmount(double)) );	
	
	doubleStellateMiscLayout->addWidget(doubleStellateMiscHeightLabel);
  doubleStellateMiscLayout->addWidget(doubleStellateMiscHeightSpinBox);

	QLabel *doubleStellateMiscCurveLabel = new QLabel(tr("Curve:"));
	QDoubleSpinBox *doubleStellateMiscCurveSpinBox = new QDoubleSpinBox;
	doubleStellateMiscCurveSpinBox->setRange(-1.0, 1.0);
	doubleStellateMiscCurveSpinBox->setSingleStep(0.01);
	doubleStellateMiscCurveSpinBox->setValue(0.0);
	doubleStellateMiscCurveSpinBox->setDecimals(2);
	connect(doubleStellateMiscCurveSpinBox, SIGNAL(valueChanged(double)),
					((MainWindow*)parentWidget()),SLOT(changeSubStellateCurve(double)) );	
	
	doubleStellateMiscLayout->addWidget(doubleStellateMiscCurveLabel);
  doubleStellateMiscLayout->addWidget(doubleStellateMiscCurveSpinBox);

	QPushButton *doubleStellateMiscButton = new QPushButton(tr("Perform Remeshing"), this);
	connect(doubleStellateMiscButton, SIGNAL(clicked()),
					((MainWindow*)parentWidget()),SLOT(perform_remeshing()) );
					
	doubleStellateMiscLayout->addWidget(doubleStellateMiscButton);
	doubleStellateMiscLayout->addStretch(1);
	doubleStellateMiscWidget->setLayout(doubleStellateMiscLayout);
	
	//doo sabin bc
	QVBoxLayout *dooSabinBCLayout = new QVBoxLayout;
	QPushButton *dooSabinBCButton = new QPushButton(tr("Perform Remeshing"), this);
	connect(dooSabinBCButton, SIGNAL(clicked()),
					((MainWindow*)parentWidget()),SLOT(perform_remeshing()) );
					
	dooSabinBCLayout->addWidget(dooSabinBCButton);
	dooSabinBCLayout->addStretch(1);
	dooSabinBCWidget->setLayout(dooSabinBCLayout);
	
	//doo sabin bc new
	QVBoxLayout *dooSabinBCNewLayout = new QVBoxLayout;

	QLabel *dooSabinBCNewScaleLabel = new QLabel(tr("Scale:"));
	QDoubleSpinBox *dooSabinBCNewScaleSpinBox = new QDoubleSpinBox;
	dooSabinBCNewScaleSpinBox->setRange(0.0,2.0);
	dooSabinBCNewScaleSpinBox->setSingleStep(0.01);
	dooSabinBCNewScaleSpinBox->setValue(1.0);
	dooSabinBCNewScaleSpinBox->setDecimals(2);
	connect(dooSabinBCNewScaleSpinBox, SIGNAL(valueChanged(double)),
					((MainWindow*)parentWidget()),SLOT(changeDooSabinBCnewScale(double)) );	
	
	dooSabinBCNewLayout->addWidget(dooSabinBCNewScaleLabel);
  dooSabinBCNewLayout->addWidget(dooSabinBCNewScaleSpinBox);

	QLabel *dooSabinBCNewLengthLabel = new QLabel(tr("Length:"));
	QDoubleSpinBox *dooSabinBCNewLengthSpinBox = new QDoubleSpinBox;
	dooSabinBCNewLengthSpinBox->setRange(0.0, 1.0);
	dooSabinBCNewLengthSpinBox->setSingleStep(0.01);
	dooSabinBCNewLengthSpinBox->setValue(1.0);
	dooSabinBCNewLengthSpinBox->setDecimals(2);	
	connect(dooSabinBCNewLengthSpinBox, SIGNAL(valueChanged(double)),
					((MainWindow*)parentWidget()),SLOT(changeDooSabinBCnewLength(double)) );	
	
	dooSabinBCNewLayout->addWidget(dooSabinBCNewLengthLabel);
  dooSabinBCNewLayout->addWidget(dooSabinBCNewLengthSpinBox);

	QPushButton *dooSabinBCNewButton = new QPushButton(tr("Perform Remeshing"), this);
	connect(dooSabinBCNewButton, SIGNAL(clicked()),
					((MainWindow*)parentWidget()),SLOT(perform_remeshing()) );
					
	dooSabinBCNewLayout->addWidget(dooSabinBCNewButton);
	dooSabinBCNewLayout->addStretch(1);
	dooSabinBCNewWidget->setLayout(dooSabinBCNewLayout);
	
	//dome
	QVBoxLayout *domeLayout = new QVBoxLayout;

	QLabel *domeHeightLabel = new QLabel(tr("Height:"));
	QDoubleSpinBox *domeHeightSpinBox = new QDoubleSpinBox;
	domeHeightSpinBox->setRange(0.0, 2.0);
	domeHeightSpinBox->setSingleStep(0.01);
	domeHeightSpinBox->setValue(1.0);
	domeHeightSpinBox->setDecimals(2);
	connect(domeHeightSpinBox, SIGNAL(valueChanged(double)),
					((MainWindow*)parentWidget()),SLOT(changeDomeSubdivisionLength(double)) );	
	
	domeLayout->addWidget(domeHeightLabel);
  domeLayout->addWidget(domeHeightSpinBox);

	QLabel *domeScaleLabel = new QLabel(tr("Scale:"));
	QDoubleSpinBox *domeScaleSpinBox = new QDoubleSpinBox;
	domeScaleSpinBox->setRange(0.0, 2.0);
	domeScaleSpinBox->setSingleStep(0.01);
	domeScaleSpinBox->setValue(1.0);
	domeScaleSpinBox->setDecimals(2);
	connect(domeScaleSpinBox, SIGNAL(valueChanged(double)),
					((MainWindow*)parentWidget()),SLOT(changeDomeSubdivisionScale(double)) );	
	
	domeLayout->addWidget(domeScaleLabel);
  domeLayout->addWidget(domeScaleSpinBox);

	QPushButton *domeButton = new QPushButton(tr("Perform Remeshing"), this);
	connect(domeButton, SIGNAL(clicked()),
					((MainWindow*)parentWidget()),SLOT(perform_remeshing()) );
	domeLayout->addWidget(domeButton);
	domeLayout->addStretch(1);
	domeWidget->setLayout(domeLayout);
	
	//subdivide face
	QVBoxLayout *subdivideFaceLayout = new QVBoxLayout;
	QCheckBox *subdivideFaceCheckBox = new QCheckBox(tr("Use Quads (off -> triangles)"));
	connect(subdivideFaceCheckBox, SIGNAL(stateChanged(int)),
					((MainWindow*)parentWidget()),SLOT(toggleUseQuadsFlag(int)) );

	subdivideFaceLayout->addWidget(subdivideFaceCheckBox);
	subdivideFaceCheckBox->setChecked(true);
	QPushButton *subdivideFaceButton = new QPushButton(tr("Subdivide Selected Faces"), this);
	connect(subdivideFaceButton, SIGNAL(clicked()),
					((MainWindow*)parentWidget()),SLOT(subdivide_face()) );
					
	subdivideFaceLayout->addWidget(subdivideFaceButton);
	subdivideFaceLayout->addStretch(1);
	subdivideFaceWidget->setLayout(subdivideFaceLayout);
	
	//add the widgets to a stack controlled by the qcombobox
  QStackedWidget *miscellaneousStackedWidget = new QStackedWidget;
  miscellaneousStackedWidget->addWidget(fractalWidget );
	miscellaneousStackedWidget->addWidget(doubleStellateMiscWidget );
  miscellaneousStackedWidget->addWidget(dooSabinBCWidget );
	miscellaneousStackedWidget->addWidget(dooSabinBCNewWidget );
	miscellaneousStackedWidget->addWidget(domeWidget );
	miscellaneousStackedWidget->addWidget(subdivideFaceWidget );
	
	//add the stackedwidget to the layout
  miscellaneousLayout->addWidget(miscellaneousStackedWidget);
	//connect the combobox to the stacked widget
  connect(miscellaneousComboBox, SIGNAL(activated(int)),
          miscellaneousStackedWidget, SLOT(setCurrentIndex(int)));
	
	miscellaneousLayout->addStretch(1);
	miscellaneousWidget->setLayout(miscellaneousLayout);
	
	//connect the combobox signal to the mode switching slot
	connect(miscellaneousComboBox, SIGNAL(currentIndexChanged(int)),
					this,SLOT(switchMiscellaneousMode(int)) );
}

void RemeshingMode::switchMiscellaneousMode(int index){	
	// QMessageBox::about(this, "HEY",tr("index = %1").arg(index));
	
	switch(index){
		case 0: lastMiscellaneousMode = DLFLWindow::NormalMode;
						lastMiscellaneousScheme = DLFLWindow::Fractal;
						((MainWindow*)parentWidget())->setMode(DLFLWindow::NormalMode);
		break;
		case 1:	lastMiscellaneousMode = DLFLWindow::NormalMode;
						lastMiscellaneousScheme = DLFLWindow::ModifiedDoubleStellate;
						((MainWindow*)parentWidget())->setMode(DLFLWindow::NormalMode);
		break;	
		case 2: lastMiscellaneousMode = DLFLWindow::NormalMode;
						lastMiscellaneousScheme = DLFLWindow::Dome;
						((MainWindow*)parentWidget())->setMode(DLFLWindow::NormalMode);
		break;
		case 3: lastMiscellaneousMode = DLFLWindow::NormalMode;
						lastMiscellaneousScheme = DLFLWindow::DooSabinBC;
						((MainWindow*)parentWidget())->setMode(DLFLWindow::NormalMode);
		break;
		case 4: lastMiscellaneousMode = DLFLWindow::NormalMode;
						lastMiscellaneousScheme = DLFLWindow::DooSabinBCNew;
						((MainWindow*)parentWidget())->setMode(DLFLWindow::NormalMode);
		break;
		case 5: lastMiscellaneousMode = DLFLWindow::SubDivideFace;
						((MainWindow*)parentWidget())->setMode(lastMiscellaneousMode);
		break;
		default:
		break;
	}
	((MainWindow*)parentWidget())->setRemeshingScheme(lastMiscellaneousScheme);
}

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