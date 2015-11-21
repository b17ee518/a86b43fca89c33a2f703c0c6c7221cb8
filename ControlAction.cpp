#include "ControlAction.h"
#include "ControlManager.h"
#include <QDebug>
#include "kandatacalc.h"

#define DELAY_TIME			(250*_intervalMul)
#define DELAY_TIME_CLICK	(250*_intervalMul)
#define DELAY_TIME_LONG	(500*_intervalMul)
#define DELAY_TIME_PAGE	(300*_intervalMul)
#define DELAY_TIME_SUPERLONG	(1200*_intervalMul)
#define DELAY_TIME_SUPERSUPERLONG	(2000*_intervalMul)
#define DELAY_TIME_SKIP	(7000*_intervalMul)

ControlAction::ControlAction(QObject* parent /*= NULL*/)
:QObject(parent)
{
	_intervalMul = ControlManager::getInstance().getIntervalMul();
}

void ControlAction::setDoneRequest(const QString& api)
{
	if (api == _expectingRequest)
	{
		_expectingRequest = "";
	}
}

void ControlAction::tryRetry()
{
	if (_retryTime < 10)
	{
		_retryTime++;
		_waiting = false;
		QString str = QString::fromLocal8Bit("Retrying %1:%2")
			.arg(_retryTime)
			.arg(ControlManager::getInstance().getStateStr());
		ControlManager::getInstance().setStateStr(str);
		
	}
	else
	{
		ControlManager::getInstance().setToTerminate("Terminated:RetryTimeOver");
		emit sigCheckFail();
	}
}

bool WaitCondAction::action()
{
	auto& cm = ControlManager::getInstance();
	switch (_state)
	{
	case WaitCondAction::State::None:
		setState(State::Waiting, "Wait:Waiting");
		resetRetryAndWainting();
		break;
	case WaitCondAction::State::Waiting:

		if (!_waiting)
		{
			_waiting = true;
			cm.setInactiveWaiting(true);
			QTimer::singleShot(_waitMS, Qt::PreciseTimer, this, [this, &cm]()
			{
				_waiting = false;
				cm.setInactiveWaiting(false);
				setState(State::HomePortChecking, "Wait:HomePortChecking");
			});
		}
		break;
	case WaitCondAction::State::HomePortChecking:
		if (!_waiting)
		{
			_waiting = true;
			QTimer::singleShot(DELAY_TIME, Qt::PreciseTimer, this, [this, &cm]()
			{
				// home port
				if (cm.checkColors(
					213, 247, 251, 155, 32
					, 231, 114, 224, 201, 66))
				{
					_waiting = false;
					setState(State::HomePortDone, "Wait:HomePortDone");
				}
				else
				{
					tryRetry();
				}
			});
		}
		break;
	case WaitCondAction::State::HomePortDone:
		if (!_waiting)
		{
			_waiting = true;
			QTimer::singleShot(DELAY_TIME_CLICK, Qt::PreciseTimer, this, [this, &cm]()
			{
				cm.moveMouseToAndClick(199, 135.5f, 23, 21.5f); // change button
				setState(State::FakeHenseiChecking, "Wait:FakeHenseiChecking");
				resetRetryAndWainting();
			});
		}
		break;
	case WaitCondAction::State::FakeHenseiChecking:
		if (!_waiting)
		{
			_waiting = true;
			QTimer::singleShot(DELAY_TIME, Qt::PreciseTimer, this, [this, &cm]()
			{
				// home port
				if (cm.checkColors(
					34, 144, 238, 213, 54
					, 404, 208, 84, 178, 105))
				{
					_waiting = false;
					setState(State::FakeHenseiDone, "Wait:FakeHenseiDone");
				}
				else
				{
					tryRetry();
				}
			});
		}
		break;
	case WaitCondAction::State::FakeHenseiDone:
		if (!_waiting)
		{
			_waiting = true;
			_expectingRequest = "/kcsapi/api_port/port";
			QTimer::singleShot(DELAY_TIME_CLICK, Qt::PreciseTimer, this, [this, &cm]()
			{
				cm.moveMouseToAndClick(74, 252); // port button
				setState(State::ExpectingPort, "Wait:ExpectingPort");
				resetRetryAndWainting();
			});
		}
		break;
	case WaitCondAction::State::ExpectingPort:
		if (_expectingRequest == "")
		{
			cm.moveMouseTo(400, 200);
			setState(State::Done, "Wait:Done");
		}
		break;
	case WaitCondAction::State::Done:
		return true;
		break;
	default:
		break;
	}
	return false;
}

void WaitCondAction::setWaitMS(qint64 waitms)
{
	_waitMS = waitms;
	if (_waitMS < 0)
	{
		_waitMS = 1;
	}
}

void WaitCondAction::setState(State state, const char* str)
{
	_state = state;
	ControlManager::getInstance().setStateStr(str);
}

