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

#include <QtGui>
#include "stylesheeteditor.hh"

 StyleSheetEditor::StyleSheetEditor(QWidget *parent)
     : QWidget(parent)
 {
     ui.setupUi(this);

     QRegExp regExp("Q(.*)Style");
     QString defaultStyle = QApplication::style()->metaObject()->className();
     if (regExp.exactMatch(defaultStyle))
         defaultStyle = regExp.cap(1);

     ui.styleCombo->addItems(QStyleFactory::keys());
     ui.styleCombo->setCurrentIndex(ui.styleCombo->findText(defaultStyle));
     //ui.styleSheetCombo->setCurrentIndex(ui.styleSheetCombo->findText("Coffee"));
     loadStyleSheet("default");
 }

 void StyleSheetEditor::on_styleCombo_activated(const QString &styleName)
 {
     qApp->setStyle(styleName);
     ui.applyButton->setEnabled(false);
 }

 void StyleSheetEditor::on_styleSheetCombo_activated(const QString &sheetName)
 {
     loadStyleSheet(sheetName);
 }

 void StyleSheetEditor::on_styleTextEdit_textChanged()
 {
     ui.applyButton->setEnabled(true);
 }

 void StyleSheetEditor::on_applyButton_clicked()
 {
     qApp->setStyleSheet(ui.styleTextEdit->toPlainText());
     ui.applyButton->setEnabled(false);
 }

 void StyleSheetEditor::loadStyleSheet(const QString &sheetName)
 {
     QFile file(":/qss/" + sheetName.toLower() + ".qss");
     file.open(QFile::ReadOnly);
     QString styleSheet = QLatin1String(file.readAll());

     ui.styleTextEdit->setPlainText(styleSheet);
     qApp->setStyleSheet(styleSheet);
     ui.applyButton->setEnabled(false);
 }


