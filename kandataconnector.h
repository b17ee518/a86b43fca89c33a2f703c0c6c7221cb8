#ifndef KANDATACONNECTOR_H
#define KANDATACONNECTOR_H

#include <QString>
#include <QColor>
#include "kandata.h"
#include "kanreqdata.h"
#include "kansavedata.h"
#include "kandatacalc.h"

#define PARSEFLAG_NORMAL 0x00
#define PARSEFLAG_IGNORE 0x01
#define PARSEFLAG_OUTPUT 0x02

#define DAPILOG() APILOG(_pathAndQuery, _requestBody, _responseBody)
#define DAPILOGDEBUG() APILOGDEBUG(_pathAndQuery, _requestBody, _responseBody)

class KanDataConnector
{
private:
	KanDataConnector(void);
public:
	static KanDataConnector& getInstance(void) {
		static KanDataConnector singleton;
		return singleton;
	}

public:
	bool Parse(const QString& pathAndQuery, const QString& requestBody, const QString& responseBody);

	kcsapi_ship2 *findShipFromShipno(int shipno);
	const kcsapi_mst_ship *findMstShipFromShipid(int shipid) const;
	const QString findMstShipTypeNameFromSType(int stype) const;
	const Api_Mst_Mission *findMstMissionFromMissionid(int missionid) const;
	kcsapi_slotitem *findSlotitemFromId(int id);
	const kcsapi_mst_slotitem *findMstSlotItemFromSlotitemid(int slotitemid) const;
	bool isShipHasSlotitem(const kcsapi_ship2* pship, SlotitemType sitype, int count = 1, int* hasCount = NULL);
	bool isShipHasSlotitem(int shipno, SlotitemType sitype, int count = 1, int* hasCount = NULL)
	{
		if (count <= 0)
		{
			return true;
		}
		return isShipHasSlotitem(findShipFromShipno(shipno), sitype, count, hasCount);
	}

	void setOutputAllLog(bool bVal) { _outputAllLog = bVal; }

	void callUpdateOverviewTable() { updateOverviewTable(); }

private:
	void updateOverviewTable();
	void updateMissionTable();
	void updateRepairTable();
	void updateFleetTable();
	void updateExpeditionTable();
	void updateRepairDockTable();
	void updateBuildDockTable();
	void updateInfoTitleBattle(bool bBattle = false, bool bSelfDamaged = false);
	void updateInfoTitleCond();
	void updateWeaponTable();

	QString logBattleResult(bool bWrite = true);
	void logBattleDetail(bool bCombined);
	QString getLogDevLeadStr();
	void logBuildResult();
	void logCreateItemResult(int slotitemid, int fuel, int bull, int steel, int bauxite);
	QString getFormationStr(int formation);

	void updateBattle(const kcsapi_battle &api_battle, KanBattleType type);

	void processHougekiDamages(const kcsapi_battle_hougeki& api_hougeki
		, bool bOnlyCombinedSelf
		, bool bOnlyCombinedEnemy
		, bool bIsFriendly = false);
	void processAirBaseAttackDamages(const kcsapi_battle_base_attack& api_air_base_attack);
	void processKoukuDamages(const kcsapi_battle_kouku& api_kouku);
	void processSupportDamages(int api_support_flag, const kcsapi_battle_support_info& api_support);
	void processOpeningDamages(const kcsapi_battle_opening_atack& api_opening_atack);
	void processRaigekiDamages(const kcsapi_battle_raigeki& api_raigeki);

	void onGetPracticeListData(const kcsapi_practicelist& api_practicelist);

