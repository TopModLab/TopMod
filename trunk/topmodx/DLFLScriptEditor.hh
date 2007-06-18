#ifndef _DLFLSCRIPT_EDITOR_HH_
#define _DLFLSCRIPT_EDITOR_HH_

#ifdef WITH_PYTHON

#undef slots
#include <Python.h>
#define slots

#include <QWidget>
#include <QPushButton>

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
  DLFLScriptEditor( QWidget *parent = 0, Qt::WindowFlags f = Qt::Tool );
  ~DLFLScriptEditor( );

  QColor& outputBgColor( ) { return mOutputBgColor; };
  QColor& inputBgColor( ) { return mInputBgColor; };

signals :
  void cmdExecuted( );
  void addToHistory( const QString& item );
  void requestObject( QString fileName );
private slots :
  void executeCommand( );
public slots :
  void loadObject( DLFLObject* obj, QString fileName );
private :
  Editor *mLineEdit;
  QTextEdit *mTextEdit;

  QPushButton *mHideButton;

  PythonHighlighter *pyhigh;

  QColor mOutputBgColor;
  QColor mInputBgColor;

  void PyInit();

};

#endif

#endif // _DLFLSCRIPT_EDITOR_HH_
