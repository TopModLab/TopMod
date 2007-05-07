/****************************************************************************
 **
 **
 ****************************************************************************/
#include <QtGui>

#include "ConicalMode.hh"

ConicalMode::ConicalMode(QWidget *parent, QShortcutManager *sm)
		: QWidget(parent)
{		
	setParent(0);
	mParent = parent;
	
	//create the stacked widget and all child widget pages
	mCutbyEdgeWidget = new QWidget;
  mCutbyVertexWidget = new QWidget;
  mCutbyEdgeVertexWidget = new QWidget;
  mCutbyFaceWidget = new QWidget;
  mTruncateEdgeWidget = new QWidget;
  mTruncateVertexWidget = new QWidget;
  mDualConvexHullWidget = new QWidget;

	setupCutbyEdge();
	setupCutbyVertex();
	setupCutbyEdgeVertex();
	setupCutbyFace();
	setupTruncateEdge();
	setupTruncateVertex();
	setupDualConvexHull();

	mCutbyEdgeAction = new QAction(QIcon(":images/doosabin_extrude.png"),tr("Cut by Edge"),this);
	mCutbyEdgeAction->setCheckable(true);
	sm->registerAction(mCutbyEdgeAction, "Conical Operations", "");
	mCutbyEdgeAction->setStatusTip(tr("Enter Cut by Edge Mode"));
	mCutbyEdgeAction->setToolTip(tr("Cut by Edge Mode"));
	connect(mCutbyEdgeAction, SIGNAL(triggered()), this, SLOT(triggerCutbyEdge()));

	mCutbyVertexAction = new QAction(QIcon(":images/doosabin_extrude.png"),tr("Cut by Vertex"),this);
	mCutbyVertexAction->setCheckable(true);
	sm->registerAction(mCutbyVertexAction, "Conical Operations", "");
	mCutbyVertexAction->setStatusTip(tr("Enter Cut by Vertex Mode"));
	mCutbyVertexAction->setToolTip(tr("Cut by Vertex Mode"));
	connect(mCutbyVertexAction, SIGNAL(triggered()), this, SLOT(triggerCutbyVertex()));

	mCutbyEdgeVertexAction = new QAction(QIcon(":images/doosabin_extrude.png"),tr("Cut by Edge-Vertex"),this);
	mCutbyEdgeVertexAction->setCheckable(true);
	sm->registerAction(mCutbyEdgeVertexAction, "Conical Operations", "");
	mCutbyEdgeVertexAction->setStatusTip(tr("Enter Cut by Edge-Vertex Mode"));
	mCutbyEdgeVertexAction->setToolTip(tr("Cut by EdgeVertex Mode"));
	connect(mCutbyEdgeVertexAction, SIGNAL(triggered()), this, SLOT(triggerCutbyEdgeVertex()));

	mCutbyFaceAction = new QAction(QIcon(":images/doosabin_extrude.png"),tr("Cut by Face"),this);
	mCutbyFaceAction->setCheckable(true);
	sm->registerAction(mCutbyFaceAction, "Conical Operations", "");
	mCutbyFaceAction->setStatusTip(tr("Enter Cut by Face Mode"));
	mCutbyFaceAction->setToolTip(tr("Cut by Face Mode"));
	connect(mCutbyFaceAction, SIGNAL(triggered()), this, SLOT(triggerCutbyFace()));

	mTruncateEdgeAction = new QAction(QIcon(":images/doosabin_extrude.png"),tr("Truncate Edge"),this);
	mTruncateEdgeAction->setCheckable(true);
	sm->registerAction(mTruncateEdgeAction, "Conical Operations", "");
	mTruncateEdgeAction->setStatusTip(tr("Enter Truncate Edge Mode"));
	mTruncateEdgeAction->setToolTip(tr("Truncate Edge Mode"));
	connect(mTruncateEdgeAction, SIGNAL(triggered()), this, SLOT(triggerTruncateEdge()));

	mTruncateVertexAction = new QAction(QIcon(":images/doosabin_extrude.png"),tr("Truncate Vertex"),this);
	mTruncateVertexAction->setCheckable(true);
	sm->registerAction(mTruncateVertexAction, "Conical Operations", "");
	mTruncateVertexAction->setStatusTip(tr("Enter Truncate Vertex Mode"));
	mTruncateVertexAction->setToolTip(tr("Truncate Vertex Mode"));
	connect(mTruncateVertexAction, SIGNAL(triggered()), this, SLOT(triggerTruncateVertex()));
	
	mDualConvexHullAction = new QAction(QIcon(":images/doosabin_extrude.png"),tr("Dual Convex Hull"),this);
	mDualConvexHullAction->setCheckable(true);
	sm->registerAction(mDualConvexHullAction, "Conical Operations", "");
	mDualConvexHullAction->setStatusTip(tr("Enter Dual Convex Hull Mode"));
	mDualConvexHullAction->setToolTip(tr("Dual Convex Hull Mode"));
	connect(mDualConvexHullAction, SIGNAL(triggered()), this, SLOT(triggerDualConvexHull()));

}

