#include "MainWindow.h"
#include "ui_MainWindow.h" // 极其重要：解决所有 ui->stackedWidget 报错

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);

    m_regPage = new RegisterPage(this);
    m_waitPage = new WaitingPage(this);
    m_exitPage = new ExitPage(this);

    // 清理并添加页面
    while(ui->stackedWidget->count() > 0)
        ui->stackedWidget->removeWidget(ui->stackedWidget->currentWidget());

    ui->stackedWidget->addWidget(m_regPage);  // Index 0
    ui->stackedWidget->addWidget(m_waitPage); // Index 1
    ui->stackedWidget->addWidget(m_exitPage); // Index 2

    m_controller = new TrafficControlCenter(ui->stackedWidget, this);

    connect(m_regPage, &RegisterPage::sigStart, [this](QString plate){
        m_controller->startMonitoring(plate);
    });

    connect(m_exitPage, &ExitPage::sigPayDone, [this](){
        ui->stackedWidget->setCurrentIndex(0);
    });
}

MainWindow::~MainWindow() {
    delete ui;
}
