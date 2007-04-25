/****************************************************************************
 **
 **
 ****************************************************************************/

#ifndef REMESHINGMODE_H
#define REMESHINGMODE_H

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

class RemeshingMode : public QWidget {
	Q_OBJECT

public:
	RemeshingMode(QWidget *parent = 0);
	void insertTab(QTabWidget *tabWidget);
	int getLastMode();
	
protected:
	void setupDual();
	void setupThreeConversion();
	void setupFourConversion();
	void setupFiveConversion();
	void setupThreePreservation();
	void setupFourPreservation();
	void setupFivePreservation();
	void setupSixPreservation();
	void setupMiscellaneous();	

public slots:
	void switchMode(int index);	
	void switchThreeConversionMode(int index);
	void switchFourConversionMode(int index);
	void switchFiveConversionMode(int index);
	void switchThreePreservationMode(int index);
	void switchFourPreservationMode(int index);
	void switchFivePreservationMode(int index);
	void switchSixPreservationMode(int index);
	void switchMiscellaneousMode(int index);
	
private:
	
	int lastMode;
	int lastMiscellaneousMode;
	
	int lastThreeConversionScheme;
	int lastFourConversionScheme;
	int lastFiveConversionScheme;
	int lastThreePreservationScheme;
	int lastFourPreservationScheme;
	int lastFivePreservationScheme;
	int lastSixPreservationScheme;
	int lastMiscellaneousScheme;
	
	QLabel *remeshingLabel;
	QGroupBox *remeshingGroup;
	QVBoxLayout *remeshingBoxLayout;
	QComboBox *remeshingComboBox;
	
	QStackedWidget *stackedWidget;
	
	QWidget *dualWidget;
	QWidget *threeConversionWidget;
	QWidget *fourConversionWidget;
	QWidget *fiveConversionWidget; 
	QWidget *threePreservationWidget;
	QWidget *fourPreservationWidget;
	QWidget *fivePreservationWidget; 
	QWidget *sixPreservationWidget;	
	QWidget *miscellaneousWidget;	
	
	QVBoxLayout *dualLayout;
	QVBoxLayout *threeConversionLayout;
	QVBoxLayout *fourConversionLayout;
	QVBoxLayout *fiveConversionLayout; 
	QVBoxLayout *threePreservationLayout;
	QVBoxLayout *fourPreservationLayout;
	QVBoxLayout *fivePreservationLayout; 
	QVBoxLayout *sixPreservationLayout; 
	QVBoxLayout *miscellaneousLayout;
	
	QComboBox *threeConversionComboBox;
	QComboBox *fourConversionComboBox;
	QLabel *fourConversionLabel;
	
	
	
};

#endif