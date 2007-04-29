#include "DLFLScriptEditor.hh"

#include <iostream>
#include <QtGui>

DLFLScriptEditor::DLFLScriptEditor( QWidget *parent, Qt::WindowFlags f ) : QWidget(parent,f)
{
  // mHideButton = new QPushButton( QIcon(":/images/hide_scripteditor.png"), tr(""), this );
  // mHideButton->setToolTip(tr("Hide Script Editor"));
  // connect(mHideButton, SIGNAL(clicked()), this, SLOT(toggleShowHide()));

  mTextEdit = new QTextEdit;    
  mLineEdit = new QLineEdit;
  QObject::connect(mLineEdit, SIGNAL(returnPressed()), this, SLOT(executeCommand()));    

  QPalette whiteOnBlack;
  whiteOnBlack.setColor(QPalette::Active, QPalette::Text, Qt::white );
  whiteOnBlack.setColor(QPalette::Active, QPalette::Base, Qt::black );
  mTextEdit->setPalette( whiteOnBlack );
  mLineEdit->setPalette( whiteOnBlack );

  QVBoxLayout *mainLayout = new QVBoxLayout;
  // mainLayout->addWidget(mHideButton);
  mainLayout->addWidget(mTextEdit);
  mainLayout->addWidget(mLineEdit);
  setLayout(mainLayout);
    
  //mShell = new QProcess();
  //processChannelMode = QProcess::SeparateChannels;
  //mShell->setProcessChannelMode(QProcess::MergedChannels);
  // QObject::connect(mShell, SIGNAL(readyRead()), this, SLOT(readStandardOut()));
  //mShell->start("sh", QStringList() << "-i", QIODevice::ReadWrite);
  
  //setWindowTitle(tr("Script Editor"));
  //QSize size( 700, 200 );
  //size = size.expandedTo(minimumSizeHint());
  //setMaximumSize ( size );
  //resize(size);

  PyInit( );
}

DLFLScriptEditor::~DLFLScriptEditor( ) {
  if( Py_IsInitialized() )
    Py_Finalize( );
}

// void DLFLScriptEditor::toggleShowHide( )
// {
//   if( isVisible( ) )
//     hide( );
//   else {
//     show( );
//     mLineEdit->setFocus();
//   }
// }

void DLFLScriptEditor::executeCommand( )
{
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
  //mShell->write(cmd);
  mTextEdit->insertPlainText( "\ndlfl> " + command );
  if( !result.isEmpty() )
  mTextEdit->insertPlainText( "\n"+result );
  mLineEdit->clear();

  QScrollBar *vBar = mTextEdit->verticalScrollBar();
  vBar->triggerAction(QAbstractSlider::SliderToMaximum);
}

/*void DLFLScriptEditor::readStandardOut( )
{
  mTextEdit->append(mShell->readAll());    
}
*/

void DLFLScriptEditor::PyInit( ) 
{
  //#if defined(__APPLE__) && PY_MAJOR_VERSION==2 && PY_MINOR_VERSION<=1
  //PyMac_Initialize( );
  //#else
  Py_Initialize( );
  //#endif

  if( Py_IsInitialized() ) {
    PyObject *dlfl_module = PyImport_ImportModule("dlfl");
    const char *version = Py_GetVersion( );
    QString qversion(version);
    mTextEdit->append("Python " + qversion + "\n" );
    if( dlfl_module != NULL )
      mTextEdit->append("dlfl> import dlfl\n");
    else {
      mTextEdit->append("no dlfl python module found");
      mTextEdit->insertPlainText("\n");
      PyErr_Print();
    }
  }
}

void DLFLScriptEditor::keyPressEvent( QKeyEvent * e ) {
  int x = 0;
  switch( e->key() ) {
  case Qt::Key_Up :
    x = 1;
    break;
  case Qt::Key_Down :
    x = 2;
    break;
  }

  int y = x;
}
