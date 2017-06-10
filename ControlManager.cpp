#include "ControlManager.h"
//#include <QTestEventList>
#include "mainwindow.h"
#include <QMouseEvent>
#include <QGraphicsSceneMouseEvent>
#include "kandataconnector.h"
#include "kansavedata.h"
#include "kandatacalc.h"
#include <QtMath>
#include "ExpeditionManager.h"
#include <QApplication>
#include <QSettings>

#ifdef Q_OS_WIN
#include <windows.h>
#endif

#define COL_ALLOWRANCE 3
#define COND_DAIHATSU	80

ControlManager::ControlManager()
{
}


ControlManager::~ControlManager()
{
}

bool ControlManager::BuildNext_Kira()
{
	_target = ActionTarget::Kira;
	pushPreSupplyCheck();
	if (_kiraSetting.forceCurrent)
	{
		_todoShipids.clear();
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

	QList<int> wasteShipListByPreCheck = pushPreShipFullCheck();

	int togoShipId = _todoShipids.at(0);
	while (isShipKiraDone(togoShipId)
		|| isShipInOtherTeam(togoShipId, 0)
		|| isShipInDock(togoShipId)
		|| hasSlotitem(togoShipId, SlotitemType::JouRiKuTei, 3)
		|| hasSlotitem(togoShipId, SlotitemType::Sonar)
		|| hasSlotitem(togoShipId, SlotitemType::BaKuRai)
		|| hasSlotitem(togoShipId, SlotitemType::Sonar_L)
		|| hasSlotitem(togoShipId, SlotitemType::YuSou, 3)
		|| noSlotitem(togoShipId)
		|| noAttackItem(togoShipId)
		|| (!_kiraSetting.forceCurrent&&isLowCond(togoShipId)))
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

		auto randomwaste = getOneWasteShipId(flagshipid, curSecond, wasteShipListByPreCheck);
		preChAction->setShips(flagshipid, randomwaste);
		_actionList.append(preChAction);
		bTogoSameToCurSecond = true;
	}

	if (!shouldChangeSecondShip() && !bTogoSameToCurSecond && wasteShipListByPreCheck.isEmpty())
	{
		bChangeSecond = false;
	}

	int wasteId = curSecond;
	if (bChangeSecond)
	{
		wasteId = getOneWasteShipId(togoShipId, flagshipid, wasteShipListByPreCheck);
	}
	chAction->setShips(togoShipId, wasteId);
	_actionList.append(chAction);

	SortieAction* sortieAction = new SortieAction();
	sortieAction->setAreaAndMap(1, 1);
	_actionList.append(sortieAction);
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
	_target = ActionTarget::Fuel;

	pushPreSupplyCheck();
	if (stopWhenCheck())
	{
		setToTerminate("Termination:StopWhenDone");
		return false;
	}

	int flagshipid = getCurrentFlagshipId();
	/*
	if (!isShipType(flagshipid, ShipType::SenSui))
	{
	setToTerminate("Terminated:Ship type");
	return false;
	}
	*/
	if (!isFlagshipOnly(0) || !isShipType(flagshipid, ShipType::SenSui))
	{
		// southeast
		return BuildNext_SouthEast();
	}

	if (flagshipSevereDamaged())
	{
		setToTerminate("Terminated:Flagship");
		return false;
	}

	pushPreShipFullCheck();
	/*
	if (isShipFull())
	{
	setToTerminate("Terminated:ShipFull");
	return false;
	//		_actionList.append(new DestroyShipAction());
	}
	*/

	SortieAction* sortieAction = new SortieAction();
	sortieAction->setAreaAndMap(2, 3);
	_actionList.append(sortieAction);
	_actionList.append(new SortieAdvanceAction());
	_actionList.append(new ChargeAction());
	_actionList.append(new RepeatAction());

	setState(State::Ready, "Ready");
	return true;
}

bool ControlManager::chooseSSShipList(int teamSize, QList<int>& ships, QList<int>& sortInTeamShips, QString& errorMessage)
{
	KanSaveData* pksd = &KanSaveData::getInstance();
	errorMessage = "";

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
		for (auto ssid : group)
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
			if (isShipInOtherTeam(ssid, 0))
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

	if (bestShips.size() < teamSize)
	{
		errorMessage = "Terminated:No Enough Ship";
		return false;
	}

	// sort bestships
	qSort(bestShips.begin(), bestShips.end(), [](const  kcsapi_ship2* left, const kcsapi_ship2* right)
	{
		return left->api_cond > right->api_cond; // cond from high to low
	});

	int minCond = bestShips.at(teamSize - 1)->api_cond;
	if (minCond <= _sortieMinCond)
	{
		// wait
		qint64 waitMS = qCeil((_sortieWaitCond - minCond) / 3.0) * 3 * 60 * 1000;
		auto waitAction = new WaitCondAction();
		waitAction->setWaitMS(waitMS);
		_actionList.append(waitAction);
	}

	ships.clear();
	for (int i = 0; i < teamSize; i++)
	{
		ships.push_front(bestShips.at(i)->api_id);
	}

	// sort in team first
	sortInTeamShips.clear();

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
		for (auto& group : _ssShips)
		{
			if (group.contains(shipno))
			{
				pgroup = &group;
				break;
			}
		}
		if (!pgroup)
		{
			errorMessage = "Terminated:Wrong SS Data";
			return false;
		}

		for (int gpshipno : (*pgroup))
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
	return true;
}

bool ControlManager::BuildNext_SouthEast()
{
	_target = ActionTarget::SouthEast;

	pushPreSupplyCheck();
	if (stopWhenCheck())
	{
		setToTerminate("Termination:StopWhenDone");
		return false;
	}
	_southEastTeamSize = getTeamSize(0);
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

	pushPreShipFullCheck();

	QList<int> ships;
	QList<int> sortInTeamShips;
	QString errorMessage;
	if (!chooseSSShipList(_southEastTeamSize, ships, sortInTeamShips, errorMessage))
	{
		setToTerminate(errorMessage.toLocal8Bit());
		return false;
	}

	// change hensei sort
	auto chSortAction = new ChangeHenseiAction();
	chSortAction->setShips(sortInTeamShips);
	_actionList.append(chSortAction);

	// change hensei
	auto chAction = new ChangeHenseiAction();
	chAction->setShips(ships);
	_actionList.append(chAction);

	SortieAction* sortieAction = new SortieAction();
	sortieAction->setAreaAndMap(2, 3);
	_actionList.append(sortieAction);
	_actionList.append(new SortieCommonAdvanceAction());
	_actionList.append(new ChargeAction());
	_actionList.append(new RepeatAction());

	setState(State::Ready, "Ready");
	return true;
}

