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
/*
 * Replace \ and "
 *
    QString str = "";
    QJsonDocument doc = QJsonDocument::fromJson(str.toLocal8Bit());
    KBasicData d;
    d.ReadFromJObj(doc.object()["api_data"].toObject());
*/

bool kcsapi_basic::ReadFromJObj(const QJsonObject &jobj)
{
    /*
    jarray = jobj["api_furniture"].toArray();
    api_furniture.clear();
    foreach(const QJsonValue &v, jarray)
    {
        api_furniture.append(v.toInt());
    }
    api_member_id = jobj["api_member_id"].toString();
    */

    _SREAD(api_member_id);
    _SREAD(api_nickname);
    _SREAD(api_nickname_id);
    _IREAD(api_active_flag);
    _LREAD(api_starttime);
    _IREAD(api_level);
    _IREAD(api_rank);
    _IREAD(api_experience);
    _SREAD(api_fleetname);  //object
    _SREAD(api_comment);
    _SREAD(api_comment_id);
    _IREAD(api_max_chara);
    _IREAD(api_max_slotitem);
    _IREAD(api_max_kagu);
    _IREAD(api_playtime);
    _IREAD(api_tutorial);
    _AIREAD(api_furniture);
    _IREAD(api_count_deck);
    _IREAD(api_count_kdock);
    _IREAD(api_count_ndock);
    _IREAD(api_fcoin);
    _IREAD(api_st_win);
    _IREAD(api_st_lose);
    _IREAD(api_ms_count);
    _IREAD(api_ms_success);
    _IREAD(api_pt_win);
    _IREAD(api_pt_lose);
    _IREAD(api_pt_challenged);
    _IREAD(api_pt_challenged_win);
    _IREAD(api_firstflag);
    _IREAD(api_tutorial_progress);
    _AIREAD(api_pvp);

//    qDebug(api_comment.toUtf8());

    return true;
}


bool kcsapi_battleresult_enemyinfo::ReadFromJObj(const QJsonObject &jobj)
{

    QString api_level;
    QString api_rank;
    QString api_deck_name;
}
