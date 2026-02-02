#include "paymentwidget.h"
#include <QNetworkRequest>

PaymentWidget::PaymentWidget(const QString &plate, int spaceId, QWidget *parent)
    : QWidget(parent)
    , m_currentPlate(plate)
    , m_spaceId(spaceId)
    , m_recordId(0)
    , m_fee(0.0)
    , m_networkManager(new QNetworkAccessManager(this))
{
    setupUI();
    
    connect(m_networkManager, &QNetworkAccessManager::finished, 
            this, &PaymentWidget::onNetworkReply);
    
    loadFeeInfo();
}

void PaymentWidget::setupUI()
{
    setWindowTitle("支付信息");
    resize(600, 500);
    
    // 设置窗口背景渐变
    setStyleSheet(
        "PaymentWidget {"
        "    background: qlineargradient(x1:0, y1:0, x2:1, y2:1,"
        "                stop:0 #667eea, stop:1 #764ba2);"
        "}"
    );
    
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(30);
    mainLayout->setContentsMargins(50, 50, 50, 50);
    
    // 创建主容器widget
    QWidget *containerWidget = new QWidget(this);
    containerWidget->setStyleSheet(
        "QWidget {"
        "    background-color: rgba(255, 255, 255, 0.95);"
        "    border-radius: 20px;"
        "    box-shadow: 0 8px 32px rgba(0, 0, 0, 0.1);"
        "}"
    );
    
    QVBoxLayout *containerLayout = new QVBoxLayout(containerWidget);
    containerLayout->setSpacing(30);
    containerLayout->setContentsMargins(40, 40, 40, 40);
    
    // 标题区域
    QHBoxLayout *titleLayout = new QHBoxLayout();
    titleLayout->setAlignment(Qt::AlignCenter);
    
    QLabel *titleIcon = new QLabel(this);
    titleIcon->setText("💳");
    titleIcon->setStyleSheet("font-size: 32px; margin-right: 15px;");
    titleLayout->addWidget(titleIcon);
    
    m_titleLabel = new QLabel("支付信息", this);
    m_titleLabel->setAlignment(Qt::AlignCenter);
    m_titleLabel->setStyleSheet(
        "font-size: 32px; "
        "font-weight: 700; "
        "color: #2c3e50; "
        "font-family: 'Segoe UI', 'Helvetica Neue', sans-serif;"
    );
    titleLayout->addWidget(m_titleLabel);
    
    containerLayout->addLayout(titleLayout);
    
    // 费用信息卡片 - 更现代化的设计
    QWidget *feeCard = new QWidget(this);
    feeCard->setStyleSheet(
        "QWidget {"
        "    background-color: #ffffff;"
        "    border: 2px solid #ecf0f1;"
        "    border-radius: 20px;"
        "    padding: 35px;"
        "}"
    );
    
    QVBoxLayout *feeLayout = new QVBoxLayout(feeCard);
    feeLayout->setSpacing(25);
    
    // 停车费用 - 更突出的显示
    m_feeLabel = new QLabel("停车费用: ¥0.00", feeCard);
    m_feeLabel->setStyleSheet(
        "font-size: 32px; "
        "font-weight: 700; "
        "color: #e74c3c; "
        "font-family: 'Segoe UI', sans-serif; "
        "text-align: center; "
        "padding: 15px; "
        "background-color: rgba(231, 76, 60, 0.1); "
        "border-radius: 15px;"
    );
    m_feeLabel->setAlignment(Qt::AlignCenter);
    feeLayout->addWidget(m_feeLabel);
    
    // 停车时长
    m_durationLabel = new QLabel("⏱️ 停车时长: 计算中...", feeCard);
    m_durationLabel->setStyleSheet(
        "font-size: 18px; "
        "color: #7f8c8d; "
        "font-family: 'Segoe UI', sans-serif;"
    );
    feeLayout->addWidget(m_durationLabel);
    
    // 入场时间
    m_enterTimeLabel = new QLabel("🚗 入场时间: 加载中...", feeCard);
    m_enterTimeLabel->setStyleSheet(
        "font-size: 18px; "
        "color: #7f8c8d; "
        "font-family: 'Segoe UI', sans-serif;"
    );
    feeLayout->addWidget(m_enterTimeLabel);
    
    // 出场时间
    m_exitTimeLabel = new QLabel("🚙 出场时间: 加载中...", feeCard);
    m_exitTimeLabel->setStyleSheet(
        "font-size: 18px; "
        "color: #7f8c8d; "
        "font-family: 'Segoe UI', sans-serif;"
    );
    feeLayout->addWidget(m_exitTimeLabel);
    
    containerLayout->addWidget(feeCard);
    
    // 支付方式选择区域
    QWidget *paymentContainer = new QWidget(this);
    paymentContainer->setStyleSheet(
        "QWidget {"
        "    background-color: #f8f9fa;"
        "    border-radius: 15px;"
        "    padding: 20px;"
        "}"
    );
    
    QVBoxLayout *paymentLayout = new QVBoxLayout(paymentContainer);
    paymentLayout->setSpacing(15);
    
    QLabel *paymentLabel = new QLabel("选择支付方式", this);
    paymentLabel->setStyleSheet(
        "font-size: 20px; "
        "color: #2c3e50; "
        "font-family: 'Segoe UI', sans-serif; "
        "font-weight: 600;"
    );
    paymentLayout->addWidget(paymentLabel);
    
    m_paymentMethodCombo = new QComboBox(this);
    m_paymentMethodCombo->addItems({"💵 现金", "💳 银行卡", "📱 移动支付", "🌐 在线支付"});
    m_paymentMethodCombo->setStyleSheet(
        "QComboBox {"
        "    padding: 15px 20px;"
        "    font-size: 16px;"
        "    border: 2px solid #ecf0f1;"
        "    border-radius: 12px;"
        "    background-color: #ffffff;"
        "    font-family: 'Segoe UI', sans-serif;"
        "    color: #2c3e50;"
        "}"
        "QComboBox:focus {"
        "    border-color: #3498db;"
        "    outline: none;"
        "}"
        "QComboBox::drop-down {"
        "    border: none;"
        "    padding-right: 15px;"
        "}"
        "QComboBox::down-arrow {"
        "    image: url();"
        "    width: 12px;"
        "    height: 12px;"
        "}"
    );
    paymentLayout->addWidget(m_paymentMethodCombo);
    
    containerLayout->addWidget(paymentContainer);
    
    // 支付按钮 - 更现代化的渐变按钮
    m_payButton = new QPushButton("确认支付", this);
    m_payButton->setFixedHeight(60);
    m_payButton->setStyleSheet(
        "QPushButton {"
        "    background: qlineargradient(x1:0, y1:0, x2:1, y2:0,"
        "                stop:0 #2ecc71, stop:1 #27ae60);"
        "    color: white;"
        "    border: none;"
        "    font-size: 20px;"
        "    font-weight: 600;"
        "    border-radius: 30px;"
        "    padding: 0 50px;"
        "    font-family: 'Segoe UI', sans-serif;"
        "    box-shadow: 0 4px 15px rgba(46, 204, 113, 0.3);"
        "}"
        "QPushButton:hover {"
        "    background: qlineargradient(x1:0, y1:0, x2:1, y2:0,"
        "                stop:0 #58d68d, stop:1 #2ecc71);"
        "    box-shadow: 0 6px 20px rgba(46, 204, 113, 0.4);"
        "}"
        "QPushButton:pressed {"
        "    background: qlineargradient(x1:0, y1:0, x2:1, y2:0,"
        "                stop:0 #27ae60, stop:1 #219653);"
        "    box-shadow: 0 2px 10px rgba(46, 204, 113, 0.2);"
        "}"
    );
    connect(m_payButton, &QPushButton::clicked, this, &PaymentWidget::onPayClicked);
    containerLayout->addWidget(m_payButton, 0, Qt::AlignCenter);
    
    mainLayout->addWidget(containerWidget);
    mainLayout->addStretch();
}

