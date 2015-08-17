#pragma once
#include <QPoint>
#include <QList>
#include "ControlAction.h"

class ControlManager
{
public:
	enum class State
	{
		None,
		Ready,
		Started,
		Paused,
		Terminated,
	};

public:
	static ControlManager& getInstance() { static ControlManager instance; return instance; }
	
private:
	ControlManager();
	virtual ~ControlManager();
	ControlManager(ControlManager const&);
	void operator=(ControlManager const&);
	
public:
	void LoadToDoShipList();

	void BuildNext();
	void Run();
	void Pause();
	void Resume();
	void Terminate();

	int getCurrentShipId();
	bool shouldChangeSecondShip();
	int getOneWasteShipId();
	bool isShipFull();

private:
	void moveMouseTo(const QPoint& pt);
	void mouseClick();
	void waitForResponse(const QString& api);

	QList<int> _todoShipids;
	QList<ControlAction> _actionList;

	State state = State::None;
}; 

