#ifndef WeaponMainWindow_H
#define WeaponMainWindow_H

#include "submainwindow.h"
#include <QTimer>
#include "kqui_collapsibleframe.h"
#include "kqrowdata.h"

namespace Ui {
class WeaponMainWindow;
}

class UIWeaponData{
public:
    UIWeaponData(){}

    void setData(bool bLocked, int level, QString shipname, int shiplv, int alv, int distance)
    {
        this->bLocked = bLocked;
        this->level = level;
        this->shipname = shipname;
        this->shiplv = shiplv;
		this->alv = alv;
		this->distance = distance;
    }

    bool bLocked = false;
    int level = 0;
    QString shipname;
    int shiplv = 0;
	int alv = 0;
	int distance = 0;
};

class UIWeaponGroupData{
public:
    UIWeaponGroupData(){}

    void setMetaData(int id, QString name, int type, int rare)
    {
        this->id = id;
        this->name = name;
		this->type = type;
        this->rare = rare;
    }

	static bool higherThan(const UIWeaponGroupData& left, const UIWeaponGroupData& right);

    int id = 0;
    QString name;
    int rare = 0;
	int type = 0;

    QList<UIWeaponData> weapons;
};

class WeaponMainWindow : public SubMainWindow
{
    Q_OBJECT

public:
    explicit WeaponMainWindow(QWidget *parent = 0);
    ~WeaponMainWindow();

    void updateWeaponTable(const QList<QString>& lst, const QList<QColor>& cols);

    void clearWeaponData();
	void addWeaponData(int slotitemId, 
		const QString& itemname,
		int type,
		int rare,
		bool bLocked,
		int level, 
		int alv, 
		const QString& shipname,
		int shiplv,
		int distance);

	void buildTable();

private:

    void buildSingleTable(const UIWeaponGroupData& groupData);
	void setWeaponColumnFormat(KQUI_CollapsibleFrame* pFrame);
	void setButtonColor(KQUI_CollapsibleFrame* pFrame, int colindex, bool bRed);

    Ui::WeaponMainWindow *ui;

    QTimer * _pUpdateTimer = NULL;

    QList<KQUI_CollapsibleFrame*> _lstCollapsibleFrames;

    QList<UIWeaponGroupData> _weaponGroupList;

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

#endif // WeaponMainWindow_H
