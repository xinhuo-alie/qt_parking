#include "occupyspacedialog.h"

OccupySpaceDialog::OccupySpaceDialog(const QString &plate, int spaceId, const QString &location, QWidget *parent)
    : QDialog(parent)
    , m_currentPlate(plate)
    , m_spaceId(spaceId)
    , m_location(location)
    , m_networkManager(new QNetworkAccessManager(this))
{
    setupUI();
    
    connect(m_networkManager, &QNetworkAccessManager::finished, 
            this, &OccupySpaceDialog::onNetworkReply);
}

void OccupySpaceDialog::setupUI()
{
    setWindowTitle("确认占用车位");
    setFixedSize(450, 300);
    setModal(true);
    
    // 设置对话框背景渐变
    setStyleSheet(
        "OccupySpaceDialog {"
        "    background: qlineargradient(x1:0, y1:0, x2:1, y2:1,"
        "                stop:0 #667eea, stop:1 #764ba2);"
        "}"
    );
    
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(30);
    mainLayout->setContentsMargins(40, 40, 40, 40);
    
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
    containerLayout->setContentsMargins(35, 35, 35, 35);
    
    // 标题区域
    QHBoxLayout *titleLayout = new QHBoxLayout();
    titleLayout->setAlignment(Qt::AlignCenter);
    
    QLabel *titleIcon = new QLabel(this);
    titleIcon->setText("🅿️");
    titleIcon->setStyleSheet("font-size: 32px; margin-right: 15px;");
    titleLayout->addWidget(titleIcon);
    
    QLabel *titleLabel = new QLabel("确认占用车位", this);
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setStyleSheet(
        "font-size: 24px; "
        "font-weight: 700; "
        "color: #2c3e50; "
        "font-family: 'Segoe UI', 'Helvetica Neue', sans-serif;"
    );
    titleLayout->addWidget(titleLabel);
    
    containerLayout->addLayout(titleLayout);
    
    // 提示信息 - 更现代化的设计
    m_infoLabel = new QLabel(QString("确认要占用停车位 %1 吗？\n车牌号: %2")
                            .arg(m_location)
                            .arg(m_currentPlate), this);
    m_infoLabel->setAlignment(Qt::AlignCenter);
    m_infoLabel->setStyleSheet(
        "font-size: 16px; "
        "color: #2c3e50; "
        "font-family: 'Segoe UI', sans-serif; "
        "line-height: 1.6; "
        "padding: 20px; "
        "background-color: rgba(52, 152, 219, 0.1); "
        "border-radius: 15px;"
    );
    m_infoLabel->setWordWrap(true);
    containerLayout->addWidget(m_infoLabel);
    
    // 按钮区域 - 更现代化的布局
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->setSpacing(25);
    buttonLayout->setAlignment(Qt::AlignCenter);
    
    m_confirmButton = new QPushButton("✅ 确认占用", this);
    m_confirmButton->setFixedHeight(50);
    m_confirmButton->setStyleSheet(
        "QPushButton {"
        "    background: qlineargradient(x1:0, y1:0, x2:1, y2:0,"
        "                stop:0 #2ecc71, stop:1 #27ae60);"
        "    color: white;"
        "    border: none;"
        "    font-size: 16px;"
        "    font-weight: 600;"
        "    border-radius: 25px;"
        "    padding: 0 30px;"
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
    connect(m_confirmButton, &QPushButton::clicked, this, &OccupySpaceDialog::onConfirmClicked);
    buttonLayout->addWidget(m_confirmButton);
    
    m_cancelButton = new QPushButton("❌ 取消", this);
    m_cancelButton->setFixedHeight(50);
    m_cancelButton->setStyleSheet(
        "QPushButton {"
        "    background: qlineargradient(x1:0, y1:0, x2:1, y2:0,"
        "                stop:0 #95a5a6, stop:1 #7f8c8d);"
        "    color: white;"
        "    border: none;"
        "    font-size: 16px;"
        "    font-weight: 600;"
        "    border-radius: 25px;"
        "    padding: 0 30px;"
        "    font-family: 'Segoe UI', sans-serif;"
        "    box-shadow: 0 4px 15px rgba(149, 165, 166, 0.3);"
        "}"
        "QPushButton:hover {"
        "    background: qlineargradient(x1:0, y1:0, x2:1, y2:0,"
        "                stop:0 #bdc3c7, stop:1 #95a5a6);"
        "    box-shadow: 0 6px 20px rgba(149, 165, 166, 0.4);"
        "}"
        "QPushButton:pressed {"
        "    background: qlineargradient(x1:0, y1:0, x2:1, y2:0,"
        "                stop:0 #7f8c8d, stop:1 #6c7b7d);"
        "    box-shadow: 0 2px 10px rgba(149, 165, 166, 0.2);"
        "}"
    );
    connect(m_cancelButton, &QPushButton::clicked, this, &OccupySpaceDialog::onCancelClicked);
    buttonLayout->addWidget(m_cancelButton);
    
    containerLayout->addLayout(buttonLayout);
    
    mainLayout->addWidget(containerWidget);
}

void OccupySpaceDialog::onConfirmClicked()
{
    m_confirmButton->setEnabled(false);
    m_confirmButton->setText("处理中...");
    
    // 构建占用车位请求
    QJsonObject json;
    json["plate"] = m_currentPlate;
    
    QJsonDocument doc(json);
    QByteArray data = doc.toJson();
    
    QNetworkRequest request(QUrl(QString("http://127.0.0.1:8080/api/spaces/%1/occupy").arg(m_spaceId)));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    
    m_networkManager->post(request, data);
}

void OccupySpaceDialog::onCancelClicked()
{
    reject();
}

void OccupySpaceDialog::onNetworkReply(QNetworkReply *reply)
{
    if (reply->error() == QNetworkReply::NoError) {
        QByteArray response = reply->readAll();
        QJsonDocument doc = QJsonDocument::fromJson(response);
        QJsonObject obj = doc.object();
        
        if (obj["code"].toInt() == 0) {
            QJsonObject dataObj = obj["data"].toObject();
            QString msg = dataObj["msg"].toString();
            
            showMessage("成功", msg.isEmpty() ? "占用车位成功！" : msg);
            emit occupySuccess();
            accept();
        } else {
            QString errorMsg = obj["msg"].toString();
            showMessage("占用失败", errorMsg.isEmpty() ? "占用车位失败，请重试" : errorMsg, true);
            m_confirmButton->setEnabled(true);
            m_confirmButton->setText("确认");
        }
    } else {
        showMessage("网络错误", "网络连接失败，请检查网络设置", true);
        m_confirmButton->setEnabled(true);
        m_confirmButton->setText("确认");
    }
    
    reply->deleteLater();
}

void OccupySpaceDialog::showMessage(const QString &title, const QString &message, bool isError)
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