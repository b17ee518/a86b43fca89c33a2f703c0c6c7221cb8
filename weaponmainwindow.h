﻿#ifndef WeaponMainWindow_H
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

    void setData(bool bLocked, int level, QString shipname, int shiplv, int alv)
    {
        this->bLocked = bLocked;
        this->level = level;
        this->shipname = shipname;
        this->shiplv = shiplv;
		this->alv = alv;
    }

    bool bLocked = false;
    int level = 0;
    QString shipname;
    int shiplv = 0;
	int alv = 0;
};

class UIWeaponGroupData{
public:
    UIWeaponGroupData(){}

    void setMetaData(int id, QString name, int rare)
    {
        this->id = id;
        this->name = name;
        this->rare = rare;
    }

	static bool higherThan(const UIWeaponGroupData& left, const UIWeaponGroupData& right);

    int id = 0;
    QString name;
    int rare = 0;

    QList<UIWeaponData> weapons;
};

class WeaponMainWindow : public SubMainWindow
{
    Q_OBJECT

public:
    explicit WeaponMainWindow(QWidget *parent = 0);
    ~WeaponMainWindow();

    void updateWeaponTable(QList<QString> lst, QList<QColor> cols);

    void clearWeaponData();
	void addWeaponData(int slotitemId, QString itemname, int rare, bool bLocked, int level, int alv, QString shipname, int shiplv);

	void buildTable();

private:

    void buildSingleTable(const UIWeaponGroupData& groupData);
	void setWeaponColumnFormat(KQUI_CollapsibleFrame* pFrame);
	void setButtonColor(KQUI_CollapsibleFrame* pFrame, int colindex, bool bRed);

    Ui::WeaponMainWindow *ui;

    QTimer * pUpdateTimer;

    QList<KQUI_CollapsibleFrame*> lstCollapsibleFrames;

    QList<UIWeaponGroupData> weaponGroupList;

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

#endif // WeaponMainWindow_H
