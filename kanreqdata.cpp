#include "kanreqdata.h"

void KanReqData::ReadFromString(QString strfunc, QString reqstr)
{
    reqfunc = strfunc;
    query.clear();
    query.setQuery(reqstr);

    //    qDebug(query.queryItemValue("api_token").toUtf8());
}

QString KanReqData::GetItemAsString(QString item)
{
    return query.queryItemValue(item);
}
