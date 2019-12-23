#pragma once
#include <QList>
#include <QElapsedTimer>
#include <QTimer>
#include <QObject>
#include <QMap>

class ControlManager;
class ControlAction : public QObject
{
	Q_OBJECT
public:
	ControlAction(QObject* parent = NULL);
	virtual ~ControlAction()
	{
	};

	virtual bool action() { return true; }

	void setDoneRequest(const QString& api);

signals:
	void sigCheckFail();
	void sigFatal();

protected:
	void resetRetryAndWainting()
	{
		_retryTime = 0;
		_waiting = false;
	}
	void tryRetry();

	int _retryTime = 0;
	bool _waiting = false;
	QString _expectingRequest;

	double _intervalMul = 1.0f;
};

class WaitCondAction : public ControlAction
{
public:
	enum class State
	{
		None,
		Waiting,

		HomePortChecking,
		HomePortDone, // click change
		FakeHenseiChecking,
		FakeHenseiDone, // click port

		ExpectingPort,
		Done,
	};
	WaitCondAction(QObject* parent = NULL)
		:ControlAction(parent)
	{
	}

	virtual bool action() override;

public:
	void setWaitMS(qint64 waitms, bool isExpedition);
	void setWaitName(const QString& name);

private:
	qint64 _waitMSTo = 0;
	qint64 _waitMS = 1;
	QString _waitName = "Wait:Waiting";

	State _state = State::None;
	void setState(State state, const char* str);

};

class WaitNextPortAction : public ControlAction
{
public:
	enum class State
	{
		None,

		ExpectingPort,
		Done,
	};
	WaitNextPortAction(QObject* parent = NULL)
		:ControlAction(parent)
	{
	}

	virtual bool action() override;

public:
	void setMaxWaitMS(qint64 waitms);
	qint64 _maxWaitMS = 30 * 1000;

private:
	State _state = State::None;
	void setState(State state, const char* str);

};

class ChangeHenseiAction : public ControlAction
{
public:
	enum class State
	{
		None,
		HomePortChecking,
		HomePortDone, // click change

		ChangeFleetChecking,
		ChangeFleetDone,

		HenseiChecking,
		HenseiDone, // click change one / skip / remove all

		RemoveAllOtherChecking,
		RemoveAllOtherDone,

		FindShipChecking,	// cur page
		FindShipChangeSort,	// sort change
		FindShipChangeFilter,	// filter change
		FindShipDone,		// click leftmost
		FindShipFirstPageChecking,
		FindShipFirstPageDone,
		FindShipNextPageChecking,
		FindShipNextPageDone, // skip to
		FindShipOKChecking,
		FindShipOKDone, //exchange set flag

		ReturnToPortChecking,
		ReturnToPortDone,
		ExpectingPort,
		Done,
	};
	ChangeHenseiAction(QObject* parent = NULL)
		:ControlAction(parent)
	{
	}

	virtual bool action() override;

	void setShips(int ship0, int ship1);
	void setShips(const QList<int>& ships);
	void setTeam(int team);
	void resetCurPage()
	{
		_curPage = 0;
	}


public:
	QList<int> _ships;
	QList<int> _pageList;
	QList<int> _posList;
	int _lastPage = 0;

	int _curPage = 0;
	int _nowIndex = 0;

	int _team = 0;

private:
	State _state = State::None;
	void setState(State state, const char* str);
};

class ChargeAction : public ControlAction
{
public:
	enum class State
	{
		None,
		Skipping,
		HomePortChecking,
		HomePortDone, // click charge
		NeedChargeChecking,
		NeedChargeDone, // click first one
		OKToChargeChecking,
		OKToChargeDone, // click charge
		FinishedChargeChecking,
		FinishedChargeDone, // click back
		ExpectingPort,
		Done,
	};

	ChargeAction(QObject* parent = NULL)
		:ControlAction(parent) {}	// only charge flagship of team1

	virtual bool action() override;

public:
	void setTeam(int team);
	int _team = 0;

	void setSkipExpedition(bool skip);
	bool _skipExpedition = false;

private:
	bool _teamChanged = false;
	bool _forceTeamChange = false;

private:
	State _state = State::None;
	void setState(State state, const char* str);
};

class ExpeditionAction : public ControlAction
{
public:
	ExpeditionAction(QObject* parent = NULL)
		:ControlAction(parent) {}

