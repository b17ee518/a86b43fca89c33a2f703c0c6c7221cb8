#ifndef KANDATACALC_H
#define KANDATACALC_H

#include <QString>
#include "kansavedata.h"

enum class WoundState
{
	Full,
	Little,
	Small,
	Middle,
	Big,
	Dead,
};

enum class ChargeState
{
	Full,
	Small,  //50%up
	Middle, //
	Big,
	Empty,
};

enum class CondState
{
	Kira,
	Normal,
	Small,
	Middle,
	Big,
};

class KanDataCalc
{
public:

	enum class LandBaseState
	{
		None,
		ShutsuGeki,
		BouKu,
	};

	static int GetAdmiralNextLevelExp(int curExp, int curLevel);
	static WoundState GetWoundState(int curHP, int maxHP);
	static ChargeState GetChargeState(int curVal, int maxVal);
	static CondState GetCondState(int cond);

	static int CalcTeamTyku(QList<ShipWithSlotItemsFullData> shipFullDataList, int& minTyku, int& maxTyku, LandBaseState landbaseState = LandBaseState::None);
	static double CalcTeamSakuTeki(QList<ShipWithSlotItemsFullData> shipFullDataList, int teitokuLevel, double mapModifier = 1.0, int maxSlotCount = 6);

private:
	static int GetAirCraftLevelBonus(SlotitemType slotitemtype, int alv);
	static int GetAirCraftExp(int alv);
};

#endif // KANDATACALC_H
