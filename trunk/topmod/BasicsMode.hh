/****************************************************************************
 **
 **
 ****************************************************************************/

#ifndef BASICSMODE_H
#define BASICSMODE_H

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

// class MainWindow;

class BasicsMode : public QWidget {
	Q_OBJECT

public:
	BasicsMode(QWidget *parent = 0);
	void insertTab(QTabWidget *tabWidget);
	int getLastMode();
			
protected:
	void setupInsertEdge();
	void setupDeleteEdge();
	void setupCollapseEdge();
	void setupSubdivideEdge();
	void setupConnectEdges();
	void setupSpliceCorners();
	void setupTransforms();
	
// public slots:
// 	void setMode(int index);
	
private:
	
	int lastMode;
	
	QLabel *basicsLabel;
	QGroupBox *basicsGroup;
	QVBoxLayout *basicsBoxLayout;
	QComboBox *basicsComboBox;
	
	QStackedWidget *stackedWidget;
	
	QWidget *insertEdgeWidget;
	QWidget *deleteEdgeWidget;
	QWidget *collapseEdgeWidget;
	QWidget *subdivideEdgeWidget; 
	QWidget *connectEdgesWidget;
	QWidget *spliceCornersWidget; 
	QWidget *transformsWidget;	
	
	QVBoxLayout *insertEdgeLayout;
	QVBoxLayout *deleteEdgeLayout;
	QVBoxLayout *collapseEdgeLayout;
	QVBoxLayout *subdivideEdgeLayout; 
	QVBoxLayout *connectEdgesLayout;
	QVBoxLayout *spliceCornersLayout; 
	QVBoxLayout *transformsLayout;
	
	//transform spinboxes
	QDoubleSpinBox *xScaleSpinBox;
	QDoubleSpinBox *yScaleSpinBox;
	QDoubleSpinBox *zScaleSpinBox;
	QDoubleSpinBox *xPosSpinBox;
	QDoubleSpinBox *yPosSpinBox;
	QDoubleSpinBox *zPosSpinBox;
	
	
public slots:
	void switchMode(int index);	
	void freeze_transforms();
	
	// void mousePressEvent(QMouseEvent * event);
	// void mouseReleaseEvent(QMouseEvent * event);
	// void mouseMoveEvent(QMouseEvent * event);
	
};

#endif