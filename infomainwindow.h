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

    void updateOverviewTable(const QList<QString>& lst, const QList<QColor>& cols);
    void updateMissionTable(const QString& buttonTitle, const QList<KQRowData>& rows);
    void updateFleetTable(int n, const QString& buttonTitle, int colindex, bool bRed, const QList<KQRowData>& rows);
    void updateRepairTable(const QString& buttonTitle, const QList<KQRowData>& rows);
    void updateTitle(const QString& title, int colindex);

private:

    void setOverviewColumnFormat();
    void setMissionColumnFormat();
    void setFleetColumnFormat();
    void setFleetColumnFormat(KQTableWidget * pTableWidget);
    void setRepairColumnFormat();

    Ui::InfoMainWindow *ui;

    QTimer * pUpdateTimer = NULL;

	KQUI_CollapsibleFrame * pFleetFrames[4];	// should be set

    QList<KQUI_CollapsibleFrame*> lstCollapsibleFrames;

public slots:
	void slotOnTableSizeChanged();

	void onToggleSleepMode(bool bSleep);

private slots:
    void slotUpdateTimer();
    void on_pbClose_clicked();
    void on_pbMinimize_clicked();
};

#endif // INFOMAINWINDOW_H
