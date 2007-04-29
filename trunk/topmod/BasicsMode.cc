/****************************************************************************
 **
 **
 ****************************************************************************/
#include <QtGui>

#include "BasicsMode.hh"

BasicsMode::BasicsMode(QWidget *parent)
		: QWidget(parent) {
		
	this->setParent(parent);
	//here we set the default mode for when the application is executed.
	((MainWindow*)parentWidget())->setMode(DLFLWindow::InsertEdge);
	
	mInsertEdgeWidget = new QWidget;
  mDeleteEdgeWidget = new QWidget;
  mCollapseEdgeWidget = new QWidget;
  mSubdivideEdgeWidget = new QWidget;
  mConnectEdgesWidget = new QWidget;
  mSpliceCornersWidget = new QWidget;
  mTransformsWidget = new QWidget;

	//each mode widget will each be added to the 
	//ToolOptionsDockWidget of the MainWindow class
	setupInsertEdge();
	setupDeleteEdge();
	setupCollapseEdge();
	setupSubdivideEdge();
	setupConnectEdges();
	setupSpliceCorners();
	setupTransforms();
	
	mInsertEdgeAction = new QAction(tr("Insert Edge"),this);
	mInsertEdgeAction->setCheckable(true);
	mInsertEdgeAction->setChecked(true);
	// sm->registerAction(mInsertEdgeAction, "Basics Modes", "9");
	mInsertEdgeAction->setStatusTip(tr("Enter Insert Edge Mode"));
	mInsertEdgeAction->setToolTip(tr("Insert Edge Mode"));
	connect(mInsertEdgeAction, SIGNAL(triggered()), this, SLOT(triggerInsertEdge()));

	mDeleteEdgeAction = new QAction(tr("Delete Edge"),this);
	mDeleteEdgeAction->setCheckable(true);
	// sm->registerAction(mDeleteEdgeAction, "Basics Modes", "9");
	mDeleteEdgeAction->setStatusTip(tr("Enter Delete Edge Mode"));
	mDeleteEdgeAction->setToolTip(tr("Delete Edge Mode"));
	connect(mDeleteEdgeAction, SIGNAL(triggered()), this, SLOT(triggerDeleteEdge()));

	mCollapseEdgeAction = new QAction(tr("Collapse Edge"),this);
	mCollapseEdgeAction->setCheckable(true);
	// sm->registerAction(mCollapseEdgeAction, "Basics Modes", "9");
	mCollapseEdgeAction->setStatusTip(tr("Enter Collapse Edge Mode"));
	mCollapseEdgeAction->setToolTip(tr("Collapse Edge Mode"));
	connect(mCollapseEdgeAction, SIGNAL(triggered()), this, SLOT(triggerCollapseEdge()));

	mSubdivideEdgeAction = new QAction(tr("Subdivide Edge"),this);
	mSubdivideEdgeAction->setCheckable(true);
	// sm->registerAction(mDeleteEdgeAction, "Basics Modes", "9");
	mSubdivideEdgeAction->setStatusTip(tr("Enter Subdivide Edge Mode"));
	mSubdivideEdgeAction->setToolTip(tr("Subdivide Edge Mode"));
	connect(mSubdivideEdgeAction, SIGNAL(triggered()), this, SLOT(triggerSubdivideEdge()));
	
	mConnectEdgesAction = new QAction(tr("Connect Edges"),this);
	mConnectEdgesAction->setCheckable(true);
	// sm->registerAction(mDeleteEdgeAction, "Basics Modes", "9");
	mConnectEdgesAction->setStatusTip(tr("Enter Connect Edges Mode"));
	mConnectEdgesAction->setToolTip(tr("Connect Edges Mode"));
	connect(mConnectEdgesAction, SIGNAL(triggered()), this, SLOT(triggerConnectEdges()));
	
	mSpliceCornersAction = new QAction(tr("Splice Corners"),this);
	mSpliceCornersAction->setCheckable(true);
	// sm->registerAction(mDeleteEdgeAction, "Basics Modes", "9");
	mSpliceCornersAction->setStatusTip(tr("Enter Splice Corners Mode"));
	mSpliceCornersAction->setToolTip(tr("Splice Corners Mode"));
	connect(mSpliceCornersAction, SIGNAL(triggered()), this, SLOT(triggerSpliceCorners()));

	mTransformsAction = new QAction(tr("Transforms"),this);
	mTransformsAction->setCheckable(true);
	// sm->registerAction(mDeleteEdgeAction, "Basics Modes", "9");
	mTransformsAction->setStatusTip(tr("Enter Transforms Mode"));
	mTransformsAction->setToolTip(tr("Transforms Mode"));
	connect(mTransformsAction, SIGNAL(triggered()), this, SLOT(triggerTransforms()));
		
}

