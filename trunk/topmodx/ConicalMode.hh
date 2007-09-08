/****************************************************************************
 **
 **
 ****************************************************************************/

#ifndef CONICALMODE_H
#define CONICALMODE_H

#include <QWidget>

#include <QGridLayout>
#include <QStackedWidget>
#include <QComboBox>

#include "MainWindow.hh"

class MainWindow;

class ConicalMode : public QWidget {
	Q_OBJECT

public:
	ConicalMode(QWidget *parent, QShortcutManager *sm);
	void addActions(QActionGroup *actionGroup, QToolBar *toolBar, QStackedWidget *stackedWidget);
	QMenu *getMenu();
	void retranslateUi();
	
	QDoubleSpinBox *createDoubleSpinBox(QGridLayout *layout, QLabel *label, QString s, double low, double high, double step, double value, double decimals, int row, int col);

	QWidget *mCutbyEdgeWidget;
	QWidget *mCutbyVertexWidget;
	QWidget *mCutbyEdgeVertexWidget;
	QWidget *mCutbyFaceWidget;
	QWidget *mTruncateEdgeWidget;
	QWidget *mTruncateVertexWidget; 
	QWidget *mDualConvexHullWidget; 

	QAction *mCutbyEdgeAction;
	QAction *mCutbyVertexAction;
	QAction *mCutbyEdgeVertexAction;
	QAction *mCutbyFaceAction;
	QAction *mTruncateEdgeAction;
	QAction *mTruncateVertexAction; 
	QAction *mDualConvexHullAction;
	
	QGridLayout *mCutbyEdgeLayout;
	QGridLayout *mCutbyVertexLayout;
	QGridLayout *mCutbyEdgeVertexLayout;
	QGridLayout *mCutbyFaceLayout;
	QGridLayout *mTruncateEdgeLayout;
	QGridLayout *mTruncateVertexLayout; 
	QGridLayout *mDualConvexHullLayout;
	
protected:
	void setupCutbyEdge();
	void setupCutbyVertex();
	void setupCutbyEdgeVertex();
	void setupCutbyFace();
	void setupTruncateEdge();
	void setupTruncateVertex();
	void setupDualConvexHull();
	
public slots:
	void changeCutOffsetE(double value);
	void changeCutOffsetV(double value);
	void toggleGlobalCut(int state);
	void toggleSelectedCut(int state);
	void changeTiltPlane1(double value);
	void changeTiltPlane2(double value);
	
	void triggerCutbyEdge();
	void triggerCutbyVertex();
	void triggerCutbyEdgeVertex();
	void triggerCutbyFace();
	void triggerTruncateEdge();
	void triggerTruncateVertex();
	void triggerDualConvexHull();

private: 

	QWidget *mParent;
	QMenu *mConicalMenu;

	QDoubleSpinBox *cutbyEdgeOffsetSpinBox;
	QDoubleSpinBox *cutbyEdgeTiltPlane1SpinBox;
	QDoubleSpinBox *cutbyEdgeTiltPlane2SpinBox;
	QDoubleSpinBox *cutbyVertexOffsetSpinBox;
	QDoubleSpinBox *cutbyEdgeVertexOffsetESpinBox;
	QDoubleSpinBox *cutbyEdgeVertexOffsetVSpinBox;
	QDoubleSpinBox *cutbyEdgeVertexTiltPlane1SpinBox;
	QDoubleSpinBox *cutbyEdgeVertexTiltPlane2SpinBox;
	QDoubleSpinBox *cutbyFaceOffsetSpinBox;
	QDoubleSpinBox *truncateEdgeOffsetSpinBox;
	QDoubleSpinBox *truncateVertexOffsetSpinBox;

	QCheckBox *cutbyEdgeGlobalCheckBox;
	QCheckBox *cutbyEdgeCutSelectedCheckBox;
	QCheckBox *cutbyVertexGlobalCheckBox;
	QCheckBox *cutbyVertexCutSelectedCheckBox;
	QCheckBox *cutbyEdgeVertexGlobalCheckBox;
	QCheckBox *cutbyEdgeVertexCutSelectedCheckBox;
	QCheckBox *cutbyFaceGlobalCheckBox;
	QCheckBox *cutbyFaceCutSelectedCheckBox;
	QCheckBox *truncateEdgeGlobalCheckBox;
	QCheckBox *truncateEdgeCutSelectedCheckBox;
	QCheckBox *truncateVertexGlobalCheckBox;
	QCheckBox *truncateVertexCutSelectedCheckBox;
	
	QLabel *cutbyEdgeOffsetLabel;
	QLabel *cutbyVertexOffsetLabel;
	QLabel *cutbyFaceOffsetLabel;
	
};

#endif