bool ChangeHenseiAction::action()
{
	auto& cm = ControlManager::getInstance();
	switch (_state)
	{
	case ChangeHenseiAction::State::None:

		_nowIndex = 0;
		setState(State::HomePortChecking, "Hensei:HomePortChecking");
		while (cm.isHenseiDone(_ships, _team, _nowIndex))
		{
			_nowIndex++;
			if (_nowIndex >= _ships.size())
			{
				setState(State::Done, "Hensei:Done");
				break;
			}
		}

		resetRetryAndWainting();
		break;
	case ChangeHenseiAction::State::HomePortChecking:
		if (!_waiting)
		{
			_waiting = true;
			QTimer::singleShot(DELAY_TIME, Qt::PreciseTimer, this, [this, &cm]()
			{
				// home port
				if (cm.checkColors(
					213, 247, 251, 155, 32
					, 231, 114, 224, 201, 66))
				{
					_waiting = false;
					setState(State::HomePortDone, "Hensei:HomePortDone");
				}
				else
				{
					tryRetry();
				}
			});
		}
		break;
	case ChangeHenseiAction::State::HomePortDone:
		if (!_waiting)
		{
			_waiting = true;
			if (cm.isHenseiDone(_ships, _team))
			{
				setState(State::Done, "Hensei:Done");
				resetRetryAndWainting();
			}
			else
			{
				QTimer::singleShot(DELAY_TIME_CLICK, Qt::PreciseTimer, this, [this, &cm]()
				{
					cm.moveMouseToAndClick(199, 135.5f, 23, 21.5f); // change button
					if (_team == 0)
					{
						setState(State::HenseiChecking, "Hensei:HenseiChecking");
					}
					else
					{
						setState(State::ChangeFleetChecking, "Hensei:ChangeFleetChecking");
					}
					resetRetryAndWainting();
				});
			}
		}
		break;
	case ChangeHenseiAction::State::ChangeFleetChecking:
		if (!_waiting)
		{
			_waiting = true;
			QTimer::singleShot(DELAY_TIME, Qt::PreciseTimer, this, [this, &cm]()
			{
				// home port
				if (cm.checkColors(
					34, 144, 238, 213, 54
					, 404, 208, 84, 178, 105))
				{
					_waiting = false;
					setState(State::ChangeFleetDone, "Hensei:ChangeFleetDone");
				}
				else
				{
					tryRetry();
				}
			});
		}
		break;
	case ChangeHenseiAction::State::ChangeFleetDone:
		if (!_waiting)
		{
			_waiting = true;
			QTimer::singleShot(DELAY_TIME_CLICK, Qt::PreciseTimer, this, [this, &cm]()
			{
				cm.moveMouseToAndClick(135+30*_team, 118); // Fleet

				setState(State::HenseiChecking, "Hensei:HenseiChecking");
				resetRetryAndWainting();
			});
		}
		break;
	case ChangeHenseiAction::State::HenseiChecking:
		if (!_waiting)
		{
			_waiting = true;
			QTimer::singleShot(DELAY_TIME, Qt::PreciseTimer, this, [this, &cm]()
			{
				// home port
				if (cm.checkColors(
					34, 144, 238, 213, 54
					, 404, 208, 84, 178, 105))
				{
					_waiting = false;
					setState(State::HenseiDone, "Hensei:HenseiDone");
				}
				else
				{
					tryRetry();
				}
			});
		}
		break;
	case ChangeHenseiAction::State::HenseiDone:
		if (!_waiting)
		{
			_waiting = true;
			QTimer::singleShot(DELAY_TIME_CLICK, Qt::PreciseTimer, this, [this, &cm]()
			{
				while (cm.isHenseiDone(_ships, _team, _nowIndex))
				{
					_nowIndex++;
					if (_nowIndex >= _ships.size())
					{
						setState(State::ReturnToPortChecking, "Hensei:ReturnToPortChecking");
						resetRetryAndWainting();
						return;
					}
				}

				if (_nowIndex < 6)
				{
					cm.moveMouseToAndClick(410 + (_nowIndex % 2) * 342, 216 + (_nowIndex / 2) * 113); // Fleet member change button
				}

				setState(State::FindShipChecking, "Hensei:FindShipChecking");
				resetRetryAndWainting();
			});			
		}
		break;
	case ChangeHenseiAction::State::FindShipChecking:
		if (!_waiting)
		{
			_waiting = true;
			QTimer::singleShot(DELAY_TIME, Qt::PreciseTimer, this, [this, &cm]()
			{
				if (cm.checkColors(
					34, 144, 238, 213, 54
					, 770, 113, 44, 144, 144))
				{
					_waiting = false;
					setState(State::FindShipDone, "Hensei:FindShipDone");
				}
				else if (cm.checkColors(
					34, 144, 238, 213, 54
					, 709, 449, 196, 188, 105))
				{
					_waiting = false;
					setState(State::FindShipChangeSort, "Hensei:FindShipChangeSort");
				}
				else
				{
					tryRetry();
				}
			});
		}
		break;
	case ChangeHenseiAction::State::FindShipChangeSort:
		if (!_waiting)
		{
			_waiting = true;
			QTimer::singleShot(DELAY_TIME_CLICK, Qt::PreciseTimer, this, [this, &cm]()
			{
				cm.moveMouseToAndClick(775, 112, 15, 5); // sort button
				setState(State::FindShipChecking, "Hensei:FindShipChecking");
				resetRetryAndWainting();
			});
		}
		break;
	case ChangeHenseiAction::State::FindShipDone:
		if (!_waiting)
		{
			_waiting = true;
			QTimer::singleShot(DELAY_TIME_CLICK, Qt::PreciseTimer, this, [this, &cm]()
			{
				cm.moveMouseToAndClick(435, 449); // left most button
				setState(State::FindShipFirstPageChecking, "Hensei:FindShipFirstPageChecking");
				resetRetryAndWainting();
			});
		}
		break;
	case ChangeHenseiAction::State::FindShipFirstPageChecking:
		if (!_waiting)
		{
			_waiting = true;
			QTimer::singleShot(DELAY_TIME, Qt::PreciseTimer, this, [this, &cm]()
			{
				// home port
				if (cm.checkColors(
					34, 144, 238, 213, 54
					, 513, 449, 30, 190, 195))
				{
					_waiting = false;
					setState(State::FindShipFirstPageDone, "Hensei:FindShipFirstPageDone");
				}
				else
				{
					tryRetry();
				}
			});
		}
		break;
	case ChangeHenseiAction::State::FindShipFirstPageDone:
		if (!_waiting)
		{
			_waiting = true;
			if (_pageList[_nowIndex] == 0)
			{
				QTimer::singleShot(DELAY_TIME_CLICK, Qt::PreciseTimer, this, [this, &cm]()
				{
					cm.moveMouseToAndClick(439, 169 + _cellHeight*_posList[_nowIndex]);
					setState(State::FindShipOKChecking, "Hensei:FindShipOKChecking");
					resetRetryAndWainting();
				});
			}
			else
			{
				QTimer::singleShot(DELAY_TIME_CLICK, Qt::PreciseTimer, this, [this, &cm]()
				{
					if (_lastPage == _pageList[_nowIndex])
					{
						cm.moveMouseToAndClick(707, 452); // last page
						_curPage = _lastPage;
					}
					else if (_pageList[_nowIndex] >= 5)
					{
						cm.moveMouseToAndClick(677, 449); // 6th page
						_curPage = 5;
					}
					else
					{
						cm.moveMouseToAndClick(547, 446); // second page
						_curPage = 1;
					}
					setState(State::FindShipNextPageChecking, "Hensei:FindShipNextPageChecking");
					resetRetryAndWainting();
				});
			}
		}
		break;
	case ChangeHenseiAction::State::FindShipNextPageChecking:
		if (!_waiting)
		{
			_waiting = true;
			QTimer::singleShot(DELAY_TIME_PAGE, Qt::PreciseTimer, this, [this, &cm]()
			{
				_waiting = false;
				setState(State::FindShipNextPageDone, "Hensei:FindShipNextPageDone");
				// just assume page flip is ok
			});
		}
		break;
	case ChangeHenseiAction::State::FindShipNextPageDone:
		if (!_waiting)
		{
			_waiting = true;
			if (_pageList[_nowIndex] == _curPage)
			{
				QTimer::singleShot(DELAY_TIME_CLICK, Qt::PreciseTimer, this, [this, &cm]()
				{
					cm.moveMouseToAndClick(439, 169 + _cellHeight*_posList[_nowIndex]);
					setState(State::FindShipOKChecking, "Hensei:FindShipOKChecking");
					resetRetryAndWainting();
				});
			}
			else
			{
				QTimer::singleShot(DELAY_TIME_CLICK, Qt::PreciseTimer, this, [this, &cm]()
				{
					if (_curPage == 1)
					{
						cm.moveMouseToAndClick(579, 447); // third page
						_curPage++;
					}
					else if (_pageList[_nowIndex] >= _curPage + 5)
					{
						cm.moveMouseToAndClick(677, 449); // 6th page
						_curPage += 5;
					}
					else
					{
						cm.moveMouseToAndClick(609, 446); // fourth and further
						_curPage++;
					}
					setState(State::FindShipNextPageChecking, "Hensei:FindShipNextPageChecking");
					resetRetryAndWainting();
				});
			}
		}
		break;
	case ChangeHenseiAction::State::FindShipOKChecking:
		if (!_waiting)
		{
			_waiting = true;
			QTimer::singleShot(DELAY_TIME_SUPERLONG, Qt::PreciseTimer, this, [this, &cm]()
			{
				// home port
				if (cm.checkColors(
					34, 144, 238, 213, 54
					, 682, 432, 37, 162, 162))
				{
					_waiting = false;
					setState(State::FindShipOKDone, "Hensei:FindShipOKDone");
				}
				else
				{
					tryRetry();
				}
			});
		}
		break;
	case ChangeHenseiAction::State::FindShipOKDone:
		if (!_waiting)
		{
			_waiting = true;
			QTimer::singleShot(DELAY_TIME_CLICK, Qt::PreciseTimer, this, [this, &cm]()
			{
				cm.moveMouseToAndClick(691, 443, 40, 12); // change button
				if (_nowIndex == _ships.size()-1)
				{
					setState(State::ReturnToPortChecking, "Hensei:ReturnToPortChecking");
					resetRetryAndWainting();
				}
				else
				{
					_waiting = true;
					QTimer::singleShot(DELAY_TIME_SUPERSUPERLONG, Qt::PreciseTimer, this, [this, &cm]()
					{
						if (cm.isHenseiDone(_ships, _team, _nowIndex))
						{
							_nowIndex++;
							setState(State::HenseiChecking, "Hensei:HenseiChecking");
							resetRetryAndWainting();
						}
						else
						{
							qDebug() << "hensei error";
							emit sigCheckFail();
							cm.setToTerminate("Terminated:HenseiError");
						}
					});
				}
			});
		}
		break;
	case ChangeHenseiAction::State::ReturnToPortChecking:
		if (!_waiting)
		{
			_waiting = true;
			QTimer::singleShot(DELAY_TIME, Qt::PreciseTimer, this, [this, &cm]()
			{
				// home port
				if (cm.checkColors(
					34, 144, 238, 213, 54
					, 404, 208, 84, 178, 105))
				{
					_waiting = false;
					setState(State::ReturnToPortDone, "Hensei:ReturnToPortDone");
				}
				else
				{
					tryRetry();
				}
			});
		}
		break;
	case  ChangeHenseiAction::State::ReturnToPortDone:
		if (!_waiting)
		{
			_waiting = true;
			_expectingRequest = "/kcsapi/api_port/port";
			QTimer::singleShot(DELAY_TIME_CLICK, Qt::PreciseTimer, this, [this, &cm]()
			{
				cm.moveMouseToAndClick(74, 252); // port button
				setState(State::ExpectingPort, "Hensei:ExpectingPort");
				resetRetryAndWainting();
			});
		}
		break;
	case ChangeHenseiAction::State::ExpectingPort:
		if (_expectingRequest == "")
		{
			cm.moveMouseTo(400, 200);
			setState(State::Done, "Hensei:Done");
		}
		break;
	case ChangeHenseiAction::State::Done:
		if (cm.isHenseiDone(_ships, _team))
		{
			return true;
		}
		else
		{
			emit sigCheckFail();
			cm.setToTerminate("Terminated:HenseiError");
		}
		break;
	default:
		break;
	}

	return false;
}

