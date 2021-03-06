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

#include "RemoteNotifyHandler.h"

#ifdef Q_OS_WIN
#include <windows.h>
#endif

#define COL_ALLOWRANCE 3
#define COND_DAIHATSU	80

#define SHIP_BAY		111563
#define SHIP_CHIYODA_2	1353
#define SHIP_CHITOSE_2	123

QMap<int, int> ControlManager::areaIndexMap =
{
	{ 1, 1 },
	{ 2, 2 },
	{ 3, 3 },
	{ 7, 4 },
	{ 4, 5 },
	{ 5, 6 },
	{ 6, 7 },
};

ControlManager::ControlManager()
{
}


ControlManager::~ControlManager()
{
}

bool ControlManager::BuildNext_Kira()
{
	_target = ActionTarget::Kira;
	if (_kiraSetting.repeatCounter == 0)
	{
		_morningSetting.lastBuiltState = MorningStage::AssumeJobDone;
		_target = ActionTarget::None;
		_kiraSetting.repeatCounter = -1;
		return false;
	}

	pushPreSupplyCheck();
	QList<int> willBeInDockList;
	pushPreRepairCheck(willBeInDockList, false, false, false, true, false);
	if (_kiraSetting.forceCurrent)
	{
		_todoKiraShipids.clear();
		_todoKiraShipids.push_front(getCurrentFlagshipId());
	}
	if (_todoKiraShipids.empty())
	{
		setToTerminate("Terminated:NoShipForKira", false, RemoteNotifyHandler::Level::Low);
		return false;
	}
	if (flagshipSevereDamaged())
	{
		setToTerminate("Terminated:FlagshipNGKira"); // fatal
		return false;
	}

	QList<int> wasteShipListByPreCheck = pushPreShipFullCheck();

	int togoShipId = _todoKiraShipids.at(0);
	while (isShipKiraDone(togoShipId)
		|| isShipInOtherTeam(togoShipId, 0)
		|| isShipInDock(togoShipId)
		/*
	 || hasSlotitem(togoShipId, SlotitemType::Sonar)
	 || hasSlotitem(togoShipId, SlotitemType::BaKuRai)
	 || hasSlotitem(togoShipId, SlotitemType::Sonar_L)
			*/
		|| hasSlotitem(togoShipId, SlotitemType::YuSou, 3)
		|| noSlotitem(togoShipId)
		|| noAttackItem(togoShipId)
		|| (!_kiraSetting.forceCurrent&&isLowCond(togoShipId))
		|| willBeInDockList.contains(togoShipId))
	{
		_todoKiraShipids.removeAt(0);
		if (_todoKiraShipids.empty())
		{
			setToTerminate("Terminated:NoShip", false, RemoteNotifyHandler::Level::Info);
			return false;
		}
		togoShipId = _todoKiraShipids.at(0);
	}

	int flagshipid = getCurrentFlagshipId();
	int flagshipIndex = _todoKiraShipids.indexOf(flagshipid);
	if (flagshipIndex > 0 && !isShipKiraDone(flagshipid))
	{
		_todoKiraShipids.swap(flagshipIndex, 0);
	}

	togoShipId = _todoKiraShipids.at(0);
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

	_morningSetting.lastBuiltState = MorningStage::AssumeJobDone;
	if (_kiraSetting.repeatCounter > 0)
	{
		_kiraSetting.repeatCounter--;
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


bool ControlManager::BuildNext_Bauxite()
{
	_target = ActionTarget::Bauxite;

	pushPreSupplyCheck();
	QList<int> willBeInDockList;

	int useUpToDockSize = 3;

	pushPreRepairCheck(willBeInDockList, true, true, false, false, true, useUpToDockSize);

	pushPreShipFullCheck();

	SortieAction* sortieAction = new SortieAction();
	sortieAction->setAreaAndMap(2, 2);
	_actionList.append(sortieAction);
	_actionList.append(new SortieCommonAdvanceAction());
	_actionList.append(new ChargeAction());
	_actionList.append(new RepeatAction());

	_morningSetting.lastBuiltState = MorningStage::AssumeJobDone;
	setState(State::Ready, "Ready");
	return true;

}


bool ControlManager::BuildNext_Bullet()
{
	_target = ActionTarget::Bullet;

	pushPreSupplyCheck();
	QList<int> willBeInDockList;

	int useUpToDockSize = 4;

	pushPreRepairCheck(willBeInDockList, false, false, false, true, true, useUpToDockSize, false, true);

	pushPreShipFullCheck();

	QList<int> ships;
	QList<int> sortInTeamShips;
	QString errorMessage;
	if (!chooseSSShipList(1, ships, sortInTeamShips, willBeInDockList, errorMessage, false, true, 30))
	{
		setToTerminate(errorMessage.toLocal8Bit(), false, RemoteNotifyHandler::Level::Info);
		return false;
	}

	/*
	// change hensei sort
	auto chSortAction = new ChangeHenseiAction();
	chSortAction->setShips(sortInTeamShips);
	_actionList.append(chSortAction);
	*/

	// change hensei
	auto chAction = new ChangeHenseiAction();
	chAction->setShips(ships);
	_actionList.append(chAction);

	SortieAction* sortieAction = new SortieAction();
	sortieAction->setAreaAndMap(3, 2);
	_actionList.append(sortieAction);
	_actionList.append(new SortieAdvanceAction());
	_actionList.append(new ChargeAction());
	_actionList.append(new RepeatAction());

	_morningSetting.lastBuiltState = MorningStage::AssumeJobDone;
	setState(State::Ready, "Ready");
	return true;

}

bool ControlManager::BuildNext_Fuel()
{
	_target = ActionTarget::Fuel;
	int flagshipid = getCurrentFlagshipId();
	if (!isFlagshipOnly(0) ||
		(!isShipType(flagshipid, ShipType::SenSui) && !isShipType(flagshipid, ShipType::SenBou)))
	{
		// southeast
		_target = ActionTarget::SouthEast;
	}

	bool stopWhenHit = stopWhenCheck();

	if (stopWhenHit && isMorningMode())
	{
		_morningSetting.lastBuiltState = MorningStage::AssumeJobDone;
		_target = ActionTarget::None;
		return false;
	}

	pushPreSupplyCheck();
	QList<int> willBeInDockList;

	int useUpToDockSize = 3;
	if (_southEastSetting.stopWhen == StopWhen::None && _target == ActionTarget::Fuel)
	{
		useUpToDockSize = 4;
	}

	pushPreRepairCheck(willBeInDockList, false, false, true, true, false, useUpToDockSize);

	if (stopWhenHit)
	{
		setToTerminate("Termination:StopWhenDone", false, RemoteNotifyHandler::Level::Low);
		return false;
	}

	int teamSize = getTeamSize(0);
	if (_target == ActionTarget::SouthEast)
	{
		KanSaveData* pksd = &KanSaveData::getInstance();
		if (pksd->totalSouthEastWin >= 5)
		{
			setToTerminate("Terminated:DoneFuelMission", false, RemoteNotifyHandler::Level::Low);
			return false;
		}

		if (teamSize < 4)
		{
			// keep 4 and up for formation
			teamSize = 4;
		}
	}

	pushPreShipFullCheck();

	QList<int> ships;
	QList<int> sortInTeamShips;
	QString errorMessage;
	if (!chooseSSShipList(teamSize, ships, sortInTeamShips, willBeInDockList, errorMessage, _target == ActionTarget::Fuel))
	{
		setToTerminate(errorMessage.toLocal8Bit(), false, RemoteNotifyHandler::Level::Info);
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
	if (_southEastSetting.is2_2)
	{
		sortieAction->setAreaAndMap(2, 2);
	}
	else
	{
		sortieAction->setAreaAndMap(2, 3);
	}
	_actionList.append(sortieAction);
	_actionList.append(new SortieCommonAdvanceAction());
	_actionList.append(new ChargeAction());
	_actionList.append(new RepeatAction());

	_morningSetting.lastBuiltState = MorningStage::AssumeJobDone;
	setState(State::Ready, "Ready");
	return true;
}

class BestShipInfo
{
public:
	kcsapi_ship2* pship;
	qint64 nDockCompleteTime = 0;

	BestShipInfo(kcsapi_ship2* ship, qint64 completeTime)
	{
		pship = ship;
		nDockCompleteTime = completeTime;
	}
};

void ControlManager::clearSSShipList()
{
	_ssShips.clear();
}

bool ControlManager::chooseSSShipList(int teamSize, QList<int>& ships, QList<int>& sortInTeamShips, QList<int> excludeShipList, QString& errorMessage, bool onlyHighKaihi/*=false*/, bool needDamageControl/*=false*/, int limitHighestLevel/*=-1*/)
{
	KanSaveData* pksd = &KanSaveData::getInstance();
	errorMessage = "";

	if (_ssShips.isEmpty())
	{
		createSSShipList(limitHighestLevel > 0);
	}

	const int kaihiBorderLine = 0;

	// find possible hensei
	KanDataConnector* pkdc = &KanDataConnector::getInstance();
	// find best in each group
	QList<BestShipInfo> bestShips;
	for (auto& group : _ssShips)
	{
		kcsapi_ship2* pbestship = nullptr;
		kcsapi_ship2* pbestInDockShip = nullptr;
		qint64 bestInDockCompleteTime = std::numeric_limits<qint64>::max();
		for (auto ssid : group)
		{
			auto pcurship = pkdc->findShipFromShipno(ssid);

			if (onlyHighKaihi)
			{
				if (pcurship->api_kaihi[0] < kaihiBorderLine)
				{
					continue;
				}
			}

			if (needDamageControl)
			{
				if (!pkdc->isShipHasSlotitem(pcurship, SlotitemType::OuKyu))
				{
					continue;
				}
			}

			if (limitHighestLevel > 0)
			{
				if (pcurship->api_lv > limitHighestLevel)
				{
					continue;
				}
			}

			// cond ng later
			/*
			if (pcurship->api_cond <= _sortieMinCond)
			{
			continue;
			}
			*/
			// damage ng
			if ((KanDataCalc::GetWoundState(pcurship->api_nowhp, pcurship->api_maxhp) >= WoundState::Big || KanDataCalc::GetWoundState(pcurship->api_nowhp, pcurship->api_maxhp) >= WoundState::Middle && !needDamageControl) && !isShipInDock(ssid))
			{
				continue;
			}

			// in other team
			if (isShipInOtherTeam(ssid, 0))
			{
				continue;
			}

			if (excludeShipList.contains(ssid))
			{
				continue;
			}

			// in dock
			qint64 completetime = 0;
			if (isShipInDock(ssid, &completetime))
			{
				if (completetime < bestInDockCompleteTime)
				{
					bestInDockCompleteTime = completetime;
					pbestInDockShip = pcurship;
				}
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
			bestShips.append(BestShipInfo(pbestship, 0));
		}
		else if (pbestInDockShip)
		{
			bestShips.append(BestShipInfo(pbestInDockShip, bestInDockCompleteTime));
		}
	}

	if (bestShips.size() < teamSize)
	{
		errorMessage = "Terminated:No Enough Ship";
		return false;
	}

	// sort bestships
	std::sort(bestShips.begin(), bestShips.end(), [](const  BestShipInfo& left, const BestShipInfo& right)
	{
		if (left.nDockCompleteTime < right.nDockCompleteTime)
		{
			return true;
		}
		else if (left.nDockCompleteTime == right.nDockCompleteTime)
		{
			if (left.pship->api_cond > right.pship->api_cond)
			{
				return true;
			}
			else if (left.pship->api_cond == right.pship->api_cond)
			{
				if (left.pship->api_kaihi[0] > right.pship->api_kaihi[0])
				{
					return true;
				}
				else if (left.pship->api_kaihi[0] == right.pship->api_kaihi[0])
				{
					if (left.pship->api_lucky[0] > right.pship->api_lucky[0])
					{
						return true;
					}
				}
			}
		}

		return false;
	});

	qint64 maxWaitMS = 0;

	qint64 ctUtc = TimerMainWindow::currentMSUtc();

	qint64 minCompleteTime = bestShips.at(0).nDockCompleteTime;
	if (minCompleteTime > 0)
	{
		maxWaitMS = minCompleteTime - ctUtc - 60 * 1000 + 5000 + randVal(-1000, 1000);
	}

	int minCond = bestShips.at(teamSize - 1).pship->api_cond;
	if (minCond <= _sortieMinCond)
	{
		// wait
		qint64 waitMS = qCeil((_sortieWaitCond - minCond) / 3.0) * 3 * 60 * 1000;
		if (waitMS > maxWaitMS)
		{
			maxWaitMS = waitMS;
		}
	}

	if (maxWaitMS > 0)
	{
		auto waitAction = new WaitCondAction();
		waitAction->setWaitMS(maxWaitMS, false);
		_actionList.append(waitAction);
	}

	ships.clear();
	for (int i = 0; i < teamSize; i++)
	{
		ships.push_front(bestShips.at(i).pship->api_id);
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

bool ControlManager::BuildNext_Level()
{
	_target = ActionTarget::Level;
	pushPreSupplyCheck();
	QList<int> willBeInDockList;
	pushPreRepairCheck(willBeInDockList, false, false, true, true, false);

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
							setToTerminate("Terminated:MoreThanOneMaruyu", false, RemoteNotifyHandler::Level::Low);
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
							setToTerminate("Terminated:NeedNDock", false, RemoteNotifyHandler::Level::Low);
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
		setToTerminate("Terminated:TeamSize", false, RemoteNotifyHandler::Level::Low);
		return false;
	}
	if (!bHaveMaruyu)
	{
		setToTerminate("Terminated:NoMaruyu", false, RemoteNotifyHandler::Level::Low);
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
					&& !isShipInOtherTeam(ship.api_id, -1)
					&& !willBeInDockList.contains(ship.api_id))
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
			setToTerminate("Terminated:NoAvailableMaruyu", false, RemoteNotifyHandler::Level::Info);
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
		waitAction->setWaitMS(waitMS, false);
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

	QList<int> willBeInDockList;
	auto fastRepairShips = pushPreRepairCheck(willBeInDockList, true, true, false, false, false);

	KanSaveData* pksd = &KanSaveData::getInstance();
	KanDataConnector* pkdc = &KanDataConnector::getInstance();

	pushPreShipFullCheck();

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
						if (!fastRepairShips.contains(pship->api_id))
						{
							minCond = pship->api_cond;
						}
					}

					if (!fastRepairShips.contains(pship->api_id))
					{
						WoundState ws = KanDataCalc::GetWoundState(pship->api_nowhp, pship->api_maxhp);

						if (ws >= WoundState::Middle)
						{
							setToTerminate("Terminated:Damage", false, RemoteNotifyHandler::Level::Low);
							return false;
						}
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
		setToTerminate("Terminated:ShipCount", false, RemoteNotifyHandler::Level::Low);
		return false;
	}

	if (!allSS)
	{
		if (!pksd->deckSaveData.empty())
		{
			if (pksd->deckSaveData.first().totalMinTaiku < 350)
			{
				setToTerminate("Terminated:Taiku", false, RemoteNotifyHandler::Level::Low);
				return false;
			}
		}
		if (totalYusou < 4)
		{
			setToTerminate("Terminated:Yusou", false, RemoteNotifyHandler::Level::Low);
			return false;
		}
	}

	if (minCond <= _sortieMinCond)
	{
		// wait
		qint64 waitMS = qCeil((_sortieWaitCond - minCond) / 3.0) * 3 * 60 * 1000;
		auto waitAction = new WaitCondAction();
		waitAction->setWaitMS(waitMS, false);
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

void ControlManager::GenerateMissionTodo()
{
	const auto& pksd = KanSaveData::getInstance();
	_missionSetting.todoMissionList.clear();
	_missionSetting.acceptedMissionList.clear();
	_missionSetting.dropMissionList.clear();
	_missionSetting.todoGreedyMissionList.clear();

	for (const auto& quest : pksd.activeQuestData)
	{
		if (!_missionSetting.acceptedMissionList.contains(quest.api_no))
		{
			_missionSetting.acceptedMissionList.append(quest.api_no);
		}
	}

	bool yuSouMode = false;
	bool southEastMode = false;

	QList<int> todoMissionList;
	todoMissionList.append((int)MissionDefines::Expedition3);
	todoMissionList.append((int)MissionDefines::Expedition10);

	todoMissionList.append((int)MissionDefines::WeeklyExpedition);

	todoMissionList.append((int)MissionDefines::NyuKyo5);
	todoMissionList.append((int)MissionDefines::Charge12);

	todoMissionList.append((int)MissionDefines::First1_1);
	todoMissionList.append((int)MissionDefines::Second1_1);

	todoMissionList.append((int)MissionDefines::WeeklyA);

	QList<int> toDropMissionList;

	foreach(const auto& quest, pksd.fullQuestData)
	{
		if (quest.api_no == (int)MissionDefines::YuSou3)
		{
			yuSouMode = true;
			toDropMissionList.append((int)MissionDefines::WeeklyI);
			toDropMissionList.append((int)MissionDefines::KuBou3);

			todoMissionList.append((int)MissionDefines::YuSou3);

			todoMissionList.append((int)MissionDefines::WeeklyRo);

			todoMissionList.append((int)MissionDefines::YuSou5);

			todoMissionList.append((int)MissionDefines::WeeklyYuSou);
			break;
		}
		else if (quest.api_no == (int)MissionDefines::SouthEast5)
		{
			southEastMode = true;
			toDropMissionList.append((int)MissionDefines::YuSou5);
			toDropMissionList.append((int)MissionDefines::WeeklyRo);
			toDropMissionList.append((int)MissionDefines::WeeklyYuSou);

			todoMissionList.append((int)MissionDefines::SouthEast5);

			todoMissionList.append((int)MissionDefines::WeeklyI);

			todoMissionList.append((int)MissionDefines::KuBou3);
			break;
		}
	}
	todoMissionList.append((int)MissionDefines::Defeat10);

	foreach(const int id, todoMissionList)
	{
		foreach(const auto& quest, pksd.fullQuestData)
		{
			if (quest.api_no == id)
			{
				_missionSetting.todoMissionList.append(quest.api_no);
			}
		}
	}
	foreach(const int id, toDropMissionList)
	{
		foreach(const auto& quest, pksd.fullQuestData)
		{
			if (quest.api_no == id)
			{
				_missionSetting.dropMissionList.append(quest.api_no);
			}
		}
	}

}

bool ControlManager::BuildNext_Morning(bool isFromRepeating)
{
	_target = ActionTarget::Morning;
	_parentTarget = ActionTarget::Morning;
	pushPreSupplyCheck();

	QList<int> willBeInDockList;
	pushPreRepairCheck(willBeInDockList, false, false, true, true, false);

	pushPreShipFullCheck();

	KanSaveData* pksd = &KanSaveData::getInstance();

	if (!isFromRepeating)
	{
		LoadToDoShipList_Kira();
		_morningSetting.morningStage = MorningStage::None;
	}

	// if quest list hit override step
	bool hasWeeklyA = false;
	for (const auto& quest : pksd->activeQuestData)
	{
		bool isCompleted = quest.api_state == 3 || _morningSetting.lastBuiltState == MorningStage::AssumeJobDone;
		switch ((MissionDefines)quest.api_no)
		{
		case MissionDefines::First1_1:
			_morningSetting.morningStage = isCompleted ? MorningStage::Second1_1_Mission : MorningStage::First1_1;
			break;
		case MissionDefines::Second1_1:
			_morningSetting.morningStage = isCompleted ? MorningStage::KiraAMonday_Mission : MorningStage::Second1_1;
			break;
		case MissionDefines::WeeklyA:
			_morningSetting.morningStage = isCompleted ? MorningStage::YuSou3_Mission : MorningStage::KiraAMonday;
			hasWeeklyA = true;
			break;
		case MissionDefines::YuSou3:
			if (!hasWeeklyA)
			{
				_morningSetting.morningStage = isCompleted ? MorningStage::SouthEast5_Mission : MorningStage::YuSou3;
			}
			break;
		case MissionDefines::SouthEast5:
			if (isCompleted)
			{
				_morningSetting.morningStage = MorningStage::Done;
				LoadAnyTemplateSettings();
				AnySetting setting = getAnyTemplateSetting(1, 5);
				setAnySetting(setting);

				KanSaveData::getInstance().totalAnyCount = 0;
				KanDataConnector::getInstance().callUpdateOverviewTable();
				setToTerminate("Morning:Done", true, RemoteNotifyHandler::Level::Low);
				return false;
			}
			else if (!hasWeeklyA)
			{
				_morningSetting.morningStage = MorningStage::SouthEast5;
			}
			break;
		}
		if (!_missionSetting.acceptedMissionList.contains(quest.api_no))
		{
			_missionSetting.acceptedMissionList.append(quest.api_no);
		}
	}

	if (_morningSetting.morningStage == MorningStage::SouthEast5)
	{
		if (pksd->totalSouthEastWin >= 5)
		{
			_morningSetting.morningStage = MorningStage::None;
			LoadAnyTemplateSettings();
			AnySetting setting = getAnyTemplateSetting(1, 5);
			setAnySetting(setting);

			KanSaveData::getInstance().totalAnyCount = 0;
			KanDataConnector::getInstance().callUpdateOverviewTable();
			setToTerminate("Morning:Done", true, RemoteNotifyHandler::Level::Low);
			return false;
		}
	}

	if (isFromRepeating)
	{
		if (_morningSetting.morningStage == MorningStage::None || _morningSetting.morningStage == _morningSetting.lastBuiltState)
		{
			setToTerminate("Morning:Fatal", true, RemoteNotifyHandler::Level::Low);
			return false;
		}
	}

	_morningSetting.lastBuiltState = _morningSetting.morningStage;

	_missionSetting.todoMissionList.clear();
	_missionSetting.todoGreedyMissionList.clear();
	_missionSetting.dropMissionList.clear();

	switch (_morningSetting.morningStage)
	{
	case MorningStage::None:
	case MorningStage::First1_1_Mission:

		_target = ActionTarget::Mission;
		_actionList.append(new MissionAction());
		_actionList.append(new RepeatAction());

		break;
	case MorningStage::First1_1:

		if (_missionSetting.acceptedMissionList.contains((int)MissionDefines::First1_1))
		{
			KanSaveData::getInstance().resetTotals();
			KanDataConnector::getInstance().callUpdateOverviewTable();

			_target = ActionTarget::Kira;
			_kiraSetting.repeatCounter = 1;
			_actionList.append(new RepeatAction());
		}
		else
		{
			_morningSetting.morningStage = MorningStage::Second1_1_Mission;
			return false;
		}

		break;
	case MorningStage::Second1_1_Mission:

		_target = ActionTarget::Mission;
		_actionList.append(new MissionAction());
		_actionList.append(new RepeatAction());

		break;
	case MorningStage::Second1_1:

		if (_missionSetting.acceptedMissionList.contains((int)MissionDefines::Second1_1))
		{
			_target = ActionTarget::Kira;
			_kiraSetting.repeatCounter = 1;
			_actionList.append(new RepeatAction());
		}
		else
		{
			_morningSetting.morningStage = MorningStage::KiraAMonday_Mission;
			return false;
		}

		break;
	case MorningStage::KiraAMonday_Mission:

		_target = ActionTarget::Mission;
		_actionList.append(new MissionAction());
		_actionList.append(new RepeatAction());

		break;
	case MorningStage::KiraAMonday:

		if (_missionSetting.acceptedMissionList.contains((int)MissionDefines::WeeklyA))
		{
			_target = ActionTarget::Kira;
			_actionList.append(new RepeatAction());
		}
		else
		{
			_morningSetting.morningStage = MorningStage::YuSou3_Mission;
			return false;
		}

		break;
	case MorningStage::YuSou3_Mission:

		_target = ActionTarget::Mission;

		_actionList.append(new MissionAction());
		_actionList.append(new RepeatAction());

		break;
	case MorningStage::YuSou3:

		if (_missionSetting.acceptedMissionList.contains((int)MissionDefines::YuSou3))
		{
			_target = ActionTarget::Any;

			LoadAnyTemplateSettings();
			AnySetting setting = getAnyTemplateSetting(2, 2);
			setting.onlySSTeamSize = 3;

			setting.count = 2;
			if (_missionSetting.acceptedMissionList.contains((int)MissionDefines::YuSou5))
			{
				setting.count = 3;
			}
			setting.additionalToSSTeam.append(SHIP_BAY);
			setting.additionalToSSTeam.append(SHIP_CHIYODA_2);
			setting.additionalToSSTeam.append(SHIP_CHITOSE_2);
			setting.allowMiddleDamageSortie = true;

			setAnySetting(setting);

			KanSaveData::getInstance().totalAnyCount = 0;
			KanDataConnector::getInstance().callUpdateOverviewTable();

			_target = ActionTarget::Any;
			_actionList.append(new RepeatAction());
		}
		else
		{
			_morningSetting.morningStage = MorningStage::SouthEast5_Mission;
			return false;
		}

		break;
	case MorningStage::SouthEast5_Mission:

		_target = ActionTarget::Mission;

		_actionList.append(new MissionAction());
		_actionList.append(new RepeatAction());

		break;
	case MorningStage::SouthEast5:

		if (_missionSetting.acceptedMissionList.contains((int)MissionDefines::SouthEast5))
		{
			LoadAnyTemplateSettings();
			AnySetting setting = getAnyTemplateSetting(2, 1);
			setting.count = 5;
			setting.onlySSTeamSize = 6;

			setAnySetting(setting);

			_target = ActionTarget::Any;

			KanSaveData::getInstance().totalAnyCount = 0;
			KanDataConnector::getInstance().callUpdateOverviewTable();

			_actionList.append(new RepeatAction());
		}
		else
		{
			LoadAnyTemplateSettings();
			AnySetting setting = getAnyTemplateSetting(1, 5);
			setAnySetting(setting);

			_morningSetting.morningStage = MorningStage::Done;
			setToTerminate("Morning:Done", true, RemoteNotifyHandler::Level::Low);
			return false;
		}

		break;
	default:
		break;
	}

	_missionSetting.acceptedMissionList.clear();

	setState(State::Ready, "Ready");
	return true;
}

bool ControlManager::BuildNext_Mission()
{
	_target = ActionTarget::Mission;
	pushPreSupplyCheck();

	QList<int> willBeInDockList;
	pushPreRepairCheck(willBeInDockList, false, false, true, true, false);

	pushPreShipFullCheck();

	if (_missionSetting.todoMissionList.isEmpty())
	{
		setToTerminate("Terminated:NoMission", true, RemoteNotifyHandler::Level::Low);
		return false;
	}

	_actionList.append(new MissionAction());
	_actionList.append(new RepeatAction());
	setState(State::Ready, "Ready");
	return true;
}

bool ControlManager::switchToGreedyMission()
{
	auto &pksd = KanSaveData::getInstance();
	if (_missionSetting.todoGreedyMissionList.isEmpty() || pksd.activeQuestData.count() >= pksd.portdata.api_parallel_quest_count)
	{
		_missionSetting.todoGreedyMissionList.clear();
		return false;
	}
	_missionSetting.todoMissionList.clear();
	_missionSetting.todoMissionList.append(_missionSetting.todoGreedyMissionList);
	_missionSetting.todoGreedyMissionList.clear();
	return true;
}

void ControlManager::LoadAnyTemplateSettings()
{
	_anyTemplateSettings.clear();

	QString filename = MainWindow::getAbsoluteResourcePath();
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
			if (key.compare("Team", Qt::CaseInsensitive) == 0)
			{
				int team = setting->value(key).toInt();
				_anyTemplateSettings[pair].team = team;
				continue;
			}

			int cell = key.toInt(&bOk);

			QRegExp rxCheck("MapEx[0-9]?Check");
			rxCheck.setCaseSensitivity(Qt::CaseInsensitive);
			QRegExp rxClick("MapEx[0-9]?Click");
			rxClick.setCaseSensitivity(Qt::CaseInsensitive);

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
				else if (key.contains(rxCheck))
				{
					if (splited.size() != 10)
					{
						// error
						continue;
					}
					int index = 0;
					QRegExp rx("(\\d+)");
					if (rx.indexIn(key) != -1)
					{
						index = rx.cap(1).toInt() - 1;
					}
					auto& l = _anyTemplateSettings[pair].mapExClickList[index];
					for (const auto& str : splited)
					{
						l.checkList.append(str.toInt(&bOk));
					}
				}
				else if (key.contains(rxClick))
				{
					if (splited.size() != 4)
					{
						// error
						continue;
					}
					int index = 0;
					QRegExp rx("(\\d+)");
					if (rx.indexIn(key) != -1)
					{
						index = rx.cap(1).toInt() - 1;
					}
					auto& l = _anyTemplateSettings[pair].mapExClickList[index];
					for (const auto& str : splited)
					{
						l.clickPoint.append(str.toInt(&bOk));
					}
				}
				else if (!key.compare("LD1", Qt::CaseInsensitive))
				{
					if (splited.size() != 2)
					{
						// error
						continue;
					}
					_anyTemplateSettings[pair].ld1ClickX = splited[0].toInt(&bOk);
					_anyTemplateSettings[pair].ld1ClickY = splited[1].toInt(&bOk);
				}
				else if (!key.compare("LD2", Qt::CaseInsensitive))
				{
					if (splited.size() != 2)
					{
						// error
						continue;
					}
					_anyTemplateSettings[pair].ld2ClickX = splited[0].toInt(&bOk);
					_anyTemplateSettings[pair].ld2ClickY = splited[1].toInt(&bOk);
				}
				else if (!key.compare("LD3", Qt::CaseInsensitive))
				{
					if (splited.size() != 2)
					{
						// error
						continue;
					}
					_anyTemplateSettings[pair].ld3ClickX = splited[0].toInt(&bOk);
					_anyTemplateSettings[pair].ld3ClickY = splited[1].toInt(&bOk);
				}
				else if (!key.compare("TillDrop", Qt::CaseInsensitive))
				{
					_anyTemplateSettings[pair].tillDropShip = splited[0].toInt(&bOk);
				}
				else if (!key.compare("Stop", Qt::CaseInsensitive))
				{
					_anyTemplateSettings[pair].shouldStopAfterCharge = splited[0].toInt(&bOk);
				}
				else if (!key.compare("FullCond", Qt::CaseInsensitive))
				{
					_anyTemplateSettings[pair].waitForFullCond = splited[0].toInt(&bOk);
				}
				else if (!key.compare("FullRepair", Qt::CaseInsensitive))
				{
					_anyTemplateSettings[pair].waitForFullRepair = splited[0].toInt(&bOk);
				}
				continue;
			}

			QString settingStr = setting->value(key).toString();
			if (settingStr.isEmpty())
			{
				continue;
			}

			// click at selectable position
			// usage 1CS;XY_100_100
			if (settingStr.contains("XY_"))
			{
				int xyIndex = settingStr.indexOf("XY_");
				QString xyPosStr = settingStr.right(settingStr.length() - (3 + xyIndex));
				auto stringList = xyPosStr.split('_');
				if (stringList.size() < 2)
				{
					continue;
				}
				int x = stringList[0].toInt(&bOk);
				if (!bOk)
				{
					continue;
				}
				int y = stringList[1].toInt(&bOk);
				if (!bOk)
				{
					continue;
				}

				_anyTemplateSettings[pair].cells[cell].clickX = x;
				_anyTemplateSettings[pair].cells[cell].clickY = y;

				if (xyIndex == 0)
				{
					continue;
				}
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

bool ControlManager::BuildNext_RestoreHensei()
{
	_target = ActionTarget::RestoreHensei;

	bool anyJobOn = false;
	// load ini to restoreShips
	if (!_restoreHenseiSetting.restoreSettingText.isEmpty())
	{
		QString filename = MainWindow::getAbsoluteResourcePath();
		filename += "/action/restorehenseisettings.ini";
		QSettings* setting = new QSettings(filename, QSettings::IniFormat);
		setting->setIniCodec("UTF-8");

		if (setting->childGroups().contains(_restoreHenseiSetting.restoreSettingText))
		{
			setting->beginGroup(_restoreHenseiSetting.restoreSettingText);

			KanSaveData* pksd = &KanSaveData::getInstance();

			for (int i = 0; i < pksd->portdata.api_deck_port.count(); i++)
			{
				QList<int> shipList;
				for (int j = 0; j < 6; j++)
				{
					QString valueName = QString::number(i + 1) + "_" + QString::number(j + 1) + "_Ship";

					int shipId = setting->value(valueName, "0").toInt();
					if (shipId > 0)
					{
						shipList.append(shipId);
					}
				}
				if (shipList.count() > 0)
				{
					ChangeHenseiAction * action = new ChangeHenseiAction();
					action->setShips(shipList);
					action->setTeam(i);
					_actionList.append(action);
					anyJobOn = true;
				}
			}
			setting->endGroup();
		}
	}

	if (!anyJobOn)
	{
		setToTerminate("Terminate:NoRestoreSetting", true, RemoteNotifyHandler::Level::Low);
		return false;
	}

	_actionList.append(new RepeatAction());

	setState(State::Ready, "Ready");
	return true;
}

bool ControlManager::BuildNext_Any(bool advanceOnly, bool stopAfterCharge)
{
	_target = ActionTarget::Any;

	if (!advanceOnly)
	{
		pushPreSupplyCheck();

		QList<int> willBeInDockList;
		QList<int> fastRepairShips;
		if (_anySetting.autoFastRepair)
		{
			fastRepairShips = pushPreRepairCheck(willBeInDockList, true, true, false, false, _anySetting.allowMiddleDamageSortie, 3, _anySetting.waitForFullRepair || _anySetting.waitForFullCond);
		}
		else if (_anySetting.onlySSTeamSize > 0)
		{
			pushPreRepairCheck(willBeInDockList, false, false, true, true, _anySetting.allowMiddleDamageSortie, 3, _anySetting.waitForFullCond || _anySetting.waitForFullRepair);
		}

		KanSaveData* pksd = &KanSaveData::getInstance();
		KanDataConnector* pkdc = &KanDataConnector::getInstance();

		pushPreShipFullCheck();

		if (_anySetting.count >= 0 && _anySetting.count <= pksd->totalAnyCount)
		{
			if (isMorningMode())
			{
				_morningSetting.lastBuiltState = MorningStage::AssumeJobDone;
				_target = ActionTarget::None;
				return false;
			}
			setToTerminate("Terminated:CountReached", false, RemoteNotifyHandler::Level::Low);
			return false;
		}

		if (_anySetting.tillDropShip > 0 && pksd->battleresultdata.api_get_ship.api_ship_id == _anySetting.tillDropShip)
		{
			setToTerminate("Terminated:Dropped", false, RemoteNotifyHandler::Level::High);
			return false;
		}

		QList<int> ships;
		QList<int> shipsAllTeam;
		int minCond = std::numeric_limits<int>::max();
		int minCondShipId = -1;
		int shipCount = 0;

		if (_anySetting.onlySSTeamSize > 0)
		{
			QList<int> sortInTeamShips;
			QString errorMessage;
			if (!chooseSSShipList(_anySetting.onlySSTeamSize, ships, sortInTeamShips, willBeInDockList, errorMessage))
			{
				setToTerminate(errorMessage.toLocal8Bit(), false, RemoteNotifyHandler::Level::Low);
				return false;
			}

			if (_anySetting.additionalToSSTeam.count() > 0)
			{
				sortInTeamShips.append(_anySetting.additionalToSSTeam);
				ships.append(_anySetting.additionalToSSTeam);
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
				Q_FOREACH(const auto& deck, pksd->portdata.api_deck_port)
				{
					if (deck.api_id == _anySetting.team || pksd->portdata.api_combined_Flag > 0 && deck.api_id <= 2)
					{
						for (auto id : deck.api_ship)
						{
							if (id >= 0)
							{
								ships.append(id);
							}
						}
					}
				}
			}
		}

		if (ships.count() == 0)
		{
			setToTerminate("Terminated:NoShip", false, RemoteNotifyHandler::Level::High);
			return false;
		}

		for (auto id : ships)
		{
			const kcsapi_ship2* pship = pkdc->findShipFromShipno(id);
			if (pship)
			{
				shipCount++;

				if (pship->api_cond < minCond)
				{
					if (!fastRepairShips.contains(pship->api_id))
					{
						minCond = pship->api_cond;
						minCondShipId = id;
					}
				}

				if (!fastRepairShips.contains(pship->api_id))
				{
					WoundState ws = KanDataCalc::GetWoundState(pship->api_nowhp, pship->api_maxhp);

					if (ws >= WoundState::Middle && !_anySetting.allowMiddleDamageSortie || ws >= WoundState::Big)
					{
						setToTerminate("Terminated:Damage", false, RemoteNotifyHandler::Level::Low);
						return false;
					}
				}
			}

		}

		if (_anySetting.swapLowCond && minCondShipId > 0 && ships[0] != minCondShipId)
		{
			int index = ships.indexOf(minCondShipId);
			int flagship = ships[0];
			ships[0] = minCondShipId;
			ships[index] = flagship;
			auto chAction = new ChangeHenseiAction();
			chAction->setShips(ships);
			_actionList.append(chAction);
		}
		if (!stopAfterCharge)
		{
			if (_anySetting.checkCond)
			{
				if (_anySetting.waitForFullCond && minCond < _sortieFullCond)
				{
					// wait
					qint64 waitMS = qCeil((_sortieFullCond - minCond) / 3.0) * 3 * 60 * 1000;
					auto waitAction = new WaitCondAction();
					waitAction->setWaitMS(waitMS, false);
					_actionList.append(waitAction);
				}
				else if (minCond <= _sortieMinCond)
				{
					// wait
					qint64 waitMS = qCeil((_sortieWaitCond - minCond) / 3.0) * 3 * 60 * 1000;
					auto waitAction = new WaitCondAction();
					waitAction->setWaitMS(waitMS, false);
					_actionList.append(waitAction);
				}
			}


			SortieAction* sortieAction = new SortieAction();
			sortieAction->setAreaAndMap(_anySetting.area, _anySetting.map
				, _anySetting.areaCheckList, _anySetting.mapClickPoint
				, _anySetting.mapExClickList);
			sortieAction->setTeam(_anySetting.team);
			sortieAction->setShouldPauseNext(_anySetting.pauseAtStartMap);
			_actionList.append(sortieAction);
		}
	}

	if (!stopAfterCharge)
	{
		_actionList.append(new SortieCommonAdvanceAction());
		_actionList.append(new ChargeAction());
		if (!advanceOnly)
		{
			_actionList.append(new RepeatAction());
		}
	}
	_morningSetting.lastBuiltState = MorningStage::AssumeJobDone;
	setState(State::Ready, "Ready");
	return !_actionList.empty();
}


bool ControlManager::BuildNext_Expedition()
{
	// no check on pushPreSupplyCheck();

	_target = ActionTarget::Expedition;
	int team = -1;
	auto timerWindow = MainWindow::mainWindow()->timerWindow();

	ExpeditionManager::getInstance().ParsePresetBySettingFileAndRebuild();

	QList<int> excludeTeams;
	SingleExpedition* pExp;
	qint64 ctUtc = TimerMainWindow::currentMSUtc();
	qint64 waitMS = 0;
	for (int i = 0; i < 3; i++)
	{
		waitMS = timerWindow->getMinExpeditionMS(team, excludeTeams);
		pExp = ExpeditionManager::getInstance().getShouldNextSchedule(team, ctUtc, ctUtc + waitMS);
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
		setToTerminate("Termination:FatalOrDateChange", false, RemoteNotifyHandler::Level::Low);
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
	if (pksd->portdata.api_deck_port.size() > team)
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
	QList<int> todoships = LoadRawKiraListForExpedition();
	srand(ctUtc);
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
		waitAction->setWaitMS(waitMS, true);
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

	_handlingExpeditionTeam = team;

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
		setToTerminate("Termination:NoShipToDestroy", true, RemoteNotifyHandler::Level::Info);
		return false;
	}
	DestroyShipAction* action = new DestroyShipAction();
	action->setShips(toDestroyList);
	_actionList.append(action);
	_actionList.append(new RepeatAction());	//should be nothing
	setState(State::Ready, "Ready");
	return true;
}

bool ControlManager::BuildNext_Repair()
{
	_target = ActionTarget::Repair;

	QList<int> willBeInDockList;
	pushPreRepairCheck(willBeInDockList, false, false, false, false, false);
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
		setToTerminate("Termination:DevelopDone", true, RemoteNotifyHandler::Level::Info);
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
	_todoKiraShipids.clear();
	QFile * file = NULL;

	if (QFile::exists(MainWindow::getAbsoluteResourcePath() + "/action/" + "import_kira.table"))
	{
		file = new QFile(MainWindow::getAbsoluteResourcePath() + "/action/" + "import_kira.table");
	}
	else
	{
		file = new QFile(MainWindow::getAbsoluteResourcePath() + "/action/" + "import.table");
	}

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
						_todoKiraShipids.append(line.toInt());
					}
				}
			}
			file->close();
		}
		delete file;
	}
	return true;
}


QList<int> ControlManager::LoadRawKiraListForExpedition()
{
	QList<int> kiraList;
	QFile * file = new QFile(MainWindow::getAbsoluteResourcePath() + "/action/" + "import.table");
	if (file)
	{
		if (file->open(QIODevice::ReadOnly | QIODevice::Text))
		{
			QTextStream instream(file);
			bool bFirstLine = true;
			while (!instream.atEnd())
			{
				QString line = instream.readLine();
				if (bFirstLine)
				{
					bFirstLine = false;
					continue;
				}
				int index = line.indexOf("\t");
				if (index > 0)
				{
					line = line.left(index);
				}
				if (!line.isEmpty())
				{
					kiraList.append(line.toInt());
				}
			}
			file->close();
		}
		delete file;
	}
	return kiraList;
}


bool ControlManager::LoadDestroyableList()
{
	_destroyableMstIds.clear();
	QFile * file = new QFile(MainWindow::getAbsoluteResourcePath() + "/action/" + "destroyable.table");
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
	std::sort(toTempDestroyList.begin(), toTempDestroyList.end(), [](const kcsapi_ship2& left, const kcsapi_ship2& right) {
		return left.api_id > right.api_id;
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


QList<int> ControlManager::pushPreRepairCheck(QList<int>& willBeInDockList,
	bool bCanUseFastRepair,
	bool onlyInTeam,
	bool onlyShortSevere,
	bool onlySmallShip,
	bool onlySevere,
	int useUpToNDockSize/*=3*/,
	bool fastRepairSmallDamage/*=false*/,
	bool includeLowLevel/*=false*/)
{
	if (!_shouldAutoPushRepair)
	{
		return QList<int>();
	}

	KanSaveData* pksd = &KanSaveData::getInstance();

	const int repairLessThanTime = 2 * 60 * 60 * 1000;
	const int tinyRepairTime = 12 * 60 * 1000;

	QList<int> takenNDock;
	for (auto& info : pksd->portdata.api_ndock)
	{
		if (info.api_ship_id > 0)
		{
			takenNDock.append(info.api_id - 1);
		}
	}

	if (takenNDock.size() >= useUpToNDockSize)
	{
		if (!bCanUseFastRepair || takenNDock.size() == 4)
		{
			return QList<int>();
		}
	}

	QList<int> availableNormalSlots;
	int fastSlot = -1;

	int skippedCount = 0;
	const int dockSize = 4;
	for (int i = 0; i < dockSize; i++)
	{
		if (!takenNDock.contains(i))
		{
			if (skippedCount < dockSize - useUpToNDockSize)
			{
				skippedCount++;
				if (fastSlot < 0)
				{
					fastSlot = i;
				}
				continue;
			}
			availableNormalSlots.append(i);
		}
	}

	QList<kcsapi_ship2> toRepairShipList;
	QList<kcsapi_ship2> toFastRepairShipList;

	auto ctUtc = TimerMainWindow::currentMSUtc();
	QDateTime blockShiftedRepairTime = ExpeditionManager::getInstance().currentShiftedDateTime();//QDateTime::currentDateTime();
	ExpeditionManager::getInstance().setNormalizedTime(blockShiftedRepairTime, 22, 30);
	//blockRepairTime.setTime(QTime(22, 30));

	for (auto& ship : pksd->portdata.api_ship)
	{
		if (ship.api_ndock_time > 0
			&& (ship.api_lv > 3 || includeLowLevel)
			&& ship.api_locked
			&& !isShipInDock(ship.api_id))
		{
			bool isInAnyTeam = isShipInOtherTeam(ship.api_id, -1);
			WoundState ws = KanDataCalc::GetWoundState(ship.api_nowhp, ship.api_maxhp);

			bool isSenSui = isShipType(ship.api_id, ShipType::SenBou) || isShipType(ship.api_id, ShipType::SenSui);
			bool isSmallShip = isSenSui
				|| isShipType(ship.api_id, ShipType::KuChiKu)
				|| isShipType(ship.api_id, ShipType::KeiJun)
				|| isShipType(ship.api_id, ShipType::SuiBou)
				|| isShipType(ship.api_id, ShipType::KaiBou)
				|| isShipType(ship.api_id, ShipType::YouRiKu);
			bool isNormalKubo = isShipType(ship.api_id, ShipType::KuBou)
				|| isShipType(ship.api_id, ShipType::KeiKuBou);

			bool doNotOvernight = ship.api_ndock_time + ctUtc < blockShiftedRepairTime.toMSecsSinceEpoch();
			bool canBeRepairedSoon = (ship.api_ndock_time < repairLessThanTime || isSenSui)
				&& doNotOvernight;
			bool isFlagShip = isShipAnyFlagShip(ship.api_id);

			if (canBeRepairedSoon && (!isInAnyTeam || !bCanUseFastRepair))
			{
				bool shouldAdd = true;

				if (onlySmallShip && !isSmallShip)
				{
					shouldAdd = false;
				}

				if (shouldAdd && onlyShortSevere)
				{
					if (ws < WoundState::Middle && ship.api_ndock_time > tinyRepairTime)
					{
						shouldAdd = false;
					}
					else if (!isSmallShip)
					{
						shouldAdd = false;
					}
				}

				if (shouldAdd && onlySevere)
				{
					if (ws <= WoundState::Middle && !isFlagShip && !isNormalKubo)
					{
						shouldAdd = false;
					}
				}

				if (onlyInTeam && isInAnyTeam && !bCanUseFastRepair || !isInAnyTeam)
				{
					if (shouldAdd)
					{
						toRepairShipList.append(ship);
					}
				}
			}
			else if (bCanUseFastRepair)
			{
				if (ws >= WoundState::Middle || ws == WoundState::Small && fastRepairSmallDamage)
				{
					if (ws > WoundState::Middle || !onlySevere || isFlagShip || fastRepairSmallDamage || isNormalKubo)
					{
						if (onlyInTeam && isInAnyTeam)
						{
							toFastRepairShipList.append(ship);
						}
						else if (doNotOvernight)
						{
							toRepairShipList.append(ship);
						}
					}
				}
			}
		}
	}

	std::sort(toRepairShipList.begin(), toRepairShipList.end(), [](const kcsapi_ship2& left, const kcsapi_ship2& right) {
		return left.api_ndock_time < right.api_ndock_time;
	});

	QList<int> fastships;
	if (toFastRepairShipList.size() > 0 && fastSlot >= 0)
	{
		QList<int> usingSlots;

		for (int i = 0; i < toFastRepairShipList.size(); i++)
		{
			fastships.append(toFastRepairShipList[i].api_id);
		}

		for (int i = 0; i < toFastRepairShipList.size(); i++)
		{
			usingSlots.append(fastSlot);
		}

		if (fastships.size() > 0)
		{
			RepairShipAction* fastAction = new RepairShipAction();
			fastAction->setShips(fastships, usingSlots, true);
			_actionList.append(fastAction);
		}
	}

	if (toRepairShipList.size() > 0)
	{
		QList<int> ships;
		QList<int> usingSlots;

		int minSize = availableNormalSlots.size();
		if (minSize > toRepairShipList.size())
		{
			minSize = toRepairShipList.size();
		}

		for (int i = 0; i < minSize; i++)
		{
			usingSlots.append(availableNormalSlots[i]);
		}

		for (int i = 0; i < minSize; i++)
		{
			ships.append(toRepairShipList[i].api_id);
		}

		if (ships.size() > 0)
		{
			RepairShipAction* action = new RepairShipAction();
			action->setShips(ships, usingSlots, false);
			_actionList.append(action);
		}
		willBeInDockList = ships;
	}

	return fastships;
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
		setToTerminate("Terminated:NoAction", false);
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

	RemoteNotifyHandler::getInstance().Notify("Terminated", RemoteNotifyHandler::Level::Info);

	qDeleteAll(_actionList);
	_actionList.clear();
	setPauseNextVal(false);
	setInactiveWaiting(false);
	_target = ActionTarget::None;
	_parentTarget = ActionTarget::None;
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
	qint64 ctUtc = TimerMainWindow::currentMSUtc();
	qint64 waitMS = std::numeric_limits<int>::max();
	for (int i = 0; i < 3; i++)
	{
		waitMS = MainWindow::mainWindow()->timerWindow()->getMinExpeditionMS(team, excludeTeams);
		pExp = ExpeditionManager::getInstance().getShouldNextSchedule(team, ctUtc, ctUtc + waitMS);
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
		else
		{
			int team = -1;
			auto timerWindow = MainWindow::mainWindow()->timerWindow();

			ExpeditionManager::getInstance().ParsePresetBySettingFileAndRebuild();

			QList<int> excludeTeams;
			SingleExpedition* pExp;
			qint64 ctUtc = TimerMainWindow::currentMSUtc();
			qint64 waitMS = 0;
			for (int i = 0; i < 3; i++)
			{
				waitMS = timerWindow->getMinExpeditionMS(team, excludeTeams);
				pExp = ExpeditionManager::getInstance().getShouldNextSchedule(team, ctUtc, ctUtc + waitMS);
				if (!pExp)
				{
					excludeTeams.append(team);
				}
				else
				{
					break;
				}
			}

			if (pExp && team >= 0)
			{
				if (_handlingExpeditionTeam != team)
				{
					// keep lastTarget
					Terminate(true);
					_autoExpeditioningFlag = true;
					if (BuildNext_Expedition())
					{
						StartJob();
					}
				}
			}
		}
	}
}

void ControlManager::setToTerminate(const char* title, bool forceSound, RemoteNotifyHandler::Level overrideLevel)
{
	RemoteNotifyHandler::Level level = overrideLevel;
	if (overrideLevel == RemoteNotifyHandler::Level::Invalid)
	{
		if (forceSound)
		{
			level = RemoteNotifyHandler::Level::High;
		}
		else
		{
			level = RemoteNotifyHandler::Level::Info;
		}
	}
	RemoteNotifyHandler::getInstance().Notify(title, level);
	setState(State::ToTerminate, title, false, forceSound);
}

void ControlManager::createSSShipList(bool noMinLevelHPCheck)
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
			if (noMinLevelHPCheck || (ship.api_lv >= 50 && ship.api_maxhp >= 10))
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
	if (kancount + keep > kanmaxcount)
	{
		return true;
	}
	if (checkSlotitem)
	{
		// check slotitem too
		if (isSlotItemFull(keep * 3 - 3))
		{
			return true;
		}
	}
	return false;
}

bool ControlManager::isSlotItemFull(int keep /*= 6*/)
{
	KanSaveData* pksd = &KanSaveData::getInstance();
	int slotitemcount = pksd->slotitemdata.count() + pksd->slotitemcountoffset;
	int slotitemmaxcount = pksd->portdata.api_basic.api_max_slotitem;
	if (slotitemcount + keep > slotitemmaxcount)
	{
		return true;
	}
	return false;

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
	std::sort(ships.begin(), ships.end(), [](const  kcsapi_ship2& left, const kcsapi_ship2& right)
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

	if (pksd->portdata.api_combined_Flag > 0)
	{
		onBoarding[0] = false;
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


bool ControlManager::isShipInTeam(int shipno, int team)
{
	KanSaveData* pksd = &KanSaveData::getInstance();

	for (auto& deck : pksd->portdata.api_deck_port)
	{
		if (deck.api_id != team + 1)
		{
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

bool ControlManager::isShipAnyFlagShip(int shipno)
{
	KanSaveData* pksd = &KanSaveData::getInstance();

	for (auto& deck : pksd->portdata.api_deck_port)
	{
		if (shipno == deck.api_ship.first())
		{
			return true;
		}
	}
	return false;

}

bool ControlManager::isShipInDock(int shipno, qint64* completeTime/* = NULL*/)
{
	KanSaveData* pksd = &KanSaveData::getInstance();
	if (completeTime != NULL)
	{
		*completeTime = 0;
	}
	for (auto& ndock : pksd->portdata.api_ndock)
	{
		if (ndock.api_ship_id == shipno)
		{
			if (completeTime != NULL)
			{
				*completeTime = ndock.api_complete_time;
			}
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

int ControlManager::getNeedSupplyLDTeam()
{
	KanSaveData* pksd = &KanSaveData::getInstance();
	if (pksd->airBaseNeedSupplyList.isEmpty())
	{
		return -1;
	}
	return pksd->airBaseNeedSupplyList[0] - 1;
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

	if (isShipType(shipno, ShipType::SuiBou))
	{
		return false;
	}

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
	else if (_target == ActionTarget::Fuel)
	{
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
	if (_target == ActionTarget::Bauxite)
	{

		KanSaveData* pksd = &KanSaveData::getInstance();
		if ((pksd->nextdata.api_next == 1 || pksd->nextdata.api_next == 0) && pksd->nextdata.api_maparea_id == 2 && pksd->nextdata.api_mapinfo_no == 2)
		{
			return false;
		}
	}
	return (hugestDamageInTeam(-1) >= WoundState::Big);
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

bool ControlManager::shouldAssignLD()
{
	if (_target != ActionTarget::Any)
	{
		return false;
	}
	if (_anySetting.ld1ClickX > 0 && _anySetting.ld1ClickY > 0
		|| _anySetting.ld2ClickX > 0 && _anySetting.ld2ClickY > 0
		|| _anySetting.ld3ClickX > 0 && _anySetting.ld3ClickY > 0)
	{
		return true;
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

	if (team < 0)
	{
		for (auto& deck : pksd->portdata.api_deck_port)
		{
			auto state = hugestDamageInTeam(deck.api_id - 1);
			if (state > maxState)
			{
				maxState = state;
			}
		}
		return maxState;
	}

	if (pksd->portdata.api_deck_port.size() > team)
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
		auto col = image.pixel(item._pt*_dpiScale);
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

QRgb ControlManager::getColorAtPosition(const QPoint& pt)
{
	auto w = MainWindow::mainWindow()->getBrowserWidget();
	auto image = w->grab().toImage();
	auto col = image.pixel(pt*_dpiScale);
	return col;
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

	if (team < 0)
	{
		for (auto& deck : pksd->portdata.api_deck_port)
		{
			if (needChargeFlagship(deck.api_id - 1))
			{
				return true;
			}
		}
		return false;
	}

	if (pksd->portdata.api_deck_port.size() > team)
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

	if (team < 0)
	{
		for (auto& deck : pksd->portdata.api_deck_port)
		{
			if (needChargeAnyShip(deck.api_id - 1))
			{
				return true;
			}
		}
		return false;
	}


	if (pksd->portdata.api_deck_port.size() > team)
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
			_parentTarget = ActionTarget::None;
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

bool ControlManager::switchBackToLastAction()
{
	_autoExpeditioningFlag = false;
	if (_target != _lastTarget && _lastTarget != ActionTarget::None)
	{
		_target = _lastTarget;
		return true;
	}
	return false;
}

void ControlManager::clearLastTarget()
{
	_lastTarget = ActionTarget::None;
}

void ControlManager::clearParentTarget()
{
	_parentTarget = ActionTarget::None;
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

void ControlManager::moveMouseToAndClick(float x, float y, float offsetX /*= 5*/, float offsetY /*= 3*/, bool moveOnly/* = false*/)
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
		auto sendMouseEvents = [this, ptAdjusted, moveOnly](QObject* w) {
			if (!w)
			{
				return;
			}
#if defined Q_OS_WIN || defined Q_OS_MAC

			if (!moveOnly)
			{
				QMouseEvent e(QEvent::MouseButtonPress, ptAdjusted, Qt::LeftButton, Qt::LeftButton, Qt::NoModifier);
				QApplication::sendEvent(w, &e);

				QMouseEvent e2(QEvent::MouseButtonRelease, ptAdjusted, Qt::LeftButton, Qt::LeftButton, Qt::NoModifier);
				QApplication::sendEvent(w, &e2);
			}
			else
			{
				QMouseEvent e0(QEvent::MouseMove, ptAdjusted, Qt::LeftButton, Qt::LeftButton, Qt::NoModifier);
				QApplication::sendEvent(w, &e0);
			}
#else
			if (!moveOnly)
			{
				QPoint ptG = w->mapToGlobal(QPoint(ptAdjusted.x(), ptAdjusted.y()));
				MainWindow::mainWindow()->clickAtGlobalPos(ptG);
			}

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
					QWidget* w = qobject_cast<QWidget*>(obj);
					if (w != NULL)
					{
						sendMouseEvents(w);
						break;
					}
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

void ControlManager::moveMouseTo(float x, float y, float offsetX /*= 5*/, float offsetY /*= 3*/)
{
	moveMouseToAndClick(x, y, offsetX, offsetY, true);
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

void ControlManager::setPortDataDirty()
{
	_isPortDataDirty = true;
	KanDataConnector::getInstance().callUpdateOverviewTable();
}

void ControlManager::clearPortDataDirtyFlag()
{
	_isPortDataDirty = false;
	KanDataConnector::getInstance().callUpdateOverviewTable();
}

