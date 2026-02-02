#ifndef REPORTSTATISTICS_H
#define REPORTSTATISTICS_H

#include <QMainWindow>
#include <QJsonObject>
#include "networkmanager.h"
#include "parkingspacemanager.h"

namespace Ui {
class ReportStatistics;
}

class ReportStatistics : public QMainWindow
{
    Q_OBJECT

public:
    explicit ReportStatistics(QWidget *parent = nullptr);
    ~ReportStatistics();

private slots:
    // 报告按钮点击事件
    void on_btnGetRevenueReport_clicked();
    void on_btnGetParkingReport_clicked();
    void on_btnGetSpaceUsageReport_clicked();
    void on_btnGetOccupancyRateReport_clicked();
    void on_btnGetCarStatistics_clicked();
    void on_btnGetCarTypeDistribution_clicked();
    void on_btnGetDashboardSummary_clicked();
    void on_btnGetDetailedReport_clicked();

private:
    Ui::ReportStatistics *ui;
    NetworkManager *m_networkManager;
    ParkingSpaceManager *m_parkingSpaceManager;
    
    // 初始化UI
    void initUI();
    // 显示成功信息
    void showSuccessMessage(const QString &message);
    // 显示错误信息
    void showErrorMessage(const QString &message);
    // 清除所有报告显示
    void clearAllReports();
    // 显示收入报告
    void displayRevenueReport(const QJsonObject &reportData);
    // 显示停车统计报告
    void displayParkingReport(const QJsonObject &reportData);
    // 显示空间使用率报告
    void displaySpaceUsageReport(const QJsonObject &reportData);
    // 显示占用率报告
    void displayOccupancyRateReport(const QJsonObject &reportData);
    // 显示车辆统计报告
    void displayCarStatistics(const QJsonObject &reportData);
    // 显示车辆类型分布报告
    void displayCarTypeDistribution(const QJsonObject &reportData);
    // 显示仪表板摘要
    void displayDashboardSummary(const QJsonObject &reportData);
    // 显示详细报告
    void displayDetailedReport(const QJsonObject &reportData);
};

#endif // REPORTSTATISTICS_H