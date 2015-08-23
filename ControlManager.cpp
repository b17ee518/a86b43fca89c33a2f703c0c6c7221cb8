#include "ControlManager.h"
#include <QTestEventList>
#include "mainwindow.h"
#include <QMouseEvent>
#include <QGraphicsSceneMouseEvent>

ControlManager::ControlManager()
{
}


ControlManager::~ControlManager()
{
}

void ControlManager::BuildNext_Kira()
{
	if (_todoShipids.empty())
	{
		return;
	}
	if (!_actionList.empty())
	{
		return;
	}
	if (isShipFull())
	{
		_actionList.append(DestroyShipAction());
	}
	_actionList.append(ChangeHenseiAction(_todoShipids.at(0), getOneWasteShipId()));
	_actionList.append(SortieAction());
	_actionList.append(SortieAdvanceAction());
	_actionList.append(BackFromSortieAction());
	_actionList.append(ChargeAction());

	state = State::Ready;
}

void ControlManager::BuildNext_Fuel()
{
	if (!_actionList.empty())
	{
		return;
	}

	if (isShipFull())
	{
		_actionList.append(DestroyShipAction());
	}
	_actionList.append(SortieAction());
	_actionList.append(SortieAdvanceAction());
	_actionList.append(BackFromSortieAction());
	_actionList.append(ChargeAction());
}

void ControlManager::LoadToDoShipList_Kira()
{
	_todoShipids.clear();

	_todoShipids.append(1); // test
	_todoShipids.append(2); // test
	_todoShipids.append(3); // test
}

void ControlManager::Run()
{
	if (state == State::Paused)
	{
		return;
	}

	if (_actionList.empty())
	{
		return;
	}
	if (!_actionList[0].action())
	{
		_actionList.removeAt(0);
		if (_actionList.empty())
		{
		}
	}
}

void ControlManager::Pause()
{
	state = State::Paused;
}

void ControlManager::Resume()
{
	state = State::Started;
}

void ControlManager::Terminate()
{
	state = State::Terminated;
}

int ControlManager::getOneWasteShipId()
{
	return -1;
}

bool ControlManager::isShipFull()
{
	return false;
}

bool ControlManager::shouldChangeSecondShip()
{
	return false;
}

int ControlManager::getCurrentShipId()
{
	return -1;
}

void ControlManager::moveMouseToAndClick(const QPoint& pt)
{
	QMouseEvent e(QEvent::MouseButtonPress, pt, Qt::LeftButton, Qt::LeftButton, Qt::NoModifier);
	QApplication::sendEvent(MainWindow::mainWindow()->getBrowserWidget(), &e);
	QMouseEvent e2(QEvent::MouseButtonRelease, pt, Qt::LeftButton, Qt::LeftButton, Qt::NoModifier);
	QApplication::sendEvent(MainWindow::mainWindow()->getBrowserWidget(), &e2);
	/*
	QTestEventList *events = new QTestEventList();
	events->addMouseMove(pt);
	events->simulate(MainWindow::mainWindow());
	*/
}


