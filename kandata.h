﻿#ifndef KANDATA_H
#define KANDATA_H

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QList>
#include <QPair>

/*
 * End points
    "/kcsapi/api_start2",
    "/kcsapi/api_port/port",
    "/kcsapi/api_get_member/basic",
    "/kcsapi/api_get_member/ship",
    "/kcsapi/api_get_member/ship2",
    "/kcsapi/api_get_member/ship3",
    "/kcsapi/api_get_member/slot_item",
    "/kcsapi/api_get_member/useitem",
    "/kcsapi/api_get_member/deck",
    "/kcsapi/api_get_member/deck_port",
    "/kcsapi/api_get_member/ndock",
    "/kcsapi/api_get_member/kdock",
    "/kcsapi/api_get_member/material",
    "/kcsapi/api_get_member/questlist",
    "/kcsapi/api_req_hensei/change",
    "/kcsapi/api_req_kousyou/getship",
    "/kcsapi/api_req_kousyou/createitem",
    "/kcsapi/api_req_kousyou/createship",
    "/kcsapi/api_req_kousyou/createship_speedchange",
    "/kcsapi/api_req_kousyou/destroyship",
    "/kcsapi/api_req_kousyou/destroyitem2",
    "/kcsapi/api_req_nyukyo/start",
    "/kcsapi/api_req_nyukyo/speedchange",
    "/kcsapi/api_req_hokyu/charge",
    "/kcsapi/api_req_kaisou/powerup",
    "/kcsapi/api_req_member/updatedeckname",
    "/kcsapi/api_req_member/updatecomment",
    "/kcsapi/api_req_sortie/battle",
    "/kcsapi/api_req_sortie/battleresult",
*/

/*
 * Full List
kcsapi_basic
kcsapi_battleresult
kcsapi_battleresult_enemyinfo
kcsapi_battleresult_getship
kcsapi_charge
kcsapi_charge_ship
kcsapi_createitem
kcsapi_createship
kcsapi_deck
kcsapi_destroyitem2
kcsapi_destroyship
kcsapi_kdock
kcsapi_kdock_getship
kcsapi_material
kcsapi_mst_ship
kcsapi_mst_slotitem
kcsapi_mst_slotitem_equiptype
kcsapi_mst_stype
kcsapi_mst_useitem
kcsapi_ndock
kcsapi_port
kcsapi_powerup
kcsapi_quest
kcsapi_questlist
kcsapi_ship
kcsapi_ship2
kcsapi_ship3
kcsapi_slot_data
kcsapi_slotitem
kcsapi_start2
kcsapi_useitem
*/













class KAPIBaseData
{
public:
    KAPIBaseData(){}
    virtual ~KAPIBaseData(){}
    virtual bool ReadFromJObj(const QJsonObject &jobj) = 0;

protected:
    QJsonArray jarray;
    bool bParseRet;
};

class kcsapi_basic : public KAPIBaseData
{

public:
    kcsapi_basic(){}

    virtual bool ReadFromJObj(const QJsonObject &jobj);

    QString api_member_id;
    QString api_nickname;
    QString api_nickname_id;
    int api_active_flag;
    qint64 api_starttime;
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


class kcsapi_battleresult_enemyinfo : public KAPIBaseData
{

public:
    kcsapi_battleresult_enemyinfo(){}

    virtual bool ReadFromJObj(const QJsonObject &jobj);

    QString api_level;
    QString api_rank;
    QString api_deck_name;
};

class kcsapi_battleresult_getship : public KAPIBaseData
{

public:
    kcsapi_battleresult_getship(){}

    virtual bool ReadFromJObj(const QJsonObject &jobj);

    int api_ship_id;
    QString api_ship_type;
    QString api_ship_name;
    QString api_ship_getmes;
};

/**
 * @brief The kcsapi_battleresult class
 */
class kcsapi_battleresult : public KAPIBaseData
{

public:
    kcsapi_battleresult(){}

    virtual bool ReadFromJObj(const QJsonObject &jobj);

