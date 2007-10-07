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

#include "qshortcutmanager.hh"

#include "qshortcutdialog.hh"

#include <QTextStream>

/*!
	\file qshortcutmanager.cpp
	\brief implementation of the QShortcutManager class
	
	\see QShortcutManager
*/

/*!
	\class QShortcutManager
	\brief Generic shortcut management class.
	
	This class manages shortcuts for the complete application, plugins included.
	The only thing needed is to register actions and to make sure not to override
	shortcuts of managed actions.
	
	\see registerAction()
	\see unregisterAction()
*/

#include <QDir>
#include <QFile>
// #include "qtglogal.h"
//typedef void* quintptr; // Has a previous declaration Qt 4.2

/*!
	\brief Constructor
*/
QShortcutManager::QShortcutManager()
 : sLang("en_us") {
	pDialog = new QShortcutDialog(this);
	pDoc = new QDomDocument("SHORTCUTS");
	readXml();
}

/*!
	\brief Destructor
*/
QShortcutManager::~QShortcutManager()
{
	writeXml();
	
	delete pDoc;
}

QShortcutDialog* QShortcutManager::getShortcutDialog(){
	return pDialog;
}

/*!
	\brief Shows the shortcut configuration dialog
*/
void QShortcutManager::configure()
{
	pDialog->exec();
}

/*!
	\brief Applies the changes
*/
void QShortcutManager::applyAll()
{
	
}

/*!
	\brief Applies the change
*/
void QShortcutManager::apply(const QString& s, const QString& t)
{
	QHash<QString, QString>::iterator i;
	
	#ifdef _DEBUG_
	//qDebug("target = %s;", qPrintable(t));
	#endif
	
	QStringList l = t.split('/');
	
	QString name = l.takeAt(l.count() - 1);
	QString context = l.join("/");
	
	if ( s.count() )
	{
		if ( m_shortcuts.contains(s) )
		{
			if ( m_shortcuts.value(s) == t )
				return;
			
			apply("", m_shortcuts.value(s));
		}
		
		m_shortcuts[s] = t;
	}
	
	#ifdef _DEBUG_
	qDebug("context = %s; name = %s; shortcut = %s;",
			qPrintable(context),
			qPrintable(name),
			qPrintable(s));
	#endif
	
	QDomElement e = node(name, context);
	
	// update XML config file
	e.setAttribute("shortcut", s);
	
	// apply shortcut sequences to registered actions
	foreach ( QAction *a, m_actions[t] )
	{
		a->setShortcut(s);
	}
	
}

/*!
	\brief changes shortcut data according to \a lang
	
	\param lang current language
	
	If another language was previously managed, shortcuts are automatically
	saved.
*/
void QShortcutManager::languageChanged(const QString& lang)
{
	if ( lang == sLang )
		return;
	
	#ifdef _DEBUG_
	qDebug() << "giving up " << sLang << " and switching to " << lang;
	#endif
	
	// save previous shortcuts
	writeXml();
	
	sLang = lang;
	
	// read new shortcuts
	readXml();
	
	QHash<QString, QList<QAction*> >::iterator i;
	
	for ( i = m_actions.begin(); i != m_actions.end(); i++ )
	{
		QDomElement e = node(i.key().section('/', -1), i.key().section('/', 0, -2));
		QString ks = e.attribute("shortcut");
		
		if ( ks.isEmpty() )
		{
			apply(e.attribute("default"), i.key());
		} else {
			apply(ks, i.key());
		}
	}
	
	pDialog->retranslate();
}

/*!
	\internal
*/
void QShortcutManager::readXml()
{
	QFile f(file(sLang));
	
	#ifdef _EDYUK_DEBUG_
	qDebug() << "reading xml shortcut file : " << file(sLang) << " ...";
	#endif
	
	if ( !f.open(QFile::ReadOnly | QFile::Text) )
	{
		if ( !f.open(QFile::WriteOnly | QFile::Text) )
			qWarning("Unable to access shortcuts...");
		
		QString s = "<!DOCTYPE SHORTCUTS>\n<SHORTCUTS>\n\n</SHORTCUTS>\n";
		
		QTextStream out(&f);
		out << s;
		
		f.close();
		
		if ( !f.open(QFile::ReadOnly | QFile::Text) )
			return (void)qWarning("Unable to access shortcuts...");
	}
	
	if ( !pDoc->setContent(&f) || pDoc->documentElement().isNull() )
	{
		QDomElement root = pDoc->createElement("SHORTCUTS");
		
		pDoc->appendChild(root);
	}
}

