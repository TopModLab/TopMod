/****************************************************************************
 **
 **
 ****************************************************************************/

#ifndef TEXTURINGMODE_H
#define TEXTURINGMODE_H

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

class TexturingMode : public QWidget {
	Q_OBJECT

public:
	TexturingMode(QWidget *parent = 0);
	void insertTab(QTabWidget *tabWidget);
	int getLastMode();
	
protected:
	void setupTileTexturing();
	
public slots:
	void switchMode(int index);	
	
	// void mousePressEvent(QMouseEvent * event);
	// void mouseReleaseEvent(QMouseEvent * event);
	// void mouseMoveEvent(QMouseEvent * event);
	

private:
	
	int lastMode;
	
	QLabel *texturingLabel;
	QGroupBox *texturingGroup;
	QVBoxLayout *texturingBoxLayout;
	QComboBox *texturingComboBox;
	
	QStackedWidget *stackedWidget;
	
	QWidget *tileTexturingWidget;	
	
	QVBoxLayout *tileTexturingLayout;
};

#endif