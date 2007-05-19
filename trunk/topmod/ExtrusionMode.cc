/****************************************************************************
 **
 **
 ****************************************************************************/
#include <QtGui>

#include "ExtrusionMode.hh"

ExtrusionMode::ExtrusionMode(QWidget *parent, QShortcutManager *sm)
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
	
	mCubicalExtrudeAction = new QAction(QIcon(":images/extrude_cubical.png"),tr("Cubical"),this);
	mCubicalExtrudeAction->setCheckable(true);
	sm->registerAction(mCubicalExtrudeAction, "Extrusion Operations", "");
	mCubicalExtrudeAction->setStatusTip(tr("Enter Cubical Extrude Mode"));
	mCubicalExtrudeAction->setToolTip(tr("Cubical Extrude Mode"));
	connect(mCubicalExtrudeAction, SIGNAL(triggered()), this, SLOT(triggerCubicalExtrude()));
	
	mDodecahedralExtrudeAction = new QAction(QIcon(":images/extrude_dodecahedral.png"),tr("Dodecahedral"),this);
	mDodecahedralExtrudeAction->setCheckable(true);
	mDodecahedralExtrudeAction->setEnabled(true);
	sm->registerAction(mDodecahedralExtrudeAction, "Extrusion Operations", "");
	mDodecahedralExtrudeAction->setStatusTip(tr("Enter Dodecahedral Extrude Mode"));
	mDodecahedralExtrudeAction->setToolTip(tr("Dodecahedral Extrude Mode"));
	connect(mDodecahedralExtrudeAction, SIGNAL(triggered()), this, SLOT(triggerDodecahedralExtrude()));
	
	mIcosahedralExtrudeAction = new QAction(QIcon(":images/extrude_icosahedral.png"),tr("Icosahedral"),this);
	mIcosahedralExtrudeAction->setCheckable(true);
	mIcosahedralExtrudeAction->setEnabled(true);
	sm->registerAction(mIcosahedralExtrudeAction, "Extrusion Operations", "");
	mIcosahedralExtrudeAction->setStatusTip(tr("Enter Icosahedral Extrude Mode"));
	mIcosahedralExtrudeAction->setToolTip(tr("Icosahedral Extrude Mode"));
	connect(mIcosahedralExtrudeAction, SIGNAL(triggered()), this, SLOT(triggerIcosahedralExtrude()));
	
	mOctahedralExtrudeAction = new QAction(QIcon(":images/extrude_octahedral.png"),tr("Octahedral"),this);
	mOctahedralExtrudeAction->setCheckable(true);
	mOctahedralExtrudeAction->setEnabled(true);
	sm->registerAction(mOctahedralExtrudeAction, "Extrusion Operations", "");
	mOctahedralExtrudeAction->setStatusTip(tr("Enter Octahedral Extrude Mode"));
	mOctahedralExtrudeAction->setToolTip(tr("Octahedral Extrude Mode"));
	connect(mOctahedralExtrudeAction, SIGNAL(triggered()), this, SLOT(triggerOctahedralExtrude()));
	
	mStellateExtrudeAction = new QAction(QIcon(":images/extrude_stellate.png"),tr("Stellate"),this);
	mStellateExtrudeAction->setCheckable(true);
	sm->registerAction(mStellateExtrudeAction, "Extrusion Operations", "");
	mStellateExtrudeAction->setStatusTip(tr("Enter Stellate Extrude Mode"));
	mStellateExtrudeAction->setToolTip(tr("Stellate Extrude Mode"));
	connect(mStellateExtrudeAction, SIGNAL(triggered()), this, SLOT(triggerStellateExtrude()));
	
	mDoubleStellateExtrudeAction = new QAction(QIcon(":images/extrude_doublestellate.png"),tr("Double Stellate"),this);
	mDoubleStellateExtrudeAction->setCheckable(true);
	sm->registerAction(mDoubleStellateExtrudeAction, "Extrusion Operations", "");
	mDoubleStellateExtrudeAction->setStatusTip(tr("Enter Double Stellate Mode"));
	mDoubleStellateExtrudeAction->setToolTip(tr("Double Stellate Mode"));
	connect(mDoubleStellateExtrudeAction, SIGNAL(triggered()), this, SLOT(triggerDoubleStellateExtrude()));

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
	((MainWindow*)mParent)->setMode(DLFLWindow::ExtrudeFaceDS);
}

