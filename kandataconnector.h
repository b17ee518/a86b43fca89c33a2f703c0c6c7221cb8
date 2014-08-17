#ifndef KANDATACONNECTOR_H
#define KANDATACONNECTOR_H

#include <QString>
#include <QColor>
#include "kandata.h"
#include "kanreqdata.h"
#include "kansavedata.h"

enum
{
	KANBATTLETYPE_DAY,
	KANBATTLETYPE_NIGHT,
	KANBATTLETYPE_DAYTONIGHT,
	KANBATTLETYPE_NIGHTTODAY,

	KANBATTLETYPE_COMBINED_BEGIN,

	KANBATTLETYPE_COMBINED_KOUKU,
	KANBATTLETYPE_COMBINED_KOUKUNIGHT,
	KANBATTLETYPE_COMBINED_DAY,
	KANBATTLETYPE_COMBINED_NIGHT,
	KANBATTLETYPE_COMBINED_DAYTONIGHT,
};


#define PARSEFLAG_NORMAL 0x00
#define PARSEFLAG_IGNORE 0x01
#define PARSEFLAG_OUTPUT 0x02

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
	bool Parse(QString pathAndQuery, QString requestBody, QString responseBody);

private:
	void updateOverviewTable();
	void updateMissionTable();
	void updateRepairTable();
	void updateFleetTable();
	void updateExpeditionTable();
	void updateRepairDockTable();
	void updateBuildDockTable();
	void updateInfoTitleBattle(bool bBattle=false, QList<int> *enemyhps=0);
	void updateInfoTitleCond();

	QString logBattleResult(bool bWrite=true);
	void logBattleDetail(bool bCombined);
	QString getLogDevLeadStr();
	void logBuildResult();
	void logCreateItemResult(int slotitemid, int fuel, int bull, int steel, int bauxite);
	QString getFormationStr(int formation);

	QList<int> updateBattle(const kcsapi_battle &api_battle, int type);

	void getShipColors(const kcsapi_ship2 *pship, QColor *pcolCond=0, QColor *pcolWound=0, int* pcondstate=0, int* pwoundstate=0);
	void getShipChargeColors(const kcsapi_ship2 *pship, const kcsapi_mst_ship *pmstship, QColor *pcolFuel, QColor *pcolBullet);
	QString getShipWoundStateString(const kcsapi_ship2 *pship);
	bool isShipRepairing(const kcsapi_ship2 *pship);

	void processHouraiDamages(const kcsapi_battle_hougeki* api_hougeki, QList<float>* totalfdamage, QList<float>* totaledamage, QList<float>* totalfdamage_combined, bool bCombined);

	void checkWoundQuit();

	bool RemoveShip(int shipno);

	kcsapi_ship2 *findShipFromShipno(int shipno);
	const kcsapi_mst_ship *findMstShipFromShipid(int shipid) const;
	const Api_Mst_Mission *findMstMissionFromMissionid(int missionid) const;
	const kcsapi_slotitem *findSlotitemFromId(int id) const;
	const kcsapi_mst_slotitem *findMstSlotItemFromSlotitemid(int slotitemid) const;

private:

	bool start2_parse();
	bool port_port_parse();
	bool get_member_basic_parse();
	bool get_member_ship_parse();
	bool get_member_ship2_parse();
	bool get_member_ship3_parse();
	bool get_member_slot_item_parse();
	bool get_member_useitem_parse();
	bool get_member_deck_parse();
	bool get_member_deck_port_parse();
	bool get_member_ndock_parse();
	bool get_member_kdock_parse();
	bool get_member_material_parse();
	bool req_hensei_change_parse();
	bool req_hensei_lock_parse();
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
	bool req_kaisou_remodeling_parse();
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
	bool req_combined_battle_airbattle_parse();
	bool req_combined_battle_battleresult_parse();
	bool req_combined_battle_battle_parse();
	bool req_combined_battle_midnight_battle_parse();
	bool req_combined_battle_sp_midnight_parse();
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
	bool get_member_picture_book_parse();
	bool get_member_book2_parse();
	bool req_member_get_incentive_parse();
	bool get_member_payitem_parse();

private:
	int start2_flag;
	int port_port_flag;
	int get_member_basic_flag;
	int get_member_ship_flag;
	int get_member_ship2_flag;
	int get_member_ship3_flag;
	int get_member_slot_item_flag;
	int get_member_useitem_flag;
	int get_member_deck_flag;
	int get_member_deck_port_flag;
	int get_member_ndock_flag;
	int get_member_kdock_flag;
	int get_member_material_flag;
	int req_hensei_change_flag;
	int req_hensei_lock_flag;
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
	int req_kaisou_remodeling_flag;
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
	int req_combined_battle_airbattle_flag;
	int req_combined_battle_battleresult_flag;
	int req_combined_battle_battle_flag;
	int req_combined_battle_midnight_battle_flag;
	int req_combined_battle_sp_midnight_flag;
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
	int get_member_picture_book_flag;
	int get_member_book2_flag;
	int req_member_get_incentive_flag;
	int get_member_payitem_flag;

private:
	QColor colWhite;
	QColor colGray;
	QColor colOrange;
	QColor colYellow;
	QColor colRed;
	QColor colBlue;

	QString pathAndQuery;
	QString requestBody;
	QString responseBody;
	QJsonDocument jdoc;
	QJsonObject jobj;
	KanReqData req;
	KanSaveData * pksd;
};

#endif // KANDATACONNECTOR_H