void BasicsMode::triggerInsertEdge(){
	
	((MainWindow*)parentWidget())->setToolOptions(mInsertEdgeWidget);
	((MainWindow*)parentWidget())->setMode(DLFLWindow::InsertEdge);
}

void BasicsMode::triggerDeleteEdge(){
	
	((MainWindow*)parentWidget())->setToolOptions(mDeleteEdgeWidget);
	((MainWindow*)parentWidget())->setMode(DLFLWindow::DeleteEdge);
}

void BasicsMode::triggerCollapseEdge(){
	
	((MainWindow*)parentWidget())->setToolOptions(mCollapseEdgeWidget);
	((MainWindow*)parentWidget())->setMode(DLFLWindow::CollapseEdge);
}

void BasicsMode::triggerSubdivideEdge(){
	
	((MainWindow*)parentWidget())->setToolOptions(mSubdivideEdgeWidget);
	((MainWindow*)parentWidget())->setMode(DLFLWindow::SubDivideEdge);
}

void BasicsMode::triggerConnectEdges(){
	
	((MainWindow*)parentWidget())->setToolOptions(mConnectEdgesWidget);
	((MainWindow*)parentWidget())->setMode(DLFLWindow::ConnectEdges);
}

void BasicsMode::triggerSpliceCorners(){
	
	((MainWindow*)parentWidget())->setToolOptions(mSpliceCornersWidget);
	((MainWindow*)parentWidget())->setMode(DLFLWindow::SpliceCorners);
}

void BasicsMode::triggerTransforms(){
	
	((MainWindow*)parentWidget())->setToolOptions(mTransformsWidget);
	((MainWindow*)parentWidget())->setMode(DLFLWindow::NormalMode);
}

void BasicsMode::addActions(QActionGroup *actionGroup, QToolBar *toolBar, QStackedWidget *stackedWidget){
	
	actionGroup->addAction(mInsertEdgeAction);
	actionGroup->addAction(mDeleteEdgeAction);	
	actionGroup->addAction(mCollapseEdgeAction);	
	actionGroup->addAction(mSubdivideEdgeAction);	
	actionGroup->addAction(mConnectEdgesAction);	
	actionGroup->addAction(mSpliceCornersAction);
	actionGroup->addAction(mTransformsAction);
	
	toolBar->addAction(mInsertEdgeAction);
	toolBar->addAction(mDeleteEdgeAction);	
	toolBar->addAction(mCollapseEdgeAction);	
	toolBar->addAction(mSubdivideEdgeAction);	
	toolBar->addAction(mConnectEdgesAction);	
	toolBar->addAction(mSpliceCornersAction);
	toolBar->addAction(mTransformsAction);
	
	stackedWidget->addWidget(mInsertEdgeWidget);
	stackedWidget->addWidget(mDeleteEdgeWidget);	
	stackedWidget->addWidget(mCollapseEdgeWidget);	
	stackedWidget->addWidget(mSubdivideEdgeWidget);	
	stackedWidget->addWidget(mConnectEdgesWidget);	
	stackedWidget->addWidget(mSpliceCornersWidget);
	stackedWidget->addWidget(mTransformsWidget);

}

