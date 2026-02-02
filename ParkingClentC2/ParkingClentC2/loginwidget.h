#ifndef LOGINWIDGET_H
#define LOGINWIDGET_H

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QMessageBox>

class LoginWidget : public QWidget
{
    Q_OBJECT

public:
    explicit LoginWidget(QWidget *parent = nullptr);

signals:
    void loginSuccess(const QString &plate);

private slots:
    void onLoginClicked();
    void onNetworkReply(QNetworkReply *reply);

private:
    QLineEdit *m_plateInput;
    QPushButton *m_loginButton;
    QNetworkAccessManager *m_networkManager;
    QWidget *m_containerWidget;  // 保存容器widget的引用
    
    void setupUI();
    void showMessage(const QString &title, const QString &message, bool isError = false);
    
protected:
    void resizeEvent(QResizeEvent *event) override;
};

#endif // LOGINWIDGET_H