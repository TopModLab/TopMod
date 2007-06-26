#ifdef WITH_PYTHON

#include "DLFLScriptEditor.hh"

#include <QtGui>

DLFLScriptEditor::DLFLScriptEditor( DLFLObjectPtr obj, QWidget *parent, Qt::WindowFlags f ) 
	: QWidget(parent,f), dlfl_module(NULL),dlfl_dict(NULL),mEchoing(true) {

  mTextEdit = new QTextEdit;
  mLineEdit = new Editor;
  QObject::connect(mLineEdit, SIGNAL(ctrlReturnPressed()), this, SLOT(executeCommand()));
  QObject::connect( this, SIGNAL(addToHistory(const QString&)), mLineEdit, SLOT(appendHistory(const QString&)) );

  QPalette whiteOnBlack, whiteOnGrey;
  mInputBgColor = QColor(0,0,5,255);
  whiteOnBlack.setColor(QPalette::Active, QPalette::Text, Qt::white );
  whiteOnBlack.setColor(QPalette::Active, QPalette::Base, mInputBgColor );

  mOutputBgColor = QColor(20,20,25,255);
  whiteOnGrey.setColor(QPalette::Active, QPalette::Text, Qt::white );
  whiteOnGrey.setColor(QPalette::Active, QPalette::Base, mOutputBgColor );

  mTextEdit->setPalette( whiteOnGrey );
  mLineEdit->setPalette( whiteOnBlack );

  mTextEdit->setReadOnly(true);

  mTextEdit->setTabStopWidth(25);
  mLineEdit->setTabStopWidth(25);

  pyhigh = new PythonHighlighter(mTextEdit);

  QVBoxLayout *mainLayout = new QVBoxLayout;
  mainLayout->addWidget(mTextEdit);
  mainLayout->addWidget(mLineEdit);
  setLayout(mainLayout);

  PyInit( );
	PyDLFL_PassObject( obj );
}

DLFLScriptEditor::~DLFLScriptEditor( ) {
  if( Py_IsInitialized() )
    Py_Finalize( );
}

