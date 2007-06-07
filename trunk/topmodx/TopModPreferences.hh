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

	//selected vertex thickness
	double mSelectedVertexThickness;
	QLabel *mSelectedVertexThicknessLabel;
	QDoubleSpinBox *mSelectedVertexThicknessSpinBox;

	//selected edge thickness
	double mSelectedEdgeThickness;
	QLabel *mSelectedEdgeThicknessLabel;
	QDoubleSpinBox *mSelectedEdgeThicknessSpinBox;
	
	//selected edge color
	QColor mSelectedEdgeColor;
	QPushButton *mSelectedEdgeColorButton;
	QLabel *mSelectedEdgeColorLabel;
	
	//selected vertex color
	QColor mSelectedVertexColor;
	QPushButton *mSelectedVertexColorButton;
	QLabel *mSelectedVertexColorLabel;
	
	//selected face color
	QColor mSelectedFaceColor;
	QPushButton *mSelectedFaceColorButton;
	QLabel *mSelectedFaceColorLabel;
	
	//vertex id bg color
	QColor mVertexIDBgColor;
	QPushButton *mVertexIDBgColorButton;
	QLabel *mVertexIDBgColorLabel;
	
	//face id bg color
	QColor mFaceIDBgColor;
	QPushButton *mFaceIDBgColorButton;
	QLabel *mFaceIDBgColorLabel;
	
	//edge id bg color
	QColor mEdgeIDBgColor;
	QPushButton *mEdgeIDBgColorButton;
	QLabel *mEdgeIDBgColorLabel;
	
	//reset button
	QPushButton *mResetColorsButton;
	
	//reset camera button
	QPushButton *mResetCameraButton;
	
public:
	TopModPreferences(QSettings *settings, StyleSheetEditor *sse, QShortcutManager *sm, QWidget *parent = 0 );
	~TopModPreferences();
	
	int display();
	void readSettings();

protected:
	void createTabs();
	void setupColors();
	void setupMain();
	
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
	void setSelectedVertexColor();
	void setSelectedEdgeColor();
	void setSelectedFaceColor();
	void setVertexIDBgColor();
	void setFaceIDBgColor();
	void setEdgeIDBgColor();

	void loadDefaults();

};

#endif
