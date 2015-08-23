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

	enum class SortieTarget
	{
		None,
		Kira,
		Fuel,
	};

public:
	static ControlManager* getInstance() { static ControlManager instance; return &instance; }
	
private:
	ControlManager();
	virtual ~ControlManager();
	ControlManager(ControlManager const&);
	void operator=(ControlManager const&);
	
public:
	void LoadToDoShipList_Kira();
	void BuildNext_Kira();

	void BuildNext_Fuel();

	void Run();
	void Pause();
	void Resume();
	void Terminate();

	int getCurrentShipId();
	bool shouldChangeSecondShip();
	int getOneWasteShipId();
	bool isShipFull();

//private:
	void moveMouseToAndClick(const QPoint& pt);
	void waitForResponse(const QString& api);

	QList<int> _todoShipids;
	QList<ControlAction> _actionList;
	
	State state = State::None;
}; 