bool ControlManager::BuildNext_Level()
{
	_target = ActionTarget::Level;
	pushPreSupplyCheck();

	KanSaveData* pksd = &KanSaveData::getInstance();
	KanDataConnector* pkdc = &KanDataConnector::getInstance();

	pushPreShipFullCheck();
	/*
	if (isShipFull())
	{
	setToTerminate("Terminated:ShipFull");
	return false;
	}
	*/

	bool bHaveMaruyu = false;
	int redFaceCount = 0;
	int totalShipCount = 0;
	int minCond = 100;
	bool bNeedChangeMaruyu = false;

	QList<int> toChangeIdList;
	if (pksd->portdata.api_deck_port.size())
	{
		auto& firstFleet = pksd->portdata.api_deck_port.first();
		for (auto id : firstFleet.api_ship)
		{
			if (id >= 0)
			{
				const kcsapi_ship2* pship = pkdc->findShipFromShipno(id);
				if (pship)
				{
					const kcsapi_mst_ship* pmstship = pkdc->findMstShipFromShipid(pship->api_ship_id);
					WoundState ws = KanDataCalc::GetWoundState(pship->api_nowhp, pship->api_maxhp);
					if ((pmstship->api_stype == (int)ShipType::SenSui || pmstship->api_stype == (int)ShipType::SenBou)
						&& (pship->api_maxhp < 10 || (pship->api_lv < 99 && pship->api_locked)))
					{
						if (bHaveMaruyu)
						{
							setToTerminate("Terminated:MoreThanOneMaruyu");
							return false;
						}
						bHaveMaruyu = true;
						if (ws > WoundState::Middle)
						{
							bNeedChangeMaruyu = true;
						}
						else
						{
							toChangeIdList.append(id);
						}
					}
					else
					{
						toChangeIdList.append(id);
						if (ws > WoundState::Small)
						{
							setToTerminate("Terminated:NeedNDock");
							return false;
						}
						if (pship->api_cond < 30)
						{
							redFaceCount++;
							if (pship->api_cond < minCond)
							{
								minCond = pship->api_cond;
							}
						}
					}
					totalShipCount++;
				}
			}
		}
	}
	if (totalShipCount < 6)
	{
		setToTerminate("Terminated:TeamSize");
		return false;
	}
	if (!bHaveMaruyu)
	{
		setToTerminate("Terminated:NoMaruyu");
		return false;
	}

	if (bNeedChangeMaruyu)
	{
		// find one maruyu
		int maruyuId = -1;
		int bigMaruyuId = -1;
		for (auto& ship : pksd->portdata.api_ship)
		{
			const kcsapi_mst_ship * pmstship = pkdc->findMstShipFromShipid(ship.api_ship_id);
			if (!pmstship)
			{
				continue;
			}
			if ((pmstship->api_stype == (int)ShipType::SenSui || pmstship->api_stype == (int)ShipType::SenBou)
				&& (ship.api_maxhp < 10 || (ship.api_lv < 99 && ship.api_locked)))
			{
				if (isShipCharged(ship.api_id)
					&& !isShipDamaged(ship.api_id)
					&& !isShipInDock(ship.api_id)
					&& !isShipInOtherTeam(ship.api_id, -1))
				{
					if (ship.api_lv >= 20)
					{
						bigMaruyuId = ship.api_id;
						continue;
					}
					maruyuId = ship.api_id;
					break;
				}
			}
		}
		if (maruyuId < 0 && bigMaruyuId >= 0)
		{
			maruyuId = bigMaruyuId;
		}
		toChangeIdList.append(maruyuId);
		if (maruyuId < 0 || toChangeIdList.size() != 6)
		{
			setToTerminate("Terminated:NoAvailableMaruyu");
			return false;
		}
		// change hensei
		auto chAction = new ChangeHenseiAction();
		chAction->setShips(toChangeIdList);
		_actionList.append(chAction);
	}
	// wait cond if over three red face
	if (_levelNoWait)
	{
		if (bNeedChangeMaruyu)
		{
			_actionList.append(new WaitNextPortAction());
		}
	}
	else if (redFaceCount > 3)
	{
		qint64 waitMS = qCeil((_sortieWaitCond - minCond) / 3.0) * 3 * 60 * 1000;

		auto waitAction = new WaitCondAction();
		waitAction->setWaitMS(waitMS);
		_actionList.append(waitAction);
	}
	SortieAction* sortieAction = new SortieAction();
	sortieAction->setAreaAndMap(3, 2);
	_actionList.append(sortieAction);
	_actionList.append(new SortieCommonAdvanceAction());
	_actionList.append(new ChargeAction());
	_actionList.append(new RepeatAction());
	setState(State::Ready, "Ready");
	return true;
}

bool ControlManager::BuildNext_Rank()
{
	_target = ActionTarget::Rank;
	pushPreSupplyCheck();

	KanSaveData* pksd = &KanSaveData::getInstance();
	KanDataConnector* pkdc = &KanDataConnector::getInstance();

	// allow ship full termination
	if (isShipFull())
	{
		setToTerminate("Terminated:ShipFull");
		return false;
	}

	int totalYusou = 0;
	bool allSS = true;
	int minCond = std::numeric_limits<int>::max();
	int shipCount = 0;

	if (pksd->portdata.api_deck_port.size())
	{
		auto& firstFleet = pksd->portdata.api_deck_port.first();
		for (auto id : firstFleet.api_ship)
		{
			if (id >= 0)
			{
				const kcsapi_ship2* pship = pkdc->findShipFromShipno(id);
				if (pship)
				{
					shipCount++;
					const kcsapi_mst_ship* pmstship = pkdc->findMstShipFromShipid(pship->api_ship_id);
					if (pmstship->api_stype != (int)ShipType::SenSui &&
						pmstship->api_stype != (int)ShipType::SenBou)
					{
						allSS = false;
					}

					if (pship->api_cond < minCond)
					{
						minCond = pship->api_cond;
					}

					WoundState ws = KanDataCalc::GetWoundState(pship->api_nowhp, pship->api_maxhp);

					if (ws >= WoundState::Middle)
					{
						setToTerminate("Terminated:Damage");
						return false;
					}

					int yusouCount = 0;
					pkdc->isShipHasSlotitem(pship, SlotitemType::YuSou, 1, &yusouCount);
					totalYusou += yusouCount;
				}
			}
		}
	}

	if (shipCount < 6)
	{
		setToTerminate("Terminated:ShipCount");
		return false;
	}

	if (!allSS)
	{
		if (!pksd->deckSaveData.empty())
		{
			if (pksd->deckSaveData.first().totalTaiku < 360)
			{
				setToTerminate("Terminated:Taiku");
				return false;
			}
		}
		if (totalYusou < 4)
		{
			setToTerminate("Terminated:Yusou");
			return false;
		}
	}

	if (minCond <= _sortieMinCond)
	{
		// wait
		qint64 waitMS = qCeil((_sortieWaitCond - minCond) / 3.0) * 3 * 60 * 1000;
		auto waitAction = new WaitCondAction();
		waitAction->setWaitMS(waitMS);
		_actionList.append(waitAction);
	}

	SortieAction* sortieAction = new SortieAction();
	sortieAction->setAreaAndMap(5, 4);
	_actionList.append(sortieAction);
	_actionList.append(new SortieCommonAdvanceAction());
	_actionList.append(new ChargeAction());
	_actionList.append(new RepeatAction());
	setState(State::Ready, "Ready");
	return true;
}