void ExtrusionMode::triggerCubicalExtrude(){

	((MainWindow*)mParent)->setToolOptions(mCubicalExtrudeWidget);
	((MainWindow*)mParent)->setMode(DLFLWindow::ExtrudeFace);	
}

void ExtrusionMode::triggerDodecahedralExtrude(){
	
	((MainWindow*)mParent)->setToolOptions(mDodecahedralExtrudeWidget);
	((MainWindow*)mParent)->setMode(DLFLWindow::ExtrudeFaceDodeca);
}

void ExtrusionMode::triggerIcosahedralExtrude(){
	
	((MainWindow*)mParent)->setToolOptions(mIcosahedralExtrudeWidget);
	((MainWindow*)mParent)->setMode(DLFLWindow::ExtrudeFaceIcosa);
}

void ExtrusionMode::triggerOctahedralExtrude(){
	
	((MainWindow*)mParent)->setToolOptions(mOctahedralExtrudeWidget);
	((MainWindow*)mParent)->setMode(DLFLWindow::ExtrudeDualFace);
}

void ExtrusionMode::triggerStellateExtrude(){
	
	((MainWindow*)mParent)->setToolOptions(mStellateExtrudeWidget);
	((MainWindow*)mParent)->setMode(DLFLWindow::StellateFace);
}

