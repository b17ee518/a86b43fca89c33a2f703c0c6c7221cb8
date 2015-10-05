#include "ControlManager.h"
#include <QTestEventList>
#include "mainwindow.h"
#include <QMouseEvent>
#include <QGraphicsSceneMouseEvent>
#include "kandataconnector.h"
#include "kansavedata.h"
#include "kandatacalc.h"
#include <QtMath>

#define COL_ALLOWRANCE 3
#define COND_DAIHATSU	80

ControlManager::ControlManager()
{
}


ControlManager::~ControlManager()
{
}

bool ControlManager::BuildNext_Kira(bool bForceCurrent/*=false*/)
{
	if (stopWhenCheck())
	{
		setToTerminate("Termination:StopWhenDone");
		return false;
	}
	if (bForceCurrent)
	{
		_todoShipids.push_front(getCurrentFlagshipId());
	}
	if (_todoShipids.empty())
	{
		setToTerminate("Terminated:NoShip");
		return false;
	}
	if (flagshipSevereDamaged())
	{
		setToTerminate("Terminated:Flagship");
		return false;
	}

	_target = SortieTarget::Kira;
	if (isShipFull())
	{
		setToTerminate("Terminated:ShipFull");
		return false;
//		_actionList.append(new DestroyShipAction());
	}

	int togoShipId = _todoShipids.at(0);
	while (isShipKiraDone(togoShipId) 
		|| isShipInOtherTeam(togoShipId) 
		|| isShipInDock(togoShipId)
		|| hasSlotitem(togoShipId, SlotitemType::JouRiKuTei)
		|| hasSlotitem(togoShipId, SlotitemType::Sonar)
		|| hasSlotitem(togoShipId, SlotitemType::BaKuRai)
		|| hasSlotitem(togoShipId, SlotitemType::Sonar_L)
		|| noSlotitem(togoShipId))
	{
		_todoShipids.removeAt(0);
		if (_todoShipids.empty())
		{
			setToTerminate("Terminated:NoShip");
			return false;
		}
		togoShipId = _todoShipids.at(0);
	}

	int flagshipid = getCurrentFlagshipId();
	int flagshipIndex = _todoShipids.indexOf(flagshipid);
	if (flagshipIndex > 0 && !isShipKiraDone(flagshipid))
	{
		_todoShipids.swap(flagshipIndex, 0);
	}
	
	togoShipId = _todoShipids.at(0);
	auto chAction = new ChangeHenseiAction();

	int curSecond = getCurrentSecondshipId();
	bool bChangeSecond = true;
	bool bTogoSameToCurSecond = false;

	if (isTreatedSameShip(togoShipId, curSecond))
	{
		// must exclude second first
		auto preChAction = new ChangeHenseiAction();
		preChAction->setShips(togoShipId, -1);
		_actionList.append(preChAction);
		bTogoSameToCurSecond = true;
	}

	if (!shouldChangeSecondShip() && !bTogoSameToCurSecond)
	{
		bChangeSecond = false;
	}

	int wasteId = curSecond;
	if (bChangeSecond)
	{
		wasteId = getOneWasteShipId(togoShipId, getCurrentFlagshipId());
	}
	chAction->setShips(togoShipId, wasteId);
	_actionList.append(chAction);

	_actionList.append(new SortieAction());
	_actionList.append(new SortieAdvanceAction());
	_actionList.append(new ChargeAction());
	_actionList.append(new RepeatAction());

	setState(State::Ready, "Ready");
	return true;
}

bool ControlManager::BuildNext_Fuel()
{
	// for repeat action
	/*
	if (!_actionList.empty())
	{
		return;
	}
	*/

	if (stopWhenCheck())
	{
		setToTerminate("Termination:StopWhenDone");
		return false;
	}
	if (!isFlagshipOnly())
	{
		// southeast
		return BuildNext_SouthEast();
	}

	if (flagshipSevereDamaged())
	{
		setToTerminate("Terminated:Flagship");
		return false;
	}

	int flagshipid = getCurrentFlagshipId();
	if (!isShipType(flagshipid, ShipType::SenSui))
	{
		setToTerminate("Terminated:Ship type");
		return false;
	}

	if (isShipFull())
	{
		setToTerminate("Terminated:ShipFull");
		return false;
//		_actionList.append(new DestroyShipAction());
	}
	
	_target = SortieTarget::Fuel;

	_actionList.append(new SortieAction());
	_actionList.append(new SortieAdvanceAction());
	_actionList.append(new ChargeAction());
	_actionList.append(new RepeatAction());

	setState(State::Ready, "Ready");
	return true;
}

