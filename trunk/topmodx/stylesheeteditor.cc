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


