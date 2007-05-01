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

class TexturingMode : public QWidget {
	Q_OBJECT

public:
	TexturingMode(QWidget *parent = 0);
	void addActions(QActionGroup *actionGroup, QToolBar *toolBar, QStackedWidget *stackedWidget);
	QMenu *getMenu();

	QAction *mTileTexturingAction;
	QWidget *mTileTexturingWidget;	

protected:
	void setupTileTexturing();
	
public slots:
	void triggerTileTexturing();

private:

	QWidget *mParent;
	QMenu *mTexturingMenu;
	
	QBoxLayout *mTileTexturingLayout;	
	
};

#endif
