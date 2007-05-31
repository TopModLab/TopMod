#include "TopModPreferences.hh"

TopModPreferences::TopModPreferences(QSettings *settings, StyleSheetEditor *sse, QShortcutManager *sm, QWidget *parent) 
:	QDialog(parent) {
	
	mSettings = settings;
	mParent = parent;
	setSizeGripEnabled(false);
	resize(600,500);
	setMinimumSize(600,500);
	setWindowTitle(tr("TopMod Preferences"));
	// setWindowFlags(Qt::FramelessWindowHint | Qt::Dialog);
	
	// mMainTab = new QWidget;
	mStyleSheetsTab = sse;
	mShortcutsManager = sm;
	mShortcutsTab = sm->getShortcutDialog();
	
	mPrefTabs = new QTabWidget;
	//setup layouts
	QVBoxLayout *mainLayout = new QVBoxLayout;
	mainLayout->addWidget(mPrefTabs);
	QHBoxLayout *lowLayout = new QHBoxLayout;
	mainLayout->addLayout(lowLayout,1);
	lowLayout->addStretch(1);
	
	mOkButton = new QPushButton(tr("OK"));
	// mOkButton->setDefault(true);
	connect(mOkButton, SIGNAL(clicked()), this, SLOT(saveSettings()));
	mCancelButton = new QPushButton(tr("Cancel"));
	connect(mCancelButton, SIGNAL(clicked()), this, SLOT(discardSettings()));
	lowLayout->addWidget(mOkButton);
	lowLayout->addWidget(mCancelButton);
	setLayout(mainLayout);

	createTabs();
	readSettings();
	setupColors();
	setupMain();

}

TopModPreferences::~TopModPreferences(){

}

int TopModPreferences::display(){
	
	mShortcutsTab->exec();
	return exec();
}

void TopModPreferences::createTabs(){

	mMainTab = new QWidget;
	mColorsTab = new QWidget;
	 
	mPrefTabs->addTab(mMainTab,tr("Main"));
	mPrefTabs->addTab(mColorsTab,tr("Colors"));
	mPrefTabs->addTab(((QWidget*)mShortcutsTab),tr("Shortcuts"));
	mPrefTabs->addTab(mStyleSheetsTab,tr("Stylesheets"));
}

void TopModPreferences::saveSettings(){
	mSettings->beginGroup("Colors");
	mSettings->setValue("ViewportColor", mViewportColor);
	mSettings->setValue("RenderColor",mRenderColor);
	mSettings->setValue("CoolLightColor",mCoolLightColor);
	mSettings->setValue("WarmLightColor",mWarmLightColor);
	mSettings->setValue("WireframeColor",mWireframeColor);
	mSettings->setValue("SilhouetteColor",mSilhouetteColor);
	mSettings->setValue("PatchBoundaryColor",mPatchBoundaryColor);
	mSettings->setValue("SelectedVertexColor",mSelectedVertexColor);
	mSettings->setValue("SelectedEdgeColor",mSelectedEdgeColor);
	mSettings->setValue("SelectedFaceColor",mSelectedFaceColor);
	mSettings->setValue("VertexIDBgColor",mVertexIDBgColor);
	mSettings->setValue("EdgeIDBgColor",mEdgeIDBgColor);
	mSettings->setValue("FaceIDBgColor",mFaceIDBgColor);
	mSettings->setValue("LightIntensity",mLightIntensity);
	mSettings->endGroup();
	
	mSettings->beginGroup("Display");
	mSettings->setValue("WireframeThickness", mWireframeThickness);
	mSettings->setValue("SilhouetteThickness", mSilhouetteThickness);
	mSettings->setValue("VertexThickness", mVertexThickness);
	mSettings->setValue("SelectedVertexThickness", mSelectedVertexThickness);
	mSettings->setValue("SelectedEdgeThickness", mSelectedEdgeThickness);
	mSettings->endGroup();

	
	accept();
}

