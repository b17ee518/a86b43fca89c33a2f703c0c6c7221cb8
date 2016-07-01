﻿#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "kandataconnector.h"
#include <QMessageBox>

#include "ControlManager.h"
#include "ExpeditionManager.h"

#define TIMESHIFT_MIN	30

void MainWindow::onDoJobFuel()
{
	auto& cm = ControlManager::getInstance();
	cm.Terminate();

	if (!(QApplication::queryKeyboardModifiers()&Qt::ShiftModifier))
	{
		QMessageBox* pMessageBox = new QMessageBox(
			QMessageBox::NoIcon
			, QString::fromLocal8Bit("")
			, QString::fromLocal8Bit("終了条件")
			, QMessageBox::NoButton
			, this
			, Qt::Dialog | Qt::MSWindowsFixedSizeDialogHint | Qt::FramelessWindowHint);

		auto resetButton = pMessageBox->addButton(QString::fromLocal8Bit("クリア"), QMessageBox::ResetRole);
		auto yusouButton = pMessageBox->addButton(QString::fromLocal8Bit("輸送×3"), QMessageBox::AcceptRole);
		auto southEastButton = pMessageBox->addButton(QString::fromLocal8Bit("南西×5"), QMessageBox::ActionRole);
		auto foreverButton = pMessageBox->addButton(QString::fromLocal8Bit("永久"), QMessageBox::RejectRole);
		Q_UNUSED(foreverButton);

		pMessageBox->setDefaultButton(QMessageBox::NoButton);
//		pMessageBox->setAttribute(Qt::WA_DeleteOnClose, true);
		pMessageBox->exec();

		auto clickedButton = pMessageBox->clickedButton();
		if (clickedButton == resetButton)
		{
			KanSaveData::getInstance().resetTotals();
			KanDataConnector::getInstance().callUpdateOverviewTable();
			return;
		}
		else if (clickedButton == yusouButton)
		{
			cm.setStopWhen(ControlManager::StopWhen::Yusou3);
		}
		else if (clickedButton == southEastButton)
		{
			cm.setStopWhen(ControlManager::StopWhen::SouthEast5);
		}
		delete pMessageBox;
	}

	if (cm.isFlagshipOnly(0))
	{
		cm.BuildNext_Fuel();
	}
	else
	{
		cm.BuildNext_SouthEast();
	}

	cm.StartJob();
}

void MainWindow::onDoJobKira()
{
	auto& cm = ControlManager::getInstance();
	cm.Terminate();

	cm.LoadToDoShipList_Kira();

	if (QApplication::queryKeyboardModifiers()&Qt::ShiftModifier)
	{
		cm.BuildNext_Kira(true);
	}
	else
	{
		cm.BuildNext_Kira();
	}
	cm.StartJob();

}

void MainWindow::onDoJobLevel()
{
	auto& cm = ControlManager::getInstance();
	cm.Terminate();

	if (QApplication::queryKeyboardModifiers()&Qt::ShiftModifier)
	{
		cm.setLevelNoWait(false);
	}
	else
	{
		cm.setLevelNoWait(true);
	}
	if (cm.BuildNext_Level())
	{
		cm.StartJob();
	}
}

void MainWindow::onDoJobExpedition(bool bDo)
{
	auto& cm = ControlManager::getInstance();
	cm.Terminate();
	if (bDo)
	{
		if (cm.BuildNext_Expedition())
		{
			cm.StartJob();
		}
		else
		{
			return;
		}
	}
	ui->pbWaitExpedition->setChecked(bDo);
}

void MainWindow::onToggleJobExpedition()
{
	auto& cm = ControlManager::getInstance();
	if (cm.isTerminated())
	{
		onDoJobExpedition(true);
	}
	else
	{
		onDoJobExpedition(false);
	}
}