void ControlManager::LoadAnyTemplateSettings()
{
	_anyTemplateSettings.clear();

	QString filename = QApplication::applicationDirPath();
	filename += "/action/anysettings.ini";
	QSettings* setting = new QSettings(filename, QSettings::IniFormat);
	setting->setIniCodec("UTF-8");

	auto groups = setting->childGroups();
	Q_FOREACH(QString group, groups)
	{
		QStringList strlist = group.split('-');
		if (strlist.size() != 2)
		{
			continue;
		}
		bool bOk = false;
		int area = strlist[0].toInt(&bOk);
		if (!bOk)
		{
			continue;
		}
		int map = strlist[1].toInt(&bOk);
		if (!bOk)
		{
			continue;
		}

		setting->beginGroup(group);
		QPair<int, int> pair = QPair<int, int>(area, map);
		_anyTemplateSettings[pair].area = area;
		_anyTemplateSettings[pair].map = map;
		Q_FOREACH(QString key, setting->childKeys())
		{
			int cell = key.toInt(&bOk);
			if (!bOk)
			{
				auto splited = setting->value(key).toStringList();
				if (!key.compare("AreaCheck", Qt::CaseInsensitive))
				{
					if (splited.size() != 10)
					{
						// error
						continue;
					}
					for (const auto& str : splited)
					{
						_anyTemplateSettings[pair].areaCheckList.append(str.toInt(&bOk));
					}
				}
				else if (!key.compare("MapClick", Qt::CaseInsensitive))
				{
					if (splited.size() != 4)
					{
						// error
						continue;
					}
					for (const auto& str : splited)
					{
						_anyTemplateSettings[pair].mapClickPoint.append(str.toInt(&bOk));
					}
				}

				else if (!key.compare("MapExCheck", Qt::CaseInsensitive))
				{
					if (splited.size() != 10)
					{
						// error
						continue;
					}
					for (const auto& str : splited)
					{
						_anyTemplateSettings[pair].mapExCheckList.append(str.toInt(&bOk));
					}
				}
				else if (!key.compare("MapExClick", Qt::CaseInsensitive))
				{
					if (splited.size() != 4)
					{
						// error
						continue;
					}
					for (const auto& str : splited)
					{
						_anyTemplateSettings[pair].mapExClickPoint.append(str.toInt(&bOk));
					}
				}

				else if (!key.compare("MapEx2Check", Qt::CaseInsensitive))
				{
					if (splited.size() != 10)
					{
						// error
						continue;
					}
					for (const auto& str : splited)
					{
						_anyTemplateSettings[pair].mapEx2CheckList.append(str.toInt(&bOk));
					}
				}
				else if (!key.compare("MapEx2Click", Qt::CaseInsensitive))
				{
					if (splited.size() != 4)
					{
						// error
						continue;
					}
					for (const auto& str : splited)
					{
						_anyTemplateSettings[pair].mapEx2ClickPoint.append(str.toInt(&bOk));
					}
				}
				continue;
			}

			QString settingStr = setting->value(key).toString();
			if (settingStr.isEmpty())
			{
				continue;
			}
			QString formationStr = settingStr.left(1);
			int formation = formationStr.toInt(&bOk);
			if (!bOk)
			{
				continue;
			}
			_anyTemplateSettings[pair].cells[cell].formation = formation;
			if (settingStr.contains('C') || settingStr.contains('c'))
			{
				_anyTemplateSettings[pair].cells[cell].bCount = true;
			}
			if (settingStr.contains('R') || settingStr.contains('r'))
			{
				_anyTemplateSettings[pair].cells[cell].bReturnNext = true;
			}
			if (settingStr.contains('S') || settingStr.contains('s'))
			{
				_anyTemplateSettings[pair].cells[cell].bNeedS = true;
			}
			if (settingStr.contains('F') || settingStr.contains('f'))
			{
				_anyTemplateSettings[pair].cells[cell].bKillFlagship = true;
			}
			if (settingStr.contains('W') || settingStr.contains('w'))
			{
				_anyTemplateSettings[pair].cells[cell].bNeedWin = true;
			}
			if (settingStr.contains('T') || settingStr.contains('t'))
			{
				_anyTemplateSettings[pair].cells[cell].bTerminateNext = true;
			}
			if (settingStr.contains('A') || settingStr.contains('a'))
			{
				_anyTemplateSettings[pair].cells[cell].bAskForProceed = true;
			}
		}
		setting->endGroup();

	}
	delete setting;
}

bool ControlManager::BuildNext_Any()
{
	_target = ActionTarget::Any;
	pushPreSupplyCheck();

	KanSaveData* pksd = &KanSaveData::getInstance();
	KanDataConnector* pkdc = &KanDataConnector::getInstance();

	//TODO: read AnySetting
	if (isShipFull())
	{
		setToTerminate("Terminated:ShipFull");
		return false;
	}

	if (_anySetting.count >= 0 && _anySetting.count <= pksd->totalAnyCount)
	{
		setToTerminate("Terminated:CountReached");
		return false;
	}

	QList<int> ships;
	int minCond = std::numeric_limits<int>::max();
	int shipCount = 0;

	if (_anySetting.onlySSTeamSize > 0)
	{
		QList<int> sortInTeamShips;
		QString errorMessage;
		if (!chooseSSShipList(_anySetting.onlySSTeamSize, ships, sortInTeamShips, errorMessage))
		{
			setToTerminate(errorMessage.toLocal8Bit());
			return false;
		}

		auto chSortAction = new ChangeHenseiAction();
		chSortAction->setShips(sortInTeamShips);
		_actionList.append(chSortAction);

		// change hensei
		auto chAction = new ChangeHenseiAction();
		chAction->setShips(ships);
		_actionList.append(chAction);

	}
	else
	{
		if (pksd->portdata.api_deck_port.size())
		{
			auto& firstFleet = pksd->portdata.api_deck_port.first();
			for (auto id : firstFleet.api_ship)
			{
				if (id >= 0)
				{
					ships.append(id);
				}
			}
		}
	}

	for (auto id : ships)
	{
		const kcsapi_ship2* pship = pkdc->findShipFromShipno(id);
		if (pship)
		{
			shipCount++;

			if (pship->api_cond < minCond)
			{
				minCond = pship->api_cond;
			}

			WoundState ws = KanDataCalc::GetWoundState(pship->api_nowhp, pship->api_maxhp);

			if (ws >= WoundState::Middle && !_anySetting.allowMiddleDamageSortie || ws >= WoundState::Big)
			{
				setToTerminate("Terminated:Damage");
				return false;
			}
		}

		if (minCond <= _sortieMinCond && _anySetting.checkCond)
		{
			// wait
			qint64 waitMS = qCeil((_sortieWaitCond - minCond) / 3.0) * 3 * 60 * 1000;
			auto waitAction = new WaitCondAction();
			waitAction->setWaitMS(waitMS);
			_actionList.append(waitAction);
		}
	}

	SortieAction* sortieAction = new SortieAction();
	sortieAction->setAreaAndMap(_anySetting.area, _anySetting.map
		, _anySetting.areaCheckList, _anySetting.mapClickPoint
		, _anySetting.mapExCheckList, _anySetting.mapExClickPoint
		, _anySetting.mapEx2CheckList, _anySetting.mapEx2ClickPoint);
	sortieAction->setShouldPauseNext(_anySetting.pauseAtStartMap);
	_actionList.append(sortieAction);
	_actionList.append(new SortieCommonAdvanceAction());
	_actionList.append(new ChargeAction());
	_actionList.append(new RepeatAction());
	setState(State::Ready, "Ready");
	return true;
}