void TopModPreferences::readSettings(){

	//load the settings file and set defaults if values don't exist
	mSettings->beginGroup("Colors");
	mViewportColor = mSettings->value("ViewportColor", QColor(255,255,255)).value<QColor>();
	mRenderColor = mSettings->value("RenderColor", QColor(255,255,255)).value<QColor>();
	mCoolLightColor = mSettings->value("CoolLightColor",QColor(51,51,102)).value<QColor>();
	mWarmLightColor = mSettings->value("WarmLightColor",QColor(255,255,153)).value<QColor>();
	mWireframeColor = mSettings->value("WireframeColor", QColor(0,255,0,127)).value<QColor>();
	mSilhouetteColor = mSettings->value("SilhouetteColor", QColor(0,0,0,127)).value<QColor>();
	mPatchBoundaryColor = mSettings->value("PatchBoundaryColor", QColor(0,0,0)).value<QColor>();
	mSelectedVertexColor = mSettings->value("SelectedVertexColor", QColor(0,127,0,127)).value<QColor>();
	mSelectedEdgeColor = mSettings->value("SelectedEdgeColor", QColor(127,0,0,127)).value<QColor>();
	mSelectedFaceColor = mSettings->value("SelectedFaceColor", QColor(0,0,127,127)).value<QColor>();
	mVertexIDBgColor = mSettings->value("VertexIDBgColor", QColor(127,0,0,127)).value<QColor>();
	mEdgeIDBgColor = mSettings->value("EdgeIDBgColor", QColor(0,127,0,127)).value<QColor>();
	mFaceIDBgColor = mSettings->value("FaceIDBgColor", QColor(0,0,127,127)).value<QColor>();
	mLightIntensity = mSettings->value("LightIntensity", 2.0).toDouble();
	mSettings->endGroup();
	
	mSettings->beginGroup("Display");
	mWireframeThickness = mSettings->value("WireframeThickness", 0.1).toDouble();
	mSilhouetteThickness = mSettings->value("SilhouetteThickness", 6.0).toDouble();
	mVertexThickness = mSettings->value("VertexThickness",5.0).toDouble();
	mSelectedVertexThickness = mSettings->value("SelectedVertexThickness",8.0).toDouble();
	mSelectedEdgeThickness = mSettings->value("SelectedEdgeThickness",5.0).toDouble();
	mSettings->endGroup();
	//initialize the settings
	
	((MainWindow*)mParent)->getDLFLWindow()->getActive()->setViewportColor(mViewportColor);
	((MainWindow*)mParent)->getDLFLWindow()->getActive()->setRenderColor(mRenderColor);
	((MainWindow*)mParent)->getDLFLWindow()->setCoolLightColor(mCoolLightColor);
	((MainWindow*)mParent)->getDLFLWindow()->setWarmLightColor(mWarmLightColor);
	((MainWindow*)mParent)->getDLFLWindow()->getActive()->setWireframeColor(mWireframeColor);
	((MainWindow*)mParent)->getDLFLWindow()->getActive()->setSilhouetteColor(mSilhouetteColor);
	((MainWindow*)mParent)->getDLFLWindow()->getActive()->setSelectedVertexColor(mSelectedVertexColor);
	((MainWindow*)mParent)->getDLFLWindow()->getActive()->setSelectedEdgeColor(mSelectedEdgeColor);
	((MainWindow*)mParent)->getDLFLWindow()->getActive()->setSelectedFaceColor(mSelectedFaceColor);
	// ((MainWindow*)mParent)->getDLFLWindow()->getActive()->setPatchBoundaryColor(mPatchBoundaryColor);
	((MainWindow*)mParent)->getDLFLWindow()->getActive()->setVertexIDBgColor(mVertexIDBgColor);
	((MainWindow*)mParent)->getDLFLWindow()->getActive()->setEdgeIDBgColor(mEdgeIDBgColor);
	((MainWindow*)mParent)->getDLFLWindow()->getActive()->setFaceIDBgColor(mFaceIDBgColor);
	
	((MainWindow*)mParent)->getDLFLWindow()->getActive()->setWireframeThickness(mWireframeThickness);	
	((MainWindow*)mParent)->getDLFLWindow()->getActive()->setSilhouetteThickness(mSilhouetteThickness);	
	((MainWindow*)mParent)->getDLFLWindow()->getActive()->setVertexThickness(mVertexThickness);	
	((MainWindow*)mParent)->getDLFLWindow()->getActive()->setSelectedVertexThickness(mSelectedVertexThickness);	
	((MainWindow*)mParent)->getDLFLWindow()->getActive()->setSelectedEdgeThickness(mSelectedEdgeThickness);	

}

void TopModPreferences::discardSettings(){
	reject();
}

