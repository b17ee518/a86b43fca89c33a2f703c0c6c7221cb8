#include "kandata.h"
#include "kansavedata.h"

#define _SREAD(name)    name=jobj[#name].toString();
#define _IREAD(name)    name=jobj[#name].toInt();
#define _LREAD(name)    name=(qint64)jobj[#name].toDouble();
#define _FREAD(name)    name=(float)jobj[#name].toDouble();

#define _SIREAD(name)\
	if (jobj[#name].isString())\
	{\
		##name##_s = jobj[#name].toString();\
		name = ##name##_s.toInt();\
	}\
	else\
	{\
		_IREAD(name);\
	}

#define _AIREAD(name)   \
	jarray=jobj[#name].toArray();\
	name.clear();\
	foreach(const QJsonValue &v, jarray)\
	{\
		int val = 0;\
		if (v.isString())\
		{\
			val = v.toString().toInt();\
		}\
		else\
		{\
			val = v.toInt();\
		}\
		name.append(val);\
	}
#define _AFREAD(name)   \
	jarray=jobj[#name].toArray();\
	name.clear();\
	foreach(const QJsonValue &v, jarray)\
	{\
		name.append(v.toDouble());\
	}

#define _AAIREAD(name) \
	jarray = jobj[#name].toArray();\
	name.clear();\
	foreach(const QJsonValue &v, jarray)\
	{\
	if (v.isArray())\
		{\
		QJsonArray jt = v.toArray();\
		QList<int> lt;\
		foreach(const QJsonValue &vt, jt)\
			{\
			lt.append(vt.toInt());\
			}\
			name.append(lt);\
		}\
		else\
		{\
		QList<int> lt;\
		lt.append(v.toInt());\
		name.append(lt);\
		}\
	}
#define _AAFREAD(name) \
	jarray = jobj[#name].toArray();\
	name.clear();\
	foreach(const QJsonValue &v, jarray)\
	{\
	if (v.isArray())\
		{\
		QJsonArray jt = v.toArray();\
		QList<float> lt;\
		foreach(const QJsonValue &vt, jt)\
			{\
			lt.append(vt.toDouble());\
			}\
			name.append(lt);\
		}\
		else\
		{\
		QList<float> lt;\
		lt.append(v.toDouble());\
		name.append(lt);\
		}\
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
		name.append((qint64)v.toDouble());\
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
	name.ReadFromJObj(jobj[#name].toObject());


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
		if (v.isArray())
		{
			QJsonArray jt = v.toArray();
			QList<int> lt;
			foreach(const QJsonValue &vt, jt)
			{
				lt.append(vt.toInt());
			}
			api_furniture.append(lt);
		}
		else
		{
			QList<int> lt;
			lt.append(v.toInt());
			api_furniture.append(lt);
		}
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

	return bParseRet;
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

bool kcsapi_battleresult_escape::ReadFromJObj(const QJsonObject &jobj)
{
	_AIREAD(api_escape_idx);
	_AIREAD(api_tow_idx);

	return bParseRet;
}


bool kcsapi_battleresult::ReadFromJObj(const QJsonObject &jobj)
{
	_AIREAD(api_ship_id);
	_SREAD(api_win_rank);
	_IREAD(api_get_exp);
	_IREAD(api_mvp);
	// combined
	_IREAD(api_mvp_combined);
	_IREAD(api_member_lv);
	_IREAD(api_member_exp);
	_IREAD(api_get_base_exp);
	_AIREAD(api_get_ship_exp);
	_AAIREAD(api_get_exp_lvup);
	// combined
	_AIREAD(api_get_ship_exp_combined);
	_AAIREAD(api_get_exp_lvup_combined);
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
	_IREAD(api_escape_flag);
	_CREAD(api_escape, kcsapi_battleresult_escape);

	return bParseRet;

}


bool kcsapi_slotitem::ReadFromJObj(const QJsonObject &jobj)
{
	_IREAD(api_id);
	_IREAD(api_slotitem_id);
	_IREAD(api_locked);
	_IREAD(api_level);
	_IREAD(api_alv);

	return bParseRet;

}

bool kcsapi_slotitem::ReadFromSlotItem(const kcsapi_slotitem& slotitem)
{
	if (slotitem.api_id == this->api_id)
	{
		this->api_slotitem_id = slotitem.api_slotitem_id;
		this->api_locked = slotitem.api_locked;
		this->api_level = slotitem.api_level;
		this->api_alv = slotitem.api_alv;
		return true;
	}
	return false;
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
	_IREAD(api_locked_equip);
	_IREAD(api_sally_area);
	_IREAD(api_slot_ex);

	return bParseRet;

}

bool kcsapi_ship2::ReadFromShip(const kcsapi_ship &ship)
{
	if (ship.api_id == api_id)
	{
		api_id = ship.api_id;
		api_sortno = ship.api_sortno;
		api_ship_id = ship.api_ship_id;
		api_lv = ship.api_ship_id;
//        api_exp = ship.api_exp;
		api_nowhp = ship.api_nowhp;
		api_maxhp = ship.api_maxhp;
		api_leng = ship.api_leng;
		api_slot = ship.api_slot;
		api_onslot = ship.api_onslot;
		api_kyouka = ship.api_kyouka;
		api_backs = ship.api_backs;
		api_fuel = ship.api_fuel;
		api_bull = ship.api_bull;
		api_slotnum = ship.api_slotnum;
		api_ndock_time = ship.api_ndock_time;
		api_ndock_item = ship.api_ndock_item;
		api_srate = ship.api_srate;
		api_cond = ship.api_cond;
		api_karyoku = ship.api_karyoku;
		api_raisou = ship.api_raisou;
		api_taiku = ship.api_taiku;
		api_soukou = ship.api_soukou;
		api_kaihi = ship.api_kaihi;
		api_taisen = ship.api_taisen;
		api_sakuteki = ship.api_sakuteki;
		api_lucky = ship.api_lucky;
		return true;
	}
	return false;
}

bool kcsapi_ship2::ReadFromMstShip(const kcsapi_mst_ship& mstship, int id)
{
	api_id = id;
	api_sortno = mstship.api_sortno;
	api_ship_id = mstship.api_id;
	api_lv = 1;

	api_exp.clear();
	api_exp.push_back(0); // total exp
	api_exp.push_back(100); // next level exp
	api_exp.push_back(0); // ?

	api_nowhp = mstship.api_taik.first();
	api_maxhp = api_nowhp;
	api_leng = mstship.api_leng;

	//add slotitem later
	api_slot.clear();
	api_slot.push_back(-1);
	api_slot.push_back(-1);
	api_slot.push_back(-1);
	api_slot.push_back(-1);
	api_slot.push_back(-1);

	api_onslot.clear();
	api_onslot.push_back(0);
	api_onslot.push_back(0);
	api_onslot.push_back(0);
	api_onslot.push_back(0);
	api_onslot.push_back(0);

	api_kyouka.clear();
	api_kyouka.push_back(0);
	api_kyouka.push_back(0);
	api_kyouka.push_back(0);
	api_kyouka.push_back(0);
	api_kyouka.push_back(0);

	api_backs = mstship.api_backs;
	api_fuel = mstship.api_fuel_max;
	api_bull = mstship.api_bull_max;
	api_slotnum = mstship.api_slot_num;
	api_ndock_time = 0;

	api_ndock_item.clear();
	api_ndock_item.push_back(0);
	api_ndock_item.push_back(0);

	api_srate = api_backs; //?
	api_cond = 40;

	// no data
	api_karyoku.clear();
	api_karyoku.push_back(0);
	api_karyoku.push_back(0);
	api_raisou.clear();
	api_raisou.push_back(0);
	api_raisou.push_back(0);
	api_taiku.clear();
	api_taiku.push_back(0);
	api_taiku.push_back(0);
	api_soukou.clear();
	api_soukou.push_back(0);
	api_soukou.push_back(0);
	api_kaihi.clear();
	api_kaihi.push_back(0);
	api_kaihi.push_back(0);
	api_taisen.clear();
	api_taisen.push_back(0);
	api_taisen.push_back(0);
	api_sakuteki.clear();
	api_sakuteki.push_back(0);
	api_sakuteki.push_back(0);
	api_lucky.clear();
	api_lucky.push_back(0);
	api_lucky.push_back(0);

	api_locked = 0;
	api_locked_equip = 0;
	api_sally_area = 0;
	api_slot_ex = -1;

	return false;
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

bool kcsapi_ship_deck::ReadFromJObj(const QJsonObject &jobj)
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
	_AIREAD(api_slottype24);
	_AIREAD(api_slottype25);
	_AIREAD(api_slottype26);
	_AIREAD(api_slottype27);
	_AIREAD(api_slottype28);
	_AIREAD(api_slottype29);
	_AIREAD(api_slottype30);
	_AIREAD(api_slottype31);
	_AIREAD(api_slottype32);
	_AIREAD(api_slottype33);
	_AIREAD(api_slottype34);
	_AIREAD(api_slottype35);
	_AIREAD(api_slottype36);

	return bParseRet;
}


bool kcsapi_start2::ReadFromJObj(const QJsonObject &jobj)
{

	_ACREAD(api_mst_ship, kcsapi_mst_ship);
	_ACREAD(api_mst_slotitem, kcsapi_mst_slotitem);
	_ACREAD(api_mst_useitem, kcsapi_mst_useitem);
	_ACREAD(api_mst_stype, kcsapi_mst_stype);
	_ACREAD(api_mst_slotitem_equiptype, kcsapi_mst_slotitem_equiptype);

	_ACREAD(api_mst_shipgraph, Api_Mst_Shipgraph);
	_ACREAD(api_mst_slotitemgraph, Api_Mst_Slotitemgraph);
	_ACREAD(api_mst_furniture, Api_Mst_Furniture);
	_ACREAD(api_mst_furnituregraph, Api_Mst_Furnituregraph);
	_ACREAD(api_mst_payitem, Api_Mst_Payitem);
	_CREAD(api_mst_item_shop, Api_Mst_Item_Shop);
	_ACREAD(api_mst_maparea, Api_Mst_Maparea);
	_ACREAD(api_mst_mapinfo, Api_Mst_Mapinfo);
	_ACREAD(api_mst_mapbgm, Api_Mst_Mapbgm);
	_ACREAD(api_mst_mapcell, Api_Mst_Mapcell);
	_ACREAD(api_mst_mission, Api_Mst_Mission);
	_CREAD(api_mst_const, Api_Mst_Const);
	_ACREAD(api_mst_shipupgrade, Api_Mst_Shipupgrade);
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


bool Api_Mst_Ship::ReadFromJObj(const QJsonObject &jobj)
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
	_SREAD(api_homemes); //o
	_SREAD(api_gomes); //o
	_SREAD(api_gomes2); //o
	_SREAD(api_sinfo);
	_IREAD(api_afterfuel);
	_IREAD(api_afterbull);
	_ASREAD(api_touchs);     //o
	_SREAD(api_missions); //o
	_SREAD(api_systems); //o
	_IREAD(api_fuel_max);
	_IREAD(api_bull_max);
	_IREAD(api_voicef);
	return bParseRet;
}


bool Api_Mst_Shipgraph::ReadFromJObj(const QJsonObject &jobj)
{
	_IREAD(api_id);
	_IREAD(api_sortno);
	_SREAD(api_filename);
	_SREAD(api_version);
	_AIREAD(api_boko_n);
	_AIREAD(api_boko_d);
	_AIREAD(api_kaisyu_n);
	_AIREAD(api_kaisyu_d);
	_AIREAD(api_kaizo_n);
	_AIREAD(api_kaizo_d);
	_AIREAD(api_map_n);
	_AIREAD(api_map_d);
	_AIREAD(api_ensyuf_n);
	_AIREAD(api_ensyuf_d);
	_AIREAD(api_ensyue_n);
	_AIREAD(api_battle_n);
	_AIREAD(api_battle_d);
	_AIREAD(api_weda);
	_AIREAD(api_wedb);

	return bParseRet;
}


bool Api_Equip_Type::ReadFromJObj(const QJsonObject &jobj)
{
	_IREAD(_1);
	_IREAD(_2);
	_IREAD(_3);
	_IREAD(_4);
	_IREAD(_5);
	_IREAD(_6);
	_IREAD(_7);
	_IREAD(_8);
	_IREAD(_9);
	_IREAD(_10);
	_IREAD(_11);
	_IREAD(_12);
	_IREAD(_13);
	_IREAD(_14);
	_IREAD(_15);
	_IREAD(_16);
	_IREAD(_17);
	_IREAD(_18);
	_IREAD(_19);
	_IREAD(_20);
	_IREAD(_21);
	_IREAD(_22);
	_IREAD(_23);
	_IREAD(_24);
	_IREAD(_25);
	_IREAD(_26);
	_IREAD(_27);
	_IREAD(_28);
	_IREAD(_29);
	_IREAD(_30);
	_IREAD(_31);

	return bParseRet;
}


bool Api_Mst_Stype::ReadFromJObj(const QJsonObject &jobj)
{

	_IREAD(api_id);
	_IREAD(api_sortno);
	_SREAD(api_name);
	_IREAD(api_scnt);
	_IREAD(api_kcnt);
	_CREAD(api_equip_type, Api_Equip_Type);
	return bParseRet;
}


bool Api_Mst_Slotitem::ReadFromJObj(const QJsonObject &jobj)
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


bool Api_Mst_Slotitemgraph::ReadFromJObj(const QJsonObject &jobj)
{

	_IREAD(api_id);
	_IREAD(api_sortno);
	_SREAD(api_filename);
	_SREAD(api_version);
	return bParseRet;
}


bool Api_Mst_Furniture::ReadFromJObj(const QJsonObject &jobj)
{

	_IREAD(api_id);
	_IREAD(api_type);
	_IREAD(api_no);
	_SREAD(api_title);
	_SREAD(api_description);
	_IREAD(api_rarity);
	_IREAD(api_price);
	_IREAD(api_saleflg);
	return bParseRet;
}


bool Api_Mst_Furnituregraph::ReadFromJObj(const QJsonObject &jobj)
{

	_IREAD(api_id);
	_IREAD(api_type);
	_IREAD(api_no);
	_SREAD(api_filename);
	_SREAD(api_version);
	return bParseRet;
}


bool Api_Mst_Useitem::ReadFromJObj(const QJsonObject &jobj)
{

	_IREAD(api_id);
	_IREAD(api_usetype);
	_IREAD(api_category);
	_SREAD(api_name);
	_ASREAD(api_description);
	_IREAD(api_price);
	return bParseRet;
}


bool Api_Mst_Payitem::ReadFromJObj(const QJsonObject &jobj)
{

	_IREAD(api_id);
	_IREAD(api_type);
	_SREAD(api_name);
	_SREAD(api_description);
	_AIREAD(api_item);
	_IREAD(api_price);
	return bParseRet;
}


bool Api_Mst_Maparea::ReadFromJObj(const QJsonObject &jobj)
{

	_IREAD(api_id);
	_SREAD(api_name);
	_IREAD(api_type);
	return bParseRet;
}


bool Api_Mst_Mapinfo::ReadFromJObj(const QJsonObject &jobj)
{

	_IREAD(api_id);
	_IREAD(api_maparea_id);
	_IREAD(api_no);
	_SREAD(api_name);
	_IREAD(api_level);
	_SREAD(api_opetext);
	_SREAD(api_infotext);
	_AIREAD(api_item);
	_IREAD(api_max_maphp);
	_IREAD(api_required_defeat_count);
	return bParseRet;
}


bool Api_Mst_Mapbgm::ReadFromJObj(const QJsonObject &jobj)
{

	_IREAD(api_id);
	_IREAD(api_maparea_id);
	_IREAD(api_no);
	_AIREAD(api_map_bgm);
	_AIREAD(api_boss_bgm);
	return bParseRet;
}


bool Api_Mst_Mapcell::ReadFromJObj(const QJsonObject &jobj)
{

	_IREAD(api_map_no);
	_IREAD(api_maparea_id);
	_IREAD(api_mapinfo_no);
	_IREAD(api_id);
	_IREAD(api_no);
	_IREAD(api_color_no);
	return bParseRet;
}


bool Api_Mst_Mission::ReadFromJObj(const QJsonObject &jobj)
{

	_IREAD(api_id);
	_IREAD(api_maparea_id);
	_SREAD(api_name);
	_SREAD(api_details);
	_IREAD(api_time);
	_IREAD(api_difficulty);
	_FREAD(api_use_fuel);
	_FREAD(api_use_bull);
	_AIREAD(api_win_item1);
	_AIREAD(api_win_item2);
	return bParseRet;
}


bool Api_Mst_Shipupgrade::ReadFromJObj(const QJsonObject &jobj)
{

	_IREAD(api_id);
	_IREAD(api_original_ship_id);
	_IREAD(api_upgrade_type);
	_IREAD(api_upgrade_level);
	_IREAD(api_drawing_count);
	_IREAD(api_sortno);
	return bParseRet;
}


bool Api_Mst_Item_Shop::ReadFromJObj(const QJsonObject &jobj)
{

	_AIREAD(api_cabinet_1);
	_AIREAD(api_cabinet_2);
	return bParseRet;
}


bool Api_Mst_Const::ReadFromJObj(const QJsonObject &jobj)
{

	_CREAD(api_boko_max_ships, Api_Boko_Max_Ships);
	return bParseRet;
}


bool Api_Boko_Max_Ships::ReadFromJObj(const QJsonObject &jobj)
{

	_SREAD(api_string_value);
	_IREAD(api_int_value);
	return bParseRet;
}


bool kcsapi_next_enemy::ReadFromJObj(const QJsonObject &jobj)
{
	_IREAD(api_enemy_id);
	_IREAD(api_result);
	_SREAD(api_result_str);
	return bParseRet;
}


bool kcsapi_next::ReadFromJObj(const QJsonObject &jobj)
{

	_IREAD(api_rashin_flg);
	_IREAD(api_rashin_id);
	_IREAD(api_maparea_id);
	_IREAD(api_mapinfo_no);
	_IREAD(api_no);
	_IREAD(api_color_no);
	_IREAD(api_event_id);
	_IREAD(api_event_kind);
	_IREAD(api_next);
	_IREAD(api_bosscell_no);
	_IREAD(api_bosscomp);
//    int api_comment_kind;
//    int api_production_kind;
	_CREAD(api_enemy, kcsapi_next_enemy);
	return bParseRet;
}


bool kcsapi_mission_start::ReadFromJObj(const QJsonObject &jobj)
{
	_LREAD(api_complatetime);
	_SREAD(api_complatetime_str);
	return bParseRet;
}

bool kcsapi_battle_kouku_stage1::ReadFromJObj(const QJsonObject &jobj)
{
	_IREAD(api_f_count);
	_IREAD(api_f_lostcount);
	_IREAD(api_e_count);
	_IREAD(api_e_lostcount);
	_IREAD(api_disp_seiku);
	_AIREAD(api_touch_plane);
	return bParseRet;
}

bool kcsapi_battle_kouku_stage2::ReadFromJObj(const QJsonObject &jobj)
{
	_IREAD(api_f_count);
	_IREAD(api_f_lostcount);
	_IREAD(api_e_count);
	_IREAD(api_e_lostcount);
	return bParseRet;
}

bool kcsapi_battle_kouku_stage3::ReadFromJObj(const QJsonObject &jobj)
{
	_AIREAD(api_frai_flag);
	_AIREAD(api_erai_flag);
	_AIREAD(api_fbak_flag);
	_AIREAD(api_ebak_flag);
	_AIREAD(api_fcl_flag);
	_AIREAD(api_ecl_flag);
	_AFREAD(api_fdam);
	_AFREAD(api_edam);

	return bParseRet;
}

bool kcsapi_battle_kouku::ReadFromJObj(const QJsonObject &jobj)
{
	_AAIREAD(api_plane_from); //? f, e list
	_CREAD(api_stage1, kcsapi_battle_kouku_stage1);
	_CREAD(api_stage2, kcsapi_battle_kouku_stage2);
	_CREAD(api_stage3, kcsapi_battle_kouku_stage3);
	// combined
	_CREAD(api_stage3_combined, kcsapi_battle_kouku_stage3);

	return bParseRet;
}

bool kcsapi_battle_support_info::ReadFromJObj(const QJsonObject &jobj)
{
	_CREAD(api_support_airatack, kcsapi_battle_support_airatack);
	_CREAD(api_support_hourai, kcsapi_battle_support_hourai);

	return bParseRet;
}

bool kcsapi_battle_opening_atack::ReadFromJObj(const QJsonObject &jobj)
{
	_AIREAD(api_frai);
	_AIREAD(api_erai);
	_AFREAD(api_fdam);
	_AFREAD(api_edam);
	_AFREAD(api_fydam);
	_AFREAD(api_eydam);
	_AIREAD(api_fcl);
	_AIREAD(api_ecl);

	return bParseRet;
}

bool kcsapi_battle_hougeki::ReadFromJObj(const QJsonObject &jobj)
{
	_AIREAD(api_at_list);
	_AIREAD(api_at_type);
	_AAIREAD(api_df_list);
	_AAIREAD(api_si_list);
	_AAIREAD(api_cl_list);

	// midnight
	_AIREAD(api_sp_list);

	_AAFREAD(api_damage);

	return bParseRet;
}

bool kcsapi_battle_raigeki::ReadFromJObj(const QJsonObject &jobj)
{
	_AIREAD(api_frai);
	_AIREAD(api_erai);
	_AFREAD(api_fdam);
	_AFREAD(api_edam);
	_AFREAD(api_fydam);
	_AFREAD(api_eydam);
	_AIREAD(api_fcl);
	_AIREAD(api_ecl);

	return bParseRet;
}

bool kcsapi_battle::ReadFromJObj(const QJsonObject &jobj)
{
	_IREAD(api_dock_id);
	_SIREAD(api_deck_id);
	_AIREAD(api_ship_ke); //enemy ship list from #1
	_AIREAD(api_ship_lv); //enemy ship lv from #1
	_AIREAD(api_nowhps); // both hps from #1 (13)
	_AIREAD(api_maxhps);

	// combined
	_AIREAD(api_nowhps_combined);
	_AIREAD(api_maxhps_combined);

	_IREAD(api_midnight_flag);
	_AAIREAD(api_eSlot); // from #0
	_AAIREAD(api_eKyouka); // from #0
	_AAIREAD(api_fParam); // from $0
	// combined
	_AAIREAD(api_fParam_combined);
	_AAIREAD(api_eParam); // from $0

	_AIREAD(api_escape_idx);
	_AIREAD(api_escape_idx_combined);

	_AIREAD(api_search); //?
	_AIREAD(api_formation); // f, e, i { null, "単縦陣", "複縦陣", "輪形陣", "梯形陣", "単横陣" } { null, "同航戦", "反航戦", "Ｔ字戦(有利)", "Ｔ字戦(不利)" }
	_AIREAD(api_stage_flag); // ??kouku, ??support/opening?, hougeki
	_AIREAD(api_stage_flag2);

	// midnight
	_AIREAD(api_touch_plane);
	_AIREAD(api_flare_pos);

	_CREAD(api_kouku, kcsapi_battle_kouku);

	// combined
	_CREAD(api_kouku2, kcsapi_battle_kouku);

	_IREAD(api_support_flag);
	_CREAD(api_support_info, kcsapi_battle_support_info);
	_IREAD(api_opening_flag);
	_CREAD(api_opening_atack, kcsapi_battle_opening_atack);
	_AIREAD(api_hourai_flag); // hougeki1, hougeki2, hougeki3, raigeki
	_CREAD(api_hougeki1, kcsapi_battle_hougeki);
	_CREAD(api_hougeki2, kcsapi_battle_hougeki);
	_CREAD(api_hougeki3, kcsapi_battle_hougeki);
	_CREAD(api_raigeki, kcsapi_battle_raigeki);

	// midnight
	_CREAD(api_hougeki, kcsapi_battle_hougeki); //midnight
	
	return bParseRet;
}

bool kcsapi_battle_support_airatack_stage1::ReadFromJObj(const QJsonObject &jobj)
{
	_IREAD(api_f_count);
	_IREAD(api_f_lostcount);
	_IREAD(api_e_count);
	_IREAD(api_e_lostcount);

	return bParseRet;
}

bool kcsapi_battle_support_airatack_stage2::ReadFromJObj(const QJsonObject &jobj)
{
	_IREAD(api_f_count);
	_IREAD(api_f_lostcount);

	return bParseRet;
}

bool kcsapi_battle_support_airatack_stage3::ReadFromJObj(const QJsonObject &jobj)
{
	_AIREAD(api_erai_flag);
	_AIREAD(api_ebak_flag);
	_AIREAD(api_ecl_flag);
	_AFREAD(api_edam);

	return bParseRet;
}

bool kcsapi_battle_support_airatack::ReadFromJObj(const QJsonObject &jobj)
{
	_IREAD(api_deck_id);
	_AIREAD(api_ship_id);
	_AIREAD(api_undressing_flag); //??
	_AIREAD(api_stage_flag);
	_AAIREAD(api_plane_from);
	_CREAD(api_stage1, kcsapi_battle_support_airatack_stage1);
	_CREAD(api_stage2, kcsapi_battle_support_airatack_stage2);
	_CREAD(api_stage3, kcsapi_battle_support_airatack_stage3);

	return bParseRet;
}


bool kcsapi_battle_support_hourai::ReadFromJObj(const QJsonObject &jobj)
{
	_IREAD(api_deck_id);
	_AIREAD(api_ship_id);
	_AIREAD(api_undressing_flag);
	_AIREAD(api_cl_list);
	_AFREAD(api_damage);

	return bParseRet;
}

bool kcsapi_clearitemget_bounus_item::ReadFromJObj(const QJsonObject &jobj)
{
	_IREAD(api_id);
	_SREAD(api_name);

	return bParseRet;
}

bool kcsapi_clearitemget_bounus::ReadFromJObj(const QJsonObject &jobj)
{
	_IREAD(api_count);
	_IREAD(api_type);
	// check
	_CREAD(api_item, kcsapi_clearitemget_bounus_item);

	return bParseRet;
}

bool kcsapi_clearitemget::ReadFromJObj(const QJsonObject &jobj)
{
	_AIREAD(api_material);
	_IREAD(api_bounus_count);
	_ACREAD(api_bounus, kcsapi_clearitemget_bounus);

	return bParseRet;
}

bool kcsapi_practice_enemyinfo_deck_ships::ReadFromJObj(const QJsonObject &jobj)
{
	_IREAD(api_id);
	_IREAD(api_shipid);
	_IREAD(api_level);
	_IREAD(api_star);
	return bParseRet;
}

bool kcsapi_practice_enemyinfo_deck::ReadFromJObj(const QJsonObject &jobj)
{
	_ACREAD(api_ships, kcsapi_practice_enemyinfo_deck_ships);
	return bParseRet;
}

bool kcsapi_practice_enemyinfo::ReadFromJObj(const QJsonObject &jobj)
{
	_IREAD(api_member_id);
	_SREAD(api_nickname);
	_SREAD(api_nickname_id);
	_SREAD(api_cmt);
	_SREAD(api_cmt_id);
	_IREAD(api_level);
	_IREAD(api_rank);
	_AIREAD(api_experience);
	_IREAD(api_friend);
	_AIREAD(api_ship);
	_AIREAD(api_slotitem);
	_IREAD(api_furniture);
	_SREAD(api_deckname);
	_SREAD(api_deckname_id);
	_CREAD(api_deck, kcsapi_practice_enemyinfo_deck);

	return bParseRet;
}

bool kcsapi_hensei_lock::ReadFromJObj(const QJsonObject &jobj)
{
	_IREAD(api_locked);

	return bParseRet;
}

bool kcsapi_kaisou_lock::ReadFromJObj(const QJsonObject &jobj)
{
	_IREAD(api_locked);

	return bParseRet;
}

bool kcsapi_remodel_slot::ReadFromJObj(const QJsonObject &jobj)
{
	_IREAD(api_remodel_flag);
	_AIREAD(api_remodel_id);
	_AIREAD(api_after_material);
	_IREAD(api_voice_id);
	_CREAD(api_after_slot, kcsapi_slotitem);

	return bParseRet;
}