/****************************************************************************
 **
 ** Copyright (C) 2006-2007 Trolltech ASA. All rights reserved.
 **
 ** This file is part of the example classes of the Qt Toolkit.
 **
 ** This file may be used under the terms of the GNU General Public
 ** License version 2.0 as published by the Free Software Foundation
 ** and appearing in the file LICENSE.GPL included in the packaging of
 ** this file.  Please review the following information to ensure GNU
 ** General Public Licensing requirements will be met:
 ** http://www.trolltech.com/products/qt/opensource.html
 **
 ** If you are unsure which license is appropriate for your use, please
 ** review the following information:
 ** http://www.trolltech.com/products/qt/licensing.html or contact the
 ** sales department at sales@trolltech.com.
 **
 ** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
 ** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 **
 ****************************************************************************/

 #include <QtGui>

 #include "stylesheeteditor.hh"

 StyleSheetEditor::StyleSheetEditor(QWidget *parent)
     : QDialog(parent)
 {
     ui.setupUi(this);

     QRegExp regExp("Q(.*)Style");
     QString defaultStyle = QApplication::style()->metaObject()->className();
     if (regExp.exactMatch(defaultStyle))
         defaultStyle = regExp.cap(1);

     ui.styleCombo->addItems(QStyleFactory::keys());
     ui.styleCombo->setCurrentIndex(ui.styleCombo->findText(defaultStyle));
     //ui.styleSheetCombo->setCurrentIndex(ui.styleSheetCombo->findText("Coffee"));
     //loadStyleSheet("Coffee");
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


