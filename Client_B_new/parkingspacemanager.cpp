#include "parkingspacemanager.h"
#include <QUrlQuery>

ParkingSpaceManager::ParkingSpaceManager(NetworkManager *networkManager, QObject *parent)
    : QObject(parent), m_networkManager(networkManager)
{}

ParkingSpaceManager::~ParkingSpaceManager()
{}

void ParkingSpaceManager::addParkingSpace(
    const QString &location,
    const QString &type,
    double hourlyRate,
    std::function<void (QJsonObject)> onSuccess,
    std::function<void (QString)> onError)
{
    QJsonObject body;
    body["location"] = location;
    if (!type.isEmpty()) {
        body["type"] = type;
    }
    body["hourlyRate"] = hourlyRate;

    m_networkManager->post(
        "/spaces",
        body,
        onSuccess,
        onError);
}

void ParkingSpaceManager::getParkingSpace(
    int id,
    std::function<void (QJsonObject)> onSuccess,
    std::function<void (QString)> onError)
{
    m_networkManager->get(
        QString("/spaces/%1").arg(id),
        onSuccess,
        onError);
}

void ParkingSpaceManager::updateParkingSpace(
    int id,
    const QJsonObject &updateData,
    std::function<void (QJsonObject)> onSuccess,
    std::function<void (QString)> onError)
{
    // 添加调试信息，查看接收到的updateData
    qDebug() << "updateParkingSpace接收到的updateData: " << updateData;
    
    // 检查updateData中是否包含status字段
    if (updateData.contains("status")) {
        QString status = updateData["status"].toString();
        qDebug() << "updateData中的status值: " << status;
        qDebug() << "status值的长度: " << status.length();
        qDebug() << "status值的十六进制表示: ";
        for (QChar c : status) {
            qDebug() << QString("%1").arg(c.unicode(), 4, 16, QLatin1Char('0'));
        }
    }
    
    // 检查updateData是否为空
    if (updateData.isEmpty()) {
        qDebug() << "警告：updateData为空";
        if (onError) {
            onError("更新数据为空");
        }
        return;
    }
    
    // 构造完整的请求路径
    QString path = QString("/spaces/%1/status").arg(id);
    qDebug() << "构造的请求路径: " << path;
    
    m_networkManager->put(
        path,
        updateData,
        onSuccess,
        onError);
}

void ParkingSpaceManager::deleteParkingSpace(
    int id,
    std::function<void (QJsonObject)> onSuccess,
    std::function<void (QString)> onError)
{
    m_networkManager->del(
        QString("/spaces/%1").arg(id),
        onSuccess,
        onError);
}

void ParkingSpaceManager::getParkingSpaces(
    int page,
    int limit,
    const QString &status,
    const QString &type,
    std::function<void (QJsonObject)> onSuccess,
    std::function<void (QString)> onError)
{
    QString path = "/spaces";
    QUrlQuery query;
    query.addQueryItem("page", QString::number(page));
    query.addQueryItem("limit", QString::number(limit));
    if (!status.isEmpty()) {
        query.addQueryItem("status", status);
    }
    if (!type.isEmpty()) {
        query.addQueryItem("type", type);
    }
    
    QString queryString = query.toString();
    if (!queryString.isEmpty()) {
        path += "?" + queryString;
    }

    m_networkManager->get(
        path,
        onSuccess,
        onError);
}

void ParkingSpaceManager::getRevenueReport(
    const QString &startDate,
    const QString &endDate,
    const QString &type,
    std::function<void (QJsonObject)> onSuccess,
    std::function<void (QString)> onError)
{
    QString path = "/reports/revenue";
    QUrlQuery query;
    if (!startDate.isEmpty()) {
        query.addQueryItem("startDate", startDate);
    }
    if (!endDate.isEmpty()) {
        query.addQueryItem("endDate", endDate);
    }
    if (!type.isEmpty()) {
        query.addQueryItem("type", type);
    }
    
    QString queryString = query.toString();
    if (!queryString.isEmpty()) {
        path += "?" + queryString;
    }

    m_networkManager->get(
        path,
        onSuccess,
        onError);
}

