#include "queuewidget.h"

QueueWidget::QueueWidget(const QString &plate, QWidget *parent)
    : QWidget(parent)
    , m_currentPlate(plate)
    , m_queuePosition(0)
    , m_networkManager(new QNetworkAccessManager(this))
    , m_updateTimer(new QTimer(this))
{
    setupUI();
    
    connect(m_networkManager, &QNetworkAccessManager::finished, 
            this, &QueueWidget::onNetworkReply);
    connect(m_updateTimer, &QTimer::timeout, this, &QueueWidget::onQueueUpdate);
    
    // 加入排队
    joinQueue();
}

QueueWidget::~QueueWidget()
{
    if (m_updateTimer->isActive()) {
        m_updateTimer->stop();
    }
}

void QueueWidget::setupUI()
{
    setWindowTitle("排队信息");
    resize(500, 400);
    
    // 设置窗口背景渐变
    setStyleSheet(
        "QueueWidget {"
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
    titleIcon->setText("🕐");
    titleIcon->setStyleSheet("font-size: 32px; margin-right: 15px;");
    titleLayout->addWidget(titleIcon);
    
    m_titleLabel = new QLabel("排队信息", this);
    m_titleLabel->setAlignment(Qt::AlignCenter);
    m_titleLabel->setStyleSheet(
        "font-size: 32px; "
        "font-weight: 700; "
        "color: #2c3e50; "
        "font-family: 'Segoe UI', 'Helvetica Neue', sans-serif;"
    );
    titleLayout->addWidget(m_titleLabel);
    
    containerLayout->addLayout(titleLayout);
    
    // 状态信息卡片 - 更现代化的设计
    QWidget *statusCard = new QWidget(this);
    statusCard->setStyleSheet(
        "QWidget {"
        "    background-color: #ffffff;"
        "    border: 2px solid #ecf0f1;"
        "    border-radius: 20px;"
        "    padding: 35px;"
        "}"
    );
    
    QVBoxLayout *statusLayout = new QVBoxLayout(statusCard);
    statusLayout->setSpacing(25);
    
    // 排队状态 - 添加图标和更好的样式
    m_statusLabel = new QLabel("正在加入排队...", statusCard);
    m_statusLabel->setStyleSheet(
        "font-size: 20px; "
        "color: #2c3e50; "
        "font-family: 'Segoe UI', sans-serif; "
        "font-weight: 600; "
        "padding: 15px; "
        "background-color: rgba(52, 152, 219, 0.1); "
        "border-radius: 12px; "
        "text-align: center;"
    );
    m_statusLabel->setAlignment(Qt::AlignCenter);
    statusLayout->addWidget(m_statusLabel);
    
    // 排队位置
    m_positionLabel = new QLabel("📍 排队位置: -", statusCard);
    m_positionLabel->setStyleSheet(
        "font-size: 18px; "
        "color: #7f8c8d; "
        "font-family: 'Segoe UI', sans-serif;"
    );
    statusLayout->addWidget(m_positionLabel);
    
    // 排队时间
    m_timeLabel = new QLabel("⏰ 排队时间: -", statusCard);
    m_timeLabel->setStyleSheet(
        "font-size: 18px; "
        "color: #7f8c8d; "
        "font-family: 'Segoe UI', sans-serif;"
    );
    statusLayout->addWidget(m_timeLabel);
    
    // 提示信息
    m_infoLabel = new QLabel("请耐心等待，系统会为您分配停车位...", statusCard);
    m_infoLabel->setStyleSheet(
        "font-size: 16px; "
        "color: #95a5a6; "
        "font-family: 'Segoe UI', sans-serif; "
        "font-style: italic; "
        "text-align: center; "
        "padding: 15px; "
        "background-color: rgba(149, 165, 166, 0.1); "
        "border-radius: 10px;"
    );
    m_infoLabel->setAlignment(Qt::AlignCenter);
    statusLayout->addWidget(m_infoLabel);
    
    containerLayout->addWidget(statusCard);
    
    // 取消排队按钮 - 更现代化的渐变按钮
    // m_cancelButton = new QPushButton("取消排队", this);
    // m_cancelButton->setFixedHeight(55);
    // m_cancelButton->setStyleSheet(
    //     "QPushButton {"
    //     "    background: qlineargradient(x1:0, y1:0, x2:1, y2:0,"
    //     "                stop:0 #95a5a6, stop:1 #7f8c8d);"
    //     "    color: white;"
    //     "    border: none;"
    //     "    font-size: 18px;"
    //     "    font-weight: 600;"
    //     "    border-radius: 27px;"
    //     "    padding: 0 40px;"
    //     "    font-family: 'Segoe UI', sans-serif;"
    //     "    box-shadow: 0 4px 15px rgba(149, 165, 166, 0.3);"
    //     "}"
    //     "QPushButton:hover {"
    //     "    background: qlineargradient(x1:0, y1:0, x2:1, y2:0,"
    //     "                stop:0 #bdc3c7, stop:1 #95a5a6);"
    //     "    box-shadow: 0 6px 20px rgba(149, 165, 166, 0.4);"
    //     "}"
    //     "QPushButton:pressed {"
    //     "    background: qlineargradient(x1:0, y1:0, x2:1, y2:0,"
    //     "                stop:0 #7f8c8d, stop:1 #6c7b7d);"
    //     "    box-shadow: 0 2px 10px rgba(149, 165, 166, 0.2);"
    //     "}"
    // );
    // connect(m_cancelButton, &QPushButton::clicked, this, &QueueWidget::onCancelQueueClicked);
    // containerLayout->addWidget(m_cancelButton, 0, Qt::AlignCenter);
    
    mainLayout->addWidget(containerWidget);
    mainLayout->addStretch();
}

void QueueWidget::joinQueue()
{
    // 构建排队请求
    QJsonObject json;
    json["plate"] = m_currentPlate;
    
    QJsonDocument doc(json);
    QByteArray data = doc.toJson();
    
    QNetworkRequest request(QUrl("http://127.0.0.1:8080/api/queue/join"));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    
    m_networkManager->post(request, data);
}

void QueueWidget::onQueueUpdate()
{
    // 重新加入排队以获取最新信息
    joinQueue();
}

void QueueWidget::onNetworkReply(QNetworkReply *reply)
{
    if (reply->error() == QNetworkReply::NoError) {
        QByteArray response = reply->readAll();
        QJsonDocument doc = QJsonDocument::fromJson(response);
        QJsonObject obj = doc.object();
        
        if (obj["code"].toInt() == 0) {
            QJsonObject dataObj = obj["data"].toObject();
            QString msg = dataObj["msg"].toString();
            
            if (msg == "Space assigned directly") {
                // 直接分配了停车位
                QJsonObject spaceData = dataObj["data"].toObject();
                int spaceId = spaceData["spaceId"].toInt();
                
                m_statusLabel->setText("恭喜！已为您分配停车位");
                m_positionLabel->setText("停车位: " + spaceData["spaceId"].toString());
                m_timeLabel->setText("分配时间: " + QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"));
                m_infoLabel->setText("即将跳转到停车信息页面...");
                
                // 停止定时器
                if (m_updateTimer->isActive()) {
                    m_updateTimer->stop();
                }
                
                // 延迟2秒后发送信号
                QTimer::singleShot(4000, [this, spaceId]() {
                    emit spaceAssigned(spaceId);
                });
                
            } else {
                // 排队信息
                QJsonObject queueData = dataObj["data"].toObject();
                m_queuePosition = queueData["position"].toInt();
                m_queueTime = queueData["queueTime"].toString();
                
                m_statusLabel->setText("排队中...");
                m_positionLabel->setText(QString("排队位置: 第 %1 位").arg(m_queuePosition));
                m_timeLabel->setText(QString("排队时间: %1").arg(m_queueTime));
                m_infoLabel->setText("请耐心等待，系统会为您分配停车位...");
                
                // 启动定时器，每5秒更新一次
                if (!m_updateTimer->isActive()) {
                    m_updateTimer->start(5000);
                }
            }
        } else {
            QString errorMsg = obj["msg"].toString();
            showMessage("排队失败", errorMsg.isEmpty() ? "排队失败，请重试" : errorMsg, true);
        }
    } else {
        showMessage("网络错误", "网络连接失败，请检查网络设置", true);
    }
    
    reply->deleteLater();
}

void QueueWidget::onCancelQueueClicked()
{
    // 停止定时器
    if (m_updateTimer->isActive()) {
        m_updateTimer->stop();
    }
    
    showMessage("提示", "已取消排队");
    close();
}

void QueueWidget::showMessage(const QString &title, const QString &message, bool isError)
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