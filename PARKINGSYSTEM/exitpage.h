#ifndef EXITPAGE_H
#define EXITPAGE_H

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui { class ExitPage; }
QT_END_NAMESPACE

class ExitPage : public QWidget {
    Q_OBJECT
public:
    explicit ExitPage(QWidget *parent = nullptr);
    ~ExitPage();
    void setInfo(QString plate, double money);
signals:
    void sigPayDone();
private slots:
    void on_payBtn_clicked();
private:
    Ui::ExitPage *ui;
};
#endif