bool ControlManager::BuildNext_Expedition()
{
	// no check on pushPreSupplyCheck();

	_target = ActionTarget::Expedition;
	int team = -1;
	auto timerWindow = MainWindow::mainWindow()->timerWindow();

	QList<int> excludeTeams;
	SingleExpedition* pExp;
	qint64 ct = TimerMainWindow::currentMS();
	qint64 waitMS = 0;
	for (int i = 0; i < 3; i++)
	{
		waitMS = timerWindow->getMinExpeditionMS(team, excludeTeams);
		pExp = ExpeditionManager::getInstance().getShouldNextSchedule(team, ct, ct + waitMS);
		if (!pExp)
		{
			excludeTeams.append(team);
		}
		else
		{
			break;
		}
	}

	if (!pExp)
	{
		setToTerminate("Termination:Fatal", false);
		// date may change here
		return false;
	}

	if (team < 0)
	{
		return true;
	}
	// set pexp

	KanSaveData* pksd = &KanSaveData::getInstance();
	if (!pksd->portdata.api_ship.size())
	{
		setToTerminate("Termination:Fatal", true);
		return false;
	}
	QList<int> ships;
	if (pksd->portdata.api_deck_port.size())
	{
		auto& fleet = pksd->portdata.api_deck_port.at(team);
		if (fleet.api_ship.size())
		{
			ships = fleet.api_ship;
		}
		else
		{
			setToTerminate("Termination:Fatal", true);
			return false;
		}
	}
	else
	{
		setToTerminate("Termination:Fatal", true);
		return false;
	}

	ships.removeAll(-1);

	LoadToDoShipList_Kira();
	QList<int> todoships = _todoShipids;
	srand(ct);
	std::random_shuffle(todoships.begin(), todoships.end());
	bool needChangeHensei = false;
	QList<int> toShips = ships;

	int shipCount = pExp->shipTypes.count();
	while (toShips.size() > shipCount)
	{
		toShips.removeLast();
	}
	while (toShips.size() < shipCount)
	{
		toShips.append(-1);
	}
	while (ships.size() < shipCount)
	{
		ships.append(-1);
	}

	// TODO: adjust exchangeable special ships
	for (int i = 0; i < shipCount; i++)
	{
		int toShipid = -1;
		QList<int> excludeShipids;
		for (int j = 0; j < shipCount; j++)
		{
			if (j != i)
			{
				excludeShipids.append(toShips[j]);
			}
		}
		QString failReason = pExp->checkMatches(ships[i], i, team, toShipid, todoships, excludeShipids);
		if (!failReason.isEmpty())
		{
			setToTerminate((QString("Termination:") + failReason).toStdString().c_str(), true);
			return false;
		}
		if (toShipid < 0)
		{
			continue;
		}
		else if (toShipid == toShips[i])
		{
			continue;
		}
		else
		{
			needChangeHensei = true;
			toShips[i] = toShipid;
		}
	}

	if (waitMS > 0)
	{
		waitMS += 5000 + randVal(-1000, 1000);
		auto waitAction = new WaitCondAction();
		waitAction->setWaitMS(waitMS);
		waitAction->setWaitName(QString("W:%1").arg(ExpeditionManager::getInstance().getCurrentPreset()).toLocal8Bit());
		_actionList.append(waitAction);
	}

	// charge with skip
	auto chargeAction = new ChargeAction();

	foreach(auto& deck, pksd->portdata.api_deck_port)
	{
		if (deck.api_id == team + 1)
		{
			if (deck.api_mission[0] > 0 || needChargeFlagship(team))
			{
				chargeAction->setSkipExpedition(true);
			}
			break;
		}
	}

	chargeAction->setTeam(team);
	_actionList.append(chargeAction);

	// change hensei
	if (needChangeHensei)
	{
		auto changeAction = new ChangeHenseiAction();
		changeAction->setShips(toShips);
		changeAction->setTeam(team);
		_actionList.append(changeAction);
	}

	// go expedition
	auto expeditionAction = new ExpeditionAction();
	expeditionAction->setTeamAndTarget(team, pExp->destPage, pExp->destIndex);
	_actionList.append(expeditionAction);

	_actionList.append(new RepeatAction());

	setState(State::Ready, "Ready");
	return true;
}

bool ControlManager::BuildNext_Destroy()
{
	_target = ActionTarget::Destroy;

	QList<int> wasteShipList;
	QList<int> toDestroyList = GenerateToDestroyList(wasteShipList);
	if (toDestroyList.isEmpty())
	{
		setToTerminate("Termination:Fatal", true);
		return false;
	}
	DestroyShipAction* action = new DestroyShipAction();
	action->setShips(toDestroyList);
	_actionList.append(action);
	_actionList.append(new RepeatAction());	//should be nothing
	setState(State::Ready, "Ready");
	return true;
}


bool ControlManager::BuildNext_Develop()
{
	_target = ActionTarget::Develop;
	DevelopAction* action = new DevelopAction();

	QMap<int, int>::iterator it = _developSetting.toDevelopSlotItemList.begin();
	bool bAdded = false;
	for (; it != _developSetting.toDevelopSlotItemList.end(); ++it)
	{
		int id = it.key();
		int count = it.value();
		if (count <= 0)
		{
			continue;
		}
		if (id <= 0 || !KanDataConnector::getInstance().findMstSlotItemFromSlotitemid(id))
		{
			continue;
		}
		if (ControlManager::getInstance().getTotalSlotItemCountForID(id) >= count)
		{
			continue;
		}
		bAdded = true;
		action->addItem(id, count);
	}
	if (!bAdded)
	{
		delete action;
		setToTerminate("Termination:Fatal", true);
		return false;
	}
	_actionList.append(action);
	_actionList.append(new RepeatAction());
	setState(State::Ready, "Ready");
	return true;

}

bool ControlManager::stopWhenCheck()
{
	if (!isFuelMode() && !isSouthEastMode())
	{
		return false;
	}

	switch (_southEastSetting.stopWhen)
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
		setInactiveWaiting(false);
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
	QFile * file = new QFile(QApplication::applicationDirPath() + "/action/" + "import.table");
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
			file->close();
		}
		delete file;
	}
	return true;
}


bool ControlManager::LoadDestroyableList()
{
	_destroyableMstIds.clear();
	QFile * file = new QFile(QApplication::applicationDirPath() + "/action/" + "destroyable.table");
	if (file)
	{
		if (file->open(QIODevice::ReadOnly | QIODevice::Text))
		{
			QTextStream instream(file);
			while (!instream.atEnd())
			{
				QString line = instream.readLine();
				int index = line.indexOf("\t");
				if (index > 0)
				{
					line = line.left(index);
				}
				if (!line.isEmpty())
				{
					_destroyableMstIds.append(line.toInt());
				}
			}
			file->close();
		}
		delete file;
	}
	return true;

}

