#include "parkinginfowidget.h"

ParkingInfoWidget::ParkingInfoWidget(const QString &plate, int spaceId, QWidget *parent)
    : QWidget(parent)
    , m_currentPlate(plate)
    , m_spaceId(spaceId)
    , m_networkManager(new QNetworkAccessManager(this))
{
    setupUI();
    
    connect(m_networkManager, &QNetworkAccessManager::finished, 
            this, &ParkingInfoWidget::onNetworkReply);
    
    loadSpaceInfo();
}

void ParkingInfoWidget::setupUI()
{
    setWindowTitle("停车信息");
    resize(600, 500);
    
    // 设置窗口背景渐变
    setStyleSheet(
        "ParkingInfoWidget {"
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
    titleIcon->setText("🚗");
    titleIcon->setStyleSheet("font-size: 32px; margin-right: 15px;");
    titleLayout->addWidget(titleIcon);
    
    m_titleLabel = new QLabel("停车信息", this);
    m_titleLabel->setAlignment(Qt::AlignCenter);
    m_titleLabel->setStyleSheet(
        "font-size: 32px; "
        "font-weight: 700; "
        "color: #2c3e50; "
        "font-family: 'Segoe UI', 'Helvetica Neue', sans-serif;"
    );
    titleLayout->addWidget(m_titleLabel);
    
    containerLayout->addLayout(titleLayout);
    
    // 信息卡片 - 更现代化的设计
    QWidget *infoCard = new QWidget(this);
    infoCard->setStyleSheet(
        "QWidget {"
        "    background-color: #ffffff;"
        "    border: 2px solid #ecf0f1;"
        "    border-radius: 20px;"
        "    padding: 35px;"
        "}"
    );
    
    QVBoxLayout *cardLayout = new QVBoxLayout(infoCard);
    cardLayout->setSpacing(25);
    
    // 停车位位置 - 添加图标和更好的样式
    m_locationLabel = new QLabel("📍 停车位位置: 加载中...", infoCard);
    m_locationLabel->setStyleSheet(
        "font-size: 18px; "
        "color: #2c3e50; "
        "font-family: 'Segoe UI', sans-serif; "
        "padding: 10px; "
        "background-color: rgba(52, 152, 219, 0.1); "
        "border-radius: 10px;"
    );
    cardLayout->addWidget(m_locationLabel);
    
    // 停车位类型
    m_typeLabel = new QLabel("🅿️ 停车位类型: 加载中...", infoCard);
    m_typeLabel->setStyleSheet(
        "font-size: 18px; "
        "color: #2c3e50; "
        "font-family: 'Segoe UI', sans-serif; "
        "padding: 10px; "
        "background-color: rgba(46, 204, 113, 0.1); "
        "border-radius: 10px;"
    );
    cardLayout->addWidget(m_typeLabel);
    
    // 收费标准
    m_rateLabel = new QLabel("💰 收费标准: 加载中...", infoCard);
    m_rateLabel->setStyleSheet(
        "font-size: 18px; "
        "color: #2c3e50; "
        "font-family: 'Segoe UI', sans-serif; "
        "padding: 10px; "
        "background-color: rgba(241, 196, 15, 0.1); "
        "border-radius: 10px;"
    );
    cardLayout->addWidget(m_rateLabel);
    
    // 占用时间
    m_occupiedTimeLabel = new QLabel("⏰ 占用时间: 加载中...", infoCard);
    m_occupiedTimeLabel->setStyleSheet(
        "font-size: 18px; "
        "color: #2c3e50; "
        "font-family: 'Segoe UI', sans-serif; "
        "padding: 10px; "
        "background-color: rgba(155, 89, 182, 0.1); "
        "border-radius: 10px;"
    );
    cardLayout->addWidget(m_occupiedTimeLabel);
    
    // 停车时长
    m_durationLabel = new QLabel("🕐 停车时长: 计算中...", infoCard);
    m_durationLabel->setStyleSheet(
        "font-size: 18px; "
        "color: #2c3e50; "
        "font-family: 'Segoe UI', sans-serif; "
        "padding: 10px; "
        "background-color: rgba(230, 126, 34, 0.1); "
        "border-radius: 10px;"
    );
    cardLayout->addWidget(m_durationLabel);
    
    containerLayout->addWidget(infoCard);
    
    // 出场按钮 - 更现代化的渐变按钮
    m_exitButton = new QPushButton("🚗 出场", this);
    m_exitButton->setFixedHeight(60);
    m_exitButton->setStyleSheet(
        "QPushButton {"
        "    background: qlineargradient(x1:0, y1:0, x2:1, y2:0,"
        "                stop:0 #e74c3c, stop:1 #c0392b);"
        "    color: white;"
        "    border: none;"
        "    font-size: 20px;"
        "    font-weight: 600;"
        "    border-radius: 30px;"
        "    padding: 0 50px;"
        "    font-family: 'Segoe UI', sans-serif;"
        "    box-shadow: 0 4px 15px rgba(231, 76, 60, 0.3);"
        "}"
        "QPushButton:hover {"
        "    background: qlineargradient(x1:0, y1:0, x2:1, y2:0,"
        "                stop:0 #ec7063, stop:1 #e74c3c);"
        "    box-shadow: 0 6px 20px rgba(231, 76, 60, 0.4);"
        "}"
        "QPushButton:pressed {"
        "    background: qlineargradient(x1:0, y1:0, x2:1, y2:0,"
        "                stop:0 #c0392b, stop:1 #a93226);"
        "    box-shadow: 0 2px 10px rgba(231, 76, 60, 0.2);"
        "}"
    );
    connect(m_exitButton, &QPushButton::clicked, this, &ParkingInfoWidget::onExitClicked);
    containerLayout->addWidget(m_exitButton, 0, Qt::AlignCenter);
    
    mainLayout->addWidget(containerWidget);
    mainLayout->addStretch();
}

void ParkingInfoWidget::loadSpaceInfo()
{
    QNetworkRequest request(QUrl(QString("http://127.0.0.1:8080/api/spaces/%1").arg(m_spaceId)));
    m_networkManager->get(request);
}

void ParkingInfoWidget::onExitClicked()
{
    m_exitButton->setEnabled(false);
    m_exitButton->setText("处理中...");
    
    QNetworkRequest request(QUrl(QString("http://127.0.0.1:8080/api/spaces/%1/release").arg(m_spaceId)));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    
    QJsonObject json;
    QJsonDocument doc(json);
    QByteArray data = doc.toJson();
    
    m_networkManager->post(request, data);
}

void ParkingInfoWidget::onNetworkReply(QNetworkReply *reply)
{
    if (reply->error() == QNetworkReply::NoError) {
        QByteArray response = reply->readAll();
        QJsonDocument doc = QJsonDocument::fromJson(response);
        QJsonObject obj = doc.object();
        
        if (obj["code"].toInt() == 0) {
            QJsonObject dataObj = obj["data"].toObject();
            QJsonObject spaceData = dataObj["data"].toObject();
            
            if (reply->url().toString().contains("/release")) {
                // 释放停车位成功
                QString msg = dataObj["msg"].toString();
                showMessage("成功", msg.isEmpty() ? "出场成功！" : msg);
                emit exitRequested(m_spaceId);
            } else {
                // 获取停车位信息成功
                QString location = spaceData["location"].toString();
                QString type = spaceData["type"].toString();
                double hourlyRate = spaceData["hourlyRate"].toDouble();
                QString occupiedTime = spaceData["occupiedTime"].toString();
                
                m_locationLabel->setText(QString("停车位位置: %1").arg(location));
                m_typeLabel->setText(QString("停车位类型: %1").arg(type));
                m_rateLabel->setText(QString("收费标准: ¥%1/小时").arg(hourlyRate));
                m_occupiedTimeLabel->setText(QString("占用时间: %1").arg(occupiedTime.isEmpty() ? "未知" : occupiedTime));
                
                // 计算停车时长（简化计算，实际应该解析时间）
                if (!occupiedTime.isEmpty()) {
                    m_durationLabel->setText("停车时长: 计算中...");
                } else {
                    m_durationLabel->setText("停车时长: 未知");
                }
            }
        } else {
            QString errorMsg = obj["msg"].toString();
            showMessage("操作失败", errorMsg.isEmpty() ? "操作失败，请重试" : errorMsg, true);
            
            if (reply->url().toString().contains("/release")) {
                m_exitButton->setEnabled(true);
                m_exitButton->setText("出场");
            }
        }
    } else {
        showMessage("网络错误", "网络连接失败，请检查网络设置", true);
        if (reply->url().toString().contains("/release")) {
            m_exitButton->setEnabled(true);
            m_exitButton->setText("出场");
        }
    }
    
    reply->deleteLater();
}

void ParkingInfoWidget::showMessage(const QString &title, const QString &message, bool isError)
{
    QMessageBox msgBox(this);
    msgBox.setWindowTitle(title);
    msgBox.setText(message);
    
    if (isError) {
        msgBox.setIcon(QMessageBox::Critical);
        msgBox.setStyleSheet(
            "QMessageBox { background-color: #ffffff; }"
            "QPushButton { background-color: #e74c3c; color: white; border: none; padding: 8px 16px; border-radius: 4px; }"
        );
    } else {
        msgBox.setIcon(QMessageBox::Information);
        msgBox.setStyleSheet(
            "QMessageBox { background-color: #ffffff; }"
            "QPushButton { background-color: #27ae60; color: white; border: none; padding: 8px 16px; border-radius: 4px; }"
        );
    }
    
    msgBox.exec();
}