QMenu *ConicalMode::getMenu(){
	
	mConicalMenu = new QMenu(tr("Conical"));
	
	mConicalMenu->addAction(mCutbyEdgeAction);
	mConicalMenu->addAction(mCutbyVertexAction);
	mConicalMenu->addAction(mCutbyEdgeVertexAction);
	mConicalMenu->addAction(mCutbyFaceAction);
	mConicalMenu->addAction(mTruncateEdgeAction);
	mConicalMenu->addAction(mTruncateVertexAction); 
	mConicalMenu->addAction(mDualConvexHullAction);
	
	return mConicalMenu;
}

void ConicalMode::triggerCutbyEdge(){
	
	((MainWindow*)mParent)->setToolOptions(mCutbyEdgeWidget);
	((MainWindow*)mParent)->setMode(DLFLWindow::CutEdge);
}

void ConicalMode::triggerCutbyVertex(){
	
	((MainWindow*)mParent)->setToolOptions(mCutbyVertexWidget);
	((MainWindow*)mParent)->setMode(DLFLWindow::CutVertex);
}

void ConicalMode::triggerCutbyEdgeVertex(){
	
	((MainWindow*)mParent)->setToolOptions(mCutbyEdgeVertexWidget);
	((MainWindow*)mParent)->setMode(DLFLWindow::CutEdgeandVertex);
}

void ConicalMode::triggerCutbyFace(){
	
	((MainWindow*)mParent)->setToolOptions(mCutbyFaceWidget);
	((MainWindow*)mParent)->setMode(DLFLWindow::CutFace);
}

void ConicalMode::triggerTruncateEdge(){
	
	((MainWindow*)mParent)->setToolOptions(mTruncateEdgeWidget);
	((MainWindow*)mParent)->setMode(DLFLWindow::TruncateEdge);
}

void ConicalMode::triggerTruncateVertex(){
	
	((MainWindow*)mParent)->setToolOptions(mTruncateVertexWidget);
	((MainWindow*)mParent)->setMode(DLFLWindow::MarkVertex);
}

void ConicalMode::triggerDualConvexHull(){
	
	((MainWindow*)mParent)->setToolOptions(mDualConvexHullWidget);
	((MainWindow*)mParent)->setMode(DLFLWindow::ConvexHullMode);
}