void ExtrusionMode::triggerDoubleStellateExtrude(){
	
	((MainWindow*)mParent)->setToolOptions(mDoubleStellateExtrudeWidget);
	((MainWindow*)mParent)->setMode(DLFLWindow::DoubleStellateFace);
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
void ExtrusionMode::setRotation(int value){
	dooSabinRotationSpinBox->setValue(value);
	cubicalRotationSpinBox->setValue(value);
	octahedralRotationSpinBox->setValue(value);
	
	((MainWindow*)mParent)->changeExtrudeRotation(value);
}

void ExtrusionMode::setSegments(int value){
	dooSabinSegmentsSpinBox->setValue(value);
	cubicalSegmentsSpinBox->setValue(value);
	dodecahedralSegmentsSpinBox->setValue(value);
	icosahedralSegmentsSpinBox->setValue(value);
	octahedralSegmentsSpinBox->setValue(value);

	((MainWindow*)mParent)->changeNumExtrusions(value);
}

void ExtrusionMode::setScale(double value){
	dooSabinScaleSpinBox->setValue(value);
	cubicalScaleSpinBox->setValue(value);
	dodecahedralScaleSpinBox->setValue(value);
	icosahedralScaleSpinBox->setValue(value);
	octahedralScaleSpinBox->setValue(value);
		
	((MainWindow*)mParent)->changeExtrudeScale(value);
}

void ExtrusionMode::setupDooSabinExtrude(){
	
	mDooSabinExtrudeLayout = new QBoxLayout(QBoxLayout::LeftToRight);
	mDooSabinExtrudeLayout->setMargin(0);
	
	//length
	dooSabinLengthLabel = new QLabel(tr("Length"));
	dooSabinLengthSpinBox = new QDoubleSpinBox;
	dooSabinLengthSpinBox->setRange(-100.0, 100.0);
	dooSabinLengthSpinBox->setSingleStep(0.1);
	dooSabinLengthSpinBox->setValue(2.0);
	dooSabinLengthSpinBox->setDecimals(1);
	dooSabinLengthSpinBox->setMaximumSize(75,25);
	connect(dooSabinLengthSpinBox, SIGNAL(valueChanged(double)),
          this, SLOT(setLength(double)));

	mDooSabinExtrudeLayout->addWidget(dooSabinLengthLabel);
  mDooSabinExtrudeLayout->addWidget(dooSabinLengthSpinBox);

	//rotation
	dooSabinRotationLabel = new QLabel(tr("Rotation"));
	dooSabinRotationSpinBox = new QSpinBox;
	dooSabinRotationSpinBox->setRange(0, 360);
	dooSabinRotationSpinBox->setSingleStep(1);
	dooSabinRotationSpinBox->setValue(0.0);
	dooSabinRotationSpinBox->setMaximumSize(75,25);
	connect(dooSabinRotationSpinBox, SIGNAL(valueChanged(int)),
          this, SLOT(setRotation(int)));
	
	mDooSabinExtrudeLayout->addWidget(dooSabinRotationLabel);
  mDooSabinExtrudeLayout->addWidget(dooSabinRotationSpinBox);

	//scale
	dooSabinScaleLabel = new QLabel(tr("Scale"));
	dooSabinScaleSpinBox = new QDoubleSpinBox;
	dooSabinScaleSpinBox->setRange(0.0, 10.0);
	dooSabinScaleSpinBox->setSingleStep(0.01);
	dooSabinScaleSpinBox->setValue(1.0);
	dooSabinScaleSpinBox->setDecimals(2);
	dooSabinScaleSpinBox->setMaximumSize(75,25);
	connect(dooSabinScaleSpinBox, SIGNAL(valueChanged(double)),
          this, SLOT(setScale(double)));
	
	mDooSabinExtrudeLayout->addWidget(dooSabinScaleLabel);
  mDooSabinExtrudeLayout->addWidget(dooSabinScaleSpinBox);

	//segments
	dooSabinSegmentsLabel = new QLabel(tr("Segments"));
	dooSabinSegmentsSpinBox = new QSpinBox;
	dooSabinSegmentsSpinBox->setRange(1, 50);
	dooSabinSegmentsSpinBox->setSingleStep(1);
	dooSabinSegmentsSpinBox->setValue(1);
	dooSabinSegmentsSpinBox->setMaximumSize(75,25);
	connect(dooSabinSegmentsSpinBox, SIGNAL(valueChanged(int)),
          this, SLOT(setSegments(int)));	
	
	mDooSabinExtrudeLayout->addWidget(dooSabinSegmentsLabel);
  mDooSabinExtrudeLayout->addWidget(dooSabinSegmentsSpinBox);

	// //bendT
	// QLabel *dooSabinBendTLabel = new QLabel(tr("Bend T"));
	// QDoubleSpinBox *dooSabinBendTSpinBox = new QDoubleSpinBox;
	// dooSabinBendTSpinBox->setRange(-100.0, 100.0);
	// dooSabinBendTSpinBox->setSingleStep(0.1);
	// dooSabinBendTSpinBox->setValue(0.0);
	// dooSabinBendTSpinBox->setDecimals(1);
	// dooSabinBendTSpinBox->setMaximumSize(75,25);
	// mDooSabinExtrudeLayout->addWidget(dooSabinBendTLabel);
	//   mDooSabinExtrudeLayout->addWidget(dooSabinBendTSpinBox);

	// //bendB
	// QLabel *dooSabinBendBLabel = new QLabel(tr("Bend B"));
	// QDoubleSpinBox *dooSabinBendBSpinBox = new QDoubleSpinBox;
	// dooSabinBendBSpinBox->setRange(-100.0, 100.0);
	// dooSabinBendBSpinBox->setSingleStep(0.1);
	// dooSabinBendBSpinBox->setValue(0.0);
	// dooSabinBendBSpinBox->setDecimals(1);
	// dooSabinBendBSpinBox->setMaximumSize(75,25);
	// mDooSabinExtrudeLayout->addWidget(dooSabinBendBLabel);
	//   mDooSabinExtrudeLayout->addWidget(dooSabinBendBSpinBox);
 	
	mDooSabinExtrudeLayout->addStretch(1);
	mDooSabinExtrudeWidget->setWindowTitle(tr("Doo Sabin Extrusion"));
	mDooSabinExtrudeWidget->setLayout(mDooSabinExtrudeLayout);	
	
}

void ExtrusionMode::setupCubicalExtrude(){
	
	mCubicalExtrudeLayout = new QBoxLayout(QBoxLayout::LeftToRight);
	mCubicalExtrudeLayout->setMargin(0);
	
	//length
	cubicalLengthLabel = new QLabel(tr("Length"));
	cubicalLengthSpinBox = new QDoubleSpinBox;
	cubicalLengthSpinBox->setRange(-100.0, 100.0);
	cubicalLengthSpinBox->setSingleStep(0.1);
	cubicalLengthSpinBox->setValue(2.0);
	cubicalLengthSpinBox->setDecimals(1);
	cubicalLengthSpinBox->setMaximumSize(75,25);
	connect(cubicalLengthSpinBox, SIGNAL(valueChanged(double)),
          this, SLOT(setLength(double)));

	mCubicalExtrudeLayout->addWidget(cubicalLengthLabel);
  mCubicalExtrudeLayout->addWidget(cubicalLengthSpinBox);

	//rotation
	cubicalRotationLabel = new QLabel(tr("Rotation"));
	cubicalRotationSpinBox = new QSpinBox;
	cubicalRotationSpinBox->setRange(0, 360);
	cubicalRotationSpinBox->setSingleStep(1);
	cubicalRotationSpinBox->setValue(0.0);
	cubicalRotationSpinBox->setMaximumSize(75,25);
	connect(cubicalRotationSpinBox, SIGNAL(valueChanged(int)),
          this, SLOT(setRotation(int)));

	mCubicalExtrudeLayout->addWidget(cubicalRotationLabel);
  mCubicalExtrudeLayout->addWidget(cubicalRotationSpinBox);

	//scale
	cubicalScaleLabel = new QLabel(tr("Scale"));
	cubicalScaleSpinBox = new QDoubleSpinBox;
	cubicalScaleSpinBox->setRange(0.0, 10.0);
	cubicalScaleSpinBox->setSingleStep(0.01);
	cubicalScaleSpinBox->setValue(1.0);
	cubicalScaleSpinBox->setDecimals(2);
	cubicalScaleSpinBox->setMaximumSize(75,25);
	connect(cubicalScaleSpinBox, SIGNAL(valueChanged(double)),
          this, SLOT(setScale(double)));
	
	mCubicalExtrudeLayout->addWidget(cubicalScaleLabel);
  mCubicalExtrudeLayout->addWidget(cubicalScaleSpinBox);

	//segments
	cubicalSegmentsLabel = new QLabel(tr("Segments"));
	cubicalSegmentsSpinBox = new QSpinBox;
	cubicalSegmentsSpinBox->setRange(1, 50);
	cubicalSegmentsSpinBox->setSingleStep(1);
	cubicalSegmentsSpinBox->setValue(1);
	cubicalSegmentsSpinBox->setMaximumSize(75,25);
	connect(cubicalSegmentsSpinBox, SIGNAL(valueChanged(int)),
          this, SLOT(setSegments(int)));

	mCubicalExtrudeLayout->addWidget(cubicalSegmentsLabel);
  mCubicalExtrudeLayout->addWidget(cubicalSegmentsSpinBox);

	// //bendT
	// QLabel *cubicalBendTLabel = new QLabel(tr("Bend T"));
	// QDoubleSpinBox *cubicalBendTSpinBox = new QDoubleSpinBox;
	// cubicalBendTSpinBox->setRange(-100.0, 100.0);
	// cubicalBendTSpinBox->setSingleStep(0.1);
	// cubicalBendTSpinBox->setValue(0.0);
	// cubicalBendTSpinBox->setDecimals(1);
	// cubicalBendTSpinBox->setMaximumSize(75,25);
	// mCubicalExtrudeLayout->addWidget(cubicalBendTLabel);
	//   mCubicalExtrudeLayout->addWidget(cubicalBendTSpinBox);
	// 
	// //bendB
	// QLabel *cubicalBendBLabel = new QLabel(tr("Bend B"));
	// QDoubleSpinBox *cubicalBendBSpinBox = new QDoubleSpinBox;
	// cubicalBendBSpinBox->setRange(-100.0, 100.0);
	// cubicalBendBSpinBox->setSingleStep(0.1);
	// cubicalBendBSpinBox->setValue(0.0);
	// cubicalBendBSpinBox->setDecimals(1);
	// cubicalBendBSpinBox->setMaximumSize(75,25);
// /	mCubicalExtrudeLayout->addWidget(cubicalBendBLabel);
	//   mCubicalExtrudeLayout->addWidget(cubicalBendBSpinBox);
	
	mCubicalExtrudeLayout->addStretch(1);
	mCubicalExtrudeWidget->setWindowTitle(tr("Cubical Extrusion"));
	mCubicalExtrudeWidget->setLayout(mCubicalExtrudeLayout);
}

void ExtrusionMode::setupDodecahedralExtrude(){
	
	mDodecahedralExtrudeLayout = new QBoxLayout(QBoxLayout::LeftToRight);
	mDodecahedralExtrudeLayout->setMargin(0);

	//length
	dodecahedralLengthLabel = new QLabel(tr("Length:"));
	dodecahedralLengthSpinBox = new QDoubleSpinBox;
	dodecahedralLengthSpinBox->setRange(-100, 100);
	dodecahedralLengthSpinBox->setSingleStep(0.1);
	dodecahedralLengthSpinBox->setValue(2.0);
	dodecahedralLengthSpinBox->setDecimals(1);
	dodecahedralLengthSpinBox->setMaximumSize(75,25);
	connect(dodecahedralLengthSpinBox, SIGNAL(valueChanged(double)),
          this, SLOT(setLength(double)));
	
	mDodecahedralExtrudeLayout->addWidget(dodecahedralLengthLabel);
  mDodecahedralExtrudeLayout->addWidget(dodecahedralLengthSpinBox);

	//scale
	dodecahedralScaleLabel = new QLabel(tr("Scale:"));
	dodecahedralScaleSpinBox = new QDoubleSpinBox;
	dodecahedralScaleSpinBox->setRange(0, 10);
	dodecahedralScaleSpinBox->setSingleStep(.01);
	dodecahedralScaleSpinBox->setValue(1);
	dodecahedralScaleSpinBox->setMaximumSize(75,25);
	connect(dodecahedralScaleSpinBox, SIGNAL(valueChanged(double)),
          this, SLOT(setScale(double)));

	mDodecahedralExtrudeLayout->addWidget(dodecahedralScaleLabel);
  mDodecahedralExtrudeLayout->addWidget(dodecahedralScaleSpinBox);

	//segments
	dodecahedralSegmentsLabel = new QLabel(tr("Segments:"));
	dodecahedralSegmentsSpinBox = new QSpinBox;
	dodecahedralSegmentsSpinBox->setRange(1, 50);
	dodecahedralSegmentsSpinBox->setSingleStep(1);
	dodecahedralSegmentsSpinBox->setValue(1);
	dodecahedralSegmentsSpinBox->setMaximumSize(75,25);
	connect(dodecahedralSegmentsSpinBox, SIGNAL(valueChanged(int)),
          this, SLOT(setSegments(int)));

	mDodecahedralExtrudeLayout->addWidget(dodecahedralSegmentsLabel);
  mDodecahedralExtrudeLayout->addWidget(dodecahedralSegmentsSpinBox);
	
	//mesh flat edges checkbox
	hexagonalizeCheckBox = new QCheckBox(tr("Hexagonalize"),this);
	connect(hexagonalizeCheckBox, SIGNAL(stateChanged(int)),
          ((MainWindow*)mParent), SLOT(toggleHexagonalizeDodecaExtrudeFlag(int)));
	
	mDodecahedralExtrudeLayout->addWidget(hexagonalizeCheckBox);

	mDodecahedralExtrudeLayout->addStretch(1);
	mDodecahedralExtrudeWidget->setWindowTitle(tr("Dodecahedral Extrusion"));
	mDodecahedralExtrudeWidget->setLayout(mDodecahedralExtrudeLayout);	
}

void ExtrusionMode::setupIcosahedralExtrude(){
	
	mIcosahedralExtrudeLayout = new QBoxLayout(QBoxLayout::LeftToRight);
	mIcosahedralExtrudeLayout->setMargin(0);
	
	//length
	icosahedralLengthLabel = new QLabel(tr("Length:"));
	icosahedralLengthSpinBox = new QDoubleSpinBox;
	icosahedralLengthSpinBox->setRange(-100, 100);
	icosahedralLengthSpinBox->setSingleStep(0.1);
	icosahedralLengthSpinBox->setValue(1.0);
	icosahedralLengthSpinBox->setDecimals(1);
	icosahedralLengthSpinBox->setMaximumSize(75,25);
	connect(icosahedralLengthSpinBox, SIGNAL(valueChanged(double)),
          this, SLOT(setLength(double)));
	
	mIcosahedralExtrudeLayout->addWidget(icosahedralLengthLabel);
  mIcosahedralExtrudeLayout->addWidget(icosahedralLengthSpinBox);

	//scale
	icosahedralScaleLabel = new QLabel(tr("Scale:"));
	icosahedralScaleSpinBox = new QDoubleSpinBox;
	icosahedralScaleSpinBox->setRange(0, 10);
	icosahedralScaleSpinBox->setSingleStep(.01);
	icosahedralScaleSpinBox->setValue(1);
	icosahedralScaleSpinBox->setMaximumSize(75,25);
	connect(icosahedralScaleSpinBox, SIGNAL(valueChanged(double)),
          this, SLOT(setScale(double)));

	mIcosahedralExtrudeLayout->addWidget(icosahedralScaleLabel);
  mIcosahedralExtrudeLayout->addWidget(icosahedralScaleSpinBox);

	//segments
	icosahedralSegmentsLabel = new QLabel(tr("Segments:"));
	icosahedralSegmentsSpinBox = new QSpinBox;
	icosahedralSegmentsSpinBox->setRange(1, 50);
	icosahedralSegmentsSpinBox->setSingleStep(1);
	icosahedralSegmentsSpinBox->setValue(1);
	icosahedralSegmentsSpinBox->setMaximumSize(75,25);
	connect(icosahedralSegmentsSpinBox, SIGNAL(valueChanged(int)),
          this, SLOT(setSegments(int)));

	mIcosahedralExtrudeLayout->addWidget(icosahedralSegmentsLabel);
  mIcosahedralExtrudeLayout->addWidget(icosahedralSegmentsSpinBox);
	mIcosahedralExtrudeLayout->addStretch(1);
	mIcosahedralExtrudeWidget->setWindowTitle(tr("Icosahedral Extrusion"));
	mIcosahedralExtrudeWidget->setLayout(mIcosahedralExtrudeLayout);	
}

void ExtrusionMode::setupOctahedralExtrude(){
	
	mOctahedralExtrudeLayout = new QBoxLayout(QBoxLayout::LeftToRight);
	mOctahedralExtrudeLayout->setMargin(0);

	//length
	octahedralLengthLabel = new QLabel(tr("Length"));
	octahedralLengthSpinBox = new QDoubleSpinBox;
	octahedralLengthSpinBox->setRange(-100.0, 100.0);
	octahedralLengthSpinBox->setSingleStep(0.1);
	octahedralLengthSpinBox->setValue(1.0);
	octahedralLengthSpinBox->setDecimals(1);
	octahedralLengthSpinBox->setMaximumSize(75,25);
	connect(octahedralLengthSpinBox, SIGNAL(valueChanged(double)),
          this, SLOT(setLength(double)));

	mOctahedralExtrudeLayout->addWidget(octahedralLengthLabel);
  mOctahedralExtrudeLayout->addWidget(octahedralLengthSpinBox);

	//rotation
	octahedralRotationLabel = new QLabel(tr("Rotation"));
	octahedralRotationSpinBox = new QSpinBox;
	octahedralRotationSpinBox->setRange(0, 360);
	octahedralRotationSpinBox->setSingleStep(1);
	octahedralRotationSpinBox->setValue(0.0);
	octahedralRotationSpinBox->setMaximumSize(75,25);
	connect(octahedralRotationSpinBox, SIGNAL(valueChanged(int)),
          this, SLOT(setRotation(int)));

	mOctahedralExtrudeLayout->addWidget(octahedralRotationLabel);
  mOctahedralExtrudeLayout->addWidget(octahedralRotationSpinBox);

	//scale
	octahedralScaleLabel = new QLabel(tr("Scale"));
	octahedralScaleSpinBox = new QDoubleSpinBox;
	octahedralScaleSpinBox->setRange(0.0, 10.0);
	octahedralScaleSpinBox->setSingleStep(0.01);
	octahedralScaleSpinBox->setValue(1.0);
	octahedralScaleSpinBox->setDecimals(2);
	octahedralScaleSpinBox->setMaximumSize(75,25);
	connect(octahedralScaleSpinBox, SIGNAL(valueChanged(double)),
          this, SLOT(setScale(double)));

	mOctahedralExtrudeLayout->addWidget(octahedralScaleLabel);
  mOctahedralExtrudeLayout->addWidget(octahedralScaleSpinBox);

	//segments
	octahedralSegmentsLabel = new QLabel(tr("Segments"));
	octahedralSegmentsSpinBox = new QSpinBox;
	octahedralSegmentsSpinBox->setRange(1, 50);
	octahedralSegmentsSpinBox->setSingleStep(1);
	octahedralSegmentsSpinBox->setValue(1);
	octahedralSegmentsSpinBox->setMaximumSize(75,25);
	connect(octahedralSegmentsSpinBox, SIGNAL(valueChanged(int)),
          this, SLOT(setSegments(int)));

	mOctahedralExtrudeLayout->addWidget(octahedralSegmentsLabel);
  mOctahedralExtrudeLayout->addWidget(octahedralSegmentsSpinBox);
	
	//mesh flat edges checkbox
	meshFlatEdgesCheckBox = new QCheckBox(tr("Mesh Flat Edges"),this);
	connect(meshFlatEdgesCheckBox, SIGNAL(stateChanged(int)),
          ((MainWindow*)mParent), SLOT(toggleDualMeshEdgesFlag(int)));

	mOctahedralExtrudeLayout->addWidget(meshFlatEdgesCheckBox);

	mOctahedralExtrudeLayout->addStretch(1);
	mOctahedralExtrudeWidget->setWindowTitle(tr("Octahedral Extrusion"));
	mOctahedralExtrudeWidget->setLayout(mOctahedralExtrudeLayout);	
}

void ExtrusionMode::setupStellateExtrude(){
	
	mStellateExtrudeLayout = new QBoxLayout(QBoxLayout::LeftToRight);
	mStellateExtrudeLayout->setMargin(0);
	
	//length
	stellateLengthLabel = new QLabel(tr("Length"));
	stellateLengthSpinBox = new QDoubleSpinBox;
	stellateLengthSpinBox->setRange(-100.0, 100.0);
	stellateLengthSpinBox->setSingleStep(0.1);
	stellateLengthSpinBox->setValue(1.0);
	stellateLengthSpinBox->setDecimals(1);
	stellateLengthSpinBox->setMaximumSize(75,25);
	
	connect(stellateLengthSpinBox, SIGNAL(valueChanged(double)),
          this, SLOT(setLength(double)));

	mStellateExtrudeLayout->addWidget(stellateLengthLabel);
	mStellateExtrudeLayout->addWidget(stellateLengthSpinBox);
	mStellateExtrudeLayout->addStretch(1);	
	mStellateExtrudeWidget->setWindowTitle(tr("Stellate Extrusion"));
	mStellateExtrudeWidget->setLayout(mStellateExtrudeLayout);
}

void ExtrusionMode::setupDoubleStellateExtrude(){
	
	mDoubleStellateExtrudeLayout = new QBoxLayout(QBoxLayout::LeftToRight);
	mDoubleStellateExtrudeLayout->setMargin(0);
	//length
	doubleStellateLengthLabel = new QLabel(tr("Length"));
	doubleStellateLengthSpinBox = new QDoubleSpinBox;
	doubleStellateLengthSpinBox->setRange(-100.0, 100.0);
	doubleStellateLengthSpinBox->setSingleStep(0.1);
	doubleStellateLengthSpinBox->setValue(1.0);
	doubleStellateLengthSpinBox->setDecimals(1);
	doubleStellateLengthSpinBox->setMaximumSize(75,25);
	
	connect(doubleStellateLengthSpinBox, SIGNAL(valueChanged(double)),
          this, SLOT(setLength(double)));
	
	mDoubleStellateExtrudeLayout->addWidget(doubleStellateLengthLabel);
	mDoubleStellateExtrudeLayout->addWidget(doubleStellateLengthSpinBox);
	mDoubleStellateExtrudeLayout->addStretch(1);
	mDoubleStellateExtrudeWidget->setWindowTitle(tr("Double Stellate Extrusion"));
	mDoubleStellateExtrudeWidget->setLayout(mDoubleStellateExtrudeLayout);	
}
