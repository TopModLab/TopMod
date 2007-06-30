/****************************************************************************
 **
 **
 ****************************************************************************/
#include <QtGui>

#include "HighgenusMode.hh"

/*!
	\ingroup gui
	@{
	
	\class HighgenusMode
	\brief High Genus Operations GUI elements.
	
	\note 
	
	\see HighgenusMode
*/

/*!
* \brief Constructor
* 
* @param parent the MainWindow widget
* @param sm the shortcut manager class for adding a custom shortcut to each menu action or icon
* @param actionList the master list of actions for use with the CommandCompleter class
* 
*/
HighgenusMode::HighgenusMode(QWidget *parent, QShortcutManager *sm, QWidget *actionList)
		: QWidget(parent)
{		
	setParent(0);
	mParent = parent;
	
	mAddHoleHandleWidget = new QWidget;
  mAddHoleHandleCVWidget = new QWidget;
  mAddHandleSIWidget = new QWidget;
  mRindModelingScalingWidget = new QWidget;
  mRindModelingThicknessWidget = new QWidget;
  mWireframeModelingWidget = new QWidget;
  mWireframeModeling2Widget = new QWidget;
  mColumnModelingWidget = new QWidget;
	mSierpinskyWidget = new QWidget;
	mMultiFaceHandleWidget = new QWidget;                
	mMengerSpongeWidget = new QWidget;  

	setupAddHoleHandle();
	setupAddHoleHandleCV();
	setupAddHandleSI();
	setupRindModelingScaling();
	setupRindModelingThickness();
	setupWireframeModeling();
	setupWireframeModeling2();
	setupColumnModeling();
	setupSierpinsky();
	setupMultiFaceHandle();
	setupMengerSponge();
	
	//setup the 10 modes for high genus modeling
	mAddHoleHandleAction = new QAction(QIcon(":images/highgenus_addholehandle.png"),tr("Add Hole/Handle"),this);
	mAddHoleHandleAction->setCheckable(true);
	sm->registerAction(mAddHoleHandleAction, "High Genus Operations", "");
	mAddHoleHandleAction->setStatusTip(tr("Enter Add Hole/Handle Mode"));
	mAddHoleHandleAction->setToolTip(tr("Add Hole/Handle Mode"));
	connect(mAddHoleHandleAction, SIGNAL(triggered()), this, SLOT(triggerAddHoleHandle()));
	actionList->addAction(mAddHoleHandleAction);
	
	mAddHoleHandleCVAction = new QAction(QIcon(":images/highgenus_addholehandlecv.png"),tr("Add Hole/Handle CV"),this);
	mAddHoleHandleCVAction->setCheckable(true);
	sm->registerAction(mAddHoleHandleCVAction, "High Genus Operations", "");
	mAddHoleHandleCVAction->setStatusTip(tr("Enter Add Hole/Handle CV Mode"));
	mAddHoleHandleCVAction->setToolTip(tr("Add Hole/Handle CV Mode"));
	connect(mAddHoleHandleCVAction, SIGNAL(triggered()), this, SLOT(triggerAddHoleHandleCV()));
	actionList->addAction(mAddHoleHandleCVAction);

	mAddHandleSIAction = new QAction(QIcon(":images/highgenus_addhandlesi.png"),tr("Add Handle SI"),this);
	mAddHandleSIAction->setCheckable(true);
	sm->registerAction(mAddHandleSIAction, "High Genus Operations", "");
	mAddHandleSIAction->setStatusTip(tr("Enter Add Handle (Shape Interpolation) Mode"));
	mAddHandleSIAction->setToolTip(tr("Add Handle (Shape Interpolation) Mode"));
	connect(mAddHandleSIAction, SIGNAL(triggered()), this, SLOT(triggerAddHandleSI()));
	actionList->addAction(mAddHandleSIAction);

	mRindModelingScalingAction = new QAction(QIcon(":images/highgenus_rind.png"),tr("Rind Modeling Scaling"),this);
	mRindModelingScalingAction->setCheckable(true);
	sm->registerAction(mRindModelingScalingAction, "High Genus Operations", "");
	mRindModelingScalingAction->setStatusTip(tr("Enter Rind Modeling Scaling Mode"));
	mRindModelingScalingAction->setToolTip(tr("Rind Modeling Scaling Mode"));
	connect(mRindModelingScalingAction, SIGNAL(triggered()), this, SLOT(triggerRindModelingScaling()));
	actionList->addAction(mRindModelingScalingAction);

	mRindModelingThicknessAction = new QAction(QIcon(":images/highgenus_rind.png"),tr("Rind Modeling Thickness"),this);
	mRindModelingThicknessAction->setCheckable(true);
	sm->registerAction(mRindModelingThicknessAction, "High Genus Operations", "");
	mRindModelingThicknessAction->setStatusTip(tr("Enter Rind Modeling Thickness Mode"));
	mRindModelingThicknessAction->setToolTip(tr("Rind Modeling Thickness Mode"));
	connect(mRindModelingThicknessAction, SIGNAL(triggered()), this, SLOT(triggerRindModelingThickness()));
	actionList->addAction(mRindModelingThicknessAction);

	mWireframeModelingAction = new QAction(QIcon(":images/highgenus_wireframe.png"),tr("Wireframe Modeling"),this);
	mWireframeModelingAction->setCheckable(true);
	sm->registerAction(mWireframeModelingAction, "High Genus Operations", "");
	mWireframeModelingAction->setStatusTip(tr("Enter Wireframe Modeling Mode"));
	mWireframeModelingAction->setToolTip(tr("Wireframe Modeling Mode"));
	connect(mWireframeModelingAction, SIGNAL(triggered()), this, SLOT(triggerWireframeModeling()));
	actionList->addAction(mWireframeModelingAction);

	mWireframeModeling2Action = new QAction(QIcon(":images/highgenus_wireframe.png"),tr("Wireframe Modeling *EXP!!!"),this);
	mWireframeModeling2Action->setCheckable(true);
	sm->registerAction(mWireframeModeling2Action, "High Genus Operations", "");
	mWireframeModeling2Action->setStatusTip(tr("Enter Wireframe Modeling Mode"));
	mWireframeModeling2Action->setToolTip(tr("Wireframe Modeling Mode"));
	connect(mWireframeModeling2Action, SIGNAL(triggered()), this, SLOT(triggerWireframeModeling2()));
	actionList->addAction(mWireframeModeling2Action);

	mColumnModelingAction = new QAction(QIcon(":images/highgenus_column.png"),tr("Column Modeling"),this);
	mColumnModelingAction->setCheckable(true);
	sm->registerAction(mColumnModelingAction, "High Genus Operations", "");
	mColumnModelingAction->setStatusTip(tr("Enter Column Modeling Mode"));
	mColumnModelingAction->setToolTip(tr("Column Modeling Mode"));
	connect(mColumnModelingAction, SIGNAL(triggered()), this, SLOT(triggerColumnModeling()));
	actionList->addAction(mColumnModelingAction);

	mSierpinskyAction = new QAction(QIcon(":images/highgenus_sierpinsky.png"),tr("Sierpinsky"),this);
	mSierpinskyAction->setCheckable(true);
	sm->registerAction(mSierpinskyAction, "High Genus Operations", "");
	mSierpinskyAction->setStatusTip(tr("Enter Sierpinsky Mode"));
	mSierpinskyAction->setToolTip(tr("Sierpinsky Mode"));
	connect(mSierpinskyAction, SIGNAL(triggered()), this, SLOT(triggerSierpinsky()));
	actionList->addAction(mSierpinskyAction);

	mMultiFaceHandleAction = new QAction(QIcon(":images/highgenus_multifacehandle.png"),tr("Multi-face Handle"),this);
	mMultiFaceHandleAction->setCheckable(true);
	sm->registerAction(mMultiFaceHandleAction, "High Genus Operations", "");
	mMultiFaceHandleAction->setStatusTip(tr("Enter Multi-face Handle Mode"));
	mMultiFaceHandleAction->setToolTip(tr("Multi-face Handle Mode"));
	connect(mMultiFaceHandleAction, SIGNAL(triggered()), this, SLOT(triggerMultiFaceHandle()));
	actionList->addAction(mMultiFaceHandleAction);

	mMengerSpongeAction = new QAction(QIcon(":images/highgenus_mengersponge.png"),tr("Menger Sponge"),this);
	mMengerSpongeAction->setCheckable(true);
	sm->registerAction(mMengerSpongeAction, "High Genus Operations", "");
	mMengerSpongeAction->setStatusTip(tr("Enter Menger Sponge Mode"));
	mMengerSpongeAction->setToolTip(tr("Menger Sponge Mode"));
	connect(mMengerSpongeAction, SIGNAL(triggered()), this, SLOT(triggerMengerSponge()));
	actionList->addAction(mMengerSpongeAction);
}

