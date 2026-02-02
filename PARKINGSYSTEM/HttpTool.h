#ifndef HTTPTOOL_H
#define HTTPTOOL_H
#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <functional>

class HttpTool : public QObject {
    Q_OBJECT
public:
    static void get(const QString &path, std::function<void(QJsonObject)> onSuccess);
};
#endif
