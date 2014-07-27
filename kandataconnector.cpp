﻿#include "kandataconnector.h"
#include "kandata.h"
#include "kansavedata.h"
#include "kandatacalc.h"
#include <QJsonDocument>
#include <QJsonObject>

#include "infomainwindow.h"
#include "mainwindow.h"
#include "kqtime.h"
#include "klog.h"

/*
 * End points
	"/kcsapi/api_start2",
	"/kcsapi/api_port/port",
	"/kcsapi/api_get_member/basic",
	"/kcsapi/api_get_member/ship",  //?
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

bool questDataSort(const kcsapi_quest &left, const kcsapi_quest &right)
{
	return left.api_no < right.api_no;
}

void KanDataConnector::Parse(QString pathAndQuery, QString requestBody, QString responseBody)
{
#define DAPILOG() APILOG(pathAndQuery, requestBody, responseBody)
	// svdata=
	responseBody = responseBody.right(responseBody.length()-7);
	QJsonDocument jdoc = QJsonDocument::fromJson(responseBody.toLocal8Bit());
	QJsonObject jobj = jdoc.object()["api_data"].toObject();

	KanReqData req;
	req.ReadFromString(pathAndQuery, requestBody);

	KanSaveData * pksd = &KanSaveData::getInstance();

	if (pathAndQuery == "/kcsapi/api_start2")
	{
		pksd->start2data.ReadFromJObj(jobj);
		// mst table no update
	}
	else if (pathAndQuery == "/kcsapi/api_port/port")
	{
		pksd->portdata.ReadFromJObj(jobj);
		pksd->shipcountoffset = 0;
		// material, deck, ndock, ship2, basic
		updateOverviewTable();
		updateMissionTable();
		updateFleetTable();
		updateRepairTable();

		updateRepairDockTable();
		updateExpeditionTable();

		pksd->nextdata.api_no = -1;
		updateInfoTitle();
	}
	else if (pathAndQuery == "/kcsapi/api_get_member/basic")
	{
		pksd->portdata.api_basic.ReadFromJObj(jobj);
		updateOverviewTable();
	}
	else if (pathAndQuery == "/kcsapi/api_get_member/ship")
	{
		kcsapi_ship api_ship;
		api_ship.ReadFromJObj(jobj);
		kcsapi_ship2 * pship = findShipFromShipno(api_ship.api_id);
		if (pship)
		{
			pship->ReadFromShip(api_ship);
			updateFleetTable();
			updateRepairTable();
		}
	}
	else if (pathAndQuery == "/kcsapi/api_get_member/ship2")
	{
		pksd->portdata.api_ship.clear();
		QJsonArray jarray = jdoc.object()["api_data"].toArray();
		for (int i=0; i<jarray.count(); i++)
		{
			kcsapi_ship2 api_ship2item;
			api_ship2item.ReadFromJObj(jarray[i].toObject());
			pksd->portdata.api_ship.append(api_ship2item);
		}

		pksd->shipcountoffset = 0;

		updateOverviewTable();
		updateFleetTable();
		updateRepairTable();
	}
	else if (pathAndQuery == "/kcsapi/api_get_member/ship3")
	{
		kcsapi_ship3 api_ship3;
		api_ship3.ReadFromJObj(jobj);

//        pksd->portdata.api_ship = api_ship3.api_ship_data;
		foreach(const kcsapi_ship2 &v, api_ship3.api_ship_data)
		{
			QList<kcsapi_ship2>::iterator it;
			for (it=pksd->portdata.api_ship.begin(); it!=pksd->portdata.api_ship.end(); ++it)
			{
				if (v.api_id == it->api_id)
				{
					(*it) = v;
				}
			}
		}


		pksd->portdata.api_deck_port = api_ship3.api_deck_data;
		updateOverviewTable();
		updateFleetTable();
		updateRepairTable();

		pksd->shipcountoffset = 0;
		updateExpeditionTable();
	}
	else if (pathAndQuery == "/kcsapi/api_get_member/slot_item")
	{
		pksd->slotitemdata.clear();
		QJsonArray jarray = jdoc.object()["api_data"].toArray();
		for (int i=0; i<jarray.count(); i++)
		{
			kcsapi_slotitem api_slotitem;
			api_slotitem.ReadFromJObj(jarray[i].toObject());
			pksd->slotitemdata.append(api_slotitem);
		}

		pksd->slotitemcountoffset = 0;

		updateOverviewTable();
		//TODO: update tootip?
	}
	else if (pathAndQuery == "/kcsapi/api_get_member/useitem")
	{
	}
	else if (pathAndQuery == "/kcsapi/api_get_member/deck" || pathAndQuery == "/kcsapi/api_get_member/deck_port")
	{
		pksd->portdata.api_deck_port.clear();
		QJsonArray jarray = jdoc.object()["api_data"].toArray();
		for (int i=0; i<jarray.count(); i++)
		{
			kcsapi_deck api_deck;
			api_deck.ReadFromJObj(jarray[i].toObject());
			pksd->portdata.api_deck_port.append(api_deck);
		}
		updateOverviewTable();
		updateFleetTable();
		updateRepairTable();
	}
	else if (pathAndQuery == "/kcsapi/api_get_member/ndock")
	{
		pksd->portdata.api_ndock.clear();
		QJsonArray jarray = jdoc.object()["api_data"].toArray();
		for (int i=0; i<jarray.count(); i++)
		{
			kcsapi_ndock api_ndock;
			api_ndock.ReadFromJObj(jarray[i].toObject());
			pksd->portdata.api_ndock.append(api_ndock);
		}

		updateOverviewTable();
		updateFleetTable();
		updateRepairTable();

		updateRepairDockTable();
	}
	else if (pathAndQuery == "/kcsapi/api_get_member/kdock")
	{
		pksd->kdockdata.clear();
		QJsonArray jarray = jdoc.object()["api_data"].toArray();
		for (int i=0; i<jarray.count(); i++)
		{
			kcsapi_kdock api_kdock;
			api_kdock.ReadFromJObj(jarray[i].toObject());
			pksd->kdockdata.append(api_kdock);
		}

		updateBuildDockTable();
	}
	else if (pathAndQuery == "/kcsapi/api_get_member/material")
	{
		pksd->portdata.api_material.clear();
		QJsonArray jarray = jdoc.object()["api_data"].toArray();
		for (int i=0; i<jarray.count(); i++)
		{
			kcsapi_material api_material;
			api_material.ReadFromJObj(jarray[i].toObject());
			pksd->portdata.api_material.append(api_material);
		}

		updateOverviewTable();
	}
	else if (pathAndQuery == "/kcsapi/api_get_member/questlist")
	{
		// TODO: remove quest
		kcsapi_questlist api_questlist;
		api_questlist.ReadFromJObj(jobj);

		int questcount = api_questlist.api_count-5*(api_questlist.api_disp_page-1);
		if (questcount > api_questlist.api_list.count())
		{
			questcount = api_questlist.api_list.count();
		}

		int beginindex = api_questlist.api_list[0].api_no;
		int endindex = api_questlist.api_list[questcount-1].api_no;
		/*
		if (api_questlist.api_disp_page == 1)
		{
			beginindex = -1;
		}
		else if (api_questlist.api_disp_page == api_questlist.api_page_count)
		{
			endindex = 10000000;
		}
		*/

		//delete all in questdata
		QList<kcsapi_quest>::iterator it;
		for (it=pksd->questdata.begin(); it!=pksd->questdata.end();)
		{
			if (it->api_no >= beginindex && it->api_no <= endindex)
			{
				it = pksd->questdata.erase(it);
			}
			else
			{
				++it;
			}
		}

		// add to
		for (int i=0; i<questcount; i++)
		{
			if (api_questlist.api_list[i].api_state > 1)
			{
				pksd->questdata.append(api_questlist.api_list[i]);
			}
		}
		qSort(pksd->questdata.begin(), pksd->questdata.end(), questDataSort);

		updateMissionTable();
	}

	//

	else if (pathAndQuery == "/kcsapi/api_req_hensei/change")
	{
		int team = req.GetItemAsString("api_id").toInt();
		int index = req.GetItemAsString("api_ship_idx").toInt();
		int shipid = req.GetItemAsString("api_ship_id").toInt();

		QList<int> * lstship = &(pksd->portdata.api_deck_port[team-1].api_ship);

		while(lstship->count() < 6)
		{
			lstship->append(-1);
		}

		if (index == -1)
		{
			shipid = lstship->first();
			lstship->clear();
			lstship->append(shipid);
		}
		else if (shipid == -1)
		{
			lstship->removeAt(index);
		}
		else
		{
			int prev = -1;
			int previndex = -1;
			if (lstship->count() >= index+1)
			{
				prev = (*lstship)[index];
				for (int i=0; i<lstship->count(); i++)
				{
					if ((*lstship)[i] == shipid)
					{
						previndex = i;
						break;
					}
				}
			}

			(*lstship)[index] = shipid;
			if (previndex >= 0)
			{
				(*lstship)[previndex] = prev;
			}
		}
		updateFleetTable();
	}
	else if (pathAndQuery == "/kcsapi/api_req_kousyou/getship")
	{
		int kdock_id = req.GetItemAsString("api_kdock_id").toInt();
		pksd->kdockdata[kdock_id-1].api_created_ship_id = 0;

		kcsapi_kdock_getship api_kdock_getship;
		api_kdock_getship.ReadFromJObj(jobj);
		foreach(const kcsapi_slotitem &v, api_kdock_getship.api_slotitem)
		{
			if (v.api_slotitem_id >= 0)
			{
				pksd->slotitemcountoffset++;
			}
		}

		pksd->shipcountoffset++;

		updateOverviewTable();
		updateBuildDockTable();
	}
	else if (pathAndQuery == "/kcsapi/api_req_kousyou/createitem")
	{
		kcsapi_createitem api_createitem;
		api_createitem.ReadFromJObj(jobj);
		if (api_createitem.api_create_flag == 1)
		{
			pksd->slotitemcountoffset++;
		}
		for (int i=0; i<api_createitem.api_material.count(); i++)
		{
			pksd->portdata.api_material[i].api_value = api_createitem.api_material[i];
		}

		updateOverviewTable();
	}
	else if (pathAndQuery == "/kcsapi/api_req_kousyou/createship")
	{
		int usef = req.GetItemAsString("api_item_1").toInt();
		int usebu = req.GetItemAsString("api_item_2").toInt();
		int uses = req.GetItemAsString("api_item_3").toInt();
		int useba = req.GetItemAsString("api_item_3").toInt();
		int usedev = req.GetItemAsString("api_item_4").toInt();
		int bLarge = req.GetItemAsString("api_large_flag").toInt();
		if (bLarge)
		{
			DAPILOG();
		}

		pksd->portdata.api_material[MATERIALDATAINDEX_FUEL].api_value-=usef;
		pksd->portdata.api_material[MATERIALDATAINDEX_BULLET].api_value-=usebu;
		pksd->portdata.api_material[MATERIALDATAINDEX_STEEL].api_value-=uses;
		pksd->portdata.api_material[MATERIALDATAINDEX_BAUXITE].api_value-=useba;
		pksd->portdata.api_material[MATERIALDATAINDEX_DEVELOPMENT].api_value-=usedev;

	}
	else if (pathAndQuery == "/kcsapi/api_req_kousyou/createship_speedchange")
	{
		int dockid = req.GetItemAsString("api_kdock_id").toInt()-1;
		if (dockid >= 0)
		{
			pksd->kdockdata[dockid].api_complete_time = 0;
			updateBuildDockTable();
		}

	}
	else if (pathAndQuery == "/kcsapi/api_req_kousyou/destroyship")
	{
		int shipno = req.GetItemAsString("api_ship_id").toInt();
		bool bDone = false;

		QList<kcsapi_deck>::iterator it;
		for (it=pksd->portdata.api_deck_port.begin(); it!=pksd->portdata.api_deck_port.end(); ++it)
		{
			for (int i=0; i<it->api_ship.count(); i++)
			{
				if (it->api_ship[i] == shipno)
				{
					it->api_ship.removeAt(i);
					bDone = true;
					break;
				}
			}
			if (bDone)
			{
				break;
			}
		}

		// slotitem
		const kcsapi_ship2 * pship = findShipFromShipno(shipno);
		if (pship)
		{
			foreach(int slotitemid, pship->api_slot)
			{
				if (slotitemid >= 0)
				{
					pksd->slotitemcountoffset--;
				}
			}
		}

		// material
		kcsapi_destroyship api_destroyship;
		api_destroyship.ReadFromJObj(jobj);
		QList<int> api_material = api_destroyship.api_material;
		for (int i=0; i<api_material.count(); i++)
		{
			pksd->portdata.api_material[i].api_value = api_material[i];
		}

		if (!RemoveShip(shipno))
		{
			pksd->shipcountoffset--;
		}
		updateOverviewTable();
		updateFleetTable();

		updateRepairTable();
	}
	else if (pathAndQuery == "/kcsapi/api_req_kousyou/destroyitem2")
	{
		QString idsstr = req.GetItemAsString("api_slotitem_ids");
		QStringList idslst = idsstr.split("%2C");
		pksd->slotitemcountoffset-=idslst.count();


		kcsapi_destroyitem2 api_destroyitem2;
		api_destroyitem2.ReadFromJObj(jobj);
		QList<int> api_get_material = api_destroyitem2.api_get_material;
		for (int i=0; i<api_get_material.count(); i++)
		{
			pksd->portdata.api_material[i].api_value+=api_get_material[i];
		}
		updateOverviewTable();
	}
	else if (pathAndQuery == "/kcsapi/api_req_nyukyo/start")
	{
	}
	else if (pathAndQuery == "/kcsapi/api_req_nyukyo/speedchange")
	{
		int dockid = req.GetItemAsString("api_ndock_id").toInt()-1;
		if (dockid >= 0)
		{
			int shipno = pksd->portdata.api_ndock[dockid].api_ship_id;
			pksd->portdata.api_ndock[dockid].api_ship_id = 0;
			kcsapi_ship2 * pship = findShipFromShipno(shipno);
			if (pship)
			{
				pship->api_ndock_time = 0;
				updateRepairTable();
			}
			updateRepairDockTable();
		}
	}
	else if (pathAndQuery == "/kcsapi/api_req_kousyou/createship_speedchange")
	{
		int kdock = req.GetItemAsString("api_kdock_id").toInt();
		pksd->kdockdata[kdock-1].api_complete_time = 0;
		updateBuildDockTable();
	}
	else if (pathAndQuery == "/kcsapi/api_req_hokyu/charge")
	{
		/*
		int kind = req.GetItemAsString("api_kind").toInt();

		QString shipsstr = req.GetItemAsString("api_id_items");
		QStringList shipslst = shipsstr.split("%2C");
		foreach(const QString &v, shipslst)
		{
			kcsapi_ship2 * pship = findShipFromShipno(v.toInt());
			kcsapi_mst_ship * pmstship = findMstShipFromShipid(pship->api_ship_id);
			if (kind == 1 || kind == 3)
			{
				pship->api_fuel = pmstship->api_fuel_max;
			}
			if (kind == 2 || kind == 3)
			{
				pship->api_bull = pmstship->api_bull_max;
			}
		}
		// api_on_slot
		*/

		kcsapi_charge api_charge;
		api_charge.ReadFromJObj(jobj);

		foreach (const kcsapi_charge_ship &v, api_charge.api_ship)
		{
			QList<kcsapi_ship2>::iterator it;
			for (it=pksd->portdata.api_ship.begin(); it!=pksd->portdata.api_ship.end(); ++it)
			{
				if (it->api_id == v.api_id)
				{
					it->api_fuel = v.api_fuel;
					it->api_bull = v.api_bull;
					break;
				}
			}
		}

		for(int i=0; i<api_charge.api_material.count(); i++)
		{
			pksd->portdata.api_material[i].api_value = api_charge.api_material[i];
		}

		updateFleetTable();
	}
	else if (pathAndQuery == "/kcsapi/api_req_kaisou/powerup")
	{
		QStringList shipsids = req.GetItemAsString("api_id_items").split("%2C");
		foreach(const QString &v, shipsids)
		{
			pksd->shipcountoffset--;
		}

		kcsapi_powerup api_powerup;
		api_powerup.ReadFromJObj(jobj);
		pksd->portdata.api_deck_port = api_powerup.api_deck;

		updateOverviewTable();
		updateFleetTable();
	}
	else if (pathAndQuery == "/kcsapi/api_req_member/updatedeckname")
	{
		// ignore
	}
	else if (pathAndQuery == "/kcsapi/api_req_member/updatecomment")
	{
		// ignore
	}
	else if (pathAndQuery == "/kcsapi/api_req_sortie/battle")
	{
		pksd->battledata.ReadFromJObj(jobj);

		updateBattle(pksd->battledata, KANBATTLETYPE_DAY);

		DAPILOG();
	}
	else if (pathAndQuery == "/kcsapi/api_req_sortie/battleresult")
	{
		// drop
		kcsapi_battleresult api_battleresult;
		api_battleresult.ReadFromJObj(jobj);
		int shipid = api_battleresult.api_get_ship.api_ship_id;
		if (shipid > 0)
		{
			pksd->shipcountoffset++;
		}
		const kcsapi_mst_ship * pmstship = findMstShipFromShipid(shipid);
		if (pmstship)
		{
			foreach(int slotitemid, pmstship->api_defeq)
			{
				if (slotitemid > 0)
				{
					pksd->slotitemcountoffset++;
				}
			}
		}
	}
	//
	else if (pathAndQuery == "/kcsapi/api_auth_member/logincheck")
	{
		// ignore
	}
	else if (pathAndQuery == "/kcsapi/api_req_mission/result")
	{
		DAPILOG();
	}
	else if (pathAndQuery == "/kcsapi/api_get_member/record")
	{
	}
	else if (pathAndQuery == "/kcsapi/api_get_member/book2")
	{
	}
	else if (pathAndQuery == "/kcsapi/api_req_kaisou/slotset")
	{
		//TODO: ignore slot set?
	}
	else if (pathAndQuery == "/kcsapi/api_req_hensei/lock")
	{
		//ignore
	}
	else if (pathAndQuery == "/kcsapi/api_req_member/itemuse_cond")
	{
		//TODO: mamiya
		DAPILOG();
	}
	else if (pathAndQuery == "/kcsapi/api_req_kousyou/open_new_dock" || pathAndQuery == "/kcsapi/api_req_nyukyo/open_new_dock")
	{
		// ignore
	}
	else if (pathAndQuery == "/kcsapi/api_req_kaisou/remodeling")
	{
		// ignore
	}
	else if (pathAndQuery == "/kcsapi/api_get_member/practice")
	{
		// ignore
	}

	//

	else if (pathAndQuery == "/kcsapi/api_req_map/start")
	{
		pksd->lastdeckid = req.GetItemAsString("api_deck_id").toInt()-1;
		pksd->nextdata.ReadFromJObj(jobj);

		updateInfoTitle();
		checkWoundQuit();
	}
	else if (pathAndQuery == "/kcsapi/api_req_map/next")
	{
		pksd->nextdata.ReadFromJObj(jobj);
		
		updateInfoTitle();
		checkWoundQuit();

	}
	else if (pathAndQuery == "/kcsapi/api_req_mission/start")
	{
		// expedition
		kcsapi_mission_start api_mission_start;
		api_mission_start.ReadFromJObj(jobj);

		int missionid = req.GetItemAsString("api_mission_id").toInt();
		int deckid = req.GetItemAsString("api_deck_id").toInt();
		if (deckid > 0)
		{
			pksd->portdata.api_deck_port[deckid-1].api_mission[0] = 1;
			pksd->portdata.api_deck_port[deckid-1].api_mission[1] = missionid;
			pksd->portdata.api_deck_port[deckid-1].api_mission[2] = api_mission_start.api_complatetime;
		}
		updateExpeditionTable();

	}
	else if (pathAndQuery == "/kcsapi/api_req_member/get_incentive")
	{
		// ignore
	}
	else if (pathAndQuery == "/kcsapi/api_get_member/furniture")
	{

	}
	else if (pathAndQuery == "/kcsapi/api_get_member/unsetslot")
	{
	}
	else if (pathAndQuery == "/kcsapi/api_get_member/mission")
	{

	}
	else if (pathAndQuery == "/kcsapi/api_get_member/payitem")
	{

	}
	else if (pathAndQuery == "/kcsapi/api_get_member/mapinfo")
	{
		//??
		DAPILOG();
	}
	else if (pathAndQuery == "/kcsapi/api_get_member/mapcell")
	{
		//??
		DAPILOG();
	}
	else if (pathAndQuery == "/kcsapi/api_get_member/record")
	{
	}
	else if (pathAndQuery == "/kcsapi/api_req_practice/battle")
	{
		pksd->battledata.ReadFromJObj(jobj);

		updateBattle(pksd->battledata, KANBATTLETYPE_DAY);
	}
	else if (pathAndQuery == "/kcsapi/api_req_battle_midnight/sp_midnight")
	{
		DAPILOG();
	}
	else if (pathAndQuery == "/kcsapi/api_req_sortie/night_to_day")
	{
		//TODO night to day:
		pksd->battledata.ReadFromJObj(jobj);

		updateBattle(pksd->battledata, KANBATTLETYPE_NIGHTTODAY);

		DAPILOG();
	}
	else if (pathAndQuery == "/kcsapi/api_req_battle_midnight/battle")
	{
		pksd->battledata.ReadFromJObj(jobj);

		updateBattle(pksd->battledata, KANBATTLETYPE_NIGHT);

		DAPILOG();
	}
	else if (pathAndQuery == "/kcsapi/api_req_practice/midnight_battle")
	{
		pksd->battledata.ReadFromJObj(jobj);

		updateBattle(pksd->battledata, KANBATTLETYPE_NIGHT);

		DAPILOG();

	}
	else if (pathAndQuery == "/kcsapi/api_req_member/get_practice_enemyinfo")
	{
		// ignore
		/*
		kcsapi_practice_enemyinfo api_practice_enemyinfo;
		api_practice_enemyinfo.ReadFromJObj(jobj);
		*/
	}
	else if (pathAndQuery == "/kcsapi/api_req_practice/battle_result")
	{
		// ignore
	}
	else if (pathAndQuery == "/kcsapi/api_req_quest/start")
	{
		//ignore
	}
	else if (pathAndQuery == "/kcsapi/api_req_quest/stop")
	{
		int questid = req.GetItemAsString("api_quest_id").toInt();

		for (QList<kcsapi_quest>::iterator it = pksd->questdata.begin(); it != pksd->questdata.end(); ++it)
		{
			if (it->api_no == questid)
			{
				pksd->questdata.erase(it);
				break;
			}
		}

		updateMissionTable();
	}
	else if (pathAndQuery == "/kcsapi/api_req_member/itemuse")
	{

	}
	else if (pathAndQuery == "/kcsapi/api_req_quest/clearitemget")
	{
		kcsapi_clearitemget api_clearitemget;
		api_clearitemget.ReadFromJObj(jobj);
		for (int i = 0; i < api_clearitemget.api_material.count(); i++)
		{
			pksd->portdata.api_material[i].api_value += api_clearitemget.api_material[i];
		}
		if (api_clearitemget.api_bounus_count)
		{
			for (int i = 0; i < api_clearitemget.api_bounus_count; i++)
			{
				int index = api_clearitemget.api_bounus[i].api_type + 3;
				if (index < 7)
				{
					pksd->portdata.api_material[index].api_value += api_clearitemget.api_bounus[i].api_count;
				}
			}
		}

		int questid = req.GetItemAsString("api_quest_id").toInt();

		for (QList<kcsapi_quest>::iterator it = pksd->questdata.begin(); it != pksd->questdata.end(); ++it)
		{
			if (it->api_no == questid)
			{
				pksd->questdata.erase(it);
				break;
			}
		}

		updateMissionTable();
	}
	else if (pathAndQuery == "/kcsapi/api_get_member/picture_book")
	{
	}
	else if (pathAndQuery == "/kcsapi/api_req_ranking/getlist")
	{

	}
	else if (pathAndQuery == "/kcsapi/api_req_furniture/buy")
	{
	}
	else if (pathAndQuery == "/kcsapi/api_req_furniture/change")
	{
	}
	else
	{
		DAPILOG();
	}
	/*
/kcaspi/api_req_member/getothersdeck
/kcsapi/api_get_member/actionlog
/kcsapi/api_req_init/nickname
/kcsapi/api_req_init/firstship
/kcsapi/api_world/get_worldinfo
/kcsapi/api_world/register
/kcsapi/api_start
	*/

}

