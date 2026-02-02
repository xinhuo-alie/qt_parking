#include "exitpage.h"
#include "ui_exitpage.h" // 解决 incomplete type 'Ui::ExitPage'

ExitPage::ExitPage(QWidget *parent) : QWidget(parent), ui(new Ui::ExitPage) {
    ui->setupUi(this);
}

ExitPage::~ExitPage() {
    delete ui;
}

void ExitPage::setInfo(QString plate, double money) {
    // 确保 UI 里的标签叫 moneyLabel
    ui->moneyLabel->setText(QString("车牌：%1\n应缴金额：%2元").arg(plate).arg(money));
}

void ExitPage::on_payBtn_clicked() {
    emit sigPayDone();
}
