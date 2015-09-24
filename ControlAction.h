#pragma once
#include <QList>
#include <QElapsedTimer>
#include <QTimer>
#include <QObject>

class ControlManager;
class ControlAction : public QObject
{
	Q_OBJECT
public:
	ControlAction(QObject* parent = NULL)
		:QObject(parent)
	{

	}
	virtual ~ControlAction()
	{
	};

	virtual bool action(){ return true; }

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

		HenseiChecking,
		HenseiDone, // click change one / skip

		FindShipChecking,	// cur page
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
	int _cellHeight = 28;

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
		:ControlAction(parent){}	// only charge flagship of team1

	virtual bool action() override;

private:
	State _state = State::None;
	void setState(State state, const char* str);
};

class DestroyShipAction : public ControlAction
{
public:
	DestroyShipAction(QObject* parent = NULL)
		:ControlAction(parent){}
	
	virtual bool action() override;
};

class SortieAction : public ControlAction
{
public:
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
		SelectMapDone, // click map
		SortieCheckChecking,
		SortieCheckDone, // click ok
		TeamSelectChecking,
		TeamSelectDone, // click go
		ExpectingMapStart, // wait for http	
		Done,
	};
	SortieAction(QObject* parent = NULL)
		:ControlAction(parent)
	{
	}

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
	void setState(State state, const char* str);
};

class SortieCommonAdvanceAction : public ControlAction
{
public:
	enum class State
	{
		None,
		Checking,

		ClickElse,
		SelectFormation,
		ClickLeft,
		ClickRight,

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