bool ControlManager::BuildNext_SouthEast()
{
	if (stopWhenCheck())
	{
		setToTerminate("Termination:StopWhenDone");
		return false;
	}
	_southEastTeamSize = getTeamSize();
	KanSaveData* pksd = &KanSaveData::getInstance();
	if (pksd->totalSouthEastWin >= 5)
	{
		setToTerminate("Terminated:Done Mission");
		return false;
	}
	if (_southEastTeamSize < 4)
	{
		// keep 4 and up for formation
		_southEastTeamSize = 4;
	}

	if (isShipFull())
	{
		setToTerminate("Terminated:ShipFull");
		return false;
		//		_actionList.append(new DestroyShipAction());
	}

	if (_ssShips.isEmpty())
	{
		createSSShipList();
	}

	// find possible hensei
	KanDataConnector* pkdc = &KanDataConnector::getInstance();
	// find best in each group
	QList<kcsapi_ship2*> bestShips;
	for (auto& group : _ssShips)
	{
		kcsapi_ship2* pbestship = nullptr;
		for (auto ssid:group)
		{
			auto pcurship = pkdc->findShipFromShipno(ssid);
			// cond ng later
			/*
			if (pcurship->api_cond <= _sortieMinCond)
			{
				continue;
			}
			*/
			// damage ng
			if (KanDataCalc::GetWoundState(pcurship->api_nowhp, pcurship->api_maxhp) >= WoundState::Middle)
			{
				continue;
			}
			// in dock
			if (isShipInDock(ssid))
			{
				continue;
			}
			// in other team
			if (isShipInOtherTeam(ssid))
			{
				continue;
			}

			if (!pbestship)
			{
				pbestship = pcurship;
				continue;
			}
			if (pcurship->api_cond > pbestship->api_cond)
			{
				pbestship = pcurship;
			}
		}
		if (pbestship)
		{
			bestShips.append(pbestship);
		}
	}

	if (bestShips.size() < _southEastTeamSize)
	{
		setToTerminate("Terminated:No Enough Ship");
		return false;
	}

	// sort bestships
	qSort(bestShips.begin(), bestShips.end(), [](const  kcsapi_ship2* left, const kcsapi_ship2* right)
	{
		return left->api_cond > right->api_cond; // cond from high to low
	});

	int minCond = bestShips.at(_southEastTeamSize - 1)->api_cond;
	if (minCond <= _sortieMinCond)
	{
		// wait
		_waitMS = qCeil((_sortieWaitCond - minCond) / 3.0) * 3000;
		_actionList.append(new WaitCondAction());
	}

	QList<int> ships;
	for (int i = 0; i < _southEastTeamSize; i++)
	{
		ships.push_front(bestShips.at(i)->api_id);
	}
	
	// sort in team first
	QList<int> sortInTeamShips;

	QList<int> nonEmptyShipList;
	if (pksd->portdata.api_deck_port.size())
	{
		auto& firstFleet = pksd->portdata.api_deck_port.first();
		for (auto id : firstFleet.api_ship)
		{
			if (id >= 0)
			{
				nonEmptyShipList.append(id);
			}
		}
	}
	for (int shipno : ships)
	{
		bool bDone = false;
		QList<int>* pgroup = NULL;
		for (auto& group:_ssShips)
		{
			if (group.contains(shipno))
			{
				pgroup = &group;
				break;
			}
		}
		if (!pgroup)
		{
			setToTerminate("Terminated:Wrong SS Data");
			return false;
		}

		for (int gpshipno:(*pgroup))
		{
			for (int neshipno : nonEmptyShipList)
			{
				// already in team 1
				if (gpshipno == neshipno)
				{
					sortInTeamShips.append(gpshipno);
					bDone = true;
					break;
				}
			}
			if (bDone)
			{
				break;
			}
		}

		if (!bDone) //none in team 1
		{
			sortInTeamShips.append(shipno);
		}
	}


	_target = SortieTarget::SouthEast;

	// change hensei sort
	auto chSortAction = new ChangeHenseiAction();
	chSortAction->setShips(sortInTeamShips);
	_actionList.append(chSortAction);

	// change hensei
	auto chAction = new ChangeHenseiAction();
	chAction->setShips(ships);
	_actionList.append(chAction);
	
	_actionList.append(new SortieAction());
	_actionList.append(new SortieCommonAdvanceAction());
	_actionList.append(new ChargeAction());
	_actionList.append(new RepeatAction());
	
	setState(State::Ready, "Ready");
	return true;
}

