#include "TopModPreferences.hh"

TopModPreferences::TopModPreferences(QSettings *settings, StyleSheetEditor *sse, QShortcutManager *sm, QWidget *parent) 
:	QDialog(parent) {
	
	mSettings = settings;
	mParent = parent;
	setSizeGripEnabled(false);
	resize(700,600);
	setFixedSize(700,600);
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
#ifdef WITH_PYTHON
	mScriptEditTab = new QWidget;
	mPrefTabs->addTab(mScriptEditTab,tr("Script Editor"));
#endif
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
	mSettings->setValue("NormalColor",mNormalColor);
	mSettings->setValue("FaceCentroidColor",mFaceCentroidColor);
	mSettings->setValue("FaceIDBgColor",mFaceIDBgColor);
	mSettings->setValue("LightIntensity",mLightIntensity);
	mSettings->endGroup();
	
	mSettings->beginGroup("Display");
	mSettings->setValue("NormalThickness", mNormalThickness);
	mSettings->setValue("FaceCentroidThickness", mFaceCentroidThickness);
	mSettings->setValue("NormalLength", mNormalLength);
	mSettings->setValue("SilhouetteThickness", mSilhouetteThickness);
	mSettings->setValue("VertexThickness", mVertexThickness);
	mSettings->setValue("SelectedVertexThickness", mSelectedVertexThickness);
	mSettings->setValue("SelectedEdgeThickness", mSelectedEdgeThickness);
	mSettings->endGroup();

	mSettings->beginGroup("Camera");
	mSettings->setValue("NearPlane",mCameraNearPlaneSpinBox->value());
	mSettings->setValue("FarPlane",mCameraFarPlaneSpinBox->value());	
	mSettings->setValue("Fov",mCameraFovSpinBox->value());
	mSettings->endGroup();
	
	mSettings->beginGroup("Save Options");
	mSettings->setValue("SaveDirectory", mSaveDirectoryLineEdit->text());
	mSettings->setValue("AutoSave", mAutoSaveCheckBox->checkState());
	mSettings->setValue("AutoSaveDelay", mAutoSaveDelaySpinBox->value());
	mSettings->setValue("IncrementalSave", mIncrementalSaveCheckBox->checkState());
	mSettings->setValue("IncrementalSaveMax", mIncrementalSaveMaxSpinBox->value());
	mSettings->endGroup();
	
	mSettings->beginGroup("MainWindow");
	mSettings->setValue("pos", ((MainWindow*)mParent)->pos());
	mSettings->setValue("size", ((MainWindow*)mParent)->size());
	// std::cout << "size = " << ((MainWindow*)mParent)->size().width();
	mSettings->setValue("showStartupDialogAtStartup", ((MainWindow*)mParent)->getShowStartupDialogAtStartup());
	mSettings->setValue("toolOptionsPos", ((MainWindow*)mParent)->mToolOptionsDockWidget->pos());
	mSettings->setValue("CommandCompleterIndex", mCommandCompleterIndexToggle->checkState());
	
	#ifdef WITH_PYTHON
	mSettings->setValue("scriptEditorPos", ((MainWindow*)mParent)->mScriptEditorDockWidget->pos());
	mSettings->setValue("scriptEditorSize", ((MainWindow*)mParent)->mScriptEditorDockWidget->size());
	#endif
	mSettings->endGroup();
	
	accept();
}

