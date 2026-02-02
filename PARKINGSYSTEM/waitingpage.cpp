#include "waitingpage.h"
#include "ui_waitingpage.h" // 解决 ui_waitingpage.h not found 报错

WaitingPage::WaitingPage(QWidget *parent) : QWidget(parent), ui(new Ui::WaitingPage) {
    ui->setupUi(this);
}

WaitingPage::~WaitingPage() {
    delete ui;
}

void WaitingPage::setQueueLabel(int num) {
    // 确保 UI 里的标签叫 statusLabel
    ui->statusLabel->setText(QString("当前便道排队中，前方还有 %1 辆").arg(num));
}
