/********************************************************************************
** Form generated from reading UI file 'ExitPage.ui'
**
** Created by: Qt User Interface Compiler version 6.8.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_EXITPAGE_H
#define UI_EXITPAGE_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ExitPage
{
public:
    QLabel *moneyLabel;
    QPushButton *payBtn;

    void setupUi(QWidget *ExitPage)
    {
        if (ExitPage->objectName().isEmpty())
            ExitPage->setObjectName("ExitPage");
        ExitPage->resize(207, 301);
        moneyLabel = new QLabel(ExitPage);
        moneyLabel->setObjectName("moneyLabel");
        moneyLabel->setGeometry(QRect(50, 100, 101, 41));
        payBtn = new QPushButton(ExitPage);
        payBtn->setObjectName("payBtn");
        payBtn->setGeometry(QRect(60, 220, 80, 18));

        retranslateUi(ExitPage);

        QMetaObject::connectSlotsByName(ExitPage);
    } // setupUi

    void retranslateUi(QWidget *ExitPage)
    {
        ExitPage->setWindowTitle(QCoreApplication::translate("ExitPage", "Form", nullptr));
        moneyLabel->setText(QCoreApplication::translate("ExitPage", "\345\272\224\344\273\230\351\207\221\351\242\235\357\274\232", nullptr));
        payBtn->setText(QCoreApplication::translate("ExitPage", "\346\224\257\344\273\230", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ExitPage: public Ui_ExitPage {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_EXITPAGE_H
