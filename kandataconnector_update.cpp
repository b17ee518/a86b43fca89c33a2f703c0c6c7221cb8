#include "kandataconnector.h"
#include "kandata.h"
#include "kansavedata.h"

#include "infomainwindow.h"
#include "mainwindow.h"
#include "ControlManager.h"
#include <cmath>

#include "klog.h"

#include <QApplication>

void KanDataConnector::updateOverviewTable()
{
	QList<QString> lst;
	QList<QColor> cols;
	QString tip;
	KanSaveData * pksd = &KanSaveData::getInstance();

	int kancount = pksd->portdata.api_ship.count() + pksd->shipcountoffset;
	int kanmaxcount = pksd->portdata.api_basic.api_max_chara;
	int slotitemcount = pksd->slotitemdata.count() + pksd->slotitemcountoffset;
	int slotitemmaxcount = pksd->portdata.api_basic.api_max_slotitem;
	int instantrepaircount = pksd->portdata.api_material[(int)MaterialDataIndex::InstantRepair].api_value;
	//	int instantbuildcount = pksd->portdata.api_material[(int)MaterialDataIndex::InstandBuild].api_value;
	int lv = pksd->portdata.api_basic.api_level;
	int nextexp = KanDataCalc::GetAdmiralNextLevelExp(pksd->portdata.api_basic.api_experience, lv);
	//	int fcoin = pksd->portdata.api_basic.api_fcoin;

	bool isPortDirty = ControlManager::getInstance().isPortDataDirty();
	QString dirtyStr = isPortDirty ? QString::fromLocal8Bit("■") : "";

	lst.append(QString::fromLocal8Bit("所有艦娘数: %1/%2").arg(kancount).arg(kanmaxcount));
	lst.append(QString::fromLocal8Bit("所有装備数: %1/%2").arg(slotitemcount).arg(slotitemmaxcount));
	lst.append(QString::fromLocal8Bit("提督Lv. %1(次:%2) %3").arg(lv).arg(nextexp).arg(dirtyStr));
	lst.append(QString::fromLocal8Bit("高速修復材: %1").arg(instantrepaircount));



	//	lst.append(QString::fromLocal8Bit("高速建造材: %1").arg(instantbuildcount));
	//	lst.append(QString::fromLocal8Bit("家具コイン: %1").arg(fcoin));
	lst.append(QString::fromLocal8Bit("ボス: %1(%2/%3) 南西: %4 輸: %5 潜: %6 東急: %7 Any: %8")
		.arg(pksd->totalBossReached)
		.arg(pksd->totalBossWin)
		.arg(pksd->totalBossSRank)
		.arg(pksd->totalSouthEastWin)
		.arg(pksd->totalKilledYusou)
		.arg(pksd->totalKilledSensui)
		.arg(pksd->totalTokyuWin)
		.arg(pksd->totalAnyCount));

	tip = QString::fromLocal8Bit("航: %1 出撃: %2")
		.arg(pksd->totalKilledKubou)
		.arg(pksd->totalSortie);

	//
	if (kanmaxcount == kancount)
	{
		cols.append(_colRed);
	}
	else if (kanmaxcount - kancount < 5)
	{
		cols.append(_colOrange);
	}
	else
	{
		cols.append(_colWhite);
	}
	//
	if (slotitemmaxcount == slotitemcount)
	{
		cols.append(_colRed);
	}
	else if (slotitemmaxcount - slotitemcount < 20)
	{
		cols.append(_colOrange);
	}
	else
	{
		cols.append(_colWhite);
	}
	//
	if (isPortDirty)
	{
		cols.append(_colPurple);
	}
	else
	{
		cols.append(_colWhite);
	}
	cols.append(_colWhite);

	if (pksd->totalSouthEastWin >= 5)
	{
		cols.append(_colYellow);
	}
	else if (pksd->totalKilledYusou >= 3)
	{
		cols.append(_colAqua);
	}
	else if (pksd->totalBossWin >= 12
		&& pksd->totalBossReached >= 24
		&& pksd->totalBossSRank >= 6
		&& pksd->totalSortie >= 36)
	{
		cols.append(_colYellow);
	}
	else
	{
		cols.append(_colWhite);
	}

	MainWindow::infoWindow()->updateOverviewTable(lst, cols, tip);
}

void KanDataConnector::updateMissionTable()
{
	KanSaveData * pksd = &KanSaveData::getInstance();

	QList<KQRowData> rows;

	int questcount = pksd->questdata.count();
	for (int i = 0; i < questcount; i++)
	{
		KQRowData rd;
		rd.appendCell(KQRowCellData(" - "));
		rd.appendCell(KQRowCellData(pksd->questdata[i].api_title, pksd->questdata[i].api_detail, _colWhite));

		QString progress;
		bool bDone = false;
		switch (pksd->questdata[i].api_state)
		{
		case 1:
			qDebug("Quest not taken.");
			break;
		case 2:
			break;
		case 3:
			bDone = true;
			progress = QString::fromLocal8Bit("(完遂)");
			break;
		}
		if (!bDone)
		{
			switch (pksd->questdata[i].api_progress_flag)
			{
			case 0:
				progress = "(0%)";
				break;
			case 1:
				progress = "(50%)";
				break;
			case 2:
				progress = "(80%)";
				break;
			}
		}

		rd.appendCell(KQRowCellData(progress));
		rows.append(rd);
	}

	MainWindow::infoWindow()->updateMissionTable(QString::fromLocal8Bit("遂行中任務(%1)").arg(questcount), rows);
}

bool repairTableSort(const kcsapi_ship2 &left, const kcsapi_ship2 &right)
{
	return left.api_ndock_time > right.api_ndock_time;
}

void KanDataConnector::updateRepairTable()
{
	KanSaveData * pksd = &KanSaveData::getInstance();
	QList<kcsapi_ship2> lstShips;
	foreach(const kcsapi_ship2 &ship2, pksd->portdata.api_ship)
	{
		if (ship2.api_ndock_time && !isShipRepairing(&ship2))
		{
			lstShips.append(ship2);
		}
	}
	std::sort(lstShips.begin(), lstShips.end(), repairTableSort);

	int repaircount = lstShips.count();
	QList<KQRowData> rows;
	foreach(const kcsapi_ship2 &v, lstShips)
	{
		// Name cond
		// Lv cond
		// Cond cond
		// Time cond
		// HP w
		// State w
		int shipid = v.api_ship_id;
		const kcsapi_mst_ship * pmstship = findMstShipFromShipid(shipid);

		QColor colCond;
		QColor colWound;
		getShipColors(&v, &colCond, &colWound);
		QString woundstatestr = getShipWoundStateString(&v);

		KQRowData rd;
		rd.appendCell(KQRowCellData(pmstship->api_name, colCond));
		rd.appendCell(KQRowCellData(QString::fromLocal8Bit("Lv.%1").arg(v.api_lv), colCond));
		rd.appendCell(KQRowCellData(QString::fromLocal8Bit("(%1)").arg(v.api_cond), colCond));
		KQTime t;
		t.setTotalMS(v.api_ndock_time);
		rd.appendCell(KQRowCellData(t.toString(), colCond));
		rd.appendCell(KQRowCellData(QString::fromLocal8Bit("(%1/%2)").arg(v.api_nowhp).arg(v.api_maxhp), colWound));
		rd.appendCell(KQRowCellData(woundstatestr, colWound));

		rows.append(rd);
	}

	MainWindow::infoWindow()->updateRepairTable(QString::fromLocal8Bit("入渠まちーズ(%1)").arg(repaircount), rows);
}

