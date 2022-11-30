/********************************************************************************
** Form generated from reading UI file 'do_work_dialog.ui'
**
** Created by: Qt User Interface Compiler version 5.12.11
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DO_WORK_DIALOG_H
#define UI_DO_WORK_DIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_Do_work_dialog
{
public:
    QPushButton *pb_start;
    QPushButton *pb_start_file;

    void setupUi(QDialog *Do_work_dialog)
    {
        if (Do_work_dialog->objectName().isEmpty())
            Do_work_dialog->setObjectName(QString::fromUtf8("Do_work_dialog"));
        Do_work_dialog->resize(400, 300);
        pb_start = new QPushButton(Do_work_dialog);
        pb_start->setObjectName(QString::fromUtf8("pb_start"));
        pb_start->setGeometry(QRect(40, 110, 81, 41));
        pb_start_file = new QPushButton(Do_work_dialog);
        pb_start_file->setObjectName(QString::fromUtf8("pb_start_file"));
        pb_start_file->setGeometry(QRect(250, 110, 81, 41));

        retranslateUi(Do_work_dialog);

        QMetaObject::connectSlotsByName(Do_work_dialog);
    } // setupUi

    void retranslateUi(QDialog *Do_work_dialog)
    {
        Do_work_dialog->setWindowTitle(QApplication::translate("Do_work_dialog", "Dialog", nullptr));
        pb_start->setText(QApplication::translate("Do_work_dialog", "Start", nullptr));
        pb_start_file->setText(QApplication::translate("Do_work_dialog", "Start_File", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Do_work_dialog: public Ui_Do_work_dialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DO_WORK_DIALOG_H
