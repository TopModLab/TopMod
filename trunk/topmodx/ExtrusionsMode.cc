#include <QtGui>

#include "ExtrusionsMode.hh"

/*!
	\ingroup gui
	@{
	
	\class ExtrusionsMode
	\brief Extrusion Operations GUI elements.
	
	\note 
	
	\see ExtrusionsMode
*/

/*!
* \brief Constructor
* 
* @param parent the MainWindow widget
* @param sm the shortcut manager class for adding a custom shortcut to each menu action or icon
* @param actionList the master list of actions for use with the CommandCompleter class
* 
*/
ExtrusionsMode::ExtrusionsMode(QWidget *parent, QShortcutManager *sm, QWidget *actionList)
		: QWidget(parent)
{		
	setParent(0);
	mParent = parent;
	
	//create the stacked widget and all child widget pages
	mDooSabinExtrudeWidget = new QWidget;
  mCubicalExtrudeWidget = new QWidget;
  mDodecahedralExtrudeWidget = new QWidget;
  mIcosahedralExtrudeWidget = new QWidget;
  mOctahedralExtrudeWidget = new QWidget;
  mStellateExtrudeWidget = new QWidget;
  mDoubleStellateExtrudeWidget = new QWidget;
  mDomeExtrudeWidget = new QWidget;
	
	//setup stacked widget pages here, 
	//each in a separate function to make the code more readable
	setupCubicalExtrude();
	setupDooSabinExtrude();
	setupDodecahedralExtrude();
	setupIcosahedralExtrude();
	setupOctahedralExtrude();
	setupStellateExtrude();
	setupDoubleStellateExtrude();
	setupDomeExtrude();
 	
	mDooSabinExtrudeAction = new QAction(QIcon(":images/extrude_doosabin.png"),tr("Doo Sabin"),this);
	mDooSabinExtrudeAction->setCheckable(true);
	sm->registerAction(mDooSabinExtrudeAction, "Extrusion Operations", "");
	mDooSabinExtrudeAction->setStatusTip(tr("Enter Doo Sabin Extrude Mode"));
	mDooSabinExtrudeAction->setToolTip(tr("Doo Sabin Extrude Mode"));
	connect(mDooSabinExtrudeAction, SIGNAL(triggered()), this, SLOT(triggerDooSabinExtrude()));
	actionList->addAction(mDooSabinExtrudeAction);
	
	mCubicalExtrudeAction = new QAction(QIcon(":images/extrude_cubical.png"),tr("Cubical"),this);
	mCubicalExtrudeAction->setCheckable(true);
	sm->registerAction(mCubicalExtrudeAction, "Extrusion Operations", "");
	mCubicalExtrudeAction->setStatusTip(tr("Enter Cubical Extrude Mode"));
	mCubicalExtrudeAction->setToolTip(tr("Cubical Extrude Mode"));
	connect(mCubicalExtrudeAction, SIGNAL(triggered()), this, SLOT(triggerCubicalExtrude()));
	actionList->addAction(mCubicalExtrudeAction);
	
	mDodecahedralExtrudeAction = new QAction(QIcon(":images/extrude_dodecahedral.png"),tr("Dodecahedral"),this);
	mDodecahedralExtrudeAction->setCheckable(true);
	mDodecahedralExtrudeAction->setEnabled(true);
	sm->registerAction(mDodecahedralExtrudeAction, "Extrusion Operations", "");
	mDodecahedralExtrudeAction->setStatusTip(tr("Enter Dodecahedral Extrude Mode"));
	mDodecahedralExtrudeAction->setToolTip(tr("Dodecahedral Extrude Mode"));
	connect(mDodecahedralExtrudeAction, SIGNAL(triggered()), this, SLOT(triggerDodecahedralExtrude()));
	actionList->addAction(mDodecahedralExtrudeAction);
	
	mIcosahedralExtrudeAction = new QAction(QIcon(":images/extrude_icosahedral.png"),tr("Icosahedral"),this);
	mIcosahedralExtrudeAction->setCheckable(true);
	mIcosahedralExtrudeAction->setEnabled(true);
	sm->registerAction(mIcosahedralExtrudeAction, "Extrusion Operations", "");
	mIcosahedralExtrudeAction->setStatusTip(tr("Enter Icosahedral Extrude Mode"));
	mIcosahedralExtrudeAction->setToolTip(tr("Icosahedral Extrude Mode"));
	connect(mIcosahedralExtrudeAction, SIGNAL(triggered()), this, SLOT(triggerIcosahedralExtrude()));
	actionList->addAction(mIcosahedralExtrudeAction);
	
	mOctahedralExtrudeAction = new QAction(QIcon(":images/extrude_octahedral.png"),tr("Octahedral"),this);
	mOctahedralExtrudeAction->setCheckable(true);
	mOctahedralExtrudeAction->setEnabled(true);
	sm->registerAction(mOctahedralExtrudeAction, "Extrusion Operations", "");
	mOctahedralExtrudeAction->setStatusTip(tr("Enter Octahedral Extrude Mode"));
	mOctahedralExtrudeAction->setToolTip(tr("Octahedral Extrude Mode"));
	connect(mOctahedralExtrudeAction, SIGNAL(triggered()), this, SLOT(triggerOctahedralExtrude()));
	actionList->addAction(mOctahedralExtrudeAction);
	
	mStellateExtrudeAction = new QAction(QIcon(":images/extrude_stellate.png"),tr("Stellate"),this);
	mStellateExtrudeAction->setCheckable(true);
	sm->registerAction(mStellateExtrudeAction, "Extrusion Operations", "");
	mStellateExtrudeAction->setStatusTip(tr("Enter Stellate Extrude Mode"));
	mStellateExtrudeAction->setToolTip(tr("Stellate Extrude Mode"));
	connect(mStellateExtrudeAction, SIGNAL(triggered()), this, SLOT(triggerStellateExtrude()));
	actionList->addAction(mStellateExtrudeAction);
	
	mDoubleStellateExtrudeAction = new QAction(QIcon(":images/extrude_doublestellate.png"),tr("Double Stellate"),this);
	mDoubleStellateExtrudeAction->setCheckable(true);
	sm->registerAction(mDoubleStellateExtrudeAction, "Extrusion Operations", "");
	mDoubleStellateExtrudeAction->setStatusTip(tr("Enter Double Stellate Mode"));
	mDoubleStellateExtrudeAction->setToolTip(tr("Double Stellate Mode"));
	connect(mDoubleStellateExtrudeAction, SIGNAL(triggered()), this, SLOT(triggerDoubleStellateExtrude()));
	actionList->addAction(mDoubleStellateExtrudeAction);

	mDomeExtrudeAction = new QAction(QIcon(":images/extrude_dome.png"),tr("Dome"),this);
	mDomeExtrudeAction->setCheckable(true);
	sm->registerAction(mDomeExtrudeAction, "Extrusion Operations", "");
	mDomeExtrudeAction->setStatusTip(tr("Enter Dome Extrude Mode"));
	mDomeExtrudeAction->setToolTip(tr("Dome Extrude Mode"));
	connect(mDomeExtrudeAction, SIGNAL(triggered()), this, SLOT(triggerDomeExtrude()));
	actionList->addAction(mDomeExtrudeAction);
}

