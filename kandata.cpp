#include "kandata.h"

#define _SREAD(name)    name=jobj[#name].toString();
#define _IREAD(name)    name=jobj[#name].toInt();
#define _LREAD(name)    name=(long)jobj[#name].toDouble();

#define _AIREAD(name)   jarray=jobj[#name].toArray();\
                        name.clear();\
                        foreach(const QJsonValue &v, jarray)\
                        {\
                            name.append(v.toInt());\
                        }


KanData::KanData()
{
}


bool KAdmiralData::ReadFromeJObj(const QJsonObject &jobj)
{
    /*
    jarray = jobj["api_furniture"].toArray();
    api_furniture.clear();
    foreach(const QJsonValue &v, jarray)
    {
        api_furniture.append(v.toInt());
    }
    */
}
