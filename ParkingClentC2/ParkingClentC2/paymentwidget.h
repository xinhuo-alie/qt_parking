#ifndef PAYMENTWIDGET_H
#define PAYMENTWIDGET_H

#include <QWidget>
#include <QComboBox>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QMessageBox>

class PaymentWidget : public QWidget
{
    Q_OBJECT

public:
    explicit PaymentWidget(const QString &plate, int spaceId, QWidget *parent = nullptr);

signals:
    void paymentCompleted();

private slots:
    void onPayClicked();
    void onNetworkReply(QNetworkReply *reply);

private:
    QString m_currentPlate;
    int m_spaceId;
    int m_recordId;
    double m_fee;
    
    QLabel *m_titleLabel;
    QLabel *m_feeLabel;
    QLabel *m_durationLabel;
    QLabel *m_enterTimeLabel;
    QLabel *m_exitTimeLabel;
    QComboBox *m_paymentMethodCombo;
    QPushButton *m_payButton;
    
    QNetworkAccessManager *m_networkManager;
    
    void setupUI();
    void loadFeeInfo();
    void showMessage(const QString &title, const QString &message, bool isError = false);
};

#endif // PAYMENTWIDGET_H