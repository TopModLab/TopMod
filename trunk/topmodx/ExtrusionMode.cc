#include <QtGui>

#include "ExtrusionMode.hh"

/*!
	\ingroup gui
	@{
	
	\class ExtrusionMode
	\brief Extrusion Operations GUI elements.
	
	\note 
	
	\see ExtrusionMode
*/

/*!
* \brief Constructor
* 
* @param parent the MainWindow widget
* @param sm the shortcut manager class for adding a custom shortcut to each menu action or icon
* @param actionList the master list of actions for use with the CommandCompleter class
* 
*/
ExtrusionMode::ExtrusionMode(QWidget *parent, QShortcutManager *sm, QWidget *actionList)
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
	
	//setup stacked widget pages here, 
	//each in a separate function to make the code more readable
	setupCubicalExtrude();
	setupDooSabinExtrude();
	setupDodecahedralExtrude();
	setupIcosahedralExtrude();
	setupOctahedralExtrude();
	setupStellateExtrude();
	setupDoubleStellateExtrude();
 	
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
}

QMenu* ExtrusionMode::getMenu(){
	
	mExtrusionMenu = new QMenu(tr("Extrusion"));
	
	mExtrusionMenu->addAction(mDooSabinExtrudeAction);
	mExtrusionMenu->addAction(mCubicalExtrudeAction);
	mExtrusionMenu->addAction(mDodecahedralExtrudeAction);
	mExtrusionMenu->addAction(mIcosahedralExtrudeAction);
	mExtrusionMenu->addAction(mOctahedralExtrudeAction);
	mExtrusionMenu->addAction(mStellateExtrudeAction);
	mExtrusionMenu->addAction(mDoubleStellateExtrudeAction);	
	
	return mExtrusionMenu;	
}

void ExtrusionMode::addActions(QActionGroup *actionGroup, QToolBar *toolBar, QStackedWidget *stackedWidget){
	
	actionGroup->addAction(mDooSabinExtrudeAction);
	actionGroup->addAction(mCubicalExtrudeAction);
	actionGroup->addAction(mDodecahedralExtrudeAction);
	actionGroup->addAction(mIcosahedralExtrudeAction);
	actionGroup->addAction(mOctahedralExtrudeAction);
	actionGroup->addAction(mStellateExtrudeAction);
	actionGroup->addAction(mDoubleStellateExtrudeAction);
	
	toolBar->addAction(mDooSabinExtrudeAction);
	toolBar->addAction(mCubicalExtrudeAction);
	toolBar->addAction(mDodecahedralExtrudeAction);   
	toolBar->addAction(mIcosahedralExtrudeAction);   	
	toolBar->addAction(mOctahedralExtrudeAction);     
	toolBar->addAction(mStellateExtrudeAction);
	toolBar->addAction(mDoubleStellateExtrudeAction);
	
	stackedWidget->addWidget(mDooSabinExtrudeWidget);
	stackedWidget->addWidget(mCubicalExtrudeWidget);
	stackedWidget->addWidget(mDodecahedralExtrudeWidget);   
	stackedWidget->addWidget(mIcosahedralExtrudeWidget);   	
	stackedWidget->addWidget(mOctahedralExtrudeWidget);     
	stackedWidget->addWidget(mStellateExtrudeWidget);
	stackedWidget->addWidget(mDoubleStellateExtrudeWidget);
	
}

void ExtrusionMode::triggerDooSabinExtrude(){
	
	((MainWindow*)mParent)->setToolOptions(mDooSabinExtrudeWidget);
	((MainWindow*)mParent)->setMode(MainWindow::ExtrudeFaceDS);
	((MainWindow*)mParent)->setSpinBoxes(dooSabinLengthSpinBox,dooSabinRotationSpinBox,dooSabinScaleSpinBox,dooSabinSegmentsSpinBox);
}

void ExtrusionMode::triggerCubicalExtrude(){

	((MainWindow*)mParent)->setToolOptions(mCubicalExtrudeWidget);
	((MainWindow*)mParent)->setMode(MainWindow::ExtrudeMultipleFaces);	
	((MainWindow*)mParent)->setSpinBoxes(cubicalLengthSpinBox,cubicalRotationSpinBox,cubicalScaleSpinBox,cubicalSegmentsSpinBox);
}

void ExtrusionMode::triggerDodecahedralExtrude(){
	
	((MainWindow*)mParent)->setToolOptions(mDodecahedralExtrudeWidget);
	((MainWindow*)mParent)->setMode(MainWindow::ExtrudeFaceDodeca);
	((MainWindow*)mParent)->setSpinBoxes(dodecahedralLengthSpinBox,dodecahedralScaleSpinBox,dodecahedralSegmentsSpinBox);
}

