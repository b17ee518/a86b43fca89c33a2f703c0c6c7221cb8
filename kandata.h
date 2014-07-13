#ifndef KANDATA_H
#define KANDATA_H

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QList>

class KAPIBaseData
{
public:
    KAPIBaseData(){}
    virtual bool ReadFromJObj(const QJsonObject &jobj) = 0;

protected:
    QJsonArray jarray;
};

class KBasicData : public KAPIBaseData
{

public:
    KBasicData(){}

    virtual bool ReadFromJObj(const QJsonObject &jobj);

    QString api_member_id;
    QString api_nickname;
    QString api_nickname_id;
    int api_active_flag;
    long api_starttime;
    int api_level;
    int api_rank;
    int api_experience;
    QString api_fleetname;  //object
    QString api_comment;
    QString api_comment_id;
    int api_max_chara;
    int api_max_slotitem;
    int api_max_kagu;
    int api_playtime;
    int api_tutorial;
    QList<int> api_furniture;
    int api_count_deck;
    int api_count_kdock;
    int api_count_ndock;
    int api_fcoin;
    int api_st_win;
    int api_st_lose;
    int api_ms_count;
    int api_ms_success;
    int api_pt_win;
    int api_pt_lose;
    int api_pt_challenged;
    int api_pt_challenged_win;
    int api_firstflag;
    int api_tutorial_progress;
    QList<int> api_pvp;

};

class KanData
{
public:
    KanData();

private:
};

#endif // KANDATA_H
