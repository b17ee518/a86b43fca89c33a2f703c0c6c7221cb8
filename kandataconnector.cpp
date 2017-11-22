#include "kandataconnector.h"
#include "kansavedata.h"
#include "kandatacalc.h"

#include "mainwindow.h"
#include "klog.h"

#include "ControlManager.h"

void KanDataConnector::getShipColors(const kcsapi_ship2 *pship, QColor *pcolCond/*=0*/, QColor *pcolWound/*=0*/, CondState* pcondstate/*=0*/, WoundState* pwoundstate/*=0*/)
{
	if (pcolCond || pcondstate)
	{
		CondState condstate = KanDataCalc::GetCondState(pship->api_cond);
		QColor col;
		switch (condstate)
		{
		case CondState::Normal:
			col = _colWhite;
			break;
		case CondState::Small:
			col = _colGray;
			break;
		case CondState::Middle:
			col = _colOrange;
			break;
		case CondState::Big:
			col = _colRed;
			break;
		case CondState::Kira:
			col = _colYellow;
			break;
		}
		if (pcolCond)
		{
			*pcolCond = col;
		}
		if (pcondstate)
		{
			*pcondstate = condstate;
		}
	}
	if (pcolWound || pwoundstate)
	{
		WoundState woundstate = KanDataCalc::GetWoundState(pship->api_nowhp, pship->api_maxhp);
		QColor col;
		switch (woundstate)
		{
		case WoundState::Full:
			col = _colWhite;
			break;
		case WoundState::Little:
			col = _colGray;
			break;
		case WoundState::Small:
			col = _colYellow;
			break;
		case WoundState::Middle:
			col = _colOrange;
			break;
		case WoundState::Big:
			col = _colRed;
			break;
		case WoundState::Dead:
			col = _colBlue;
			break;
		}
		if (pcolWound)
		{
			*pcolWound = col;
		}
		if (pwoundstate)
		{
			*pwoundstate = woundstate;
		}
	}
}

void KanDataConnector::getShipChargeColors(const kcsapi_ship2 *pship, const kcsapi_mst_ship *pmstship, QColor *pcolFuel, QColor *pcolBullet)
{
	ChargeState fuelstate = KanDataCalc::GetChargeState(pship->api_fuel, pmstship->api_fuel_max);
	ChargeState bullstate = KanDataCalc::GetChargeState(pship->api_bull, pmstship->api_bull_max);

	ChargeState state = fuelstate;
	QColor * pcol = pcolFuel;

	switch (state)
	{
	case ChargeState::Full:
		*pcol = _colWhite;
		break;
	case ChargeState::Small:
		*pcol = _colGray;
		break;
	case ChargeState::Middle:
		*pcol = _colOrange;
		break;
	case ChargeState::Big:
		*pcol = _colRed;
		break;
	case ChargeState::Empty:
		*pcol = _colBlue;
		break;
	}
	state = bullstate;
	pcol = pcolBullet;

	switch (state)
	{
	case ChargeState::Full:
		*pcol = _colWhite;
		break;
	case ChargeState::Small:
		*pcol = _colGray;
		break;
	case ChargeState::Middle:
		*pcol = _colOrange;
		break;
	case ChargeState::Big:
		*pcol = _colRed;
		break;
	case ChargeState::Empty:
		*pcol = _colBlue;
		break;
	}
}

QString KanDataConnector::getShipWoundStateString(const kcsapi_ship2 *pship)
{
	if (isShipRepairing(pship))
	{
		return QString::fromLocal8Bit("渠");
	}
	WoundState woundstate = KanDataCalc::GetWoundState(pship->api_nowhp, pship->api_maxhp);
	switch (woundstate)
	{
	case WoundState::Full:
		return "";
		break;
	case WoundState::Little:
		return "";
		break;
	case WoundState::Small:
		return QString::fromLocal8Bit("小");
		break;
	case WoundState::Middle:
		return QString::fromLocal8Bit("中");
		break;
	case WoundState::Big:
		return QString::fromLocal8Bit("大");
		break;
	case WoundState::Dead:
		return QString::fromLocal8Bit("沈");
		break;
	}
	return "";
}

