#ifndef KANDATACALC_H
#define KANDATACALC_H

#include <QString>

enum
{
    WOUNDSTATE_FULL,
    WOUNDSTATE_LITTLE,
    WOUNDSTATE_SMALL,
    WOUNDSTATE_MIDDLE,
    WOUNDSTATE_BIG,
    WOUNDSTATE_DEAD,
};

enum
{
    CHARGESTATE_FULL,
    CHARGESTATE_SMALL,  //50%up
    CHARGESTATE_MIDDLE, //
    CHARGESTATE_BIG,
    CHARGESTATE_EMPTY,
};

enum
{
	CONDSTATE_KIRA,
    CONDSTATE_NORMAL,
    CONDSTATE_SMALL,
    CONDSTATE_MIDDLE,
    CONDSTATE_BIG,
};

class KanDataCalc
{
public:

    static int GetAdmiralNextLevelExp(int curExp, int curLevel);
    static int GetWoundState(int curHP, int maxHP);
    static int GetChargeState(int curVal, int maxVal);
    static int GetCondState(int cond);
};

#endif // KANDATACALC_H
