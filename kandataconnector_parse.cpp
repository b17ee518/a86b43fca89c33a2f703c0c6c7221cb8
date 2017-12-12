#include "kandataconnector.h"
#include "kandata.h"
#include "kansavedata.h"
#include <QJsonDocument>
#include <QJsonObject>

#include "mainwindow.h"
#include "klog.h"

#include "ControlManager.h"

bool questDataSort(const kcsapi_quest &left, const kcsapi_quest &right)
{
	return left.api_no < right.api_no;
}

bool KanDataConnector::start2_parse()
{

	pksd->start2data.ReadFromJObj(_jobj);
	// mst table no update

	return true;
}

bool KanDataConnector::port_port_parse()
{
	pksd->portdata.ReadFromJObj(_jobj);
	pksd->shipcountoffset = 0;
	// material, deck, ndock, ship2, basic
	updateOverviewTable();
	updateMissionTable();
	updateFleetTable();
	updateRepairTable();

	updateRepairDockTable();
	updateExpeditionTable();

	pksd->nextdata.api_no = -1;
	updateInfoTitleBattle();
	updateInfoTitleCond();

	static bool firstTime = true;
	if (firstTime)
	{
		updateWeaponTable();
		firstTime = false;
	}

	if (isAutoRepairing())
	{
		MainWindow::mainWindow()->timerWindow()->setAutoRepairTime(true, true);
	}

	ControlManager::getInstance().clearPortDataDirtyFlag();

	return true;
}

bool KanDataConnector::get_member_basic_parse()
{
	pksd->portdata.api_basic.ReadFromJObj(_jobj);
	updateOverviewTable();
	return true;
}

bool KanDataConnector::get_member_require_info_parse()
{
	// api_basic! do not update
	//	pksd->portdata.api_basic.ReadFromJObj(_jobj["api_basic"].toObject());

	// api_slot_item
	{
		pksd->slotitemdata.clear();
		QJsonArray jarray = _jobj["api_slot_item"].toArray();
		for (int i = 0; i < jarray.count(); i++)
		{
			kcsapi_slotitem api_slotitem;
			api_slotitem.ReadFromJObj(jarray[i].toObject());
			pksd->slotitemdata.append(api_slotitem);

			if (api_slotitem.api_id > pksd->maxslotitemid)
			{
				pksd->maxslotitemid = api_slotitem.api_id;
			}
		}
		pksd->slotitemcountoffset = 0;
	}

	// api_unsetslot: ignored

	// api_kdock
	{
		pksd->kdockdata.clear();
		QJsonArray jarray = _jobj["api_kdock"].toArray();
		for (int i = 0; i < jarray.count(); i++)
		{
			kcsapi_kdock api_kdock;
			api_kdock.ReadFromJObj(jarray[i].toObject());
			pksd->kdockdata.append(api_kdock);
		}
	}

	updateBuildDockTable();

	if (pksd->createshipdata.isValueSet())
	{
		logBuildResult();
		pksd->createshipdata.clearValue();
	}

	// api_use_item
	// api_furniture

	updateOverviewTable();
	updateWeaponTable();

	return true;
}


bool KanDataConnector::get_member_ship_parse()
{
	kcsapi_ship api_ship;
	api_ship.ReadFromJObj(_jobj);
	if (api_ship.api_id > 0)
	{
		kcsapi_ship2 * pship = findShipFromShipno(api_ship.api_id);
		if (pship)
		{
			pship->ReadFromShip(api_ship);
			updateFleetTable();
			updateRepairTable();
			updateInfoTitleCond();
		}
	}
	return true;
}

bool KanDataConnector::get_member_ship2_parse()
{
	pksd->portdata.api_ship.clear();
	QJsonArray jarray = _jdoc.object()["api_data"].toArray();
	for (int i = 0; i < jarray.count(); i++)
	{
		kcsapi_ship2 api_ship2item;
		api_ship2item.ReadFromJObj(jarray[i].toObject());
		pksd->portdata.api_ship.append(api_ship2item);
	}

	pksd->shipcountoffset = 0;

	updateOverviewTable();
	updateFleetTable();
	updateRepairTable();
	updateInfoTitleCond();
	return true;
}