void KanDataConnector::updateFleetTable()
{
	if (!pksd->portdata.api_ship.size())
	{
		return;
	}

	MainWindow::shipWindow()->clearShipData();

	KanSaveData * pksd = &KanSaveData::getInstance();
	foreach(const kcsapi_deck &v, pksd->portdata.api_deck_port)
	{
		// No.
		// Name cond
		// Lv cond
		// Cond cond
		// Exp cond
		// Fuel f
		// Bullet b
		// Hp w
		// State w
		QList<KQRowData> rows;
		bool bRed = false;
		int shipcount = 0;

		int kiracount = 0;
		bool bNeedCharge = false;

		bool bCondDown = false;

		int alllv = 0;
		int flagshipLv = 0;
		int areasp33mul = 1;

		//		int sakuteki_teisatsu = 0;
		//		int sakuteki_dentan = 0;

		int keijuncount = 0;
		int kuchikucount = 0;
		int drumcount = 0;
		int drumedshipcount = 0;

		bool bFlagshipKira = false;

		foreach(int shipno, v.api_ship)
		{
			if (shipno <= 0)
			{
				break;
			}
			shipcount++;
			KQRowData rd;

			const kcsapi_ship2 * pship = findShipFromShipno(shipno);
			if (!pship)
			{
				continue;
			}
			int shipid = pship->api_ship_id;
			const kcsapi_mst_ship * pmstship = findMstShipFromShipid(shipid);

			QColor colCond;
			QColor colWound;
			QColor colFuel;
			QColor colBullet;

			CondState condstate;
			WoundState woundstate;

			int damekonstate = 0;
			int shireibustate = 0;
			int hokyustate = 0;


			getShipColors(pship, &colCond, &colWound, &condstate, &woundstate);
			if (pmstship)
			{
				getShipChargeColors(pship, pmstship, &colFuel, &colBullet);
			}

			if (condstate > CondState::Small)
			{
				bCondDown = true;
			}
			if (woundstate > WoundState::Middle)
			{
				bRed = true;
			}

			int nextexp = pship->api_exp[1];
			QString woundstatestr = getShipWoundStateString(pship);

			if (condstate == CondState::Kira)
			{
				kiracount++;
				if (shipcount == 1)
				{
					bFlagshipKira = true;
				}
			}
			if (!bNeedCharge && pmstship)
			{
				if (pship->api_fuel != pmstship->api_fuel_max || pship->api_bull != pmstship->api_bull_max)
				{
					bNeedCharge = true;
				}
			}

			if (pmstship)
			{
				if (pmstship->api_stype == (int)ShipType::KeiJun)
				{
					keijuncount++;
				}
				else if (pmstship->api_stype == (int)ShipType::KuChiKu)
				{
					kuchikucount++;
				}
			}

			QList<int> slotitems = pship->api_slot;
			slotitems.append(pship->api_slot_ex);

			for (int i = 0; i < slotitems.count(); i++)
			{
				int slotitemid = slotitems[i];
				bool bHaveDrum = false;
				foreach(const kcsapi_slotitem &v, pksd->slotitemdata)
				{
					if (v.api_id == slotitemid)
					{
						const kcsapi_mst_slotitem* pmstslotitem = findMstSlotItemFromSlotitemid(v.api_slotitem_id);
						if (pmstslotitem)
						{
							if (pmstslotitem->api_type.count() > 2)
							{
								// ok to cast even if not exist
								SlotitemType type = static_cast<SlotitemType>(pmstslotitem->api_type[2]);

								// drum
								if (type == SlotitemType::YuSou)
								{
									drumcount++;
									bHaveDrum = true;
								}
								else if (type == SlotitemType::OuKyu)
								{
									if (pmstslotitem->api_name == QString::fromLocal8Bit("応急修理女神"))
									{
										damekonstate = 2;
									}
									else if (damekonstate < 1)
									{
										damekonstate = 1;
									}
								}
								else if (type == SlotitemType::ShiReiBu)
								{
									shireibustate = 1;
								}
								else if (type == SlotitemType::HoKyuBuShi)
								{
									hokyustate = 1;
								}
							}
						}
					}
				}
				if (bHaveDrum)
				{
					drumedshipcount++;
				}
			}
			alllv += pship->api_lv;
			if (flagshipLv <= 0)
			{
				flagshipLv = pship->api_lv;
			}

			QString damekonstr = "";
			if (damekonstate == 1)
			{
				damekonstr = QString::fromLocal8Bit("ダ");
			}
			else if (damekonstate == 2)
			{
				damekonstr = QString::fromLocal8Bit("女");
			}

			if (shireibustate)
			{
				damekonstr += QString::fromLocal8Bit("司");
			}

			if (hokyustate)
			{
				damekonstr += QString::fromLocal8Bit("補");
			}

			rd.appendCell(KQRowCellData(QString("%1:").arg(shipcount)));
			rd.appendCell(KQRowCellData(pmstship ? pmstship->api_name : "", colCond)); // tooltip?
			rd.appendCell(KQRowCellData(QString("Lv.%1").arg(pship->api_lv), colCond));
			rd.appendCell(KQRowCellData(QString("(%1)").arg(pship->api_cond), colCond));
			rd.appendCell(KQRowCellData(QString::fromLocal8Bit("次:%1").arg(nextexp), colCond));
			rd.appendCell(KQRowCellData(QString::fromLocal8Bit("燃"), colFuel));
			rd.appendCell(KQRowCellData(QString::fromLocal8Bit("弾"), colBullet));
			rd.appendCell(KQRowCellData(QString("(%1/%2)").arg(pship->api_nowhp).arg(pship->api_maxhp), colWound));
			rd.appendCell(KQRowCellData(woundstatestr, colWound));
			rd.appendCell(KQRowCellData(damekonstr, _colYellow));
			rows.append(rd);
		}

		// 2-5:1 / 3-5:4 / 6-1:4 / 6-2:3 / 6-3:3
		auto setting = ControlManager::getInstance().getAnySetting();
		int mapSakuBorder = 0;
		int mapMinSakuBorder = 0;

		if (setting.area == 1)
		{
			if (setting.map == 6)
			{
				areasp33mul = 3;
				mapSakuBorder = 30;
			}
		}
		else if (setting.area == 2)
		{
			if (setting.map == 5)
			{
				areasp33mul = 1;
				mapSakuBorder = 33;
			}
		}
		else if (setting.area == 3)
		{
			if (setting.map == 5)
			{
				areasp33mul = 4;
				mapSakuBorder = 28;
			}
		}
		else if (setting.area == 6)
		{
			if (setting.map == 1)
			{
				areasp33mul = 4;
				mapSakuBorder = 25;
				mapMinSakuBorder = 16;
			}
			else if (setting.map == 2)
			{
				areasp33mul = 3;
				mapSakuBorder = 50;
				mapMinSakuBorder = 40;
			}
			else if (setting.map == 3)
			{
				areasp33mul = 3;
				mapSakuBorder = 38;
			}
			else if (setting.map == 5)
			{
				areasp33mul = 3;
				mapSakuBorder = 50;
			}
		}

		int colindex = 0;
		if (bNeedCharge || bCondDown)
		{
			// need hokyu
			colindex = 5;
		}
		else if (kiracount == 6)
		{
			// 6 kira
			colindex = 2;
		}
		else if (kiracount >= 4
			&& keijuncount == 1
			&& kuchikucount == 5
			&& drumcount >= 6
			&& drumedshipcount >= 3
			&& flagshipLv >= 50
			&& alllv >= 200)
		{
			// toukyu 1
			colindex = 3;
		}
		else if (kiracount >= 4
			&& kuchikucount >= 5
			&& drumcount >= 10
			&& drumedshipcount >= 4
			&& flagshipLv >= 65
			&& alllv >= 240)
		{
			// toukyu 2
			colindex = 3;
		}
		else if (kiracount >= 4
			&& kuchikucount >= 3
			&& drumcount >= 4
			&& drumedshipcount >= 3
			&& flagshipLv >= 50
			&& alllv >= 200)
		{
			colindex = 4;
		}
		else if (kiracount && bFlagshipKira)
		{
			colindex = 1;
		}

		while (pksd->deckSaveData.size() < v.api_id)
		{
			pksd->deckSaveData.append(KanSaveData::DeckSaveData());
		}

		// do not use above calc
		QList<ShipWithSlotItemsFullData> teamFullData = pksd->createTeamFullData(v.api_id - 1);
		int minTyku = 0;
		int maxTyku = 0;
		KanDataCalc::CalcTeamTyku(teamFullData, minTyku, maxTyku, KanDataCalc::LandBaseState::None);
		double totalSaku = KanDataCalc::CalcTeamSakuTeki(teamFullData, pksd->portdata.api_basic.api_level, areasp33mul);

		KanSaveData::DeckSaveData* pdsd = &(pksd->deckSaveData[v.api_id - 1]);
		pdsd->totalLevel = alllv;
		pdsd->totalMinTaiku = minTyku;
		pdsd->totalMaxTaiku = maxTyku;
		pdsd->totalSaku = totalSaku;

		QString strtitle = QString::fromLocal8Bit("%1 (Lv計:%2 制空:%3 索敵:%4")
			.arg(v.api_name)
			.arg(alllv)
			.arg(minTyku)
			.arg(totalSaku, 0, 'f', 2);

		if (mapSakuBorder > 0)
		{
			if (mapMinSakuBorder > 0)
			{
				strtitle += QString::fromLocal8Bit("[%1~%2]")
					.arg(mapMinSakuBorder)
					.arg(mapSakuBorder);
			}
			else
			{
				strtitle += QString::fromLocal8Bit("[%1]").arg(mapSakuBorder);
			}
		}

		strtitle += ")";

		if (areasp33mul > 1)
		{
			strtitle += QString::fromLocal8Bit("×%1").arg(areasp33mul);
		}


		if (v.api_id == 1 && (pksd->airBaseBadCond || pksd->airBaseNeedSupply))
		{
			strtitle += QString::fromLocal8Bit(" --航基 ");
			if (pksd->airBaseBadCond)
			{
				strtitle += QString::fromLocal8Bit("疲");
			}
			if (pksd->airBaseNeedSupply)
			{
				strtitle += QString::fromLocal8Bit("補");
				bRed = true;
			}
		}

		MainWindow::infoWindow()->updateFleetTable(v.api_id - 1, strtitle, colindex, bRed, rows);
	}

	MainWindow::shipWindow()->buildTable();
}

