/****************************************************************************
 **
 **
 ****************************************************************************/

#ifndef VERSETOPMOD_H
#define VERSETOPMOD_H

#include "verse.h"

#include <QWidget>

class QLineEdit;
class QTextEdit;
// class QProcess;

// void* mVerseTopModPtr;        // global variable which points to an arbitrary object

class VerseTopMod : public QWidget {
	Q_OBJECT
	
public:
	VerseTopMod(QWidget *parent = 0, Qt::WindowFlags f = Qt::Tool );
	~VerseTopMod();
	
	void callback_node_create(void *user, VNodeID node_id, VNodeType type, VNodeOwner ownership);
	// static void wrapper_callback_node_create(void *user, VNodeID node_id, VNodeType type, VNodeOwner ownership);
	
	void callback_accept_connect(void *user, uint32 avatar, void *address, void *connection, uint8 *host_id);
	// static void wrapper_callback_accept_connect(void *user, uint32 avatar, void *address, void *connection, uint8 *host_id);
	
public slots:
	void executeCommand();
	//actions in the file menu
	void connectLocalhost();
	void connectHost();
	void disconnectHost();
	void disconnectAll();

private:
  QLineEdit *mLineEdit;
  QTextEdit *mTextEdit;
  
};

#endif
