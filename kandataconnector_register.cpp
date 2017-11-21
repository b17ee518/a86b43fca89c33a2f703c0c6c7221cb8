#include "kandataconnector.h"

#include "klog.h"
#include "ControlManager.h"

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
 /kcsapi/api_req_kaisou/slot_exchange_index
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
 /kcsapi/api_req_combined_battle/each_battle
 /kcsapi/api_req_combined_battle/each_battle_water
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
 /kcsapi/api_req_ranking/mxltvkpyuklh
 /kcsapi/api_get_member/picture_book
 /kcsapi/api_get_member/book2
 /kcsapi/api_req_member/get_incentive
 /kcsapi/api_get_member/payitem
 /kcsapi/api_req_map/select_eventmap_rank

 /kcsapi/api_dmm_payment/paycheck

 /kcsapi/api_req_kaisou/open_exslot
 /kcsapi/api_req_kaisou/slotset_ex

 */

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
	_colPurple = QColor(204, 51, 204);

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
	req_hensei_preset_register_flag = PARSEFLAG_NORMAL;
	req_hensei_preset_select_flag = PARSEFLAG_NORMAL;
	req_hensei_preset_delete_flag = PARSEFLAG_NORMAL;
	get_member_preset_deck_flag = PARSEFLAG_NORMAL;
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
	req_kaisou_slot_exchange_index_flag = PARSEFLAG_NORMAL;
	req_kaisou_remodeling_flag = PARSEFLAG_NORMAL;
	req_kaisou_slot_deprive_flag = PARSEFLAG_NORMAL;
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
	req_sortie_ld_airbattle_flag = PARSEFLAG_NORMAL;
	req_combined_battle_airbattle_flag = PARSEFLAG_NORMAL;
	req_combined_battle_ld_airbattle_flag = PARSEFLAG_NORMAL;
	req_combined_battle_battlewater_flag = PARSEFLAG_NORMAL;
	req_combined_battle_battleresult_flag = PARSEFLAG_NORMAL;
	req_combined_battle_battle_flag = PARSEFLAG_NORMAL;
	req_combined_battle_midnight_battle_flag = PARSEFLAG_NORMAL;
	req_combined_battle_sp_midnight_flag = PARSEFLAG_NORMAL;
	req_combined_battle_goback_port_flag = PARSEFLAG_NORMAL;
	req_combined_battle_ec_battle_flag = PARSEFLAG_NORMAL;
	req_combined_battle_ec_midnight_battle_flag = PARSEFLAG_NORMAL;
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
	req_ranking_mxltvkpyuklh_flag = PARSEFLAG_NORMAL;
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
	get_member_base_air_corps_flag = PARSEFLAG_NORMAL;
	req_air_corps_set_action_flag = PARSEFLAG_NORMAL;
	req_map_start_air_base_flag = PARSEFLAG_NORMAL;
	req_air_corps_supply_flag = PARSEFLAG_NORMAL;
	req_air_corps_set_action_flag = PARSEFLAG_NORMAL;
	req_sortie_goback_port_flag = PARSEFLAG_NORMAL;


	/************************************************************************/
	/*                                                                      */
	/************************************************************************/

	//	req_member_itemuse_cond_flag = PARSEFLAG_OUTPUT;
	//	req_sortie_battleresult_flag = PARSEFLAG_OUTPUT;
	//	req_sortie_battle_flag = PARSEFLAG_OUTPUT;
	//	req_battle_midnight_battle_flag = PARSEFLAG_OUTPUT;
	//	req_battle_midnight_sp_midnight_flag = PARSEFLAG_OUTPUT;
	req_sortie_night_to_day_flag = PARSEFLAG_OUTPUT;
	req_combined_battle_ec_night_to_day_flag = PARSEFLAG_OUTPUT;
	//	req_combined_battle_ec_battle_flag = PARSEFLAG_OUTPUT;
	//	req_combined_battle_ec_midnight_battle_flag = PARSEFLAG_OUTPUT;
	//	req_combined_battle_each_battle_flag = PARSEFLAG_OUTPUT;
	//	req_combined_battle_each_battle_water_flag = PARSEFLAG_OUTPUT;
	//	start2_flag = PARSEFLAG_OUTPUT;
	//	port_port_flag = PARSEFLAG_OUTPUT;
	//	req_sortie_airbattle_flag = PARSEFLAG_OUTPUT;
	//	req_combined_battle_airbattle_flag = PARSEFLAG_OUTPUT;
	//	req_combined_battle_battlewater_flag = PARSEFLAG_OUTPUT;
	//	req_combined_battle_battleresult_flag = PARSEFLAG_OUTPUT;
	//	req_combined_battle_battle_flag = PARSEFLAG_OUTPUT;
	//	req_combined_battle_midnight_battle_flag = PARSEFLAG_OUTPUT;
	//	req_combined_battle_sp_midnight_flag = PARSEFLAG_OUTPUT;

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
		if ((api##_flag & PARSEFLAG_IGNORE) == 0)\
										{\
			if (api##_flag & PARSEFLAG_OUTPUT)\
															{\
				DAPILOG();\
															}\
			bRet = api##_parse();\
										}\
					}
#define PARSEAPI(apistr, api) else PARSEAPIF(apistr, api)

	PARSEAPIF("/kcsapi/api_start2", start2)
		PARSEAPI("/kcsapi/api_port/port", port_port)
		PARSEAPI("/kcsapi/api_get_member/basic", get_member_basic)
		PARSEAPI("/kcsapi/api_get_member/require_info", get_member_require_info)
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
		PARSEAPI("/kcsapi/api_req_hensei/preset_register", req_hensei_preset_register)
		PARSEAPI("/kcsapi/api_req_hensei/preset_select", req_hensei_preset_select)
		PARSEAPI("/kcsapi/api_req_hensei/preset_delete", req_hensei_preset_delete)
		PARSEAPI("/kcsapi/api_get_member/preset_deck", get_member_preset_deck)
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
		PARSEAPI("/kcsapi/api_req_kaisou/slot_exchange_index", req_kaisou_slot_exchange_index)
		PARSEAPI("/kcsapi/api_req_kaisou/remodeling", req_kaisou_remodeling)
		PARSEAPI("/kcsapi/api_req_kaisou/slot_deprive", req_kaisou_slot_deprive)
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
		PARSEAPI("/kcsapi/api_req_sortie/ld_airbattle", req_sortie_ld_airbattle)
		PARSEAPI("/kcsapi/api_req_combined_battle/airbattle", req_combined_battle_airbattle)
		PARSEAPI("/kcsapi/api_req_combined_battle/ld_airbattle", req_combined_battle_ld_airbattle)
		PARSEAPI("/kcsapi/api_req_combined_battle/battle_water", req_combined_battle_battlewater)
		PARSEAPI("/kcsapi/api_req_combined_battle/battleresult", req_combined_battle_battleresult)
		PARSEAPI("/kcsapi/api_req_combined_battle/battle", req_combined_battle_battle)
		PARSEAPI("/kcsapi/api_req_combined_battle/midnight_battle", req_combined_battle_midnight_battle)
		PARSEAPI("/kcsapi/api_req_combined_battle/sp_midnight", req_combined_battle_sp_midnight)
		PARSEAPI("/kcsapi/api_req_combined_battle/goback_port", req_combined_battle_goback_port)
		PARSEAPI("/kcsapi/api_req_combined_battle/ec_battle", req_combined_battle_ec_battle)
		PARSEAPI("/kcsapi/api_req_combined_battle/ec_midnight_battle", req_combined_battle_ec_midnight_battle)
		PARSEAPI("/kcsapi/api_req_combined_battle/ec_night_to_day", req_combined_battle_ec_night_to_day)
		PARSEAPI("/kcsapi/api_req_combined_battle/each_battle", req_combined_battle_each_battle)
		PARSEAPI("/kcsapi/api_req_combined_battle/each_battle_water", req_combined_battle_each_battle_water)
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
		PARSEAPI("/kcsapi/api_req_ranking/mxltvkpyuklh", req_ranking_mxltvkpyuklh)
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
		PARSEAPI("/kcsapi/api_get_member/base_air_corps", get_member_base_air_corps)
		PARSEAPI("/kcsapi/api_req_air_corps/set_plane", req_air_corps_set_plane)
		PARSEAPI("/kcsapi/api_req_map/start_air_base", req_map_start_air_base)
		PARSEAPI("/kcsapi/api_req_air_corps/supply", req_air_corps_supply)
		PARSEAPI("/kcsapi/api_req_air_corps/set_action", req_air_corps_set_action)
		PARSEAPI("/kcsapi/api_req_sortie/goback_port", req_sortie_goback_port)

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
