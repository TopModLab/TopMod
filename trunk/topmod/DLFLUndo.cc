/* $Id: DLFLUndo.cc,v 4.1 2004/02/24 20:41:44 vinod Exp $ */

#include "DLFLWindow.hh"

//-- Subroutines dealing with undo and redo for DLFLWindow --//

void DLFLWindow::clearUndoList(void)
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

void DLFLWindow::clearRedoList(void)
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

void DLFLWindow::undoPush(void)
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

void DLFLWindow::undo(void) {
	
	if ( !undoList.empty() ) {		
		// Restore previous object
		// Put current object to end of redo list
		// Take last element of undo list and re-create current object
		StringStreamPtr curobj = new StringStream;
		object.writeDLFL(*curobj);
		redoList.push_back(curobj);

		StringStreamPtr oldobj = undoList.back();;
		object.readDLFL(*oldobj);
		undoList.pop_back(); delete oldobj;

		recomputePatches();
		recomputeNormals();
		// Clear selection lists to avoid dangling pointers
		DLFLWindow::clearSelected();
		redraw();
		/* is document modified? - dave */
		setModified(true);
		mIsPrimitive = false; 
	}
	if (undoList.empty()) {
		setModified(false);
	}
}

void DLFLWindow::redo(void) {
	
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

		recomputePatches();
		recomputeNormals();
		// Clear selection lists to avoid dangling pointers
		DLFLWindow::clearSelected();
		redraw();
		/* is document modified? - dave */
		setModified(true);
		mIsPrimitive = false;
	}
}
