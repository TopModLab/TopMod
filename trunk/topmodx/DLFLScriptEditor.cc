#ifdef WITH_PYTHON

#include "DLFLScriptEditor.hh"

#include <QtGui>

#ifdef Q_WS_MAC
#include <CoreFoundation/CFBundle.h>
#endif // Q_WS_MAC

DLFLScriptEditor::DLFLScriptEditor( DLFLObjectPtr obj, QWidget *parent, Qt::WindowFlags f ) 
	: QWidget(parent), dlfl_module(NULL),dlfl_dict(NULL),mEchoing(true),pathPython(""),mTabWidth(3),addToPath(".") {

	setMinimumSize( 350, 200 );

	// Text Edit
  mHistoryBox = new QTextEdit;
	mHistoryBox->setTabChangesFocus(false);
  mCommandEdit = new Editor;
	mCommandEdit->setTabChangesFocus(false);
	mCommandEdit->setTabStopWidth( fontMetrics().width( " " ) * mTabWidth ); // req. width in pixels
	
  QObject::connect(mCommandEdit, SIGNAL(ctrlReturnPressed()), this, SLOT(executeCommand()));
  QObject::connect( this, SIGNAL(addToHistory(const QString&)), mCommandEdit, SLOT(appendHistory(const QString&)) );

  QPalette whiteOnBlack, whiteOnGrey;
  mInputBgColor = QColor(0,0,5,255);
  whiteOnBlack.setColor(QPalette::Active, QPalette::Text, Qt::white );
  whiteOnBlack.setColor(QPalette::Active, QPalette::Base, mInputBgColor );
  whiteOnBlack.setColor(QPalette::Inactive, QPalette::Text, Qt::white );
  whiteOnBlack.setColor(QPalette::Inactive, QPalette::Base, mInputBgColor );
	whiteOnBlack.setColor(QPalette::Disabled, QPalette::Text, Qt::white );
  whiteOnBlack.setColor(QPalette::Disabled, QPalette::Base, mInputBgColor );

  mOutputBgColor = QColor(20,20,25,255);
  whiteOnGrey.setColor(QPalette::Active, QPalette::Text, Qt::white );
  whiteOnGrey.setColor(QPalette::Active, QPalette::Base, mOutputBgColor );
  whiteOnGrey.setColor(QPalette::Inactive, QPalette::Text, Qt::white );
  whiteOnGrey.setColor(QPalette::Inactive, QPalette::Base, mOutputBgColor );
  whiteOnGrey.setColor(QPalette::Disabled, QPalette::Text, Qt::white );
  whiteOnGrey.setColor(QPalette::Disabled, QPalette::Base, mOutputBgColor );

  mHistoryBox->setPalette( whiteOnGrey );
  mCommandEdit->setPalette( whiteOnBlack );

  mHistoryBox->setReadOnly(true);

  mHistoryBox->setTabStopWidth(fontMetrics().width( " " ) * mTabWidth);
  mCommandEdit->setTabStopWidth(fontMetrics().width( " " ) * mTabWidth);

  pyhigh = new PythonHighlighter(mHistoryBox);

	// Menu Actions
	mMenuBar = new QMenuBar(this);
	
	mScriptMenu = mMenuBar->addMenu(tr("&Script"));

	mExecFile = new QAction(QIcon(":images/applications-system.png"),tr("&Execute File"), this);
	mExecFile->setStatusTip(tr("Execute an existing Python script"));
	connect(mExecFile, SIGNAL(triggered()), this, SLOT(execFile()));
	mScriptMenu->addAction(mExecFile);

	mOpenFile = new QAction(QIcon(":images/folder-open.png"),tr("&Open File"), this);
	mOpenFile->setStatusTip(tr("Open the input window to a Python script"));
	connect(mOpenFile, SIGNAL(triggered()), this, SLOT(openFile()));
	mScriptMenu->addAction(mOpenFile);

	mSaveFile = new QAction(QIcon(":images/document-save-as.png"),tr("&Save File"), this);
	mSaveFile->setStatusTip(tr("Save the input window to a Python script"));
	connect(mSaveFile, SIGNAL(triggered()), this, SLOT(saveFile()));
	mScriptMenu->addAction(mSaveFile);

	mSaveOutput = new QAction(QIcon(":images/document-save-as.png"),tr("Save Out&put"), this);
	mSaveOutput->setStatusTip(tr("Save the output window to a Python script"));
	connect(mSaveOutput, SIGNAL(triggered()), this, SLOT(saveFile()));
	mScriptMenu->addAction(mSaveOutput);

	mClearHistory = new QAction(QIcon(":images/clear_scriptoutput.png"),tr("Clear &History"), this);
	mClearHistory->setStatusTip(tr("Clears both output window and history"));
	connect(mClearHistory, SIGNAL(triggered()), this, SLOT(clearHistory()));
	mScriptMenu->addAction(mClearHistory);

	mClearInput = new QAction(QIcon(":images/clear_scriptinput.png"),tr("&Clear Input"), this);
	mClearInput->setStatusTip(tr("Clears everythin in the input window"));
	connect(mClearInput, SIGNAL(triggered()), this, SLOT(clearInput()));
	mScriptMenu->addAction(mClearInput);
	
	mToggleEchoing = new QAction(QIcon(":images/accessories-text-editor.png"),tr("&Toggle Echoing"), this);
	mToggleEchoing->setStatusTip(tr("Turn on/off command echoing"));
	connect(mToggleEchoing, SIGNAL(triggered()), this, SLOT(toggleEchoing()));
	mScriptMenu->addAction(mToggleEchoing);

	mFormatMenu = mMenuBar->addMenu(tr("&Format"));

	mConvertSpaces = new QAction(QIcon(":images/applications-system.png"),tr("&Convert Spaces To Tabs"), this);
	mConvertSpaces->setStatusTip((tr("Convert each set of spaces into a TAB character")));
	connect(mConvertSpaces, SIGNAL(triggered()), this, SLOT(spacesToTABs()));
	mFormatMenu->addAction(mConvertSpaces);

	mSetTabWidth = new QAction(QIcon(":images/applications-system.png"),tr("&Set Tab Width"), this);
	mSetTabWidth->setStatusTip((tr("Will affect conversion of spaces to Tabs")));
	connect(mSetTabWidth, SIGNAL(triggered()), this, SLOT(toggleTabWidthWidget()));
	mSetTabWidth->setCheckable(true);
	mSetTabWidth->setChecked(false);
	mFormatMenu->addAction(mSetTabWidth);

	mSetTabWidthWidget = new QSpinBox( this );
	mSetTabWidthWidget->setMinimum(1);
	mSetTabWidthWidget->setSingleStep(1);
	mSetTabWidthWidget->setValue( mTabWidth );
	mSetTabWidthWidget->setPrefix( "Tab Width: ");
	mSetTabWidthWidget->setSuffix( " space(s)");
	mSetTabWidthWidget->hide();
	connect( mSetTabWidthWidget, SIGNAL(valueChanged(int)), this, SLOT(setTabWidth(int)) );

	// Layout
  QVBoxLayout *mainLayout = new QVBoxLayout(this);
	mainLayout->setMenuBar(mMenuBar);
  mainLayout->addWidget(mHistoryBox);
  mainLayout->addWidget(mCommandEdit);
	mainLayout->addWidget(mSetTabWidthWidget);
  setLayout(mainLayout);

#ifdef Q_WS_MAC
	// Get path of bundle
	CFURLRef appUrlRef = CFBundleCopyBundleURL(CFBundleGetMainBundle());
	CFStringRef macPath = CFURLCopyFileSystemPath(appUrlRef,
																								kCFURLPOSIXPathStyle);
	const char *pathPtr = CFStringGetCStringPtr(macPath,
																							CFStringGetSystemEncoding());
	addToPath = QString(pathPtr);
	CFRelease(appUrlRef);
	CFRelease(macPath);
#endif

  PyInit( );
	if( dlfl_module )
		PyDLFL_PassObject( obj );
}