bool KanDataConnector::isShipRepairing(const kcsapi_ship2 *pship)
{
	KanSaveData * pksd = &KanSaveData::getInstance();
	foreach(const kcsapi_ndock &v, pksd->portdata.api_ndock)
	{
		if (v.api_ship_id == pship->api_id)
		{
			return true;
		}
	}
	return false;
}

bool KanDataConnector::isAutoRepairing(int flagshipno/*=-1*/)
{
	if (flagshipno < 0)
	{
		KanSaveData * pksd = &KanSaveData::getInstance();
		if (pksd->portdata.api_deck_port.size() > 0)
		{
			auto& shiplist = pksd->portdata.api_deck_port.at(0).api_ship;
			if (shiplist.size())
			{
				flagshipno = shiplist.first();
			}
		}
	}
	if (isShipHasSlotitem(flagshipno, SlotitemType::KanSyu))
	{
		return true;
	}
	return false;
}

bool KanDataConnector::RemoveShip(int shipno)
{
	for (QList<kcsapi_ship2>::iterator it = pksd->portdata.api_ship.begin();
		it != pksd->portdata.api_ship.end(); ++it)
	{
		if (it->api_id == shipno)
		{
			foreach(auto itemid, it->api_slot)
			{
				RemoveSlotItem(itemid);
			}
			RemoveSlotItem(it->api_slot_ex);
			pksd->portdata.api_ship.erase(it);
			return true;
		}
	}
	return false;
}

bool KanDataConnector::RemoveSlotItem(int id)
{
	for (auto it = pksd->slotitemdata.begin();
		it != pksd->slotitemdata.end(); ++it)
	{
		if (it->api_id == id)
		{
			pksd->slotitemdata.erase(it);
			return true;
		}
	}
	return false;
}

bool KanDataConnector::AddShip(const kcsapi_ship2& ship)
{
	if (findShipFromShipno(ship.api_id))
	{
		return false;
	}
	pksd->portdata.api_ship.push_back(ship);
	return true;
}

bool KanDataConnector::AddShip(int shipid, const QList<int>& slotitems)
{
	int newid = 0;
	foreach(auto& it, pksd->portdata.api_ship)
	{
		if (it.api_id > newid)
		{
			newid = it.api_id;
		}
	}
	newid++;
	kcsapi_ship2 ship;
	auto pmstship = findMstShipFromShipid(shipid);
	if (pmstship)
	{
		if (ship.ReadFromMstShip(*pmstship, newid))
		{
			int i = 0;
			foreach(auto slotitemid, slotitems)
			{
				ship.api_slot[i] = slotitemid;
				i++;
			}
			return AddShip(ship);
		}
	}
	return false;
}

bool KanDataConnector::AddSlotItem(const kcsapi_slotitem& item)
{
	if (findSlotitemFromId(item.api_id))
	{
		return false;
	}
	pksd->slotitemdata.push_back(item);
	if (item.api_id > pksd->maxslotitemid)
	{
		pksd->maxslotitemid = item.api_id;
	}
	return true;
}

bool KanDataConnector::AddSlotItem(int id, int slotitemId)
{
	kcsapi_slotitem slotitem;
	if (id < 0)
	{
		id = pksd->maxslotitemid + 1;
	}
	slotitem.api_id = id;
	slotitem.api_slotitem_id = slotitemId;
	slotitem.api_locked = 0;
	slotitem.api_level = 0;

	return AddSlotItem(slotitem);
}

kcsapi_ship2 *KanDataConnector::findShipFromShipno(int shipno)
{
	for (QList<kcsapi_ship2>::iterator it = pksd->portdata.api_ship.begin();
		it != pksd->portdata.api_ship.end(); ++it)
	{
		if (it->api_id == shipno)
		{
			return &(*it);
		}
	}
	qDebug("No ship data.");
	return 0;
}

