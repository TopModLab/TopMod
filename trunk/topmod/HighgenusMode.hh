/****************************************************************************
 **
 **
 ****************************************************************************/

#ifndef HIGHGENUSMODE_H
#define HIGHGENUSMODE_H

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

class HighgenusMode : public QWidget {
	Q_OBJECT

public:
	HighgenusMode(QWidget *parent = 0);
	void insertTab(QTabWidget *tabWidget);
	int getLastMode();

protected:
	void setupAddHoleHandle();
	void setupAddHoleHandleCV();
	void setupAddHandleSI();
	void setupRindModelingScaling();
	void setupRindModelingThickness();
	void setupWireframeModeling();
	void setupColumnModeling();
	void setupSierpinsky();
	void setupMultiFaceHandle();
	void setupMengerSponge();
	
public slots:
	void switchMode(int index);	
	void numSegmentsValueChanged(int value);
	void changeMultiFaceAlgorithm(bool on);
	void toggleMultiFaceHandleUseMaxOffsetFlag(int state);
	void toggleSymmetricWeightsFlag(int state);
	
	void toggleCrustCleanupFlag(int state);
	void numSegmentsConnectValueChanged(int value);

private:
	
	int lastMode;
	
	QLabel *highgenusLabel;
	QGroupBox *highgenusGroup;
	QVBoxLayout *highgenusBoxLayout;
	QComboBox *highgenusComboBox;
	
	QStackedWidget *stackedWidget;
	
	QWidget *addHoleHandleWidget;
	QWidget *addHoleHandleCVWidget;
	QWidget *addHandleSIWidget;
	QWidget *rindModelingScalingWidget; 
	QWidget *rindModelingThicknessWidget;
	QWidget *wireframeModelingWidget; 
	QWidget *columnModelingWidget;	
	QWidget *sierpinskyWidget;
	QWidget *multiFaceHandleWidget;
	QWidget *mengerSpongeWidget;
	
	QVBoxLayout *addHoleHandleLayout;
	QVBoxLayout *addHoleHandleCVLayout;
	QVBoxLayout *addHandleSILayout;
	QVBoxLayout *rindModelingScalingLayout;
	QVBoxLayout *rindModelingThicknessLayout;
	QVBoxLayout *wireframeModelingLayout;
	QVBoxLayout *columnModelingLayout; 
	QVBoxLayout *sierpinskyLayout;
	QVBoxLayout *multiFaceHandleLayout; 
	QVBoxLayout *mengerSpongeLayout;
	
	QCheckBox *rindModelingThicknessCleanupCheckBox;
	QCheckBox *rindModelingScalingCleanupCheckBox;

	QSpinBox *addHoleHandleNumSegmentsConnectSpinBox;
	QSpinBox *addHoleHandleNumSegmentsSpinBox;
	
	QSpinBox *addHoleHandleCVNumSegmentsSpinBox;
	QSpinBox *addHoleHandleCVNumSegmentsConnectSpinBox;
	
	QSpinBox *addHandleSINumSegmentsSpinBox;
	QSpinBox *addHandleSINumSegmentsConnectSpinBox;
	
	QSpinBox *addHandleSITwistsConnectSpinBox;
	QDoubleSpinBox *addHandleSIWeight2SpinBox;
	
	QLabel *multiFaceHandleExtrudeDistanceLabel;
	QLabel *multiFaceHandleScaleLabel;
	QDoubleSpinBox *multiFaceHandleExtrudeDistanceSpinBox;
	QCheckBox *multiFaceHandleMaxOffsetsCheckBox;
	QDoubleSpinBox *multiFaceHandleScaleSpinBox;
};

#endif