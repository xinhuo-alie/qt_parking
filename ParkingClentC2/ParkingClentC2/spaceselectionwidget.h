#ifndef SPACESELECTIONWIDGET_H
#define SPACESELECTIONWIDGET_H

#include <QWidget>
#include <QGridLayout>
#include <QPushButton>
#include <QLabel>
#include <QTimer>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QMessageBox>
#include <QScrollArea>
#include <QVBoxLayout>
#include "occupyspacedialog.h"

class SpaceSelectionWidget : public QWidget
{
    Q_OBJECT

public:
    explicit SpaceSelectionWidget(const QString &plate, QWidget *parent = nullptr);
    ~SpaceSelectionWidget();

signals:
    void spaceSelected(int spaceId);
    void queueRequested();

private slots:
    void onRefreshSpaces();
    void onNetworkReply(QNetworkReply *reply);
    void onQueueButtonClicked();
    void onOccupySpaceConfirmed();
    void onAvailableSpaceClicked();
    void onMySpaceClicked();private:
    QString m_currentPlate;
    QGridLayout *m_spacesLayout;
    QWidget *m_spacesContainer;
    QScrollArea *m_scrollArea;
    QLabel *m_statusLabel;
    QPushButton *m_queueButton;
    QNetworkAccessManager *m_networkManager;
    QTimer *m_refreshTimer;
    int m_selectedSpaceId;
    QString m_selectedLocation;
    
    void setupUI();
    void updateSpacesDisplay(const QJsonArray &spaces);
    void showMessage(const QString &title, const QString &message, bool isError = false);
};

#endif // SPACESELECTIONWIDGET_H