DLFLScriptEditor::~DLFLScriptEditor( ) {
  if( Py_IsInitialized() )
    Py_Finalize( );
}

void DLFLScriptEditor::executeCommand( ) {
	// grab the command typed
  QString command = mCommandEdit->toPlainText();

  if( mCommandEdit->textCursor().hasSelection() ) {
		// if some text was selected, then only execute that as the command
    command = mCommandEdit->textCursor().selection().toPlainText();
  }
  
	// split up entered code by line
  QStringList cmdList = command.split('\n',QString::SkipEmptyParts);
  QStringList resultList;

  QString result;

  mHistoryBox->moveCursor( QTextCursor::End );

  if( !command.isEmpty() ) {
    emit makingChange(); // for undo push

    if( Py_IsInitialized() ) {
			// Have to regrab these things every time
			// to get updated dictionary from last exec

			// Grab dlfl dictionary
			dlfl_module = PyImport_AddModule("dlfl");
			dlfl_dict = PyModule_GetDict( dlfl_module );
			// Grab __main__ dictionary
			main_module = PyImport_AddModule("__main__");
			main_dict = PyModule_GetDict( main_module );

			// Into main_dict: from dlfl import *
			PyDict_Update(main_dict, dlfl_dict);

			// Run this to redirect python stdout
      PyRun_SimpleString( "import sys, __main__" );
      PyRun_SimpleString( "__main__.mio = MyIO()" );
      PyRun_SimpleString( "sys.stdout = __main__.mio" );
			// Loop through each line of code looking for block statements
      for(int i = 0; i < cmdList.size(); i++ ) {
				QString si = cmdList.at(i);
				if( si.endsWith(":") ) { 
					// then it is a block statement
					int j = i+1;
					while( j < cmdList.size() && (cmdList.at(j).startsWith("\t") || cmdList.at(j).startsWith(" ")) ) {
						/*// If it begins with a space figure out if we're in the same indentation level
						if( cmdList.at(j).startsWith(" ") ) {
							QRegExp rx = QRegExp("(^[ ])([ ])*");
							int pos = rx.indexIn( cmdList.at(j) );
							QStringList list = rx.capturedTexts();
							
							}*/

						// keep going until the tab indent level goes back out
						QString sj = cmdList.at(j);
						si += QString("\n")+sj;
						cmdList.removeAt(j);
					}
					// update with new multiline command
					cmdList.replace(i,si);
				}
				// Check if it is a load command (let TopMod handle this instead of Python)
				if( cmdList.at(i).contains(QRegExp("\\bload\\("))) {
					QStringList list = cmdList.at(i).split("\"", QString::SkipEmptyParts);
					cmdList.removeAt(i);
					emit requestObject(list.at(1)); // 3 parts: load(, filename.obj, and )
					emit cmdExecuted(); // print out in history window
					continue;
				}

				// Clear out stdout
				PyRun_SimpleString( "__main__.mio.clear()" );

				// ** Run The Command
				const char *cmd = cmdList.at(i).toLocal8Bit().constData();
				PyObject *rstring = PyRun_String( cmd, Py_file_input, main_dict, main_dict );
				// Grab the result from redirected stdout class into a global variable
				PyRun_SimpleString( "__main__.__result = __main__.mio.s" );

				if( rstring != NULL ) {
					// resultObject1 has any possible result from running the command
					PyObject *resultObject1 = PyObject_Str( rstring );
					// resultObject2 has any possible thing printed to stdout from running the command
					PyObject *resultObject2 = PyDict_GetItemString( main_dict, "__result" );
					if( resultObject1 != NULL && resultObject2 != NULL ) {
						char *string1 = PyString_AsString( resultObject1 );
						char *string2 = PyString_AsString( PyObject_Str(resultObject2) );
						result = QString( string1 );
						// Go for string1 first, if nothing then try string2
						if( result.isEmpty() || result == "" || result == "None" )
							result = QString( string2 );
						Py_DECREF( resultObject1 );
						Py_DECREF( resultObject2 );
					}
					Py_DECREF( rstring );
					emit cmdExecuted();
				} else {
					// There was an error with the command
					// Print the error message
					PyObject *object, *data, *traceback;
					PyErr_Fetch( &object, &data, &traceback );
					PyObject * traceStr = PyObject_Str( data );
					char *string = PyString_AsString( traceStr );
					result = QString( string ) + QString("  "); // the spaces b/c I chop(2) later
				}
				resultList << result; // append result to resultList
      } // for loop
    } // end Py_IsInitialized
    emit addToHistory(command);
  } else { // enter when command.isEmpty() == true
    command = QString("");
    result = QString("");
  }

	// Now create the output for the history window
  for(int i = 0; i < cmdList.size(); i++ ) {
		// print each command
    mHistoryBox->insertPlainText( "\n" + cmdList.at(i) );
    if( i < resultList.size() ) {
			// followed by all the items in it's result list
      QString res = resultList.at(i);
      if( !res.isEmpty() && res != QString("None") ) { // don't print if None
				res.replace(QRegExp("\\n"), "\n# "); // print as a comment at each line for copy/paste ease
				res.chop(2);
				mHistoryBox->insertPlainText( "\n# "+ res ); // # is a comment in python
      }
    }
  }
	mHistoryBox->insertPlainText("\n");
	
	// If the user highlighted to execute only a portion of the command
	// then don't erase, otherwise do:
  if( !mCommandEdit->textCursor().hasSelection() ) {
		clearInput( );
  }

	// Make sure history window is scrolled all the way down to latest command
  QScrollBar *vBar = mHistoryBox->verticalScrollBar();
  vBar->triggerAction(QAbstractSlider::SliderToMaximum);
}

