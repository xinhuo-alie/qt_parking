#ifndef MAINMENU_H
#define MAINMENU_H

#include <QMainWindow>

namespace Ui {
class MainMenu;
}

class MainMenu : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainMenu(QWidget *parent = nullptr);
    ~MainMenu();

private slots:
    // 菜单按钮点击事件
    void on_btnParkingManagement_clicked();
    void on_btnReportStatistics_clicked();

private:
    Ui::MainMenu *ui;
    // 初始化UI
    void initUI();
};

#endif // MAINMENU_H