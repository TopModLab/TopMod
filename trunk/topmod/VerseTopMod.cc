
#include <iostream>
#include <QtGui>

#include <stdio.h>
#include <stdlib.h>

#include "VerseTopMod.hh"

/* A callback for connection acception: will be called when server accepts this client. */
void VerseTopMod::callback_accept_connect(void *user, uint32 avatar, void *address, void *connection, uint8 *host_id) {

	uint32 i, mask = 0;

	mTextEdit->insertPlainText("Connected to a Verse host!\n\nListing nodes:\n");

	/* Build node subscription mask. */
	for(i = 0; i < V_NT_NUM_TYPES; i++)
		mask |= 1 << i;
	
	verse_send_node_index_subscribe(mask);     /* Request listing of all nodes. */
}

/* A callback for node creation: is called to report information about existing nodes, too. */
void VerseTopMod::callback_node_create(void *user, VNodeID node_id, VNodeType type, VNodeOwner ownership) {

	mTextEdit->insertPlainText(" Node #" + QString(node_id) + " has type " + QString(type) + "\n");
}

//constructor
VerseTopMod::VerseTopMod(QWidget *parent, Qt::WindowFlags f)
	: QWidget(parent) {
		
	mTextEdit = new QTextEdit;    
	mLineEdit = new QLineEdit;
	QObject::connect(mLineEdit, SIGNAL(returnPressed()), this, SLOT(executeCommand()));    

	QPalette whiteOnBlack;
	whiteOnBlack.setColor(QPalette::Active, QPalette::Text, Qt::white );
	whiteOnBlack.setColor(QPalette::Active, QPalette::Base, Qt::black );
	mTextEdit->setPalette( whiteOnBlack );
	mLineEdit->setPalette( whiteOnBlack );

	QVBoxLayout *mainLayout = new QVBoxLayout;
	mainLayout->addWidget(mTextEdit);
	mainLayout->addWidget(mLineEdit);
	setLayout(mainLayout);

	/* Register callbacks for interesting commands. */
	// verse_callback_set((void *)verse_send_connect_accept, (void *)callback_accept_connect, NULL);
	// verse_callback_set((void *)verse_send_node_create,	  (void *)callback_node_create, NULL);

	/* Kick off program by connecting to Verse host on local machine. */
	verse_send_connect("TopMod", "password", "localhost", NULL);
	// while(TRUE)
	// verse_callback_update(10000);   /* Listen to network, get callbacks. */
	
}

VerseTopMod::~VerseTopMod(){
	
}

void VerseTopMod::executeCommand( )
{
  QString command = mLineEdit->text();
  QString result;

  mTextEdit->moveCursor( QTextCursor::End );

  if( !command.isEmpty() ) {

  } 
	else {
    command = QString("");
    result = QString("");
  }

  const QByteArray cmd(command.toLatin1());
  mTextEdit->insertPlainText( "VerseTopMod> " + command );
  if( !result.isEmpty() )
  	mTextEdit->insertPlainText( "\n"+result );
  mLineEdit->clear();

  QScrollBar *vBar = mTextEdit->verticalScrollBar();
  vBar->triggerAction(QAbstractSlider::SliderToMaximum);
}

void VerseTopMod::connectLocalhost(){
	
}

void VerseTopMod::connectHost(){
	
}

void VerseTopMod::disconnectHost(){
	
}

void VerseTopMod::disconnectAll(){
	
}