bool KanDataConnector::get_member_ship3_parse()
{
	kcsapi_ship3 api_ship3;
	api_ship3.ReadFromJObj(_jobj);

	//        pksd->portdata.api_ship = api_ship3.api_ship_data;
	foreach(const kcsapi_ship2 &v, api_ship3.api_ship_data)
	{
		QList<kcsapi_ship2>::iterator it;
		for (it = pksd->portdata.api_ship.begin(); it != pksd->portdata.api_ship.end(); ++it)
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
	updateInfoTitleCond();
	return true;
}

// same as ship3
bool KanDataConnector::get_member_ship_deck_parse()
{
	kcsapi_ship_deck api_ship_deck;
	api_ship_deck.ReadFromJObj(_jobj);

	//        pksd->portdata.api_ship = api_ship_deck.api_ship_data;
	foreach(const kcsapi_ship2 &v, api_ship_deck.api_ship_data)
	{
		QList<kcsapi_ship2>::iterator it;
		for (it = pksd->portdata.api_ship.begin(); it != pksd->portdata.api_ship.end(); ++it)
		{
			if (v.api_id == it->api_id)
			{
				(*it) = v;
			}
		}
	}

	foreach(const kcsapi_deck &v, api_ship_deck.api_deck_data)
	{
		QList<kcsapi_deck>::iterator it;
		for (it = pksd->portdata.api_deck_port.begin(); it != pksd->portdata.api_deck_port.end(); ++it)
		{
			if (v.api_id == it->api_id)
			{
				(*it) = v;
			}
		}
	}

	updateOverviewTable();
	updateFleetTable();
	updateRepairTable();

	pksd->shipcountoffset = 0;
	updateExpeditionTable();
	updateInfoTitleCond();
	return true;
}

bool KanDataConnector::get_member_slot_item_parse()
{
	pksd->slotitemdata.clear();
	QJsonArray jarray = _jdoc.object()["api_data"].toArray();
	for (int i = 0; i < jarray.count(); i++)
	{
		kcsapi_slotitem api_slotitem;
		api_slotitem.ReadFromJObj(jarray[i].toObject());
		pksd->slotitemdata.append(api_slotitem);

		if (api_slotitem.api_id > pksd->maxslotitemid)
		{
			pksd->maxslotitemid = api_slotitem.api_id;
		}
	}

	pksd->slotitemcountoffset = 0;

	updateOverviewTable();
	updateWeaponTable();
	//TODO: update tootip?
	return true;
}

bool KanDataConnector::get_member_useitem_parse()
{
	return true;
}

bool KanDataConnector::get_member_deck_parse()
{
	pksd->portdata.api_deck_port.clear();
	QJsonArray jarray = _jdoc.object()["api_data"].toArray();
	for (int i = 0; i < jarray.count(); i++)
	{
		kcsapi_deck api_deck;
		api_deck.ReadFromJObj(jarray[i].toObject());
		pksd->portdata.api_deck_port.append(api_deck);
	}
	updateOverviewTable();
	updateFleetTable();
	updateRepairTable();
	updateInfoTitleCond();
	return true;
}

bool KanDataConnector::get_member_deck_port_parse()
{
	return get_member_deck_parse();
}

bool KanDataConnector::get_member_ndock_parse()
{
	pksd->portdata.api_ndock.clear();
	QJsonArray jarray = _jdoc.object()["api_data"].toArray();
	for (int i = 0; i < jarray.count(); i++)
	{
		kcsapi_ndock api_ndock;
		api_ndock.ReadFromJObj(jarray[i].toObject());
		pksd->portdata.api_ndock.append(api_ndock);
	}

	updateOverviewTable();
	updateFleetTable();
	updateRepairTable();

	updateRepairDockTable();
	return true;
}

bool KanDataConnector::get_member_kdock_parse()
{
	pksd->kdockdata.clear();
	QJsonArray jarray = _jdoc.object()["api_data"].toArray();
	for (int i = 0; i < jarray.count(); i++)
	{
		kcsapi_kdock api_kdock;
		api_kdock.ReadFromJObj(jarray[i].toObject());
		pksd->kdockdata.append(api_kdock);
	}

	updateBuildDockTable();

	if (pksd->createshipdata.isValueSet())
	{
		logBuildResult();
		pksd->createshipdata.clearValue();
	}
	return true;
}

bool KanDataConnector::get_member_material_parse()
{
	pksd->portdata.api_material.clear();
	QJsonArray jarray = _jdoc.object()["api_data"].toArray();
	for (int i = 0; i < jarray.count(); i++)
	{
		kcsapi_material api_material;
		api_material.ReadFromJObj(jarray[i].toObject());
		pksd->portdata.api_material.append(api_material);
	}

	updateOverviewTable();
	return true;
}

bool KanDataConnector::req_hensei_change_parse()
{
	int team = _req.GetItemAsString("api_id").toInt();
	int index = _req.GetItemAsString("api_ship_idx").toInt();
	int shipid = _req.GetItemAsString("api_ship_id").toInt();

	QList<int> * lstship = &(pksd->portdata.api_deck_port[team - 1].api_ship);

	while (lstship->count() < KanSaveData::maxSingleTeamSize)
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
		while (lstship->count() < KanSaveData::maxSingleTeamSize)
		{
			lstship->append(-1);
		}
	}
	else
	{
		int prev = -1;
		//		int previndex = -1;

		bool bDone = false;

		// shipid was in some team (including self)
		int teamcount = pksd->portdata.api_deck_port.count();
		for (int i = 0; i < teamcount; i++)
		{
			QList<int> * lstteamship = &(pksd->portdata.api_deck_port[i].api_ship);
			while (lstteamship->count() < KanSaveData::maxSingleTeamSize)
			{
				lstteamship->append(-1);
			}
			int teamshipcount = lstteamship->count();
			for (int j = 0; j < teamshipcount; j++)
			{
				if (lstteamship->at(j) == shipid)
				{
					// swap with current
					if (lstship->count() >= index + 1)
					{
						prev = lstship->at(index);
						if (prev < 0)
						{
							lstteamship->removeAt(j);
							while (lstteamship->count() < KanSaveData::maxSingleTeamSize)
							{
								lstteamship->append(-1);
							}
							if (i == team - 1)
							{
								index--;
							}
						}
						else
						{
							(*lstteamship)[j] = prev;
						}
						if (index >= 0 && index < lstship->count())
						{
							(*lstship)[index] = shipid;
						}
						bDone = true;
						break;
					}
				}
			}
			if (bDone)
			{
				break;
			}
		}

		if (!bDone)
		{
			// shipid was free
			(*lstship)[index] = shipid;

			/*
			if (lstship->count() >= index + 1)
			{
			prev = (*lstship)[index];
			for (int i = 0; i < lstship->count(); i++)
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
			*/
		}
	}
	updateFleetTable();

	// assume only fleet 1
	if (team == 1)
	{
		bool bAutoRepairing = false;
		// has auto repair slotitem
		if (lstship->size())
		{
			if (isAutoRepairing(lstship->at(0)))
			{
				bAutoRepairing = true;
			}
		}
		MainWindow::mainWindow()->timerWindow()->setAutoRepairTime(bAutoRepairing);
	}
	return true;
}

