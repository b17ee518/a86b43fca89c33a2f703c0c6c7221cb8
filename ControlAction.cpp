#include "ControlAction.h"
#include "ControlManager.h"
#include <QDebug>
#include "kandatacalc.h"

#define DELAY_TIME			250
#define DELAY_TIME_CLICK	250
#define DELAY_TIME_LONG	500
#define DELAY_TIME_PAGE	300
#define DELAY_TIME_SUPERLONG	1200

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
			int waitMS = cm.getWaitMS();
			if (waitMS < 0)
			{
				waitMS = 1;
			}
			QTimer::singleShot(waitMS, this, [this, &cm]()
			{
				_waiting = false;
				setState(State::HomePortChecking, "Wait:HomePortChecking");
			});
		}
		break;
	case WaitCondAction::State::HomePortChecking:
		if (!_waiting)
		{
			_waiting = true;
			QTimer::singleShot(DELAY_TIME, this, [this, &cm]()
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
			QTimer::singleShot(DELAY_TIME_CLICK, this, [this, &cm]()
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
			QTimer::singleShot(DELAY_TIME, this, [this, &cm]()
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
			QTimer::singleShot(DELAY_TIME_CLICK, this, [this, &cm]()
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
		while (cm.isHenseiDone(_ships, _nowIndex))
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
			QTimer::singleShot(DELAY_TIME, this, [this, &cm]()
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
			if (cm.isHenseiDone(_ships))
			{
				setState(State::Done, "Hensei:Done");
				resetRetryAndWainting();
			}
			else
			{
				QTimer::singleShot(DELAY_TIME_CLICK, this, [this, &cm]()
				{
					cm.moveMouseToAndClick(199, 135.5f, 23, 21.5f); // change button
					setState(State::HenseiChecking, "Hensei:HenseiChecking");
					resetRetryAndWainting();
				});
			}
		}
		break;
	case ChangeHenseiAction::State::HenseiChecking:
		if (!_waiting)
		{
			_waiting = true;
			QTimer::singleShot(DELAY_TIME, this, [this, &cm]()
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
			QTimer::singleShot(DELAY_TIME_CLICK, this, [this, &cm]()
			{
				while (cm.isHenseiDone(_ships, _nowIndex))
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
			QTimer::singleShot(DELAY_TIME, this, [this, &cm]()
			{
				if (cm.checkColors(
					34, 144, 238, 213, 54
					, 770, 113, 44, 144, 144))
				{
					_waiting = false;
					setState(State::FindShipDone, "Hensei:FindShipDone");
				}
				else
				{
					tryRetry();
				}
			});
		}
		break;
	case ChangeHenseiAction::State::FindShipDone:
		if (!_waiting)
		{
			_waiting = true;
			QTimer::singleShot(DELAY_TIME_CLICK, this, [this, &cm]()
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
			QTimer::singleShot(DELAY_TIME, this, [this, &cm]()
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
				cm.moveMouseToAndClick(439, 169 + _cellHeight*_posList[_nowIndex]);
				setState(State::FindShipOKChecking, "Hensei:FindShipOKChecking");
				resetRetryAndWainting();
			}
			else
			{
				QTimer::singleShot(DELAY_TIME_CLICK, this, [this, &cm]()
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
			QTimer::singleShot(DELAY_TIME_PAGE, this, [this, &cm]()
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
				cm.moveMouseToAndClick(439, 169 + _cellHeight*_posList[_nowIndex]);
				setState(State::FindShipOKChecking, "Hensei:FindShipOKChecking");
				resetRetryAndWainting();
			}
			else
			{
				QTimer::singleShot(DELAY_TIME_CLICK, this, [this, &cm]()
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
			QTimer::singleShot(DELAY_TIME, this, [this, &cm]()
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
			QTimer::singleShot(DELAY_TIME_CLICK, this, [this, &cm]()
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
					QTimer::singleShot(DELAY_TIME_SUPERLONG, this, [this, &cm]()
					{
						if (cm.isHenseiDone(_ships, _nowIndex))
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
			QTimer::singleShot(DELAY_TIME, this, [this, &cm]()
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
			QTimer::singleShot(DELAY_TIME_CLICK, this, [this, &cm]()
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
			setState(State::Done, "Hensei:Done");
		}
		break;
	case ChangeHenseiAction::State::Done:
		if (cm.isHenseiDone(_ships))
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
		if (!cm.needChargeFlagship())
		{
			setState(State::Done, "Charge:Done");
		}
		else
		{
			setState(State::HomePortChecking, "Charge:HomePortChecking");
		}
		resetRetryAndWainting();
		break;
	case ChargeAction::State::HomePortChecking:
		if (!_waiting)
		{
			_waiting = true;
			QTimer::singleShot(DELAY_TIME, this, [this, &cm]()
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
			QTimer::singleShot(DELAY_TIME_CLICK, this, [this, &cm]()
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
			QTimer::singleShot(DELAY_TIME, this, [this, &cm]()
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
			QTimer::singleShot(DELAY_TIME_CLICK, this, [this, &cm]()
			{
				if (cm.isSouthEastMode())
				{
					cm.moveMouseToAndClick(117, 120, 2, 2); // all ships
				}
				else
				{
					cm.moveMouseToAndClick(117, 167, 2, 2); // first ship
				}
				setState(State::OKToChargeChecking, "Charge:OKToChargeChecking");
				resetRetryAndWainting();
			});
		}
		break;
	case ChargeAction::State::OKToChargeChecking:
		if (!_waiting)
		{
			_waiting = true;
			QTimer::singleShot(DELAY_TIME, this, [this, &cm]()
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
			QTimer::singleShot(DELAY_TIME_CLICK, this, [this, &cm]()
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
			QTimer::singleShot(DELAY_TIME, this, [this, &cm]()
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
			QTimer::singleShot(DELAY_TIME_CLICK, this, [this, &cm]()
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
			if (cm.needChargeAnyShip() || cm.hugestDamageInTeam() >= WoundState::Middle)
			{
				cm.setToTerminate("Terminated:Damage");
				emit sigFatal();
				return false;
			}
		}
		else
		{
			if (cm.needChargeFlagship() || cm.hugestDamageInTeam() >= WoundState::Big)
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
			QTimer::singleShot(DELAY_TIME, this, [this, &cm]()
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
			QTimer::singleShot(DELAY_TIME_CLICK, this, [this, &cm]()
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
			QTimer::singleShot(DELAY_TIME, this, [this, &cm]()
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
			QTimer::singleShot(DELAY_TIME_CLICK, this, [this, &cm]()
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
			QTimer::singleShot(DELAY_TIME, this, [this, &cm]()
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
				QTimer::singleShot(DELAY_TIME_CLICK, this, [this, &cm]()
				{
					cm.moveMouseToAndClick(235, 445, 14, 9); // area 2
					setState(State::SelectMapChecking, "Sortie:SelectMapChecking");
					resetRetryAndWainting();
				});
			}
			// fule mode select 2
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
			QTimer::singleShot(DELAY_TIME, this, [this, &cm]()
			{
				if (cm.checkColors(
					246, 326, 197, 106, 166
					, 354, 370, 244, 206, 94))
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
				QTimer::singleShot(DELAY_TIME_CLICK, this, [this, &cm]()
				{
					cm.moveMouseToAndClick(288.5f, 350, 102.5f, 36); // map 3
					setState(State::SortieCheckChecking, "Sortie:SortieCheckChecking");
					resetRetryAndWainting();
				});
			}
			// fule mode select 3
		}
		else if (cm.isKiraMode())
		{
			if (!_waiting)
			{
				_waiting = true;
				QTimer::singleShot(DELAY_TIME_CLICK, this, [this, &cm]()
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
			QTimer::singleShot(DELAY_TIME, this, [this, &cm]()
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
			QTimer::singleShot(DELAY_TIME_CLICK, this, [this, &cm]()
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
			QTimer::singleShot(DELAY_TIME, this, [this, &cm]()
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
			QTimer::singleShot(DELAY_TIME_CLICK, this, [this, &cm]()
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
			QTimer::singleShot(DELAY_TIME_LONG, this, [this, &cm]()
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
			QTimer::singleShot(_shouldRetrieve ? DELAY_TIME_SUPERLONG : DELAY_TIME_LONG, this, [this, &cm]()
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
			QTimer::singleShot(DELAY_TIME_CLICK, this, [this, &cm]()
			{
				cm.moveMouseToAndClick(445, 184); // formation button
				setStateToChecking();
				resetRetryAndWainting();
			});
		}
		break;
	case SortieCommonAdvanceAction::State::ClickElse:

		if (!_waiting)
		{
			_waiting = true;
			QTimer::singleShot(DELAY_TIME_CLICK, this, [this, &cm]()
			{
				cm.moveMouseToAndClick(729, 416, 15, 15); // formation button
				setStateToChecking();
				resetRetryAndWainting();
			});
		}
		break;
	case SortieCommonAdvanceAction::State::ClickLeft:

		if (!_waiting)
		{
			_waiting = true;
			QTimer::singleShot(DELAY_TIME_CLICK, this, [this, &cm]()
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
			QTimer::singleShot(DELAY_TIME_CLICK, this, [this, &cm]()
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