QMenu* ExtrusionsMode::getMenu(){
	
	mExtrusionMenu = new QMenu(tr("Extrusion"));
	
	mExtrusionMenu->addAction(mDooSabinExtrudeAction);
	mExtrusionMenu->addAction(mCubicalExtrudeAction);
	mExtrusionMenu->addAction(mDodecahedralExtrudeAction);
	mExtrusionMenu->addAction(mIcosahedralExtrudeAction);
	mExtrusionMenu->addAction(mOctahedralExtrudeAction);
	mExtrusionMenu->addAction(mStellateExtrudeAction);
	mExtrusionMenu->addAction(mDoubleStellateExtrudeAction);	
	mExtrusionMenu->addAction(mDomeExtrudeAction);	
	
	return mExtrusionMenu;	
}

void ExtrusionsMode::addActions(QActionGroup *actionGroup, QToolBar *toolBar, QStackedWidget *stackedWidget){
	
	actionGroup->addAction(mDooSabinExtrudeAction);
	actionGroup->addAction(mCubicalExtrudeAction);
	actionGroup->addAction(mDodecahedralExtrudeAction);
	actionGroup->addAction(mIcosahedralExtrudeAction);
	actionGroup->addAction(mOctahedralExtrudeAction);
	actionGroup->addAction(mStellateExtrudeAction);
	actionGroup->addAction(mDoubleStellateExtrudeAction);
	actionGroup->addAction(mDomeExtrudeAction);
	
	toolBar->addAction(mDooSabinExtrudeAction);
	toolBar->addAction(mCubicalExtrudeAction);
	toolBar->addAction(mDodecahedralExtrudeAction);   
	toolBar->addAction(mIcosahedralExtrudeAction);   	
	toolBar->addAction(mOctahedralExtrudeAction);     
	toolBar->addAction(mStellateExtrudeAction);
	toolBar->addAction(mDoubleStellateExtrudeAction);
	toolBar->addAction(mDomeExtrudeAction);
	
	stackedWidget->addWidget(mDooSabinExtrudeWidget);
	stackedWidget->addWidget(mCubicalExtrudeWidget);
	stackedWidget->addWidget(mDodecahedralExtrudeWidget);   
	stackedWidget->addWidget(mIcosahedralExtrudeWidget);   	
	stackedWidget->addWidget(mOctahedralExtrudeWidget);     
	stackedWidget->addWidget(mStellateExtrudeWidget);
	stackedWidget->addWidget(mDoubleStellateExtrudeWidget);
	stackedWidget->addWidget(mDomeExtrudeWidget);
	
}