void KanDataConnector::updateOverviewTable()
{
	QList<QString> lst;
	QList<QColor> cols;
	KanSaveData * pksd = &KanSaveData::getInstance();

	int kancount = pksd->portdata.api_ship.count()+pksd->shipcountoffset;
	int kanmaxcount = pksd->portdata.api_basic.api_max_chara;
	int slotitemcount = pksd->slotitemdata.count()+pksd->slotitemcountoffset;
	int slotitemmaxcount = pksd->portdata.api_basic.api_max_slotitem;
	int instantrepaircount = pksd->portdata.api_material[MATERIALDATAINDEX_INSTANDREPAIR].api_value;
	int instantbuildcount = pksd->portdata.api_material[MATERIALDATAINDEX_INSTANTBUILD].api_value;
	int lv = pksd->portdata.api_basic.api_level;
	int nextexp = KanDataCalc::GetAdmiralNextLevelExp(pksd->portdata.api_basic.api_experience, lv);
	int fcoin = pksd->portdata.api_basic.api_fcoin;

	lst.append(QString::fromLocal8Bit("所有艦娘数: %1/%2").arg(kancount).arg(kanmaxcount));
	lst.append(QString::fromLocal8Bit("所有装備数: %1/%2").arg(slotitemcount).arg(slotitemmaxcount));
	lst.append(QString::fromLocal8Bit("高速修復材: %1").arg(instantrepaircount));
	lst.append(QString::fromLocal8Bit("高速建造材: %1").arg(instantbuildcount));
	lst.append(QString::fromLocal8Bit("提督Lv. %1(次:%2)").arg(lv).arg(nextexp));
	lst.append(QString::fromLocal8Bit("家具コイン: %1").arg(fcoin));

	//
	if (kanmaxcount == kancount)
	{
		cols.append(colRed);
	}
	else if (kanmaxcount - kancount < 5)
	{
		cols.append(colOrange);
	}
	else
	{
		cols.append(colWhite);
	}
	//
	if (slotitemmaxcount == slotitemcount)
	{
		cols.append(colRed);
	}
	else if (slotitemmaxcount - slotitemcount < 20)
	{
		cols.append(colOrange);
	}
	else
	{
		cols.append(colWhite);
	}
	//
	cols.append(colWhite);
	cols.append(colWhite);
	cols.append(colWhite);
	cols.append(colWhite);

	MainWindow::infoWindow()->updateOverviewTable(lst, cols);
}

