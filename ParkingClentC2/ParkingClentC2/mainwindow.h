#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStackedWidget>
#include <QString>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class LoginWidget;
class SpaceSelectionWidget;
class ParkingInfoWidget;
class PaymentWidget;
class QueueWidget;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onLoginSuccess(const QString &plate);
    void onSpaceSelected(int spaceId);
    void onExitRequested(int spaceId);
    void onPaymentCompleted();
    void onQueueRequested();
    void onSpaceAssigned(int spaceId);

private:
    Ui::MainWindow *ui;
    QStackedWidget *m_stackedWidget;
    
    LoginWidget *m_loginWidget;
    SpaceSelectionWidget *m_spaceSelectionWidget;
    ParkingInfoWidget *m_parkingInfoWidget;
    PaymentWidget *m_paymentWidget;
    QueueWidget *m_queueWidget;
    
    QString m_currentPlate;
    int m_currentSpaceId;
    
    void setupUI();
    void switchToWidget(QWidget *widget);
};

#endif // MAINWINDOW_H