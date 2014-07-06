#ifndef KQTIMERSUBMAINWINDOW_H
#define KQTIMERSUBMAINWINDOW_H

#include <QMainWindow>
#include "kqsubmainwindowbase.h"

namespace Ui {
class KQTimerSubMainWindow;
}

class KQTimerSubMainWindow : public KQSubMainWindowBase
{
    Q_OBJECT

public:
    explicit KQTimerSubMainWindow(QWidget *parent = 0);
    ~KQTimerSubMainWindow();

private:
    Ui::KQTimerSubMainWindow *ui;
};

#endif // KQTIMERSUBMAINWINDOW_H
