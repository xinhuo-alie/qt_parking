#ifndef PARKINGINFOWIDGET_H
#define PARKINGINFOWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QMessageBox>

class ParkingInfoWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ParkingInfoWidget(const QString &plate, int spaceId, QWidget *parent = nullptr);

signals:
    void exitRequested(int spaceId);

private slots:
    void onExitClicked();
    void onNetworkReply(QNetworkReply *reply);

private:
    QString m_currentPlate;
    int m_spaceId;
    
    QLabel *m_titleLabel;
    QLabel *m_locationLabel;
    QLabel *m_typeLabel;
    QLabel *m_rateLabel;
    QLabel *m_occupiedTimeLabel;
    QLabel *m_durationLabel;
    QPushButton *m_exitButton;
    
    QNetworkAccessManager *m_networkManager;
    
    void setupUI();
    void loadSpaceInfo();
    void showMessage(const QString &title, const QString &message, bool isError = false);
};

#endif // PARKINGINFOWIDGET_H