const kcsapi_mst_ship *KanDataConnector::findMstShipFromShipid(int shipid) const
{
	foreach(const kcsapi_mst_ship &v, pksd->start2data.api_mst_ship)
	{
		if (v.api_id == shipid)
		{
			return &v;
		}
	}
	qDebug("No mst ship data.");
	return 0;
}

const QString KanDataConnector::findMstShipTypeNameFromSType(int stype) const
{
	foreach(auto& v, pksd->start2data.api_mst_stype)
	{
		if (v.api_id == stype)
		{
			return v.api_name;
		}
	}
	qDebug("No mst stype data.");
	return "";
}

const Api_Mst_Mission *KanDataConnector::findMstMissionFromMissionid(int missionid) const
{
	foreach(const Api_Mst_Mission &v, pksd->start2data.api_mst_mission)
	{
		if (v.api_id == missionid)
		{
			return &v;
		}
	}
	qDebug("No mst mission data.");
	return 0;
}

kcsapi_slotitem * KanDataConnector::findSlotitemFromId(int id)
{
	for (auto it = pksd->slotitemdata.begin();
		it != pksd->slotitemdata.end(); ++it)
	{
		if (it->api_id == id)
		{
			return &(*it);
		}
	}
	qDebug("No slotitem data.");
	return 0;
}

const kcsapi_mst_slotitem * KanDataConnector::findMstSlotItemFromSlotitemid(int slotitemid) const
{
	foreach(const kcsapi_mst_slotitem &v, pksd->start2data.api_mst_slotitem)
	{
		if (v.api_id == slotitemid)
		{
			return &v;
		}
	}
	qDebug("No mst slotitem data.");
	return 0;
}


bool KanDataConnector::isShipHasSlotitem(const kcsapi_ship2* pship, SlotitemType sitype, int count/*=1*/, int* hasCount/* = NULL*/)
{
	if (!pship)
	{
		return false;
	}
	if (count <= 0 && !hasCount)
	{
		return true;
	}

	int nowCount = 0;
	QList<int> fullSlot = pship->api_slot;
	fullSlot.append(pship->api_slot_ex);
	for (auto slotitemid : fullSlot)
	{
		if (slotitemid >= 0)
		{
			auto pslotitem = findSlotitemFromId(slotitemid);
			if (pslotitem)
			{
				auto pmstslotitem = findMstSlotItemFromSlotitemid(pslotitem->api_slotitem_id);
				if (pmstslotitem)
				{
					if (pmstslotitem->api_type.count() > 2)
					{
						int type = pmstslotitem->api_type[2];
						if ((int)sitype == type)
						{
							nowCount++;
							if (nowCount >= count && !hasCount)
							{
								return true;
							}
						}
					}
				}
			}
		}
	}
	if (hasCount)
	{
		*hasCount = nowCount;
	}
	if (nowCount >= count)
	{
		return true;
	}
	return false;
}

void KanDataConnector::checkWoundQuit()
{
	int lastdeckid = pksd->lastdeckid;
	bool bClose = false;
	if (lastdeckid >= 0 && lastdeckid < 4)
	{
		foreach(int shipno, pksd->portdata.api_deck_port[lastdeckid].api_ship)
		{
			if (shipno > 0)
			{
				const kcsapi_ship2 * pship = findShipFromShipno(shipno);
				if (pship)
				{
					if (KanDataCalc::GetWoundState(pship->api_nowhp, pship->api_maxhp) > WoundState::Middle)
					{
						if (pship->api_locked > 0 || pship->api_lv > 2)
						{
							bClose = true;
							break;
						}
					}
				}
			}
		}
	}
	if (pksd->bCombinedSelf && !bClose)
	{
		int combineddeckid = lastdeckid + 1;
		if (combineddeckid >= 0 && combineddeckid < 4)
		{
			foreach(int shipno, pksd->portdata.api_deck_port[combineddeckid].api_ship)
			{
				if (shipno > 0)
				{
					const kcsapi_ship2 * pship = findShipFromShipno(shipno);
					if (pship)
					{
						if (KanDataCalc::GetWoundState(pship->api_nowhp, pship->api_maxhp) > WoundState::Middle)
						{
							if (pship->api_locked > 0 || pship->api_lv > 2)
							{
								bClose = true;
								break;
							}
						}
					}
				}
			}
		}
	}
	if (bClose)
	{
		MainWindow::mainWindow()->close();
	}
}

