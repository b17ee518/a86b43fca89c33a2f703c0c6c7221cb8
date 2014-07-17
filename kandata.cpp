#include "kandata.h"
#include "kansavedata.h"

#define _SREAD(name)    name=jobj[#name].toString();
#define _IREAD(name)    name=jobj[#name].toInt();
#define _LREAD(name)    name=(long)jobj[#name].toDouble();

#define _AIREAD(name)   \
    jarray=jobj[#name].toArray();\
    name.clear();\
    foreach(const QJsonValue &v, jarray)\
    {\
        name.append(v.toInt());\
    }

#define _AAIREAD(name) \
    jarray = jobj[#name].toArray();\
    name.clear();\
    foreach(const QJsonValue &v, jarray)\
    {\
        QJsonArray jt = v.toArray();\
        QList<int> lt;\
        foreach(const QJsonValue &vt, jt)\
        {\
            lt.append(vt.toInt());\
        }\
        name.append(lt);\
    }

#define _ASREAD(name)   \
    jarray=jobj[#name].toArray();\
    name.clear();\
    foreach(const QJsonValue &v, jarray)\
    {\
        name.append(v.toString());\
    }

#define _ALREAD(name)   \
    jarray=jobj[#name].toArray();\
    name.clear();\
    foreach(const QJsonValue &v, jarray)\
    {\
        name.append((long)v.toDouble());\
    }

#define _ACREAD(name, c)   \
    jarray=jobj[#name].toArray();\
    name.clear();\
    foreach(const QJsonValue &v, jarray)\
    {\
        c tc;\
        tc.ReadFromJObj(v.toObject());\
        name.append(tc);\
    }

#define _CREAD(name, c)\
    name.ReadFromJObj(jobj[#c].toObject());


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
    /*
    jarray = jobj["api_furniture"].toArray();
    api_furniture.clear();
    foreach(const QJsonValue &v, jarray)
    {
        QJsonArray jt = v.toArray();
        QList<int> lt;
        foreach(const QJsonValue &vt, jt)
        {
            lt.append(vt.toInt());
        }
        api_furniture.append(lt);
    }
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
    UpdateSaveData(typeid(this).name(), this);

    return true;
}


bool kcsapi_battleresult_enemyinfo::ReadFromJObj(const QJsonObject &jobj)
{

    _SREAD(api_level);
    _SREAD(api_rank);
    _SREAD(api_deck_name);

    return bParseRet;
}


bool kcsapi_battleresult_getship::ReadFromJObj(const QJsonObject &jobj)
{
    _IREAD(api_ship_id);
    _SREAD(api_ship_type);
    _SREAD(api_ship_name);
    _SREAD(api_ship_getmes);

    return bParseRet;
}


bool kcsapi_battleresult::ReadFromJObj(const QJsonObject &jobj)
{
    _AIREAD(api_ship_id);
    _SREAD(api_win_rank);
    _IREAD(api_get_exp);
    _IREAD(api_mvp);
    _IREAD(api_member_lv);
    _IREAD(api_member_exp);
    _IREAD(api_get_base_exp);
    _AIREAD(api_get_ship_exp);
    _AAIREAD(api_get_exp_lvup);
    _IREAD(api_dests);
    _IREAD(api_destsf);
    _AIREAD(api_lost_flag);
    _SREAD(api_quest_name);
    _IREAD(api_quest_level);
    _CREAD(api_enemy_info, kcsapi_battleresult_enemyinfo);
    _IREAD(api_first_clear);
    _AIREAD(api_get_flag);
    _CREAD(api_get_ship, kcsapi_battleresult_getship);
    _IREAD(api_get_eventflag);
    _IREAD(api_get_exmap_rate);
    _IREAD(api_get_exmap_useitem_id);

    return bParseRet;

}


bool kcsapi_slotitem::ReadFromJObj(const QJsonObject &jobj)
{
    _IREAD(api_id);
    _IREAD(api_slotitem_id);

    return bParseRet;

}


bool kcsapi_charge_ship::ReadFromJObj(const QJsonObject &jobj)
{
    _IREAD(api_id);
    _IREAD(api_fuel);
    _IREAD(api_bull);
    _AIREAD(api_onslot);

    return bParseRet;

}


bool kcsapi_charge::ReadFromJObj(const QJsonObject &jobj)
{
    _ACREAD(api_ship, kcsapi_charge_ship);
    _AIREAD(api_material);
    _IREAD(api_use_bou);

    return bParseRet;

}


bool kcsapi_createitem::ReadFromJObj(const QJsonObject &jobj)
{
    _IREAD(api_id);
    _IREAD(api_slotitem_id);
    _IREAD(api_create_flag);
    _IREAD(api_shizai_flag);
    _CREAD(api_slot_item, kcsapi_charge_ship);
    _AIREAD(api_material);
    _IREAD(api_type3);
    _AIREAD(api_unsetslot);

    return bParseRet;

}


bool kcsapi_createship::ReadFromJObj(const QJsonObject &jobj)
{

    return true;

}


bool kcsapi_deck::ReadFromJObj(const QJsonObject &jobj)
{
    _IREAD(api_member_id);
    _IREAD(api_id);
    _SREAD(api_name);
    _SREAD(api_name_id);
    _ALREAD(api_mission);
    _SREAD(api_flagship);
    _AIREAD(api_ship);

    return bParseRet;

}


bool kcsapi_destroyitem2::ReadFromJObj(const QJsonObject &jobj)
{
    _AIREAD(api_get_material);

    return bParseRet;

}


bool kcsapi_destroyship::ReadFromJObj(const QJsonObject &jobj)
{
    _AIREAD(api_material);

    return bParseRet;

}


bool kcsapi_kdock::ReadFromJObj(const QJsonObject &jobj)
{
    _IREAD(api_member_id);
    _IREAD(api_id);
    _IREAD(api_state);
    _IREAD(api_created_ship_id);
    _LREAD(api_complete_time);
    _SREAD(api_complete_time_str);
    _IREAD(api_item1);
    _IREAD(api_item2);
    _IREAD(api_item3);
    _IREAD(api_item4);
    _IREAD(api_item5);

    return bParseRet;

}


bool kcsapi_ship2::ReadFromJObj(const QJsonObject &jobj)
{
    _IREAD(api_id);
    _IREAD(api_sortno);
    _IREAD(api_ship_id);
    _IREAD(api_lv);
    _AIREAD(api_exp);
    _IREAD(api_nowhp);
    _IREAD(api_maxhp);
    _IREAD(api_leng);
    _AIREAD(api_slot);
    _AIREAD(api_onslot);
    _AIREAD(api_kyouka);
    _IREAD(api_backs);
    _IREAD(api_fuel);
    _IREAD(api_bull);
    _IREAD(api_slotnum);
    _LREAD(api_ndock_time);
    _AIREAD(api_ndock_item);
    _IREAD(api_srate);
    _IREAD(api_cond);
    _AIREAD(api_karyoku);
    _AIREAD(api_raisou);
    _AIREAD(api_taiku);
    _AIREAD(api_soukou);
    _AIREAD(api_kaihi);
    _AIREAD(api_taisen);
    _AIREAD(api_sakuteki);
    _AIREAD(api_lucky);
    _IREAD(api_locked);

    return bParseRet;

}


bool kcsapi_kdock_getship::ReadFromJObj(const QJsonObject &jobj)
{
    _IREAD(api_id);
    _IREAD(api_ship_id);
    _ACREAD(api_kdock, kcsapi_kdock);
    _CREAD(api_ship, kcsapi_ship2);
    _ACREAD(api_slotitem, kcsapi_slotitem);

    return bParseRet;

}


bool kcsapi_material::ReadFromJObj(const QJsonObject &jobj)
{
    _IREAD(api_member_id);
    _IREAD(api_id);
    _IREAD(api_value);

    return bParseRet;

}


bool kcsapi_mst_ship::ReadFromJObj(const QJsonObject &jobj)
{
    _IREAD(api_id);
    _IREAD(api_sortno);
    _SREAD(api_name);
    _SREAD(api_yomi);
    _IREAD(api_stype);
    _IREAD(api_ctype);
    _IREAD(api_cnum);
    _SREAD(api_enqflg);
    _IREAD(api_afterlv);
    _SREAD(api_aftershipid);
    _AIREAD(api_taik);
    _AIREAD(api_souk);
    _AIREAD(api_tous);
    _AIREAD(api_houg);
    _AIREAD(api_raig);
    _AIREAD(api_baku);
    _AIREAD(api_tyku);
    _AIREAD(api_atap);
    _AIREAD(api_tais);
    _AIREAD(api_houm);
    _AIREAD(api_raim);
    _AIREAD(api_kaih);
    _AIREAD(api_houk);
    _AIREAD(api_raik);
    _AIREAD(api_bakk);
    _AIREAD(api_saku);
    _AIREAD(api_sakb);
    _AIREAD(api_luck);
    _IREAD(api_sokuh);
    _IREAD(api_soku);
    _IREAD(api_leng);
    _AIREAD(api_grow);
    _IREAD(api_slot_num);
    _AIREAD(api_maxeq);
    _AIREAD(api_defeq);
    _IREAD(api_buildtime);
    _AIREAD(api_broken);
    _AIREAD(api_powup);
    _AIREAD(api_gumax);
    _IREAD(api_backs);
    _SREAD(api_getmes);
    _SREAD(api_homemes);  //o
    _SREAD(api_gomes);    //o
    _SREAD(api_gomes2);   //o
    _SREAD(api_sinfo);
    _IREAD(api_afterfuel);
    _IREAD(api_afterbull);
    _ASREAD(api_touchs); //o
    _SREAD(api_missions); //o
    _SREAD(api_systems);  //o
    _IREAD(api_fuel_max);
    _IREAD(api_bull_max);
    _IREAD(api_voicef);

    return bParseRet;

}


bool kcsapi_mst_slotitem::ReadFromJObj(const QJsonObject &jobj)
{
    _IREAD(api_id);
    _IREAD(api_sortno);
    _SREAD(api_name);
    _AIREAD(api_type);
    _IREAD(api_taik);
    _IREAD(api_souk);
    _IREAD(api_houg);
    _IREAD(api_raig);
    _IREAD(api_soku);
    _IREAD(api_baku);
    _IREAD(api_tyku);
    _IREAD(api_tais);
    _IREAD(api_atap);
    _IREAD(api_houm);
    _IREAD(api_raim);
    _IREAD(api_houk);
    _IREAD(api_raik);
    _IREAD(api_bakk);
    _IREAD(api_saku);
    _IREAD(api_sakb);
    _IREAD(api_luck);
    _IREAD(api_leng);
    _IREAD(api_rare);
    _AIREAD(api_broken);
    _SREAD(api_info);
    _SREAD(api_usebull);

    return bParseRet;

}


bool kcsapi_mst_slotitem_equiptype::ReadFromJObj(const QJsonObject &jobj)
{
    _IREAD(api_id);
    _SREAD(api_name);
    _IREAD(api_show_flg);

    return bParseRet;

}


bool kcsapi_mst_stype::ReadFromJObj(const QJsonObject &jobj)
{
    _IREAD(api_id);
    _IREAD(api_sortno);
    _SREAD(api_name);
    _IREAD(api_scnt);
    _IREAD(api_kcnt);

    return bParseRet;

}


bool kcsapi_mst_useitem::ReadFromJObj(const QJsonObject &jobj)
{
    _IREAD(api_id);
    _IREAD(api_usetype);
    _IREAD(api_category);
    _SREAD(api_name);
    _ASREAD(api_description);
    _IREAD(api_price);

    return bParseRet;

}


bool kcsapi_ndock::ReadFromJObj(const QJsonObject &jobj)
{
    _IREAD(api_member_id);
    _IREAD(api_id);
    _IREAD(api_state);
    _IREAD(api_ship_id);
    _LREAD(api_complete_time);
    _SREAD(api_complete_time_str);
    _IREAD(api_item1);
    _IREAD(api_item2);
    _IREAD(api_item3);
    _IREAD(api_item4);

    return bParseRet;

}


bool kcsapi_port::ReadFromJObj(const QJsonObject &jobj)
{
    _ACREAD(api_material, kcsapi_material);
    _ACREAD(api_deck_port, kcsapi_deck);
    _ACREAD(api_ndock, kcsapi_ndock);
    _ACREAD(api_ship, kcsapi_ship2);
    _CREAD(api_basic, kcsapi_basic);

    return bParseRet;

}


bool kcsapi_powerup::ReadFromJObj(const QJsonObject &jobj)
{
    _IREAD(api_powerup_flag);
    _CREAD(api_ship, kcsapi_ship2);
    _ACREAD(api_deck, kcsapi_deck);

    return bParseRet;

}


bool kcsapi_quest::ReadFromJObj(const QJsonObject &jobj)
{
    _IREAD(api_no);
    _IREAD(api_category);
    _IREAD(api_type);
    _IREAD(api_state);
    _SREAD(api_title);
    _SREAD(api_detail);
    _AIREAD(api_get_material);
    _IREAD(api_bonus_flag);
    _IREAD(api_progress_flag);

    return bParseRet;

}


bool kcsapi_questlist::ReadFromJObj(const QJsonObject &jobj)
{
    _IREAD(api_count);
    _IREAD(api_page_count);
    _IREAD(api_disp_page);
    _ACREAD(api_list, kcsapi_quest);
    _IREAD(api_exec_count);

    return bParseRet;

}


bool kcsapi_ship::ReadFromJObj(const QJsonObject &jobj)
{
    _IREAD(api_member_id);
    _IREAD(api_id);
    _IREAD(api_sortno);
    _SREAD(api_name);
    _SREAD(api_yomi);
    _IREAD(api_stype);
    _IREAD(api_ship_id);
    _IREAD(api_lv);
    _IREAD(api_exp);
    _IREAD(api_afterlv);
    _IREAD(api_aftershipid);
    _IREAD(api_nowhp);
    _IREAD(api_maxhp);
    _AIREAD(api_taik);
    _AIREAD(api_souk);
    _AIREAD(api_houg);
    _AIREAD(api_raig);
    _AIREAD(api_baku);
    _AIREAD(api_tyku);
    _AIREAD(api_houm);
    _AIREAD(api_raim);
    _AIREAD(api_saku);
    _AIREAD(api_luck);
    _IREAD(api_soku);
    _IREAD(api_leng);
    _AIREAD(api_slot);
    _AIREAD(api_onslot);
    _AIREAD(api_broken);
    _AIREAD(api_powup);
    _AIREAD(api_kyouka);
    _IREAD(api_backs);
    _IREAD(api_fuel);
    _IREAD(api_fuel_max);
    _IREAD(api_bull);
    _IREAD(api_bull_max);
    _SREAD(api_gomes);    //o
    _SREAD(api_gomes2);   //o
    _IREAD(api_slotnum);
    _IREAD(api_ndock_time);
    _SREAD(api_ndock_time_str);
    _AIREAD(api_ndock_item);
    _IREAD(api_star);
    _IREAD(api_srate);
    _IREAD(api_cond);
    _AIREAD(api_karyoku);
    _AIREAD(api_raisou);
    _AIREAD(api_taiku);
    _AIREAD(api_soukou);
    _AIREAD(api_kaihi);
    _AIREAD(api_taisen);
    _AIREAD(api_sakuteki);
    _AIREAD(api_lucky);
    _IREAD(api_use_fuel);
    _IREAD(api_use_bull);
    _IREAD(api_voicef);

    return bParseRet;

}


bool kcsapi_ship3::ReadFromJObj(const QJsonObject &jobj)
{
    _ACREAD(api_ship_data, kcsapi_ship2);
    _ACREAD(api_deck_data, kcsapi_deck);

    return bParseRet;

}


bool kcsapi_slot_data::ReadFromJObj(const QJsonObject &jobj)
{

    _AIREAD(api_slottype1);
    _AIREAD(api_slottype2);
    _AIREAD(api_slottype3);
    _AIREAD(api_slottype4);
    _AIREAD(api_slottype5);
    _AIREAD(api_slottype6);
    _AIREAD(api_slottype7);
    _AIREAD(api_slottype8);
    _AIREAD(api_slottype9);
    _AIREAD(api_slottype10);
    _AIREAD(api_slottype11);
    _AIREAD(api_slottype12);
    _AIREAD(api_slottype13);
    _AIREAD(api_slottype14);
    _AIREAD(api_slottype15);
    _AIREAD(api_slottype16);
    _AIREAD(api_slottype17);
    _AIREAD(api_slottype18);
    _AIREAD(api_slottype19);
    _AIREAD(api_slottype20);
    _AIREAD(api_slottype21);
    _AIREAD(api_slottype22);
    _AIREAD(api_slottype23);

    return bParseRet;
}


bool kcsapi_start2::ReadFromJObj(const QJsonObject &jobj)
{
    _ACREAD(api_mst_ship, kcsapi_mst_ship);
    _ACREAD(api_mst_slotitem, kcsapi_mst_slotitem);
    _ACREAD(api_mst_useitem, kcsapi_mst_useitem);
    _ACREAD(api_mst_stype, kcsapi_mst_stype);
    _ACREAD(api_mst_slotitem_equiptype, kcsapi_mst_slotitem_equiptype);

    return bParseRet;


}


bool kcsapi_useitem::ReadFromJObj(const QJsonObject &jobj)
{
    _IREAD(api_member_id);
    _IREAD(api_id);
    _IREAD(api_value);
    _IREAD(api_usetype);
    _IREAD(api_category);
    _SREAD(api_name);
    _ASREAD(api_description);
    _IREAD(api_price);
    _IREAD(api_count);

    return bParseRet;

}


void KAPIBaseData::UpdateSaveData(QString name, KAPIBaseData *pdata)
{
    KanSaveData::getInstance().UpdateDataFromResponse(name, pdata);
}