void TopModPreferences::readSettings(){

	//load the settings file and set defaults if values don't exist
	mSettings->beginGroup("Colors");
	mViewportColorDefault = QColor(67,68,88); //this is where you set defaults now. only one time in the code it
	mViewportColor = mSettings->value("ViewportColor", mViewportColorDefault).value<QColor>();
	mRenderColorDefault = QColor(255,151,92);
	mRenderColor = mSettings->value("RenderColor", mRenderColorDefault).value<QColor>();
	mCoolLightColorDefault = QColor(27,26,23);
	mCoolLightColor = mSettings->value("CoolLightColor",mCoolLightColorDefault).value<QColor>();
	mWarmLightColorDefault = QColor(255,250,226);
	mWarmLightColor = mSettings->value("WarmLightColor",mWarmLightColorDefault).value<QColor>();
	mWireframeColorDefault = QColor(0,0,0,127);
	mWireframeColor = mSettings->value("WireframeColor", mWireframeColorDefault).value<QColor>();
	mSilhouetteColorDefault = QColor(0,0,0,127);
	mSilhouetteColor = mSettings->value("SilhouetteColor", mSilhouetteColorDefault).value<QColor>();
	mPatchBoundaryColorDefault = QColor(0,0,0);
	mPatchBoundaryColor = mSettings->value("PatchBoundaryColor", mPatchBoundaryColor).value<QColor>();
	mSelectedVertexColorDefault = QColor(0,255,0,127);
	mSelectedVertexColor = mSettings->value("SelectedVertexColor", mSelectedVertexColorDefault).value<QColor>();
	mSelectedEdgeColorDefault = QColor(255,0,0,127);
	mSelectedEdgeColor = mSettings->value("SelectedEdgeColor", mSelectedEdgeColorDefault).value<QColor>();
	mSelectedFaceColorDefault = QColor(247,0,229,127);
	mSelectedFaceColor = mSettings->value("SelectedFaceColor", mSelectedFaceColorDefault).value<QColor>();
	mVertexIDBgColorDefault = QColor(127,0,0,127);
	mVertexIDBgColor = mSettings->value("VertexIDBgColor", mVertexIDBgColorDefault).value<QColor>();
	mEdgeIDBgColorDefault = QColor(0,127,0,127);
	mEdgeIDBgColor = mSettings->value("EdgeIDBgColor", mEdgeIDBgColorDefault).value<QColor>();
	mNormalColorDefault = QColor(127,127,0);
	mNormalColor = mSettings->value("NormalColor", mNormalColorDefault).value<QColor>();
	mFaceCentroidColorDefault = QColor(127,0,127);
	mFaceCentroidColor = mSettings->value("FaceCentroidColor", mFaceCentroidColorDefault).value<QColor>();
	mFaceIDBgColorDefault = QColor(0,0,127,127);
	mFaceIDBgColor = mSettings->value("FaceIDBgColor", mFaceIDBgColorDefault).value<QColor>();
	mSettings->endGroup();
	
	mSettings->beginGroup("Display");
	mLightIntensityDefault = 2.0;
	mLightIntensity = mSettings->value("LightIntensity", mLightIntensityDefault).toDouble();
	mWireframeThicknessDefault = 0.1;
	mWireframeThickness = mSettings->value("WireframeThickness", mWireframeThicknessDefault).toDouble();
	mSilhouetteThicknessDefault = 6.0;
	mSilhouetteThickness = mSettings->value("SilhouetteThickness", mSilhouetteThicknessDefault).toDouble();
	mVertexThicknessDefault = 5.0;
	mVertexThickness = mSettings->value("VertexThickness",mVertexThicknessDefault).toDouble();
	mSelectedVertexThicknessDefault = 8.0;
	mSelectedVertexThickness = mSettings->value("SelectedVertexThickness", mSelectedVertexThicknessDefault).toDouble();
	mSelectedEdgeThicknessDefault = 5.0;
	mSelectedEdgeThickness = mSettings->value("SelectedEdgeThickness", mSelectedEdgeThicknessDefault).toDouble();
	mNormalThicknessDefault = 0.1;
	mNormalThickness = mSettings->value("NormalThickness", mNormalThicknessDefault).toDouble();
	mNormalLengthDefault = 1.0;
	mNormalLength = mSettings->value("NormalLength", mNormalLengthDefault).toDouble();
	mFaceCentroidThicknessDefault = 5.0;
	mFaceCentroidThickness = mSettings->value("FaceCentroidThickness", mFaceCentroidThicknessDefault).toDouble();
	mSettings->endGroup();
	
	mSettings->beginGroup("Camera");
	mCameraNearPlaneDefault = 1.0;
	mCameraNearPlane = mSettings->value("NearPlane", mCameraNearPlaneDefault).toDouble();
	mCameraFarPlaneDefault = 200;
	mCameraFarPlane = mSettings->value("FarPlane", mCameraFarPlaneDefault).toDouble();
	mCameraFovDefault = 60;
	mCameraFov = mSettings->value("Fov", mCameraFovDefault).toDouble();
	mSettings->endGroup();
	
	mSettings->beginGroup("Save Options");
	// std::cout<< QDir::currentPath().toLatin1().data() << "\n";
	// mSaveDirectory = QDir::currentPath();
	mSaveDirectoryDefault = QDir::currentPath();
	mSaveDirectory = mSettings->value("SaveDirectory", mSaveDirectoryDefault).toString();
	// std::cout<< mSaveDirectory.toLatin1().data() << "\n";
	mAutoSaveDefault = false;
	mAutoSave = mSettings->value("AutoSave", mAutoSaveDefault).toBool();
	mAutoSaveDelayDefault = 5;
	mAutoSaveDelay = mSettings->value("AutoSaveDelay", mAutoSaveDelayDefault).toDouble();
	mIncrementalSaveDefault = false;
	mIncrementalSave = mSettings->value("IncrementalSave", mIncrementalSaveDefault).toBool();
	mIncrementalSaveMaxDefault = 20;
	mIncrementalSaveMax = mSettings->value("IncrementalSaveMax", mIncrementalSaveMaxDefault).toDouble();
	mSettings->endGroup();

	mSettings->beginGroup("MainWindow");
	QPoint pos = mSettings->value("pos", QPoint(100, 100)).toPoint();
	if (pos.y()==0) pos.setY(pos.y()+20);
	QSize size = mSettings->value("size", QSize(800, 600)).toSize();
	QPoint toolOptionsPos = mSettings->value("toolOptionsPos", QPoint()).toPoint();
	
	mCommandCompleterIndexDefault = false;
	mCommandCompleterIndex = mSettings->value("CommandCompleterIndex", mCommandCompleterIndexDefault).toBool();
	
	#ifdef WITH_PYTHON
	QSize scriptEditorSize = mSettings->value("scriptEditorSize", QSize(500,300)).toSize();
	QPoint scriptEditorPos = mSettings->value("scriptEditorPos", QPoint(20, QApplication::desktop()->height()-20)).toPoint();	
	#endif
	mShowStartupDialogAtStartup = mSettings->value("showStartupDialogAtStartup", false).toBool();
	mSettings->endGroup();

	((MainWindow*)mParent)->resize(size.width(),size.height());
	((MainWindow*)mParent)->move(pos);
	((MainWindow*)mParent)->setShowStartupDialogAtStartup((int)mShowStartupDialogAtStartup);
	((MainWindow*)mParent)->mToolOptionsDockWidget->setGeometry(10 + ((MainWindow*)mParent)->x(),((MainWindow*)mParent)->y()+150,((MainWindow*)mParent)->mToolOptionsDockWidget->width(),((MainWindow*)mParent)->mToolOptionsDockWidget->height());	
	#ifdef WITH_PYTHON
	((MainWindow*)mParent)->mScriptEditorDockWidget->resize(scriptEditorSize.width(),scriptEditorSize.height());
	((MainWindow*)mParent)->mScriptEditorDockWidget->move(scriptEditorPos);	
	#endif
	
	//initialize the settings
	((MainWindow*)mParent)->getActive()->setViewportColor(mViewportColor);
	((MainWindow*)mParent)->getActive()->setRenderColor(mRenderColor);
	((MainWindow*)mParent)->getActive()->setCoolLightColor(mCoolLightColor);
	((MainWindow*)mParent)->getActive()->setWarmLightColor(mWarmLightColor);
	((MainWindow*)mParent)->getActive()->setWireframeColor(mWireframeColor);
	((MainWindow*)mParent)->getActive()->setSilhouetteColor(mSilhouetteColor);
	((MainWindow*)mParent)->getActive()->setSelectedVertexColor(mSelectedVertexColor);
	((MainWindow*)mParent)->getActive()->setSelectedEdgeColor(mSelectedEdgeColor);
	((MainWindow*)mParent)->getActive()->setSelectedFaceColor(mSelectedFaceColor);
	// ((MainWindow*)mParent)->getActive()->setPatchBoundaryColor(mPatchBoundaryColor);
	((MainWindow*)mParent)->getActive()->setVertexIDBgColor(mVertexIDBgColor);
	((MainWindow*)mParent)->getActive()->setEdgeIDBgColor(mEdgeIDBgColor);
	((MainWindow*)mParent)->getActive()->setNormalColor(mNormalColor);
	((MainWindow*)mParent)->getActive()->setFaceCentroidColor(mFaceCentroidColor);
	((MainWindow*)mParent)->getActive()->setFaceIDBgColor(mFaceIDBgColor);
	((MainWindow*)mParent)->getActive()->setFaceCentroidColor(mFaceCentroidColor);
	((MainWindow*)mParent)->getActive()->setNormalColor(mNormalColor);
	((MainWindow*)mParent)->getActive()->setNormalThickness(mNormalThickness);	
	((MainWindow*)mParent)->getActive()->setFaceCentroidThickness(mFaceCentroidThickness);	
	((MainWindow*)mParent)->getActive()->setNormalLength(mNormalLength);	
	((MainWindow*)mParent)->getActive()->setWireframeThickness(mWireframeThickness);	
	((MainWindow*)mParent)->getActive()->setSilhouetteThickness(mSilhouetteThickness);	
	((MainWindow*)mParent)->getActive()->setVertexThickness(mVertexThickness);	
	((MainWindow*)mParent)->getActive()->setSelectedVertexThickness(mSelectedVertexThickness);	
	((MainWindow*)mParent)->getActive()->setSelectedEdgeThickness(mSelectedEdgeThickness);
	
	((MainWindow*)mParent)->getActive()->setNearPlane(mCameraNearPlane);
	((MainWindow*)mParent)->getActive()->setFarPlane(mCameraFarPlane);
	((MainWindow*)mParent)->getActive()->setFOV(mCameraFov);
		//save options
	((MainWindow*)mParent)->setSaveDirectory(mSaveDirectory);
	((MainWindow*)mParent)->setAutoSave(mAutoSave);
	((MainWindow*)mParent)->setAutoSaveDelay(mAutoSaveDelay);
	((MainWindow*)mParent)->setIncrementalSave(mIncrementalSave);
	((MainWindow*)mParent)->setIncrementalSaveMax(mIncrementalSaveMax);
	((MainWindow*)mParent)->setCommandCompleterIndexToggle(mCommandCompleterIndex);

}

