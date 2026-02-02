#include "loginwidget.h"

LoginWidget::LoginWidget(QWidget *parent)
    : QWidget(parent)
    , m_networkManager(new QNetworkAccessManager(this))
{
    setupUI();
    connect(m_networkManager, &QNetworkAccessManager::finished, 
            this, &LoginWidget::onNetworkReply);
}

void LoginWidget::setupUI()
{
    setWindowTitle("智能停车系统");
    resize(800, 600);  // 增大默认尺寸
    setMinimumSize(600, 500);  // 增大最小尺寸
    
    // 设置窗口背景渐变
    setStyleSheet(
        "LoginWidget {"
        "    background: qlineargradient(x1:0, y1:0, x2:1, y2:1,"
        "                stop:0 #667eea, stop:1 #764ba2);"
        "}"
    );
    
    // 创建主布局，使用相对位置和拉伸
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(50, 50, 50, 50);  // 相对边距
    mainLayout->setSpacing(30);
    
    // 添加顶部拉伸区域
    mainLayout->addStretch(1);
    
    // 创建内容容器 - 使用相对大小而不是固定大小
    QWidget *contentWidget = new QWidget(this);
    contentWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    contentWidget->setMaximumWidth(700);  // 限制最大宽度
    contentWidget->setStyleSheet(
        "QWidget {"
        "    background-color: rgba(255, 255, 255, 0.95);"
        "    border-radius: 25px;"
        "    padding: 40px;"
        "}"
    );
    
    mainLayout->addWidget(contentWidget, 0, Qt::AlignCenter);
    
    // 添加底部拉伸区域
    mainLayout->addStretch(1);
    
    // 内容布局 - 使用垂直布局让元素自动分布
    QVBoxLayout *contentLayout = new QVBoxLayout(contentWidget);
    contentLayout->setSpacing(25);
    contentLayout->setAlignment(Qt::AlignCenter);
    
    // 标题区域 - 增大图标和字体
    QHBoxLayout *titleLayout = new QHBoxLayout();
    titleLayout->setAlignment(Qt::AlignCenter);
    
    QLabel *iconLabel = new QLabel(this);
    iconLabel->setText("🚗");
    iconLabel->setStyleSheet("font-size: 64px; margin-right: 20px;");  // 大幅增大图标
    titleLayout->addWidget(iconLabel);
    
    QLabel *titleLabel = new QLabel("智能停车系统", this);
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setStyleSheet(
        "font-size: 48px; "  // 大幅增大字体
        "font-weight: 700; "
        "color: #2c3e50; "
        "font-family: 'Segoe UI', 'Helvetica Neue', sans-serif;"
    );
    titleLayout->addWidget(titleLabel);
    
    contentLayout->addLayout(titleLayout);
    
    // 副标题
    QLabel *subtitleLabel = new QLabel("欢迎使用智能停车服务", this);
    subtitleLabel->setAlignment(Qt::AlignCenter);
    subtitleLabel->setStyleSheet(
        "font-size: 24px; "  // 增大字体
        "color: #7f8c8d; "
        "font-family: 'Segoe UI', sans-serif; "
        "margin-bottom: 30px;"
    );
    contentLayout->addWidget(subtitleLabel);
    
    // 输入框容器 - 大幅增大尺寸
    QWidget *inputContainer = new QWidget(contentWidget);
    inputContainer->setStyleSheet(
        "QWidget {"
        "    background-color: #ffffff;"
        "    border: 2px solid #ecf0f1;"
        "    border-radius: 20px;"  // 增大圆角
        "    padding: 15px;"
        "}"
        "QWidget:focus {"
        "    border-color: #3498db;"
        "}"
    );
    inputContainer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    inputContainer->setMinimumHeight(80);  // 大幅增大高度
    inputContainer->setMaximumHeight(100);
    
    QHBoxLayout *inputLayout = new QHBoxLayout(inputContainer);
    inputLayout->setContentsMargins(25, 20, 25, 20);  // 增大内边距
    inputLayout->setSpacing(20);
    
    // 车牌图标
    QLabel *plateIcon = new QLabel(inputContainer);
    plateIcon->setText("🚙");
    plateIcon->setStyleSheet("font-size: 36px; color: #95a5a6;");  // 增大图标
    plateIcon->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    inputLayout->addWidget(plateIcon);
    
    // 车牌输入 - 大幅增大字体和输入框
    m_plateInput = new QLineEdit(inputContainer);
    m_plateInput->setPlaceholderText("请输入车牌号，如：京A00001");
    m_plateInput->setClearButtonEnabled(true);
    m_plateInput->setStyleSheet(
        "QLineEdit {"
        "    border: none;"
        "    font-size: 24px; "  // 大幅增大字体
        "    padding: 15px;"     // 大幅增大内边距
        "    color: #2c3e50;"
        "    font-family: 'Segoe UI', sans-serif;"
        "    background: transparent;"
        "}"
        "QLineEdit:focus {"
        "    outline: none;"
        "}"
        "QLineEdit::placeholder {"
        "    color: #bdc3c7;"
        "    font-size: 20px; "  // 增大占位符字体
        "}"
    );
    m_plateInput->setFocusPolicy(Qt::StrongFocus);
    m_plateInput->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    inputLayout->addWidget(m_plateInput);
    
    contentLayout->addWidget(inputContainer);
    
    // 提示文字
    QLabel *tipLabel = new QLabel("输入您的车牌号即可开始使用", this);
    tipLabel->setAlignment(Qt::AlignCenter);
    tipLabel->setStyleSheet(
        "font-size: 18px; "  // 增大字体
        "color: #95a5a6; "
        "font-family: 'Segoe UI', sans-serif;"
    );
    contentLayout->addWidget(tipLabel);
    
    // 添加拉伸区域，让按钮靠下
    contentLayout->addStretch();
    
    // 登录按钮 - 大幅增大尺寸
    m_loginButton = new QPushButton("开始体验", this);
    m_loginButton->setFixedHeight(90);  // 大幅增大按钮高度
    m_loginButton->setMinimumWidth(250);  // 设置最小宽度
    m_loginButton->setStyleSheet(
        "QPushButton {"
        "    background: qlineargradient(x1:0, y1:0, x2:1, y2:0,"
        "                stop:0 #3498db, stop:1 #2980b9);"
        "    color: white;"
        "    border: none;"
        "    font-size: 28px; "  // 大幅增大字体
        "    font-weight: 600;"
        "    border-radius: 45px;"  // 调整圆角
        "    padding: 0 80px;"     // 增大内边距
        "    font-family: 'Segoe UI', sans-serif;"
        "}"
        "QPushButton:hover {"
        "    background: qlineargradient(x1:0, y1:0, x2:1, y2:0,"
        "                stop:0 #5dade2, stop:1 #3498db);"
        "}"
        "QPushButton:pressed {"
        "    background: qlineargradient(x1:0, y1:0, x2:1, y2:0,"
        "                stop:0 #2980b9, stop:1 #21618c);"
        "}"
    );
    connect(m_loginButton, &QPushButton::clicked, this, &LoginWidget::onLoginClicked);
    contentLayout->addWidget(m_loginButton, 0, Qt::AlignCenter);
    
    // 保存容器引用用于resize事件
    m_containerWidget = contentWidget;
}

