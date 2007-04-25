/****************************************************************************
 **
 **
 ****************************************************************************/

#ifndef CONICALMODE_H
#define CONICALMODE_H

#include <QWidget>

#include <QBoxLayout>
#include <QStackedWidget>
#include <QComboBox>

#include "MainWindow.hh"

class MainWindow;
class DLFLWindow;

class QComboBox;
class QStackedWidget;
class QGroupBox;
class QVBoxLayout;
class QCheckBox;
class QLabel;

class ConicalMode : public QWidget {
	Q_OBJECT

public:
	ConicalMode(QWidget *parent = 0);
	void insertTab(QTabWidget *tabWidget);
	int getLastMode();
	
protected:
	void setupCutbyEdge();
	void setupCutbyVertex();
	void setupCutbyEdgeVertex();
	void setupCutbyFace();
	void setupTruncateEdge();
	void setupTruncateVertex();
	void setupDualConvexHull();
	
public slots:
	void switchMode(int index);
	void changeCutOffsetE(double value);
	void changeCutOffsetV(double value);
	void toggleGlobalCut(int state);
	void toggleSelectedCut(int state);
	void changeTiltPlane1(double value);
	void changeTiltPlane2(double value);	
	
private:
	
	int lastMode;
	
	QLabel *conicalLabel;
	QGroupBox *conicalGroup;
	QVBoxLayout *conicalBoxLayout;
	QComboBox *conicalComboBox;
	
	QStackedWidget *stackedWidget;
	
	QWidget *cutbyEdgeWidget;
	QWidget *cutbyVertexWidget;
	QWidget *cutbyEdgeVertexWidget;
	QWidget *cutbyFaceWidget;
	QWidget *truncateEdgeWidget;
	QWidget *truncateVertexWidget; 
	QWidget *dualConvexHullWidget; 
	
	QVBoxLayout *cutbyEdgeLayout;
	QVBoxLayout *cutbyVertexLayout;
	QVBoxLayout *cutbyEdgeVertexLayout;
	QVBoxLayout *cutbyFaceLayout;
	QVBoxLayout *truncateEdgeLayout;
	QVBoxLayout *truncateVertexLayout; 
	QVBoxLayout *dualConvexHullLayout; 

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
};

#endif