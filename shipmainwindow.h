#ifndef ShipMainWindow_H
#define ShipMainWindow_H

#include "submainwindow.h"
#include <QTimer>
#include "kqui_collapsibleframe.h"
#include "kqrowdata.h"

namespace Ui {
class ShipMainWindow;
}

class UIShipData{
public:
	UIShipData(){}

    void setData(int team, bool bExpedition, QString name, int lv, int cond, bool bNeedCharge, bool bNeedRepair, int drumCount, int sortnum, int shipid)
    {
		this->team = team;
		this->bExpedition = bExpedition;
		this->name = name;
		this->lv = lv;
		this->cond = cond;
		this->bNeedCharge = bNeedCharge;
		this->bNeedRepair = bNeedRepair;
		this->drumCount = drumCount;
		this->sortnum = sortnum;
		this->shipid = shipid;
	}
	static bool higherThan(const UIShipData& left, const UIShipData& right);

	int team = 0;
	bool bExpedition = false;
	QString name;
	int lv = 0;
	int cond = 0;
	bool bNeedCharge = false;
	bool bNeedRepair = false;
	int drumCount = 0;
	int sortnum = 0;
	int shipid = 0;
};

class UIShipGroupData{
public:
	UIShipGroupData(){}

    void setMetaData(int stype, QString stypename)
    {
		this->stype = stype;
		this->stypename = stypename;
    }

	static bool higherThan(const UIShipGroupData& left, const UIShipGroupData& right);

	int stype = 0;
	QString stypename;

    QList<UIShipData> ships;
};

class ShipMainWindow : public SubMainWindow
{
    Q_OBJECT

public:
    explicit ShipMainWindow(QWidget *parent = 0);
    ~ShipMainWindow();

    void updateShipTable(QList<QString> lst, QList<QColor> cols);

    void clearShipData();
	void addShipData(int stype, QString stypename, int team, bool bExpedition, QString name, int lv, int cond, bool bNeedCharge, bool bNeedRepair, int drumCount, int sortnum, int shipid);
	
	void buildTable();

private:

    void buildSingleTable(const UIShipGroupData& groupData);
    void setShipColumnFormat(KQUI_CollapsibleFrame* pFrame);

	QColor getMainColor(UIShipData ship);
	QColor getCondColor(UIShipData ship, bool* pbKira=NULL);

	void setButtonColor(KQUI_CollapsibleFrame* pFrame, int colindex, bool bRed);

    Ui::ShipMainWindow *ui;

    QTimer * pUpdateTimer;

    QList<KQUI_CollapsibleFrame*> lstCollapsibleFrames;

    QList<UIShipGroupData> shipGroupList;

	bool needRebuildTable;
	int lastToggledId;

	bool doNotRecordLast;
	QList<int> checkedIdList;

public slots:
    void slotOnTableSizeChanged();
	void slotSetVisible(bool bValue);
	void slotToggled(bool bValue);
	void slotTextChanged(QString text);

	void onToggleSleepMode(bool bSleep);

private slots:
    void slotUpdateTimer();
    void on_pbClose_clicked();
    void on_pbMinimize_clicked();
};

#endif // ShipMainWindow_H