bool KanDataConnector::req_hensei_lock_parse()
{
	int shipid = _req.GetItemAsString("api_ship_id").toInt();
	kcsapi_hensei_lock api_hensei_lock;
	api_hensei_lock.ReadFromJObj(_jobj);
	kcsapi_ship2 * pship = findShipFromShipno(shipid);
	if (pship)
	{
		pship->api_locked = api_hensei_lock.api_locked;
	}
	return true;
}

bool KanDataConnector::req_hensei_preset_register_parse()
{
	return true;
}

bool KanDataConnector::req_hensei_preset_select_parse()
{
	kcsapi_hensei_preset_select api_hensei_preset_select;
	api_hensei_preset_select.ReadFromJObj(_jobj);

	int team = api_hensei_preset_select.api_id;

	if (api_hensei_preset_select.api_ship.size())
	{
		QList<int> * lstship = &(pksd->portdata.api_deck_port[team - 1].api_ship);
		lstship->clear();
		for (auto shipid : api_hensei_preset_select.api_ship)
		{
			lstship->append(shipid);
		}
		updateFleetTable();

		// assume only fleet 1
		if (team == 1)
		{
			bool bAutoRepairing = false;
			// has auto repair slotitem
			if (lstship->size())
			{
				if (isAutoRepairing(lstship->at(0)))
				{
					bAutoRepairing = true;
				}
			}
			MainWindow::mainWindow()->timerWindow()->setAutoRepairTime(bAutoRepairing);
		}
	}

	return true;
}

bool KanDataConnector::req_hensei_preset_delete_parse()
{
	return true;
}


bool KanDataConnector::get_member_preset_deck_parse()
{
	// hensei
	ControlManager::getInstance().setPortDataDirty();
	return true;
}

bool KanDataConnector::req_hensei_combined_parse()
{
	return true;
}

bool KanDataConnector::get_member_unsetslot_parse()
{
	// slotitem that does not on
	//	kcsapi_slot_data api_unsetslot_data;
	//	api_unsetslot_data.ReadFromJObj(jobj);
	return true;
}

bool KanDataConnector::req_kaisou_unsetslot_all_parse()
{
	int shipno = _req.GetItemAsString("api_id").toInt();
	auto pship = findShipFromShipno(shipno);
	if (pship)
	{
		for (int i = 0; i < pship->api_slot.size(); i++)
		{
			pship->api_slot[i] = -1;
		}
	}

	updateWeaponTable();
	updateFleetTable();
	return true;
}

bool KanDataConnector::req_kousyou_getship_parse()
{
	int kdock_id = _req.GetItemAsString("api_kdock_id").toInt();
	pksd->kdockdata[kdock_id - 1].api_created_ship_id = 0;

	kcsapi_kdock_getship api_kdock_getship;
	api_kdock_getship.ReadFromJObj(_jobj);
	foreach(const kcsapi_slotitem &v, api_kdock_getship.api_slotitem)
	{
		if (v.api_slotitem_id >= 0)
		{
			AddSlotItem(v);
			//pksd->slotitemcountoffset++;
		}
	}

	//	pksd->shipcountoffset++;
	AddShip(api_kdock_getship.api_ship);

	updateOverviewTable();
	updateBuildDockTable();
	updateWeaponTable();
	return true;
}

bool KanDataConnector::req_kousyou_createitem_parse()
{
	kcsapi_createitem api_createitem;
	api_createitem.ReadFromJObj(_jobj);
	if (api_createitem.api_create_flag == 1)
	{
		AddSlotItem(api_createitem.api_slot_item);
		//pksd->slotitemcountoffset++;
	}
	for (int i = 0; i < api_createitem.api_material.count(); i++)
	{
		pksd->portdata.api_material[i].api_value = api_createitem.api_material[i];
	}

	updateOverviewTable();
	updateWeaponTable();

	int usefuel = _req.GetItemAsString("api_item1").toInt();
	int usebull = _req.GetItemAsString("api_item2").toInt();
	int usesteel = _req.GetItemAsString("api_item3").toInt();
	int usebauxite = _req.GetItemAsString("api_item4").toInt();
	//TODO: adjust item?
	logCreateItemResult(api_createitem.api_slot_item.api_slotitem_id, usefuel, usebull, usesteel, usebauxite);

	return true;
}

bool KanDataConnector::req_kousyou_createship_parse()
{
	int usefuel = _req.GetItemAsString("api_item1").toInt();
	int usebull = _req.GetItemAsString("api_item2").toInt();
	int usesteel = _req.GetItemAsString("api_item3").toInt();
	int usebauxite = _req.GetItemAsString("api_item4").toInt();
	int usedev = _req.GetItemAsString("api_item5").toInt();
	int bLarge = _req.GetItemAsString("api_large_flag").toInt();
	int kdockid = _req.GetItemAsString("api_kdock_id").toInt();
	int highspeed = _req.GetItemAsString("api_highspeed").toInt();
	if (bLarge)
	{
		DAPILOG();
	}

	pksd->portdata.api_material[(int)MaterialDataIndex::Fuel].api_value -= usefuel;
	pksd->portdata.api_material[(int)MaterialDataIndex::Bullet].api_value -= usebull;
	pksd->portdata.api_material[(int)MaterialDataIndex::Steel].api_value -= usesteel;
	pksd->portdata.api_material[(int)MaterialDataIndex::Bauxite].api_value -= usebauxite;
	pksd->portdata.api_material[(int)MaterialDataIndex::Development].api_value -= usedev;
	if (highspeed)
	{
		pksd->portdata.api_material[(int)MaterialDataIndex::InstandBuild].api_value -= 1;
		updateOverviewTable();
	}

	pksd->createshipdata.setValue(usefuel, usebull, usesteel, usebauxite, usedev, kdockid);

	return true;
}