void TopModPreferences::discardSettings(){
	reject();
}

void TopModPreferences::setButtonColor(QColor color, QPushButton *button){
	// color = c;
	QPalette p = button->palette();
	p.setColor(QPalette::Button, color);
	button->setPalette(p);
}

/*!
* \brief load all the default settings and attach this as a signal for a reset button
*/
void TopModPreferences::loadDefaults(){

	mViewportColor = mViewportColorDefault;
	((MainWindow*)mParent)->getActive()->setViewportColor(mViewportColor);
	setButtonColor(mViewportColor, mViewportColorButton);

	mRenderColor = mRenderColorDefault;
	((MainWindow*)mParent)->getActive()->setRenderColor(mRenderColor);
	setButtonColor(mRenderColor, mRenderColorButton);

	mCoolLightColor = mCoolLightColorDefault;
	((MainWindow*)mParent)->getActive()->setCoolLightColor(mCoolLightColor);
	setButtonColor(mCoolLightColor, mCoolLightColorButton);

	mWarmLightColor = mWarmLightColorDefault;
	((MainWindow*)mParent)->getActive()->setWarmLightColor(mWarmLightColor);
	setButtonColor(mWarmLightColor, mWarmLightColorButton);

	mWireframeColor = mWireframeColorDefault;
	((MainWindow*)mParent)->getActive()->setWireframeColor(mWireframeColor);
	setButtonColor(mWireframeColor, mWireframeColorButton);

	mSilhouetteColor = mSilhouetteColorDefault;
	((MainWindow*)mParent)->getActive()->setSilhouetteColor(mSilhouetteColor);
	setButtonColor(mSilhouetteColor, mSilhouetteColorButton);

	mPatchBoundaryColor = mPatchBoundaryColorDefault;
	// ((MainWindow*)mParent)->getActive()->setPatchBoundaryColor(mPatchBoundaryColor);
	setButtonColor(mPatchBoundaryColor, mPatchBoundaryColorButton);
	
	mSelectedVertexColor = mSelectedVertexColorDefault;
	((MainWindow*)mParent)->getActive()->setSelectedVertexColor(mSelectedVertexColor);
	setButtonColor(mSelectedVertexColor, mSelectedVertexColorButton);
	
	mSelectedEdgeColor = mSelectedEdgeColorDefault;
	((MainWindow*)mParent)->getActive()->setSelectedEdgeColor(mSelectedEdgeColor);
	setButtonColor(mSelectedEdgeColor, mSelectedEdgeColorButton);
	
	mSelectedFaceColor = mSelectedFaceColorDefault;
	((MainWindow*)mParent)->getActive()->setSelectedFaceColor(mSelectedFaceColor);
	setButtonColor(mSelectedFaceColor, mSelectedFaceColorButton);
	
	mVertexIDBgColor = mVertexIDBgColor;
	((MainWindow*)mParent)->getActive()->setVertexIDBgColor(mVertexIDBgColor);
	setButtonColor(mVertexIDBgColor, mVertexIDBgColorButton);
	
	mNormalColor = mNormalColorDefault;
	((MainWindow*)mParent)->getActive()->setNormalColor(mNormalColor);
	setButtonColor(mNormalColor, mNormalColorButton);

	mFaceCentroidColor = mFaceCentroidColorDefault;
	((MainWindow*)mParent)->getActive()->setFaceCentroidColor(mFaceCentroidColor);
	setButtonColor(mFaceCentroidColor, mFaceCentroidColorButton);
	
	mFaceIDBgColor = mFaceIDBgColorDefault;
	((MainWindow*)mParent)->getActive()->setFaceIDBgColor(mFaceIDBgColor);
	setButtonColor(mFaceIDBgColor, mFaceIDBgColorButton);
	
	mLightIntensity = mLightIntensityDefault;
	mLightIntensitySpinBox->setValue(mLightIntensity); 
	
	mWireframeThickness = mWireframeThicknessDefault;
	mWireframeThicknessSpinBox->setValue(mWireframeThickness);	
	
	mSilhouetteThickness = mSilhouetteThicknessDefault;
	mSilhouetteThicknessSpinBox->setValue(mSilhouetteThickness);
	
	mNormalThickness = mNormalThicknessDefault;
	mNormalThicknessSpinBox->setValue(mNormalThickness);	

	mNormalLength = mNormalLengthDefault;
	mNormalLengthSpinBox->setValue(mNormalLength);	

	mFaceCentroidThickness = mFaceCentroidThicknessDefault;
	mFaceCentroidThicknessSpinBox->setValue(mFaceCentroidThickness);	

	mVertexThickness = mVertexThicknessDefault;
	mVertexThicknessSpinBox->setValue(mVertexThickness);	
	
	mSelectedVertexThickness = mSelectedVertexThicknessDefault;
	mVertexThicknessSpinBox->setValue(mSelectedVertexThickness);
	
	mSelectedEdgeThickness = mSelectedEdgeThicknessDefault;
	mSelectedEdgeThicknessSpinBox->setValue(mSelectedVertexThickness);
	
	
	//camera stuff
	mCameraNearPlane = mCameraNearPlaneDefault;
	((MainWindow*)mParent)->getActive()->setNearPlane(mCameraNearPlane);
	
	mCameraFarPlane = mCameraFarPlaneDefault;
	((MainWindow*)mParent)->getActive()->setFarPlane(mCameraFarPlane);
	
	mCameraFov = mCameraFovDefault;
	((MainWindow*)mParent)->getActive()->setFOV(mCameraFov);
	
	//save options!
	mSaveDirectory = mSaveDirectoryDefault;
	((MainWindow*)mParent)->setSaveDirectory(mSaveDirectory);

	mAutoSave = mAutoSaveDefault;
	((MainWindow*)mParent)->setAutoSave(mAutoSave);
	
	mAutoSaveDelay = mAutoSaveDelayDefault;
	((MainWindow*)mParent)->setAutoSaveDelay(mAutoSaveDelay);

	mIncrementalSave = mIncrementalSaveDefault;
	((MainWindow*)mParent)->setIncrementalSave(mIncrementalSave);
	
	mIncrementalSaveMax = mIncrementalSaveMaxDefault;
	((MainWindow*)mParent)->setIncrementalSaveMax(mIncrementalSaveMax);
	
	mCommandCompleterIndex = mCommandCompleterIndexDefault;
	((MainWindow*)mParent)->setCommandCompleterIndexToggle(mCommandCompleterIndex);
	
}

