#include "ControlAction.h"
#include "ControlManager.h"
#include <QDebug>
#include "kandatacalc.h"
#include "mainwindow.h"
#include "ExpeditionManager.h"

#define DELAY_TIME			(250*_intervalMul)
#define DELAY_TIME_CLICK	(250*_intervalMul)
#define DELAY_TIME_LONG	(500*_intervalMul)
#define DELAY_TIME_PAGE	(300*_intervalMul)
#define DELAY_TIME_SUPERLONG	(1200*_intervalMul)
#define DELAY_TIME_SUPERSUPERLONG	(2000*_intervalMul)
#define DELAY_TIME_SKIP_DESTROY	(4500*_intervalMul)
#define DELAY_TIME_SKIP_REPAIR	(10000*_intervalMul)
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
		ControlManager::getInstance().setToTerminate("Terminated:RetryTimeOver", true);
		emit sigCheckFail();
	}
}

bool WaitCondAction::action()
{
	auto& cm = ControlManager::getInstance();
	switch (_state)
	{
	case WaitCondAction::State::None:
	{
		qint64 ct = TimerMainWindow::currentMS();
		_waitMS = _waitMSTo - ct;
		if (_waitMS < 0)
		{
			_waitMS = 1;
		}
		setState(State::Waiting, _waitName.toLocal8Bit());
		resetRetryAndWainting();
	}
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
			cm.setupAutoExpedition();
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

void WaitCondAction::setWaitMS(qint64 waitms, bool isExpedition)
{
	qint64 ct = TimerMainWindow::currentMS();
	if (!isExpedition)
	{
		int team = -1;
		auto timerWindow = MainWindow::mainWindow()->timerWindow();

		QList<int> excludeTeams;
		SingleExpedition* pExp;
		qint64 waitMSExpedition = 0;
		for (int i = 0; i < 3; i++)
		{
			waitMSExpedition = timerWindow->getMinExpeditionMS(team, excludeTeams);
			pExp = ExpeditionManager::getInstance().getShouldNextSchedule(team, ct, ct + waitMSExpedition);
			if (!pExp)
			{
				excludeTeams.append(team);
			}
			else
			{
				break;
			}
		}

		if (pExp)
		{
			waitMSExpedition += 5000 + ControlManager::getInstance().randVal(-1000, 1000);
			if (waitMSExpedition < waitms)
			{
				waitms = waitMSExpedition;
			}
		}
	}

	_waitMSTo = waitms + ct;
}

void WaitCondAction::setWaitName(const QString& name)
{
	_waitName = name;
}

void WaitCondAction::setState(State state, const char* str)
{
	_state = state;
	ControlManager::getInstance().setStateStr(str);
}

bool WaitNextPortAction::action()
{
	auto& cm = ControlManager::getInstance();
	switch (_state)
	{
	case WaitNextPortAction::State::None:
		MainWindow::mainWindow()->timerWindow()->playSound(TimerMainWindow::SoundIndex::Action);
		_expectingRequest = "/kcsapi/api_port/port";
		setState(State::ExpectingPort, "Wait:WaitingForPort");
		resetRetryAndWainting();
		break;
	case WaitNextPortAction::State::ExpectingPort:
		if (_expectingRequest == "")
		{
			cm.moveMouseTo(400, 200);
			cm.setInactiveWaiting(false);
			setState(State::Done, "Wait:Done");
			cm.setupAutoExpedition();
		}
		else if (!_waiting)
		{
			_waiting = true;
			cm.setInactiveWaiting(true);
			QTimer::singleShot(_maxWaitMS, Qt::PreciseTimer, this, [this, &cm]()
			{
				_waiting = false;
				cm.setInactiveWaiting(false);
				setState(State::Done, "Wait:Done");
			});
		}
		break;
	case WaitNextPortAction::State::Done:
		_expectingRequest = "";
		cm.setInactiveWaiting(false);
		return true;
		break;
	default:
		break;
	}
	return false;

}

void WaitNextPortAction::setMaxWaitMS(qint64 waitms)
{
	_maxWaitMS = waitms;
	if (_maxWaitMS < 0)
	{
		_maxWaitMS = 1;
	}
}

void WaitNextPortAction::setState(State state, const char* str)
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
		if (cm.getTeamSize(_team) == _ships.size())
		{
			while (cm.isHenseiDone(_ships, _team, _nowIndex))
			{
				_nowIndex++;
				if (_nowIndex == _ships.size())
				{
					setState(State::Done, "Hensei:Done");
					break;
				}
			}
		}

		for (auto shipno : _ships)
		{
			int page;
			int pos;
			if (ControlManager::getInstance().findPagePosByShipId(shipno, page, pos, _lastPage))
			{
				_pageList.append(page);
				_posList.append(pos);
			}
			else
			{
				cm.setToTerminate("Terminated:HenseiError", true);
				emit sigCheckFail();
				return false;
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
				cm.moveMouseToAndClick(135 + 30 * _team, 118); // Fleet

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
				// need remove all
				if (cm.getTeamSize(_team) > _ships.size())
				{
					setState(State::RemoveAllOtherChecking, "Hensei:RemoveAllOtherChecking");
					resetRetryAndWainting();
					return;
				}

				while (cm.isHenseiDone(_ships, _team, _nowIndex))
				{
					_nowIndex++;
					if (_nowIndex == _ships.size())
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
	case ChangeHenseiAction::State::RemoveAllOtherChecking:
		if (!_waiting)
		{
			_waiting = true;
			QTimer::singleShot(DELAY_TIME, Qt::PreciseTimer, this, [this, &cm]()
			{
				if (cm.checkColors(
					34, 144, 238, 213, 54
					, 404, 208, 84, 178, 105))
				{
					_waiting = false;
					setState(State::RemoveAllOtherDone, "Hensei:RemoveAllOtherDone");
				}
				else
				{
					tryRetry();
				}
			});
		}
		break;
	case ChangeHenseiAction::State::RemoveAllOtherDone:
		if (!_waiting)
		{
			_waiting = true;
			QTimer::singleShot(DELAY_TIME_CLICK, Qt::PreciseTimer, this, [this, &cm]()
			{
				cm.moveMouseToAndClick(420, 119, 20, 3); // remove all button

				// jump to done directly
				QTimer::singleShot(DELAY_TIME_SUPERLONG, Qt::PreciseTimer, this, [this, &cm]()
				{
					setState(State::HenseiDone, "Hensei:HenseiDone");
					resetRetryAndWainting();
				});
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
				if (_nowIndex == _ships.size() - 1)
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
							cm.setToTerminate("Terminated:HenseiError", true);
							emit sigCheckFail();
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
			cm.setupAutoExpedition();
		}
		break;
	case ChangeHenseiAction::State::Done:
		if (cm.isHenseiDone(_ships, _team))
		{
			return true;
		}
		else
		{
			cm.setToTerminate("Terminated:HenseiError", true);
			emit sigCheckFail();
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
	for (int shipno : ships)
	{
		if (shipno < 0)
		{
			continue;
		}
		_ships.append(shipno);
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
				if ((_team > 0 || _forceTeamChange) && !_teamChanged)
				{
					cm.moveMouseToAndClick(148 + _team * 30, 119); // team
					setState(State::NeedChargeChecking, "Charge:NeedChargeChecking");
					// for second team
					if (_team == 1)
					{
						if (cm.checkColors(183, 118, 35, 160, 161))
						{
							_teamChanged = true;
						}
					}
					else
					{
						_teamChanged = true;
					}
					_forceTeamChange = true;
				}
				else
				{
					if (cm.isKiraMode() && _team == 0)
					{
						cm.moveMouseToAndClick(466, 165, 20, 5); // first ship
						setState(State::OKToChargeChecking, "Charge:OKToChargeChecking");
					}
					else
					{
						cm.moveMouseToAndClick(117, 120, 2, 2); // all ships
						setState(State::OKToChargeDone, "Charge:OKToChargeDone");
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
				// other teams! todo check
				for (int i = 0; i < 4; i++)
				{
					if (i != _team)
					{
						if (cm.needChargeFlagship(i))
						{
							_team = i;
							_teamChanged = false;
							_forceTeamChange = true;
							setState(State::NeedChargeChecking, "Charge:NeedChargeChecking");
							resetRetryAndWainting();
							return;
						}
					}
				}
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
			cm.setupAutoExpedition();
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
	auto& cm = ControlManager::getInstance();
	switch (_state)
	{
	case DestroyShipAction::State::None:

		_nowIndex = 0;
		setState(State::HomePortChecking, "Destroy:HomePortChecking");
		while (!cm.isShipExist(_ships[_nowIndex]))
		{
			_nowIndex++;
			if (_nowIndex == _ships.size())
			{
				setState(State::Done, "Destroy:Done");
				break;
			}
		}

		_nowEndIndex = _nowIndex;
		_nowSelectingPosIndex = 0;
		_posList.clear();

		int page;
		int pos;
		if (ControlManager::getInstance().findPagePosByShipId(_ships[_nowIndex], page, pos, _lastPage))
		{
			_page = page;
			_posList.append(pos);

			for (int i = _nowIndex + 1; i < _ships.count(); i++)
			{
				if (!cm.isShipExist(_ships[i]))
				{
					continue;
				}
				else
				{
					int tpage;
					int tpos;
					int tlastPage;
					if (ControlManager::getInstance().findPagePosByShipId(_ships[i], tpage, tpos, tlastPage))
					{
						if (tpage == _page)
						{
							_nowEndIndex = i;
							_posList.append(tpos);
						}
					}
				}
			}
		}
		else
		{
			cm.setToTerminate("Terminated:DestroyError", true);
			emit sigCheckFail();
			return false;
		}
		resetRetryAndWainting();
		break;
	case DestroyShipAction::State::HomePortChecking:
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
					setState(State::HomePortDone, "Destroy:HomePortDone");
				}
				else
				{
					tryRetry();
				}
			});
		}
		break;
	case DestroyShipAction::State::HomePortDone:
		if (!_waiting)
		{
			_waiting = true;
			if (!cm.isShipExist(_ships[_nowIndex]))
			{
				setState(State::Done, "Destroy:Done");
				resetRetryAndWainting();
			}
			else
			{
				QTimer::singleShot(DELAY_TIME_CLICK, Qt::PreciseTimer, this, [this, &cm]()
				{
					cm.moveMouseToAndClick(274, 361, 23, 21.5f); // kousyou button
					setState(State::KouShouSelectChecking, "Destroy:KouShouSelectChecking");
					resetRetryAndWainting();
				});
			}
		}
		break;
	case DestroyShipAction::State::KouShouSelectChecking:
		if (!_waiting)
		{
			_waiting = true;
			QTimer::singleShot(DELAY_TIME, Qt::PreciseTimer, this, [this, &cm]()
			{
				// destroy
				if (cm.checkColors(
					300, 257, 131, 58, 44
					, 240, 241, 253, 245, 220))
				{
					_waiting = false;
					setState(State::KouShouSelectDone, "Destroy:KouShouSelectDone");
				}
				else
				{
					tryRetry();
				}
			});
		}
		break;
	case DestroyShipAction::State::KouShouSelectDone:
		if (!_waiting)
		{
			_waiting = true;
			QTimer::singleShot(DELAY_TIME_CLICK, Qt::PreciseTimer, this, [this, &cm]()
			{
				cm.moveMouseToAndClick(229, 252, 66, 22); // destroy

				setState(State::FindShipChecking, "Destroy:FindShipChecking");
				resetRetryAndWainting();
			});
		}
		break;
	case DestroyShipAction::State::FindShipChecking:
		if (!_waiting)
		{
			_waiting = true;
			QTimer::singleShot(DELAY_TIME, Qt::PreciseTimer, this, [this, &cm]()
			{
				if (cm.checkColors(
					31, 358, 206, 205, 52
					, 579, 113, 44, 144, 144))
				{
					_waiting = false;
					setState(State::FindShipDone, "Destroy:FindShipDone");
				}
				else if (cm.checkColors(
					31, 358, 206, 205, 52
					, 354, 111, 67, 60, 49))
				{
					_waiting = false;
					setState(State::FindShipChangeSort, "Destroy:FindShipChangeSort");
				}
				else
				{
					tryRetry();
				}
			});
		}
		break;
	case DestroyShipAction::State::FindShipChangeSort:
		if (!_waiting)
		{
			_waiting = true;
			QTimer::singleShot(DELAY_TIME_CLICK, Qt::PreciseTimer, this, [this, &cm]()
			{
				cm.moveMouseToAndClick(584, 112, 15, 5); // sort button
				setState(State::FindShipChecking, "Destroy:FindShipChecking");
				resetRetryAndWainting();
			});
		}
		break;
	case DestroyShipAction::State::FindShipDone:
		if (!_waiting)
		{
			_waiting = true;
			QTimer::singleShot(DELAY_TIME_CLICK, Qt::PreciseTimer, this, [this, &cm]()
			{
				if (cm.checkColors(
					31, 358, 206, 205, 52
					, 298, 450, 30, 190, 195))
				{
					setState(State::FindShipFirstPageDone, "Destroy:FindShipFirstPageDone");
					resetRetryAndWainting();
				}
				else
				{
					cm.moveMouseToAndClick(218, 452); // left most button
					setState(State::FindShipFirstPageChecking, "Destroy:FindShipFirstPageChecking");
					resetRetryAndWainting();
				}
			});
		}
		break;
	case DestroyShipAction::State::FindShipFirstPageChecking:
		if (!_waiting)
		{
			_waiting = true;
			QTimer::singleShot(DELAY_TIME, Qt::PreciseTimer, this, [this, &cm]()
			{
				if (cm.checkColors(
					31, 358, 206, 205, 52
					, 298, 450, 30, 190, 195))
				{
					_waiting = false;
					setState(State::FindShipFirstPageDone, "Destroy:FindShipFirstPageDone");
				}
				else
				{
					tryRetry();
				}
			});
		}
		break;
	case DestroyShipAction::State::FindShipFirstPageDone:
		if (!_waiting)
		{
			_waiting = true;
			if (_page < 0 || _posList[_nowSelectingPosIndex] < 0)
			{
				cm.setToTerminate("Terminated:DestroyPageError", true);
				emit sigCheckFail();
				return false;
			}
			else if (_page == 0)
			{
				QTimer::singleShot(DELAY_TIME_CLICK, Qt::PreciseTimer, this, [this, &cm]()
				{
					cm.moveMouseToAndClick(300, 136 + _cellHeight*_posList[_nowSelectingPosIndex]);

					_nowSelectingPosIndex++;
					if (_nowSelectingPosIndex >= _posList.count())
					{
						setState(State::FindShipOKChecking, "Destroy:FindShipOKChecking");
					}
					resetRetryAndWainting();
				});
			}
			else
			{
				QTimer::singleShot(DELAY_TIME_CLICK, Qt::PreciseTimer, this, [this, &cm]()
				{
					if (_lastPage == _page)
					{
						cm.moveMouseToAndClick(496, 452); // last page
						_curPage = _lastPage;
					}
					else if (_page >= 5)
					{
						cm.moveMouseToAndClick(461, 449); // 6th page
						_curPage = 5;
					}
					else
					{
						cm.moveMouseToAndClick(332, 446); // second page
						_curPage = 1;
					}
					setState(State::FindShipNextPageChecking, "Destroy:FindShipNextPageChecking");
					resetRetryAndWainting();
				});
			}
		}
		break;
	case DestroyShipAction::State::FindShipNextPageChecking:
		if (!_waiting)
		{
			_waiting = true;
			QTimer::singleShot(DELAY_TIME_PAGE, Qt::PreciseTimer, this, [this, &cm]()
			{
				_waiting = false;
				setState(State::FindShipNextPageDone, "Destroy:FindShipNextPageDone");
				// just assume page flip is ok
			});
		}
		break;
	case DestroyShipAction::State::FindShipNextPageDone:
		if (!_waiting)
		{
			_waiting = true;
			if (_page == _curPage)
			{
				QTimer::singleShot(DELAY_TIME_CLICK, Qt::PreciseTimer, this, [this, &cm]()
				{
					cm.moveMouseToAndClick(300, 136 + _cellHeight*_posList[_nowSelectingPosIndex]);
					_nowSelectingPosIndex++;
					if (_nowSelectingPosIndex >= _posList.count())
					{
						setState(State::FindShipOKChecking, "Destroy:FindShipOKChecking");
					}
					resetRetryAndWainting();
				});
			}
			else
			{
				QTimer::singleShot(DELAY_TIME_CLICK, Qt::PreciseTimer, this, [this, &cm]()
				{
					if (_curPage == 1)
					{
						cm.moveMouseToAndClick(362, 447); // third page
						_curPage++;
					}
					else if (_page >= _curPage + 5)
					{
						cm.moveMouseToAndClick(461, 449); // 6th page
						_curPage += 5;
					}
					else
					{
						cm.moveMouseToAndClick(393, 446); // fourth and further
						_curPage++;
					}
					setState(State::FindShipNextPageChecking, "Destroy:FindShipNextPageChecking");
					resetRetryAndWainting();
				});
			}
		}
		break;
	case DestroyShipAction::State::FindShipOKChecking:
		if (!_waiting)
		{
			_waiting = true;
			QTimer::singleShot(DELAY_TIME_LONG, Qt::PreciseTimer, this, [this, &cm]()
			{
				if (cm.checkColors(
					31, 358, 206, 205, 52
					, 678, 444, 20, 96, 89))
				{
					_waiting = false;
					setState(State::FindShipOKDone, "Destroy:FindShipOKDone");
				}
				else
				{
					tryRetry();
				}
			});
		}
		break;
	case DestroyShipAction::State::FindShipOKDone:
		if (!_waiting)
		{
			_waiting = true;
			QTimer::singleShot(DELAY_TIME_CLICK, Qt::PreciseTimer, this, [this, &cm]()
			{
				cm.moveMouseToAndClick(700, 434, 40, 12); // Destroy button
				if (_nowEndIndex == _ships.size() - 1)
				{
					_waiting = true;
					{
						QTimer::singleShot(DELAY_TIME_SKIP_DESTROY + cm.randVal(0, 250), Qt::PreciseTimer, this, [this, &cm]()
						{
							setState(State::ReturnToPortChecking, "Destroy:ReturnToPortChecking");
							resetRetryAndWainting();
						});
					}
				}
				else
				{
					_waiting = true;
					QTimer::singleShot(DELAY_TIME_SKIP_DESTROY + cm.randVal(0, 250), Qt::PreciseTimer, this, [this, &cm]()
					{
						bool misDestroyed = false;
						for (int i = _nowIndex; i <= _nowEndIndex; i++)
						{
							if (cm.isShipExist(_ships[i]))
							{
								misDestroyed = true;
								break;
							}
						}

						if (misDestroyed)
						{
							qDebug() << "error";
							cm.setToTerminate("Terminated:DestroyError", true);
							emit sigCheckFail();
						}
						else
						{
							_nowIndex = _nowEndIndex + 1;
							_nowEndIndex = _nowIndex;
							_nowSelectingPosIndex = 0;

							int page;
							int pos;
							if (ControlManager::getInstance().findPagePosByShipId(_ships[_nowIndex], page, pos, _lastPage))
							{
								_page = page;
								_posList.append(pos);

								for (int i = _nowIndex + 1; i < _ships.count(); i++)
								{
									if (!cm.isShipExist(_ships[i]))
									{
										continue;
									}
									else
									{
										int tpage;
										int tpos;
										int tlastPage;
										if (ControlManager::getInstance().findPagePosByShipId(_ships[i], tpage, tpos, tlastPage))
										{
											if (tpage == _page)
											{
												_nowEndIndex = i;
												_posList.append(tpos);
											}
										}
									}
								}

								setState(State::FindShipChecking, "Destroy:Checking");
								resetRetryAndWainting();
							}
							else
							{
								cm.setToTerminate("Terminated:DestroyError", true);
								emit sigCheckFail();
							}
						}
					});
				}
			});
		}
		break;
	case DestroyShipAction::State::ReturnToPortChecking:
		if (!_waiting)
		{
			_waiting = true;
			QTimer::singleShot(DELAY_TIME, Qt::PreciseTimer, this, [this, &cm]()
			{
				if (cm.checkColors(
					31, 358, 206, 205, 52
					, 329, 112, 66, 57, 47))
				{
					_waiting = false;
					setState(State::ReturnToPortDone, "Destroy:ReturnToPortDone");
				}
				else
				{
					tryRetry();
				}
			});
		}
		break;
	case  DestroyShipAction::State::ReturnToPortDone:
		if (!_waiting)
		{
			_waiting = true;
			_expectingRequest = "/kcsapi/api_port/port";
			QTimer::singleShot(DELAY_TIME_CLICK, Qt::PreciseTimer, this, [this, &cm]()
			{
				cm.moveMouseToAndClick(74, 252); // port button
				setState(State::ExpectingPort, "Destroy:ExpectingPort");
				resetRetryAndWainting();
			});
		}
		break;
	case DestroyShipAction::State::ExpectingPort:
		if (_expectingRequest == "")
		{
			cm.moveMouseTo(400, 200);
			setState(State::Done, "Destroy:Done");
			cm.setupAutoExpedition();
		}
		break;
	case DestroyShipAction::State::Done:
		return true;
		break;
	default:
		break;
	}

	return false;
}

void DestroyShipAction::setState(State state, const char* str)
{
	_state = state;
	ControlManager::getInstance().setStateStr(str);
}

void DestroyShipAction::setShips(const QList<int>& ships)
{
	for (int shipno : ships)
	{
		if (shipno < 0)
		{
			continue;
		}
		_ships.append(shipno);
	}
}


bool RepairShipAction::action()
{
	auto& cm = ControlManager::getInstance();
	switch (_state)
	{
	case RepairShipAction::State::None:

		_nowIndex = 0;
		setState(State::HomePortChecking, "Repair:HomePortChecking");
		while (!cm.isShipExist(_ships[_nowIndex]))
		{
			_nowIndex++;
			if (_nowIndex == _ships.size())
			{
				setState(State::Done, "Repair:Done");
				break;
			}
		}

		for (int i = 0; i < _ships.size(); i++)
		{
			_pageList.append(-1);
			_posList.append(-1);
		}

		int page;
		int pos;
		if (ControlManager::getInstance().findPagePosByShipId(_ships[_nowIndex], page, pos, _lastPage))
		{
			_pageList[_nowIndex] = page;
			_posList[_nowIndex] = pos;
		}
		else
		{
			cm.setToTerminate("Terminated:RepairError", true);
			emit sigCheckFail();
			return false;
		}
		resetRetryAndWainting();
		break;
	case RepairShipAction::State::HomePortChecking:
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
					setState(State::HomePortDone, "Repair:HomePortDone");
				}
				else
				{
					tryRetry();
				}
			});
		}
		break;
	case RepairShipAction::State::HomePortDone:
		if (!_waiting)
		{
			_waiting = true;
			if (!cm.isShipExist(_ships[_nowIndex]))
			{
				setState(State::Done, "Repair:Done");
				resetRetryAndWainting();
			}
			else
			{
				QTimer::singleShot(DELAY_TIME_CLICK, Qt::PreciseTimer, this, [this, &cm]()
				{
					cm.moveMouseToAndClick(132, 382, 23, 21.5f); // nyukyo button
					setState(State::NyuKyoSelectChecking, "Repair:NyuKyoSelectChecking");
					resetRetryAndWainting();
				});
			}
		}
		break;
	case RepairShipAction::State::NyuKyoSelectChecking:
		if (!_waiting)
		{
			_waiting = true;
			QTimer::singleShot(DELAY_TIME, Qt::PreciseTimer, this, [this, &cm]()
			{
				// Repair
				if (cm.checkColors(
					27, 306, 231, 100, 5
					, 74, 221, 209, 178, 65))
				{
					_waiting = false;
					setState(State::NyuKyoSelectDone, "Repair:NyuKyoSelectDone");
				}
				else
				{
					tryRetry();
				}
			});
		}
		break;
	case RepairShipAction::State::NyuKyoSelectDone:
		if (!_waiting)
		{
			_waiting = true;
			QTimer::singleShot(DELAY_TIME_CLICK, Qt::PreciseTimer, this, [this, &cm]()
			{
				cm.moveMouseToAndClick(250, 162 + _usingSlots[_nowIndex] * 82, 42, 12); // Slot

				setState(State::FindShipChecking, "Repair:FindShipChecking");
				resetRetryAndWainting();
			});
		}
		break;
	case RepairShipAction::State::FindShipChecking:
		if (!_waiting)
		{
			_waiting = true;
			QTimer::singleShot(DELAY_TIME, Qt::PreciseTimer, this, [this, &cm]()
			{
				if (cm.checkColors(
					27, 306, 231, 100, 5
					, 768, 111, 44, 144, 144))
				{
					_waiting = false;
					setState(State::FindShipDone, "Repair:FindShipDone");
				}
				else if (cm.checkColors(
					27, 306, 231, 100, 5
					, 712, 457, 196, 188, 105))
				{
					_waiting = false;
					setState(State::FindShipChangeSort, "Repair:FindShipChangeSort");
				}
				else
				{
					tryRetry();
				}
			});
		}
		break;
	case RepairShipAction::State::FindShipChangeSort:
		if (!_waiting)
		{
			_waiting = true;
			QTimer::singleShot(DELAY_TIME_CLICK, Qt::PreciseTimer, this, [this, &cm]()
			{
				cm.moveMouseToAndClick(773, 110, 15, 5); // sort button
				setState(State::FindShipChecking, "Repair:FindShipChecking");
				resetRetryAndWainting();
			});
		}
		break;
	case RepairShipAction::State::FindShipDone:
		if (!_waiting)
		{
			_waiting = true;
			QTimer::singleShot(DELAY_TIME_CLICK, Qt::PreciseTimer, this, [this, &cm]()
			{
				if (cm.checkColors(
					27, 306, 231, 100, 5
					, 515, 457, 49, 190, 193))
				{
					setState(State::FindShipFirstPageDone, "Repair:FindShipFirstPageDone");
					resetRetryAndWainting();
				}
				else
				{
					cm.moveMouseToAndClick(439, 457); // left most button
					setState(State::FindShipFirstPageChecking, "Repair:FindShipFirstPageChecking");
					resetRetryAndWainting();
				}
			});
		}
		break;
	case RepairShipAction::State::FindShipFirstPageChecking:
		if (!_waiting)
		{
			_waiting = true;
			QTimer::singleShot(DELAY_TIME, Qt::PreciseTimer, this, [this, &cm]()
			{
				if (cm.checkColors(
					27, 306, 231, 100, 5
					, 515, 457, 49, 190, 193))
				{
					_waiting = false;
					setState(State::FindShipFirstPageDone, "Repair:FindShipFirstPageDone");
				}
				else
				{
					tryRetry();
				}
			});
		}
		break;
	case RepairShipAction::State::FindShipFirstPageDone:
		if (!_waiting)
		{
			_waiting = true;
			if (_pageList[_nowIndex] < 0 || _posList[_nowIndex] < 0)
			{
				cm.setToTerminate("Terminated:RepairPageError", true);
				emit sigCheckFail();
				return false;
			}
			else if (_pageList[_nowIndex] == 0)
			{
				QTimer::singleShot(DELAY_TIME_CLICK, Qt::PreciseTimer, this, [this, &cm]()
				{
					cm.moveMouseToAndClick(439, 138 + _cellHeight*_posList[_nowIndex]);
					setState(State::FindShipOKChecking, "Repair:FindShipOKChecking");
					resetRetryAndWainting();
				});
			}
			else
			{
				QTimer::singleShot(DELAY_TIME_CLICK, Qt::PreciseTimer, this, [this, &cm]()
				{
					if (_lastPage == _pageList[_nowIndex])
					{
						cm.moveMouseToAndClick(712, 457); // last page
						_curPage = _lastPage;
					}
					else if (_pageList[_nowIndex] >= 5)
					{
						cm.moveMouseToAndClick(680, 457); // 6th page
						_curPage = 5;
					}
					else
					{
						cm.moveMouseToAndClick(550, 454); // second page
						_curPage = 1;
					}
					setState(State::FindShipNextPageChecking, "Repair:FindShipNextPageChecking");
					resetRetryAndWainting();
				});
			}
		}
		break;
	case RepairShipAction::State::FindShipNextPageChecking:
		if (!_waiting)
		{
			_waiting = true;
			QTimer::singleShot(DELAY_TIME_PAGE, Qt::PreciseTimer, this, [this, &cm]()
			{
				_waiting = false;
				setState(State::FindShipNextPageDone, "Repair:FindShipNextPageDone");
				// just assume page flip is ok
			});
		}
		break;
	case RepairShipAction::State::FindShipNextPageDone:
		if (!_waiting)
		{
			_waiting = true;
			if (_pageList[_nowIndex] == _curPage)
			{
				QTimer::singleShot(DELAY_TIME_CLICK, Qt::PreciseTimer, this, [this, &cm]()
				{
					cm.moveMouseToAndClick(439, 138 + _cellHeight*_posList[_nowIndex]);
					setState(State::FindShipOKChecking, "Repair:FindShipOKChecking");
					resetRetryAndWainting();
				});
			}
			else
			{
				QTimer::singleShot(DELAY_TIME_CLICK, Qt::PreciseTimer, this, [this, &cm]()
				{
					if (_curPage == 1)
					{
						cm.moveMouseToAndClick(580, 454); // third page
						_curPage++;
					}
					else if (_pageList[_nowIndex] >= _curPage + 5)
					{
						cm.moveMouseToAndClick(680, 457); // 6th page
						_curPage += 5;
					}
					else
					{
						cm.moveMouseToAndClick(609, 455); // fourth and further
						_curPage++;
					}
					setState(State::FindShipNextPageChecking, "Repair:FindShipNextPageChecking");
					resetRetryAndWainting();
				});
			}
		}
		break;
	case RepairShipAction::State::FindShipOKChecking:
		if (!_waiting)
		{
			_waiting = true;
			QTimer::singleShot(DELAY_TIME_LONG, Qt::PreciseTimer, this, [this, &cm]()
			{
				if (cm.checkColors(
					650, 422, 57, 189, 186
					, 675, 441, 16, 78, 86))
				{
					_waiting = false;
					setState(State::FindShipOKDone, "Repair:FindShipOKDone");
				}
				else
				{
					tryRetry();
				}
			});
		}
		break;
	case RepairShipAction::State::FindShipOKDone:
		if (!_waiting)
		{
			_waiting = true;
			QTimer::singleShot(DELAY_TIME_CLICK, Qt::PreciseTimer, this, [this, &cm]()
			{
				if (_useFastRepair)
				{
					cm.moveMouseToAndClick(734, 289, 27, 7); // toggle
					setState(State::NyuKyoFastToggleChecking, "Repair:NyuKyoFastToggleChecking");
					resetRetryAndWainting();
				}
				else
				{
					cm.moveMouseToAndClick(685, 432, 40, 12); // Repair button
					setState(State::NyuKyoOKChecking, "Repair:NyuKyoOKChecking");
					resetRetryAndWainting();
				}
			});
		}
		break;
	case RepairShipAction::State::NyuKyoFastToggleChecking:
		_waiting = true;
		QTimer::singleShot(DELAY_TIME_LONG, Qt::PreciseTimer, this, [this, &cm]()
		{
			if (cm.checkColors(
				697, 288, 40, 138, 134
				, 675, 441, 16, 78, 86))
			{
				_waiting = false;
				setState(State::NyuKyoFastTogglDone, "Repair:NyuKyoFastTogglDone");
			}
			else
			{
				tryRetry();
			}
		});
		break;
	case RepairShipAction::State::NyuKyoFastTogglDone:
		if (!_waiting)
		{
			_waiting = true;
			QTimer::singleShot(DELAY_TIME_CLICK, Qt::PreciseTimer, this, [this, &cm]()
			{
				cm.moveMouseToAndClick(685, 432, 40, 12); // Repair button
				setState(State::NyuKyoOKChecking, "Repair:NyuKyoOKChecking");
				resetRetryAndWainting();
			});
		}
		break;
	case RepairShipAction::State::NyuKyoOKChecking:
		_waiting = true;
		QTimer::singleShot(DELAY_TIME_LONG, Qt::PreciseTimer, this, [this, &cm]()
		{
			if (cm.checkColors(
				428, 23, 26, 174, 162
				, 347, 387, 138, 203, 193))
			{
				_waiting = false;
				setState(State::NyuKyoOKDone, "Repair:NyuKyoOKDone");
			}
			else
			{
				tryRetry();
			}
		});
		break;
	case RepairShipAction::State::NyuKyoOKDone:
		if (!_waiting)
		{
			_waiting = true;
			QTimer::singleShot(DELAY_TIME_CLICK, Qt::PreciseTimer, this, [this, &cm]()
			{
				cm.moveMouseToAndClick(502, 399, 24, 8); // ok button
				setState(State::Skipping, "Repair:Skipping");
				resetRetryAndWainting();
			});
		}
		break;

	case RepairShipAction::State::Skipping:
		if (!_waiting)
		{
			_waiting = true;
			int delayTime = DELAY_TIME_SUPERSUPERLONG;
			if (_useFastRepair)
			{
				delayTime = DELAY_TIME_SKIP_REPAIR + cm.randVal(0, 250);
			}

			if (_nowIndex == _ships.size() - 1)
			{
				_waiting = true;
				{
					QTimer::singleShot(delayTime, Qt::PreciseTimer, this, [this, &cm]()
					{
						setState(State::ReturnToPortChecking, "Repair:ReturnToPortChecking");
						resetRetryAndWainting();
					});
				}
			}
			else
			{
				_waiting = true;
				QTimer::singleShot(delayTime, Qt::PreciseTimer, this, [this, &cm]()
				{
					if (_state != State::Skipping)
					{
						return;
					}
					if (_useFastRepair && !cm.isShipDamaged(_ships[_nowIndex]) ||
						!_useFastRepair && cm.isShipInDock(_ships[_nowIndex]))
					{
						_nowIndex++;

						int page;
						int pos;
						if (ControlManager::getInstance().findPagePosByShipId(_ships[_nowIndex], page, pos, _lastPage))
						{
							_pageList[_nowIndex] = page;
							_posList[_nowIndex] = pos;
							setState(State::NyuKyoSelectChecking, "Repair:NyuKyoSelectChecking");
							resetRetryAndWainting();
						}
						else
						{
							cm.setToTerminate("Terminated:RepairError", true);
							emit sigCheckFail();
						}
					}
					else
					{
						qDebug() << "error";
						cm.setToTerminate("Terminated:RepairError", true);
						emit sigCheckFail();
					}
				});
			}
		}
		break;
	case RepairShipAction::State::ReturnToPortChecking:
		if (!_waiting)
		{
			_waiting = true;
			QTimer::singleShot(DELAY_TIME, Qt::PreciseTimer, this, [this, &cm]()
			{
				if (cm.checkColors(
					27, 306, 231, 100, 5
					, 30, 476, 100, 99, 94))
				{
					_waiting = false;
					setState(State::ReturnToPortDone, "Repair:ReturnToPortDone");
				}
				else
				{
					tryRetry();
				}
			});
		}
		break;
	case  RepairShipAction::State::ReturnToPortDone:
		if (!_waiting)
		{
			_waiting = true;
			_expectingRequest = "/kcsapi/api_port/port";
			QTimer::singleShot(DELAY_TIME_CLICK, Qt::PreciseTimer, this, [this, &cm]()
			{
				cm.moveMouseToAndClick(74, 252); // port button
				setState(State::ExpectingPort, "Repair:ExpectingPort");
				resetRetryAndWainting();
			});
		}
		break;
	case RepairShipAction::State::ExpectingPort:
		if (_expectingRequest == "")
		{
			cm.moveMouseTo(400, 200);
			setState(State::Done, "Repair:Done");
			cm.setupAutoExpedition();
		}
		break;
	case RepairShipAction::State::Done:
		return true;
		break;
	default:
		break;
	}

	return false;
}

void RepairShipAction::setState(State state, const char* str)
{
	_state = state;
	ControlManager::getInstance().setStateStr(str);
}

void RepairShipAction::setShips(const QList<int>& ships, const QList<int>& usingSlots, bool useFastRepair)
{
	for (int i = 0; i < ships.size(); i++)
	{
		if (ships[i] < 0)
		{
			continue;
		}
		_ships.append(ships[i]);
		_usingSlots.append(usingSlots[i]);
	}

	_useFastRepair = useFastRepair;
}

void DevelopAction::addItem(int itemId, int buildCount)
{
	_toBuildSlotItems[itemId] = buildCount;// +ControlManager::getInstance().getTotalSlotItemCountForID(itemId);
}


bool DevelopAction::action()
{
	auto& cm = ControlManager::getInstance();
	switch (_state)
	{
	case State::None:
	{
		setState(DevelopAction::State::SelectDevelopChecking, "SelectDevelopChecking");
		QMap<int, int>::iterator it = _toBuildSlotItems.begin();
		bool bAllDone = true;
		while (it != _toBuildSlotItems.end()) {
			int slotitemId = it.key();
			if (cm.getTotalSlotItemCountForID(slotitemId) < it.value())
			{
				bAllDone = false;
				break;
			}
			++it;
		}
		if (bAllDone)
		{
			setState(State::ReturnToPortChecking, "Develop:ReturnToPortChecking");
			break;
		}

		resetRetryAndWainting();
	}
	break;
	case State::SelectDevelopChecking:
		if (!_waiting)
		{
			_waiting = true;
			QTimer::singleShot(DELAY_TIME, Qt::PreciseTimer, this, [this, &cm]()
			{
				// ok to develop
				if (cm.checkColors(
					220, 360, 204, 113, 48
					, 700, 441, 76, 231, 223))
				{
					// skip to ok
					_waiting = false;
					setState(State::SelectOKDone, "Develop:SelectOKDone");
				}
				// destroy
				else if (cm.checkColors(
					300, 257, 131, 58, 44
					, 240, 241, 253, 245, 220))
				{
					_waiting = false;
					setState(State::SelectDevelopDone, "Develop:SelectDevelopDone");
				}
				else
				{
					tryRetry();
				}
			});
		}
		break;
	case State::SelectDevelopDone:
		if (!_waiting)
		{
			_waiting = true;
			QTimer::singleShot(DELAY_TIME_CLICK, Qt::PreciseTimer, this, [this, &cm]()
			{
				cm.moveMouseToAndClick(204, 338, 66, 22); // destroy
				setState(State::SelectOKChecking, "Develop:SelectOKChecking");
				resetRetryAndWainting();
			});
		}
		break;
	case State::SelectOKChecking:
		if (!_waiting)
		{
			_waiting = true;
			QTimer::singleShot(DELAY_TIME, Qt::PreciseTimer, this, [this, &cm]()
			{
				// ok to develop
				if (cm.checkColors(
					220, 360, 204, 113, 48
					, 700, 441, 76, 231, 223))
				{
					_waiting = false;
					setState(State::SelectOKDone, "Develop:SelectOKDone");
				}
				else
				{
					tryRetry();
				}
			});
		}
		break;
	case State::SelectOKDone:
		if (!_waiting)
		{
			_waiting = true;
			QTimer::singleShot(DELAY_TIME_CLICK, Qt::PreciseTimer, this, [this, &cm]()
			{
				cm.moveMouseToAndClick(697, 443, 55, 12); // ok button
				setState(State::Skipping, "Develop:Skipping");
				resetRetryAndWainting();
			});
		}
		break;

	case State::Skipping:
		if (!_waiting)
		{
			_waiting = true;
			QTimer::singleShot(DELAY_TIME, Qt::PreciseTimer, this, [this, &cm]()
			{
				// destroy
				if (cm.checkColors(
					300, 257, 131, 58, 44
					, 240, 241, 253, 245, 220))
				{
					_waiting = false;

					QMap<int, int>::iterator it = _toBuildSlotItems.begin();
					bool bAllDone = true;
					while (it != _toBuildSlotItems.end()) {
						int slotitemId = it.key();
						if (cm.getTotalSlotItemCountForID(slotitemId) < it.value())
						{
							bAllDone = false;
							break;
						}
						++it;
					}
					if (bAllDone)
					{
						setState(State::ReturnToPortChecking, "Develop:ReturnToPortChecking");
					}
					else
					{
						setState(State::SelectDevelopDone, "Develop:SelectDevelopDone");
					}
				}
				else
				{
					cm.moveMouseToAndClick(738, 401);
					resetRetryAndWainting();
				}
			});
		}
		break;
	case State::ReturnToPortChecking:
		if (!_waiting)
		{
			_waiting = true;
			QTimer::singleShot(DELAY_TIME, Qt::PreciseTimer, this, [this, &cm]()
			{
				if (cm.checkColors(
					31, 358, 206, 205, 52
					, 502, 448, 140, 56, 19))
				{
					_waiting = false;
					setState(State::ReturnToPortDone, "Develop:ReturnToPortDone");
				}
				else
				{
					tryRetry();
				}
			});
		}
		break;
	case  State::ReturnToPortDone:
		if (!_waiting)
		{
			_waiting = true;
			_expectingRequest = "/kcsapi/api_port/port";
			QTimer::singleShot(DELAY_TIME_CLICK, Qt::PreciseTimer, this, [this, &cm]()
			{
				cm.moveMouseToAndClick(74, 252); // port button
				setState(State::ExpectingPort, "Develop:ExpectingPort");
				resetRetryAndWainting();
			});
		}
		break;
	case State::ExpectingPort:
		if (_expectingRequest == "")
		{
			cm.moveMouseTo(400, 200);
			setState(State::Done, "Develop:Done");
			cm.setupAutoExpedition();
		}
		break;
	case State::Done:
		return true;
		break;
	}

	return false;
}


void DevelopAction::setState(State state, const char* str)
{
	_state = state;
	ControlManager::getInstance().setStateStr(str);
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
				cm.setToTerminate("Terminated:Damage", true);

				emit sigFatal();
				return false;
			}
		}
		else if (cm.isLevelMode() || cm.isRankMode())
		{
			if (cm.needChargeAnyShip(0) || cm.hugestDamageInTeam(0) > WoundState::Middle)
			{
				cm.setToTerminate("Terminated:Damage", true);
				emit sigFatal();
				return false;
			}
		}
		else
		{
			// any
			if ((cm.needChargeFlagship(-1) || cm.hugestDamageInTeam(-1) >= WoundState::Big))
			{
				cm.setToTerminate("Terminated:Damage", true);
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
					//					468, 224, 115, 182, 85
					468, 224, 108, 175, 78
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
					600, 121, 237, 146, 14
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
	{
		if (_area == 1)
		{
			// skip to select map
			setState(State::SelectMapDone, "Sortie:SelectMapDone");
		}
		else
		{
			if (!_waiting)
			{
				_waiting = true;
				QTimer::singleShot(DELAY_TIME_CLICK, Qt::PreciseTimer, this, [this, &cm]()
				{
					QMap<int, QList<float>> areaPoints;
					areaPoints[2] = { 235, 445, 14, 9 };
					areaPoints[3] = { 304, 445, 14, 9 };
					areaPoints[4] = { 377, 445, 14, 9 };
					areaPoints[5] = { 452, 445, 14, 9 };
					areaPoints[6] = { 528, 445, 14, 9 };

					QList<float> pPoint;
					if (_isEvent)
					{
						pPoint = { 711, 445, 14, 9 };	//event
					}
					else
					{
						pPoint = areaPoints[_area];
					}

					cm.moveMouseToAndClick(pPoint[0], pPoint[1], pPoint[2], pPoint[3]);
					setState(State::SelectMapChecking, "Sortie:SelectMapChecking");
					resetRetryAndWainting();
				});
			}
		}
	}
	break;
	case SortieAction::State::SelectMapChecking:
		if (!_waiting)
		{
			_waiting = true;
			QTimer::singleShot(DELAY_TIME, Qt::PreciseTimer, this, [this, &cm]()
			{

				QList<float> pPoint;
				if (_isEvent)
				{
					pPoint = _areaCheckList;
				}
				else
				{
					QMap<int, QList<float>> areaPoints;
					areaPoints[2] = {
						246, 326, 197, 106, 166
						, 354, 370, 244, 206, 94 };

					areaPoints[3] = {
						512, 263, 84, 90, 107
						, 618, 190, 179, 211, 206 };

					areaPoints[4] = {
						384, 197, 206, 227, 147
						, 340, 340, 155, 90, 93 };

					areaPoints[5] = {
						520, 365, 24, 32, 72
						, 657, 327, 122, 139, 123 };

					areaPoints[6] = {
						215, 251, 28, 104, 209
						, 215, 362, 87, 117, 102 };
					pPoint = areaPoints[_area];
				}

				if (cm.checkColors(pPoint[0], pPoint[1], pPoint[2], pPoint[3], pPoint[4]
					, pPoint[5], pPoint[6], pPoint[7], pPoint[8], pPoint[9]))
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

		if (!_waiting)
		{
			_waiting = true;

			// use in every mode
			if (cm.needChargeCondAirBase(cm.getAnySetting().checkAirBaseCond))
			{
				cm.setToTerminate("Terminated:AirBase", true, RemoteNotifyHandler::Level::Low);
				emit sigFatal();
				return false;
			}

			QTimer::singleShot(DELAY_TIME_CLICK, Qt::PreciseTimer, this, [this, &cm]()
			{
				QList<float> pPoint;
				if (_isEvent)
				{
					pPoint = _mapClickPoint;
				}
				else
				{
					QMap<int, QList<float>> mapPoints;
					mapPoints[1] = { 281, 207, 100, 41 };
					mapPoints[2] = { 600, 199, 60, 36 };
					mapPoints[3] = { 288.5f, 350, 102.5f, 36 };
					mapPoints[4] = { 600, 350, 60, 36 };
					mapPoints[5] = mapPoints[6] = { 720, 275, 30, 50 };
					pPoint = mapPoints[_map];
				}

				cm.moveMouseToAndClick(pPoint[0], pPoint[1], pPoint[2], pPoint[3]); // map 1

				if (_map > 4)	// when ex map is over 10
				{
					setState(State::SelectExMapChecking, "Sortie:SelectExMapChecking");
				}
				else
				{
					if (_isEvent)
					{
						setState(State::SkipBoardChecking, "Sortie:SkipBoardChecking");
					}
					else
					{
						setState(State::SortieCheckChecking, "Sortie:SortieCheckChecking");
					}
				}

				resetRetryAndWainting();
			});
		}

		break;


	case SortieAction::State::SelectExMapChecking:
		if (!_waiting)
		{
			_waiting = true;
			QTimer::singleShot(DELAY_TIME_LONG, Qt::PreciseTimer, this, [this, &cm]()
			{
				QList<float> pPoint;
				if (_isEvent)
				{
					pPoint = _mapExCheckList;
				}
				else
				{
					QMap<int, QList<float>> areaPoints;
					areaPoints[1] = {
						160, 291, 250, 43, 43
						, 162, 279, 255, 255, 255 };
					areaPoints[2] = {
						160, 291, 250, 43, 43
						, 162, 279, 255, 255, 255 };
					areaPoints[3] = {
						160, 291, 250, 43, 43
						, 162, 279, 255, 255, 255 };
					areaPoints[4] = {
						160, 291, 250, 43, 43
						, 162, 279, 255, 255, 255 };
					areaPoints[5] = {
						160, 291, 250, 43, 43
						, 162, 279, 255, 255, 255 };
					areaPoints[6] = {
						160, 291, 250, 43, 43
						, 162, 279, 255, 255, 255 };
					pPoint = areaPoints[_area];
				}

				if (cm.checkColors(pPoint[0], pPoint[1], pPoint[2], pPoint[3], pPoint[4]
					, pPoint[5], pPoint[6], pPoint[7], pPoint[8], pPoint[9]))
				{
					_waiting = false;
					setState(State::SelectExMapDone, "Sortie:SelectExMapDone");
				}
				else
				{
					tryRetry();
				}
			});
		}
		break;
	case SortieAction::State::SelectExMapDone:

		if (!_waiting)
		{
			_waiting = true;
			QTimer::singleShot(DELAY_TIME_CLICK, Qt::PreciseTimer, this, [this, &cm]()
			{
				QList<float> pPoint;
				if (_isEvent)
				{
					pPoint = _mapExClickPoint;
				}
				else
				{
					QMap<int, QList<float>> mapPoints;
					mapPoints[5] = { 482, 208, 126, 41 };
					mapPoints[6] = { 466, 349, 121, 51 };
					pPoint = mapPoints[_map];
				}
				cm.moveMouseToAndClick(pPoint[0], pPoint[1], pPoint[2], pPoint[3]); // map 1

				if (_isEvent)
				{
					if (_map > 20)	// ex2
					{
						setState(State::SelectEx2MapChecking, "Sortie:SelectEx2MapChecking");
					}
					else
					{
						setState(State::SkipBoardChecking, "Sortie:SkipBoardChecking");
					}
				}
				else
				{
					setState(State::SortieCheckChecking, "Sortie:SortieCheckChecking");
				}
				resetRetryAndWainting();
			});
		}

		break;
	case SortieAction::State::SelectEx2MapChecking:
		if (!_waiting)
		{
			_waiting = true;
			QTimer::singleShot(DELAY_TIME_LONG, Qt::PreciseTimer, this, [this, &cm]()
			{
				QList<float> pPoint;
				pPoint = _mapEx2CheckList;

				if (cm.checkColors(pPoint[0], pPoint[1], pPoint[2], pPoint[3], pPoint[4]
					, pPoint[5], pPoint[6], pPoint[7], pPoint[8], pPoint[9]))
				{
					_waiting = false;
					setState(State::SelectEx2MapDone, "Sortie:SelectEx2MapDone");
				}
				else
				{
					tryRetry();
				}
			});
		}
		break;
	case SortieAction::State::SelectEx2MapDone:

		if (!_waiting)
		{
			_waiting = true;
			QTimer::singleShot(DELAY_TIME_CLICK, Qt::PreciseTimer, this, [this, &cm]()
			{
				QList<float> pPoint;
				pPoint = _mapEx2ClickPoint;
				cm.moveMouseToAndClick(pPoint[0], pPoint[1], pPoint[2], pPoint[3]); // map 1

				// should always be ev
				if (_isEvent)
				{
					setState(State::SkipBoardChecking, "Sortie:SkipBoardChecking");
				}
				else
				{
					setState(State::SortieCheckChecking, "Sortie:SortieCheckChecking");
				}
				resetRetryAndWainting();
			});
		}

		break;

	case SortieAction::State::SkipBoardChecking:
		if (!_waiting)
		{
			_waiting = true;
			QTimer::singleShot(DELAY_TIME, Qt::PreciseTimer, this, [this, &cm]()
			{
				// blackboard

				// init blackboard
				if (cm.checkColors(
					451, 83, 181, 154, 104
					, 342, 380, 43, 68, 18))
				{
					_waiting = true;
					QTimer::singleShot(DELAY_TIME_CLICK, Qt::PreciseTimer, this, [this, &cm]()
					{
						cm.moveMouseToAndClick(365, 253, 136, 88); // blackboard
						setState(State::SkipBoardChecking, "Sortie:SkipBoardChecking");
						resetRetryAndWainting();
					});
				}
				else if (cm.checkColors(
					582, 58, 34, 162, 158
					, 342, 380, 49, 69, 79))
				{
					_waiting = false;
					setState(State::SkipBoardDone, "Sortie:SkipBoardDone");
				}
				else
				{
					tryRetry();
				}
			});
		}
		break;
	case SortieAction::State::SkipBoardDone:

		if (!_waiting)
		{
			_waiting = true;
			QTimer::singleShot(DELAY_TIME_CLICK, Qt::PreciseTimer, this, [this, &cm]()
			{
				cm.moveMouseToAndClick(365, 253, 136, 88); // blackboard
				setState(State::SortieCheckChecking, "Sortie:SortieCheckChecking");
				resetRetryAndWainting();
			});
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
					739, 458, 110, 198, 193
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
				// sortie go button
				if (cm.checkColors(
					633, 434, 229, 69, 66
					, 551, 237, 255, 246, 242) ||
					// with airbase
					cm.checkColors(630, 434, 156, 48, 31
					, 82, 230, 218, 196, 196
					, 608, 454, 88, 179, 83) ||
					// normal active)
					cm.checkColors(
					600, 467, 237, 235, 210
					, 551, 237, 255, 246, 242) ||
					// with airbase
					cm.checkColors(575, 456, 125, 143, 99
					, 82, 230, 218, 196, 196,
					680, 478, 93, 168, 168))
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
				cm.moveMouseToAndClick(631, 434, 54, 6); // ok to start
				setState(State::ExpectingMapStart, "Sortie:ExpectingMapStart");
				resetRetryAndWainting();
			});
		}
		break;
	case SortieAction::State::ExpectingMapStart:
		if (_expectingRequest == "")
		{
			setState(State::Done, "Sortie:Done");
			if (_autoPauseNext)
			{
				cm.PauseNext();
				MainWindow::mainWindow()->setPauseNextChanged(true);
			}
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

void SortieAction::setAreaAndMap(int area, int map
	, const QList<float>& areaCheckList/* = QList<float>()*/
	, const QList<float>& mapClickPoint/* = QList<float>()*/
	, const QList<float>& mapExCheckList/* = QList<float>()*/
	, const QList<float>& mapExClickPoint/* = QList<float>()*/
	, const QList<float>& mapEx2CheckList/* = QList<float>()*/
	, const QList<float>& mapEx2ClickPoint/* = QList<float>()*/)
{
	_area = area;
	_map = map;
	_areaCheckList = areaCheckList;
	_mapClickPoint = mapClickPoint;
	_mapExCheckList = mapExCheckList;
	_mapExClickPoint = mapExClickPoint;
	_mapEx2CheckList = mapEx2CheckList;
	_mapEx2ClickPoint = mapEx2ClickPoint;

	if (_area >= 30 || _area < 0)
	{
		_isEvent = true;
	}
}

void SortieAction::setShouldPauseNext(bool bPause)
{
	_autoPauseNext = bPause;
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
					cm.setupAutoExpedition();
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
				cm.setupAutoExpedition();
				break;
			}
			if (!_shouldRetrieve)
			{
				_shouldRetrieve = cm.shouldRetrieve();
			}
			if (!_shouldRetrieve && cm.shouldTerminateForAny())
			{
				cm.setToTerminate("Terminated:TerminateCell", true, RemoteNotifyHandler::Level::Low);
				emit sigFatal();
				return false;
			}
			QTimer::singleShot(_shouldRetrieve ? DELAY_TIME_SUPERLONG : DELAY_TIME_LONG, Qt::PreciseTimer, this, [this, &cm]()
			{
				_totalFormationCount = 0;
				if (cm.checkColors(
					447, 82, 93, 255, 131
					, 424, 298, 93, 255, 131
					, 477, 177, 0, 120, 121
					)
					||
					cm.checkColors(
					447, 81, 93, 255, 131
					, 470, 252, 93, 255, 131
					, 477, 177, 0, 120, 121
					))
				{
					_withKeiKaiJin = true;
					_totalFormationCount = 6;
				}
				else if (
					cm.checkColors(
					513, 82, 93, 255, 131
					, 424, 298, 93, 255, 131
					, 539, 177, 0, 120, 121
					))
				{
					_withKeiKaiJin = true;
					_totalFormationCount = 5;
				}
				// select formation
				else if (cm.checkColors(
					416, 177, 0, 120, 121
					, 450, 185, 233, 231, 227
					, 398, 190, 214, 210, 111))
				{
					_withKeiKaiJin = false;
					_totalFormationCount = 5;
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
					, 511, 288, 238, 255, 255))
				{
					if (cm.isPortDataDirty())
					{
						cm.setToTerminate("Fatal:PortDirty", true);
						emit sigFatal();
						return;
					}
					_waiting = false;
					// click left or right

					if (_shouldRetrieve || cm.shouldRetrieve() || cm.shouldRetrieveForAny())
					{
						if (cm.shouldAskForProceedForAny())
						{
							cm.setToTerminate("Terminated:TerminateCell", false, RemoteNotifyHandler::Level::Low);
							MainWindow::mainWindow()->timerWindow()->playSound(TimerMainWindow::SoundIndex::Action);
							emit sigFatal();
						}
						else
						{
							setState(State::ClickRight, "SortieAdv:Retrieve");
						}
					}
					else
					{
						setState(State::ClickLeft, "SortieAdv:Advance");
					}

				}
				// combined formation
				else if (cm.checkColors(
					460, 167, 0, 122, 123
					, 475, 180, 8, 109, 110
					, 566, 165, 230, 217, 93))
				{
					_waiting = false;
					setState(State::SelectCombinedFormation, "SortieAdv:SelectCombinedFormation");
				}
				// else
				else
				{
					_waiting = false;
					setState(State::ClickElse, "SortieAdv:ClickElse");
				}

				if (_totalFormationCount > 0)
				{
					if (cm.isPortDataDirty())
					{
						cm.setToTerminate("Fatal:PortDirty", true);
						emit sigFatal();
						return;
					}
					_waiting = false;
					setState(State::SelectFormation, "SortieAdv:SelectFormation");
				}
			});
		}
		break;
	case SortieCommonAdvanceAction::State::SelectFormation:

		if (!_waiting)
		{
			cm.setPortDataDirty();
			_waiting = true;
			if (_shouldRetrieve
				&& !cm.isLevelMode()	// level mode always set to retrieve
				)
			{
				cm.setToTerminate("Terminated:SortieShouldRetrieve", true);
				emit sigFatal();
				return false;
			}
			else
			{
				QTimer::singleShot(DELAY_TIME_CLICK, Qt::PreciseTimer, this, [this, &cm]()
				{
					QMap<int, QList<float>> points;
					switch (_totalFormationCount)
					{
					case 3:
					case 4:
					case 5:
						if (_withKeiKaiJin)
						{
							points[1] = { 513, 184 };
							points[2] = { 646, 184 };
							points[3] = { 445, 341 };
							points[4] = { 577, 341 };
							points[5] = { 706, 341 };
						}
						else
						{
							points[1] = { 445, 184 };
							points[2] = { 577, 184 };
							points[3] = { 706, 184 };
							points[4] = { 513, 341 };
							points[5] = { 646, 341 };

						}
						break;
					case 6:
						points[1] = { 445, 184 };
						points[2] = { 577, 184 };
						points[3] = { 706, 184 };
						points[4] = { 445, 341 };
						points[5] = { 577, 341 };
						points[6] = { 706, 341 };
						break;
					}
					int formation = 1;

					if (cm.isAnyMode())
					{
						const auto& setting = cm.getAnySetting();
						int cell = KanSaveData::getInstance().nextdata.api_no;
						if (setting.cells.contains(cell))
						{
							formation = setting.cells[cell].formation;
							if (formation > points.count() || formation < 1)
							{
								formation = 1;
							}
						}
						// all ss always 6
						if (_withKeiKaiJin && cm.isAllSSShips(0) && cm.getTeamSize(0) >= 5)
						{
							if (formation == 1 || formation == 5)
							{
								formation = 6;
							}
						}
					}
					cm.moveMouseToAndClick(points[formation][0], points[formation][1]); // formation button
					setStateToChecking();
					resetRetryAndWainting();
				});
			}
		}
		break;

	case SortieCommonAdvanceAction::State::SelectCombinedFormation:

		if (!_waiting)
		{
			_waiting = true;
			if (_shouldRetrieve
				&& !cm.isLevelMode()	// level mode always set to retrieve
				)
			{
				cm.setToTerminate("Terminated:SortieShouldRetrieve", true);
				emit sigFatal();
				return false;
			}
			else
			{
				QTimer::singleShot(DELAY_TIME_CLICK, Qt::PreciseTimer, this, [this, &cm]()
				{
					QMap<int, QList<float>> points;
					points[1] = { 497, 172 };
					points[2] = { 662, 172 };
					points[3] = { 497, 308 };
					points[4] = { 662, 308 };
					int formation = 4;

					if (cm.isAnyMode())
					{
						const auto& setting = cm.getAnySetting();
						int cell = KanSaveData::getInstance().nextdata.api_no;
						if (setting.cells.contains(cell))
						{
							formation = setting.cells[cell].formation;
							if (formation > 4 || formation < 1)
							{
								formation = 4;
							}
						}
					}
					cm.moveMouseToAndClick(points[formation][0], points[formation][1]); // formation button
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
				const auto& setting = cm.getAnySetting();
				int cell = KanSaveData::getInstance().nextdata.api_no;
				if (setting.cells.contains(cell) && setting.cells[cell].clickX >= 0 && setting.cells[cell].clickY >= 0)
				{
					cm.moveMouseToAndClick(setting.cells[cell].clickX, setting.cells[cell].clickY);
				}
				else
				{
					cm.moveMouseToAndClick(729, 416, 15, 15);
				}

				setStateToChecking();
				resetRetryAndWainting();
			});
		}
		break;
	case SortieCommonAdvanceAction::State::ClickLeft:

		if (!_waiting)
		{
			_waiting = true;
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
	MainWindow* mainWindow = MainWindow::mainWindow();

	bool newJobFromSwitch = false;
	switch (_state)
	{
	case RepeatAction::State::None:
		if (cm.isFuelMode() || cm.isSouthEastMode())
		{
			if (cm.BuildNext_Fuel())
			{
				setState(State::Done, "Repeat:Done");
				cm.StartJob();
			}
			else
			{
				newJobFromSwitch = mainWindow->switchToExpeditionWait();
			}
		}
		else if (cm.isKiraMode())
		{
			if (cm.BuildNext_Kira())
			{
				setState(State::Done, "Repeat:Done");
				cm.StartJob();
			}
			else
			{
				newJobFromSwitch = mainWindow->switchToExpeditionWait();
			}
		}
		else if (cm.isExpeditionMode())
		{
			if (cm.isAutoExpeditioning())
			{
				if (cm.switchBackToLastAction())
				{
					return action();
				}
			}
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
			else
			{
				newJobFromSwitch = mainWindow->switchToExpeditionWait();
			}
		}
		else if (cm.isRankMode())
		{
			if (cm.BuildNext_Rank())
			{
				setState(State::Done, "Repeat:Done");
				cm.StartJob();
			}
			else
			{
				newJobFromSwitch = mainWindow->switchToExpeditionWait();
			}
		}
		else if (cm.isAnyMode())
		{
			if (cm.BuildNext_Any(false))
			{
				setState(State::Done, "Repeat:Done");
				cm.StartJob();
			}
			else
			{
				newJobFromSwitch = mainWindow->switchToExpeditionWait();
			}
		}
		else if (cm.isDestroyMode())
		{
			// do not repeat
			newJobFromSwitch = mainWindow->switchToExpeditionWait();
		}
		else if (cm.isRepairMode())
		{
			// do not repeat
			newJobFromSwitch = mainWindow->switchToExpeditionWait();
		}
		else if (cm.isDevelopMode())
		{
			if (cm.BuildNext_Develop())
			{
				setState(State::Done, "Repeat:Done");
				cm.StartJob();
			}
			else
			{
				newJobFromSwitch = mainWindow->switchToExpeditionWait();
			}
		}
		else if (cm.isMissionMode())
		{
			// do not repeat
			newJobFromSwitch = mainWindow->switchToExpeditionWait();
		}
		else if (cm.isRestoreHenseiMode())
		{
			// do not repeat
			newJobFromSwitch = mainWindow->switchToExpeditionWait();
		}
		break;
	case RepeatAction::State::Done:
		return true;
		break;
	default:
		break;
	}
	if (newJobFromSwitch)
	{
		setState(State::Done, "Repeat:Done");
		cm.StartJob();
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

		if (cm.needChargeAnyShip(_team) || cm.hugestDamageInTeam(_team) > WoundState::Full)
		{
			cm.setToTerminate("Terminated:Damage", true);
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
					//					468, 224, 115, 182, 85
					468, 224, 108, 175, 78
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
					583, 182, 242, 233, 221
					, 138, 459, 236, 228, 216))
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
		if (_area > 0)
		{
			if (!_waiting)
			{
				_waiting = true;
				QTimer::singleShot(DELAY_TIME_CLICK, Qt::PreciseTimer, this, [this, &cm]()
				{
					cm.moveMouseToAndClick(135 + 60 * _area, 436); // area
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
					75, 210, 244, 222, 168
					, 583, 182, 242, 233, 221))
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
				cm.moveMouseToAndClick(404, 172 + _item * 31, 101, 5); // item
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
					749, 460, 119, 255, 237
					, 673, 460, 143, 245, 222))
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
					if (_team == 1 && MainWindow::mainWindow()->getWebWidgetType() != WebWidgetType::WebEngine)
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
				cm.moveMouseToAndClick(364 + 30 * _team, 118); // team
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
				// go button
				if (cm.checkColors(
					634, 434, 68, 151, 224
					, 590, 448, 249, 255, 253)
					|| cm.checkColors(
					684, 436, 238, 169, 25
					, 570, 436, 255, 235, 191))
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
			cm.setupAutoExpedition();
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

bool MissionAction::action()
{
	auto& cm = ControlManager::getInstance();
	switch (_state)
	{
	case MissionAction::State::None:

		cm.moveMouseTo(400, 240);
		setState(State::HomePortChecking, "Mission:HomePortChecking");
		resetRetryAndWainting();

		break;
	case MissionAction::State::HomePortChecking:
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
					setState(State::HomePortDone, "Mission:HomePortDone");
				}
				else
				{
					tryRetry();
				}
			});
		}
		break;
	case MissionAction::State::HomePortDone:

		if (!_waiting)
		{
			_waiting = true;
			QTimer::singleShot(DELAY_TIME_CLICK, Qt::PreciseTimer, this, [this, &cm]()
			{
				KanSaveData::getInstance().requireRecordQuestList();
				cm.moveMouseToAndClick(563, 47); // mission button
				setState(State::CheckingMissionState, "Mission:CheckingMissionState");
				resetRetryAndWainting();
			});
		}
		break;
	case MissionAction::State::CheckingMissionState:
		if (!_waiting)
		{
			_waiting = true;
			QTimer::singleShot(DELAY_TIME, Qt::PreciseTimer, this, [this, &cm]()
			{
				do
				{
					auto& pksd = KanSaveData::getInstance();
					if (pksd.isRecordingLastQuestList)
					{
						if (!_isSkippingMissionComplete)
						{
							tryRetry();
						}
						else
						{
							_waiting = false;
							if (cm.checkColors(
								417, 398, 255, 246, 242
								, 438, 403, 35, 158, 159))
							{
								_waiting = false;
								setState(State::CompleteSkip, "Mission::CompleteSkip");
								break;
							}
						}
					}
					else
					{
						_waiting = false;
						_isSkippingMissionComplete = false;
						if (cm.checkColors(
							417, 398, 255, 246, 242
							, 438, 403, 35, 158, 159))
						{
							_waiting = false;
							setState(State::CompleteSkip, "Mission::CompleteSkip");
							break;
						}
						else if (!cm.checkColors(
							699, 461, 244, 237, 224
							))

						{
							_waiting = false;
							setState(State::MissionSkipping, "Mission::MissionSkipping");
							break;
						}
						else if (cm.checkColors(
							202, 77, 196, 169, 87
							, 26, 132, 205, 233, 235
							, 52, 475, 178, 178, 178))
						{
							const auto& questList = pksd.lastQuestList;
							if (_isCompleting)
							{
								if (questList.api_completed_kind == 0)
								{
									_isCompleting = false;
									if (questList.api_disp_page != 1)
									{
										_waiting = false;
										setState(State::SelectFirstPage, "Mission::SelectFirstPage");
										break;
									}
								}
							}

							if (_isCompleting)
							{
								bool toComplete = false;
								for (int i = 0; i < questList.api_list.count(); i++)
								{
									if (questList.api_list[i].api_state == 3)
									{
										_targetIndex = i;
										_waiting = false;
										setState(State::CompleteMission, "Mission:CompleteMission");
										toComplete = true;
										break;
									}
								}
								if (toComplete)
								{
									break;
								}
								else
								{
									_waiting = false;
									setState(State::SelectNextPage, "Mission::SelectNextPage");
									break;
								}
							}
							// to accept new
							else
							{
								auto& missionSetting = cm.getMissionSetting();

								bool toAccept = false;
								for (int i = 0; i < questList.api_list.count(); i++)
								{
									const auto& quest = questList.api_list[i];
									if (missionSetting.todoMissionList.contains(quest.api_no))
									{
										if (quest.api_state == 1)
										{
											_targetIndex = i;
											if (questList.api_exec_count < maxMissionAcceptCount)
											{
												_waiting = false;
												setState(State::AcceptMission, "Mission::AcceptMission");
												toAccept = true;
												break;
											}
										}
										else
										{
											if (!missionSetting.acceptedMissionList.contains(quest.api_no))
											{
												missionSetting.acceptedMissionList.append(quest.api_no);
											}
										}
									}
								}
								if (toAccept)
								{
									break;
								}
								else
								{
									if (questList.api_disp_page < questList.api_page_count)
									{
										_waiting = false;
										setState(State::SelectNextPage, "Mission::SelectNextPage");
										break;
									}
									else
									{
										_waiting = false;
										setState(State::ReturnToPortChecking, "Mission::ReturnToPortChecking");
										break;
									}
								}
							}
						}
					}

				} while (false);
			});
		}
		break;
	case MissionAction::State::MissionSkipping:
		if (!_waiting)
		{
			_waiting = true;
			QTimer::singleShot(DELAY_TIME_CLICK, Qt::PreciseTimer, this, [this, &cm]()
			{
				cm.moveMouseToAndClick(701, 462); // ooyodo
				setState(State::CheckingMissionState, "Mission:CheckingMissionState");
				resetRetryAndWainting();
			});
		}
		break;
	case MissionAction::State::SelectNextPage:
		if (!_waiting)
		{
			_waiting = true;
			QTimer::singleShot(DELAY_TIME_CLICK, Qt::PreciseTimer, this, [this, &cm]()
			{
				KanSaveData::getInstance().requireRecordQuestList();
				cm.moveMouseToAndClick(619, 462); // next page
				setState(State::CheckingMissionState, "Mission:CheckingMissionState");
				resetRetryAndWainting();
			});
		}
		break;
	case MissionAction::State::SelectFirstPage:
		if (!_waiting)
		{
			_waiting = true;
			QTimer::singleShot(DELAY_TIME_CLICK, Qt::PreciseTimer, this, [this, &cm]()
			{
				KanSaveData::getInstance().requireRecordQuestList();
				cm.moveMouseToAndClick(263, 464); // first page
				setState(State::CheckingMissionState, "Mission:CheckingMissionState");
				resetRetryAndWainting();
			});
		}
		break;
	case MissionAction::State::AcceptMission:
		if (!_waiting)
		{
			_waiting = true;
			QTimer::singleShot(DELAY_TIME_CLICK, Qt::PreciseTimer, this, [this, &cm]()
			{
				KanSaveData::getInstance().requireRecordQuestList();
				cm.moveMouseToAndClick(568, 142 + _targetIndex * 68, 113, 17); // cell
				setState(State::CheckingMissionState, "Mission:CheckingMissionState");
				resetRetryAndWainting();
			});
		}
		break;
	case MissionAction::State::CompleteMission:
		if (!_waiting)
		{
			_waiting = true;
			QTimer::singleShot(DELAY_TIME_CLICK, Qt::PreciseTimer, this, [this, &cm]()
			{
				KanSaveData::getInstance().requireRecordQuestList();
				_isSkippingMissionComplete = true;
				cm.moveMouseToAndClick(568, 142 + _targetIndex * 68, 113, 17); // cell
				setState(State::CheckingMissionState, "Mission:CheckingMissionState");
				cm.moveMouseTo(40, 40);
				resetRetryAndWainting();
			});
		}
		break;
	case MissionAction::State::CompleteSkip:
		if (!_waiting)
		{
			_waiting = true;
			QTimer::singleShot(DELAY_TIME_CLICK, Qt::PreciseTimer, this, [this, &cm]()
			{
				cm.moveMouseToAndClick(397, 403, 25, 5); // close
				setState(State::CheckingMissionState, "Mission:CheckingMissionState");
				resetRetryAndWainting();
			});
		}
		break;
	case MissionAction::State::ReturnToPortChecking:
		if (!_waiting)
		{
			_waiting = true;
			QTimer::singleShot(DELAY_TIME, Qt::PreciseTimer, this, [this, &cm]()
			{
				if (cm.checkColors(
					202, 77, 196, 169, 87
					, 26, 132, 205, 233, 235
					, 52, 475, 178, 178, 178))
				{
					_waiting = false;
					setState(State::ReturnToPortDone, "Mission:ReturnToPortDone");
				}
				else
				{
					tryRetry();
				}
			});
		}
		break;
	case  MissionAction::State::ReturnToPortDone:
		if (!_waiting)
		{
			_waiting = true;
			_expectingRequest = "/kcsapi/api_port/port";
			QTimer::singleShot(DELAY_TIME_CLICK, Qt::PreciseTimer, this, [this, &cm]()
			{
				cm.moveMouseToAndClick(56, 459, 30, 10); // return button
				setState(State::ExpectingPort, "Mission:ExpectingPort");
				resetRetryAndWainting();
			});
		}
		break;
	case MissionAction::State::ExpectingPort:
		if (_expectingRequest == "")
		{
			cm.moveMouseTo(400, 200);
			setState(State::Done, "Mission:Done");
			cm.setupAutoExpedition();
		}
		break;
	case MissionAction::State::Done:
		return true;
		break;
	default:
		break;

	}
	return false;
}

void MissionAction::setState(State state, const char* str)
{
	_state = state;
	ControlManager::getInstance().setStateStr(str);
}