void ChangeHenseiAction::setShips(int ship0, int ship1)
{
	QList<int> ships;
	ships.push_back(ship0);
	if (ship1 >= 0)
	{
		ships.push_back(ship1);
	}
	setShips(ships);
}

void ChangeHenseiAction::setShips(const QList<int>& ships)
{
	for (int shipno: ships)
	{
		if (shipno < 0)
		{
			continue;
		}
		_ships.append(shipno);

		int page;
		int pos;
		if (ControlManager::getInstance().findPagePosByShipId(shipno, page, pos, _lastPage))
		{
			_pageList.append(page);
			_posList.append(pos);
		}
	}
}

void ChangeHenseiAction::setTeam(int team)
{
	if (team >= 0 && team < 4)
	{
		_team = team;
	}
}

void ChangeHenseiAction::setState(State state, const char* str)
{
	_state = state;
	ControlManager::getInstance().setStateStr(str);
}

bool ChargeAction::action()
{
	auto& cm = ControlManager::getInstance();
	switch (_state)
	{
	case ChargeAction::State::None:

		cm.moveMouseTo(400, 240);
		if (_skipExpedition)
		{
			setState(State::Skipping, "Charge:Skipping");
		}
		else if (!cm.needChargeFlagship(_team))
		{
			setState(State::Done, "Charge:Done");
		}
		else
		{
			setState(State::HomePortChecking, "Charge:HomePortChecking");
		}
		resetRetryAndWainting();
		break;
	case ChargeAction::State::Skipping:
		if (!_waiting)
		{
			_waiting = true;
			QTimer::singleShot(DELAY_TIME, Qt::PreciseTimer, this, [this, &cm]()
			{
				// charge panel
				if (cm.checkColors(
					766, 124, 191, 137, 0
					, 771, 164, 114, 91, 70))
				{
					_waiting = false;
					if (!cm.needChargeFlagship(_team))
					{
						setState(State::FinishedChargeDone, "Charge:FinishedChargeDone");
						resetRetryAndWainting();
					}
					else
					{
						setState(State::NeedChargeDone, "Charge:NeedChargeDone");
						resetRetryAndWainting();
					}
				}
				else
				{
					cm.moveMouseToAndClick(101, 217); // charge button
					resetRetryAndWainting();
				}
			});
		}
		break;
	case ChargeAction::State::HomePortChecking:
		if (!_waiting)
		{
			_waiting = true;
			QTimer::singleShot(DELAY_TIME, Qt::PreciseTimer, this, [this, &cm]()
			{
				// home port
				if (cm.checkColors(
					213, 247, 251, 155, 32
					, 231, 114, 224, 201, 66))
				{
					_waiting = false;
					setState(State::HomePortDone, "Charge:HomePortDone");
				}
				else
				{
					tryRetry();
				}
			});
		}
		break;
	case ChargeAction::State::HomePortDone:

		if (!_waiting)
		{
			_waiting = true;
			QTimer::singleShot(DELAY_TIME_CLICK, Qt::PreciseTimer, this, [this, &cm]()
			{
				cm.moveMouseToAndClick(76.5f, 220, 26.5f, 21); // charge button
				setState(State::NeedChargeChecking, "Charge:NeedChargeChecking");
				resetRetryAndWainting();
			});
		}
		break;
	case ChargeAction::State::NeedChargeChecking:
		if (!_waiting)
		{
			_waiting = true;
			QTimer::singleShot(DELAY_TIME, Qt::PreciseTimer, this, [this, &cm]()
			{
				if (cm.checkColors(
					766, 124, 191, 137, 0
					, 771, 164, 114, 91, 70))
				{
					_waiting = false;
					setState(State::NeedChargeDone, "Charge:NeedChargeDone");
				}
				else
				{
					tryRetry();
				}
			});
		}
		break;
	case ChargeAction::State::NeedChargeDone:
		if (!_waiting)
		{
			_waiting = true;
			QTimer::singleShot(DELAY_TIME_CLICK, Qt::PreciseTimer, this, [this, &cm]()
			{
				if (_team > 0 && !_teamChanged)
				{
					cm.moveMouseToAndClick(148+_team*30, 119); // team
					setState(State::NeedChargeChecking, "Charge:NeedChargeChecking");
					_teamChanged = true;
				}
				else
				{
					if (cm.isSouthEastMode() || cm.isFuelMode() || cm.isExpeditionMode() || cm.isLevelMode())
					{
						cm.moveMouseToAndClick(117, 120, 2, 2); // all ships
						setState(State::OKToChargeDone, "Charge:OKToChargeDone");
					}
					else
					{
						cm.moveMouseToAndClick(117, 167, 2, 2); // first ship
						setState(State::OKToChargeChecking, "Charge:OKToChargeChecking");
					}
				}
				resetRetryAndWainting();
			});
		}
		break;
	case ChargeAction::State::OKToChargeChecking:
		if (!_waiting)
		{
			_waiting = true;
			QTimer::singleShot(DELAY_TIME, Qt::PreciseTimer, this, [this, &cm]()
			{
				if (cm.checkColors(
					705, 431, 94, 191, 203
					, 625, 254, 113, 110, 74))
				{
					_waiting = false;
					setState(State::OKToChargeDone, "Charge:OKToChargeDone");
				}
				else
				{
					tryRetry();
				}
			});
		}
		break;
	case ChargeAction::State::OKToChargeDone:
		if (!_waiting)
		{
			_waiting = true;
			QTimer::singleShot(DELAY_TIME_CLICK, Qt::PreciseTimer, this, [this, &cm]()
			{
				cm.moveMouseToAndClick(702, 440.5f, 39, 9.5f); // charge button
				setState(State::FinishedChargeChecking, "Charge:FinishedChargeChecking");
				resetRetryAndWainting();
			});
		}
		break;
	case ChargeAction::State::FinishedChargeChecking:
		if (!_waiting)
		{
			_waiting = true;
			QTimer::singleShot(DELAY_TIME, Qt::PreciseTimer, this, [this, &cm]()
			{
				if (cm.checkColors(
					516, 164, 54, 255, 0
					, 625, 254, 113, 110, 74))
				{
					_waiting = false;
					setState(State::FinishedChargeDone, "Charge:FinishedChargeDone");
				}
				else
				{
					tryRetry();
				}
			});
		}
		break;
	case ChargeAction::State::FinishedChargeDone:
		if (!_waiting)
		{
			_waiting = true;
			_expectingRequest = "/kcsapi/api_port/port";
			QTimer::singleShot(DELAY_TIME_CLICK, Qt::PreciseTimer, this, [this, &cm]()
			{
				cm.moveMouseToAndClick(75, 261); // home button
				setState(State::ExpectingPort, "Charge:ExpectingPort");
				resetRetryAndWainting();
			});
		}
		break;
	case ChargeAction::State::ExpectingPort:
		if (_expectingRequest == "")
		{
			cm.moveMouseTo(400, 200);
			setState(State::Done, "Charge:Done");
		}
		break;
	case ChargeAction::State::Done:
		return true;
		break;
	default:
		break;
	}
	return false;
}

