#ifndef INFOMAINWINDOW_H
#define INFOMAINWINDOW_H

#include "submainwindow.h"
#include <QTimer>
#include "kqui_collapsibleframe.h"
#include "kqrowdata.h"

namespace Ui {
	class InfoMainWindow;
}

#define UPDATENEXT_TRYTIME 4

class InfoMainWindow : public SubMainWindow
{
	Q_OBJECT

public:
	explicit InfoMainWindow(QWidget *parent = 0);
	~InfoMainWindow();

	void updateOverviewTable(const QList<QString>& lst, const QList<QColor>& cols, const QString& tip);
	void updateMissionTable(const QString& buttonTitle, const QList<KQRowData>& rows);
    void updateFleetTable(int n, const QString& buttonTitle, int colindex, bool bRed, const QList<KQRowData>& rows, const QString& tip);
	void updateRepairTable(const QString& buttonTitle, const QList<KQRowData>& rows);
	void updateTitle(const QString& title, int colindex);

private:

	void setOverviewColumnFormat();
	void setMissionColumnFormat();
	void setFleetColumnFormat();
	void setFleetColumnFormat(KQTableWidget * pTableWidget);
	void setRepairColumnFormat();

	Ui::InfoMainWindow *ui;

	QTimer * _pUpdateTimer = NULL;

	KQUI_CollapsibleFrame * _pFleetFrames[4];	// should be set

	QList<KQUI_CollapsibleFrame*> _lstCollapsibleFrames;

	bool _needUpdateNext[UPDATENEXT_TRYTIME];// = { true, false, false, false };

	public slots:
	void slotOnTableSizeChanged();

	void onToggleSleepMode(bool bSleep);

	private slots:
	void slotUpdateTimer();
	void on_pbClose_clicked();
	void on_pbMinimize_clicked();

};

#endif // INFOMAINWINDOW_H