void KanDataConnector::updateMissionTable()
{
	KanSaveData * pksd = &KanSaveData::getInstance();

	QList<KQRowData> rows;

	int questcount = pksd->questdata.count();
	for (int i=0; i<questcount; i++)
	{
		KQRowData rd;
		rd.appendCell(KQRowCellData(" - "));
		rd.appendCell(KQRowCellData(pksd->questdata[i].api_title, pksd->questdata[i].api_detail, colWhite));

		QString progress;
		bool bDone = false;
		switch (pksd->questdata[i].api_state)
		{
		case 1:
			qDebug("Quest not taken.");
			break;
		case 2:
			break;
		case 3:
			bDone = true;
			progress = QString::fromLocal8Bit("(完遂)");
			break;
		}
		if (!bDone)
		{
			switch (pksd->questdata[i].api_progress_flag)
			{
			case 0:
				progress = "(0%)";
				break;
			case 1:
				progress = "(50%)";
				break;
			case 2:
				progress = "(80%)";
				break;
			}
		}

		rd.appendCell(KQRowCellData(progress));
		rows.append(rd);
	}

	MainWindow::infoWindow()->updateMissionTable(QString::fromLocal8Bit("遂行中任務(%1)").arg(questcount), rows);
}

bool repairTableSort(const kcsapi_ship2 &left, const kcsapi_ship2 &right)
{
	return left.api_ndock_time > right.api_ndock_time;
}

