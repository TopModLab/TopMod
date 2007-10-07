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
*
* Short description of this file
*
* name of .hh file containing function prototypes
*
*/

//#include "function.h"
//#include "valuemanager.h"
#include "editor.hh"
//#include "evaluator.h"
//#include "result.h"
//#include "lexer.h"

#include "PythonHighlighter.hh"

//#include <netwm.h>
//#include <fixx11h.h>  // netwm.h includes X11 headers which conflict with qevent

#include <QApplication>
#include <QScrollBar>
#include <QLabel>
//#include <QLineEdit>
#include <QListWidget>
#include <QPainter>
#include <QRegExp>
#include <QStringList>
#include <QStyle>
#include <QTimer>
#include <QToolTip>
#include <QHBoxLayout>
#include <QWheelEvent>
#include <QKeyEvent>
#include <QFrame>
#include <QMouseEvent>
#include <QStyleOption>
//#include <QX11Info>

/*class EditorHighlighter : public PythonHighlighter {
public:
  EditorHighlighter( Editor* );

private:
  Editor* editor;
  };*/

class Editor::Private {
public:
  Private() { }

  ~Private() { }

  QString unExecuted;
  QStringList history;
  int index;
  bool autoCompleteEnabled;
  EditorCompletion* completion;
  QTimer* completionTimer;
  bool syntaxHighlightEnabled;
  PythonHighlighter* highlighter;
  QMap<ColorType,QColor> highlightColors;
  QTimer* matchingTimer;
};

class EditorCompletion::Private {
public:
  Editor* editor;
  QFrame *completionPopup;
  QListWidget *completionListBox;
};

class ChoiceItem: public QListWidgetItem {
  public:
  ChoiceItem( const QString&, QListWidget* );
    void setMinNameWidth (int w) { minNameWidth = w; }
    int nameWidth() const;

  protected:
    void paint( QPainter* p );

  private:
    QString item;
    QString desc;
    int minNameWidth;
};

ChoiceItem::ChoiceItem( const QString& text, QListWidget* listBox ):
  QListWidgetItem( text, listBox ), minNameWidth(0)
{
  QStringList list = text.split( ':' );

  if( !list.isEmpty() )
    item = list[0];

  if( list.count() > 1 )
    desc = list[1];
}

// Returns width of this particular list item's name.
int ChoiceItem::nameWidth() const
{
  if(item.isEmpty())
    return 0;

  QFontMetrics fm = listWidget()->fontMetrics();
  return fm.width( item );
}

void ChoiceItem::paint( QPainter* painter )
{
  int itemHeight = listWidget()->height();
  QFontMetrics fm = painter->fontMetrics();
  int yPos = ( ( itemHeight - fm.height() ) / 2 ) + fm.ascent();
  painter->drawText( 3, yPos, item );

  //int xPos = fm.width( item );
  int xPos = qMax(fm.width(item), minNameWidth);

  if( !isSelected() ) {
    QPalette p = listWidget()->palette();
    p.setCurrentColorGroup( QPalette::Disabled );

    painter->setPen( p.color( QPalette::Text ).dark() );
  }

  painter->drawText( 10 + xPos, yPos, desc );
}

/*EditorHighlighter::EditorHighlighter( Editor* e ):
  QSyntaxHighlighter( e )
{
  editor = e;
}

void EditorHighlighter::highlightBlock ( const QString & text )
{
  if( !editor->isSyntaxHighlightEnabled() )
  {
    setFormat( 0, text.length(), editor->palette().color( QPalette::Text ) );
    return;
  }

  QStringList fnames = FunctionManager::instance()->functionList(FunctionManager::All);
  fnames.sort(); // Sort list so we can bin search it.

  TokenList tokens = Lexer::tokenize( text );

  for( int i = 0; i < tokens.count(); i++ )
  {
    Token& token = tokens[i];
    QString text = token.text().toLower();
    QColor color = editor->palette().color( QPalette::Text );

    switch( token.type() )
    {
      case Token::Number:
        color = editor->highlightColor( Editor::Number );
        break;

      case Token::Identifier:
        color = editor->highlightColor( Editor::Variable );

        if( qBinaryFind( fnames.begin(), fnames.end(), text ) != fnames.end() )
          color = editor->highlightColor( Editor::FunctionName );

        break;

      case Token::Operator:
        break;

      default: break;
    }

    if( token.pos() >= 0 )
      setFormat( token.pos(), token.text().length(), color );
  }
  return;
}*/