void TopModPreferences::setupMain(){

	QGridLayout *mMainLayout = new QGridLayout;
	
	// mResetCameraButton = new QPushButton(tr("Reset"));
	// connect(mResetCameraButton,SIGNAL(clicked()),
	// 				((MainWindow*)mParent), SLOT(switchPerspView()));
	
	// mShowStartupDialogAtStartupCheckBox = new QCheckBox(tr("Show Tutorial Dialog at Startup"));
	// mShowStartupDialogAtStartupCheckBox->setChecked(mShowStartupDialogAtStartup);
	// connect(mShowStartupDialogAtStartupCheckBox,SIGNAL(stateChanged(int)),this,SLOT(setShowStartupDialogAtStartup(int)));
	
	QStyle* colorPickerStyle = new QPlastiqueStyle;
	QPalette p;

	//camera nearplane
	mCameraNearPlaneSpinBox = addSpinBoxPreference(mCameraNearPlaneLabel, tr("Near Plane:"), 0.0, 100, 0.1, mCameraNearPlane, 1, mMainLayout, 0, 0);
	connect(mCameraNearPlaneSpinBox, SIGNAL(valueChanged(double)),((MainWindow*)mParent)->getActive(), SLOT(setNearPlane(double)));

	//camera farplane
	mCameraFarPlaneSpinBox = addSpinBoxPreference(mCameraFarPlaneLabel, tr("Far Plane:"), 10, 1000, 0.1, mCameraFarPlane, 1, mMainLayout, 1, 0);
	connect(mCameraFarPlaneSpinBox, SIGNAL(valueChanged(double)),((MainWindow*)mParent)->getActive(), SLOT(setFarPlane(double)));

	//camera nearplane
	mCameraFovSpinBox = addSpinBoxPreference(mCameraFovLabel, tr("Field of View:"), 15, 100, 1, mCameraFov, 0, mMainLayout, 2, 0);
	connect(mCameraFovSpinBox, SIGNAL(valueChanged(double)),((MainWindow*)mParent)->getActive(), SLOT(setFOV(double)));
	
	//auto save toggle
	mAutoSaveCheckBox = new QCheckBox(tr("Auto Save:"), this);
	mAutoSaveCheckBox->setChecked(mAutoSave);
	connect(mAutoSaveCheckBox, SIGNAL(stateChanged(int)),((MainWindow*)mParent), SLOT(setAutoSave(int)));
	mMainLayout->addWidget(mAutoSaveCheckBox,3,0);
	
	//auto save delay in minutes
	mAutoSaveDelaySpinBox = addSpinBoxPreference(mAutoSaveDelayLabel, tr("Auto Save Delay\n(in minutes):"), 1, 30, 1, mAutoSaveDelay, 0, mMainLayout, 4, 0);
	connect(mAutoSaveDelaySpinBox, SIGNAL(valueChanged(double)),((MainWindow*)mParent), SLOT(setAutoSaveDelay(double)));

	//incremental save toggle
	mIncrementalSaveCheckBox = new QCheckBox(tr("Incremental Save:"), this);
	mIncrementalSaveCheckBox->setChecked(mIncrementalSave);
	connect(mIncrementalSaveCheckBox, SIGNAL(stateChanged(int)),((MainWindow*)mParent), SLOT(setIncrementalSave(int)));
	mMainLayout->addWidget(mIncrementalSaveCheckBox,5,0);

	//max incremental save files on disk
	mIncrementalSaveMaxSpinBox = addSpinBoxPreference(mIncrementalSaveMaxLabel, tr("Max Incremental Saves:"), 2, 50, 1, mIncrementalSaveMax, 0, mMainLayout, 6, 0);
	connect(mIncrementalSaveMaxSpinBox, SIGNAL(valueChanged(double)),((MainWindow*)mParent), SLOT(setIncrementalSaveMax(double)));
	
	//default save directory
	mSaveDirectoryLabel = new QLabel(tr("Default Save Directory"),this);
	mSaveDirectoryLineEdit = new QLineEdit(this);
	std::cout << mSaveDirectory.toLatin1().data() << "\n";
	mSaveDirectoryLineEdit->setText(mSaveDirectory);
	connect(mSaveDirectoryLineEdit, SIGNAL(textEdited(QString)),((MainWindow*)mParent), SLOT(setSaveDirectory(QString)));
	connect(mSaveDirectoryLineEdit, SIGNAL(editingFinished()),((MainWindow*)mParent), SLOT(checkSaveDirectory()));
	mMainLayout->addWidget(mSaveDirectoryLabel,7,0);
	mMainLayout->addWidget(mSaveDirectoryLineEdit,8,0,1,3);
	
	//command completer word typing toggle
	mCommandCompleterIndexToggle  = new QCheckBox(tr("Command Completer\nSingle Word Completion:"), this);
	mCommandCompleterIndexToggle->setChecked(mCommandCompleterIndex);
	connect(mCommandCompleterIndexToggle, SIGNAL(stateChanged(int)),((MainWindow*)mParent), SLOT(setCommandCompleterIndexToggle(int)));
	mMainLayout->addWidget(mCommandCompleterIndexToggle,9,0);
	
	mMainLayout->setRowStretch(10,2);
	mMainLayout->setColumnStretch(4,2);
	
	mMainTab->setLayout(mMainLayout);
	// mMainLayout->addWidget(mResetCameraButton,0,0);
	
	// persp->switchTo(VPPersp);
}

