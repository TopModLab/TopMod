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
* Contributor(s): Stuart Tett.
*
* ***** END GPL LICENSE BLOCK *****
*/

#ifndef _DLFLSCRIPT_EDITOR_HH_
#define _DLFLSCRIPT_EDITOR_HH_

#ifdef WITH_PYTHON

#undef slots
#ifdef _DEBUG
#undef _DEBUG
#include <Python.h>
#define _DEBUG
#else
#include <Python.h>
#endif
#define slots

/*
* The macros below is defined as real functions in <locale>,
* while <pyport.h> defines them as macros with a slightly different signature.
* Though Python 2.7.13 fixed this issue,
* in case user use lower version of Python, we undefined the macros manually.
*/
#ifdef _PY_PORT_CTYPE_UTF8_ISSUE
#undef isalnum
#undef isalpha
#undef islower
#undef isspace
#undef isupper
#undef tolower
#undef toupper
#endif

#include <QWidget>
#include <QMenuBar>
#include <QToolBar>
#include <QPushButton>
#include <QSpinBox>
#include <iostream>

//class QLineEdit;
class QTextEdit;
class QProcess;

#include <DLFLObject.h>
#include "editor.h"

#include "PythonHighlighter.h"

using namespace DLFL;

// Stuff for the DLFL Extension Module's C API
static void **PyDLFL_API;
#define PyDLFL_UsingGUI (*(void (*)(bool ugui)) PyDLFL_API[0])
#define PyDLFL_PassObject (*(void (*)(DLFLObject* obj)) PyDLFL_API[1])

class DLFLScriptEditor : public QWidget
{
	Q_OBJECT

public:
	DLFLScriptEditor(DLFLObjectPtr obj = nullptr, QWidget *parent = 0, Qt::WindowFlags f = Qt::Tool);
	~DLFLScriptEditor();

	QColor& outputBgColor() { return mOutputBgColor; };
	QColor& inputBgColor() { return mInputBgColor; };
	void retranslateUi();

signals:
	void makingChange(); // for undo push
	void cmdExecuted();
	void addToHistory(const QString& item);
	void requestObject(QString fileName);
	private slots :
	void executeCommand();
	void echoCommand(QString cmd);
	void toggleTabWidthWidget();
	public slots :
	void loadObject(DLFLObject* obj, QString fileName);
	void loadDLFLModule(QString newPath);
	void execFile();
	void openFile();
	void saveFile();
	void saveOutput();
	void clearHistory();
	void clearInput();
	void toggleEchoing();
	// Run this function to convert spaces to TABs
	void setTabWidth(int width);
	void spacesToTABs();

public:
	QString mAddToPath;

private:
	// The place where the user types the commands (multiline)
	Editor *mCommandEdit;
	// The place where executed commands are executed
	QTextEdit *mHistoryBox;
	// The menubar with some options
	QMenuBar *mMenuBar;
	QMenu *mScriptMenu;
	QAction *mExecFile, *mOpenFile, *mSaveFile, *mSaveOutput;
	QAction *mClearHistory;
	QAction *mClearInput;
	QAction *mToggleEchoing;

	// For Spaces to TABs conversion
	QMenu *mFormatMenu;
	QAction *mSetTabWidth;
	QAction *mConvertSpaces;

	PythonHighlighter *pyhigh;

	QColor mOutputBgColor;
	QColor mInputBgColor;

	void PyInit();
	PyObject *dlfl_module, *dlfl_dict;
	PyObject *main_module, *main_dict;

	bool mEchoing;

	QString pathPython;

	// The number of spaces equal to a TAB
	// will get converted at the beginning of a line
	int mTabWidth;
	QSpinBox *mSetTabWidthWidget;

};

#endif

#endif // _DLFLSCRIPT_EDITOR_HH_