QMenu* HighgenusMode::getMenu(){
	mHighgenusMenu = new QMenu(tr("High Genus"));
	
	mHighgenusMenu->addAction(mAddHoleHandleAction);
	mHighgenusMenu->addAction(mAddHoleHandleCVAction);
	mHighgenusMenu->addAction(mAddHandleSIAction);
	mHighgenusMenu->addAction(mRindModelingScalingAction); 
	mHighgenusMenu->addAction(mRindModelingThicknessAction);
	mHighgenusMenu->addAction(mWireframeModelingAction); 
	mHighgenusMenu->addAction(mWireframeModeling2Action); 
	mHighgenusMenu->addAction(mColumnModelingAction);	
	mHighgenusMenu->addAction(mSierpinskyAction);
	mHighgenusMenu->addAction(mMultiFaceHandleAction);
	mHighgenusMenu->addAction(mMengerSpongeAction);	
	
	return mHighgenusMenu;
	
}

void HighgenusMode::addActions(QActionGroup *actionGroup, QToolBar *toolBar, QStackedWidget *stackedWidget){
	
	actionGroup->addAction(mAddHoleHandleAction);
	actionGroup->addAction(mAddHoleHandleCVAction);
	actionGroup->addAction(mAddHandleSIAction);
	actionGroup->addAction(mRindModelingScalingAction); 
	actionGroup->addAction(mRindModelingThicknessAction);
	actionGroup->addAction(mWireframeModelingAction); 
	actionGroup->addAction(mWireframeModeling2Action); 
	actionGroup->addAction(mColumnModelingAction);	
	actionGroup->addAction(mSierpinskyAction);
	actionGroup->addAction(mMultiFaceHandleAction);
	actionGroup->addAction(mMengerSpongeAction);
	
	toolBar->addAction(mAddHoleHandleAction);
	toolBar->addAction(mAddHoleHandleCVAction);
	toolBar->addAction(mAddHandleSIAction);
	toolBar->addAction(mRindModelingScalingAction); 
	toolBar->addAction(mRindModelingThicknessAction);
	toolBar->addAction(mWireframeModelingAction); 
	toolBar->addAction(mWireframeModeling2Action); 
	toolBar->addAction(mColumnModelingAction);	
	toolBar->addAction(mSierpinskyAction);
	toolBar->addAction(mMultiFaceHandleAction);
	toolBar->addAction(mMengerSpongeAction);
	
	stackedWidget->addWidget(mAddHoleHandleWidget);
	stackedWidget->addWidget(mAddHoleHandleCVWidget);
	stackedWidget->addWidget(mAddHandleSIWidget);
	stackedWidget->addWidget(mRindModelingScalingWidget); 
	stackedWidget->addWidget(mRindModelingThicknessWidget);
	stackedWidget->addWidget(mWireframeModelingWidget); 
	stackedWidget->addWidget(mWireframeModeling2Widget); 
	stackedWidget->addWidget(mColumnModelingWidget);	
	stackedWidget->addWidget(mSierpinskyWidget);
	stackedWidget->addWidget(mMultiFaceHandleWidget);
	stackedWidget->addWidget(mMengerSpongeWidget);
	
}

