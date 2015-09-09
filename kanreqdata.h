#ifndef KANREQDATA_H
#define KANREQDATA_H

#include <QString>
#include <QPair>
#include <QVariant>
#include <QUrlQuery>

class KanReqData
{
public:
    KanReqData(){}

    void ReadFromString(const QString& strfunc, const QString& reqstr);
    QString GetItemAsString(const QString& item);

    QString _reqfunc;
    QUrlQuery _query;
};

#endif // KANREQDATA_H