Editor::Editor( QWidget* parent ):
  QTextEdit( parent )
{
  d = new Private;
  d->index = 0;
  d->autoCompleteEnabled = true;
  d->completion = new EditorCompletion( this );
  d->completionTimer = new QTimer( this );
  d->syntaxHighlightEnabled = true;
  d->highlighter = new PythonHighlighter(this);//EditorHighlighter( this );

  d->completionTimer->setSingleShot( true );

  //setSizePolicy( QSizePolicy::Expanding, QSizePolicy::Fixed );
  setWordWrapMode( QTextOption::NoWrap );
  setHorizontalScrollBarPolicy( Qt::ScrollBarAlwaysOff );
  //setVerticalScrollBarPolicy( Qt::ScrollBarAlwaysOff );
  setAcceptRichText( false );
  setAutoFormatting( AutoNone );
  setTabChangesFocus( false ); //true );

  connect( d->completion, SIGNAL( selectedCompletion( const QString& ) ),
    SLOT( autoComplete( const QString& ) ) );
  connect( this, SIGNAL( textChanged() ), SLOT( checkAutoComplete() ) );
  connect( d->completionTimer, SIGNAL( timeout() ), SLOT( triggerAutoComplete() ) );

  setHighlightColor( Number, QColor(0,0,127) );
  setHighlightColor( FunctionName, QColor(85,0,0) );
  setHighlightColor( Variable, QColor(0,85,0) );
  setHighlightColor( MatchedPar, QColor(255,255,183) );
}

Editor::~Editor()
{
  delete d;
}

QSize Editor::sizeHint() const
{
  ensurePolished();

  QFontMetrics fm = fontMetrics();
  int h = qMax(fm.lineSpacing(), 14) + 4;
  int w = fm.width( 'x' ) * 17 + 4;
  int m = frameWidth() * 2;

  QStyleOptionFrameV2 styleOptions;
  styleOptions.rect = rect();
  styleOptions.palette = palette();
  styleOptions.state = QStyle::State_None;

  return style()->sizeFromContents(QStyle::CT_LineEdit, &styleOptions,
             QSize( w + m, h + m ).
             expandedTo(QApplication::globalStrut()));
}

QStringList Editor::history() const
{
  return d->history;
}

void Editor::setHistory( const QStringList& h )
{
  d->history = h;
  d->index = d->history.count();
}

bool Editor::autoCompleteEnabled() const
{
  return d->autoCompleteEnabled;
}

void Editor::setAutoCompleteEnabled( bool enable )
{
  d->autoCompleteEnabled = enable;
}

void Editor::appendHistory( const QString& text )
{
  if( text.isEmpty() ) return;

  QString lastText;
  if( d->history.count() )
    lastText = d->history[ d->history.count()-1 ];
  if( text == lastText ) return;

  d->history.append( text );
  d->index = d->history.count();
}

void Editor::clearHistory()
{
  d->history.clear();
  d->index = 0;
}

void Editor::moveCursorToEnd()
{
  QTextCursor cursor = textCursor();
  cursor.movePosition( QTextCursor::End );
  setTextCursor( cursor );
}

void Editor::setPlainText(const QString &txt)
{
  QTextEdit::setPlainText(txt);
}

void Editor::checkAutoComplete()
{
  if( !d->autoCompleteEnabled )
    return;

  d->completionTimer->stop();
  d->completionTimer->start( 500 );
}

void Editor::triggerAutoComplete()
{
  if( !d->autoCompleteEnabled ) return;
  /*
  // tokenize the expression (don't worry, this is very fast)
  // faster now that it uses flex. ;)
  QTextCursor cursor = textCursor();
  cursor.movePosition( QTextCursor::Start, QTextCursor::KeepAnchor );

  QString subtext = cursor.selectedText();
  TokenList tokens = Lexer::tokenize( subtext );

  if(tokens.isEmpty() || subtext.endsWith(" "))
    return;

  Token lastToken = tokens[ tokens.count()-1 ];

  // last token must be an identifier
  if( lastToken.type() != Token::Identifier )
    return;

  QString id = lastToken.text();
  if( id.isEmpty() )
    return;

  // find matches in function names
  QStringList fnames = FunctionManager::instance()->functionList(FunctionManager::All);
  QStringList choices;

  foreach( QString str, fnames )
  {
    if( str.startsWith( id, Qt::CaseInsensitive ) )
    {
      ::Function* f = FunctionManager::instance()->function( str );
      if( f && !f->description.isEmpty() )
        str.append( ':' ).append( f->description );

      choices.append( str );
    }
  }

  choices.sort();

  // find matches in variables names
  QStringList vchoices;
  QStringList values = ValueManager::instance()->valueNames();

  for(QStringList::ConstIterator it = values.begin(); it != values.end(); ++it)
    if( (*it).startsWith( id, Qt::CaseInsensitive ) )
    {
      QString choice = ValueManager::description(*it);
      if(choice.isEmpty())
        choice = ValueManager::instance()->value(*it).toString();

      vchoices.append( QString("%1:%2").arg( *it, choice ) );
    }

  vchoices.sort();
  choices += vchoices;

  // no match, don't bother with completion
  if( choices.isEmpty() )
    return;

  // one match, complete it for the user
  if( choices.count()==1 )
  {
    QString str = choices[0].split( ':' )[0];

    // single perfect match, no need to give choices.
    if(str == id.toLower())
      return;

    str = str.remove( 0, id.length() );

    blockSignals( true );

    cursor = textCursor();
    cursor.insertText( str );
    cursor.movePosition( QTextCursor::Right, QTextCursor::KeepAnchor, str.length() );
    setTextCursor( cursor );

    blockSignals( false );
    return;
  }

  // present the user with completion choices
  d->completion->showCompletion( choices );*/
}