void ChargeAction::setTeam(int team)
{
	if (team >= 0 && team < 4)
	{
		_team = team;
	}
}

void ChargeAction::setSkipExpedition(bool skip)
{
	_skipExpedition = skip;
}

void ChargeAction::setState(State state, const char* str)
{
	_state = state;
	ControlManager::getInstance().setStateStr(str);
}

bool DestroyShipAction::action()
{
	return true;
}

bool SortieAction::action()
{
	auto& cm = ControlManager::getInstance();
	switch (_state)
	{
	case SortieAction::State::None:

		if (cm.isSouthEastMode())
		{
			if (cm.needChargeAnyShip(0) || cm.hugestDamageInTeam(0) >= WoundState::Middle)
			{
				cm.setToTerminate("Terminated:Damage");
				emit sigFatal();
				return false;
			}
		}
		else if (cm.isLevelMode())
		{
			if (cm.needChargeAnyShip(0) || cm.hugestDamageInTeam(0) > WoundState::Middle)
			{
				cm.setToTerminate("Terminated:Damage");
				emit sigFatal();
				return false;
			}
		}
		else
		{
			if (cm.needChargeFlagship(0) || cm.hugestDamageInTeam(0) >= WoundState::Big)
			{
				cm.setToTerminate("Terminated:Damage");
				emit sigFatal();
				return false;
			}
		}

		cm.moveMouseTo(400, 240);
		setState(State::HomePortChecking, "Sortie:HomePortChecking");
		resetRetryAndWainting();

		break;
	case SortieAction::State::HomePortChecking:
		if (!_waiting)
		{
			_waiting = true;
			QTimer::singleShot(DELAY_TIME, Qt::PreciseTimer, this, [this, &cm]()
			{
				// home port
				if (cm.checkColors(
					213, 247, 251, 155, 32
					, 231, 114, 224, 201, 66))
				{
					_waiting = false;
					setState(State::HomePortDone, "Sortie:HomePortDone");
				}
				else
				{
					tryRetry();
				}
			});
		}
		break;
	case SortieAction::State::HomePortDone:

		if (!_waiting)
		{
			_waiting = true;
			QTimer::singleShot(DELAY_TIME_CLICK, Qt::PreciseTimer, this, [this, &cm]()
			{
				cm.moveMouseToAndClick(195.5f, 259.5f, 36.5f, 31.5f); // sortie button
				setState(State::SortieSelectChecking, "Sortie:SortieSelectChecking");
				resetRetryAndWainting();
			});
		}
		break;
	case SortieAction::State::SortieSelectChecking:
		if (!_waiting)
		{
			_waiting = true;
			QTimer::singleShot(DELAY_TIME, Qt::PreciseTimer, this, [this, &cm]()
			{
				// sortie select
				if (cm.checkColors(
					468, 224, 115, 182, 85
					, 625, 156, 88, 194, 179))
				{
					_waiting = false;
					setState(State::SortieSelectDone, "Sortie:SortieSelectDone");
				}
				else
				{
					tryRetry();
				}
			});
		}
		break;
	case SortieAction::State::SortieSelectDone:

		if (!_waiting)
		{
			_waiting = true;
			QTimer::singleShot(DELAY_TIME_CLICK, Qt::PreciseTimer, this, [this, &cm]()
			{
				cm.moveMouseToAndClick(236, 218.5f, 58, 47.5f); // sortie button 2
				setState(State::SelectAreaChecking, "Sortie:SelectAreaChecking");
				resetRetryAndWainting();
			});
		}
		break;
	case SortieAction::State::SelectAreaChecking:
		if (!_waiting)
		{
			_waiting = true;
			QTimer::singleShot(DELAY_TIME, Qt::PreciseTimer, this, [this, &cm]()
			{
				// area 1 sortie map
				if (cm.checkColors(
					562, 119, 255, 184, 103
					, 644, 246, 137, 143, 154))
//					, 648, 118, 118, 180, 72))
				{
					_waiting = false;
					setState(State::SelectAreaDone, "Sortie:SelectAreaDone");
				}
				else
				{
					tryRetry();
				}
			});
		}
		break;
	case SortieAction::State::SelectAreaDone:
		if (cm.isFuelMode() || cm.isSouthEastMode())
		{
			if (!_waiting)
			{
				_waiting = true;
				QTimer::singleShot(DELAY_TIME_CLICK, Qt::PreciseTimer, this, [this, &cm]()
				{
					cm.moveMouseToAndClick(235, 445, 14, 9); // area 2
					setState(State::SelectMapChecking, "Sortie:SelectMapChecking");
					resetRetryAndWainting();
				});
			}
			// fule mode select 2
		}
		else if (cm.isLevelMode())
		{
			if (!_waiting)
			{
				_waiting = true;
				QTimer::singleShot(DELAY_TIME_CLICK, Qt::PreciseTimer, this, [this, &cm]()
				{
					cm.moveMouseToAndClick(304, 445, 14, 9); // area 3
					setState(State::SelectMapChecking, "Sortie:SelectMapChecking");
					resetRetryAndWainting();
				});
			}
			// selec 3
		}
		else if (cm.isKiraMode())
		{
			// skip to select map
			setState(State::SelectMapDone, "Sortie:SelectMapDone");
		}
		break;
	case SortieAction::State::SelectMapChecking:
		// only fuel mode
		if (!_waiting)
		{
			_waiting = true;
			QTimer::singleShot(DELAY_TIME, Qt::PreciseTimer, this, [this, &cm]()
			{
				if ((cm.isLevelMode() 
					&& cm.checkColors(
					512, 263, 84, 90, 107
					, 618, 190, 179, 211, 206)) ||
				 ((cm.isSouthEastMode() || cm.isFuelMode()) &&
					cm.checkColors(
					246, 326, 197, 106, 166
					, 354, 370, 244, 206, 94)))
				{
					_waiting = false;
					setState(State::SelectMapDone, "Sortie:SelectMapDone");
				}
				else
				{
					tryRetry();
				}
			});
		}
		break;
	case SortieAction::State::SelectMapDone:
		if (cm.isFuelMode() || cm.isSouthEastMode())
		{
			if (!_waiting)
			{
				_waiting = true;
				QTimer::singleShot(DELAY_TIME_CLICK, Qt::PreciseTimer, this, [this, &cm]()
				{
					cm.moveMouseToAndClick(288.5f, 350, 102.5f, 36); // map 3
					setState(State::SortieCheckChecking, "Sortie:SortieCheckChecking");
					resetRetryAndWainting();
				});
			}
			// fule mode select 3
		}
		else if (cm.isLevelMode())
		{
			if (!_waiting)
			{
				_waiting = true;
				QTimer::singleShot(DELAY_TIME_CLICK, Qt::PreciseTimer, this, [this, &cm]()
				{
					cm.moveMouseToAndClick(600, 199, 60, 36); // map 2
					setState(State::SortieCheckChecking, "Sortie:SortieCheckChecking");
					resetRetryAndWainting();
				});
			}
			// level mode select 2
		}
		else if (cm.isKiraMode())
		{
			if (!_waiting)
			{
				_waiting = true;
				QTimer::singleShot(DELAY_TIME_CLICK, Qt::PreciseTimer, this, [this, &cm]()
				{
					cm.moveMouseToAndClick(281, 207, 100, 41); // map 1
					setState(State::SortieCheckChecking, "Sortie:SortieCheckChecking");
					resetRetryAndWainting();
				});
			}
		}
		break;
	case SortieAction::State::SortieCheckChecking:
		if (!_waiting)
		{
			_waiting = true;
			QTimer::singleShot(DELAY_TIME, Qt::PreciseTimer, this, [this, &cm]()
			{
				// sortie ok button
				if (cm.checkColors(
					722, 440, 19, 86, 87
					, 82, 230, 218, 196, 196))
				{
					_waiting = false;
					setState(State::SortieCheckDone, "Sortie:SortieCheckDone");
				}
				else
				{
					tryRetry();
				}
			});
		}
		break;
	case SortieAction::State::SortieCheckDone:

		if (!_waiting)
		{
			_waiting = true;
			QTimer::singleShot(DELAY_TIME_CLICK, Qt::PreciseTimer, this, [this, &cm]()
			{
				cm.moveMouseToAndClick(688.5f, 444, 32.5f, 12); // ok kettei
				setState(State::TeamSelectChecking, "Sortie:TeamSelectChecking");
				resetRetryAndWainting();
			});
		}
		break;
	case SortieAction::State::TeamSelectChecking:
		if (!_waiting)
		{
			_waiting = true;
			QTimer::singleShot(DELAY_TIME, Qt::PreciseTimer, this, [this, &cm]()
			{
				// sortie ok button
				if (cm.checkColors(
					610, 125, 61, 61, 61
					, 551, 237, 255, 246, 242))
				{
					_waiting = false;
					setState(State::TeamSelectDone, "Sortie:TeamSelectDone");
				}
				else
				{
					tryRetry();
				}
			});
		}
		break;
	case SortieAction::State::TeamSelectDone:
		_expectingRequest = "/kcsapi/api_req_map/start";
		if (!_waiting)
		{
			_waiting = true;
			QTimer::singleShot(DELAY_TIME_CLICK, Qt::PreciseTimer, this, [this, &cm]()
			{
				cm.moveMouseToAndClick(631, 446.5f, 54, 12.5f); // ok to start
				setState(State::ExpectingMapStart, "Sortie:ExpectingMapStart");
				resetRetryAndWainting();
			});
		}
		break;
	case SortieAction::State::ExpectingMapStart:
		if (_expectingRequest == "")
		{
			setState(State::Done, "Sortie:Done");
		}
		break;
	case SortieAction::State::Done:
		return true;
		break;
	default:
		break;
	}
	return false;
}