void BasicsMode::setupInsertEdge() {
	
	mInsertEdgeLayout = new QBoxLayout(QBoxLayout::LeftToRight);
	mInsertEdgeLayout->setMargin(1);
	mInsertEdgeWidget->setLayout(mInsertEdgeLayout);	
}

void BasicsMode::setupDeleteEdge() {
	
	mDeleteEdgeLayout = new QBoxLayout(QBoxLayout::LeftToRight);
	mDeleteEdgeLayout->setMargin(1);
	//cleanup checkbox
	QCheckBox *cleanupCheckBox = new QCheckBox(tr("Cleanup"),this);
	connect(cleanupCheckBox, SIGNAL(stateChanged(int)),
          ((MainWindow*)parentWidget()), SLOT(toggleDeleteEdgeCleanupFlag(int)));
	mDeleteEdgeLayout->addWidget(cleanupCheckBox);	
	mDeleteEdgeLayout->addStretch(1);
	//set layout
	mDeleteEdgeWidget->setLayout(mDeleteEdgeLayout);	
}

void BasicsMode::setupCollapseEdge() {
	
	mCollapseEdgeLayout = new QBoxLayout(QBoxLayout::LeftToRight);
	mCollapseEdgeLayout->setMargin(1);
	mCollapseEdgeWidget->setLayout(mCollapseEdgeLayout);	
}

void BasicsMode::setupSubdivideEdge() {
	
	mSubdivideEdgeLayout = new QBoxLayout(QBoxLayout::LeftToRight);
	mSubdivideEdgeLayout->setMargin(1);
	
	//number of subdivisions spinbox
	QLabel *numSubdivsLabel = new QLabel(tr("# Subdivisions"));
	// numSubdivsLabel->setAlignment(Qt::AlignRight);
	QSpinBox *numSubdivsSpinBox = new QSpinBox;
	numSubdivsSpinBox->setRange(1, 10);
	numSubdivsSpinBox->setSingleStep(1);
	numSubdivsSpinBox->setValue(0);
	numSubdivsSpinBox->setMaximumSize(50,25);
	connect(numSubdivsSpinBox, SIGNAL(valueChanged(int)),
          ((MainWindow*)parentWidget()), SLOT(changeNumSubDivs(int)));
	
	mSubdivideEdgeLayout->addWidget(numSubdivsLabel);
	mSubdivideEdgeLayout->addWidget(numSubdivsSpinBox);
	mSubdivideEdgeLayout->addStretch(1);
	mSubdivideEdgeWidget->setLayout(mSubdivideEdgeLayout);	
	
}

void BasicsMode::setupConnectEdges(){
	
	mConnectEdgesLayout = new QBoxLayout(QBoxLayout::LeftToRight);
	mConnectEdgesLayout->setMargin(1);
	mConnectEdgesWidget->setLayout(mConnectEdgesLayout);	
	
}

void BasicsMode::setupSpliceCorners(){
	
	mSpliceCornersLayout = new QBoxLayout(QBoxLayout::LeftToRight);
	mSpliceCornersLayout->setMargin(1);
	mSpliceCornersWidget->setLayout(mSpliceCornersLayout);
	
}