bool ControlManager::stopWhenCheck()
{

	switch (_stopwhen)
	{
	case ControlManager::StopWhen::None:
		return false;
		break;
	case ControlManager::StopWhen::Yusou3:
		if (KanSaveData::getInstance().totalKilledYusou >= 3)
		{
			return true;
		}
		else
		{
			return false;
		}
		break;
	case ControlManager::StopWhen::SouthEast5:
		if (KanSaveData::getInstance().totalSouthEastWin >= 5)
		{
			return true;
		}
		else
		{
			return false;
		}
		break;
	default:
		break;
	}
	return true;
}

void ControlManager::StartJob()
{
	if (_state == State::Ready)
	{
		setState(State::Started, "Started");
	}
}

void ControlManager::setDoneRequest(const QString& api)
{
	if (_state != State::Started)
	{
		return;
	}
	if (_actionList.empty())
	{
		return;
	}
	_actionList[0]->setDoneRequest(api);
}

bool ControlManager::LoadToDoShipList_Kira()
{
	_todoShipids.clear();
	QFile * file = new QFile(QApplication::applicationDirPath() + "/action/" + "inport.table");
	if (file)
	{
		if (file->open(QIODevice::ReadOnly | QIODevice::Text))
		{
			QTextStream instream(file);
			bool bFirstLine = true;
			while (!instream.atEnd())
			{
				QString line = instream.readLine();
				bool bHaveControl = false;
				if (bFirstLine)
				{
					bFirstLine = false;
					if (line.startsWith("C", Qt::CaseInsensitive))
					{
						line = line.right(line.size() - 1);
						_toCond = line.toInt();
						bHaveControl = true;
					}
				}
				if (!bHaveControl)
				{
					int index = line.indexOf("\t");
					if (index > 0)
					{
						line = line.left(index);
					}
					if (!line.isEmpty())
					{
						_todoShipids.append(line.toInt());
					}
				}
			}
		}
	}
	return true;
}

void ControlManager::Run()
{
	if (_state == State::ToTerminate)
	{
		Terminate();
	}
	if (_state != State::Started)
	{
		return;
	}

	if (_actionList.empty())
	{
		setToTerminate("Terminated:NoAction");
		return;
	}
	if (_actionList[0]->action())
	{
		delete _actionList[0];
		_actionList.removeAt(0);
		if (_pauseNext)
		{
			setPauseNextVal(false);
			if (!_actionList.empty())
			{
				Pause();
			}
		}
	}
}

void ControlManager::Pause()
{
	setState(State::Paused, "Paused");
	setPauseNextVal(false);
}

void ControlManager::Resume()
{
	setState(State::Started, "Started:Resume");
	setPauseNextVal(false);
}

void ControlManager::Terminate()
{
	qDeleteAll(_actionList);
	_actionList.clear();
	setPauseNextVal(false);
	_stopwhen = StopWhen::None;
	setState(State::Terminated, "Terminated");
}

bool ControlManager::isPaused()
{
	return _state == State::Paused;
}

void ControlManager::PauseNext()
{
	if (_state == State::Started)
	{
		setPauseNextVal(true);
	}
}

void ControlManager::togglePauseNext()
{
	if (_state == State::Started)
	{
		setPauseNextVal(!_pauseNext);
	}
}