void KanDataConnector::updateRepairTable()
{
	KanSaveData * pksd = &KanSaveData::getInstance();
	QList<kcsapi_ship2> lstShips;
	foreach(const kcsapi_ship2 &ship2, pksd->portdata.api_ship)
	{
		if (ship2.api_ndock_time && !isShipRepairing(&ship2))
		{
			lstShips.append(ship2);
		}
	}
	qSort(lstShips.begin(), lstShips.end(), repairTableSort);

	int repaircount = lstShips.count();
	QList<KQRowData> rows;
	foreach(const kcsapi_ship2 &v, lstShips)
	{
		// Name cond
		// Lv cond
		// Cond cond
		// Time cond
		// HP w
		// State w
		int shipid = v.api_ship_id;
		const kcsapi_mst_ship * pmstship = findMstShipFromShipid(shipid);

		QColor colCond;
		QColor colWound;
		getShipColors(&v, &colCond, &colWound);
		QString woundstatestr = getShipWoundStateString(&v);

		KQRowData rd;
		rd.appendCell(KQRowCellData(pmstship->api_name, colCond));
		rd.appendCell(KQRowCellData(QString::fromLocal8Bit("Lv.%1").arg(v.api_lv), colCond));
		rd.appendCell(KQRowCellData(QString::fromLocal8Bit("(%1)").arg(v.api_cond), colCond));
		KQTime t;
		t.setTotalMS(v.api_ndock_time);
		rd.appendCell(KQRowCellData(t.toString(), colCond));
		rd.appendCell(KQRowCellData(QString::fromLocal8Bit("(%1/%2)").arg(v.api_nowhp).arg(v.api_maxhp), colWound));
		rd.appendCell(KQRowCellData(woundstatestr, colWound));

		rows.append(rd);
	}

	MainWindow::infoWindow()->updateRepairTable(QString::fromLocal8Bit("入渠まちーズ(%1)").arg(repaircount), rows);
}

