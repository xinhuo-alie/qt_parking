#include "networkmanager.h"
#include "apiconfig.h"
#include <QNetworkRequest>
#include <QJsonDocument>
#include <QNetworkReply>
#include <QUrl>

NetworkManager::NetworkManager(QObject *parent) : QObject(parent)
{
    manager = new QNetworkAccessManager(this);
}

NetworkManager::~NetworkManager()
{
    delete manager;
}

void NetworkManager::get(
    const QString &path,
    std::function<void (QJsonObject)> onSuccess,
    std::function<void (QString)> onError)
{
    sendRequest(path, QNetworkAccessManager::GetOperation, QJsonObject(), onSuccess, onError);
}

void NetworkManager::post(
    const QString &path,
    const QJsonObject &body,
    std::function<void (QJsonObject)> onSuccess,
    std::function<void (QString)> onError)
{
    sendRequest(path, QNetworkAccessManager::PostOperation, body, onSuccess, onError);
}

void NetworkManager::put(
    const QString &path,
    const QJsonObject &body,
    std::function<void (QJsonObject)> onSuccess,
    std::function<void (QString)> onError)
{
    sendRequest(path, QNetworkAccessManager::PutOperation, body, onSuccess, onError);
}

void NetworkManager::del(
    const QString &path,
    std::function<void (QJsonObject)> onSuccess,
    std::function<void (QString)> onError)
{
    sendRequest(path, QNetworkAccessManager::DeleteOperation, QJsonObject(), onSuccess, onError);
}

void NetworkManager::sendRequest(
    const QString &path,
    QNetworkAccessManager::Operation operation,
    const QJsonObject &body,
    std::function<void (QJsonObject)> onSuccess,
    std::function<void (QString)> onError)
{
    // 构建完整URL
    QString fullUrlStr = ApiConfig::BASE_URL + ApiConfig::API_VERSION + path;
    QUrl url(fullUrlStr);
    
    // 添加调试信息，查看完整URL
    qDebug() << "完整请求URL: " << fullUrlStr;
    qDebug() << "请求路径: " << path;
    qDebug() << "请求操作: " << operation;
    
    QNetworkRequest req(url);
    req.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QNetworkReply *reply = nullptr;
    QJsonDocument doc(body);
    QByteArray data = doc.toJson(QJsonDocument::Compact); // 使用Compact格式，避免不必要的换行符
    
    // 确保JSON数据不包含BOM（字节顺序标记）
    if (data.startsWith("\xEF\xBB\xBF")) {
        data.remove(0, 3);
        qDebug() << "移除了JSON数据中的BOM";
    }
    
    // 添加调试信息，查看发送的JSON数据
    qDebug() << "发送到服务器的JSON数据: " << QString(data);
    qDebug() << "JSON数据长度: " << data.length();
    
    // 添加更多调试信息，查看请求头和请求方法
    qDebug() << "请求URL: " << url.toString();
    qDebug() << "请求方法: " << (operation == QNetworkAccessManager::GetOperation ? "GET" : \
                                    operation == QNetworkAccessManager::PostOperation ? "POST" : \
                                    operation == QNetworkAccessManager::PutOperation ? "PUT" : \
                                    operation == QNetworkAccessManager::DeleteOperation ? "DELETE" : "UNKNOWN");
    qDebug() << "请求头Content-Type: " << req.header(QNetworkRequest::ContentTypeHeader).toString();
    qDebug() << "请求体大小: " << data.size() << "字节";
    qDebug() << "请求体内容: " << data.toHex(' '); // 以十六进制显示请求体内容，确保没有隐藏字符

    switch (operation) {
    case QNetworkAccessManager::GetOperation:
        qDebug() << "执行GET请求";
        reply = manager->get(req);
        break;
    case QNetworkAccessManager::PostOperation:
        qDebug() << "执行POST请求，请求体大小: " << data.size();
        reply = manager->post(req, data);
        break;
    case QNetworkAccessManager::PutOperation:
        qDebug() << "执行PUT请求，请求体大小: " << data.size();
        qDebug() << "PUT请求的完整URL: " << url.toString();
        qDebug() << "PUT请求的请求体: " << QString(data);
        reply = manager->put(req, data);
        break;
    case QNetworkAccessManager::DeleteOperation:
        qDebug() << "执行DELETE请求";
        reply = manager->deleteResource(req);
        break;
    default:
        if (onError) {
            onError("Unsupported operation");
        }
        return;
    }

    // 使用 Qt::UniqueConnection 确保信号只被连接一次
    QObject::connect(reply, &QNetworkReply::finished, this, [reply, onSuccess, onError]() mutable {
        qDebug() << "QNetworkReply::finished signal received";
        // 创建局部变量保存回调，然后立即清空原始回调指针
        auto localOnSuccess = onSuccess;
        auto localOnError = onError;
        onSuccess = nullptr;
        onError = nullptr;
        
        // 先检查是否有网络错误
        if (reply->error() != QNetworkReply::NoError) {
            qDebug() << "Network error:" << reply->errorString();
            if (localOnError) {
                localOnError(reply->errorString());
            }
            reply->deleteLater();
            return;
        }

        QByteArray responseData = reply->readAll();
        reply->deleteLater();

        // 添加调试信息，查看完整响应数据
        qDebug() << "完整响应数据: " << QString(responseData);
        qDebug() << "响应数据长度: " << responseData.length();

        QJsonDocument doc = QJsonDocument::fromJson(responseData);
        if (doc.isNull()) {
            qDebug() << "Invalid JSON response";
            if (localOnError) {
                localOnError("Invalid JSON response");
            }
            return;
        }

        QJsonObject obj = doc.object();
        qDebug() << "Response JSON:" << obj;
        
        if (obj["code"].toInt() == 0) {
            if (localOnSuccess) {
                qDebug() << "Request succeeded, calling onSuccess with full response:" << obj;
                localOnSuccess(obj);
            }
        } else {
            QString errorMsg = obj["msg"].toString();
            qDebug() << "Request failed, calling onError:" << errorMsg;
            if (localOnError) {
                localOnError(errorMsg);
            }
        }
    });
}