void ControlManager::setToTerminate(const char* title)
{
	if (_state == State::Started)
	{
		setState(State::ToTerminate, title);
	}
}

void ControlManager::createSSShipList()
{
	if (!_ssShips.isEmpty())
	{
		return;
	}

	KanSaveData* pksd = &KanSaveData::getInstance();
	KanDataConnector * pkdc = &KanDataConnector::getInstance();
	for (auto ship: pksd->portdata.api_ship)
	{
		const kcsapi_mst_ship * pmstship = pkdc->findMstShipFromShipid(ship.api_ship_id);
		if (!pmstship)
		{
			continue;
		}
		if (pmstship->api_stype == (int)ShipType::SenBou 
			|| pmstship->api_stype == (int)ShipType::SenSui)
		{
			if (ship.api_lv >= 50 && ship.api_maxhp >= 10)
			{
				bool bAdded = false;
				for (QList<int>& group:_ssShips)
				{
					/*
					if (group.isEmpty())
					{
						group.append(shipid);
						bAdded = true;
					}
					else
					{
					*/
						for (auto ssid : group)
						{
							if (isTreatedSameShip(ssid, ship.api_id))
							{
								group.push_front(ship.api_id);	// put kai back
								bAdded = true;
								break;
							}
						}
//					}
					if (bAdded)
					{
						break;
					}
				}
				if (!bAdded)
				{
					QList<int> tships;
					tships.push_front(ship.api_id);
					_ssShips.append(tships);
				}
			}
		}
	}
}

bool ControlManager::isTreatedSameShip(int shipno, int oshipno)
{
	if (shipno == oshipno)
	{
		return true;
	}

	KanDataConnector * pkdc = &KanDataConnector::getInstance();
	auto pship = pkdc->findShipFromShipno(shipno);
	if (!pship)
	{
		return false;
	}
	auto poship = pkdc->findShipFromShipno(oshipno);
	if (!poship)
	{
		return false;
	}

	int shipid = pship->api_ship_id;
	int oshipid = poship->api_ship_id;
	if (shipid == oshipid)
	{
		return true;
	}

	auto pmstship = pkdc->findMstShipFromShipid(shipid);
	if (!pmstship)
	{
		return false;
	}
	auto pomstship = pkdc->findMstShipFromShipid(oshipid);
	if (!pmstship)
	{
		return false;
	}

	if (isAfterShip(pmstship, pomstship) || isAfterShip(pomstship, pmstship))
	{
		return true;
	}

	return false;
}

bool ControlManager::isAfterShip(const kcsapi_mst_ship* pmstship, const kcsapi_mst_ship* pomstship)
{
	if (!pmstship || !pomstship)
	{
		return false;
	}

	bool bOk = false;
	int afterid = pomstship->api_aftershipid.toInt(&bOk);
	if (!bOk || afterid <= 0)
	{
		return false;
	}
	if (afterid == pmstship->api_id)
	{
		return true;
	}
	else
	{
		KanDataConnector * pkdc = &KanDataConnector::getInstance();
		auto ponewmstship = pkdc->findMstShipFromShipid(afterid);
		if (!ponewmstship)
		{
			return false;
		}
		return isAfterShip(pmstship, ponewmstship);
	}
}

int ControlManager::getOneWasteShipId(const QList<int>& excludes)
{
	KanSaveData* pksd = &KanSaveData::getInstance();
	KanDataConnector* pkdc = &KanDataConnector::getInstance();
	for (auto& ship: pksd->portdata.api_ship)
	{
		if (ship.api_lv == 1
			&& ship.api_locked == false
			&& ship.api_locked_equip == false
			&& ship.api_nowhp == ship.api_maxhp)
		{
			int shipid = ship.api_ship_id;
			const kcsapi_mst_ship * pmstship = pkdc->findMstShipFromShipid(shipid);
			if (!pmstship)
			{
				continue;
			}
			if (pmstship->api_stype == (int)ShipType::KuChiKu)
			{
				if (ship.api_fuel == pmstship->api_fuel_max
					&& ship.api_bull == pmstship->api_bull_max)
				{
					if (excludes.size() > 0)
					{
						bool bShouldContinue = false;
						for (auto excludeId:excludes)
						{
							if (isTreatedSameShip(excludeId, ship.api_id))
							{
								bShouldContinue = true;
								break;
							}
						}
						if (bShouldContinue)
						{
							continue;
						}
					}
					return ship.api_id;
				}
			}
		}
	}
	return -1;
}