void KanDataConnector::updateFleetTable()
{
	KanSaveData * pksd = &KanSaveData::getInstance();

	foreach (const kcsapi_deck &v, pksd->portdata.api_deck_port)
	{
		// No.
		// Name cond
		// Lv cond
		// Cond cond
		// Exp cond
		// Fuel f
		// Bullet b
		// Hp w
		// State w
		QList<KQRowData> rows;
		bool bRed = false;
		int shipcount = 0;

		int kiracount = 0;
		bool bNeedCharge = false;

		bool bCondDown = false;

		int alltaikyu=0;
		int alllv=0;
		foreach(int shipno, v.api_ship)
		{
			if (shipno < 0)
			{
				break;
			}
			shipcount++;
			KQRowData rd;

			const kcsapi_ship2 * pship = findShipFromShipno(shipno);
			if (!pship)
			{
				continue;
			}
			int shipid = pship->api_ship_id;
			const kcsapi_mst_ship * pmstship = findMstShipFromShipid(shipid);

			QColor colCond;
			QColor colWound;
			QColor colFuel;
			QColor colBullet;

			int condstate;
			int woundstate;

			getShipColors(pship, &colCond, &colWound, &condstate, &woundstate);
			getShipChargeColors(pship, pmstship, &colFuel, &colBullet);

			if (condstate > CONDSTATE_SMALL)
			{
				bCondDown = true;
			}
			if (woundstate > WOUNDSTATE_MIDDLE)
			{
				bRed = true;
			}

			int nextexp = pship->api_exp[1];
			QString woundstatestr = getShipWoundStateString(pship);

			if (condstate == CONDSTATE_KIRA)
			{
				kiracount++;
			}
			if (!bNeedCharge)
			{
				if (pship->api_fuel != pmstship->api_fuel_max || pship->api_bull != pmstship->api_bull_max)
				{
					bNeedCharge = true;
				}
			}

			rd.appendCell(KQRowCellData(QString("%1:").arg(shipcount)));
			rd.appendCell(KQRowCellData(pmstship->api_name, colCond)); // tooltip?
			rd.appendCell(KQRowCellData(QString("Lv.%1").arg(pship->api_lv), colCond));
			rd.appendCell(KQRowCellData(QString("(%1)").arg(pship->api_cond), colCond));
			rd.appendCell(KQRowCellData(QString::fromLocal8Bit("次:%1").arg(nextexp), colCond));
			rd.appendCell(KQRowCellData(QString::fromLocal8Bit("燃"), colFuel));
			rd.appendCell(KQRowCellData(QString::fromLocal8Bit("弾"), colBullet));
			rd.appendCell(KQRowCellData(QString("(%1/%2)").arg(pship->api_nowhp).arg(pship->api_maxhp), colWound));
			rd.appendCell(KQRowCellData(woundstatestr, colWound));

			int taikyu = 0;
			for (int i=0; i<pship->api_slot.count(); i++)
			{
				int slotitemid = pship->api_slot[i];
				bool bDone = false;
				foreach(const kcsapi_slotitem &v, pksd->slotitemdata)
				{
					if (v.api_id == slotitemid)
					{
						foreach(const kcsapi_mst_slotitem &item, pksd->start2data.api_mst_slotitem)
						{
							if (item.api_id == v.api_slotitem_id)
							{
								if (item.api_type.count() > 2)
								{
									int type = item.api_type[2];
									if (type == SLOTITEMTYPE_KANSEN
										|| type == SLOTITEMTYPE_KANBAKU
										|| type == SLOTITEMTYPE_KANKOU
										|| type == SLOTITEMTYPE_SUIBAKU)
									{
										taikyu += (int)(item.api_tyku*sqrt((double)pship->api_onslot[i]));
										bDone = true;
										break;
									}
								}
							}
						}
						if (bDone)
						{
							break;
						}
					}
				}
			}
			alltaikyu += taikyu;
			alllv += pship->api_lv;

			rows.append(rd);
		}

		int colindex = 0;
		if (bNeedCharge || bCondDown)
		{
			colindex = 3;
		}
		else if (kiracount == shipcount)
		{
			colindex = 2;
		}
		else if (kiracount)
		{
			colindex = 1;
		}

		QString strtitle = QString::fromLocal8Bit("%1 (Lv計:%2 制空:%3)").arg(v.api_name).arg(alllv).arg(alltaikyu);
		MainWindow::infoWindow()->updateFleetTable(v.api_id-1, strtitle, colindex, bRed, rows);
	}


}

