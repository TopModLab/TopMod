/*
*
* ***** BEGIN GPL LICENSE BLOCK *****
*
* This program is free software; you can redistribute it and/or
* modify it under the terms of the GNU General Public License
* as published by the Free Software Foundation; either version 2
* of the License, or (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program; if not, write to the Free Software  Foundation,
* Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
*
* The Original Code is Copyright (C) 2005 by xxxxxxxxxxxxxx
* All rights reserved.
*
* The Original Code is: all of this file.
*
* Contributor(s): none yet.
*
* ***** END GPL LICENSE BLOCK *****
*/

#ifdef QCOMPLETER

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
#endif //QCOMPLETER