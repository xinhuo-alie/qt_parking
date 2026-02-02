#include "HttpTool.h"

void HttpTool::get(const QString &path, std::function<void(QJsonObject)> onSuccess) {
    static QNetworkAccessManager manager;
    // 使用你文档中的 BASE_URL
    QNetworkRequest req(QUrl("http://127.0.0.1:8080" + path));
    QNetworkReply *reply = manager.get(req);

    QObject::connect(reply, &QNetworkReply::finished, [reply, onSuccess] {
        if (reply->error() == QNetworkReply::NoError) {
            QJsonObject obj = QJsonDocument::fromJson(reply->readAll()).object();
            if (obj["code"].toInt() == 0) onSuccess(obj); // 业务逻辑成功 [cite: 18-19]
        }
        reply->deleteLater();
    });
}
