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

    void setData(int team,
		bool bExpedition, 
		const QString& name, 
		int lv, 
		int cond, 
		bool bNeedCharge,
		bool bNeedRepair, 
		int drumCount, 
		int sortnum, 
		int shipid)
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

    void setMetaData(int stype, const QString& stypename)
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

    void updateShipTable(const QList<QString>& lst, const QList<QColor>& cols);

    void clearShipData();
	void addShipData(int stype, 
		const QString& stypename,
		int team, 
		bool bExpedition, 
		const QString& name,
		int lv, 
		int cond, 
		bool bNeedCharge,
		bool bNeedRepair, 
		int drumCount,
		int sortnum,
		int shipid);
	
	void buildTable();

private:

    void buildSingleTable(const UIShipGroupData& groupData);
    void setShipColumnFormat(KQUI_CollapsibleFrame* pFrame);

	QColor getMainColor(const UIShipData& ship);
	QColor getCondColor(const UIShipData& ship, bool* pbKira=NULL);

	void setButtonColor(KQUI_CollapsibleFrame* pFrame, int colindex, bool bRed);

    Ui::ShipMainWindow *ui;

    QTimer * _pUpdateTimer = NULL;

    QList<KQUI_CollapsibleFrame*> _lstCollapsibleFrames;

    QList<UIShipGroupData> _shipGroupList;

	bool _needRebuildTable = true;
	int _lastToggledId = 0;

	bool _doNotRecordLast = false;
	QList<int> _checkedIdList;

public slots:
    void slotOnTableSizeChanged();
	void slotSetVisible(bool bValue);
	void slotToggled(bool bValue);
	void slotTextChanged(const QString& text);

	void onToggleSleepMode(bool bSleep);

private slots:
    void slotUpdateTimer();
    void on_pbClose_clicked();
    void on_pbMinimize_clicked();
};

#endif // ShipMainWindow_H
