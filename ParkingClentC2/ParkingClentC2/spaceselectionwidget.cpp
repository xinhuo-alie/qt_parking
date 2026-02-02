#include "spaceselectionwidget.h"

SpaceSelectionWidget::SpaceSelectionWidget(const QString &plate, QWidget *parent)
    : QWidget(parent)
    , m_currentPlate(plate)
    , m_networkManager(new QNetworkAccessManager(this))
    , m_refreshTimer(new QTimer(this))
    , m_selectedSpaceId(-1)
{
    setupUI();
    
    connect(m_networkManager, &QNetworkAccessManager::finished, 
            this, &SpaceSelectionWidget::onNetworkReply);
    connect(m_refreshTimer, &QTimer::timeout, this, &SpaceSelectionWidget::onRefreshSpaces);
    
    // 立即刷新一次，然后每5秒刷新一次
    onRefreshSpaces();
    m_refreshTimer->start(5000);
}

SpaceSelectionWidget::~SpaceSelectionWidget()
{
    if (m_refreshTimer->isActive()) {
        m_refreshTimer->stop();
    }
}

void SpaceSelectionWidget::setupUI()
{
    setWindowTitle("选择停车位");
    resize(1000, 700);
    
    // 设置窗口背景渐变
    setStyleSheet(
        "SpaceSelectionWidget {"
        "    background: qlineargradient(x1:0, y1:0, x2:1, y2:1,"
        "                stop:0 #667eea, stop:1 #764ba2);"
        "}"
    );
    
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(25);
    mainLayout->setContentsMargins(40, 40, 40, 40);
    
    // 创建主容器widget，用于现代化布局
    QWidget *containerWidget = new QWidget(this);
    containerWidget->setStyleSheet(
        "QWidget {"
        "    background-color: rgba(255, 255, 255, 0.95);"
        "    border-radius: 20px;"
        "    box-shadow: 0 8px 32px rgba(0, 0, 0, 0.1);"
        "}"
    );
    
    QVBoxLayout *containerLayout = new QVBoxLayout(containerWidget);
    containerLayout->setSpacing(25);
    containerLayout->setContentsMargins(40, 40, 40, 40);
    
    // 标题和状态区域
    QHBoxLayout *headerLayout = new QHBoxLayout();
    headerLayout->setAlignment(Qt::AlignCenter);
    
    // 标题图标
    QLabel *titleIcon = new QLabel(this);
    titleIcon->setText("🅿️");
    titleIcon->setStyleSheet("font-size: 36px; margin-right: 15px;");
    headerLayout->addWidget(titleIcon);
    
    // 标题
    QLabel *titleLabel = new QLabel("选择停车位", this);
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setStyleSheet(
        "font-size: 32px; "
        "font-weight: 700; "
        "color: #2c3e50; "
        "font-family: 'Segoe UI', 'Helvetica Neue', sans-serif;"
    );
    headerLayout->addWidget(titleLabel);
    
    containerLayout->addLayout(headerLayout);
    
    // 状态标签 - 更现代化的设计
    m_statusLabel = new QLabel(QString("当前车牌: %1").arg(m_currentPlate), this);
    m_statusLabel->setAlignment(Qt::AlignCenter);
    m_statusLabel->setStyleSheet(
        "font-size: 18px; "
        "color: #7f8c8d; "
        "font-family: 'Segoe UI', sans-serif; "
        "padding: 15px; "
        "background-color: rgba(127, 140, 141, 0.1); "
        "border-radius: 12px; "
        "margin-bottom: 10px;"
    );
    containerLayout->addWidget(m_statusLabel);
    
    // 停车位显示区域 - 更现代化的滚动区域
    m_scrollArea = new QScrollArea(this);
    m_scrollArea->setWidgetResizable(true);
    m_scrollArea->setStyleSheet(
        "QScrollArea {"
        "    border: none;"
        "    border-radius: 15px;"
        "    background-color: rgba(236, 240, 241, 0.5);"
        "}"
        "QScrollBar:vertical {"
        "    border: none;"
        "    background: rgba(255, 255, 255, 0.3);"
        "    width: 8px;"
        "    border-radius: 4px;"
        "}"
        "QScrollBar::handle:vertical {"
        "    background: rgba(127, 140, 141, 0.5);"
        "    border-radius: 4px;"
        "    min-height: 20px;"
        "}"
        "QScrollBar::handle:vertical:hover {"
        "    background: rgba(127, 140, 141, 0.7);"
        "}"
    );
    
    m_spacesContainer = new QWidget();
    m_spacesLayout = new QGridLayout(m_spacesContainer);
    m_spacesLayout->setSpacing(20);
    m_spacesLayout->setContentsMargins(25, 25, 25, 25);
    
    m_scrollArea->setWidget(m_spacesContainer);
    containerLayout->addWidget(m_scrollArea);
    
    // 排队按钮 - 更现代化的渐变按钮
    m_queueButton = new QPushButton("加入排队", this);
    m_queueButton->setFixedHeight(55);
    m_queueButton->setStyleSheet(
        "QPushButton {"
        "    background: qlineargradient(x1:0, y1:0, x2:1, y2:0,"
        "                stop:0 #f39c12, stop:1 #e67e22);"
        "    color: white;"
        "    border: none;"
        "    font-size: 18px;"
        "    font-weight: 600;"
        "    border-radius: 27px;"
        "    padding: 0 40px;"
        "    font-family: 'Segoe UI', sans-serif;"
        "    box-shadow: 0 4px 15px rgba(243, 156, 18, 0.3);"
        "}"
        "QPushButton:hover {"
        "    background: qlineargradient(x1:0, y1:0, x2:1, y2:0,"
        "                stop:0 #f1c40f, stop:1 #f39c12);"
        "    box-shadow: 0 6px 20px rgba(243, 156, 18, 0.4);"
        "}"
        "QPushButton:pressed {"
        "    background: qlineargradient(x1:0, y1:0, x2:1, y2:0,"
        "                stop:0 #e67e22, stop:1 #d35400);"
        "    box-shadow: 0 2px 10px rgba(243, 156, 18, 0.2);"
        "}"
    );
    connect(m_queueButton, &QPushButton::clicked, this, &SpaceSelectionWidget::onQueueButtonClicked);
    containerLayout->addWidget(m_queueButton, 0, Qt::AlignCenter);
    
    mainLayout->addWidget(containerWidget);
}