bool KanDataConnector::req_kousyou_createship_speedchange_parse()
{
	int dockid = _req.GetItemAsString("api_kdock_id").toInt() - 1;
	if (dockid >= 0)
	{
		pksd->kdockdata[dockid].api_complete_time = 0;
		updateBuildDockTable();
		pksd->portdata.api_material[(int)MaterialDataIndex::InstandBuild].api_value -= 1;
		updateOverviewTable();
	}
	return true;
}

bool KanDataConnector::req_kousyou_destroyship_parse()
{
	QString idsstr = _req.GetItemAsString("api_ship_id");
	QStringList idslst = idsstr.split("%2C");

	foreach(auto& v, idslst)
	{
		int itemid = v.toInt();
		RemoveShip(itemid);
	}
	//	pksd->slotitemcountoffset -= idslst.count();

	kcsapi_destroyitem2 api_destroyitem2;
	api_destroyitem2.ReadFromJObj(_jobj);
	QList<int> api_get_material = api_destroyitem2.api_get_material;
	for (int i = 0; i < api_get_material.count(); i++)
	{
		pksd->portdata.api_material[i].api_value += api_get_material[i];
	}
	updateOverviewTable();
	updateFleetTable();

	updateRepairTable();
	updateWeaponTable();
	ControlManager::getInstance().setPortDataDirty();
	return true;
}

bool KanDataConnector::req_kousyou_destroyitem2_parse()
{
	QString idsstr = _req.GetItemAsString("api_slotitem_ids");
	QStringList idslst = idsstr.split("%2C");

	foreach(auto& v, idslst)
	{
		int itemid = v.toInt();
		RemoveSlotItem(itemid);
	}
	//	pksd->slotitemcountoffset -= idslst.count();


	kcsapi_destroyitem2 api_destroyitem2;
	api_destroyitem2.ReadFromJObj(_jobj);
	QList<int> api_get_material = api_destroyitem2.api_get_material;
	for (int i = 0; i < api_get_material.count(); i++)
	{
		pksd->portdata.api_material[i].api_value += api_get_material[i];
	}
	updateOverviewTable();
	updateWeaponTable();
	return true;
}

bool KanDataConnector::req_nyukyo_start_parse()
{
	int highspeed = _req.GetItemAsString("api_highspeed").toInt();
	int shipno = _req.GetItemAsString("api_ship_id").toInt();
	if (highspeed && shipno > 0)
	{
		kcsapi_ship2 * pship = findShipFromShipno(shipno);
		if (pship)
		{
			pship->api_nowhp = pship->api_maxhp;
			pship->api_ndock_time = 0;
			updateFleetTable();
			updateRepairTable();
		}
		pksd->portdata.api_material[(int)MaterialDataIndex::InstantRepair].api_value -= 1;
		updateOverviewTable();
	}
	return true;
}

bool KanDataConnector::req_nyukyo_speedchange_parse()
{
	int dockid = _req.GetItemAsString("api_ndock_id").toInt() - 1;
	if (dockid >= 0)
	{
		int shipno = pksd->portdata.api_ndock[dockid].api_ship_id;
		if (shipno > 0)
		{
			pksd->portdata.api_ndock[dockid].api_ship_id = 0;
			kcsapi_ship2 * pship = findShipFromShipno(shipno);
			if (pship)
			{
				pship->api_nowhp = pship->api_maxhp;
				pship->api_ndock_time = 0;
				updateRepairTable();
			}
			updateRepairDockTable();
			pksd->portdata.api_material[(int)MaterialDataIndex::InstantRepair].api_value -= 1;
			updateOverviewTable();
		}
	}
	return true;
}

bool KanDataConnector::req_hokyu_charge_parse()
{
	kcsapi_charge api_charge;
	api_charge.ReadFromJObj(_jobj);

	foreach(const kcsapi_charge_ship &v, api_charge.api_ship)
	{
		QList<kcsapi_ship2>::iterator it;
		for (it = pksd->portdata.api_ship.begin(); it != pksd->portdata.api_ship.end(); ++it)
		{
			if (it->api_id == v.api_id)
			{
				it->api_fuel = v.api_fuel;
				it->api_bull = v.api_bull;
				break;
			}
		}
	}

	for (int i = 0; i < api_charge.api_material.count(); i++)
	{
		pksd->portdata.api_material[i].api_value = api_charge.api_material[i];
	}

	updateFleetTable();
	return true;
}

bool KanDataConnector::req_kaisou_powerup_parse()
{
	QStringList shipsids = _req.GetItemAsString("api_id_items").split("%2C");
	foreach(const QString &v, shipsids)
	{
		int shipno = v.toInt();
		RemoveShip(shipno);
	}

	kcsapi_powerup api_powerup;
	api_powerup.ReadFromJObj(_jobj);
	pksd->portdata.api_deck_port = api_powerup.api_deck;

	updateOverviewTable();
	updateFleetTable();
	return true;
}

bool KanDataConnector::req_kaisou_slotset_parse()
{
	int shipno = _req.GetItemAsString("api_id").toInt();
	int slotindex = _req.GetItemAsString("api_slot_idx").toInt();
	int itemid = _req.GetItemAsString("api_item_id").toInt();

	auto pship = findShipFromShipno(shipno);
	if (pship)
	{
		pship->api_slot[slotindex] = itemid;
		updateWeaponTable();
		updateFleetTable();
		return true;
	}

	return false;
}

bool KanDataConnector::req_kaisou_slot_exchange_index_parse()
{
	// ignore
	return true;
}

bool KanDataConnector::req_kaisou_remodeling_parse()
{
	//TODO
	return true;
}

