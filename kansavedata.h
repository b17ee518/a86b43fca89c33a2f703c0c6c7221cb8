#ifndef KANSAVEDATA_H
#define KANSAVEDATA_H

#include "kandata.h"
#include "kanreqdata.h"

class KanSaveData
{
public:

    void UpdateDataFromResponse(QString name, KAPIBaseData * _kandata);
    void UpdateDataFromApprovedRequest(KanReqData * preq);

    kcsapi_port portdata;
    QList<kcsapi_ship> shipdata;
    kcsapi_start2 mstdata;
    QList<kcsapi_slotitem> slotitemdata;
    kcsapi_kdock kenzoudata;
    kcsapi_ndock nyukudata;
    kcsapi_material materialdata;

private:
    KanSaveData(void) {}
public:
    static KanSaveData& getInstance(void) {
        static KanSaveData singleton;
        return singleton;
    }
};

#endif // KANSAVEDATA_H
