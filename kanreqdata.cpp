#include "kanreqdata.h"

void KanReqData::ReadFromString(const QString& strfunc, const QString& reqstr)
{
	_reqfunc = strfunc;
	_query.clear();
	_query.setQuery(reqstr);

	//    qDebug(query.queryItemValue("api_token").toUtf8());
}

QString KanReqData::GetItemAsString(const QString& item)
{
	return _query.queryItemValue(item);
}
