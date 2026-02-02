#include "trafficcontrolcenter.h"
#include "HttpTool.h"
#include <QJsonArray>
#include <QJsonObject>

TrafficControlCenter::TrafficControlCenter(QStackedWidget *stack, QObject *parent)
    : QObject(parent), m_stack(stack)
{
    m_timer = new QTimer(this);
    connect(m_timer, &QTimer::timeout, this, &TrafficControlCenter::checkStatus);
}

void TrafficControlCenter::startMonitoring(QString plate) {
    m_plate = plate;
    m_timer->start(3000); // 3秒轮询一次
    checkStatus();
}

void TrafficControlCenter::checkStatus() {
    HttpTool::get("/api/cars", [this](QJsonObject res){
        QJsonArray cars = res["data"].toArray();
        bool found = false;
        for (auto v : cars) {
            QJsonObject car = v.toObject();
            if (car["plate"].toString() == m_plate) {
                found = true;
                int spaceId = car["spaceId"].toInt();
                // 根据 spaceId 切换 StackedWidget 的页面
                if (spaceId <= 0) m_stack->setCurrentIndex(1); // 等候页
                else m_stack->setCurrentIndex(2);             // 计费页
                break;
            }
        }
        if (!found) m_stack->setCurrentIndex(0); // 找不到回登记页
    });
}