void TopModPreferences::loadDefaults(){
	//load all the default settings and attach this as a signal for a reset button
	mViewportColor.setRgb(255,255,255);
	((MainWindow*)mParent)->getDLFLWindow()->getActive()->setViewportColor(mViewportColor);
	QPalette p = mViewportColorButton->palette();
	p.setColor(QPalette::Button, mViewportColor);
	mViewportColorButton->setPalette(p);

	mRenderColor.setRgb(255,255,255);
	((MainWindow*)mParent)->getDLFLWindow()->getActive()->setRenderColor(mRenderColor);
	p = mRenderColorButton->palette();
	p.setColor(QPalette::Button, mRenderColor);
	mRenderColorButton->setPalette(p);

	mCoolLightColor.setRgb(51,51,102);
	((MainWindow*)mParent)->getDLFLWindow()->setCoolLightColor(mCoolLightColor);
	p = mCoolLightColorButton->palette();
	p.setColor(QPalette::Button, mCoolLightColor);
	mCoolLightColorButton->setPalette(p);

	mWarmLightColor.setRgb(255,255,153);
	((MainWindow*)mParent)->getDLFLWindow()->setWarmLightColor(mWarmLightColor);
	p = mWarmLightColorButton->palette();
	p.setColor(QPalette::Button, mWarmLightColor);
	mWarmLightColorButton->setPalette(p);

	mWireframeColor.setRgb(0,1,0);
	((MainWindow*)mParent)->getDLFLWindow()->getActive()->setWireframeColor(mWireframeColor);
	p = mWireframeColorButton->palette();
	p.setColor(QPalette::Button, mWireframeColor);
	mWireframeColorButton->setPalette(p);

	mSilhouetteColor.setRgb(0,0,0);
	((MainWindow*)mParent)->getDLFLWindow()->getActive()->setSilhouetteColor(mSilhouetteColor);
	p = mSilhouetteColorButton->palette();
	p.setColor(QPalette::Button, mSilhouetteColor);
	mSilhouetteColorButton->setPalette(p);

	mPatchBoundaryColor.setRgb(0,0,0);
	// ((MainWindow*)mParent)->getDLFLWindow()->getActive()->setPatchBoundaryColor(mPatchBoundaryColor);
	p = mPatchBoundaryColorButton->palette();
	p.setColor(QPalette::Button, mPatchBoundaryColor);
	mPatchBoundaryColorButton->setPalette(p);
	
	mSelectedVertexColor.setRgb(0,127,0,127);
	((MainWindow*)mParent)->getDLFLWindow()->getActive()->setSelectedVertexColor(mSelectedVertexColor);
	p = mSelectedVertexColorButton->palette();
	p.setColor(QPalette::Button, mSelectedVertexColor);
	mSelectedVertexColorButton->setPalette(p);
	
	mSelectedEdgeColor.setRgb(127,0,0,127);
	((MainWindow*)mParent)->getDLFLWindow()->getActive()->setSelectedEdgeColor(mSelectedEdgeColor);
	p = mSelectedEdgeColorButton->palette();
	p.setColor(QPalette::Button, mSelectedEdgeColor);
	mSelectedEdgeColorButton->setPalette(p);
	
	mSelectedFaceColor.setRgb(0,0,127,127);
	((MainWindow*)mParent)->getDLFLWindow()->getActive()->setSelectedFaceColor(mSelectedFaceColor);
	p = mSelectedFaceColorButton->palette();
	p.setColor(QPalette::Button, mSelectedFaceColor);
	mSelectedFaceColorButton->setPalette(p);
	
	mVertexIDBgColor.setRgb(127,0,0,127);
	((MainWindow*)mParent)->getDLFLWindow()->getActive()->setVertexIDBgColor(mVertexIDBgColor);
	p = mVertexIDBgColorButton->palette();
	p.setColor(QPalette::Button, mVertexIDBgColor);
	mVertexIDBgColorButton->setPalette(p);
	
	mEdgeIDBgColor.setRgb(0,127,0,127);
	((MainWindow*)mParent)->getDLFLWindow()->getActive()->setEdgeIDBgColor(mEdgeIDBgColor);
	p = mEdgeIDBgColorButton->palette();
	p.setColor(QPalette::Button, mEdgeIDBgColor);
	mEdgeIDBgColorButton->setPalette(p);
	
	mFaceIDBgColor.setRgb(0,0,127,127);
	((MainWindow*)mParent)->getDLFLWindow()->getActive()->setFaceIDBgColor(mFaceIDBgColor);
	p = mFaceIDBgColorButton->palette();
	p.setColor(QPalette::Button, mFaceIDBgColor);
	mFaceIDBgColorButton->setPalette(p);
	
	mLightIntensity = 2.0;
	mLightIntensitySpinBox->setValue(mLightIntensity); 
	
	mWireframeThickness = 0.1;
	mWireframeThicknessSpinBox->setValue(mWireframeThickness);	
	
	mSilhouetteThickness = 6.0;
	mSilhouetteThicknessSpinBox->setValue(mSilhouetteThickness);
	
	mVertexThickness = 5.0;
	mVertexThicknessSpinBox->setValue(mVertexThickness);	
	
	mSelectedVertexThickness = 8.0;
	mVertexThicknessSpinBox->setValue(mSelectedVertexThickness);
	
	mSelectedEdgeThickness = 5.0;
	mSelectedEdgeThicknessSpinBox->setValue(mSelectedVertexThickness);
}

void TopModPreferences::setupMain(){

	QGridLayout *mMainLayout = new QGridLayout;
	
	mResetCameraButton = new QPushButton(tr("Reset"));
	connect(mResetCameraButton,SIGNAL(clicked()),
					((MainWindow*)mParent)->getDLFLWindow(), SLOT(switchPerspView()));
	
	mMainTab->setLayout(mMainLayout);
	mMainLayout->addWidget(mResetCameraButton,0,0);
	
	// persp->switchTo(VPPersp);
}