void KanDataConnector::updateExpeditionTable()
{
	KanSaveData * pksd = &KanSaveData::getInstance();
	foreach(const kcsapi_deck &v, pksd->portdata.api_deck_port)
	{
		if (v.api_id <= 1)
		{
			continue;
		}
		if (v.api_mission[0] <= 0)
		{
			MainWindow::timerWindow()->setExpeditionTime(v.api_id - 2);
		}
		else
		{
			const Api_Mst_Mission *pMission = findMstMissionFromMissionid(v.api_mission[1]);
			if (pMission)
			{
				qint64 missiontotalms = pMission->api_time * 60000;

				QColor col;
				switch (pMission->api_maparea_id)
				{
				case 1:
					col.setRgb(224, 255, 255);
					break;
				case 2:
					col.setRgb(134, 255, 134);
					break;
				case 3:
					col.setRgb(102, 153, 255);
					break;
				case 4:
					col.setRgb(221, 151, 254);
					break;
				case 5:
					col.setRgb(60, 179, 113);
					break;
				default:
					col.setRgb(252, 127, 107);
					break;
				}

				MainWindow::timerWindow()->setExpeditionTime(v.api_id - 2, v.api_mission[2], missiontotalms, pMission->api_name, col);
			}
		}
	}

}

void KanDataConnector::updateRepairDockTable()
{
	KanSaveData * pksd = &KanSaveData::getInstance();
	foreach(const kcsapi_ndock &v, pksd->portdata.api_ndock)
	{
		if (v.api_ship_id == 0)
		{
			MainWindow::timerWindow()->setRepairTime(v.api_id - 1);
		}
		else
		{
			int shipno = v.api_ship_id;
			if (shipno > 0)
			{
				const kcsapi_ship2 *pship = findShipFromShipno(shipno);
				if (pship)
				{
					const kcsapi_mst_ship *pmstship = findMstShipFromShipid(pship->api_ship_id);
					if (pmstship)
					{
						MainWindow::timerWindow()->setRepairTime(v.api_id - 1, v.api_complete_time, pship->api_ndock_time, QString("%1 Lv.%2").arg(pmstship->api_name).arg(pship->api_lv));
					}
				}
			}

		}
	}

}

void KanDataConnector::updateBuildDockTable()
{
	KanSaveData * pksd = &KanSaveData::getInstance();
	foreach(const kcsapi_kdock &v, pksd->kdockdata)
	{
		if (v.api_state < 0)
		{
			continue;
		}
		if (v.api_created_ship_id == 0)
		{
			MainWindow::timerWindow()->setBuildTime(v.api_id - 1);
		}
		else
		{
			int shipid = v.api_created_ship_id;
			//            const kcsapi_ship2 *pship = findShipFromShipno(shipno);
			const kcsapi_mst_ship *pmstship = findMstShipFromShipid(shipid);
			if (pmstship != NULL)
			{
				MainWindow::timerWindow()->setBuildTime(v.api_id - 1, v.api_complete_time, pmstship->api_buildtime, pmstship->api_name);
			}
		}
	}

}

