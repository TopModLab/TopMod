/*
* $Id: SourceFileTemplates.txt,v 1.2 2005/07/28 15:37:03 JiriHnidek Exp www-data $
*
* ***** BEGIN GPL LICENSE BLOCK *****
*
* This program is free software; you can redistribute it and/or
* modify it under the terms of the GNU General Public License
* as published by the Free Software Foundation; either version 2
* of the License, or (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program; if not, write to the Free Software  Foundation,
* Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
*
* The Original Code is Copyright (C) 2005 by xxxxxxxxxxxxxx
* All rights reserved.
*
* The Original Code is: all of this file.
*
* Contributor(s): none yet.
*
* ***** END GPL LICENSE BLOCK *****
*/

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
#ifdef WITH_PYTHON
	QWidget *mScriptEditTab;
#endif
	QShortcutDialog *mShortcutsTab;
	QShortcutManager *mShortcutsManager;
	StyleSheetEditor *mStyleSheetsTab;
	
		//COLORS
	
	//viewport color
	QColor mViewportColor, mViewportColorDefault;
	QPushButton *mViewportColorButton;
	QLabel *mViewportColorLabel;
	
	//Render color
	QColor mRenderColor,mRenderColorDefault;
	QPushButton *mRenderColorButton;
	QLabel *mRenderColorLabel;
	
	//Cool Light Color
	QColor mCoolLightColor,mCoolLightColorDefault;
	QPushButton *mCoolLightColorButton;
	QLabel *mCoolLightColorLabel;
	
	//Warm Light Color
	QColor mWarmLightColor,mWarmLightColorDefault;
	QPushButton *mWarmLightColorButton;
	QLabel *mWarmLightColorLabel;
	
	//Wireframe Color
	QColor mWireframeColor,mWireframeColorDefault;
	QPushButton *mWireframeColorButton;
	QLabel *mWireframeColorLabel;
	
	//Silhouette Color
	QColor mSilhouetteColor,mSilhouetteColorDefault;
	QPushButton *mSilhouetteColorButton;
	QLabel *mSilhouetteColorLabel;

	//FaceCentroid Color
	QColor mFaceCentroidColor, mFaceCentroidColorDefault;
	QPushButton *mFaceCentroidColorButton;
	QLabel *mFaceCentroidColorLabel;

	//Normal Color
	QColor mNormalColor,mNormalColorDefault;
	QPushButton *mNormalColorButton;
	QLabel *mNormalColorLabel;
	
	//patch boundary color
	QColor mPatchBoundaryColor,mPatchBoundaryColorDefault;
	QPushButton *mPatchBoundaryColorButton;
	QLabel *mPatchBoundaryColorLabel;
	
	//light intensity
	double mLightIntensity,mLightIntensityDefault;
	QLabel *mLightIntensityLabel;
	QDoubleSpinBox *mLightIntensitySpinBox;
	
	//wireframe thickness
	double mWireframeThickness,mWireframeThicknessDefault;
	QLabel *mWireframeThicknessLabel;
	QDoubleSpinBox *mWireframeThicknessSpinBox;
	
	//silhouette thickness
	double mSilhouetteThickness,mSilhouetteThicknessDefault;
	QLabel *mSilhouetteThicknessLabel;
	QDoubleSpinBox *mSilhouetteThicknessSpinBox;
	
	//vertex thickness
	double mVertexThickness,mVertexThicknessDefault;
	QLabel *mVertexThicknessLabel;
	QDoubleSpinBox *mVertexThicknessSpinBox;

	//Normal thickness
	double mNormalThickness,mNormalThicknessDefault;
	QLabel *mNormalThicknessLabel;
	QDoubleSpinBox *mNormalThicknessSpinBox;

	//Normal Length
	double mNormalLength,mNormalLengthDefault;
	QLabel *mNormalLengthLabel;
	QDoubleSpinBox *mNormalLengthSpinBox;

	//face centroid thickness
	double mFaceCentroidThickness,mFaceCentroidThicknessDefault;
	QLabel *mFaceCentroidThicknessLabel;
	QDoubleSpinBox *mFaceCentroidThicknessSpinBox;

	//selected vertex thickness
	double mSelectedVertexThickness,mSelectedVertexThicknessDefault;
	QLabel *mSelectedVertexThicknessLabel;
	QDoubleSpinBox *mSelectedVertexThicknessSpinBox;

	//selected edge thickness
	double mSelectedEdgeThickness,mSelectedEdgeThicknessDefault;
	QLabel *mSelectedEdgeThicknessLabel;
	QDoubleSpinBox *mSelectedEdgeThicknessSpinBox;
	
	//selected edge color
	QColor mSelectedEdgeColor,mSelectedEdgeColorDefault;
	QPushButton *mSelectedEdgeColorButton;
	QLabel *mSelectedEdgeColorLabel;
	
	//selected vertex color
	QColor mSelectedVertexColor,mSelectedVertexColorDefault;
	QPushButton *mSelectedVertexColorButton;
	QLabel *mSelectedVertexColorLabel;
	
	//selected face color
	QColor mSelectedFaceColor,mSelectedFaceColorDefault;
	QPushButton *mSelectedFaceColorButton;
	QLabel *mSelectedFaceColorLabel;
	
	//vertex id bg color
	QColor mVertexIDBgColor,mVertexIDBgColorDefault;
	QPushButton *mVertexIDBgColorButton;
	QLabel *mVertexIDBgColorLabel;
	
	//face id bg color
	QColor mFaceIDBgColor,mFaceIDBgColorDefault;
	QPushButton *mFaceIDBgColorButton;
	QLabel *mFaceIDBgColorLabel;
	
	//edge id bg color
	QColor mEdgeIDBgColor,mEdgeIDBgColorDefault;
	QPushButton *mEdgeIDBgColorButton;
	QLabel *mEdgeIDBgColorLabel;
	
	//reset button
	QPushButton *mResetColorsButton;
	
	//reset camera button
	QPushButton *mResetCameraButton;
	
	//camera nearplane/farplane/fov
	QLabel *mCameraFovLabel,*mCameraNearPlaneLabel,*mCameraFarPlaneLabel;
	QDoubleSpinBox *mCameraFovSpinBox,*mCameraNearPlaneSpinBox,*mCameraFarPlaneSpinBox;
	float mCameraFov,mCameraFovDefault;
	float mCameraNearPlane,mCameraNearPlaneDefault;
	float mCameraFarPlane,mCameraFarPlaneDefault;	
	
	bool mShowStartupDialogAtStartup;
	
	QCheckBox *mAutoSaveCheckBox;
	QCheckBox *mIncrementalSaveCheckBox;
	bool mIncrementalSave, mIncrementalSaveDefault;
	QDoubleSpinBox *mAutoSaveDelaySpinBox;
	QLabel *mAutoSaveDelayLabel;
	int mAutoSaveDelay, mAutoSaveDelayDefault;
	bool mAutoSave, mAutoSaveDefault;
	QLineEdit *mSaveDirectoryLineEdit;
	QLabel *mSaveDirectoryLabel;
	QString mSaveDirectory,mSaveDirectoryDefault;
	QDoubleSpinBox *mIncrementalSaveMaxSpinBox;
	QLabel *mIncrementalSaveMaxLabel;
	int mIncrementalSaveMax, mIncrementalSaveMaxDefault;
	
	QCheckBox *mCommandCompleterIndexToggle;
	bool mCommandCompleterIndex,mCommandCompleterIndexDefault;

	QCheckBox *mSingleClickExtrudeCheckBox;
	bool mSingleClickExtrude, mSingleClickExtrudeDefault;
	
public:
	TopModPreferences(QSettings *settings, StyleSheetEditor *sse, QShortcutManager *sm, QWidget *parent = 0 );
	~TopModPreferences();
	
	int display();
	void readSettings();
	void setSaveDirectory(QString s);

protected:
	void createTabs();
	void setupColors();
	void setupMain();
	QPushButton* addColorPreference(QLabel *label, QString text, QColor color, QGridLayout *layout, QStyle *style, int row, int col);
	void setButtonColor(QColor color, QPushButton *button);
	QDoubleSpinBox* addSpinBoxPreference(QLabel *label, QString text, double low, double high, double step, double value, int decimals, QGridLayout *layout, int row, int col);
	
	
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
	void setNormalColor();
	void setFaceCentroidColor();

	void loadDefaults();

};

#endif
