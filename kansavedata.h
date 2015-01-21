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
};

enum 
{
	SLOTITEMTYPE_SYUHOU_S = 1,
	SLOTITEMTYPE_SYUHOU_M = 2,
	SLOTITEMTYPE_SYUHOU_L = 3,
	SLOTITEMTYPE_FUKUHOU = 4,
	SLOTITEMTYPE_GYORAI = 5,
	SLOTITEMTYPE_KANSEN = 6,
	SLOTITEMTYPE_KANBAKU = 7,
	SLOTITEMTYPE_KANKOU = 8,
	SLOTITEMTYPE_TEISATSU = 9,
	SLOTITEMTYPE_SUITEI = 10,
	SLOTITEMTYPE_SUIBAKU = 11,
	SLOTITEMTYPE_DENTAN_S = 12,
	SLOTITEMTYPE_DENTAN_L = 13,
	SLOTITEMTYPE_SONAR = 14,
	SLOTITEMTYPE_BAKURAI = 15,
	SLOTITEMTYPE_SOUKA = 16,
	SLOTITEMTYPE_KIKAN = 17,
	SLOTITEMTYPE_TAIKUTAN = 18,
	SLOTITEMTYPE_TAIKANTAN = 19,
	SLOTITEMTYPE_VT = 20,
	SLOTITEMTYPE_TAIKUKIJU = 21,
	SLOTITEMTYPE_TOKUSEN = 22,
	SLOTITEMTYPE_OUKYU = 23,
	SLOTITEMTYPE_JOURIKUTEI = 24,
	SLOTITEMTYPE_OTOJAIRO = 25,
	SLOTITEMTYPE_TAISENKI = 26,
	SLOTITEMTYPE_SOUKA_M = 27,
	SLOTITEMTYPE_SOUKA_L = 28,
	SLOTITEMTYPE_TANSYOUTOU = 29,
	SLOTITEMTYPE_YUSOU = 30,
	SLOTITEMTYPE_KANSYU = 31,
	SLOTITEMTYPE_SENSUIKANGYORAI = 32,
	SLOTITEMTYPE_SYOUMEITAN = 33,
	SLOTITEMTYPE_UNKNOWN = 34,
};

enum
{
	SHIPTYPE_KAIBOU = 1,
	SHIPTYPE_KUCHIKU = 2,
	SHIPTYPE_KEIJUN = 3,
	SHIPTYPE_RAIJUN = 4,
	SHIPTYPE_JUJUN = 5,
	SHIPTYPE_KOUJUN = 6,
	SHIPTYPE_KEIKUBO = 7,
	SHIPTYPE_KOUSOKUSENKAN = 8,
	SHIPTYPE_TEISOKUSENKAN = 9,
	SHIPTYPE_KOUSEN = 10,
	SHIPTYPE_KUBO = 11,
	SHIPTYPE_DOSENKAN = 12,
	SHIPTYPE_SENSUI = 13,
	SHIPTYPE_SENBO = 14,
	SHIPTYPE_HOKYU = 15,
	SHIPTYPE_SUIBO = 16,
	SHIPTYPE_YOURIKU = 17,
	SHIPTYPE_SOUKAKUBO = 18,
	SHIPTYPE_KOUSAKU = 19,
	SHIPTYPE_SENSUIBOKAN = 20,
	SHIPTYPE_UNKNOWN = 21,
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
	int lastdeckid;
	int maxslotitemid;

private:
	KanSaveData(void);
public:
	static KanSaveData& getInstance(void) {
		static KanSaveData singleton;
		return singleton;
	}
};

#endif // KANSAVEDATA_H