void SortieAction::setState(State state, const char* str)
{
	_state = state;
	ControlManager::getInstance().setStateStr(str);
}

bool SortieAdvanceAction::action()
{
	auto& cm = ControlManager::getInstance();
	switch (_state)
	{
	case SortieAdvanceAction::State::None:
		_expectingRequest = "/kcsapi/api_port/port";
		setState(State::ExpectingPort, "Advance:ExpectingPort");
		break;
	case SortieAdvanceAction::State::ExpectingPort:
		if (!_waiting)
		{
			_waiting = true;
			QTimer::singleShot(DELAY_TIME_LONG, Qt::PreciseTimer, this, [this, &cm]()
			{
				if (_expectingRequest == "")
				{
					setState(State::Done, "Advance:Done");
				}
				else
				{
					cm.moveMouseToAndClick(289.5f, 239.5f, 33.5f, 18.5f); // advance
				}
				resetRetryAndWainting();
			});
		}
		break;
	case SortieAdvanceAction::State::Done:
		return true;
		break;
	default:
		break;
	}
	return false;
}

void SortieAdvanceAction::setState(State state, const char* str)
{
	_state = state;
	ControlManager::getInstance().setStateStr(str);
}

bool SortieCommonAdvanceAction::action()
{
	auto& cm = ControlManager::getInstance();
	switch (_state)
	{
	case SortieCommonAdvanceAction::State::None:

		setState(State::Checking, "SortieAdv:Checking");
		resetRetryAndWainting();
		_expectingRequest = "/kcsapi/api_port/port";

		break;
	case SortieCommonAdvanceAction::State::Checking:
		if (!_waiting)
		{
			_waiting = true; 
			if (_expectingRequest == "")
			{
				setState(State::Done, "Advance:Done");
				break;
			}
			if (!_shouldRetrieve)
			{
				_shouldRetrieve = cm.shouldRetrieve();
			}
			QTimer::singleShot(_shouldRetrieve ? DELAY_TIME_SUPERLONG : DELAY_TIME_LONG, Qt::PreciseTimer, this, [this, &cm]()
			{
				// select formation
				if (cm.checkColors(
					416, 177, 0, 120, 121
					, 450, 185, 233, 231, 227
					, 398, 190, 214, 210, 111))
				{
					_waiting = false;
					setState(State::SelectFormation, "SortieAdv:SelectFormation");
				}
				// leave or night
				else if (cm.checkColors(
					380, 33, 0, 3, 8
					, 334, 230, 70, 155, 194
					, 302, 237, 242, 254, 253))
				{
					_waiting = false;
					// click left or right
					if (cm.shouldNightBattle())
					{
						setState(State::ClickRight, "SortieAdv:NightBattle");
					}
					else
					{
						setState(State::ClickLeft, "SortieAdv:LeaveBattle");
					}
				}
				// advance or not
				else if (cm.checkColors(
					78, 112, 52, 81, 99
					, 579, 133, 38, 87, 115
					,  511, 288, 238, 255, 255))
				{
					_waiting = false;
					// click left or right
					if (_shouldRetrieve || cm.shouldRetrieve())
					{
						setState(State::ClickRight, "SortieAdv:Retrieve");
					}
					else
					{
						setState(State::ClickLeft, "SortieAdv:Advance");
					}
				}
				// else
				else
				{
					_waiting = false;
					setState(State::ClickElse, "SortieAdv:ClickElse");
				}
			});
		}
		break;
	case SortieCommonAdvanceAction::State::SelectFormation:

		if (!_waiting)
		{
			_waiting = true;
			if (_shouldRetrieve && !cm.isLevelMode())
			{
				if (_shouldRetrieve)
				{
					cm.setToTerminate("Terminated:Fatal");
					emit sigFatal();
					return false;
				}
			}
			else
			{
				QTimer::singleShot(DELAY_TIME_CLICK, Qt::PreciseTimer, this, [this, &cm]()
				{
					cm.moveMouseToAndClick(445, 184); // formation button
					setStateToChecking();
					resetRetryAndWainting();
				});
			}
		}
		break;
	case SortieCommonAdvanceAction::State::ClickElse:

		if (!_waiting)
		{
			_waiting = true;
			QTimer::singleShot(DELAY_TIME_CLICK, Qt::PreciseTimer, this, [this, &cm]()
			{
				cm.moveMouseToAndClick(729, 416, 15, 15);
				setStateToChecking();
				resetRetryAndWainting();
			});
		}
		break;
	case SortieCommonAdvanceAction::State::ClickLeft:

		if (!_waiting)
		{
			_waiting = true;
			if (_shouldRetrieve)
			{
				cm.setToTerminate("Terminated:Fatal");
				emit sigFatal();
				return false;
			}
			QTimer::singleShot(DELAY_TIME_CLICK, Qt::PreciseTimer, this, [this, &cm]()
			{
				cm.moveMouseToAndClick(296, 240, 25, 20); // left button
				setStateToChecking();
				resetRetryAndWainting();
			});
		}
		break;
	case SortieCommonAdvanceAction::State::ClickRight:

		if (!_waiting)
		{
			_waiting = true;
			QTimer::singleShot(DELAY_TIME_CLICK, Qt::PreciseTimer, this, [this, &cm]()
			{
				cm.moveMouseToAndClick(512, 240, 25, 20); // formation button
				setStateToChecking();
				resetRetryAndWainting();
			});
		}
		break;
	case SortieCommonAdvanceAction::State::Done:
		return true;
		break;
	default:
		break;
	}
	return false;
}