void HighgenusMode::triggerAddHoleHandle(){
	
	((MainWindow*)mParent)->setToolOptions(mAddHoleHandleWidget);
	((MainWindow*)mParent)->setMode(MainWindow::ConnectFaceVertices);
}

void HighgenusMode::triggerAddHoleHandleCV(){

	((MainWindow*)mParent)->setToolOptions(mAddHoleHandleCVWidget);
	((MainWindow*)mParent)->setMode(MainWindow::ConnectFaces);	
}

void HighgenusMode::triggerAddHandleSI(){
	
	((MainWindow*)mParent)->setToolOptions(mAddHandleSIWidget);
	((MainWindow*)mParent)->setMode(MainWindow::HermiteConnectFaces);
}

void HighgenusMode::triggerRindModelingScaling(){
	
	((MainWindow*)mParent)->setToolOptions(mRindModelingScalingWidget);
	((MainWindow*)mParent)->setMode(MainWindow::SelectFaceLoop);
	// ((MainWindow*)mParent)->setMode(MainWindow::NormalMode);
}

void HighgenusMode::triggerRindModelingThickness(){
	
	((MainWindow*)mParent)->setToolOptions(mRindModelingThicknessWidget);
	((MainWindow*)mParent)->setMode(MainWindow::SelectFaceLoop);
}

void HighgenusMode::triggerWireframeModeling(){
	
	((MainWindow*)mParent)->setToolOptions(mWireframeModelingWidget);
	((MainWindow*)mParent)->setMode(MainWindow::NormalMode);
}

void HighgenusMode::triggerWireframeModeling2(){
	
	((MainWindow*)mParent)->setToolOptions(mWireframeModeling2Widget);
	((MainWindow*)mParent)->setMode(MainWindow::MultiSelectFace);
}

void HighgenusMode::triggerColumnModeling(){
	
	((MainWindow*)mParent)->setToolOptions(mColumnModelingWidget);
	((MainWindow*)mParent)->setMode(MainWindow::NormalMode);
}

void HighgenusMode::triggerSierpinsky(){
	
	((MainWindow*)mParent)->setToolOptions(mSierpinskyWidget);
	((MainWindow*)mParent)->setMode(MainWindow::NormalMode);
}

void HighgenusMode::triggerMultiFaceHandle(){
	
	((MainWindow*)mParent)->setToolOptions(mMultiFaceHandleWidget);
	((MainWindow*)mParent)->setMode(MainWindow::MultiSelectFace);
}