void DLFLScriptEditor::echoCommand( QString cmd ) {
	if( mEchoing ) {
		mHistoryBox->insertPlainText( "\n" + cmd + "\n" );
    emit addToHistory(cmd);
		QScrollBar *vBar = mHistoryBox->verticalScrollBar();
		vBar->triggerAction(QAbstractSlider::SliderToMaximum);
	}
}

void DLFLScriptEditor::PyInit( ) {
  Py_Initialize( );

  if( Py_IsInitialized() ) {
    PyRun_SimpleString( "import sys, __main__" );
		loadDLFLModule( addToPath );

		// Create a class to redirect sys.stdout to
    PyRun_SimpleString( "class MyIO:\n\tdef __init__(self): self.s=\"\"\n\tdef write(self,x): self.s+=x\n\tdef clear(self): self.s=\"\"" );

    if( dlfl_module != NULL )
      dlfl_dict = PyModule_GetDict( dlfl_module );

    if( dlfl_module != NULL && dlfl_dict != NULL ) {
      // Setup the Python DLFL Module C API
      PyObject *c_api_object = PyDict_GetItemString( dlfl_dict, "_C_API" );
      if (PyCObject_Check(c_api_object)) {
				PyDLFL_API = (void **)PyCObject_AsVoidPtr(c_api_object);
      }
      // Tell the module we're working inside the GUI
      PyDLFL_UsingGUI(true);
    }

    // Print out the standard version info and such
    const char *version = Py_GetVersion( );
    QString qversion(version);
    mHistoryBox->append("Python " + qversion + "\n" );
    if( dlfl_module != NULL )
      mHistoryBox->append("from dlfl import *\n");
    else {
      mHistoryBox->append("# no dlfl python module found");
      mHistoryBox->insertPlainText("\n");
      PyErr_Print();
    }
  }
}