void Editor::autoComplete( const QString& item )
{/*
  if( !d->autoCompleteEnabled || item.isEmpty() )
    return;

  QTextCursor cursor = textCursor();
  cursor.movePosition( QTextCursor::Start, QTextCursor::KeepAnchor );

  TokenList tokens = Lexer::tokenize( cursor.selectedText() );

  if( tokens.isEmpty() )
    return;

  Token lastToken = tokens[ tokens.count()-1 ];
  if( lastToken.type() != Token::Identifier )
    return;

  QStringList str = item.split( ':' );

  blockSignals( true );

  cursor.setPosition( lastToken.pos() );
  cursor.movePosition( QTextCursor::Right, QTextCursor::KeepAnchor, lastToken.text().length() );
  cursor.insertText( str[0] );

  setTextCursor( cursor );

  blockSignals( false );*/
}

void Editor::historyBack()
{
  if( d->history.isEmpty() )
    return;

  d->index--;
  if( d->index < 0 )
    d->index = 0;

  if( d->index >= d->history.count()-1 ) {
    if( !(toPlainText().isEmpty()) )
      d->unExecuted = toPlainText();
    else
      d->unExecuted = QString("");
  }

	if( d->index >= 0 && d->index < d->history.count() )
		setPlainText( d->history[ d->index ] );

  QTextCursor cursor = textCursor();
  cursor.movePosition( QTextCursor::End );
  setTextCursor( cursor );

  ensureCursorVisible();
}

void Editor::goToHistoryStart( ) {
  d->index = d->history.count();
  setPlainText( d->unExecuted ); //tr("") );
}

void Editor::historyForward()
{
  if( d->history.isEmpty() )
    return;

	if( d->index < d->history.count() )
		d->index++;

  if( d->index >= d->history.count() ) {
    d->index = d->history.count();
    setPlainText( d->unExecuted ); //tr("") );
  } else {
    setPlainText( d->history[ d->index ] );
  }

  QTextCursor cursor = textCursor();
  cursor.movePosition( QTextCursor::End );
  setTextCursor( cursor );

  ensureCursorVisible();
}

void Editor::mousePressEvent ( QMouseEvent *e ) {
  if( e->button() == Qt::RightButton ) {

  }
  QTextEdit::mousePressEvent( e );
}

void Editor::keyPressEvent( QKeyEvent* e )
{
  if( e->key() == Qt::Key_PageUp )//&& e->modifiers() == Qt::CTRL )
  {
    historyBack();
    //e->accept();
    return;
  }

  if( e->key() == Qt::Key_PageDown )//&& e->modifiers() == Qt::CTRL )
  {
    historyForward();
    //e->accept();
    return;
  }

  if( (e->key() == Qt::Key_Enter && e->modifiers() == Qt::CTRL) || 
      (e->key() == Qt::Key_Return && e->modifiers() == Qt::CTRL) ) {
    d->unExecuted = QString("");
    emit ctrlReturnPressed();
    return;
  }
  /*
  if( e->key() == Qt::Key_Left ||
      e->key() == Qt::Key_Right ||
      e->key() == Qt::Key_Home ||
      e->key() == Qt::Key_End )
  {
    //checkMatching();
  }
  */
  QTextEdit::keyPressEvent( e );
}

void Editor::wheelEvent( QWheelEvent *e )
{
  /*if( e->delta() > 0 )
    historyBack();
  else if( e->delta() < 0 )
    historyForward();

  e->accept();
	*/
	QTextEdit::wheelEvent( e );
}

void Editor::setSyntaxHighlight( bool enable )
{
  d->syntaxHighlightEnabled = enable;
  d->highlighter->rehighlight();
}

bool Editor::isSyntaxHighlightEnabled() const
{
  return d->syntaxHighlightEnabled;
}

void Editor::setHighlightColor( ColorType type, QColor color )
{
  d->highlightColors[ type ] = color;
  d->highlighter->rehighlight();
}

QColor Editor::highlightColor( ColorType type )
{
  return d->highlightColors[ type ];
}


