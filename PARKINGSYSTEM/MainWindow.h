#ifndef MainWindow_H
#define MainWindow_H

#include <QMainWindow>
#include "registerpage.h"
#include "waitingpage.h"
#include "exitpage.h"
#include "trafficcontrolcenter.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
private:
    Ui::MainWindow *ui;
    RegisterPage *m_regPage;
    WaitingPage *m_waitPage;
    ExitPage *m_exitPage;
    TrafficControlCenter *m_controller;
};
#endif
