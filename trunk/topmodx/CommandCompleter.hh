#ifndef _COMMANDCOMPLETER_HH_
#define _COMMANDCOMPLETER_HH_

#include <QDialog>
#include <QLineEdit>
#include <QCompleter>
#include <QVBoxLayout>
#include <QStringList>
#include <QAbstractItemModel>
#include <QFile>
#include <QStringListModel>
#include <QApplication>
#include <QLabel>
#include <QStyle>
#include <QPalette>
#include <QShortcut>
#include <QStandardItemModel>
#include <QAction>

/*!
* \file CommandCompleter.hh
* \class CommandCompleter
* 
* \brief Quicksilver-like popup commandline interface
* 
* CommandCompleter provides a popup Quicksilver-like command line interface for accessing
* all the functionality of TopMod with keyboard input instead of mouse clicks.
* 
*/

class CommandCompleter : public QDialog {
	Q_OBJECT

public:

	CommandCompleter ( QWidget *m, QWidget * parent = 0, Qt::WindowFlags f = 0 );
	
private:
	
	QLineEdit *mLineEdit;								//!< single line input widget
	QStringList mWordList;							//!< array of strings to feed to the AutoCompleter class
	QCompleter *mCompleter;							//!< the autoCompletion widget
	QLabel *mQuickCommandLabel;					//!< short instructions displayed in the popup window
	QStringListModel *mModel;						//!< the completion model for the AutoCompleter widget
		
public slots:

	/*!
	* \brief causes the CommandCompleter dialog to popup and sets the focus to the text input box
	* 
	* this dialog for now is modal, meaning no other windows will accept focus while this window is displayed.
	* the user can hit escape to hide the window if they do not want to input anything, or simply hit enter.
	* 
	*/
	int exec(){
		mLineEdit->clear();
		if ( QDialog::exec() == QDialog::Accepted )
			return mWordList.indexOf(mLineEdit->text());
		// else hide();
		return -2;
	}

};
	
#endif