void ConicalMode::addActions(QActionGroup *actionGroup, QToolBar *toolBar, QStackedWidget *stackedWidget){
	
	actionGroup->addAction(mCutbyEdgeAction);
	actionGroup->addAction(mCutbyVertexAction);
	actionGroup->addAction(mCutbyEdgeVertexAction);
	actionGroup->addAction(mCutbyFaceAction);
	actionGroup->addAction(mTruncateEdgeAction);
	actionGroup->addAction(mTruncateVertexAction); 
	actionGroup->addAction(mDualConvexHullAction);
	
	toolBar->addAction(mCutbyEdgeAction);
	toolBar->addAction(mCutbyVertexAction);
	toolBar->addAction(mCutbyEdgeVertexAction);
	toolBar->addAction(mCutbyFaceAction);
	toolBar->addAction(mTruncateEdgeAction);
	toolBar->addAction(mTruncateVertexAction); 
	toolBar->addAction(mDualConvexHullAction);
	
	stackedWidget->addWidget(mCutbyEdgeWidget);
	stackedWidget->addWidget(mCutbyVertexWidget);
	stackedWidget->addWidget(mCutbyEdgeVertexWidget); 
	stackedWidget->addWidget(mCutbyFaceWidget);      	
	stackedWidget->addWidget(mTruncateEdgeWidget);    
	stackedWidget->addWidget(mTruncateVertexWidget); 
	stackedWidget->addWidget(mDualConvexHullWidget); 

}

void ConicalMode::changeCutOffsetE(double value){
	
	cutbyEdgeOffsetSpinBox->setValue(value);	
	cutbyFaceOffsetSpinBox->setValue(value);	
	truncateEdgeOffsetSpinBox->setValue(value);
	truncateVertexOffsetSpinBox->setValue(value);	
	cutbyEdgeVertexOffsetESpinBox->setValue(value);	
	cutbyFaceOffsetSpinBox->setValue(value);	
	
  ((MainWindow*)mParent)->changeCutOffsetE(value);
}

void ConicalMode::changeCutOffsetV(double value){
	
	cutbyVertexOffsetSpinBox->setValue(value);	
	cutbyEdgeVertexOffsetVSpinBox->setValue(value);	
	
  ((MainWindow*)mParent)->changeCutOffsetV(value);
}

void ConicalMode::toggleGlobalCut(int state){    
	
	cutbyEdgeGlobalCheckBox->setChecked(state);
	cutbyVertexGlobalCheckBox->setChecked(state);
	truncateVertexGlobalCheckBox->setChecked(state);
	cutbyEdgeVertexGlobalCheckBox->setChecked(state);
	cutbyFaceGlobalCheckBox->setChecked(state);
	truncateEdgeGlobalCheckBox->setChecked(state);
	
  ((MainWindow*)mParent)->toggleGlobalCut(state);
}
void ConicalMode::toggleSelectedCut(int state){
	
	cutbyEdgeCutSelectedCheckBox->setChecked(state);
	cutbyVertexCutSelectedCheckBox->setChecked(state);
	truncateVertexCutSelectedCheckBox->setChecked(state);
	cutbyEdgeVertexCutSelectedCheckBox->setChecked(state);
	cutbyFaceCutSelectedCheckBox->setChecked(state);
	truncateEdgeCutSelectedCheckBox->setChecked(state);
	
  ((MainWindow*)mParent)->toggleSelectedCut(state);
}

void ConicalMode::changeTiltPlane1(double value){
	
	cutbyEdgeTiltPlane1SpinBox->setValue(value);	
	cutbyEdgeVertexTiltPlane1SpinBox->setValue(value);	
	
	((MainWindow*)mParent)->changeTiltPlane1(value);
}
void ConicalMode::changeTiltPlane2(double value){
	
	cutbyEdgeTiltPlane2SpinBox->setValue(value);
	cutbyEdgeVertexTiltPlane2SpinBox->setValue(value);	
	
	((MainWindow*)mParent)->changeCutOffsetE(value); 
}

