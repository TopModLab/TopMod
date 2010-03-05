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
* Contributor(s): none yet.
*
* ***** END GPL LICENSE BLOCK *****
*/

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
  void mousePressEvent( QMouseEvent* );
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
