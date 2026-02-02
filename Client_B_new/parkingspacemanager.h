#ifndef PARKINGSPACEMANAGER_H
#define PARKINGSPACEMANAGER_H

#include <QObject>
#include <QJsonObject>
#include <functional>
#include "networkmanager.h"

class ParkingSpaceManager : public QObject
{
    Q_OBJECT
public:
    explicit ParkingSpaceManager(NetworkManager *networkManager, QObject *parent = nullptr);
    ~ParkingSpaceManager();

    // 添加停车位
    void addParkingSpace(
        const QString &location,
        const QString &type,
        double hourlyRate,
        std::function<void (QJsonObject)> onSuccess,
        std::function<void (QString)> onError);

    // 获取单个停车位信息
    void getParkingSpace(
        int id,
        std::function<void (QJsonObject)> onSuccess,
        std::function<void (QString)> onError);

    // 更新停车位信息
    void updateParkingSpace(
        int id,
        const QJsonObject &updateData,
        std::function<void (QJsonObject)> onSuccess,
        std::function<void (QString)> onError);

    // 删除停车位
    void deleteParkingSpace(
        int id,
        std::function<void (QJsonObject)> onSuccess,
        std::function<void (QString)> onError);

    // 获取停车位列表
    void getParkingSpaces(
        int page = 1,
        int limit = 20,
        const QString &status = "",
        const QString &type = "",
        std::function<void (QJsonObject)> onSuccess = nullptr,
        std::function<void (QString)> onError = nullptr);



    // 报告统计相关方法
    // 获取收入报告
    void getRevenueReport(
        const QString &startDate = "",
        const QString &endDate = "",
        const QString &type = "",
        std::function<void (QJsonObject)> onSuccess = nullptr,
        std::function<void (QString)> onError = nullptr);

    // 获取停车统计报告
    void getParkingReport(
        const QString &startDate = "",
        const QString &endDate = "",
        const QString &type = "",
        std::function<void (QJsonObject)> onSuccess = nullptr,
        std::function<void (QString)> onError = nullptr);

    // 获取空间使用率报告
    void getSpaceUsageReport(
        const QString &startDate = "",
        const QString &endDate = "",
        const QString &type = "",
        std::function<void (QJsonObject)> onSuccess = nullptr,
        std::function<void (QString)> onError = nullptr);

    // 获取占用率报告
    void getOccupancyRateReport(
        const QString &startDate = "",
        const QString &endDate = "",
        const QString &type = "",
        std::function<void (QJsonObject)> onSuccess = nullptr,
        std::function<void (QString)> onError = nullptr);

    // 获取车辆统计报告
    void getCarStatistics(
        const QString &startDate = "",
        const QString &endDate = "",
        const QString &type = "",
        std::function<void (QJsonObject)> onSuccess = nullptr,
        std::function<void (QString)> onError = nullptr);

    // 获取车辆类型分布报告
    void getCarTypeDistribution(
        const QString &startDate = "",
        const QString &endDate = "",
        const QString &type = "",
        std::function<void (QJsonObject)> onSuccess = nullptr,
        std::function<void (QString)> onError = nullptr);

    // 获取仪表板摘要
    void getDashboardSummary(
        std::function<void (QJsonObject)> onSuccess = nullptr,
        std::function<void (QString)> onError = nullptr);

    // 获取详细报告
    void getDetailedReport(
        const QString &startDate,
        const QString &endDate,
        const QString &reportType = "",
        std::function<void (QJsonObject)> onSuccess = nullptr,
        std::function<void (QString)> onError = nullptr);

private:
    NetworkManager *m_networkManager;
};

#endif // PARKINGSPACEMANAGER_H
