#ifndef _DLFLSCRIPT_EDITOR_HH_
#define _DLFLSCRIPT_EDITOR_HH_

#ifdef WITH_PYTHON

#undef slots
#include <Python.h>
#define slots

#include <QWidget>
#include <QMenuBar>
#include <QToolBar>
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
	void execFile( );
	void openFile( );
	void saveFile( );
	void clearHistory( );
	void clearInput( );
private :
	// The place where the user types the commands (multiline)
  Editor *mCommandEdit;
	// The place where executed commands are executed
  QTextEdit *mHistoryBox;
	// The menubar with some options
	QMenuBar *mMenuBar;
	QMenu *scriptMenu;
	QAction *mExecFile, *mOpenFile, *mSaveFile;
	QAction *mClearHistory;
	QAction *mClearInput;
	

  PythonHighlighter *pyhigh;

  QColor mOutputBgColor;
  QColor mInputBgColor;

  void PyInit();
  PyObject *dlfl_module, *dlfl_dict;
  PyObject *main_module, *main_dict;

	bool mEchoing;

	QString pathPython;

public :
	QString addToPath;
};

#endif

#endif // _DLFLSCRIPT_EDITOR_HH_