bool KanDataConnector::req_kaisou_slot_deprive_parse()
{
	kcsapi_kaisou_deprive api_ship_data;
	api_ship_data.ReadFromJObj(_jobj);

	QList<kcsapi_ship2>::iterator it;
	int doneCount = 0;
	for (it = pksd->portdata.api_ship.begin(); it != pksd->portdata.api_ship.end(); ++it)
	{
		if (api_ship_data.api_ship_data.api_set_ship.api_id == it->api_id)
		{
			*it = api_ship_data.api_ship_data.api_set_ship;
			doneCount++;
		}
		else if (api_ship_data.api_ship_data.api_unset_ship.api_id == it->api_id)
		{
			*it = api_ship_data.api_ship_data.api_unset_ship;
			doneCount++;
		}
		if (doneCount >= 2)
		{
			break;
		}
	}

	updateWeaponTable();

	return true;
}

bool KanDataConnector::req_mission_start_parse()
{
	// expedition
	kcsapi_mission_start api_mission_start;
	api_mission_start.ReadFromJObj(_jobj);

	int missionid = _req.GetItemAsString("api_mission_id").toInt();
	int deckid = _req.GetItemAsString("api_deck_id").toInt();
	if (deckid > 0)
	{
		pksd->portdata.api_deck_port[deckid - 1].api_mission[0] = 1;
		pksd->portdata.api_deck_port[deckid - 1].api_mission[1] = missionid;
		pksd->portdata.api_deck_port[deckid - 1].api_mission[2] = api_mission_start.api_complatetime;
	}
	updateExpeditionTable();
	updateInfoTitleCond();
	return true;
}

bool KanDataConnector::req_mission_result_parse()
{
	return true;
}

bool KanDataConnector::get_member_mission_parse()
{
	return true;
}

bool KanDataConnector::get_member_mapinfo_parse()
{
	QMap<int, QList<kcsapi_air_base_corps> > tcorpsMap;

	auto jarray = _jdoc.object()["api_data"].toObject()["api_air_base"].toArray();
	for (int i = 0; i < jarray.count(); i++)
	{
		auto corp = jarray[i].toObject();
		kcsapi_air_base_corps tcorp;
		tcorp.ReadFromJObj(corp);

		if (tcorpsMap[tcorp.api_area_id].isEmpty())
		{
			tcorpsMap[tcorp.api_area_id].append(kcsapi_air_base_corps());
			tcorpsMap[tcorp.api_area_id].append(kcsapi_air_base_corps());
			tcorpsMap[tcorp.api_area_id].append(kcsapi_air_base_corps());
		}
		tcorpsMap[tcorp.api_area_id][tcorp.api_rid - 1] = tcorp;
		pksd->airbasedata[tcorp.api_area_id] = tcorpsMap[tcorp.api_area_id];
	}

	checkAirBase();

	return true;
}

bool KanDataConnector::get_member_mapcell_parse()
{
	return true;
}

bool KanDataConnector::req_map_start_parse()
{
	pksd->lastdeckid = _req.GetItemAsString("api_deck_id").toInt() - 1;
	pksd->nextdata.ReadFromJObj(_jobj);

	updateInfoTitleBattle();
	checkWoundQuit();

	pksd->totalSortie++;
	updateOverviewTable();
	// for serial_id
	//	DAPILOGDEBUG();
	return true;
}

bool KanDataConnector::req_map_next_parse()
{
	pksd->nextdata.ReadFromJObj(_jobj);

	updateInfoTitleBattle();
	checkWoundQuit();
	return true;
}

