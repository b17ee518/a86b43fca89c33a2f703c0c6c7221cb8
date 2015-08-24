#include "ControlManager.h"
#include <QTestEventList>
#include "mainwindow.h"
#include <QMouseEvent>
#include <QGraphicsSceneMouseEvent>
#include "kandataconnector.h"
#include "kansavedata.h"
#include "kandatacalc.h"

#define COL_ALLOWRANCE 3

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
		return false;
	}
	// for repeat action
	/*
	if (!_actionList.empty())
	{
		return;
	}
	*/
	if (flagshipSevereDamaged())
	{
		_state = State::Terminated;
		return false;
	}

	_target = SortieTarget::Kira;
	if (isShipFull())
	{
		_state = State::Terminated;
		return false;
//		_actionList.append(new DestroyShipAction());
	}
	_actionList.append(new ChangeHenseiAction(_todoShipids.at(0), getOneWasteShipId()));
	_actionList.append(new SortieAction());
	_actionList.append(new SortieAdvanceAction());
	_actionList.append(new ChargeAction());
	_actionList.append(new RepeatAction());

	_state = State::Ready;
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
		_state = State::Terminated;
		return false;
	}

	_target = SortieTarget::Fuel;
	if (isShipFull())
	{
		_state = State::Terminated;
		return false;
//		_actionList.append(new DestroyShipAction());
	}
	_actionList.append(new SortieAction());
	_actionList.append(new SortieAdvanceAction());
	_actionList.append(new ChargeAction());
	_actionList.append(new RepeatAction());

	_state = State::Ready;
	return true;
}

void ControlManager::StartJob()
{
	if (_state == State::Ready)
	{
		_state = State::Started;
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
	/*
	_todoShipids.append(1); // test
	_todoShipids.append(2); // test
	_todoShipids.append(3); // test
	*/
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
		_state = State::Terminated;
		return;
	}
	if (_actionList[0]->action())
	{
		delete _actionList[0];
		_actionList.removeAt(0);
		if (_actionList.empty())
		{
		}
	}
}

void ControlManager::Pause()
{
	_state = State::Paused;
}

void ControlManager::Resume()
{
	_state = State::Started;
}

void ControlManager::Terminate()
{
	qDeleteAll(_actionList);
	_actionList.clear();
	_state = State::Terminated;
}

void ControlManager::setToTerminate()
{
	if (_state == State::Started)
	{
		_state = State::ToTerminate;
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
					return shipid;
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
		if (firstFleet.api_ship.size() < 2)
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

void ControlManager::moveMouseToAndClick(const QPoint& pt)
{
	QMouseEvent e(QEvent::MouseButtonPress, pt, Qt::LeftButton, Qt::LeftButton, Qt::NoModifier);
	QApplication::sendEvent(MainWindow::mainWindow()->getBrowserWidget(), &e);
	
	QMouseEvent e2(QEvent::MouseButtonRelease, pt, Qt::LeftButton, Qt::LeftButton, Qt::NoModifier);
	QApplication::sendEvent(MainWindow::mainWindow()->getBrowserWidget(), &e2);
}

void ControlManager::moveMouseTo(const QPoint& pt)
{
	QMouseEvent e(QEvent::MouseMove, pt, Qt::LeftButton, Qt::LeftButton, Qt::NoModifier);
	QApplication::sendEvent(MainWindow::mainWindow()->getBrowserWidget(), &e);
}