void TopModPreferences::setupColors(){
	
	//layout for colors tab page
	QGridLayout *mColorsLayout = new QGridLayout;
	//style for the color
	QStyle* colorPickerStyle = new QPlastiqueStyle;

	//viewport background color
	mViewportColorLabel = new QLabel(tr("Viewport Background:"));
	mViewportColorButton = new QPushButton(mColorsTab);
	mViewportColorButton->setMaximumSize(QSize(100,25));
	mViewportColorButton->setAutoFillBackground(true);
	mViewportColorButton->setStyle(colorPickerStyle);
	QPalette p = mViewportColorButton->palette();
	p.setColor(mViewportColorButton->backgroundRole(), mViewportColor);
	mViewportColorButton->setPalette(p);	
	connect(mViewportColorButton,SIGNAL(clicked()),this,SLOT(setViewportColor()));
	mColorsLayout->addWidget(mViewportColorLabel,0,0);
	mColorsLayout->addWidget(mViewportColorButton,0,1);

	//object render color
	mRenderColorLabel = new QLabel(tr("Object Render Color:"));
	mRenderColorButton = new QPushButton(mColorsTab);
	mRenderColorButton->setMaximumSize(QSize(100,25));
	mRenderColorButton->setAutoFillBackground(true);
	mRenderColorButton->setStyle(colorPickerStyle);
	p = mRenderColorButton->palette();
	p.setColor(mRenderColorButton->backgroundRole(), mRenderColor);
	mRenderColorButton->setPalette(p);	
	connect(mRenderColorButton,SIGNAL(clicked()),this,SLOT(setRenderColor()));
	mColorsLayout->addWidget(mRenderColorLabel,1,0);
	mColorsLayout->addWidget(mRenderColorButton,1,1);

	//CoolLight  color
	mCoolLightColorLabel = new QLabel(tr("Cool Light Color:"));
	mCoolLightColorButton = new QPushButton(mColorsTab);
	mCoolLightColorButton->setMaximumSize(QSize(100,25));
	mCoolLightColorButton->setAutoFillBackground(true);
	mCoolLightColorButton->setStyle(colorPickerStyle);
	p = mCoolLightColorButton->palette();
	p.setColor(mCoolLightColorButton->backgroundRole(), mCoolLightColor);
	mCoolLightColorButton->setPalette(p);	
	connect(mCoolLightColorButton,SIGNAL(clicked()),this,SLOT(setCoolLightColor()));
	mColorsLayout->addWidget(mCoolLightColorLabel,2,0);
	mColorsLayout->addWidget(mCoolLightColorButton,2,1);

	//WarmLight  color
	mWarmLightColorLabel = new QLabel(tr("Warm Light Color:"));
	mWarmLightColorButton = new QPushButton(mColorsTab);
	mWarmLightColorButton->setMaximumSize(QSize(100,25));
	mWarmLightColorButton->setAutoFillBackground(true);
	mWarmLightColorButton->setStyle(colorPickerStyle);
	p = mWarmLightColorButton->palette();
	p.setColor(mWarmLightColorButton->backgroundRole(), mWarmLightColor);
	mWarmLightColorButton->setPalette(p);	
	connect(mWarmLightColorButton,SIGNAL(clicked()),this,SLOT(setWarmLightColor()));
	mColorsLayout->addWidget(mWarmLightColorLabel,3,0);
	mColorsLayout->addWidget(mWarmLightColorButton,3,1);

	//Wireframe  color
	mWireframeColorLabel = new QLabel(tr("Wireframe Color:"));
	mWireframeColorButton = new QPushButton(mColorsTab);
	mWireframeColorButton->setMaximumSize(QSize(100,25));
	mWireframeColorButton->setAutoFillBackground(true);
	mWireframeColorButton->setStyle(colorPickerStyle);
	p = mWireframeColorButton->palette();
	p.setColor(mWireframeColorButton->backgroundRole(), mWireframeColor);
	mWireframeColorButton->setPalette(p);	
	connect(mWireframeColorButton,SIGNAL(clicked()),this,SLOT(setWireframeColor()));
	mColorsLayout->addWidget(mWireframeColorLabel,4,0);
	mColorsLayout->addWidget(mWireframeColorButton,4,1);

	//Silhouette  color
	mSilhouetteColorLabel = new QLabel(tr("Silhouette Color:"));
	mSilhouetteColorButton = new QPushButton(mColorsTab);
	mSilhouetteColorButton->setMaximumSize(QSize(100,25));
	mSilhouetteColorButton->setAutoFillBackground(true);
	mSilhouetteColorButton->setStyle(colorPickerStyle);
	p = mSilhouetteColorButton->palette();
	p.setColor(mSilhouetteColorButton->backgroundRole(), mSilhouetteColor);
	mSilhouetteColorButton->setPalette(p);	
	connect(mSilhouetteColorButton,SIGNAL(clicked()),this,SLOT(setSilhouetteColor()));
	mColorsLayout->addWidget(mSilhouetteColorLabel,5,0);
	mColorsLayout->addWidget(mSilhouetteColorButton,5,1);

	//PatchBoundary  color
	mPatchBoundaryColorLabel = new QLabel(tr("Patch Boundary Color:"));
	mPatchBoundaryColorButton = new QPushButton(mColorsTab);
	mPatchBoundaryColorButton->setMaximumSize(QSize(100,25));
	mPatchBoundaryColorButton->setAutoFillBackground(true);
	mPatchBoundaryColorButton->setStyle(colorPickerStyle);
	p = mPatchBoundaryColorButton->palette();
	p.setColor(mPatchBoundaryColorButton->backgroundRole(), mPatchBoundaryColor);
	mPatchBoundaryColorButton->setPalette(p);	
	connect(mPatchBoundaryColorButton,SIGNAL(clicked()),this,SLOT(setPatchBoundaryColor()));
	mColorsLayout->addWidget(mPatchBoundaryColorLabel,6,0);
	mColorsLayout->addWidget(mPatchBoundaryColorButton,6,1);
	
	//SelectedVertex  color
	mSelectedVertexColorLabel = new QLabel(tr("Selected Vertex Color:"));
	mSelectedVertexColorButton = new QPushButton(mColorsTab);
	mSelectedVertexColorButton->setMaximumSize(QSize(100,25));
	mSelectedVertexColorButton->setAutoFillBackground(true);
	mSelectedVertexColorButton->setStyle(colorPickerStyle);
	p = mSelectedVertexColorButton->palette();
	p.setColor(mSelectedVertexColorButton->backgroundRole(), mSelectedVertexColor);
	mSelectedVertexColorButton->setPalette(p);	
	connect(mSelectedVertexColorButton,SIGNAL(clicked()),this,SLOT(setSelectedVertexColor()));
	mColorsLayout->addWidget(mSelectedVertexColorLabel,7,0);
	mColorsLayout->addWidget(mSelectedVertexColorButton,7,1);
	
	//SelectedEdge  color
	mSelectedEdgeColorLabel = new QLabel(tr("Selected Edge Color:"));
	mSelectedEdgeColorButton = new QPushButton(mColorsTab);
	mSelectedEdgeColorButton->setMaximumSize(QSize(100,25));
	mSelectedEdgeColorButton->setAutoFillBackground(true);
	mSelectedEdgeColorButton->setStyle(colorPickerStyle);
	p = mSelectedEdgeColorButton->palette();
	p.setColor(mSelectedEdgeColorButton->backgroundRole(), mSelectedEdgeColor);
	mSelectedEdgeColorButton->setPalette(p);	
	connect(mSelectedEdgeColorButton,SIGNAL(clicked()),this,SLOT(setSelectedEdgeColor()));
	mColorsLayout->addWidget(mSelectedEdgeColorLabel,8,0);
	mColorsLayout->addWidget(mSelectedEdgeColorButton,8,1);
	
	//SelectedFace  color
	mSelectedFaceColorLabel = new QLabel(tr("Selected Face Color:"));
	mSelectedFaceColorButton = new QPushButton(mColorsTab);
	mSelectedFaceColorButton->setMaximumSize(QSize(100,25));
	mSelectedFaceColorButton->setAutoFillBackground(true);
	mSelectedFaceColorButton->setStyle(colorPickerStyle);
	p = mSelectedFaceColorButton->palette();
	p.setColor(mSelectedFaceColorButton->backgroundRole(), mSelectedFaceColor);
	mSelectedFaceColorButton->setPalette(p);	
	connect(mSelectedFaceColorButton,SIGNAL(clicked()),this,SLOT(setSelectedFaceColor()));
	mColorsLayout->addWidget(mSelectedFaceColorLabel,9,0);
	mColorsLayout->addWidget(mSelectedFaceColorButton,9,1);
	
	//VertexIDBg  color
	mVertexIDBgColorLabel = new QLabel(tr("Vertex ID Bg Color:"));
	mVertexIDBgColorButton = new QPushButton(mColorsTab);
	mVertexIDBgColorButton->setMaximumSize(QSize(100,25));
	mVertexIDBgColorButton->setAutoFillBackground(true);
	mVertexIDBgColorButton->setStyle(colorPickerStyle);
	p = mVertexIDBgColorButton->palette();
	p.setColor(mVertexIDBgColorButton->backgroundRole(), mVertexIDBgColor);
	mVertexIDBgColorButton->setPalette(p);	
	connect(mVertexIDBgColorButton,SIGNAL(clicked()),this,SLOT(setVertexIDBgColor()));
	mColorsLayout->addWidget(mVertexIDBgColorLabel,10,0);
	mColorsLayout->addWidget(mVertexIDBgColorButton,10,1);
	
	//FaceIDBg  color
	mFaceIDBgColorLabel = new QLabel(tr("Face ID Bg Color:"));
	mFaceIDBgColorButton = new QPushButton(mColorsTab);
	mFaceIDBgColorButton->setMaximumSize(QSize(100,25));
	mFaceIDBgColorButton->setAutoFillBackground(true);
	mFaceIDBgColorButton->setStyle(colorPickerStyle);
	p = mFaceIDBgColorButton->palette();
	p.setColor(mFaceIDBgColorButton->backgroundRole(), mFaceIDBgColor);
	mFaceIDBgColorButton->setPalette(p);	
	connect(mFaceIDBgColorButton,SIGNAL(clicked()),this,SLOT(setFaceIDBgColor()));
	mColorsLayout->addWidget(mFaceIDBgColorLabel,11,0);
	mColorsLayout->addWidget(mFaceIDBgColorButton,11,1);
	
	//EdgeIDBg  color
	mEdgeIDBgColorLabel = new QLabel(tr("Edge ID Bg Color:"));
	mEdgeIDBgColorButton = new QPushButton(mColorsTab);
	mEdgeIDBgColorButton->setMaximumSize(QSize(100,25));
	mEdgeIDBgColorButton->setAutoFillBackground(true);
	mEdgeIDBgColorButton->setStyle(colorPickerStyle);
	p = mEdgeIDBgColorButton->palette();
	p.setColor(mEdgeIDBgColorButton->backgroundRole(), mEdgeIDBgColor);
	mEdgeIDBgColorButton->setPalette(p);	
	connect(mEdgeIDBgColorButton,SIGNAL(clicked()),this,SLOT(setEdgeIDBgColor()));
	mColorsLayout->addWidget(mEdgeIDBgColorLabel,12,0);
	mColorsLayout->addWidget(mEdgeIDBgColorButton,12,1);
	
	//light intensity
	mLightIntensityLabel = new QLabel(tr("Light Intensity:"));
	mLightIntensitySpinBox = new QDoubleSpinBox;
	mLightIntensitySpinBox = new QDoubleSpinBox;
	mLightIntensitySpinBox->setRange(0, 10.0);
	mLightIntensitySpinBox->setSingleStep(0.1);
	mLightIntensitySpinBox->setValue(2.0);
	mLightIntensitySpinBox->setDecimals(1);
	mLightIntensitySpinBox->setMaximumSize(100,25);
	connect(mLightIntensitySpinBox, SIGNAL(valueChanged(double)),
          ((MainWindow*)mParent)->getDLFLWindow(), SLOT(setLightIntensity(double)));
	
	mColorsLayout->addWidget(mLightIntensityLabel,0,2);
	mColorsLayout->addWidget(mLightIntensitySpinBox,0,3);
	
	//wireframe thickness
	mWireframeThicknessLabel = new QLabel(tr("Wireframe Thickness (px):"));
	mWireframeThicknessSpinBox = new QDoubleSpinBox;
	mWireframeThicknessSpinBox = new QDoubleSpinBox;
	mWireframeThicknessSpinBox->setRange(0.1, 30.0);
	mWireframeThicknessSpinBox->setSingleStep(0.1);
	mWireframeThicknessSpinBox->setValue(0.1);
	mWireframeThicknessSpinBox->setDecimals(1);
	mWireframeThicknessSpinBox->setMaximumSize(100,25);
	connect(mWireframeThicknessSpinBox, SIGNAL(valueChanged(double)),
          ((MainWindow*)mParent)->getDLFLWindow()->getActive(), SLOT(setWireframeThickness(double)));
	//init the light intensity here:
	
	mColorsLayout->addWidget(mWireframeThicknessLabel,1,2);
	mColorsLayout->addWidget(mWireframeThicknessSpinBox,1,3);
	
	//vertex thickness
	mVertexThicknessLabel = new QLabel(tr("Vertex Thickness:"));
	mVertexThicknessSpinBox = new QDoubleSpinBox;
	mVertexThicknessSpinBox = new QDoubleSpinBox;
	mVertexThicknessSpinBox->setRange(0.1, 10.0);
	mVertexThicknessSpinBox->setSingleStep(0.1);
	mVertexThicknessSpinBox->setValue(5.0);
	mVertexThicknessSpinBox->setDecimals(1);
	mVertexThicknessSpinBox->setMaximumSize(100,25);
	connect(mVertexThicknessSpinBox, SIGNAL(valueChanged(double)),
          ((MainWindow*)mParent)->getDLFLWindow()->getActive(), SLOT(setVertexThickness(double)));
	
	mColorsLayout->addWidget(mVertexThicknessLabel,2,2);
	mColorsLayout->addWidget(mVertexThicknessSpinBox,2,3);
	
	//silhouette thickness
	mSilhouetteThicknessLabel = new QLabel(tr("Silhouette Thickness:"));
	mSilhouetteThicknessSpinBox = new QDoubleSpinBox;
	mSilhouetteThicknessSpinBox = new QDoubleSpinBox;
	mSilhouetteThicknessSpinBox->setRange(0, 15.0);
	mSilhouetteThicknessSpinBox->setSingleStep(0.2);
	mSilhouetteThicknessSpinBox->setValue(8.0);
	mSilhouetteThicknessSpinBox->setDecimals(1);
	mSilhouetteThicknessSpinBox->setMaximumSize(100,25);
	connect(mSilhouetteThicknessSpinBox, SIGNAL(valueChanged(double)),
          ((MainWindow*)mParent)->getDLFLWindow()->getActive(), SLOT(setSilhouetteThickness(double)));
	mColorsLayout->addWidget(mSilhouetteThicknessLabel,3,2);
	mColorsLayout->addWidget(mSilhouetteThicknessSpinBox,3,3);
	
	//SelectedVertex thickness
	mSelectedVertexThicknessLabel = new QLabel(tr("Selected Vertex Thickness:"));
	mSelectedVertexThicknessSpinBox = new QDoubleSpinBox;
	mSelectedVertexThicknessSpinBox = new QDoubleSpinBox;
	mSelectedVertexThicknessSpinBox->setRange(0, 15.0);
	mSelectedVertexThicknessSpinBox->setSingleStep(0.2);
	mSelectedVertexThicknessSpinBox->setValue(8.0);
	mSelectedVertexThicknessSpinBox->setDecimals(1);
	mSelectedVertexThicknessSpinBox->setMaximumSize(100,25);
	connect(mSelectedVertexThicknessSpinBox, SIGNAL(valueChanged(double)),
          ((MainWindow*)mParent)->getDLFLWindow()->getActive(), SLOT(setSelectedVertexThickness(double)));
	mColorsLayout->addWidget(mSelectedVertexThicknessLabel,4,2);
	mColorsLayout->addWidget(mSelectedVertexThicknessSpinBox,4,3);

	//SelectedEdge thickness
	mSelectedEdgeThicknessLabel = new QLabel(tr("Selected Edge Thickness:"));
	mSelectedEdgeThicknessSpinBox = new QDoubleSpinBox;
	mSelectedEdgeThicknessSpinBox = new QDoubleSpinBox;
	mSelectedEdgeThicknessSpinBox->setRange(0, 15.0);
	mSelectedEdgeThicknessSpinBox->setSingleStep(0.2);
	mSelectedEdgeThicknessSpinBox->setValue(5.0);
	mSelectedEdgeThicknessSpinBox->setDecimals(1);
	mSelectedEdgeThicknessSpinBox->setMaximumSize(100,25);
	connect(mSelectedEdgeThicknessSpinBox, SIGNAL(valueChanged(double)),
          ((MainWindow*)mParent)->getDLFLWindow()->getActive(), SLOT(setSelectedEdgeThickness(double)));
	mColorsLayout->addWidget(mSelectedEdgeThicknessLabel,5,2);
	mColorsLayout->addWidget(mSelectedEdgeThicknessSpinBox,5,3);

	mResetColorsButton = new QPushButton(tr("Reset"));
	connect(mResetColorsButton,SIGNAL(clicked()),this, SLOT(loadDefaults()));
	mColorsLayout->addWidget(mResetColorsButton,9,3);
	
	// mColorsLayout->addStretch(1);
	mColorsTab->setLayout(mColorsLayout);	
	
	
	
}