void KanDataConnector::checkAirBase()
{
	pksd->airBaseNeedSupply = false;
	pksd->airBaseBadCond = false;
	for (const auto& corps : pksd->airbasedata)
	{
		for (const auto& corp : corps)
		{
			if (corp.api_rid < 0)
			{
				continue;
			}
			for (const auto& plane : corp.api_plane_info)
			{
				if (plane.api_squadron_id < 0)
				{
					continue;
				}
				if (plane.api_count < plane.api_max_count)
				{
					pksd->airBaseNeedSupply = true;
				}
				if (plane.api_cond > 1)
				{
					pksd->airBaseBadCond = true;
				}

				if (pksd->airBaseBadCond && pksd->airBaseNeedSupply)
				{
					break;
				}
			}
			if (pksd->airBaseBadCond && pksd->airBaseNeedSupply)
			{
				break;
			}
		}
	}

	// update team
	updateFleetTable();
}

QString KanDataConnector::logBattleResult(bool bWrite/*=true*/)
{
	int maparea_id = pksd->nextdata.api_maparea_id;
	int mapinfo_no = pksd->nextdata.api_mapinfo_no;

	bool bWon = false;
	bool bSWon = false;
	QString winRankString = pksd->battleresultdata.api_win_rank;
	if (winRankString.contains('S') || winRankString.contains('A') || winRankString.contains('B'))
	{
		bWon = true;
		if (winRankString.contains('S'))
		{
			bSWon = true;
		}
	}
	if (pksd->wasLastBossCell && bWrite)
	{
		if (bWon)
		{
			if (bSWon)
			{
				pksd->totalBossSRank++;
			}
			pksd->totalBossWin++;
			if (maparea_id == 2 && mapinfo_no > 1)
			{
				ControlManager& cm = ControlManager::getInstance();
				if (!cm.isRunning()
					|| !cm.isSouthEastMode() && !cm.isFuelMode()
					|| cm.getSouthEastSetting().stopWhen != ControlManager::StopWhen::Yusou3)
				{
					pksd->totalSouthEastWin++;
				}
			}
			else if (maparea_id == 5 && mapinfo_no == 4)
			{
				pksd->totalTokyuWin++;
			}
		}
		pksd->totalBossReached++;
	}

	ControlManager& cm = ControlManager::getInstance();
	// add count
	if (cm.isAnyMode() && bWrite)
	{
		const auto& anySetting = cm.getAnySetting();
		auto it = anySetting.cells.find(pksd->nextdata.api_no);
		if (it != anySetting.cells.end())
		{
			if (it->bCount)
			{
				if (it->bKillFlagship)
				{
					if (pksd->lastFlagshipKilled)
					{
						pksd->totalAnyCount++;
					}
				}
				else if (it->bNeedS)
				{
					if (bSWon)
					{
						pksd->totalAnyCount++;
					}
				}
				else
				{
					pksd->totalAnyCount++;
				}
			}
		}
	}

	if (bWrite)
	{
		pksd->totalKilledYusou += pksd->lastKilledYusou;
		pksd->totalKilledKubou += pksd->lastKilledKubou;
		pksd->totalKilledSensui += pksd->lastKilledSensui;

		pksd->wasLastBossCell = false;
		pksd->lastWonAssumption = false;
		pksd->lastKilledYusou = 0;
		pksd->lastKilledKubou = 0;
		pksd->lastKilledSensui = 0;
	}

	QString mapareastr = "";
	QString mapinfostr = "";
	foreach(const Api_Mst_Maparea &v, pksd->start2data.api_mst_maparea)
	{
		if (v.api_id == maparea_id)
		{
			mapareastr = v.api_name;
		}
	}

	bool useLast = false;
	if (pksd->lastbattletype == KanBattleType::DayToNight
		|| pksd->lastbattletype == KanBattleType::NightToDay
		|| pksd->lastbattletype == KanBattleType::Combined_DayToNight
		//		|| pksd->lastbattletype == KanBattleType::Combined_KouKuNight
		|| pksd->lastbattletype == KanBattleType::Combined_ECNight)
	{
		useLast = true;
	}

	int seiku = pksd->battledata.api_kouku.api_stage1.api_disp_seiku;
	if (useLast)
	{
		seiku = pksd->lastSeiku;
	}
	QString seikustr = "-";
	switch (seiku)
	{
	case (int)SeiKuState::KaKuHou:
		seikustr = QString::fromLocal8Bit("制空権確保");
		break;
	case (int)SeiKuState::YuSei:
		seikustr = QString::fromLocal8Bit("航空優勢");
		break;
	case (int)SeiKuState::KoRe:
		seikustr = QString::fromLocal8Bit("航空均衡/劣勢");
		break;
	case (int)SeiKuState::SouShiTsu:
		seikustr = QString::fromLocal8Bit("制空権喪失");
		break;
	}

	QString sformstr = "-";
	int sform = -1;
	if (useLast)
	{
		sform = pksd->lastSFormation;
	}
	else if (pksd->battledata.api_formation.count() > 0)
	{
		sform = pksd->battledata.api_formation[0];
	}
	sformstr = getFormationStr(sform);
	QString eformstr = "-";
	int eform = -1;
	if (useLast)
	{
		eform = pksd->lastEFormation;
	}
	else if (pksd->battledata.api_formation.count() > 1)
	{
		eform = pksd->battledata.api_formation[1];
	}
	eformstr = getFormationStr(eform);
	QString interceptstr = "-";

	int intercept = -1;
	if (useLast)
	{
		intercept = pksd->lastIntercept;
	}
	else if (pksd->battledata.api_formation.count() > 2)
	{
		intercept = pksd->battledata.api_formation[2];
	}
	switch (intercept)
	{
	case 1:
		interceptstr = QString::fromLocal8Bit("同航戦");
		break;
	case 2:
		interceptstr = QString::fromLocal8Bit("反航戦");
		break;
	case 3:
		interceptstr = QString::fromLocal8Bit("Ｔ字戦(有利)");
		break;
	case 4:
		interceptstr = QString::fromLocal8Bit("Ｔ字戦(不利)");
		break;
	}

	QString mvpstr = "-";
	QString mvp_combinedstr = "-";
	int mvp = pksd->battleresultdata.api_mvp;
	int mvp_combined = pksd->battleresultdata.api_mvp_combined;
	if (mvp > 0 && pksd->lastdeckid >= 0 && pksd->lastdeckid < 4)
	{
		int shipno = pksd->portdata.api_deck_port[pksd->lastdeckid].api_ship[mvp - 1];
		if (shipno > 0)
		{
			const kcsapi_ship2 * pship = findShipFromShipno(shipno);
			if (pship)
			{
				const kcsapi_mst_ship * pmstship = findMstShipFromShipid(pship->api_ship_id);
				if (pmstship)
				{
					mvpstr = pmstship->api_name;
				}
			}
		}
	}
	if (mvp_combined > 0)
	{
		int shipno = pksd->portdata.api_deck_port[1].api_ship[mvp_combined - 1];
		if (shipno > 0)
		{
			const kcsapi_ship2 * pship = findShipFromShipno(shipno);
			if (pship)
			{
				const kcsapi_mst_ship * pmstship = findMstShipFromShipid(pship->api_ship_id);
				if (pmstship)
				{
					mvp_combinedstr = pmstship->api_name;
				}
			}
		}
	}


	QString dropstr = "-";
	if (pksd->battleresultdata.api_get_ship.api_ship_id > 0)
	{
		dropstr = pksd->battleresultdata.api_get_ship.api_ship_name;
	}

	QString battletypestr = "-";
	switch (pksd->lastbattletype)
	{
	case KanBattleType::Day:
		battletypestr = QString::fromLocal8Bit("昼戦");
		break;
	case KanBattleType::Night:
		battletypestr = QString::fromLocal8Bit("夜戦");
		break;
	case KanBattleType::DayToNight:
		battletypestr = QString::fromLocal8Bit("昼夜");
		break;
	case KanBattleType::NightToDay:
		battletypestr = QString::fromLocal8Bit("夜昼");
		break;
	case KanBattleType::Air:
		battletypestr = QString::fromLocal8Bit("昼空");
		break;
	case KanBattleType::Combined_Water:
		battletypestr = QString::fromLocal8Bit("連航水");
		break;
	case KanBattleType::Combined_KouKu:
		battletypestr = QString::fromLocal8Bit("連航");
		break;
		/*
	case KanBattleType::Combined_KouKuNight:
	battletypestr = QString::fromLocal8Bit("連航夜");
	break;
	*/
	case KanBattleType::Combined_Night:
		battletypestr = QString::fromLocal8Bit("連夜");
		break;
	case KanBattleType::Combined_Day:
		battletypestr = QString::fromLocal8Bit("連昼");
		break;
	case KanBattleType::Combined_DayToNight:
		battletypestr = QString::fromLocal8Bit("連昼夜");
		break;
	case KanBattleType::Combined_EC:
		battletypestr = QString::fromLocal8Bit("敵連");
		break;
	case KanBattleType::Combined_ECNight:
		battletypestr = QString::fromLocal8Bit("敵連夜");
		break;
	case KanBattleType::Combined_Each:
		battletypestr = QString::fromLocal8Bit("連連");
		break;
	case KanBattleType::Combined_EachWater:
		battletypestr = QString::fromLocal8Bit("連連水");
		break;
	case KanBattleType::Combined_ECNightToDay:
		battletypestr = QString::fromLocal8Bit("敵連夜昼");
		break;
	}

	QString writestr = mapareastr + "\t"
		+ pksd->battleresultdata.api_quest_name + "\t"
		+ pksd->battleresultdata.api_enemy_info.api_deck_name + "\t"
		+ battletypestr + "\t"
		+ pksd->battleresultdata.api_win_rank + "\t"
		+ dropstr + "\t"
		+ seikustr + "\t"
		+ sformstr + "\t"
		+ eformstr + "\t"
		+ interceptstr + "\t"
		+ mvpstr + "\t"
		+ mvp_combinedstr + "\t";

	if (bWrite)
	{
		QString filename = QString("Result_%1_%2").arg(maparea_id).arg(mapinfo_no);
		RECLOG(filename, writestr);
		// for total info
		updateOverviewTable();
	}

	return writestr;
}