	void getShipColors(const kcsapi_ship2 *pship, QColor *pcolCond = 0, QColor *pcolWound = 0, CondState* pcondstate = 0, WoundState* pwoundstate = 0);
	void getShipChargeColors(const kcsapi_ship2 *pship, const kcsapi_mst_ship *pmstship, QColor *pcolFuel, QColor *pcolBullet);
	QString getShipWoundStateString(const kcsapi_ship2 *pship);
	bool isShipRepairing(const kcsapi_ship2 *pship);
	bool isAutoRepairing(int flagshipno = -1);

private:
	QList<float> totalFDamage;
	QList<float> totalEDamage;
	QList<float> totalFDamageCombined;
	QList<float> totalEDamageCombined;
	bool bCombinedSelf;
	bool bCombinedEnemy;
private:

	void checkWoundQuit();

    bool RemoveShip(int shipno, bool withEquip=true);
	bool RemoveSlotItem(int id);
	bool AddShip(const kcsapi_ship2& ship);
	bool AddSlotItem(const kcsapi_slotitem& item);
	bool AddShip(int shipid, const QList<int>& slotitems);
	bool AddSlotItem(int id, int slotitemId);

	void checkAirBase();

private:

	bool start2_parse();
	bool port_port_parse();
	bool get_member_basic_parse();
	bool get_member_require_info_parse();
	bool get_member_ship_parse();
	bool get_member_ship2_parse();
	bool get_member_ship3_parse();
	bool get_member_ship_deck_parse();
	bool get_member_slot_item_parse();
	bool get_member_useitem_parse();
	bool get_member_deck_parse();
	bool get_member_deck_port_parse();
	bool get_member_ndock_parse();
	bool get_member_kdock_parse();
	bool get_member_material_parse();
	bool req_hensei_change_parse();
	bool req_hensei_lock_parse();
	bool req_hensei_preset_register_parse();
	bool req_hensei_preset_select_parse();
	bool req_hensei_preset_delete_parse();
	bool get_member_preset_deck_parse();
	bool req_hensei_combined_parse();
	bool get_member_unsetslot_parse();
	bool req_kaisou_unsetslot_all_parse();
	bool req_kousyou_getship_parse();
	bool req_kousyou_createitem_parse();
	bool req_kousyou_createship_parse();
	bool req_kousyou_createship_speedchange_parse();
	bool req_kousyou_destroyship_parse();
	bool req_kousyou_destroyitem2_parse();
	bool req_nyukyo_start_parse();
	bool req_nyukyo_speedchange_parse();
	bool req_hokyu_charge_parse();
	bool req_kaisou_powerup_parse();
	bool req_kaisou_slotset_parse();
	bool req_kaisou_slot_exchange_index_parse();
	bool req_kaisou_remodeling_parse();
	bool req_kaisou_slot_deprive_parse();
	bool req_mission_start_parse();
	bool req_mission_result_parse();
	bool get_member_mission_parse();
	bool get_member_mapinfo_parse();
	bool get_member_mapcell_parse();
	bool req_map_start_parse();
	bool req_map_next_parse();
	bool req_sortie_battleresult_parse();
	bool req_sortie_battle_parse();
	bool req_battle_midnight_battle_parse();
	bool req_battle_midnight_sp_midnight_parse();
	bool req_sortie_night_to_day_parse();
	bool req_sortie_airbattle_parse();
	bool req_sortie_ld_airbattle_parse();
	bool req_sortie_ld_shooting_parse();
	bool req_combined_battle_airbattle_parse();
	bool req_combined_battle_ld_airbattle_parse();
	bool req_combined_battle_ld_shooting_parse();
	bool req_combined_battle_battlewater_parse();
	bool req_combined_battle_battleresult_parse();
	bool req_combined_battle_battle_parse();
	bool req_combined_battle_midnight_battle_parse();
	bool req_combined_battle_sp_midnight_parse();
	bool req_combined_battle_goback_port_parse();
	bool req_combined_battle_ec_battle_parse();
	bool req_combined_battle_ec_midnight_battle_parse();
	bool req_combined_battle_ec_night_to_day_parse();
	bool req_combined_battle_each_battle_parse();
	bool req_combined_battle_each_battle_water_parse();
	bool get_member_practice_parse();
	bool req_member_get_practice_enemyinfo_parse();
	bool req_practice_battle_parse();
	bool req_practice_midnight_battle_parse();
	bool req_practice_battle_result_parse();
	bool get_member_questlist_parse();
	bool req_quest_start_parse();
	bool req_quest_stop_parse();
	bool req_quest_clearitemget_parse();
	bool get_member_sortie_conditions_parse();
	bool req_member_updatedeckname_parse();
	bool req_member_updatecomment_parse();
	bool req_kousyou_open_new_dock_parse();
	bool req_nyukyo_open_new_dock_parse();
	bool auth_member_logincheck_parse();
	bool get_member_furniture_parse();
	bool req_furniture_buy_parse();
	bool req_furniture_change_parse();
	bool req_member_itemuse_parse();
	bool req_member_itemuse_cond_parse();
	bool get_member_record_parse();
	bool req_ranking_getlist_parse();
	bool req_ranking_mxltvkpyuklh_parse();
	bool get_member_picture_book_parse();
	bool get_member_book2_parse();
	bool req_member_get_incentive_parse();
	bool req_member_set_oss_condition_parse();
	bool get_member_payitem_parse();
	bool req_kaisou_lock_parse();			// lock slot item
	bool req_mission_return_instruction_parse();	// force mission return
	bool req_member_payitemuse_parse();
	bool req_kaisou_marriage_parse();
	bool req_kousyou_remodel_slotlist_parse();
	bool req_kousyou_remodel_slotlist_detail_parse();
	bool req_kousyou_remodel_slot_parse();		// kaisyu
	bool req_furniture_music_list_parse();
	bool req_furniture_music_play_parse();
	bool req_furniture_set_portbgm_parse();
	bool req_map_select_eventmap_rank_parse();
	bool dmm_payment_paycheck_parse();
	bool req_kaisou_open_exslot_parse();
	bool req_kaisou_slotset_ex_parse();
	bool get_member_base_air_corps_parse();	// air base list
	bool req_air_corps_set_plane_parse();
	bool req_map_start_air_base_parse();
	bool req_air_corps_supply_parse();
	bool req_air_corps_set_action_parse();
	bool req_sortie_goback_port_parse();
	bool req_member_set_flagship_position_parse();
    bool req_map_anchorage_repair_parse();


private:
	int start2_flag;
	int port_port_flag;
	int get_member_basic_flag;
	int get_member_require_info_flag;
	int get_member_ship_flag;
	int get_member_ship2_flag;
	int get_member_ship3_flag;
	int get_member_ship_deck_flag;
	int get_member_slot_item_flag;
	int get_member_useitem_flag;
	int get_member_deck_flag;
	int get_member_deck_port_flag;
	int get_member_ndock_flag;
	int get_member_kdock_flag;
	int get_member_material_flag;
	int req_hensei_change_flag;
	int req_hensei_lock_flag;
	int req_hensei_preset_register_flag;
	int req_hensei_preset_select_flag;
	int req_hensei_preset_delete_flag;
	int get_member_preset_deck_flag;
	int req_hensei_combined_flag;
	int get_member_unsetslot_flag;
	int req_kaisou_unsetslot_all_flag;
	int req_kousyou_getship_flag;
	int req_kousyou_createitem_flag;
	int req_kousyou_createship_flag;
	int req_kousyou_createship_speedchange_flag;
	int req_kousyou_destroyship_flag;
	int req_kousyou_destroyitem2_flag;
	int req_nyukyo_start_flag;
	int req_nyukyo_speedchange_flag;
	int req_hokyu_charge_flag;
	int req_kaisou_powerup_flag;
	int req_kaisou_slotset_flag;
	int req_kaisou_slot_exchange_index_flag;
	int req_kaisou_remodeling_flag;
	int req_kaisou_slot_deprive_flag;
	int req_mission_start_flag;
	int req_mission_result_flag;
	int get_member_mission_flag;
	int get_member_mapinfo_flag;
	int get_member_mapcell_flag;
	int req_map_start_flag;
	int req_map_next_flag;
	int req_sortie_battleresult_flag;
	int req_sortie_battle_flag;
	int req_battle_midnight_battle_flag;
	int req_battle_midnight_sp_midnight_flag;
	int req_sortie_night_to_day_flag;
	int req_sortie_airbattle_flag;
	int req_sortie_ld_airbattle_flag;
	int req_sortie_ld_shooting_flag;
	int req_combined_battle_airbattle_flag;
	int req_combined_battle_ld_airbattle_flag;
	int req_combined_battle_ld_shooting_flag;
	int req_combined_battle_battlewater_flag;
	int req_combined_battle_battleresult_flag;
	int req_combined_battle_battle_flag;
	int req_combined_battle_midnight_battle_flag;
	int req_combined_battle_sp_midnight_flag;
	int req_combined_battle_goback_port_flag;
	int req_combined_battle_ec_battle_flag;
	int req_combined_battle_ec_midnight_battle_flag;
	int req_combined_battle_ec_night_to_day_flag;
	int req_combined_battle_each_battle_flag;
	int req_combined_battle_each_battle_water_flag;
	int get_member_practice_flag;
	int req_member_get_practice_enemyinfo_flag;
	int req_practice_battle_flag;
	int req_practice_midnight_battle_flag;
	int req_practice_battle_result_flag;
	int get_member_questlist_flag;
	int req_quest_start_flag;
	int req_quest_stop_flag;
	int req_quest_clearitemget_flag;
	int get_member_sortie_conditions_flag;
	int req_member_updatedeckname_flag;
	int req_member_updatecomment_flag;
	int req_kousyou_open_new_dock_flag;
	int req_nyukyo_open_new_dock_flag;
	int auth_member_logincheck_flag;
	int get_member_furniture_flag;
	int req_furniture_buy_flag;
	int req_furniture_change_flag;
	int req_member_itemuse_flag;
	int req_member_itemuse_cond_flag;
	int get_member_record_flag;
	int req_ranking_getlist_flag;
	int req_ranking_mxltvkpyuklh_flag;	// getlist
	int get_member_picture_book_flag;
	int get_member_book2_flag;
	int req_member_get_incentive_flag;
	int req_member_set_oss_condition_flag;
	int get_member_payitem_flag;
	int req_kaisou_lock_flag;			// lock slot item
	int req_mission_return_instruction_flag;	// force mission return
	int req_member_payitemuse_flag;
	int req_kaisou_marriage_flag;
	int req_kousyou_remodel_slotlist_flag;
	int req_kousyou_remodel_slotlist_detail_flag;
	int req_kousyou_remodel_slot_flag;		// kaisyu
	int req_furniture_music_list_flag;
	int req_furniture_music_play_flag;
	int req_furniture_set_portbgm_flag;
	int req_map_select_eventmap_rank_flag;
	int dmm_payment_paycheck_flag;
	int req_kaisou_open_exslot_flag;
	int req_kaisou_slotset_ex_flag;
	int get_member_base_air_corps_flag;	// air base list
	int req_air_corps_set_plane_flag;
	int req_map_start_air_base_flag;
	int req_air_corps_supply_flag;
	int req_air_corps_set_action_flag;
	int req_sortie_goback_port_flag;
	int req_member_set_flagship_position_flag;
    int req_map_anchorage_repair_flag;

private:
	QColor _colWhite;
	QColor _colGray;
	QColor _colOrange;
	QColor _colYellow;
	QColor _colRed;
	QColor _colBlue;
	QColor _colAqua;
	QColor _colPurple;

	QString _pathAndQuery;
	QString _requestBody;
	QString _responseBody;
	QJsonDocument _jdoc;
	QJsonObject _jobj;
	KanReqData _req;
	KanSaveData * pksd = NULL;

	bool _outputAllLog = false;
};

#endif // KANDATACONNECTOR_H