    QList<int> api_ship_id;
    QString api_win_rank;
    int api_get_exp;
    int api_mvp;
    int api_member_lv;
    int api_member_exp;
    int api_get_base_exp;
    QList<int> api_get_ship_exp;
    QList<QList<int>> api_get_exp_lvup;
    int api_dests;
    int api_destsf;
    QList<int> api_lost_flag;
    QString api_quest_name;
    int api_quest_level;
    kcsapi_battleresult_enemyinfo api_enemy_info;
    int api_first_clear;
    QList<int> api_get_flag;
    kcsapi_battleresult_getship api_get_ship;
    int api_get_eventflag;
    int api_get_exmap_rate;
    int api_get_exmap_useitem_id;
};

/**
 * @brief The kcsapi_slotitem class
 */
class kcsapi_slotitem : public KAPIBaseData
{

public:
    kcsapi_slotitem(){}

    virtual bool ReadFromJObj(const QJsonObject &jobj);

    int api_id;
    int api_slotitem_id;
};


class kcsapi_charge_ship : public KAPIBaseData
{

public:
    kcsapi_charge_ship(){}

    virtual bool ReadFromJObj(const QJsonObject &jobj);

    int api_id;
    int api_fuel;
    int api_bull;
    QList<int> api_onslot;
};

/**
 * @brief The kcsapi_charge class
 */
class kcsapi_charge : public KAPIBaseData
{

public:
    kcsapi_charge(){}

    virtual bool ReadFromJObj(const QJsonObject &jobj);

    QList<kcsapi_charge_ship> api_ship;
    QList<int> api_material;
    int api_use_bou;
};

/**
 * @brief The kcsapi_createitem class
 */
class kcsapi_createitem : public KAPIBaseData
{

public:
    kcsapi_createitem(){}

    virtual bool ReadFromJObj(const QJsonObject &jobj);

    int api_id;
    int api_slotitem_id;
    int api_create_flag;
    int api_shizai_flag;
    kcsapi_slotitem api_slot_item;
    QList<int> api_material;
    int api_type3;
    QList<int> api_unsetslot;
};

/**
 * @brief The kcsapi_createship class
 */
class kcsapi_createship : public KAPIBaseData
{

public:
    kcsapi_createship(){}

    virtual bool ReadFromJObj(const QJsonObject &jobj);

};

/**
 * @brief The kcsapi_deck class
 */
class kcsapi_deck : public KAPIBaseData
{

public:
    kcsapi_deck(){}

    virtual bool ReadFromJObj(const QJsonObject &jobj);

    int api_member_id;
    int api_id;
    QString api_name;
    QString api_name_id;
    QList<qint64> api_mission;
    QString api_flagship;
    QList<int> api_ship;
};

/**
 * @brief The kcsapi_destroyitem2 class
 */
class kcsapi_destroyitem2 : public KAPIBaseData
{

public:
    kcsapi_destroyitem2(){}

    virtual bool ReadFromJObj(const QJsonObject &jobj);

    QList<int> api_get_material;
};

/**
 * @brief The kcsapi_destroyship class
 */
class kcsapi_destroyship : public KAPIBaseData
{

public:
    kcsapi_destroyship(){}

    virtual bool ReadFromJObj(const QJsonObject &jobj);
    QList<int> api_material;
};

/**
 * @brief The kcsapi_kdock class
 */
class kcsapi_kdock : public KAPIBaseData
{

public:
    kcsapi_kdock(){}

    virtual bool ReadFromJObj(const QJsonObject &jobj);
    int api_member_id;
    int api_id;
    int api_state;
    int api_created_ship_id;
    qint64 api_complete_time;
    QString api_complete_time_str;
    int api_item1;
    int api_item2;
    int api_item3;
    int api_item4;
    int api_item5;
};

/**
 * @brief The kcsapi_ship2 class
 */
class kcsapi_ship2 : public KAPIBaseData
{

public:
    kcsapi_ship2(){}

    virtual bool ReadFromJObj(const QJsonObject &jobj);
    int api_id;
    int api_sortno;
    int api_ship_id;
    int api_lv;
    QList<int> api_exp;
    int api_nowhp;
    int api_maxhp;
    int api_leng;
    QList<int> api_slot;
    QList<int> api_onslot;
    QList<int> api_kyouka;
    int api_backs;
    int api_fuel;
    int api_bull;
    int api_slotnum;
    qint64 api_ndock_time;
    QList<int> api_ndock_item;
    int api_srate;
    int api_cond;
    QList<int> api_karyoku;
    QList<int> api_raisou;
    QList<int> api_taiku;
    QList<int> api_soukou;
    QList<int> api_kaihi;
    QList<int> api_taisen;
    QList<int> api_sakuteki;
    QList<int> api_lucky;
    int api_locked;
};

/**
 * @brief The kcsapi_kdock_getship class
 */
class kcsapi_kdock_getship : public KAPIBaseData
{

public:
    kcsapi_kdock_getship(){}