QPushButton* TopModPreferences::addColorPreference(QLabel *label, QString text, QColor color, QGridLayout *layout, QStyle *style, int row, int col){
	
	label = new QLabel(text);
	QPushButton *button = new QPushButton(mColorsTab);
	button->setMaximumSize(QSize(100,25));
	button->setAutoFillBackground(true);
	button->setStyle(style);
	QPalette p = button->palette();
	p.setColor(button->backgroundRole(), color);
	button->setPalette(p);	
	layout->addWidget(label,row,col);
	layout->addWidget(button,row,col+1);
	return button;
}

QDoubleSpinBox* TopModPreferences::addSpinBoxPreference(QLabel *label, QString text, double low, double high, double step, double value, int decimals, QGridLayout *layout, int row, int col){
	label = new QLabel(text);
	QDoubleSpinBox *spinbox = new QDoubleSpinBox;
	spinbox->setRange(low, high);
	spinbox->setSingleStep(step);
	spinbox->setValue(value);
	spinbox->setDecimals(decimals);
	spinbox->setMaximumSize(100,25);	
	layout->addWidget(label,row,col);
	layout->addWidget(spinbox,row,col+1);
	return spinbox;
}

void TopModPreferences::setupColors(){
	
	//layout for colors tab page
	QGridLayout *mColorsLayout = new QGridLayout;
	//style for the color
	QStyle* colorPickerStyle = new QPlastiqueStyle;
	QPalette p;

	//viewport background color
	mViewportColorButton = addColorPreference(mViewportColorLabel, tr("Viewport Background:"), mViewportColor, mColorsLayout, colorPickerStyle, 0, 0);
	connect(mViewportColorButton,SIGNAL(clicked()),this,SLOT(setViewportColor()));
	//object render color
	mRenderColorButton = addColorPreference(mRenderColorLabel, tr("Object Render Color:"), mRenderColor, mColorsLayout, colorPickerStyle, 1, 0);
	connect(mRenderColorButton,SIGNAL(clicked()),this,SLOT(setRenderColor()));
	//CoolLight  color
	mCoolLightColorButton = addColorPreference(mCoolLightColorLabel, tr("Cool Light Color:"), mCoolLightColor, mColorsLayout, colorPickerStyle, 2, 0);
	connect(mCoolLightColorButton,SIGNAL(clicked()),this,SLOT(setCoolLightColor()));
	//WarmLight  color
	mWarmLightColorButton = addColorPreference(mWarmLightColorLabel, tr("Warm Light Color:"), mWarmLightColor, mColorsLayout, colorPickerStyle, 3, 0);
	connect(mWarmLightColorButton,SIGNAL(clicked()),this,SLOT(setWarmLightColor()));
	//Wireframe  color
	mWireframeColorButton = addColorPreference(mWireframeColorLabel, tr("Wireframe Color:"), mWireframeColor, mColorsLayout, colorPickerStyle, 4, 0);
	connect(mWireframeColorButton,SIGNAL(clicked()),this,SLOT(setWireframeColor()));
	//Silhouette  color
	mSilhouetteColorButton = addColorPreference(mSilhouetteColorLabel, tr("Silhouette Color:"), mSilhouetteColor, mColorsLayout, colorPickerStyle, 5, 0);
	connect(mSilhouetteColorButton,SIGNAL(clicked()),this,SLOT(setSilhouetteColor()));
	//PatchBoundary  color
	mPatchBoundaryColorButton = addColorPreference(mPatchBoundaryColorLabel, tr("Patch Boundary Color:"), mPatchBoundaryColor, mColorsLayout, colorPickerStyle, 6, 0);
	connect(mPatchBoundaryColorButton,SIGNAL(clicked()),this,SLOT(setPatchBoundaryColor()));
	//SelectedVertex  color
	mSelectedVertexColorButton = addColorPreference(mSelectedVertexColorLabel, tr("Selected Vertex Color:"), mSelectedVertexColor, mColorsLayout, colorPickerStyle, 7, 0);
	connect(mSelectedVertexColorButton,SIGNAL(clicked()),this,SLOT(setSelectedVertexColor()));
	//SelectedEdge  color
	mSelectedEdgeColorButton = addColorPreference(mSelectedEdgeColorLabel, tr("Selected Edge Color:"), mSelectedEdgeColor, mColorsLayout, colorPickerStyle, 8, 0);
	connect(mSelectedEdgeColorButton,SIGNAL(clicked()),this,SLOT(setSelectedEdgeColor()));
	//SelectedFace  color
	mSelectedFaceColorButton = addColorPreference(mSelectedFaceColorLabel, tr("Selected Face Color:"), mSelectedFaceColor, mColorsLayout, colorPickerStyle, 9, 0);
	connect(mSelectedFaceColorButton,SIGNAL(clicked()),this,SLOT(setSelectedFaceColor()));
	//VertexIDBg  color
	mVertexIDBgColorButton = addColorPreference(mVertexIDBgColorLabel, tr("Vertex ID Bg Color:"), mVertexIDBgColor, mColorsLayout, colorPickerStyle, 10, 0);
	connect(mVertexIDBgColorButton,SIGNAL(clicked()),this,SLOT(setVertexIDBgColor()));
	//FaceIDBg  color
	mFaceIDBgColorButton = addColorPreference(mFaceIDBgColorLabel, tr("Face ID Bg Color:"), mFaceIDBgColor, mColorsLayout, colorPickerStyle, 11, 0);
	connect(mFaceIDBgColorButton,SIGNAL(clicked()),this,SLOT(setFaceIDBgColor()));
	//EdgeIDBg  color
	mEdgeIDBgColorButton = addColorPreference(mEdgeIDBgColorLabel, tr("Edge ID Bg Color:"), mEdgeIDBgColor, mColorsLayout, colorPickerStyle, 12, 0);
	connect(mEdgeIDBgColorButton,SIGNAL(clicked()),this,SLOT(setEdgeIDBgColor()));
	//Normal  color
	mNormalColorButton = addColorPreference(mNormalColorLabel, tr("Normal Color:"), mNormalColor, mColorsLayout, colorPickerStyle, 13, 0);
	connect(mNormalColorButton,SIGNAL(clicked()),this,SLOT(setNormalColor()));
	//FaceCentroid  color
	mFaceCentroidColorButton = addColorPreference(mFaceCentroidColorLabel, tr("Face Centroid Color:"), mFaceCentroidColor, mColorsLayout, colorPickerStyle, 14, 0);
	connect(mFaceCentroidColorButton,SIGNAL(clicked()),this,SLOT(setFaceCentroidColor()));
	
	//new column
	
	//light intensity
	mLightIntensitySpinBox = addSpinBoxPreference(mLightIntensityLabel, tr("Light Intensity:"), 0.0, 10.0, 0.1, mLightIntensity, 1, mColorsLayout, 0, 2);
	connect(mLightIntensitySpinBox, SIGNAL(valueChanged(double)),((MainWindow*)mParent)->getActive(), SLOT(setLightIntensity(double)));
	//wireframe thickness
	mWireframeThicknessSpinBox = addSpinBoxPreference(mWireframeThicknessLabel, tr("Wireframe Thickness:"), 0.1, 30.0, 0.1, mWireframeThickness, 1, mColorsLayout, 1, 2);
	connect(mWireframeThicknessSpinBox, SIGNAL(valueChanged(double)),((MainWindow*)mParent)->getActive(), SLOT(setWireframeThickness(double)));
	//vertex thickness
	mVertexThicknessSpinBox = addSpinBoxPreference(mVertexThicknessLabel, tr("Vertex Size:"), 0.1, 10.0, 0.5, mVertexThickness, 1, mColorsLayout, 2, 2);
	connect(mVertexThicknessSpinBox, SIGNAL(valueChanged(double)),((MainWindow*)mParent)->getActive(), SLOT(setVertexThickness(double)));
	//silhouette thickness
	mSilhouetteThicknessSpinBox = addSpinBoxPreference(mSilhouetteThicknessLabel, tr("Silhouette Thickness:"), 0.1, 15.0, 0.5, mSilhouetteThickness, 1, mColorsLayout, 3, 2);
	connect(mSilhouetteThicknessSpinBox, SIGNAL(valueChanged(double)),((MainWindow*)mParent)->getActive(), SLOT(setSilhouetteThickness(double)));
	//SelectedVertex thickness
	mSelectedVertexThicknessSpinBox = addSpinBoxPreference(mSelectedVertexThicknessLabel, tr("Selected Vertex Size:"), 0.1, 15.0, 0.5, mSelectedVertexThickness, 1, mColorsLayout, 4, 2);
	connect(mSelectedVertexThicknessSpinBox, SIGNAL(valueChanged(double)),((MainWindow*)mParent)->getActive(), SLOT(setSelectedVertexThickness(double)));
	//SelectedEdge thickness
	mSelectedEdgeThicknessSpinBox = addSpinBoxPreference(mSelectedEdgeThicknessLabel, tr("Selected Edge Thickness:"), 0.1, 15.0, 0.5, mSelectedEdgeThickness, 1, mColorsLayout, 5, 2);
	connect(mSelectedEdgeThicknessSpinBox, SIGNAL(valueChanged(double)),((MainWindow*)mParent)->getActive(), SLOT(setSelectedEdgeThickness(double)));
	//Normal Size
	mNormalThicknessSpinBox = addSpinBoxPreference(mNormalThicknessLabel, tr("Normal Thickness:"), 0.1, 15.0, 0.5, mNormalThickness, 1, mColorsLayout, 6, 2);
	connect(mNormalThicknessSpinBox, SIGNAL(valueChanged(double)),((MainWindow*)mParent)->getActive(), SLOT(setNormalThickness(double)));
	//SelectedEdge thickness
	mNormalLengthSpinBox = addSpinBoxPreference(mNormalLengthLabel, tr("Normal Length:"), 0.1, 15.0, 0.5, mNormalLength, 1, mColorsLayout, 7, 2);
	connect(mNormalLengthSpinBox, SIGNAL(valueChanged(double)),((MainWindow*)mParent)->getActive(), SLOT(setNormalLength(double)));
	//SelectedEdge thickness
	mFaceCentroidThicknessSpinBox = addSpinBoxPreference(mFaceCentroidThicknessLabel, tr("Face Centroid Thickness:"), 0.1, 15.0, 0.5, mFaceCentroidThickness, 1, mColorsLayout, 8, 2);
	connect(mFaceCentroidThicknessSpinBox, SIGNAL(valueChanged(double)),((MainWindow*)mParent)->getActive(), SLOT(setFaceCentroidThickness(double)));

	//reset button
	mResetColorsButton = new QPushButton(tr("Reset"));
	connect(mResetColorsButton,SIGNAL(clicked()),this, SLOT(loadDefaults()));
	mColorsLayout->addWidget(mResetColorsButton,14,3);
	
	mColorsLayout->setRowStretch(15,17);
	mColorsLayout->setColumnStretch(4,10);

	// mColorsLayout->addStretch(1);
	mColorsTab->setLayout(mColorsLayout);	
	
}

