#include "registerpage.h"
#include "ui_registerpage.h" // 必须包含此行，解决 incomplete type 报错

RegisterPage::RegisterPage(QWidget *parent) : QWidget(parent), ui(new Ui::RegisterPage) {
    ui->setupUi(this);
}

RegisterPage::~RegisterPage() {
    delete ui;
}

void RegisterPage::on_regBtn_clicked() {
    QString plate = ui->plateEdit->text().trimmed(); // 确保 UI 里的输入框叫 plateEdit
    if(!plate.isEmpty()) emit sigStart(plate);
}