void BasicsMode::setupTransforms(){
	
	mTransformsLayout = new QBoxLayout(QBoxLayout::LeftToRight);
	mTransformsLayout->setMargin(5);
	QLabel *transformLabel = new QLabel(tr("Translate:"));
	mTransformsLayout->addWidget(transformLabel);
	
	//x transform
	QLabel *xPosLabel = new QLabel(tr("X"));
	xPosSpinBox = new QDoubleSpinBox;
	xPosSpinBox->setRange(-100.0, 100.0);
	xPosSpinBox->setSingleStep(0.5);
	xPosSpinBox->setValue(0.0);
	xPosSpinBox->setDecimals(1);
	xPosSpinBox->setMaximumSize(50,25);
	connect(xPosSpinBox, SIGNAL(valueChanged(double)),
          ((MainWindow*)parentWidget()), SLOT(translatex(double)));

	mTransformsLayout->addWidget(xPosLabel);
  mTransformsLayout->addWidget(xPosSpinBox);

	//y transform
	QLabel *yPosLabel = new QLabel(tr("Y"));
	yPosSpinBox = new QDoubleSpinBox;
	yPosSpinBox->setRange(-100.0, 100.0);
	yPosSpinBox->setSingleStep(0.5);
	yPosSpinBox->setValue(0.0);
	yPosSpinBox->setDecimals(1);
	yPosSpinBox->setMaximumSize(50,25);
	connect(yPosSpinBox, SIGNAL(valueChanged(double)),
          ((MainWindow*)parentWidget()), SLOT(translatey(double)));

	mTransformsLayout->addWidget(yPosLabel);
  mTransformsLayout->addWidget(yPosSpinBox);

	//z transform
	QLabel *zPosLabel = new QLabel(tr("Z"));
	zPosSpinBox = new QDoubleSpinBox;
	zPosSpinBox->setRange(-100.0, 100.0);
	zPosSpinBox->setSingleStep(0.5);
	zPosSpinBox->setValue(0.0);
	zPosSpinBox->setDecimals(1);
	zPosSpinBox->setMaximumSize(50,25);
	connect(zPosSpinBox, SIGNAL(valueChanged(double)),
          ((MainWindow*)parentWidget()), SLOT(translatez(double)));

	mTransformsLayout->addWidget(zPosLabel);
  mTransformsLayout->addWidget(zPosSpinBox);
	
	QLabel *scaleLabel = new QLabel(tr("Scale:"));
	mTransformsLayout->addWidget(scaleLabel);
	
	//x scale
	QLabel *xScaleLabel = new QLabel(tr("X"));
	xScaleSpinBox = new QDoubleSpinBox;
	xScaleSpinBox->setRange(0.1, 10.0);
	xScaleSpinBox->setSingleStep(0.1);
	xScaleSpinBox->setValue(1.0);
	xScaleSpinBox->setDecimals(1);	
	xScaleSpinBox->setMaximumSize(50,25);
	connect(xScaleSpinBox, SIGNAL(valueChanged(double)),
          ((MainWindow*)parentWidget()), SLOT(scalex(double)));

	mTransformsLayout->addWidget(xScaleLabel);
  mTransformsLayout->addWidget(xScaleSpinBox);
	
	//y scale
	QLabel *yScaleLabel = new QLabel(tr("Y"));
	yScaleSpinBox = new QDoubleSpinBox;
	yScaleSpinBox->setRange(0.1, 10.0);
	yScaleSpinBox->setSingleStep(0.1);
	yScaleSpinBox->setValue(1.0);
	yScaleSpinBox->setDecimals(1);
	yScaleSpinBox->setMaximumSize(50,25);
	connect(yScaleSpinBox, SIGNAL(valueChanged(double)),
          ((MainWindow*)parentWidget()), SLOT(scaley(double)));

	mTransformsLayout->addWidget(yScaleLabel);
  mTransformsLayout->addWidget(yScaleSpinBox);

	//z scale
	QLabel *zScaleLabel = new QLabel(tr("Z"));
	zScaleSpinBox = new QDoubleSpinBox;
	zScaleSpinBox->setRange(0.1, 10.0);
	zScaleSpinBox->setSingleStep(0.1);
	zScaleSpinBox->setValue(1.0);
	zScaleSpinBox->setDecimals(1);
	zScaleSpinBox->setMaximumSize(50,25);
	connect(zScaleSpinBox, SIGNAL(valueChanged(double)),
          ((MainWindow*)parentWidget()), SLOT(scalez(double)));
	mTransformsLayout->addWidget(zScaleLabel);
  mTransformsLayout->addWidget(zScaleSpinBox);

	QPushButton *freezeTransformsButton = new QPushButton("&Freeze Transforms", this);
	connect(freezeTransformsButton, SIGNAL(clicked()),
          this, SLOT(freeze_transforms()));
	
	mTransformsLayout->addWidget(freezeTransformsButton);	
	mTransformsLayout->addStretch(1);
	mTransformsWidget->setLayout(mTransformsLayout);	
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