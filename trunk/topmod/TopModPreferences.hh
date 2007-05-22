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


class TopModPreferences : public QDialog {
	Q_OBJECT

private:
	QSettings *mSettings;
	QTabWidget *mPrefTabs;
	QWidget *mParent;
	
	QPushButton *mOkButton;
	QPushButton *mCancelButton;
	
public:
	TopModPreferences(QSettings *settings, QWidget *parent = 0 );
	~TopModPreferences();

protected:
	void createTabs();

	
};

#endif