    virtual bool ReadFromJObj(const QJsonObject &jobj);
    int api_id;
    int api_ship_id;
    QList<kcsapi_kdock> api_kdock;
    kcsapi_ship2 api_ship;
    QList<kcsapi_slotitem> api_slotitem;
};

/**
 * @brief The kcsapi_material class
 */
class kcsapi_material: public KAPIBaseData
{

public:
    kcsapi_material(){}

    virtual bool ReadFromJObj(const QJsonObject &jobj);
    int api_member_id;
    int api_id;
    int api_value;
};

/**
 * @brief The kcsapi_mst_ship class
 */
class kcsapi_mst_ship: public KAPIBaseData
{

public:
    kcsapi_mst_ship(){}

    virtual bool ReadFromJObj(const QJsonObject &jobj);
    int api_id;
    int api_sortno;
    QString api_name;
    QString api_yomi;
    int api_stype;
    int api_ctype;
    int api_cnum;
    QString api_enqflg;
    int api_afterlv;
    QString api_aftershipid;
    QList<int> api_taik;
    QList<int> api_souk;
    QList<int> api_tous;
    QList<int> api_houg;
    QList<int> api_raig;
    QList<int> api_baku;
    QList<int> api_tyku;
    QList<int> api_atap;
    QList<int> api_tais;
    QList<int> api_houm;
    QList<int> api_raim;
    QList<int> api_kaih;
    QList<int> api_houk;
    QList<int> api_raik;
    QList<int> api_bakk;
    QList<int> api_saku;
    QList<int> api_sakb;
    QList<int> api_luck;
    int api_sokuh;
    int api_soku;
    int api_leng;
    QList<int> api_grow;
    int api_slot_num;
    QList<int> api_maxeq;
    QList<int> api_defeq;
    int api_buildtime;
    QList<int> api_broken;
    QList<int> api_powup;
    QList<int> api_gumax;
    int api_backs;
    QString api_getmes;
    QString api_homemes;  //o
    QString api_gomes;    //o
    QString api_gomes2;   //o
    QString api_sinfo;
    int api_afterfuel;
    int api_afterbull;
    QList<QString> api_touchs; //o
    QString api_missions; //o
    QString api_systems;  //o
    int api_fuel_max;
    int api_bull_max;
    int api_voicef;
};

/**
 * @brief The kcsapi_mst_slotitem class
 */
class kcsapi_mst_slotitem: public KAPIBaseData
{

public:
    kcsapi_mst_slotitem(){}

    virtual bool ReadFromJObj(const QJsonObject &jobj);
    int api_id;
    int api_sortno;
    QString api_name;
    QList<int> api_type;
    int api_taik;
    int api_souk;
    int api_houg;
    int api_raig;
    int api_soku;
    int api_baku;
    int api_tyku;
    int api_tais;
    int api_atap;
    int api_houm;
    int api_raim;
    int api_houk;
    int api_raik;
    int api_bakk;
    int api_saku;
    int api_sakb;
    int api_luck;
    int api_leng;
    int api_rare;
    QList<int> api_broken;
    QString api_info;
    QString api_usebull;
};

/**
 * @brief The kcsapi_mst_slotitem_equiptype class
 */
class kcsapi_mst_slotitem_equiptype: public KAPIBaseData
{

public:
    kcsapi_mst_slotitem_equiptype(){}

    virtual bool ReadFromJObj(const QJsonObject &jobj);
    int api_id;
    QString api_name;
    int api_show_flg;
};

/**
 * @brief The kcsapi_mst_stype class
 */
class kcsapi_mst_stype: public KAPIBaseData
{

public:
    kcsapi_mst_stype(){}

    virtual bool ReadFromJObj(const QJsonObject &jobj);
    int api_id;
    int api_sortno;
    QString api_name;
    int api_scnt;
    int api_kcnt;
};

/**
 * @brief The kcsapi_mst_useitem class
 */
class kcsapi_mst_useitem: public KAPIBaseData
{

public:
    kcsapi_mst_useitem(){}

