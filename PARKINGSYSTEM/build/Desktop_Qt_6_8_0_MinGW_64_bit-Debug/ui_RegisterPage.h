/********************************************************************************
** Form generated from reading UI file 'RegisterPage.ui'
**
** Created by: Qt User Interface Compiler version 6.8.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_REGISTERPAGE_H
#define UI_REGISTERPAGE_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_RegisterPage
{
public:
    QLineEdit *plateEdit;
    QPushButton *regBtn;

    void setupUi(QWidget *RegisterPage)
    {
        if (RegisterPage->objectName().isEmpty())
            RegisterPage->setObjectName("RegisterPage");
        RegisterPage->resize(194, 233);
        plateEdit = new QLineEdit(RegisterPage);
        plateEdit->setObjectName("plateEdit");
        plateEdit->setGeometry(QRect(20, 50, 151, 21));
        regBtn = new QPushButton(RegisterPage);
        regBtn->setObjectName("regBtn");
        regBtn->setGeometry(QRect(50, 140, 81, 20));

        retranslateUi(RegisterPage);

        QMetaObject::connectSlotsByName(RegisterPage);
    } // setupUi

    void retranslateUi(QWidget *RegisterPage)
    {
        RegisterPage->setWindowTitle(QCoreApplication::translate("RegisterPage", "Form", nullptr));
        plateEdit->setText(QCoreApplication::translate("RegisterPage", "\350\276\223\345\205\245\350\275\246\347\211\214\345\217\267\357\274\232", nullptr));
        regBtn->setText(QCoreApplication::translate("RegisterPage", "\346\263\250\345\206\214", nullptr));
    } // retranslateUi

};

namespace Ui {
    class RegisterPage: public Ui_RegisterPage {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_REGISTERPAGE_H
