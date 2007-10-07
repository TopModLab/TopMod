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

#ifdef WITH_VERSE

#include <iostream>
#include <QtGui>

#include <stdio.h>
#include <stdlib.h>

#include "VerseTopMod.hh"

// /* A callback for connection acception: will be called when server accepts this client. */
// void callback_accept_connect(void *user, uint32 avatar, void *address, void *connection, uint8 *host_id) {
// 
// 	uint32 i, mask = 0;
// 	VerseTopMod::Instance()->write("Connected to a Verse host!\n\nListing nodes:\n");
// 	// std::cout <<"Connected to a Verse host!\n\nListing nodes:\n";
// 	
// 	/* Build node subscription mask. */
// 	for(i = 0; i < V_NT_NUM_TYPES; i++)
// 		mask |= 1 << i;
// 	verse_send_node_index_subscribe(mask);     /* Request listing of all nodes. */
// }
// 
// /* A callback for node creation: is called to report information about existing nodes, too. */
// void callback_node_create(void *user, VNodeID node_id, VNodeType type, VNodeOwner ownership) {
// 	VerseTopMod::Instance()->write(" Node #" + QString(node_id) + " has type " + QString(type) + "\n");
// 	// std::cout <<" Node #" << node_id << " has type " << type << std::endl;
// }

VerseTopMod* VerseTopMod::pinstance = 0;// initialize pointer

VerseTopMod* VerseTopMod::Instance (QWidget *parent) 
{
  if (pinstance == 0)  // is it the first call?
    pinstance = new VerseTopMod(parent); // create sole instance
  return pinstance; // address of sole instance
}

VerseTopMod* VerseTopMod::Instance () 
{
  if (pinstance == 0)  // is it the first call?
    pinstance = new VerseTopMod(); // create sole instance
  return pinstance; // address of sole instance
}

//constructor
VerseTopMod::VerseTopMod(QWidget *parent, Qt::WindowFlags f)
	: QWidget(parent), isConnected(false) {
	
	mParent = parent;
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
	//create update timer 
	mTimer = new QTimer(this);
  connect(mTimer, SIGNAL(timeout()), this, SLOT(updateVerse()));

	mProcess = new QProcess(this);
	// connect(mProcess, SIGNAL(readyRead()), this, SLOT(writeStandardOutput()));
}

VerseTopMod::VerseTopMod(const VerseTopMod& v) : QWidget(0){
	
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
  mTextEdit->insertPlainText( "\nVerseTopMod> " + command );
  if( !result.isEmpty() )
  	mTextEdit->insertPlainText( "\n"+result );
  mLineEdit->clear();

  QScrollBar *vBar = mTextEdit->verticalScrollBar();
  vBar->triggerAction(QAbstractSlider::SliderToMaximum);
}

void VerseTopMod::write(QString s){
	// std::cout << "VerseTopMod> " << s << std::endl;
	mTextEdit->insertPlainText(" " + s + "\n");
}

void VerseTopMod::connectLocalhost(){

	write(tr("Connecting to localhost!"));
	t_verse_connect("localhost");

	/* Register callbacks for interesting commands. */
	// verse_callback_set((void *)verse_send_connect_accept, (void *)callback_accept_connect, NULL);
	// verse_callback_set((void *)verse_send_node_create,	  (void *)callback_node_create, NULL);
	// 
	// /* Kick off program by connecting to Verse host on local machine. */
	// verse_send_connect("TopMod", "password", "localhost", NULL);
	// while(TRUE)
	// verse_callback_update(100);   /* Listen to network, get callbacks. */
	
  mTimer->start(100);
	((MainWindow*)mParent)->verseConnected();
}

void VerseTopMod::connectHost(){	
	bool ok;
	QString text = QInputDialog::getText(this,tr("Connect to Verse Server"),
 																						tr("Server Address:"), QLineEdit::Normal,
																						tr("localhost"), &ok);
	if (ok && !text.isEmpty() && text.length() < 64){
		write(tr("Connecting to %1!").arg(text));
		QByteArray ba = text.toLatin1();
		t_verse_connect(ba.data());
		mTimer->start(100);
		((MainWindow*)mParent)->verseConnected();		
	}
	
}

void VerseTopMod::disconnectHost(){
	VerseSession *session = NULL;
	session = session_menu();	
	if (session){
		write(tr("Disconnecting session %1!").arg(session->address));
		end_verse_session(session);	
		// mTimer->stop();
	}
	end_all_verse_sessions();
	((MainWindow*)mParent)->verseDisconnected();
}

void VerseTopMod::disconnectAll(){
	write(tr("Disconnecting all sessions!"));	
	end_all_verse_sessions();
	// mTimer->stop();
	((MainWindow*)mParent)->verseDisconnected();
}

void VerseTopMod::updateVerse(){
	// write(tr("Update Verse"));	
	t_verse_update();
}

void VerseTopMod::writeStandardOutput(){
	write(QString(mProcess->readAllStandardOutput()));
}

void VerseTopMod::startServer(){
	connect(mProcess, SIGNAL(readyRead()), this, SLOT(writeStandardOutput()));
	#ifdef WIN32
	mProcess->start("verse.exe");
	#else
	mProcess->start("verse");
	#endif
	((MainWindow*)mParent)->verseStarted();
}

void VerseTopMod::killServer(){
	mProcess->kill();
	((MainWindow*)mParent)->verseKilled();	
}

VerseTopMod::~VerseTopMod(){ }

#endif