    virtual bool ReadFromJObj(const QJsonObject &jobj);
    int api_id;
    int api_usetype;
    int api_category;
    QString api_name;
    QList<QString> api_description;
    int api_price;
};

/**
 * @brief The kcsapi_ndock class
 */
class kcsapi_ndock: public KAPIBaseData
{

public:
    kcsapi_ndock(){}

    virtual bool ReadFromJObj(const QJsonObject &jobj);
    int api_member_id;
    int api_id;
    int api_state;
    int api_ship_id;
    qint64 api_complete_time;
    QString api_complete_time_str;
    int api_item1;
    int api_item2;
    int api_item3;
    int api_item4;
};


/**
 * @brief The kcsapi_powerup class
 */
class kcsapi_powerup: public KAPIBaseData
{

public:
    kcsapi_powerup(){}

    virtual bool ReadFromJObj(const QJsonObject &jobj);
    int api_powerup_flag;
    kcsapi_ship2 api_ship;
    QList<kcsapi_deck> api_deck;
};

/**
 * @brief The kcsapi_quest class
 */
class kcsapi_quest: public KAPIBaseData
{

public:
    kcsapi_quest(){}

    virtual bool ReadFromJObj(const QJsonObject &jobj);

    int api_no;
    int api_category;
    int api_type;
    int api_state;
    QString api_title;
    QString api_detail;
    QList<int> api_get_material;
    int api_bonus_flag;
    int api_progress_flag;
};

/**
 * @brief The kcsapi_questlist class
 */
class kcsapi_questlist: public KAPIBaseData
{

public:
    kcsapi_questlist(){}

    virtual bool ReadFromJObj(const QJsonObject &jobj);

    int api_count;
    int api_page_count;
    int api_disp_page;
    QList<kcsapi_quest> api_list;
    int api_exec_count;
};

/**
 * @brief The kcsapi_ship class
 */
class kcsapi_ship: public KAPIBaseData
{

public:
    kcsapi_ship(){}

    virtual bool ReadFromJObj(const QJsonObject &jobj);

    int api_member_id;
    int api_id;
    int api_sortno;
    QString api_name;
    QString api_yomi;
    int api_stype;
    int api_ship_id;
    int api_lv;
    int api_exp;
    int api_afterlv;
    int api_aftershipid;
    int api_nowhp;
    int api_maxhp;
    QList<int> api_taik;
    QList<int> api_souk;
    QList<int> api_houg;
    QList<int> api_raig;
    QList<int> api_baku;
    QList<int> api_tyku;
    QList<int> api_houm;
    QList<int> api_raim;
    QList<int> api_saku;
    QList<int> api_luck;
    int api_soku;
    int api_leng;
    QList<int> api_slot;
    QList<int> api_onslot;
    QList<int> api_broken;
    QList<int> api_powup;
    QList<int> api_kyouka;
    int api_backs;
    int api_fuel;
    int api_fuel_max;
    int api_bull;
    int api_bull_max;
    QString api_gomes;    //o
    QString api_gomes2;   //o
    int api_slotnum;
    int api_ndock_time;
    QString api_ndock_time_str;
    QList<int> api_ndock_item;
    int api_star;
    int api_srate;
    int api_cond;
    QList<int> api_karyoku;
    QList<int> api_raisou;
    QList<int> api_taiku;
    QList<int> api_soukou;
    QList<int> api_kaihi;
    QList<int> api_taisen;
    QList<int> api_sakuteki;
    QList<int> api_lucky;
    int api_use_fuel;
    int api_use_bull;
    int api_voicef;
};

/**
 * @brief The kcsapi_ship3 class
 */
class kcsapi_ship3: public KAPIBaseData
{

public:
    kcsapi_ship3(){}

    virtual bool ReadFromJObj(const QJsonObject &jobj);

    QList<kcsapi_ship2> api_ship_data;
    QList<kcsapi_deck> api_deck_data;

    //
    //public kcsapi_slot_data api_slot_data;
};
class kcsapi_slot_data: public KAPIBaseData
{

public:
    kcsapi_slot_data(){}

