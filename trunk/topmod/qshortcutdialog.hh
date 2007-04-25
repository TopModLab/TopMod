/****************************************************************************
**
** Copyright (C) 2006 FullMetalCoder
**
** This file is part of the Edyuk project (beta version)
** 
** This file may be used under the terms of the GNU General Public License
** version 2 as published by the Free Software Foundation and appearing in the
** file GPL.txt included in the packaging of this file.
**
** Notes :	Parts of the project are derivative work of Trolltech's QSA library
** or Trolltech's Qt4 framework but, unless notified, every single line of code
** is the work of the Edyuk team or a contributor. 
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/

#ifndef _QSHORTCUT_DIALOG_H_
#define _QSHORTCUT_DIALOG_H_

#include "qcumber.hh"

#include "ui_shortcutdialog.h"

#include <QtXml>

/*!
	\file qshortcutdialog.h
	\brief Definition of the QShortcutDialog class
	
	\see QShortcutDialog
*/

#include <QObject>
#include <QDialog>

class QTreeWidgetItem;
class QShortcutManager;

class QCUMBER_EXPORT QShortcutDialog : public QDialog, private Ui::ShortcutDialog
{
	Q_OBJECT
	
	public:
		QShortcutDialog(QShortcutManager *m, QWidget *p = 0);
		
		void retranslate();
		
		void exec();
		
	private slots:
		void on_twShortcuts_itemDoubleClicked(QTreeWidgetItem *i, int col);
		
	private:
		QShortcutManager *pManager;
};

#endif // _DEV_SHORTCUT_DIALOG_H_