void DLFLScriptEditor::executeCommand( ) {
  QString command = mLineEdit->toPlainText();

  if( mLineEdit->textCursor().hasSelection() ) {
    command = mLineEdit->textCursor().selection().toPlainText();
  }
  
  QStringList cmdList = command.split('\n',QString::SkipEmptyParts);
  QStringList resultList;

  QString result;

  mTextEdit->moveCursor( QTextCursor::End );

  if( !command.isEmpty() ) {
    emit makingChange(); // for undo push

    if( Py_IsInitialized() ) {
      PyObject *main = PyImport_AddModule("__main__");
      PyObject *dlfl = PyImport_AddModule("dlfl");
      PyObject* main_dict = PyModule_GetDict( main );
      PyObject* dlfl_dict = PyModule_GetDict( dlfl );

      PyRun_SimpleString( "import sys, __main__" );
      PyRun_SimpleString( "__main__.mio = MyIO()" );
      PyRun_SimpleString( "sys.stdout = __main__.mio" );

      for(int i = 0; i < cmdList.size(); i++ ) {
				QString si = cmdList.at(i);
				if( si.endsWith(":") ) {
					int j = i+1;
					while( j < cmdList.size() && cmdList.at(j).startsWith("\t") ) {
						QString sj = cmdList.at(j);
						si += QString("\n")+sj;
						cmdList.removeAt(j);
						//j++;
					}
					cmdList.replace(i,si);
				}
				if( cmdList.at(i).contains(QRegExp("\\bload\\("))) {
					QStringList list = cmdList.at(i).split("\"", QString::SkipEmptyParts);
					cmdList.removeAt(i);
					emit requestObject(list.at(1)); // 3 parts: load(, filename.obj, and )
					emit cmdExecuted();
					continue;
				}

				PyRun_SimpleString( "__main__.mio.clear()" );

				const char *cmd = cmdList.at(i).toLocal8Bit().constData();
				PyObject *rstring = PyRun_String( cmd, Py_file_input, main_dict, dlfl_dict );
				PyRun_SimpleString( "__main__.__result = __main__.mio.s");
				//PyRun_SimpleString( "if(len(__main__.mio.s) > 0 ): __main__.__result = __main__.mio.s[0]");

				if( rstring != NULL ) {
					PyObject *resultObject1 = PyObject_Str( rstring );
					PyObject *resultObject2 = PyDict_GetItemString( main_dict, "__result" );
					if( resultObject1 != NULL && resultObject2 != NULL ) {
						char *string1 = PyString_AsString( resultObject1 );
						char *string2 = PyString_AsString( PyObject_Str(resultObject2) );
						result = QString( string1 );
						if( result.isEmpty() || result == "" || result == "None" )
							result = QString( string2 );
						Py_DECREF( resultObject1 );
						Py_DECREF( resultObject2 );
					}
					Py_DECREF( rstring );
					emit cmdExecuted();
				} else {
					//PyErr_Print();
					PyObject *object, *data, *traceback;
					PyErr_Fetch( &object, &data, &traceback );
					PyObject * traceStr = PyObject_Str( data );
					char *string = PyString_AsString( traceStr );
					result = QString( string );
				}
				resultList << result; // append result to resultList
      } // for loop
    } // end Py_IsInitialized
    emit addToHistory(command);
  } else { // enter when command.isEmpty() == true
    command = QString("");
    result = QString("");
  }
    
  for(int i = 0; i < cmdList.size(); i++ ) {
    mTextEdit->insertPlainText( "\n" + cmdList.at(i) );
    if( i < resultList.size() ) {
      QString res = resultList.at(i);
      if( !res.isEmpty() && res != QString("None") ) {
				res.replace(QRegExp("\\n"), "\n# ");
				res.chop(2);
				mTextEdit->insertPlainText( "\n# "+ res ); // # is a comment in python
      } else { mTextEdit->insertPlainText("\n"); }
    } else { mTextEdit->insertPlainText("\n"); }
  }

  if( !mLineEdit->textCursor().hasSelection() ) {
    mLineEdit->clear();
    mLineEdit->goToHistoryStart( );
  }

  QScrollBar *vBar = mTextEdit->verticalScrollBar();
  vBar->triggerAction(QAbstractSlider::SliderToMaximum);
}

void DLFLScriptEditor::echoCommand( QString cmd ) {
	if( mEchoing ) {
		mTextEdit->insertPlainText( "\n" + cmd + "\n" );
    emit addToHistory(cmd);
		QScrollBar *vBar = mTextEdit->verticalScrollBar();
		vBar->triggerAction(QAbstractSlider::SliderToMaximum);
	}
}

void DLFLScriptEditor::PyInit( ) {
  Py_Initialize( );

  if( Py_IsInitialized() ) {
    // Import the DLFL Module
    dlfl_module = PyImport_ImportModule("dlfl");
    PyRun_SimpleString( "import sys, __main__" );
    //PyRun_SimpleString( "def my_displayhook(o): __main__.__result=o" );
    //PyRun_SimpleString( "sys.displayhook=my_displayhook" );

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
    mTextEdit->append("Python " + qversion + "\n" );
    if( dlfl_module != NULL )
      mTextEdit->append("from dlfl import *\n");
    else {
      mTextEdit->append("no dlfl python module found");
      mTextEdit->insertPlainText("\n");
      PyErr_Print();
    }
  }
}

// A slot to pass the GUI object pointer to the DLFLModule
void DLFLScriptEditor::loadObject( DLFLObject *obj, QString fileName ) {
  if( dlfl_module == NULL && dlfl_dict == NULL )
    return;
  PyDLFL_PassObject( obj );
  // Print out the equivalent command to loading an object
  QString command = tr("load(\"")+fileName+tr("\")");
  mTextEdit->insertPlainText( "\n" + command + "\n" );
}

#endif // WITH_PYTHON
