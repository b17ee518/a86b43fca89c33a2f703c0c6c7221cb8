#ifndef KANSAVEDATA_H
#define KANSAVEDATA_H

#include "kandata.h"
#include "kanreqdata.h"

enum
{
	MATERIALDATAINDEX_FUEL = 0,
	MATERIALDATAINDEX_BULLET,
	MATERIALDATAINDEX_STEEL,
	MATERIALDATAINDEX_BAUXITE,
	MATERIALDATAINDEX_INSTANTBUILD,
	MATERIALDATAINDEX_INSTANTREPAIR,
	MATERIALDATAINDEX_DEVELOPMENT,
	MATERIALDATAINDEX_POWERUP,
};

enum 
{
	SLOTITEMTYPE_SYUHOU_S = 1,	//	小口径主砲
	SLOTITEMTYPE_SYUHOU_M = 2,	//	中口径主砲
	SLOTITEMTYPE_SYUHOU_L = 3,	//	大口径主砲
	SLOTITEMTYPE_FUKUHOU = 4,	//	副砲
	SLOTITEMTYPE_GYORAI = 5,	//	魚雷
	SLOTITEMTYPE_KANSEN = 6,	//	艦上戦闘機
	SLOTITEMTYPE_KANBAKU = 7,	//	艦上爆撃機
	SLOTITEMTYPE_KANKOU = 8,	//	艦上攻撃機
	SLOTITEMTYPE_TEISATSU = 9,	//	艦上偵察機
	SLOTITEMTYPE_SUITEI = 10,	//	水上偵察機
	SLOTITEMTYPE_SUIBAKU = 11,	//	水上爆撃機
	SLOTITEMTYPE_DENTAN_S = 12,	//	小型電探
	SLOTITEMTYPE_DENTAN_L = 13,	//	大型電探
	SLOTITEMTYPE_SONAR = 14,	//	ソナー
	SLOTITEMTYPE_BAKURAI = 15,	//	爆雷
	SLOTITEMTYPE_SOUKA = 16,	//	追加装甲
	SLOTITEMTYPE_KIKAN = 17,	//	機関部強化
	SLOTITEMTYPE_TAIKUTAN = 18,	//	対空強化弾
	SLOTITEMTYPE_TAIKANTAN = 19,	//	対艦強化弾
	SLOTITEMTYPE_VT = 20,	//	VT信管
	SLOTITEMTYPE_TAIKUKIJU = 21,	//	対空機銃
	SLOTITEMTYPE_TOKUSEN = 22,	//	特殊潜航艇
	SLOTITEMTYPE_OUKYU = 23,	//	応急修理要員
	SLOTITEMTYPE_JOURIKUTEI = 24,	//	上陸用舟艇
	SLOTITEMTYPE_OTOJAIRO = 25,	//	オートジャイロ
	SLOTITEMTYPE_TAISENKI = 26,	//	対潜哨戒機
	SLOTITEMTYPE_SOUKA_M = 27,	//	追加装甲(中型)
	SLOTITEMTYPE_SOUKA_L = 28,	//	追加装甲(大型)
	SLOTITEMTYPE_TANSYOUTOU = 29,	//	探照灯
	SLOTITEMTYPE_YUSOU = 30,	//	簡易輸送部材
	SLOTITEMTYPE_KANSYU = 31,	//	艦艇修理施設
	SLOTITEMTYPE_SENSUIKANGYORAI = 32,	//	潜水艦魚雷
	SLOTITEMTYPE_SYOUMEITAN = 33,	//	照明弾
	SLOTITEMTYPE_SIREIBU = 34,	//	司令部施設
	SLOTITEMTYPE_KOUKU_YOUIN = 35,	//	航空要員
	SLOTITEMTYPE_KOUSYA = 36,	//	高射装置
	SLOTITEMTYPE_TAICHI = 37,	//	対地装備
	SLOTITEMTYPE_SYUHOU_L_II = 38,	//	大口径主砲（II）
	SLOTITEMTYPE_SUIJOU_YOUIN = 39,	//	水上艦要員
	SLOTITEMTYPE_SONAR_L = 40,	//	大型ソナー
	SLOTITEMTYPE_HIKOUTEI_L = 41,	//	大型飛行艇
	SLOTITEMTYPE_TANSYOUTOU_L = 42,	//	大型探照灯
	SLOTITEMTYPE_SENTOURYOUSYOKU = 43,	//	戦闘糧食
	SLOTITEMTYPE_HOKYUBUSHI = 44,	//	補給物資
	SLOTITEMTYPE_DENTAN_L_II = 93,	//	大型電探（II）
};

