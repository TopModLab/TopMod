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

/*!
	\file TexturingMode.hh
	\brief Definition of the TexturingMode class
	
	\see TexturingMode
*/

#include "MainWindow.hh"
class MainWindow;

class TexturingMode : public QWidget {
	Q_OBJECT

public:
	TexturingMode(QWidget *parent, QShortcutManager *sm, QWidget *actionList);
	void addActions(QActionGroup *actionGroup, QToolBar *toolBar, QStackedWidget *stackedWidget);
	QMenu *getMenu();
	void retranslateUi();

	QAction *mTileTexturingAction;
	QWidget *mTileTexturingWidget;	

protected:
	void setupTileTexturing();
	QDoubleSpinBox *createDoubleSpinBox(QGridLayout *layout, QLabel *label, QString s, double low, double high, double step, double value, double decimals, int row, int col);
	
	
public slots:
	void triggerTileTexturing();

private:

	QWidget *mParent;
	QMenu *mTexturingMenu;
	QDoubleSpinBox *tileTexturingNumTilesSpinBox;
	QLabel *tileTexturingNumTilesLabel;
	QPushButton *tileTexturingAssignButton;
	
	QGridLayout *mTileTexturingLayout;	
	
};

#endif
