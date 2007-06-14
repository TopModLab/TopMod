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

class CommandCompleter : public QDialog {
	Q_OBJECT

public:

	// CommandCompleter ( QStringList words, QWidget * parent = 0, Qt::WindowFlags f = 0 ) : QDialog(parent, f) {
	CommandCompleter ( QWidget *m, QWidget * parent = 0, Qt::WindowFlags f = 0 ) : QDialog(parent, f) {
		setSizeGripEnabled(false);
		setWindowFlags(Qt::SplashScreen);
		setWindowOpacity(0.7);
		
		// QStyle* plastiqueStyle = new QPlastiqueStyle;
		mQuickCommandLabel = new QLabel(tr("Type a command:"));
		this->setAutoFillBackground(true);
		// this->setStyle(plastiqueStyle);
		QPalette p = this->palette();
		p.setColor(this->backgroundRole(), QColor(255,255,255,127));
		this->setPalette(p);	
		
		mLineEdit = new QLineEdit(this);
		mCompleter = new QCompleter(this);

		mCompleter->setCaseSensitivity(Qt::CaseInsensitive);
		mCompleter->setCompletionMode(QCompleter::PopupCompletion);
		//loop through the actions added to the widget *m, add the text to a word list for now
		for (int i = 0; i < m->actions().count(); ++i)
			mWordList << ((QAction*)(m->actions().at(i)))->text();

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
	
	QLineEdit *mLineEdit;
	QStringList wordList;
	QCompleter *mCompleter;
	QLabel *mQuickCommandLabel;
	QStringListModel *mModel;
	QStringList mWordList;
		
public slots:

	int exec(){
		mLineEdit->clear();
		if ( QDialog::exec() == QDialog::Accepted )
			return mWordList.indexOf(mLineEdit->text());
		// else hide();
		return -2;
	}

};
	
#endif