/*!
	\internal
*/
void QShortcutManager::writeXml()
{
	if ( !sLang.isEmpty() )
	{
		QFile prev(file(sLang));
		QTextStream  out(&prev);
		
		if ( prev.open(QFile::WriteOnly | QFile::Text) )
			out << pDoc->toString(4).replace("    ", "\t");
		else
			qWarning("Can\'t save %s shortcuts : check out permissions",
					qPrintable(sLang));
	}
}

/*!
	\brief check if action \a a is already registered
	
	\return true if a is found, otherwise false
*/
bool QShortcutManager::contains(QAction *a) const
{
	foreach ( QList<QAction*> l, m_actions )
		if ( l.contains(a) )
			return true;
	
	return false;
}

/*!
	\brief register an action in the shortcut manager
	
	\param a action to register
	\param cxt context of the action, usually a menu/toolbar name
	\param def default shortcut
*/
void QShortcutManager::registerAction(QAction *a, const QString& cxt, const QString& def)
{
	if ( !a || contains(a) )
		return;
	
	#ifdef _DEBUG_
	qDebug() << "adding action : " << a->text()
			<< " from " << cxt
			<< " with " << def;
	#endif
	
	connect(a	, SIGNAL( destroyed(QObject*) ),
			this, SLOT  ( destroyed(QObject*) ) );
	
	m_actions[cxt + "/" + a->text()] << a;
	
	QDomElement e = node(a, cxt);
	
	e.setAttribute("default", def);
	
	QString sh = e.attribute("shortcut");
	
	if ( sh.isEmpty() && !m_shortcuts.contains(def) )
		sh = def;
	
	if ( sh.count() )
	{
		e.setAttribute("shortcut", sh);
		m_shortcuts[sh] = cxt + "/" + a->text();
		
		a->setShortcut(QKeySequence(sh));
	}
	
	//apply(def, cxt + "/" + a->text());
}

/*!
	\brief unregister an action from the shortcut manager
	
	\param a action to unregister
*/
void QShortcutManager::unregisterAction(QAction *a)
{
	if ( !a )
		return;
	
	foreach ( QList<QAction*> l, m_actions )
	{
		if ( l.contains(a) )
		{
			l.removeAll(a);
			a->setShortcut(QKeySequence(""));
		}
	}
}

/*!
 	\param lang Language whose shortcut file is asked
 	\return file name of the shrtcut file corresponding to language \a lang
*/
QString QShortcutManager::file(const QString& lang)
{
	// return 	QDir::homePath()
	// 		+ QDir::separator()
	// 		+ "."
	// 		+ QApplication::applicationName()
	// 		+ QDir::separator()
	// 		+ "shortcuts"
	// 		+ QDir::separator()
	// 		+ "shortcuts_"
	// 		+ "en_us"
	// 		+ ".xml";
	
	return "shortcuts_" + lang + ".xml";

}

/*!
	\overload
	
	\param a action whose xml representation is searched
	\param cxt context of the action (usually menu/toolbar name)
	
	\return the corresponding dom element, if none found, one is created.
*/
QDomElement QShortcutManager::node(QAction *a, const QString& cxt)
{
	if ( !a )
		qFatal("Can\'t find xml for a NULL action!!!");
	
	return node(a->text(), cxt);
}

/*!
	\brief find the xml element representing the name \a n in context \a cxt
	
	\param a action whose xml representation is searched
	\param cxt context of the action (usually menu/toolbar name)
	
	\return the corresponding dom element, if none found, one is created.
*/
QDomElement QShortcutManager::node(const QString& n, const QString& cxt)
{
	QDomElement elem;
	QDomNodeList nodes = pDoc->elementsByTagName("action");
	
	for ( int i = 0; i < nodes.size(); i++ )
	{
		elem = nodes.at(i).toElement();
		
		if ( elem.attribute("name") == n && elem.attribute("context") == cxt )
			return elem;
	}
	
	elem = pDoc->createElement("action");
	
	elem.setAttribute("name", n);
	elem.setAttribute("context", cxt);
	
	pDoc->documentElement().appendChild(elem);
	
	return elem;
}

/*!
	\internal
*/
void QShortcutManager::destroyed(QObject *o)
{
	if ( !o )
		return;
	
	//qDebug("spotted deletion...");
	
	foreach ( QList<QAction*> l, m_actions )
	{
		QList<QAction*>::iterator i = l.begin();
		
		while ( i != l.end() )
		{
			if ( quintptr(*i) == quintptr(o) )
			{
				//qDebug("erasing occurence of %s", qPrintable((*i)->text()));
				i = l.erase(i);
			} else {
				i++;
			}
		}
	}
}

