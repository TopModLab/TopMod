#ifndef _COMMANDCOMPLETER_HH_
#define _COMMANDCOMPLETER_HH_

#include <QDialog>
#include <QLineEdit>
#include <QCompleter>
#include <QVBoxLayout>
#include <QStringList>
#include <QAbstractItemModel>
#include <QFile>
#include <QStringListModel>
#include <QApplication>
#include <QLabel>
#include <QStyle>
#include <QPalette>
#include <QShortcut>
#include <QStandardItemModel>
#include <QAction>

/*!
* \file CommandCompleter.hh
* \class CommandCompleter
* 
* \brief Quicksilver-like popup commandline interface
* 
* CommandCompleter provides a popup Quicksilver-like command line interface for accessing
* all the functionality of TopMod with keyboard input instead of mouse clicks.
* 
*/

class CommandCompleter : public QDialog {
	Q_OBJECT

public:

	/*!
	* \brief Constructor
	* 
	* @param m the actionList passed from the MainWindow class after each Mode class is instantiated
	* @param parent the MainWindow parent widget 
	* @param f window flags for the QDialog constructor
	* 
	*/
	CommandCompleter ( QWidget *m, QWidget * parent = 0, Qt::WindowFlags f = 0 ) : QDialog(parent, f) {
		setSizeGripEnabled(false);
		setWindowFlags(Qt::SplashScreen);
		setWindowOpacity(1.0);
		
		// QStyle* plastiqueStyle = new QPlastiqueStyle;
		mQuickCommandLabel = new QLabel(tr("Type a command:"));
		this->setAutoFillBackground(true);
		// this->setStyle(plastiqueStyle);
		QPalette p = this->palette();
		p.setColor(this->backgroundRole(), QColor(67,68,88,255));
		this->setPalette(p);	
		
		mLineEdit = new QLineEdit(this);
		mCompleter = new QCompleter(this);

		mCompleter->setCaseSensitivity(Qt::CaseInsensitive);
		mCompleter->setCompletionMode(QCompleter::PopupCompletion);
		//loop through the actions added to the widget *m, add the text to a word list for now
		for (int i = 0; i < m->actions().count(); ++i)
			mWordList << ((QAction*)(m->actions().at(i)))->toolTip();

		mModel = new QStringListModel(mWordList,mCompleter);
		mCompleter->setModel(mModel);
		mLineEdit->setCompleter(mCompleter);
		
		QVBoxLayout *vbox = new QVBoxLayout(this);
		vbox->addWidget(mQuickCommandLabel);
		vbox->addWidget(mLineEdit);

		// QShortcut *shortcut = new QShortcut(QKeySequence(tr("Enter")),this);
		connect(mLineEdit, SIGNAL(editingFinished()), this, SLOT(accept()));
	}
	
private:
	
	QLineEdit *mLineEdit;								//!< single line input widget
	QStringList mWordList;							//!< array of strings to feed to the AutoCompleter class
	QCompleter *mCompleter;							//!< the autoCompletion widget
	QLabel *mQuickCommandLabel;					//!< short instructions displayed in the popup window
	QStringListModel *mModel;						//!< the completion model for the AutoCompleter widget
		
public slots:

	/*!
	* \brief causes the CommandCompleter dialog to popup and sets the focus to the text input box
	* 
	* this dialog for now is modal, meaning no other windows will accept focus while this window is displayed.
	* the user can hit escape to hide the window if they do not want to input anything, or simply hit enter.
	* 
	*/
	int exec(){
		mLineEdit->clear();
		if ( QDialog::exec() == QDialog::Accepted )
			return mWordList.indexOf(mLineEdit->text());
		// else hide();
		return -2;
	}

};
	
#endif