#ifndef _EDITOR_H_
#define _EDITOR_H_

#include <QTextEdit>

enum ColorType { Number, FunctionName, Variable, MatchedPar };

class QEvent;
class QKeyEvent;
class QWheelEvent;
class QWidget;
class Evaluator;
class QStringList;

class Editor : public QTextEdit {
 Q_OBJECT 

public :

  Editor( QWidget* parent = 0 );
  ~Editor();

  QSize sizeHint() const;

  QStringList history() const;
  void setHistory( const QStringList& history );
  void goToHistoryStart( );

  bool autoCompleteEnabled() const;
  void setAutoCompleteEnabled( bool enable );

  void setSyntaxHighlight( bool enable );
  bool isSyntaxHighlightEnabled() const;

  void setHighlightColor( ColorType type, QColor color );
  QColor highlightColor( ColorType type );

signals:
  void ctrlReturnPressed();

public slots:
  void appendHistory( const QString& text );
  void clearHistory();

  void moveCursorToEnd();

  void setPlainText(const QString &txt);

protected slots:
  void checkAutoComplete();
  void triggerAutoComplete();
  void autoComplete( const QString& item );
  void historyBack();
  void historyForward();

protected:
  void keyPressEvent( QKeyEvent* );
  void wheelEvent( QWheelEvent* );

private:
  class Private;
  Private* d;
  Editor( const Editor& );
  Editor& operator=( const Editor& );
};


class EditorCompletion : public QObject {
  Q_OBJECT

public:
  EditorCompletion( Editor* editor );
  ~EditorCompletion();

  bool eventFilter( QObject *o, QEvent *e );
  void doneCompletion();
  void showCompletion( const QStringList &choices );

protected slots:
  void moveCompletionPopup();

signals:
  void selectedCompletion( const QString& item );

private:
  class Private;
  Private* d;
  EditorCompletion( const EditorCompletion& );
  EditorCompletion& operator=( const EditorCompletion& );
};

#endif // _EDITOR_H_
