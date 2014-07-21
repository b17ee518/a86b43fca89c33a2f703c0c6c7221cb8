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
    MATERIALDATAINDEX_INSTANDREPAIR,
    MATERIALDATAINDEX_DEVELOPMENT,
};

class KanSaveData
{
public:

    kcsapi_port portdata;
//    QList<kcsapi_ship> shipdata;
    kcsapi_start2 start2data;
    QList<kcsapi_slotitem> slotitemdata;
    QList<kcsapi_kdock> kdockdata;
    QList<kcsapi_material> materialdata;
    QList<kcsapi_quest> questdata;
    kcsapi_next nextdata;

    int shipcountoffset;
    int slotitemcountoffset;

private:
    KanSaveData(void);
public:
    static KanSaveData& getInstance(void) {
        static KanSaveData singleton;
        return singleton;
    }
};

#endif // KANSAVEDATA_H
