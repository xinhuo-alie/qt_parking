#ifndef WAITINGPAGE_H
#define WAITINGPAGE_H

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui { class WaitingPage; }
QT_END_NAMESPACE

class WaitingPage : public QWidget {
    Q_OBJECT
public:
    explicit WaitingPage(QWidget *parent = nullptr);
    ~WaitingPage();
    void setQueueLabel(int num);
private:
    Ui::WaitingPage *ui;
};
#endif