void KanDataConnector::updateExpeditionTable()
{
	KanSaveData * pksd = &KanSaveData::getInstance();
	foreach (const kcsapi_deck &v, pksd->portdata.api_deck_port)
	{
		if (v.api_id <= 1)
		{
			continue;
		}
		if (v.api_mission[0] <= 0)
		{
			MainWindow::timerWindow()->setExpeditionTime(v.api_id-2);
		}
		else
		{
			const Api_Mst_Mission *pMission = findMstMissionFromMissionid(v.api_mission[1]);
			qint64 missiontotalms = pMission->api_time*60000;

			QColor col;
			switch (pMission->api_maparea_id)
			{
			case 1:
				col.setRgb(224, 255, 255);
				break;
			case 2:
				col.setRgb(134, 255, 134);
				break;
			case 3:
				col.setRgb(151, 254, 243);
				break;
			case 4:
				col.setRgb(221, 151, 254);
				break;
			case 5:
				col.setRgb(60, 179, 113);
				break;
			default:
				col.setRgb(252, 127, 107);
				break;
			}

			MainWindow::timerWindow()->setExpeditionTime(v.api_id-2, v.api_mission[2], missiontotalms, pMission->api_name, col);
		}
	}

}

void KanDataConnector::updateRepairDockTable()
{
	KanSaveData * pksd = &KanSaveData::getInstance();
	foreach (const kcsapi_ndock &v, pksd->portdata.api_ndock)
	{
		if (v.api_ship_id == 0)
		{
			MainWindow::timerWindow()->setRepairTime(v.api_id-1);
		}
		else
		{
			int shipno = v.api_ship_id; // ?
			const kcsapi_ship2 *pship = findShipFromShipno(shipno);
			if (pship)
			{
				const kcsapi_mst_ship *pmstship = findMstShipFromShipid(pship->api_ship_id);
				if (pmstship)
				{
					MainWindow::timerWindow()->setRepairTime(v.api_id - 1, v.api_complete_time, pship->api_ndock_time, QString("%1 Lv.%2").arg(pmstship->api_name).arg(pship->api_lv));
				}
			}
			
		}
	}

}

void KanDataConnector::updateBuildDockTable()
{
	KanSaveData * pksd = &KanSaveData::getInstance();
	foreach (const kcsapi_kdock &v, pksd->kdockdata)
	{
		if (v.api_state < 0)
		{
			continue;
		}
		if (v.api_created_ship_id == 0)
		{
			MainWindow::timerWindow()->setBuildTime(v.api_id-1);
		}
		else
		{
			int shipid = v.api_created_ship_id;
//            const kcsapi_ship2 *pship = findShipFromShipno(shipno);
			const kcsapi_mst_ship *pmstship = findMstShipFromShipid(shipid);
			MainWindow::timerWindow()->setBuildTime(v.api_id-1, v.api_complete_time, pmstship->api_buildtime, pmstship->api_name);
		}
	}

}

void KanDataConnector::updateInfoTitle(bool bBattle, QList<int> * enemyhps)
{
	KanSaveData * pksd = &KanSaveData::getInstance();
	if (pksd->nextdata.api_no < 0 && !enemyhps)
	{
		MainWindow::infoWindow()->updateTitle("", false);
		return;
	}
	QString strtitle;

	if (pksd->nextdata.api_no >= 0)
	{
		strtitle = QString::fromLocal8Bit("現:%1 ボス:%2")
			.arg(pksd->nextdata.api_no)
			.arg(pksd->nextdata.api_bosscell_no);
	}

	if (bBattle)
	{
		int totalremain = 0;
		int acremain = 0;
		int actotal = 0;
		int transremain = 0;
		int transtotal = 0;
		int subremain = 0;
		int subtotal = 0;

		for (int i = 1; i<pksd->battledata.api_ship_ke.count(); i++)
		{
			int shipid = pksd->battledata.api_ship_ke[i];
			if (shipid > 0)
			{
				const kcsapi_mst_ship * pmstship = findMstShipFromShipid(shipid);
				if (pmstship)
				{
					if ((*enemyhps)[i] > 0)
					{
						totalremain++;
					}
					switch (pmstship->api_stype)
					{
					case SHIPTYPE_KUBO:
					case SHIPTYPE_KEIKUBO:
					case SHIPTYPE_SOUKAKUBO:
						actotal++;
						if ((*enemyhps)[i] > 0)
						{
							acremain++;
						}
						break;
					case SHIPTYPE_HOKYU:
						transtotal++;
						if ((*enemyhps)[i] > 0)
						{
							transremain++;
						}
						break;
					case SHIPTYPE_SENSUI:
					case SHIPTYPE_SENBO:
						subtotal++;
						if ((*enemyhps)[i] > 0)
						{
							subremain++;
						}
						break;
					}
					
				}
			}
		}

		strtitle += QString::fromLocal8Bit(" - 残:%1, 輸%2(%3), 航:%4(%5), 潜%6(%7)")
			.arg(totalremain)
			.arg(transremain)
			.arg(transtotal)
			.arg(acremain)
			.arg(actotal)
			.arg(subremain)
			.arg(subtotal);
	}

	int colindex = 0;
	if (pksd->nextdata.api_no==pksd->nextdata.api_bosscell_no)
	{
		colindex = 1;
	}
	MainWindow::infoWindow()->updateTitle(strtitle, colindex);
}