void KanDataConnector::updateInfoTitleBattle(bool bBattle, bool bSelfDamaged)
{
	KanSaveData * pksd = &KanSaveData::getInstance();
	/*
	if (pksd->nextdata.api_no < 0 && !enemyhps)
	{
	MainWindow::infoWindow()->updateTitle("", false);
	return;
	}
	*/
	QString strtitle;

	if (pksd->nextdata.api_no >= 0)
	{
		strtitle = QString::fromLocal8Bit("現:%1 ボス:%2")
			.arg(pksd->nextdata.api_no)
			.arg(pksd->nextdata.api_bosscell_no);
		if (!bBattle && pksd->nextdata.api_eventmap.api_max_maphp > 0)
		{
			strtitle += QString::fromLocal8Bit(" (%1/%2, DMG:%3)")
				.arg(pksd->nextdata.api_eventmap.api_now_maphp)
				.arg(pksd->nextdata.api_eventmap.api_max_maphp)
				.arg(pksd->nextdata.api_eventmap.api_dmg);
		}
	}

	if (bBattle)
	{
		pksd->lastWonAssumption = false;
		pksd->lastFlagshipKilled = false;

		int totalremain = 0;
		int acremain = 0;
		int actotal = 0;
		int transremain = 0;
		int transtotal = 0;
		int subremain = 0;
		int subtotal = 0;

		int totalecount = 0;

		int combined_totalcount = 0;
		int combined_remain = 0;

		// only consider enemy team 1
		for (int i = 0; i < pksd->battledata.api_ship_ke.count(); i++)
		{
			int shipid = pksd->battledata.api_ship_ke[i];
			if (shipid > 0)
			{
				const kcsapi_mst_ship * pmstship = findMstShipFromShipid(shipid);
				if (pmstship)
				{
					totalecount++;
					if (pksd->remainLastBattleHPs.enemy[i] > 0)
					{
						totalremain++;
					}
					else if (i == 0)
					{
						pksd->lastWonAssumption = true;
						pksd->lastFlagshipKilled = true;
					}
					switch (static_cast<ShipType>(pmstship->api_stype))
					{
					case ShipType::KuBou:
					case ShipType::KeiKuBou:
					case ShipType::SouKaKuBou:
						actotal++;
						if (pksd->remainLastBattleHPs.enemy[i] > 0)
						{
							acremain++;
						}
						break;
					case ShipType::HoKyu:
					case ShipType::HoKyu_HaYaSuI:
						transtotal++;
						if (pksd->remainLastBattleHPs.enemy[i] > 0)
						{
							transremain++;
						}
						break;
					case ShipType::SenSui:
					case ShipType::SenBou:
						subtotal++;
						if (pksd->remainLastBattleHPs.enemy[i] > 0)
						{
							subremain++;
						}
						break;
					}

				}
			}
		}
		// only consider enemy team 1
		for (int i = 0; i < pksd->battledata.api_ship_ke_combined.count(); i++)
		{
			int shipid = pksd->battledata.api_ship_ke_combined[i];
			if (shipid > 0)
			{
				const kcsapi_mst_ship * pmstship = findMstShipFromShipid(shipid);
				if (pmstship)
				{
					combined_totalcount++;
					if (pksd->remainLastBattleHPs.combinedEnemy[i] > 0)
					{
						combined_remain++;
					}
					switch (static_cast<ShipType>(pmstship->api_stype))
					{
					case ShipType::KuBou:
					case ShipType::KeiKuBou:
					case ShipType::SouKaKuBou:
						actotal++;
						if (pksd->remainLastBattleHPs.combinedEnemy[i] > 0)
						{
							acremain++;
						}
						break;
					case ShipType::HoKyu:
					case ShipType::HoKyu_HaYaSuI:
						transtotal++;
						if (pksd->remainLastBattleHPs.combinedEnemy[i] > 0)
						{
							transremain++;
						}
						break;
					case ShipType::SenSui:
					case ShipType::SenBou:
						subtotal++;
						if (pksd->remainLastBattleHPs.combinedEnemy[i] > 0)
						{
							subremain++;
						}
						break;
					}

				}
			}
		}

		QString eflagshipremainstr = "";
		if (pksd->remainLastBattleHPs.enemy.count() > 0 &&
			pksd->remainLastBattleHPs.enemy[0] > 0)
		{
			eflagshipremainstr = QString::fromLocal8Bit("旗");
		}
		QString eCombinedRemainStr = "";
		if (combined_totalcount > 0)
		{
			eCombinedRemainStr = QString::fromLocal8Bit("(%1)").arg(combined_remain);
		}

		if (!pksd->lastWonAssumption)
		{
			if (totalremain == 0 || !bSelfDamaged)
			{
				pksd->lastWonAssumption = true;
			}
			else
			{
				switch (totalecount)
				{
				case 2:
				case 3:
					if (totalremain <= 1)
					{
						pksd->lastWonAssumption = true;
					}
					break;
				case 4:
				case 5:
				case 6:
					if (totalremain <= 2)
					{
						pksd->lastWonAssumption = true;
					}
					break;
				default:
					break;
				}
			}
		}

		if (!pksd->lastWonAssumption && totalecount > totalremain)
		{
			int selfTotalDamage = 0;
			int enemyTotalDamage = 0;
			int selfBeginTotalHP = 0;
			int enemyBeginTotalHP = 0;

			int sCount = pksd->beginLastBattleHPs.self.count();
			//			int sCombinedCount = pksd->beginLastBattleHPs.combinedSelf.count();
			int eCount = pksd->beginLastBattleHPs.enemy.count();
			if (sCount == pksd->remainLastBattleHPs.self.count()
				//				&& sCombinedCount == pksd->remainLastBattleHPs.combinedSelf.count()
				&& eCount == pksd->remainLastBattleHPs.enemy.count())
			{
				for (int i = 0; i < sCount; i++)
				{
					selfTotalDamage += pksd->beginLastBattleHPs.self[i] - pksd->remainLastBattleHPs.self[i];
					selfBeginTotalHP += pksd->beginLastBattleHPs.self[i];
				}
				/*
				for (int i = 1; i < sCombinedCount; i++)
				{
				selfTotalDamage += pksd->beginLastBattleHPs.combinedSelf[i] - pksd->remainLastBattleHPs.combinedSelf[i];
				}
				*/
				for (int i = 0; i < eCount; i++)
				{
					enemyTotalDamage += pksd->beginLastBattleHPs.enemy[i] - pksd->remainLastBattleHPs.enemy[i];
					enemyBeginTotalHP += pksd->beginLastBattleHPs.enemy[i];
				}

				if (((int)(enemyTotalDamage * 100.0 / enemyBeginTotalHP)) >
					((int)(selfTotalDamage * 100.0 / selfBeginTotalHP))*2.5)
				{
					pksd->lastWonAssumption = true;
				}
			}
		}

		QString loseStr = "";
		if (!pksd->lastWonAssumption)
		{
			loseStr = QString::fromLocal8Bit("敗");
		}

		QString bossDamagedStr = "";
		if (pksd->battledata.api_boss_damaged > 0)
		{
			bossDamagedStr = QString::fromLocal8Bit("★");
		}
		loseStr = bossDamagedStr + loseStr;
		strtitle += QString::fromLocal8Bit(" - %1残:%2%3%4, 輸%5(%6), 航:%7(%8), 潜%9(%10)")
			.arg(loseStr)
			.arg(totalremain)
			.arg(eflagshipremainstr)
			.arg(eCombinedRemainStr)
			.arg(transremain)
			.arg(transtotal)
			.arg(acremain)
			.arg(actotal)
			.arg(subremain)
			.arg(subtotal);

		pksd->wasLastBossCell = (pksd->nextdata.api_no == pksd->nextdata.api_bosscell_no || pksd->nextdata.api_color_no == 5);

		// special for calc 5-4
		/*
		if (pksd->nextdata.api_maparea_id == 5 && pksd->nextdata.api_mapinfo_no == 4)
		{
		if (pksd->nextdata.api_no == 19)
		{
		pksd->wasLastBossCell = true;
		}
		}
		*/

		pksd->lastKilledYusou = transtotal - transremain;
		pksd->lastKilledKubou = actotal - acremain;
		pksd->lastKilledSensui = subtotal - subremain;
	}

	int colindex = 0;
	if (pksd->nextdata.api_no == pksd->nextdata.api_bosscell_no || pksd->nextdata.api_color_no == 5)
	{
		colindex = 1;
	}
	MainWindow::infoWindow()->updateTitle(strtitle, colindex);
}

