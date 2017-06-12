#ifndef KANDATACALC_H
#define KANDATACALC_H

#include <QString>

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

	static int GetAdmiralNextLevelExp(int curExp, int curLevel);
	static WoundState GetWoundState(int curHP, int maxHP);
	static ChargeState GetChargeState(int curVal, int maxVal);
	static CondState GetCondState(int cond);
};

#endif // KANDATACALC_H