void SpaceSelectionWidget::onRefreshSpaces()
{
    QNetworkRequest request(QUrl("http://127.0.0.1:8080/api/spaces"));
    m_networkManager->get(request);
}

void SpaceSelectionWidget::onNetworkReply(QNetworkReply *reply)
{
    if (reply->error() == QNetworkReply::NoError) {
        QByteArray response = reply->readAll();
        QJsonDocument doc = QJsonDocument::fromJson(response);
        QJsonObject obj = doc.object();
        
        if (obj["code"].toInt() == 0) {
            QJsonObject dataObj = obj["data"].toObject();
            QJsonArray spacesArray = dataObj["data"].toArray();
            updateSpacesDisplay(spacesArray);
        } else {
            QString errorMsg = obj["msg"].toString();
            showMessage("获取失败", errorMsg.isEmpty() ? "获取停车位信息失败" : errorMsg, true);
        }
    } else {
        showMessage("网络错误", "网络连接失败，请检查网络设置", true);
    }
    
    reply->deleteLater();
}

void SpaceSelectionWidget::updateSpacesDisplay(const QJsonArray &spaces)
{
    // 清除现有按钮
    QLayoutItem *child;
    while ((child = m_spacesLayout->takeAt(0)) != nullptr) {
        delete child->widget();
        delete child;
    }
    
    int availableCount = 0;
    int row = 0, col = 0;
    const int columns = 4; // 每行4个停车位
    
    for (int i = 0; i < spaces.size(); ++i) {
        QJsonObject space = spaces[i].toObject();
        int id = space["id"].toInt();
        QString location = space["location"].toString();
        QString status = space["status"].toString();
        QString currentPlate = space["currentPlate"].toString();
        double hourlyRate = space["hourlyRate"].toDouble();
        
        QPushButton *spaceButton = new QPushButton(this);
        spaceButton->setProperty("spaceId", id);
        
        QString buttonText = QString("%1\n¥%2/小时").arg(location).arg(hourlyRate);
        spaceButton->setText(buttonText);
        spaceButton->setFixedSize(180, 120);
        
        // 根据状态设置样式 - 更现代化的设计
        QString styleSheet;
        if (status == "available") {
            styleSheet = 
                "QPushButton {"
                "    background: qlineargradient(x1:0, y1:0, x2:1, y2:0,"
                "                stop:0 #2ecc71, stop:1 #27ae60);"
                "    color: white;"
                "    border: none;"
                "    font-weight: 600;"
                "    font-size: 16px;"
                "    font-family: 'Segoe UI', sans-serif;"
                "    border-radius: 15px;"
                "    cursor: pointer;"
                "    box-shadow: 0 4px 15px rgba(46, 204, 113, 0.3);"
                "    padding: 10px;"
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
                "}";
            availableCount++;
            connect(spaceButton, &QPushButton::clicked, this, &SpaceSelectionWidget::onAvailableSpaceClicked);
        } else if (currentPlate == m_currentPlate) {
            styleSheet = 
                "QPushButton {"
                "    background: qlineargradient(x1:0, y1:0, x2:1, y2:0,"
                "                stop:0 #f39c12, stop:1 #e67e22);"
                "    color: white;"
                "    border: none;"
                "    font-weight: 600;"
                "    font-size: 16px;"
                "    font-family: 'Segoe UI', sans-serif;"
                "    border-radius: 15px;"
                "    cursor: pointer;"
                "    box-shadow: 0 4px 15px rgba(243, 156, 18, 0.3);"
                "    padding: 10px;"
                "}"
                "QPushButton:hover {"
                "    background: qlineargradient(x1:0, y1:0, x2:1, y2:0,"
                "                stop:0 #f1c40f, stop:1 #f39c12);"
                "    box-shadow: 0 6px 20px rgba(243, 156, 18, 0.4);"
                "}"
                "QPushButton:pressed {"
                "    background: qlineargradient(x1:0, y1:0, x2:1, y2:0,"
                "                stop:0 #e67e22, stop:1 #d35400);"
                "    box-shadow: 0 2px 10px rgba(243, 156, 18, 0.2);"
                "}";
            connect(spaceButton, &QPushButton::clicked, this, &SpaceSelectionWidget::onMySpaceClicked);
        } else {
            styleSheet = 
                "QPushButton {"
                "    background: qlineargradient(x1:0, y1:0, x2:1, y2:0,"
                "                stop:0 #e74c3c, stop:1 #c0392b);"
                "    color: white;"
                "    border: none;"
                "    font-weight: 600;"
                "    font-size: 16px;"
                "    font-family: 'Segoe UI', sans-serif;"
                "    border-radius: 15px;"
                "    cursor: not-allowed;"
                "    opacity: 0.7;"
                "    padding: 10px;"
                "}";
            spaceButton->setEnabled(false);
        }
        
        spaceButton->setStyleSheet(styleSheet);
        m_spacesLayout->addWidget(spaceButton, row, col);
        
        col++;
        if (col >= columns) {
            col = 0;
            row++;
        }
    }
    
    // 更新状态标签 - 更现代化的显示
    m_statusLabel->setText(QString("🚗 当前车牌: %1 | 可用停车位: %2/%3")
                          .arg(m_currentPlate)
                          .arg(availableCount)
                          .arg(spaces.size()));
    
    // 显示/隐藏排队按钮
    m_queueButton->setVisible(availableCount == 0);
}