void KanDataConnector::updateInfoTitleCond()
{
	KanSaveData * pksd = &KanSaveData::getInstance();
	if (pksd->nextdata.api_no >= 0)
	{
		return;
	}

	int suibocount = 0;
	int suibokiras = 0;
	int suibokirawarnings = 0;
	int suibokirafinalwarnings = 0;
	int keijuncount = 0;
	int keijunkiras = 0;
	int kuchikucount = 0;
	int kuchikukiras = 0;

	int lvmin = 20;

	bool hasKiraFile = false;
	if (QFile::exists(QApplication::applicationDirPath() + "/action/" + "import_kira.table"))
	{
		hasKiraFile = true;
	}
	QList<int> rawKiraList = ControlManager::getInstance().LoadRawKiraListForExpedition();

	foreach(const kcsapi_ship2 &v, pksd->portdata.api_ship)
	{
		if (!rawKiraList.contains(v.api_id))
		{
			continue;
		}
		int shipid = v.api_ship_id;
		if (shipid > 0)
		{
			const kcsapi_mst_ship * pmstship = findMstShipFromShipid(shipid);
			if (pmstship)
			{
				if (pmstship->api_stype == (int)ShipType::SuiBou
					|| pmstship->api_stype == (int)ShipType::YouRiKu)
				{
					if (v.api_locked && v.api_lv >= lvmin && isShipHasSlotitem(&v, SlotitemType::JouRiKuTei))
					{
						suibocount++;
						if (KanDataCalc::GetCondState(v.api_cond) == CondState::Kira)
						{
							suibokiras++;
						}
						if (KanDataCalc::GetCondState(v.api_cond - 13) != CondState::Kira)
						{
							suibokirawarnings++;
						}
						if (KanDataCalc::GetCondState(v.api_cond - 3) != CondState::Kira)
						{
							suibokirafinalwarnings++;
						}
					}
				}
				if (pmstship->api_stype == (int)ShipType::KeiJun)
				{
					if (v.api_locked && v.api_lv >= lvmin)
					{
						keijuncount++;
						if (KanDataCalc::GetCondState(v.api_cond) == CondState::Kira)
						{
							keijunkiras++;
						}
					}
				}
				if (pmstship->api_stype == (int)ShipType::KuChiKu)
				{
					if (v.api_locked && v.api_lv >= lvmin && isShipHasSlotitem(&v, SlotitemType::YuSou))
					{
						kuchikucount++;
						if (KanDataCalc::GetCondState(v.api_cond) == CondState::Kira)
						{
							kuchikukiras++;
						}
					}
				}
			}
		}
	}

	QString hasKiraWarn = "";
	if (hasKiraFile)
	{
		hasKiraWarn = "*";
	}
	QString titlestr = QString::fromLocal8Bit("キラ%1 - 水母:%2(%3/%4/%5) 軽巡:%6(%7) 駆逐:%8(%9)")
		.arg(hasKiraWarn)
		.arg(suibokiras)
		.arg(suibocount - suibokiras)
		.arg(suibokirawarnings)
		.arg(suibokirafinalwarnings)
		.arg(keijunkiras)
		.arg(keijuncount - keijunkiras)
		.arg(kuchikukiras)
		.arg(kuchikucount - kuchikukiras);

	int colindex = 0;
	if (suibokiras == suibocount && kuchikukiras >= 18 && keijunkiras >= 4 && suibokirawarnings == 0)
	{
		colindex = 1;
	}
	else if (suibokiras < suibocount || kuchikukiras < 10 || keijunkiras < 3)
	{
		colindex = 2;
	}
	else if (suibokirafinalwarnings > 0)
	{
		colindex = 4;
	}
	else if (suibokirawarnings > 0)
	{
		colindex = 3;
	}
	MainWindow::infoWindow()->updateTitle(titlestr, colindex);
}

void KanDataConnector::updateWeaponTable()
{
	if (!pksd->portdata.api_ship.size())
	{
		return;
	}
	MainWindow::weaponWindow()->clearWeaponData();
	MainWindow::weaponWindow()->buildTable();
}

