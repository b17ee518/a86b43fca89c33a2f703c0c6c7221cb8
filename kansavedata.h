﻿#ifndef KANSAVEDATA_H
#define KANSAVEDATA_H

#include "kandata.h"
#include "kanreqdata.h"

enum class KanBattleType
{
	Day,
	Night,
	DayToNight,
	NightToDay,

	LDShooting,    // no flag hougeki only

	Air,

	Combined_Begin,

	Combined_KouKu,
	Combined_Water,
	//	Combined_KouKuNight,
	Combined_Day,
	Combined_Night,		// sp_night
	Combined_DayToNight,
	Combined_EC,
	Combined_ECNight,
	Combined_Each,	// 12 vs 12
	Combined_EachWater,	// 12 vs 12
	Combined_ECNightToDay,

	Combined_LDShooting
};

enum class MaterialDataIndex
{
	Fuel = 0,
	Bullet,
	Steel,
	Bauxite,
	InstantBuild,
	InstantRepair,
	Development,
	PowerUp,
};

enum class SlotitemType
{
	SyuHou_S = 1,	//	小口径主砲
	SyuHou_M = 2,	//	中口径主砲
	SyuHou_L = 3,	//	大口径主砲
	FuKuHou = 4,	//	副砲
	GyoRai = 5,	//	魚雷
	KanSen = 6,	//	艦上戦闘機
	KanBaKu = 7,	//	艦上爆撃機
	KanKou = 8,	//	艦上攻撃機
	KanJouTeiSaTsu = 9,	//	艦上偵察機
	SuiTei = 10,	//	水上偵察機
	SuiBaKu = 11,	//	水上爆撃機
	DenTan_S = 12,	//	小型電探
	DenTan_L = 13,	//	大型電探
	Sonar = 14,	//	ソナー
	BaKuRai = 15,	//	爆雷
	SouKa = 16,	//	追加装甲
	KiKan = 17,	//	機関部強化
	TaiKuDan = 18,	//	対空強化弾
	TaiKanDan = 19,	//	対艦強化弾
	VT = 20,	//	VT信管
	TaiKuKiJu = 21,	//	対空機銃
	ToKuSen = 22,	//	特殊潜航艇
	OuKyu = 23,	//	応急修理要員
	JouRiKuTei = 24,	//	上陸用舟艇
	Autogiro = 25,	//	オートジャイロ
	TaiSenKi = 26,	//	対潜哨戒機
	SouKa_M = 27,	//	追加装甲(中型)
	SouKa_L = 28,	//	追加装甲(大型)
	TanSyouTou = 29,	//	探照灯
	YuSou = 30,	//	簡易輸送部材
	KanSyu = 31,	//	艦艇修理施設
	SenSuiKanGyoRai = 32,	//	潜水艦魚雷
	SyouMeiDan = 33,	//	照明弾
	ShiReiBu = 34,	//	司令部施設
	KouKuYouIn = 35,	//	航空要員
	KouSya = 36,	//	高射装置
	TaiChi = 37,	//	対地装備
	SyuHou_L_II = 38,	//	大口径主砲（II）
	SuiJouYouIn = 39,	//	水上艦要員
	Sonar_L = 40,	//	大型ソナー
	HiKouTei_L = 41,	//	大型飛行艇
	TanSyouTou_L = 42,	//	大型探照灯
	SenTouRyouSyoKu = 43,	//	戦闘糧食
	HoKyuBuShi = 44,	//	補給物資
	SuiJouSenTouKi = 45,	// 水上戦闘機
	ToKuGaTaUChiBiTei = 46,	// 特型内火艇
	RiKuJouKouGeKiKi = 47,	// 陸上攻撃機
	KyoKuChiSenTouKi = 48,	// 局地戦闘機
	YuSouKiZai = 50,	// 輸送機材
	SenSuiKanSouBi = 51,	// 潜水艦装備
	FunSen = 56,	//噴式戦闘機
	FunSenBaku = 57,	//噴式戦闘爆撃機
	FunKou = 58,	//噴式攻撃機
	FunTei = 59,	//噴式偵察機	
	DenTan_L_II = 93,	// 大型電探（II）
	KanJouTeiSaTsu_II = 94,	// 艦上偵察機(II)
};

enum class ShipType
{
	KaiBou = 1,	//	海防艦	
	KuChiKu = 2,	//	駆逐艦	
	KeiJun = 3,	//	軽巡洋艦	
	RaiJun = 4,	//	重雷装巡洋艦	
	JuJun = 5,	//	重巡洋艦	
	KouJun = 6,	//	航空巡洋艦	
	KeiKuBou = 7,	//	軽空母	
	KouSoKuSenKan = 8,	//	戦艦	
	TeiSoKuSenKan = 9,	//	戦艦	
	KouSen = 10,	//	航空戦艦	
	KuBou = 11,	//	正規空母	
	DoKyuSenKan = 12,	//	超弩級戦艦	
	SenSui = 13,	//	潜水艦	
	SenBou = 14,	//	潜水空母	
	HoKyu = 15,	//	補給艦	
	SuiBou = 16,	//	水上機母艦	
	YouRiKu = 17,	//	揚陸艦	
	SouKaKuBou = 18,	//	装甲空母	
	KouSaKu = 19,	//	工作艦	
	SenSuiBoKan = 20,	//	潜水母艦	
	RenJun = 21,	//	練習巡洋艦	
	HoKyu_HaYaSuI = 22,	//	補給艦	速吸
};

