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

class CommandCompleter : public QDialog {
	Q_OBJECT

public:

	CommandCompleter ( QWidget * parent = 0, Qt::WindowFlags f = 0 ) : QDialog(parent, f) {
		lineEdit = new QLineEdit(this);

		completer = new QCompleter(this);
		completer->setCaseSensitivity(Qt::CaseInsensitive);
		completer->setCompletionMode(QCompleter::UnfilteredPopupCompletion);
		completer->setModel(modelFromFile(":/resources/commands.txt"));

		lineEdit->setCompleter(completer);
		
		QVBoxLayout *vbox = new QVBoxLayout(this);
		vbox->addWidget(lineEdit);

	}
	
	QAbstractItemModel *modelFromFile(const QString& fileName) {
	    QFile file(fileName);
	    if (!file.open(QFile::ReadOnly))
	        return new QStringListModel(completer);

	    QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
	    QStringList words;
	    while (!file.atEnd()) {
	        QByteArray line = file.readLine();
	        if (!line.isEmpty())
	            words << line.trimmed();
	    }
	    QApplication::restoreOverrideCursor();
			return new QStringListModel(words, completer);
	}
	
private:
	
	QLineEdit *lineEdit;
	QStringList wordList;
	QCompleter *completer;
		
public slots:

	QString exec(){
		if ( QDialog::exec() == QDialog::Accepted )
			return lineEdit->text();
		return QString();
	}


};
	
#endif