void KanDataConnector::updateBattle(const kcsapi_battle &api_battle, KanBattleType type)
{
	ControlManager::getInstance().setPortDataDirty();

	int dockid = 0;
	int dockid_combinedSelf = -1;
	bCombinedSelf = false;
	bCombinedEnemy = false;

	pksd->remainLastBattleHPs.enemy.clear();
	pksd->remainLastBattleHPs.self.clear();
	pksd->remainLastBattleHPs.combinedSelf.clear();
	pksd->remainLastBattleHPs.combinedEnemy.clear();

	pksd->beginLastBattleHPs.enemy.clear();
	pksd->beginLastBattleHPs.self.clear();
	pksd->beginLastBattleHPs.combinedSelf.clear();
	pksd->beginLastBattleHPs.combinedEnemy.clear();

	if (type < KanBattleType::Combined_Begin)
	{
		dockid = api_battle.api_dock_id - 1;
		// midnight
		if (type == KanBattleType::Night || type == KanBattleType::DayToNight || dockid < 0)
		{
			dockid = api_battle.api_deck_id - 1;
		}
		pksd->bCombinedSelf = bCombinedSelf;
	}
	else
	{
		if (api_battle.api_deck_id > 0)
		{
			dockid = api_battle.api_deck_id - 1;
		}
		if (type == KanBattleType::Combined_Each || type == KanBattleType::Combined_EachWater)
		{
			bCombinedEnemy = true;
			bCombinedSelf = true;
		}
		else if (type == KanBattleType::Combined_EC || type == KanBattleType::Combined_ECNight || type == KanBattleType::Combined_ECNightToDay)
		{
			// enemy combined
			bCombinedEnemy = true;

			if (api_battle.api_active_deck.size() > 0)
			{
				if (api_battle.api_active_deck[0] == 2)
				{
					bCombinedSelf = true;
				}
			}
		}
		else
		{
			bCombinedSelf = true;
		}


		if (bCombinedEnemy)
		{
			pksd->bCombinedEnemy = bCombinedEnemy;
		}
		if (bCombinedSelf)
		{
			dockid_combinedSelf = dockid + 1;
			pksd->bCombinedSelf = bCombinedSelf;
		}
	}

	if (dockid >= 0)
	{
		pksd->lastbattletype = type;

		if (type != KanBattleType::DayToNight
			&& type != KanBattleType::NightToDay
			&& type != KanBattleType::Combined_DayToNight
			&& type != KanBattleType::Combined_ECNight)
		{
			pksd->lastSeiku = api_battle.api_kouku.api_stage1.api_disp_seiku;
			pksd->lastSFormation = -1;
			pksd->lastEFormation = -1;
			pksd->lastIntercept = -1;
			if (pksd->battledata.api_formation.count() > 0)
			{
				pksd->lastSFormation = pksd->battledata.api_formation[0];
			}
			if (pksd->battledata.api_formation.count() > 1)
			{
				pksd->lastEFormation = pksd->battledata.api_formation[1];
			}
			if (pksd->battledata.api_formation.count() > 2)
			{
				pksd->lastIntercept = pksd->battledata.api_formation[2];
			}
		}

		pksd->lastdeckid = dockid;

		QList<kcsapi_ship2*> pships;
		foreach(int shipid, pksd->portdata.api_deck_port[dockid].api_ship)
		{
			if (shipid > 0)
			{
				kcsapi_ship2 * pship = findShipFromShipno(shipid);
				if (pship)
				{
					pships.append(findShipFromShipno(shipid));
				}
			}
		}
		QList<kcsapi_ship2*>pships_combined;
		if (dockid_combinedSelf >= 0)
		{
			foreach(int shipid, pksd->portdata.api_deck_port[dockid_combinedSelf].api_ship)
			{
				if (shipid > 0)
				{
					kcsapi_ship2 * pship = findShipFromShipno(shipid);
					if (pship)
					{
						pships_combined.append(findShipFromShipno(shipid));
					}
				}
			}
		}

		totalFDamage.clear();
		for (int i = 0; i < KanSaveData::maxSingleTeamSize; i++)
		{
			totalFDamage.append(0);
		}
		totalFDamageCombined.clear();
		for (int i = 0; i < KanSaveData::maxSingleTeamSize; i++)
		{
			totalFDamageCombined.append(0);
		}
		totalEDamage.clear();
		for (int i = 0; i < KanSaveData::maxSingleTeamSize; i++)
		{
			totalEDamage.append(0);
		}
		totalEDamageCombined.clear();
		for (int i = 0; i < KanSaveData::maxSingleTeamSize; i++)
		{
			totalEDamageCombined.append(0);
		}

		for (int i = 0; i < pships.count(); i++)
		{
			pksd->beginLastBattleHPs.self.append(api_battle.api_f_nowhps[i]);
		}
		for (int i = 0; i < pships_combined.count(); i++)
		{
			if (api_battle.api_f_nowhps_combined.count() > 0)
			{
				pksd->beginLastBattleHPs.combinedSelf.append(api_battle.api_f_nowhps_combined[i]);
			}
			else
			{
				pksd->beginLastBattleHPs.combinedSelf.append(api_battle.api_f_nowhps[i + 6]);
			}
		}
		for (int i = 0; i < api_battle.api_ship_ke.count(); i++)
		{
			pksd->beginLastBattleHPs.enemy.append(api_battle.api_e_nowhps[i]);
		}
		for (int i = 0; i < api_battle.api_ship_ke_combined.count(); i++)
		{
			if (api_battle.api_e_nowhps_combined.count() > 0)
			{
				pksd->beginLastBattleHPs.combinedEnemy.append(api_battle.api_e_nowhps_combined[i]);
			}
			else
			{
				pksd->beginLastBattleHPs.combinedEnemy.append(api_battle.api_e_nowhps[i + 6]);
			}
		}

		//////////////////////////////////////////////////////////////////////////
		/************************************************************************/
		/* BattleParse                                                          */
		/************************************************************************/
		//////////////////////////////////////////////////////////////////////////

		//#define TRYLOG(func, str)	try {func;} catch(...){ERRLOG("Error Parse Battle: " str)}
#define TRYLOG(func, str) func;

		// air base attack
		if (!api_battle.api_air_base_attack.isEmpty())
		{
			for (const auto& item : api_battle.api_air_base_attack)
			{
				TRYLOG(processAirBaseAttackDamages(item), "AirBaseAttack");
			}
		}

		// injection
		//　航空ダメージ

		int stageflagcount = api_battle.api_stage_flag.count();
		//TODO: formation
		if (stageflagcount >= 3)
		{
			if (api_battle.api_stage_flag[2] > 0)
			{
				TRYLOG(processKoukuDamages(api_battle.api_injection_kouku), "InjecionKouku");
			}
			//　航空ダメージ
			if (api_battle.api_stage_flag[2] > 0)
			{
				TRYLOG(processKoukuDamages(api_battle.api_kouku), "KoukuSen");
			}
		}
		int stageflagcount2 = api_battle.api_stage_flag2.count();
		if (stageflagcount2 >= 3)
		{
			//　航空ダメージ
			if (api_battle.api_stage_flag2[2] > 0)
			{
				TRYLOG(processKoukuDamages(api_battle.api_kouku2), "KoukuSen2");
			}
		}

		TRYLOG(processSupportDamages(api_battle.api_support_flag, api_battle.api_support_info), "Support");
		TRYLOG(processSupportDamages(api_battle.api_n_support_flag, api_battle.api_n_support_info), "NightSupport");

		// opening taisen
		if (api_battle.api_opening_taisen_flag)
		{
			bool bSelfCombineDamage = bCombinedSelf;
			if (type == KanBattleType::Combined_KouKu || type == KanBattleType::Combined_Day)
			{
				bSelfCombineDamage = false;
			}
			TRYLOG(processHougekiDamages(api_battle.api_opening_taisen, bSelfCombineDamage, false), "OpeningTaisen");	// no such condition?
		}

		// opening
		if (api_battle.api_opening_flag > 0)
		{
			TRYLOG(processOpeningDamages(api_battle.api_opening_atack), "OpeningAttack");
		}

		// hourai
		int houraiflagcount = api_battle.api_hourai_flag.count();
		if (houraiflagcount > 0)
		{
			int hougeki1flag = 0;
			int hougeki2flag = 0;
			int hougeki3flag = 0;

			hougeki1flag = api_battle.api_hourai_flag[0];
			if (houraiflagcount > 1)
			{
				hougeki2flag = api_battle.api_hourai_flag[1];
			}
			if (houraiflagcount > 2)
			{
				hougeki3flag = api_battle.api_hourai_flag[2];
			}

			if (bCombinedSelf &&
				(type == KanBattleType::Combined_KouKu || type == KanBattleType::Combined_Day))
			{
				//raigekiflag = api_battle.api_hourai_flag[1];
				hougeki2flag = api_battle.api_hourai_flag[2];
				hougeki3flag = api_battle.api_hourai_flag[3];
			}
			if (hougeki1flag)
			{
				bool bCombineDamageSelf = false;
				bool bCombineDamageEnemy = bCombinedEnemy;
				if (type == KanBattleType::Combined_KouKu || type == KanBattleType::Combined_Day)
				{
					bCombineDamageSelf = bCombinedSelf;
				}
				TRYLOG(processHougekiDamages(api_battle.api_hougeki1
					, bCombineDamageSelf
					, bCombineDamageEnemy), "Hougeki1");
			}
			if (hougeki2flag)
			{
				bool bCombineDamageSelf = false;
				bool bCombineDamageEnemy = false;
				TRYLOG(processHougekiDamages(api_battle.api_hougeki2
					, bCombineDamageSelf
					, bCombineDamageEnemy), "Hougeki2");
			}
			if (hougeki3flag)
			{
				//TODO!!! check
				bool bCombineDamageSelf = bCombinedSelf;
				bool bCombineDamageEnemy = bCombinedEnemy;
				if (type == KanBattleType::Combined_KouKu || type == KanBattleType::Combined_Day)
				{
					bCombineDamageSelf = false;
				}
				TRYLOG(processHougekiDamages(api_battle.api_hougeki3
					, bCombineDamageSelf
					, bCombineDamageEnemy), "Hougeki3");
			}


			// raigeki
			TRYLOG(processRaigekiDamages(api_battle.api_raigeki), "Raigeki");
		}

		// midnight
		{
			bool bEnemyDamageCombined = false;
			if (bCombinedEnemy)
			{
				//TODO!!!! check
				if (api_battle.api_active_deck.count() >= 2)
				{
					if (api_battle.api_active_deck[1] != 1)
					{
						bEnemyDamageCombined = true;
					}
				}
			}

			TRYLOG(processHougekiDamages(api_battle.api_hougeki
				, bCombinedSelf		/* always second team*/
				, bEnemyDamageCombined), "NightHougeki");
			TRYLOG(processHougekiDamages(api_battle.api_n_hougeki1, bCombinedSelf, bEnemyDamageCombined), "NightHougeki1");
			TRYLOG(processHougekiDamages(api_battle.api_n_hougeki2, bCombinedSelf, bEnemyDamageCombined), "NightHougeki2");
		}

		//////////////////////////////////////////////////////////////////////////
		/************************************************************************/
		/* EndOfBattle                                                          */
		/************************************************************************/
		//////////////////////////////////////////////////////////////////////////

		for (int i = 0; i < pships.count(); i++)
		{
			if (pships[i])
			{
				pships[i]->api_nowhp -= (int)totalFDamage[i];
			}
		}
		for (int i = 0; i < pships_combined.count(); i++)
		{
			if (pships_combined[i])
			{
				pships_combined[i]->api_nowhp -= (int)totalFDamageCombined[i];
			}
		}
		updateFleetTable();

		for (int i = 0; i < api_battle.api_ship_ke.count(); i++)
		{
			int nowhp = pksd->beginLastBattleHPs.enemy[i] - (int)totalEDamage[i];
			if (nowhp < 0)
			{
				nowhp = 0;
			}
			pksd->remainLastBattleHPs.enemy.append(nowhp);
		}
		for (int i = 0; i < api_battle.api_ship_ke_combined.count(); i++)
		{
			int nowhp = pksd->beginLastBattleHPs.combinedEnemy[i] - (int)totalEDamageCombined[i];
			if (nowhp < 0)
			{
				nowhp = 0;
			}
			pksd->remainLastBattleHPs.combinedEnemy.append(nowhp);
		}

		for (int i = 0; i < pships.count(); i++)
		{
			pksd->remainLastBattleHPs.self.append(pships[i]->api_nowhp);
		}
		for (int i = 0; i < pships_combined.count(); i++)
		{
			pksd->remainLastBattleHPs.combinedSelf.append(pships_combined[i]->api_nowhp);
		}

		bool bSelfDamaged = false;
		for (auto damage : totalFDamage)
		{
			if (damage > 0)
			{
				bSelfDamaged = true;
				break;
			}
		}
		if (!bSelfDamaged)
		{
			for (auto damage : totalFDamageCombined)
			{
				if (damage > 0)
				{
					bSelfDamaged = true;
					break;
				}
			}
		}
		updateInfoTitleBattle(true, bSelfDamaged);
	}
}

