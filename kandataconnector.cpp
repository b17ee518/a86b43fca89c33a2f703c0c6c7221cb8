#include "kandataconnector.h"
#include "kandata.h"
#include "kansavedata.h"
#include "kandatacalc.h"
#include <QJsonDocument>
#include <QJsonObject>

#include "infomainwindow.h"
#include "mainwindow.h"
#include "kqtime.h"
#include "klog.h"
#include "ControlManager.h"

#define DAPILOG() APILOG(_pathAndQuery, _requestBody, _responseBody)
#define DAPILOGDEBUG() APILOGDEBUG(_pathAndQuery, _requestBody, _responseBody)

/*
 * End points
 /kcsapi/api_start2
 /kcsapi/api_port/port
 /kcsapi/api_get_member/basic
 /kcsapi/api_get_member/ship
 /kcsapi/api_get_member/ship2
 /kcsapi/api_get_member/ship3
 /kcsapi/api_get_member/ship_deck
 /kcsapi/api_get_member/slot_item
 /kcsapi/api_get_member/useitem
 /kcsapi/api_get_member/deck
 /kcsapi/api_get_member/deck_port
 /kcsapi/api_get_member/ndock
 /kcsapi/api_get_member/kdock
 /kcsapi/api_get_member/material

 /kcsapi/api_req_hensei/change
 /kcsapi/api_req_hensei/lock
 /kcsapi/api_req_hensei/combined
 /kcsapi/api_get_member/unsetslot
 /kcsapi/api_req_kaisou/unsetslot_all

 /kcsapi/api_req_kousyou/getship
 /kcsapi/api_req_kousyou/createitem
 /kcsapi/api_req_kousyou/createship
 /kcsapi/api_req_kousyou/createship_speedchange
 /kcsapi/api_req_kousyou/destroyship
 /kcsapi/api_req_kousyou/destroyitem2
 /kcsapi/api_req_nyukyo/start
 /kcsapi/api_req_nyukyo/speedchange

 /kcsapi/api_req_hokyu/charge

 /kcsapi/api_req_kaisou/powerup
 /kcsapi/api_req_kaisou/slotset
 /kcsapi/api_req_kaisou/remodeling

 /kcsapi/api_req_mission/start
 /kcsapi/api_req_mission/result
 /kcsapi/api_get_member/mission

 /kcsapi/api_get_member/mapinfo
 /kcsapi/api_get_member/mapcell
 /kcsapi/api_req_map/start
 /kcsapi/api_req_map/next

 /kcsapi/api_req_sortie/battleresult
 /kcsapi/api_req_sortie/battle
 /kcsapi/api_req_battle_midnight/battle
 /kcsapi/api_req_battle_midnight/sp_midnight
 /kcsapi/api_req_sortie/night_to_day

 /kcsapi/api_req_combined_battle/airbattle
 /kcsapi/api_req_combined_battle/battleresult
 /kcsapi/api_req_combined_battle/battle
 /kcsapi/api_req_combined_battle/midnight_battle
 /kcsapi/api_req_combined_battle/sp_midnight

 /kcsapi/api_req_combined_battle/goback_port

 /kcsapi/api_get_member/practice
 /kcsapi/api_req_member/get_practice_enemyinfo
 /kcsapi/api_req_practice/battle
 /kcsapi/api_req_practice/midnight_battle
 /kcsapi/api_req_practice/battle_result

 /kcsapi/api_get_member/questlist
 /kcsapi/api_req_quest/start
 /kcsapi/api_req_quest/stop
 /kcsapi/api_req_quest/clearitemget

 /kcsapi/api_get_member/sortie_conditions
 /kcsapi/api_req_member/updatedeckname
 /kcsapi/api_req_member/updatecomment
 /kcsapi/api_req_kousyou/open_new_dock
 /kcsapi/api_req_nyukyo/open_new_dock
 /kcsapi/api_auth_member/logincheck
 /kcsapi/api_get_member/furniture
 /kcsapi/api_req_furniture/buy
 /kcsapi/api_req_furniture/change
 /kcsapi/api_req_member/itemuse
 /kcsapi/api_req_member/itemuse_cond
 /kcsapi/api_get_member/record
 /kcsapi/api_req_ranking/getlist
 /kcsapi/api_get_member/picture_book
 /kcsapi/api_get_member/book2
 /kcsapi/api_req_member/get_incentive
 /kcsapi/api_get_member/payitem
 /kcsapi/api_req_map/select_eventmap_rank

 /kcsapi/api_dmm_payment/paycheck

 /kcsapi/api_req_kaisou/open_exslot
 /kcsapi/api_req_kaisou/slotset_ex

*/

bool questDataSort(const kcsapi_quest &left, const kcsapi_quest &right)
{
	return left.api_no < right.api_no;
}


KanDataConnector::KanDataConnector(void)
{
	pksd = &KanSaveData::getInstance();
	_colWhite = QColor(255, 255, 255);
	_colGray = QColor(192, 192, 192);
	_colOrange = QColor(255, 153, 0);
	_colYellow = QColor(255, 255, 0);
	_colRed = QColor(255, 0, 0);
	_colBlue = QColor(0, 0, 255);
	_colAqua = QColor(0, 255, 255);

	start2_flag = PARSEFLAG_NORMAL;
	port_port_flag = PARSEFLAG_NORMAL;
	get_member_basic_flag = PARSEFLAG_NORMAL;
	get_member_ship_flag = PARSEFLAG_NORMAL;
	get_member_ship2_flag = PARSEFLAG_NORMAL;
	get_member_ship3_flag = PARSEFLAG_NORMAL;
	get_member_ship_deck_flag = PARSEFLAG_NORMAL;
	get_member_slot_item_flag = PARSEFLAG_NORMAL;
	get_member_useitem_flag = PARSEFLAG_NORMAL;
	get_member_deck_flag = PARSEFLAG_NORMAL;
	get_member_deck_port_flag = PARSEFLAG_NORMAL;
	get_member_ndock_flag = PARSEFLAG_NORMAL;
	get_member_kdock_flag = PARSEFLAG_NORMAL;
	get_member_material_flag = PARSEFLAG_NORMAL;
	req_hensei_change_flag = PARSEFLAG_NORMAL;
	req_hensei_lock_flag = PARSEFLAG_NORMAL;
	req_hensei_combined_flag = PARSEFLAG_NORMAL;
	get_member_unsetslot_flag = PARSEFLAG_NORMAL;
	req_kaisou_unsetslot_all_flag = PARSEFLAG_NORMAL;
	req_kousyou_getship_flag = PARSEFLAG_NORMAL;
	req_kousyou_createitem_flag = PARSEFLAG_NORMAL;
	req_kousyou_createship_flag = PARSEFLAG_NORMAL;
	req_kousyou_createship_speedchange_flag = PARSEFLAG_NORMAL;
	req_kousyou_destroyship_flag = PARSEFLAG_NORMAL;
	req_kousyou_destroyitem2_flag = PARSEFLAG_NORMAL;
	req_nyukyo_start_flag = PARSEFLAG_NORMAL;
	req_nyukyo_speedchange_flag = PARSEFLAG_NORMAL;
	req_hokyu_charge_flag = PARSEFLAG_NORMAL;
	req_kaisou_powerup_flag = PARSEFLAG_NORMAL;
	req_kaisou_slotset_flag = PARSEFLAG_NORMAL;
	req_kaisou_remodeling_flag = PARSEFLAG_NORMAL;
	req_mission_start_flag = PARSEFLAG_NORMAL;
	req_mission_result_flag = PARSEFLAG_NORMAL;
	get_member_mission_flag = PARSEFLAG_NORMAL;
	get_member_mapinfo_flag = PARSEFLAG_NORMAL;
	get_member_mapcell_flag = PARSEFLAG_NORMAL;
	req_map_start_flag = PARSEFLAG_NORMAL;
	req_map_next_flag = PARSEFLAG_NORMAL;
	req_sortie_battleresult_flag = PARSEFLAG_NORMAL;
	req_sortie_battle_flag = PARSEFLAG_NORMAL;
	req_battle_midnight_battle_flag = PARSEFLAG_NORMAL;
	req_battle_midnight_sp_midnight_flag = PARSEFLAG_NORMAL;
	req_sortie_night_to_day_flag = PARSEFLAG_NORMAL;
	req_sortie_airbattle_flag = PARSEFLAG_NORMAL;
	req_combined_battle_airbattle_flag = PARSEFLAG_NORMAL;
	req_combined_battle_battlewater_flag = PARSEFLAG_NORMAL;
	req_combined_battle_battleresult_flag = PARSEFLAG_NORMAL;
	req_combined_battle_battle_flag = PARSEFLAG_NORMAL;
	req_combined_battle_midnight_battle_flag = PARSEFLAG_NORMAL;
	req_combined_battle_sp_midnight_flag = PARSEFLAG_NORMAL;
	req_combined_battle_goback_port_flag = PARSEFLAG_NORMAL;
	get_member_practice_flag = PARSEFLAG_NORMAL;
	req_member_get_practice_enemyinfo_flag = PARSEFLAG_NORMAL;
	req_practice_battle_flag = PARSEFLAG_NORMAL;
	req_practice_midnight_battle_flag = PARSEFLAG_NORMAL;
	req_practice_battle_result_flag = PARSEFLAG_NORMAL;
	get_member_questlist_flag = PARSEFLAG_NORMAL;
	req_quest_start_flag = PARSEFLAG_NORMAL;
	req_quest_stop_flag = PARSEFLAG_NORMAL;
	req_quest_clearitemget_flag = PARSEFLAG_NORMAL;
	get_member_sortie_conditions_flag = PARSEFLAG_NORMAL;
	req_member_updatedeckname_flag = PARSEFLAG_NORMAL;
	req_member_updatecomment_flag = PARSEFLAG_NORMAL;
	req_kousyou_open_new_dock_flag = PARSEFLAG_NORMAL;
	req_nyukyo_open_new_dock_flag = PARSEFLAG_NORMAL;
	auth_member_logincheck_flag = PARSEFLAG_NORMAL;
	get_member_furniture_flag = PARSEFLAG_NORMAL;
	req_furniture_buy_flag = PARSEFLAG_NORMAL;
	req_furniture_change_flag = PARSEFLAG_NORMAL;
	req_member_itemuse_flag = PARSEFLAG_NORMAL;
	req_member_itemuse_cond_flag = PARSEFLAG_NORMAL;
	get_member_record_flag = PARSEFLAG_NORMAL;
	req_ranking_getlist_flag = PARSEFLAG_NORMAL;
	get_member_picture_book_flag = PARSEFLAG_NORMAL;
	get_member_book2_flag = PARSEFLAG_NORMAL;
	req_member_get_incentive_flag = PARSEFLAG_NORMAL;
	get_member_payitem_flag = PARSEFLAG_NORMAL;
	req_kaisou_lock_flag = PARSEFLAG_NORMAL;			// lock slot item
	req_mission_return_instruction_flag = PARSEFLAG_NORMAL;	// force mission return
	req_member_payitemuse_flag = PARSEFLAG_NORMAL;
	req_kaisou_marriage_flag = PARSEFLAG_NORMAL;
	req_kousyou_remodel_slotlist_flag = PARSEFLAG_NORMAL;
	req_kousyou_remodel_slotlist_detail_flag = PARSEFLAG_NORMAL;
	req_kousyou_remodel_slot_flag = PARSEFLAG_NORMAL;		// kaisyu
	req_furniture_music_list_flag = PARSEFLAG_NORMAL;
	req_furniture_music_play_flag = PARSEFLAG_NORMAL;
	req_furniture_set_portbgm_flag = PARSEFLAG_NORMAL;
	req_map_select_eventmap_rank_flag = PARSEFLAG_NORMAL;


	/************************************************************************/
	/*                                                                      */
	/************************************************************************/

//	req_member_itemuse_cond_flag = PARSEFLAG_OUTPUT;
//	req_sortie_battleresult_flag = PARSEFLAG_OUTPUT;
//	req_sortie_battle_flag = PARSEFLAG_OUTPUT;
//	req_battle_midnight_battle_flag = PARSEFLAG_OUTPUT;
//	req_battle_midnight_sp_midnight_flag = PARSEFLAG_OUTPUT;
	req_sortie_night_to_day_flag = PARSEFLAG_OUTPUT;
//	start2_flag = PARSEFLAG_OUTPUT;
//	port_port_flag = PARSEFLAG_OUTPUT;
	req_sortie_airbattle_flag = PARSEFLAG_OUTPUT;
	req_combined_battle_airbattle_flag = PARSEFLAG_OUTPUT;
	req_combined_battle_battlewater_flag = PARSEFLAG_OUTPUT;
	req_combined_battle_battleresult_flag = PARSEFLAG_OUTPUT;
	req_combined_battle_battle_flag = PARSEFLAG_OUTPUT;
	req_combined_battle_midnight_battle_flag = PARSEFLAG_OUTPUT;
	req_combined_battle_sp_midnight_flag = PARSEFLAG_OUTPUT;

//	req_kaisou_remodeling_flag = PARSEFLAG_OUTPUT;
//	req_kousyou_remodel_slot_flag = PARSEFLAG_OUTPUT;
}

