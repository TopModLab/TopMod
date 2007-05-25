#ifndef STYLESHEETEDITOR_H
#define STYLESHEETEDITOR_H

#include <QDialog>

#include "ui_stylesheeteditor.h"

class StyleSheetEditor : public QWidget {
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