void KanDataConnector::processHougekiDamages(const kcsapi_battle_hougeki& api_hougeki
	, bool bOnlyCombinedSelf
	, bool bOnlyCombinedEnemy)
{
	// must skip 0!!!
	QList<float>* fdamage = &totalFDamage;
	QList<float>* edamage = &totalEDamage;

	// without instruction
	// old?
	if (api_hougeki.api_at_eflag.isEmpty())
	{
		if (bOnlyCombinedSelf)
		{
			fdamage = &totalFDamageCombined;
		}
		if (bOnlyCombinedEnemy)
		{
			edamage = &totalEDamageCombined;
		}
		for (int j = 0; j < api_hougeki.api_at_list.count(); j++)
		{
			bool bfattack = true;
			bool bfdefend = true;

			int attackpos = api_hougeki.api_at_list[j];
			// TODO MaxTeamMemberCount
			if (attackpos >= 6)
			{
				bfattack = false;
				attackpos -= 6;
			}

			if (attackpos >= 0)
			{
				for (int k = 0; k < api_hougeki.api_df_list[j].count(); k++)
				{
					int defendpos = api_hougeki.api_df_list[j][k];
					// TODO MaxTeamMemberCount
					if (defendpos >= 6)
					{
						bfdefend = false;
						defendpos -= 6;
					}

					if (defendpos >= 0)
					{
						if (bfdefend)
						{
							(*fdamage)[defendpos] += api_hougeki.api_damage[j][k];
						}
						else
						{
							(*edamage)[defendpos] += api_hougeki.api_damage[j][k];
						}
					}
				}
			}
		}
	}
	else
	{
		for (int j = 0; j < api_hougeki.api_at_list.count(); j++)
		{
			for (int k = 0; k < api_hougeki.api_df_list[j].count(); k++)
			{
				int defendpos = api_hougeki.api_df_list[j][k];

				if (defendpos >= 0)
				{
					if (api_hougeki.api_at_eflag[j] > 0)
					{
						// from enemy
						// TODO MaxTeamMemberCount
						// !!!!!!!!!!!!!!!!!! seven
						if (bCombinedSelf && defendpos >= KanSaveData::maxCombinedTeamSize)
						{
							fdamage = &totalFDamageCombined;
							defendpos -= KanSaveData::maxCombinedTeamSize;
						}
						else
						{
							fdamage = &totalFDamage;
						}
						(*fdamage)[defendpos] += api_hougeki.api_damage[j][k];
					}
					else
					{
						// TODO MaxTeamMemberCount
						if (bCombinedEnemy && defendpos >= KanSaveData::maxCombinedTeamSize)
						{
							edamage = &totalEDamageCombined;
							defendpos -= KanSaveData::maxCombinedTeamSize;
						}
						else
						{
							edamage = &totalEDamage;
						}
						(*edamage)[defendpos] += api_hougeki.api_damage[j][k];
					}
				}
			}
		}
	}

}

void KanDataConnector::processAirBaseAttackDamages(const kcsapi_battle_base_attack& api_air_base_attack)
{
	//　航空ダメージ (enemy only)
	if (api_air_base_attack.api_stage_flag.size() >= 2 && api_air_base_attack.api_stage_flag[2] > 0)
	{
		for (int i = 0; i < api_air_base_attack.api_stage3.api_edam.count(); i++)
		{
			totalEDamage[i] += api_air_base_attack.api_stage3.api_edam[i];
		}

		if (bCombinedEnemy)
		{
			// enemy combined
			for (int i = 0; i < api_air_base_attack.api_stage3_combined.api_edam.count(); i++)
			{
				totalEDamageCombined[i] += api_air_base_attack.api_stage3_combined.api_edam[i];
			}
		}
	}
}

