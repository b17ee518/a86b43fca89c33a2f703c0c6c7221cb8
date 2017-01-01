#include "kandataconnector.h"
#include "kandata.h"
#include "kansavedata.h"

#include "infomainwindow.h"
#include "mainwindow.h"
#include <cmath>

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

	lst.append(QString::fromLocal8Bit("所有艦娘数: %1/%2").arg(kancount).arg(kanmaxcount));
	lst.append(QString::fromLocal8Bit("所有装備数: %1/%2").arg(slotitemcount).arg(slotitemmaxcount));
	lst.append(QString::fromLocal8Bit("提督Lv. %1(次:%2)").arg(lv).arg(nextexp));
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
	cols.append(_colWhite);
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
	qSort(lstShips.begin(), lstShips.end(), repairTableSort);

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

		int alltaiku = 0;
		int alltaikubonus = 0;
		int alllv = 0;
		int flagshipLv = 0;
		int allsakuteki = 0;
		//		double allsakuteki_sp = 0;
		double allsakuteki_sp33 = 0;

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

			int taiku = 0;
			int taikubonus = 0;

			QList<int> slotitems = pship->api_slot;
			slotitems.append(pship->api_slot_ex);

			int slotsakuteki = 0;
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

								// taiku
								if (type == SlotitemType::KanSen
									|| type == SlotitemType::SuiJouSenTouKi
									|| type == SlotitemType::KanBaKu
									|| type == SlotitemType::KanKou
									|| type == SlotitemType::SuiBaKu
									|| type == SlotitemType::RiKuJouKouGeKiKi
									|| type == SlotitemType::KyoKuChiSenTouKi)
								{
									double taikuBase = pmstslotitem->api_tyku*sqrt((double)pship->api_onslot[i]);
									// geigeki
									if (type == SlotitemType::KyoKuChiSenTouKi)
									{
										taikuBase += pmstslotitem->api_houk*1.5;
									}
									// hoshi
									else if (type == SlotitemType::KanSen
										|| type == SlotitemType::SuiJouSenTouKi)
									{
										taikuBase += v.api_level*0.2;
									}
									taiku += (int)taikuBase;

									// >> bonus
									int alv = v.api_alv;
									if (type == SlotitemType::KanSen
										|| type == SlotitemType::SuiJouSenTouKi)
									{
										switch (alv)
										{
										case 1:
											taikubonus += 1;
											break;
										case 2:
											taikubonus += 4;
											break;
										case 3:
											taikubonus += 6;
											break;
										case 4:
											taikubonus += 11;
											break;
										case 5:
											taikubonus += 16;
											break;
										case 6:
											taikubonus += 17;
											break;
										case 7:
											taikubonus += 25;
											break;
										default:
											break;
										}
									}
									else if (type == SlotitemType::KanKou
										|| type == SlotitemType::KanBaKu
										|| type == SlotitemType::RiKuJouKouGeKiKi)
									{
										if (alv >= 7)
										{
											taikubonus += 3;
										}
									}
									else if (type == SlotitemType::SuiBaKu)
									{
										if (alv >= 7)
										{
											taikubonus += 9;
										}
									}
								}

								// drum
								if (type == SlotitemType::YuSou)
								{
									drumcount++;
									bHaveDrum = true;
								}

								if (type == SlotitemType::OuKyu)
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

								// sakuteki
								// old
								/*
								if (type == SlotitemType::TEISATSU || type == SlotitemType::SUITEI)
								{
								sakuteki_teisatsu += item.api_saku;
								}
								if (type == SlotitemType::DENTAN_L || type == SlotitemType::DENTAN_S)
								{
								sakuteki_dentan += item.api_saku;
								}
								*/
								// new
								slotsakuteki += pmstslotitem->api_saku;
								switch (type)
								{
								case SlotitemType::KanBaKu:
									//									allsakuteki_sp += pmstslotitem->api_saku * 1.0376255;
									allsakuteki_sp33 += pmstslotitem->api_saku * 0.6;
									break;
								case SlotitemType::KanKou:
									//									allsakuteki_sp += pmstslotitem->api_saku * 1.3677954;
									allsakuteki_sp33 += pmstslotitem->api_saku * 0.8;
									break;
								case SlotitemType::KanJouTeiSaTsu:
									//									allsakuteki_sp += pmstslotitem->api_saku * 1.6592780;
									allsakuteki_sp33 += pmstslotitem->api_saku * 1.0;
									break;
								case SlotitemType::KanJouTeiSaTsu_II:
									//									allsakuteki_sp += pmstslotitem->api_saku * 1.6592780;
									allsakuteki_sp33 += pmstslotitem->api_saku * 1.0;
									break;
								case SlotitemType::SuiTei:
									//									allsakuteki_sp += pmstslotitem->api_saku * 2.0;
									allsakuteki_sp33 += (pmstslotitem->api_saku + sqrt(v.api_level)*1.2) * 1.2;
									break;
								case SlotitemType::SuiBaKu:
									//									allsakuteki_sp += pmstslotitem->api_saku * 1.7787282;
									allsakuteki_sp33 += pmstslotitem->api_saku * 1.1;
									break;
								case SlotitemType::DenTan_S:
									//									allsakuteki_sp += pmstslotitem->api_saku * 1.0045358;
									allsakuteki_sp33 += (pmstslotitem->api_saku + sqrt(v.api_level)*1.25) * 0.6;
									break;
								case SlotitemType::DenTan_L:
									//									allsakuteki_sp += pmstslotitem->api_saku * 0.9906638;
									allsakuteki_sp33 += (pmstslotitem->api_saku + sqrt(v.api_level)*1.25) * 0.6;
									break;
								case SlotitemType::DenTan_L_II:
									//									allsakuteki_sp += pmstslotitem->api_saku * 0.9906638;
									allsakuteki_sp33 += (pmstslotitem->api_saku + sqrt(v.api_level)*1.25) * 0.6;
									break;
								case SlotitemType::TanSyouTou:
									//									allsakuteki_sp += pmstslotitem->api_saku * 0.9067950;
									allsakuteki_sp33 += pmstslotitem->api_saku * 0.6;
									break;

								case SlotitemType::KanSen:
									allsakuteki_sp33 += pmstslotitem->api_saku * 0.6;
									break;
								case SlotitemType::TaiSenKi:
									allsakuteki_sp33 += pmstslotitem->api_saku * 0.6;
									break;
								case SlotitemType::SiReiBu:
									allsakuteki_sp33 += pmstslotitem->api_saku * 0.6;
									break;
								case SlotitemType::KouKuYouIn:
									allsakuteki_sp33 += pmstslotitem->api_saku * 0.6;
									break;
								case SlotitemType::SuiJouYouIn:
									allsakuteki_sp33 += pmstslotitem->api_saku * 0.6;
									break;
								case SlotitemType::Sonar_L:
									allsakuteki_sp33 += pmstslotitem->api_saku * 0.6;
									break;
								case SlotitemType::HiKouTei_L:
									allsakuteki_sp33 += pmstslotitem->api_saku * 0.6;
									break;
								case SlotitemType::TanSyouTou_L:
									allsakuteki_sp33 += pmstslotitem->api_saku * 0.6;
									break;
								case SlotitemType::SuiJouSenTouKi:
									allsakuteki_sp33 += pmstslotitem->api_saku * 0.6;
									break;
								case SlotitemType::FunSenBaku:
									allsakuteki_sp33 += pmstslotitem->api_saku * 0.6;

								default:
									break;
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
			alltaiku += taiku;
			alltaikubonus += taikubonus;
			alllv += pship->api_lv;
			if (flagshipLv <= 0)
			{
				flagshipLv = pship->api_lv;
			}
			allsakuteki += pship->api_sakuteki[0];
			//			allsakuteki_sp += sqrt((double)pship->api_sakuteki[0]) * 1.6841056;

			// TODO: areamul
			// 2-5:1 / 3-5:4 / 6-1:4 / 6-2:3 / 6-3:3
			//allsakuteki_sp33 *= areasp33mul;

			allsakuteki_sp33 += sqrt((double)pship->api_sakuteki[0] - slotsakuteki);

			QString damekonstr = "";
			if (damekonstate == 1)
			{
				damekonstr = QString::fromLocal8Bit("ダ");
			}
			else if (damekonstate == 2)
			{
				damekonstr = QString::fromLocal8Bit("女");
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

		//		allsakuteki_sp = (int)(sakuteki_teisatsu * 2 + sakuteki_dentan + sqrt((double)(allsakuteki - sakuteki_dentan - sakuteki_teisatsu)));
		//		allsakuteki_sp += ((pksd->portdata.api_basic.api_level + 5) / 5) * 5 * (-0.6142467);
		allsakuteki_sp33 -= ceil(pksd->portdata.api_basic.api_level*0.4);

		// only calc this team
		allsakuteki_sp33 += (6 - v.api_ship.count()) * 2;

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
		KanSaveData::DeckSaveData* pdsd = &(pksd->deckSaveData[v.api_id - 1]);
		pdsd->totalLevel = alllv;
		pdsd->totalTaiku = alltaiku + alltaikubonus;
		pdsd->taikuWithoutBonus = alltaiku;
		pdsd->totalSakuteki = allsakuteki;
		pdsd->sakutekiSp33 = (int)allsakuteki_sp33;

		QString strtitle = QString::fromLocal8Bit("%1 (Lv計:%2 制空:%3[%4] 索敵:%5[%6])")
			.arg(v.api_name)
			.arg(alllv)
			.arg(alltaiku)
			.arg(alltaiku + alltaikubonus)
			.arg(allsakuteki)
			.arg(allsakuteki_sp33, 0, 'f', 2);


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
			MainWindow::timerWindow()->setBuildTime(v.api_id - 1, v.api_complete_time, pmstship->api_buildtime, pmstship->api_name);
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
		for (int i = 1; i < pksd->battledata.api_ship_ke.count(); i++)
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
					else if (i == 1)
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
		for (int i = 1; i < pksd->battledata.api_ship_ke_combined.count(); i++)
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
		if (pksd->remainLastBattleHPs.enemy.count() > 1 &&
			pksd->remainLastBattleHPs.enemy[1] > 0)
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
				for (int i = 1; i < sCount; i++)
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
				for (int i = 1; i < eCount; i++)
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

		pksd->wasLastBossCell = (pksd->nextdata.api_no == pksd->nextdata.api_bosscell_no);

		// special for calc 5-4
		if (pksd->nextdata.api_maparea_id == 5 && pksd->nextdata.api_mapinfo_no == 4)
		{
			if (pksd->nextdata.api_no == 19)
			{
				pksd->wasLastBossCell = true;
			}
		}

		pksd->lastKilledYusou = transtotal - transremain;
		pksd->lastKilledKubou = actotal - acremain;
		pksd->lastKilledSensui = subtotal - subremain;
	}

	int colindex = 0;
	if (pksd->nextdata.api_no == pksd->nextdata.api_bosscell_no)
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
	int keijuncount = 0;
	int keijunkiras = 0;
	int kuchikucount = 0;
	int kuchikukiras = 0;

	int lvmin = 20;

	foreach(const kcsapi_ship2 &v, pksd->portdata.api_ship)
	{
		int shipid = v.api_ship_id;
		/*
		bool bSkip = false;

		foreach(const kcsapi_deck &deck, pksd->portdata.api_deck_port)
		{
		if (deck.api_mission[0] > 0)
		{
		foreach(int shipno, deck.api_ship)
		{
		if (shipno == v.api_id)
		{
		bSkip = true;
		break;
		}
		}
		}
		if (bSkip)
		{
		break;
		}
		}

		if (bSkip)
		{
		continue;
		}
		*/
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
						if (KanDataCalc::GetCondState(v.api_cond - 3) != CondState::Kira)
						{
							suibokirawarnings++;
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

	QString titlestr = QString::fromLocal8Bit("キラ - 水母:%1(%2/%3) 軽巡:%4(%5) 駆逐:%6(%7)")
		.arg(suibokiras)
		.arg(suibocount - suibokiras)
		.arg(suibokirawarnings)
		.arg(keijunkiras)
		.arg(keijuncount - keijunkiras)
		.arg(kuchikukiras)
		.arg(kuchikucount - kuchikukiras);

	int colindex = 0;
	if (suibokiras == suibocount && kuchikukiras * 2 >= kuchikucount && keijunkiras * 2 >= keijuncount && suibokirawarnings == 0)
	{
		colindex = 1;
	}
	else if (suibokiras < suibocount || kuchikukiras < 10 || keijunkiras < 4)
	{
		colindex = 2;
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
	int dockid = 0;
	int dockid_combinedSelf = -1;
	bool bCombinedSelf = false;
	bool bCombinedEnemy = false;

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
		if (type == KanBattleType::Combined_Each || type == KanBattleType::Combined_EachWater)
		{
			bCombinedEnemy = true;
			bCombinedSelf = true;
		}
		else if (type == KanBattleType::Combined_EC || type == KanBattleType::Combined_ECNight)
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
			//			&& type != KanBattleType::Combined_KouKuNight
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

		QList<float> totalfdamage;
		for (int i = 0; i < 7; i++)
		{
			totalfdamage.append(0);
		}
		QList<float> totalfdamage_combined;
		for (int i = 0; i < 7; i++)
		{
			totalfdamage_combined.append(0);
		}
		QList<float> totaledamage;
		for (int i = 0; i < 7; i++)
		{
			totaledamage.append(0);
		}
		QList<float> totaledamage_combined;
		for (int i = 0; i < 7; i++)
		{
			totaledamage_combined.append(0);
		}
		/*
		QList<int> api_nowhps = api_battle.api_nowhps;
		QList<int> api_nowhps_combined = api_battle.api_nowhps_combined;
		*/
		pksd->beginLastBattleHPs.self.append(0);
		pksd->beginLastBattleHPs.combinedSelf.append(0);
		pksd->beginLastBattleHPs.enemy.append(0);
		pksd->beginLastBattleHPs.combinedEnemy.append(0);

		for (int i = 0; i < pships.count(); i++)
		{
			pksd->beginLastBattleHPs.self.append(api_battle.api_nowhps[i + 1]);
		}
		for (int i = 0; i < pships_combined.count(); i++)
		{
			pksd->beginLastBattleHPs.combinedSelf.append(api_battle.api_nowhps_combined[i + 1]);
		}
		for (int i = 1; i < api_battle.api_ship_ke.count(); i++)
		{
			pksd->beginLastBattleHPs.enemy.append(api_battle.api_nowhps[i + 6]);
		}
		for (int i = 1; i < api_battle.api_ship_ke_combined.count(); i++)
		{
			pksd->beginLastBattleHPs.combinedEnemy.append(api_battle.api_nowhps_combined[i + 6]);
		}


		// air base attack
		if (!api_battle.api_air_base_attack.isEmpty())
		{
			for (const auto& item : api_battle.api_air_base_attack)
			{
				//　航空ダメージ (enemy only)
				if (item.api_stage_flag.size() >= 2 && item.api_stage_flag[2] > 0)
				{
					for (int i = 1; i < item.api_stage3.api_edam.count(); i++)
					{
						totaledamage[i] += item.api_stage3.api_edam[i];
					}

					if (bCombinedEnemy)
					{
						// enemy combined
						for (int i = 1; i < item.api_stage3_combined.api_edam.count(); i++)
						{
							totaledamage_combined[i] += item.api_stage3_combined.api_edam[i];
						}
					}
				}
			}
		}

		// injection
		//　航空ダメージ
		if (api_battle.api_stage_flag[2] > 0)
		{
			for (int i = 1; i < api_battle.api_injection_kouku.api_stage3.api_fdam.count(); i++)
			{
				totalfdamage[i] += api_battle.api_injection_kouku.api_stage3.api_fdam[i];
			}
			for (int i = 1; i < api_battle.api_injection_kouku.api_stage3.api_edam.count(); i++)
			{
				totaledamage[i] += api_battle.api_injection_kouku.api_stage3.api_edam[i];
			}
			if (bCombinedSelf)
			{
				for (int i = 1; i < api_battle.api_injection_kouku.api_stage3_combined.api_fdam.count(); i++)
				{
					totalfdamage_combined[i] += api_battle.api_injection_kouku.api_stage3_combined.api_fdam[i];
				}
			}
			if (bCombinedEnemy)
			{
				for (int i = 1; i < api_battle.api_injection_kouku.api_stage3_combined.api_edam.count(); i++)
				{
					totaledamage_combined[i] += api_battle.api_injection_kouku.api_stage3_combined.api_edam[i];
				}
			}
		}

		//TODO: formation
		int stageflagcount = api_battle.api_stage_flag.count();
		if (stageflagcount >= 3)
		{
			//　航空ダメージ
			if (api_battle.api_stage_flag[2] > 0)
			{
				for (int i = 1; i < api_battle.api_kouku.api_stage3.api_fdam.count(); i++)
				{
					totalfdamage[i] += api_battle.api_kouku.api_stage3.api_fdam[i];
				}
				for (int i = 1; i < api_battle.api_kouku.api_stage3.api_edam.count(); i++)
				{
					totaledamage[i] += api_battle.api_kouku.api_stage3.api_edam[i];
				}
				if (bCombinedSelf)
				{
					for (int i = 1; i < api_battle.api_kouku.api_stage3_combined.api_fdam.count(); i++)
					{
						totalfdamage_combined[i] += api_battle.api_kouku.api_stage3_combined.api_fdam[i];
					}
				}
				if (bCombinedEnemy)
				{
					for (int i = 1; i < api_battle.api_kouku.api_stage3_combined.api_edam.count(); i++)
					{
						totaledamage_combined[i] += api_battle.api_kouku.api_stage3_combined.api_edam[i];
					}
				}
			}
		}
		int stageflagcount2 = api_battle.api_stage_flag2.count();
		if (stageflagcount2 >= 3)
		{
			//　航空ダメージ
			if (api_battle.api_stage_flag2[2] > 0)
			{
				// kouku2
				for (int i = 1; i < api_battle.api_kouku2.api_stage3.api_fdam.count(); i++)
				{
					totalfdamage[i] += api_battle.api_kouku2.api_stage3.api_fdam[i];
				}
				for (int i = 1; i < api_battle.api_kouku2.api_stage3.api_edam.count(); i++)
				{
					totaledamage[i] += api_battle.api_kouku2.api_stage3.api_edam[i];
				}
				if (bCombinedSelf)
				{
					// kouku2
					for (int i = 1; i < api_battle.api_kouku2.api_stage3_combined.api_fdam.count(); i++)
					{
						totalfdamage_combined[i] += api_battle.api_kouku2.api_stage3_combined.api_fdam[i];
					}
				}
				if (bCombinedEnemy)
				{
					// kouku2
					for (int i = 1; i < api_battle.api_kouku2.api_stage3_combined.api_edam.count(); i++)
					{
						totaledamage_combined[i] += api_battle.api_kouku2.api_stage3_combined.api_edam[i];
					}
				}

			}
		}

		if (type != KanBattleType::Night
			&& type != KanBattleType::DayToNight
			//			&& type != KanBattleType::Combined_KouKuNight 
			&& type != KanBattleType::Combined_DayToNight
			&& type != KanBattleType::Combined_Night
			&& type != KanBattleType::Combined_ECNight)
		{
			// support
			if (api_battle.api_support_flag > 0)
			{
				switch (api_battle.api_support_flag)
				{
				case 1:	// kubaku
				{
					int supportstageflagcount = api_battle.api_support_info.api_support_airatack.api_stage_flag.count();
					if (supportstageflagcount >= 3)
					{
						if (api_battle.api_support_info.api_support_airatack.api_stage_flag[2] > 0)
						{
							for (int i = 1; i < api_battle.api_support_info.api_support_airatack.api_stage3.api_edam.count(); i++)
							{
								totaledamage[i] += api_battle.api_support_info.api_support_airatack.api_stage3.api_edam[i];
							}
							// TODO:!!!! check here
							if (bCombinedEnemy)
							{
								for (int i = 1; i < api_battle.api_support_info.api_support_airatack.api_stage3_combined.api_edam.count(); i++)
								{
									totaledamage_combined[i] += api_battle.api_support_info.api_support_airatack.api_stage3_combined.api_edam[i];
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
						for (int i = 1; i < api_battle.api_support_info.api_support_hourai.api_damage.count(); i++)
						{
							totaledamage[i] += api_battle.api_support_info.api_support_hourai.api_damage[i];
						}
					}
					else
					{
						int damageCount = api_battle.api_support_info.api_support_hourai.api_damage.count();
						if (damageCount <= 7)
						{
							for (int i = 1; i < damageCount; i++)
							{
								totaledamage[i] += api_battle.api_support_info.api_support_hourai.api_damage[i];
							}
						}
						else
						{
							for (int i = 1; i < 7; i++)
							{
								totaledamage[i] += api_battle.api_support_info.api_support_hourai.api_damage[i];
							}
							for (int i = 7; i < damageCount; i++)
							{
								totaledamage_combined[i - 6] += api_battle.api_support_info.api_support_hourai.api_damage[i];
							}
						}
					}
				}
				break;
				default:
					break;
				}
			}

			// opening taisen
			if (api_battle.api_opening_taisen_flag)
			{
				// copied from below
				// combined?

				bool bSelfCombineDamage = bCombinedSelf;
				if (type == KanBattleType::Combined_KouKu || type == KanBattleType::Combined_Day)
				{
					bSelfCombineDamage = false;
				}
				processHouraiDamages(&(api_battle.api_opening_taisen)
					, &totalfdamage
					, &totaledamage
					, &totalfdamage_combined
					, &totaledamage_combined
					, bSelfCombineDamage
					, false);	// no such condition?
			}


			// opening
			// TODO: combined?
			if (api_battle.api_opening_flag > 0)
			{
				for (int i = 1; i < api_battle.api_opening_atack.api_fdam.count(); i++)
				{
					int damagePos = i;

					if (damagePos > 6)
					{
						damagePos -= 6;
					}
					if (bCombinedSelf/*api_battle.api_formation[0] == 11*/)
					{
						totalfdamage_combined[damagePos] += api_battle.api_opening_atack.api_fdam[i];
					}
					else
					{
						totalfdamage[damagePos] += api_battle.api_opening_atack.api_fdam[i];
					}

				}
				int damageCount = api_battle.api_opening_atack.api_edam.count();
				if (bCombinedEnemy && damageCount > 7)
				{
					for (int i = 1; i < 7; i++)
					{
						totaledamage[i] += api_battle.api_opening_atack.api_edam[i];
					}
					for (int i = 7; i < damageCount; i++)
					{
						totaledamage_combined[i - 6] += api_battle.api_opening_atack.api_edam[i];
					}
				}
				else
				{
					for (int i = 1; i < damageCount; i++)
					{
						totaledamage[i] += api_battle.api_opening_atack.api_edam[i];
					}
				}
			}

			// hourai
			int houraiflagcount = api_battle.api_hourai_flag.count();
			if (houraiflagcount >= 4)
			{
				int hougeki1flag = api_battle.api_hourai_flag[0];
				int hougeki2flag = api_battle.api_hourai_flag[1];
				int hougeki3flag = api_battle.api_hourai_flag[2];
				int raigekiflag = api_battle.api_hourai_flag[3];
				if (bCombinedSelf &&
					(type == KanBattleType::Combined_KouKu || type == KanBattleType::Combined_Day))
				{
					raigekiflag = api_battle.api_hourai_flag[1];
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
					processHouraiDamages(&(api_battle.api_hougeki1)
						, &totalfdamage
						, &totaledamage
						, &totalfdamage_combined
						, &totaledamage_combined
						, bCombineDamageSelf
						, bCombineDamageEnemy);
				}
				if (hougeki2flag)
				{
					bool bCombineDamageSelf = false;
					bool bCombineDamageEnemy = false;
					processHouraiDamages(&(api_battle.api_hougeki2)
						, &totalfdamage
						, &totaledamage
						, &totalfdamage_combined
						, &totaledamage_combined
						, bCombineDamageSelf
						, bCombineDamageEnemy);
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
					processHouraiDamages(&(api_battle.api_hougeki3)
						, &totalfdamage
						, &totaledamage
						, &totalfdamage_combined
						, &totaledamage_combined
						, bCombineDamageSelf
						, bCombineDamageEnemy);
				}
				// raigeki
				if (raigekiflag)
				{
					for (int i = 1; i < api_battle.api_raigeki.api_fdam.count(); i++)
					{
						// TODO!!!!! check
						int damagePos = i;
						if (damagePos > 6)
						{
							damagePos -= 6;
						}
						if (bCombinedSelf/*api_battle.api_formation[0] == 11*/)
						{
							totalfdamage_combined[damagePos] += api_battle.api_raigeki.api_fdam[i];
						}
						else
						{
							totalfdamage[damagePos] += api_battle.api_raigeki.api_fdam[i];
						}
						/*
						if (bCombinedSelf)
						{
						totalfdamage_combined[i] += api_battle.api_raigeki.api_fdam[i];
						}
						else
						{
						totalfdamage[i] += api_battle.api_raigeki.api_fdam[i];
						}
						*/
					}
					int damageCount = api_battle.api_raigeki.api_edam.count();
					if (bCombinedEnemy && damageCount > 7)
					{
						for (int i = 1; i < 7; i++)
						{
							totaledamage[i] += api_battle.api_raigeki.api_edam[i];
						}
						for (int i = 7; i < damageCount; i++)
						{
							totaledamage_combined[i - 6] += api_battle.api_raigeki.api_edam[i];
						}
					}
					else
					{
						for (int i = 1; i < damageCount; i++)
						{
							totaledamage[i] += api_battle.api_raigeki.api_edam[i];
						}
					}
				}
			}
		}

		else
		{
			// midnight
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

			processHouraiDamages(&(api_battle.api_hougeki)
				, &totalfdamage
				, &totaledamage
				, &totalfdamage_combined
				, &totaledamage_combined
				, bCombinedSelf		// always second team
				, bEnemyDamageCombined);
		}

		for (int i = 0; i < pships.count(); i++)
		{
			if (pships[i])
			{
				pships[i]->api_nowhp -= (int)totalfdamage[i + 1];
			}
		}
		for (int i = 0; i < pships_combined.count(); i++)
		{
			if (pships_combined[i])
			{
				pships_combined[i]->api_nowhp -= (int)totalfdamage_combined[i + 1];
			}
		}
		updateFleetTable();

		pksd->remainLastBattleHPs.enemy.append(0);
		for (int i = 1; i < api_battle.api_ship_ke.count(); i++)
		{
			int nowhp = api_battle.api_nowhps[i + 6] - (int)totaledamage[i];
			if (nowhp < 0)
			{
				nowhp = 0;
			}
			pksd->remainLastBattleHPs.enemy.append(nowhp);
		}
		pksd->remainLastBattleHPs.combinedEnemy.append(0);
		for (int i = 1; i < api_battle.api_ship_ke_combined.count(); i++)
		{
			int nowhp = api_battle.api_nowhps_combined[i + 6] - (int)totaledamage_combined[i];
			if (nowhp < 0)
			{
				nowhp = 0;
			}
			pksd->remainLastBattleHPs.combinedEnemy.append(nowhp);
		}

		pksd->remainLastBattleHPs.self.append(0);
		for (int i = 0; i < pships.count(); i++)
		{
			pksd->remainLastBattleHPs.self.append(pships[i]->api_nowhp);
		}
		pksd->remainLastBattleHPs.combinedSelf.append(0);
		for (int i = 0; i < pships_combined.count(); i++)
		{
			pksd->remainLastBattleHPs.combinedSelf.append(pships_combined[i]->api_nowhp);
		}

		bool bSelfDamaged = false;
		for (auto damage : totalfdamage)
		{
			if (damage > 0)
			{
				bSelfDamaged = true;
				break;
			}
		}
		if (!bSelfDamaged)
		{
			for (auto damage : totalfdamage_combined)
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

void KanDataConnector::processHouraiDamages(const kcsapi_battle_hougeki* api_hougeki
	, QList<float>* totalfdamage
	, QList<float>* totaledamage
	, QList<float>* totalfdamage_combined
	, QList<float>* totaledamage_combined
	, bool bOnlyCombinedSelf
	, bool bOnlyCombinedEnemy)
{
	// must skip 0!!!
	QList<float>* fdamage = totalfdamage;
	QList<float>* edamage = totaledamage;

	// without instruction
	if (api_hougeki->api_at_eflag.isEmpty())
	{
		if (bOnlyCombinedSelf)
		{
			fdamage = totalfdamage_combined;
		}
		if (bOnlyCombinedEnemy)
		{
			edamage = totaledamage_combined;
		}
		for (int j = 1; j < api_hougeki->api_at_list.count(); j++)
		{
			bool bfattack = true;
			bool bfdefend = true;

			int attackpos = api_hougeki->api_at_list[j];
			if (attackpos > 6)
			{
				bfattack = false;
				attackpos -= 6;
			}

			if (attackpos > 0)
			{
				for (int k = 0; k < api_hougeki->api_df_list[j].count(); k++)
				{
					int defendpos = api_hougeki->api_df_list[j][k];
					if (defendpos > 6)
					{
						bfdefend = false;
						defendpos -= 6;
					}

					if (defendpos > 0)
					{
						if (bfdefend)
						{
							(*fdamage)[defendpos] += api_hougeki->api_damage[j][k];
						}
						else
						{
							(*edamage)[defendpos] += api_hougeki->api_damage[j][k];
						}
					}
				}
			}
		}
	}
	else
	{
		for (int j = 1; j < api_hougeki->api_at_list.count(); j++)
		{
			for (int k = 0; k < api_hougeki->api_df_list[j].count(); k++)
			{
				int defendpos = api_hougeki->api_df_list[j][k];

				if (defendpos > 0)
				{
					if (api_hougeki->api_at_eflag[j] > 0)
					{
						// from enemy
						if (defendpos > 6)
						{
							fdamage = totalfdamage_combined;
							defendpos -= 6;
						}
						else
						{
							fdamage = totalfdamage;
						}
						(*fdamage)[defendpos] += api_hougeki->api_damage[j][k];
					}
					else
					{
						if (defendpos > 6)
						{
							edamage = totaledamage_combined;
							defendpos -= 6;
						}
						else
						{
							edamage = totaledamage;
						}
						(*edamage)[defendpos] += api_hougeki->api_damage[j][k];
					}
				}
			}
		}
	}

}