    virtual bool ReadFromJObj(const QJsonObject &jobj);
    QList<int> api_slottype1;
    QList<int> api_slottype2;
    QList<int> api_slottype3;
    QList<int> api_slottype4;
    QList<int> api_slottype5;
    QList<int> api_slottype6;
    QList<int> api_slottype7;
    QList<int> api_slottype8;
    QList<int> api_slottype9;
    QList<int> api_slottype10;
    QList<int> api_slottype11;
    QList<int> api_slottype12;
    QList<int> api_slottype13;
    QList<int> api_slottype14;
    QList<int> api_slottype15;
    QList<int> api_slottype16;
    QList<int> api_slottype17;
    QList<int> api_slottype18;
    QList<int> api_slottype19;
    QList<int> api_slottype20;
    QList<int> api_slottype21;
    QList<int> api_slottype22;
    QList<int> api_slottype23;
};

class Api_Mst_Item_Shop: public KAPIBaseData
{

public:
    Api_Mst_Item_Shop(){}

virtual bool ReadFromJObj(const QJsonObject &jobj);
    QList<int> api_cabinet_1;
    QList<int> api_cabinet_2;
};

class Api_Boko_Max_Ships: public KAPIBaseData
{

public:
    Api_Boko_Max_Ships(){}

virtual bool ReadFromJObj(const QJsonObject &jobj);
    QString api_string_value;
    int api_int_value;
};

class Api_Mst_Const: public KAPIBaseData
{

public:
    Api_Mst_Const(){}

virtual bool ReadFromJObj(const QJsonObject &jobj);
    Api_Boko_Max_Ships api_boko_max_ships;
};


class Api_Mst_Ship: public KAPIBaseData
{

public:
    Api_Mst_Ship(){}

virtual bool ReadFromJObj(const QJsonObject &jobj);
    int api_id;
    int api_sortno;
    QString api_name;
    QString api_yomi;
    int api_stype;
    int api_ctype;
    int api_cnum;
    QString api_enqflg;
    int api_afterlv;
    QString api_aftershipid;
    QList<int> api_taik;
    QList<int> api_souk;
    QList<int> api_tous;
    QList<int> api_houg;
    QList<int> api_raig;
    QList<int> api_baku;
    QList<int> api_tyku;
    QList<int> api_atap;
    QList<int> api_tais;
    QList<int> api_houm;
    QList<int> api_raim;
    QList<int> api_kaih;
    QList<int> api_houk;
    QList<int> api_raik;
    QList<int> api_bakk;
    QList<int> api_saku;
    QList<int> api_sakb;
    QList<int> api_luck;
    int api_sokuh;
    int api_soku;
    int api_leng;
    QList<int> api_grow;
    int api_slot_num;
    QList<int> api_maxeq;
    QList<int> api_defeq;
    int api_buildtime;
    QList<int> api_broken;
    QList<int> api_powup;
    QList<int> api_gumax;
    int api_backs;
    QString api_getmes;
    QString api_homemes; //o
    QString api_gomes; //o
    QString api_gomes2; //o
    QString api_sinfo;
    int api_afterfuel;
    int api_afterbull;
    QList<QString> api_touchs; //o[]
    QString api_missions; //o
    QString api_systems; //o
    int api_fuel_max;
    int api_bull_max;
    int api_voicef;
};

class Api_Mst_Shipgraph: public KAPIBaseData
{

public:
    Api_Mst_Shipgraph(){}

virtual bool ReadFromJObj(const QJsonObject &jobj);
    int api_id;
    int api_sortno;
    QString api_filename;
    QString api_version;
    QList<int> api_boko_n;
    QList<int> api_boko_d;
    QList<int> api_kaisyu_n;
    QList<int> api_kaisyu_d;
    QList<int> api_kaizo_n;
    QList<int> api_kaizo_d;
    QList<int> api_map_n;
    QList<int> api_map_d;
    QList<int> api_ensyuf_n;
    QList<int> api_ensyuf_d;
    QList<int> api_ensyue_n;
    QList<int> api_battle_n;
    QList<int> api_battle_d;
    QList<int> api_weda;
    QList<int> api_wedb;
};

class Api_Equip_Type: public KAPIBaseData
{

public:
    Api_Equip_Type(){}

virtual bool ReadFromJObj(const QJsonObject &jobj);
    int _1;
    int _2;
    int _3;
    int _4;
    int _5;
    int _6;
    int _7;
    int _8;
    int _9;
    int _10;
    int _11;
    int _12;
    int _13;
    int _14;
    int _15;
    int _16;
    int _17;
    int _18;
    int _19;
    int _20;
    int _21;
    int _22;
    int _23;
    int _24;
    int _25;
    int _26;
    int _27;
    int _28;
    int _29;
    int _30;
    int _31;
};

class Api_Mst_Stype: public KAPIBaseData
{

public:
    Api_Mst_Stype(){}

virtual bool ReadFromJObj(const QJsonObject &jobj);
    int api_id;
    int api_sortno;
    QString api_name;
    int api_scnt;
    int api_kcnt;
    Api_Equip_Type api_equip_type;
};

class Api_Mst_Slotitem: public KAPIBaseData
{

public:
    Api_Mst_Slotitem(){}

virtual bool ReadFromJObj(const QJsonObject &jobj);
    int api_id;
    int api_sortno;
    QString api_name;
    QList<int> api_type;
    int api_taik;
    int api_souk;
    int api_houg;
    int api_raig;
    int api_soku;
    int api_baku;
    int api_tyku;
    int api_tais;
    int api_atap;
    int api_houm;
    int api_raim;
    int api_houk;
    int api_raik;
    int api_bakk;
    int api_saku;
    int api_sakb;
    int api_luck;
    int api_leng;
    int api_rare;
    QList<int> api_broken;
    QString api_info;
    QString api_usebull;
};

class Api_Mst_Slotitemgraph: public KAPIBaseData
{

public:
    Api_Mst_Slotitemgraph(){}

virtual bool ReadFromJObj(const QJsonObject &jobj);
    int api_id;
    int api_sortno;
    QString api_filename;
    QString api_version;
};

class Api_Mst_Furniture: public KAPIBaseData
{

public:
    Api_Mst_Furniture(){}

virtual bool ReadFromJObj(const QJsonObject &jobj);
    int api_id;
    int api_type;
    int api_no;
    QString api_title;
    QString api_description;
    int api_rarity;
    int api_price;
    int api_saleflg;
};

class Api_Mst_Furnituregraph: public KAPIBaseData
{

public:
    Api_Mst_Furnituregraph(){}

virtual bool ReadFromJObj(const QJsonObject &jobj);
    int api_id;
    int api_type;
    int api_no;
    QString api_filename;
    QString api_version;
};

class Api_Mst_Useitem: public KAPIBaseData
{

public:
    Api_Mst_Useitem(){}

virtual bool ReadFromJObj(const QJsonObject &jobj);
    int api_id;
    int api_usetype;
    int api_category;
    QString api_name;
    QList<QString> api_description;
    int api_price;
};

class Api_Mst_Payitem: public KAPIBaseData
{

public:
    Api_Mst_Payitem(){}

virtual bool ReadFromJObj(const QJsonObject &jobj);
    int api_id;
    int api_type;
    QString api_name;
    QString api_description;
    QList<int> api_item;
    int api_price;
};

class Api_Mst_Maparea: public KAPIBaseData
{

public:
    Api_Mst_Maparea(){}

virtual bool ReadFromJObj(const QJsonObject &jobj);
    int api_id;
    QString api_name;
    int api_type;
};

class Api_Mst_Mapinfo: public KAPIBaseData
{

public:
    Api_Mst_Mapinfo(){}

virtual bool ReadFromJObj(const QJsonObject &jobj);
    int api_id;
    int api_maparea_id;
    int api_no;
    QString api_name;
    int api_level;
    QString api_opetext;
    QString api_infotext;
    QList<int> api_item;
    QList<int> api_max_maphp; //int?
    QList<int> api_required_defeat_count; //int?
};

class Api_Mst_Mapbgm: public KAPIBaseData
{

public:
    Api_Mst_Mapbgm(){}

virtual bool ReadFromJObj(const QJsonObject &jobj);
    int api_id;
    int api_maparea_id;
    int api_no;
    QList<int> api_map_bgm;
    QList<int> api_boss_bgm;
};

class Api_Mst_Mapcell: public KAPIBaseData
{

public:
    Api_Mst_Mapcell(){}

virtual bool ReadFromJObj(const QJsonObject &jobj);
    int api_map_no;
    int api_maparea_id;
    int api_mapinfo_no;
    int api_id;
    int api_no;
    int api_color_no;
};

class Api_Mst_Mission: public KAPIBaseData
{

public:
    Api_Mst_Mission(){}

virtual bool ReadFromJObj(const QJsonObject &jobj);
    int api_id;
    int api_maparea_id;
    QString api_name;
    QString api_details;
    int api_time;
    int api_difficulty;
    float api_use_fuel;
    float api_use_bull;
    QList<int> api_win_item1;
    QList<int> api_win_item2;
};

class Api_Mst_Shipupgrade: public KAPIBaseData
{

public:
    Api_Mst_Shipupgrade(){}

virtual bool ReadFromJObj(const QJsonObject &jobj);
    int api_id;
    int api_original_ship_id;
    int api_upgrade_type;
    int api_upgrade_level;
    int api_drawing_count;
    int api_sortno;
};
/**
 * @brief The kcsapi_start2 class
 */
class kcsapi_start2: public KAPIBaseData
{

public:
    kcsapi_start2(){}

