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

    void ReadFromString(QString strfunc, QString reqstr);
    QString GetItemAsString(QString item);

    QString reqfunc;
    QUrlQuery query;
};

#endif // KANREQDATA_H
