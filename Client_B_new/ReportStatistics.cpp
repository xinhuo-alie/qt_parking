#include "ReportStatistics.h"
#include "ui_ReportStatistics.h"
#include <QApplication>
#include <QStyleFactory>
#include <QMessageBox>
#include <QJsonArray>
#include <QDebug>

ReportStatistics::ReportStatistics(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ReportStatistics),
    m_networkManager(new NetworkManager(this)),
    m_parkingSpaceManager(new ParkingSpaceManager(m_networkManager, this))
{
    ui->setupUi(this);
    initUI();
}

ReportStatistics::~ReportStatistics()
{
    delete m_parkingSpaceManager;
    delete m_networkManager;
    delete ui;
}

void ReportStatistics::initUI()
{
    // 设置窗口标题
    this->setWindowTitle("停车场管理系统 - 报告统计");
    
    // 设置样式表，使用绿色主基调
    QString styleSheet = "" 
        "/* 主窗口背景 - 使用渐变色 */" 
        "QMainWindow { " 
        "    background: qlineargradient(x1:0, y1:0, x2:1, y2:1, stop:0 #f5f5f5, stop:1 #e0e0e0);" 
        "}" 
        "" 
        "/* 组框样式 - 使用主题色 */" 
        "QGroupBox { " 
        "    font: bold 13px;" 
        "    border: 2px solid #2ecc71;" 
        "    border-radius: 10px;" 
        "    margin-top: 8px;" 
        "    background-color: #ffffff;" 
        "}" 
        "" 
        "/* 按钮样式 - 渐变效果 */" 
        "QPushButton { " 
        "    background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #2ecc71, stop:1 #27ae60);" 
        "    color: white;" 
        "    font: bold 12px;" 
        "    border-radius: 8px;" 
        "    padding: 10px 15px;" 
        "    border: 2px solid #27ae60;" 
        "}" 
        "" 
        "/* 按钮悬停效果 */" 
        "QPushButton:hover { " 
        "    background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #34d97c, stop:1 #2ecc71);" 
        "    border: 2px solid #2ecc71;" 
        "}" 
        "" 
        "/* 按钮按下效果 */" 
        "QPushButton:pressed { " 
        "    background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #27ae60, stop:1 #229954);" 
        "    border: 2px solid #229954;" 
        "}" 
        "" 
        "/* 文本编辑框样式 */" 
        "QLineEdit { " 
        "    border: 2px solid #e0e0e0;" 
        "    border-radius: 5px;" 
        "    padding: 5px;" 
        "    background-color: white;" 
        "}" 
        "" 
        "/* 下拉框样式 */" 
        "QComboBox { " 
        "    border: 2px solid #e0e0e0;" 
        "    border-radius: 5px;" 
        "    padding: 5px;" 
        "    background-color: white;" 
        "}" 
        "" 
        "/* 文本浏览器样式 */" 
        "QTextBrowser { " 
        "    border: 2px solid #e0e0e0;" 
        "    border-radius: 5px;" 
        "    background-color: white;" 
        "}";
    
    this->setStyleSheet(styleSheet);
}

void ReportStatistics::showSuccessMessage(const QString &message)
{
    QMessageBox::information(this, "成功", message);
}

void ReportStatistics::showErrorMessage(const QString &message)
{
    QMessageBox::critical(this, "错误", message);
}

void ReportStatistics::clearAllReports()
{
    ui->txtReportDisplay->clear();
}

void ReportStatistics::displayRevenueReport(const QJsonObject &reportData)
{
    QString reportText = "收入报告\n";
    reportText += "========================\n";
    reportText += QString("总收入: %1 元\n").arg(reportData["totalRevenue"].toDouble());
    reportText += QString("已支付笔数: %1\n").arg(reportData["paidCount"].toInt());
    reportText += QString("未支付笔数: %1\n").arg(reportData["unpaidCount"].toInt());
    reportText += QString("平均费用: %1 元\n").arg(reportData["averageFee"].toDouble());
    
    // 显示按周期的数据
    QJsonArray byPeriod = reportData["byPeriod"].toArray();
    if (!byPeriod.isEmpty()) {
        reportText += "\n按周期统计:\n";
        for (const QJsonValue &periodValue : byPeriod) {
            QJsonObject period = periodValue.toObject();
            reportText += QString("周期: %1, 收入: %2 元, 笔数: %3\n")
                .arg(period["period"].toString())
                .arg(period["revenue"].toDouble())
                .arg(period["count"].toInt());
        }
    }
    
    ui->txtReportDisplay->setText(reportText);
}