	enum class State
	{
		None,
		HomePortChecking,
		HomePortDone, // click sortie
		SortieSelectChecking,
		SortieSelectDone, // click sortie
		SelectAreaChecking,
		SelectAreaDone, // click area
		SelectItemChecking,
		SelectItemDone, // click item
		SortieCheckChecking,
		SortieCheckDone, // click ok
		TeamSelectChecking,
		TeamSelectDone, // click team
		TeamSelectedChecking,
		TeamSelectedDone, // click go
		ExpectingExpedition, // wait for http	click port
		Skipping,	// click to skip expedition
		ExpectingPort,
		Done,
	};

public:
	void setTeamAndTarget(int team, int area, int item);
	int _team = 2;
	int _area = 0;
	int _item = 1;

	virtual bool action() override;

private:
	State _state = State::None;
	void setState(State state, const char* str);
};

class DestroyShipAction : public ControlAction
{
public:
	DestroyShipAction(QObject* parent = NULL)
		:ControlAction(parent) {}

	enum class State
	{
		None,
		HomePortChecking,
		HomePortDone, // click koushou
		KouShouSelectChecking,
		KouShouSelectDone, // click destroy
		SelectDestroyChecking,
		SelectDestoryDone,
		FindShipChecking,	// cur page
		FindShipChangeSort,	// sort change
		FindShipDone,		// click ship
		FindShipFirstPageChecking,
		FindShipFirstPageDone,
		FindShipNextPageChecking,
		FindShipNextPageDone, // skip to
		FindShipOKChecking,
		FindShipOKDone, //destroy
		Skipping,	// click to skip
		ReturnToPortChecking,
		ReturnToPortDone,
		ExpectingPort,
		Done,
	};
	void setShips(const QList<int>& ships);
	void resetCurPage()
	{
		_curPage = 0;
	}

	virtual bool action() override;


public:
	QList<int> _ships;
	int _page;
	QList<int> _posList;
	int _lastPage = 0;

	int _curPage = 0;
	int _nowIndex = 0;
	int _nowEndIndex = 0;

	int _nowSelectingPosIndex = 0;

private:
	State _state = State::None;
	void setState(State state, const char* str);
};

class RepairShipAction : public ControlAction
{
public:
	RepairShipAction(QObject* parent = NULL)
		:ControlAction(parent) {}

	enum class State
	{
		None,
		HomePortChecking,
		HomePortDone, // click koushou
		NyuKyoSelectChecking,
		NyuKyoSelectDone, // click ndock
		SelectSlotChecking,
		SelectSlotDone,
		FindShipChecking,	// cur page
		FindShipChangeSort,	// sort change
		FindShipDone,		// click ship
		FindShipFirstPageChecking,
		FindShipFirstPageDone,
		FindShipNextPageChecking,
		FindShipNextPageDone, // skip to
		FindShipOKChecking,
		FindShipOKDone, // ok
		NyuKyoFastToggleChecking,
		NyuKyoFastTogglDone,
		NyuKyoOKChecking,
		NyuKyoOKDone,
		Skipping,	// click to skip
		ReturnToPortChecking,
		ReturnToPortDone,
		ExpectingPort,
		Done,
	};
	void setShips(const QList<int>& ships, const QList<int>& normalSlots, bool useFastRepair);
	void resetCurPage()
	{
		_curPage = 0;
	}

	virtual bool action() override;


public:
	QList<int> _ships;
	QList<int> _usingSlots;

	QList<int> _pageList;
	QList<int> _posList;
	int _lastPage = 0;

	int _curPage = 0;
	int _nowIndex = 0;
	int _cellHeight = 31;

	int _usingSlot = -1;

	bool _useFastRepair = false;

private:
	State _state = State::None;
	void setState(State state, const char* str);
};

class DevelopAction : public ControlAction
{
public:

	DevelopAction(QObject* parent = NULL)
		:ControlAction(parent) {}

	enum class State
	{
		None,
		SelectDevelopChecking,
		SelectDevelopDone,
		SelectOKChecking,
		SelectOKDone,
		Skipping,
		ReturnToPortChecking,
		ReturnToPortDone,
		ExpectingPort,
		Done,
	};
	void addItem(int itemId, int buildCount);

	virtual bool action() override;


public:
	QMap<int, int> _toBuildSlotItems;

private:
	State _state = State::None;
	void setState(State state, const char* str);
};

class SortieAction : public ControlAction
{
public:
	struct MapExClick
	{
		QList<float> checkList;
		QList<float> clickPoint;
	};

