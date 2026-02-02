#ifndef TRAFFICCONTROLCENTER_H
#define TRAFFICCONTROLCENTER_H

#include <QObject>
#include <QTimer>
#include <QStackedWidget>

class TrafficControlCenter : public QObject {
    Q_OBJECT
public:
    explicit TrafficControlCenter(QStackedWidget *stack, QObject *parent = nullptr);
    void startMonitoring(QString plate);

private slots:
    void checkStatus();

private:
    QStackedWidget *m_stack;
    QTimer *m_timer;
    QString m_plate;
};

#endif
