/********************************************************************************
** Form generated from reading UI file 'WaitingPage.ui'
**
** Created by: Qt User Interface Compiler version 6.8.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_WAITINGPAGE_H
#define UI_WAITINGPAGE_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_WaitingPage
{
public:
    QLabel *statusLabel;

    void setupUi(QWidget *WaitingPage)
    {
        if (WaitingPage->objectName().isEmpty())
            WaitingPage->setObjectName("WaitingPage");
        WaitingPage->resize(400, 300);
        statusLabel = new QLabel(WaitingPage);
        statusLabel->setObjectName("statusLabel");
        statusLabel->setGeometry(QRect(150, 100, 191, 101));

        retranslateUi(WaitingPage);

        QMetaObject::connectSlotsByName(WaitingPage);
    } // setupUi

    void retranslateUi(QWidget *WaitingPage)
    {
        WaitingPage->setWindowTitle(QCoreApplication::translate("WaitingPage", "Form", nullptr));
        statusLabel->setText(QCoreApplication::translate("WaitingPage", "\346\216\222\351\230\237\344\270\255... ...", nullptr));
    } // retranslateUi

};

namespace Ui {
    class WaitingPage: public Ui_WaitingPage {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_WAITINGPAGE_H
