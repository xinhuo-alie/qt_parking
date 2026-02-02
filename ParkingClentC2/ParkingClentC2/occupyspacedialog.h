#ifndef OCCUPYSPACEDIALOG_H
#define OCCUPYSPACEDIALOG_H

#include <QDialog>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QMessageBox>

class OccupySpaceDialog : public QDialog
{
    Q_OBJECT

public:
    explicit OccupySpaceDialog(const QString &plate, int spaceId, const QString &location, QWidget *parent = nullptr);

signals:
    void occupySuccess();

private slots:
    void onConfirmClicked();
    void onCancelClicked();
    void onNetworkReply(QNetworkReply *reply);

private:
    QString m_currentPlate;
    int m_spaceId;
    QString m_location;
    
    QLabel *m_infoLabel;
    QPushButton *m_confirmButton;
    QPushButton *m_cancelButton;
    
    QNetworkAccessManager *m_networkManager;
    
    void setupUI();
    void showMessage(const QString &title, const QString &message, bool isError = false);
};

#endif // OCCUPYSPACEDIALOG_H