int ControlManager::getOneWasteShipId(int exclude1/*=-1*/, int exclude2/*=-1*/)
{
	QList<int> excludes;
	if (exclude1 >= 0)
	{
		excludes.append(exclude1);
	}
	if (exclude2 >= 0)
	{
		excludes.append(exclude2);
	}
	return getOneWasteShipId(excludes);
}

bool ControlManager::isShipFull()
{
	KanSaveData* pksd = &KanSaveData::getInstance();
	int kancount = pksd->portdata.api_ship.count() + pksd->shipcountoffset;
	int kanmaxcount = pksd->portdata.api_basic.api_max_chara;
	if (kancount+3 <= kanmaxcount)
	{
		return false;
	}
	return true;
}

bool ControlManager::findPagePosByShipId(int shipno, int& page, int& pos, int& lastPage)
{
	KanSaveData* pksd = &KanSaveData::getInstance();
	QList<kcsapi_ship2> ships = pksd->portdata.api_ship;	// copy to sort
	page = -1;
	pos = -1;
	lastPage = 0;
	if (!ships.size())
	{
		return false;
	}
	lastPage = (ships.size() - 1) / 10;
	qSort(ships.begin(), ships.end(), [](const  kcsapi_ship2& left, const kcsapi_ship2& right)
	{ 
		return left.api_id > right.api_id; // newer if id is greater
	});

	for (int i = 0; i < ships.size(); i++)
	{
		if (ships[i].api_id == shipno)
		{
			page = i / 10; // from 0
			pos = i % 10;
			return true;
		}
	}
	return false;
}

bool ControlManager::isShipKiraDone(int shipno)
{
	KanDataConnector* pkdc = &KanDataConnector::getInstance();
	auto pship = pkdc->findShipFromShipno(shipno);
	if (pship)
	{
		if (pship->api_cond >= _toCond)
		{
			if (_toCond >= COND_DAIHATSU)
			{
				return true;
			}

			auto pmstship = pkdc->findMstShipFromShipid(pship->api_ship_id);
			if (pmstship)
			{
				if (pmstship->api_stype == (int)ShipType::SuiBou
					|| pmstship->api_stype == (int)ShipType::YouRiKu)
				{
					if (pship->api_cond >= COND_DAIHATSU)
					{
						return true;
					}
				}
				else
				{
					return true;
				}
			}
		}
	}
	return false;
}

bool ControlManager::isShipInOtherTeam(int shipno)
{
	KanSaveData* pksd = &KanSaveData::getInstance();
	bool bFirst = true;
	for (auto& deck : pksd->portdata.api_deck_port)
	{
		if (bFirst)
		{
			bFirst = false;
			continue;
		}
		for (auto id : deck.api_ship)
		{
			if (id == shipno)
			{
				return true;
			}
		}
	}
	return false;
}


bool ControlManager::isShipInDock(int shipno)
{
	KanSaveData* pksd = &KanSaveData::getInstance();
	for (auto& ndock : pksd->portdata.api_ndock)
	{
		if (ndock.api_ship_id == shipno)
		{
				return true;
		}
	}
	return false;
}

bool ControlManager::isHenseiDone(const QList<int>& ships, int index/*=-1*/)
{
	KanSaveData* pksd = &KanSaveData::getInstance();

	if (pksd->portdata.api_deck_port.size())
	{
		auto& firstFleet = pksd->portdata.api_deck_port.first();
		QList<int> nonEmptyShipList;
		for (auto id:firstFleet.api_ship)
		{
			if (id >= 0)
			{
				nonEmptyShipList.append(id);
			}
		}
		if (nonEmptyShipList.size() <= ships.size())
		{
			if (index >= 0 && index < ships.size())
			{
				if (firstFleet.api_ship[index] == ships[index])
				{
					return true;
				}
				else
				{
					return false;
				}
			}
			else if (nonEmptyShipList.size() != ships.size())
			{
				return false;
			}
			else 
			{
				for (int i = 0; i < ships.size(); i++)
				{
					if (firstFleet.api_ship[i] != ships[i])
					{
						return false;
					}
				}
			}
		}
		else
		{
			return false;
		}
	}
	else
	{
		return false;
	}
	return true;
}