// A slot to pass the GUI object pointer to the DLFLModule
void DLFLScriptEditor::loadObject( DLFLObject *obj, QString fileName ) {
  if( dlfl_module == NULL && dlfl_dict == NULL )
    return;
	if( dlfl_module )
		PyDLFL_PassObject( obj ); // pass the object pointer to currObj in DLFLModule.cc
  // Print out the equivalent command to loading an object
  QString command = tr("load(\"")+fileName+tr("\")");
  mHistoryBox->insertPlainText( "\n" + command + "\n" );
}

static bool syspath_append( const char *dirname ) {
	// Append a directory to the PYTHONHOME path for where to look for modules
	PyObject *mod_sys, *dict, *path, *dir;
	PyErr_Clear(  );
	dir = Py_BuildValue( "s", dirname );
	mod_sys = PyImport_ImportModule( "sys" );	/* new ref */
	dict = PyModule_GetDict( mod_sys );	/* borrowed ref */
	path = PyDict_GetItemString( dict, "path" );	/* borrowed ref */
	if( !PyList_Check( path ) )
		return false;
	PyList_Append( path, dir );
	if( PyErr_Occurred(  ) ) {
		Py_FatalError( "could not build sys.path" );
		return false;
	}
	Py_DECREF( mod_sys );
	return true;
}

void DLFLScriptEditor::loadDLFLModule( QString newPath ) {
	if( !syspath_append( newPath.toLocal8Bit().constData() ) )
		return;
	if( dlfl_module == NULL && Py_IsInitialized() ) {
		dlfl_module = PyImport_ImportModule("dlfl");
	}
}

void DLFLScriptEditor::execFile( ) {
	// Grab dlfl dictionary
	dlfl_module = PyImport_AddModule("dlfl");
	dlfl_dict = PyModule_GetDict( dlfl_module );
	// Grab __main__ dictionary
	main_module = PyImport_AddModule("__main__");
	main_dict = PyModule_GetDict( main_module );

	QString filename = 
		QFileDialog::getOpenFileName(this,
																 tr("Exec File"),
																 "$HOME",
																 tr("Python Files (*.py);;All Files (*)"),
																0, QFileDialog::DontUseSheet);
	QString error("");

	if( !filename.isEmpty() ) {
		QString execfileString = QString("execfile(\"") + filename + QString("\")");
		if( Py_IsInitialized() && main_dict != NULL ) {
			const char *cmd = execfileString.toLocal8Bit().constData();
			PyRun_SimpleString( "import sys" );
			PyObject *rstring = PyRun_String( cmd, Py_file_input, main_dict, main_dict );
			if( rstring == NULL ) {
				// There was an error with the command
				// Print the error message
				PyObject *object, *data, *traceback;
				PyErr_Fetch( &object, &data, &traceback );
				PyObject * traceStr = PyObject_Str( data );
				char *string = PyString_AsString( traceStr );
				error = QString("# ") + QString(string);
			}
			Py_DECREF( rstring );
			emit addToHistory(execfileString);
			echoCommand( execfileString );
			if( !error.isEmpty() ) { echoCommand( error ); }
		}
	}
}

