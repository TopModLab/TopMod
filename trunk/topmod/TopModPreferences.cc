#include "TopModPreferences.hh"

TopModPreferences::TopModPreferences(QSettings *settings, QWidget *parent) 
:	QDialog(parent) {
	
	mSettings = settings;
	mParent = parent;
	setSizeGripEnabled(false);
	resize(400,400);
	setMinimumSize(400,400);
	setWindowTitle(tr("TopMod Preferences"));
	setWindowFlags(Qt::FramelessWindowHint | Qt::Dialog);
	
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
	setupColors();
	// mPrefTabs->addTab()


}

TopModPreferences::~TopModPreferences(){

}

void TopModPreferences::createTabs(){

	mMainTab = new QWidget;
	mColorsTab = new QWidget;
	mShortcutsTab = new QWidget;
	mStylesheetsTab = new QWidget;
	 
	mPrefTabs->addTab(mMainTab,tr("Main"));
	mPrefTabs->addTab(mColorsTab,tr("Colors"));
	mPrefTabs->addTab(mShortcutsTab,tr("Shortcuts"));
	
}
// void MainWindow::closeEvent(QCloseEvent *event) {
// 	accept();
// 	event->accept();
// }

void TopModPreferences::saveSettings(){
	mSettings->beginGroup("Colors");
	mSettings->setValue("ViewportColor", mViewportColor);
	mSettings->endGroup();
	
	accept();
}

void TopModPreferences::discardSettings(){
	
	
	
	reject();
}

void TopModPreferences::setupColors(){
	
	//layout for colors tab page
	QGridLayout *mColorsLayout = new QGridLayout;

	mViewportColorLabel = new QLabel(tr("Viewport Background:"));
	mViewportColorButton = new QPushButton(mColorsTab);
	mViewportColorButton->setMaximumSize(QSize(75,25));
	mViewportColorButton->setAutoFillBackground(true);
	QPalette p = mViewportColorButton->palette();
	p.setColor(QPalette::Button, Qt::red);
	// p.setBrush( QPalette::Button, QBrush(QColor(128, 128, 255)) );
	mViewportColorButton->setPalette(p);
	
	connect(mViewportColorButton,SIGNAL(clicked()),this,SLOT(setViewportColor()));
	
	mColorsLayout->addWidget(mViewportColorLabel,0,0);
	mColorsLayout->addWidget(mViewportColorButton,0,1);
	// mColorsLayout->addWidget(mViewportColorDialog);

	mColorsTab->setLayout(mColorsLayout);
	
}

void TopModPreferences::setViewportColor(){
	
	mViewportColor = QColorDialog::getColor();
	if (mViewportColor.isValid()){
		((MainWindow*)mParent)->getDLFLWindow()->getActive()->setViewportColor(mViewportColor);
		QPalette p = mViewportColorButton->palette();
		p.setColor(QPalette::Button, mViewportColor);
		// p.setBrush( QPalette::Button, QBrush(QColor(128, 128, 255)) );
		mViewportColorButton->setPalette(p);
	}
}
