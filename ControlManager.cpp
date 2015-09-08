#include "ControlManager.h"
#include <QTestEventList>
#include "mainwindow.h"
#include <QMouseEvent>
#include <QGraphicsSceneMouseEvent>
#include "kandataconnector.h"
#include "kansavedata.h"
#include "kandatacalc.h"

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
	if (_todoShipids.empty())
	{
		setState(State::Terminated, "Terminated:NoShip");
		return false;
	}
	if (flagshipSevereDamaged())
	{
		setState(State::Terminated, "Terminated:Flagship");
		return false;
	}

	_target = SortieTarget::Kira;
	if (isShipFull())
	{
		setState(State::Terminated, "Terminated:ShipFull");
		return false;
//		_actionList.append(new DestroyShipAction());
	}

	int togoShipId = _todoShipids.at(0);
	while (isShipKiraDone(togoShipId) 
		|| isShipInOtherTeam(togoShipId) 
		|| isShipInDock(togoShipId)
		|| hasSlotitem(togoShipId, SLOTITEMTYPE_JOURIKUTEI)
		|| hasSlotitem(togoShipId, SLOTITEMTYPE_SONAR)
		|| hasSlotitem(togoShipId, SLOTITEMTYPE_BAKURAI)
		|| hasSlotitem(togoShipId, SLOTITEMTYPE_SONAR_L)
		|| noSlotitem(togoShipId))
	{
		_todoShipids.removeAt(0);
		if (_todoShipids.empty())
		{
			setState(State::Terminated, "Terminated:NoShip");
			return false;
		}
		togoShipId = _todoShipids.at(0);
	}

	int flagshipid = getCurrentFlagshipId();
	int flagshipIndex = _todoShipids.indexOf(flagshipid);
	if (flagshipIndex > 0)
	{
		_todoShipids.swap(flagshipIndex, 0);
	}

	togoShipId = _todoShipids.at(0);
	auto chAction = new ChangeHenseiAction();
	chAction->setShips(togoShipId, shouldChangeSecondShip()?getOneWasteShipId():getCurrentSecondshipId());
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

	if (flagshipSevereDamaged())
	{
		setState(State::Terminated, "Terminated:Flagship");
		return false;
	}

	if (!isFlagshipOnly())
	{
		setState(State::Terminated, "Terminated:More than one ship");
		return false;
	}

	int flagshipid = getCurrentFlagshipId();
	if (!isShipType(flagshipid, SHIPTYPE_SENSUI))
	{
		setState(State::Terminated, "Terminated:Ship type");
		return false;
	}

	_target = SortieTarget::Fuel;
	if (isShipFull())
	{
		setState(State::Terminated, "Terminated:ShipFull");
		return false;
//		_actionList.append(new DestroyShipAction());
	}
	_actionList.append(new SortieAction());
	_actionList.append(new SortieAdvanceAction());
	_actionList.append(new ChargeAction());
	_actionList.append(new RepeatAction());

	setState(State::Ready, "Ready");
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
		setState(State::Terminated, "Terminated:NoAction");
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
}

void ControlManager::Resume()
{
	setState(State::Started, "Started:Resume");
}

void ControlManager::Terminate()
{
	qDeleteAll(_actionList);
	_actionList.clear();
	setPauseNextVal(_pauseNext);
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

void ControlManager::setToTerminate()
{
	if (_state == State::Started)
	{
		setState(State::ToTerminate, "ToTerminate");
	}
}

int ControlManager::getOneWasteShipId()
{
	KanSaveData* pksd = &KanSaveData::getInstance();
	KanDataConnector* pkdc = &KanDataConnector::getInstance();
	for (auto ship: pksd->portdata.api_ship)
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
			if (pmstship->api_stype == SHIPTYPE_KUCHIKU)
			{
				if (ship.api_fuel == pmstship->api_fuel_max
					&& ship.api_bull == pmstship->api_bull_max)
				{
					return ship.api_id;
				}
			}
		}
	}
	return -1;
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
	QList<kcsapi_ship2> ships = pksd->portdata.api_ship;
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
				if (pmstship->api_stype == SHIPTYPE_SUIBO
					|| pmstship->api_stype == SHIPTYPE_YOURIKU)
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
	for (auto deck : pksd->portdata.api_deck_port)
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
	for (auto ndock : pksd->portdata.api_ndock)
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
		auto firstFleet = pksd->portdata.api_deck_port.first();
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
	KanSaveData* pksd = &KanSaveData::getInstance();

	if (pksd->portdata.api_deck_port.size())
	{
		auto firstFleet = pksd->portdata.api_deck_port.first();
		QList<int> nonEmptyShipList;
		for (auto id : firstFleet.api_ship)
		{
			if (id >= 0)
			{
				nonEmptyShipList.append(id);
			}
		}
		if (nonEmptyShipList.size() == 1)
		{
			return true;
		}
	}
	return false;
}

bool ControlManager::isShipType(int shipno, int stype)
{
	KanDataConnector* pkdc = &KanDataConnector::getInstance();

	auto pship = pkdc->findShipFromShipno(shipno);
	if (pship)
	{
		auto pmstship = pkdc->findMstShipFromShipid(pship->api_ship_id);
		if (pmstship)
		{
			if (pmstship->api_stype == stype)
			{
				return true;
			}
		}
	}

	return false;
}

bool ControlManager::hasSlotitem(int shipno, int sitype)
{
	KanDataConnector* pkdc = &KanDataConnector::getInstance();

	auto pship = pkdc->findShipFromShipno(shipno);
	if (pship)
	{
		for (auto slotitemid : pship->api_slot)
		{
			if (slotitemid >= 0)
			{
				auto pslotitem = pkdc->findSlotitemFromId(slotitemid);
				if (pslotitem)
				{
					auto pmstslotitem = pkdc->findMstSlotItemFromSlotitemid(pslotitem->api_slotitem_id);
					if (pmstslotitem)
					{
						if (pmstslotitem->api_type.count() > 2)
						{
							int type = pmstslotitem->api_type[2];
							if (sitype == type)
							{
								return true;
							}
						}
					}
				}
			}
		}
	}
	return false;
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
		auto firstFleet = pksd->portdata.api_deck_port.first();
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
			if (KanDataCalc::GetWoundState(pship->api_nowhp, pship->api_maxhp) < WOUNDSTATE_BIG)
			{
				return false;
			}
		}
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
	for (auto item:checklist)
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
		auto firstFleet = pksd->portdata.api_deck_port.first();

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
		auto firstFleet = pksd->portdata.api_deck_port.first();
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

int ControlManager::getCurrentFlagshipId()
{
	KanSaveData* pksd = &KanSaveData::getInstance();

	if (!pksd->portdata.api_ship.size())
	{
		return false;
	}

	if (pksd->portdata.api_deck_port.size())
	{
		auto firstFleet = pksd->portdata.api_deck_port.first();
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
		auto firstFleet = pksd->portdata.api_deck_port.first();
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
		if (_state == State::Terminated)
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

