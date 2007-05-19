/********************************************************************************
** Form generated from reading ui file 'shortcutdialog.ui'
**
** Created: Sat May 19 17:29:05 2007
**      by: Qt User Interface Compiler version 4.2.3
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_SHORTCUTDIALOG_H
#define UI_SHORTCUTDIALOG_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QHBoxLayout>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QTreeWidget>
#include <QtGui/QVBoxLayout>

class Ui_ShortcutDialog
{
public:
    QVBoxLayout *vboxLayout;
    QTreeWidget *twShortcuts;
    QHBoxLayout *hboxLayout;
    QSpacerItem *spacerItem;
    QPushButton *okButton;
    QPushButton *cancelButton;

    void setupUi(QDialog *ShortcutDialog)
    {
    ShortcutDialog->setObjectName(QString::fromUtf8("ShortcutDialog"));
    vboxLayout = new QVBoxLayout(ShortcutDialog);
    vboxLayout->setSpacing(4);
    vboxLayout->setMargin(2);
    vboxLayout->setObjectName(QString::fromUtf8("vboxLayout"));
    twShortcuts = new QTreeWidget(ShortcutDialog);
    twShortcuts->headerItem()->setIcon(0, QIcon(QString::fromUtf8(":/exec.png")));
    twShortcuts->setObjectName(QString::fromUtf8("twShortcuts"));

    vboxLayout->addWidget(twShortcuts);

    hboxLayout = new QHBoxLayout();
    hboxLayout->setSpacing(2);
    hboxLayout->setMargin(0);
    hboxLayout->setObjectName(QString::fromUtf8("hboxLayout"));
    spacerItem = new QSpacerItem(131, 31, QSizePolicy::Expanding, QSizePolicy::Minimum);

    hboxLayout->addItem(spacerItem);

    okButton = new QPushButton(ShortcutDialog);
    okButton->setObjectName(QString::fromUtf8("okButton"));

    hboxLayout->addWidget(okButton);

    cancelButton = new QPushButton(ShortcutDialog);
    cancelButton->setObjectName(QString::fromUtf8("cancelButton"));

    hboxLayout->addWidget(cancelButton);


    vboxLayout->addLayout(hboxLayout);


    retranslateUi(ShortcutDialog);

    QSize size(400, 300);
    size = size.expandedTo(ShortcutDialog->minimumSizeHint());
    ShortcutDialog->resize(size);

    QObject::connect(okButton, SIGNAL(clicked()), ShortcutDialog, SLOT(accept()));
    QObject::connect(cancelButton, SIGNAL(clicked()), ShortcutDialog, SLOT(reject()));

    QMetaObject::connectSlotsByName(ShortcutDialog);
    } // setupUi

    void retranslateUi(QDialog *ShortcutDialog)
    {
    ShortcutDialog->setWindowTitle(QApplication::translate("ShortcutDialog", "Set shortcuts", 0, QApplication::UnicodeUTF8));
    twShortcuts->headerItem()->setText(0, QApplication::translate("ShortcutDialog", "Action", 0, QApplication::UnicodeUTF8));
    twShortcuts->headerItem()->setText(1, QApplication::translate("ShortcutDialog", "Shortcut", 0, QApplication::UnicodeUTF8));
    okButton->setText(QApplication::translate("ShortcutDialog", "OK", 0, QApplication::UnicodeUTF8));
    cancelButton->setText(QApplication::translate("ShortcutDialog", "Cancel", 0, QApplication::UnicodeUTF8));
    Q_UNUSED(ShortcutDialog);
    } // retranslateUi

};

namespace Ui {
    class ShortcutDialog: public Ui_ShortcutDialog {};
} // namespace Ui

#endif // UI_SHORTCUTDIALOG_H