void KanDataConnector::processKoukuDamages(const kcsapi_battle_kouku& api_kouku)
{
	for (int i = 0; i < api_kouku.api_stage3.api_fdam.count(); i++)
	{
		totalFDamage[i] += api_kouku.api_stage3.api_fdam[i];
	}
	for (int i = 0; i < api_kouku.api_stage3.api_edam.count(); i++)
	{
		totalEDamage[i] += api_kouku.api_stage3.api_edam[i];
	}
	if (bCombinedSelf)
	{
		for (int i = 0; i < api_kouku.api_stage3_combined.api_fdam.count(); i++)
		{
			totalFDamageCombined[i] += api_kouku.api_stage3_combined.api_fdam[i];
		}
	}
	if (bCombinedEnemy)
	{
		for (int i = 0; i < api_kouku.api_stage3_combined.api_edam.count(); i++)
		{
			totalEDamageCombined[i] += api_kouku.api_stage3_combined.api_edam[i];
		}
	}

}

void KanDataConnector::processSupportDamages(int api_support_flag, const kcsapi_battle_support_info& api_support_info)
{
	// TODO: taisen?

	// support
	if (api_support_flag > 0)
	{
		switch (api_support_flag)
		{
		case 1:	// kubaku
		{
			int supportstageflagcount = api_support_info.api_support_airatack.api_stage_flag.count();
			if (supportstageflagcount >= 3)
			{
				if (api_support_info.api_support_airatack.api_stage_flag[2] > 0)
				{
					for (int i = 0; i < api_support_info.api_support_airatack.api_stage3.api_edam.count(); i++)
					{
						totalEDamage[i] += api_support_info.api_support_airatack.api_stage3.api_edam[i];
					}
					// TODO:!!!! check here
					if (bCombinedEnemy)
					{
						for (int i = 0; i < api_support_info.api_support_airatack.api_stage3_combined.api_edam.count(); i++)
						{
							totalEDamageCombined[i] += api_support_info.api_support_airatack.api_stage3_combined.api_edam[i];
						}
					}
				}
			}

		}
		break;
		case 2: // hougeki
		case 3: // raigeki
		{
			if (!bCombinedEnemy)
			{
				for (int i = 0; i < api_support_info.api_support_hourai.api_damage.count(); i++)
				{
					totalEDamage[i] += api_support_info.api_support_hourai.api_damage[i];
				}
			}
			else
			{
				int damageCount = api_support_info.api_support_hourai.api_damage.count();
				if (damageCount <= KanSaveData::maxCombinedTeamSize)
				{
					for (int i = 0; i < damageCount; i++)
					{
						totalEDamage[i] += api_support_info.api_support_hourai.api_damage[i];
					}
				}
				else
				{
					for (int i = 0; i < KanSaveData::maxCombinedTeamSize; i++)
					{
						totalEDamage[i] += api_support_info.api_support_hourai.api_damage[i];
					}
					for (int i = KanSaveData::maxCombinedTeamSize; i < damageCount; i++)
					{
						totalEDamageCombined[i - KanSaveData::maxCombinedTeamSize] += api_support_info.api_support_hourai.api_damage[i];
					}
				}
			}
		}
		break;
		default:
			break;
		}
	}

}

void KanDataConnector::processOpeningDamages(const kcsapi_battle_opening_atack& api_opening_atack)
{
	for (int i = 0; i < api_opening_atack.api_fdam.count(); i++)
	{
		int damagePos = i;

		if (bCombinedSelf/*api_battle.api_formation[0] == 11*/)
		{
			if (damagePos >= KanSaveData::maxCombinedTeamSize)
			{
				damagePos -= KanSaveData::maxCombinedTeamSize;
			}
			totalFDamageCombined[damagePos] += api_opening_atack.api_fdam[i];
		}
		else
		{
			// TODO check
			totalFDamage[damagePos] += api_opening_atack.api_fdam[i];
		}

	}
	int damageCount = api_opening_atack.api_edam.count();
	// TODO MaxTeamMemberCount
	if (bCombinedEnemy && damageCount > KanSaveData::maxCombinedTeamSize)
	{
		// TODO MaxTeamMemberCount
		for (int i = 0; i < KanSaveData::maxCombinedTeamSize; i++)
		{
			totalEDamage[i] += api_opening_atack.api_edam[i];
		}
		// TODO MaxTeamMemberCount
		for (int i = KanSaveData::maxCombinedTeamSize; i < damageCount; i++)
		{
			// TODO MaxTeamMemberCount
			totalEDamageCombined[i - KanSaveData::maxCombinedTeamSize] += api_opening_atack.api_edam[i];
		}
	}
	else
	{
		for (int i = 0; i < damageCount; i++)
		{
			totalEDamage[i] += api_opening_atack.api_edam[i];
		}
	}
}

void KanDataConnector::processRaigekiDamages(const kcsapi_battle_raigeki& api_raigeki)
{
	int damageCount = api_raigeki.api_fdam.count();
	// TODO MaxTeamMemberCount
	if (bCombinedSelf && damageCount > KanSaveData::maxCombinedTeamSize)
	{
		// TODO MaxTeamMemberCount
		for (int i = 0; i < KanSaveData::maxCombinedTeamSize; i++)
		{
			totalFDamage[i] += api_raigeki.api_fdam[i];
		}
		// TODO MaxTeamMemberCount
		for (int i = KanSaveData::maxCombinedTeamSize; i < damageCount; i++)
		{
			totalFDamageCombined[i - KanSaveData::maxCombinedTeamSize] += api_raigeki.api_fdam[i];
		}
	}
	else
	{
		for (int i = 0; i < api_raigeki.api_fdam.count(); i++)
		{
			// TODO!!!!! check
			int damagePos = i;
			if (bCombinedSelf/*api_battle.api_formation[0] == 11*/)
			{
				if (damagePos >= KanSaveData::maxCombinedTeamSize)
				{
					damagePos -= KanSaveData::maxCombinedTeamSize;
				}
				totalFDamageCombined[damagePos] += api_raigeki.api_fdam[i];
			}
			else
			{
				totalFDamage[damagePos] += api_raigeki.api_fdam[i];
			}
		}
	}

	damageCount = api_raigeki.api_edam.count();
	// TODO MaxTeamMemberCount
	if (bCombinedEnemy && damageCount > KanSaveData::maxCombinedTeamSize)
	{
		for (int i = 0; i < KanSaveData::maxCombinedTeamSize; i++)
		{
			totalEDamage[i] += api_raigeki.api_edam[i];
		}
		for (int i = KanSaveData::maxCombinedTeamSize; i < damageCount; i++)
		{
			totalEDamageCombined[i - KanSaveData::maxCombinedTeamSize] += api_raigeki.api_edam[i];
		}
	}
	else
	{
		for (int i = 0; i < damageCount; i++)
		{
			totalEDamage[i] += api_raigeki.api_edam[i];
		}
	}
}