void KanDataConnector::getShipColors(const kcsapi_ship2 *pship, QColor *pcolCond, QColor *pcolWound, int* pcondstate, int* pwoundstate)
{
	if (pcolCond || pcondstate)
	{
		int condstate = KanDataCalc::GetCondState(pship->api_cond);
		QColor col;
		switch (condstate)
		{
		case CONDSTATE_NORMAL:
			col = colWhite;
			break;
		case CONDSTATE_SMALL:
			col = colGray;
			break;
		case CONDSTATE_MIDDLE:
			col = colOrange;
			break;
		case CONDSTATE_BIG:
			col = colRed;
			break;
		case CONDSTATE_KIRA:
			col = colYellow;
			break;
		}
		if (pcolCond)
		{
			*pcolCond = col;
		}
		if (pcondstate)
		{
			*pcondstate = condstate;
		}
	}
	if (pcolWound || pwoundstate)
	{
		int woundstate = KanDataCalc::GetWoundState(pship->api_nowhp, pship->api_maxhp);
		QColor col;
		switch (woundstate)
		{
		case WOUNDSTATE_FULL:
			col = colWhite;
			break;
		case WOUNDSTATE_LITTLE:
			col = colGray;
			break;
		case WOUNDSTATE_SMALL:
			col = colYellow;
			break;
		case WOUNDSTATE_MIDDLE:
			col = colOrange;
			break;
		case WOUNDSTATE_BIG:
			col = colRed;
			break;
		case WOUNDSTATE_DEAD:
			col = colBlue;
			break;
		}
		if (pcolWound)
		{
			*pcolWound = col;
		}
		if (pwoundstate)
		{
			*pwoundstate = woundstate;
		}
	}
}

void KanDataConnector::getShipChargeColors(const kcsapi_ship2 *pship, const kcsapi_mst_ship *pmstship, QColor *pcolFuel, QColor *pcolBullet)
{
	int fuelstate = KanDataCalc::GetChargeState(pship->api_fuel, pmstship->api_fuel_max);
	int bullstate = KanDataCalc::GetChargeState(pship->api_bull, pmstship->api_bull_max);

	int state = fuelstate;
	QColor * pcol = pcolFuel;

	switch (state)
	{
	case CHARGESTATE_FULL:
		*pcol = colWhite;
		break;
	case CHARGESTATE_SMALL:
		*pcol = colYellow;
		break;
	case CHARGESTATE_MIDDLE:
		*pcol = colOrange;
		break;
	case CHARGESTATE_BIG:
		*pcol = colRed;
		break;
	case CHARGESTATE_EMPTY:
		*pcol = colBlue;
		break;
	}
	state = bullstate;
	pcol = pcolBullet;

	switch (state)
	{
	case CHARGESTATE_FULL:
		*pcol = colWhite;
		break;
	case CHARGESTATE_SMALL:
		*pcol = colYellow;
		break;
	case CHARGESTATE_MIDDLE:
		*pcol = colOrange;
		break;
	case CHARGESTATE_BIG:
		*pcol = colRed;
		break;
	case CHARGESTATE_EMPTY:
		*pcol = colBlue;
		break;
	}
}

QString KanDataConnector::getShipWoundStateString(const kcsapi_ship2 *pship)
{
	if (isShipRepairing(pship))
	{
		return QString::fromLocal8Bit("渠");
	}
	int woundstate = KanDataCalc::GetWoundState(pship->api_nowhp, pship->api_maxhp);
	switch (woundstate)
	{
	case WOUNDSTATE_FULL:
		return "";
		break;
	case WOUNDSTATE_LITTLE:
		return "";
		break;
	case WOUNDSTATE_SMALL:
		return QString::fromLocal8Bit("小");
		break;
	case WOUNDSTATE_MIDDLE:
		return QString::fromLocal8Bit("中");
		break;
	case WOUNDSTATE_BIG:
		return QString::fromLocal8Bit("大");
		break;
	case WOUNDSTATE_DEAD:
		return QString::fromLocal8Bit("沈");
		break;
	}
	return "";
}

bool KanDataConnector::isShipRepairing(const kcsapi_ship2 *pship)
{
	KanSaveData * pksd = &KanSaveData::getInstance();
	foreach(const kcsapi_ndock &v, pksd->portdata.api_ndock)
	{
		if (v.api_ship_id == pship->api_id)
		{
			return true;
		}
	}
	return false;
}

bool KanDataConnector::RemoveShip(int shipno)
{
	KanSaveData * pksd = &KanSaveData::getInstance();

	for (QList<kcsapi_ship2>::iterator it=pksd->portdata.api_ship.begin();
		 it!=pksd->portdata.api_ship.end(); ++it)
	{
		if (it->api_id == shipno)
		{
			pksd->portdata.api_ship.erase(it);
			return true;
		}
	}
	return false;
}

kcsapi_ship2 *KanDataConnector::findShipFromShipno(int shipno)
{
	KanSaveData * pksd = &KanSaveData::getInstance();

	for (QList<kcsapi_ship2>::iterator it=pksd->portdata.api_ship.begin();
		 it!=pksd->portdata.api_ship.end(); ++it)
	{
		if (it->api_id == shipno)
		{
			return &(*it);
		}
	}
	qDebug("No ship data.");
	return 0;
}

const kcsapi_mst_ship *KanDataConnector::findMstShipFromShipid(int shipid) const
{
	KanSaveData * pksd = &KanSaveData::getInstance();
	foreach(const kcsapi_mst_ship &v, pksd->start2data.api_mst_ship)
	{
		if (v.api_id == shipid)
		{
			return &v;
		}
	}
	qDebug("No mst ship data.");
	return 0;
}

const Api_Mst_Mission *KanDataConnector::findMstMissionFromMissionid(int missionid) const
{
	KanSaveData * pksd = &KanSaveData::getInstance();
	foreach(const Api_Mst_Mission &v, pksd->start2data.api_mst_mission)
	{
		if (v.api_id == missionid)
		{
			return &v;
		}
	}
	qDebug("No mst mission data.");
	return 0;
}