void ConicalMode::setupCutbyEdge(){
	
	mCutbyEdgeLayout = new QBoxLayout(QBoxLayout::LeftToRight);
	mCutbyEdgeLayout->setMargin(0);
	
	QLabel *cutbyEdgeOffsetLabel = new QLabel(tr("Offset:"));
	cutbyEdgeOffsetSpinBox = new QDoubleSpinBox;
	cutbyEdgeOffsetSpinBox->setRange(0, 1);
	cutbyEdgeOffsetSpinBox->setSingleStep(0.01);
	cutbyEdgeOffsetSpinBox->setValue(0.25);	
	cutbyEdgeOffsetSpinBox->setMaximumSize(75,25);
	connect(cutbyEdgeOffsetSpinBox, SIGNAL(valueChanged(double)),
          this, SLOT(changeCutOffsetE(double)));
	
	mCutbyEdgeLayout->addWidget(cutbyEdgeOffsetLabel);
  mCutbyEdgeLayout->addWidget(cutbyEdgeOffsetSpinBox);

	QLabel *cutbyEdgeTiltPlane1Label = new QLabel(tr("Tilt plane:"));
	cutbyEdgeTiltPlane1SpinBox = new QDoubleSpinBox;
	cutbyEdgeTiltPlane1SpinBox->setRange(-1, 1);
	cutbyEdgeTiltPlane1SpinBox->setSingleStep(.01);
	cutbyEdgeTiltPlane1SpinBox->setValue(0);	
	cutbyEdgeTiltPlane1SpinBox->setMaximumSize(75,25);
	connect(cutbyEdgeTiltPlane1SpinBox, SIGNAL(valueChanged(double)),
          this, SLOT(changeTiltPlane1(double)));
	
	mCutbyEdgeLayout->addWidget(cutbyEdgeTiltPlane1Label);
  mCutbyEdgeLayout->addWidget(cutbyEdgeTiltPlane1SpinBox);

	QLabel *cutbyEdgeTiltPlane2Label = new QLabel(tr("Tilt plane:"));
	cutbyEdgeTiltPlane2SpinBox = new QDoubleSpinBox;
	cutbyEdgeTiltPlane2SpinBox->setRange(-1, 1);
	cutbyEdgeTiltPlane2SpinBox->setSingleStep(.01);
	cutbyEdgeTiltPlane2SpinBox->setValue(0);
	cutbyEdgeTiltPlane2SpinBox->setMaximumSize(75,25);
	connect(cutbyEdgeTiltPlane2SpinBox, SIGNAL(valueChanged(double)),
          this, SLOT(changeTiltPlane2(double)));
	
	mCutbyEdgeLayout->addWidget(cutbyEdgeTiltPlane2Label);
  mCutbyEdgeLayout->addWidget(cutbyEdgeTiltPlane2SpinBox);

	cutbyEdgeGlobalCheckBox = new QCheckBox(tr("global cut"),this);
	cutbyEdgeGlobalCheckBox->setChecked(Qt::Unchecked);
	connect(cutbyEdgeGlobalCheckBox, SIGNAL(stateChanged(int)),
          this, SLOT(toggleGlobalCut(int)));
				
	mCutbyEdgeLayout->addWidget(cutbyEdgeGlobalCheckBox);
	
	cutbyEdgeCutSelectedCheckBox = new QCheckBox(tr("cut selected edges"),this);
	cutbyEdgeCutSelectedCheckBox->setChecked(Qt::Checked);
	connect(cutbyEdgeCutSelectedCheckBox, SIGNAL(stateChanged(int)),
          this, SLOT(toggleSelectedCut(int)));
				
	mCutbyEdgeLayout->addWidget(cutbyEdgeCutSelectedCheckBox);
		
	QPushButton *performCuttingButton = new QPushButton(tr("Perform Cutting"), this);
	connect(performCuttingButton, SIGNAL(clicked()),
          ((MainWindow*)mParent), SLOT(perform_cutting()));
	
	mCutbyEdgeLayout->addWidget(performCuttingButton);
	mCutbyEdgeLayout->addStretch(1);
	mCutbyEdgeWidget->setWindowTitle("Cut by Edge");
	mCutbyEdgeWidget->setLayout(mCutbyEdgeLayout);	
	
}

