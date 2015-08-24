#include "ControlAction.h"
#include "ControlManager.h"

#define DELAY_TIME			250
#define DELAY_TIME_CLICK	250
#define DELAY_TIME_LONG	500

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
		ControlManager::getInstance()->setToTerminate();
		emit sigCheckFail();
	}
}

bool ChangeHenseiAction::action()
{
	if (ControlManager::getInstance()->isFuelMode())
	{
		return true;
	}
	return true;
}

bool ChargeAction::action()
{
	auto cm = ControlManager::getInstance();
	switch (_state)
	{
	case ChargeAction::State::None:

		cm->moveMouseTo(400, 240);
		if (!cm->needChargeFlagship())
		{
			_state = State::Done;
		}
		else
		{
			_state = State::HomePortChecking;
		}
		resetRetryAndWainting();
		break;
	case ChargeAction::State::HomePortChecking:
		if (!_waiting)
		{
			_waiting = true;
			QTimer::singleShot(DELAY_TIME, this, [this, cm]()
			{
				// home port
				if (cm->checkColors(
					213, 247, 251, 155, 32
					, 231, 114, 224, 201, 66))
				{
					_waiting = false;
					this->_state = State::HomePortDone;
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
			QTimer::singleShot(DELAY_TIME_CLICK, this, [this, cm]()
			{
				cm->moveMouseToAndClick(75, 212); // charge button
				_state = State::NeedChargeChecking;
				resetRetryAndWainting();
			});
		}
		break;
	case ChargeAction::State::NeedChargeChecking:
		if (!_waiting)
		{
			_waiting = true;
			QTimer::singleShot(DELAY_TIME, this, [this, cm]()
			{
				if (cm->checkColors(
					516, 167, 230, 215, 201
					, 625, 254, 113, 110, 74))
				{
					_waiting = false;
					this->_state = State::NeedChargeDone;
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
			QTimer::singleShot(DELAY_TIME_CLICK, this, [this, cm]()
			{
				cm->moveMouseToAndClick(120, 167); // first ship
				_state = State::OKToChargeChecking;
				resetRetryAndWainting();
			});
		}
		break;
	case ChargeAction::State::OKToChargeChecking:
		if (!_waiting)
		{
			_waiting = true;
			QTimer::singleShot(DELAY_TIME, this, [this, cm]()
			{
				if (cm->checkColors(
					705, 431, 94, 191, 203
					, 625, 254, 113, 110, 74))
				{
					_waiting = false;
					this->_state = State::OKToChargeDone;
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
			QTimer::singleShot(DELAY_TIME_CLICK, this, [this, cm]()
			{
				cm->moveMouseToAndClick(705, 431); // charge button
				_state = State::FinishedChargeChecking;
				resetRetryAndWainting();
			});
		}
		break;
	case ChargeAction::State::FinishedChargeChecking:
		if (!_waiting)
		{
			_waiting = true;
			QTimer::singleShot(DELAY_TIME, this, [this, cm]()
			{
				if (cm->checkColors(
					516, 164, 54, 255, 0
					, 625, 254, 113, 110, 74))
				{
					_waiting = false;
					this->_state = State::FinishedChargeDone;
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
			QTimer::singleShot(DELAY_TIME_CLICK, this, [this, cm]()
			{
				cm->moveMouseToAndClick(75, 261); // home button
				_state = State::ExpectingPort;
				resetRetryAndWainting();
			});
		}
		break;
	case ChargeAction::State::ExpectingPort:
		if (_expectingRequest == "")
		{
			_state = State::Done;
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

bool DestroyShipAction::action()
{
	return true;
}

bool SortieAction::action()
{
	auto cm = ControlManager::getInstance();
	switch (_state)
	{
	case SortieAction::State::None:

		if (cm->needChargeFlagship() || cm->flagshipSevereDamaged())
		{
			cm->setToTerminate();
			emit sigFatal();
			return false;
		}

		cm->moveMouseTo(400, 240);		
		_state = State::HomePortChecking;
		resetRetryAndWainting();

		break;
	case SortieAction::State::HomePortChecking:
		if (!_waiting)
		{
			_waiting = true;
			QTimer::singleShot(DELAY_TIME, this, [this, cm]()
			{
				// home port
				if (cm->checkColors(
					213, 247, 251, 155, 32
					, 231, 114, 224, 201, 66))
				{
					_waiting = false;
					this->_state = State::HomePortDone;
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
			QTimer::singleShot(DELAY_TIME_CLICK, this, [this, cm]()
			{
				cm->moveMouseToAndClick(213, 247); // sortie button
				_state = State::SortieSelectChecking;
				resetRetryAndWainting();
			});
		}
		break;
	case SortieAction::State::SortieSelectChecking:
		if (!_waiting)
		{
			_waiting = true;
			QTimer::singleShot(DELAY_TIME, this, [this, cm]()
			{
				// sortie select
				if (cm->checkColors(
					468, 224, 115, 182, 85
					, 625, 156, 88, 194, 179))
				{
					_waiting = false;
					this->_state = State::SortieSelectDone;
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
			QTimer::singleShot(DELAY_TIME_CLICK, this, [this, cm]()
			{
				cm->moveMouseToAndClick(213, 247); // sortie button
				_state = State::SelectAreaChecking;
				resetRetryAndWainting();
			});
		}
		break;
	case SortieAction::State::SelectAreaChecking:
		if (!_waiting)
		{
			_waiting = true;
			QTimer::singleShot(DELAY_TIME, this, [this, cm]()
			{
				// area 1 sortie map
				if (cm->checkColors(
					562, 119, 255, 184, 103
					, 644, 246, 137, 143, 154))
//					, 648, 118, 118, 180, 72))
				{
					_waiting = false;
					this->_state = State::SelectAreaDone;
				}
				else
				{
					tryRetry();
				}
			});
		}
		break;
	case SortieAction::State::SelectAreaDone:
		if (cm->isFuelMode())
		{
			if (!_waiting)
			{
				_waiting = true;
				QTimer::singleShot(DELAY_TIME_CLICK, this, [this, cm]()
				{
					cm->moveMouseToAndClick(234, 443); // area 2
					_state = State::SelectMapChecking;
					resetRetryAndWainting();
				});
			}
			// fule mode select 2
		}
		else if (cm->isKiraMode())
		{
			// skip to select map
			_state = State::SelectMapDone;
		}
		break;
	case SortieAction::State::SelectMapChecking:
		// only fuel mode
		if (!_waiting)
		{
			_waiting = true;
			QTimer::singleShot(DELAY_TIME, this, [this, cm]()
			{
				if (cm->checkColors(
					246, 326, 197, 106, 166
					, 354, 370, 244, 206, 94))
				{
					_waiting = false;
					this->_state = State::SelectMapDone;
				}
				else
				{
					tryRetry();
				}
			});
		}
		break;
	case SortieAction::State::SelectMapDone:
		if (cm->isFuelMode())
		{
			if (!_waiting)
			{
				_waiting = true;
				QTimer::singleShot(DELAY_TIME_CLICK, this, [this, cm]()
				{
					cm->moveMouseToAndClick(291, 350); // map 3
					_state = State::SortieCheckChecking;
					resetRetryAndWainting();
				});
			}
			// fule mode select 3
		}
		else if (cm->isKiraMode())
		{
			if (!_waiting)
			{
				_waiting = true;
				QTimer::singleShot(DELAY_TIME_CLICK, this, [this, cm]()
				{
					cm->moveMouseToAndClick(285, 185); // map 1
					_state = State::SortieCheckChecking;
					resetRetryAndWainting();
				});
			}
		}
		break;
	case SortieAction::State::SortieCheckChecking:
		if (!_waiting)
		{
			_waiting = true;
			QTimer::singleShot(DELAY_TIME, this, [this, cm]()
			{
				// sortie ok button
				if (cm->checkColors(
					722, 440, 19, 86, 87
					, 82, 230, 218, 196, 196))
				{
					_waiting = false;
					this->_state = State::SortieCheckDone;
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
			QTimer::singleShot(DELAY_TIME_CLICK, this, [this, cm]()
			{
				cm->moveMouseToAndClick(722, 440); // ok
				_state = State::TeamSelectChecking;
				resetRetryAndWainting();
			});
		}
		break;
	case SortieAction::State::TeamSelectChecking:
		if (!_waiting)
		{
			_waiting = true;
			QTimer::singleShot(DELAY_TIME, this, [this, cm]()
			{
				// sortie ok button
				if (cm->checkColors(
					610, 125, 61, 61, 61
					, 551, 237, 255, 246, 242))
				{
					_waiting = false;
					this->_state = State::TeamSelectDone;
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
			QTimer::singleShot(DELAY_TIME_CLICK, this, [this, cm]()
			{
				cm->moveMouseToAndClick(655, 453); // ok
				_state = State::ExpectingMapStart;
				resetRetryAndWainting();
			});
		}
		break;
	case SortieAction::State::ExpectingMapStart:
		if (_expectingRequest == "")
		{
			_state = State::Done;
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

bool SortieAdvanceAction::action()
{
	auto cm = ControlManager::getInstance();
	switch (_state)
	{
	case SortieAdvanceAction::State::None:
		_expectingRequest = "/kcsapi/api_port/port";
		_state = State::ExpectingPort;
		break;
	case SortieAdvanceAction::State::ExpectingPort:
		if (!_waiting)
		{
			_waiting = true;
			QTimer::singleShot(DELAY_TIME_LONG, this, [this, cm]()
			{
				if (_expectingRequest == "")
				{
					_state = State::Done;
				}
				else
				{
					cm->moveMouseToAndClick(295, 238); // ok
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

bool RepeatAction::action()
{
	auto cm = ControlManager::getInstance();
	switch (_state)
	{
	case RepeatAction::State::None:
		if (cm->isFuelMode())
		{
			if (cm->BuildNext_Fuel())
			{
				cm->StartJob();
			}
			_state = State::Done;
		}
		else if (cm->isKiraMode())
		{
			if (cm->BuildNext_Kira())
			{
				cm->StartJob();
			}
			_state = State::Done;
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