void ReportStatistics::displayParkingReport(const QJsonObject &reportData)
{
    QString reportText = "停车统计报告\n";
    reportText += "========================\n";
    reportText += QString("总停车次数: %1\n").arg(reportData["totalParkings"].toInt());
    reportText += QString("平均停车时长: %1 分钟\n").arg(reportData["averageDuration"].toInt());
    
    // 显示高峰时段
    QJsonArray peakHours = reportData["peakHours"].toArray();
    if (!peakHours.isEmpty()) {
        reportText += "高峰时段: ";
        for (const QJsonValue &hourValue : peakHours) {
            reportText += hourValue.toString() + " ";
        }
        reportText += "\n";
    }
    
    // 显示按类型统计
    QJsonObject byType = reportData["byType"].toObject();
    if (!byType.isEmpty()) {
        reportText += "\n按车辆类型统计:\n";
        for (const QString &type : byType.keys()) {
            reportText += QString("%1: %2\n").arg(type).arg(byType[type].toInt());
        }
    }
    
    ui->txtReportDisplay->setText(reportText);
}

void ReportStatistics::displaySpaceUsageReport(const QJsonObject &reportData)
{
    QString reportText = "空间使用率报告\n";
    reportText += "========================\n";
    reportText += QString("总停车位数: %1\n").arg(reportData["totalSpaces"].toInt());
    reportText += QString("平均使用率: %1%\n").arg(reportData["averageUsage"].toDouble() * 100, 0, 'f', 2);
    reportText += QString("峰值使用率: %1%\n").arg(reportData["peakUsage"].toDouble() * 100, 0, 'f', 2);
    
    // 显示按位置统计
    QJsonObject byLocation = reportData["byLocation"].toObject();
    if (!byLocation.isEmpty()) {
        reportText += "\n按位置统计使用率:\n";
        for (const QString &location : byLocation.keys()) {
            reportText += QString("%1: %2%\n").arg(location).arg(byLocation[location].toDouble() * 100, 0, 'f', 2);
        }
    }
    
    ui->txtReportDisplay->setText(reportText);
}

void ReportStatistics::displayOccupancyRateReport(const QJsonObject &reportData)
{
    QString reportText = "占用率报告\n";
    reportText += "========================\n";
    reportText += QString("当前占用数: %1\n").arg(reportData["currentOccupancy"].toInt());
    reportText += QString("当前占用率: %1%\n").arg(reportData["occupancyRate"].toDouble() * 100, 0, 'f', 2);
    reportText += QString("趋势: %1\n").arg(reportData["trend"].toString());
    
    // 显示按小时趋势
    QJsonArray hourlyTrend = reportData["hourlyTrend"].toArray();
    if (!hourlyTrend.isEmpty()) {
        reportText += "\n按小时占用率趋势:\n";
        for (const QJsonValue &hourValue : hourlyTrend) {
            QJsonObject hour = hourValue.toObject();
            reportText += QString("时间: %1, 占用数: %2, 占用率: %3%\n")
                .arg(hour["hour"].toString())
                .arg(hour["occupancy"].toInt())
                .arg(hour["rate"].toDouble() * 100, 0, 'f', 2);
        }
    }
    
    ui->txtReportDisplay->setText(reportText);
}

void ReportStatistics::displayCarStatistics(const QJsonObject &reportData)
{
    QString reportText = "车辆统计报告\n";
    reportText += "========================\n";
    reportText += QString("总车辆数: %1\n").arg(reportData["totalCars"].toInt());
    reportText += QString("新增车辆数: %1\n").arg(reportData["newCars"].toInt());
    
    // 显示按类型统计
    QJsonObject byType = reportData["byType"].toObject();
    if (!byType.isEmpty()) {
        reportText += "\n按车辆类型统计:\n";
        for (const QString &type : byType.keys()) {
            reportText += QString("%1: %2\n").arg(type).arg(byType[type].toInt());
        }
    }
    
    // 显示热门颜色
    QJsonArray topColors = reportData["topColors"].toArray();
    if (!topColors.isEmpty()) {
        reportText += "\n热门颜色:\n";
        for (const QJsonValue &colorValue : topColors) {
            reportText += colorValue.toString() + " ";
        }
        reportText += "\n";
    }
    
    ui->txtReportDisplay->setText(reportText);
}

void ReportStatistics::displayCarTypeDistribution(const QJsonObject &reportData)
{
    QString reportText = "车辆类型分布报告\n";
    reportText += "========================\n";
    
    // 显示车辆类型分布
    QJsonArray distribution = reportData["distribution"].toArray();
    if (!distribution.isEmpty()) {
        for (const QJsonValue &itemValue : distribution) {
            QJsonObject item = itemValue.toObject();
            reportText += QString("%1: %2 辆 (占比 %3%)\n")
                .arg(item["type"].toString())
                .arg(item["count"].toInt())
                .arg(item["percentage"].toDouble(), 0, 'f', 2);
        }
    }
    
    ui->txtReportDisplay->setText(reportText);
}