void ConicalMode::setupCutbyVertex(){
	
	mCutbyVertexLayout = new QBoxLayout(QBoxLayout::LeftToRight);
	mCutbyVertexLayout->setMargin(0);
	
	QLabel *cutbyVertexOffsetLabel = new QLabel(tr("Offset:"));
	cutbyVertexOffsetSpinBox = new QDoubleSpinBox;
	cutbyVertexOffsetSpinBox->setRange(0, 1);
	cutbyVertexOffsetSpinBox->setSingleStep(0.01);
	cutbyVertexOffsetSpinBox->setValue(0.25);	
	cutbyVertexOffsetSpinBox->setMaximumSize(75,25);
	connect(cutbyVertexOffsetSpinBox, SIGNAL(valueChanged(double)),
          this, SLOT(changeCutOffsetV(double)));
	
	mCutbyVertexLayout->addWidget(cutbyVertexOffsetLabel);
  mCutbyVertexLayout->addWidget(cutbyVertexOffsetSpinBox);

	cutbyVertexGlobalCheckBox = new QCheckBox(tr("global cut"),this);
	cutbyVertexGlobalCheckBox->setChecked(Qt::Unchecked);
	connect(cutbyVertexGlobalCheckBox, SIGNAL(stateChanged(int)),
          this, SLOT(toggleGlobalCut(int)));
				
	mCutbyVertexLayout->addWidget(cutbyVertexGlobalCheckBox);
	
	cutbyVertexCutSelectedCheckBox = new QCheckBox(tr("cut selected edges"),this);
	cutbyVertexCutSelectedCheckBox->setChecked(Qt::Checked);
	connect(cutbyVertexCutSelectedCheckBox, SIGNAL(stateChanged(int)),
          this, SLOT(toggleSelectedCut(int)));
				
	mCutbyVertexLayout->addWidget(cutbyVertexCutSelectedCheckBox);	
	
	QPushButton *performCuttingButton = new QPushButton(tr("Perform Cutting"), this);
	connect(performCuttingButton, SIGNAL(clicked()),
          ((MainWindow*)mParent), SLOT(perform_cutting()));

	mCutbyVertexLayout->addWidget(performCuttingButton);
	mCutbyVertexLayout->addStretch(1);
	mCutbyVertexWidget->setWindowTitle("Cut by Vertex");
	mCutbyVertexWidget->setLayout(mCutbyVertexLayout);	
	
}