void TopModPreferences::setViewportColor(){
	mViewportColor.setRgba(QColorDialog::getRgba(mViewportColor.rgba()));
	if (mViewportColor.isValid()){
		((MainWindow*)mParent)->getActive()->setViewportColor(mViewportColor);
		setButtonColor(mViewportColor, mViewportColorButton);
	}
}

void TopModPreferences::setRenderColor(){
	mRenderColor.setRgba(QColorDialog::getRgba(mRenderColor.rgba()));
	if (mRenderColor.isValid()){
		((MainWindow*)mParent)->getActive()->setRenderColor(mRenderColor);
		setButtonColor(mRenderColor, mRenderColorButton);
	}
}

void TopModPreferences::setCoolLightColor(){
	mCoolLightColor.setRgba(QColorDialog::getRgba(mCoolLightColor.rgba()));
	if (mCoolLightColor.isValid()){
		((MainWindow*)mParent)->getActive()->setCoolLightColor(mCoolLightColor);
		setButtonColor(mCoolLightColor, mCoolLightColorButton);
	}
}

void TopModPreferences::setWarmLightColor(){
	mWarmLightColor.setRgba(QColorDialog::getRgba(mWarmLightColor.rgba()));
	if (mWarmLightColor.isValid()){
		((MainWindow*)mParent)->getActive()->setWarmLightColor(mWarmLightColor);
		setButtonColor(mWarmLightColor, mWarmLightColorButton);
	}
}

