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

/* $Id: DLFLUndo.cc,v 4.1 2004/02/24 20:41:44 vinod Exp $ */

#include "MainWindow.hh"

//-- Subroutines dealing with undo and redo for DLFLWindow --//

void MainWindow::clearUndoList(void)
{
  StringStreamPtrList::iterator first, last;
  first = undoList.begin(); last = undoList.end();
  while ( first != last )
     {
       StringStreamPtr temp = (*first); ++first;
       delete temp;
     }
  undoList.clear();
}

void MainWindow::clearRedoList(void)
{
  StringStreamPtrList::iterator first, last;
  first = redoList.begin(); last = redoList.end();
  while ( first != last )
     {
       StringStreamPtr temp = (*first); ++first;
       delete temp;
     }
  redoList.clear();
}

void MainWindow::undoPush(void)
{
     // Don't do anything unless undo is required
  if ( useUndo == false ) return;

     // Put current object on top of undo list
     // Check if we have reached undo limit, in which case remove oldest state
     // and add current state to end of list.
  if ( undoList.size() > undolimit )
     {
       StringStreamPtr temp = undoList.front();
       delete temp;
       undoList.pop_front();
     }

  StringStreamPtr curobj = new StringStream;
  object.writeDLFL(*curobj);
  undoList.push_back(curobj);

     // Evertime a new operation is done, previous state is put into UndoList
     // At the same time the redo list should be cleared, because we have
     // nothing to redo immediately after an operation.
  clearRedoList();
}

void MainWindow::undo(void) {
	
	if ( !undoList.empty() ) {		
 		// Restore previous object
		// Put current object to end of redo list
		// Take last element of undo list and re-create current object
		StringStreamPtr curobj = new StringStream;
		object.writeDLFL(*curobj);
		redoList.push_back(curobj);
		StringStreamPtr oldobj = undoList.back();
		object.readDLFL(*oldobj,true);
		undoList.pop_back(); delete oldobj;
		active->recomputePatches();
		active->recomputeNormals();
		// Clear selection lists to avoid dangling pointers
		MainWindow::clearSelected();
		redraw();
		/* is document modified? - dave */
		setModified(true);
		mIsPrimitive = false; 
	}
	if (undoList.empty()) {
		setModified(false);
		if (mWasPrimitive)
			mIsPrimitive = true;
	}
}

void MainWindow::redo(void) {
	
  if ( !redoList.empty() ) {
		// Redo previously undone operation
		// Put current object to end of undo list
		// Take last element of redo list and re-create current object
		StringStreamPtr curobj = new StringStream;
		object.writeDLFL(*curobj);
		undoList.push_back(curobj);

		StringStreamPtr newobj = redoList.back();
		object.readDLFL(*newobj);
		redoList.pop_back(); delete newobj;

		active->recomputePatches();
		active->recomputeNormals();
		// Clear selection lists to avoid dangling pointers
		MainWindow::clearSelected();
		redraw();
		/* is document modified? - dave */
		setModified(true);
		mIsPrimitive = false;
	}
}