void ConicalMode::setupCutbyEdgeVertex(){
	
	mCutbyEdgeVertexLayout = new QBoxLayout(QBoxLayout::LeftToRight);
	mCutbyEdgeVertexLayout->setMargin(0);
	
	QLabel *cutbyEdgeVertexOffsetELabel = new QLabel(tr("Offset (Edge):"));
	cutbyEdgeVertexOffsetESpinBox = new QDoubleSpinBox;
	cutbyEdgeVertexOffsetESpinBox->setRange(0, 1);
	cutbyEdgeVertexOffsetESpinBox->setSingleStep(0.01);
	cutbyEdgeVertexOffsetESpinBox->setValue(0.25);	
	cutbyEdgeVertexOffsetESpinBox->setMaximumSize(65,25);
	connect(cutbyEdgeVertexOffsetESpinBox, SIGNAL(valueChanged(double)),
          this, SLOT(changeCutOffsetE(double)));
	
	mCutbyEdgeVertexLayout->addWidget(cutbyEdgeVertexOffsetELabel);
  mCutbyEdgeVertexLayout->addWidget(cutbyEdgeVertexOffsetESpinBox);

	QLabel *cutbyEdgeVertexOffsetVLabel = new QLabel(tr("Offset (Vertex):"));
	cutbyEdgeVertexOffsetVSpinBox = new QDoubleSpinBox;
	cutbyEdgeVertexOffsetVSpinBox->setRange(0, 1);
	cutbyEdgeVertexOffsetVSpinBox->setSingleStep(0.01);
	cutbyEdgeVertexOffsetVSpinBox->setValue(0.25);	
	cutbyEdgeVertexOffsetVSpinBox->setMaximumSize(65,25);
	connect(cutbyEdgeVertexOffsetVSpinBox, SIGNAL(valueChanged(double)),
          this, SLOT(changeCutOffsetV(double)));
	
	mCutbyEdgeVertexLayout->addWidget(cutbyEdgeVertexOffsetVLabel);
  mCutbyEdgeVertexLayout->addWidget(cutbyEdgeVertexOffsetVSpinBox);

	QLabel *cutbyEdgeVertexTiltPlane1Label = new QLabel(tr("Tilt plane:"));
	cutbyEdgeVertexTiltPlane1SpinBox = new QDoubleSpinBox;
	cutbyEdgeVertexTiltPlane1SpinBox->setRange(-1, 1);
	cutbyEdgeVertexTiltPlane1SpinBox->setSingleStep(.01);
	cutbyEdgeVertexTiltPlane1SpinBox->setValue(0);	
	cutbyEdgeVertexTiltPlane1SpinBox->setMaximumSize(65,25);
	connect(cutbyEdgeVertexTiltPlane1SpinBox, SIGNAL(valueChanged(double)),
          this, SLOT(changeTiltPlane1(double)));
	
	mCutbyEdgeVertexLayout->addWidget(cutbyEdgeVertexTiltPlane1Label);
  mCutbyEdgeVertexLayout->addWidget(cutbyEdgeVertexTiltPlane1SpinBox);

	QLabel *cutbyEdgeVertexTiltPlane2Label = new QLabel(tr("Tilt plane:"));
	cutbyEdgeVertexTiltPlane2SpinBox = new QDoubleSpinBox;
	cutbyEdgeVertexTiltPlane2SpinBox->setRange(-1, 1);
	cutbyEdgeVertexTiltPlane2SpinBox->setSingleStep(0.01);
	cutbyEdgeVertexTiltPlane2SpinBox->setValue(0);	
	cutbyEdgeVertexTiltPlane2SpinBox->setMaximumSize(65,25);
	connect(cutbyEdgeVertexTiltPlane2SpinBox, SIGNAL(valueChanged(double)),
          this, SLOT(changeTiltPlane2(double)));
	
	mCutbyEdgeVertexLayout->addWidget(cutbyEdgeVertexTiltPlane2Label);
  mCutbyEdgeVertexLayout->addWidget(cutbyEdgeVertexTiltPlane2SpinBox);

	cutbyEdgeVertexGlobalCheckBox = new QCheckBox(tr("global cut"),this);
	cutbyEdgeVertexGlobalCheckBox->setChecked(Qt::Unchecked);
	connect(cutbyEdgeVertexGlobalCheckBox, SIGNAL(stateChanged(int)),
          this, SLOT(toggleGlobalCut(int)));
				
	mCutbyEdgeVertexLayout->addWidget(cutbyEdgeVertexGlobalCheckBox);
	
	cutbyEdgeVertexCutSelectedCheckBox = new QCheckBox(tr("cut selected edges"),this);
	cutbyEdgeVertexCutSelectedCheckBox->setChecked(Qt::Checked);
	connect(cutbyEdgeVertexCutSelectedCheckBox, SIGNAL(stateChanged(int)),
          this, SLOT(toggleSelectedCut(int)));
				
	mCutbyEdgeVertexLayout->addWidget(cutbyEdgeVertexCutSelectedCheckBox);

	QPushButton *performCuttingButton = new QPushButton(tr("Perform Cutting"), this);
	connect(performCuttingButton, SIGNAL(clicked()),
          ((MainWindow*)mParent), SLOT(perform_cutting()));

	mCutbyEdgeVertexLayout->addWidget(performCuttingButton);
	mCutbyEdgeVertexLayout->addStretch(1);
	mCutbyEdgeVertexWidget->setWindowTitle("Cut by Edge/Vertex");
	mCutbyEdgeVertexWidget->setLayout(mCutbyEdgeVertexLayout);	
	
}

