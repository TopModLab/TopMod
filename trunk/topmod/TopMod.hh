#ifndef _TOPMOD_H
#define _TOPMOD_H

#include <QApplication>
#include <QEvent>
// 
#include "MainWindow.hh"

class TopMod : public QApplication {
	Q_OBJECT

private:
		MainWindow *mainWindow;

public:
	TopMod( int & argc, char ** argv, bool GUIenabled );
	~TopMod();
	
protected:
	bool event(QEvent *event);
	
};

#endif
