#include "kansavedata.h"

#define _CASTBEGIN(cname)\
    if (name.contains(#cname))\
    {\
        cname *kandata = static_cast<cname*>(_kandata)
#define _CASTEND()\
    }

void KanSaveData::UpdateDataFromResponse(QString name, KAPIBaseData *_kandata)
{
    _CASTBEGIN(kcsapi_basic);
        this->portdata.api_basic = *kandata;
    _CASTEND();
}

void KanSaveData::UpdateDataFromApprovedRequest(KanReqData *preq)
{

}

