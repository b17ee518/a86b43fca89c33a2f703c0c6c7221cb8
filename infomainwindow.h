#ifndef INFOMAINWINDOW_H
#define INFOMAINWINDOW_H

#include "submainwindow.h"
#include <QTimer>
#include "kqui_collapsibleframe.h"

namespace Ui {
class InfoMainWindow;
}

class InfoMainWindow : public SubMainWindow
{
    Q_OBJECT

public:
    explicit InfoMainWindow(QWidget *parent = 0);
    ~InfoMainWindow();

private:

    void setOverviewColumnFormat();
    void setMissionColumnFormat();
    void setFleetColumnFormat();
    void setFleetColumnFormat(KQTableWidget * pTableWidget);
    void setRepairColumnFormat();

    Ui::InfoMainWindow *ui;

    QTimer * pUpdateTimer;

    QList<KQUI_CollapsibleFrame*> lstCollapsibleFrames;

public slots:
    void slotOnTableSizeChanged();

private slots:
    void slotUpdateTimer();
    void on_pbClose_clicked();
    void on_pbMinimize_clicked();
};

#endif // INFOMAINWINDOW_H