    virtual bool ReadFromJObj(const QJsonObject &jobj);

    QList<kcsapi_mst_ship> api_mst_ship;
    QList<kcsapi_mst_slotitem> api_mst_slotitem;
    QList<kcsapi_mst_useitem> api_mst_useitem;
    QList<kcsapi_mst_stype> api_mst_stype;
    QList<kcsapi_mst_slotitem_equiptype> api_mst_slotitem_equiptype;

    // Keep

    QList<Api_Mst_Shipgraph> api_mst_shipgraph;
    QList<Api_Mst_Slotitemgraph> api_mst_slotitemgraph;
    QList<Api_Mst_Furniture> api_mst_furniture;
    QList<Api_Mst_Furnituregraph> api_mst_furnituregraph;
    QList<Api_Mst_Payitem> api_mst_payitem;
    Api_Mst_Item_Shop api_mst_item_shop;
    QList<Api_Mst_Maparea> api_mst_maparea;
    QList<Api_Mst_Mapinfo> api_mst_mapinfo;
    QList<Api_Mst_Mapbgm> api_mst_mapbgm;
    QList<Api_Mst_Mapcell> api_mst_mapcell;
    QList<Api_Mst_Mission> api_mst_mission;
    Api_Mst_Const api_mst_const;
    QList<Api_Mst_Shipupgrade> api_mst_shipupgrade;

};

/**
 * @brief The kcsapi_port class
 */
class kcsapi_port: public KAPIBaseData
{

public:
    kcsapi_port(){}