void ReportStatistics::displayDashboardSummary(const QJsonObject &reportData)
{
    QString reportText = "仪表板摘要\n";
    reportText += "========================\n";
    
    // 显示汇总信息
    QJsonObject summary = reportData["summary"].toObject();
    if (!summary.isEmpty()) {
        reportText += "汇总信息:\n";
        reportText += QString("总收入: %1 元\n").arg(summary["totalRevenue"].toDouble());
        reportText += QString("总停车次数: %1\n").arg(summary["totalParkings"].toInt());
        reportText += QString("总车辆数: %1\n").arg(summary["totalCars"].toInt());
        reportText += QString("总停车位数: %1\n").arg(summary["totalSpaces"].toInt());
    }
    
    // 显示今日信息
    QJsonObject today = reportData["today"].toObject();
    if (!today.isEmpty()) {
        reportText += "\n今日信息:\n";
        reportText += QString("今日收入: %1 元\n").arg(today["revenue"].toDouble());
        reportText += QString("今日停车次数: %1\n").arg(today["parkings"].toInt());
        reportText += QString("今日占用率: %1%\n").arg(today["occupancy"].toDouble() * 100, 0, 'f', 2);
    }
    
    // 显示实时信息
    QJsonObject realTime = reportData["realTime"].toObject();
    if (!realTime.isEmpty()) {
        reportText += "\n实时信息:\n";
        reportText += QString("可用停车位: %1\n").arg(realTime["availableSpaces"].toInt());
        reportText += QString("占用停车位: %1\n").arg(realTime["occupiedSpaces"].toInt());
        reportText += QString("当前停车数: %1\n").arg(realTime["currentCars"].toInt());
    }
    
    ui->txtReportDisplay->setText(reportText);
}

void ReportStatistics::displayDetailedReport(const QJsonObject &reportData)
{
    QString reportText = "详细报告\n";
    reportText += "========================\n";
    
    // 显示报告周期
    QJsonObject period = reportData["period"].toObject();
    if (!period.isEmpty()) {
        reportText += "报告周期:\n";
        reportText += QString("开始日期: %1\n").arg(period["startDate"].toString());
        reportText += QString("结束日期: %1\n").arg(period["endDate"].toString());
    }
    
    // 显示收入信息
    QJsonObject revenue = reportData["revenue"].toObject();
    if (!revenue.isEmpty()) {
        reportText += "\n收入信息:\n";
        reportText += QString("总收入: %1 元\n").arg(revenue["total"].toDouble());
        reportText += QString("日平均收入: %1 元\n").arg(revenue["dailyAverage"].toDouble());
        reportText += QString("增长率: %1%\n").arg(revenue["growth"].toDouble() * 100, 0, 'f', 2);
    }
    
    // 显示停车信息
    QJsonObject parking = reportData["parking"].toObject();
    if (!parking.isEmpty()) {
        reportText += "\n停车信息:\n";
        reportText += QString("总停车次数: %1\n").arg(parking["total"].toInt());
        reportText += QString("日平均停车次数: %1\n").arg(parking["dailyAverage"].toDouble());
        reportText += QString("增长率: %1%\n").arg(parking["growth"].toDouble() * 100, 0, 'f', 2);
    }
    
    // 显示详细统计
    QJsonObject detailedStats = reportData["detailedStats"].toObject();
    if (!detailedStats.isEmpty()) {
        reportText += "\n详细统计:\n";
        reportText += QString("收入峰值日: %1\n").arg(detailedStats["peakRevenueDay"].toString());
        reportText += QString("停车峰值日: %1\n").arg(detailedStats["peakParkingDay"].toString());
        reportText += QString("最高效停车位: %1\n").arg(detailedStats["bestPerformingSpace"].toInt());
        reportText += QString("最常用车辆类型: %1\n").arg(detailedStats["mostUsedCarType"].toString());
    }
    
    ui->txtReportDisplay->setText(reportText);
}

// 报告按钮点击事件处理
void ReportStatistics::on_btnGetRevenueReport_clicked()
{
    clearAllReports();
    
    m_parkingSpaceManager->getRevenueReport(
        "", "", "",
        [this](QJsonObject data) {
            if (data["code"].toInt() == 0) {
                QJsonObject reportData = data["data"].toObject();
                displayRevenueReport(reportData);
                showSuccessMessage("收入报告获取成功");
            } else {
                showErrorMessage("获取收入报告失败: " + data["msg"].toString());
            }
        },
        [this](QString error) {
            showErrorMessage("获取收入报告失败: " + error);
        }
    );
}

