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

 #ifndef STYLESHEETEDITOR_H
 #define STYLESHEETEDITOR_H

 #include <QDialog>

 #include "ui_stylesheeteditor.h"

class StyleSheetEditor : public QDialog {
Q_OBJECT

public:
  StyleSheetEditor(QWidget *parent = 0);

private slots:
  void on_styleCombo_activated(const QString &styleName);
  void on_styleSheetCombo_activated(const QString &styleSheetName);
  void on_styleTextEdit_textChanged();
  void on_applyButton_clicked();
  
private:
  void loadStyleSheet(const QString &sheetName);

  Ui::StyleSheetEditor ui;
};

#endif
