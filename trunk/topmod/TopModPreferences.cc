#include "TopModPreferences.hh"

TopModPreferences::TopModPreferences(QSettings *settings, StyleSheetEditor *sse, QShortcutManager *sm, QWidget *parent) 
:	QDialog(parent) {
	
	mSettings = settings;
	mParent = parent;
	setSizeGripEnabled(false);
	resize(600,400);
	setMinimumSize(600,400);
	setWindowTitle(tr("TopMod Preferences"));
	setWindowFlags(Qt::FramelessWindowHint | Qt::Dialog);
	
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
	mSettings->setValue("LightIntensity",mLightIntensity);
	mSettings->endGroup();
	
	mSettings->beginGroup("Display");
	mSettings->setValue("WireframeThickness", mWireframeThickness);
	mSettings->setValue("SilhouetteThickness", mSilhouetteThickness);
	mSettings->setValue("VertexThickness", mVertexThickness);
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
	mWireframeColor = mSettings->value("WireframeColor", QColor(0,1,0)).value<QColor>();
	mSilhouetteColor = mSettings->value("SilhouetteColor", QColor(0,0,0)).value<QColor>();
	mPatchBoundaryColor = mSettings->value("PatchBoundaryColor", QColor(0,0,0)).value<QColor>();
	mLightIntensity = mSettings->value("LightIntensity", 2.0).toDouble();
	mSettings->endGroup();
	
	mSettings->beginGroup("Display");
	mWireframeThickness = mSettings->value("WireframeThickness", 5.0).toDouble();
	mSilhouetteThickness = mSettings->value("SilhouetteThickness", 6.0).toDouble();
	mVertexThickness = mSettings->value("VertexThickness",5.0).toDouble();
	mSettings->endGroup();
	//initialize the settings
	
	((MainWindow*)mParent)->getDLFLWindow()->getActive()->setViewportColor(mViewportColor);
	((MainWindow*)mParent)->getDLFLWindow()->getActive()->setRenderColor(mRenderColor);
	((MainWindow*)mParent)->getDLFLWindow()->setCoolLightColor(mCoolLightColor);
	((MainWindow*)mParent)->getDLFLWindow()->setWarmLightColor(mWarmLightColor);
	((MainWindow*)mParent)->getDLFLWindow()->getActive()->setWireframeColor(mWireframeColor);
	((MainWindow*)mParent)->getDLFLWindow()->getActive()->setSilhouetteColor(mSilhouetteColor);
	// ((MainWindow*)mParent)->getDLFLWindow()->getActive()->setPatchBoundaryColor(mPatchBoundaryColor);
	
	((MainWindow*)mParent)->getDLFLWindow()->getActive()->setWireframeThickness(mWireframeThickness);	
	((MainWindow*)mParent)->getDLFLWindow()->getActive()->setSilhouetteThickness(mSilhouetteThickness);	
	((MainWindow*)mParent)->getDLFLWindow()->getActive()->setVertexThickness(mVertexThickness);	

}

void TopModPreferences::discardSettings(){
	reject();
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
	mWireframeThicknessSpinBox->setValue(1.5);
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
	

	// mColorsLayout->addStretch(1);
	mColorsTab->setLayout(mColorsLayout);	
}

void TopModPreferences::setViewportColor(){
	QColorDialog::setCustomColor(1,mViewportColor.rgb());
	mViewportColor = QColorDialog::getColor();
	if (mViewportColor.isValid()){
		((MainWindow*)mParent)->getDLFLWindow()->getActive()->setViewportColor(mViewportColor);
		QPalette p = mViewportColorButton->palette();
		p.setColor(QPalette::Button, mViewportColor);
		mViewportColorButton->setPalette(p);
	}
}

void TopModPreferences::setRenderColor(){
	mRenderColor = QColorDialog::getRgba();
	if (mRenderColor.isValid()){
		((MainWindow*)mParent)->getDLFLWindow()->getActive()->setRenderColor(mRenderColor);
		QPalette p = mRenderColorButton->palette();
		p.setColor(QPalette::Button, mRenderColor);
		mRenderColorButton->setPalette(p);
	}
}

void TopModPreferences::setCoolLightColor(){
	mCoolLightColor = QColorDialog::getRgba();
	if (mCoolLightColor.isValid()){
		((MainWindow*)mParent)->getDLFLWindow()->setCoolLightColor(mCoolLightColor);
		QPalette p = mCoolLightColorButton->palette();
		p.setColor(QPalette::Button, mCoolLightColor);
		mCoolLightColorButton->setPalette(p);
	}
}

void TopModPreferences::setWarmLightColor(){
	mWarmLightColor = QColorDialog::getRgba();
	if (mWarmLightColor.isValid()){
		((MainWindow*)mParent)->getDLFLWindow()->setWarmLightColor(mWarmLightColor);
		QPalette p = mWarmLightColorButton->palette();
		p.setColor(QPalette::Button, mWarmLightColor);
		mWarmLightColorButton->setPalette(p);
	}
}

void TopModPreferences::setWireframeColor(){
	mWireframeColor = QColorDialog::getRgba();
	if (mWireframeColor.isValid()){
		((MainWindow*)mParent)->getDLFLWindow()->getActive()->setWireframeColor(mWireframeColor);
		QPalette p = mWireframeColorButton->palette();
		p.setColor(QPalette::Button, mWireframeColor);
		mWireframeColorButton->setPalette(p);
	}
}

void TopModPreferences::setSilhouetteColor(){
	mSilhouetteColor = QColorDialog::getRgba();
	if (mSilhouetteColor.isValid()){
		((MainWindow*)mParent)->getDLFLWindow()->getActive()->setSilhouetteColor(mSilhouetteColor);
		QPalette p = mSilhouetteColorButton->palette();
		p.setColor(QPalette::Button, mSilhouetteColor);
		mSilhouetteColorButton->setPalette(p);
	}
}

void TopModPreferences::setPatchBoundaryColor(){
	mPatchBoundaryColor = QColorDialog::getRgba();
	if (mPatchBoundaryColor.isValid()){
		// ((MainWindow*)mParent)->getDLFLWindow()->getActive()->setPatchBoundaryColor(mPatchBoundaryColor);
		QPalette p = mPatchBoundaryColorButton->palette();
		p.setColor(QPalette::Button, mPatchBoundaryColor);
		mPatchBoundaryColorButton->setPalette(p);
	}
}