void MainWindow::onExportAllList()
{
	KanSaveData* pksd = &KanSaveData::getInstance();
	KanDataConnector * pkdc = &KanDataConnector::getInstance();
	struct ExportInfo
	{
		int shipno;
		int shiptype;
		int shipid;
		QString shipname;
		int lv;
		int cond;
	};
	QList<ExportInfo> infos;
	if (pksd->portdata.api_ship.size())
	{
		for (auto& ship:pksd->portdata.api_ship)
		{
			if (ship.api_lv < 10)
			{
				continue;
			}

			auto pmstship = pkdc->findMstShipFromShipid(ship.api_ship_id);
			if (pmstship)
			{
				ExportInfo info;
				info.shipno = ship.api_id;
				info.shiptype = pmstship->api_stype;
				info.shipid = ship.api_ship_id;
				info.lv = ship.api_lv;
				info.shipname = pmstship->api_name;
				info.cond = ship.api_cond;
				infos.append(info);
			}
		}
	}
	if (infos.size())
	{
		qSort(infos.begin(), infos.end(), [](const ExportInfo&left, const ExportInfo&right)
		{
			if (left.shiptype < right.shiptype)
			{
				return true;
			}
			else if (left.shiptype == right.shiptype)
			{
				/*
				if (left.cond < right.cond)
				{
					return true;
				}
				else if (left.cond == right.cond)
				{*/
					if (left.shipid < right.shipid)
					{
						return true;
					}
					else if (left.shipid == right.shipid)
					{
						if (left.shipno < right.shipno)
						{
							return true;
						}
					}
//				}
			}
			return false;
		});


		QFile * file = new QFile(QApplication::applicationDirPath() + "/action/" + "exported.table");
		if (file)
		{
			if (file->open(QIODevice::WriteOnly | QIODevice::Text))
			{
				int lastShipType = -1;
				for (auto& info:infos)
				{
					QString str;
					if (lastShipType >= 0 && lastShipType != info.shiptype)
					{
						str += "\n";
					}
					lastShipType = info.shiptype;
					str += "-\t" +
						QString::number(info.shipno) + "\t" + 
						info.shipname + "\t" + 
						QString::number(info.lv) + "\t" +
						QString::number(info.cond) + "\n";

					QTextStream outstream(file);
					outstream.setCodec("UTF-16");
					outstream.setGenerateByteOrderMark(true);
					outstream << str;
				}

				file->close();
			}


			delete file;
		}
	}
	// export master data
	KanSaveData::getInstance().logAllExport();
}

void MainWindow::onTerminateJob()
{
	ControlManager::getInstance().Terminate();
}

void MainWindow::slotSetLogAll(bool bSet)
{
	KanDataConnector::getInstance().setOutputAllLog(bSet);
}

void MainWindow::onJobPauseResume()
{
	if (ControlManager::getInstance().isPaused())
	{
		ControlManager::getInstance().Resume();
	}
	else
	{
		ControlManager::getInstance().Pause();
	}
}

void MainWindow::onJobPauseNext()
{
	if (ControlManager::getInstance().isPaused())
	{
		ControlManager::getInstance().Resume();
	}
	else
	{
		ControlManager::getInstance().togglePauseNext();
	}
	ui->pbPauseNext->setChecked(ControlManager::getInstance().getPauseNextVal());
}

void MainWindow::slotJobtTimeout()
{
	ControlManager::getInstance().Run();
}

void MainWindow::setPauseNextChanged(bool bVal)
{
	ui->pbPauseNext->setChecked(bVal);
}

void MainWindow::setJobTerminated()
{
	ui->pbPauseNext->setChecked(false);
	ui->pbWaitExpedition->setChecked(false);
}

void MainWindow::onIncreaseSouthEast()
{
	KanSaveData::getInstance().adjustSouthEast(1);
	KanDataConnector::getInstance().callUpdateOverviewTable();
}

void MainWindow::onDecreaseSouthEast()
{
	KanSaveData::getInstance().adjustSouthEast(-1);
	KanDataConnector::getInstance().callUpdateOverviewTable();
}

void MainWindow::onIncreaseTimeShift()
{
	setTimeShift(ExpeditionManager::getInstance().getTimeShiftMin() + TIMESHIFT_MIN);
}

void MainWindow::onDecreaseTimeShift()
{
	setTimeShift(ExpeditionManager::getInstance().getTimeShiftMin() - TIMESHIFT_MIN);
}
void MainWindow::setTimeShift(int min)
{
	ui->lbTimeShift->setText(QString::number(min));
	ExpeditionManager::getInstance().setTimeShiftMin(min);
}
