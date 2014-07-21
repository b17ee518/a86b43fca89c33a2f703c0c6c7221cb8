#include "kansavedata.h"

KanSaveData::KanSaveData()
{
    kcsapi_material tm;
    materialdata.append(tm);
    materialdata.append(tm);
    materialdata.append(tm);
    materialdata.append(tm);
    materialdata.append(tm);
    materialdata.append(tm);
    materialdata.append(tm);

    shipcountoffset = 0;
    slotitemcountoffset = 0;
}
