/****************************************************************************
 **
 **
 ****************************************************************************/
#ifdef WITH_VERSE

#ifndef VERSETOPMOD_H
#define VERSETOPMOD_H

#include <QWidget>
#include <QTimer>
#include "TKE_verse.h"
#include "TIF_verse.h"
#include "TKE_global.h"

// #include "TLI_dynamiclist.h"
// #include "verse_session.c"
// #include "verse.h"

class QLineEdit;
class QTextEdit;

// extern ListBase session_list;

class VerseTopMod : public QWidget {
	Q_OBJECT
	
public:
  static VerseTopMod* Instance();
	void write(QString s);
protected:
	// VerseTopMod();
	VerseTopMod(QWidget *parent = 0, Qt::WindowFlags f = Qt::Tool );
	VerseTopMod(const VerseTopMod&);
	VerseTopMod& operator= (const VerseTopMod&);
	~VerseTopMod();
private:
	bool isConnected;
  QLineEdit *mLineEdit;
  QTextEdit *mTextEdit;
	static VerseTopMod* pinstance;    
	// QEventLoop *mEventLoop;
	QTimer *mTimer;
	
public slots:
	void executeCommand();
	//actions in the file menu
	void connectLocalhost();
	void connectHost();
	void disconnectHost();
	void disconnectAll();  
	void updateVerse();
};

#endif

#endif
