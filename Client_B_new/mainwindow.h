#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QJsonObject>
#include <QTableWidgetItem>
#include "networkmanager.h"
#include "parkingspacemanager.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    // 停车位管理相关槽函数
    void on_btnAddParkingSpace_clicked();
    void on_btnGetParkingSpace_clicked();
    void on_btnGetParkingSpaces_clicked();
    void on_btnUpdateParkingSpace_clicked();
    void on_btnVisualizeParkingSpaces_clicked();
    void handleDeleteParkingSpace();
    // 表格点击事件处理
    void on_tableParkingSpaces_itemClicked(QTableWidgetItem *item);

private:
    Ui::MainWindow *ui;
    NetworkManager *m_networkManager;
    ParkingSpaceManager *m_parkingSpaceManager;

    // 初始化UI
    void initUI();
    // 显示成功信息
    void showSuccessMessage(const QString &message);
    // 显示错误信息
    void showErrorMessage(const QString &message);
};

#endif // MAINWINDOW_H
