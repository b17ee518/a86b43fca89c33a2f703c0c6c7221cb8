#include "kansavedata.h"
#include "klog.h"

#include "kandataconnector.h"

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


	filename = "MstShip";
	KLog::getInstance().DeleteLog(filename);
	RECLINE(filename, "Ship\tID\tSortNo\tName\tShipType\tCategory\tRarity");
	for (auto& item : start2data.api_mst_ship)
	{
		QString writestr = QString::fromLocal8Bit("-\t%1\t%2\t%3\t%4\t%5\t%6")
			.arg(item.api_id)
			.arg(item.api_sortno)
			.arg(item.api_name)
			.arg(item.api_stype)
			.arg(item.api_ctype)
			.arg(item.api_backs);

		RECLINE(filename, writestr);
	}

	filename = "MstSlotitem";
	KLog::getInstance().DeleteLog(filename);
	RECLINE(filename, "Ship\tID\tSortNo\tName\tCategory\tGallery\tMstCategory\tIcon\tAirCategory");
	for (auto& item : start2data.api_mst_slotitem)
	{
		QList<int> category = item.api_type;
		while (category.count() < 5)
		{
			category.append(-1);
		}

		QString writestr = QString::fromLocal8Bit("-\t%1\t%2\t%3\t%4\t%5\t%6\t%7\t%8")
			.arg(item.api_id)
			.arg(item.api_sortno)
			.arg(item.api_name)
			.arg(category[0])
			.arg(category[1])
			.arg(category[2])
			.arg(category[3])
			.arg(category[4]);

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
	totalTokyuWin = 0;
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

void KanSaveData::adjustAnyAndSouthEast(int addval)
{
	totalSouthEastWin += addval;
	if (totalSouthEastWin < 0)
	{
		totalSouthEastWin = 0;
	}

	totalAnyCount += addval;
	if (totalAnyCount < 0)
	{
		totalAnyCount = 0;
	}
}

void KanSaveData::clearQuestByType(int type, int beginIndex, int endIndex, int page)
{
	if (!activeQuestData.size())
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
		for (it = activeQuestData.begin(); it != activeQuestData.end();)
		{
			if (it->api_type == type || type == 0 || type == 9)
			{
				count++;
				if (count > page*questPerPage)
				{
					it = activeQuestData.erase(it);
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
	for (it = activeQuestData.begin(); it != activeQuestData.end();)
	{
		if ((it->api_type == type || type == 0 || type == 9)
			&& it->api_no >= beginIndex && it->api_no <= endIndex)
		{
			it = activeQuestData.erase(it);
		}
		else
		{
			++it;
		}
	}

}

void KanSaveData::requireRecordQuestList()
{
	isRecordingLastQuestList = true;
	lastQuestList.api_count = -1;
}

void KanSaveData::recordLastQuestList(const kcsapi_questlist& questList)
{
	if (isRecordingLastQuestList)
	{
		if (questList.api_list.count() > 0)
		{
			lastQuestList = questList;
			isRecordingLastQuestList = false;
		}
	}
}

QList<ShipWithSlotItemsFullData> KanSaveData::createTeamFullData(int team)
{
	QList<ShipWithSlotItemsFullData> datalist;
	for (const auto& deck : portdata.api_deck_port)
	{
		if (deck.api_id == team + 1)
		{
			for (int shipno : deck.api_ship)
			{
				ShipWithSlotItemsFullData data;
				data.InitWithShipAndSlots(shipno);
				datalist.append(data);
			}
		}
	}
	return datalist;
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

void ShipWithSlotItemsFullData::InitWithShipAndSlots(int shipno)
{
	KanDataConnector* pkdc = &KanDataConnector::getInstance();
	pship = pkdc->findShipFromShipno(shipno);
	slotitemlist.clear();
	mstslotitemlist.clear();

	if (pship != NULL)
	{
		pmstship = pkdc->findMstShipFromShipid(pship->api_ship_id);

		for (int i = 0; i < pship->api_slot.count(); i++)
		{
			int slotitemno = pship->api_slot[i];
			if (slotitemno >= 0)
			{
				kcsapi_slotitem* slotitemdata = pkdc->findSlotitemFromId(slotitemno);
				slotitemlist.append(slotitemdata);
				if (slotitemdata != NULL)
				{
					mstslotitemlist.append(pkdc->findMstSlotItemFromSlotitemid(slotitemdata->api_slotitem_id));
				}
				else
				{
					mstslotitemlist.append(NULL);
				}
			}
			else
			{
				slotitemlist.append(NULL);
				mstslotitemlist.append(NULL);
			}
		}
		int slotitemno = pship->api_slot_ex;
		if (slotitemno >= 0)
		{
			kcsapi_slotitem* slotitemdata = pkdc->findSlotitemFromId(slotitemno);
			slotitemlist.append(slotitemdata);
			if (slotitemdata != NULL)
			{
				mstslotitemlist.append(pkdc->findMstSlotItemFromSlotitemid(slotitemdata->api_slotitem_id));
			}
			else
			{
				mstslotitemlist.append(NULL);
			}
		}
		else
		{
			slotitemlist.append(NULL);
			mstslotitemlist.append(NULL);
		}
	}
}
