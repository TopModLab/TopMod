/****************************************************************************
 **
 **
 ****************************************************************************/

#ifndef BASICSMODE_H
#define BASICSMODE_H

#include <QWidget>
#include <QPushButton>
#include <QGridLayout>

/*!
	\file BasicsMode.hh
	\brief Definition of the BasicsMode class
	
	\see BasicsMode
*/

#include "MainWindow.hh"

class MainWindow;

class QComboBox;
class QPushButton;
class QGroupBox;
class QCheckBox;
class QLabel;

class BasicsMode : public QWidget {
	Q_OBJECT
	
public:
	BasicsMode(QWidget *parent, QShortcutManager *sm, QWidget *actionList);
	void addActions(QActionGroup *actionGroup, QToolBar *toolBar, QStackedWidget *stackedWidget);
	QMenu* getMenu();
	void retranslateUi();
	
	QAction *mInsertEdgeAction;
	QAction *mDeleteEdgeAction;	
	QAction *mCollapseEdgeAction;	
	QAction *mSubdivideEdgeAction;	
	QAction *mConnectEdgesAction;	
	QAction *mSpliceCornersAction;
	QAction *mTransformsAction;
	QAction *mSelectionOptionsAction;
	
	QWidget *mInsertEdgeWidget;
	QWidget *mDeleteEdgeWidget;
	QWidget *mCollapseEdgeWidget;
	QWidget *mSubdivideEdgeWidget; 
	QWidget *mConnectEdgesWidget;
	QWidget *mSpliceCornersWidget; 
	QWidget *mTransformsWidget;	
	QWidget *mSelectionOptionsWidget;	
	
	QGridLayout *mInsertEdgeLayout;
	QGridLayout *mDeleteEdgeLayout;
	QGridLayout *mCollapseEdgeLayout;
	QGridLayout *mSubdivideEdgeLayout; 
	QGridLayout *mConnectEdgesLayout;
	QGridLayout *mSpliceCornersLayout; 
	QGridLayout *mTransformsLayout;
	QGridLayout *mSelectionOptionsLayout;

protected:
	void setupInsertEdge();
	void setupDeleteEdge();
	void setupCollapseEdge();
	void setupSubdivideEdge();
	void setupConnectEdges();
	void setupSpliceCorners();
	void setupTransforms();
	void setupSelectionOptions();
	QDoubleSpinBox *createDoubleSpinBox(QGridLayout *layout, QLabel *label, QString s, double low, double high, double step, double value, double decimals, int row, int col);
	

private:
	
	QWidget *mParent;
	QMenu *mBasicsMenu;	
	
	QLabel *numSubdivsLabel;
	QDoubleSpinBox *numSubdivsSpinBox;
	QLabel *transformLabel;
	QLabel *xPosLabel;
	QLabel *yPosLabel;
	QLabel *zPosLabel;	
	QLabel *scaleLabel;
	QLabel *xScaleLabel;
	QLabel *yScaleLabel;
	QLabel *zScaleLabel;
	
	QLabel *noOptionsInsertEdgeLabel;
	QCheckBox *cleanupDeleteEdgeCheckBox;
	QLabel *noOptionsCollapseEdgeLabel;
	QLabel *noOptionsConnectEdgesLabel;
	QLabel *noOptionsSpliceCornersLabel;
	
	QPushButton *freezeTransformsButton;
	
	//transform spinboxes
	QDoubleSpinBox *xScaleSpinBox;
	QDoubleSpinBox *yScaleSpinBox;
	QDoubleSpinBox *zScaleSpinBox;
	QDoubleSpinBox *xPosSpinBox;
	QDoubleSpinBox *yPosSpinBox;
	QDoubleSpinBox *zPosSpinBox;
	
	QLabel *mFaceAreaToleranceLabel;
	QDoubleSpinBox *mFaceAreaToleranceSpinBox;
	
public slots:
	void freezeTransforms();
		
	void triggerInsertEdge();
	void triggerDeleteEdge();
	void triggerCollapseEdge();
	void triggerSubdivideEdge();
	void triggerConnectEdges();
	void triggerSpliceCorners();
	void triggerTransforms();
	void triggerSelectionOptions();
};

#endif