void TopModPreferences::setViewportColor(){
	mViewportColor.setRgba(QColorDialog::getRgba(mViewportColor.rgba()));
	if (mViewportColor.isValid()){
		((MainWindow*)mParent)->getDLFLWindow()->getActive()->setViewportColor(mViewportColor);
		QPalette p = mViewportColorButton->palette();
		p.setColor(QPalette::Button, mViewportColor);
		mViewportColorButton->setPalette(p);
	}
}

void TopModPreferences::setRenderColor(){
	mRenderColor.setRgba(QColorDialog::getRgba(mRenderColor.rgba()));
	if (mRenderColor.isValid()){
		((MainWindow*)mParent)->getDLFLWindow()->getActive()->setRenderColor(mRenderColor);
		QPalette p = mRenderColorButton->palette();
		p.setColor(QPalette::Button, mRenderColor);
		mRenderColorButton->setPalette(p);
	}
}

void TopModPreferences::setCoolLightColor(){
	mCoolLightColor.setRgba(QColorDialog::getRgba(mCoolLightColor.rgba()));
	if (mCoolLightColor.isValid()){
		((MainWindow*)mParent)->getDLFLWindow()->setCoolLightColor(mCoolLightColor);
		QPalette p = mCoolLightColorButton->palette();
		p.setColor(QPalette::Button, mCoolLightColor);
		mCoolLightColorButton->setPalette(p);
	}
}

