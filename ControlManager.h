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

	enum class ActionTarget
	{
		None,
		Kira,
		Fuel,
		SouthEast,
		Expedition,
		Level,
		Rank,
		Any,
		Destroy,
		Develop,
		Repair,
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

	struct KiraSetting
	{
		bool forceCurrent = false;
	};
	struct SouthEastSetting
	{
		StopWhen stopWhen = StopWhen::Yusou3;
		bool is2_2 = false;
	};
	struct ExpeditionSetting
	{
	};
	struct LevelSetting
	{
	};
	struct RankSetting
	{
	};
	struct CellAnySetting
	{
		//		int cellId;
		int formation = 1;
		bool bCount = false;
		bool bReturnNext = false;
		bool bNeedS = false;
		bool bKillFlagship = false;
		bool bNeedWin = false;
		bool bTerminateNext = false;
		bool bAskForProceed = false;
	};
	struct AnySetting
	{
		int area = 1;
		int map = 5;
		int count = 1;
		bool checkAirBaseCond = false;
		bool checkCond = true;
		bool allowMiddleDamageSortie = false;
		bool pauseAtStartMap = false;
		int onlySSTeamSize = 0;
		QMap<int, CellAnySetting> cells;

		QList<float> areaCheckList;
		QList<float> mapClickPoint;	// click E1~E3 or Ex button
		QList<float> mapExCheckList;	// for Ex only
		QList<float> mapExClickPoint;	// click after Ex
		QList<float> mapEx2CheckList;	// for Ex only
		QList<float> mapEx2ClickPoint;	// click after Ex
	};

	struct DevelopSetting
	{
		QMap<int, int> toDevelopSlotItemList;
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

	bool LoadDestroyableList();
	QList<int> GenerateToDestroyList(QList<int>& wasteShipList);

	void pushPreSupplyCheck();

	void pushPreRepairCheck(bool bCanUseFastRepair, bool includingFirstTeam, bool onlyShortSevere);

	QList<int> pushPreShipFullCheck();

	bool BuildNext_Kira();

	bool BuildNext_Fuel();

	bool BuildNext_East();

	bool BuildNext_Level();

	bool BuildNext_Rank();

	void LoadAnyTemplateSettings();
	bool BuildNext_Any();

	bool LoadExpeditionPossibleList(); // TODO
	bool BuildNext_Expedition();

	bool BuildNext_Destroy();

	bool BuildNext_Repair();

	bool BuildNext_Develop();

	bool stopWhenCheck();

	void StartJob();

	void Run();
	void Pause();
	void Resume();
	void Terminate(bool bSilent = false);

	bool isPaused();
	void PauseNext();
	void togglePauseNext();

	bool checkShouldAutoWait();
	void setupAutoExpedition();

	bool isTerminated(){ return _state == State::Terminated; }

	void setToTerminate(const char* title, bool forceSound = false);

	void createSSShipList();
	bool chooseSSShipList(int teamSize, QList<int>& ships, QList<int>&sortInTeamShips, QString& errorMessage, bool onlySensui = false);
	bool isTreatedSameShip(int shipno, int oshipno);
	bool isAfterShip(const kcsapi_mst_ship* pmstship, const kcsapi_mst_ship* pomstship, QList<int> checkedIdList = QList<int>()/*for convert*/);
	// is pmstship after of pomstship

	int getCurrentFlagshipId();
	int getCurrentSecondshipId();
	bool shouldChangeSecondShip();
	bool needChargeFlagship(int team);
	bool needChargeAnyShip(int team);
	int getOneWasteShipId(int exclude1 = -1, int exclude2 = -1, QList<int>shipList = QList<int>());
	int getOneWasteShipId(const QList<int>& excludes, QList<int>shipList = QList<int>());
	bool isShipFull(int keep = 3, bool checkSlotitem = true);
	bool isSlotItemFull(int keep = 6);
	bool findPagePosByShipId(int shipno, int& page, int& pos, int& lastPage);
	bool isShipKiraDone(int shipno);
	bool isLowCond(int shipno);
	bool isShipInOtherTeam(int shipno, int team, bool excludeOnBoardingExpedition = false);
	bool isShipInTeam(int shipno, int team);
	bool isShipInDock(int shipno);
	bool isShipDamaged(int shipno);
	bool isShipCharged(int shipno);
	bool needChargeCondAirBase(bool checkCond);
	bool isShipExist(int shipno);

	int getShipLevel(int shipno);

	int getTotalSlotItemCountForID(int slotitemId);

	bool isHenseiDone(const QList<int>& ships, int team, int index = -1);
	bool isFlagshipOnly(int team);
	int isAllSSShips(int team);
	int getTeamSize(int team);
	bool isShipType(int shipno, ShipType stype);
	bool hasSlotitem(int shipno, SlotitemType sitype, int count = 1);
	bool noSlotitem(int shipno);
	bool noAttackItem(int shipno);

	int getShipCondVal(int shipno);

	bool flagshipSevereDamaged();

	bool shouldNightBattle();
	bool shouldRetrieve();
	bool shouldRetrieveForAny();
	bool shouldAskForProceedForAny();
	bool shouldTerminateForAny();
	WoundState hugestDamageInTeam(int team);

	void setSouthEastSetting(const SouthEastSetting& southEastSetting);
	void setKiraSetting(const KiraSetting& kiraSetting);
	void setAnySetting(const AnySetting& anySetting);
	void setDevelopSetting(const DevelopSetting& developSetting);

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

	inline bool isKiraMode(){ return _target == ActionTarget::Kira; }
	inline bool isFuelMode(){ return _target == ActionTarget::Fuel; }
	inline bool isSouthEastMode(){ return _target == ActionTarget::SouthEast; }
	inline bool isLevelMode(){ return _target == ActionTarget::Level; }
	inline bool isExpeditionMode(){ return _target == ActionTarget::Expedition; }
	inline bool isRankMode(){ return _target == ActionTarget::Rank; }
	inline bool isAnyMode(){ return _target == ActionTarget::Any; }
	inline bool isDestroyMode(){ return _target == ActionTarget::Destroy; }
	inline bool isRepairMode(){ return _target == ActionTarget::Repair; }
	inline bool isDevelopMode(){ return _target == ActionTarget::Develop; }

	void setState(State state, const char* str, bool bSilent = false, bool forceSound = false);
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

	inline void setShouldAutoExpedition(bool bAuto){ _shouldAutoSwitchToExpeditionFlag = bAuto; }
	inline bool shouldAutoExpedition(){ return _shouldAutoSwitchToExpeditionFlag; }

	inline bool isAutoExpeditioning(){ return _autoExpeditioningFlag; }

	void switchBackToLastAction();
	void clearLastTarget();

	const KiraSetting& getKiraSetting(){ return _kiraSetting; }
	const SouthEastSetting& getSouthEastSetting(){ return _southEastSetting; }
	const LevelSetting& getLevelSetting(){ return _levelSetting; }
	const ExpeditionSetting& getExpeditionSetting(){ return _expeditionSetting; }
	const RankSetting& getRankSetting() { return _rankSetting; }
	const AnySetting& getAnySetting() { return _anySetting; }
	const DevelopSetting& getDevelopSetting() { return _developSetting; }
	AnySetting getAnyTemplateSetting(int area, int map);

	//private:
	void moveMouseToAndClick(float x, float y, float offsetX = 5, float offsetY = 3);
	void moveMouseTo(float x, float y, float offsetX = 5, float offsetY = 3);

	void setPauseNextVal(bool bVal);
	inline bool getPauseNextVal(){ return _pauseNext; }

	void waitForResponse(const QString& api);

	qreal randVal(qreal min, qreal max);

	bool isPortDataDirty(){ return _isPortDataDirty; }
	void setPortDataDirty();
	void clearPortDataDirtyFlag();

private:

	bool _isPortDataDirty = true;

	QString _lastTerminationReason;

	QList<int> _destroyableMstIds;

	QList<int> _todoShipids;
	int _toCond = 70;
	QList<ControlAction*> _actionList;

	State _state = State::None;
	bool _inactiveWaiting = false;
	ActionTarget _target = ActionTarget::None;
	ActionTarget _lastTarget = ActionTarget::None;
	bool _autoExpeditioningFlag = false;

	QString _stateStr;
	bool _pauseNext = false;

	QList<QList<int> > _ssShips;

	int _sortieMinCond = 30;
	int _sortieWaitCond = 40;

	bool _levelNoWait = false;

	double _intervalMul = 1.0f;

	bool _shouldAutoSwitchToExpeditionFlag = true;
	KiraSetting _kiraSetting;
	SouthEastSetting _southEastSetting;
	LevelSetting _levelSetting;
	ExpeditionSetting _expeditionSetting;
	RankSetting _rankSetting;
	AnySetting _anySetting;
	DevelopSetting _developSetting;

	QMap<QPair<int, int>, AnySetting> _anyTemplateSettings;
};

