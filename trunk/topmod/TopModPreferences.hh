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
#include <QDoubleSpinBox>
#include <QLabel>

#include "MainWindow.hh"
#include "qshortcutdialog.hh"
#include "qshortcutmanager.hh"

class QShortcutDialog;
class QShortcutManager;
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
	QShortcutDialog *mShortcutsTab;
	QShortcutManager *mShortcutsManager;
	StyleSheetEditor *mStyleSheetsTab;
	
		//COLORS
	
	//viewport color
	QColor mViewportColor;
	QPushButton *mViewportColorButton;
	QLabel *mViewportColorLabel;
	
	//Render color
	QColor mRenderColor;
	QPushButton *mRenderColorButton;
	QLabel *mRenderColorLabel;
	
	//Cool Light Color
	QColor mCoolLightColor;
	QPushButton *mCoolLightColorButton;
	QLabel *mCoolLightColorLabel;
	
	//Warm Light Color
	QColor mWarmLightColor;
	QPushButton *mWarmLightColorButton;
	QLabel *mWarmLightColorLabel;
	
	//Wireframe Color
	QColor mWireframeColor;
	QPushButton *mWireframeColorButton;
	QLabel *mWireframeColorLabel;
	
	//Silhouette Color
	QColor mSilhouetteColor;
	QPushButton *mSilhouetteColorButton;
	QLabel *mSilhouetteColorLabel;
	
	//patch boundary color
	QColor mPatchBoundaryColor;
	QPushButton *mPatchBoundaryColorButton;
	QLabel *mPatchBoundaryColorLabel;
	
	//light intensity
	double mLightIntensity;
	QLabel *mLightIntensityLabel;
	QDoubleSpinBox *mLightIntensitySpinBox;
	
	//wireframe thickness
	double mWireframeThickness;
	QLabel *mWireframeThicknessLabel;
	QDoubleSpinBox *mWireframeThicknessSpinBox;
	
	//silhouette thickness
	double mSilhouetteThickness;
	QLabel *mSilhouetteThicknessLabel;
	QDoubleSpinBox *mSilhouetteThicknessSpinBox;
	
	//vertex thickness
	double mVertexThickness;
	QLabel *mVertexThicknessLabel;
	QDoubleSpinBox *mVertexThicknessSpinBox;
	
	//reset button
	QPushButton *mResetColorsButton;
	
public:
	TopModPreferences(QSettings *settings, StyleSheetEditor *sse, QShortcutManager *sm, QWidget *parent = 0 );
	~TopModPreferences();
	
	int display();
	void readSettings();


protected:
	void createTabs();
	void setupColors();
	
public slots:

	void saveSettings();
	void discardSettings();

	void setViewportColor();
	void setRenderColor();
	void setCoolLightColor();
	void setWarmLightColor();
	void setWireframeColor();
	void setSilhouetteColor();
	void setPatchBoundaryColor();
	
	void loadDefaults();

};

#endif