void TopModPreferences::setWireframeColor(){
	mWireframeColor.setRgba(QColorDialog::getRgba(mWireframeColor.rgba()));
	if (mWireframeColor.isValid()){
		((MainWindow*)mParent)->getActive()->setWireframeColor(mWireframeColor);
		setButtonColor(mWireframeColor, mWireframeColorButton);
	}
}

void TopModPreferences::setSilhouetteColor(){
	mSilhouetteColor.setRgba(QColorDialog::getRgba(mSilhouetteColor.rgba()));
	if (mSilhouetteColor.isValid()){
		((MainWindow*)mParent)->getActive()->setSilhouetteColor(mSilhouetteColor);
		setButtonColor(mSilhouetteColor, mSilhouetteColorButton);
	}
}

void TopModPreferences::setPatchBoundaryColor(){
	mPatchBoundaryColor.setRgba(QColorDialog::getRgba(mPatchBoundaryColor.rgba()));
	if (mPatchBoundaryColor.isValid()){
		// ((MainWindow*)mParent)->getActive()->setPatchBoundaryColor(mPatchBoundaryColor);
		setButtonColor(mPatchBoundaryColor, mPatchBoundaryColorButton);
	}
}

void TopModPreferences::setSelectedVertexColor(){
	mSelectedVertexColor.setRgba(QColorDialog::getRgba(mSelectedVertexColor.rgba()));
	if (mSelectedVertexColor.isValid()){
		((MainWindow*)mParent)->getActive()->setSelectedVertexColor(mSelectedVertexColor);
		setButtonColor(mSelectedVertexColor, mSelectedVertexColorButton);
	}
}

