#ifndef KANDATACALC_H
#define KANDATACALC_H

#include <QString>

enum
{
    WOUNDSTATE_NORMAL,
    WOUNDSTATE_SMALL,
    WOUNDSTATE_MIDDLE,
    WOUNDSTATE_BIG,
    WOUNDSTATE_DEAD,
};

class KanDataCalc
{
public:

    static int GetAdmiralNextLevelExp(int curExp, int curLevel);
    static QString GetWoundStateAsString(int curHP, int maxHP);
    static int GetWoundState(int curHP, int maxHP);
};

#endif // KANDATACALC_H
