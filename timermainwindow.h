#ifndef TIMERMAINWINDOW_H
#define TIMERMAINWINDOW_H

#include "submainwindow.h"

namespace Ui {
class TimerMainWindow;
}

class TimerMainWindow : public SubMainWindow
{
    Q_OBJECT

public:
    explicit TimerMainWindow(QWidget *parent = 0);
    ~TimerMainWindow();

private slots:
    void on_pbClose_clicked();

    void on_pbMinimize_clicked();

private:
    Ui::TimerMainWindow *ui;
};

#endif // TIMERMAINWINDOW_H
