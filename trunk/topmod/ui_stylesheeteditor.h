/********************************************************************************
** Form generated from reading ui file 'stylesheeteditor.ui'
**
** Created: Sat May 19 16:32:28 2007
**      by: Qt User Interface Compiler version 4.2.3
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_STYLESHEETEDITOR_H
#define UI_STYLESHEETEDITOR_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QComboBox>
#include <QtGui/QGridLayout>
#include <QtGui/QHBoxLayout>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QTextEdit>
#include <QtGui/QWidget>

class Ui_StyleSheetEditor
{
public:
    QGridLayout *gridLayout;
    QSpacerItem *spacerItem;
    QSpacerItem *spacerItem1;
    QComboBox *styleSheetCombo;
    QSpacerItem *spacerItem2;
    QComboBox *styleCombo;
    QLabel *label_7;
    QHBoxLayout *hboxLayout;
    QSpacerItem *spacerItem3;
    QPushButton *applyButton;
    QTextEdit *styleTextEdit;
    QLabel *label_8;

    void setupUi(QWidget *StyleSheetEditor)
    {
    StyleSheetEditor->setObjectName(QString::fromUtf8("StyleSheetEditor"));
    gridLayout = new QGridLayout(StyleSheetEditor);
    gridLayout->setSpacing(6);
    gridLayout->setMargin(9);
    gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
    spacerItem = new QSpacerItem(32, 20, QSizePolicy::MinimumExpanding, QSizePolicy::Minimum);

    gridLayout->addItem(spacerItem, 0, 6, 1, 1);

    spacerItem1 = new QSpacerItem(32, 20, QSizePolicy::MinimumExpanding, QSizePolicy::Minimum);

    gridLayout->addItem(spacerItem1, 0, 0, 1, 1);

    styleSheetCombo = new QComboBox(StyleSheetEditor);
    styleSheetCombo->setObjectName(QString::fromUtf8("styleSheetCombo"));

    gridLayout->addWidget(styleSheetCombo, 0, 5, 1, 1);

    spacerItem2 = new QSpacerItem(10, 16, QSizePolicy::Fixed, QSizePolicy::Minimum);

    gridLayout->addItem(spacerItem2, 0, 3, 1, 1);

    styleCombo = new QComboBox(StyleSheetEditor);
    styleCombo->setObjectName(QString::fromUtf8("styleCombo"));
    QSizePolicy sizePolicy(static_cast<QSizePolicy::Policy>(5), static_cast<QSizePolicy::Policy>(0));
    sizePolicy.setHorizontalStretch(0);
    sizePolicy.setVerticalStretch(0);
    sizePolicy.setHeightForWidth(styleCombo->sizePolicy().hasHeightForWidth());
    styleCombo->setSizePolicy(sizePolicy);

    gridLayout->addWidget(styleCombo, 0, 2, 1, 1);

    label_7 = new QLabel(StyleSheetEditor);
    label_7->setObjectName(QString::fromUtf8("label_7"));
    QSizePolicy sizePolicy1(static_cast<QSizePolicy::Policy>(0), static_cast<QSizePolicy::Policy>(5));
    sizePolicy1.setHorizontalStretch(0);
    sizePolicy1.setVerticalStretch(0);
    sizePolicy1.setHeightForWidth(label_7->sizePolicy().hasHeightForWidth());
    label_7->setSizePolicy(sizePolicy1);

    gridLayout->addWidget(label_7, 0, 1, 1, 1);

    hboxLayout = new QHBoxLayout();
    hboxLayout->setSpacing(6);
    hboxLayout->setMargin(0);
    hboxLayout->setObjectName(QString::fromUtf8("hboxLayout"));
    spacerItem3 = new QSpacerItem(321, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

    hboxLayout->addItem(spacerItem3);

    applyButton = new QPushButton(StyleSheetEditor);
    applyButton->setObjectName(QString::fromUtf8("applyButton"));
    applyButton->setEnabled(false);

    hboxLayout->addWidget(applyButton);


    gridLayout->addLayout(hboxLayout, 2, 0, 1, 7);

    styleTextEdit = new QTextEdit(StyleSheetEditor);
    styleTextEdit->setObjectName(QString::fromUtf8("styleTextEdit"));

    gridLayout->addWidget(styleTextEdit, 1, 0, 1, 7);

    label_8 = new QLabel(StyleSheetEditor);
    label_8->setObjectName(QString::fromUtf8("label_8"));
    QSizePolicy sizePolicy2(static_cast<QSizePolicy::Policy>(0), static_cast<QSizePolicy::Policy>(5));
    sizePolicy2.setHorizontalStretch(0);
    sizePolicy2.setVerticalStretch(0);
    sizePolicy2.setHeightForWidth(label_8->sizePolicy().hasHeightForWidth());
    label_8->setSizePolicy(sizePolicy2);

    gridLayout->addWidget(label_8, 0, 4, 1, 1);


    retranslateUi(StyleSheetEditor);

    QSize size(445, 289);
    size = size.expandedTo(StyleSheetEditor->minimumSizeHint());
    StyleSheetEditor->resize(size);


    QMetaObject::connectSlotsByName(StyleSheetEditor);
    } // setupUi

    void retranslateUi(QWidget *StyleSheetEditor)
    {
    StyleSheetEditor->setWindowTitle(QApplication::translate("StyleSheetEditor", "Style Editor", 0, QApplication::UnicodeUTF8));
    styleSheetCombo->clear();
    styleSheetCombo->addItem(QApplication::translate("StyleSheetEditor", "Default", 0, QApplication::UnicodeUTF8));
    styleSheetCombo->addItem(QApplication::translate("StyleSheetEditor", "Modo", 0, QApplication::UnicodeUTF8));
    label_7->setText(QApplication::translate("StyleSheetEditor", "Style:", 0, QApplication::UnicodeUTF8));
    applyButton->setText(QApplication::translate("StyleSheetEditor", "&Apply", 0, QApplication::UnicodeUTF8));
    label_8->setText(QApplication::translate("StyleSheetEditor", "Style Sheet:", 0, QApplication::UnicodeUTF8));
    Q_UNUSED(StyleSheetEditor);
    } // retranslateUi

};

namespace Ui {
    class StyleSheetEditor: public Ui_StyleSheetEditor {};
} // namespace Ui

#endif // UI_STYLESHEETEDITOR_H
