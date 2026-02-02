#ifndef NETWORKMANAGER_H
#define NETWORKMANAGER_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QJsonObject>
#include <functional>

class NetworkManager : public QObject
{
    Q_OBJECT
public:
    explicit NetworkManager(QObject *parent = nullptr);
    ~NetworkManager();

    // GET请求
    void get(
        const QString &path,
        std::function<void (QJsonObject)> onSuccess,
        std::function<void (QString)> onError);

    // POST请求
    void post(
        const QString &path,
        const QJsonObject &body,
        std::function<void (QJsonObject)> onSuccess,
        std::function<void (QString)> onError);

    // PUT请求
    void put(
        const QString &path,
        const QJsonObject &body,
        std::function<void (QJsonObject)> onSuccess,
        std::function<void (QString)> onError);

    // DELETE请求
    void del(
        const QString &path,
        std::function<void (QJsonObject)> onSuccess,
        std::function<void (QString)> onError);

private:
    QNetworkAccessManager *manager;

    // 通用请求处理
    void sendRequest(
        const QString &path,
        QNetworkAccessManager::Operation operation,
        const QJsonObject &body,
        std::function<void (QJsonObject)> onSuccess,
        std::function<void (QString)> onError);
};

#endif // NETWORKMANAGER_H
