#include "TopModPreferences.hh"

TopModPreferences::TopModPreferences(QSettings *settings, QWidget *parent) 
:	QDialog(parent) {
	
	mSettings = settings;
	mParent = parent;
	setSizeGripEnabled(false);
	
	mPrefTabs = new QTabWidget;
	//setup layouts
	QVBoxLayout *mainLayout = new QVBoxLayout;
	mainLayout->addWidget(mPrefTabs);
	QHBoxLayout *lowLayout = new QHBoxLayout;
	mainLayout->addLayout(lowLayout,1);
	lowLayout->addStretch(1);
	
	mOkButton = new QPushButton(tr("OK"));
	// mOkButton->setDefault(true);
	connect(mOkButton, SIGNAL(clicked()), this, SLOT(accept()));
	mCancelButton = new QPushButton(tr("Cancel"));
	connect(mCancelButton, SIGNAL(clicked()), this, SLOT(reject()));
	lowLayout->addWidget(mOkButton);
	lowLayout->addWidget(mCancelButton);
	setLayout(mainLayout);
	
	createTabs();
	// mPrefTabs->addTab()
	
}

TopModPreferences::~TopModPreferences(){
	
}

void TopModPreferences::createTabs(){
	
	QWidget *mMainTab = new QWidget;
	QWidget *mColorsTab = new QWidget;
	QWidget *mShortcutsTab = new QWidget;
	
	mPrefTabs->addTab(mMainTab,tr("Main"));
	mPrefTabs->addTab(mColorsTab,tr("Colors"));
	mPrefTabs->addTab(mShortcutsTab,tr("Shortcuts"));	
	
}
// void MainWindow::closeEvent(QCloseEvent *event) {
// 	accept();
// 	event->accept();
// }