EditorCompletion::EditorCompletion( Editor* editor ): QObject( editor )
{
  d = new Private;
  d->editor = editor;

  d->completionPopup = new QFrame( editor->topLevelWidget() );
  d->completionPopup->setWindowFlags( Qt::Popup );
  d->completionPopup->setFrameStyle( QFrame::Box | QFrame::Plain );
  d->completionPopup->setLineWidth( 1 );
  d->completionPopup->installEventFilter( this );
  d->completionPopup->setSizePolicy( QSizePolicy::Expanding, QSizePolicy::Minimum);

  QLayout *layout = new QVBoxLayout(d->completionPopup);
  layout->setMargin(0);

  d->completionListBox = new QListWidget( d->completionPopup );
  d->completionPopup->setFocusProxy( d->completionListBox );
  d->completionListBox->setFrameStyle( QFrame::NoFrame );
  //d->completionListBox->setVariableWidth( true );
  d->completionListBox->installEventFilter( this );

  layout->addWidget(d->completionListBox);
}

EditorCompletion::~EditorCompletion()
{
  delete d;
}

bool EditorCompletion::eventFilter( QObject *obj, QEvent *ev )
{
  if ( obj == d->completionPopup || obj == d->completionListBox )
  {

    if ( ev->type() == QEvent::KeyPress )
    {
      QKeyEvent *ke = (QKeyEvent*)ev;
      if ( ke->key() == Qt::Key_Enter || ke->key() == Qt::Key_Return  )
      {
        doneCompletion();
        return true;
      }
      else if ( ke->key() == Qt::Key_Left || ke->key() == Qt::Key_Right ||
      ke->key() == Qt::Key_Up || ke->key() == Qt::Key_Down ||
      ke->key() == Qt::Key_Home || ke->key() == Qt::Key_End ||
      ke->key() == Qt::Key_PageUp || ke->key() == Qt::Key_PageDown )
        return false;

      d->completionPopup->close();
      d->editor->setFocus();
      QApplication::sendEvent( d->editor, ev );
      return true;
    }

    if ( ev->type() == QEvent::MouseButtonDblClick )
    {
      doneCompletion();
      return true;
    }

  }

  return false;
}

void EditorCompletion::doneCompletion()
{
  d->completionPopup->close();
  d->editor->setFocus();
  emit selectedCompletion( d->completionListBox->currentItem()->text() );
}

void EditorCompletion::showCompletion( const QStringList &choices )
{
  static bool shown = false;

  if( choices.isEmpty() )
    return;

  d->completionListBox->clear();
  int maxWidth = 0;
  for( int i = 0; i < choices.count(); i++ ) {
    ChoiceItem *item = new ChoiceItem( choices[i], d->completionListBox );
    int itemMaxWidth = item->nameWidth();

    if(itemMaxWidth > maxWidth)
      maxWidth = itemMaxWidth;
  }

  for(unsigned i = 0; i < d->completionListBox->count(); ++i) {
    ChoiceItem *item = static_cast<ChoiceItem *>(d->completionListBox->item(i));
    item->setMinNameWidth(maxWidth);
  }

  d->completionListBox->setCurrentItem( 0 );

  // size of the pop-up
  d->completionPopup->setMaximumHeight( 120 );
  d->completionPopup->resize( d->completionListBox->sizeHint() +
			      QSize( d->completionListBox->verticalScrollBar()->width() + 4,
				     d->completionListBox->horizontalScrollBar()->height() + 4 ) );

  if(!shown)
  {
    d->completionPopup->show();
    QTimer::singleShot ( 0, this, SLOT(moveCompletionPopup()) );
  }
  else
  {
    moveCompletionPopup();
    d->completionPopup->show();
  }
}

void EditorCompletion::moveCompletionPopup()
{/*
  int h = d->completionListBox->height();
  int w = d->completionListBox->width();

  // position, reference is editor's cursor position in global coord
  QFontMetrics fm( d->editor->font() );

//  int pixelsOffset = fm.width( d->editor->text(), curPos );
//  pixelsOffset -= d->editor->contentsX();
  QPoint pos = d->editor->mapToGlobal( QPoint( d->editor->cursorRect().right(), d->editor->height() ) );

  // if popup is partially invisible, move to other position
  NETRootInfo info(QX11Info::display(),
	  NET::CurrentDesktop | NET::WorkArea | NET::NumberOfDesktops,
	  -1, false);
  info.activate(); // wtf is this needed for?
  NETRect NETarea = info.workArea(info.currentDesktop());

  QRect area(NETarea.pos.x, NETarea.pos.y, NETarea.size.width, NETarea.size.height);

  if( pos.y() + h > area.y() + area.height() )
    pos.setY( pos.y() - h - d->editor->height() );
  if( pos.x() + w > area.x() + area.width() )
    pos.setX( area.x() + area.width() - w );

  d->completionPopup->move( pos );
  d->completionListBox->setFocus();*/
}