void TopModPreferences::setWarmLightColor(){
	mWarmLightColor.setRgba(QColorDialog::getRgba(mWarmLightColor.rgba()));
	if (mWarmLightColor.isValid()){
		((MainWindow*)mParent)->getDLFLWindow()->setWarmLightColor(mWarmLightColor);
		QPalette p = mWarmLightColorButton->palette();
		p.setColor(QPalette::Button, mWarmLightColor);
		mWarmLightColorButton->setPalette(p);
	}
}

void TopModPreferences::setWireframeColor(){
	mWireframeColor.setRgba(QColorDialog::getRgba(mWireframeColor.rgba()));
	if (mWireframeColor.isValid()){
		((MainWindow*)mParent)->getDLFLWindow()->getActive()->setWireframeColor(mWireframeColor);
		QPalette p = mWireframeColorButton->palette();
		p.setColor(QPalette::Button, mWireframeColor);
		mWireframeColorButton->setPalette(p);
	}
}

void TopModPreferences::setSilhouetteColor(){
	mSilhouetteColor.setRgba(QColorDialog::getRgba(mSilhouetteColor.rgba()));
	if (mSilhouetteColor.isValid()){
		((MainWindow*)mParent)->getDLFLWindow()->getActive()->setSilhouetteColor(mSilhouetteColor);
		QPalette p = mSilhouetteColorButton->palette();
		p.setColor(QPalette::Button, mSilhouetteColor);
		mSilhouetteColorButton->setPalette(p);
	}
}