enum SeiKuState
{
	KaKuHou = 1,
	YuSei = 2,
	KoRe = 3,
	SouShiTsu = 4,
};

class CreateShipSaveData
{
public:
	void setValue(int fuel, int bull, int steel, int bauxite, int dev, int kdock);
	void clearValue();
	inline bool isValueSet() { return _flag > 0; }
	inline bool isAll30() {
		return _usefuel == 30
			&& _usebull == 30
			&& _usesteel == 30
			&& _usebauxite == 30;
	}

	int _usefuel;
	int _usebull;
	int _usesteel;
	int _usebauxite;
	int _usedevelopment;
	int _kdockid;
	int _flag;
};

class ShipWithSlotItemsFullData
{
public:
	const kcsapi_ship2* pship;
	const kcsapi_mst_ship* pmstship;
	QList<const kcsapi_slotitem*> slotitemlist;
	QList<const kcsapi_mst_slotitem*> mstslotitemlist;

public:
	void InitWithShipAndSlots(int shipno);
};

class KanSaveData
{
public:

	struct DeckSaveData
	{
		int totalLevel = 0;
		int totalMinTaiku = 0;
		int totalMaxTaiku = 0;
		double totalSaku = 0.0;
	};

	// public member without _

	kcsapi_port portdata;
	//    QList<kcsapi_ship> shipdata;
	kcsapi_start2 start2data;
	QList<kcsapi_slotitem> slotitemdata;

	QList<kcsapi_kdock> kdockdata;
    QList<kcsapi_quest> fullQuestData;
    QList<kcsapi_quest> activeQuestData;
	kcsapi_next nextdata;
	kcsapi_battle battledata;
	kcsapi_battleresult battleresultdata;

	kcsapi_practicelist practicelistdata;

	CreateShipSaveData createshipdata;

	QMap<int, QList<kcsapi_air_base_corps> > airbasedata;
	QMap<int, kcsapi_map_info> mapinfodata;
	bool airBaseNeedSupply = false;
	bool airBaseBadCond = false;
	QList<int> airBaseNeedSupplyList;

	QList<int> enemyhpdata;

	QList<DeckSaveData> deckSaveData;

	kcsapi_questlist lastQuestList;

	int shipcountoffset = 0;
	int slotitemcountoffset = 0;
	KanBattleType lastbattletype = KanBattleType::Day;
	int lastdeckid = 0;
	int maxslotitemid = 0;
	bool bCombinedSelf = false;
	bool bCombinedEnemy = false;

	int lastSeiku = 0;
	int lastSFormation = -1;
	int lastEFormation = -1;
	int lastIntercept = -1;
	int lastBattleFuel = -1;

	bool lastWonAssumption = false;
	bool lastFlagshipKilled = false;
	// all from index 1
	struct LastBattleHpSaveData
	{
		QList<int> self;
		QList<int> combinedSelf;
		QList<int> enemy;
		QList<int> combinedEnemy;
	};

	LastBattleHpSaveData beginLastBattleHPs;
	LastBattleHpSaveData remainLastBattleHPs;

	int lastKilledYusou = 0;
	int lastKilledKubou = 0;
	int lastKilledSensui = 0;
	bool wasLastBossCell = false;

	int totalKilledYusou = 0;
	int totalKilledKubou = 0;
	int totalKilledSensui = 0;
	int totalBossWin = 0;
	int totalBossReached = 0;
	int totalBossSRank = 0;
	int totalSortie = 0;
	int totalSouthEastWin = 0;
	int totalTokyuWin = 0;
	int totalAnyCount = 0;

	static const int maxSingleTeamSize = 7;
	static const int maxCombinedTeamSize = 6;

public:
	void logAllExport();

	void resetTotals();
	void adjustAnyAndSouthEast(int addval);

	void clearQuestByType(int type, int beginIndex, int endIndex, int page = -1);

	void requireRecordQuestList();
	void recordLastQuestList(const kcsapi_questlist& questList);

	QList<ShipWithSlotItemsFullData> createTeamFullData(int team);

	bool isRecordingLastQuestList = false;

private:
	KanSaveData(void);
public:
	static KanSaveData& getInstance(void) {
		static KanSaveData singleton;
		return singleton;
	}
};

#endif // KANSAVEDATA_H