bool ControlManager::isFlagshipOnly()
{
	return getTeamSize() == 1;
}

int ControlManager::getTeamSize()
{
	KanSaveData* pksd = &KanSaveData::getInstance();

	if (pksd->portdata.api_deck_port.size())
	{
		auto& firstFleet = pksd->portdata.api_deck_port.first();
		QList<int> nonEmptyShipList;
		for (auto id : firstFleet.api_ship)
		{
			if (id >= 0)
			{
				nonEmptyShipList.append(id);
			}
		}
		return nonEmptyShipList.size();
	}
	return 0;
}

bool ControlManager::isShipType(int shipno, ShipType stype)
{
	KanDataConnector* pkdc = &KanDataConnector::getInstance();

	auto pship = pkdc->findShipFromShipno(shipno);
	if (pship)
	{
		auto pmstship = pkdc->findMstShipFromShipid(pship->api_ship_id);
		if (pmstship)
		{
			if (pmstship->api_stype == (int)stype)
			{
				return true;
			}
		}
	}

	return false;
}

bool ControlManager::hasSlotitem(int shipno, SlotitemType sitype)
{
	return KanDataConnector::getInstance().isShipHasSlotitem(shipno, sitype);
}

bool ControlManager::noSlotitem(int shipno)
{
	KanDataConnector* pkdc = &KanDataConnector::getInstance();

	auto pship = pkdc->findShipFromShipno(shipno);
	if (pship)
	{
		for (auto slotitemid : pship->api_slot)
		{
			if (slotitemid >= 0)
			{
				return false;
			}
		}
	}
	return true;
}

bool ControlManager::flagshipSevereDamaged()
{
	KanSaveData* pksd = &KanSaveData::getInstance();
	KanDataConnector* pkdc = &KanDataConnector::getInstance();

	if (!pksd->portdata.api_ship.size())
	{
		return true;
	}

	if (pksd->portdata.api_deck_port.size())
	{
		auto& firstFleet = pksd->portdata.api_deck_port.first();
		if (firstFleet.api_ship.size())
		{
			int shipno = firstFleet.api_ship.first();

			const kcsapi_ship2 * pship = pkdc->findShipFromShipno(shipno);
			if (!pship)
			{
				return true;
			}
			int shipid = pship->api_ship_id;
			const kcsapi_mst_ship * pmstship = pkdc->findMstShipFromShipid(shipid);
			if (!pmstship)
			{
				return true;
			}

			// first ship found
			if (KanDataCalc::GetWoundState(pship->api_nowhp, pship->api_maxhp) < WoundState::Big)
			{
				return false;
			}
		}
	}
	return true;
}

bool ControlManager::shouldNightBattle()
{
	KanSaveData* pksd = &KanSaveData::getInstance();
	if (pksd->lastWonAssumption)
	{
		return false;
	}
	return true;
}

bool ControlManager::shouldRetrieve()
{
	return (hugestDamageInTeam() >= WoundState::Big);
}

WoundState ControlManager::hugestDamageInTeam()
{
	KanSaveData* pksd = &KanSaveData::getInstance();
	KanDataConnector* pkdc = &KanDataConnector::getInstance();

	WoundState maxState = WoundState::Full;

	if (!pksd->portdata.api_ship.size())
	{
		return WoundState::Dead;
	}

	if (pksd->portdata.api_deck_port.size())
	{
		auto& firstFleet = pksd->portdata.api_deck_port.first();
		if (firstFleet.api_ship.size())
		{
			for (auto shipno: firstFleet.api_ship)
			{
				if (shipno < 0)
				{
					continue;
				}
				const kcsapi_ship2 * pship = pkdc->findShipFromShipno(shipno);
				if (!pship)
				{
					continue;
				}
				int shipid = pship->api_ship_id;
				const kcsapi_mst_ship * pmstship = pkdc->findMstShipFromShipid(shipid);
				if (!pmstship)
				{
					continue;
				}

				// ship found
				auto wstate = KanDataCalc::GetWoundState(pship->api_nowhp, pship->api_maxhp);
				if (wstate > maxState)
				{
					maxState = wstate;
				}
			}
		}
	}
	return maxState;
}