QList<int> ControlManager::GenerateToDestroyList(QList<int>& wasteShipList)
{
	LoadDestroyableList();

	KanSaveData* pksd = &KanSaveData::getInstance();
	KanDataConnector* pkdc = &KanDataConnector::getInstance();
	QList<kcsapi_ship2> toTempDestroyList;
	for (auto& ship : pksd->portdata.api_ship)
	{
		const kcsapi_mst_ship * pmstship = pkdc->findMstShipFromShipid(ship.api_ship_id);
		if (!pmstship)
		{
			continue;
		}
		if (_destroyableMstIds.contains(pmstship->api_id)
			&& !ship.api_locked
			&& !ship.api_locked_equip
			&& ship.api_lv < 3)
		{
			toTempDestroyList.append(ship);
		}
	}
	qSort(toTempDestroyList.begin(), toTempDestroyList.end(), [](const kcsapi_ship2& left, const kcsapi_ship2& right){
		if (left.api_id > right.api_id)
		{
			return true;
		}
		return false;
	});

	int kuchiku[2] = { -1, -1 };
	int kindex = 0;
	int lastMasterId = -1;
	QList<int> toDestroyList;
	for (auto& ship : toTempDestroyList)
	{
		bool reserve = false;
		if (kindex < 2)
		{
			const kcsapi_mst_ship * pmstship = pkdc->findMstShipFromShipid(ship.api_ship_id);
			if (pmstship->api_stype == (int)ShipType::KuChiKu
				&& ship.api_maxhp == ship.api_nowhp
				&& ship.api_bull == pmstship->api_bull_max
				&& ship.api_fuel == pmstship->api_fuel_max)
			{
				if (lastMasterId != pmstship->api_id)
				{
					kuchiku[kindex] = ship.api_id;
					lastMasterId = pmstship->api_id;
					kindex++;
					reserve = true;
				}
			}
		}
		if (!reserve)
		{
			toDestroyList.append(ship.api_id);
		}
	}
	if (kuchiku[0] < 0 || kuchiku[1] < 0)
	{
		return QList<int>();
	}
	wasteShipList.append(kuchiku[0]);
	wasteShipList.append(kuchiku[1]);
	return toDestroyList;
}


void ControlManager::pushPreSupplyCheck()
{
	for (int i = 0; i < 4; i++)
	{
		if (needChargeFlagship(i))
		{
			ChargeAction* action = new ChargeAction();
			action->setTeam(i);
			_actionList.append(action);
		}
	}
}