void ConicalMode::setupCutbyFace(){
	
	mCutbyFaceLayout = new QBoxLayout(QBoxLayout::LeftToRight);
	mCutbyFaceLayout->setMargin(0);

	QLabel *cutbyFaceOffsetLabel = new QLabel(tr("Offset:"));
	cutbyFaceOffsetSpinBox = new QDoubleSpinBox;
	cutbyFaceOffsetSpinBox->setRange(0, 1);
	cutbyFaceOffsetSpinBox->setSingleStep(0.01);
	cutbyFaceOffsetSpinBox->setValue(0.25);	
	cutbyFaceOffsetSpinBox->setMaximumSize(75,25);
	connect(cutbyFaceOffsetSpinBox, SIGNAL(valueChanged(double)),
          this, SLOT(changeCutOffsetE(double)));
	
	mCutbyFaceLayout->addWidget(cutbyFaceOffsetLabel);
  mCutbyFaceLayout->addWidget(cutbyFaceOffsetSpinBox);

	cutbyFaceGlobalCheckBox = new QCheckBox(tr("global cut"),this);
	cutbyFaceGlobalCheckBox->setChecked(Qt::Unchecked);
	connect(cutbyFaceGlobalCheckBox, SIGNAL(stateChanged(int)),
          this, SLOT(toggleGlobalCut(int)));

	mCutbyFaceLayout->addWidget(cutbyFaceGlobalCheckBox);
	
	cutbyFaceCutSelectedCheckBox = new QCheckBox(tr("cut selected faces"),this);
	cutbyFaceCutSelectedCheckBox->setChecked(Qt::Checked);
	connect(cutbyFaceCutSelectedCheckBox, SIGNAL(stateChanged(int)),
          this, SLOT(toggleSelectedCut(int)));
				
	mCutbyFaceLayout->addWidget(cutbyFaceCutSelectedCheckBox);

	QPushButton *performCuttingButton = new QPushButton(tr("Perform Cutting"), this);
	connect(performCuttingButton, SIGNAL(clicked()),
          ((MainWindow*)mParent), SLOT(perform_cutting()));

	mCutbyFaceLayout->addWidget(performCuttingButton);
	mCutbyFaceLayout->addStretch(1);
	mCutbyFaceWidget->setWindowTitle("Cut by Face");
	mCutbyFaceWidget->setLayout(mCutbyFaceLayout);	
	
}

void ConicalMode::setupTruncateEdge(){
	
	mTruncateEdgeLayout = new QBoxLayout(QBoxLayout::LeftToRight);
	mTruncateEdgeLayout->setMargin(0);
	
	QLabel *truncateEdgeOffsetLabel = new QLabel(tr("Offset:"));
	truncateEdgeOffsetSpinBox = new QDoubleSpinBox;
	truncateEdgeOffsetSpinBox->setRange(0, 1);
	truncateEdgeOffsetSpinBox->setSingleStep(0.01);
	truncateEdgeOffsetSpinBox->setValue(0.25);	
	truncateEdgeOffsetSpinBox->setMaximumSize(75,25);
	connect(truncateEdgeOffsetSpinBox, SIGNAL(valueChanged(double)),
          this, SLOT(changeCutOffsetE(double)));
	
	mTruncateEdgeLayout->addWidget(truncateEdgeOffsetLabel);
  mTruncateEdgeLayout->addWidget(truncateEdgeOffsetSpinBox);

	truncateEdgeGlobalCheckBox = new QCheckBox(tr("global cut"),this);
	truncateEdgeGlobalCheckBox->setChecked(Qt::Unchecked);
	connect(truncateEdgeGlobalCheckBox, SIGNAL(stateChanged(int)),
          this, SLOT(toggleGlobalCut(int)));

	mTruncateEdgeLayout->addWidget(truncateEdgeGlobalCheckBox);
	
	truncateEdgeCutSelectedCheckBox = new QCheckBox(tr("cut selected vertices"),this);
	truncateEdgeCutSelectedCheckBox->setChecked(Qt::Checked);
	connect(truncateEdgeCutSelectedCheckBox, SIGNAL(stateChanged(int)),
          this, SLOT(toggleSelectedCut(int)));
				
	mTruncateEdgeLayout->addWidget(truncateEdgeCutSelectedCheckBox);	
	
	QPushButton *performCuttingButton = new QPushButton(tr("Perform Cutting"), this);
	connect(performCuttingButton, SIGNAL(clicked()),
          ((MainWindow*)mParent), SLOT(perform_cutting()));

	mTruncateEdgeLayout->addWidget(performCuttingButton);
	mTruncateEdgeLayout->addStretch(1);
	mTruncateEdgeWidget->setWindowTitle("Truncate Edge");
	mTruncateEdgeWidget->setLayout(mTruncateEdgeLayout);	
	
}

