#pragma once
#include <QList>
#include <QTime>
#include <QMap>

class SingleExpedition
{
public:
	SingleExpedition(){};
	~SingleExpedition(){};

	void Clear();
	void addShip(int type, bool kira, int drum);
	void setInfo(const QTime& cost, int page, int index);
	void addSpecialExcludes(int shipid);

	QString checkMatches(int shipid, int index, int team, int& toShipid, 
		const QList<int>& shipList, const QList<int>& excludeShips);

	bool checkDaihatsu(int shipid, int shiptype);

	QList<int> shipTypes;	// minus for special ship
	QList<bool> kiraState;
	QList<int> drumCount;
	QList<int> specialExcludes;
	QTime costTime;
	int destPage = -1;
	int destIndex = -1;
};

class ExpeditionSchedule
{
public:
	ExpeditionSchedule(){};
	~ExpeditionSchedule(){};

	void Clear();
	void addExpedition(const QTime& endTime, const SingleExpedition& exp);
	// end time
	QMap<QTime, SingleExpedition> expeditions;
};

class ExpeditionManager
{
	enum class ExpeditionPreset
	{
		General,
		Fuel,
		RepairAndBauxite,
		Bauxite,
	};

public:
	static ExpeditionManager& getInstance() { static ExpeditionManager instance; return instance; }

private:
	ExpeditionManager();
	virtual ~ExpeditionManager(){}
	ExpeditionManager(ExpeditionManager const&);
	void operator=(ExpeditionManager const&);

public:

	void Clear();
	void BuildByPreset(const QString& preset);
	void BuildByPreset(ExpeditionPreset preset);

	bool ParsePresetBySettingFileAndRebuild();

	ExpeditionSchedule* getSchedule(int team);
	SingleExpedition* getShouldNextSchedule(int team, qint64 ct, qint64 bt);


	void BuildSingle20(ExpeditionSchedule* pschedule, int toHour, int toMin);
	void BuildSingle30(ExpeditionSchedule* pschedule, int toHour, int toMin);
	void BuildSingle40(ExpeditionSchedule* pschedule, int toHour, int toMin);

	void BuildSingleTokyu1(ExpeditionSchedule* pschedule, int toHour, int toMin);
	void BuildSingleTokyu2(ExpeditionSchedule* pschedule, int toHour, int toMin);

	void BuildSingle2last(ExpeditionSchedule* pschedule, int toHour, int toMin);
	void BuildSingle3last(ExpeditionSchedule* pschedule, int toHour, int toMin);
	void BuildSingle5last(ExpeditionSchedule* pschedule, int toHour, int toMin);

	void BuildSingleNezumiYusou(ExpeditionSchedule* pschedule, int toHour, int toMin);
	void BuildSingleBauxiteYusou(ExpeditionSchedule* pschedule, int toHour, int toMin);

	void BuildSingleNone(ExpeditionSchedule* pschedule, int toHour, int toMin);

	int getTimeShiftMin(){ return _timeShiftMin; }
	// should only be called by mainwindow!!
	void setTimeShiftMin(int min);

	const QString& getCurrentPreset() { return _currentPreset; }

private:
	void buildSingleByPresetLine(ExpeditionSchedule* pschedule, const QString& presetName, int hour, int minute);
	void dateChange();

	QList<ExpeditionSchedule> _schedules;	// each team

	QMap<QString, QList<ExpeditionSchedule> > _presetSchedules;
	QString _currentPreset;

	QString _expeditionDefineFileName;

	int _timeShiftMin = 0;
}; 