void TopModPreferences::setSelectedEdgeColor(){
	mSelectedEdgeColor.setRgba(QColorDialog::getRgba(mSelectedEdgeColor.rgba()));
	if (mSelectedEdgeColor.isValid()){
		((MainWindow*)mParent)->getActive()->setSelectedEdgeColor(mSelectedEdgeColor);
		setButtonColor(mSelectedEdgeColor, mSelectedEdgeColorButton);
	}
}

void TopModPreferences::setSelectedFaceColor(){
	mSelectedFaceColor.setRgba(QColorDialog::getRgba(mSelectedFaceColor.rgba()));
	if (mSelectedFaceColor.isValid()){
		((MainWindow*)mParent)->getActive()->setSelectedFaceColor(mSelectedFaceColor);
		setButtonColor(mSelectedFaceColor, mSelectedFaceColorButton);
	}
}

void TopModPreferences::setFaceIDBgColor(){
	mFaceIDBgColor.setRgba(QColorDialog::getRgba(mFaceIDBgColor.rgba()));
	if (mFaceIDBgColor.isValid()){
		((MainWindow*)mParent)->getActive()->setFaceIDBgColor(mFaceIDBgColor);
		setButtonColor(mFaceIDBgColor, mFaceIDBgColorButton);
	}
}

void TopModPreferences::setVertexIDBgColor(){
	mVertexIDBgColor.setRgba(QColorDialog::getRgba(mVertexIDBgColor.rgba()));
	if (mVertexIDBgColor.isValid()){
		((MainWindow*)mParent)->getActive()->setVertexIDBgColor(mVertexIDBgColor);
		setButtonColor(mVertexIDBgColor, mVertexIDBgColorButton);
	}
}

void TopModPreferences::setEdgeIDBgColor(){
	mEdgeIDBgColor.setRgba(QColorDialog::getRgba(mEdgeIDBgColor.rgba()));
	if (mEdgeIDBgColor.isValid()){
		((MainWindow*)mParent)->getActive()->setEdgeIDBgColor(mEdgeIDBgColor);
		setButtonColor(mEdgeIDBgColor, mEdgeIDBgColorButton);
	}
}

void TopModPreferences::setNormalColor(){
	mNormalColor.setRgba(QColorDialog::getRgba(mNormalColor.rgba()));
	if (mNormalColor.isValid()){
		((MainWindow*)mParent)->getActive()->setNormalColor(mNormalColor);
		setButtonColor(mNormalColor, mNormalColorButton);
	}
}

void TopModPreferences::setFaceCentroidColor(){
	mFaceCentroidColor.setRgba(QColorDialog::getRgba(mFaceCentroidColor.rgba()));
	if (mFaceCentroidColor.isValid()){
		((MainWindow*)mParent)->getActive()->setFaceCentroidColor(mFaceCentroidColor);
		setButtonColor(mFaceCentroidColor, mFaceCentroidColorButton);
	}
}

void TopModPreferences::setSaveDirectory(QString s){
	mSaveDirectory = s;
	mSaveDirectoryLineEdit->setText(s);
}