void PaymentWidget::loadFeeInfo()
{
    QString url = QString("http://127.0.0.1:8080/api/spaces/calculate-fee?spaceId=%1&plate=%2")
                  .arg(m_spaceId)
                  .arg(m_currentPlate);
    
    QNetworkRequest request = QNetworkRequest(QUrl(url));
    m_networkManager->get(request);
}

void PaymentWidget::onPayClicked()
{
    if (m_recordId == 0) {
        showMessage("错误", "无法获取停车记录，请重试", true);
        return;
    }
    
    // 获取支付方式
    QString paymentMethod;
    switch (m_paymentMethodCombo->currentIndex()) {
        case 0: paymentMethod = "cash"; break;
        case 1: paymentMethod = "card"; break;
        case 2: paymentMethod = "mobile"; break;
        case 3: paymentMethod = "online"; break;
        default: paymentMethod = "cash"; break;
    }
    
    // 禁用按钮防止重复点击
    m_payButton->setEnabled(false);
    m_payButton->setText("支付中...");
    
    // 构建支付请求
    QJsonObject json;
    json["recordId"] = m_recordId;
    json["paymentMethod"] = paymentMethod;
    
    QJsonDocument doc(json);
    QByteArray data = doc.toJson();
    
    QNetworkRequest request(QUrl("http://127.0.0.1:8080/api/payments/pay"));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    
    m_networkManager->post(request, data);
}