void ExtrusionMode::triggerIcosahedralExtrude(){
	
	((MainWindow*)mParent)->setToolOptions(mIcosahedralExtrudeWidget);
	((MainWindow*)mParent)->setMode(MainWindow::ExtrudeFaceIcosa);
	((MainWindow*)mParent)->setSpinBoxes(icosahedralLengthSpinBox,icosahedralScaleSpinBox,icosahedralSegmentsSpinBox);
}

void ExtrusionMode::triggerOctahedralExtrude(){
	
	((MainWindow*)mParent)->setToolOptions(mOctahedralExtrudeWidget);
	((MainWindow*)mParent)->setMode(MainWindow::ExtrudeDualFace);
	((MainWindow*)mParent)->setSpinBoxes(octahedralLengthSpinBox,octahedralRotationSpinBox,octahedralScaleSpinBox,octahedralSegmentsSpinBox);
}

void ExtrusionMode::triggerStellateExtrude(){
	
	((MainWindow*)mParent)->setToolOptions(mStellateExtrudeWidget);
	((MainWindow*)mParent)->setMode(MainWindow::StellateFace);
	((MainWindow*)mParent)->setSpinBoxes(stellateLengthSpinBox);
}

void ExtrusionMode::triggerDoubleStellateExtrude(){
	
	((MainWindow*)mParent)->setToolOptions(mDoubleStellateExtrudeWidget);
	((MainWindow*)mParent)->setMode(MainWindow::DoubleStellateFace);
	((MainWindow*)mParent)->setSpinBoxes(doubleStellateLengthSpinBox);
}

void ExtrusionMode::setLength(double value){
	dooSabinLengthSpinBox->setValue(value);
	cubicalLengthSpinBox->setValue(value);
	dodecahedralLengthSpinBox->setValue(value);
	icosahedralLengthSpinBox->setValue(value);
	octahedralLengthSpinBox->setValue(value);
	stellateLengthSpinBox->setValue(value);
	doubleStellateLengthSpinBox->setValue(value);
	
	((MainWindow*)mParent)->changeExtrudeLength(value);
}
void ExtrusionMode::setRotation(double value){
	dooSabinRotationSpinBox->setValue(value);
	cubicalRotationSpinBox->setValue(value);
	octahedralRotationSpinBox->setValue(value);
	
	((MainWindow*)mParent)->changeExtrudeRotation((int)value);
}

void ExtrusionMode::setSegments(double value){
	dooSabinSegmentsSpinBox->setValue(value);
	cubicalSegmentsSpinBox->setValue(value);
	dodecahedralSegmentsSpinBox->setValue(value);
	icosahedralSegmentsSpinBox->setValue(value);
	octahedralSegmentsSpinBox->setValue(value);

	((MainWindow*)mParent)->changeNumExtrusions((int)value);
}

void ExtrusionMode::setScale(double value){
	dooSabinScaleSpinBox->setValue(value);
	cubicalScaleSpinBox->setValue(value);
	dodecahedralScaleSpinBox->setValue(value);
	icosahedralScaleSpinBox->setValue(value);
	octahedralScaleSpinBox->setValue(value);
		
	((MainWindow*)mParent)->changeExtrudeScale(value);
}

QDoubleSpinBox *ExtrusionMode::createDoubleSpinBox(QGridLayout *layout, QLabel *label, QString s, double low, double high, double step, double value, double decimals, int row, int col){
	label = new QLabel(s);
	QDoubleSpinBox *spinbox = new QDoubleSpinBox;
	spinbox->setRange(low, high);
	spinbox->setSingleStep(step);
	spinbox->setValue(value);
	spinbox->setDecimals(decimals);
	spinbox->setMaximumSize(75,25);
	layout->addWidget(label,row,col);
  layout->addWidget(spinbox,row,col+1);

	return spinbox;
}

