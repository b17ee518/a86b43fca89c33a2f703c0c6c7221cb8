#include "kansavedata.h"

KanSaveData::KanSaveData()
{
    kcsapi_material tm;
    tm.api_value = 0;
    portdata.api_material.append(tm);
    portdata.api_material.append(tm);
    portdata.api_material.append(tm);
    portdata.api_material.append(tm);
    portdata.api_material.append(tm);
    portdata.api_material.append(tm);
    portdata.api_material.append(tm);

    shipcountoffset = 0;
    slotitemcountoffset = 0;
}
