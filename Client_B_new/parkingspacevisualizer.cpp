#include "parkingspacevisualizer.h"
#include <QPainter>
#include <QMouseEvent>
#include <QMessageBox>
#include <QDateTime>
#include <QDebug>
#include <QIcon>

ParkingSpaceVisualizer::ParkingSpaceVisualizer(QWidget *parent)
    : QDialog(parent)
{
    // 设置窗口标题和属性
    setWindowTitle("🏢 停车位可视化");
    setWindowIcon(QIcon::fromTheme("office-building")); // 使用系统图标
    setMinimumSize(800, 600);
    resize(1000, 800);
    
    // 设置窗口属性，增强用户体验
    setWindowFlags(Qt::Window | Qt::WindowMinMaxButtonsHint | Qt::WindowCloseButtonHint);
    setAttribute(Qt::WA_DeleteOnClose);
    setModal(false); // 非模态窗口
    
    // 设置窗口样式 - 美化界面
    QString styleSheet = R"(
        QDialog {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:1, stop:0 #f5f5f5, stop:1 #e0e0e0);
            border: 2px solid #2ecc71;
            border-radius: 10px;
            box-shadow: 0 5px 20px rgba(0, 0, 0, 0.15);
        }
        QDialog::title {
            color: #2ecc71;
            font: bold 14px;
        }
    )";
    this->setStyleSheet(styleSheet);
}

ParkingSpaceVisualizer::~ParkingSpaceVisualizer()
{
}

void ParkingSpaceVisualizer::setParkingSpaces(const QJsonArray &spaces)
{
    m_spaces = spaces;
    calculateSpacePositions();
    update(); // 触发重绘
}

void ParkingSpaceVisualizer::calculateSpacePositions()
{
    m_spaceRects.clear();
    
    const int margin = 50;
    const int spaceWidth = 80;
    const int spaceHeight = 120;
    const int spacing = 20;
    
    int x = margin;
    int y = margin;
    int maxX = width() - margin - spaceWidth;
    
    for (int i = 0; i < m_spaces.size(); ++i) {
        QJsonObject spaceObj = m_spaces[i].toObject();
        int spaceId = spaceObj["id"].toInt();
        
        QRect rect(x, y, spaceWidth, spaceHeight);
        m_spaceRects[spaceId] = rect;
        
        // 更新位置，准备下一个停车位
        x += spaceWidth + spacing;
        if (x > maxX) {
            x = margin;
            y += spaceHeight + spacing;
        }
    }
}

void ParkingSpaceVisualizer::paintEvent(QPaintEvent *event)
{
    QDialog::paintEvent(event);
    
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    
    // 绘制停车场背景
    painter.fillRect(rect(), QColor(240, 240, 240));
    
    // 绘制停车位
    for (int i = 0; i < m_spaces.size(); ++i) {
        QJsonObject spaceObj = m_spaces[i].toObject();
        int spaceId = spaceObj["id"].toInt();
        
        if (!m_spaceRects.contains(spaceId)) {
            continue;
        }
        
        QRect rect = m_spaceRects[spaceId];
        QString status = spaceObj["status"].toString();
        QString type = spaceObj.contains("type") ? spaceObj["type"].toString() : "";
        
        // 根据状态设置颜色
        QColor fillColor;
        QColor borderColor;
        
        if (status == "occupied") {
            fillColor = QColor(255, 99, 71); // 番茄红色表示占用
            borderColor = QColor(220, 20, 60); // 深红色边框
        } else if (status == "available") {
            if (type == "VIP") {
                fillColor = QColor(135, 206, 250); // 浅蓝色表示VIP空闲
                borderColor = QColor(0, 191, 255); // 深蓝色边框
            } else {
                fillColor = QColor(144, 238, 144); // 浅绿色表示普通空闲
                borderColor = QColor(34, 139, 34); // 深绿色边框
            }
        } else {
            fillColor = QColor(192, 192, 192); // 灰色表示其他状态
            borderColor = QColor(128, 128, 128); // 深灰色边框
        }
        
        // 绘制阴影效果
        QRect shadowRect = rect.adjusted(3, 3, 3, 3);
        painter.fillRect(shadowRect, QColor(0, 0, 0, 30));
        
        // 绘制停车位填充色
        painter.fillRect(rect, fillColor);
        
        // 绘制边框
        painter.setPen(QPen(borderColor, 2));
        painter.drawRect(rect);
        
        // 绘制停车位编号
        painter.setPen(Qt::white);
        painter.setFont(QFont("Arial", 14, QFont::Bold));
        QString text = QString::number(spaceId);
        QRect textRect = painter.boundingRect(rect, Qt::AlignCenter, text);
        painter.drawText(textRect, Qt::AlignCenter, text);
        
        // 绘制VIP标识
        if (type == "VIP") {
            QPen vipPen(QColor(0, 191, 255), 2);
            painter.setPen(vipPen);
            painter.setFont(QFont("Arial", 10, QFont::Bold, true)); // 使用第四个参数设置斜体
            
            // 绘制VIP标签背景
            QRect vipRect(rect.left() + 8, rect.bottom() - 20, 40, 15);
            painter.fillRect(vipRect, QColor(255, 255, 255, 180));
            painter.drawRect(vipRect);
            
            // 绘制VIP文本
            painter.drawText(vipRect, Qt::AlignCenter, "VIP");
        }
        
        // 绘制位置信息
        painter.setFont(QFont("Arial", 9, QFont::Medium));
        painter.setPen(QColor(60, 60, 60));
        QString location = spaceObj["location"].toString();
        QRect locationRect(rect.left(), rect.bottom() + 8, rect.width(), 20);
        painter.drawText(locationRect, Qt::AlignCenter, location);
        
        // 绘制状态标签
        painter.setFont(QFont("Arial", 8, QFont::Bold));
        QString statusText = status == "occupied" ? "已占用" : "空闲";
        QColor statusColor = status == "occupied" ? QColor(255, 255, 255) : QColor(60, 60, 60);
        QRect statusRect(rect.left() + 8, rect.top() + 5, 50, 15);
        
        // 绘制状态标签背景
        QColor statusBgColor = status == "occupied" ? QColor(220, 20, 60, 180) : QColor(255, 255, 255, 180);
        painter.fillRect(statusRect, statusBgColor);
        
        // 绘制状态边框和文本
        painter.setPen(statusColor);
        painter.drawText(statusRect, Qt::AlignCenter, statusText);
    }
}