void LoginWidget::onLoginClicked()
{
    QString plate = m_plateInput->text().trimmed();
    if (plate.isEmpty()) {
        showMessage("输入错误", "请输入车牌号", true);
        return;
    }
    
    // 禁用按钮防止重复点击
    m_loginButton->setEnabled(false);
    m_loginButton->setText("登录中...");
    
    // 构建请求
    QJsonObject json;
    json["plate"] = plate;
    
    QJsonDocument doc(json);
    QByteArray data = doc.toJson();
    
    QNetworkRequest request(QUrl("http://127.0.0.1:8080/api/cars"));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    
    m_networkManager->post(request, data);
}

void LoginWidget::onNetworkReply(QNetworkReply *reply)
{
    m_loginButton->setEnabled(true);
    m_loginButton->setText("开始体验");
    
    if (reply->error() == QNetworkReply::NoError) {
        QByteArray response = reply->readAll();
        QJsonDocument doc = QJsonDocument::fromJson(response);
        QJsonObject obj = doc.object();
        
        if (obj["code"].toInt() == 0) {
            // 登录成功，获取车牌号
            QString plate = m_plateInput->text().trimmed();
            showMessage("成功", "登录成功！");
            emit loginSuccess(plate);
        } else {
            QString errorMsg = obj["msg"].toString();
            showMessage("登录失败", errorMsg.isEmpty() ? "登录失败，请重试" : errorMsg, true);
        }
    } else {
        showMessage("网络错误", "网络连接失败，请检查网络设置", true);
    }
    
    reply->deleteLater();
}

void LoginWidget::showMessage(const QString &title, const QString &message, bool isError)
{
    QMessageBox msgBox(this);
    msgBox.setWindowTitle(title);
    msgBox.setText(message);
    msgBox.setWindowFlags(msgBox.windowFlags() | Qt::FramelessWindowHint);
    
    if (isError) {
        msgBox.setIcon(QMessageBox::Critical);
        msgBox.setStyleSheet(
            "QMessageBox { "
            "    background: qlineargradient(x1:0, y1:0, x2:1, y2:1,"
            "                stop:0 #ff6b6b, stop:1 #ee5a52);"
            "    border-radius: 15px;"
            "    padding: 20px;"
            "}"
            "QLabel { "
            "    color: white; "
            "    font-size: 18px; "  // 增大字体
            "    font-family: 'Segoe UI', sans-serif;"
            "}"
            "QPushButton { "
            "    background-color: rgba(255, 255, 255, 0.2); "
            "    color: white; "
            "    border: 1px solid rgba(255, 255, 255, 0.3); "
            "    padding: 12px 30px; "  // 增大内边距
            "    border-radius: 25px; "
            "    font-family: 'Segoe UI', sans-serif; "
            "    font-weight: 500;"
            "    font-size: 16px; "  // 增大按钮字体
            "}"
            "QPushButton:hover { "
            "    background-color: rgba(255, 255, 255, 0.3); "
            "}"
        );
    } else {
        msgBox.setIcon(QMessageBox::Information);
        msgBox.setStyleSheet(
            "QMessageBox { "
            "    background: qlineargradient(x1:0, y1:0, x2:1, y2:1,"
            "                stop:0 #2ecc71, stop:1 #27ae60);"
            "    border-radius: 15px;"
            "    padding: 20px;"
            "}"
            "QLabel { "
            "    color: white; "
            "    font-size: 18px; "  // 增大字体
            "    font-family: 'Segoe UI', sans-serif;"
            "}"
            "QPushButton { "
            "    background-color: rgba(255, 255, 255, 0.2); "
            "    color: white; "
            "    border: 1px solid rgba(255, 255, 255, 0.3); "
            "    padding: 12px 30px; "  // 增大内边距
            "    border-radius: 25px; "
            "    font-family: 'Segoe UI', sans-serif; "
            "    font-weight: 500;"
            "    font-size: 16px; "  // 增大按钮字体
            "}"
            "QPushButton:hover { "
            "    background-color: rgba(255, 255, 255, 0.3); "
            "}"
        );
    }
    
    msgBox.exec();
}

void LoginWidget::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    // 容器使用布局管理，不需要手动居中
}