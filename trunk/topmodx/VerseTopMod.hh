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

/****************************************************************************
 **
 **
 ****************************************************************************/
#ifdef WITH_VERSE

#ifndef VERSETOPMOD_H
#define VERSETOPMOD_H

#include <QWidget>
#include <QProcess>
#include <QTimer>

#include "MainWindow.hh"
#include "TKE_verse.h"
#include "TIF_verse.h"
#include "TKE_global.h"

// #include "TLI_dynamiclist.h"
// #include "verse_session.c"
// #include "verse.h"

class MainWindow;
class QLineEdit;
class QTextEdit;

// extern ListBase session_list;

class VerseTopMod : public QWidget {
	Q_OBJECT
	
public:
  static VerseTopMod* Instance(QWidget *parent);
  static VerseTopMod* Instance();
	void write(QString s);
protected:
	// VerseTopMod();
	VerseTopMod(QWidget *parent = 0, Qt::WindowFlags f = Qt::Tool );
	VerseTopMod(const VerseTopMod&);
	VerseTopMod& operator= (const VerseTopMod&);
	~VerseTopMod();
private:
	bool isConnected;
  QLineEdit *mLineEdit;
  QTextEdit *mTextEdit;
	static VerseTopMod* pinstance;    
	// QEventLoop *mEventLoop;
	QTimer *mTimer;
	QProcess *mProcess;
	QWidget *mParent;
	
public slots:

	void killServer();
	void startServer();
	void writeStandardOutput();
	void executeCommand();
	//actions in the file menu
	void connectLocalhost();
	void connectHost();
	void disconnectHost();
	void disconnectAll();  
	void updateVerse();
};

#endif

#endif
