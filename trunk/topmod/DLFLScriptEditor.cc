#ifdef WITH_PYTHON

#include "DLFLScriptEditor.hh"

#include <QtGui>

DLFLScriptEditor::DLFLScriptEditor( QWidget *parent, Qt::WindowFlags f ) : QWidget(parent,f) {

  mTextEdit = new QTextEdit;    
  mLineEdit = new QLineEdit;
  QObject::connect(mLineEdit, SIGNAL(returnPressed()), this, SLOT(executeCommand()));    

  QPalette whiteOnBlack;
  whiteOnBlack.setColor(QPalette::Active, QPalette::Text, Qt::white );
  whiteOnBlack.setColor(QPalette::Active, QPalette::Base, Qt::black );
  mTextEdit->setPalette( whiteOnBlack );
  mLineEdit->setPalette( whiteOnBlack );

  QVBoxLayout *mainLayout = new QVBoxLayout;
  mainLayout->addWidget(mTextEdit);
  mainLayout->addWidget(mLineEdit);
  setLayout(mainLayout);

  PyInit( );
}

DLFLScriptEditor::~DLFLScriptEditor( ) {
  if( Py_IsInitialized() )
    Py_Finalize( );
}


void DLFLScriptEditor::executeCommand( ) {
  QString command = mLineEdit->text();
  QString result;

  mTextEdit->moveCursor( QTextCursor::End );

  if( !command.isEmpty() ) {
    if( Py_IsInitialized() ) {
      PyObject *main = PyImport_AddModule("__main__");
      PyObject *dlfl = PyImport_AddModule("dlfl");
      PyObject* main_dict = PyModule_GetDict( main );
      PyObject* dlfl_dict = PyModule_GetDict( dlfl );

      const char *cmd = command.toLocal8Bit().data();
      PyObject *rstring = PyRun_String( cmd, Py_eval_input, main_dict, dlfl_dict );
      if( rstring != NULL ) {
	PyObject *resultObject = PyObject_Str( rstring );
	if( resultObject != NULL ) {
	  char *string = PyString_AsString( resultObject );
	  result = QString( string );
	  Py_DECREF( resultObject );
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
    }
  } else {
    command = QString("");
    result = QString("");
  }

  const QByteArray cmd(command.toLatin1());
  mTextEdit->insertPlainText( "\ndlfl> " + command );
  if( !result.isEmpty() )
  mTextEdit->insertPlainText( "\n"+result );
  mLineEdit->clear();

  QScrollBar *vBar = mTextEdit->verticalScrollBar();
  vBar->triggerAction(QAbstractSlider::SliderToMaximum);
}

void DLFLScriptEditor::PyInit( ) {
  Py_Initialize( );

  if( Py_IsInitialized() ) {
    // Import the DLFL Module
    PyObject *dlfl_module = PyImport_ImportModule("dlfl");
    PyObject* dlfl_dict = PyModule_GetDict( dlfl_module );

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
      mTextEdit->append("dlfl> from dlfl import *\n");
    else {
      mTextEdit->append("no dlfl python module found");
      mTextEdit->insertPlainText("\n");
      PyErr_Print();
    }
  }
}

// A slot to pass the GUI object pointer to the DLFLModule
void DLFLScriptEditor::loadObject( DLFLObject *obj, QString fileName ) {
  PyDLFL_PassObject( obj );
  // Print out the equivalent command to loading an object
  QString command = tr("load(\"")+fileName+tr("\")");
  mTextEdit->insertPlainText( "\ndlfl> " + command );
}

#endif