bool KanDataConnector::Parse(const QString& pathAndQuery, const QString& requestBody, const QString& responseBody)
{
	_pathAndQuery = pathAndQuery;
	_requestBody = requestBody;
	// svdata=
	_responseBody = responseBody.right(responseBody.length() - 7);

	_jdoc = QJsonDocument::fromJson(_responseBody.toLocal8Bit());
	int api_result = _jdoc.object()["api_result"].toInt();
	if (api_result != 1)
	{
		DAPILOG();
		return false;
	}
	_jobj = _jdoc.object()["api_data"].toObject();

	_req.ReadFromString(pathAndQuery, requestBody);

	bool bRet = false;

#define PARSEAPIF(apistr, api)\
	if (_pathAndQuery == apistr)\
	{\
		if ((##api##_flag & PARSEFLAG_IGNORE) == 0)\
		{\
			if (##api##_flag & PARSEFLAG_OUTPUT)\
			{\
				DAPILOG();\
			}\
			bRet = ##api##_parse();\
		}\
	}
#define PARSEAPI(apistr, api) else PARSEAPIF(apistr, api)

	PARSEAPIF("/kcsapi/api_start2", start2)
		PARSEAPI("/kcsapi/api_port/port", port_port)
		PARSEAPI("/kcsapi/api_get_member/basic", get_member_basic)
		PARSEAPI("/kcsapi/api_get_member/ship", get_member_ship)
		PARSEAPI("/kcsapi/api_get_member/ship2", get_member_ship2)
		PARSEAPI("/kcsapi/api_get_member/ship3", get_member_ship3)
		PARSEAPI("/kcsapi/api_get_member/ship_deck", get_member_ship_deck)
		PARSEAPI("/kcsapi/api_get_member/slot_item", get_member_slot_item)
		PARSEAPI("/kcsapi/api_get_member/useitem", get_member_useitem)
		PARSEAPI("/kcsapi/api_get_member/deck", get_member_deck)
		PARSEAPI("/kcsapi/api_get_member/deck_port", get_member_deck_port)
		PARSEAPI("/kcsapi/api_get_member/ndock", get_member_ndock)
		PARSEAPI("/kcsapi/api_get_member/kdock", get_member_kdock)
		PARSEAPI("/kcsapi/api_get_member/material", get_member_material)
		PARSEAPI("/kcsapi/api_req_hensei/change", req_hensei_change)
		PARSEAPI("/kcsapi/api_req_hensei/lock", req_hensei_lock)
		PARSEAPI("/kcsapi/api_req_hensei/combined", req_hensei_combined)
		PARSEAPI("/kcsapi/api_get_member/unsetslot", get_member_unsetslot)
		PARSEAPI("/kcsapi/api_req_kaisou/unsetslot_all", req_kaisou_unsetslot_all)
		PARSEAPI("/kcsapi/api_req_kousyou/getship", req_kousyou_getship)
		PARSEAPI("/kcsapi/api_req_kousyou/createitem", req_kousyou_createitem)
		PARSEAPI("/kcsapi/api_req_kousyou/createship", req_kousyou_createship)
		PARSEAPI("/kcsapi/api_req_kousyou/createship_speedchange", req_kousyou_createship_speedchange)
		PARSEAPI("/kcsapi/api_req_kousyou/destroyship", req_kousyou_destroyship)
		PARSEAPI("/kcsapi/api_req_kousyou/destroyitem2", req_kousyou_destroyitem2)
		PARSEAPI("/kcsapi/api_req_nyukyo/start", req_nyukyo_start)
		PARSEAPI("/kcsapi/api_req_nyukyo/speedchange", req_nyukyo_speedchange)
		PARSEAPI("/kcsapi/api_req_hokyu/charge", req_hokyu_charge)
		PARSEAPI("/kcsapi/api_req_kaisou/powerup", req_kaisou_powerup)
		PARSEAPI("/kcsapi/api_req_kaisou/slotset", req_kaisou_slotset)
		PARSEAPI("/kcsapi/api_req_kaisou/remodeling", req_kaisou_remodeling)
		PARSEAPI("/kcsapi/api_req_mission/start", req_mission_start)
		PARSEAPI("/kcsapi/api_req_mission/result", req_mission_result)
		PARSEAPI("/kcsapi/api_get_member/mission", get_member_mission)
		PARSEAPI("/kcsapi/api_get_member/mapinfo", get_member_mapinfo)
		PARSEAPI("/kcsapi/api_get_member/mapcell", get_member_mapcell)
		PARSEAPI("/kcsapi/api_req_map/start", req_map_start)
		PARSEAPI("/kcsapi/api_req_map/next", req_map_next)
		PARSEAPI("/kcsapi/api_req_sortie/battleresult", req_sortie_battleresult)
		PARSEAPI("/kcsapi/api_req_sortie/battle", req_sortie_battle)
		PARSEAPI("/kcsapi/api_req_battle_midnight/battle", req_battle_midnight_battle)
		PARSEAPI("/kcsapi/api_req_battle_midnight/sp_midnight", req_battle_midnight_sp_midnight)
		PARSEAPI("/kcsapi/api_req_sortie/night_to_day", req_sortie_night_to_day)
		PARSEAPI("/kcsapi/api_req_sortie/airbattle", req_sortie_airbattle)
		PARSEAPI("/kcsapi/api_req_combined_battle/airbattle", req_combined_battle_airbattle)
		PARSEAPI("/kcsapi/api_req_combined_battle/battle_water", req_combined_battle_battlewater)
		PARSEAPI("/kcsapi/api_req_combined_battle/battleresult", req_combined_battle_battleresult)
		PARSEAPI("/kcsapi/api_req_combined_battle/battle", req_combined_battle_battle)
		PARSEAPI("/kcsapi/api_req_combined_battle/midnight_battle", req_combined_battle_midnight_battle)
		PARSEAPI("/kcsapi/api_req_combined_battle/sp_midnight", req_combined_battle_sp_midnight)
		PARSEAPI("/kcsapi/api_req_combined_battle/goback_port", req_combined_battle_goback_port)
		PARSEAPI("/kcsapi/api_get_member/practice", get_member_practice)
		PARSEAPI("/kcsapi/api_req_member/get_practice_enemyinfo", req_member_get_practice_enemyinfo)
		PARSEAPI("/kcsapi/api_req_practice/battle", req_practice_battle)
		PARSEAPI("/kcsapi/api_req_practice/midnight_battle", req_practice_midnight_battle)
		PARSEAPI("/kcsapi/api_req_practice/battle_result", req_practice_battle_result)
		PARSEAPI("/kcsapi/api_get_member/questlist", get_member_questlist)
		PARSEAPI("/kcsapi/api_req_quest/start", req_quest_start)
		PARSEAPI("/kcsapi/api_req_quest/stop", req_quest_stop)
		PARSEAPI("/kcsapi/api_req_quest/clearitemget", req_quest_clearitemget)
		PARSEAPI("/kcsapi/api_get_member/sortie_conditions", get_member_sortie_conditions)
		PARSEAPI("/kcsapi/api_req_member/updatedeckname", req_member_updatedeckname)
		PARSEAPI("/kcsapi/api_req_member/updatecomment", req_member_updatecomment)
		PARSEAPI("/kcsapi/api_req_kousyou/open_new_dock", req_kousyou_open_new_dock)
		PARSEAPI("/kcsapi/api_req_nyukyo/open_new_dock", req_nyukyo_open_new_dock)
		PARSEAPI("/kcsapi/api_auth_member/logincheck", auth_member_logincheck)
		PARSEAPI("/kcsapi/api_get_member/furniture", get_member_furniture)
		PARSEAPI("/kcsapi/api_req_furniture/buy", req_furniture_buy)
		PARSEAPI("/kcsapi/api_req_furniture/change", req_furniture_change)
		PARSEAPI("/kcsapi/api_req_member/itemuse", req_member_itemuse)
		PARSEAPI("/kcsapi/api_req_member/itemuse_cond", req_member_itemuse_cond)
		PARSEAPI("/kcsapi/api_get_member/record", get_member_record)
		PARSEAPI("/kcsapi/api_req_ranking/getlist", req_ranking_getlist)
		PARSEAPI("/kcsapi/api_get_member/picture_book", get_member_picture_book)
		PARSEAPI("/kcsapi/api_get_member/book2", get_member_book2)
		PARSEAPI("/kcsapi/api_req_member/get_incentive", req_member_get_incentive)
		PARSEAPI("/kcsapi/api_get_member/payitem", get_member_payitem)
		PARSEAPI("/kcsapi/api_req_kaisou/lock", req_kaisou_lock)
		PARSEAPI("/kcsapi/api_req_mission/return_instruction", req_mission_return_instruction)
		PARSEAPI("/kcsapi/api_req_member/payitemuse", req_member_payitemuse)
		PARSEAPI("/kcsapi/api_req_kaisou/marriage", req_kaisou_marriage)
		PARSEAPI("/kcsapi/api_req_kousyou/remodel_slotlist", req_kousyou_remodel_slotlist)
		PARSEAPI("/kcsapi/api_req_kousyou/remodel_slotlist_detail", req_kousyou_remodel_slotlist_detail)
		PARSEAPI("/kcsapi/api_req_kousyou/remodel_slot", req_kousyou_remodel_slot)
		PARSEAPI("/kcsapi/api_req_furniture/music_list", req_furniture_music_list)
		PARSEAPI("/kcsapi/api_req_furniture/music_play", req_furniture_music_play)
		PARSEAPI("/kcsapi/api_req_furniture/set_portbgm", req_furniture_set_portbgm)
		PARSEAPI("/kcsapi/api_req_map/select_eventmap_rank", req_map_select_eventmap_rank)
		PARSEAPI("/kcsapi/api_dmm_payment/paycheck", dmm_payment_paycheck)
		PARSEAPI("/kcsapi/api_req_kaisou/open_exslot", req_kaisou_open_exslot)
		PARSEAPI("/kcsapi/api_req_kaisou/slotset_ex", req_kaisou_slotset_ex)
		PARSEAPI("/kcsapi/api_req_member/itemuse_cond", req_member_itemuse_cond)

	if (!bRet)
	{
		DAPILOG();
	}
	else
	{
		if (ControlManager::getInstance().isRunning())
		{
			ControlManager::getInstance().setDoneRequest(pathAndQuery);
		}
	}
	if (_outputAllLog)
	{
		DAPILOGDEBUG();
	}

	return false;
	/*
/kcaspi/api_req_member/getothersdeck
/kcsapi/api_get_member/actionlog
/kcsapi/api_req_init/nickname
/kcsapi/api_req_init/firstship
/kcsapi/api_world/get_worldinfo
/kcsapi/api_world/register
/kcsapi/api_start
	*/

}

void KanDataConnector::updateOverviewTable()
{
	QList<QString> lst;
	QList<QColor> cols;
	KanSaveData * pksd = &KanSaveData::getInstance();

	int kancount = pksd->portdata.api_ship.count()+pksd->shipcountoffset;
	int kanmaxcount = pksd->portdata.api_basic.api_max_chara;
	int slotitemcount = pksd->slotitemdata.count()+pksd->slotitemcountoffset;
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
	lst.append(QString::fromLocal8Bit("ボス: %1(%2/%3) 南西: %4 輸: %5 航: %6 潜: %7 出撃:%8")
		.arg(pksd->totalBossReached)
		.arg(pksd->totalBossWin)
		.arg(pksd->totalBossSRank)
		.arg(pksd->totalSouthEastWin)
		.arg(pksd->totalKilledYusou)
		.arg(pksd->totalKilledKubou)
		.arg(pksd->totalKilledSensui)
		.arg(pksd->totalSortie));

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

	MainWindow::infoWindow()->updateOverviewTable(lst, cols);
}

void KanDataConnector::updateMissionTable()
{
	KanSaveData * pksd = &KanSaveData::getInstance();

	QList<KQRowData> rows;

	int questcount = pksd->questdata.count();
	for (int i=0; i<questcount; i++)
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
	foreach (const kcsapi_deck &v, pksd->portdata.api_deck_port)
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

		int alltaiku=0;
		int alltaikubonus = 0;
		int alllv=0;
		int flagshipLv = 0;
		int allsakuteki = 0;
		double allsakuteki_sp = 0;

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

			for (int i = 0; i<slotitems.count(); i++)
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
									|| type == SlotitemType::KanBaKu
									|| type == SlotitemType::KanKou
									|| type == SlotitemType::SuiBaKu)
								{
									taiku += (int)(pmstslotitem->api_tyku*sqrt((double)pship->api_onslot[i]));
									// bonus
									int alv = v.api_alv;
									if (type == SlotitemType::KanSen)
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
									else if (type == SlotitemType::KanKou || type == SlotitemType::KanBaKu)
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
								switch (type)
								{
								case SlotitemType::KanBaKu:
									allsakuteki_sp += pmstslotitem->api_saku * 1.0376255;
									break;
								case SlotitemType::KanKou:
									allsakuteki_sp += pmstslotitem->api_saku * 1.3677954;
									break;
								case SlotitemType::TeiSaTsu:
									allsakuteki_sp += pmstslotitem->api_saku * 1.6592780;
									break;
								case SlotitemType::SuiTei:
									allsakuteki_sp += pmstslotitem->api_saku * 2.0;
									break;
								case SlotitemType::SuiBaKu:
									allsakuteki_sp += pmstslotitem->api_saku * 1.7787282;
									break;
								case SlotitemType::DenTan_S:
									allsakuteki_sp += pmstslotitem->api_saku * 1.0045358;
									break;
								case SlotitemType::DenTan_L:
									allsakuteki_sp += pmstslotitem->api_saku * 0.9906638;
									break;
								case SlotitemType::TanSyouTou:
									allsakuteki_sp += pmstslotitem->api_saku * 0.9067950;
									break;
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
			allsakuteki_sp += sqrt((double)pship->api_sakuteki[0]) * 1.6841056;
			
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
		allsakuteki_sp += ((pksd->portdata.api_basic.api_level + 5) / 5) * 5 * (-0.6142467);

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

		QString strtitle = QString::fromLocal8Bit("%1 (Lv計:%2 制空:%3[%4] 索敵:%5[%6])")
			.arg(v.api_name)
			.arg(alllv)
			.arg(alltaiku)
			.arg(alltaiku+alltaikubonus)
			.arg(allsakuteki)
			.arg((int)allsakuteki_sp);
		MainWindow::infoWindow()->updateFleetTable(v.api_id-1, strtitle, colindex, bRed, rows);
	}

	MainWindow::shipWindow()->buildTable();
}

void KanDataConnector::updateExpeditionTable()
{
	KanSaveData * pksd = &KanSaveData::getInstance();
	foreach (const kcsapi_deck &v, pksd->portdata.api_deck_port)
	{
		if (v.api_id <= 1)
		{
			continue;
		}
		if (v.api_mission[0] <= 0)
		{
			MainWindow::timerWindow()->setExpeditionTime(v.api_id-2);
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
	foreach (const kcsapi_ndock &v, pksd->portdata.api_ndock)
	{
		if (v.api_ship_id == 0)
		{
			MainWindow::timerWindow()->setRepairTime(v.api_id-1);
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
	foreach (const kcsapi_kdock &v, pksd->kdockdata)
	{
		if (v.api_state < 0)
		{
			continue;
		}
		if (v.api_created_ship_id == 0)
		{
			MainWindow::timerWindow()->setBuildTime(v.api_id-1);
		}
		else
		{
			int shipid = v.api_created_ship_id;
//            const kcsapi_ship2 *pship = findShipFromShipno(shipno);
			const kcsapi_mst_ship *pmstship = findMstShipFromShipid(shipid);
			MainWindow::timerWindow()->setBuildTime(v.api_id-1, v.api_complete_time, pmstship->api_buildtime, pmstship->api_name);
		}
	}

}

void KanDataConnector::updateInfoTitleBattle(bool bBattle, QList<int> * enemyhps, bool bSelfDamaged)
{
	KanSaveData * pksd = &KanSaveData::getInstance();
	if (pksd->nextdata.api_no < 0 && !enemyhps)
	{
		MainWindow::infoWindow()->updateTitle("", false);
		return;
	}
	QString strtitle;

	if (pksd->nextdata.api_no >= 0)
	{
		strtitle = QString::fromLocal8Bit("現:%1 ボス:%2")
			.arg(pksd->nextdata.api_no)
			.arg(pksd->nextdata.api_bosscell_no);
	}

	if (bBattle)
	{
		pksd->lastWonAssumption = false;

		int totalremain = 0;
		int acremain = 0;
		int actotal = 0;
		int transremain = 0;
		int transtotal = 0;
		int subremain = 0;
		int subtotal = 0;

		int totalecount = 0;
		for (int i = 1; i<pksd->battledata.api_ship_ke.count(); i++)
		{
			int shipid = pksd->battledata.api_ship_ke[i];
			if (shipid > 0)
			{
				const kcsapi_mst_ship * pmstship = findMstShipFromShipid(shipid);
				if (pmstship)
				{
					totalecount++;
					if ((*enemyhps)[i] > 0)
					{
						totalremain++;
					}
					else if (i == 1)
					{
						pksd->lastWonAssumption = true;
					}
					switch (static_cast<ShipType>(pmstship->api_stype))
					{
					case ShipType::KuBou:
					case ShipType::KeiKuBou:
					case ShipType::SouKaKuBou:
						actotal++;
						if ((*enemyhps)[i] > 0)
						{
							acremain++;
						}
						break;
					case ShipType::HoKyu:
					case ShipType::HoKyu_HaYaSuI:
						transtotal++;
						if ((*enemyhps)[i] > 0)
						{
							transremain++;
						}
						break;
					case ShipType::SenSui:
					case ShipType::SenBou:
						subtotal++;
						if ((*enemyhps)[i] > 0)
						{
							subremain++;
						}
						break;
					}
					
				}
			}
		}

		QString eflagshipremainstr = "";
		if ((*enemyhps)[1] > 0)
		{
			eflagshipremainstr = QString::fromLocal8Bit("旗");
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

		QString loseStr = "";
		if (!pksd->lastWonAssumption)
		{
			loseStr = QString::fromLocal8Bit("敗");
		}

		strtitle += QString::fromLocal8Bit(" - %1残:%2%3, 輸%4(%5), 航:%6(%7), 潜%8(%9)")
			.arg(loseStr)
			.arg(totalremain)
			.arg(eflagshipremainstr)
			.arg(transremain)
			.arg(transtotal)
			.arg(acremain)
			.arg(actotal)
			.arg(subremain)
			.arg(subtotal);

		pksd->wasLastBossCell = (pksd->nextdata.api_no == pksd->nextdata.api_bosscell_no);
		pksd->lastKilledYusou = transtotal - transremain;
		pksd->lastKilledKubou = actotal - acremain;
		pksd->lastKilledSensui = subtotal - subremain;
	}

	int colindex = 0;
	if (pksd->nextdata.api_no==pksd->nextdata.api_bosscell_no)
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
	int keijuncount = 0;
	int keijunkiras = 0;
	int kuchikucount = 0;
	int kuchikukiras = 0;

	int lvmin = 20;

	foreach (const kcsapi_ship2 &v, pksd->portdata.api_ship)
	{
		int shipid = v.api_ship_id;
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

		if (shipid > 0)
		{
			const kcsapi_mst_ship * pmstship = findMstShipFromShipid(shipid);
			if (pmstship)
			{
				if (pmstship->api_stype == (int)ShipType::SuiBou)
				{
					if (v.api_locked && v.api_lv >= lvmin)
					{
						suibocount++;
						if (KanDataCalc::GetCondState(v.api_cond) == CondState::Kira)
						{
							suibokiras++;
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
					if (v.api_locked && v.api_lv >= lvmin)
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

	QString titlestr = QString::fromLocal8Bit("キラ - 水母:%1(%2) 軽巡:%3(%4) 駆逐:%5(%6)")
		.arg(suibokiras)
		.arg(suibocount - suibokiras)
		.arg(keijunkiras)
		.arg(keijuncount - keijunkiras)
		.arg(kuchikukiras)
		.arg(kuchikucount - kuchikukiras);

	int colindex = 0;
	if (suibokiras == suibocount && kuchikukiras*3 >= kuchikucount*2 && keijunkiras*3 >= keijuncount)
	{
		colindex = 1;
	}
	else if (suibokiras < suibocount || kuchikukiras < 10 || keijunkiras < 4)
	{
		colindex = 2;
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
		if (pksd->portdata.api_deck_port.size())
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
	for (QList<kcsapi_ship2>::iterator it=pksd->portdata.api_ship.begin();
		 it!=pksd->portdata.api_ship.end(); ++it)
	{
		if (it->api_id == shipno)
		{
            foreach (auto itemid , it->api_slot)
			{
				RemoveSlotItem(itemid);
			}
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
    foreach (auto& it , pksd->portdata.api_ship)
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
            foreach (auto slotitemid , slotitems)
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
	for (QList<kcsapi_ship2>::iterator it=pksd->portdata.api_ship.begin();
		 it!=pksd->portdata.api_ship.end(); ++it)
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


bool KanDataConnector::isShipHasSlotitem(kcsapi_ship2* pship, SlotitemType sitype)
{
	if (!pship)
	{
		return false;
	}

	for (auto slotitemid : pship->api_slot)
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
							return true;
						}
					}
				}
			}
		}
	}
	return false;
}

QList<int> KanDataConnector::updateBattle(const kcsapi_battle &api_battle, KanBattleType type)
{
	int dockid = 0;
	int dockid_combined = -1;
	bool bCombined = false;
	if (type < KanBattleType::Combined_Begin)
	{
		dockid = api_battle.api_dock_id - 1;
		// midnight
		if (type == KanBattleType::Night || type == KanBattleType::DayToNight || dockid < 0)
		{
			dockid = api_battle.api_deck_id - 1;
		}
		pksd->bCombined = bCombined;
	}
	else
	{
		bCombined = true;
		dockid_combined = dockid+1;
		pksd->bCombined = bCombined;
	}

	QList<int> enemyhps;

	if (dockid >= 0)
	{
		pksd->lastbattletype = type;

		if (type != KanBattleType::DayToNight
			&& type != KanBattleType::NightToDay
			&& type != KanBattleType::Combined_DayToNight)
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
		if (dockid_combined >= 0)
		{
			foreach(int shipid, pksd->portdata.api_deck_port[dockid_combined].api_ship)
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

		QList<int> api_nowhps = api_battle.api_nowhps;
		QList<int> api_nowhps_combined = api_battle.api_nowhps_combined;
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
				if (bCombined)
				{
					for (int i = 1; i < api_battle.api_kouku.api_stage3_combined.api_fdam.count(); i++)
					{
						totalfdamage_combined[i] += api_battle.api_kouku.api_stage3_combined.api_fdam[i];
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
				if (bCombined)
				{
					// kouku2
					for (int i = 1; i < api_battle.api_kouku2.api_stage3_combined.api_fdam.count(); i++)
					{
						totalfdamage_combined[i] += api_battle.api_kouku2.api_stage3_combined.api_fdam[i];
					}
				}

			}
		}

		if (type != KanBattleType::Night && type != KanBattleType::DayToNight && type != KanBattleType::Combined_KouKuNight && type != KanBattleType::Combined_DayToNight && type != KanBattleType::Combined_Night)
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
						}
					}

				}
					break;
				case 2: // hougeki
				case 3: // raigeki
				{
					for (int i = 1; i < api_battle.api_support_info.api_support_hourai.api_damage.count(); i++)
					{
						totaledamage[i] += api_battle.api_support_info.api_support_hourai.api_damage[i];
					}
				}
					break;
				default:
					break;
				}
			}

			// opening
			// TODO: combined?
			if (api_battle.api_opening_flag > 0)
			{
				for (int i = 0; i < api_battle.api_opening_atack.api_fdam.count(); i++)
				{
					if (bCombined/*api_battle.api_formation[0] == 11*/)
					{
						totalfdamage_combined[i] += api_battle.api_opening_atack.api_fdam[i];
					}
					else
					{
						totalfdamage[i] += api_battle.api_opening_atack.api_fdam[i];
					}
				}
				for (int i = 0; i < api_battle.api_opening_atack.api_edam.count(); i++)
				{
					totaledamage[i] += api_battle.api_opening_atack.api_edam[i];
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
				if (bCombined && type == KanBattleType::Combined_KouKu || type == KanBattleType::Combined_Day)
				{
					raigekiflag = api_battle.api_hourai_flag[1];
					hougeki2flag = api_battle.api_hourai_flag[2];
					hougeki3flag = api_battle.api_hourai_flag[3];
				}
				if (hougeki1flag)
				{
					bool bCombineDamage = false;
					if (type == KanBattleType::Combined_KouKu || type == KanBattleType::Combined_Day)
					{
						bCombineDamage = bCombined;
					}
					processHouraiDamages(&(api_battle.api_hougeki1), &totalfdamage, &totaledamage, &totalfdamage_combined, bCombineDamage);
				}
				if (hougeki2flag)
				{
					bool bCombineDamage = false;
					processHouraiDamages(&(api_battle.api_hougeki2), &totalfdamage, &totaledamage, &totalfdamage_combined, bCombineDamage);
				}
				if (hougeki3flag)
				{
					bool bCombineDamage = bCombined;
					if (type == KanBattleType::Combined_KouKu || type == KanBattleType::Combined_Day)
					{
						bCombineDamage = false;
					}
					processHouraiDamages(&(api_battle.api_hougeki3), &totalfdamage, &totaledamage, &totalfdamage_combined, bCombineDamage);
				}
				// raigeki
				if (raigekiflag)
				{
					for (int i = 0; i < api_battle.api_raigeki.api_fdam.count(); i++)
					{
						if (bCombined)
						{
							totalfdamage_combined[i] += api_battle.api_raigeki.api_fdam[i];
						}
						else
						{
							totalfdamage[i] += api_battle.api_raigeki.api_fdam[i];
						}
					}
					for (int i = 0; i < api_battle.api_raigeki.api_edam.count(); i++)
					{
						totaledamage[i] += api_battle.api_raigeki.api_edam[i];
					}
				}
			}
		}

		else
		{
			// midnight
			processHouraiDamages(&(api_battle.api_hougeki), &totalfdamage, &totaledamage, &totalfdamage_combined, bCombined);
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

		enemyhps.append(0);
		for (int i = 1; i < api_battle.api_ship_ke.count(); i++)
		{
			int nowhp = api_battle.api_nowhps[i + 6] - (int)totaledamage[i];
			enemyhps.append(nowhp);
		}

		bool bSelfDamaged = false;
		for (auto damage:totalfdamage)
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
		updateInfoTitleBattle(true, &enemyhps, bSelfDamaged);

	}
	return enemyhps;
	
}

void KanDataConnector::processHouraiDamages(const kcsapi_battle_hougeki* api_hougeki, QList<float>* totalfdamage, QList<float>* totaledamage, QList<float>* totalfdamage_combined, bool bOnlyCombined)
{
	// must skip 0!!!
	QList<float>* fdamage = totalfdamage;
	if (bOnlyCombined)
	{
		fdamage = totalfdamage_combined;
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
						(*totaledamage)[defendpos] += api_hougeki->api_damage[j][k];
					}
				}
			}
		}

	}
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
	if (pksd->bCombined && !bClose)
	{
		int combineddeckid = lastdeckid + 1;
		if (combineddeckid >=0 && combineddeckid < 4)
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
	if (pksd->wasLastBossCell)
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
				pksd->totalSouthEastWin++;
			}
		}
		pksd->totalBossReached++;
	}
	pksd->totalKilledYusou += pksd->lastKilledYusou;
	pksd->totalKilledKubou += pksd->lastKilledKubou;
	pksd->totalKilledSensui += pksd->lastKilledSensui;

	pksd->wasLastBossCell = false;
	pksd->lastWonAssumption = false;
	pksd->lastKilledYusou = 0;
	pksd->lastKilledKubou = 0;
	pksd->lastKilledSensui = 0;

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
		|| pksd->lastbattletype == KanBattleType::Combined_DayToNight)
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
	case KanBattleType::Combined_KouKuNight:
		battletypestr = QString::fromLocal8Bit("連航夜");
		break;
	case KanBattleType::Combined_Day:
		battletypestr = QString::fromLocal8Bit("連昼");
		break;
	case KanBattleType::Combined_DayToNight:
		battletypestr = QString::fromLocal8Bit("連昼夜");
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
	}

	// for total info
	updateOverviewTable();

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

	for (int i = 0; i < 5; i++)
	{
		equiplines.append(QString::fromLocal8Bit("\t装備%1\t").arg(i + 1));
	}

	int shipcount = 0;
	foreach(const kcsapi_ship2 * pship, sships)
	{
		shipcount++;
		const kcsapi_mst_ship * pmstship = findMstShipFromShipid(pship->api_ship_id);
		if (pmstship)
		{
			shipnameline += pmstship->api_name + "\t";
		}
		lvline += QString("%1").arg(pship->api_lv) + "\t";

		for (int i = 0; i < pship->api_slot.count(); i++)
		{
			QString strslotitemname = "-";

			if (pship->api_slot[i] > 0)
			{
				const kcsapi_slotitem * pslotitem = findSlotitemFromId(pship->api_slot[i]);
				if (pslotitem)
				{
					const kcsapi_mst_slotitem * pmstslotitem = findMstSlotItemFromSlotitemid(pslotitem->api_slotitem_id);
					if (pmstslotitem)
					{
						strslotitemname = pmstslotitem->api_name;
					}
				}
			}
			/*
			const kcsapi_mst_slotitem * pmstslotitem = findMstSlotItemFromSlotitemid(pship->api_slot[i]);
			foreach(const kcsapi_mst_slotitem &mstslotitem, pksd->start2data.api_mst_slotitem)
			{
				if (mstslotitem.api_id == pship->api_slot[i])
				{
					strslotitemname = mstslotitem.api_name;
					break;
				}
			}
			*/
			equiplines[i] += strslotitemname + "\t";
		}

		beforehpline += QString("%1").arg(pksd->battledata.api_nowhps[shipcount]) + "\t";
		afterhpline += QString("%1").arg(pship->api_nowhp) + "\t";
		if (pmstship)
		{
			nenryoline += QString("%1%").arg(pship->api_fuel * 100 / pmstship->api_fuel_max) + "\t";
			danyakuline += QString("%1%").arg(pship->api_bull * 100 / pmstship->api_bull_max) + "\t";
		}
		condline += QString("%1").arg(pship->api_cond) + "\t";
	}

	int i = 0;
	foreach (const kcsapi_mst_ship * pmstship, eships)
	{
		i++;
		enemynameline += pmstship->api_name + "\t";
		int ehp = pksd->enemyhpdata[i];
		if (ehp < 0)
		{
			ehp = 0;
		}
		ehpline += QString("%1").arg(ehp) + "\t";
	}

	QString writestr = infoline + "\n"
		+ shipnameline + "\n"
		+ lvline + "\n";
	foreach (const QString &str, equiplines)
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

bool KanDataConnector::start2_parse()
{

	pksd->start2data.ReadFromJObj(_jobj);
	// mst table no update

	return true;
}

bool KanDataConnector::port_port_parse()
{
	pksd->portdata.ReadFromJObj(_jobj);
	pksd->shipcountoffset = 0;
	// material, deck, ndock, ship2, basic
	updateOverviewTable();
	updateMissionTable();
	updateFleetTable();
	updateRepairTable();

	updateRepairDockTable();
	updateExpeditionTable();

	pksd->nextdata.api_no = -1;
	updateInfoTitleBattle();
	updateInfoTitleCond();

    static bool firstTime = true;
    if (firstTime)
    {
        updateWeaponTable();
        firstTime = false;
    }

	if (isAutoRepairing())
	{
		MainWindow::mainWindow()->timerWindow()->setAutoRepairTime(true, true);
	}

	return true;
}

bool KanDataConnector::get_member_basic_parse()
{
	pksd->portdata.api_basic.ReadFromJObj(_jobj);
	updateOverviewTable();
	return true;
}

bool KanDataConnector::get_member_ship_parse()
{
	kcsapi_ship api_ship;
	api_ship.ReadFromJObj(_jobj);
	if (api_ship.api_id > 0)
	{
		kcsapi_ship2 * pship = findShipFromShipno(api_ship.api_id);
		if (pship)
		{
			pship->ReadFromShip(api_ship);
			updateFleetTable();
			updateRepairTable();
			updateInfoTitleCond();
		}
	}
	return true;
}

bool KanDataConnector::get_member_ship2_parse()
{
	pksd->portdata.api_ship.clear();
	QJsonArray jarray = _jdoc.object()["api_data"].toArray();
	for (int i = 0; i<jarray.count(); i++)
	{
		kcsapi_ship2 api_ship2item;
		api_ship2item.ReadFromJObj(jarray[i].toObject());
		pksd->portdata.api_ship.append(api_ship2item);
	}

	pksd->shipcountoffset = 0;

	updateOverviewTable();
	updateFleetTable();
	updateRepairTable();
	updateInfoTitleCond();
	return true;
}

bool KanDataConnector::get_member_ship3_parse()
{
	kcsapi_ship3 api_ship3;
	api_ship3.ReadFromJObj(_jobj);

	//        pksd->portdata.api_ship = api_ship3.api_ship_data;
	foreach(const kcsapi_ship2 &v, api_ship3.api_ship_data)
	{
		QList<kcsapi_ship2>::iterator it;
		for (it = pksd->portdata.api_ship.begin(); it != pksd->portdata.api_ship.end(); ++it)
		{
			if (v.api_id == it->api_id)
			{
				(*it) = v;
			}
		}
	}


	pksd->portdata.api_deck_port = api_ship3.api_deck_data;
	updateOverviewTable();
	updateFleetTable();
	updateRepairTable();

	pksd->shipcountoffset = 0;
	updateExpeditionTable();
	updateInfoTitleCond();
	return true;
}

// same as ship3
bool KanDataConnector::get_member_ship_deck_parse()
{
	kcsapi_ship_deck api_ship_deck;
	api_ship_deck.ReadFromJObj(_jobj);

	//        pksd->portdata.api_ship = api_ship_deck.api_ship_data;
	foreach(const kcsapi_ship2 &v, api_ship_deck.api_ship_data)
	{
		QList<kcsapi_ship2>::iterator it;
		for (it = pksd->portdata.api_ship.begin(); it != pksd->portdata.api_ship.end(); ++it)
		{
			if (v.api_id == it->api_id)
			{
				(*it) = v;
			}
		}
	}

	int decksize = pksd->portdata.api_deck_port.count();
	if (api_ship_deck.api_deck_data.size() == decksize)
	{
		for (int i = 0; i < decksize; i++)
		{
			api_ship_deck.api_deck_data[i].api_mission = pksd->portdata.api_deck_port[i].api_mission;
		}
	}
	pksd->portdata.api_deck_port = api_ship_deck.api_deck_data;

	updateOverviewTable();
	updateFleetTable();
	updateRepairTable();

	pksd->shipcountoffset = 0;
	updateExpeditionTable();
	updateInfoTitleCond();
	return true;
}

bool KanDataConnector::get_member_slot_item_parse()
{
	pksd->slotitemdata.clear();
	QJsonArray jarray = _jdoc.object()["api_data"].toArray();
	for (int i = 0; i<jarray.count(); i++)
	{
		kcsapi_slotitem api_slotitem;
		api_slotitem.ReadFromJObj(jarray[i].toObject());
		pksd->slotitemdata.append(api_slotitem);

		if (api_slotitem.api_id > pksd->maxslotitemid)
		{
			pksd->maxslotitemid = api_slotitem.api_id;
		}
	}

	pksd->slotitemcountoffset = 0;

	updateOverviewTable();
	updateWeaponTable();
	//TODO: update tootip?
	return true;
}

bool KanDataConnector::get_member_useitem_parse()
{
	return true;
}

bool KanDataConnector::get_member_deck_parse()
{
	pksd->portdata.api_deck_port.clear();
	QJsonArray jarray = _jdoc.object()["api_data"].toArray();
	for (int i = 0; i<jarray.count(); i++)
	{
		kcsapi_deck api_deck;
		api_deck.ReadFromJObj(jarray[i].toObject());
		pksd->portdata.api_deck_port.append(api_deck);
	}
	updateOverviewTable();
	updateFleetTable();
	updateRepairTable();
	updateInfoTitleCond();
	return true;
}

bool KanDataConnector::get_member_deck_port_parse()
{
	return get_member_deck_parse();
}

bool KanDataConnector::get_member_ndock_parse()
{
	pksd->portdata.api_ndock.clear();
	QJsonArray jarray = _jdoc.object()["api_data"].toArray();
	for (int i = 0; i<jarray.count(); i++)
	{
		kcsapi_ndock api_ndock;
		api_ndock.ReadFromJObj(jarray[i].toObject());
		pksd->portdata.api_ndock.append(api_ndock);
	}

	updateOverviewTable();
	updateFleetTable();
	updateRepairTable();

	updateRepairDockTable();
	return true;
}

bool KanDataConnector::get_member_kdock_parse()
{
	pksd->kdockdata.clear();
	QJsonArray jarray = _jdoc.object()["api_data"].toArray();
	for (int i = 0; i<jarray.count(); i++)
	{
		kcsapi_kdock api_kdock;
		api_kdock.ReadFromJObj(jarray[i].toObject());
		pksd->kdockdata.append(api_kdock);
	}

	updateBuildDockTable();

	if (pksd->createshipdata.isValueSet())
	{
		logBuildResult();
		pksd->createshipdata.clearValue();
	}
	return true;
}

bool KanDataConnector::get_member_material_parse()
{
	pksd->portdata.api_material.clear();
	QJsonArray jarray = _jdoc.object()["api_data"].toArray();
	for (int i = 0; i<jarray.count(); i++)
	{
		kcsapi_material api_material;
		api_material.ReadFromJObj(jarray[i].toObject());
		pksd->portdata.api_material.append(api_material);
	}

	updateOverviewTable();
	return true;
}

bool KanDataConnector::req_hensei_change_parse()
{
	int team = _req.GetItemAsString("api_id").toInt();
	int index = _req.GetItemAsString("api_ship_idx").toInt();
	int shipid = _req.GetItemAsString("api_ship_id").toInt();

	QList<int> * lstship = &(pksd->portdata.api_deck_port[team - 1].api_ship);

	while (lstship->count() < 6)
	{
		lstship->append(-1);
	}

	if (index == -1)
	{
		shipid = lstship->first();
		lstship->clear();
		lstship->append(shipid);
	}
	else if (shipid == -1)
	{
		lstship->removeAt(index);
		while (lstship->count() < 6)
		{
			lstship->append(-1);
		}
	}
	else
	{
		int prev = -1;
//		int previndex = -1;

		bool bDone = false;

		// shipid was in some team (including self)
		int teamcount = pksd->portdata.api_deck_port.count();
		for (int i = 0; i < teamcount; i++)
		{
			QList<int> * lstteamship = &(pksd->portdata.api_deck_port[i].api_ship);
			while (lstteamship->count() < 6)
			{
				lstteamship->append(-1);
			}
			int teamshipcount = lstteamship->count();
			for (int j = 0; j < teamshipcount; j++)
			{
				if (lstteamship->at(j) == shipid)
				{
					// swap with current
					if (lstship->count() >= index + 1)
					{
						prev = lstship->at(index);
						if (prev < 0)
						{
							lstteamship->removeAt(j);
							while (lstteamship->count() < 6)
							{
								lstteamship->append(-1);
							}
							if (i == team-1)
							{
								index--;
							}
						}
						else
						{
							(*lstteamship)[j] = prev;
						}
						if (index >= 0 && index < lstship->count())
						{
							(*lstship)[index] = shipid;
						}
						bDone = true;
						break;
					}
				}
			}
			if (bDone)
			{
				break;
			}
		}

		if (!bDone)
		{
			// shipid was free
			(*lstship)[index] = shipid;

			/*
			if (lstship->count() >= index + 1)
			{
				prev = (*lstship)[index];
				for (int i = 0; i < lstship->count(); i++)
				{
					if ((*lstship)[i] == shipid)
					{
						previndex = i;
						break;
					}
				}
			}

			(*lstship)[index] = shipid;
			if (previndex >= 0)
			{
				(*lstship)[previndex] = prev;
			}
			*/
		}
	}
	updateFleetTable();

	// assume only fleet 1
	if (team == 1)
	{
		bool bAutoRepairing = false;
		// has auto repair slotitem
		if (lstship->size())
		{
			if (isAutoRepairing(lstship->at(0)))
			{
				bAutoRepairing = true;
			}
		}
		MainWindow::mainWindow()->timerWindow()->setAutoRepairTime(bAutoRepairing);
	}
	return true;
}

bool KanDataConnector::req_hensei_lock_parse()
{
	int shipid = _req.GetItemAsString("api_ship_id").toInt();
	kcsapi_hensei_lock api_hensei_lock;
	api_hensei_lock.ReadFromJObj(_jobj);
	kcsapi_ship2 * pship = findShipFromShipno(shipid);
	if (pship)
	{
		pship->api_locked = api_hensei_lock.api_locked;
	}
	return true;
}

bool KanDataConnector::req_hensei_combined_parse()
{
	return true;
}

bool KanDataConnector::get_member_unsetslot_parse()
{
	// slotitem that does not on
//	kcsapi_slot_data api_unsetslot_data;
//	api_unsetslot_data.ReadFromJObj(jobj);
	return true;
}

bool KanDataConnector::req_kaisou_unsetslot_all_parse()
{
	int shipno = _req.GetItemAsString("api_id").toInt();
	auto pship = findShipFromShipno(shipno);
	if (pship)
	{
		for (int i = 0; i < pship->api_slot.size(); i++)
		{
			pship->api_slot[i] = -1;
		}
	}

	updateWeaponTable();
	updateFleetTable();
	return true;
}

bool KanDataConnector::req_kousyou_getship_parse()
{
	int kdock_id = _req.GetItemAsString("api_kdock_id").toInt();
	pksd->kdockdata[kdock_id - 1].api_created_ship_id = 0;

	kcsapi_kdock_getship api_kdock_getship;
	api_kdock_getship.ReadFromJObj(_jobj);
	foreach(const kcsapi_slotitem &v, api_kdock_getship.api_slotitem)
	{
		if (v.api_slotitem_id >= 0)
		{
			AddSlotItem(v);
			//pksd->slotitemcountoffset++;
		}
	}

//	pksd->shipcountoffset++;
	AddShip(api_kdock_getship.api_ship);

	updateOverviewTable();
	updateBuildDockTable();
	updateWeaponTable();
	return true;
}

bool KanDataConnector::req_kousyou_createitem_parse()
{
	kcsapi_createitem api_createitem;
	api_createitem.ReadFromJObj(_jobj);
	if (api_createitem.api_create_flag == 1)
	{
		AddSlotItem(api_createitem.api_slot_item);
		//pksd->slotitemcountoffset++;
	}
	for (int i = 0; i<api_createitem.api_material.count(); i++)
	{
		pksd->portdata.api_material[i].api_value = api_createitem.api_material[i];
	}

	updateOverviewTable();
	updateWeaponTable();

	int usefuel = _req.GetItemAsString("api_item1").toInt();
	int usebull = _req.GetItemAsString("api_item2").toInt();
	int usesteel = _req.GetItemAsString("api_item3").toInt();
	int usebauxite = _req.GetItemAsString("api_item4").toInt();
	//TODO: adjust item?
	logCreateItemResult(api_createitem.api_slot_item.api_slotitem_id, usefuel, usebull, usesteel, usebauxite);

	return true;
}

bool KanDataConnector::req_kousyou_createship_parse()
{
	int usefuel = _req.GetItemAsString("api_item1").toInt();
	int usebull = _req.GetItemAsString("api_item2").toInt();
	int usesteel = _req.GetItemAsString("api_item3").toInt();
	int usebauxite = _req.GetItemAsString("api_item4").toInt();
	int usedev = _req.GetItemAsString("api_item5").toInt();
	int bLarge = _req.GetItemAsString("api_large_flag").toInt();
	int kdockid = _req.GetItemAsString("api_kdock_id").toInt();
	int highspeed = _req.GetItemAsString("api_highspeed").toInt();
	if (bLarge)
	{
		DAPILOG();
	}

	pksd->portdata.api_material[(int)MaterialDataIndex::Fuel].api_value -= usefuel;
	pksd->portdata.api_material[(int)MaterialDataIndex::Bullet].api_value -= usebull;
	pksd->portdata.api_material[(int)MaterialDataIndex::Steel].api_value -= usesteel;
	pksd->portdata.api_material[(int)MaterialDataIndex::Bauxite].api_value -= usebauxite;
	pksd->portdata.api_material[(int)MaterialDataIndex::Development].api_value -= usedev;
	if (highspeed)
	{
		pksd->portdata.api_material[(int)MaterialDataIndex::InstandBuild].api_value -= 1;
		updateOverviewTable();
	}

	pksd->createshipdata.setValue(usefuel, usebull, usesteel, usebauxite, usedev, kdockid);

	return true;
}

bool KanDataConnector::req_kousyou_createship_speedchange_parse()
{
	int dockid = _req.GetItemAsString("api_kdock_id").toInt() - 1;
	if (dockid >= 0)
	{
		pksd->kdockdata[dockid].api_complete_time = 0;
		updateBuildDockTable();
		pksd->portdata.api_material[(int)MaterialDataIndex::InstandBuild].api_value -= 1;
		updateOverviewTable();
	}
	return true;
}

bool KanDataConnector::req_kousyou_destroyship_parse()
{
	int shipno = _req.GetItemAsString("api_ship_id").toInt();
	if (shipno > 0)
	{
		/*
		bool bDone = false;

		QList<kcsapi_deck>::iterator it;
		for (it = pksd->portdata.api_deck_port.begin(); it != pksd->portdata.api_deck_port.end(); ++it)
		{
			for (int i = 0; i<it->api_ship.count(); i++)
			{
				if (it->api_ship[i] == shipno)
				{
					it->api_ship.removeAt(i);
					bDone = true;
					break;
				}
			}
			if (bDone)
			{
				break;
			}
		}

		// slotitem
		const kcsapi_ship2 * pship = findShipFromShipno(shipno);
		if (pship)
		{
			foreach(int slotitemid, pship->api_slot)
			{
				if (slotitemid >= 0)
				{
					pksd->slotitemcountoffset--;
				}
			}
		}
		*/
		// material
		kcsapi_destroyship api_destroyship;
		api_destroyship.ReadFromJObj(_jobj);
		QList<int> api_material = api_destroyship.api_material;
		for (int i = 0; i<api_material.count(); i++)
		{
			pksd->portdata.api_material[i].api_value = api_material[i];
		}

		RemoveShip(shipno);
		updateOverviewTable();
		updateFleetTable();

		updateRepairTable();
		updateWeaponTable();
	}
	return true;
}

bool KanDataConnector::req_kousyou_destroyitem2_parse()
{
	QString idsstr = _req.GetItemAsString("api_slotitem_ids");
	QStringList idslst = idsstr.split("%2C");

    foreach (auto& v , idslst)
	{
		int itemid = v.toInt();
		RemoveSlotItem(itemid);
	}
//	pksd->slotitemcountoffset -= idslst.count();


	kcsapi_destroyitem2 api_destroyitem2;
	api_destroyitem2.ReadFromJObj(_jobj);
	QList<int> api_get_material = api_destroyitem2.api_get_material;
	for (int i = 0; i<api_get_material.count(); i++)
	{
		pksd->portdata.api_material[i].api_value += api_get_material[i];
	}
	updateOverviewTable();
	updateWeaponTable();
	return true;
}

bool KanDataConnector::req_nyukyo_start_parse()
{
	int highspeed = _req.GetItemAsString("api_highspeed").toInt();
	int shipno = _req.GetItemAsString("api_ship_id").toInt();
	if (highspeed && shipno > 0)
	{
		kcsapi_ship2 * pship = findShipFromShipno(shipno);
		if (pship)
		{
			pship->api_nowhp = pship->api_maxhp;
			pship->api_ndock_time = 0;
			updateFleetTable();
			updateRepairTable();
		}
		pksd->portdata.api_material[(int)MaterialDataIndex::InstantRepair].api_value -= 1;
		updateOverviewTable();
	}
	return true;
}

bool KanDataConnector::req_nyukyo_speedchange_parse()
{
	int dockid = _req.GetItemAsString("api_ndock_id").toInt() - 1;
	if (dockid >= 0)
	{
		int shipno = pksd->portdata.api_ndock[dockid].api_ship_id;
		if (shipno > 0)
		{
			pksd->portdata.api_ndock[dockid].api_ship_id = 0;
			kcsapi_ship2 * pship = findShipFromShipno(shipno);
			if (pship)
			{
				pship->api_nowhp = pship->api_maxhp;
				pship->api_ndock_time = 0;
				updateRepairTable();
			}
			updateRepairDockTable();
			pksd->portdata.api_material[(int)MaterialDataIndex::InstantRepair].api_value -= 1;
			updateOverviewTable();
		}
	}
	return true;
}

bool KanDataConnector::req_hokyu_charge_parse()
{
	kcsapi_charge api_charge;
	api_charge.ReadFromJObj(_jobj);

	foreach(const kcsapi_charge_ship &v, api_charge.api_ship)
	{
		QList<kcsapi_ship2>::iterator it;
		for (it = pksd->portdata.api_ship.begin(); it != pksd->portdata.api_ship.end(); ++it)
		{
			if (it->api_id == v.api_id)
			{
				it->api_fuel = v.api_fuel;
				it->api_bull = v.api_bull;
				break;
			}
		}
	}

	for (int i = 0; i<api_charge.api_material.count(); i++)
	{
		pksd->portdata.api_material[i].api_value = api_charge.api_material[i];
	}

	updateFleetTable();
	return true;
}

bool KanDataConnector::req_kaisou_powerup_parse()
{
	QStringList shipsids = _req.GetItemAsString("api_id_items").split("%2C");
	foreach(const QString &v, shipsids)
	{
		int shipno = v.toInt();
		RemoveShip(shipno);
	}

	kcsapi_powerup api_powerup;
	api_powerup.ReadFromJObj(_jobj);
	pksd->portdata.api_deck_port = api_powerup.api_deck;

	updateOverviewTable();
	updateFleetTable();
	return true;
}

bool KanDataConnector::req_kaisou_slotset_parse()
{
	int shipno = _req.GetItemAsString("api_id").toInt();
	int slotindex = _req.GetItemAsString("api_slot_idx").toInt();
	int itemid = _req.GetItemAsString("api_item_id").toInt();

	auto pship = findShipFromShipno(shipno);
	if (pship)
	{
		pship->api_slot[slotindex] = itemid;
		updateWeaponTable();
		updateFleetTable();
		return true;
	}

	return false;
}

bool KanDataConnector::req_kaisou_remodeling_parse()
{
	//TODO
	return true;
}

bool KanDataConnector::req_mission_start_parse()
{
	// expedition
	kcsapi_mission_start api_mission_start;
	api_mission_start.ReadFromJObj(_jobj);

	int missionid = _req.GetItemAsString("api_mission_id").toInt();
	int deckid = _req.GetItemAsString("api_deck_id").toInt();
	if (deckid > 0)
	{
		pksd->portdata.api_deck_port[deckid - 1].api_mission[0] = 1;
		pksd->portdata.api_deck_port[deckid - 1].api_mission[1] = missionid;
		pksd->portdata.api_deck_port[deckid - 1].api_mission[2] = api_mission_start.api_complatetime;
	}
	updateExpeditionTable();
	updateInfoTitleCond();
	return true;
}

bool KanDataConnector::req_mission_result_parse()
{
	return true;
}

bool KanDataConnector::get_member_mission_parse()
{
	return true;
}

bool KanDataConnector::get_member_mapinfo_parse()
{
	return true;
}

bool KanDataConnector::get_member_mapcell_parse()
{
	return true;
}

bool KanDataConnector::req_map_start_parse()
{
	pksd->lastdeckid = _req.GetItemAsString("api_deck_id").toInt() - 1;
	pksd->nextdata.ReadFromJObj(_jobj);

	updateInfoTitleBattle();
	checkWoundQuit();

	pksd->totalSortie++;
	updateOverviewTable();
	// for serial_id
//	DAPILOGDEBUG();
	return true;
}

bool KanDataConnector::req_map_next_parse()
{
	pksd->nextdata.ReadFromJObj(_jobj);

	updateInfoTitleBattle();
	checkWoundQuit();
	return true;
}

bool KanDataConnector::req_sortie_battleresult_parse()
{
	pksd->battleresultdata.ReadFromJObj(_jobj);
	int shipid = pksd->battleresultdata.api_get_ship.api_ship_id;
	if (shipid > 0)
	{
		pksd->shipcountoffset++;
		const kcsapi_mst_ship * pmstship = findMstShipFromShipid(shipid);
		if (pmstship)
		{
			foreach(int slotitemid, pmstship->api_defeq)
			{
				if (slotitemid > 0)
				{
					pksd->slotitemcountoffset++;
				}
			}
		}
	}

	logBattleResult();
	logBattleDetail(false);
	return true;
}

bool KanDataConnector::req_sortie_battle_parse()
{
	pksd->battledata.ReadFromJObj(_jobj);

	pksd->enemyhpdata = updateBattle(pksd->battledata, KanBattleType::Day);
	return true;
}

bool KanDataConnector::req_battle_midnight_battle_parse()
{
	pksd->battledata.ReadFromJObj(_jobj);

	pksd->enemyhpdata = updateBattle(pksd->battledata, KanBattleType::DayToNight);
	return true;
}

bool KanDataConnector::req_battle_midnight_sp_midnight_parse()
{
	pksd->battledata.ReadFromJObj(_jobj);

	pksd->enemyhpdata = updateBattle(pksd->battledata, KanBattleType::Night);
	return true;
}

bool KanDataConnector::req_sortie_night_to_day_parse()
{
	//TODO night to day:
	pksd->battledata.ReadFromJObj(_jobj);

	pksd->enemyhpdata = updateBattle(pksd->battledata, KanBattleType::NightToDay);
	return true;
}

bool KanDataConnector::req_sortie_airbattle_parse()
{
	pksd->battledata.ReadFromJObj(_jobj);

	pksd->enemyhpdata = updateBattle(pksd->battledata, KanBattleType::Air);
	// TODO check air???
	return true;
}

bool KanDataConnector::req_combined_battle_airbattle_parse()
{
	pksd->battledata.ReadFromJObj(_jobj);

	pksd->enemyhpdata = updateBattle(pksd->battledata, KanBattleType::Combined_KouKu);
	return true;
}

bool KanDataConnector::req_combined_battle_battlewater_parse()
{
	pksd->battledata.ReadFromJObj(_jobj);

	pksd->enemyhpdata = updateBattle(pksd->battledata, KanBattleType::Combined_Water);
	return true;
}

bool KanDataConnector::req_combined_battle_battleresult_parse()
{
	pksd->battleresultdata.ReadFromJObj(_jobj);
	int shipid = pksd->battleresultdata.api_get_ship.api_ship_id;
	if (shipid > 0)
	{
		pksd->shipcountoffset++;
		const kcsapi_mst_ship * pmstship = findMstShipFromShipid(shipid);
		if (pmstship)
		{
			foreach(int slotitemid, pmstship->api_defeq)
			{
				if (slotitemid > 0)
				{
					pksd->slotitemcountoffset++;
				}
			}
		}
	}

	logBattleResult();
	logBattleDetail(true);
	return true;
}

bool KanDataConnector::req_combined_battle_battle_parse()
{
	pksd->battledata.ReadFromJObj(_jobj);

	pksd->enemyhpdata = updateBattle(pksd->battledata, KanBattleType::Combined_Day);
	return true;
}

bool KanDataConnector::req_combined_battle_midnight_battle_parse()
{
	pksd->battledata.ReadFromJObj(_jobj);

	pksd->enemyhpdata = updateBattle(pksd->battledata, KanBattleType::Combined_DayToNight);
	return true;
}

bool KanDataConnector::req_combined_battle_sp_midnight_parse()
{
	pksd->battledata.ReadFromJObj(_jobj);

	pksd->enemyhpdata = updateBattle(pksd->battledata, KanBattleType::Combined_Night);
	return true;

}

bool KanDataConnector::req_combined_battle_goback_port_parse()
{
	// TODO: add flags to ship?
	return true;
}

bool KanDataConnector::get_member_practice_parse()
{
	return true;
}

bool KanDataConnector::req_member_get_practice_enemyinfo_parse()
{
	return true;
}

bool KanDataConnector::req_practice_battle_parse()
{
	pksd->battledata.ReadFromJObj(_jobj);

	updateBattle(pksd->battledata, KanBattleType::Day);
	return true;
}

bool KanDataConnector::req_practice_midnight_battle_parse()
{
	pksd->battledata.ReadFromJObj(_jobj);

	updateBattle(pksd->battledata, KanBattleType::Night);
	return true;
}

bool KanDataConnector::req_practice_battle_result_parse()
{
	return true;
}

bool KanDataConnector::get_member_questlist_parse()
{
	// TODO: remove quest
	kcsapi_questlist api_questlist;
	api_questlist.ReadFromJObj(_jobj);

	int questcount = api_questlist.api_count - 5 * (api_questlist.api_disp_page - 1);
	if (questcount > api_questlist.api_list.count())
	{
		questcount = api_questlist.api_list.count();
	}

	if (!questcount)
	{
		pksd->questdata.removeLast();
		// when last page nothing??
	}
	else
	{
		int beginindex = api_questlist.api_list[0].api_no;
		int endindex = api_questlist.api_list[questcount - 1].api_no;
		/*
		if (api_questlist.api_disp_page == 1)
		{
		beginindex = -1;
		}
		else if (api_questlist.api_disp_page == api_questlist.api_page_count)
		{
		endindex = 10000000;
		}
		*/

		//delete all in questdata
		QList<kcsapi_quest>::iterator it;
		for (it = pksd->questdata.begin(); it != pksd->questdata.end();)
		{
			if (it->api_no >= beginindex && it->api_no <= endindex)
			{
				it = pksd->questdata.erase(it);
			}
			else
			{
				++it;
			}
		}

		// add to
		for (int i = 0; i<questcount; i++)
		{
			if (api_questlist.api_list[i].api_state > 1)
			{
				pksd->questdata.append(api_questlist.api_list[i]);
			}
		}
		qSort(pksd->questdata.begin(), pksd->questdata.end(), questDataSort);
	}


	updateMissionTable();
	return true;
}

bool KanDataConnector::req_quest_start_parse()
{
	return true;
}

bool KanDataConnector::req_quest_stop_parse()
{
	int questid = _req.GetItemAsString("api_quest_id").toInt();

	for (QList<kcsapi_quest>::iterator it = pksd->questdata.begin(); it != pksd->questdata.end(); ++it)
	{
		if (it->api_no == questid)
		{
			pksd->questdata.erase(it);
			break;
		}
	}

	updateMissionTable();
	return true;
}

bool KanDataConnector::req_quest_clearitemget_parse()
{
	kcsapi_clearitemget api_clearitemget;
	api_clearitemget.ReadFromJObj(_jobj);
	for (int i = 0; i < api_clearitemget.api_material.count(); i++)
	{
		pksd->portdata.api_material[i].api_value += api_clearitemget.api_material[i];
	}
	if (api_clearitemget.api_bounus_count)
	{
		for (int i = 0; i < api_clearitemget.api_bounus_count; i++)
		{
			auto pbonus = &(api_clearitemget.api_bounus[i]);
			if (pbonus->api_type == 1)
			{
				int index = pbonus->api_item.api_id;
				if (index < 7)
				{
					pksd->portdata.api_material[index].api_value += pbonus->api_count;
				}
			}
			else if (pbonus->api_type == 3)
			{
				// furniture box
			}
			else if (pbonus->api_type == 12)
			{
				// item
				AddSlotItem(-1, pbonus->api_item.api_id);
			}
			else if (pbonus->api_type == 13)
			{
				// mamiya/ presentbox
			}
			else if (pbonus->api_type == 99)
			{
				// ?
			}
			else
			{
				DAPILOG();
			}
			// ship?
		}
	}

	int questid = _req.GetItemAsString("api_quest_id").toInt();

	for (QList<kcsapi_quest>::iterator it = pksd->questdata.begin(); it != pksd->questdata.end(); ++it)
	{
		if (it->api_no == questid)
		{
			pksd->questdata.erase(it);
			break;
		}
	}

	updateMissionTable();
	return true;
}

bool KanDataConnector::get_member_sortie_conditions_parse()
{
	return true;
}

bool KanDataConnector::req_member_updatedeckname_parse()
{
	return true;
}

bool KanDataConnector::req_member_updatecomment_parse()
{
	return true;
}

bool KanDataConnector::req_kousyou_open_new_dock_parse()
{
	return true;
}

bool KanDataConnector::req_nyukyo_open_new_dock_parse()
{
	return true;
}

bool KanDataConnector::auth_member_logincheck_parse()
{
	return true;
}

bool KanDataConnector::get_member_furniture_parse()
{
	return true;
}

bool KanDataConnector::req_furniture_buy_parse()
{
	return true;
}

bool KanDataConnector::req_furniture_change_parse()
{
	return true;
}

bool KanDataConnector::req_member_itemuse_parse()
{
	return true;
}

bool KanDataConnector::req_member_itemuse_cond_parse()
{
	return true;
}

bool KanDataConnector::get_member_record_parse()
{
	return true;
}

bool KanDataConnector::req_ranking_getlist_parse()
{
	return true;
}

bool KanDataConnector::get_member_picture_book_parse()
{
	return true;
}

bool KanDataConnector::get_member_book2_parse()
{
	return true;
}

bool KanDataConnector::req_member_get_incentive_parse()
{
	return true;
}

bool KanDataConnector::get_member_payitem_parse()
{
	return true;
}

bool KanDataConnector::req_kaisou_lock_parse()
{
	int slotitemid = _req.GetItemAsString("api_slotitem_id").toInt();

	kcsapi_kaisou_lock api_kaisou_lock;
	api_kaisou_lock.ReadFromJObj(_jobj);
	auto pslotitem = findSlotitemFromId(slotitemid);
	if (pslotitem)
	{
		pslotitem->api_locked = api_kaisou_lock.api_locked;
		updateWeaponTable();
	}
	return true;
}

bool KanDataConnector::req_mission_return_instruction_parse()
{
	return true;
}

bool KanDataConnector::req_member_payitemuse_parse()
{
	return true;
}

bool KanDataConnector::req_kaisou_marriage_parse()
{
	return true;
}

bool KanDataConnector::req_kousyou_remodel_slotlist_parse()
{
	return true;
}

bool KanDataConnector::req_kousyou_remodel_slotlist_detail_parse()
{
	return true;
}

bool KanDataConnector::req_kousyou_remodel_slot_parse()
{
	int oslotitemid = _req.GetItemAsString("api_slot_id").toInt();

	kcsapi_remodel_slot api_remodel_slot;
	api_remodel_slot.ReadFromJObj(_jobj);
	if (oslotitemid == api_remodel_slot.api_after_slot.api_id)
	{
		auto pslotitem = findSlotitemFromId(oslotitemid);
		pslotitem->ReadFromSlotItem(api_remodel_slot.api_after_slot);
	}
	else
	{
		RemoveSlotItem(oslotitemid);
		AddSlotItem(api_remodel_slot.api_after_slot);
	}
	updateWeaponTable();

	return true;
}

bool KanDataConnector::req_furniture_music_list_parse()
{
	return true;
}

bool KanDataConnector::req_furniture_music_play_parse()
{
	return true;
}

bool KanDataConnector::req_furniture_set_portbgm_parse()
{
	return true;
}

bool KanDataConnector::req_map_select_eventmap_rank_parse()
{
	return true;
}

bool KanDataConnector::dmm_payment_paycheck_parse()
{
	return true;
}

bool KanDataConnector::req_kaisou_open_exslot_parse()
{
	return true;
}

bool KanDataConnector::req_kaisou_slotset_ex_parse()
{
	int slotitemid = _req.GetItemAsString("api_item_id").toInt();
	int shipno = _req.GetItemAsString("api_id").toInt();
	kcsapi_ship2* pship = findShipFromShipno(shipno);
	if (pship)
	{
		pship->api_slot_ex = slotitemid;
		updateWeaponTable();
		updateFleetTable();
	}
	return true;
}