void SortieCommonAdvanceAction::setStateToChecking()
{
	setState(State::Checking, _shouldRetrieve ? "SortieAdv:CheckingToRetrieve" : "SortieAdv:Checking");
}

void SortieCommonAdvanceAction::setState(State state, const char* str)
{
	_state = state;
	ControlManager::getInstance().setStateStr(str);
}

bool RepeatAction::action()
{
	auto& cm = ControlManager::getInstance();
	switch (_state)
	{
	case RepeatAction::State::None:
		if (cm.isFuelMode())
		{
			if (cm.BuildNext_Fuel())
			{
				setState(State::Done, "Repeat:Done");
				cm.StartJob();
			}
		}
		else if (cm.isKiraMode())
		{
			if (cm.BuildNext_Kira())
			{
				setState(State::Done, "Repeat:Done");
				cm.StartJob();
			}
		}
		else if (cm.isSouthEastMode())
		{
			if (cm.BuildNext_SouthEast())
			{
				setState(State::Done, "Repeat:Done");
				cm.StartJob();
			}
		}
		else if (cm.isExpeditionMode())
		{
			if (cm.BuildNext_Expedition())
			{
				setState(State::Done, "Repeat:Done");
				cm.StartJob();
			}
		}
		else if (cm.isLevelMode())
		{
			if (cm.BuildNext_Level())
			{
				setState(State::Done, "Repeat:Done");
				cm.StartJob();
			}
		}
		break;
	case RepeatAction::State::Done:
		return true;
		break;
	default:
		break;
	}
	return false;
}