void ConicalMode::setupTruncateVertex(){
	
	mTruncateVertexLayout = new QBoxLayout(QBoxLayout::LeftToRight);
	mTruncateVertexLayout->setMargin(0);
	
	QLabel *truncateVertexOffsetLabel = new QLabel(tr("Offset:"));
	truncateVertexOffsetSpinBox = new QDoubleSpinBox;
	truncateVertexOffsetSpinBox->setRange(0, 1);
	truncateVertexOffsetSpinBox->setSingleStep(0.01);
	truncateVertexOffsetSpinBox->setValue(0.25);	
	truncateVertexOffsetSpinBox->setMaximumSize(75,25);
	connect(truncateVertexOffsetSpinBox, SIGNAL(valueChanged(double)),
          this, SLOT(changeCutOffsetE(double)));
	
	mTruncateVertexLayout->addWidget(truncateVertexOffsetLabel);
  mTruncateVertexLayout->addWidget(truncateVertexOffsetSpinBox);

	truncateVertexGlobalCheckBox = new QCheckBox(tr("global cut"),this);
	truncateVertexGlobalCheckBox->setChecked(Qt::Unchecked);
	connect(truncateVertexGlobalCheckBox, SIGNAL(stateChanged(int)),
          this, SLOT(toggleGlobalCut(int)));

	mTruncateVertexLayout->addWidget(truncateVertexGlobalCheckBox);
	
	truncateVertexCutSelectedCheckBox = new QCheckBox(tr("cut selected vertices"),this);
	truncateVertexCutSelectedCheckBox->setChecked(Qt::Checked);
	connect(truncateVertexCutSelectedCheckBox, SIGNAL(stateChanged(int)),
          this, SLOT(toggleSelectedCut(int)));
				
	mTruncateVertexLayout->addWidget(truncateVertexCutSelectedCheckBox);	

	QPushButton *performCuttingButton = new QPushButton(tr("Perform Cutting"), this);
	connect(performCuttingButton, SIGNAL(clicked()),
          ((MainWindow*)mParent), SLOT(perform_cutting()));

	mTruncateVertexLayout->addWidget(performCuttingButton);	
	mTruncateVertexLayout->addStretch(1);
	mTruncateVertexWidget->setWindowTitle("Truncate Vertex");
	mTruncateVertexWidget->setLayout(mTruncateVertexLayout);
	
}

void ConicalMode::setupDualConvexHull(){
	
	mDualConvexHullLayout = new QBoxLayout(QBoxLayout::LeftToRight);
	mDualConvexHullLayout->setMargin(0);

	QPushButton *convexHullButton = new QPushButton(tr("Create Convex Hull"), this);
	connect(convexHullButton, SIGNAL(clicked()),
          ((MainWindow*)mParent), SLOT(create_convexhull()));

	mDualConvexHullLayout->addWidget(convexHullButton);
	
	QPushButton *dualConvexHullButton = new QPushButton(tr("Create Dual Convex Hull"), this);
	connect(dualConvexHullButton, SIGNAL(clicked()),
          ((MainWindow*)mParent), SLOT(create_dual_convexhull()));

	mDualConvexHullLayout->addWidget(dualConvexHullButton);	
	mDualConvexHullLayout->addStretch(1);
	mDualConvexHullWidget->setWindowTitle("Dual Convex Hull");
	mDualConvexHullWidget->setLayout(mDualConvexHullLayout);	
}