void DLFLScriptEditor::openFile( ) {
	QString filename = 
		QFileDialog::getOpenFileName(this,
																 tr("Open Python File"),
																 "$HOME",
																 tr("Python Files (*.py);;All Files (*)"),
																0, QFileDialog::DontUseSheet);
	if(!filename.isEmpty()) {
		QFile file(filename);
		file.open(QIODevice::ReadOnly);
		QByteArray ba = file.readAll();
		QString content;
		content.append(ba);
		mCommandEdit->setPlainText(content);
		file.close();
	}
}

void DLFLScriptEditor::saveFile( ) {
	QString filename = 
		QFileDialog::getSaveFileName(this,
																 tr("Save Python File"),
																 "myscript",
																 tr("Python Files (*.py);;All Files (*)"),
																	0, QFileDialog::DontUseSheet);
	if(!filename.isEmpty()) {
		QFile file(filename);
		file.open(QIODevice::WriteOnly);
		file.write(mCommandEdit->toPlainText().toLocal8Bit());
		file.close();
	}
}

void DLFLScriptEditor::saveOutput( ) {
	QString filename = 
		QFileDialog::getSaveFileName(this,
																 tr("Save Python File"),
																 "myscript",
																 tr("Python Files (*.py);;All Files (*)"));
	if(!filename.isEmpty()) {
		QFile file(filename);
		file.open(QIODevice::WriteOnly);
		file.write(mHistoryBox->toPlainText().toLocal8Bit());
		file.close();
	}
}

void DLFLScriptEditor::clearHistory( ) {
	mHistoryBox->clear();
	mCommandEdit->clearHistory();
	mCommandEdit->goToHistoryStart( );
}

void DLFLScriptEditor::clearInput( ) {
	mCommandEdit->clear();
	mCommandEdit->goToHistoryStart( );
}

void DLFLScriptEditor::toggleEchoing( ) {
	mEchoing = !mEchoing;
}

void DLFLScriptEditor::retranslateUi(){
	
}

void DLFLScriptEditor::spacesToTABs( ) {
	// Grab the input
  QString command = mCommandEdit->toPlainText();
	// regex for set of spaces
	QString rxs = QString("\n[ ]");//{") + QString().setNum(mTabWidth) + QString("})+");
	QRegExp rx = QRegExp(rxs);

	int pos = 0;
	while( (pos = rx.indexIn(command, pos)) != -1 ) {
		int spaces = 1;
		QString tabs = QString("");
		while( command.at(pos+1+spaces) == QChar(' ') ) {
			spaces++;
			if( spaces % 3 == 0 )
				tabs += QString("\t");
		}
		command.replace( pos+1, spaces, tabs );
		pos += rx.matchedLength();
	}
	mCommandEdit->setPlainText( command );
}

void DLFLScriptEditor::toggleTabWidthWidget( ) {
	if( mSetTabWidthWidget->isVisible() ) {
		mSetTabWidthWidget->hide();
		mSetTabWidth->setChecked(false);
	} else {
		mSetTabWidthWidget->show();
		mSetTabWidth->setChecked(true);
	}
}

void DLFLScriptEditor::setTabWidth( int width ) {
	mTabWidth = width; // width in spaces
	mCommandEdit->setTabStopWidth( fontMetrics().width( " " ) * mTabWidth ); // req. width in pixels¯
  mHistoryBox->setTabStopWidth(fontMetrics().width( " " ) * mTabWidth );
	QString hist = mHistoryBox->toPlainText();
	mHistoryBox->setPlainText(hist);
	QString text = mCommandEdit->toPlainText();
	mCommandEdit->setPlainText(text);
}

#endif // WITH_PYTHON
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
*
* Short description of this file
*
* name of .hh file containing function prototypes
*
*/