void ReportStatistics::on_btnGetParkingReport_clicked()
{
    clearAllReports();
    
    m_parkingSpaceManager->getParkingReport(
        "", "", "",
        [this](QJsonObject data) {
            if (data["code"].toInt() == 0) {
                QJsonObject reportData = data["data"].toObject();
                displayParkingReport(reportData);
                showSuccessMessage("停车统计报告获取成功");
            } else {
                showErrorMessage("获取停车统计报告失败: " + data["msg"].toString());
            }
        },
        [this](QString error) {
            showErrorMessage("获取停车统计报告失败: " + error);
        }
    );
}

void ReportStatistics::on_btnGetSpaceUsageReport_clicked()
{
    clearAllReports();
    
    m_parkingSpaceManager->getSpaceUsageReport(
        "", "", "",
        [this](QJsonObject data) {
            if (data["code"].toInt() == 0) {
                QJsonObject reportData = data["data"].toObject();
                displaySpaceUsageReport(reportData);
                showSuccessMessage("空间使用率报告获取成功");
            } else {
                showErrorMessage("获取空间使用率报告失败: " + data["msg"].toString());
            }
        },
        [this](QString error) {
            showErrorMessage("获取空间使用率报告失败: " + error);
        }
    );
}

void ReportStatistics::on_btnGetOccupancyRateReport_clicked()
{
    clearAllReports();
    
    m_parkingSpaceManager->getOccupancyRateReport(
        "", "", "",
        [this](QJsonObject data) {
            if (data["code"].toInt() == 0) {
                QJsonObject reportData = data["data"].toObject();
                displayOccupancyRateReport(reportData);
                showSuccessMessage("占用率报告获取成功");
            } else {
                showErrorMessage("获取占用率报告失败: " + data["msg"].toString());
            }
        },
        [this](QString error) {
            showErrorMessage("获取占用率报告失败: " + error);
        }
    );
}

void ReportStatistics::on_btnGetCarStatistics_clicked()
{
    clearAllReports();
    
    m_parkingSpaceManager->getCarStatistics(
        "", "", "",
        [this](QJsonObject data) {
            if (data["code"].toInt() == 0) {
                QJsonObject reportData = data["data"].toObject();
                displayCarStatistics(reportData);
                showSuccessMessage("车辆统计报告获取成功");
            } else {
                showErrorMessage("获取车辆统计报告失败: " + data["msg"].toString());
            }
        },
        [this](QString error) {
            showErrorMessage("获取车辆统计报告失败: " + error);
        }
    );
}

void ReportStatistics::on_btnGetCarTypeDistribution_clicked()
{
    clearAllReports();
    
    m_parkingSpaceManager->getCarTypeDistribution(
        "", "", "",
        [this](QJsonObject data) {
            if (data["code"].toInt() == 0) {
                QJsonObject reportData = data["data"].toObject();
                displayCarTypeDistribution(reportData);
                showSuccessMessage("车辆类型分布报告获取成功");
            } else {
                showErrorMessage("获取车辆类型分布报告失败: " + data["msg"].toString());
            }
        },
        [this](QString error) {
            showErrorMessage("获取车辆类型分布报告失败: " + error);
        }
    );
}

void ReportStatistics::on_btnGetDashboardSummary_clicked()
{
    clearAllReports();
    
    m_parkingSpaceManager->getDashboardSummary(
        [this](QJsonObject data) {
            if (data["code"].toInt() == 0) {
                QJsonObject reportData = data["data"].toObject();
                displayDashboardSummary(reportData);
                showSuccessMessage("仪表板摘要获取成功");
            } else {
                showErrorMessage("获取仪表板摘要失败: " + data["msg"].toString());
            }
        },
        [this](QString error) {
            showErrorMessage("获取仪表板摘要失败: " + error);
        }
    );
}

void ReportStatistics::on_btnGetDetailedReport_clicked()
{
    clearAllReports();
    
    // 默认使用当前日期作为开始和结束日期
    QString currentDate = QDate::currentDate().toString("yyyy-MM-dd");
    
    m_parkingSpaceManager->getDetailedReport(
        currentDate, currentDate, "summary",
        [this](QJsonObject data) {
            if (data["code"].toInt() == 0) {
                QJsonObject reportData = data["data"].toObject();
                displayDetailedReport(reportData);
                showSuccessMessage("详细报告获取成功");
            } else {
                showErrorMessage("获取详细报告失败: " + data["msg"].toString());
            }
        },
        [this](QString error) {
            showErrorMessage("获取详细报告失败: " + error);
        }
    );
}