#include "kansavedata.h"
#include "klog.h"

void KanSaveData::logAllExport()
{
	QString filename;
	/*
	filename = "MstMapArea";
	KLog::getInstance().DeleteLog(filename);
	RECLINE(filename, "MapArea\tID\tName\tType");
	for (auto& item:start2data.api_mst_maparea)
	{
		QString writestr = QString::fromLocal8Bit("-\t%1\t%2\t%3")
			.arg(item.api_id)
			.arg(item.api_name)
			.arg(item.api_type);

		RECLINE(filename, writestr);
	}

	filename = "MstMapInfo";
	KLog::getInstance().DeleteLog(filename);
	RECLINE(filename, "MapInfo\tID\tNo\tName\tInfoText\tOpeText");
	for (auto& item : start2data.api_mst_mapinfo)
	{
		QString writestr = QString::fromLocal8Bit("-\t%1\t%2\t%3\t%4\t%5")
			.arg(item.api_id)
			.arg(item.api_no)
			.arg(item.api_name)
			.arg(item.api_infotext)
			.arg(item.api_opetext);

		RECLINE(filename, writestr);
	}


	filename = "MstMapCell";
	KLog::getInstance().DeleteLog(filename);
	RECLINE(filename, "MapCell\tID\tNo\tMapNo\tMapAreaID\tMapInfoNo");
	for (auto& item : start2data.api_mst_mapcell)
	{
		QString writestr = QString::fromLocal8Bit("-\t%1\t%2\t%3\t%4\t%5")
			.arg(item.api_id)
			.arg(item.api_no)
			.arg(item.api_map_no)
			.arg(item.api_maparea_id)
			.arg(item.api_mapinfo_no);

		RECLINE(filename, writestr);
	}
	*/
	filename = "MstStype";
	KLog::getInstance().DeleteLog(filename);
	RECLINE(filename, "Stype\tID\tName");
	for (auto& item : start2data.api_mst_stype)
	{
		QString writestr = QString::fromLocal8Bit("-\t%1\t%2")
			.arg(item.api_id)
			.arg(item.api_name);

		RECLINE(filename, writestr);
	}

	filename = "MstSlotitemType";
	KLog::getInstance().DeleteLog(filename);
	RECLINE(filename, "SlotitemType\tID\tName");
	for (auto& item : start2data.api_mst_slotitem_equiptype)
	{
		QString writestr = QString::fromLocal8Bit("-\t%1\t%2")
			.arg(item.api_id)
			.arg(item.api_name);

		RECLINE(filename, writestr);
	}

	filename = "MstUseitem";
	KLog::getInstance().DeleteLog(filename);
	RECLINE(filename, "Useitem\tID\tName\tUseType\tCategory");
	for (auto& item : start2data.api_mst_useitem)
	{
		QString writestr = QString::fromLocal8Bit("-\t%1\t%2\t%3\t%4")
			.arg(item.api_id)
			.arg(item.api_name)
			.arg(item.api_usetype)
			.arg(item.api_category);

		RECLINE(filename, writestr);
	}
}

void KanSaveData::resetTotals()
{
	totalKilledYusou = 0;
	totalKilledKubou = 0;
	totalKilledSensui = 0;
	totalBossWin = 0;
	totalBossReached = 0;
	totalBossSRank = 0;
	totalSortie = 0;
	totalSouthEastWin = 0;
}

KanSaveData::KanSaveData()
{
    kcsapi_material tm;
    tm.api_value = 0;
    portdata.api_material.append(tm);
    portdata.api_material.append(tm);
    portdata.api_material.append(tm);
    portdata.api_material.append(tm);
    portdata.api_material.append(tm);
    portdata.api_material.append(tm);
    portdata.api_material.append(tm);

    shipcountoffset = 0;
    slotitemcountoffset = 0;
	lastdeckid = 0;

	createshipdata.clearValue();
}

void KanSaveData::adjustSouthEast(int addval)
{
	totalSouthEastWin += addval;
	if (totalSouthEastWin < 0)
	{
		totalSouthEastWin = 0;
	}
}

void KanSaveData::clearQuestByType(int type, int beginIndex, int endIndex, int page)
{
	if (!questdata.size())
	{
		return;
	}

	const int questPerPage = 5;

	// for normal remove beginIndex = 0

	if (beginIndex < 0 && page > 0)
	{
		// remove all out of page
		QList<kcsapi_quest>::iterator it;
		int count = 0;
		for (it = questdata.begin(); it != questdata.end();)
		{
			if (it->api_type == type || type == 0 || type == 9)
			{
				count++;
				if (count > page*questPerPage)
				{
					it = questdata.erase(it);
				}
				else
				{
					++it;
				}
			}
		}
		return;
	}

	if (endIndex < 0)
	{
		endIndex = std::numeric_limits<int>::max();
	}
	
	QList<kcsapi_quest>::iterator it;
	for (it = questdata.begin(); it != questdata.end();)
	{
		if ((it->api_type == type || type == 0 || type == 9)
			&& it->api_no >= beginIndex && it->api_no <= endIndex)
		{
			it = questdata.erase(it);
		}
		else
		{
			++it;
		}
	}

}

void CreateShipSaveData::setValue(int fuel, int bull, int steel, int bauxite, int dev, int kdock)
{
	_usefuel = fuel;
	_usebull = bull;
	_usesteel = steel;
	_usebauxite = bauxite;
	_usedevelopment = dev;
	_kdockid = kdock;
	_flag = 1;
}

void CreateShipSaveData::clearValue()
{
	_flag = 0;
}