void TopModPreferences::setPatchBoundaryColor(){
	mPatchBoundaryColor.setRgba(QColorDialog::getRgba(mPatchBoundaryColor.rgba()));
	if (mPatchBoundaryColor.isValid()){
		// ((MainWindow*)mParent)->getDLFLWindow()->getActive()->setPatchBoundaryColor(mPatchBoundaryColor);
		QPalette p = mPatchBoundaryColorButton->palette();
		p.setColor(QPalette::Button, mPatchBoundaryColor);
		mPatchBoundaryColorButton->setPalette(p);
	}
}

void TopModPreferences::setSelectedVertexColor(){
	mSelectedVertexColor.setRgba(QColorDialog::getRgba(mSelectedVertexColor.rgba()));
	if (mSelectedVertexColor.isValid()){
		((MainWindow*)mParent)->getDLFLWindow()->getActive()->setSelectedVertexColor(mSelectedVertexColor);
		QPalette p = mSelectedVertexColorButton->palette();
		p.setColor(QPalette::Button, mSelectedVertexColor);
		mSelectedVertexColorButton->setPalette(p);
	}
}

void TopModPreferences::setSelectedEdgeColor(){
	mSelectedEdgeColor.setRgba(QColorDialog::getRgba(mSelectedEdgeColor.rgba()));
	if (mSelectedEdgeColor.isValid()){
		((MainWindow*)mParent)->getDLFLWindow()->getActive()->setSelectedEdgeColor(mSelectedEdgeColor);
		QPalette p = mSelectedEdgeColorButton->palette();
		p.setColor(QPalette::Button, mSelectedEdgeColor);
		mSelectedEdgeColorButton->setPalette(p);
	}
}

