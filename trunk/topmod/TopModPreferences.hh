#ifndef _TOPMODPREFERENCES_H
#define _TOPMODPREFERENCES_H

#include <QtGui>
#include <QTabWidget>
#include <QPushButton>
#include <QSettings>
#include <QWidget>
#include <QDialog>
#include <QBoxLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QColorDialog>
#include <QColor>
#include <QPalette>

#include "MainWindow.hh"

class MainWindow;

class TopModPreferences : public QDialog {
	Q_OBJECT

private:
	QSettings *mSettings;
	QTabWidget *mPrefTabs;
	QWidget *mParent;
	
	QPushButton *mOkButton;
	QPushButton *mCancelButton;
	
	//tab pages
	QWidget *mMainTab;
	QWidget *mColorsTab;
	QWidget *mStylesheetsTab;
	QWidget *mShortcutsTab;
	
	//color dialogs test
	// QColorDialog *mViewportColorDialog;
	QPushButton *mViewportColorButton;
	QLabel *mViewportColorLabel;
	
	//colors
	QColor mViewportColor;
	QColor mRenderColor;
	QColor mCoolLightColor;
	QColor mWarmLightColor;
	
public:
	TopModPreferences(QSettings *settings, QWidget *parent = 0 );
	~TopModPreferences();

protected:
	void createTabs();
	void setupColors();
	
public slots:

	void saveSettings();
	void discardSettings();

	void setViewportColor();
};

#endif
