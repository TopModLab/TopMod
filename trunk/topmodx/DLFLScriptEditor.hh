#ifndef _DLFLSCRIPT_EDITOR_HH_
#define _DLFLSCRIPT_EDITOR_HH_

#ifdef WITH_PYTHON

#undef slots
#include <Python.h>
#define slots

#include <QWidget>
#include <QPushButton>
#include <iostream>

//class QLineEdit;
class QTextEdit;
class QProcess;

#include <DLFLObject.hh>
#include "editor.hh"

#include "PythonHighlighter.hh"

using namespace DLFL;

// Stuff for the DLFL Extension Module's C API
static void **PyDLFL_API;
#define PyDLFL_UsingGUI				\
  (*(void (*)(bool ugui)) PyDLFL_API[0])
#define PyDLFL_PassObject			\
  (*(void (*)(DLFLObject* obj)) PyDLFL_API[1])

class DLFLScriptEditor : public QWidget {
Q_OBJECT

public:
  DLFLScriptEditor( DLFLObjectPtr obj = NULL, QWidget *parent = 0, Qt::WindowFlags f = Qt::Tool );
  ~DLFLScriptEditor( );

  QColor& outputBgColor( ) { return mOutputBgColor; };
  QColor& inputBgColor( ) { return mInputBgColor; };

signals :
  void makingChange( ); // for undo push
  void cmdExecuted( );
  void addToHistory( const QString& item );
  void requestObject( QString fileName );
private slots :
  void executeCommand( );
	void echoCommand( QString cmd );
public slots :
  void loadObject( DLFLObject* obj, QString fileName );
	void loadDLFLModule( QString newPath );
private :
  Editor *mLineEdit;
  QTextEdit *mTextEdit;

  QPushButton *mHideButton;

  PythonHighlighter *pyhigh;

  QColor mOutputBgColor;
  QColor mInputBgColor;

  void PyInit();
  PyObject *dlfl_module, *dlfl_dict;

	bool mEchoing;

	QString pathPython;

public :
	QString addToPath;
};

#endif

#endif // _DLFLSCRIPT_EDITOR_HH_