void TopModPreferences::setSelectedFaceColor(){
	mSelectedFaceColor.setRgba(QColorDialog::getRgba(mSelectedFaceColor.rgba()));
	if (mSelectedFaceColor.isValid()){
		((MainWindow*)mParent)->getDLFLWindow()->getActive()->setSelectedFaceColor(mSelectedFaceColor);
		QPalette p = mSelectedFaceColorButton->palette();
		p.setColor(QPalette::Button, mSelectedFaceColor);
		mSelectedFaceColorButton->setPalette(p);
	}
}

void TopModPreferences::setFaceIDBgColor(){
	mFaceIDBgColor.setRgba(QColorDialog::getRgba(mFaceIDBgColor.rgba()));
	if (mFaceIDBgColor.isValid()){
		((MainWindow*)mParent)->getDLFLWindow()->getActive()->setFaceIDBgColor(mFaceIDBgColor);
		QPalette p = mFaceIDBgColorButton->palette();
		p.setColor(QPalette::Button, mFaceIDBgColor);
		mFaceIDBgColorButton->setPalette(p);
	}
}

void TopModPreferences::setVertexIDBgColor(){
	mVertexIDBgColor.setRgba(QColorDialog::getRgba(mVertexIDBgColor.rgba()));
	if (mVertexIDBgColor.isValid()){
		((MainWindow*)mParent)->getDLFLWindow()->getActive()->setVertexIDBgColor(mVertexIDBgColor);
		QPalette p = mVertexIDBgColorButton->palette();
		p.setColor(QPalette::Button, mVertexIDBgColor);
		mVertexIDBgColorButton->setPalette(p);
	}
}

void TopModPreferences::setEdgeIDBgColor(){
	mEdgeIDBgColor.setRgba(QColorDialog::getRgba(mEdgeIDBgColor.rgba()));
	if (mEdgeIDBgColor.isValid()){
		((MainWindow*)mParent)->getDLFLWindow()->getActive()->setEdgeIDBgColor(mEdgeIDBgColor);
		QPalette p = mEdgeIDBgColorButton->palette();
		p.setColor(QPalette::Button, mEdgeIDBgColor);
		mEdgeIDBgColorButton->setPalette(p);
	}
}