void HighgenusMode::triggerMengerSponge(){
	
	((MainWindow*)mParent)->setToolOptions(mMengerSpongeWidget);
	((MainWindow*)mParent)->setMode(MainWindow::NormalMode);
}

void HighgenusMode::toggleCrustCleanupFlag(int state)
{
	rindModelingThicknessCleanupCheckBox->setChecked(state);
	rindModelingScalingCleanupCheckBox->setChecked(state);
	
	((MainWindow*)mParent)->toggleCrustCleanupFlag(state);
}

QDoubleSpinBox *HighgenusMode::createDoubleSpinBox(QGridLayout *layout, QLabel *label, QString s, double low, double high, double step, double value, double decimals, int row, int col){
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


void HighgenusMode::setupAddHoleHandle(){
	
	mAddHoleHandleLayout = new QGridLayout;
	mAddHoleHandleLayout->setVerticalSpacing(1);
	mAddHoleHandleLayout->setHorizontalSpacing(1);
	// mAddHoleHandleLayout->setMargin(0);
	addHoleHandleNumSegmentsSpinBox = createDoubleSpinBox(mAddHoleHandleLayout, addHoleHandleNumSegmentsLabel, tr("# Segments"), 1, 100, 1, 10, 0, 0,0);
	connect(addHoleHandleNumSegmentsSpinBox, SIGNAL(valueChanged(double)), this, SLOT(numSegmentsValueChanged(double)));
	
	addHoleHandleNumSegmentsConnectSpinBox = createDoubleSpinBox(mAddHoleHandleLayout, addHoleHandleNumSegmentsConnectLabel, tr("# Segments to\nConnect (0=all)"), 0, 10, 1, 0, 0, 1,0);
	connect(addHoleHandleNumSegmentsConnectSpinBox, SIGNAL(valueChanged(double)), this, SLOT(numSegmentsConnectValueChanged(double)));
	
	mAddHoleHandleLayout->setRowStretch(2,1);
	mAddHoleHandleLayout->setColumnStretch(2,1);
	mAddHoleHandleWidget->setWindowTitle("Add Hole/Handle");
	mAddHoleHandleWidget->setLayout(mAddHoleHandleLayout);	
}

//slot for num segments to reset the max segments range
void HighgenusMode::numSegmentsValueChanged(double value){
	
	((MainWindow*)mParent)->changeNumSegments(value);
	
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
void HighgenusMode::numSegmentsConnectValueChanged(double value){
	
	((MainWindow*)mParent)->changeMaxSegments(value);
	
	addHoleHandleNumSegmentsConnectSpinBox->setValue(value);
	addHoleHandleCVNumSegmentsConnectSpinBox->setValue(value);
	addHandleSINumSegmentsConnectSpinBox->setValue(value);	
}

void HighgenusMode::setupAddHoleHandleCV(){
	
	mAddHoleHandleCVLayout = new QGridLayout;
	// mAddHoleHandleCVLayout->setMargin(0);
	mAddHoleHandleCVLayout->setVerticalSpacing(1);
	mAddHoleHandleCVLayout->setHorizontalSpacing(1);

	addHoleHandleCVNumSegmentsSpinBox = createDoubleSpinBox(mAddHoleHandleCVLayout, addHoleHandleCVNumSegmentsLabel, tr("# Segments"), 1, 100, 1, 10, 0, 0,0);
	connect(addHoleHandleCVNumSegmentsSpinBox, SIGNAL(valueChanged(double)), this, SLOT(numSegmentsValueChanged(double)));
	
	addHoleHandleCVNumSegmentsConnectSpinBox = createDoubleSpinBox(mAddHoleHandleCVLayout, addHoleHandleCVNumSegmentsConnectLabel, tr("# Segments to\nConnect (0=all)"), 0, 10, 1, 0, 0, 1,0);
	connect(addHoleHandleNumSegmentsConnectSpinBox, SIGNAL(valueChanged(double)), this, SLOT(numSegmentsConnectValueChanged(double)));
	
	mAddHoleHandleCVLayout->setRowStretch(4,1);
	mAddHoleHandleCVLayout->setColumnStretch(2,1);
	mAddHoleHandleCVWidget->setWindowTitle("Add Hole/Handle (Closest Vertex)");
	mAddHoleHandleCVWidget->setLayout(mAddHoleHandleCVLayout);	
}

void HighgenusMode::setupAddHandleSI(){
	
	mAddHandleSILayout = new QGridLayout;
	// mAddHandleSILayout->setMargin(0);
	mAddHandleSILayout->setVerticalSpacing(1);
	mAddHandleSILayout->setHorizontalSpacing(1);
	
	//number of segments
	addHandleSINumSegmentsSpinBox = createDoubleSpinBox(mAddHandleSILayout, addHandleSINumSegmentsLabel, tr("# Segments"), 1, 100, 1, 10, 0, 0,0);
	connect(addHandleSINumSegmentsSpinBox, SIGNAL(valueChanged(double)), this, SLOT(numSegmentsValueChanged(double)));
	//number of segments to connect
	addHandleSINumSegmentsConnectSpinBox = createDoubleSpinBox(mAddHandleSILayout, addHandleSINumSegmentsConnectLabel, tr("# Segments to\nConnect (0=all)"), 0, 10, 1, 0, 0, 1,0);
	connect(addHoleHandleNumSegmentsConnectSpinBox, SIGNAL(valueChanged(double)), this, SLOT(numSegmentsConnectValueChanged(double)));
	//weight 1
	addHandleSIWeight1SpinBox = createDoubleSpinBox(mAddHandleSILayout, addHandleSIWeight1Label, tr("Weight 1"), -100, 100, 0.1, 5.0, 1, 2,0);
	connect(addHandleSIWeight1SpinBox, SIGNAL(valueChanged(double)), ((MainWindow*)mParent), SLOT(changeWeight1(double)));
	//symmetric weights checkbox
	QCheckBox *addHandleSISymmetricWeightsCheckBox = new QCheckBox(tr("Symmetric Weights"),this);
	addHandleSISymmetricWeightsCheckBox->setChecked(Qt::Checked);
	connect(addHandleSISymmetricWeightsCheckBox, SIGNAL(stateChanged(int)), this, SLOT(toggleSymmetricWeightsFlag(int)));
	mAddHandleSILayout->addWidget(addHandleSISymmetricWeightsCheckBox,3,0,1,2);
	//weight 2 - must be disabled when the checkbox is unchecked...
	addHandleSIWeight2SpinBox = createDoubleSpinBox(mAddHandleSILayout, addHandleSIWeight2Label, tr("Weight 1"), -100, 100, 0.1, 5.0, 1, 4,0);	
	addHandleSIWeight2SpinBox->setEnabled(false);
	connect(addHandleSIWeight2SpinBox, SIGNAL(valueChanged(double)), ((MainWindow*)mParent), SLOT(changeWeight2(double)));

	//add twists?
	addHandleSITwistsConnectSpinBox = createDoubleSpinBox(mAddHandleSILayout, addHandleSITwistsConnectLabel, tr("Extra Twists"), 0, 100, 1, 0, 0, 5,0);	
	connect(addHandleSITwistsConnectSpinBox, SIGNAL(valueChanged(double)), ((MainWindow*)mParent), SLOT(changeExtraTwists(double)));
	
	mAddHandleSILayout->setRowStretch(6,1);
	mAddHandleSILayout->setColumnStretch(2,1);
	mAddHandleSIWidget->setWindowTitle("Add Handle (Shape Interpolation)");
	mAddHandleSIWidget->setLayout(mAddHandleSILayout);	
	
}

void HighgenusMode::toggleSymmetricWeightsFlag(int state){
	
	((MainWindow*)mParent)->toggleSymmetricWeightsFlag(state);
	
	if (state) addHandleSIWeight2SpinBox->setEnabled(false);
	else addHandleSIWeight2SpinBox->setEnabled(true);
}

void HighgenusMode::setupRindModelingScaling(){
	
	mRindModelingScalingLayout = new QGridLayout;
	// mRindModelingScalingLayout->setMargin(0);
	mRindModelingScalingLayout->setVerticalSpacing(1);
	mRindModelingScalingLayout->setHorizontalSpacing(1);

	//scale factor - 0.00-10.00
	rindModelingScalingSpinBox = createDoubleSpinBox(mRindModelingScalingLayout, rindModelingScalingLabel, tr("Scale"), 0, 10.0, 0.01, 0.9, 2, 0,0);	
	connect(rindModelingScalingSpinBox, SIGNAL(valueChanged(double)), ((MainWindow*)mParent), SLOT(changeCrustScaleFactor(double)));
	//symmetric weights checkbox
	rindModelingScalingCleanupCheckBox = new QCheckBox(tr("Cleanup when peeling"),this);
	rindModelingScalingCleanupCheckBox->setChecked(Qt::Unchecked);
	connect(rindModelingScalingCleanupCheckBox, SIGNAL(stateChanged(int)), this, SLOT(toggleCrustCleanupFlag(int)));
	mRindModelingScalingLayout->addWidget(rindModelingScalingCleanupCheckBox,1,0,1,2);
	//create crust button
	QPushButton *rindModelingScalingCreateCrustButton = new QPushButton("Create Crust", this);
	connect(rindModelingScalingCreateCrustButton, SIGNAL(clicked()), ((MainWindow*)mParent), SLOT(crustModeling4())); // ((MainWindow*)mParent), SLOT(crust_modeling1())); //crust_modeling3
	mRindModelingScalingLayout->addWidget(rindModelingScalingCreateCrustButton,2,0,1,2);	
	//set layout and add stretch
	mRindModelingScalingLayout->setRowStretch(3,1);
	mRindModelingScalingLayout->setColumnStretch(2,1);
	mRindModelingScalingWidget->setWindowTitle("Rind Modeling (Scaling)");
	mRindModelingScalingWidget->setLayout(mRindModelingScalingLayout);	
}

void HighgenusMode::setupRindModelingThickness(){

	mRindModelingThicknessLayout = new QGridLayout;
	// mRindModelingThicknessLayout->setMargin(0);
	mRindModelingThicknessLayout->setVerticalSpacing(1);
	mRindModelingThicknessLayout->setHorizontalSpacing(1);
	
	//thickness
	rindModelingThicknessSpinBox = createDoubleSpinBox(mRindModelingThicknessLayout, rindModelingThicknessLabel, tr("Thickness:"), -1.0, 1.0, 0.01, 0.5, 2, 0,0);	
	connect(rindModelingThicknessSpinBox, SIGNAL(valueChanged(double)), ((MainWindow*)mParent), SLOT(changeCrustThickness(double)));
	//symmetric weights checkbox
	rindModelingThicknessCleanupCheckBox = new QCheckBox(tr("Cleanup when peeling"),this);
	rindModelingThicknessCleanupCheckBox->setChecked(Qt::Unchecked);
	connect(rindModelingThicknessCleanupCheckBox, SIGNAL(stateChanged(int)), this, SLOT(toggleCrustCleanupFlag(int)));
	mRindModelingThicknessLayout->addWidget(rindModelingThicknessCleanupCheckBox, 1,0,1,2);
	//create crust button
	QPushButton *rindModelingThicknessCreateButton = new QPushButton("Create Crust", this);
	connect(rindModelingThicknessCreateButton, SIGNAL(clicked()), ((MainWindow*)mParent), SLOT(crustModeling3()));
	mRindModelingThicknessLayout->addWidget(rindModelingThicknessCreateButton,2,0,1,2);	
	//set layout and add stretch
	mRindModelingThicknessLayout->setRowStretch(3,1);
	mRindModelingThicknessLayout->setColumnStretch(2,1);
	mRindModelingThicknessWidget->setWindowTitle("Rind Modeling (Thickness)");
	mRindModelingThicknessWidget->setLayout(mRindModelingThicknessLayout);
}

void HighgenusMode::setupWireframeModeling(){
	
	mWireframeModelingLayout = new QGridLayout;
	// mWireframeModelingLayout->setMargin(0);
	mWireframeModelingLayout->setVerticalSpacing(1);
	mWireframeModelingLayout->setHorizontalSpacing(1);
	//thickness
	wireframeModelingThicknessSpinBox = createDoubleSpinBox(mWireframeModelingLayout, wireframeModelingThicknessLabel, tr("Thickness:"), 0.0, 1.0, 0.01, 0.25, 2, 0,0);	
	connect(wireframeModelingThicknessSpinBox, SIGNAL(valueChanged(double)), ((MainWindow*)mParent), SLOT(changeWireframeThickness(double)));
	//create wireframe button
	QPushButton *wireframeModelingCreateButton = new QPushButton("Create Wireframe", this);
	connect(wireframeModelingCreateButton, SIGNAL(clicked()), ((MainWindow*)mParent), SLOT(makeWireframe()));
	mWireframeModelingLayout->addWidget(wireframeModelingCreateButton,1,0,1,2);	
	//set layout and add stretch
	mWireframeModelingLayout->setRowStretch(2,1);
	mWireframeModelingLayout->setColumnStretch(2,1);
	mWireframeModelingWidget->setWindowTitle("Wireframe Modeling");
	mWireframeModelingWidget->setLayout(mWireframeModelingLayout);
}

void HighgenusMode::setupWireframeModeling2(){
	
	mWireframeModeling2Layout = new QGridLayout;
	// mWireframeModeling2Layout->setMargin(0);
	mWireframeModeling2Layout->setVerticalSpacing(1);
	mWireframeModeling2Layout->setHorizontalSpacing(1);
	//thickness
	wireframeModeling2ThicknessSpinBox = createDoubleSpinBox(mWireframeModeling2Layout, wireframeModeling2ThicknessLabel, tr("Thickness:"), 0.0, 1.0, 0.01, 0.25, 2, 0,0);	
	connect(wireframeModeling2ThicknessSpinBox, SIGNAL(valueChanged(double)), ((MainWindow*)mParent), SLOT(changeWireframeThickness(double)));
	//create wireframe button
	QPushButton *wireframeModeling2CreateButton = new QPushButton("Create Wireframe", this);
	connect(wireframeModeling2CreateButton, SIGNAL(clicked()), ((MainWindow*)mParent), SLOT(makeWireframe2()));
	mWireframeModeling2Layout->addWidget(wireframeModeling2CreateButton,1,0,1,2);	
	//set layout and add stretch
	mWireframeModeling2Layout->setRowStretch(2,1);
	mWireframeModeling2Layout->setColumnStretch(2,1);
	mWireframeModeling2Widget->setWindowTitle("Wireframe Modeling ***EXP!!");
	mWireframeModeling2Widget->setLayout(mWireframeModeling2Layout);
}

void HighgenusMode::setupColumnModeling(){
	
	mColumnModelingLayout = new QGridLayout;	
	// mColumnModelingLayout->setMargin(0);
	mColumnModelingLayout->setVerticalSpacing(1);
	mColumnModelingLayout->setHorizontalSpacing(1);
	
	//thickness
	columnModelingThicknessSpinBox = createDoubleSpinBox(mColumnModelingLayout, columnModelingThicknessLabel, tr("Thickness:"), 0.0, 1.0, 0.01, 0.25, 2, 0,0);	
	connect(columnModelingThicknessSpinBox, SIGNAL(valueChanged(double)), ((MainWindow*)mParent), SLOT(changeColumnThickness(double)));
	//number of segments
	columnModelingNumSegmentsSpinBox = createDoubleSpinBox(mColumnModelingLayout, columnModelingNumSegmentsLabel, tr("# Segments:"), 4.0, 100, 1, 4, 0, 1,0);	
	connect(columnModelingNumSegmentsSpinBox, SIGNAL(valueChanged(double)), ((MainWindow*)mParent), SLOT(changeColumnSegments(double)));
	//create column button
	QPushButton *columnModelingCreateButton = new QPushButton("Create Columns", this);
	connect(columnModelingCreateButton, SIGNAL(clicked()), ((MainWindow*)mParent), SLOT(makeWireframeWithColumns()));
	mColumnModelingLayout->addWidget(columnModelingCreateButton,2,0,1,2);	
	//set layout and add stretch
	mColumnModelingLayout->setRowStretch(3,1);
	mColumnModelingLayout->setColumnStretch(2,1);
	mColumnModelingWidget->setWindowTitle("Column Modeling");
	mColumnModelingWidget->setLayout(mColumnModelingLayout);	
}

void HighgenusMode::setupSierpinsky(){
	
	mSierpinskyLayout = new QGridLayout;	
	// mSierpinskyLayout->setMargin(0);
	mSierpinskyLayout->setVerticalSpacing(1);
	mSierpinskyLayout->setHorizontalSpacing(1);	
	//create column button
	QPushButton *sierpinskyButton = new QPushButton("Create Sierpinsky\nTetrahedra", this);
	connect(sierpinskyButton, SIGNAL(clicked()), ((MainWindow*)mParent), SLOT(multiConnectMidpoints()));
	mSierpinskyLayout->addWidget(sierpinskyButton,0,0);
	
	mSierpinskyLayout->setRowStretch(1,1);
	mSierpinskyLayout->setColumnStretch(1,1);
	mSierpinskyWidget->setWindowTitle("Sierpinsky Tetrahedra");
	mSierpinskyWidget->setLayout(mSierpinskyLayout);
}

void HighgenusMode::setupMultiFaceHandle(){
	
	mMultiFaceHandleLayout = new QGridLayout;
	// mMultiFaceHandleLayout->setMargin(0);
	mMultiFaceHandleLayout->setVerticalSpacing(1);
	mMultiFaceHandleLayout->setHorizontalSpacing(1);
	
	QGroupBox *multiFaceAlgorithmGroupBox = new QGroupBox(tr("Use Convex Hull\nAlgorithm"));
	multiFaceAlgorithmGroupBox->setCheckable(true);
	multiFaceAlgorithmGroupBox->setChecked(false);
	
	mMultiFaceAlgorithmLayout = new QGridLayout;
	mMultiFaceAlgorithmLayout->setVerticalSpacing(2);
	mMultiFaceAlgorithmLayout->setHorizontalSpacing(2);

	multiFaceAlgorithmGroupBox->setAlignment(Qt::AlignLeft);

	connect(multiFaceAlgorithmGroupBox, SIGNAL(toggled(bool)),
	          this, SLOT(changeMultiFaceAlgorithm(bool)));
	
	//scaling
	multiFaceHandleScaleSpinBox = createDoubleSpinBox(mMultiFaceAlgorithmLayout, multiFaceHandleScaleLabel, tr("Scale Factor:"), 0.0, 5.0, 0.01, 1.0, 2, 0,0);	
	connect(multiFaceHandleScaleSpinBox, SIGNAL(valueChanged(double)), ((MainWindow*)mParent), SLOT(changeMultiFaceHandleScaleFactor(double)));
	//extrude distance
	multiFaceHandleExtrudeDistanceSpinBox = createDoubleSpinBox(mMultiFaceAlgorithmLayout, multiFaceHandleExtrudeDistanceLabel, tr("Extrude Dist.\nFactor:"), -2.0, 2.0, 0.1, 0.5, 1, 1,0);	
	connect(multiFaceHandleExtrudeDistanceSpinBox, SIGNAL(valueChanged(double)), ((MainWindow*)mParent), SLOT(changeMultiFaceHandleExtrudeDist(double)));	
	//use max. offsets
	multiFaceHandleMaxOffsetsCheckBox = new QCheckBox(tr("Use max offsets"),this);
	connect(multiFaceHandleMaxOffsetsCheckBox, SIGNAL(stateChanged(int)), this, SLOT(toggleMultiFaceHandleUseMaxOffsetFlag(int)));
	mMultiFaceAlgorithmLayout->addWidget(multiFaceHandleMaxOffsetsCheckBox,2,0,1,2);
	multiFaceAlgorithmGroupBox->setLayout(mMultiFaceAlgorithmLayout);
	
	//add the groupbox to the multiface layout
	mMultiFaceHandleLayout->addWidget(multiFaceAlgorithmGroupBox,0,0);
	QPushButton *multiFaceHandleButton = new QPushButton("Connect Selected Faces", this);
	connect(multiFaceHandleButton, SIGNAL(clicked()), ((MainWindow*)mParent), SLOT(createMultiFaceHandle()));
	mMultiFaceHandleLayout->addWidget(multiFaceHandleButton,1,0,1,2);
	
	mMultiFaceHandleLayout->setRowStretch(4,1);
	mMultiFaceHandleLayout->setColumnStretch(2,1);
	mMultiFaceHandleWidget->setWindowTitle("Multi-Face Handle");
	mMultiFaceHandleWidget->setLayout(mMultiFaceHandleLayout);
}

void HighgenusMode::toggleMultiFaceHandleUseMaxOffsetFlag(int state){
	
	((MainWindow*)mParent)->toggleMultiFaceHandleUseMaxOffsetFlag(state);
	
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
		((MainWindow*)mParent)->mfh_use_closest_edge_algo();	
		// //set all three widgets to be disabled
		// multiFaceHandleScaleLabel->setEnabled(false);
		// multiFaceHandleExtrudeDistanceLabel->setEnabled(false);
		// multiFaceHandleExtrudeDistanceSpinBox->setEnabled(false);
		// multiFaceHandleMaxOffsetsCheckBox->setEnabled(false);
		// multiFaceHandleScaleSpinBox->setEnabled(false);		
		
	}
	else { 		
		((MainWindow*)mParent)->mfh_use_convex_hull_algo();	
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
	
	mMengerSpongeLayout = new QGridLayout;
	// mMengerSpongeLayout->setMargin(0);
	mMengerSpongeLayout->setVerticalSpacing(1);
	mMengerSpongeLayout->setHorizontalSpacing(1);	
	//thickness
	mengerSpongeThicknessSpinBox = createDoubleSpinBox(mMengerSpongeLayout, mengerSpongeThicknessLabel, tr("Thickness:"), 0.01, 1.0, 0.01, 0.67, 2, 0,0);	
	connect(mengerSpongeThicknessSpinBox, SIGNAL(valueChanged(double)), ((MainWindow*)mParent), SLOT(changeMengerSpongeThickness(double)));
	//threshold
	mengerSpongeThresholdSpinBox = createDoubleSpinBox(mMengerSpongeLayout, mengerSpongeThresholdLabel, tr("Edge Collapse\nThreshold Factor:"), 0.0, 10.0, 0.1, 0.0, 1, 1,0);	
	connect(mengerSpongeThresholdSpinBox, SIGNAL(valueChanged(double)), ((MainWindow*)mParent), SLOT(changeMengerSpongeCollapseThreshold(double)));
	//fractional thickness checkbox
	QCheckBox *mengerSpongFractionalThicknessCheckBox = new QCheckBox(tr("Fractional Thickness"),this);
	connect(mengerSpongFractionalThicknessCheckBox, SIGNAL(stateChanged(int)), ((MainWindow*)mParent), SLOT(toggleMengerSpongeFractionalThicknessFlag(int)));
	mMengerSpongeLayout->addWidget(mengerSpongFractionalThicknessCheckBox,2,0,1,2);
	//create menger sponge button
	QPushButton *mengerSpongeButton = new QPushButton("Create Menger Sponge", this);
	connect(mengerSpongeButton, SIGNAL(clicked()), ((MainWindow*)mParent), SLOT(createSponge()));
	mMengerSpongeLayout->addWidget(mengerSpongeButton,3,0,1,2);	
	
	mMengerSpongeLayout->setRowStretch(4,1);
	mMengerSpongeLayout->setColumnStretch(2,1);
	mMengerSpongeWidget->setWindowTitle("Menger Sponge");
	mMengerSpongeWidget->setLayout(mMengerSpongeLayout);
}