enum
{
	SHIPTYPE_KAIBOU = 1,	//	海防艦	
	SHIPTYPE_KUCHIKU = 2,	//	駆逐艦	
	SHIPTYPE_KEIJUN = 3,	//	軽巡洋艦	
	SHIPTYPE_RAIJUN = 4,	//	重雷装巡洋艦	
	SHIPTYPE_JUJUN = 5,	//	重巡洋艦	
	SHIPTYPE_KOUJUN = 6,	//	航空巡洋艦	
	SHIPTYPE_KEIKUBO = 7,	//	軽空母	
	SHIPTYPE_KOUSOKUSENKAN = 8,	//	戦艦	
	SHIPTYPE_TEISOKUSENKAN = 9,	//	戦艦	
	SHIPTYPE_KOUSEN = 10,	//	航空戦艦	
	SHIPTYPE_KUBO = 11,	//	正規空母	
	SHIPTYPE_DOSENKAN = 12,	//	超弩級戦艦	
	SHIPTYPE_SENSUI = 13,	//	潜水艦	
	SHIPTYPE_SENBO = 14,	//	潜水空母	
	SHIPTYPE_HOKYU = 15,	//	補給艦	
	SHIPTYPE_SUIBO = 16,	//	水上機母艦	
	SHIPTYPE_YOURIKU = 17,	//	揚陸艦	
	SHIPTYPE_SOUKAKUBO = 18,	//	装甲空母	
	SHIPTYPE_KOUSAKU = 19,	//	工作艦	
	SHIPTYPE_SENSUIBOKAN = 20,	//	潜水母艦	
	SHIPTYPE_RENJUN = 21,	//	練習巡洋艦	
	SHIPTYPE_HOKYU_HAYASUI = 22,	//	補給艦	速吸
};

enum
{
	SEIKU_KAKUHO = 1,
	SEIKU_YUSEI = 2,
	SEIKU_KORE = 3,
	SEIKU_SOUSITSU = 4,
};

class CreateShipSaveData
{
public:
	void setValue(int fuel, int bull, int steel, int bauxite, int dev, int kdock);
	void clearValue();
	bool isValueSet(){ return flag > 0; }
	bool isAll30(){ return usefuel == 30 && usebull == 30 && usesteel == 30 && usebauxite == 30; }

	int usefuel;
	int usebull;
	int usesteel;
	int usebauxite;
	int usedevelopment;
	int kdockid;
	int flag;
};

class KanSaveData
{
public:

	kcsapi_port portdata;
//    QList<kcsapi_ship> shipdata;
	kcsapi_start2 start2data;
	QList<kcsapi_slotitem> slotitemdata;

	QList<kcsapi_kdock> kdockdata;
	QList<kcsapi_quest> questdata;
	kcsapi_next nextdata;
	kcsapi_battle battledata;
	kcsapi_battleresult battleresultdata;

	CreateShipSaveData createshipdata;

	QList<int> enemyhpdata;

	int shipcountoffset;
	int slotitemcountoffset;
	int lastbattletype;
	int lastdeckid = 0;
	int maxslotitemid;
	bool bCombined = false;

	int lastSeiku = 0;
	int lastSFormation = -1;
	int lastEFormation = -1;
	int lastIntercept = -1;

public:
	void logAllExport();

private:
	KanSaveData(void);
public:
	static KanSaveData& getInstance(void) {
		static KanSaveData singleton;
		return singleton;
	}
};

#endif // KANSAVEDATA_H