void ParkingSpaceVisualizer::mousePressEvent(QMouseEvent *event)
{
    QPoint pos = event->pos();
    
    // 检查点击的位置是否在某个停车位上
    for (auto it = m_spaceRects.begin(); it != m_spaceRects.end(); ++it) {
        int spaceId = it.key();
        QRect rect = it.value();
        
        if (rect.contains(pos)) {
            QJsonObject spaceObj;
            for (int i = 0; i < m_spaces.size(); ++i) {
                QJsonObject obj = m_spaces[i].toObject();
                if (obj["id"].toInt() == spaceId) {
                    spaceObj = obj;
                    break;
                }
            }
            
            // 显示详细信息
            showSpaceDetails(spaceId);
            break;
        }
    }
    
    QDialog::mousePressEvent(event);
}

void ParkingSpaceVisualizer::resizeEvent(QResizeEvent *event)
{
    QDialog::resizeEvent(event);
    
    // 窗口大小变化时，重新计算停车位位置
    calculateSpacePositions();
    update(); // 触发重绘
}

void ParkingSpaceVisualizer::showSpaceDetails(int spaceId)
{
    QJsonObject spaceObj;
    for (int i = 0; i < m_spaces.size(); ++i) {
        QJsonObject obj = m_spaces[i].toObject();
        if (obj["id"].toInt() == spaceId) {
            spaceObj = obj;
            break;
        }
    }
    
    if (spaceObj.isEmpty()) {
        return;
    }
    
    // 准备详细信息
    QString plateNumber = spaceObj.contains("currentPlate") ? spaceObj["currentPlate"].toString() : "无";
    QString startTime = spaceObj.contains("occupiedTime") ? spaceObj["occupiedTime"].toString() : "无";
    QString parkingInfo = calculateParkingInfo(spaceObj);
    
    QString message = QString("停车位ID: %1\n位置: %2\n类型: %3\n状态: %4\n车牌: %5\n开始时间: %6\n%7")
        .arg(spaceId)
        .arg(spaceObj["location"].toString())
        .arg(spaceObj["type"].toString())
        .arg(spaceObj["status"].toString())
        .arg(plateNumber)
        .arg(startTime)
        .arg(parkingInfo);
    
    QMessageBox::information(this, "停车位详细信息", message);
}

QString ParkingSpaceVisualizer::calculateParkingInfo(const QJsonObject &spaceObj)
{
    if (!spaceObj.contains("occupiedTime")) {
        return "停车时长: 无\n当前收费: 0元";
    }
    
    QString occupiedTimeStr = spaceObj["occupiedTime"].toString();
    QDateTime startTime = QDateTime::fromString(occupiedTimeStr, Qt::ISODate);
    QDateTime currentTime = QDateTime::currentDateTime();
    
    if (!startTime.isValid()) {
        return "停车时长: 无效时间\n当前收费: 0元";
    }
    
    qint64 seconds = startTime.secsTo(currentTime);
    int hours = seconds / 3600;
    int minutes = (seconds % 3600) / 60;
    
    double hourlyRate = spaceObj["hourlyRate"].toDouble();
    double totalCost = hours * hourlyRate;
    if (minutes > 0) {
        totalCost += hourlyRate; // 不足一小时按一小时计算
    }
    
    return QString("停车时长: %1小时%2分钟\n当前收费: %3元")
        .arg(hours)
        .arg(minutes)
        .arg(QString::number(totalCost, 'f', 2));
}