void ExtrusionsMode::triggerDooSabinExtrude(){
	
	((MainWindow*)mParent)->setToolOptions(mDooSabinExtrudeWidget);
	((MainWindow*)mParent)->setMode(MainWindow::ExtrudeFaceDS);
	((MainWindow*)mParent)->setExtrusionMode(MainWindow::DooSabinExtrude);
	((MainWindow*)mParent)->setSpinBoxes(dooSabinLengthSpinBox,dooSabinRotationSpinBox,dooSabinScaleSpinBox,dooSabinSegmentsSpinBox);
}

void ExtrusionsMode::triggerCubicalExtrude(){

	((MainWindow*)mParent)->setToolOptions(mCubicalExtrudeWidget);
	((MainWindow*)mParent)->setMode(MainWindow::ExtrudeFace);	
	((MainWindow*)mParent)->setExtrusionMode(MainWindow::CubicalExtrude);
	((MainWindow*)mParent)->setSpinBoxes(cubicalLengthSpinBox,cubicalRotationSpinBox,cubicalScaleSpinBox,cubicalSegmentsSpinBox);
}

void ExtrusionsMode::triggerDodecahedralExtrude(){
	
	((MainWindow*)mParent)->setToolOptions(mDodecahedralExtrudeWidget);
	// if (((MainWindow*)mParent)->getExtrudeMultiple())
	// 	((MainWindow*)mParent)->setMode(MainWindow::SelectMultiple);
	// else
	((MainWindow*)mParent)->setMode(MainWindow::ExtrudeFaceDodeca);
	((MainWindow*)mParent)->setExtrusionMode(MainWindow::DodecahedralExtrude);
	((MainWindow*)mParent)->setSpinBoxes(dodecahedralLengthSpinBox,dodecahedralScaleSpinBox,dodecahedralSegmentsSpinBox);
}

void ExtrusionsMode::triggerIcosahedralExtrude(){
	
	((MainWindow*)mParent)->setToolOptions(mIcosahedralExtrudeWidget);
	((MainWindow*)mParent)->setMode(MainWindow::ExtrudeFaceIcosa);
	((MainWindow*)mParent)->setExtrusionMode(MainWindow::IcosahedralExtrude);
	((MainWindow*)mParent)->setSpinBoxes(icosahedralLengthSpinBox,icosahedralScaleSpinBox,icosahedralSegmentsSpinBox);
}

