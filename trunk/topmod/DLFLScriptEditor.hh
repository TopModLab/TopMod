#ifndef _DLFLSCRIPT_EDITOR_CC_
#define _DLFLSCRIPT_EDITOR_CC_

#undef slots
#include <Python.h>
#define slots

#include <QWidget>
#include <QPushButton>

class QLineEdit;
class QTextEdit;
class QProcess;

class DLFLScriptEditor : public QWidget {
Q_OBJECT

public:
  DLFLScriptEditor( QWidget *parent = 0, Qt::WindowFlags f = Qt::Tool );
  ~DLFLScriptEditor( );
signals :
  //void findNext( const QString &str, Qt::CaseSensitivity cs );
  //void findPrevious( const QString &str, Qt::CaseSensitivity cs );
// public slots :
  // void toggleShowHide( );
private slots :
  void executeCommand( );
  //void readStandardOut( );
  void keyPressEvent ( QKeyEvent * e );
private :
  QLineEdit *mLineEdit;
  QTextEdit *mTextEdit;
  //QProcess  *mShell;

  QPushButton *mHideButton;

  void PyInit();

};

#endif // _DLFLSCRIPT_EDITOR_CC_
