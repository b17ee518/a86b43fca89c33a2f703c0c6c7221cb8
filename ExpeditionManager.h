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
		RepairAndBauxite,
		Bauxite,
	};

public:
	static ExpeditionManager& getInstance() { static ExpeditionManager instance; return instance; }

private:
	ExpeditionManager(){};
	virtual ~ExpeditionManager(){};
	ExpeditionManager(ExpeditionManager const&);
	void operator=(ExpeditionManager const&);

public:

	void Clear();
	void BuildByPreset(const QString& preset);
	void BuildByPreset(ExpeditionPreset preset);
	ExpeditionSchedule* getSchedule(int team);
	SingleExpedition* getShouldNextSchedule(int team, qint64 ct, qint64 bt);

	// TODO: check overnight / daihatsu

private:
	QList<ExpeditionSchedule> _schedules;	// each team

}; 