void ExtrusionsMode::triggerOctahedralExtrude(){
	
	((MainWindow*)mParent)->setToolOptions(mOctahedralExtrudeWidget);
	((MainWindow*)mParent)->setMode(MainWindow::ExtrudeDualFace);
	((MainWindow*)mParent)->setExtrusionMode(MainWindow::OctahedralExtrude);
	((MainWindow*)mParent)->setSpinBoxes(octahedralLengthSpinBox,octahedralRotationSpinBox,octahedralScaleSpinBox,octahedralSegmentsSpinBox);
}

void ExtrusionsMode::triggerStellateExtrude(){
	
	((MainWindow*)mParent)->setToolOptions(mStellateExtrudeWidget);
	((MainWindow*)mParent)->setMode(MainWindow::StellateFace);
	((MainWindow*)mParent)->setExtrusionMode(MainWindow::StellateExtrude);
	((MainWindow*)mParent)->setSpinBoxes(stellateLengthSpinBox);
}

void ExtrusionsMode::triggerDoubleStellateExtrude(){
	
	((MainWindow*)mParent)->setToolOptions(mDoubleStellateExtrudeWidget);
	((MainWindow*)mParent)->setMode(MainWindow::DoubleStellateFace);
	((MainWindow*)mParent)->setExtrusionMode(MainWindow::DoubleStellateExtrude);
	((MainWindow*)mParent)->setSpinBoxes(doubleStellateLengthSpinBox);
}

void ExtrusionsMode::triggerDomeExtrude(){
	
	((MainWindow*)mParent)->setToolOptions(mDomeExtrudeWidget);
	((MainWindow*)mParent)->setMode(MainWindow::ExtrudeFaceDome);
	((MainWindow*)mParent)->setExtrusionMode(MainWindow::DomeExtrude);
	((MainWindow*)mParent)->setSpinBoxes(domeHeightSpinBox,domeScaleSpinBox);
}

void ExtrusionsMode::setLength(double value){
	dooSabinLengthSpinBox->setValue(value);
	cubicalLengthSpinBox->setValue(value);
	dodecahedralLengthSpinBox->setValue(value);
	icosahedralLengthSpinBox->setValue(value);
	octahedralLengthSpinBox->setValue(value);
	stellateLengthSpinBox->setValue(value);
	doubleStellateLengthSpinBox->setValue(value);
	
	((MainWindow*)mParent)->changeExtrudeLength(value);
}
void ExtrusionsMode::setRotation(double value){
	dooSabinRotationSpinBox->setValue(value);
	cubicalRotationSpinBox->setValue(value);
	octahedralRotationSpinBox->setValue(value);
	
	((MainWindow*)mParent)->changeExtrudeRotation((int)value);
}

void ExtrusionsMode::setSegments(double value){
	dooSabinSegmentsSpinBox->setValue(value);
	cubicalSegmentsSpinBox->setValue(value);
	dodecahedralSegmentsSpinBox->setValue(value);
	icosahedralSegmentsSpinBox->setValue(value);
	octahedralSegmentsSpinBox->setValue(value);

	((MainWindow*)mParent)->changeNumExtrusions((int)value);
}

void ExtrusionsMode::setScale(double value){
	dooSabinScaleSpinBox->setValue(value);
	cubicalScaleSpinBox->setValue(value);
	dodecahedralScaleSpinBox->setValue(value);
	icosahedralScaleSpinBox->setValue(value);
	octahedralScaleSpinBox->setValue(value);
		
	((MainWindow*)mParent)->changeExtrudeScale(value);
}

