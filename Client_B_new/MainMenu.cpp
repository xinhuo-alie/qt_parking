#include "MainMenu.h"
#include "ui_MainMenu.h"
#include "mainwindow.h"
#include "ReportStatistics.h"
#include <QApplication>
#include <QStyleFactory>

MainMenu::MainMenu(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainMenu)
{
    ui->setupUi(this);
    initUI();
}

MainMenu::~MainMenu()
{
    delete ui;
}

void MainMenu::initUI()
{
    // 设置窗口标题
    this->setWindowTitle("停车场管理系统 - 主菜单");
    
    // 设置主窗口背景样式
    this->setStyleSheet("" 
        "QMainWindow { " 
        "    background: qlineargradient(x1:0, y1:0, x2:1, y2:1, stop:0 #f5f5f5, stop:1 #e0e0e0);" 
        "    color: #333333; " 
        "    font-family: 'Segoe UI', Tahoma, Geneva, Verdana, sans-serif; " 
        "}");
    
    // 设置系统名称标签样式
    ui->lblSystemName->setStyleSheet("" 
        "color: qlineargradient(x1:0, y1:0, x2:1, y2:0, stop:0 #3498db, stop:0.5 #2ecc71, stop:1 #e74c3c); " 
        "text-shadow: 2px 2px 4px rgba(0, 0, 0, 0.1); " 
        "margin-bottom: 10px;");
    
    // 确保按钮不继承任何默认样式
    ui->btnParkingManagement->setAttribute(Qt::WA_StyledBackground, true);
    ui->btnReportStatistics->setAttribute(Qt::WA_StyledBackground, true);
    
    // 直接清除可能的默认样式
    ui->btnParkingManagement->setStyleSheet("");
    ui->btnReportStatistics->setStyleSheet("");
    
    // 设置停车场管理按钮样式 - 使用更直接的方式
    QString parkingBtnStyle = 
        "QPushButton { " 
        "    color: white;" 
        "    font: bold 14px;" 
        "    border-radius: 12px;" 
        "    padding: 15px 30px;" 
        "    border: 2px solid #2980b9;" 
        "    box-shadow: 0 4px 15px rgba(0, 0, 0, 0.1);" 
        "    background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #3498db, stop:1 #2980b9);" 
        "    min-width: 300px;" 
        "    min-height: 50px;" 
        "}" 
        "QPushButton:hover { " 
        "    background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #4aa3df, stop:1 #3498db);" 
        "    box-shadow: 0 6px 20px rgba(52, 152, 219, 0.3);" 
        "    border-color: #3498db;" 
        "}" 
        "QPushButton:pressed { " 
        "    background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #2980b9, stop:1 #1f618d);" 
        "    box-shadow: 0 2px 10px rgba(0, 0, 0, 0.2);" 
        "    border-color: #1f618d;" 
        "}" 
        "QPushButton:disabled { " 
        "    background: #bdc3c7;" 
        "    border-color: #95a5a6;" 
        "    color: #7f8c8d;" 
        "    box-shadow: none;" 
        "}";
    
    // 设置报告统计按钮样式 - 使用更直接的方式
    QString reportBtnStyle = 
        "QPushButton { " 
        "    color: white;" 
        "    font: bold 14px;" 
        "    border-radius: 12px;" 
        "    padding: 15px 30px;" 
        "    border: 2px solid #27ae60;" 
        "    box-shadow: 0 4px 15px rgba(0, 0, 0, 0.1);" 
        "    background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #2ecc71, stop:1 #27ae60);" 
        "    min-width: 300px;" 
        "    min-height: 50px;" 
        "}" 
        "QPushButton:hover { " 
        "    background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #34da78, stop:1 #2ecc71);" 
        "    box-shadow: 0 6px 20px rgba(46, 204, 113, 0.3);" 
        "    border-color: #2ecc71;" 
        "}" 
        "QPushButton:pressed { " 
        "    background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #27ae60, stop:1 #229954);" 
        "    box-shadow: 0 2px 10px rgba(0, 0, 0, 0.2);" 
        "    border-color: #229954;" 
        "}" 
        "QPushButton:disabled { " 
        "    background: #bdc3c7;" 
        "    border-color: #95a5a6;" 
        "    color: #7f8c8d;" 
        "    box-shadow: none;" 
        "}";
    
    // 应用样式
    ui->btnParkingManagement->setStyleSheet(parkingBtnStyle);
    ui->btnReportStatistics->setStyleSheet(reportBtnStyle);
}

void MainMenu::on_btnParkingManagement_clicked()
{
    // 打开停车场管理界面
    MainWindow *mainWindow = new MainWindow();
    mainWindow->setAttribute(Qt::WA_DeleteOnClose);
    mainWindow->show();
}

void MainMenu::on_btnReportStatistics_clicked()
{
    // 打开报告统计界面
    ReportStatistics *reportStatistics = new ReportStatistics();
    reportStatistics->setAttribute(Qt::WA_DeleteOnClose);
    reportStatistics->show();
}