bool KanDataConnector::req_sortie_battleresult_parse()
{
	pksd->battleresultdata.ReadFromJObj(_jobj);
	int shipid = pksd->battleresultdata.api_get_ship.api_ship_id;
	if (shipid > 0)
	{
		pksd->shipcountoffset++;
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

	logBattleResult();
	logBattleDetail(false);

	ControlManager::getInstance().clearPortDataDirtyFlag();

	return true;
}

bool KanDataConnector::req_sortie_battle_parse()
{
	pksd->battledata.ReadFromJObj(_jobj);

	updateBattle(pksd->battledata, KanBattleType::Day);
	return true;
}

bool KanDataConnector::req_battle_midnight_battle_parse()
{
	pksd->battledata.ReadFromJObj(_jobj);

	updateBattle(pksd->battledata, KanBattleType::DayToNight);
	return true;
}

bool KanDataConnector::req_battle_midnight_sp_midnight_parse()
{
	pksd->battledata.ReadFromJObj(_jobj);

	updateBattle(pksd->battledata, KanBattleType::Night);
	return true;
}

bool KanDataConnector::req_sortie_night_to_day_parse()
{
	//TODO night to day:
	pksd->battledata.ReadFromJObj(_jobj);

	updateBattle(pksd->battledata, KanBattleType::NightToDay);
	return true;
}

bool KanDataConnector::req_sortie_airbattle_parse()
{
	pksd->battledata.ReadFromJObj(_jobj);

	updateBattle(pksd->battledata, KanBattleType::Air);
	// TODO check air???
	return true;
}

bool KanDataConnector::req_sortie_ld_airbattle_parse()
{
	pksd->battledata.ReadFromJObj(_jobj);

	updateBattle(pksd->battledata, KanBattleType::Air);
	// TODO check air???
	return true;

}

bool KanDataConnector::req_combined_battle_airbattle_parse()
{
	pksd->battledata.ReadFromJObj(_jobj);

	updateBattle(pksd->battledata, KanBattleType::Combined_KouKu);
	return true;
}

bool KanDataConnector::req_combined_battle_ld_airbattle_parse()
{
	pksd->battledata.ReadFromJObj(_jobj);

	updateBattle(pksd->battledata, KanBattleType::Combined_KouKu);
	return true;
}

bool KanDataConnector::req_combined_battle_battlewater_parse()
{
	pksd->battledata.ReadFromJObj(_jobj);

	updateBattle(pksd->battledata, KanBattleType::Combined_Water);
	return true;
}

bool KanDataConnector::req_combined_battle_battleresult_parse()
{
	pksd->battleresultdata.ReadFromJObj(_jobj);
	int shipid = pksd->battleresultdata.api_get_ship.api_ship_id;
	if (shipid > 0)
	{
		pksd->shipcountoffset++;
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

	logBattleResult();
	logBattleDetail(true);

	ControlManager::getInstance().clearPortDataDirtyFlag();

	return true;
}

bool KanDataConnector::req_combined_battle_battle_parse()
{
	pksd->battledata.ReadFromJObj(_jobj);

	updateBattle(pksd->battledata, KanBattleType::Combined_Day);
	return true;
}

bool KanDataConnector::req_combined_battle_midnight_battle_parse()
{
	pksd->battledata.ReadFromJObj(_jobj);

	updateBattle(pksd->battledata, KanBattleType::Combined_DayToNight);
	return true;
}

bool KanDataConnector::req_combined_battle_sp_midnight_parse()
{
	pksd->battledata.ReadFromJObj(_jobj);

	updateBattle(pksd->battledata, KanBattleType::Combined_Night);
	return true;

}

bool KanDataConnector::req_combined_battle_goback_port_parse()
{
	// TODO: add flags to ship?
	return true;
}

bool KanDataConnector::req_combined_battle_ec_battle_parse()
{
	pksd->battledata.ReadFromJObj(_jobj);

	updateBattle(pksd->battledata, KanBattleType::Combined_EC);
	return true;
}

bool KanDataConnector::req_combined_battle_ec_midnight_battle_parse()
{
	pksd->battledata.ReadFromJObj(_jobj);

	updateBattle(pksd->battledata, KanBattleType::Combined_ECNight);
	return true;
}

bool KanDataConnector::req_combined_battle_ec_night_to_day_parse()
{
	pksd->battledata.ReadFromJObj(_jobj);

	updateBattle(pksd->battledata, KanBattleType::Combined_ECNightToDay);
	return true;
}

bool KanDataConnector::req_combined_battle_each_battle_parse()
{
	pksd->battledata.ReadFromJObj(_jobj);

	updateBattle(pksd->battledata, KanBattleType::Combined_Each);
	return true;
}

bool KanDataConnector::req_combined_battle_each_battle_water_parse()
{
	pksd->battledata.ReadFromJObj(_jobj);

	updateBattle(pksd->battledata, KanBattleType::Combined_EachWater);
	return true;
}

bool KanDataConnector::get_member_practice_parse()
{
	return true;
}

bool KanDataConnector::req_member_get_practice_enemyinfo_parse()
{
	return true;
}

bool KanDataConnector::req_practice_battle_parse()
{
	pksd->battledata.ReadFromJObj(_jobj);

	updateBattle(pksd->battledata, KanBattleType::Day);
	return true;
}

bool KanDataConnector::req_practice_midnight_battle_parse()
{
	pksd->battledata.ReadFromJObj(_jobj);

	updateBattle(pksd->battledata, KanBattleType::Night);
	return true;
}

bool KanDataConnector::req_practice_battle_result_parse()
{
	return true;
}

bool KanDataConnector::get_member_questlist_parse()
{
	int api_tab_id = _req.GetItemAsString("api_tab_id").toInt();
	// 0 all, 9 current, 1 daily, 2 weekly, 3 monthly, 4 once, 5 other
	// 32 ??
	// api_type matches

	// TODO: remove quest
	kcsapi_questlist api_questlist;
	api_questlist.ReadFromJObj(_jobj);

	QList<kcsapi_quest>::iterator it = api_questlist.api_list.begin();
	for (; it != api_questlist.api_list.end();)
	{
		if (it->api_no <= 0)
		{
			it = api_questlist.api_list.erase(it);
		}
		else
		{
			++it;
		}
	}

	if (!api_questlist.api_list.size())
	{
		/*
		// nothing in current
		if (api_tab_id == 0 || api_tab_id == 9)
		{
		pksd->questdata.clear();
		}
		else
		{
		*/
		// first page nothing: clear type
		if (api_questlist.api_disp_page == 1)
		{
			pksd->clearQuestByType(api_tab_id, 0, -1);
		}
		// last page nothing: clear all later
		else if (api_questlist.api_disp_page >= api_questlist.api_page_count)
		{
			pksd->clearQuestByType(api_tab_id, -1, -1, api_questlist.api_page_count);
		}
		//		}
	}
	else
	{
		int beginIndex = std::numeric_limits<int>::max();
		int endIndex = -1;
		for (const auto& quest : api_questlist.api_list)
		{
			if (quest.api_no < beginIndex)
			{
				beginIndex = quest.api_no;
			}
			if (quest.api_no > endIndex)
			{
				endIndex = quest.api_no;
			}
		}
		// TODO:? do not know if prev page's last state
		// should be ok

		// last page: remove all later
		if (api_questlist.api_disp_page >= api_questlist.api_page_count)
		{
			endIndex = -1;
		}

		// first clear all
		pksd->clearQuestByType(api_tab_id, beginIndex, endIndex);
		// then add back
		for (int i = 0; i < api_questlist.api_list.size(); i++)
		{
			if (api_questlist.api_list[i].api_state > 1)
			{
				pksd->questdata.append(api_questlist.api_list[i]);
			}
		}
		std::sort(pksd->questdata.begin(), pksd->questdata.end(), questDataSort);
	}

	if (pksd->questdata.size() > api_questlist.api_exec_count)
	{
		DAPILOG();
	}

	/*
	int questcount = api_questlist.api_count - questPerPage * (api_questlist.api_disp_page - 1);
	if (questcount > api_questlist.api_list.count())
	{
	questcount = api_questlist.api_list.count();
	}

	if (!questcount)
	{
	if (pksd->questdata.size())
	{
	pksd->questdata.removeLast();
	}
	// when last page nothing??
	}
	else
	{
	int beginindex = api_questlist.api_list[0].api_no;
	int endindex = api_questlist.api_list[questcount - 1].api_no;

	//delete all in questdata
	QList<kcsapi_quest>::iterator it;
	for (it = pksd->questdata.begin(); it != pksd->questdata.end();)
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
	for (int i = 0; i<questcount; i++)
	{
	if (api_questlist.api_list[i].api_state > 1)
	{
	pksd->questdata.append(api_questlist.api_list[i]);
	}
	}
	std::sort(pksd->questdata.begin(), pksd->questdata.end(), questDataSort);
	}
	*/

	updateMissionTable();
	return true;
}

bool KanDataConnector::req_quest_start_parse()
{
	return true;
}

bool KanDataConnector::req_quest_stop_parse()
{
	int questid = _req.GetItemAsString("api_quest_id").toInt();

	for (QList<kcsapi_quest>::iterator it = pksd->questdata.begin(); it != pksd->questdata.end(); ++it)
	{
		if (it->api_no == questid)
		{
			pksd->questdata.erase(it);
			break;
		}
	}

	updateMissionTable();
	return true;
}

bool KanDataConnector::req_quest_clearitemget_parse()
{
	kcsapi_clearitemget api_clearitemget;
	api_clearitemget.ReadFromJObj(_jobj);
	for (int i = 0; i < api_clearitemget.api_material.count(); i++)
	{
		pksd->portdata.api_material[i].api_value += api_clearitemget.api_material[i];
	}
	if (api_clearitemget.api_bounus_count)
	{
		for (int i = 0; i < api_clearitemget.api_bounus_count; i++)
		{
			auto pbonus = &(api_clearitemget.api_bounus[i]);
			if (pbonus->api_type == 1)
			{
				int index = pbonus->api_item.api_id;
				if (index < 7)
				{
					pksd->portdata.api_material[index].api_value += pbonus->api_count;
				}
			}
			else if (pbonus->api_type == 3)
			{
				// furniture box
			}
			else if (pbonus->api_type == 12)
			{
				// item
				AddSlotItem(-1, pbonus->api_item.api_id);
			}
			else if (pbonus->api_type == 13)
			{
				// mamiya/ presentbox
			}
			else if (pbonus->api_type == 14)
			{
				// ??
			}
			else if (pbonus->api_type == 15)
			{
				// change item
			}
			else if (pbonus->api_type == 99)
			{
				// ?
			}
			else
			{
				DAPILOG();
			}
			// ship?
		}
	}

	int questid = _req.GetItemAsString("api_quest_id").toInt();

	for (QList<kcsapi_quest>::iterator it = pksd->questdata.begin(); it != pksd->questdata.end(); ++it)
	{
		if (it->api_no == questid)
		{
			pksd->questdata.erase(it);
			break;
		}
	}

	updateMissionTable();
	return true;
}

bool KanDataConnector::get_member_sortie_conditions_parse()
{
	return true;
}

bool KanDataConnector::req_member_updatedeckname_parse()
{
	return true;
}

bool KanDataConnector::req_member_updatecomment_parse()
{
	return true;
}

bool KanDataConnector::req_kousyou_open_new_dock_parse()
{
	return true;
}

bool KanDataConnector::req_nyukyo_open_new_dock_parse()
{
	return true;
}

bool KanDataConnector::auth_member_logincheck_parse()
{
	return true;
}

bool KanDataConnector::get_member_furniture_parse()
{
	return true;
}

bool KanDataConnector::req_furniture_buy_parse()
{
	return true;
}

bool KanDataConnector::req_furniture_change_parse()
{
	return true;
}

bool KanDataConnector::req_member_itemuse_parse()
{
	return true;
}

bool KanDataConnector::req_member_itemuse_cond_parse()
{
	return true;
}

bool KanDataConnector::get_member_record_parse()
{
	return true;
}

bool KanDataConnector::req_ranking_getlist_parse()
{
	return true;
}

bool KanDataConnector::req_ranking_mxltvkpyuklh_parse()
{
	return true;
}

bool KanDataConnector::get_member_picture_book_parse()
{
	return true;
}

bool KanDataConnector::get_member_book2_parse()
{
	return true;
}

bool KanDataConnector::req_member_get_incentive_parse()
{
	return true;
}

bool KanDataConnector::get_member_payitem_parse()
{
	return true;
}

bool KanDataConnector::req_kaisou_lock_parse()
{
	int slotitemid = _req.GetItemAsString("api_slotitem_id").toInt();

	kcsapi_kaisou_lock api_kaisou_lock;
	api_kaisou_lock.ReadFromJObj(_jobj);
	auto pslotitem = findSlotitemFromId(slotitemid);
	if (pslotitem)
	{
		pslotitem->api_locked = api_kaisou_lock.api_locked;
		updateWeaponTable();
	}
	return true;
}

bool KanDataConnector::req_mission_return_instruction_parse()
{
	return true;
}

bool KanDataConnector::req_member_payitemuse_parse()
{
	return true;
}

bool KanDataConnector::req_kaisou_marriage_parse()
{
	return true;
}

bool KanDataConnector::req_kousyou_remodel_slotlist_parse()
{
	return true;
}

bool KanDataConnector::req_kousyou_remodel_slotlist_detail_parse()
{
	return true;
}

bool KanDataConnector::req_kousyou_remodel_slot_parse()
{
	int oslotitemid = _req.GetItemAsString("api_slot_id").toInt();

	kcsapi_remodel_slot api_remodel_slot;
	api_remodel_slot.ReadFromJObj(_jobj);
	if (oslotitemid == api_remodel_slot.api_after_slot.api_id)
	{
		auto pslotitem = findSlotitemFromId(oslotitemid);
		pslotitem->ReadFromSlotItem(api_remodel_slot.api_after_slot);
	}
	else
	{
		RemoveSlotItem(oslotitemid);
		AddSlotItem(api_remodel_slot.api_after_slot);
	}
	updateWeaponTable();

	return true;
}

bool KanDataConnector::req_furniture_music_list_parse()
{
	return true;
}

bool KanDataConnector::req_furniture_music_play_parse()
{
	return true;
}

bool KanDataConnector::req_furniture_set_portbgm_parse()
{
	return true;
}

bool KanDataConnector::req_map_select_eventmap_rank_parse()
{
	return true;
}

bool KanDataConnector::dmm_payment_paycheck_parse()
{
	return true;
}

bool KanDataConnector::req_kaisou_open_exslot_parse()
{
	return true;
}

bool KanDataConnector::req_kaisou_slotset_ex_parse()
{
	int slotitemid = _req.GetItemAsString("api_item_id").toInt();
	int shipno = _req.GetItemAsString("api_id").toInt();
	kcsapi_ship2* pship = findShipFromShipno(shipno);
	if (pship)
	{
		pship->api_slot_ex = slotitemid;
		updateWeaponTable();
		updateFleetTable();
	}
	return true;
}

bool KanDataConnector::get_member_base_air_corps_parse()
{
	QMap<int, QList<kcsapi_air_base_corps> > tcorpsMap;

	auto jarray = _jdoc.object()["api_data"].toArray();
	for (int i = 0; i < jarray.count(); i++)
	{
		auto corp = jarray[i].toObject();
		kcsapi_air_base_corps tcorp;
		tcorp.ReadFromJObj(corp);

		if (tcorpsMap[tcorp.api_area_id].isEmpty())
		{
			tcorpsMap[tcorp.api_area_id].append(kcsapi_air_base_corps());
			tcorpsMap[tcorp.api_area_id].append(kcsapi_air_base_corps());
			tcorpsMap[tcorp.api_area_id].append(kcsapi_air_base_corps());
		}
		tcorpsMap[tcorp.api_area_id][tcorp.api_rid - 1] = tcorp;
		pksd->airbasedata[tcorp.api_area_id] = tcorpsMap[tcorp.api_area_id];
	}

	checkAirBase();

	return true;
}

bool KanDataConnector::req_air_corps_set_plane_parse()
{
	//	api_base_id = 1 & api_item_id = 63064 & api_squadron_id = 4
	int baseid = _req.GetItemAsString("api_base_id").toInt();
	//	int itemid = _req.GetItemAsString("api_item_id").toInt();
	int squadronid = _req.GetItemAsString("api_squadron_id").toInt();
	int areaid = _req.GetItemAsString("api_area_id").toInt();

	while (pksd->airbasedata[areaid].size() < baseid)
	{
		pksd->airbasedata[areaid].append(kcsapi_air_base_corps());
	}

	while (pksd->airbasedata[areaid][baseid - 1].api_plane_info.size() < squadronid)
	{
		pksd->airbasedata[areaid][baseid - 1].api_plane_info.append(kcsapi_air_base_corps_plane_info());
	}

	kcsapi_air_base_corps tcorps;
	tcorps.ReadFromJObj(_jobj);
	for (const auto& item : tcorps.api_plane_info)
	{
		pksd->airbasedata[areaid][baseid - 1].api_plane_info[item.api_squadron_id - 1] = item;
	}
	pksd->airbasedata[areaid][baseid - 1].api_distance = tcorps.api_distance;

	checkAirBase();

	return true;
}

bool KanDataConnector::req_map_start_air_base_parse()
{
	return true;
}

bool KanDataConnector::req_air_corps_supply_parse()
{
	//api_base_id=1&api_squadron_id=1,2,3,4
	int baseid = _req.GetItemAsString("api_base_id").toInt();
	int areaid = _req.GetItemAsString("api_area_id").toInt();
	QList<QString> squadronIds = _req.GetItemAsString("api_squadron_id").split("%2C");

	kcsapi_air_base_corps tcorp;
	/*
	tcorp.api_plane_info.append(kcsapi_air_base_corps_plane_info());
	tcorp.api_plane_info.append(kcsapi_air_base_corps_plane_info());
	tcorp.api_plane_info.append(kcsapi_air_base_corps_plane_info());
	tcorp.api_plane_info.append(kcsapi_air_base_corps_plane_info());
	*/
	tcorp.ReadFromJObj(_jobj);

	while (pksd->airbasedata[areaid].size() < baseid)
	{
		pksd->airbasedata[areaid].append(kcsapi_air_base_corps());
	}

	for (const auto& idStr : squadronIds)
	{
		int squadronid = idStr.toInt();
		while (pksd->airbasedata[areaid][baseid - 1].api_plane_info.size() < squadronid)
		{
			pksd->airbasedata[areaid][baseid - 1].api_plane_info.append(kcsapi_air_base_corps_plane_info());
		}
		for (const auto& plane : tcorp.api_plane_info)
		{
			if (plane.api_squadron_id == squadronid)
			{
				pksd->airbasedata[areaid][baseid - 1].api_plane_info[squadronid - 1] = plane;
				break;
			}
		}
	}

	checkAirBase();

	return true;
}

bool KanDataConnector::req_air_corps_set_action_parse()
{
	return true;
}

bool KanDataConnector::req_sortie_goback_port_parse()
{
	return true;
}
