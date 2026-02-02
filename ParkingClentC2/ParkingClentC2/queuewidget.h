#ifndef QUEUEWIDGET_H
#define QUEUEWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QTimer>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QMessageBox>

class QueueWidget : public QWidget
{
    Q_OBJECT

public:
    explicit QueueWidget(const QString &plate, QWidget *parent = nullptr);
    ~QueueWidget();

signals:
    void spaceAssigned(int spaceId);

private slots:
    void onQueueUpdate();
    void onNetworkReply(QNetworkReply *reply);
    void onCancelQueueClicked();

private:
    QString m_currentPlate;
    int m_queuePosition;
    QString m_queueTime;
    
    QLabel *m_titleLabel;
    QLabel *m_statusLabel;
    QLabel *m_positionLabel;
    QLabel *m_timeLabel;
    QLabel *m_infoLabel;
    QPushButton *m_cancelButton;
    
    QNetworkAccessManager *m_networkManager;
    QTimer *m_updateTimer;
    
    void setupUI();
    void joinQueue();
    void showMessage(const QString &title, const QString &message, bool isError = false);
};

#endif // QUEUEWIDGET_H