QString KanDataConnector::getFormationStr(int formation)
{
	switch (formation)
	{
	case 1:
		return QString::fromLocal8Bit("単縦陣");
		break;
	case 2:
		return QString::fromLocal8Bit("複縦陣");
		break;
	case 3:
		return QString::fromLocal8Bit("輪形陣");
		break;
	case 4:
		return QString::fromLocal8Bit("梯形陣");
		break;
	case 5:
		return QString::fromLocal8Bit("単横陣");
		break;

	case 11:
		return QString::fromLocal8Bit("第一警戒航行序列");
		break;
	case 12:
		return QString::fromLocal8Bit("第二警戒航行序列");
		break;
	case 13:
		return QString::fromLocal8Bit("第三警戒航行序列");
		break;
	case 14:
		return QString::fromLocal8Bit("第四警戒航行序列");
		break;
	}
	return "-";
}


void KanDataConnector::logBattleDetail(bool bCombined)
{
	QString infoline = logBattleResult(false);

	QList<const kcsapi_ship2*> sships;
	QList<const kcsapi_mst_ship*> eships;
	if (pksd->lastdeckid >= 0 && pksd->lastdeckid < 4)
	{
		foreach(int shipno, pksd->portdata.api_deck_port[pksd->lastdeckid].api_ship)
		{
			if (shipno > 0)
			{
				const kcsapi_ship2 * pship = findShipFromShipno(shipno);
				if (pship)
				{
					sships.append(pship);
				}
			}
		}
		if (bCombined)
		{
			foreach(int shipno, pksd->portdata.api_deck_port[1].api_ship)
			{
				if (shipno > 0)
				{
					const kcsapi_ship2 * pship = findShipFromShipno(shipno);
					if (pship)
					{
						sships.append(pship);
					}
				}
			}
		}
	}
	foreach(int shipid, pksd->battledata.api_ship_ke)
	{
		if (shipid > 0)
		{
			const kcsapi_mst_ship * pmstship = findMstShipFromShipid(shipid);
			if (pmstship)
			{
				eships.append(pmstship);
			}
		}
	}
	foreach(int shipid, pksd->battledata.api_ship_ke_combined)
	{
		if (shipid > 0)
		{
			const kcsapi_mst_ship * pmstship = findMstShipFromShipid(shipid);
			if (pmstship)
			{
				eships.append(pmstship);
			}
		}
	}


	QString shipnameline = QString::fromLocal8Bit("\t艦名:\t");
	QString lvline = QString::fromLocal8Bit("\tLv:\t");
	QList<QString> equiplines;
	QString beforehpline = QString::fromLocal8Bit("\t前HP:\t");
	QString afterhpline = QString::fromLocal8Bit("\t後HP:\t");
	QString nenryoline = QString::fromLocal8Bit("\t燃料:\t");
	QString danyakuline = QString::fromLocal8Bit("\t弾薬:\t");
	QString condline = QString::fromLocal8Bit("\tコンデ:\t");
	QString enemynameline = QString::fromLocal8Bit("\t敵艦:\t");
	QString ehpline = QString::fromLocal8Bit("\t敵残HP:\t");

	for (int i = 0; i < 6; i++)
	{
		equiplines.append(QString::fromLocal8Bit("\t装備%1\t").arg(i + 1));
	}

	int shipcount = 0;
	foreach(const kcsapi_ship2 * pship, sships)
	{
		const kcsapi_mst_ship * pmstship = findMstShipFromShipid(pship->api_ship_id);
		if (pmstship)
		{
			shipnameline += pmstship->api_name + "\t";
		}
		lvline += QString("%1").arg(pship->api_lv) + "\t";

		QList<int> fullSlot = pship->api_slot;
		fullSlot.append(pship->api_slot_ex);
		for (int i = 0; i < fullSlot.count(); i++)
		{
			QString strslotitemname = "-";

			if (fullSlot[i] > 0)
			{
				const kcsapi_slotitem * pslotitem = findSlotitemFromId(fullSlot[i]);
				if (pslotitem)
				{
					const kcsapi_mst_slotitem * pmstslotitem = findMstSlotItemFromSlotitemid(pslotitem->api_slotitem_id);
					if (pmstslotitem)
					{
						strslotitemname = pmstslotitem->api_name;
					}
				}
			}
			equiplines[i] += strslotitemname + "\t";
		}

		if (pksd->battledata.api_f_nowhps.count() > shipcount)
		{
			beforehpline += QString("%1").arg(pksd->battledata.api_f_nowhps[shipcount]) + "\t";
			afterhpline += QString("%1").arg(pship->api_nowhp) + "\t";
			if (pmstship)
			{
				nenryoline += QString("%1%").arg(pship->api_fuel * 100 / pmstship->api_fuel_max) + "\t";
				danyakuline += QString("%1%").arg(pship->api_bull * 100 / pmstship->api_bull_max) + "\t";
			}
			condline += QString("%1").arg(pship->api_cond) + "\t";
		}

		shipcount++;
	}

	int i = 0;
	foreach(const kcsapi_mst_ship * pmstship, eships)
	{
		enemynameline += pmstship->api_name + "\t";
		int ehp = 0;
		if (i >= 6)
		{
			ehp = pksd->remainLastBattleHPs.combinedEnemy[i - 6];
		}
		else
		{
			ehp = pksd->remainLastBattleHPs.enemy[i];
		}

		if (ehp < 0)
		{
			ehp = 0;
		}
		ehpline += QString("%1").arg(ehp) + "\t";
		i++;
	}

	QString writestr = infoline + "\n"
		+ shipnameline + "\n"
		+ lvline + "\n";
	foreach(const QString &str, equiplines)
	{
		writestr += str + "\n";
	}
	writestr += beforehpline + "\n"
		+ afterhpline + "\n"
		+ nenryoline + "\n"
		+ danyakuline + "\n"
		+ condline + "\n"
		+ enemynameline + "\n"
		+ ehpline + "\n";

	int maparea_id = pksd->nextdata.api_maparea_id;
	int mapinfo_no = pksd->nextdata.api_mapinfo_no;
	QString filename = QString("Detail_%1_%2").arg(maparea_id).arg(mapinfo_no);
	RECLOG(filename, writestr);
}