QList<int> ControlManager::pushPreShipFullCheck()
{
	if (isShipFull())
	{
		QList<int> wasteShipList;
		DestroyShipAction* action = new DestroyShipAction();

		QList<int> toDestroyList = GenerateToDestroyList(wasteShipList);

		if (!toDestroyList.size())
		{
			delete action;
			return QList<int>();
		}
		action->setShips(toDestroyList);
		_actionList.append(action);
		return wasteShipList;
	}
	return QList<int>();
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
		setToTerminate("Terminated:NoAction", true);
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

void ControlManager::Terminate(bool bSilent)
{
	if (_state != State::ToTerminate)
	{
		setState(State::Terminated, "Terminated", bSilent);
	}
	else
	{
		setState(State::Terminated, _stateStr.toStdString().c_str(), bSilent);
	}
	qDeleteAll(_actionList);
	_actionList.clear();
	setPauseNextVal(false);
	setInactiveWaiting(false);
	_target = ActionTarget::None;
	_autoExpeditioningFlag = false;
	MainWindow::mainWindow()->setJobTerminated();
	//	_southEastSetting.stopWhen = StopWhen::None;
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

bool ControlManager::checkShouldAutoWait()
{
	if (!_shouldAutoSwitchToExpeditionFlag)
	{
		return false;
	}

	if (_pauseNext)
	{
		return false;
	}

	int team;
	QList<int> excludeTeams;
	SingleExpedition* pExp;
	qint64 ct = TimerMainWindow::currentMS();
	qint64 waitMS = std::numeric_limits<int>::max();
	for (int i = 0; i < 3; i++)
	{
		waitMS = MainWindow::mainWindow()->timerWindow()->getMinExpeditionMS(team, excludeTeams);
		pExp = ExpeditionManager::getInstance().getShouldNextSchedule(team, ct, ct + waitMS);
		if (!pExp)
		{
			excludeTeams.append(team);
		}
		else
		{
			break;
		}
	}

	if (!pExp)
	{
		return false;
	}

	if (waitMS <= 0)
	{
		return true;
	}
	KanSaveData* pksd = &KanSaveData::getInstance();
	bool bFirst = true;
	for (const auto& deck : pksd->portdata.api_deck_port)
	{
		if (bFirst)
		{
			bFirst = false;
			continue;
		}
		if (!deck.api_mission.empty())
		{
			if (deck.api_mission.first() != 1)
			{
				if (excludeTeams.contains(deck.api_id - 1))
				{
					continue;
				}
				return true;
			}
		}
	}
	return false;
}

void ControlManager::setupAutoExpedition()
{
	if (checkShouldAutoWait())
	{
		if (_target != ActionTarget::Expedition)
		{
			_lastTarget = _target;
			Terminate(true);
			_autoExpeditioningFlag = true;
			if (BuildNext_Expedition())
			{
				StartJob();
			}
		}
	}
}

void ControlManager::setToTerminate(const char* title, bool forceSound)
{
	setState(State::ToTerminate, title, false, forceSound);
}

void ControlManager::createSSShipList()
{
	if (!_ssShips.isEmpty())
	{
		return;
	}

	KanSaveData* pksd = &KanSaveData::getInstance();
	KanDataConnector * pkdc = &KanDataConnector::getInstance();
	for (auto& ship : pksd->portdata.api_ship)
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
				for (QList<int>& group : _ssShips)
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

bool ControlManager::isAfterShip(const kcsapi_mst_ship* pmstship, const kcsapi_mst_ship* pomstship, QList<int> checkedIdList/*=QList<int>()*/)
{
	if (!pmstship || !pomstship)
	{
		return false;
	}

	bool bOk = false;
	int afterid = pomstship->api_aftershipid.toInt(&bOk);
	if (checkedIdList.contains(afterid))
	{
		return false;
	}
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
		checkedIdList.append(afterid);
		return isAfterShip(pmstship, ponewmstship, checkedIdList);
	}
}

int ControlManager::getOneWasteShipId(const QList<int>& excludes, QList<int>shipList/* = QList<int>()*/)
{
	KanSaveData* pksd = &KanSaveData::getInstance();
	KanDataConnector* pkdc = &KanDataConnector::getInstance();
	if (_destroyableMstIds.isEmpty())
	{
		LoadDestroyableList();
	}

	for (auto& ship : pksd->portdata.api_ship)
	{
		if (ship.api_lv == 1
			&& ship.api_locked == false
			&& ship.api_locked_equip == false
			&& ship.api_nowhp == ship.api_maxhp)
		{
			int shipid = ship.api_ship_id;
			const kcsapi_mst_ship * pmstship = pkdc->findMstShipFromShipid(shipid);
			if (!pmstship || !_destroyableMstIds.contains(shipid))
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
						for (auto excludeId : excludes)
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
					if (!shipList.isEmpty())
					{
						if (!shipList.contains(ship.api_id))
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

int ControlManager::getOneWasteShipId(int exclude1/*=-1*/, int exclude2/*=-1*/, QList<int>shipList/* = QList<int>()*/)
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
	return getOneWasteShipId(excludes, shipList);
}

bool ControlManager::isShipFull(int keep/* =3*/, bool checkSlotitem/* =true*/)
{
	KanSaveData* pksd = &KanSaveData::getInstance();
	int kancount = pksd->portdata.api_ship.count() + pksd->shipcountoffset;
	int kanmaxcount = pksd->portdata.api_basic.api_max_chara;
	if (kancount + keep <= kanmaxcount)
	{
		return false;
	}
	if (checkSlotitem)
	{
		// check slotitem too
		if (isSlotItemFull(keep * 3 - 3))
		{
			return false;
		}
	}
	return true;
}

bool ControlManager::isSlotItemFull(int keep /*= 6*/)
{
	KanSaveData* pksd = &KanSaveData::getInstance();
	int slotitemcount = pksd->slotitemdata.count() + pksd->slotitemcountoffset;
	int slotitemmaxcount = pksd->portdata.api_basic.api_max_slotitem;
	if (slotitemcount + keep <= slotitemmaxcount)
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

bool ControlManager::isLowCond(int shipno)
{
	KanDataConnector* pkdc = &KanDataConnector::getInstance();
	auto pship = pkdc->findShipFromShipno(shipno);
	if (pship)
	{
		if (KanDataCalc::GetCondState(pship->api_cond - 1) >= CondState::Middle)
		{
			return true;
		}
	}
	return false;
}

bool ControlManager::isShipInOtherTeam(int shipno, int team, bool excludeOnBoardingExpedition/*=false*/)
{
	KanSaveData* pksd = &KanSaveData::getInstance();

	QList<bool> onBoarding;
	onBoarding.append(true);
	onBoarding.append(true);
	onBoarding.append(true);

	QList<int> excludeTeams;
	TimerMainWindow* timerWindow = MainWindow::mainWindow()->timerWindow();
	for (int i = 0; i < 3; i++)
	{
		int tteam = -1;
		auto waitMS = timerWindow->getMinExpeditionMS(tteam, excludeTeams);
		if (tteam >= 0)
		{
			onBoarding[tteam - 1] = (waitMS <= 0);
			excludeTeams.append(tteam);
		}
	}

	for (auto& deck : pksd->portdata.api_deck_port)
	{
		if (deck.api_id == team + 1)
		{
			continue;
		}

		if (excludeOnBoardingExpedition)
		{
			int index = deck.api_id - 2;
			if (index >= 0)
			{
				if (onBoarding[index])
				{
					continue;
				}
			}
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

bool ControlManager::isShipDamaged(int shipno)
{
	KanDataConnector* pkdc = &KanDataConnector::getInstance();
	auto pship = pkdc->findShipFromShipno(shipno);
	if (pship)
	{
		if (pship->api_nowhp == pship->api_maxhp)
		{
			return false;
		}
	}
	return true;
}

bool ControlManager::isShipCharged(int shipno)
{
	KanDataConnector* pkdc = &KanDataConnector::getInstance();
	auto pship = pkdc->findShipFromShipno(shipno);
	if (pship)
	{
		auto pmstship = pkdc->findMstShipFromShipid(pship->api_ship_id);
		if (pmstship)
		{
			if (pship->api_fuel == pmstship->api_fuel_max
				&& pship->api_bull == pmstship->api_bull_max)
			{
				return true;
			}
		}
	}
	return false;
}

bool ControlManager::needChargeCondAirBase(bool checkCond)
{
	KanSaveData* pksd = &KanSaveData::getInstance();
	if ((checkCond&&pksd->airBaseBadCond) || pksd->airBaseNeedSupply)
	{
		return true;
	}
	return false;
}

bool ControlManager::isShipExist(int shipno)
{
	KanDataConnector* pkdc = &KanDataConnector::getInstance();
	auto pship = pkdc->findShipFromShipno(shipno);
	if (pship)
	{
		return true;
	}
	return false;
}

int ControlManager::getShipLevel(int shipno)
{
	KanDataConnector* pkdc = &KanDataConnector::getInstance();
	auto pship = pkdc->findShipFromShipno(shipno);
	if (pship)
	{
		return pship->api_lv;
	}
	return 0;
}

int ControlManager::getTotalSlotItemCountForID(int slotitemId)
{
	KanSaveData * pksd = &KanSaveData::getInstance();
	int count = 0;
	foreach(const kcsapi_slotitem& v, pksd->slotitemdata)
	{
		if (v.api_slotitem_id == slotitemId)
		{
			count++;
		}
	}
	return count;
}

bool ControlManager::isHenseiDone(const QList<int>& ships, int team, int index/*=-1*/)
{
	KanSaveData* pksd = &KanSaveData::getInstance();

	if (pksd->portdata.api_deck_port.size())
	{
		auto& fleet = pksd->portdata.api_deck_port.at(team);
		QList<int> nonEmptyShipList;
		for (auto id : fleet.api_ship)
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
				if (index >= fleet.api_ship.size())
				{
					return false;
				}
				else if (fleet.api_ship[index] == ships[index])
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
					if (i >= fleet.api_ship.size())
					{
						return false;
					}
					if (fleet.api_ship[i] != ships[i])
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

bool ControlManager::isFlagshipOnly(int team)
{
	return getTeamSize(team) == 1;
}

int ControlManager::isAllSSShips(int team)
{
	KanSaveData* pksd = &KanSaveData::getInstance();
	if (team < 0)
	{
		return 0;
	}

	if (pksd->portdata.api_deck_port.size() > team)
	{
		auto& fleet = pksd->portdata.api_deck_port.at(team);
		QList<int> nonEmptyShipList;
		for (auto id : fleet.api_ship)
		{
			if (id >= 0)
			{
				if (!isShipType(id, ShipType::SenSui) && !isShipType(id, ShipType::SenBou))
				{
					return 0;
				}
			}
		}
	}
	return getTeamSize(team);
}

int ControlManager::getTeamSize(int team)
{
	KanSaveData* pksd = &KanSaveData::getInstance();
	if (team < 0)
	{
		return 0;
	}
	if (pksd->portdata.api_deck_port.size() > team)
	{
		auto& fleet = pksd->portdata.api_deck_port.at(team);
		QList<int> nonEmptyShipList;
		for (auto id : fleet.api_ship)
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

bool ControlManager::hasSlotitem(int shipno, SlotitemType sitype, int count/*=1*/)
{
	if (count <= 0)
	{
		return true;
	}
	return KanDataConnector::getInstance().isShipHasSlotitem(shipno, sitype, count);
}

bool ControlManager::noSlotitem(int shipno)
{
	KanDataConnector* pkdc = &KanDataConnector::getInstance();

	auto pship = pkdc->findShipFromShipno(shipno);
	if (pship)
	{
		QList<int> fullSlot = pship->api_slot;
		fullSlot.append(pship->api_slot_ex);
		for (auto slotitemid : fullSlot)
		{
			if (slotitemid >= 0)
			{
				return false;
			}
		}
	}
	return true;
}

bool ControlManager::noAttackItem(int shipno)
{
	KanDataConnector* pkdc = &KanDataConnector::getInstance();

	auto pship = pkdc->findShipFromShipno(shipno);
	if (pship)
	{
		QList<int> fullSlot = pship->api_slot;
		fullSlot.append(pship->api_slot_ex);
		for (auto slotitemid : fullSlot)
		{
			auto* pslotitem = pkdc->findSlotitemFromId(slotitemid);
			if (pslotitem)
			{
				auto* pmstslotitem = pkdc->findMstSlotItemFromSlotitemid(pslotitem->api_slotitem_id);
				if (pmstslotitem)
				{
					if (pmstslotitem->api_type.size() > 2)
					{
						switch (pmstslotitem->api_type[2])
						{
						case (int)SlotitemType::SyuHou_S:
						case (int)SlotitemType::SyuHou_M:
						case (int)SlotitemType::SyuHou_L:
						case (int)SlotitemType::FuKuHou:
						case (int)SlotitemType::GyoRai:
						case (int)SlotitemType::ToKuSen:
						case (int)SlotitemType::KanBaKu:
						case (int)SlotitemType::KanKou:
						case (int)SlotitemType::SuiBaKu:
						case (int)SlotitemType::SyuHou_L_II:
							return false;
						default:
							break;
						}
					}
				}
			}
		}
	}
	return true;
}


int ControlManager::getShipCondVal(int shipno)
{
	KanDataConnector* pkdc = &KanDataConnector::getInstance();
	auto pship = pkdc->findShipFromShipno(shipno);
	if (pship)
	{
		return pship->api_cond;
	}
	return -1;
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
	if (_target == ActionTarget::Any)
	{
		auto it = _anySetting.cells.find(pksd->nextdata.api_no);
		if (it != _anySetting.cells.end())
		{
			if (it->bNeedS)
			{
				return true;
			}
			if (it->bKillFlagship && !pksd->lastFlagshipKilled)
			{
				return true;
			}
			if (it->bNeedWin && !pksd->lastWonAssumption)
			{
				return true;
			}
		}
		return false;
	}
	if (pksd->lastWonAssumption)
	{
		return false;
	}
	return true;
}

bool ControlManager::shouldRetrieve()
{
	if (_target == ActionTarget::Level)
	{
		return true;
	}
	return (hugestDamageInTeam(0) >= WoundState::Big || hugestDamageInTeam(1) >= WoundState::Big);
}

bool ControlManager::shouldRetrieveForAny()
{
	if (_target != ActionTarget::Any)
	{
		return false;
	}
	KanSaveData* pksd = &KanSaveData::getInstance();
	auto it = _anySetting.cells.find(pksd->nextdata.api_no);
	if (it != _anySetting.cells.end())
	{
		if (it->bReturnNext)
		{
			return true;
		}
	}
	return false;
}

bool ControlManager::shouldAskForProceedForAny()
{
	if (_target != ActionTarget::Any)
	{
		return false;
	}
	KanSaveData* pksd = &KanSaveData::getInstance();
	auto it = _anySetting.cells.find(pksd->nextdata.api_no);
	if (it != _anySetting.cells.end())
	{
		if (it->bAskForProceed)
		{
			return true;
		}
	}
	return false;
}

bool ControlManager::shouldTerminateForAny()
{
	if (_target != ActionTarget::Any)
	{
		return false;
	}
	KanSaveData* pksd = &KanSaveData::getInstance();
	auto it = _anySetting.cells.find(pksd->nextdata.api_no);
	if (it != _anySetting.cells.end())
	{
		if (it->bTerminateNext)
		{
			return true;
		}
	}
	return false;
}

WoundState ControlManager::hugestDamageInTeam(int team)
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
		auto& fleet = pksd->portdata.api_deck_port.at(team);
		if (fleet.api_ship.size())
		{
			for (auto shipno : fleet.api_ship)
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

void ControlManager::setSouthEastSetting(const SouthEastSetting& southEastSetting)
{
	_southEastSetting = southEastSetting;
}

void ControlManager::setKiraSetting(const KiraSetting& kiraSetting)
{
	_kiraSetting = kiraSetting;
}

void ControlManager::setAnySetting(const AnySetting& anySetting)
{
	_anySetting = anySetting;
}

void ControlManager::setDevelopSetting(const DevelopSetting& developSetting)
{
	_developSetting = developSetting;
}

bool ControlManager::isActiveRunning()
{
	if (!isRunning())
	{
		return false;
	}
	if (_inactiveWaiting)
	{
		return false;
	}
	return true;
}

bool ControlManager::checkColors(const QList<CheckColor>& checklist)
{
	if (checklist.empty())
	{
		return false;
	}
	auto w = MainWindow::mainWindow()->getBrowserWidget();
	auto image = w->grab().toImage();
	for (auto& item : checklist)
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

bool ControlManager::needChargeFlagship(int team)
{
	KanSaveData* pksd = &KanSaveData::getInstance();
	KanDataConnector* pkdc = &KanDataConnector::getInstance();

	if (!pksd->portdata.api_ship.size())
	{
		return false;
	}

	if (pksd->portdata.api_deck_port.size())
	{
		auto& fleet = pksd->portdata.api_deck_port.at(team);
		if (fleet.api_ship.size())
		{
			int shipno = fleet.api_ship.first();

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
				|| pship->api_bull < pmstship->api_bull_max
				)
			{
				return true;
			}
		}
	}
	return false;
}

bool ControlManager::needChargeAnyShip(int team)
{
	KanSaveData* pksd = &KanSaveData::getInstance();
	KanDataConnector* pkdc = &KanDataConnector::getInstance();

	if (!pksd->portdata.api_ship.size())
	{
		return false;
	}

	if (pksd->portdata.api_deck_port.size())
	{
		auto& fleet = pksd->portdata.api_deck_port.at(team);
		if (fleet.api_ship.size())
		{
			for (auto shipno : fleet.api_ship)
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

void ControlManager::setState(State state, const char* str, bool bSilent, bool forceSound)
{
	if (_state != state)
	{
		_state = state;
		if (_state == State::Terminated || _state == State::ToTerminate)
		{
			_lastTerminationReason = str;
			if (forceSound
				|| (_target != ActionTarget::Expedition && _target != ActionTarget::None)
				)
			{
				MainWindow::mainWindow()->timerWindow()->playSound(TimerMainWindow::SoundIndex::Terminated, bSilent);
			}

			_autoExpeditioningFlag = false;
			_target = ActionTarget::None;
			if (!_lastTerminationReason.isEmpty())
			{
				QTimer::singleShot(4000, Qt::PreciseTimer, [this]()
				{
					if (_stateStr.startsWith(_lastTerminationReason) && !_lastTerminationReason.isEmpty())
					{
						_stateStr = _stateStr.replace(0, _lastTerminationReason.count() + 1, "");
					}
					_lastTerminationReason = "";
					if (this->_state == State::Terminated)
					{
						setStateStr("");
					}
					else
					{
						setStateStr(_stateStr);
					}
				});
			}
		}
	}
	setStateStr(str);
}

void ControlManager::setStateStr(const QString& str)
{
	_stateStr = str;
	if (!_stateStr.startsWith(_lastTerminationReason))
	{
		_stateStr = _lastTerminationReason + " " + _stateStr;
	}
	MainWindow::mainWindow()->timerWindow()->setTitle(_stateStr);
}

void ControlManager::switchBackToLastAction()
{
	_target = _lastTarget;
	_autoExpeditioningFlag = false;
}

void ControlManager::clearLastTarget()
{
	_lastTarget = ActionTarget::None;
}

ControlManager::AnySetting ControlManager::getAnyTemplateSetting(int area, int map)
{
	QPair<int, int> pair = QPair<int, int>(area, map);
	if (_anyTemplateSettings.contains(pair))
	{
		return _anyTemplateSettings[pair];
	}
	AnySetting setting;
	setting.area = area;
	setting.map = map;
	return setting;
}

void ControlManager::moveMouseToAndClick(float x, float y, float offsetX /*= 5*/, float offsetY /*= 3*/)
{
	QPointF ptAdjusted = QPointF(x + randVal(-offsetX, offsetX), y + randVal(-offsetY, offsetY));

	if (MainWindow::mainWindow()->getWebWidgetType() == WebWidgetType::IE)
	{
#ifdef Q_OS_WIN
		INPUT input;
		input.type = INPUT_MOUSE;
		input.mi.dx = ptAdjusted.x() + 5;
		input.mi.dy = ptAdjusted.y() + 25;
		input.mi.dx *= (65536 / GetSystemMetrics(SM_CXSCREEN));
		input.mi.dy *= (65536 / GetSystemMetrics(SM_CYSCREEN));
		input.mi.dwFlags = (MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_MOVE | MOUSEEVENTF_LEFTDOWN | MOUSEEVENTF_LEFTUP);
		input.mi.mouseData = 0;
		input.mi.dwExtraInfo = NULL;
		input.mi.time = 0;
		SendInput(1, &input, sizeof(INPUT));
		/*
		int adjx = ptAdjusted.x();
		int adjy = ptAdjusted.y() + 20;
		HWND hwnd = (HWND)MainWindow::mainWindow()->winId();
		PostMessage(hwnd, WM_LBUTTONDOWN, 0, (adjx) & ((adjy) << 16));
		PostMessage(hwnd, WM_LBUTTONUP, 0, (adjx) & ((adjy) << 16));
		*/
#endif
	}
	else
	{
		auto sendMouseEvents = [this, ptAdjusted](QWidget* w){
			if (!w)
			{
				return;
			}
#ifdef Q_OS_WIN
			QMouseEvent e(QEvent::MouseButtonPress, ptAdjusted, Qt::LeftButton, Qt::LeftButton, Qt::NoModifier);
			QApplication::sendEvent(w, &e);

			QMouseEvent e2(QEvent::MouseButtonRelease, ptAdjusted, Qt::LeftButton, Qt::LeftButton, Qt::NoModifier);
			QApplication::sendEvent(w, &e2);
#else
			QPoint ptG = w->mapToGlobal(QPoint(ptAdjusted.x(), ptAdjusted.y()));
			MainWindow::mainWindow()->clickAtGlobalPos(ptG);

#endif


		};

		auto browserWidget = MainWindow::mainWindow()->getBrowserWidget();
		if (MainWindow::mainWindow()->getWebWidgetType() == WebWidgetType::WebEngine)
		{
#if (QT_VERSION >= QT_VERSION_CHECK(5, 6, 0))
			QWebEngineView *webView = dynamic_cast<QWebEngineView *>(browserWidget);
			if (NULL != webView)
			{
				Q_FOREACH(QObject* obj, webView->page()->view()->children())
				{
					sendMouseEvents(qobject_cast<QWidget*>(obj));
				}
			}

			// reset mouse pos for webengine
			moveMouseTo(0, 0);
#endif
		}
		else
		{
			sendMouseEvents(browserWidget);
		}

	}

}

void ControlManager::moveMouseTo(float x, float y, float offsetX /*= 5*/, float offsetY /*= 3*/)
{
	QPointF ptAdjusted = QPointF(x + randVal(-offsetX, offsetX), y + randVal(-offsetY, offsetY));
	if (MainWindow::mainWindow()->getWebWidgetType() == WebWidgetType::IE)
	{
#ifdef Q_OS_WIN
		INPUT input;
		input.type = INPUT_MOUSE;
		input.mi.dx = ptAdjusted.x() + 5;
		input.mi.dy = ptAdjusted.y() + 25;
		input.mi.dx *= (65536 / GetSystemMetrics(SM_CXSCREEN));
		input.mi.dy *= (65536 / GetSystemMetrics(SM_CYSCREEN));
		input.mi.dwFlags = (MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_MOVE);
		input.mi.mouseData = 0;
		input.mi.dwExtraInfo = NULL;
		input.mi.time = 0;
		SendInput(1, &input, sizeof(INPUT));

		/*
		int adjx = ptAdjusted.x();
		int adjy = ptAdjusted.y() + 20;
		HWND hwnd = (HWND)MainWindow::mainWindow()->winId();
		PostMessage(hwnd, WM_MOUSEMOVE, 0, (adjx)& ((adjy) << 16));
		*/
#endif
	}
	else
	{
		auto sendMouseEvents = [this, ptAdjusted](QWidget* w){
			if (!w)
			{
				return;
			}
			QMouseEvent e(QEvent::MouseMove, ptAdjusted, Qt::NoButton, Qt::NoButton, Qt::NoModifier);
			QApplication::sendEvent(w, &e);
		};

		auto browserWidget = MainWindow::mainWindow()->getBrowserWidget();
		if (MainWindow::mainWindow()->getWebWidgetType() == WebWidgetType::WebEngine)
		{
#if (QT_VERSION >= QT_VERSION_CHECK(5, 6, 0))
			QWebEngineView *webView = dynamic_cast<QWebEngineView *>(browserWidget);
			if (NULL != webView)
			{
				Q_FOREACH(QObject* obj, webView->page()->view()->children())
				{
					sendMouseEvents(qobject_cast<QWidget*>(obj));
				}
			}
#endif
		}
		else
		{
			sendMouseEvents(browserWidget);
		}

	}
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
	qreal value = static_cast<qreal>(qrand()) * (max - min) / RAND_MAX + min;
	return value;
}