void ControlManager::setStopWhen(StopWhen stopwhen)
{
	_stopwhen = stopwhen;
}

bool ControlManager::checkColors(const QList<CheckColor>& checklist)
{
	if (checklist.empty())
	{
		return false;
	}
	auto w = MainWindow::mainWindow()->getBrowserWidget();
	auto image = w->grab().toImage();
	for (auto& item:checklist)
	{
		auto col = image.pixel(item._pt);
		int r = qRed(col);
		int g = qGreen(col);
		int b = qBlue(col);

		int colAllowrance = COL_ALLOWRANCE;
		if (r - item._r > colAllowrance
			|| item._r - r > colAllowrance
			|| g - item._g > colAllowrance
			|| item._g - g > colAllowrance
			|| b - item._b > colAllowrance
			|| item._b - b > colAllowrance)
		{
			return false;
		}
	}
	return true;
}

bool ControlManager::shouldChangeSecondShip()
{
	KanSaveData* pksd = &KanSaveData::getInstance();
	KanDataConnector* pkdc = &KanDataConnector::getInstance();

	if (!pksd->portdata.api_ship.size())
	{
		return true;
	}

	if (pksd->portdata.api_deck_port.size())
	{
		auto& firstFleet = pksd->portdata.api_deck_port.first();

		QList<int> nonEmptyShipList;
		for (auto id : firstFleet.api_ship)
		{
			if (id >= 0)
			{
				nonEmptyShipList.append(id);
			}
		}
		if (nonEmptyShipList.size() < 2)
		{
			return true;
		}
		else
		{
			int shipno = firstFleet.api_ship[1];

			const kcsapi_ship2 * pship = pkdc->findShipFromShipno(shipno);
			if (!pship)
			{
				return true;
			}
			int shipid = pship->api_ship_id;
			const kcsapi_mst_ship * pmstship = pkdc->findMstShipFromShipid(shipid);
			if (!pmstship)
			{
				return true;
			}

			// second ship found
			if (pship->api_nowhp != pship->api_maxhp)
			{
				return true;
			}
			if (pship->api_lv > 1)
			{
				return true;
			}

		}
	}
	return false;
}

bool ControlManager::needChargeFlagship()
{
	KanSaveData* pksd = &KanSaveData::getInstance();
	KanDataConnector* pkdc = &KanDataConnector::getInstance();

	if (!pksd->portdata.api_ship.size())
	{
		return false;
	}

	if (pksd->portdata.api_deck_port.size())
	{
		auto& firstFleet = pksd->portdata.api_deck_port.first();
		if (firstFleet.api_ship.size())
		{
			int shipno = firstFleet.api_ship.first();

			const kcsapi_ship2 * pship = pkdc->findShipFromShipno(shipno);
			if (!pship)
			{
				return false;
			}
			int shipid = pship->api_ship_id;
			const kcsapi_mst_ship * pmstship = pkdc->findMstShipFromShipid(shipid);
			if (!pmstship)
			{
				return false;
			}

			// first ship found

			if (pship->api_fuel < pmstship->api_fuel_max
				||pship->api_bull < pmstship->api_bull_max
				)
			{
				return true;
			}
		}
	}
	return false;
}