void KanDataConnector::logBuildResult()
{
	if (pksd->createshipdata.isAll30())
	{
		return;
	}


	int kdockid = pksd->createshipdata._kdockid - 1;
	if (kdockid >= 0 && pksd->kdockdata.count() > kdockid)
	{
		QString writestr;

		int shipid = pksd->kdockdata[kdockid].api_created_ship_id;
		const kcsapi_mst_ship *pmstship = findMstShipFromShipid(shipid);

		writestr += pmstship->api_name + "\t";

		writestr += getLogDevLeadStr();// + "\t";

		QString filename = QString("BuildLog_%1_%2_%3_%4_%5")
			.arg(pksd->createshipdata._usefuel)
			.arg(pksd->createshipdata._usebull)
			.arg(pksd->createshipdata._usesteel)
			.arg(pksd->createshipdata._usebauxite)
			.arg(pksd->createshipdata._usedevelopment);

		RECLOG(filename, writestr);
	}

}

void KanDataConnector::logCreateItemResult(int slotitemid, int fuel, int bull, int steel, int bauxite)
{
	QString writestr;


	QString slotitemnamestr;
	if (slotitemid > 0)
	{
		const kcsapi_mst_slotitem * pmstslotitem = findMstSlotItemFromSlotitemid(slotitemid);
		if (pmstslotitem)
		{
			slotitemnamestr = pmstslotitem->api_name;
		}
	}
	else
	{
		slotitemnamestr = QString::fromLocal8Bit("ペンギン");
	}
	writestr += slotitemnamestr + "\t";
	writestr += getLogDevLeadStr();

	QString filename = QString("CreateItemLog_%1_%2_%3_%4")
		.arg(fuel)
		.arg(bull)
		.arg(steel)
		.arg(bauxite);

	RECLOG(filename, writestr);
}

QString KanDataConnector::getLogDevLeadStr()
{
	QString writestr;

	QString leadnamestr;
	QString leadlvstr;
	int shipno = pksd->portdata.api_deck_port[0].api_ship[0];
	if (shipno > 0)
	{
		const kcsapi_ship2 * pleadship = findShipFromShipno(shipno);
		if (pleadship)
		{
			const kcsapi_mst_ship * pleadmstship = findMstShipFromShipid(pleadship->api_ship_id);
			if (pleadmstship)
			{
				leadnamestr += pleadmstship->api_name;
			}
			leadlvstr = QString("%1").arg(pleadship->api_lv);
		}
	}

	writestr += leadnamestr + "\t" + leadlvstr + "\t";
	writestr += QString("%1").arg(pksd->portdata.api_basic.api_level);
	return writestr;
}
