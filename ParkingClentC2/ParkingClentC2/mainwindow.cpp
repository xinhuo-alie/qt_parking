#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "loginwidget.h"
#include "spaceselectionwidget.h"
#include "parkinginfowidget.h"
#include "paymentwidget.h"
#include "queuewidget.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_currentSpaceId(-1)
{
    ui->setupUi(this);
    setupUI();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setupUI()
{
    setWindowTitle("智能停车系统");
    resize(1200, 800);
    setMinimumSize(800, 600);
    
    // 设置主窗口背景渐变
    setStyleSheet(
        "QMainWindow {"
        "    background: qlineargradient(x1:0, y1:0, x2:1, y2:1,"
        "                stop:0 #667eea, stop:1 #764ba2);"
        "}"
        "QStatusBar {"
        "    background-color: rgba(255, 255, 255, 0.9);"
        "    color: #2c3e50;"
        "    font-family: 'Segoe UI', sans-serif;"
        "    font-size: 14px;"
        "}"
    );
    
    // 创建状态栏
    statusBar()->showMessage("欢迎使用智能停车系统");
    
    // 创建堆栈窗口 - 添加现代化样式
    m_stackedWidget = new QStackedWidget(this);
    m_stackedWidget->setStyleSheet(
        "QStackedWidget {"
        "    background: transparent;"
        "    border: none;"
        "}"
    );
    setCentralWidget(m_stackedWidget);
    
    // 创建各个功能模块
    m_loginWidget = new LoginWidget(this);
    m_spaceSelectionWidget = nullptr;
    m_parkingInfoWidget = nullptr;
    m_paymentWidget = nullptr;
    m_queueWidget = nullptr;
    
    // 添加到堆栈窗口
    m_stackedWidget->addWidget(m_loginWidget);
    
    // 连接信号槽
    connect(m_loginWidget, &LoginWidget::loginSuccess, 
            this, &MainWindow::onLoginSuccess);
    
    // 显示登录界面
    switchToWidget(m_loginWidget);
}

void MainWindow::switchToWidget(QWidget *widget)
{
    if (widget) {
        m_stackedWidget->setCurrentWidget(widget);
    }
}

void MainWindow::onLoginSuccess(const QString &plate)
{
    m_currentPlate = plate;
    
    // 创建停车位选择界面
    if (!m_spaceSelectionWidget) {
        m_spaceSelectionWidget = new SpaceSelectionWidget(m_currentPlate, this);
        m_stackedWidget->addWidget(m_spaceSelectionWidget);
        
        connect(m_spaceSelectionWidget, &SpaceSelectionWidget::spaceSelected,
                this, &MainWindow::onSpaceSelected);
        connect(m_spaceSelectionWidget, &SpaceSelectionWidget::queueRequested,
                this, &MainWindow::onQueueRequested);
    }
    
    switchToWidget(m_spaceSelectionWidget);
}

void MainWindow::onSpaceSelected(int spaceId)
{
    m_currentSpaceId = spaceId;
    
    // 创建停车信息界面
    if (!m_parkingInfoWidget) {
        m_parkingInfoWidget = new ParkingInfoWidget(m_currentPlate, m_currentSpaceId, this);
        m_stackedWidget->addWidget(m_parkingInfoWidget);
        
        connect(m_parkingInfoWidget, &ParkingInfoWidget::exitRequested,
                this, &MainWindow::onExitRequested);
    } else {
        // 更新空间ID
        delete m_parkingInfoWidget;
        m_parkingInfoWidget = new ParkingInfoWidget(m_currentPlate, m_currentSpaceId, this);
        m_stackedWidget->addWidget(m_parkingInfoWidget);
        
        connect(m_parkingInfoWidget, &ParkingInfoWidget::exitRequested,
                this, &MainWindow::onExitRequested);
    }
    
    switchToWidget(m_parkingInfoWidget);
}

void MainWindow::onExitRequested(int spaceId)
{
    // 创建支付界面
    if (!m_paymentWidget) {
        m_paymentWidget = new PaymentWidget(m_currentPlate, spaceId, this);
        m_stackedWidget->addWidget(m_paymentWidget);
        
        connect(m_paymentWidget, &PaymentWidget::paymentCompleted,
                this, &MainWindow::onPaymentCompleted);
    } else {
        // 更新空间ID
        delete m_paymentWidget;
        m_paymentWidget = new PaymentWidget(m_currentPlate, spaceId, this);
        m_stackedWidget->addWidget(m_paymentWidget);
        
        connect(m_paymentWidget, &PaymentWidget::paymentCompleted,
                this, &MainWindow::onPaymentCompleted);
    }
    
    switchToWidget(m_paymentWidget);
}

void MainWindow::onPaymentCompleted()
{
    // 支付完成后返回停车位选择界面
    switchToWidget(m_spaceSelectionWidget);
}

void MainWindow::onQueueRequested()
{
    // 创建排队界面
    if (!m_queueWidget) {
        m_queueWidget = new QueueWidget(m_currentPlate, this);
        m_stackedWidget->addWidget(m_queueWidget);
        
        connect(m_queueWidget, &QueueWidget::spaceAssigned,
                this, &MainWindow::onSpaceAssigned);
    } else {
        // 重新创建排队界面
        delete m_queueWidget;
        m_queueWidget = new QueueWidget(m_currentPlate, this);
        m_stackedWidget->addWidget(m_queueWidget);
        
        connect(m_queueWidget, &QueueWidget::spaceAssigned,
                this, &MainWindow::onSpaceAssigned);
    }
    
    switchToWidget(m_queueWidget);
}

void MainWindow::onSpaceAssigned(int spaceId)
{
    // 分配停车位后，显示停车信息界面
    onSpaceSelected(spaceId);
}