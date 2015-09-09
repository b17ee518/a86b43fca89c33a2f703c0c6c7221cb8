#pragma once
#include <QPoint>
#include <QList>
#include "ControlAction.h"
#include "kansavedata.h"

class ControlManager
{
public:
	enum class State
	{
		None,
		Ready,
		Started,
		Paused,
		ToTerminate,
		Terminated,
	};

	enum class SortieTarget
	{
		None,
		Kira,
		Fuel,
	};

	class CheckColor
	{
	public:
		CheckColor(int x, int y, int r, int g, int b)
		{
			_pt = QPoint(x, y);
			_r = r;
			_g = g;
			_b = b;
		}
		QPoint _pt;
		int _r;
		int _g;
		int _b;
	};

public:
	static ControlManager& getInstance() { 
		static ControlManager instance; 
		return instance; 
	}
	
private:
	ControlManager();
	virtual ~ControlManager();
	ControlManager(ControlManager const&);
	void operator=(ControlManager const&);
	
public:
	void setDoneRequest(const QString& api);

	bool LoadToDoShipList_Kira();
	bool BuildNext_Kira();

	bool BuildNext_Fuel();
	
	void StartJob();

	void Run();
	void Pause();
	void Resume();
	void Terminate();

	bool isPaused();
	void PauseNext();

	void setToTerminate();

	int getCurrentFlagshipId();
	int getCurrentSecondshipId();
	bool shouldChangeSecondShip();
	bool needChargeFlagship();
	int getOneWasteShipId();
	bool isShipFull();
	bool findPagePosByShipId(int shipno, int& page, int& pos, int& lastPage);
	bool isShipKiraDone(int shipno);
	bool isShipInOtherTeam(int shipno);
	bool isShipInDock(int shipno);

	bool isHenseiDone(const QList<int>& ships, int index = -1);
	bool isFlagshipOnly();
	bool isShipType(int shipno, ShipType stype);
	bool hasSlotitem(int shipno, SlotitemType sitype);
	bool noSlotitem(int shipno);
			
	bool flagshipSevereDamaged();

	bool isRunning(){ return _state == State::Started; }

	bool checkColors(const QList<CheckColor>& checklist);

	bool checkColors(int x, int y, int r, int g, int b)
	{
		QList<CheckColor> lst;
		lst.push_back(CheckColor(x, y, r, g, b));
		return checkColors(lst);
	}
	bool checkColors(int x1, int y1, int r1, int g1, int b1
		, int x2, int y2, int r2, int g2, int b2)
	{
		QList<CheckColor> lst;
		lst.push_back(CheckColor(x1, y1, r1, g1, b1));
		lst.push_back(CheckColor(x2, y2, r2, g2, b2));
		return checkColors(lst);
	}
	bool checkColors(int x1, int y1, int r1, int g1, int b1
		, int x2, int y2, int r2, int g2, int b2
		, int x3, int y3, int r3, int g3, int b3)
	{
		QList<CheckColor> lst;
		lst.push_back(CheckColor(x1, y1, r1, g1, b1));
		lst.push_back(CheckColor(x2, y2, r2, g2, b2));
		lst.push_back(CheckColor(x3, y3, r3, g3, b3));
		return checkColors(lst);
	}

	inline bool isKiraMode(){ return _target == SortieTarget::Kira; }
	inline bool isFuelMode(){ return _target == SortieTarget::Fuel; }

	void setState(State state, const char* str);

	void setStateStr(const QString& str);
	inline const QString& getStateStr(){ return _stateStr; }

//private:
	void moveMouseToAndClick(float x, float y, float offsetX = 5, float offsetY = 3);
	void moveMouseTo(float x, float y, float offsetX = 5, float offsetY = 3);
	
	void setPauseNextVal(bool bVal);
	inline bool getPauseNextVal(){ return _pauseNext; }

	void waitForResponse(const QString& api);
	
	qreal randVal(qreal min, qreal max);

	QList<int> _todoShipids;
	int _toCond = 70;
	QList<ControlAction*> _actionList;
	
	State _state = State::None;
	SortieTarget _target = SortieTarget::None;

	QString _stateStr;
	bool _pauseNext = false;
}; 