void RepeatAction::setState(State state, const char* str)
{
	_state = state;
	ControlManager::getInstance().setStateStr(str);
}

void ExpeditionAction::setTeamAndTarget(int team, int area, int item)
{
	if (team > 0 && team < 4)
	{
		_team = team;
	}
	if (area >= 0 && area < 5)
	{
		_area = area;
	}
	if (item >= 0 && item < 8)
	{
		_item = item;
	}
}

bool ExpeditionAction::action()
{
	auto& cm = ControlManager::getInstance();
	switch (_state)
	{
	case ExpeditionAction::State::None:

		if (cm.needChargeAnyShip(_team) || cm.hugestDamageInTeam(_team) > WoundState::Full
			|| (_area != 0 && _area != 4))
		{
			cm.setToTerminate("Terminated:Damage");
			emit sigFatal();
			return false;
		}

		cm.moveMouseTo(400, 240);
		setState(State::HomePortChecking, "Expedition:HomePortChecking");
		resetRetryAndWainting();

		break;
	case ExpeditionAction::State::HomePortChecking:
		if (!_waiting)
		{
			_waiting = true;
			QTimer::singleShot(DELAY_TIME, Qt::PreciseTimer, this, [this, &cm]()
			{
				// home port
				if (cm.checkColors(
					213, 247, 251, 155, 32
					, 231, 114, 224, 201, 66))
				{
					_waiting = false;
					setState(State::HomePortDone, "Expedition:HomePortDone");
				}
				else
				{
					tryRetry();
				}
			});
		}
		break;
	case ExpeditionAction::State::HomePortDone:

		if (!_waiting)
		{
			_waiting = true;
			QTimer::singleShot(DELAY_TIME_CLICK, Qt::PreciseTimer, this, [this, &cm]()
			{
				cm.moveMouseToAndClick(195.5f, 259.5f, 36.5f, 31.5f); // sortie button
				setState(State::SortieSelectChecking, "Expedition:SortieSelectChecking");
				resetRetryAndWainting();
			});
		}
		break;
	case ExpeditionAction::State::SortieSelectChecking:
		if (!_waiting)
		{
			_waiting = true;
			QTimer::singleShot(DELAY_TIME, Qt::PreciseTimer, this, [this, &cm]()
			{
				// sortie select
				if (cm.checkColors(
					468, 224, 115, 182, 85
					, 625, 156, 88, 194, 179))
				{
					_waiting = false;
					setState(State::SortieSelectDone, "Expedition:SortieSelectDone");
				}
				else
				{
					tryRetry();
				}
			});
		}
		break;
	case ExpeditionAction::State::SortieSelectDone:

		if (!_waiting)
		{
			_waiting = true;
			QTimer::singleShot(DELAY_TIME_CLICK, Qt::PreciseTimer, this, [this, &cm]()
			{
				cm.moveMouseToAndClick(675, 218.5f, 58, 47.5f); // expedition button
				setState(State::SelectAreaChecking, "Expedition:SelectAreaChecking");
				resetRetryAndWainting();
			});
		}
		break;
	case ExpeditionAction::State::SelectAreaChecking:
		if (!_waiting)
		{
			_waiting = true;
			QTimer::singleShot(DELAY_TIME, Qt::PreciseTimer, this, [this, &cm]()
			{
				if (cm.checkColors(
					508, 112, 250, 174, 89
					, 592, 165, 35, 158, 159))
				{
					_waiting = false;
					setState(State::SelectAreaDone, "Expedition:SelectAreaDone");
				}
				else
				{
					tryRetry();
				}
			});
		}
		break;
	case ExpeditionAction::State::SelectAreaDone:
		if (_area == 4)
		{
			if (!_waiting)
			{
				_waiting = true;
				QTimer::singleShot(DELAY_TIME_CLICK, Qt::PreciseTimer, this, [this, &cm]()
				{
					cm.moveMouseToAndClick(135+60*_area , 436); // area
					setState(State::SelectItemChecking, "Expedition:SelectItemChecking");
					resetRetryAndWainting();
				});
			}
		}
		else if (_area == 0)
		{
			// skip to select item
			setState(State::SelectItemDone, "Expedition:SelectItemDone");
		}
		break;
	case ExpeditionAction::State::SelectItemChecking:
		if (!_waiting)
		{
			_waiting = true;
			QTimer::singleShot(DELAY_TIME, Qt::PreciseTimer, this, [this, &cm]()
			{
				if (cm.checkColors(
					233, 170, 86, 82, 76
					, 213, 233, 236, 228, 215))
				{
					_waiting = false;
					setState(State::SelectItemDone, "Expedition:SelectItemDone");
				}
				else
				{
					tryRetry();
				}
			});
		}
		break;
	case ExpeditionAction::State::SelectItemDone:
		if (!_waiting)
		{
			_waiting = true;
			QTimer::singleShot(DELAY_TIME_CLICK, Qt::PreciseTimer, this, [this, &cm]()
			{
				cm.moveMouseToAndClick(404, 172+_item*31, 101, 5); // item
				setState(State::SortieCheckChecking, "Expedition:SortieCheckChecking");
				resetRetryAndWainting();
			});
		}
		break;
	case ExpeditionAction::State::SortieCheckChecking:
		if (!_waiting)
		{
			_waiting = true;
			QTimer::singleShot(DELAY_TIME, Qt::PreciseTimer, this, [this, &cm]()
			{
				// expedition ok button
				if (cm.checkColors(
					709, 438, 23, 94, 97
					, 667, 436, 255, 248, 238))
				{
					_waiting = false;
					setState(State::SortieCheckDone, "Expedition:SortieCheckDone");
				}
				else
				{
					tryRetry();
				}
			});
		}
		break;
	case ExpeditionAction::State::SortieCheckDone:

		if (!_waiting)
		{
			_waiting = true;
			QTimer::singleShot(DELAY_TIME_CLICK, Qt::PreciseTimer, this, [this, &cm]()
			{
				cm.moveMouseToAndClick(678, 444, 32.5f, 12); // ok kettei
				setState(State::TeamSelectChecking, "Expedition:TeamSelectChecking");
				resetRetryAndWainting();
			});
		}
		break;
	case ExpeditionAction::State::TeamSelectChecking:
		if (!_waiting)
		{
			_waiting = true;
			QTimer::singleShot(DELAY_TIME, Qt::PreciseTimer, this, [this, &cm]()
			{
				if (cm.checkColors(
					571, 129, 35, 35, 35
					, 637, 170, 255, 246, 242))
				{
					_waiting = false;
					if (_team == 1)
					{
						setState(State::TeamSelectedChecking, "Expedition:TeamSelectedChecking");
					}
					else
					{
						setState(State::TeamSelectDone, "Expedition:TeamSelectDone");
					}
				}
				else
				{
					tryRetry();
				}
			});
		}
		break;
	case ExpeditionAction::State::TeamSelectDone:
		if (!_waiting)
		{
			_waiting = true;
			QTimer::singleShot(DELAY_TIME_LONG, Qt::PreciseTimer, this, [this, &cm]()	// use longer
			{
				cm.moveMouseToAndClick(364+30*_team, 118); // team
				setState(State::TeamSelectedChecking, "Expedition:TeamSelectedChecking");
				resetRetryAndWainting();
			});
		}
		break;
	case ExpeditionAction::State::TeamSelectedChecking:
		if (!_waiting)
		{
			_waiting = true;
			QTimer::singleShot(DELAY_TIME, Qt::PreciseTimer, this, [this, &cm]()
			{
				if (cm.checkColors(
					652, 429, 44, 165, 153
					, 694, 451, 255, 226, 90))
				{
					_waiting = false;
					setState(State::TeamSelectedDone, "Expedition:TeamSelectedDone");
				}
				else
				{
					tryRetry();
				}
			});
		}
		break;
		break;
	case ExpeditionAction::State::TeamSelectedDone:
		if (!_waiting)
		{
			_waiting = true;
			QTimer::singleShot(DELAY_TIME_CLICK, Qt::PreciseTimer, this, [this, &cm]()
			{
				cm.moveMouseToAndClick(631, 446.5f, 54, 12.5f); // ok to start
				setState(State::Skipping, "Expedition:Skipping");
				resetRetryAndWainting();
			});
		}
		break;

	case ExpeditionAction::State::Skipping:
		if (!_waiting)
		{
			_waiting = true;
			QTimer::singleShot(DELAY_TIME_SKIP, Qt::PreciseTimer, this, [this, &cm]()
			{
				_expectingRequest = "/kcsapi/api_port/port";
				cm.moveMouseToAndClick(75, 261); // home button
				setState(State::ExpectingPort, "Charge:ExpectingPort");
				resetRetryAndWainting();
			});
		}
		break;
	case ExpeditionAction::State::ExpectingPort:
		if (_expectingRequest == "")
		{
			cm.moveMouseTo(400, 200);
			setState(State::Done, "Expedition:Done");
		}
		break;
	case ExpeditionAction::State::Done:
		return true;
		break;
	default:
		break;
	}
	return false;
}

void ExpeditionAction::setState(State state, const char* str)
{
	_state = state;
	ControlManager::getInstance().setStateStr(str);
}