void ParkingSpaceManager::getParkingReport(
    const QString &startDate,
    const QString &endDate,
    const QString &type,
    std::function<void (QJsonObject)> onSuccess,
    std::function<void (QString)> onError)
{
    QString path = "/reports/parking";
    QUrlQuery query;
    if (!startDate.isEmpty()) {
        query.addQueryItem("startDate", startDate);
    }
    if (!endDate.isEmpty()) {
        query.addQueryItem("endDate", endDate);
    }
    if (!type.isEmpty()) {
        query.addQueryItem("type", type);
    }
    
    QString queryString = query.toString();
    if (!queryString.isEmpty()) {
        path += "?" + queryString;
    }

    m_networkManager->get(
        path,
        onSuccess,
        onError);
}

void ParkingSpaceManager::getSpaceUsageReport(
    const QString &startDate,
    const QString &endDate,
    const QString &type,
    std::function<void (QJsonObject)> onSuccess,
    std::function<void (QString)> onError)
{
    QString path = "/reports/space-usage";
    QUrlQuery query;
    if (!startDate.isEmpty()) {
        query.addQueryItem("startDate", startDate);
    }
    if (!endDate.isEmpty()) {
        query.addQueryItem("endDate", endDate);
    }
    if (!type.isEmpty()) {
        query.addQueryItem("type", type);
    }
    
    QString queryString = query.toString();
    if (!queryString.isEmpty()) {
        path += "?" + queryString;
    }

    m_networkManager->get(
        path,
        onSuccess,
        onError);
}

void ParkingSpaceManager::getOccupancyRateReport(
    const QString &startDate,
    const QString &endDate,
    const QString &type,
    std::function<void (QJsonObject)> onSuccess,
    std::function<void (QString)> onError)
{
    QString path = "/reports/occupancy-rate";
    QUrlQuery query;
    if (!startDate.isEmpty()) {
        query.addQueryItem("startDate", startDate);
    }
    if (!endDate.isEmpty()) {
        query.addQueryItem("endDate", endDate);
    }
    if (!type.isEmpty()) {
        query.addQueryItem("type", type);
    }
    
    QString queryString = query.toString();
    if (!queryString.isEmpty()) {
        path += "?" + queryString;
    }

    m_networkManager->get(
        path,
        onSuccess,
        onError);
}

void ParkingSpaceManager::getCarStatistics(
    const QString &startDate,
    const QString &endDate,
    const QString &type,
    std::function<void (QJsonObject)> onSuccess,
    std::function<void (QString)> onError)
{
    QString path = "/reports/car-statistics";
    QUrlQuery query;
    if (!startDate.isEmpty()) {
        query.addQueryItem("startDate", startDate);
    }
    if (!endDate.isEmpty()) {
        query.addQueryItem("endDate", endDate);
    }
    if (!type.isEmpty()) {
        query.addQueryItem("type", type);
    }
    
    QString queryString = query.toString();
    if (!queryString.isEmpty()) {
        path += "?" + queryString;
    }

    m_networkManager->get(
        path,
        onSuccess,
        onError);
}

void ParkingSpaceManager::getCarTypeDistribution(
    const QString &startDate,
    const QString &endDate,
    const QString &type,
    std::function<void (QJsonObject)> onSuccess,
    std::function<void (QString)> onError)
{
    QString path = "/reports/car-type-distribution";
    QUrlQuery query;
    if (!startDate.isEmpty()) {
        query.addQueryItem("startDate", startDate);
    }
    if (!endDate.isEmpty()) {
        query.addQueryItem("endDate", endDate);
    }
    if (!type.isEmpty()) {
        query.addQueryItem("type", type);
    }
    
    QString queryString = query.toString();
    if (!queryString.isEmpty()) {
        path += "?" + queryString;
    }

    m_networkManager->get(
        path,
        onSuccess,
        onError);
}

void ParkingSpaceManager::getDashboardSummary(
    std::function<void (QJsonObject)> onSuccess,
    std::function<void (QString)> onError)
{
    m_networkManager->get(
        "/reports/dashboard",
        onSuccess,
        onError);
}

void ParkingSpaceManager::getDetailedReport(
    const QString &startDate,
    const QString &endDate,
    const QString &reportType,
    std::function<void (QJsonObject)> onSuccess,
    std::function<void (QString)> onError)
{
    QString path = "/reports/detailed";
    QUrlQuery query;
    query.addQueryItem("startDate", startDate);
    query.addQueryItem("endDate", endDate);
    if (!reportType.isEmpty()) {
        query.addQueryItem("reportType", reportType);
    }
    
    QString queryString = query.toString();
    if (!queryString.isEmpty()) {
        path += "?" + queryString;
    }

    m_networkManager->get(
        path,
        onSuccess,
        onError);
}