void SpaceSelectionWidget::onAvailableSpaceClicked()
{
    QPushButton *button = qobject_cast<QPushButton*>(sender());
    if (button) {
        m_selectedSpaceId = button->property("spaceId").toInt();
        m_selectedLocation = button->text().split("\n")[0]; // 获取位置信息
        
        // 显示占用确认对话框
        OccupySpaceDialog *dialog = new OccupySpaceDialog(m_currentPlate, m_selectedSpaceId, m_selectedLocation, this);
        connect(dialog, &OccupySpaceDialog::occupySuccess, this, &SpaceSelectionWidget::onOccupySpaceConfirmed);
        dialog->exec();
    }
}

void SpaceSelectionWidget::onMySpaceClicked()
{
    QPushButton *button = qobject_cast<QPushButton*>(sender());
    if (button) {
        m_selectedSpaceId = button->property("spaceId").toInt();
        m_selectedLocation = button->text().split("\n")[0]; // 获取位置信息
        
        // 直接获取车位信息并跳转到停车信息面板
        QString url = QString("http://127.0.0.1:8080/api/spaces/%1").arg(m_selectedSpaceId);
        QNetworkRequest request = QNetworkRequest(QUrl(url));
        
        QNetworkReply *reply = m_networkManager->get(request);
        connect(reply, &QNetworkReply::finished, [this, reply]() {
            if (reply->error() == QNetworkReply::NoError) {
                // 直接跳转到停车信息面板，不需要额外处理
                emit spaceSelected(m_selectedSpaceId);
            } else {
                showMessage("网络错误", "网络连接失败，请检查网络设置", true);
            }
            reply->deleteLater();
        });
    }
}

void SpaceSelectionWidget::onOccupySpaceConfirmed()
{
    emit spaceSelected(m_selectedSpaceId);
}

void SpaceSelectionWidget::onQueueButtonClicked()
{
    emit queueRequested();
}

void SpaceSelectionWidget::showMessage(const QString &title, const QString &message, bool isError)
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