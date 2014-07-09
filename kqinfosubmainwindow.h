#ifndef KQINFOSUBMAINWINDOW_H
#define KQINFOSUBMAINWINDOW_H

#include <QMainWindow>
#include "kqsubmainwindowbase.h"

namespace Ui {
class KQInfoSubMainWindow;
}

class KQInfoSubMainWindow : public KQSubMainWindowBase
{
    Q_OBJECT

public:
    explicit KQInfoSubMainWindow(QWidget *parent = 0);
    ~KQInfoSubMainWindow();

private:
    Ui::KQInfoSubMainWindow *ui;

public slots:

protected slots:

    void slotDebugAfterInit();
    void slotDebugAfterInit2();
};

#endif // KQINFOSUBMAINWINDOW_H
