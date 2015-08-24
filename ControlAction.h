#pragma once
#include <QList>
#include <QElapsedTimer>
#include <QTimer>
#include <QObject>

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

class ChangeHenseiAction : public ControlAction
{
public:
	ChangeHenseiAction(const QList<int>){}
	ChangeHenseiAction(int shipid, int wasteShipid = -1){ Q_UNUSED(shipid); Q_UNUSED(wasteShipid); }

	virtual bool action() override;

public:
	QList<int> _shipList;
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
};