void PaymentWidget::onNetworkReply(QNetworkReply *reply)
{
    if (reply->error() == QNetworkReply::NoError) {
        QByteArray response = reply->readAll();
        QJsonDocument doc = QJsonDocument::fromJson(response);
        QJsonObject obj = doc.object();
        
        if (obj["code"].toInt() == 0) {
            QJsonObject dataObj = obj["data"].toObject();
            
            if (reply->url().toString().contains("calculate-fee")) {
                // 获取费用信息成功
                QJsonObject feeData = dataObj["data"].toObject();
                m_recordId = feeData["recordId"].toInt();
                m_fee = feeData["fee"].toDouble();
                QString enterTime = feeData["enterTime"].toString();
                QString exitTime = feeData["exitTime"].toString();
                
                // 更新显示
                m_feeLabel->setText(QString("停车费用: ¥%1").arg(m_fee, 0, 'f', 2));
                m_enterTimeLabel->setText(QString("入场时间: %1").arg(enterTime.isEmpty() ? "未知" : enterTime));
                m_exitTimeLabel->setText(QString("出场时间: %1").arg(exitTime.isEmpty() ? "未知" : exitTime));
                
                // 计算停车时长（简化计算）
                m_durationLabel->setText("停车时长: 已计算");
                
            } else if (reply->url().toString().contains("/payments/pay")) {
                // 支付成功
                QJsonObject paymentData = dataObj["data"].toObject();
                QString msg = dataObj["msg"].toString();
                
                showMessage("支付成功", msg.isEmpty() ? "支付成功！" : msg);
                emit paymentCompleted();
            }
        } else {
            QString errorMsg = obj["msg"].toString();
            showMessage("操作失败", errorMsg.isEmpty() ? "操作失败，请重试" : errorMsg, true);
            
            if (reply->url().toString().contains("/payments/pay")) {
                m_payButton->setEnabled(true);
                m_payButton->setText("确认支付");
            }
        }
    } else {
        showMessage("网络错误", "网络连接失败，请检查网络设置", true);
        if (reply->url().toString().contains("/payments/pay")) {
            m_payButton->setEnabled(true);
            m_payButton->setText("确认支付");
        }
    }
    
    reply->deleteLater();
}

void PaymentWidget::showMessage(const QString &title, const QString &message, bool isError)
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
            "    font-size: 14px; "
            "    font-family: 'Segoe UI', sans-serif;"
            "}"
            "QPushButton { "
            "    background-color: rgba(255, 255, 255, 0.2); "
            "    color: white; "
            "    border: 1px solid rgba(255, 255, 255, 0.3); "
            "    padding: 8px 20px; "
            "    border-radius: 20px; "
            "    font-family: 'Segoe UI', sans-serif; "
            "    font-weight: 500;"
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
            "    font-size: 14px; "
            "    font-family: 'Segoe UI', sans-serif;"
            "}"
            "QPushButton { "
            "    background-color: rgba(255, 255, 255, 0.2); "
            "    color: white; "
            "    border: 1px solid rgba(255, 255, 255, 0.3); "
            "    padding: 8px 20px; "
            "    border-radius: 20px; "
            "    font-family: 'Segoe UI', sans-serif; "
            "    font-weight: 500;"
            "}"
            "QPushButton:hover { "
            "    background-color: rgba(255, 255, 255, 0.3); "
            "}"
        );
    }
    
    msgBox.exec();
}