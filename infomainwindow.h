#ifndef INFOMAINWINDOW_H
#define INFOMAINWINDOW_H

#include "submainwindow.h"
#include <QTimer>
#include "kqui_collapsibleframe.h"
#include "kqrowdata.h"

namespace Ui {
class InfoMainWindow;
}

class InfoMainWindow : public SubMainWindow
{
    Q_OBJECT

public:
    explicit InfoMainWindow(QWidget *parent = 0);
    ~InfoMainWindow();

    void updateOverviewTable(QList<QString> lst, QList<QColor> cols);
    void updateMissionTable(QString buttonTitle, QList<KQRowData> rows);
    void updateFleetTable(int n, QString buttonTitle, int colindex, bool bRed, QList<KQRowData> rows);
    void updateRepairTable(QString buttonTitle, QList<KQRowData> rows);

private:

    void setOverviewColumnFormat();
    void setMissionColumnFormat();
    void setFleetColumnFormat();
    void setFleetColumnFormat(KQTableWidget * pTableWidget);
    void setRepairColumnFormat();

    Ui::InfoMainWindow *ui;

    QTimer * pUpdateTimer;

    KQUI_CollapsibleFrame * pFleetFrames[4];

    QList<KQUI_CollapsibleFrame*> lstCollapsibleFrames;

public slots:
    void slotOnTableSizeChanged();

private slots:
    void slotUpdateTimer();
    void on_pbClose_clicked();
    void on_pbMinimize_clicked();
};

#endif // INFOMAINWINDOW_H