    virtual bool ReadFromJObj(const QJsonObject &jobj);
    QList<kcsapi_material> api_material;
    QList<kcsapi_deck> api_deck_port;
    QList<kcsapi_ndock> api_ndock;
    QList<kcsapi_ship2> api_ship;
    kcsapi_basic api_basic;
    //public Api_Log[] api_log;
};

/**
 * @brief The kcsapi_useitem class
 */
class kcsapi_useitem: public KAPIBaseData
{

public:
    kcsapi_useitem(){}

    virtual bool ReadFromJObj(const QJsonObject &jobj);

    int api_member_id;
    int api_id;
    int api_value;
    int api_usetype;
    int api_category;
    QString api_name;
    QList<QString> api_description;
    int api_price;
    int api_count;
};

class kcsapi_next_enemy : public KAPIBaseData
{
public:
    kcsapi_next_enemy(){}

    virtual bool ReadFromJObj(const QJsonObject &jobj);

    int api_enemy_id;
    int api_result;
    QString api_result_str;
};

/**
 * @brief The kcsapi_next class
 */
class kcsapi_next: public KAPIBaseData
{
public:
    kcsapi_next(){}

    virtual bool ReadFromJObj(const QJsonObject &jobj);
    int api_rashin_flg;
    int api_rashin_id;
    int api_maparea_id;
    int api_mapinfo_no;
    int api_no;
    int api_color_no;
    int api_event_id;
    int api_event_kind;
    int api_next;
    int api_bosscell_no;
    int api_bosscomp;
//    int api_comment_kind;
//    int api_production_kind;
    kcsapi_next_enemy api_enemy;
};

/**
 * @brief The kcsapi_mission_start class
 */
class kcsapi_mission_start: public KAPIBaseData
{
public:
    kcsapi_mission_start(){}

    virtual bool ReadFromJObj(const QJsonObject &jobj);

    qint64 api_complatetime;
    QString api_complatetime_str;

};

class KanData
{
public:
    KanData();

private:
};

#endif // KANDATA_H