void KanDataConnector::updateBattle(const kcsapi_battle &api_battle, int type)
{
	int dockid = api_battle.api_dock_id - 1;

	// midnight
	if (type == KANBATTLETYPE_NIGHT || dockid < 0)
	{
		dockid = api_battle.api_deck_id - 1;
	}

	if (dockid >= 0)
	{
		KanSaveData * pksd = &KanSaveData::getInstance();
		pksd->lastdeckid = dockid;

		QList<kcsapi_ship2*> pships;
		foreach(int shipid, pksd->portdata.api_deck_port[dockid].api_ship)
		{
			pships.append(findShipFromShipno(shipid));
		}
		QList<float> totalfdamage;
		for (int i = 0; i < 7; i++)
		{
			totalfdamage.append(0);
		}
		QList<float> totaledamage;
		for (int i = 0; i < 7; i++)
		{
			totaledamage.append(0);
		}

		QList<int> api_nowhps = api_battle.api_nowhps;
		//TODO: formation
		int stageflagcount = api_battle.api_stage_flag.count();
		if (stageflagcount >= 3)
		{
			//　航空ダメージ
			if (api_battle.api_stage_flag[2] > 0)
			{
				for (int i = 1; i < api_battle.api_kouku.api_stage3.api_fdam.count(); i++)
				{
					totalfdamage[i] += api_battle.api_kouku.api_stage3.api_fdam[i];
				}
				for (int i = 1; i < api_battle.api_kouku.api_stage3.api_edam.count(); i++)
				{
					totaledamage[i] += api_battle.api_kouku.api_stage3.api_edam[i];
				}
			}
		}

		if (type != KANBATTLETYPE_NIGHT)
		{
			// support
			if (api_battle.api_support_flag > 0)
			{
				switch (api_battle.api_support_flag)
				{
				case 1:	// kubaku
				{
					int supportstageflagcount = api_battle.api_support_info.api_support_airatack.api_stage_flag.count();
					if (supportstageflagcount >= 3)
					{
						if (api_battle.api_support_info.api_support_airatack.api_stage_flag[2] > 0)
						{
							for (int i = 1; i < api_battle.api_support_info.api_support_airatack.api_stage3.api_edam.count(); i++)
							{
								totaledamage[i] += api_battle.api_support_info.api_support_airatack.api_stage3.api_edam[i];
							}
						}
					}

				}
					break;
				case 2: // hougeki
				case 3: // raigeki
				{
					for (int i = 1; i < api_battle.api_support_info.api_support_hourai.api_damage.count(); i++)
					{
						totaledamage[i] += api_battle.api_support_info.api_support_hourai.api_damage[i];
					}
				}
					break;
				default:
					break;
				}
			}

			// opening
			if (api_battle.api_opening_flag > 0)
			{
				for (int i = 0; i < api_battle.api_opening_atack.api_fdam.count(); i++)
				{
					totalfdamage[i] += api_battle.api_opening_atack.api_fdam[i];
				}
				for (int i = 0; i < api_battle.api_opening_atack.api_edam.count(); i++)
				{
					totaledamage[i] += api_battle.api_opening_atack.api_edam[i];
				}
			}

			// hourai
			int houraiflagcount = api_battle.api_hourai_flag.count();
			if (houraiflagcount >= 4)
			{
				if (api_battle.api_hourai_flag[0] > 0)
				{
					processHouraiDamages(&(api_battle.api_hougeki1), &totalfdamage, &totaledamage);
				}
				if (api_battle.api_hourai_flag[1] > 0)
				{
					processHouraiDamages(&(api_battle.api_hougeki2), &totalfdamage, &totaledamage);
				}
				if (api_battle.api_hourai_flag[2] > 0)
				{
					processHouraiDamages(&(api_battle.api_hougeki3), &totalfdamage, &totaledamage);
				}
				// raigeki
				if (api_battle.api_hourai_flag[3] > 0)
				{
					for (int i = 0; i < api_battle.api_raigeki.api_fdam.count(); i++)
					{
						totalfdamage[i] += api_battle.api_raigeki.api_fdam[i];
					}
					for (int i = 0; i < api_battle.api_raigeki.api_edam.count(); i++)
					{
						totaledamage[i] += api_battle.api_raigeki.api_edam[i];
					}
				}
			}
		}

		else
		{
			// midnight
			processHouraiDamages(&(api_battle.api_hougeki), &totalfdamage, &totaledamage);
		}

		for (int i = 0; i < pships.count(); i++)
		{
			if (pships[i])
			{
				pships[i]->api_nowhp -= (int)totalfdamage[i + 1];
			}
		}
		updateFleetTable();
		//TODO: record battle

		QList<int> enemyhps;
		enemyhps.append(0);
		for (int i = 1; i < api_battle.api_ship_ke.count(); i++)
		{
			int nowhp = api_battle.api_nowhps[i + 6] - (int)totaledamage[i];
			enemyhps.append(nowhp);
		}

		updateInfoTitle(true, &enemyhps);
	}
}

void KanDataConnector::processHouraiDamages(const kcsapi_battle_hougeki* api_hougeki, QList<float>* totalfdamage, QList<float>* totaledamage)
{
	// must skip 0!!!
	for (int j = 1; j < api_hougeki->api_at_list.count(); j++)
	{
		bool bfattack = true;
		bool bfdefend = true;

		int attackpos = api_hougeki->api_at_list[j];
		if (attackpos > 6)
		{
			bfattack = false;
			attackpos -= 6;
		}

		if (attackpos > 0)
		{
			for (int k = 0; k < api_hougeki->api_df_list[j].count(); k++)
			{
				int defendpos = api_hougeki->api_df_list[j][k];
				if (defendpos > 6)
				{
					bfdefend = false;
					defendpos -= 6;
				}

				if (defendpos > 0)
				{
					if (bfdefend)
					{
						(*totalfdamage)[defendpos] += api_hougeki->api_damage[j][k];
					}
					else
					{
						(*totaledamage)[defendpos] += api_hougeki->api_damage[j][k];
					}
				}
			}
		}

	}
}

void KanDataConnector::checkWoundQuit()
{
	KanSaveData * pksd = &KanSaveData::getInstance();

	int lastdeckid = pksd->lastdeckid;
	bool bClose = false;
	if (lastdeckid >= 0 && lastdeckid < 4)
	{
		foreach(int shipno, pksd->portdata.api_deck_port[lastdeckid].api_ship)
		{
			const kcsapi_ship2 * pship = findShipFromShipno(shipno);
			if (pship)
			{
				if (KanDataCalc::GetWoundState(pship->api_nowhp, pship->api_maxhp) > WOUNDSTATE_MIDDLE)
				{
					if (pship->api_locked > 0 || pship->api_lv > 2)
					{
						bClose = true;
						break;
					}
				}
			}
		}
	}
	if (bClose)
	{
		MainWindow::mainWindow()->close();
	}
}