	enum class State
	{
		None,
		HomePortChecking,
		HomePortDone, // click sortie
		SortieSelectChecking,
		SortieSelectDone, // click sortie
		SelectAreaChecking,
		SelectAreaDone, // click area
		SelectMapChecking,
		SelectMapDone, // click map or ex
		SelectExMapChecking,
		SelectExMapDone, // click ex map

		SelectEx2MapChecking,
		SelectEx2MapDone, // click ex map
		SelectEx3MapChecking,
		SelectEx3MapDone, // click ex map
		SelectEx4MapChecking,
		SelectEx4MapDone, // click ex map

		LDChargeChecking,
		LDChargeDone,   // click charge button
		LDSelectTeamChecking,
		LDSelectTeamDone,   // click team
		LDDoChargeChecking,
		LDDoChargeDone,
		LDAllChargeDone,    // click somewhere

		SkipBoardChecking,
		SkipBoardDone,	// skip mission (ev only)


		SortieCheckChecking,
		SortieCheckDone, // click ok
		TeamSelectChecking,

		TeamSelectSelectTeam, // click other team

		TeamSelectDone, // click go

		ExpectingMapStart, // wait for http	
		Done,
	};
	SortieAction(QObject* parent = NULL)
		:ControlAction(parent)
	{
	}

public:
	void setAreaAndMap(int area, int map
		, const QList<float>& areaCheckList = QList<float>()
		, const QList<float>& mapClickPoint = QList<float>()
		, const QMap<int, MapExClick>& mapExClickList = QMap<int, MapExClick>());

	void setTeam(int team) { _team = team; }

	void setShouldPauseNext(bool bPause);

	int _area = 1;
	int _map = 1;

	int _team = 1;

	bool _isEvent = false;

	bool _autoPauseNext = false;

	QList<float> _areaCheckList;
	QList<float> _mapClickPoint;	// click E1~E3 or Ex button
	QMap<int, MapExClick> _mapExClickList;

	virtual bool action() override;

private:
	State _state = State::None;
	void setState(State state, const char* str);
};

class SortieAdvanceAction : public ControlAction
{
public:
	enum class State
	{
		None,
		ExpectingPort, // wait for http	
		Done,
	};
	SortieAdvanceAction(QObject* parent = NULL)
		:ControlAction(parent)
	{
	}

	virtual bool action() override;

private:
	State _state = State::None;
	int _clickCount = 0;
	void setState(State state, const char* str);
};

class SortieCommonAdvanceAction : public ControlAction
{
public:
	enum class State
	{
		None,
		Checking,

		LDChecking,
		LDDone,

		ClickElse,
		SelectFormation,
		SelectCombinedFormation,
		ClickLeft,
		ClickRight,
		ClickAirBaseOK,
		ClickAirBaseROK,

		Done,
	};
	SortieCommonAdvanceAction(QObject* parent = NULL)
		:ControlAction(parent)
	{
	}

	virtual bool action() override;

private:
	State _state = State::None;
	void setState(State state, const char* str);

	void setStateToChecking();
	bool _shouldRetrieve = false;

	int _totalFormationCount = 0;

	int _ldCheckingIndex = -1;

	bool _withKeiKaiJin = false;
};

class MissionAction : public ControlAction
{
public:
	enum class State
	{
		None,
		HomePortChecking,
		HomePortDone, // click sortie
		MissionSelectChecking,
		MissionSelectDone, // click sortie

		MissionSkipping, // ooyodo

		CheckingMissionState,

		SelectNextPage,
		SelectFirstPage,
		AcceptMission,
		CompleteMission,

		CompleteSkip,

		ReturnToPortChecking,
		ReturnToPortDone,
		ExpectingPort,
		Done,

	};

	MissionAction(QObject* parent = NULL)
		:ControlAction(parent)
	{
	}

	virtual bool action() override;

private:
	State _state = State::None;
	void setState(State state, const char* str);

	bool _isCompleting = true;
	int _targetIndex = -1;
	bool _isSkippingMissionComplete = false;

public:
	static const int maxMissionAcceptCount = 7;
};

class RepeatAction : public ControlAction
{
public:
	enum class State
	{
		None,
		Done,
	};
	RepeatAction(QObject* parent = NULL)
		:ControlAction(parent)
	{
	}

	virtual bool action() override;

private:
	State _state = State::None;
	void setState(State state, const char* str);
};
