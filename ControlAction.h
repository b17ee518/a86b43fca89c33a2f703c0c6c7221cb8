#pragma once
#include <QList>

class ControlAction
{
public:
	ControlAction(){};
	virtual ~ControlAction(){};

	virtual bool action(){ return true; }
};

class ChangeHenseiAction : public ControlAction
{
public:
	ChangeHenseiAction(const QList<int>){}
	ChangeHenseiAction(int shipid, int wasteShipid = -1){ Q_UNUSED(shipid); Q_UNUSED(wasteShipid); }

	virtual bool action() override;

public:
	QList<int> shipList;
};

class ChargeAction : public ControlAction
{
public:
	ChargeAction(){};	// only charge flagship of team1

	virtual bool action() override;
};

class DestroyShipAction : public ControlAction
{
public:
	DestroyShipAction(){};
	
	virtual bool action() override;
};

class SortieAction : public ControlAction
{
public:
	SortieAction(){};

	virtual bool action() override;
};

class SortieAdvanceAction : public ControlAction
{
public:
	SortieAdvanceAction(){};

	virtual bool action() override;
};

class BackFromSortieAction : public ControlAction
{
public:
	BackFromSortieAction(){};

	virtual bool action() override;
};