void ExtrusionMode::setupDooSabinExtrude(){
	
	mDooSabinExtrudeLayout = new QGridLayout;
	mDooSabinExtrudeLayout->setVerticalSpacing(1);
	mDooSabinExtrudeLayout->setHorizontalSpacing(1);
	// mDooSabinExtrudeLayout->setMargin(0);
	//length
	dooSabinLengthSpinBox = createDoubleSpinBox(mDooSabinExtrudeLayout, dooSabinLengthLabel, tr("Length"), -100.0, 100.0, 0.1, 2.0, 1, 0,0);
	connect(dooSabinLengthSpinBox, SIGNAL(valueChanged(double)),this, SLOT(setLength(double)));
	//rotation
	dooSabinRotationSpinBox = createDoubleSpinBox(mDooSabinExtrudeLayout, dooSabinRotationLabel, tr("Rotation"), 0, 360, 1, 0.0, 1, 1,0);
	connect(dooSabinRotationSpinBox, SIGNAL(valueChanged(double)),this, SLOT(setRotation(double)));
	//scale
	dooSabinScaleSpinBox = createDoubleSpinBox(mDooSabinExtrudeLayout, dooSabinScaleLabel, tr("Scale"), 0.0, 10.0, 0.01, 1.0, 2, 2,0);
	connect(dooSabinScaleSpinBox, SIGNAL(valueChanged(double)),this, SLOT(setScale(double)));
	//segments
	dooSabinSegmentsSpinBox = createDoubleSpinBox(mDooSabinExtrudeLayout, dooSabinSegmentsLabel, tr("Segments"), 1, 50, 1, 1, 0, 3,0);
	connect(dooSabinSegmentsSpinBox, SIGNAL(valueChanged(double)),this, SLOT(setSegments(double)));

	mDooSabinExtrudeLayout->setRowStretch(4,1);
	mDooSabinExtrudeLayout->setColumnStretch(2,1);
	mDooSabinExtrudeWidget->setWindowTitle(tr("Doo Sabin Extrusion"));
	mDooSabinExtrudeWidget->setLayout(mDooSabinExtrudeLayout);	
}

void ExtrusionMode::setupCubicalExtrude(){	
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

	mCubicalExtrudeLayout->setRowStretch(4,1);
	mCubicalExtrudeLayout->setColumnStretch(2,1);
	mCubicalExtrudeWidget->setWindowTitle(tr("Cubical Extrusion"));
	mCubicalExtrudeWidget->setLayout(mCubicalExtrudeLayout);	
}

void ExtrusionMode::setupDodecahedralExtrude(){
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

	mDodecahedralExtrudeLayout->setRowStretch(4,1);
	mDodecahedralExtrudeLayout->setColumnStretch(2,1);
	mDodecahedralExtrudeWidget->setWindowTitle(tr("Dodecahedral Extrusion"));
	mDodecahedralExtrudeWidget->setLayout(mDodecahedralExtrudeLayout);	
}

void ExtrusionMode::setupIcosahedralExtrude(){
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

	mIcosahedralExtrudeLayout->setRowStretch(3,1);
	mIcosahedralExtrudeLayout->setColumnStretch(2,1);
	mIcosahedralExtrudeWidget->setWindowTitle(tr("Icosahedral Extrusion"));
	mIcosahedralExtrudeWidget->setLayout(mIcosahedralExtrudeLayout);
}

void ExtrusionMode::setupOctahedralExtrude(){
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
	
	mOctahedralExtrudeLayout->setRowStretch(5,1);
	mOctahedralExtrudeLayout->setColumnStretch(2,1);
	mOctahedralExtrudeWidget->setWindowTitle(tr("Octahedral Extrusion"));
	mOctahedralExtrudeWidget->setLayout(mOctahedralExtrudeLayout);
}

void ExtrusionMode::setupStellateExtrude(){
	mStellateExtrudeLayout = new QGridLayout;
	mStellateExtrudeLayout->setVerticalSpacing(1);
	mStellateExtrudeLayout->setHorizontalSpacing(1);
	// mStellateExtrudeLayout->setMargin(0);
	//length
	stellateLengthSpinBox = createDoubleSpinBox(mStellateExtrudeLayout, stellateLengthLabel, tr("Length"), -100.0, 100.0, 0.1, 2.0, 1, 0,0);
	connect(stellateLengthSpinBox, SIGNAL(valueChanged(double)),this, SLOT(setLength(double)));
	
	mStellateExtrudeLayout->setRowStretch(1,1);
	mStellateExtrudeLayout->setColumnStretch(2,1);
	mStellateExtrudeWidget->setWindowTitle(tr("Stellate Extrusion"));
	mStellateExtrudeWidget->setLayout(mStellateExtrudeLayout);	
}

void ExtrusionMode::setupDoubleStellateExtrude(){
	mDoubleStellateExtrudeLayout = new QGridLayout;
	mDoubleStellateExtrudeLayout->setVerticalSpacing(1);
	mDoubleStellateExtrudeLayout->setHorizontalSpacing(1);
	// mDoubleStellateExtrudeLayout->setMargin(0);
	//length
	doubleStellateLengthSpinBox = createDoubleSpinBox(mDoubleStellateExtrudeLayout, doubleStellateLengthLabel, tr("Length"), -100.0, 100.0, 0.1, 2.0, 1, 0,0);
	connect(doubleStellateLengthSpinBox, SIGNAL(valueChanged(double)),this, SLOT(setLength(double)));
	
	mDoubleStellateExtrudeLayout->setRowStretch(1,1);
	mDoubleStellateExtrudeLayout->setColumnStretch(2,1);
	mDoubleStellateExtrudeWidget->setWindowTitle(tr("Double Stellate Extrusion"));
	mDoubleStellateExtrudeWidget->setLayout(mDoubleStellateExtrudeLayout);	
}
