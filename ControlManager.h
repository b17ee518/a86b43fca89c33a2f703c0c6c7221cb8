#pragma once
#include <QPoint>
#include <QList>
#include "ControlAction.h"
#include "kansavedata.h"
#include "kandatacalc.h"

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
		SouthEast,
		Expedition,
		Level,
	};

	enum class StopWhen
	{
		None,
		Yusou3,
		SouthEast5,
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
	bool BuildNext_Kira(bool bForceCurrent=false);

	bool BuildNext_Fuel();

	bool BuildNext_SouthEast();

	bool BuildNext_Level();

	bool LoadExpeditionPossibleList(); // TODO
	bool BuildNext_Expedition();

	bool stopWhenCheck();
	
	void StartJob();

	void Run();
	void Pause();
	void Resume();
	void Terminate();

	bool isPaused();
	void PauseNext();
	void togglePauseNext();

	bool isTerminated(){ return _state == State::Terminated; }

	void setToTerminate(const char* title);

	void createSSShipList();
	bool isTreatedSameShip(int shipno, int oshipno);
	bool isAfterShip(const kcsapi_mst_ship* pmstship, const kcsapi_mst_ship* pomstship, QList<int> checkedIdList=QList<int>()/*for convert*/); 
	// is pmstship after of pomstship

	int getCurrentFlagshipId();
	int getCurrentSecondshipId();
	bool shouldChangeSecondShip();
	bool needChargeFlagship(int team);
	bool needChargeAnyShip(int team);
	int getOneWasteShipId(int exclude1=-1, int exclude2=-1);
	int getOneWasteShipId(const QList<int>& excludes);
	bool isShipFull();
	bool findPagePosByShipId(int shipno, int& page, int& pos, int& lastPage);
	bool isShipKiraDone(int shipno);
	bool isShipInOtherTeam(int shipno, int team);
	bool isShipInDock(int shipno);
	bool isShipDamaged(int shipno);
	bool isShipCharged(int shipno);

	bool isHenseiDone(const QList<int>& ships, int team, int index = -1);
	bool isFlagshipOnly();
	int getTeamSize();
	bool isShipType(int shipno, ShipType stype);
	bool hasSlotitem(int shipno, SlotitemType sitype);
	bool noSlotitem(int shipno);

	int getShipCondVal(int shipno);
			
	bool flagshipSevereDamaged();

	bool shouldNightBattle();
	bool shouldRetrieve();
	WoundState hugestDamageInTeam(int team);

	void setStopWhen(StopWhen stopwhen);

	bool isRunning(){ return _state == State::Started; }
	bool isActiveRunning();

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
	inline bool isSouthEastMode(){ return _target == SortieTarget::SouthEast; }
	inline bool isLevelMode(){ return _target == SortieTarget::Level; }
	inline bool isExpeditionMode(){ return _target == SortieTarget::Expedition; }

	void setState(State state, const char* str);
	void setInactiveWaiting(bool waiting){ _inactiveWaiting = waiting; }

	void setStateStr(const QString& str);
	inline const QString& getStateStr(){ return _stateStr; }

	double getIntervalMul(){ return _intervalMul; }
	void setIntervalMul(double val)
	{
		_intervalMul = val; 
		if (_intervalMul <= 0.0f)
		{
			_intervalMul = 1.0f;
		}
	}

	inline void setLevelNoWait(bool bNoWait){ _levelNoWait = bNoWait; }
	inline bool getLevelNoWait(){ return _levelNoWait; }

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
	bool _inactiveWaiting = false;
	SortieTarget _target = SortieTarget::None;

	QString _stateStr;
	bool _pauseNext = false;

	QList<QList<int>> _ssShips;
	int _southEastTeamSize = 5;

	int _sortieMinCond = 30;
	int _sortieWaitCond = 40;

	bool _levelNoWait = false;

	StopWhen _stopwhen = StopWhen::None;

	double _intervalMul = 1.0f;
}; 

