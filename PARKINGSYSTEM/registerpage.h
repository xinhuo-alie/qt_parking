#ifndef REGISTERPAGE_H
#define REGISTERPAGE_H

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui { class RegisterPage; }
QT_END_NAMESPACE

class RegisterPage : public QWidget {
    Q_OBJECT
public:
    explicit RegisterPage(QWidget *parent = nullptr);
    ~RegisterPage();

signals:
    void sigStart(QString plate); // 发射车牌号给主窗口

private slots:
    void on_regBtn_clicked(); // 对应 ui 中的按钮

private:
    Ui::RegisterPage *ui;
};

#endif