bool ControlManager::needChargeAnyShip()
{
	KanSaveData* pksd = &KanSaveData::getInstance();
	KanDataConnector* pkdc = &KanDataConnector::getInstance();

	if (!pksd->portdata.api_ship.size())
	{
		return false;
	}

	if (pksd->portdata.api_deck_port.size())
	{
		auto& firstFleet = pksd->portdata.api_deck_port.first();
		if (firstFleet.api_ship.size())
		{
			for (auto shipno:firstFleet.api_ship)
			{
				if (shipno < 0)
				{
					continue;
				}
				const kcsapi_ship2 * pship = pkdc->findShipFromShipno(shipno);
				if (!pship)
				{
					continue;
				}
				int shipid = pship->api_ship_id;
				const kcsapi_mst_ship * pmstship = pkdc->findMstShipFromShipid(shipid);
				if (!pmstship)
				{
					continue;
				}

				// ship found
				if (pship->api_fuel < pmstship->api_fuel_max
					|| pship->api_bull < pmstship->api_bull_max
					)
				{
					return true;
				}
			}
		}
	}
	return false;

}

int ControlManager::getCurrentFlagshipId()
{
	KanSaveData* pksd = &KanSaveData::getInstance();

	if (!pksd->portdata.api_ship.size())
	{
		return false;
	}

	if (pksd->portdata.api_deck_port.size())
	{
		auto& firstFleet = pksd->portdata.api_deck_port.first();
		if (firstFleet.api_ship.size())
		{
			int shipno = firstFleet.api_ship.first();
			return shipno;
		}
	}
	return -1;
}

int ControlManager::getCurrentSecondshipId()
{
	KanSaveData* pksd = &KanSaveData::getInstance();

	if (!pksd->portdata.api_ship.size())
	{
		return false;
	}

	if (pksd->portdata.api_deck_port.size())
	{
		auto& firstFleet = pksd->portdata.api_deck_port.first();
		if (firstFleet.api_ship.size() > 1)
		{
			int shipno = firstFleet.api_ship.at(1);
			return shipno;
		}
	}
	return -1;

}

void ControlManager::setState(State state, const char* str)
{
	if (_state != state)
	{
		_state = state;
		if (_state == State::Terminated || _state == State::ToTerminate)
		{
			MainWindow::mainWindow()->timerWindow()->playSound(TimerMainWindow::SoundIndex::Terminated);
			QTimer::singleShot(4000, [this]()
			{
				if (this->_state == State::Terminated)
				{
					setStateStr("");
				}
			});
		}
	}
	setStateStr(str);
}

void ControlManager::setStateStr(const QString& str)
{
	_stateStr = str;
	MainWindow::mainWindow()->timerWindow()->setTitle(str);
}

void ControlManager::moveMouseToAndClick(float x, float y, float offsetX /*= 5*/, float offsetY /*= 3*/)
{
	QPointF ptAdjusted = QPointF(x+randVal(-offsetX, offsetX), y+randVal(-offsetY, offsetY));

	QMouseEvent e(QEvent::MouseButtonPress, ptAdjusted, Qt::LeftButton, Qt::LeftButton, Qt::NoModifier);
	QApplication::sendEvent(MainWindow::mainWindow()->getBrowserWidget(), &e);

	QMouseEvent e2(QEvent::MouseButtonRelease, ptAdjusted, Qt::LeftButton, Qt::LeftButton, Qt::NoModifier);
	QApplication::sendEvent(MainWindow::mainWindow()->getBrowserWidget(), &e2);

}

void ControlManager::moveMouseTo(float x, float y, float offsetX /*= 5*/, float offsetY /*= 3*/)
{
	QPointF ptAdjusted = QPointF(x + randVal(-offsetX, offsetX), y + randVal(-offsetY, offsetY));

	QMouseEvent e(QEvent::MouseMove, ptAdjusted, Qt::LeftButton, Qt::LeftButton, Qt::NoModifier);
	QApplication::sendEvent(MainWindow::mainWindow()->getBrowserWidget(), &e);
}

void ControlManager::setPauseNextVal(bool bVal)
{
	if (_pauseNext != bVal)
	{
		_pauseNext = bVal;
		MainWindow::mainWindow()->setPauseNextChanged(_pauseNext);
	}
}

qreal ControlManager::randVal(qreal min, qreal max)
{
	qreal value = static_cast<qreal>(qrand()) * (max-min) / RAND_MAX + min;
	return value;
}