QDoubleSpinBox *ExtrusionsMode::createDoubleSpinBox(QGridLayout *layout, QLabel *label, QString s, double low, double high, double step, double value, double decimals, int row, int col){
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

void ExtrusionsMode::setupDooSabinExtrude(){
	
	mDooSabinExtrudeLayout = new QGridLayout;
	mDooSabinExtrudeLayout->setVerticalSpacing(1);
	mDooSabinExtrudeLayout->setHorizontalSpacing(1);
	// mDooSabinExtrudeLayout->setMargin(0);
	//length
	dooSabinLengthSpinBox = createDoubleSpinBox(mDooSabinExtrudeLayout, dooSabinLengthLabel, tr("Length"), -100.0, 100.0, 0.1, 2.0, 1, 0,0);
	connect(dooSabinLengthSpinBox, SIGNAL(valueChanged(double)),this, SLOT(setLength(double)));
	//rotation
	// dooSabinRotationSpinBox = createDoubleSpinBox(mDooSabinExtrudeLayout, dooSabinRotationLabel, tr("Rotation"), 0, 360, 1, 0.0, 1, 1,0);
	// connect(dooSabinRotationSpinBox, SIGNAL(valueChanged(double)),this, SLOT(setRotation(double)));
	//scale
	dooSabinScaleSpinBox = createDoubleSpinBox(mDooSabinExtrudeLayout, dooSabinScaleLabel, tr("Scale"), 0.0, 10.0, 0.01, 1.0, 2, 1,0);
	connect(dooSabinScaleSpinBox, SIGNAL(valueChanged(double)),this, SLOT(setScale(double)));
	//segments
	dooSabinSegmentsSpinBox = createDoubleSpinBox(mDooSabinExtrudeLayout, dooSabinSegmentsLabel, tr("Segments"), 1, 50, 1, 1, 0, 2,0);
	connect(dooSabinSegmentsSpinBox, SIGNAL(valueChanged(double)),this, SLOT(setSegments(double)));

	QPushButton *performExtrusionButton = new QPushButton(tr("Extrude Selected Faces"), this);
	connect(performExtrusionButton, SIGNAL(clicked()), ((MainWindow*)mParent),SLOT(performExtrusion()) );
	mDooSabinExtrudeLayout->addWidget(performExtrusionButton,3,0,1,2);	

	mDooSabinExtrudeLayout->setRowStretch(4,1);
	mDooSabinExtrudeLayout->setColumnStretch(2,1);
	mDooSabinExtrudeWidget->setWindowTitle(tr("Doo Sabin Extrusion"));
	mDooSabinExtrudeWidget->setLayout(mDooSabinExtrudeLayout);	
}

void ExtrusionsMode::setupCubicalExtrude(){	
	mCubicalExtrudeLayout = new QGridLayout;
	mCubicalExtrudeLayout->setVerticalSpacing(1);
	mCubicalExtrudeLayout->setHorizontalSpacing(1);
	// mCubicalExtrudeLayout->setMargin(0);
	//length
	cubicalLengthSpinBox = createDoubleSpinBox(mCubicalExtrudeLayout, cubicalLengthLabel, tr("Length"), -100.0, 100.0, 0.1, 2.0, 1, 0,0);
	connect(cubicalLengthSpinBox, SIGNAL(valueChanged(double)),this, SLOT(setLength(double)));
	//rotation
	cubicalRotationSpinBox = createDoubleSpinBox(mCubicalExtrudeLayout, cubicalRotationLabel, tr("Rotation"), 0, 360, 1, 0.0, 1, 1,0);
	connect(cubicalRotationSpinBox, SIGNAL(valueChanged(double)),this, SLOT(setRotation(double)));
	//scale
	cubicalScaleSpinBox = createDoubleSpinBox(mCubicalExtrudeLayout, cubicalScaleLabel, tr("Scale"), 0.0, 10.0, 0.01, 1.0, 2, 2,0);
	connect(cubicalScaleSpinBox, SIGNAL(valueChanged(double)),this, SLOT(setScale(double)));
	//segments
	cubicalSegmentsSpinBox = createDoubleSpinBox(mCubicalExtrudeLayout, cubicalSegmentsLabel, tr("Segments"), 1, 50, 1, 1, 0, 3,0);
	connect(cubicalSegmentsSpinBox, SIGNAL(valueChanged(double)),this, SLOT(setSegments(double)));

	QPushButton *performExtrusionButton = new QPushButton(tr("Extrude Selected Faces"), this);
	connect(performExtrusionButton, SIGNAL(clicked()), ((MainWindow*)mParent),SLOT(performExtrusion()) );
	mCubicalExtrudeLayout->addWidget(performExtrusionButton,4,0,1,2);	

	mCubicalExtrudeLayout->setRowStretch(5,1);
	mCubicalExtrudeLayout->setColumnStretch(2,1);
	mCubicalExtrudeWidget->setWindowTitle(tr("Cubical Extrusion"));
	mCubicalExtrudeWidget->setLayout(mCubicalExtrudeLayout);	
}

void ExtrusionsMode::setupDodecahedralExtrude(){
	mDodecahedralExtrudeLayout = new QGridLayout;
	mDodecahedralExtrudeLayout->setVerticalSpacing(1);
	mDodecahedralExtrudeLayout->setHorizontalSpacing(1);
	// mDodecahedralExtrudeLayout->setMargin(0);
	//length
	dodecahedralLengthSpinBox = createDoubleSpinBox(mDodecahedralExtrudeLayout, dodecahedralLengthLabel, tr("Length"), -100.0, 100.0, 0.1, 2.0, 1, 0,0);
	connect(dodecahedralLengthSpinBox, SIGNAL(valueChanged(double)),this, SLOT(setLength(double)));
	//scale
	dodecahedralScaleSpinBox = createDoubleSpinBox(mDodecahedralExtrudeLayout, dodecahedralScaleLabel, tr("Scale"), 0.0, 10.0, 0.01, 1.0, 2, 1,0);
	connect(dodecahedralScaleSpinBox, SIGNAL(valueChanged(double)),this, SLOT(setScale(double)));
	//segments
	dodecahedralSegmentsSpinBox = createDoubleSpinBox(mDodecahedralExtrudeLayout, dodecahedralSegmentsLabel, tr("Segments"), 1, 50, 1, 1, 0, 2,0);
	connect(dodecahedralSegmentsSpinBox, SIGNAL(valueChanged(double)),this, SLOT(setSegments(double)));	
	//mesh flat edges checkbox
	hexagonalizeCheckBox = new QCheckBox(tr("Hexagonalize"),this);
	connect(hexagonalizeCheckBox, SIGNAL(stateChanged(int)),((MainWindow*)mParent), SLOT(toggleHexagonalizeDodecaExtrudeFlag(int)));
	
	mDodecahedralExtrudeLayout->addWidget(hexagonalizeCheckBox,3,1);

	QPushButton *performExtrusionButton = new QPushButton(tr("Extrude Selected Faces"), this);
	connect(performExtrusionButton, SIGNAL(clicked()), ((MainWindow*)mParent),SLOT(performExtrusion()) );
	mDodecahedralExtrudeLayout->addWidget(performExtrusionButton,4,0,1,2);	

	mDodecahedralExtrudeLayout->setRowStretch(5,1);
	mDodecahedralExtrudeLayout->setColumnStretch(2,1);
	mDodecahedralExtrudeWidget->setWindowTitle(tr("Dodecahedral Extrusion"));
	mDodecahedralExtrudeWidget->setLayout(mDodecahedralExtrudeLayout);	
}

void ExtrusionsMode::setupIcosahedralExtrude(){
	mIcosahedralExtrudeLayout = new QGridLayout;
	mIcosahedralExtrudeLayout->setVerticalSpacing(1);
	mIcosahedralExtrudeLayout->setHorizontalSpacing(1);
	// mIcosahedralExtrudeLayout->setMargin(0);
	//length
	icosahedralLengthSpinBox = createDoubleSpinBox(mIcosahedralExtrudeLayout, icosahedralLengthLabel, tr("Length"), -100.0, 100.0, 0.1, 2.0, 1, 0,0);
	connect(icosahedralLengthSpinBox, SIGNAL(valueChanged(double)),this, SLOT(setLength(double)));
	//scale
	icosahedralScaleSpinBox = createDoubleSpinBox(mIcosahedralExtrudeLayout, icosahedralScaleLabel, tr("Scale"), 0.0, 10.0, 0.01, 1.0, 2, 1,0);
	connect(icosahedralScaleSpinBox, SIGNAL(valueChanged(double)),this, SLOT(setScale(double)));
	//segments
	icosahedralSegmentsSpinBox = createDoubleSpinBox(mIcosahedralExtrudeLayout, icosahedralSegmentsLabel, tr("Segments"), 1, 50, 1, 1, 0, 2,0);
	connect(icosahedralSegmentsSpinBox, SIGNAL(valueChanged(double)),this, SLOT(setSegments(double)));	

	QPushButton *performExtrusionButton = new QPushButton(tr("Extrude Selected Faces"), this);
	connect(performExtrusionButton, SIGNAL(clicked()), ((MainWindow*)mParent),SLOT(performExtrusion()) );
	mIcosahedralExtrudeLayout->addWidget(performExtrusionButton,3,0,1,2);	

	mIcosahedralExtrudeLayout->setRowStretch(4,1);
	mIcosahedralExtrudeLayout->setColumnStretch(2,1);
	mIcosahedralExtrudeWidget->setWindowTitle(tr("Icosahedral Extrusion"));
	mIcosahedralExtrudeWidget->setLayout(mIcosahedralExtrudeLayout);
}

void ExtrusionsMode::setupOctahedralExtrude(){
	mOctahedralExtrudeLayout = new QGridLayout;
	mOctahedralExtrudeLayout->setVerticalSpacing(1);
	mOctahedralExtrudeLayout->setHorizontalSpacing(1);
	// mOctahedralExtrudeLayout->setMargin(0);
	//length
	octahedralLengthSpinBox = createDoubleSpinBox(mOctahedralExtrudeLayout, octahedralLengthLabel, tr("Length"), -100.0, 100.0, 0.1, 2.0, 1, 0,0);
	connect(octahedralLengthSpinBox, SIGNAL(valueChanged(double)),this, SLOT(setLength(double)));
	//rotation
	octahedralRotationSpinBox = createDoubleSpinBox(mOctahedralExtrudeLayout, octahedralRotationLabel, tr("Rotation"), 0, 360, 1, 0.0, 1, 1,0);
	connect(octahedralRotationSpinBox, SIGNAL(valueChanged(double)),this, SLOT(setRotation(double)));
	//scale
	octahedralScaleSpinBox = createDoubleSpinBox(mOctahedralExtrudeLayout, octahedralScaleLabel, tr("Scale"), 0.0, 10.0, 0.01, 1.0, 2, 2,0);
	connect(octahedralScaleSpinBox, SIGNAL(valueChanged(double)),this, SLOT(setScale(double)));
	//segments
	octahedralSegmentsSpinBox = createDoubleSpinBox(mOctahedralExtrudeLayout, octahedralSegmentsLabel, tr("Segments"), 1, 50, 1, 1, 0, 3,0);
	connect(octahedralSegmentsSpinBox, SIGNAL(valueChanged(double)),this, SLOT(setSegments(double)));
	//mesh flat edges checkbox
	meshFlatEdgesCheckBox = new QCheckBox(tr("Mesh Flat Edges"),this);
	connect(meshFlatEdgesCheckBox, SIGNAL(stateChanged(int)), ((MainWindow*)mParent), SLOT(toggleDualMeshEdgesFlag(int)));

	mOctahedralExtrudeLayout->addWidget(meshFlatEdgesCheckBox,4,1);
	
	QPushButton *performExtrusionButton = new QPushButton(tr("Extrude Selected Faces"), this);
	connect(performExtrusionButton, SIGNAL(clicked()), ((MainWindow*)mParent),SLOT(performExtrusion()) );
	mOctahedralExtrudeLayout->addWidget(performExtrusionButton,5,0,1,2);	
	
	mOctahedralExtrudeLayout->setRowStretch(6,1);
	mOctahedralExtrudeLayout->setColumnStretch(2,1);
	mOctahedralExtrudeWidget->setWindowTitle(tr("Octahedral Extrusion"));
	mOctahedralExtrudeWidget->setLayout(mOctahedralExtrudeLayout);
}

void ExtrusionsMode::setupStellateExtrude(){
	mStellateExtrudeLayout = new QGridLayout;
	mStellateExtrudeLayout->setVerticalSpacing(1);
	mStellateExtrudeLayout->setHorizontalSpacing(1);
	// mStellateExtrudeLayout->setMargin(0);
	//length
	stellateLengthSpinBox = createDoubleSpinBox(mStellateExtrudeLayout, stellateLengthLabel, tr("Length"), -100.0, 100.0, 0.1, 2.0, 1, 0,0);
	connect(stellateLengthSpinBox, SIGNAL(valueChanged(double)),this, SLOT(setLength(double)));
	
	QPushButton *performExtrusionButton = new QPushButton(tr("Extrude Selected Faces"), this);
	connect(performExtrusionButton, SIGNAL(clicked()), ((MainWindow*)mParent),SLOT(performExtrusion()) );
	mStellateExtrudeLayout->addWidget(performExtrusionButton,1,0,1,2);	
	
	mStellateExtrudeLayout->setRowStretch(2,1);
	mStellateExtrudeLayout->setColumnStretch(2,1);
	mStellateExtrudeWidget->setWindowTitle(tr("Stellate Extrusion"));
	mStellateExtrudeWidget->setLayout(mStellateExtrudeLayout);	
}

void ExtrusionsMode::setupDoubleStellateExtrude(){
	mDoubleStellateExtrudeLayout = new QGridLayout;
	mDoubleStellateExtrudeLayout->setVerticalSpacing(1);
	mDoubleStellateExtrudeLayout->setHorizontalSpacing(1);
	// mDoubleStellateExtrudeLayout->setMargin(0);
	//length
	doubleStellateLengthSpinBox = createDoubleSpinBox(mDoubleStellateExtrudeLayout, doubleStellateLengthLabel, tr("Length"), -100.0, 100.0, 0.1, 2.0, 1, 0,0);
	connect(doubleStellateLengthSpinBox, SIGNAL(valueChanged(double)),this, SLOT(setLength(double)));
	
	QPushButton *performExtrusionButton = new QPushButton(tr("Extrude Selected Faces"), this);
	connect(performExtrusionButton, SIGNAL(clicked()), ((MainWindow*)mParent),SLOT(performExtrusion()) );
	mDoubleStellateExtrudeLayout->addWidget(performExtrusionButton,1,0,1,2);	
	
	mDoubleStellateExtrudeLayout->setRowStretch(2,1);
	mDoubleStellateExtrudeLayout->setColumnStretch(2,1);
	mDoubleStellateExtrudeWidget->setWindowTitle(tr("Double Stellate Extrusion"));
	mDoubleStellateExtrudeWidget->setLayout(mDoubleStellateExtrudeLayout);	
}

void ExtrusionsMode::setupDomeExtrude(){
	mDomeExtrudeLayout = new QGridLayout;
	mDomeExtrudeLayout->setVerticalSpacing(1);
	mDomeExtrudeLayout->setHorizontalSpacing(1);
	// mDomeLayout->setMargin(0);
	domeHeightSpinBox = createDoubleSpinBox(mDomeExtrudeLayout, domeHeightLabel, tr("Height:"), 0.0, 2.0, 0.01, 1.0, 2, 0,0);
	connect(domeHeightSpinBox, SIGNAL(valueChanged(double)), ((MainWindow*)mParent),SLOT(changeDomeExtrudeLength(double)) );	
	domeScaleSpinBox = createDoubleSpinBox(mDomeExtrudeLayout, domeScaleLabel, tr("Scale:"), 0.0, 2.0, 0.01, 1.0, 2, 1,0);
	connect(domeScaleSpinBox, SIGNAL(valueChanged(double)), ((MainWindow*)mParent),SLOT(changeDomeExtrudeScale(double)) );	
	QPushButton *domeButton = new QPushButton(tr("Perform Extrusion"), this);
	connect(domeButton, SIGNAL(clicked()), ((MainWindow*)mParent),SLOT(performExtrusion()) );
	mDomeExtrudeLayout->addWidget(domeButton,2,0,1,2);
	mDomeExtrudeLayout->setRowStretch(3,1);
	mDomeExtrudeLayout->setColumnStretch(2,1);
	mDomeExtrudeWidget->setWindowTitle("Dome Extrusion");
	mDomeExtrudeWidget->setLayout(mDomeExtrudeLayout);
}

void ExtrusionsMode::retranslateUi(){
	
}
