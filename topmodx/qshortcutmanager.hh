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
*/

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

#ifndef _QSHORTCUT_MANAGER_H_
#define _QSHORTCUT_MANAGER_H_

#include "qcumber.hh"

/*!
	\file qshortcutmanager.h
	\brief Definition of the QShortcutManager class
	
	\see QShortcutManager
*/

#include <QHash>
#include <QList>
#include <QObject>
#include <QString>
#include <QDomElement>
#include <QtXml>

class QAction;
class QDomDocument;
class QShortcutDialog;

class QCUMBER_EXPORT QShortcutManager : public QObject
{
	Q_OBJECT
	
	friend class QShortcutDialog;
	
	public:
		QShortcutManager();
		virtual ~QShortcutManager();
		
		QShortcutDialog* getShortcutDialog();
		
		void applyAll();
		void apply(const QString& s, const QString& t);
		
		bool contains(QAction *a) const;
		
		void registerAction(QAction *a, const QString& cxt, const QString& def);
		void unregisterAction(QAction *a);
		
	public slots:
		void readXml();
		void writeXml();
		
		void configure();
		void languageChanged(const QString& lang);
		
	private slots:
		void destroyed(QObject *o);
		
	private:
		QString file(const QString& lang);
		QDomElement node(QAction *a, const QString& cxt);
		QDomElement node(const QString& n, const QString& cxt);
		
		QString sLang;
		
		QDomDocument *pDoc;
		
		QHash<QString, QString> m_shortcuts;
		QHash<QString, QList<QAction*> > m_actions;
		
		QShortcutDialog *pDialog;
};

#endif 
// _QSHORTCUT_MANAGER_H_
